#include "nimblock_sched.h"

#ifdef NIMBLOCK

/*
 * Update priorities for all apps in pending apps queue and candidate apps queue
 * Returns a new threshold based on token counts.
 */

double update_priorities_app(std::list<Application*>& pending_apps, std::list<Application*>& candidate_apps) {

    // Get most up to date time
    std::chrono::high_resolution_clock::time_point cur_time = timestamp();
    double max_tokens = 0;
    // Update tokens in pending_app queue

    for (auto it = pending_apps.begin(); it != pending_apps.end(); ++it) {
        Application* cur_app = *it;
        double current_tokens = cur_app -> priority;
        int priority_class = cur_app->priority_level;

        /**GP REPLACE START */
        // u64 degradation = cur_time - cur_app->arrival_time;
        // // Convert degradation to us
        // double deg = 0.125 * 1.0 * degradation / (COUNTS_PER_SECOND / 1000000); // mporw na diairesw me xrono use duration() timestamp()

        double degradation =duration(cur_app->arrival_time, cur_time);
        double deg = 0.125 * 1.0 * degradation;

        //the app latency is provided hardcoded no need to calculate the latency of the tasks and multiply with the batchsize
        double latency = cur_app->exec_time;

        cur_app->priority = current_tokens + priority_class * deg / (latency);
        if (cur_app->priority > max_tokens) {
            max_tokens = cur_app->priority;
        }
    }

    // Update tokens in candidate apps
    for (auto it = candidate_apps.begin(); it != candidate_apps.end(); ++it) {
        Application* cur_app = *it;
        double current_tokens = cur_app -> priority;
        int priority_class = cur_app->priority_level;

        double degradation =duration(cur_app->arrival_time, cur_time);
        double deg = 0.125 * 1.0 * degradation;

        //the app latency is provided hardcoded no need to calculate the latency of the tasks and multiply with the batchsize
        double latency = cur_app->exec_time;

        cur_app->priority = current_tokens + priority_class * deg / (latency);
        if (cur_app->priority > max_tokens) {
            max_tokens = cur_app->priority;
        }
    }
    // TODO maybe add an extra priority level for old high priority applications
    // Give tokens to currently running tasks as well
    if (max_tokens > 9.0){
        return 9.0;
    }
    else if (max_tokens > 3.0){
        return 3.0;
    }
    else{
        return 1.0;
    }
}

/* 
 * Define how many slots this application should get
 * This should quantify the benefit from having many slots
 */
int calculate_goal(Application* my_app){
        // return 10; // This performs the best so far
    int slot_goal = 0;
    slot_goal += my_app->parallelism;
    return slot_goal;
}

/*
 * Reset thresholds for resource usage based on current candidate apps
 */

void allocate_resources(std::list<Application*>& candidate_apps) {

    int assigned_slots = 0;
    // first pass gives away lowest number of slots possible
    for (auto it = candidate_apps.begin(); it != candidate_apps.end(); ++it) {
        Application* my_app = *it;
        int tasks_left = my_app->tasks.size() - my_app->tasks_completed;
        int slots = std::min(THRESHOLD_LEVEL, tasks_left);
        my_app->threshold = slots;
        assigned_slots += slots;
        #ifdef DEBUG
                //std::cout << "[DEBUG]: First Pass threshold " << my_app->threshold << std::endl;
        #endif
    }

    // second pass updates to goal slots
    if (assigned_slots < RECONFIG_REGIONS) { // antikatestisa to NUM_SLOTS_ACTIVE
        // Assign extra slots as needed
        for (auto it = candidate_apps.begin(); it != candidate_apps.end(); ++it) {
                        Application* my_app = *it;
            int tasks_left = my_app->tasks.size() - my_app->tasks_completed;
            int my_goal = calculate_goal(my_app);
            int goal_slots = std::min(my_goal, tasks_left);
            int slot_diff = goal_slots - my_app->threshold;
            my_app->threshold += slot_diff;
            assigned_slots += slot_diff;
            // Do I need to break here?
            #ifdef DEBUG
                        //std::cout << "[DEBUG]: Second Pass threshold " << my_app->threshold << std::endl;
                #endif
        }
    }

    // TODO Deadline aware second pass? Closest app to deadline gets more resources

    // final pass allocates any remaining slots by normalizing and proportioning?
    if (assigned_slots < RECONFIG_REGIONS) {
        // Assign extra slots as needed
        for (auto it = candidate_apps.begin(); it != candidate_apps.end(); ++it) {
            Application* my_app = *it;
            int tasks_left = my_app->tasks.size() - my_app->tasks_completed;
            int slot_diff = tasks_left - my_app->threshold;
            if (slot_diff > 0) {
                // extra slots are available so lets try to use them!
                my_app->threshold += slot_diff;
                assigned_slots += slot_diff;
                // Do I need to break here?
                #ifdef DEBUG
                                //std::cout << "[DEBUG]: Final Pass threshold " << my_app->threshold << std::endl;
                        #endif
            }
        }
    }
}

/*
 * Moves apps from pending_apps to candidate apps when their priority is above the threshold
 * Divide resources amongst candidate apps
 */
