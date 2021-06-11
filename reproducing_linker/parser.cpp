#include <iostream>
#include <stdio.h>
#include <bits/stdc++.h>
#include <string.h>
#include <ctype.h>
#include <vector>

using namespace std;

void __parseerror(int linenum, int lineoffset, int errcode) {
	static char* errstr[] = {
		"NUM_EXPECTED", // Number expect, anything >= 2^30 is not a number either
		"SYM_EXPECTED", // Symbol Expected
		"ADDR_EXPECTED", // Addressing Expected which is A/E/I/R
		"SYM_TOO_LONG", // Symbol Name is too long
		"TOO_MANY_DEF_IN_MODULE", // > 16
		"TOO_MANY_USE_IN_MODULE", // > 16
		"TOO_MANY_INSTR", // total num_instr exceeds memory size (512)
	};
	cout<< "Parse Error line " << linenum << "offset " << lineoffset << ": " << errstr[errcode] << endl; 
}

struct Token {
	char token[16];
	int line;
	int offset;
} token;

struct Symbol {
	char token[16];
	int addr;
	int modulenum;
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


int readInt(char tok[], int line, int offset){
	for(int i = 0; i < strlen(tok); i++){
		if(!isdigit(tok[i])){
			__parseerror(line, offset, 0);	
			return -1;
		}
	}
	return atoi(tok);

}

int readSym(char tok[], int line, int offset){
//-1 : two many characters, 0 : invalid symbol, 1 : valid symbol
	if((int)strlen(tok) > 16){
		__parseerror(line, offset, 3);	
		return -1; //len of symb can't be over 16
	}
	if(isdigit(tok[0])){
		__parseerror(line, offset, 1);
		return 0; //symb must start with alpha
	}
	for(int i = 0; i < strlen(tok); i++){
		if(isalpha(tok[i]) || isalnum(tok[i]) || isdigit(tok[i])) continue;
		else{
			__parseerror(line, offset, 1);
			return 0;
		}
	}
	return 1;

}

bool readIEAR(char tok[], int line, int offset){
	if(strcmp(tok, "I") == 0 || strcmp(tok, "A") == 0 || strcmp(tok, "E") == 0 || strcmp(tok, "R") == 0) return true;
	else{
		__parseerror(line, offset, 2);	
		return false;
	}
}

Symbol findSymbol(char* target_sym){
	struct Symbol notfound = {NULL, NULL, NULL};
	for(int s = 0; s < symbtable.size(); s++){
		if(strcmp(symbtable[s].token, target_sym) == 0){
			return symbtable[s];
		}
	}
	return notfound;
}

//Parse error checking, create symbol table
void pass1(){
    int idx = 0;
    int base = 0; 
    int modulenum = 1;
    while(idx < tokenlist.size()){
	//createModule
	int defcount;
	int usecount;
	int instcount;
	vector<Symbol> moduledefs; //defined symbs per module

	if(defcount = readInt(tokenlist[idx].token, tokenlist[idx].line, tokenlist[idx].offset)){
		if(defcount > 16){
			__parseerror(tokenlist[idx].line, tokenlist[idx].offset, 4);
			return;	
		}
		for(int i=0; i < defcount; i++){
			char* sym = tokenlist[idx + 2*i + 1].token;
			int val = readInt(tokenlist[idx + 2*i + 2].token, tokenlist[idx + 2*i + 2].line, tokenlist[idx + 2*i + 2].offset);
/**
			//TODO : Syntax error checking : missing token
			if(){
				
				return;
			}
**/			
		//Parse error : unexpected token (symbol expected)
			if(readSym(sym, tokenlist[idx + 2*i + 1].line, tokenlist[idx + 2*i + 1].offset) <= 0) return;

			//Parse error : unexpexted token (integer expected)
			if(val < 0) return;
			
			//Rule2 : symbol already exists
			if(findSymbol(sym).addr > 0){
				cout << sym << "=" << val << " Error: This variable is multiple times defined; first value used" << endl;
				continue;
			}
			//create symbol
			struct Symbol symb_el;
			strcpy(symb_el.token, sym);
			symb_el.addr = base + val;
			symb_el.modulenum = modulenum;
			symbtable.push_back(symb_el);
			moduledefs.push_back(symb_el);
		}
	}
	else if(defcount < 0) return;

	idx += defcount * 2 + 1;
	if(usecount = readInt(tokenlist[idx].token, tokenlist[idx].line, tokenlist[idx].offset)){
		if(usecount > 16){
			__parseerror(tokenlist[idx].line, tokenlist[idx].offset, 5);
			return;		
		}
		for(int i=0; i < usecount; i++){
			char* sym = tokenlist[idx + i + 1].token;
/**
			//TODO : syntax error : missing token
			if(){
			
			}
**/
			//Parse error : unexpected  token (symbol expected)
			if(readSym(sym, tokenlist[idx + i + 1].line, tokenlist[idx + i + 1].offset) <= 0) return;
			
		}
	}
	else if(usecount < 0) return;


	idx += usecount + 1;
	if(instcount = readInt(tokenlist[idx].token, tokenlist[idx].line, tokenlist[idx].offset)){
		for(int i=0; i < instcount; i++){
			char* addressmode = tokenlist[idx + 2*i + 1].token;
			int operand = readInt(tokenlist[idx + 2*i + 2].token, tokenlist[idx + 2*i + 2].line, tokenlist[idx + 2*i + 2].offset);
			//various checks
			//
			//Parse error : unexpected token (addr expected)
			if(!readIEAR(addressmode, tokenlist[idx + 2*i + 1].line, tokenlist[idx + 2*i + 1].offset)) return;
			//Parse error : unexpected token (integer expected)
			if(operand < 0) return;
		}	
	}
	else if(instcount < 0) return;

	int modulesize = instcount * 2 + 1;
 	idx += modulesize;
        base += instcount;
        modulenum++;

	//print warnings 
	//Rule5 : 
	for(int i=0; i < moduledefs.size(); i++){
		if(moduledefs[i].addr > modulesize){
			cout << "Warning: Module " << moduledefs[i].modulenum << ": " << moduledefs[i].token << " too big " << moduledefs[i].addr << " (max=" << modulesize << ") assume zero relative\n";
			moduledefs[i].addr = 0;	
		}
	}
    }
}


void pass2(){
    int idx = 0; //index for token
    int base = 0; //base address
    int count = 0; //instruction count
    int modulenum = 1;

    vector<Symbol> isused = symbtable;

    while(idx < tokenlist.size()){
        //createModule
        int defcount;
        int usecount;
        int instcount;
	vector<char*> uselist;

	if(defcount = readInt(tokenlist[idx].token, tokenlist[idx].line, tokenlist[idx].offset)){
		for(int i=0; i < defcount; i++){
	    		char* sym = tokenlist[idx + 2*i + 1].token;
			int val = readInt(tokenlist[idx + 2*i + 2].token, tokenlist[idx + 2*i + 2].line, tokenlist[idx + 2*i + 2].offset);
		}
    	}

	idx += defcount * 2 + 1;
	if(usecount = readInt(tokenlist[idx].token, tokenlist[idx].line, tokenlist[idx].offset)){
		for(int i=0; i < usecount; i++){
	    		char* sym = tokenlist[idx + i + 1].token;
			uselist.push_back(sym);
			//update used symbols
			Symbol target_sym = findSymbol(sym);
			for(int j=0; j < isused.size(); j++){
				if(strcmp(isused[j].token, sym) == 0){
					isused.erase(isused.begin() + j);	
				}
			}
		}
    	}

    	idx += usecount + 1;
	if(instcount = readInt(tokenlist[idx].token, tokenlist[idx].line, tokenlist[idx].offset)){
		for(int i=0; i < instcount; i++){
	    		char* addressmode = tokenlist[idx + 2*i + 1].token;
	    		int operand = readInt(tokenlist[idx + 2*i + 2].token, tokenlist[idx + 2*i + 2].line, tokenlist[idx + 2*i + 2].offset);;
	    		//various checks
	    		if(strcmp(addressmode, "R") == 0){
				operand += base;
			}
			else if(strcmp(addressmode, "E") == 0){
				int ref = operand % 1000;
				int opcode = operand / 1000;	
				Symbol target_sym = findSymbol(uselist[ref]);
				//Rule 3 : symbol not defined
				if(!target_sym.addr){
					cout << "Error: " << uselist[ref] << " is not defined; zero used" << endl;
				}
				else operand = opcode * 1000 + target_sym.addr;
			}
			else if(strcmp(addressmode, "I") == 0){
				//an immediate operand is unchanged
			}
			else if(strcmp(addressmode, "A") == 0){
				//operand is the absolute address
			}
			cout << count << " " << operand << endl;
			count++;
		}
    	}
    	idx += instcount * 2 + 1;
    	base += instcount;
	modulenum++;
    }

    //print warnings
    //Rule4 : symbols defined but not used
    for(int i = 0; i < isused.size(); i++){
	cout << "Warning: Module " << isused[i].modulenum << ": " << isused[i].token << " was defined but never used\n";	
    }
}


int main(){

	getToken();
	
	pass1();

	cout << "Symbol Table" << endl;
	for(int i = 0; i < symbtable.size(); i++){
        	cout << symbtable[i].token << "=" << symbtable[i].addr << endl;
        }	

	cout << endl;
	cout << "Memory Map" << endl;
	pass2();
}
