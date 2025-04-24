#include "fcfs_sched.h"

#if defined(FCFS) || defined(NONSHARING)

/* 
First Come First Served (FCFS) Approach
Also used for Non-Sharing execution
*/
PRRegion* select_prr_fcfs(std::vector<PRRegion*> &regions){

	for(PRRegion* prr : regions){
		if(prr->status==PR_IDLE){
			#ifdef DEBUG
			//std::cout << "[DEBUG]: PR region " << prr->id <<" is selected using FCFS" << std::endl;
			#endif
			return prr;
		}
	}
	#ifdef DEBUG
	//std::cout << "[DEBUG]: No PR regions are available" << std::endl;
	//std::cout << std::endl;
	#endif
	return NULL;
}

Task* select_task_fcfs(std::list<Task*> &ready_tasks, PRRegion *curr_region){
	if(ready_tasks.empty()==true || curr_region==NULL){
		#ifdef DEBUG
		//std::cout << "[DEBUG]: No tasks are available for execution" << std::endl;
		//std::cout << std::endl;
		#endif
		return NULL;
	}
	else{
		Task *ret_task = ready_tasks.front();
		ready_tasks.pop_front();

		Application *curr_app = ret_task->app_id;
		int curr_dep = curr_app->num_deps[ret_task->id];
		ret_task->src = curr_region->src_addr;

		for(int iter=0; iter<curr_dep; iter++){

			if(curr_app->id == 1){
				int tmp_ = curr_app->tasks[curr_app->dep_list[ret_task->id][iter]]->task_rdy;
				curr_app->tasks[curr_app->dep_list[ret_task->id][iter]]->dest[iter+tmp_] = curr_region->dest_addr[iter+tmp_];
				curr_app->tasks[curr_app->dep_list[ret_task->id][iter]]->task_rdy +=1;
			}
			else{
				int tmp_ = curr_app->tasks[curr_app->dep_list[ret_task->id][iter]]->task_rdy;
				curr_app->tasks[curr_app->dep_list[ret_task->id][iter]]->dest[tmp_] = curr_region->dest_addr[tmp_];
				curr_app->tasks[curr_app->dep_list[ret_task->id][iter]]->task_rdy +=1;
			}

		}

		#ifdef DEBUG
		std::cout << "[DEBUG]: Task " << ret_task->name << " is selected from " << ret_task->app_id->app_name << std::endl;
		#endif
		return ret_task;
	}
}

void fcfs_schedule(std::list<Application*> &pending_apps, std::list<Application*> &waiting_apps,std::list<Task*> &waiting_tasks){

	for(auto it = pending_apps.begin(); it != pending_apps.end();){
		Application* curr_app = *it;
		it = pending_apps.erase(it);
		waiting_apps.push_back(curr_app);
		curr_app->schedule_time = timestamp();
		for(int iter=0; iter<curr_app->tasks.size(); iter++){
			//curr_app->tasks.at(iter)->schedule_time = timestamp();
			curr_app->tasks.at(iter)->state = TASK_SCHEDULED;
			waiting_tasks.push_back(curr_app->tasks.at(iter));
		}
	}

}

void launch_fcfs(std::vector<PRRegion*> &regions, PRRegion* sel_prr){

	int sel = -1;
	if (sel_prr!=NULL)
		sel = sel_prr->id;

	for(PRRegion* prr : regions){
		if(prr->status == PR_CONFIG && prr->id!=sel){
			Task* curr_task = prr->curr_task;
			Application* curr_app = curr_task->app_id;
			int curr_dep_rev = curr_app->num_deps_reverse[curr_task->id];
			if(curr_dep_rev==curr_task->task_rdy){
				launch_task(prr);
			}
		}
	}
}


/* 
Non-Sharing Approach 
*/
void nonshare_scheduling(std::list<Application*> &pending_apps, std::list<Application*> &waiting_app, std::list<Task*> &waiting_tasks){

	if(!pending_apps.empty() && waiting_app.size()==0){
		Application* curr_app = pending_apps.front();
		pending_apps.pop_front();
		waiting_app.push_back(curr_app);

		#ifdef DEBUG
		std::cout << "[DEBUG]: Application " << curr_app->id <<" is selected using Baseline" << std::endl;
		#endif
		curr_app->schedule_time = timestamp();
		for(int iter=0; iter<curr_app->tasks.size(); iter++){
			curr_app->tasks.at(iter)->state = TASK_SCHEDULED;
			//curr_app->tasks.at(iter)->schedule_time = timestamp();
			waiting_tasks.push_back(curr_app->tasks.at(iter));
		}
	}
}

#endif
