#include "rr_sched.h"

#if defined(RR)

/*
Round Robin (RR) Approach
*/

uint32_t sel_worker = 0;

PRRegion* select_prr_rr(std::vector<PRRegion*> &regions){

	uint32_t curr_worker = sel_worker % RECONFIG_REGIONS;
    PRRegion* prr_rr = regions.at(curr_worker);

    if(prr_rr->status == PR_IDLE){

    	//sel_worker += 1;
        #ifdef DEBUG
        //std::cout << "[DEBUG]: PR region " << prr_rr->id <<" is selected using RR" << std::endl;
        #endif
        return prr_rr;

	}

    #ifdef DEBUG
    //std::cout << "[DEBUG]: No PR regions are available" << std::endl;
    #endif
    return NULL;
}


Task* select_task_rr(PRRegion* sel_prr){

    if(sel_prr==NULL or sel_prr->task_rr.empty()){

    	#ifdef DEBUG
        //std::cout << "[DEBUG]: No Tasks are available" << std::endl;
        #endif
        return NULL;
    }
    sel_worker += 1;

    Task* ret_task = sel_prr->task_rr.front();
    sel_prr->task_rr.pop_front();

	Application *curr_app = ret_task->app_id;
	int curr_dep = curr_app->num_deps[ret_task->id];

	for(int iter=0; iter<curr_dep; iter++){
		curr_app->tasks[curr_app->dep_list[ret_task->id][iter]]->task_rdy +=1;
	}


	#ifdef DEBUG
	std::cout << "[DEBUG]: Task " << ret_task->name << " is selected from " << ret_task->app_id->app_name << std::endl;
	#endif
	return ret_task;
}


uint32_t worker_holder = 0;

PRRegion* next_prr_rr(std::vector<PRRegion*> &regions){

	uint32_t curr_worker = worker_holder % RECONFIG_REGIONS;
	PRRegion* prr_rr = regions.at(curr_worker);
	worker_holder +=1;

	return prr_rr;
}

void rr_schedule(std::vector<PRRegion*> &regions, std::list<Application*> &pending_apps, std::list<Application*> &waiting_apps){

	for(auto it = pending_apps.begin(); it!=pending_apps.end();){

		Application* curr_app = *it;
		it = pending_apps.erase(it);
		waiting_apps.push_back(curr_app);
		curr_app->schedule_time = timestamp();

		for(int iter=0; iter<curr_app->tasks.size(); iter++){
			
			PRRegion* curr_region = next_prr_rr(regions);

			#ifdef DEBUG
            //std::cout << "[DEBUG]: Task " << curr_app->tasks.at(iter)->id << " from App " << curr_app->id << " is scheduled on " << curr_region->id << std::endl;
            #endif

			curr_app->tasks.at(iter)->state = TASK_SCHEDULED;
			curr_region->task_rr.push_back(curr_app->tasks.at(iter));
		}
	}
}

void launch_rr(std::vector<PRRegion*> &regions, PRRegion* sel_prr){

	int sel = -1;
	if (sel_prr!=NULL)
		sel = sel_prr->id;

	for(PRRegion* prr : regions){
		if((prr->status == PR_CONFIG || prr->status == PR_WAITING) && prr->id!=sel){
			Task* curr_task = prr->curr_task;
			Application* curr_app = curr_task->app_id;
			int curr_dep_rev = curr_app->num_deps_reverse[curr_task->id];
			int curr_dep = curr_app->num_deps[curr_task->id];
			if(curr_dep_rev==curr_task->task_rdy){
				int flag=0;
				for(uint32_t iter=0; iter<curr_dep; iter++){
					if(curr_task->compl_batch < curr_app->tasks[curr_app->dep_list[curr_task->id][iter]]->compl_batch)
						flag++;
				}
				if(flag == curr_dep)
					launch_task(prr);
			}
		}
	}
}

#endif