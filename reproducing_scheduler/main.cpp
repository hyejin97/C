#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <bits/stdc++.h>
#include <ctype.h>

using namespace std;

typedef enum {TRANS_TO_CREATE, TRANS_TO_READY, TRANS_TO_RUN, TRANS_TO_BLOCK, TRANS_TO_PREEMPT, TRANS_TO_FIN} event__trans_t;
typedef enum {STATE_CREATED, STATE_READY, STATE_RUNNING , STATE_BLOCKED, STATE_PREEMPT,  STATE_DONE} process_state_t;

string to_String(int k){
	switch(k){
		case STATE_CREATED:
			return "CREATED";
		case STATE_READY:
			return "READY";
		case STATE_RUNNING:
			return "RUNNG";
		case STATE_BLOCKED:
			return "BLOCK";
		case STATE_PREEMPT:
			return "PREEMPT";
		case STATE_DONE:
			return "Done";	

	}
}


class Process {

public:
	int PID;
	int AT; //arrival time
	int TC; //total cpu time
	int CB; //cpu bursst
	int IO; //io burst

	int state; // current state	
	int state_TS; //time stamp at the last state
	int remain_TC; //remaining time to finish
	int remain_CB; //remaining cpu burst
	
	int FT; //Finishing time
	int TT; //turnaround time
	int IT; //I/Otime
	int PRIO; //static priority
	int DPRIO; //dynamic priority
	int CW; //cpu waiting time

	Process(int pid, int at, int tc, int cb, int io, int prio){
		PID = pid;
		AT = at;
		TC = tc;
		CB = cb;
		IO = io;
		PRIO = prio;
		DPRIO = PRIO - 1;

		state_TS = 0;
		remain_TC = tc;
		remain_CB = cb;
		IT = 0;
		CW = 0;
	}
};


vector<Process*> processes;

class Scheduler{

public:
	vector<Process*> ReadyQueue;
	int quantum;

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
		ReadyQueue.push_back(p);
	}

	Process* get_next_process(){
		Process* next = nullptr;
                if(!ReadyQueue.empty()){
			next = ReadyQueue[ReadyQueue.size() - 1];
                        ReadyQueue.pop_back();	
		}
		return next;	
	}
};

class SRTF: public Scheduler{
public:
	
