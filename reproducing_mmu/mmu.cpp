#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <bits/stdc++.h>
#include <string>

#define MAX_FRAMES 128
#define MAX_PT 64

using namespace std;

struct VMA{
        int start_vp;
        int end_vp;
        int write_protected;
        int file_mapped;
}vma;

struct PTE{
	unsigned int valid:1;
	unsigned int referenced:1;
	unsigned int modified:1;
	unsigned int writeprotected:1;
	unsigned int pagedout:1;
}pte;

struct FRAME{

}frame;


class Proc{
public:
	vector<VMA> vmas;
	vector<PTE> pagetable; //represents the translations from virtual pages to physical frames for that process
};


vector<FRAME> frametable;  //describes the usage of each of its physical frames


class Pager {
public:
	virtual FRAME* select_victim_frame() = 0; // virtual base class
};


int main(int argc, char *argv[]){

	int oflag = 0;
	int num_frames;
	char* algo;
	char* options;

	int tok;
	while((tok = getopt(argc, argv, "fao:")) != -1){
		switch(tok){
			case 'f':
				num_frames = atoi(optarg);
				break;
			case 'a':
				algo = optarg;
				break;
			case 'o':
				options = optarg;
				break;
			default:
				abort();
		}
	}

	int argidx = optind;
	FILE* fp_input;
	FILE* fp_rfile;

	if(argc > argidx){
		fp_input = fopen(argv[argidx], "r");
		argidx++;
	}	
	
	if(argc > argidx){
		fp_rfile = fopen(argv[argidx], "r");
		argidx++;
	}

	int num_proc = -1;

	char* buff = NULL;
	size_t len = 0;

	while(getline(&buff, &len, fp_input) != -1){
		if(buff[0] == '#') continue;

		if(num_proc < 0){
			num_proc = atoi(buff);

			int i = 0;
			while(i < num_proc){
				getline(&buff, &len, fp_input);
				if(buff[0] == '#') continue;

				Proc* p = new Proc();
				int num_vmas = atoi(buff);
				
				int j = 0;
				while(j < num_vmas){
					getline(&buff, &len, fp_input);
					if(buff[0] == '#') continue;	

                                        vector<int> tmp;
					char* tok = strtok(buff, " ");
					while(tok != NULL){
						tmp.push_back(atoi(tok));
						tok = strtok(NULL, " ");		
					}
					VMA vma_el = {tmp[0], tmp[1], tmp[2], tmp[3]};
					p->vmas.push_back(vma_el);
					j++;	
				}
				i++;
			}
		}
		else{

			char* inst;
			int num;

			vector<char*> tmp;
			char* tok = strtok(buff, " ");
			while(tok != NULL){
				tmp.push_back(tok);
				tok = strtok(NULL, " ");
			}
			cout << tmp[0] << " " << tmp[1]	<< endl;
		}
	}


	while (get_next_instruction(&operation, &vpage)) {
 		// handle special case of “c” and “e” instruction
		 // now the real instructions for read and write
		 pte_t *pte = &current_process->page_table[vpage];
		 if ( ! pte->present) {
		 // this in reality generates the page fault exception and now you execute
		 // verify this is actually a valid page in a vma if not raise error and next inst
			 frame_t *newframe = get_frame();
			 //-> figure out if/what to do with old frame if it was mapped
			 // see general outline in MM-slides under Lab3 header and writeup below
			 // see whether and how to bring in the content of the access page.
		}
		 // check write protection
		 // simulate instruction execution by hardware by updating the R/M PTE bits
		 update_pte(read/modify) bits based on operations.
	}	 	

}
