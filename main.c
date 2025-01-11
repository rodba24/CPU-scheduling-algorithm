#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>


typedef struct
{
    char name[3];
    int priority;
    int burst_time;
    int arrival_time;
    int remaining_time;
    int ct;
    int start_time;
    int is_completed;
    int index;

} Task;

void printAvg(int wt[], int TAT[], int n)
{
    int total_waiting_time = 0, total_turnaround = 0;
    for (int i = 0; i< n; i++)
    {
        total_waiting_time += wt[i];
        total_turnaround += TAT[i];
    }
    printf("\n");
    printf("Average Waiting Time : %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround / n);
}

pthread_mutex_t mutex; // mutex used to ensure only one thread at a time accesses the shared data

int n = 5;
Task tasks[5];

// FCFS
void *fcfs(void *arg)
{
     int ct[n], TAT[n], wt[n]; // ct = completion time
    int total_waiting_time = 0, total_turnaround = 0;

    pthread_mutex_lock(&mutex);

     ct[0] = tasks[0].burst_time;
    // completetion time is burst time assuming they all arrive at 0
    TAT[0] = ct[0];
    // TAT = completion - arrival, so (completion - 0)

     wt[0] = 0; // waiting time for t1 is 0

    // calculations for t2 -> t5
    for (int i = 1; i< n; i++)
    {
    ct[i] =ct[i - 1] +tasks[i].burst_time;
        // burst time + completion of previous
     TAT[i] = ct[i];
     // just completion time because arrival time is 0
        wt[i] = TAT[i] - tasks[i].burst_time;
        // wt = TAT- BT
     }

    // print the Gantt chart and the calculated times
    printf("Scheduling algorithm name: FCFS\n");
      printf("Gantt Chart for fcfs:\n");

    // print each task's time interval
    for (int i = 0; i < n; i++)
    {
          int start_time = (i == 0) ? 0 : ct[i - 1];
        // t1 start time is 0, starttime of t2 is end of t1 and so on

    int end_time = ct[i];
        // completion time is the end time
     printf("%s[%d-%d]", tasks[i].name, start_time, end_time);
        if (i < n - 1)
        {
            printf(", ");
           }
    }
    printf("\n");

    printAvg(wt, TAT, n);

    pthread_mutex_unlock(&mutex); // unlock mutex for the next function

    pthread_exit(NULL);

    } // end of fcfs

// SJF

void *sjf(void *arg)
{
    int ct[n], TAT[n], wt[n];

    pthread_mutex_lock(&mutex); // lock mutex so no other function can use it

    for (int i= 0; i< n; i++)
    {
        tasks[i].index = i; // for sorting
        }

    // sort tasks based on burst time -> lowest bursttime =highest priority
       for (int i =0; i< n - 1; i++)
    {
        for (int j =i + 1; j< n; j++)
        {
            // compare burst time first, if equal, maintain original order
            if (tasks[i].burst_time >tasks[j].burst_time ||
                tasks[i].burst_time ==tasks[j].burst_time && tasks[i].index > tasks[j].index)
            {
                Task temp =tasks[i];
                tasks[i] =tasks[j];
                tasks[j] =temp;
            }
        }
    } // t2, t5, t1, t4, t3
    // [t1 and t4 have same completion time but
    // they should be ordered in terms of which one came first]

      ct[0] =tasks[0].burst_time;
    TAT[0] =ct[0];
    wt[0] =0; // Waiting time for T1 is 0

    // calculations for T2 -> T5
    for (int i =1; i <n; i++)
    {
    ct[i] =ct[i - 1] +tasks[i].burst_time;
        // completion time is the sum of previous task's completion time and the current task's bursttime
    TAT[i] =ct[i];       // tat is completion time cause arrival is 0
    wt[i] =TAT[i] - tasks[i].burst_time; // waiting time = TAT - BT
    }

    // print the Gantt chart
      printf("\nScheduling Algorithm Name: SJF\n");
    printf("Gantt Chart for sjf:\n");
    for (int i = 0; i< n; i++)
    {
        int start_time = (i == 0) ? 0 : ct[i -1];
        // if i ==0 then starttime is 0, if not then start time is the previous completiontime
        int end_time = ct[i];
        printf("%s[%d-%d]", tasks[i].name, start_time, end_time);
        if (i < n - 1)
        {
        printf(", ");
        }
    }
    printf("\n");

    printAvg(wt, TAT, n);

      pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
} // end of sjf

//  Priority scheduling  --->

void *ps(void *arg) {
    int current_time =0, completed = 0;
    //current time tracks where you currently standing on the gantt chart
    pthread_mutex_lock(&mutex);
    int wt[n];
    int TAT[n];
    char gantt_chart[1000] = "";// buffer for Gantt chart
    int gantt_start= -1; //initialize as -1 since no task is running
    char current_task[3];//stores the name of current task
    int idle_start= 0;  // start tracking idle time from 0
    bool is_idle= true; // track if system is currently idle

    
for (int i = 0; i < n; i++) {
        tasks[i].remaining_time =tasks[i].burst_time;//remianing time is = to bursttime
        tasks[i].start_time = -1;//-1 means not started
        tasks[i].is_completed = 0;//mark as not completed
    }

while (completed != n) {
        int highest_priority_index = -1; //index of highest prio task
        int highest_priority = -1; //value of highest prio

        // find highest priority task that has arrived but not completed
    for (int i = 0; i < n; i++) {
            // check if task: 1) has arrived 2) has remaining work 3) isn't completed
        if (tasks[i].arrival_time<= current_time && 
                tasks[i].remaining_time > 0 && 
                tasks[i].is_completed== 0) {
                     // if this task has higher priority than current highest
         if (tasks[i].priority > highest_priority) {
                  highest_priority = tasks[i].priority;
                highest_priority_index = i;
                }
                // if priorities are equal, choose the one that arrived first
        else if (tasks[i].priority == highest_priority) {
                    if (highest_priority_index == -1 || 
                tasks[i].arrival_time < tasks[highest_priority_index].arrival_time) {
                highest_priority_index = i;
                    }
                }
            }
        }
//if no task is ready cpu is idle
    if (highest_priority_index == -1) {

            if (!is_idle) {
                idle_start =current_time; //record start of idletime
                is_idle = true; // mark as idle
                
                // record the previous task in gantt chart if there was one
                if (gantt_start != -1) {
                    char entry[50];
                    sprintf(entry, "%s[%d-%d], ", current_task, gantt_start, current_time);
                    strcat(gantt_chart, entry); //append entry to gantt chart
                    gantt_start = -1;  
                }
            }
            current_time++; //increment time
 } 
 //if a task is ready to run
     else {
            if (is_idle) {
                // system was idle, record idle time
                  if (current_time > idle_start) {
                char entry[50];
                sprintf(entry, "Idle[%d-%d], ", idle_start, current_time);
                strcat(gantt_chart, entry);//append entry to gantt chart
                }
                is_idle = false; //no longer idle
        }

            if (tasks[highest_priority_index].start_time == -1) {
                //if the highest prio task has not still started then set it as the curr time, 
                //that will be the start of that task
                tasks[highest_priority_index].start_time = current_time;
            }

            //gantt chart is -1 if no task is there yet. 
            //  strcmp(current_task, tasks[highest_priority_index].name) != 0 : compares the value of the curr task
            // and high prio task and if they are same it returns 0
            
               if (gantt_start == -1 || strcmp(current_task, tasks[highest_priority_index].name) != 0) {
                if (gantt_start != -1) {
                    char entry[50];
                 sprintf(entry, "%s[%d-%d], ", current_task, gantt_start, current_time);
                    //creates a string called entry
                strcat(gantt_chart, entry);//appends the entry string to the whole ganttchart
                }
              strcpy(current_task, tasks[highest_priority_index].name);
                // copies the name of high prio task in the curr task
                gantt_start = current_time;
                //the next gantchart element will start from
                //where the last task finished.
            }

            tasks[highest_priority_index].remaining_time--;// decrement remaining time of high prio task by 1
            current_time++;// each time cpu does processing curr time is increased by 1

             // if the task is completed
     if (tasks[highest_priority_index].remaining_time == 0) {
                tasks[highest_priority_index].ct = current_time;// completion time is current time
                tasks[highest_priority_index].is_completed = 1;
                completed++;
            }
        }
    }

    // handle final task or idle period
     if (is_idle && current_time > idle_start) {
        char entry[50];
        sprintf(entry, "Idle[%d-%d] ", idle_start, current_time);
        strcat(gantt_chart, entry);
    }
    else if (gantt_start != -1) {
        char entry[50];
        sprintf(entry, "%s[%d-%d] ", current_task, gantt_start, current_time);
        strcat(gantt_chart, entry);
    }

    // print Gantt ch
    printf("\nScheduling Algorithm Name: PS\n");
    printf("Gantt Chart for PS:\n");
    printf("%s\n", gantt_chart);

    // calculate TAT and waiting time
    for (int i =0; i <n; i++) {
        TAT[i] = tasks[i].ct - tasks[i].arrival_time;
        wt[i] = TAT[i] - tasks[i].burst_time;
    }

    //for my own simplicity and to check if the entries are right
    printf("\nTask Details:\n");
    for (int i =0; i <n; i++) {
        printf("%s: Arrival Time = %d, Priority = %d, BT = %d, CT = %d\n",
            tasks[i].name, tasks[i].arrival_time, tasks[i].priority, 
        tasks[i].burst_time, tasks[i].ct);
    }

    // Print avg
    printAvg(wt, TAT, n);

    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

// read tasks from input file
void read_tasks_from_file()
{
    FILE *file = fopen("input.txt", "r"); // open input.txt for reading only
    if (file ==NULL)
    {
        perror("Unable to open file!");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++)
    {
    fscanf(file, "%s %d %d", tasks[i].name, &tasks[i].priority, &tasks[i].burst_time);
        // first col is name, 2nd col is priority 3rd col is bursttime
    tasks[i].arrival_time = rand() % 101; // to get random  arrival time
    }

    fclose(file); // close file
}

int main()
{
    pthread_t fcfs_thread, sjf_thread, ps_thread;

    pthread_mutex_init(&mutex, NULL); // intializing mutex

    srand(time(NULL)); // to get random # each time you run the code

    // read tasks from file
    read_tasks_from_file();

    // create pthreads for each scheduling
    pthread_create(&fcfs_thread, NULL, fcfs, NULL);
    pthread_create(&sjf_thread, NULL, sjf, NULL);
    pthread_create(&ps_thread, NULL, ps, NULL);

    // join all pthreads
    pthread_join(fcfs_thread, NULL);
    pthread_join(sjf_thread, NULL);
    pthread_join(ps_thread, NULL);

    // destroy
    pthread_mutex_destroy(&mutex);

    return 0;
}