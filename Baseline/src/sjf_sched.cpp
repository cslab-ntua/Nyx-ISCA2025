#include "sjf_sched.h"

#if defined(SJF)

/*
Shortest Job First (SJF) Approach
*/


std::list<Application*>::iterator next_app_sjf(std::list<Application*> &pending_apps){

	std::list<Application*>::iterator app_iter;
    double smallest_latency = INT_MAX;

    for(auto it = pending_apps.begin(); it!=pending_apps.end(); it++){

    	Application* curr_app = *it;
    	if(smallest_latency > curr_app->exec_time){
        	app_iter = it;
        	smallest_latency = curr_app->exec_time;
            }
	}
	
	return app_iter;
}


void sjf_schedule(std::list<Application*> &pending_apps, std::list<Application*> &waiting_apps, std::list<Task*> &waiting_tasks){

	while(!pending_apps.empty() and waiting_tasks.empty()){

		std::list<Application*>::iterator sel_it = next_app_sjf(pending_apps);
		Application* curr_app = *sel_it;
		sel_it = pending_apps.erase(sel_it);
		waiting_apps.push_back(curr_app);
		curr_app->schedule_time = timestamp();

		#ifdef DEBUG
	    std::cout << "[DEBUG]: App " << curr_app->id << " is selected using SJF " << std::endl;
	    #endif


		for(int iter=0; iter<curr_app->tasks.size(); iter++){

            curr_app->tasks.at(iter)->state = TASK_SCHEDULED;
            waiting_tasks.push_back(curr_app->tasks.at(iter));

		}
	}
}

PRRegion* select_prr_sjf(std::vector<PRRegion*> &regions){

	for(PRRegion* prr : regions){
		if(prr->status==PR_IDLE){
			#ifdef DEBUG
			//std::cout << "[DEBUG]: PR region " << prr->id <<" is selected using SJF" << std::endl;
			#endif
			return prr;
		}
	}
	#ifdef DEBUG
	//std::cout << "[DEBUG]: No PR regions are available" << std::endl;
	#endif
	return NULL;

}

Task* select_task_sjf(std::list<Task*> &waiting_tasks, PRRegion* sel_prr){

	if(waiting_tasks.empty()==true || sel_prr==NULL){
		#ifdef DEBUG
		//std::cout << "[DEBUG]: No tasks are available for execution" << std::endl;
		#endif
		return NULL;
	}
	else{

		Task *ret_task = waiting_tasks.front();
		waiting_tasks.pop_front();

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
}


void launch_sjf(std::vector<PRRegion*> &regions, PRRegion* sel_prr){

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