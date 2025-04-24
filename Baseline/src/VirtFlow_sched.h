#ifndef VIRTFLOW_SCHED_H
#define VIRTFLOW_SCHED_H

#include "VirtFlow_types.h"
#include "hls_hw_ip.h"
#include "hls_ip.h"
#include "event_gen.h"
#include <climits>

int init_regions(std::vector<PRRegion*> &regions);
void update_prr_state(std::vector<PRRegion*> &regions);
bool is_completed(std::list<Application*> &pending_apps, std::list<Application*> &completed_apps);

#endif
