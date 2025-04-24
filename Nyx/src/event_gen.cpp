#include "event_gen.h"
#include <cstdlib>
#include <limits.h>

/* ****** START OF 3D RENDERING ******  */
#define NUM_TASKS_RENDERING 3

const char *task_name_render[NUM_TASKS_RENDERING] = {"projection_rasterization","zculling\t","coloring_outputFB\t"};

int num_dep_render[NUM_TASKS_RENDERING] = {0,2,2};
int num_dep_render_reverse[NUM_TASKS_RENDERING] = {2,2,0};
int dep_list_render[NUM_TASKS_RENDERING][NUM_TASKS_RENDERING] = {
	{-1, -1, -1},
	{ 0,  0, -1},
	{ 1,  1, -1},
};

const char *bin_render[NUM_TASKS_RENDERING][RECONFIG_REGIONS] = {
	{"render/render_prr0_task0.bin","render/render_prr1_task0.bin","render/render_prr2_task0.bin","render/render_prr3_task0.bin","render/render_prr4_task0.bin","render/render_prr5_task0.bin","render/render_prr6_task0.bin","render/render_prr7_task0.bin"},
	{"render/render_prr0_task1.bin","render/render_prr1_task1.bin","render/render_prr2_task1.bin","render/render_prr3_task1.bin","render/render_prr4_task1.bin","render/render_prr5_task1.bin","render/render_prr6_task1.bin","render/render_prr7_task1.bin"},
	{"render/render_prr0_task2.bin","render/render_prr1_task2.bin","render/render_prr2_task2.bin","render/render_prr3_task2.bin","render/render_prr4_task2.bin","render/render_prr5_task2.bin","render/render_prr6_task2.bin","render/render_prr7_task2.bin"},
};

uint64_t mm_addr_render[NUM_TASKS_RENDERING][1] = {
	{RENDERING_INPUT},
	{0},
	{RENDERING_OUTPUT},
};

uint32_t lite_render[NUM_TASKS_RENDERING][1] = {
    {0},
    {0},
    {0},
};

uint64_t render_lat[NUM_TASKS_RENDERING] = {270809, 159562, 91589};

void create_rendering(Application* render_app){

	render_app->app_name = "3D Rendering";
	render_app->exec_time = 0.0286994 * render_app->max_batch;

	for(int i=0; i<NUM_TASKS_RENDERING; i++){

		Task* task_event = new Task(i, task_name_render[i], render_app, render_lat[i], render_app->max_batch, TASK_PENDING, bin_render[i]);
		for(int iter=0; iter<(sizeof(mm_addr_render[i])/sizeof(mm_addr_render[i][0])); iter++)
			task_event->mm_addr.push_back(mm_addr_render[i][iter]);

		for(int iter=0; iter<(sizeof(lite_render[i])/sizeof(lite_render[i][0])); iter++)
        	task_event->lite_vars.push_back(lite_render[i][iter]);


		render_app->tasks.push_back(task_event);
		render_app->dep_list.push_back(dep_list_render[i]);
		render_app->num_deps.push_back(num_dep_render[i]);
		render_app->num_deps_reverse.push_back(num_dep_render_reverse[i]);

		#ifdef NIMBLOCK
		render_app->parallelism = NUM_TASKS_RENDERING;
		#endif

	}
}

/* ****** END OF 3D RENDERING ******  */


/* ****** START OF OPTICAL FLOW ******  */
#define NUM_TASKS_OPTICAL_FLOW 9

const char *task_name_optflow[NUM_TASKS_OPTICAL_FLOW] = {"read_preprocess","gradient_xy_calc","gradient_z_calc","gradient_weight_y",
													"gradient_weight_x","outer_product\t","tensor_weight_y","tensor_weight_x","flow_calc\t"};

int num_dep_optflow[NUM_TASKS_OPTICAL_FLOW] = {0,1,1,2,1,1,1,1,1};
int num_dep_optflow_reverse[NUM_TASKS_OPTICAL_FLOW] = {2,1,1,1,1,1,1,1,0};

int dep_list_optflow[NUM_TASKS_OPTICAL_FLOW][NUM_TASKS_OPTICAL_FLOW] = {
	{-1, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 0, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 0, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 1,  2, -1, -1, -1, -1, -1, -1, -1},
	{ 3, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 4, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 5, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 6, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 7, -1, -1, -1, -1, -1, -1, -1, -1},
};

