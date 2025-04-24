#include "debug.h"
#include <string>

void print_new_app(Application* new_app){

    std::cout << "[DEBUG]: New Application --> " << new_app->app_name <<"\t Batch Size --> " << new_app->max_batch <<"\t App ID --> " << new_app->id << std::endl;

	for(const Task *curr_task : new_app->tasks){

		std::cout << "[DEBUG]: Task --> " << curr_task->name << "\t Dependencies --> ";

		int curr_dep = new_app->num_deps[curr_task->id];
		for(int iter=0; iter<curr_dep; iter++)
			std::cout << "\t" << new_app->tasks[new_app->dep_list[curr_task->id][iter]]->name;
		
		std::cout << std::endl;

		std::cout << "[DEBUG]: Reconfiguration --> " << curr_task->est_reconfig << "\tStart --> " << curr_task->est_start << "\t End --> " << curr_task->est_end << std::endl;

	}
	std::cout << "[DEBUG]: Estimated Runtime --> " << new_app->est_latency << "\t Estimated PRRs --> " << new_app->est_prrs << std::endl;

	std::cout << std::endl;
}

#ifdef SINGLE
void export_metrics(std::list<Application*> &completed_apps, double total_time)
#elif defined(QUERY)
void export_metrics(std::list<Application*> &completed_apps, double total_time, uint32_t seq_num, double interval)
#endif
{

        #ifdef QUERY

        std::ofstream outfile;
        #if defined(NONSHARING)
        std::string file_name = "baseline_nonsharing_" + std::to_string(interval)  + ".csv";
        outfile.open(file_name.c_str(), std::ios_base::app);
        #elif defined(FCFS)
        std::string file_name = "baseline_fcfs_" + std::to_string(interval) + ".csv";
        outfile.open(file_name.c_str(), std::ios_base::app);
        #elif defined(RR)
        std::string file_name = "baseline_rr_" + std::to_string(interval) + ".csv";
        outfile.open(file_name.c_str(), std::ios_base::app);
        #elif defined(SJF)
        std::string file_name = "baseline_sjf_" + std::to_string(interval) + ".csv";
        outfile.open(file_name.c_str(), std::ios_base::app);
        #elif defined(NIMBLOCK)
        std::string file_name = "baseline_nimblock_" + std::to_string(interval) + ".csv";
        outfile.open(file_name.c_str(), std::ios_base::app);
        #endif

	//outfile << "Application Name" << "," << "Batch Size" << "," << "Response Time" << "," << "Wait Time" << "," << "Total Reconfig Time" << "," << "System Throughput"<< std::endl;

        for(Application* app : completed_apps){

                double total_reconfig = 0.000f;
                //double total_exec = 0.000f;
                for(Task* task: app->tasks){
                        total_reconfig += duration(task->reconfig_start, task->reconfig_end);
                }

                outfile << app->app_name << "," << app->max_batch << ",";
                //outfile << duration(app->arrival_time,app->end_time) << "," << duration(app->schedule_time,app->end_time) << ","
                //<< duration(app->arrival_time,app->schedule_time) << ',' << completed_apps.size()/total_time <<", , ,";

                //outfile << duration(app->arrival_time,app->end_time) << "," << duration(app->arrival_time,app->schedule_time) << "," << total_reconfig << "," << completed_apps.size()/total_time << ", , ,";

		outfile << duration(app->arrival_time,app->end_time) << ",";

                //for(Task* curr_task: app->tasks){
                //      outfile << " Task " << curr_task->id << "," << duration(curr_task->schedule_time,app->end_time) << ",";
                //}

                outfile << std::endl;

        }

        outfile.close();

        #endif 

        #ifdef SINGLE

        std::ofstream outfile;
        std::string file_name = "baseline_app.csv";
        outfile.open(file_name.c_str(), std::ios_base::app);

        //outfile << "Application Name" << "," << "Batch Size" << "," << "Response Time" << "," << std::endl;

        for(Application* app : completed_apps){

                outfile << app->app_name << "," << app->max_batch << ",";
                outfile << duration(app->arrival_time,app->end_time) << ",";
                outfile << std::endl;
                /*outfile << "Task Name" << "," << "Execution Time" << "," << "Wait Time" << "," << std::endl;

                double wait_time = 0.000f;
                double exec_time = 0.000f;
                //double total_exec = 0.000f;
                for(Task* task: app->tasks){
                        outfile << task->name << ",";
                        wait_time = task->total_wait_time;
                        exec_time = duration(task->start_time, task->end_time) - wait_time;

                        outfile << exec_time << "," << wait_time;
                        outfile << std::endl;

                }
                
                outfile << std::endl;
                outfile << std::endl;
                outfile << std::endl;*/

        }
        outfile.close();


        #endif 

}

