#include "hls_ip.h"

uint8_t bitExtracted(uint32_t number, int k, int p){
    return (((1 << k) - 1) & (number >> (p - 1)));
}

void hls_ip_start(PRRegion *RegionInstance){

	assert(RegionInstance!=NULL);
	assert(RegionInstance->status==PR_CONFIG || RegionInstance->status==PR_WAITING);
	uint32_t data = 0xFFFFFFFF;
	uint32_t mask = 0x00000001;
	uint32_t val = data & mask;

	#ifdef DEBUG
	//std::cout <<"[DEBUG]: Start Task\tAXILite Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_AP_CTRL << "\tReg --> " << val << std::endl;
	#endif

	write_fpga(&val, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_AP_CTRL);

}

uint32_t hls_ip_isDone(PRRegion *RegionInstance){

	assert(RegionInstance!=NULL);
	uint32_t data = 0xFFFFFFFF;

	read_fpga(&data, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_AP_CTRL);

	uint32_t mask = 0x00000002;
	uint32_t val = data & mask;

	#ifdef DEBUG
	//std::cout <<"[DEBUG]: Done Task\tAXILite Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_AP_CTRL << "\tReg --> " << val << std::endl;
	#endif

	return val;
}

uint32_t hls_ip_isIdle(PRRegion *RegionInstance){

	assert(RegionInstance!=NULL);
   	uint32_t data = 0xFFFFFFFF;

   	read_fpga(&data, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_AP_CTRL);

   	uint32_t mask_idle = 0x00000004;
   	uint32_t val_idle = data & mask_idle;

   	uint32_t mask_start = 0x00000001;
  	uint32_t val_start = data & mask_start;

   	#ifdef DEBUG
   	//std::cout <<"[DEBUG]: Start Task\tAXILite Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_AP_CTRL << "\tReg --> " << val_start << std::endl;
   	//std::cout <<"[DEBUG]: Idle Task\tAXILite Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_AP_CTRL << "\tReg --> " << val_idle << std::endl;
   	#endif

   	return val_idle;
}

uint32_t hls_ip_isReady(PRRegion *RegionInstance){

	assert(RegionInstance!=NULL);
	uint32_t data = 0xFFFFFFFF;

	read_fpga(&data, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_AP_CTRL);

	uint32_t mask = 0x00000008;
	uint32_t val = data & mask;

	#ifdef DEBUG
	std::cout <<"[DEBUG]: Ready Task\tAXILite Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_AP_CTRL << "\tReg --> " << val << std::endl;
	#endif

	return val;
}

void hls_ip_reset(PRRegion *RegionInstance){

	assert(RegionInstance!=NULL);
	uint32_t data = 0xFFFFFFFF;
	uint32_t mask = 0x00000100;
	uint32_t val = data & mask;

	#ifdef DEBUG
	std::cout <<"[DEBUG]: Reset Task\tAXILite Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_AP_CTRL << "\tReg --> " << val << std::endl;
	#endif

	write_fpga(&val, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_AP_CTRL);

}

void hls_ip_input_a(PRRegion *RegionInstance, uint64_t Data){

	assert(RegionInstance!=NULL);
	assert(RegionInstance->status==PR_CONFIG || RegionInstance->status==PR_WAITING);
	
	uint32_t MSB = (Data >> 32) & 0xFFFFFFFF;
	uint32_t LSB = Data & 0xFFFFFFFF;

	#ifdef DEBUG
	//std::cout <<"[DEBUG]: Input A\tAXILite MSB Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_A_MSB << "\tMSB --> " << MSB << std::endl;
	//std::cout <<"[DEBUG]: Input A\tAXILite LSB Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_A_LSB << "\tLSB --> " << LSB << std::endl;
	#endif

	write_fpga(&MSB, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_A_MSB);
	write_fpga(&LSB, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_A_LSB);

}

