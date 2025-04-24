#ifndef VIRTFLOW_TYPES_H
#define VIRTFLOW_TYPES_H

#define DEBUG

//#define FCFS
//#define RR
//#define SJF
//#define NONSHARING
#define NIMBLOCK

#include <vector>
#include <stdint.h>
#include <queue>
#include <chrono>
#include <list>


static uint64_t pr_latency = 0.00598;

// Low Demand 1500ms = 1.5s
//Stressful Conditions 150ms = 0.2s
//Real-Time Conditions 50ms = 0.05s
//Instantly 0ms = 0s

#define PR_IDLE   	0
#define PR_CONFIG   1
#define PR_WAITING	2
#define PR_RUNNING  3

#define FAIL 	0
#define SUCCESS 1

#define TASK_PENDING	0
#define TASK_SCHEDULED 	1
#define TASK_WAITING	2
#define TASK_RUNNING 	3
#define TASK_ENDED		4
#define TASK_CONFIGURED 5

class Application;

class Task {

	public:
		int id;	/*Task ID in the application list*/
		Application* app_id; /*Used to access the information of the application app*/
		double latency; /*Latency of the task*/
		double est_start, est_end, est_latency, est_reconfig; /* Estimated times for estimation & debug */
		std::vector<uint64_t> mm_addr;
		std::vector<uint32_t> lite_vars;
		const char* name;
		const char** bin;
		int state;
		int max_batch;
		int compl_batch;
		int task_rdy;
		std::chrono::high_resolution_clock::time_point start_time, end_time;
		std::chrono::high_resolution_clock::time_point start_wait, end_wait;
		std::chrono::high_resolution_clock::time_point reconfig_start, reconfig_end;
		double total_wait_time;

		Task(int id, const char* name, Application* app_id, double latency, int max_batch, int state, const char** bin):
		id(id),
		name(name),
		app_id(app_id),
		latency(latency),
		max_batch(max_batch),
		state(state),
		bin(bin),
		task_rdy(0),
		compl_batch(0),
		total_wait_time(0)
		{}
};


class Application {

	public:
		const char* app_name;
		int id;
		double latency;
		double est_latency; /* Estimated Latency including Partial Reconfiguration */
        double exec_time;
		uint16_t est_prrs; /* Estimated Partial Reconfigurable Regions */
		std::chrono::high_resolution_clock::time_point arrival_time, schedule_time, end_time;
		std::vector<Task*> tasks;
		std::vector<int*> dep_list;
		std::vector<int> num_deps;
        std::vector<int> num_deps_reverse;
		int tasks_completed;
		int max_batch;

		double priority;
		int priority_level; // Real time priority, not queue priority
		int occupancy;
        int threshold;
        int parallelism; // defines the parallism factor for this application's DFG

		Application(int id, double latency, int max_batch, std::chrono::high_resolution_clock::time_point arrival_time):
		id(id),
		latency(latency),
		max_batch(max_batch),
		arrival_time(arrival_time),
		tasks_completed(0)
		{}	

};

/*
 Inserting a pending task within the priority queue of a partial reconfigurable region triggers this function
 Automatically places the task at the appropriate index based on the below function.
*/
#ifdef SJF
struct task_priority{
	bool operator()(const Task* task1, const Task* task2){
    	if(task1->app_id == task2->app_id){
        	if(task1->id < task2->id)
            	return true;
            }

        if(task1->app_id->est_latency >= task2->app_id->est_latency)
        	return true;

		return false;
	}
};
#endif


class PRRegion{
	public:
		int id;
		int status;
		Task* curr_task;
		double prr_est_latency;
        
        #ifdef SJF
        std::priority_queue<Task*, std::vector<Task*>, struct task_priority> task_queue; /* Priority Queue to select the next task within the slot based on task_compare */
        #elif defined(RR)
        std::list<Task*> task_rr; /*List to enqueue tasks for round robin scheduling*/
        #endif
		
		uint64_t decouple_addr;
		uint64_t axilite_addr;

		#ifdef DEBUG
		double sim_time;
		#endif

		PRRegion(int id, int status, uint64_t decouple_addr, uint64_t axilite_addr):
		id(id),
		status(status),
		decouple_addr(decouple_addr),
		axilite_addr(axilite_addr),
		curr_task(NULL),
		#ifdef DEBUG
		sim_time(0),
		#endif
		prr_est_latency(0)
		{}

};

#endif
