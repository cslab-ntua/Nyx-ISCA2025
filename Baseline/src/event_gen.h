#ifndef EVENT_GEN_H
#define EVENT_GEN_H

#include "VirtFlow_types.h"
#include "hls_hw_ip.h"
#include <list>
#include <iostream>
#include "applications/OpticalFlow.h"
#include "applications/Rendering.h"
#include "applications/Compression.h"
#include "applications/Lenet.h"
#include "applications/DigitRec.h"
#include "applications/Scaling.h"

//#define SINGLE
#define QUERY


const uint32_t seq_id[10][20] = {
        {3, 2, 2, 1, 1, 4, 5, 5, 4, 6, 2, 3, 1, 5, 3, 4, 3, 2, 4, 4 },
        {6, 4, 6, 2, 4, 4, 3, 3, 1, 3, 1, 6, 2, 1, 1, 5, 1, 6, 5, 5 },
        {4, 3, 1, 3, 3, 2, 6, 6, 4, 2, 6, 6, 2, 5, 4, 6, 6, 3, 4, 6 },
        {6, 1, 6, 4, 6, 2, 4, 6, 5, 3, 3, 5, 1, 5, 3, 4, 2, 5, 6, 5 },
        {6, 2, 2, 1, 2, 3, 4, 4, 5, 2, 3, 6, 5, 1, 4, 1, 4, 6, 5, 6 },
        {1, 6, 5, 3, 3, 4, 5, 6, 1, 4, 3, 5, 5, 2, 1, 6, 3, 1, 6, 5 },
        {3, 4, 3, 1, 6, 5, 6, 6, 5, 3, 4, 6, 5, 4, 1, 2, 3, 1, 3, 1 },
        {2, 1, 1, 3, 5, 4, 3, 6, 4, 1, 2, 2, 3, 2, 6, 1, 3, 5, 5, 6 },
        {2, 3, 1, 3, 4, 2, 3, 6, 3, 1, 4, 5, 2, 5, 6, 3, 2, 5, 5, 2 },
        {2, 2, 3, 4, 5, 3, 2, 1, 2, 1, 1, 6, 3, 5, 4, 1, 1, 4, 1, 6 },
};

const uint32_t seq_batch[10][20] = {
        {256, 4, 2, 1, 2, 512, 1, 1, 256, 1, 4, 512, 2, 1, 64, 16, 128, 2, 512, 1024 },
        {2, 512, 1, 2, 1024, 256, 1024, 512, 1, 512, 2, 2, 4, 1, 2, 1, 1, 2, 1, 1 },
        {512, 8, 1, 256, 512, 2, 1, 1, 512, 2, 2, 2, 4, 1, 512, 2, 2, 256, 1024, 2 },
        {2, 1, 1, 1024, 2, 4, 1024, 4, 1, 512, 256, 1, 2, 1, 128, 512, 2, 1, 2, 1 },
        {2, 2, 4, 2, 1, 1024, 1024, 512, 1, 4, 1024, 1, 1, 2, 512, 1, 1024, 2, 1, 1 },
        {2, 2, 1, 512, 256, 1024, 1, 2, 2, 256, 1024, 1, 1, 4, 1, 1, 1024, 1, 1, 1 },
        {512, 128, 1024, 1, 2, 1, 2, 2, 1, 128, 512, 2, 1, 1024, 1, 4, 32, 1, 512, 1},
        {4, 2, 2, 1024, 1, 512, 512, 2, 256, 2, 4, 2, 512, 4, 2, 2, 1024, 1, 1, 1 },
        {4, 128, 1, 1024, 256, 1, 512, 2, 256, 2, 1024, 1, 1, 1, 2, 512, 2, 1, 1, 4},
        {1, 2, 128, 1024, 1, 256, 2, 1, 1, 1, 2, 1, 512, 1, 512, 2, 1, 32, 1, 2 },
};

const uint32_t seq_p_level[10][20] = {
        {3, 9, 9, 9, 9, 3, 1, 1, 3, 1, 9, 3, 9, 1, 3, 3, 3, 9, 3, 3 },
        {1, 3, 1, 9, 3, 3, 3, 3, 9, 3, 9, 1, 9, 9, 9, 1, 9, 1, 1, 1 },
        {3, 3, 9, 3, 3, 9, 1, 1, 3, 9, 1, 1, 9, 1, 3, 1, 1, 3, 3, 1 },
        {1, 9, 1, 3, 1, 9, 3, 1, 1, 3, 3, 1, 9, 1, 3, 3, 9, 1, 1, 1 },
        {1, 9, 9, 9, 9, 3, 3, 3, 1, 9, 3, 1, 1, 9, 3, 9, 3, 1, 1, 1 },
        {9, 1, 1, 3, 3, 3, 1, 1, 9, 3, 3, 1, 1, 9, 9, 1, 3, 9, 1, 1 },
        {3, 3, 3, 9, 1, 1, 1, 1, 1, 3, 3, 1, 1, 3, 9, 9, 3, 9, 3, 9 },
        {9, 9, 9, 3, 1, 3, 3, 1, 3, 9, 9, 9, 3, 9, 1, 9, 3, 1, 1, 1 },
        {9, 3, 9, 3, 3, 9, 3, 1, 3, 9, 3, 1, 9, 1, 1, 3, 9, 1, 1, 9 },
        {9, 9, 3, 3, 1, 3, 9, 9, 9, 9, 9, 1, 3, 1, 3, 9, 9, 3, 9, 1 },
};

std::chrono::high_resolution_clock::time_point timestamp();
double duration(std::chrono::high_resolution_clock::time_point t1, std::chrono::high_resolution_clock::time_point t2);
#ifdef SINGLE
std::chrono::high_resolution_clock::time_point generate_event(std::chrono::high_resolution_clock::time_point prev_new_app, std::list<Application*> &pending_apps,
        std::list<Application*> &total_apps, int id, int batch, uint32_t MAX_APPS, double interval);

#elif defined(QUERY)

std::chrono::high_resolution_clock::time_point generate_event(std::chrono::high_resolution_clock::time_point prev_new_app, std::list<Application*> &pending_apps,
        std::list<Application*> &total_apps, int query, uint32_t MAX_APPS, double interval);
#endif


#endif
