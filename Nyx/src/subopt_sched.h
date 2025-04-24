#ifndef SUBOPT_SCHED_H
#define SUBOPT_SCHED_H

#include "VirtFlow_types.h"
#include "hls_hw_ip.h"
#include "hls_ip.h"
#include "event_gen.h"
#include <climits>


/* Suboptimal Non-Sharing functions */

PRRegion* select_prr_subopt(std::vector<PRRegion*> &regions);
Task* select_task_subopt(std::list<Task*> &ready_tasks, PRRegion *curr_region);
void launch_subopt(std::vector<PRRegion*> &regions, PRRegion* sel_prr);
void subopt_scheduling(std::list<Application*> &pending_apps, std::list<Application*> &waiting_app, std::list<Task*> &waiting_tasks);


#endif