const char *bin_optflow[NUM_TASKS_OPTICAL_FLOW][RECONFIG_REGIONS] = {
	{"optflow/optflow_prr0_task0.bin","optflow/optflow_prr1_task0.bin","optflow/optflow_prr2_task0.bin","optflow/optflow_prr3_task0.bin","optflow/optflow_prr4_task0.bin","optflow/optflow_prr5_task0.bin","optflow/optflow_prr6_task0.bin","optflow/optflow_prr7_task0.bin"},
	{"optflow/optflow_prr0_task1.bin","optflow/optflow_prr1_task1.bin","optflow/optflow_prr2_task1.bin","optflow/optflow_prr3_task1.bin","optflow/optflow_prr4_task1.bin","optflow/optflow_prr5_task1.bin","optflow/optflow_prr6_task1.bin","optflow/optflow_prr7_task1.bin"},
	{"optflow/optflow_prr0_task2.bin","optflow/optflow_prr1_task2.bin","optflow/optflow_prr2_task2.bin","optflow/optflow_prr3_task2.bin","optflow/optflow_prr4_task2.bin","optflow/optflow_prr5_task2.bin","optflow/optflow_prr6_task2.bin","optflow/optflow_prr7_task2.bin"},
	{"optflow/optflow_prr0_task3.bin","optflow/optflow_prr1_task3.bin","optflow/optflow_prr2_task3.bin","optflow/optflow_prr3_task3.bin","optflow/optflow_prr4_task3.bin","optflow/optflow_prr5_task3.bin","optflow/optflow_prr6_task3.bin","optflow/optflow_prr7_task3.bin"},
	{"optflow/optflow_prr0_task4.bin","optflow/optflow_prr1_task4.bin","optflow/optflow_prr2_task4.bin","optflow/optflow_prr3_task4.bin","optflow/optflow_prr4_task4.bin","optflow/optflow_prr5_task4.bin","optflow/optflow_prr6_task4.bin","optflow/optflow_prr7_task4.bin"},
	{"optflow/optflow_prr0_task5.bin","optflow/optflow_prr1_task5.bin","optflow/optflow_prr2_task5.bin","optflow/optflow_prr3_task5.bin","optflow/optflow_prr4_task5.bin","optflow/optflow_prr5_task5.bin","optflow/optflow_prr6_task5.bin","optflow/optflow_prr7_task5.bin"},
	{"optflow/optflow_prr0_task6.bin","optflow/optflow_prr1_task6.bin","optflow/optflow_prr2_task6.bin","optflow/optflow_prr3_task6.bin","optflow/optflow_prr4_task6.bin","optflow/optflow_prr5_task6.bin","optflow/optflow_prr6_task6.bin","optflow/optflow_prr7_task6.bin"},
	{"optflow/optflow_prr0_task7.bin","optflow/optflow_prr1_task7.bin","optflow/optflow_prr2_task7.bin","optflow/optflow_prr3_task7.bin","optflow/optflow_prr4_task7.bin","optflow/optflow_prr5_task7.bin","optflow/optflow_prr6_task7.bin","optflow/optflow_prr7_task7.bin"},
	{"optflow/optflow_prr0_task8.bin","optflow/optflow_prr1_task8.bin","optflow/optflow_prr2_task8.bin","optflow/optflow_prr3_task8.bin","optflow/optflow_prr4_task8.bin","optflow/optflow_prr5_task8.bin","optflow/optflow_prr6_task8.bin","optflow/optflow_prr7_task8.bin"},
};

uint64_t mm_addr_optflow[NUM_TASKS_OPTICAL_FLOW][1] = {
	{OPTICALFLOW_INPUT},
	{0},
	{0},
	{0},
	{0},
	{0},
	{0},
	{0},
	{OPTICALFLOW_OUTPUT},
};

uint32_t lite_optflow[NUM_TASKS_OPTICAL_FLOW][1] = {
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
};

uint64_t optflow_lat[NUM_TASKS_OPTICAL_FLOW] = {446477, 449392, 446468, 449541, 447775, 446469, 2237443, 446903, 893012};

void create_optical_flow(Application* opt_flow_app){

	opt_flow_app->app_name = "Optical Flow";
	opt_flow_app->exec_time = 0.1098438 * opt_flow_app->max_batch;

	for(int i=0; i<NUM_TASKS_OPTICAL_FLOW; i++){

		Task* task_event = new Task(i, task_name_optflow[i], opt_flow_app, optflow_lat[i], opt_flow_app->max_batch, TASK_PENDING, bin_optflow[i]);
		for(int iter=0; iter<(sizeof(mm_addr_optflow[i])/sizeof(mm_addr_optflow[i][0])); iter++)
			task_event->mm_addr.push_back(mm_addr_optflow[i][iter]);

		for(int iter=0; iter<(sizeof(lite_optflow[i])/sizeof(lite_optflow[i][0])); iter++)
        	task_event->lite_vars.push_back(lite_optflow[i][iter]);

		opt_flow_app->tasks.push_back(task_event);
		opt_flow_app->dep_list.push_back(dep_list_optflow[i]);
		opt_flow_app->num_deps.push_back(num_dep_optflow[i]);
		opt_flow_app->num_deps_reverse.push_back(num_dep_optflow_reverse[i]);

		#ifdef NIMBLOCK
		opt_flow_app->parallelism = NUM_TASKS_OPTICAL_FLOW;
		#endif

	}
}
/* ****** END OF OPTICAL FLOW ******  */

