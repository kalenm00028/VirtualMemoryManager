#include <iostream>
#include <string>
#include <queue>
#include <list>
#include <vector>

//define process states
#define PROCESS_ARRIVAL 1
#define PROCESS_HOLD 2
#define PAGE_FRAME_WAIT 3
#define READY_TO_RUN 4
#define RUNNING 5
#define IOWAIT 6
#define DONE 7

//#define PHYSICAL_MEM_MAX 8
//#define VIRTUAL_MEM_MAX 16

using namespace std;

const int PHYSICAL_MEM_MAX = 8;
const int VIRTUAL_MEM_MAX = 16;
int totalRunTime = 0;
int initialJobNumber = 1;
int currentFrame = 0;
int vmIterator = 0;
bool done = false;

class job {

    public:
        int jobNum;
        int arrTime;
        int cpuTime;
        int mem;
        int state;
        job(int cpu, int memory) {
            jobNum = initialJobNumber;
            arrTime = totalRunTime;
            cpuTime = cpu;
            mem = memory;
        }

};

/*if job1 is greater it returns true
struct time_comparator {
        bool operator()(job job1, job job2) {
            return job1.arrTime > job2.arrTime;
        }
    };
*/
list<job> processHold;


class memoryManager {

    public:

        //struct that holds a page number and
        //frame number, so the MM can keep track
        //of pages and frames. Our page table
        //will be a list of page_frame structs
        //maybe not struct page_frame

        //frame structure just contains the page number
        //it holds
        struct frame {
            int containedJob;
            //int pageNum;
        };

        //page struct just contains the frame number it
        //occupies
        struct page {
            int containedJob;
            //int frameNum;
        };

        frame frameContainer[PHYSICAL_MEM_MAX];
        page pageContainer[VIRTUAL_MEM_MAX];

        void initMem() {
            for (int i=0; i<VIRTUAL_MEM_MAX; i++) {
                pageContainer[i].containedJob = 0;
            }
            for (int j=0; j<PHYSICAL_MEM_MAX; j++) {
                frameContainer[j].containedJob = 0;
            }
        }

        int initJobMem(job initJob) {
            for (int i=0; i<initJob.mem; i++) {
                pageContainer[i].containedJob = initialJobNumber;   //Assigning this job to virtual memory
            }
            for (int j=0; j<PHYSICAL_MEM_MAX; j++) {
                if (frameContainer[j].containedJob == 0) {
                    frameContainer[j].containedJob = initialJobNumber;
                    return 0;
                }
            }
            return 1;
        }

        void printMemReport() {
            cout << "Page Report:" << endl;
            for (int i=0; i<VIRTUAL_MEM_MAX; i++) {
                cout << "\tPage " << i << ":" << pageContainer[i].containedJob << endl;
            }
            cout << "Frame Report:" << endl;
            for (int j=0; j<PHYSICAL_MEM_MAX; j++) {
                cout << "\tFrame " << j << ":" << frameContainer[j].containedJob << endl;
            }
        }

        void get_job(){
            int cpuTime;
            int mem;

            cout << "Input CPU time required: " << endl;
            cin >> cpuTime;
            cout << "Input memory size of job required: " << endl;
            cin >> mem;

            job job(cpuTime, mem);

            job.state = PROCESS_ARRIVAL;

            if (job.mem <= VIRTUAL_MEM_MAX) {
                processHold.push_front(job);
                if (initJobMem(job) == 0) {
                    printMemReport();
                }
                else {
                    cout << "All pages are full.  Sorry Sherri, try again later." << endl;
                }
            }
            else {
                cout << "Not enough virtual memory" << endl;
            }

            initialJobNumber++;
        }

        void runJob () {
            cout << "Running Job: " << processHold.front().jobNum << endl;
            cout << "Time left: " << processHold.front().cpuTime << endl;
            if (processHold.front().cpuTime > 0) {
               processHold.front().cpuTime--;
            }
            else {
                cout << processHold.front().jobNum << endl;
                processHold.pop_back();
                //pass back memory
            }
        }

        void memorySwap() {

            std::swap(pageContainer[vmIterator].containedJob, frameContainer[currentFrame].containedJob);
            if(vmIterator == VIRTUAL_MEM_MAX) {
                vmIterator = -1;
            }
            vmIterator++;
        }

        void isPageFault() {
            if (frameContainer[currentFrame].containedJob == 0) {
                cout << "PAGE FAULT :^(" << endl;
                memorySwap();
            }
        }

        void tick() {

            int userInput = 0;

            if (totalRunTime == 0) {
                get_job();
            }

            cout << "Press 1 to continue running, press any other number to quit. " << endl;
            cin >> userInput;
            if (userInput==1){
                isPageFault();
                runJob();
            }
            else {
                cout << "Terminating" << endl;
                done = true;
            }
            currentFrame++;
            totalRunTime++;
        }


};



int main() {


    memoryManager misterManager;

    misterManager.initMem();

    while (!done) {
        misterManager.tick();
    }

    return 0;

}

