#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <bits/stdc++.h>
#include <string>

using namespace std;

struct IOoperation {
	int timestamp;
	int track;
	int start_time;
	int end_time;
};
deque<IOoperation> input_seq;
vector<IOoperation> completed;

class IOscheduler {
public:
	deque<IOoperation> IOqueue;
	//virtual string getSchedname() = 0;
	virtual IOoperation get_next_operation(int curr, bool is_incr_dir) = 0;
	virtual void add_operation(IOoperation n) = 0;

	bool is_max_el(int track){
		for(int i = 0; i < IOqueue.size(); i++){
			if(IOqueue[i].track > track) return true;
		}
		return false;
	}
	
	bool is_min_el(int track){
		for(int i = 0; i < IOqueue.size(); i++){
			if(IOqueue[i].track < track) return true;
		}
		return false;
	}	
};

class FIFO : public IOscheduler {
public:
	IOoperation get_next_operation(int curr, bool is_incr_dir) {
		IOoperation nxt = IOqueue.front();
		IOqueue.pop_front();
		return nxt;
	}
	
	void add_operation(IOoperation n){
		IOqueue.push_back(n);
	}
};


class SSTF : public IOscheduler {
public:
	IOoperation get_next_operation(int curr, bool is_incr_dir) {
		int min_seek = INT_MAX;
		int min_loc;
		for(int i = 0; i < IOqueue.size(); i++) {
			int seekdist = abs(curr - IOqueue[i].track);
			if (seekdist < min_seek) {
				min_seek = seekdist;
				min_loc = i;
			}
		}
		IOoperation nxt = IOqueue[min_loc];
		IOqueue.erase(IOqueue.begin() + min_loc);
		return nxt;
	}

	void add_operation(IOoperation n) {
		IOqueue.push_back(n);
	}
};

class LOOK : public IOscheduler {
public:
	IOoperation get_next_operation(int curr, bool is_incr_dir) {
		int min_loc = -1;
		if (is_incr_dir) {
			//sort by track, increasing order
			for (int i = 0; i < IOqueue.size(); i++) {
				for (int j = i + 1; i < IOqueue.size(); j++) {
					if (IOqueue[i].track > IOqueue[j].track){
						IOoperation tmp = IOqueue[i];
						IOqueue[i] = IOqueue[j];
						IOqueue[j] = tmp;
					}
				}
			}

			for (int i = 0; i < IOqueue.size(); i++) {
				if (IOqueue[i].track >= curr) {
					min_loc = i;
					break;
				}
			}
		}
		else {
			//sort by track, decreasing order
			for (int i = 0; i < IOqueue.size(); i++) {
				for (int j = i + 1; i < IOqueue.size(); j++) {
					if (IOqueue[i].track < IOqueue[j].track) {
						IOoperation tmp = IOqueue[i];
						IOqueue[i] = IOqueue[j];
						IOqueue[j] = tmp;
					}
				}
			}

			for (int i = IOqueue.size() - 1; i >= 0; i--) {
				if (IOqueue[i].track <= curr) {
					min_loc = i;
					break;
				}
			}
		}

		IOoperation nxt = IOqueue[min_loc];
		IOqueue.erase(IOqueue.begin() + min_loc);
		return nxt;
	}

	void add_operation(IOoperation n) {
		IOqueue.push_back(n);
	}
};

class CLOOK : public IOscheduler {
public:
	int min_loc = -1;
	IOoperation get_next_operation(int curr, bool is_incr_dir) {
		for (int i = 0; i < IOqueue.size(); i++) {
			for (int j = i + 1; i < IOqueue.size(); j++) {
				if (IOqueue[i].track > IOqueue[j].track) {
					IOoperation tmp = IOqueue[i];
					IOqueue[i] = IOqueue[j];
					IOqueue[j] = tmp;
				}
			}
		}

		for (int i = 0; i < IOqueue.size(); i++) {
			if (IOqueue[i].track >= curr) {
				min_loc = i;
				break;
			}
		}

		IOoperation nxt = IOqueue[min_loc];
		IOqueue.erase(IOqueue.begin() + min_loc);
		return nxt;
	}

	void add_operation(IOoperation n) {
		IOqueue.push_back(n);
	}
};

class FLOOK : public IOscheduler {
public:
	deque<IOoperation> Activequeue;

	void swapQueue(){
		IOqueue = Activequeue;
		Activequeue.clear();
	}

	IOoperation get_next_operation(int curr, bool is_incr_dir) {
		int min_loc = -1;
		if (is_incr_dir) {
			//sort by track, increasing order
			for (int i = 0; i < IOqueue.size(); i++) {
				for (int j = i + 1; i < IOqueue.size(); j++) {
					if (IOqueue[i].track > IOqueue[j].track) {
						IOoperation tmp = IOqueue[i];
						IOqueue[i] = IOqueue[j];
						IOqueue[j] = tmp;
					}
				}
			}

			for (int i = 0; i < IOqueue.size(); i++) {
				if (IOqueue[i].track >= curr) {
					min_loc = i;
					break;
				}
			}
		}
		else {
			//sort by track, decreasing order
			for (int i = 0; i < IOqueue.size(); i++) {
				for (int j = i + 1; i < IOqueue.size(); j++) {
					if (IOqueue[i].track < IOqueue[j].track) {
						IOoperation tmp = IOqueue[i];
						IOqueue[i] = IOqueue[j];
						IOqueue[j] = tmp;
					}
				}
			}

			for (int i = IOqueue.size() - 1; i >= 0; i--) {
				if (IOqueue[i].track <= curr) {
					min_loc = i;
					break;
				}
			}
		}

		IOoperation nxt = IOqueue[min_loc];
		IOqueue.erase(IOqueue.begin() + min_loc);
		return nxt;
	}

