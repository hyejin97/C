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

string to_String(int k){
	switch(k){
		case 0:
			return "CREATED";
		case 1:
			return "READY";
		case 2:
			return "RUNNING";
		case 3:
			return "BLOCK";
		case 4:
			return "PREEMPT";		

	}
}

class Process {

public:
	int PID;
	int AT; //arrival time
	int TC; //total cpu time
	int CB; //cpu bursst
	int IO; //io burst
	
	int state_TS; //time stamp at the last state
	int remain_TC; //remaining time to finish
	int remain_CB; //remaining cpu burst

	int FT; //Finishing time
	int TT; //turnaround time
	int IT; //I/Otime
	int PRIO; //static priority
	int CW; //cpu waiting time

	int state;
	Process(int pid, int at, int tc, int cb, int io){
		PID = pid;
		AT = at;
		TC = tc;
		CB = cb;
		IO = io;

		state_TS = 0;
		remain_TC = 0;
		remain_CB = 0;
		IT = 0;
		CW = 0;
		PRIO = 1;
	}
};


vector<Process*> processes;

class Scheduler{

public:
	vector<Process*> ReadyQueue;
	virtual void add_process(Process* p){
		return;	
	}
	virtual Process* get_next_process(){
		Process* next = nullptr;
		return next;
	}
	Scheduler(){}
	~Scheduler(){
		for(int i = 0; i < ReadyQueue.size(); i++){
		 	Process* p = ReadyQueue[ReadyQueue.size() - 1];	
			ReadyQueue.pop_back();
			delete p;
		}
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

	EVENT(Process* p, int ts, int trans, int old_s, int new_s){
		process = p;
		timestamp = ts;
		transition = trans;
		oldstate = old_s;
		newstate = new_s;
	}

	EVENT(Process* p, int ts, int trans, int new_s){
		process = p;
                timestamp = ts;
                transition = trans;
		
                newstate = new_s;
	}
	
};



class Management{
//Generic Interface allows for plugging different schedulers
public:
	int ofs;
	vector<int> randvals;

	Scheduler* sched;
	vector<EVENT*> EventQueue;

	Management(char schedspec, int num, int maxprio){

		switch(schedspec){
			case 'F':
				sched = new FCFS();	
				break;
			case 'L':
				sched = new LCFS();
				break;
			case 'S':
				sched = new SRTF();
				break;
			case 'R':
				sched = new RR();
				break;
			case 'P':
				sched = new PRIO();
				break;
			case 'E':
				sched = new PREPRIO();
				break;
		}

		ofs = 0;

	}
	//get event from the event queue
	EVENT* get_event(){
		
		EVENT* next = nullptr;
		if(!EventQueue.empty()){
			next = EventQueue.front();
			EventQueue.erase(EventQueue.begin());
		}
		return next;
		 
	};	

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

class Simulation{

public:
	Management* manage;
	Simulation(Management* instance){
		manage = instance;
			
	}

