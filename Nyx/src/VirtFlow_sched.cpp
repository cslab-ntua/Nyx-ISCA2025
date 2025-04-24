#include "VirtFlow_sched.h"

/*
Check whether the task within the prr has ended its execution.
*/

#ifndef SUBOPT_BASELINE

void update_prr_state(std::vector<PRRegion*> &regions){

	for(PRRegion* prr : regions){
		if(prr->status == PR_RUNNING){
			if(hls_ip_isIdle(prr)){
				Task* curr_task = prr->curr_task;
                curr_task->compl_batch +=1;
				curr_task->end_time = timestamp();
				#ifdef SJF
				prr->prr_est_latency -=curr_task->latency;
				#endif
				prr->status=PR_IDLE;
				curr_task->app_id->tasks_completed +=1;
				curr_task->state = TASK_ENDED;
				prr->curr_task=NULL;
				#ifdef DEBUG
				std::cout << "[DEBUG]: State of PR region " << prr->id <<" is updated to IDLE" << std::endl;
				std::cout << std::endl;
				#endif				
			}
		}
	}
}

#else

void update_prr_state(std::vector<PRRegion*> &regions){

	for(PRRegion* prr : regions){
		if(prr->status == PR_RUNNING){
			if(hls_ip_isIdle(prr)){
				Task* curr_task = prr->curr_task;
				curr_task->compl_batch +=1;

				if(curr_task->compl_batch == curr_task->max_batch){
					curr_task->end_time = timestamp();
					prr->status = PR_IDLE;

					curr_task->app_id->tasks_completed +=1;
					curr_task->state = TASK_ENDED;
					prr->curr_task=NULL;
					#ifdef DEBUG
					std::cout << "[DEBUG]: State of PR region " << prr->id <<" is updated to IDLE" << std::endl;
					std::cout << std::endl;
					#endif

				}
				else{
					prr->status = PR_WAITING;
					curr_task->state = TASK_WAITING;				
				}

			}

		}
	}

}

#endif

/*
Check whether the app has been completed
*/

bool is_completed(std::list<Application*> &waiting_apps, std::list<Application*> &completed_apps){

	bool ret_val = false;

	for(auto it = waiting_apps.begin(); it != waiting_apps.end();){
		Application* curr_app = *it;
		if(curr_app->tasks_completed == curr_app->tasks.size()){
			it = waiting_apps.erase(it);
            curr_app->end_time = timestamp();
			completed_apps.push_back(curr_app);
			std::cout << "[DEBUG]: " << curr_app->app_name <<" is moved to COMPLETED and SIZE is " << completed_apps.size() << std::endl;
			std::cout << std::endl;
			ret_val = true;
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

uint64_t prr_vfifo_ctrl_addr[RECONFIG_REGIONS]={
		RECONFIG_REGION_0_VFIFO_CTRL_ADDR,
		RECONFIG_REGION_1_VFIFO_CTRL_ADDR,
		RECONFIG_REGION_2_VFIFO_CTRL_ADDR,
		RECONFIG_REGION_3_VFIFO_CTRL_ADDR,
		RECONFIG_REGION_4_VFIFO_CTRL_ADDR,
		RECONFIG_REGION_5_VFIFO_CTRL_ADDR,
		RECONFIG_REGION_6_VFIFO_CTRL_ADDR,
		RECONFIG_REGION_7_VFIFO_CTRL_ADDR,
};

uint32_t prr_src_addr[RECONFIG_REGIONS]={
		RECONFIG_REGION_0_SRC,
		RECONFIG_REGION_1_SRC,		
		RECONFIG_REGION_2_SRC,
		RECONFIG_REGION_3_SRC,
		RECONFIG_REGION_4_SRC,
		RECONFIG_REGION_5_SRC,
		RECONFIG_REGION_6_SRC,
		RECONFIG_REGION_7_SRC,
};

#define STREAMS 2

uint32_t prr_dest_addr[RECONFIG_REGIONS][STREAMS] = {
	{RECONFIG_REGION_0_DEST_0, RECONFIG_REGION_0_DEST_1},
	{RECONFIG_REGION_1_DEST_0, RECONFIG_REGION_1_DEST_1},
	{RECONFIG_REGION_2_DEST_0, RECONFIG_REGION_2_DEST_1},
	{RECONFIG_REGION_3_DEST_0, RECONFIG_REGION_3_DEST_1},
	{RECONFIG_REGION_4_DEST_0, RECONFIG_REGION_4_DEST_1},
	{RECONFIG_REGION_5_DEST_0, RECONFIG_REGION_5_DEST_1},
	{RECONFIG_REGION_6_DEST_0, RECONFIG_REGION_6_DEST_1},
	{RECONFIG_REGION_7_DEST_0, RECONFIG_REGION_7_DEST_1},
};

int init_regions(std::vector<PRRegion*> &regions){

	for(int i=0; i<RECONFIG_REGIONS; i++){
		PRRegion* curr_region = new PRRegion(i, PR_IDLE, prr_vfifo_ctrl_addr[i], prr_decouple_addr[i], prr_axilite_addr[i]);
		curr_region->src_addr = prr_src_addr[i];
		for(int dest=0; dest<STREAMS; dest++){
			curr_region->dest_addr[dest] = prr_dest_addr[i][dest];
		}
		regions.push_back(curr_region);
        hls_ip_reset(curr_region);
	}
	return SUCCESS;
}
