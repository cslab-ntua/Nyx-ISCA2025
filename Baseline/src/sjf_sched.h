#ifndef SJF_SCHED_H
#define SJF_SCHED_H

#include "VirtFlow_types.h"
#include "hls_hw_ip.h"
#include "hls_ip.h"
#include "event_gen.h"
#include <climits>

void sjf_schedule(std::list<Application*> &pending_apps, std::list<Application*> &waiting_apps, std::list<Task*> &waiting_tasks);
PRRegion* select_prr_sjf(std::vector<PRRegion*> &regions);
Task* select_task_sjf(std::list<Task*> &waiting_tasks, PRRegion* sel_prr);
void launch_sjf(std::vector<PRRegion*> &regions, PRRegion* sel_prr);
#endif