#include <iostream>
#include "VirtFlow_types.h"
#include "VirtFlow_sched.h"
#include "fcfs_sched.h"
#include "rr_sched.h"
#include "sjf_sched.h"
#include "nimblock_sched.h"
#include "event_gen.h"
#include "debug.h"
#include <fstream>


int main(int argc, char **argv){

	#ifdef SINGLE

    if(argc < 2){
    	std::cout << "[INFO]: ****** Set app $1 and batch_size $2 ******" << std::endl;
        return EXIT_FAILURE;
    }

    const uint32_t MAX_APPS = 1;
    const double interval = 0.05f;

    #elif defined(QUERY)


    if(argc < 2){
    	std::cout << "[INFO]: ****** Set query $1 and interval $2 ******" << std::endl;
        return EXIT_FAILURE;
    }

    const uint32_t MAX_APPS = 20;
    const double interval = atof(argv[2]);

    #endif

	std::cout << "[INFO]: ****** Launching Baseline ******" << std::endl;
	std::cout << std::endl;
	std::cout << "[INFO]: ****** Initializing Partial Reconfigurable Regions ******" << std::endl;
	std::vector<PRRegion*> active_regions;
	assert(init_regions(active_regions));
	std::cout << "[INFO]: ****** All Partial Reconfigurable Regions are successfully initialized ******" << std::endl;
	std::cout << std::endl;

	std::list<Application*> pending_apps, waiting_apps, completed_apps, total_apps;
	std::list<Task*> waiting_tasks;

	PRRegion* sel_prr = NULL;
	Task* sel_task = NULL;
	uint32_t icap = 0;

	#ifdef NONSHARING
	std::list<Task*> baseline_tasks;
	std::list<Application*> baseline_app;
	#endif

	int i=0;

    OpticalFlow_input();
    Rendering_input();
    Compression_input();
    Lenet_input();
    DigitRec_input();
    Scaling_input();

    #ifdef NIMBLOCK
    double threshold = 1.0;
	bool rethresh = false;
	bool ret  = false;
	// Initialize candidate app list to empty
	std::list<Application*> candidate_apps;
    #endif

	
	std::chrono::high_resolution_clock::time_point point1 = timestamp();
	std::chrono::high_resolution_clock::time_point prev_new_app = timestamp();

	while(completed_apps.size()<MAX_APPS){

		update_prr_state(active_regions);

		#ifdef SINGLE
		prev_new_app = generate_event(prev_new_app, pending_apps, total_apps, atoi(argv[1]), atoi(argv[2]), MAX_APPS, interval);
		#elif defined(QUERY)
		prev_new_app = generate_event(prev_new_app, pending_apps, total_apps, atoi(argv[1]), MAX_APPS, interval);
		#endif

		#if defined(FCFS)

		is_completed(waiting_apps, completed_apps);
		fcfs_schedule(pending_apps, waiting_apps, waiting_tasks);
		if(icap){
			sel_prr = select_prr_fcfs(active_regions);
			sel_task = select_task_fcfs(waiting_tasks, sel_prr);
		}
		launch_fcfs(active_regions, sel_prr);
		icap = pr_task(sel_prr, sel_task);

		#elif defined (RR)

		is_completed(waiting_apps, completed_apps);
		rr_schedule(active_regions, pending_apps, waiting_apps);
		if(icap){
			sel_prr = select_prr_rr(active_regions);
			sel_task = select_task_rr(sel_prr);
		}
		launch_rr(active_regions, sel_prr);
		icap = pr_task(sel_prr, sel_task);

		#elif defined(SJF)

		is_completed(waiting_apps, completed_apps);
		sjf_schedule(pending_apps, waiting_apps, waiting_tasks);
		if(icap){
			sel_prr = select_prr_sjf(active_regions);
			sel_task = select_task_sjf(waiting_tasks, sel_prr);
		}
		launch_sjf(active_regions, sel_prr);
		icap= pr_task(sel_prr, sel_task);

		#elif defined (NONSHARING)

		is_completed(baseline_app, completed_apps);
		if(icap){
			sel_prr = select_prr_fcfs(active_regions);
			sel_task = select_task_fcfs(baseline_tasks, sel_prr);
		}
		launch_fcfs(active_regions, sel_prr);
		icap = pr_task(sel_prr, sel_task);
		nonshared_scheduling(pending_apps, baseline_app, baseline_tasks);

		#elif defined (NIMBLOCK)

		// Update priorities b/c scheduling
		threshold = update_priorities_app(pending_apps, candidate_apps);
		allocate_resources(candidate_apps);

		//edw exei ena event delay
		rethresh = false;
		ret = false;

		rethresh = update_candidate_apps(pending_apps, candidate_apps, threshold);
		if (rethresh) {
			allocate_resources(candidate_apps);
		}

		ret = is_completed(candidate_apps, completed_apps);

		ret = ret || is_completed(pending_apps, completed_apps);
		if (ret) {
			// change thresholds because we have more slots available
			allocate_resources(candidate_apps);
		}

		sel_prr = NULL;
		sel_task = NULL;
		if(icap){
			sel_prr = select_prr_nimblock(active_regions);
			sel_task = select_task_nimblock(candidate_apps);
		}

		//PREEMPTION
		if (sel_prr == NULL && sel_task != NULL){
			
			PRRegion* preempt_tile = preempt_slot_apps(active_regions, sel_task);
			
			if (preempt_tile != NULL){

				// Add to pending_tasks queue to detect a dead task.
				// By moving preempted tasks to pending tasks, successors that have more tokens
				// than predecessors won't be ready to execute and we won't deadlock
				preempt_tile->curr_task->app_id->occupancy -= 1;
				preempt_tile->status = PR_IDLE;
				sel_prr = preempt_tile;
			}
		}

		launch_nimblock(active_regions, sel_prr);	
		icap = pr_task(sel_prr, sel_task);	

		#endif
	}

	std::chrono::high_resolution_clock::time_point point2 = timestamp();

	#ifdef SINGLE
    export_metrics(completed_apps, duration(point1, point2));
    #elif defined (QUERY)
    export_metrics(completed_apps, duration(point1, point2), atoi(argv[1]), interval);
    #endif

	std::cout << std::endl;
	std::cout <<"[INFO]: ****** Baseline has finished ******" << std::endl;
	std::cout <<"[INFO]: Length of completed apps --> " << completed_apps.size() << std::endl;
	std::cout << std::endl;


	return EXIT_SUCCESS;
}