/* ****** START OF IMAGE COMPRESSION ******  */

#define NUM_TASKS_IMAGE_COMPRESSION 7

const char *task_name_compress[NUM_TASKS_IMAGE_COMPRESSION] = {"padImage","extractBlockD","dct","quantize", "dequantize","idct","reconstructBlockD"};


int num_dep_compress[NUM_TASKS_IMAGE_COMPRESSION] = {0,1,1,1,1,1,1};
int num_dep_compress_reverse[NUM_TASKS_IMAGE_COMPRESSION] = {1,1,1,1,1,1,0};

int dep_list_compress[NUM_TASKS_IMAGE_COMPRESSION][NUM_TASKS_IMAGE_COMPRESSION] = {
    {-1, -1, -1, -1, -1, -1, -1},
    { 0, -1, -1, -1, -1, -1, -1},
    { 1, -1, -1, -1, -1, -1, -1},
    { 2, -1, -1, -1, -1, -1, -1},
    { 3, -1, -1, -1, -1, -1, -1},
    { 4, -1, -1, -1, -1, -1, -1},
    { 5, -1, -1, -1, -1, -1, -1},
};

const char *bin_compress[NUM_TASKS_IMAGE_COMPRESSION][RECONFIG_REGIONS] = {
   	{"compress/compress_prr0_task0.bin","compress/compress_prr1_task0.bin","compress/compress_prr2_task0.bin","compress/compress_prr3_task0.bin","compress/compress_prr4_task0.bin","compress/compress_prr5_task0.bin","compress/compress_prr6_task0.bin","compress/compress_prr7_task0.bin"},
    {"compress/compress_prr0_task1.bin","compress/compress_prr1_task1.bin","compress/compress_prr2_task1.bin","compress/compress_prr3_task1.bin","compress/compress_prr4_task1.bin","compress/compress_prr5_task1.bin","compress/compress_prr6_task1.bin","compress/compress_prr7_task1.bin"},
    {"compress/compress_prr0_task2.bin","compress/compress_prr1_task2.bin","compress/compress_prr2_task2.bin","compress/compress_prr3_task2.bin","compress/compress_prr4_task2.bin","compress/compress_prr5_task2.bin","compress/compress_prr6_task2.bin","compress/compress_prr7_task2.bin"},
    {"compress/compress_prr0_task3.bin","compress/compress_prr1_task3.bin","compress/compress_prr2_task3.bin","compress/compress_prr3_task3.bin","compress/compress_prr4_task3.bin","compress/compress_prr5_task3.bin","compress/compress_prr6_task3.bin","compress/compress_prr7_task3.bin"},
    {"compress/compress_prr0_task4.bin","compress/compress_prr1_task4.bin","compress/compress_prr2_task4.bin","compress/compress_prr3_task4.bin","compress/compress_prr4_task4.bin","compress/compress_prr5_task4.bin","compress/compress_prr6_task4.bin","compress/compress_prr7_task4.bin"},
    {"compress/compress_prr0_task5.bin","compress/compress_prr1_task5.bin","compress/compress_prr2_task5.bin","compress/compress_prr3_task5.bin","compress/compress_prr4_task5.bin","compress/compress_prr5_task5.bin","compress/compress_prr6_task5.bin","compress/compress_prr7_task5.bin"},
    {"compress/compress_prr0_task6.bin","compress/compress_prr1_task6.bin","compress/compress_prr2_task6.bin","compress/compress_prr3_task6.bin","compress/compress_prr4_task6.bin","compress/compress_prr5_task6.bin","compress/compress_prr6_task6.bin","compress/compress_prr7_task6.bin"},
};

uint64_t mm_addr_compress[NUM_TASKS_IMAGE_COMPRESSION][1] = {
   	{COMPRESSION_INPUT},
    {0},
    {0},
    {0},
    {0},
    {0},
    {COMPRESSION_OUTPUT},
};