void hls_ip_input_b(PRRegion *RegionInstance, uint64_t Data){

	assert(RegionInstance!=NULL);
	assert(RegionInstance->status==PR_CONFIG || RegionInstance->status==PR_WAITING);
	
	uint32_t MSB = (Data >> 32) & 0xFFFFFFFF;
	uint32_t LSB = Data & 0xFFFFFFFF;

	#ifdef DEBUG
	//std::cout <<"[DEBUG]: Input B\tAXILite MSB Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_B_MSB << "\tMSB --> " << MSB << std::endl;
	//std::cout <<"[DEBUG]: Input B\tAXILite LSB Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_B_LSB << "\tLSB --> " << LSB << std::endl;
	#endif

	write_fpga(&MSB, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_B_MSB);
	write_fpga(&LSB, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_B_LSB);

}

void hls_ip_input_c(PRRegion *RegionInstance, uint64_t Data){

	assert(RegionInstance!=NULL);
	assert(RegionInstance->status==PR_CONFIG || RegionInstance->status==PR_WAITING);
	
	uint32_t MSB = (Data >> 32) & 0xFFFFFFFF;
	uint32_t LSB = Data & 0xFFFFFFFF;

	#ifdef DEBUG
	//std::cout <<"[DEBUG]: Input C\tAXILite MSB Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_C_MSB << "\tMSB --> " << MSB << std::endl;
	//std::cout <<"[DEBUG]: Input C\tAXILite LSB Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_C_LSB << "\tLSB --> " << LSB << std::endl;
	#endif

	write_fpga(&MSB, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_C_MSB);
	write_fpga(&LSB, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_C_LSB);

}

void hls_ip_input_d(PRRegion *RegionInstance, uint64_t Data){

	assert(RegionInstance!=NULL);
	assert(RegionInstance->status==PR_CONFIG || RegionInstance->status==PR_WAITING);
	
	uint32_t MSB = (Data >> 32) & 0xFFFFFFFF;
	uint32_t LSB = Data & 0xFFFFFFFF;

	#ifdef DEBUG
	//std::cout <<"[DEBUG]: Input D\tAXILite MSB Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_D_MSB << "\tMSB --> " << MSB << std::endl;
	//std::cout <<"[DEBUG]: Input D\tAXILite LSB Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_D_LSB << "\tLSB --> " << LSB << std::endl;
	#endif

	write_fpga(&MSB, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_D_MSB);
	write_fpga(&LSB, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_INPUT_D_LSB);

}

void enable_dcpl(PRRegion *RegionInstance){

	assert(RegionInstance!=NULL);
	assert(RegionInstance->status==PR_IDLE);
	uint32_t data = 0x1;

	write_fpga(&data, sizeof(uint32_t), RegionInstance->decouple_addr);

	#ifdef DEBUG
	const char *msg[2]={"not decoupled", "decoupled"};
	//std::cout <<"[DEBUG]: PR region " << RegionInstance->id << " is " << msg[data] << std::endl;
	#endif

}

void disable_dcpl(PRRegion *RegionInstance){

	assert(RegionInstance!=NULL);
	assert(RegionInstance->status==PR_CONFIG);
	uint32_t data = 0x0;

	#ifdef DEBUG
	const char *msg[2]={"not decoupled", "decoupled"};
	//std::cout <<"[DEBUG]: PR region " << RegionInstance->id << " is " << msg[data] << std::endl;
	#endif

	write_fpga(&data, sizeof(uint32_t), RegionInstance->decouple_addr);

}

uint32_t pr_task(PRRegion *RegionInstance, Task *task){

	uint32_t status, status_bits;
	read_fpga(&status, sizeof(uint32_t), ICAP_STATUS);
	status_bits = bitExtracted(status, 1, 1);

	#ifdef DEBUG
	//std::cout <<"[DEBUG]: ICAP Status " << status_bits << std::endl;
	#endif

	if(status_bits==1 && RegionInstance!=NULL && task!=NULL){
		enable_dcpl(RegionInstance);
		std::string base = std::string("./bitstreams/") + task->bin[RegionInstance->id];
		std::ifstream in_file(base.c_str(),std::ios::binary);
		in_file.seekg(0,std::ios::end);
		uint32_t bin_size=in_file.tellg()/4;
		RegionInstance->status = PR_CONFIG;
		RegionInstance->curr_task = task;
		task->state = TASK_CONFIGURED;
		task->reconfig_start = timestamp();

		uint32_t status, status_bits;
		write_fpga(&bin_size, sizeof(uint32_t), ICAP_LENGTH);
		write_bin(base.c_str(), in_file.tellg(), ICAP_ADDR);

		task->reconfig_end = timestamp();
		task->start_wait = timestamp();
		task->start_time = timestamp();
		#ifdef DEBUG
		std::cout <<"[DEBUG]: Partial Reconfiguration of region " << RegionInstance->id << " with " << task->bin[RegionInstance->id] << std::endl;
		#endif
		disable_dcpl(RegionInstance);
		status_bits = 0;
	}
	return status_bits;

}

