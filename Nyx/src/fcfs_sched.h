#ifndef FCFS_SCHED_H
#define FCFS_SCHED_H

#include "VirtFlow_types.h"
#include "hls_hw_ip.h"
#include "hls_ip.h"
#include "event_gen.h"
#include <climits>

/* Non-Sharing functions */
void nonshare_scheduling(std::list<Application*> &pending_apps, std::list<Application*> &waiting_app, std::list<Task*> &waiting_tasks);

/* First Come First Served (FCFS) functions - Also used for Non-Sharing execution */
PRRegion* select_prr_fcfs(std::vector<PRRegion*> &regions);
Task* select_task_fcfs(std::list<Task*> &ready_tasks, PRRegion *curr_region);
void fcfs_schedule(std::list<Application*> &pending_apps, std::list<Application*> &waiting_apps,std::list<Task*> &waiting_tasks);
void launch_fcfs(std::vector<PRRegion*> &regions, PRRegion* sel_prr);


#endif