bool update_candidate_apps(std::list<Application*>& pending_apps, std::list<Application*>& candidate_apps, double threshold) {

    bool rethresh = false;
    for (auto it = pending_apps.begin(); it!=pending_apps.end();) {
        Application* cur_app = *it;
        if (cur_app->priority >= threshold){
            it = pending_apps.erase(it);
            candidate_apps.push_back(cur_app);
            rethresh = true;
            #ifdef DEBUG
                        std::cout << "[DEBUG]: Moving " << cur_app->id <<" to candidate apps" << std::endl;
                #endif
        }
        else {
            ++it;
        }
    }

    // Move apps that are no longer above the threshold need to be moved out
    // This only happens if a new high priority application arrives and suddenly
    // Shifts the threshold up. Only could affect new, low priority apps
    for (auto it = candidate_apps.begin(); it!=candidate_apps.end();) {

        Application* cur_app = *it;
        if (cur_app->priority < threshold){

            // Remove from list and move to other
            it = candidate_apps.erase(it);
            // Reset threshold for app, its no longer a candidate
            cur_app -> threshold =  0; //-1*NUM_SLOTS_ACTIVE; // By using a large negative number, this one is always the most overutilized
            pending_apps.push_back(cur_app);
            #ifdef DEBUG
                        std::cout << "[DEBUG]: Moving " << cur_app->id <<" to pending apps" << std::endl;
                #endif
            rethresh = true;
        }
        else {
            ++it;
        }
    }
    return rethresh;
}

PRRegion* select_prr_nimblock(std::vector<PRRegion*>& regions){

    for (auto it = regions.begin(); it != regions.end(); ++it) {

        PRRegion* slot = *it;
        if (slot -> status == PR_IDLE) {
            // Select This slot
            #ifdef DEBUG
                        //std::cout << "[DEBUG]: PR region " << slot->id <<" is selected using NIMBLOCK" << std::endl;
                        #endif
            return slot;
        }
    }
    /* No slots are available */
    return NULL;
}

bool is_ready(Task* task){
    Application* parent_app = task->app_id;
    int num_deps = parent_app->num_deps[task->id];
    int num_dep_rev = parent_app->num_deps_reverse[task->id];


    int flag=0;
    for(uint32_t iter=0; iter<num_deps; iter++){
        if(parent_app->tasks[parent_app->dep_list[task->id][iter]]->state != TASK_PENDING)
            flag++;
    }
    if(flag == num_deps)
        return true;
    else
        return false;
}

bool available_tasks(Application* my_app){

    for (auto it = my_app->tasks.begin(); it != my_app->tasks.end(); ++it) {
        Task* my_task = *it;

        if (is_ready(my_task) && (my_task->state == TASK_PENDING)) {
            return true;
        }
    }
    return false;
}

/* 
 * Select which app to give resources to from candidate_apps
 */
Application* select_app(std::list<Application*> candidate_apps) {
    Application* candidate = NULL;

    for (auto it = candidate_apps.begin(); it != candidate_apps.end(); ++it) {

        Application* my_app = *it;

        if (my_app->occupancy < my_app->threshold && available_tasks(my_app)) {
            if (candidate == NULL) {
                candidate = my_app;
                return candidate;
            } else if (my_app -> priority_level > candidate-> priority_level) {
                candidate = my_app;
            }
        }
    }
    return candidate;
}

/*
 * Select task from candidate app
 */
Task* select_task_nimblock(std::list<Application*>& candidate_apps, PRRegion *curr_region){

    if(curr_region == NULL)
        return NULL;

    Application* target_app = select_app(candidate_apps);

    if (target_app == NULL)
        return NULL;
    
    Task* my_task;

    for (auto it = target_app->tasks.begin(); it!=target_app->tasks.end(); ++it) {
        my_task = *it;
        
        if (is_ready(my_task) && (my_task->state == TASK_PENDING)) {

            Application *curr_app = my_task->app_id;
            int curr_dep = curr_app->num_deps[my_task->id];
            my_task->src = curr_region->src_addr;

            for(int iter=0; iter<curr_dep; iter++){

                if(curr_app->id == 1){
                    int tmp_ = curr_app->tasks[curr_app->dep_list[my_task->id][iter]]->task_rdy;
                    curr_app->tasks[curr_app->dep_list[my_task->id][iter]]->dest[iter+tmp_] = curr_region->dest_addr[iter+tmp_];
                    curr_app->tasks[curr_app->dep_list[my_task->id][iter]]->task_rdy +=1;
                }
                else{
                    int tmp_ = curr_app->tasks[curr_app->dep_list[my_task->id][iter]]->task_rdy;
                    curr_app->tasks[curr_app->dep_list[my_task->id][iter]]->dest[tmp_] = curr_region->dest_addr[tmp_];
                    curr_app->tasks[curr_app->dep_list[my_task->id][iter]]->task_rdy +=1;
                }
            }

            #ifdef DEBUG
            //std::cout << "[DEBUG]: Task " << my_task->name << " is selected from " << my_task->app_id->app_name << std::endl;
            #endif

            return my_task;
        }
    }
    return NULL;
}

void launch_nimblock(std::vector<PRRegion*> &regions, PRRegion* sel_prr){

    int sel = -1;
    if (sel_prr!=NULL)
        sel = sel_prr->id;

    for(PRRegion* prr : regions){
        if(prr->status == PR_CONFIG && prr->id!=sel){
            Task* curr_task = prr->curr_task;
            Application* curr_app = curr_task->app_id;
            int curr_dep_rev = curr_app->num_deps_reverse[curr_task->id];
            if(curr_dep_rev==curr_task->task_rdy){
                launch_task(prr);
            }
        }
    }
}

#endif





