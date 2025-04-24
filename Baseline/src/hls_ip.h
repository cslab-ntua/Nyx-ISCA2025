#ifndef HLS_IP_H
#define HLS_IP_H

#include "VirtFlow_types.h"
#include "hls_hw_ip.h"
#include "fpga/fpga_utils.h"
#include "debug.h"
#include <assert.h>
#include <fstream>

#define HLS_KERNEL_IDLE 0
#define HLS_KERNEL_RUNNING 1


void hls_ip_start(PRRegion *RegionInstance);
void hls_ip_reset(PRRegion *RegionInstance);
uint32_t hls_ip_isDone(PRRegion *RegionInstance);
uint32_t hls_ip_isIdle(PRRegion *RegionInstance);
uint32_t hls_ip_isReady(PRRegion *RegionInstance);

void enable_dcpl(PRRegion *RegionInstance);
void disable_dcpl(PRRegion *RegionInstance);

uint32_t pr_task(PRRegion *RegionInstance, Task *task);
void launch_task(PRRegion *RegionInstance);

#endif