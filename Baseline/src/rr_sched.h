#ifndef RR_SCHED_H
#define RR_SCHED_H

#include "VirtFlow_types.h"
#include "hls_hw_ip.h"
#include "hls_ip.h"
#include "event_gen.h"
#include <climits>

/* Round Robin (RR) functions */
void rr_schedule(std::vector<PRRegion*> &regions, std::list<Application*> &pending_apps, std::list<Application*> &waiting_apps);
PRRegion* select_prr_rr(std::vector<PRRegion*> &regions);
Task* select_task_rr(PRRegion* sel_prr);
void launch_rr(std::vector<PRRegion*> &regions, PRRegion* sel_prr);

#endif
