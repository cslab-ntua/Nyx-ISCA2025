#ifndef NIMBLOCK_SCHED_H
#define NIMBLOCK_SCHED_H

#include "VirtFlow_types.h"
#include "hls_hw_ip.h"
#include "hls_ip.h"
#include "event_gen.h"
#include <climits>
#include <list>
#include <vector>
#include <algorithm>
#include <math.h>

#define THRESHOLD_LEVEL 8 // mallon prepei na to kanw osa to PR_REGION

double update_priorities_app(std::list<Application*>& pending_apps, std::list<Application*>& candidate_apps);
void allocate_resources(std::list<Application*>& candidate_apps);
bool update_candidate_apps(std::list<Application*>& pending_apps, std::list<Application*>& candidate_apps, double threshold);
PRRegion* select_prr_nimblock(std::vector<PRRegion*>& regions);
Task* select_task_nimblock(std::list<Application*>& candidate_apps, PRRegion *curr_region);
void launch_nimblock(std::vector<PRRegion*> &regions, PRRegion* sel_prr);

#endif