uint32_t lite_compress[NUM_TASKS_IMAGE_COMPRESSION][4] = {
    {25, 10, W, H},
    {W, H, 0, 0},
    {W, H, 0, 0},
    {W, H, 0, 0},
    {W, H, 0, 0},
    {W, H, 0, 0},
    {W, H, 0, 0},
};

uint64_t compress_lat[NUM_TASKS_IMAGE_COMPRESSION] = {21410, 2070, 67250, 66018, 65994, 67250, 3077};

void create_image_compression(Application* compress_app){

	compress_app->app_name = "Image Compression";
	compress_app->exec_time = 0.0436543 * compress_app->max_batch;

	for(int i=0; i<NUM_TASKS_IMAGE_COMPRESSION; i++){

		Task* task_event = new Task(i, task_name_compress[i], compress_app, compress_lat[i], compress_app->max_batch, TASK_PENDING, bin_compress[i]);
		for(int iter=0; iter<(sizeof(mm_addr_compress[i])/sizeof(mm_addr_compress[i][0])); iter++)
			task_event->mm_addr.push_back(mm_addr_compress[i][iter]);

		for(int iter=0; iter<(sizeof(lite_compress[i])/sizeof(lite_compress[i][0])); iter++)
			task_event->lite_vars.push_back(lite_compress[i][iter]);

		compress_app->tasks.push_back(task_event);
		compress_app->dep_list.push_back(dep_list_compress[i]);
		compress_app->num_deps.push_back(num_dep_compress[i]);
        compress_app->num_deps_reverse.push_back(num_dep_compress_reverse[i]);
		
		#ifdef NIMBLOCK
		compress_app->parallelism = NUM_TASKS_IMAGE_COMPRESSION;
		#endif

	}
}

/* ****** END OF IMAGE COMPRESSION ******  */

/* ****** START OF LENET ******  */

#define NUM_TASKS_LENET 7

const char *task_name_lenet[NUM_TASKS_LENET] = {"conv1","pool1","conv2","pool2","fc1","fc2","fc3"};

int num_dep_lenet[NUM_TASKS_LENET] = {0,1,1,1,1,1,1};
int num_dep_lenet_reverse[NUM_TASKS_LENET] = {1,1,1,1,1,1,0};

int dep_list_lenet[NUM_TASKS_LENET][NUM_TASKS_LENET] = {
    {-1, -1, -1, -1, -1, -1, -1},
    { 0, -1, -1, -1, -1, -1, -1},
    { 1, -1, -1, -1, -1, -1, -1},
    { 2, -1, -1, -1, -1, -1, -1},
    { 3, -1, -1, -1, -1, -1, -1},
    { 4, -1, -1, -1, -1, -1, -1},
    { 5, -1, -1, -1, -1, -1, -1},
};

const char *bin_lenet[NUM_TASKS_LENET][RECONFIG_REGIONS] = {
    {"lenet/lenet_prr0_task0.bin","lenet/lenet_prr1_task0.bin","lenet/lenet_prr2_task0.bin","lenet/lenet_prr3_task0.bin","lenet/lenet_prr4_task0.bin","lenet/lenet_prr5_task0.bin","lenet/lenet_prr6_task0.bin","lenet/lenet_prr7_task0.bin"},
    {"lenet/lenet_prr0_task1.bin","lenet/lenet_prr1_task1.bin","lenet/lenet_prr2_task1.bin","lenet/lenet_prr3_task1.bin","lenet/lenet_prr4_task1.bin","lenet/lenet_prr5_task1.bin","lenet/lenet_prr6_task1.bin","lenet/lenet_prr7_task1.bin"},
    {"lenet/lenet_prr0_task2.bin","lenet/lenet_prr1_task2.bin","lenet/lenet_prr2_task2.bin","lenet/lenet_prr3_task2.bin","lenet/lenet_prr4_task2.bin","lenet/lenet_prr5_task2.bin","lenet/lenet_prr6_task2.bin","lenet/lenet_prr7_task2.bin"},
    {"lenet/lenet_prr0_task3.bin","lenet/lenet_prr1_task3.bin","lenet/lenet_prr2_task3.bin","lenet/lenet_prr3_task3.bin","lenet/lenet_prr4_task3.bin","lenet/lenet_prr5_task3.bin","lenet/lenet_prr6_task3.bin","lenet/lenet_prr7_task3.bin"},
    {"lenet/lenet_prr0_task4.bin","lenet/lenet_prr1_task4.bin","lenet/lenet_prr2_task4.bin","lenet/lenet_prr3_task4.bin","lenet/lenet_prr4_task4.bin","lenet/lenet_prr5_task4.bin","lenet/lenet_prr6_task4.bin","lenet/lenet_prr7_task4.bin"},
    {"lenet/lenet_prr0_task5.bin","lenet/lenet_prr1_task5.bin","lenet/lenet_prr2_task5.bin","lenet/lenet_prr3_task5.bin","lenet/lenet_prr4_task5.bin","lenet/lenet_prr5_task5.bin","lenet/lenet_prr6_task5.bin","lenet/lenet_prr7_task5.bin"},
    {"lenet/lenet_prr0_task6.bin","lenet/lenet_prr1_task6.bin","lenet/lenet_prr2_task6.bin","lenet/lenet_prr3_task6.bin","lenet/lenet_prr4_task6.bin","lenet/lenet_prr5_task6.bin","lenet/lenet_prr6_task6.bin","lenet/lenet_prr7_task6.bin"},
};

