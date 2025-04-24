#ifndef DEBUG_H
#define DEBUG_H

#include "VirtFlow_types.h"
#include <iostream>
#include <list>
#include <fstream>
#include <event_gen.h>

void print_new_app(Application* opt_flow_app);
#ifdef SINGLE
void export_metrics(std::list<Application*> &completed_apps, double total_time);
#elif defined (QUERY)
void export_metrics(std::list<Application*> &completed_apps, double total_time, uint32_t seq_num, double interval);
#endif


#endif
