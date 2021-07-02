#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <bits/stdc++.h>
#include <ctype.h>

using namespace std;

typedef enum {TRANS_TO_CREATE, TRANS_TO_READY, TRANS_TO_RUN, TRANS_TO_BLOCK, TRANS_TO_PREEMPT} event__trans_t;
typedef enum {STATE_CREATED, STATE_READY, STATE_RUNNING , STATE_BLOCKED} process_state_t;

class Process {

public:
	int PID;
	int AT; //arrival time
	int TC; //total cpu time
	int CB; //cpu bursst
	int IO; //io burst
	
	Process(int pid, int at, int tc, int cb, int io){
		PID = pid;
		AT = at;
		TC = tc;
		CB = cb;
		IO = io;
	}
};


vector<Process*> processes;

class Scheduler{

public:
	vector<Process*> ReadyQueue;
	virtual void add_process(Process* p);
	virtual Process* get_next_process();
	Scheduler(){

	}
};
//first come first serve
class FCFS: public Scheduler{

public:
	void add_process(Process* p){
		ReadyQueue.push_back(p);
	}

	Process* get_next_process(){
		Process* next = nullptr;
		if(!ReadyQueue.empty()){
			next = ReadyQueue.front(); 
			ReadyQueue.erase(ReadyQueue.begin());
		}	
		return next;
	}
};

//last come first serve
class LCFS: public Scheduler{

public:
	void add_process(Process* p){
		ReadyQueue.insert(ReadyQueue.begin(), p);
	}

	Process* get_next_process(){
		Process* next = nullptr;
                if(!ReadyQueue.empty()){
			next = ReadyQueue.front();
                        ReadyQueue.erase(ReadyQueue.begin());	
		}	
	}
};

class SRTF: public Scheduler{


};

class RR: public Scheduler{

};

class PRIO: public Scheduler{

};

class PREPRIO: public Scheduler{

};

class EVENT{

public:
	Process* process;
	int timestamp;
	int transition;
	int oldstate;
	int newstate;	
	EVENT(Process* p, int ts, int new_s){
		process = p;
		timestamp = ts;
		newstate = new_s;
	}

	void setTransition(int trans, int old_s, int new_s){
		transition = trans;
		oldstate = old_s;
		newstate = new_s;
	}
	
};



class Management{
//Generic Interface allows for plugging different schedulers
private:
	int ofs;
	vector<int> randvals;
public:
	Scheduler* sched;
	vector<EVENT*> EventQueue;

	Management(char schedspec, int num, int maxprio){

		switch(schedspec){
			case 'F':
				sched = new FCFS();	
			case 'L':
				sched = new LCFS();
			case 'S':
				sched = new SRTF();
			case 'R':
				sched = new RR();
			case 'P':
				sched = new PRIO();
			case 'E':
				sched = new PREPRIO();
		}

		ofs = 0;

	}
	//get event from the event queue
	EVENT* get_event();	

	//put event to the event queue in a chronical order
	void put_event(EVENT *e){
		int sz = EventQueue.size(); 
		for(int i = 0; i < EventQueue.size(); i ++){
			if(e->timestamp < EventQueue[i]->timestamp){
				EventQueue.insert(EventQueue.begin() + i, e);
				break;
			}
		}
		if(sz == EventQueue.size()) EventQueue.push_back(e);
	}
	
	void rm_event();
	
	int myrandom(int burst){
		int randnum = 1 + (randvals[ofs] % burst);
		
		if(ofs + 1 == randvals.size()) ofs = 0;
		return randnum;
	}

};
/**
void Simulation() {
	EVENT* evt;
 	while( (evt = get_event()) ) {
 		Process *proc = evt->evtProcess; // this is the process the event works on
 		CURRENT_TIME = evt->evtTimeStamp;
 		timeInPrevState = CURRENT_TIME – proc->state_ts;
 		switch(evt->transition) { // which state to transition to?
 			case TRANS_TO_READY: 
 			// must come from BLOCKED or from PREEMPTION
 			// must add to run queue
 				CALL_SCHEDULER = true; // conditional on whether something is run
 				break;
 			case TRANS_TO_RUN: 
 			// create event for either preemption or blocking
 				break;
 			case TRANS_TO_BLOCK: 
 			//create an event for when process becomes READY again
 				CALL_SCHEDULER = true;
				break;
 			case TRANS_TO_PREEMPT: 
			 // add to runqueue (no event is generated)
				CALL_SCHEDULER = true;
				break;
 		}
 		// remove current event object from Memory
 		delete evt; 
		evt = nullptr;
 		if(CALL_SCHEDULER) {
 			if (get_next_event_time() == CURRENT_TIME)
 				continue; //process next event from Event queue
 			CALL_SCHEDULER = false; // reset global flag
			if (CURRENT_RUNNING_PROCESS == nullptr) {
 				CURRENT_RUNNING_PROCESS = THE_SCHEDULER->get_next_process();
 				if (CURRENT_RUNNING_PROCESS == nullptr) 
 					continue;
 			// create event to make this process runnable for same time.
			} 
		} 
	} 
}

**/
int main(int argc, char* argv[]) {
	int vflag = 0; //verbose
	int fflag = 0; 
	int eflag = 0;
	char *schedspec = NULL;
        int num;
        int maxprio;
	
	int tok;
	while((tok = getopt(argc, argv, "vfes:")) != -1){
		switch(tok){
			case 'v':
				vflag = 1;
			case 'f':
				fflag = 1;
			case 'e':
				eflag = 1;
			case 's':
				schedspec = optarg;
				cout << schedspec << endl;
			default:
				abort();
		}
	}

	int argidx = optind;
	FILE* fp_input;
	FILE* fp_rand;
	
	if (argc > argidx) {
		fp_input = fopen(argv[argidx], "r");
		argidx++;
	}
	if (argc > argidx) {
		fp_rand = fopen(argv[argidx], "r");
		argidx++;	
	}

/**	
	//read input file
	int process_info[4]; //AC TC CB IO
	char* buffer = NULL;
	char* inputline = NULL;
	size_t len = 0;
	int pid = 1;

	while (getline(&buffer, &len, fp) != -1) {
		int idx = 0;
		char* token = strtok(buffer, " ");
		while (token != NULL) {
			process_info[idx] = atoi(token);
			token = strtok(NULL, " ");
			idx++;
		}
		//create processes

		Process* p = new Process(pid, process_info[0], process_info[1], process_info[2], process_info[3]);	
		processes.push_back(p);
		pid++;

	}
	
	Management instance;
	//Management instance(schedspec, num, maxprio);
	for(int i = 0; i < processes.size(); i++){
		EVENT* e = new EVENT(processes[i], processes[i]->AT, STATE_CREATED);
		instance.put_event(e);
	}
**/
}