uint64_t mm_addr_lenet[NUM_TASKS_LENET][1] = {
    {LENET_INPUT},
    {0},
    {0},
    {0},
    {0},
    {0},
    {LENET_OUTPUT},
};

uint32_t lite_lenet[NUM_TASKS_LENET][1] = {
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
};

uint64_t lenet_lat[NUM_TASKS_LENET] = {108758, 13109, 12509, 5201, 138971, 11583, 958};

void create_lenet(Application* lenet_app){

	lenet_app->app_name = "Lenet";
	lenet_app->exec_time = 0.0596197 * lenet_app->max_batch;

	for(int i=0; i<NUM_TASKS_LENET; i++){

		Task* task_event = new Task(i, task_name_lenet[i], lenet_app, lenet_lat[i], lenet_app->max_batch, TASK_PENDING, bin_lenet[i]);
		for(int iter=0; iter<(sizeof(mm_addr_lenet[i])/sizeof(mm_addr_lenet[i][0])); iter++)
			task_event->mm_addr.push_back(mm_addr_lenet[i][iter]);

		for(int iter=0; iter<(sizeof(lite_lenet[i])/sizeof(lite_lenet[i][0])); iter++)
			task_event->lite_vars.push_back(lite_lenet[i][iter]);

		lenet_app->tasks.push_back(task_event);
		lenet_app->dep_list.push_back(dep_list_lenet[i]);
		lenet_app->num_deps.push_back(num_dep_lenet[i]);
		lenet_app->num_deps_reverse.push_back(num_dep_lenet_reverse[i]);

		#ifdef NIMBLOCK
		lenet_app->parallelism = NUM_TASKS_LENET;
		#endif

    }
}

/* ****** END OF LENET ******  */

/* ****** START OF DIGITREC ******  */

#define NUM_TASKS_DIGITREC 2

const char *task_name_digitrec[NUM_TASKS_DIGITREC] = {"train_module","predict_module"};

int num_dep_digitrec[NUM_TASKS_DIGITREC] = {0,1};
int num_dep_digitrec_reverse[NUM_TASKS_DIGITREC] = {1,0};

int dep_list_digitrec[NUM_TASKS_DIGITREC][NUM_TASKS_DIGITREC] = {
	{-1, -1},
	{ 0, -1},
};

const char *bin_digitrec[NUM_TASKS_DIGITREC][RECONFIG_REGIONS] = {
	{"digitrec/digitrec_prr0_task0.bin","digitrec/digitrec_prr1_task0.bin","digitrec/digitrec_prr2_task0.bin","digitrec/digitrec_prr3_task0.bin","digitrec/digitrec_prr4_task0.bin","digitrec/digitrec_prr5_task0.bin","digitrec/digitrec_prr6_task0.bin","digitrec/digitrec_prr7_task0.bin"},
	{"digitrec/digitrec_prr0_task1.bin","digitrec/digitrec_prr1_task1.bin","digitrec/digitrec_prr2_task1.bin","digitrec/digitrec_prr3_task1.bin","digitrec/digitrec_prr4_task1.bin","digitrec/digitrec_prr5_task1.bin","digitrec/digitrec_prr6_task1.bin","digitrec/digitrec_prr7_task1.bin"},
};

uint64_t mm_addr_digitrec[NUM_TASKS_DIGITREC][2] = {
	{DIGITREC_INPUT_TEST, DIGITREC_INPUT_TRAIN},
	{DIGITREC_OUTPUT, 0},
};

uint32_t lite_digitrec[NUM_TASKS_DIGITREC][1] = {
	{0},
	{0},
};

uint64_t digitrec_lat[NUM_TASKS_DIGITREC] = {720000140, 1180120};

