#include "VirtFlow_sched.h"

/*
Check whether the task within the prr has ended its execution.
*/
void update_prr_state(std::vector<PRRegion*> &regions){

	for(PRRegion* prr : regions){
		if(prr->status == PR_RUNNING){
			if(hls_ip_isIdle(prr)){

				Task* curr_task = prr->curr_task;
				curr_task->compl_batch +=1;
				#ifdef SJF
				prr->prr_est_latency -=curr_task->latency;
				#endif
				if(curr_task->compl_batch == curr_task->max_batch){
					curr_task->end_time = timestamp();
					prr->status=PR_IDLE;
					curr_task->app_id->tasks_completed +=1;
					curr_task->state = TASK_ENDED;
					prr->curr_task=NULL;
					#ifdef DEBUG
					std::cout << "[DEBUG]: State of PR region " << prr->id <<" is updated to IDLE" << std::endl;
					//std::cout << std::endl;
					#endif	
				}
				else{
					prr->status=PR_WAITING;
					curr_task->state = TASK_WAITING;
					curr_task->start_wait = timestamp();
					#ifdef DEBUG
					//std::cout << "[DEBUG]: State of PR region " << prr->id <<" is updated to WAITING" << std::endl;
					//std::cout << std::endl;
					#endif
				}
			
			}
		}
	}
}

/*
Check whether the app has been completed
*/

bool is_completed(std::list<Application*> &waiting_apps, std::list<Application*> &completed_apps){

	for(auto it = waiting_apps.begin(); it != waiting_apps.end();){
		Application* curr_app = *it;
		if(curr_app->tasks_completed == curr_app->tasks.size()){
			it = waiting_apps.erase(it);
			curr_app->end_time = timestamp();
			completed_apps.push_back(curr_app);
			std::cout << "[DEBUG]: " << curr_app->app_name <<" is moved to COMPLETED and SIZE is " << completed_apps.size() << std::endl;
			std::cout << std::endl;
		}
		else{
			it++;
		}
	}

	return true;
}


uint64_t prr_axilite_addr[RECONFIG_REGIONS]={
		RECONFIG_REGION_0_AXILITE_ADDR,
		RECONFIG_REGION_1_AXILITE_ADDR,
		RECONFIG_REGION_2_AXILITE_ADDR,
		RECONFIG_REGION_3_AXILITE_ADDR,
		RECONFIG_REGION_4_AXILITE_ADDR,
		RECONFIG_REGION_5_AXILITE_ADDR,
		RECONFIG_REGION_6_AXILITE_ADDR,
		RECONFIG_REGION_7_AXILITE_ADDR,
};

uint64_t prr_decouple_addr[RECONFIG_REGIONS]={
		RECONFIG_REGION_0_DECOUPLE_ADDR,
		RECONFIG_REGION_1_DECOUPLE_ADDR,
		RECONFIG_REGION_2_DECOUPLE_ADDR,
		RECONFIG_REGION_3_DECOUPLE_ADDR,
		RECONFIG_REGION_4_DECOUPLE_ADDR,
		RECONFIG_REGION_5_DECOUPLE_ADDR,
		RECONFIG_REGION_6_DECOUPLE_ADDR,
		RECONFIG_REGION_7_DECOUPLE_ADDR,
};

int init_regions(std::vector<PRRegion*> &regions){

	for(int i=0; i<RECONFIG_REGIONS; i++){
		PRRegion* curr_region = new PRRegion(i, PR_IDLE, prr_decouple_addr[i], prr_axilite_addr[i]);
		regions.push_back(curr_region);
		hls_ip_reset(curr_region);
	}
	return SUCCESS;
}