	void Run(){
		EVENT* evt;
		int time = manage->EventQueue[0]->timestamp; //global time	
 		while(evt = manage->get_event()){
 			Process *proc = evt->process; // this is the process the event works on
 			int cur_time = evt->timestamp;
			int timeInPrevState = cur_time - proc->state_TS;
 			
			bool CALL_SCHEDULER = false;
			switch(evt->transition) { // which state to transition to?
 				case TRANS_TO_READY:{ 
 				// must come from BLOCKED or from PREEMPTION
 				// must add_process to run queue
 					CALL_SCHEDULER = true; // conditional on whether something is run
					EVENT* newevt = new EVENT(proc, proc->AT, TRANS_TO_RUN, evt->newstate, STATE_READY);
					manage->put_event(newevt);
					manage->sched->add_process(proc);
					proc->state_TS = cur_time;

					cout << cur_time << " " << proc->PID << " " << timeInPrevState << ": " << to_String(newevt->oldstate) << "->" << to_String(newevt->newstate) << endl;
					break;
				}
 				case TRANS_TO_RUN:{
 				// create event for either preemption or blocking
 					if(cur_time < time){ //process is running
						evt->timestamp = time;
						manage->put_event(evt);		
						continue;
					}
					runproc = sched->get_next_process();	
					//update cpu wait time
					runproc->CW += timeInPrevState; 
					//randomly pick CB (1 ~ CB)
					runproc->remain_CB = manage->myrandom(runproc->CB);
					if(runproc->remain_TC <= runproc->remain_CB){
						runproc->remain_CB -= runproc->remain_TC;
						//current process is finished
						time = cur_time + runproc->remain_TC;
						runproc->FT = time;
						runproc->remain_TC = 0;
						runproc->state_TS = cur_time;
					}
					else{
						//create event to be blocked
						EVENT* newevt = new EVENT(runproc, cur_time + runproc->remain_CB, TRANS_TO_BLOCK, evt->newstate, STATE_RUNNING);
						manage->put_event(newevt);
						time = cur_time + proc->remain_CB;
						runproc->remain_TC -= runproc->remain_CB;
						runproc->remain_CB = 0;
						runproc->state_TS = cur_time;
						
						cout << cur_time << " " << proc->PID << " " << timeInPrevState << ": " << to_String(newevt->oldstate) << "->" << to_String(newevt->newstate) << endl;
					}	
 					break;
				}
 				case TRANS_TO_BLOCK:{
 				//create an event for when process becomes READY again
 					CALL_SCHEDULER = true;
					int IOtime = manage->myrandom(proc->IO); 
					proc->IT += IOtime;
					EVENT* newevt = new EVENT(proc, cur_time+IOtime, TRANS_TO_READY, evt->newstate, STATE_BLOCKED);
					manage->put_event(newevt);	
				        manage->sched->add_process(proc);	
					proc->state_TS = cur_time;

					cout << cur_time << " " << proc->PID << " " << timeInPrevState << ": " << to_String(newevt->oldstate) << "->" << to_String(newevt->newstate) << endl;	
					break;
				}
 				case TRANS_TO_PREEMPT:{
				 // add to runqueue (no event is generated)
					CALL_SCHEDULER = true;
					manage->put_event(evt);
					manage->sched->add_process(proc);	
					proc->state_TS = cur_time;
					break;
				}
	 		}
 			// remove current event object from Memory
 			delete evt; 
			evt = nullptr;

/**
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
**/
		} 
	}
};


int main(int argc, char* argv[]) {
	int vflag = 0; //verbose
	int tflag = 0; 
	int eflag = 0;
	char *schedarg = NULL;
	
	int tok;
	while((tok = getopt(argc, argv, "vtes:")) != -1){
		switch(tok){
			case 'v':
				vflag = 1;
				break;
			case 't':
				tflag = 1;
				break;
			case 'e':
				eflag = 1;
				break;
			case 's':
				schedarg = optarg;
				break;
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
	
	//parse scheduler specification
	char schedspec;
	int num = -1;
        int maxprio = -1;
	char* ttok;
	int parsed[3];
	int pidx = 0;

	ttok = strtok(schedarg, "FLSRPE:");
        while(ttok != NULL){
                parsed[pidx] = atoi(ttok);
                ttok = strtok(NULL, ":");
                pidx++;
        }
	schedspec = schedarg[0];
	
	switch(schedspec){
		case 'R':

		case 'P':
			num = parsed[0];
                        maxprio = parsed[1];	
		case 'E':	
                        num = parsed[0];
                        maxprio = parsed[1];

	}
	
	//read input file
	int process_info[4]; //AC TC CB IO
	char* buffer = NULL;
	char* inputline = NULL;
	size_t len = 0;
	int pid = 0;

	while (getline(&buffer, &len, fp_input) != -1) {
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
	
	Management* instance = new Management(schedspec, num, maxprio);

	for(int i = 0; i < processes.size(); i++){
		EVENT* e = new EVENT(processes[i], processes[i]->AT, TRANS_TO_READY, STATE_CREATED);
		instance->put_event(e);
	}

	getline(&buffer, &len, fp_rand);
	int num_rand = atoi(buffer);
	for(int i = 0; i < num_rand; i++){
		getline(&buffer, &len, fp_rand);
		instance->randvals.push_back(atoi(buffer));
	} 

	//run simluation
	Simulation* sim = new Simulation(instance);
	sim->Run();

	delete instance;
	delete sim;

}