void create_digitrec(Application* digitrec_app){

	digitrec_app->app_name = "Digit Recognition";
	digitrec_app->exec_time = 4.5044720 * digitrec_app->max_batch;

	for(int i=0; i<NUM_TASKS_DIGITREC; i++){

		Task* task_event = new Task(i, task_name_digitrec[i], digitrec_app, digitrec_lat[i], digitrec_app->max_batch, TASK_PENDING, bin_digitrec[i]);
		for(int iter=0; iter<(sizeof(mm_addr_digitrec[i])/sizeof(mm_addr_digitrec[i][0])); iter++)
			task_event->mm_addr.push_back(mm_addr_digitrec[i][iter]);

		for(int iter=0; iter<(sizeof(lite_digitrec[i])/sizeof(lite_digitrec[i][0])); iter++)
			task_event->lite_vars.push_back(lite_digitrec[i][iter]);

		digitrec_app->tasks.push_back(task_event);
		digitrec_app->dep_list.push_back(dep_list_digitrec[i]);
		digitrec_app->num_deps.push_back(num_dep_digitrec[i]);
		digitrec_app->num_deps_reverse.push_back(num_dep_digitrec_reverse[i]);

		#ifdef NIMBLOCK
		digitrec_app->parallelism = NUM_TASKS_DIGITREC;
		#endif

	}
}

/* ****** END OF DIGITREC ******  */

/* ****** START OF IMAGE SCALING ******  */
#define NUM_TASKS_IMAGE_SCALING 9

const char *task_name_imgscale[NUM_TASKS_IMAGE_SCALING] = {"conrr_layer1","conrr_layer2","conrr_layer3","conrr_layer4",
													"conrr_layer5","conrr_layer6","conrr_layer7","decorr_layer8","uncon_layer9"};

int num_dep_imgscale[NUM_TASKS_IMAGE_SCALING] = {0,1,1,1,1,1,1,1,1};
int num_dep_imgscale_reverse[NUM_TASKS_IMAGE_SCALING] = {1,1,1,1,1,1,1,1,0};

int dep_list_imgscale[NUM_TASKS_IMAGE_SCALING][NUM_TASKS_IMAGE_SCALING] = {
	{-1, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 0, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 1, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 2, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 3, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 4, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 5, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 6, -1, -1, -1, -1, -1, -1, -1, -1},
	{ 7, -1, -1, -1, -1, -1, -1, -1, -1},
};

const char *bin_imgscale[NUM_TASKS_IMAGE_SCALING][RECONFIG_REGIONS] = {
	{"scale/scale_prr0_task0.bin","scale/scale_prr1_task0.bin","scale/scale_prr2_task0.bin","scale/scale_prr3_task0.bin","scale/scale_prr4_task0.bin","scale/scale_prr5_task0.bin","scale/scale_prr6_task0.bin","scale/scale_prr7_task0.bin"},
	{"scale/scale_prr0_task1.bin","scale/scale_prr1_task1.bin","scale/scale_prr2_task1.bin","scale/scale_prr3_task1.bin","scale/scale_prr4_task1.bin","scale/scale_prr5_task1.bin","scale/scale_prr6_task1.bin","scale/scale_prr7_task1.bin"},
	{"scale/scale_prr0_task2.bin","scale/scale_prr1_task2.bin","scale/scale_prr2_task2.bin","scale/scale_prr3_task2.bin","scale/scale_prr4_task2.bin","scale/scale_prr5_task2.bin","scale/scale_prr6_task2.bin","scale/scale_prr7_task2.bin"},
	{"scale/scale_prr0_task3.bin","scale/scale_prr1_task3.bin","scale/scale_prr2_task3.bin","scale/scale_prr3_task3.bin","scale/scale_prr4_task3.bin","scale/scale_prr5_task3.bin","scale/scale_prr6_task3.bin","scale/scale_prr7_task3.bin"},
	{"scale/scale_prr0_task4.bin","scale/scale_prr1_task4.bin","scale/scale_prr2_task4.bin","scale/scale_prr3_task4.bin","scale/scale_prr4_task4.bin","scale/scale_prr5_task4.bin","scale/scale_prr6_task4.bin","scale/scale_prr7_task4.bin"},
	{"scale/scale_prr0_task5.bin","scale/scale_prr1_task5.bin","scale/scale_prr2_task5.bin","scale/scale_prr3_task5.bin","scale/scale_prr4_task5.bin","scale/scale_prr5_task5.bin","scale/scale_prr6_task5.bin","scale/scale_prr7_task5.bin"},
	{"scale/scale_prr0_task6.bin","scale/scale_prr1_task6.bin","scale/scale_prr2_task6.bin","scale/scale_prr3_task6.bin","scale/scale_prr4_task6.bin","scale/scale_prr5_task6.bin","scale/scale_prr6_task6.bin","scale/scale_prr7_task6.bin"},
	{"scale/scale_prr0_task7.bin","scale/scale_prr1_task7.bin","scale/scale_prr2_task7.bin","scale/scale_prr3_task7.bin","scale/scale_prr4_task7.bin","scale/scale_prr5_task7.bin","scale/scale_prr6_task7.bin","scale/scale_prr7_task7.bin"},
	{"scale/scale_prr0_task8.bin","scale/scale_prr1_task8.bin","scale/scale_prr2_task8.bin","scale/scale_prr3_task8.bin","scale/scale_prr4_task8.bin","scale/scale_prr5_task8.bin","scale/scale_prr6_task8.bin","scale/scale_prr7_task8.bin"},
};