	void add_operation(IOoperation n) {
		IOqueue.push_back(n);
	}
};


unsigned int total_time = 0; //total simulated time, i.e.until the last I/O request has completed.
unsigned int tot_movement = 0; //total number of tracks the head had to be moved
double avg_turnaround = 0; //average turnaround time per operation from time of submission to time of completion
double avg_waittime = 0; //average wait time per operation(time from submission to issue of IO request to start disk operation)
unsigned int max_waittime = 0; //maximum wait time for any IO operation.

void simulate(IOscheduler* sched) {
	int time = 0;
	int head = 0;
	bool is_incr_dir = 1;
	IOoperation active_io = { 0 };
	IOoperation prev_io = { 0 };
	bool isactive = false;
	
	while (true) {
		//if a new IO arrived to the system at this current time
		if (!input_seq.empty() && time == input_seq.front().timestamp){
//			cout << "new input " << time << " " << input_seq.front().track << endl;
			sched->add_operation(input_seq.front()); //add request to IOqueue
			input_seq.pop_front();
		}

		//if an IO is active and completed at this time
		if(isactive && head == active_io.track){
//			cout << "complete " << time << " " << head << endl;

			//Compute relevant info and store in IO request for final summary
			active_io.end_time = time;
			completed.push_back(active_io);
			isactive = false;
			prev_io = active_io;

			if (input_seq.empty()) total_time = time;
		}
		if(!isactive){//if no IO request active now
			//if requests are pending
			if (!sched->IOqueue.empty()){
				//Fetch the next request from IOqueue and start the new IO.
				if (prev_io.track - active_io.track <= 0) is_incr_dir = true;
				else is_incr_dir = false;
				active_io = sched->get_next_operation(head, is_incr_dir);
				active_io.start_time = time;
				isactive = true;
				//update waittime
				int cur_io_waittime = active_io.start_time - active_io.timestamp;
                        	if (cur_io_waittime > max_waittime) max_waittime = cur_io_waittime;	
//				cout << "start new" << time << " " << active_io.track << endl;

			}	
			//else if all IO from input file processed
			else if (input_seq.empty()) {
				return; //exit simulation
			}
		}
		//if an IO is active
		if(isactive){
			//Move the head by one unit in the direction its going(to simulate seek)
			if(active_io.track - head <= 0){ //if reaches largest track in the request
				is_incr_dir = false;	
				head--;
			}
			else{
				is_incr_dir = true;
				head++;
			}
			tot_movement++;
		}
		time++;
	}
}

void print_stats() {
	unsigned int tot_turnaround = 0;
	unsigned int tot_waittime = 0;

	for (int i = 0; i < completed.size(); i++) {
		printf("%5d: %5d %5d %5d\n", i, completed[i].timestamp, completed[i].start_time, completed[i].end_time);
		tot_turnaround += (completed[i].end_time - completed[i].timestamp);
		tot_waittime += (completed[i].start_time - completed[i].timestamp);
	}

	avg_turnaround = (double)tot_turnaround / (double)completed.size();
	avg_waittime = (double)tot_waittime / (double)completed.size();
	printf("SUM: %d %d %.2lf %.2lf %d\n", total_time, tot_movement, avg_turnaround, avg_waittime, max_waittime);
}

int main(int argc, char* argv[]){
	int vflag = 0;
	int qflag = 0;
	int fflag = 0;
	FILE* fp_input = NULL;
	IOscheduler* iosched = NULL;

	char* algo = NULL;
	char* options = NULL;
	int tok;
	while ((tok = getopt(argc, argv, "s:v:q:f:")) != -1) {
		switch (tok) {
			case 's':
				algo = optarg;
				switch (algo[0]) {
					case 'i':
						iosched = new FIFO();
						break;
					case 'j':
						iosched = new SSTF();
						break;
					case 's':
						iosched = new LOOK();
						break;
					case 'c':
						iosched = new CLOOK();
						break;
					case 'f':
						//iosched = new FLOOK();
						break;
				}
			case 'v':
				vflag = 1;
				break;
			case 'q':
				qflag = 1;
				break;
			case 'f':
				fflag = 1;
				break;
		}
	}
	int argidx = optind;
	if (argc > argidx)
		fp_input = fopen(argv[argidx], "r");

	//read file and save inputs
	char* buff = NULL;
	size_t len = 0;
	int timestamp;
	int track;
	while (getline(&buff, &len, fp_input) != -1) {
		if (buff[0] == '#') continue;
		sscanf(buff, "%d %d", &timestamp, &track);
		IOoperation newio = { 0 };
		newio.timestamp = timestamp;
		newio.track = track;
		input_seq.push_back(newio);
	}

	simulate(iosched);
	print_stats();
}