	void add_process(Process* p){
		int sz = ReadyQueue.size();
		for(int i = 0; i < ReadyQueue.size(); i++){
			if(p->remain_TC < ReadyQueue[i]->remain_TC){
				ReadyQueue.insert(ReadyQueue.begin() + i, p);
				break;
			}	
		}	
		if(sz == ReadyQueue.size()) ReadyQueue.push_back(p);
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

class RR: public Scheduler{
public:
	RR(int n){
		quantum = n;
	}
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

class PRIO: public Scheduler{
public:
	vector<Process*> activeProcess;
	vector<Process*> expiredProcess;

	PRIO(int n){
		quantum = n;
	}

	void add_process(Process* p){
		if(p->DPRIO == -1){ //when dynamic priority reaches -1, add to the expired
			p->DPRIO = p->PRIO - 1;
			expiredProcess.push_back(p);
		}
		else{//add to active
                        activeProcess.push_back(p);
		}
	}

	Process* get_next_process(){
		if(activeProcess.empty()){//if active process queue is empty, swap with expired process queue
			vector<Process*> temp;
			temp = activeProcess;
			activeProcess = expiredProcess;
			expiredProcess = temp;
		}

		Process* next = activeProcess.front();
		int max_DPRIO = activeProcess.front()->DPRIO;
                int max_idx = 0;
		for(int i = 0; i < activeProcess.size(); i++){
			if(activeProcess[i]->DPRIO > max_DPRIO){
				max_DPRIO = activeProcess[i]->DPRIO; 	
				next = activeProcess[i];
				max_idx = i;
			}
		}	
    		activeProcess.erase(activeProcess.begin() + max_idx);
                return next;
		
	}

};

class PREPRIO: public Scheduler{
public:
	vector<Process*> activeProcess;
        vector<Process*> expiredProcess;

        PREPRIO(int n){
                quantum = n;
        }

        void add_process(Process* p){
                if(p->DPRIO == -1){ //when dynamic priority reaches -1, add to the expired
                        p->DPRIO = p->PRIO - 1;
                        expiredProcess.push_back(p);
                }
                else{//add to active
                        activeProcess.push_back(p);
                }
        }

        Process* get_next_process(){
                if(activeProcess.empty()){//if active process queue is empty, swap with expired process queue
                        vector<Process*> temp;
                        temp = activeProcess;
                        activeProcess = expiredProcess;
                        expiredProcess = temp;
                }

                Process* next = activeProcess.front();
                int max_DPRIO = activeProcess.front()->DPRIO;
                int max_idx = 0;
                for(int i = 0; i < activeProcess.size(); i++){
                        if(activeProcess[i]->DPRIO > max_DPRIO){
                                max_DPRIO = activeProcess[i]->DPRIO;
                                next = activeProcess[i];
                                max_idx = i;
                        }
                }
                activeProcess.erase(activeProcess.begin() + max_idx);
                return next;
	}
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

	
};



class Management{
//Generic Interface allows for plugging different schedulers
public:
	int ofs;
	vector<int> randvals;

	Scheduler* sched;
	string schedname;
	vector<EVENT*> EventQueue;

	Management(char schedspec, int num, int maxprio){

		switch(schedspec){
			case 'F':
				sched = new FCFS();	
				schedname = "FCFS";
				break;
			case 'L':
				sched = new LCFS();
				schedname = "LCFS";
				break;
			case 'S':
				sched = new SRTF();
				schedname = "SRTF";
				break;
			case 'R':
				sched = new RR(num);
				schedname = "RR";
				break;
			case 'P':
				sched = new PRIO(num);
				schedname = "PRIO";
				break;
			case 'E':
				sched = new PREPRIO(num);
				schedname = "PREPRIO";
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
	
	void rm_event(int pid){
		for(int i = 0; i < EventQueue.size(); i++){
			if(pid == EventQueue[i]->process->PID){
				EventQueue.erase(EventQueue.begin() + i);	
			} 
		}
	}
	
	int myrandom(int burst){
		int randnum = 1 + (randvals[ofs] % burst);
		if(ofs + 1 == randvals.size()) ofs = 0;
		else ofs += 1;
		return randnum;
	}

	bool isInEventQueue(Process* p, int cur_time){
		for(int i = 0; i < EventQueue.size(); i++){
			if(p->PID == EventQueue[i]->process->PID && EventQueue[i]->timestamp <= cur_time) 
				return true;
		}
		return false;
	}

};

class Simulation{
public:
	Management* manage;
	vector<pair<int, int>> IOrequest;
	int sim_FT;

	Simulation(Management* instance){
		manage = instance;
	}

	void Run(int verbose, int printevents){
		EVENT* evt;
		Process* runningproc;//previously running process
		int time = manage->EventQueue[0]->timestamp; //global time	
 		while((evt = manage->get_event())){
 			Process *proc = evt->process; // this is the process the event works on
 			int cur_time = evt->timestamp;
			int timeInPrevState;			
			bool CALL_SCHEDULER = false;
			if(evt->oldstate != STATE_CREATED) timeInPrevState = cur_time - proc->state_TS; 
			else timeInPrevState = 0;

			switch(evt->transition) { // which state to transition to?
 				case TRANS_TO_READY:{ 
 				// must come from BLOCKED or from PREEMPTION
 				// must add_process to run queue
 					CALL_SCHEDULER = true; // conditional on whether something is run
					//If E sched, preempt currently running proccess
					//if unblocking process’s dynamic priority > running process's dynamic priority AND the currently running process does not have an event pending for the same time stamp
					if(manage->schedname == "PREPRIO"){
						if(proc->DPRIO > runningproc->DPRIO && !(manage->isInEventQueue(runningproc, cur_time))){
							int remain_t = time - cur_time;
							manage->rm_event(runningproc->PID);
							runningproc->remain_TC += remain_t;
							runningproc->remain_CB += remain_t;
                                                        EVENT* newevt = new EVENT(runningproc, cur_time, TRANS_TO_PREEMPT, STATE_RUNNING, STATE_PREEMPT);
                                                        manage->put_event(newevt);
							time = cur_time;
							cout << "preempt " << runningproc->PID << " by " << proc->PID << endl;	
						}
					}

					if(verbose) cout << cur_time << " " << proc->PID << " " << timeInPrevState << ": " << to_String(evt->oldstate) << " -> " << to_String(evt->newstate) << endl;

					EVENT* newevt = new EVENT(proc, cur_time, TRANS_TO_RUN, evt->newstate, STATE_RUNNING);
					manage->put_event(newevt);
					manage->sched->add_process(proc);
					proc->state = STATE_READY;
					proc->state_TS = cur_time;
					break;
				}
 				case TRANS_TO_RUN:{
 				// create event for either preemption or blocking
 				// global time should be updated
 					if(cur_time < time){ //process is running
						evt->timestamp = time;
						manage->put_event(evt);		
						continue;
					}
					
					Process* runproc = manage->sched->get_next_process();
	
					timeInPrevState = cur_time - runproc->state_TS;	
					//update cpu wait time
					runproc->CW += timeInPrevState; 
					
					//Non-preemptive schedulers
					if(manage->schedname == "FCFS" || manage->schedname == "LCFS" || manage->schedname == "SRTF"){	
						//randomly pick CB (1 ~ CB)
						runproc->remain_CB = manage->myrandom(runproc->CB);

						if(verbose) cout << cur_time << " " << runproc->PID << " " << timeInPrevState << ": " << to_String(evt->oldstate) << " -> " << to_String(evt->newstate) << "lr= " << runproc->state_TS << " cb=" << runproc->remain_CB << " rem=" << runproc->remain_TC << " prio=" << runproc->DPRIO << endl;

						if(runproc->remain_TC <= runproc->remain_CB){
							//create event to finish
							EVENT* newevt = new EVENT(runproc, cur_time + runproc->remain_TC, TRANS_TO_FIN, evt->newstate, STATE_DONE);
							manage->put_event(newevt);
							runproc->remain_CB -= runproc->remain_TC;
							time = cur_time + runproc->remain_TC; 
							runproc->remain_TC = 0;
							runproc->state_TS = cur_time;
						}
						else{
							//create event to be blocked
							EVENT* newevt = new EVENT(runproc, cur_time + runproc->remain_CB, TRANS_TO_BLOCK, evt->newstate, STATE_BLOCKED);
							manage->put_event(newevt);
							runproc->remain_TC -= runproc->remain_CB;
							time = cur_time + runproc->remain_CB; 
							runproc->remain_CB = 0;
							runproc->state_TS = cur_time;
						}
					}//Preemptive schedulers
					else if(manage->schedname == "RR" || manage->schedname == "PRIO" || manage->schedname == "PREPRIO"){
						if(runproc->state != STATE_PREEMPT || runproc->remain_CB == 0){
							runproc->remain_CB = manage->myrandom(runproc->CB);
						}

						if(verbose) cout << cur_time << " " << runproc->PID << " " << timeInPrevState << ": " << to_String(evt->oldstate) << " -> " << to_String(evt->newstate) << " cb=" << runproc->remain_CB << " rem=" << runproc->remain_TC << " prio=" << runproc->DPRIO << endl;

						
						if(runproc->remain_CB == manage->sched->quantum){
							EVENT* newevt;
							if(runproc->remain_TC <= runproc->remain_CB){
								newevt = new EVENT(runproc, cur_time + runproc->remain_TC, TRANS_TO_FIN, evt->newstate, STATE_DONE);	
								manage->put_event(newevt);
								time = cur_time + runproc->remain_TC;
								runproc->remain_TC -= runproc->remain_TC;
								runproc->remain_CB -= runproc->remain_TC;
								runproc->state_TS = cur_time; 

							}
							else{
								newevt = new EVENT(runproc, cur_time + manage->sched->quantum, TRANS_TO_BLOCK, evt->newstate, STATE_BLOCKED);
                                        	               	manage->put_event(newevt);
                                        	                time = cur_time + manage->sched->quantum;
                                        	                runproc->remain_TC -= manage->sched->quantum;
                                        	                runproc->remain_CB -= manage->sched->quantum;
                                        	                runproc->state_TS = cur_time;
							}	
						}
	
						else if(runproc->remain_CB > manage->sched->quantum){
							if(runproc->remain_TC > manage->sched->quantum){ //preempt current process
								EVENT* newevt = new EVENT(runproc, cur_time + manage->sched->quantum, TRANS_TO_PREEMPT, evt->newstate, STATE_PREEMPT);
                                                	        manage->put_event(newevt);
                                                        	time = cur_time + manage->sched->quantum;
                                                       		runproc->remain_TC -= manage->sched->quantum;
                                                        	runproc->remain_CB -= manage->sched->quantum;
								runproc->state_TS = cur_time;
							}
							else{ //create event to finish
								EVENT* newevt = new EVENT(runproc, cur_time + runproc->remain_TC, TRANS_TO_FIN, evt->newstate, STATE_DONE);
                                                        	manage->put_event(newevt);
								time = cur_time + runproc->remain_TC; 
                                                        	runproc->remain_TC = 0;
                                                        	runproc->remain_CB -= manage->sched->quantum;
								runproc->state_TS = cur_time;		
							}
						}
						else{ 
							if(runproc->remain_TC <= runproc->remain_CB){
                                                        	//create event to finish
                                                        	EVENT* newevt = new EVENT(runproc, cur_time + runproc->remain_TC, TRANS_TO_FIN, evt->newstate, STATE_DONE);
                                                        	manage->put_event(newevt);
                                                        	runproc->remain_CB -= runproc->remain_TC;
                                                        	time = cur_time + runproc->remain_TC;
                                                       		runproc->remain_TC = 0;
                                                        	runproc->state_TS = cur_time;
                                                	}
                                                	else{
                                                        	//create event to be blocked
                                                       		EVENT* newevt = new EVENT(runproc, cur_time + runproc->remain_CB, TRANS_TO_BLOCK, evt->newstate, STATE_BLOCKED);
                                                        	manage->put_event(newevt);
                                                        	runproc->remain_TC -= runproc->remain_CB;
                                                        	time = cur_time + runproc->remain_CB;
                                                        	runproc->remain_CB = 0;
                                                        	runproc->state_TS = cur_time;
                                                	}
							
						}
					}
					runningproc = proc;
					break;
				}
 				case TRANS_TO_BLOCK:{
 				//create an event for when process becomes READY again
 					CALL_SCHEDULER = true;

					int IOtime = manage->myrandom(proc->IO);

					if(verbose) cout << cur_time << " " << proc->PID << " " << timeInPrevState << ": " << to_String(evt->oldstate) << " -> " << to_String(evt->newstate) << " ib=" << IOtime << " rem=" << proc->remain_TC << " prio=" << proc->DPRIO <<  endl; 

					 //When a process returns from I/O its dynamic priority is reset to (static_priority-1)
					if(manage->schedname == "PRIO" || manage->schedname == "PREPRIO") proc->DPRIO = proc->PRIO - 1;
					
					proc->IT += IOtime;
					IOrequest.push_back({cur_time, cur_time + IOtime});
					EVENT* newevt = new EVENT(proc, cur_time + IOtime, TRANS_TO_READY, STATE_BLOCKED, STATE_READY);
					manage->put_event(newevt);
					proc->state = STATE_BLOCKED;
					proc->state_TS = cur_time;
					

					break;
				}
 				case TRANS_TO_PREEMPT:{
				 // add to runqueue (no event is generated)
				 	CALL_SCHEDULER = true;
				 	if(verbose) cout << cur_time << " " << proc->PID << " " << timeInPrevState << ": " << to_String(evt->oldstate) << " -> " << to_String(evt->newstate) << " cb=" << proc->remain_CB << " rem=" << proc->remain_TC << " prio=" << proc->DPRIO << endl;

               				if(manage->schedname == "PRIO" || manage->schedname == "PREPRIO"){
                                                proc->DPRIO -= 1;
                                        }


	                                EVENT* newevt = new EVENT(proc, cur_time, TRANS_TO_RUN, STATE_PREEMPT, STATE_RUNNING);
                                        manage->put_event(newevt);
                                        manage->sched->add_process(proc);
					proc->state = STATE_PREEMPT;
                                        proc->state_TS = cur_time;
	
						
					break;
				}
				case TRANS_TO_FIN:{
					proc->state = STATE_DONE;
					proc->FT = cur_time;
					proc->TT = proc->FT - proc->AT;	
					sim_FT = cur_time;
	
					if(verbose) cout << cur_time << " " << proc->PID << " " << timeInPrevState << ": " << "Done" << endl;	
					
					break;
				}
	 		}
 			// remove current event object from Memory
 			delete evt; 
			evt = nullptr;

			if(printevents){
				for(int i = 0; i < manage->EventQueue.size(); i++){
					cout << manage->EventQueue[i]->timestamp << ":"  << manage->EventQueue[i]->process->PID << ":" << to_String(manage->EventQueue[i]->newstate) << " rem:" <<  manage->EventQueue[i]->process->remain_TC << " ";
				}
				cout << endl;
			}

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
	int prioflag = 0;
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
        int maxprio = 4;
	char* ttok;
	vector<int> parsed;

	ttok = strtok(schedarg, "FLSRPE:");
        while(ttok != NULL){
                parsed.push_back(atoi(ttok));
                ttok = strtok(NULL, ":");
        }
	schedspec = schedarg[0];
	switch(schedspec){	
		case 'R':{
			num = parsed[0];
			break;
		}
		case 'P':{
			num = parsed[0];
                        if(parsed.size() >= 2) maxprio = parsed[1];	
			break;
		}
		case 'E':{	
                        num = parsed[0];
                        if(parsed.size() >= 2) maxprio = parsed[1];
			break;
		}

	}
	//read input file
	int process_info[4]; //AC TC CB IO
	char* buffer = NULL;
	char* inputline = NULL;
	size_t len = 0;
	int pid = 0;

	//create management layer
	Management* instance = new Management(schedspec, num, maxprio);
	getline(&buffer, &len, fp_rand);
        int num_rand = atoi(buffer);
        for(int i = 0; i < num_rand; i++){
                getline(&buffer, &len, fp_rand);
                instance->randvals.push_back(atoi(buffer));
        }

	while(getline(&buffer, &len, fp_input) != -1){
		int idx = 0;
		char* token = strtok(buffer, " ");
		while (token != NULL) {
			process_info[idx] = atoi(token);
			token = strtok(NULL, " ");
			idx++;
		}
		//create processes
		int static_prio = instance->myrandom(maxprio);
		Process* p = new Process(pid, process_info[0], process_info[1], process_info[2], process_info[3], static_prio);	
		processes.push_back(p);
		pid++;

	}
	//create events
	for(int i = 0; i < processes.size(); i++){
		EVENT* e = new EVENT(processes[i], processes[i]->AT, TRANS_TO_READY, STATE_CREATED, STATE_READY);
		instance->put_event(e);
	}


	//run simluation
	Simulation* sim = new Simulation(instance);
	sim->Run(vflag, eflag);

	//calculate and print result
	if(instance->schedname ==  "FCFS" || instance->schedname == "LCFS" || instance->schedname == "SRTF") cout << instance->schedname << endl;
	else cout << instance->schedname << " " << instance->sched->quantum << endl;
	for(int i = 0; i < processes.size(); i++){
		printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n", processes[i]->PID, processes[i]->AT, processes[i]->TC, processes[i]->CB, processes[i]->IO, processes[i]->PRIO, processes[i]->FT, processes[i]->TT, processes[i]->IT, processes[i]->CW);	

	}
	
	//summary
	//finish time of the last event, cpu util, io util, avg turnaround, avg CW, throughput per 100 time units
	int sim_FT = sim->sim_FT;
	double cpu_util = 0.0;
	double io_util = 0.0;
	double avg_TT = 0.0;
	double avg_CW = 0.0;
	double throughput;

	for(int i = 0; i < processes.size(); i++){
		avg_TT += (double)processes[i]->TT;
		avg_CW += (double)processes[i]->CW;
		cpu_util += (double)processes[i]->TC;
	}

	int last_ft = 0; //last finish time
	sort(sim->IOrequest.begin(), sim->IOrequest.end());
	for(int i = 0; i < sim->IOrequest.size(); i++){
		if(last_ft <= sim->IOrequest[i].first || i == 0){
			io_util += (double)(sim->IOrequest[i].second - sim->IOrequest[i].first);
			last_ft = sim->IOrequest[i].second;
		}
		else if(last_ft <= sim->IOrequest[i].second){
			io_util += (double)(sim->IOrequest[i].second - last_ft);
			last_ft = sim->IOrequest[i].second;
		}
	}
	
	avg_TT /= (double)processes.size();
	avg_CW /= (double)processes.size();
	cpu_util /= (double)sim_FT;
	io_util /= (double)sim_FT;
	throughput = (double)processes.size() * 100 / (double)sim_FT;
	printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",  sim_FT, cpu_util * 100, io_util * 100, avg_TT, avg_CW, throughput);
	
	delete instance;
	delete sim;

}