uint64_t mm_addr_imgscale[NUM_TASKS_IMAGE_SCALING][1] = {
	{SCALING_INPUT},
	{0},
	{0},
	{0},
	{0},
	{0},
	{0},
	{0},
	{SCALING_OUTPUT},
};

uint32_t lite_imgscale[NUM_TASKS_IMAGE_SCALING][4] = {
    {64, 64, 0 ,0},
    {64, 64, 0, 0},
    {64, 64, 0, 0},
    {64, 64, 0, 0},
    {64, 64, 0, 0},
    {64, 64, 0, 0},
    {64, 64, 0, 0},
    {64, 64, 2, 0},
    {64, 64, 0, 0},
};

uint64_t imgscale_lat[NUM_TASKS_IMAGE_SCALING] = {14591958, 28030594, 21588914, 21588914, 21588914, 21588914, 28230530, 289633208, 18465};

void create_image_scaling(Application* imgscale_app){

	imgscale_app->app_name = "Image Scaling";
	imgscale_app->exec_time = 2.0350289 * imgscale_app->max_batch;

	for(int i=0; i<NUM_TASKS_IMAGE_SCALING; i++){

		Task* task_event = new Task(i, task_name_imgscale[i], imgscale_app, imgscale_lat[i], imgscale_app->max_batch, TASK_PENDING, bin_imgscale[i]);
		for(int iter=0; iter<(sizeof(mm_addr_imgscale[i])/sizeof(mm_addr_imgscale[i][0])); iter++)
			task_event->mm_addr.push_back(mm_addr_imgscale[i][iter]);

		for(int iter=0; iter<(sizeof(lite_imgscale[i])/sizeof(lite_imgscale[i][0])); iter++)
        	task_event->lite_vars.push_back(lite_imgscale[i][iter]);

		imgscale_app->tasks.push_back(task_event);
		imgscale_app->dep_list.push_back(dep_list_imgscale[i]);
		imgscale_app->num_deps.push_back(num_dep_imgscale[i]);
		imgscale_app->num_deps_reverse.push_back(num_dep_imgscale_reverse[i]);

		#ifdef NIMBLOCK
		imgscale_app->parallelism = NUM_TASKS_IMAGE_SCALING;
		#endif

	}
}
/* ****** END OF IMAGE SCALING ******  */



std::chrono::high_resolution_clock::time_point timestamp(){
	return std::chrono::high_resolution_clock::now();
}

double duration(std::chrono::high_resolution_clock::time_point t1, std::chrono::high_resolution_clock::time_point t2){

	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	return time_span.count();
}

void est_runtime_prrs(Application *curr_app){

	double app_runtime = 0;
	double app_runtime_without_batch = 0;
	double total_reconfig = 0;
	std::list<Task*> prr_list;

	for(Task *curr_task : curr_app->tasks){

		double start_time = 0;
		double end_time = INT_MAX;
		Task* loop_task;

		/* Start time according to dependencies */
		int curr_dep = curr_app->num_deps[curr_task->id];
		for(int iter=0; iter<curr_dep; iter++){
			double dep_start = curr_app->tasks[curr_app->dep_list[curr_task->id][iter]]->est_start; 
			start_time = std::max(start_time, dep_start);
		}

		/* Find the task that finishes sooner than others */ 
		for(Task* iter : prr_list){
			double task_end_time = iter->est_end;
			if (end_time > task_end_time){
				end_time = task_end_time;
				loop_task = iter;
			}
		}

		if(prr_list.size() < RECONFIG_REGIONS){
			if(end_time >= start_time){
				prr_list.push_back(curr_task);
			}
			else{
				prr_list.remove(loop_task);
				prr_list.push_back(curr_task);
			}
		}
		else{
			start_time = std::max(loop_task->est_reconfig, end_time + pr_latency);
			prr_list.remove(loop_task);
			prr_list.push_back(curr_task);
		}

		
		curr_task->est_reconfig = std::max(start_time, total_reconfig);
		curr_task->est_start = curr_task->est_reconfig + pr_latency;
		double compl_time = curr_task->est_start + curr_task->latency * curr_task->max_batch; /* Estimation includes Batch Size */
		app_runtime = std::max(app_runtime, compl_time);
		curr_task->est_end = std::max(app_runtime, curr_task->est_start + curr_task->latency * curr_task->max_batch); /* Estimation includes Batch Size */

		app_runtime_without_batch = std::max(app_runtime_without_batch, curr_task->est_start + curr_task->latency); /* For Simulation purposes only */
		curr_task->est_latency = std::max(app_runtime_without_batch, curr_task->est_start + curr_task->latency) - curr_task->est_start; /* Used for simulation runtime to verify functionality */

		total_reconfig = std::max(total_reconfig+pr_latency, curr_task->est_reconfig+pr_latency);

	}

	curr_app->est_latency = app_runtime;
	curr_app->est_prrs = static_cast<uint16_t>(prr_list.size());

}

