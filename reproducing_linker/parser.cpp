#include <iostream>
#include <stdio.h>
#include <bits/stdc++.h>
#include <string.h>
#include <ctype.h>
#include <vector>

using namespace std;
/**
void __parseerror(int linenum, int lineoffset, int errcode) {
	static char* errstr[] = {
		"NUM_EXPECTED", // Number expect, anything >= 2^30 is not a number either
		"SYM_EXPECTED", // Symbol Expected
		"ADDR_EXPECTED", // Addressing Expected which is A/E/I/R
		"SYM_TOO_LONG", // Symbol Name is too long
		"TOO_MANY_DEF_IN_MODULE", // > 16
		"TOO_MANY_USE_IN_MODULE", // > 16
		"TOO_MANY_INSTR”, // total num_instr exceeds memory size (512)
	};
	printf("Parse Error line %d offset %d: %s\n", linenum, lineoffset, errstr[errcode]);
}
**/
struct Token {
	char token[16];
	int line;
	int offset;
} token;

struct Symbol {
	char token[16];
	int addr;
} symbol;

vector<Token> tokenlist;
vector<Symbol> symbtable;

//get token, line, lineoffset
void getToken(){
	//getline
	//strtok		
	char tokstr[256];
        int line = 1;
        int offset = 1;
	

        while(cin.getline(tokstr, 256)){
                char *token = strtok(tokstr, " ");
                while(token != NULL){
                        //cout << token << " " << line << "th line " << offset << "\n";
			struct Token t;
			strcpy(t.token, token);
			t.line = line;
			t.offset = offset;
			tokenlist.push_back(t);
                        offset += (int)strlen(token) + 1;
                        token = strtok(NULL, " ");
                }

                line++;
                offset = 1;
        }

}


int readInt(char* tok){
	for(int i = 0; i < strlen(tok); i++){
		if(!isdigit(tok[i])) return NULL;
	}
	return atoi(tok);

}

char* readSym(char* tok){
	if((int)strlen(tok) > 16) return NULL; //len of symb can't be over 16
	
	if(isdigit(tok[0])) return NULL; //symb must start with alpha

	for(int i = 0; i < strlen(tok); i++){
		if(isalpha(tok[i]) || isalnum(tok[i]) || isdigit(tok[i])) continue;
		else return NULL;
	}
	return tok;

}

char* readIEAR(char* tok){
	if(*tok == 'I' || *tok == 'A' || *tok == 'E' || *tok == 'R') return tok;
	else return NULL;
}



void pass1(){
    //while not EOF
    int idx = 0;
    int base = 0; 
    while(idx < tokenlist.size()){
    	//createModule
   	int base_addr = base; 
	int defcount;
	int usecount;
	int instcount;
	if(defcount = readInt(tokenlist[idx].token)){
        	for(int i=0; i < defcount; i++){
			char* sym = readSym(tokenlist[idx + 2*i + 1].token);
			int val = readInt(tokenlist[idx + 2*i + 2].token);
 			struct Symbol symb_el;
			strcpy(symb_el.token, sym);
			symb_el.addr = base + val;
			symbtable.push_back(symb_el);
		}
	}
	idx += defcount * 2 + 1;
	//cout << idx << endl;
	if(usecount = readInt(tokenlist[idx].token)){
		for(int i=0; i < usecount; i++){
			char* sym = readSym(tokenlist[idx + i].token);
		}
	}
	idx += usecount + 1;
	//cout << "use : " << usecount << "idx" << idx << " " << tokenlist[idx].token << endl;
	if(instcount = readInt(tokenlist[idx].token)){
		for(int i=0; i < instcount; i++){
 			char* addressmode = readIEAR(tokenlist[idx + 2*i + 1].token);
			int operand = readInt(tokenlist[idx + 2*i + 2].token);
		//various checks
		//..
		}	
	}
	idx += instcount * 2 + 1;
	base += instcount;
	//cout << "inst : " << instcount << "idx " << idx << " " << tokenlist[idx].token << endl;
    }
}

void pass2(){
    int idx = 0;
    int base = 0;
    int count = 0;
    while(idx < tokenlist.size()){
        //createModule
        int base_addr = base
        int defcount;
        int usecount;
        int instcount;

	if(defcount = readInt(tokenlist[idx].token)){
                for(int i=0; i < defcount; i++){
                        char* sym = readSym(tokenlist[idx + 2*i + 1].token);
                        int val = readInt(tokenlist[idx + 2*i + 2].token);
                }
        }
	idx += defcount * 2 + 1;
	if(usecount = readInt(tokenlist[idx].token)){
                for(int i=0; i < usecount; i++){
                        char* sym = readSym(tokenlist[idx + i].token);
                }
        }
        idx += usecount + 1;
	if(instcount = readInt(tokenlist[idx].token)){
                for(int i=0; i < instcount; i++){
                        char* addressmode = readIEAR(tokenlist[idx + 2*i + 1].token);
                        int operand = readInt(tokenlist[idx + 2*i + 2].token);
                	//various checks
                	if(addressmode == "R"){
				operand += base;
				cout << count << " " << operand << endl;
			}
			else if(addressmode == "E"){
				
			}
			else if(addressmode == "I"){

			}
			else if(addressmode == "A"){
			
			}
                }
	} 
        idx += instcount * 2 + 1;
        base += instcount;
}

int main(){

	getToken();
/**
	for(int i = 0; i < tokenlist.size(); i++){
		cout << i << " " << tokenlist[i].token << " /";
	}	
**/	
	pass1();
	for(int i = 0; i < symbtable.size(); i++){
                cout << symbtable[i].token << "=" << symbtable[i].addr << endl;
        }	
}