void hls_ip_restart_counter(PRRegion *RegionInstance, uint32_t restart_counter){

        assert(RegionInstance!=NULL);
        assert(RegionInstance->status==PR_CONFIG || RegionInstance->status==PR_WAITING);

        #ifdef DEBUG
        //std::cout <<"[DEBUG]: Restart Counter\tAXILite Auto Restart Address --> " << RegionInstance->axilite_addr + AXILITE_ADDR_RESTART_COUNTER << "\tMSB --> " << restart_counter << std::endl;
        #endif

        write_fpga(&restart_counter, sizeof(uint32_t), RegionInstance->axilite_addr + AXILITE_ADDR_RESTART_COUNTER);

}

void hls_ip_set_axilite(PRRegion *RegionInstance, uint64_t base_addr, Task *curr_task){

	assert(RegionInstance!=NULL);
	assert(RegionInstance->status==PR_CONFIG || RegionInstance->status==PR_WAITING);

	uint64_t lite_addr = base_addr + 0x08;
	
	for(uint32_t itr : curr_task->lite_vars){

		if(!itr) break;

		#ifdef DEBUG
        //std::cout <<"[DEBUG]: AXILite Address --> " << RegionInstance->axilite_addr + lite_addr << "\tVar --> " << itr << std::endl;
        #endif

        write_fpga(&itr, sizeof(uint32_t), RegionInstance->axilite_addr + lite_addr);
        lite_addr += 0x08;

	}
}


void launch_task(PRRegion *RegionInstance){

	if(RegionInstance!=NULL && RegionInstance->curr_task!=NULL){
      	hls_ip_restart_counter(RegionInstance, 1);

		uint32_t counter=0;
		uint64_t lite_addr = 0x0C;
		for(uint64_t address : RegionInstance->curr_task->mm_addr){
			if(address && counter==0){
				hls_ip_input_a(RegionInstance, address);
				lite_addr = AXILITE_ADDR_INPUT_A_MSB;
			}
			else if(address && counter==1){
				hls_ip_input_b(RegionInstance, address);
				lite_addr = AXILITE_ADDR_INPUT_B_MSB;

			}
			else if(address && counter==2){
				hls_ip_input_c(RegionInstance, address);
				lite_addr = AXILITE_ADDR_INPUT_C_MSB;

			}
			else if(address && counter==3){
				hls_ip_input_d(RegionInstance, address);
				lite_addr = AXILITE_ADDR_INPUT_D_MSB;
			}
			counter+=1;
		}

		hls_ip_set_axilite(RegionInstance, lite_addr, RegionInstance->curr_task);
		
		if(RegionInstance->curr_task->compl_batch==0){
			//RegionInstance->curr_task->start_time = timestamp();
			RegionInstance->curr_task->end_wait = timestamp();
			RegionInstance->curr_task->total_wait_time = duration(RegionInstance->curr_task->start_wait, RegionInstance->curr_task->end_wait);
		}
		else{
			RegionInstance->curr_task->end_wait = timestamp();
			RegionInstance->curr_task->total_wait_time += duration(RegionInstance->curr_task->start_wait, RegionInstance->curr_task->end_wait);
		}
		
		hls_ip_start(RegionInstance);
		RegionInstance->status = PR_RUNNING;
		RegionInstance->curr_task->state = TASK_RUNNING;
		#ifdef DEBUG
		Task* curr_task = RegionInstance->curr_task;
		//std::cout <<"[DEBUG]: State of PR region " << RegionInstance->id <<" is updated to RUNNING" << std::endl;
		#endif
	}
}