uint32_t curr_event=0;

#ifdef QUERY

void add_new_app(std::list<Application*> &pending_apps, std::list<Application*> &total_apps, int query_id){

    int app_id = seq_id[query_id][curr_event];
	int batch_size = seq_batch[query_id][curr_event];

	Application* app_event = new Application(app_id, 0, batch_size, timestamp());

	#ifdef NIMBLOCK
	int p_level = seq_p_level[query_id][curr_event];
	app_event->priority = p_level;
	app_event->priority_level = p_level;
	app_event->parallelism = 1;
	app_event->occupancy = 0;
	#endif
	curr_event++;

    switch (app_id){
	    case 1:{
    		create_optical_flow(app_event);
        	break;
        }
        case 2:{
            create_rendering(app_event);
            break;
        }
        case 3:{
            create_image_compression(app_event);
            break;
        }
        case 4:{
            create_lenet(app_event);	
            break;
        }
        case 5:{
            create_digitrec(app_event);
            break;
        }
        case 6:{
            create_image_scaling(app_event);
            break;
        }
        default:
            return;
    }

    est_runtime_prrs(app_event);
    pending_apps.push_back(app_event);
    total_apps.push_back(app_event);

    #ifdef DEBUG
    //print_new_app(app_event);
    #endif
    return;

}
#endif



#ifdef SINGLE

void add_new_app(std::list<Application*> &pending_apps, std::list<Application*> &total_apps, int app_id, int batch_size){

	Application* app_event = new Application(app_id, 0, batch_size, timestamp());
    switch (app_id){
    	case 1:{
        	create_optical_flow(app_event);
            break;
        }
        case 2:{
        	create_rendering(app_event);
            break;
        }
        case 3:{
        	create_image_compression(app_event);
            break;
        }
       	case 4:{
            create_lenet(app_event);
            break;
        }
        case 5:{
        	create_digitrec(app_event);
            break;
        }
        case 6:{
        	create_image_scaling(app_event);
            break;
        }
        default:
        	return;
    }

        est_runtime_prrs(app_event);
        pending_apps.push_back(app_event);
        total_apps.push_back(app_event);

        #ifdef DEBUG
        //print_new_app(app_event);
        #endif
        return;

}

#endif


#ifdef SINGLE
std::chrono::high_resolution_clock::time_point generate_event(std::chrono::high_resolution_clock::time_point prev_new_app, std::list<Application*> &pending_apps,
        std::list<Application*> &total_apps, int id, int batch, uint32_t MAX_APPS, double interval)

#elif defined(QUERY)
std::chrono::high_resolution_clock::time_point generate_event(std::chrono::high_resolution_clock::time_point prev_new_app, std::list<Application*> &pending_apps,
        std::list<Application*> &total_apps, int query, uint32_t MAX_APPS, double interval)

#endif
{

        std::chrono::high_resolution_clock::time_point curr_time = timestamp();

        if(interval==0.0f && total_apps.size()<MAX_APPS){
                for(uint32_t iter=0; iter<MAX_APPS; iter++)
                        #ifdef SINGLE
                        add_new_app(pending_apps, total_apps, id, batch);
                        #elif defined(QUERY)
                        add_new_app(pending_apps, total_apps, query);
                        #endif
                        return curr_time;
        }
        else{
                double curr_dur = duration(prev_new_app, curr_time);
                if(interval<=curr_dur && total_apps.size()<MAX_APPS){
                        #ifdef SINGLE
                        add_new_app(pending_apps, total_apps, id, batch);
                        #elif defined(QUERY)
                        add_new_app(pending_apps, total_apps, query);
                        #endif
                        return curr_time;

                }

        }

        return prev_new_app;

}
