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
	cout<< "Parse Error line " << linenum << " offset " << lineoffset << ": " << errstr[errcode] << endl; 
}

struct Token {
	char token[1024];
	int line;
	int offset;
} token;

struct Symbol {
	char token[1024];
	int addr;
	int modulenum;
	char errormsg[1024];
} symbol;

vector<Token> tokenlist;
vector<Symbol> symbtable;
vector<int> modulesizes;
struct Token eof;
 
//get token, line, lineoffset
void getToken(){
	//getline
	//strtok		
	char tokstr[256];
        int line = 1;
        int offset = 1;
	

        while(cin.getline(tokstr, 1024)){
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
		eof.line = line;
                eof.offset = offset;

                line++;
                offset = 1;
        }
	if(eof.offset > 1) eof.offset -= 1;

}


int readInt(int index){
	char* tok = tokenlist[index].token;
	int line = tokenlist[index].line;
	int offset = tokenlist[index].offset;  
	
	if(index >= tokenlist.size()){
		line = eof.line;
		offset = eof.offset;
	}
	for(int i = 0; i < strlen(tok); i++){
		if(!isdigit(tok[i])){
			__parseerror(line, offset, 0);	
			return -1;
		}
	}
	return atoi(tok);

}

int readSym(int index){
//-1 : two many characters, 0 : invalid symbol, 1 : valid symbol
        char* tok = tokenlist[index].token;
        int line = tokenlist[index].line;
        int offset = tokenlist[index].offset;

        if(index >= tokenlist.size()){
                line = eof.line;
                offset = eof.offset;
        }

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

bool readIEAR(int index){
        char* tok = tokenlist[index].token;
        int line = tokenlist[index].line;
        int offset = tokenlist[index].offset;

        if(index >= tokenlist.size()){
                line = eof.line;
                offset = eof.offset;
        }

	if(strcmp(tok, "I") == 0 || strcmp(tok, "A") == 0 || strcmp(tok, "E") == 0 || strcmp(tok, "R") == 0) return true;
	else{
		__parseerror(line, offset, 2);	
		return false;
	}
}

int findSymbol(char* target_sym){
	for(int s = 0; s < symbtable.size(); s++){
		if(strcmp(symbtable[s].token, target_sym) == 0){
			return s;
		}
	}
	return -1;
}

char* convertTOdigit(int ndigit, int num){
        char strnum[1024] = "";
        sprintf(strnum, "%d", num);
        int num_zeros = ndigit - (int)strlen(strnum);
        char zeros[3]= "";
        for(int i = 0; i < num_zeros; i++){
                strcat(zeros, "0");
        }
        strcat(zeros, strnum);
	return zeros;
}

//Parse error checking, create symbol table

int pass1(){
    int idx = 0;
    int base = 0; 
    int modulenum = 1;
    int memory = 0;
    modulesizes.push_back(-1);
    while(idx < tokenlist.size()){
	//createModule
	int defcount;
	int usecount;
	int instcount;
	vector<Symbol> moduledefs; //defined symbs per module
	vector<int> moduledefidx; //save index of symbol address for token modification
	if(defcount = readInt(idx)){
		if(defcount > 16){
			__parseerror(tokenlist[idx].line, tokenlist[idx].offset, 4);
			return 0;	
		}
		for(int i=0; i < defcount; i++){
			char* sym = tokenlist[idx + 2*i + 1].token;
			//Parse error : unexpected token (symbol expected)
			if(readSym(idx + 2*i + 1) <= 0) return 0;

			int val = readInt(idx + 2*i + 2);
			//Parse error : unexpected token (integer expected)
			if(val < 0) return 0;
			
			//Rule2 : symbol already exists
			int isdefined = findSymbol(sym);	
			if(isdefined >= 0){
				strcpy(symbtable[isdefined].errormsg, " Error: This variable is multiple times defined; first value used");
				continue;
			}
			//create symbol
			struct Symbol symb_el;
			strcpy(symb_el.token, sym);
			symb_el.addr = base + val;
			symb_el.modulenum = modulenum;
			strcpy(symb_el.errormsg, "");
			symbtable.push_back(symb_el);
			moduledefs.push_back(symb_el);
			moduledefidx.push_back(idx + 2*i + 2);

		}
	}
	else if(defcount < 0) return 0;

	idx += defcount * 2 + 1;
	if(usecount = readInt(idx)){
		if(usecount > 16){
			__parseerror(tokenlist[idx].line, tokenlist[idx].offset, 5);
			return 0;		
		}
		for(int i=0; i < usecount; i++){
			char* sym = tokenlist[idx + i + 1].token;
			//Parse error : unexpected  token (symbol expected)
			if(readSym(idx + i + 1) <= 0) return 0;
			
		}
	}
	else if(usecount < 0) return 0;


	idx += usecount + 1;
	if(instcount = readInt(idx)){
		memory += instcount;
		if(memory >= 512){
			__parseerror(tokenlist[idx].line, tokenlist[idx].offset, 6); 
			return 0;
		}
		for(int i=0; i < instcount; i++){
			char* addressmode = tokenlist[idx + 2*i + 1].token;
			int operand = readInt(idx + 2*i + 2);
			//various checks
			//
			//Parse error : unexpected token (addr expected)
			if(!readIEAR(idx + 2*i + 1)) return 0;
			//Parse error : unexpected token (integer expected)
			if(operand < 0) return 0;
		}	
	}
	else if(instcount < 0) return 0;

	//print warnings 
	//Rule5 : address of def exceeds the size of module 
	for(int i=0; i < moduledefs.size(); i++){
		//cout << moduledefs[i].modulenum << " " << moduledefs[i].token << " " << moduledefs[i].addr << " " << instcount << endl;
		if(moduledefs[i].addr - base >= instcount){
			cout << "Warning: Module " << moduledefs[i].modulenum << ": " << moduledefs[i].token << " too big " << moduledefs[i].addr << " (max=" << instcount-1 << ") assume zero relative\n";
			
			int symbidx = findSymbol(moduledefs[i].token);
			symbtable[symbidx].addr = base;	//set address to 0
		}
	}
	modulesizes.push_back(instcount);
        idx += instcount * 2 + 1;
        base += instcount;
        modulenum++;
    }
    return 1;
}


void pass2(){
    int idx = 0; //index for token
    int base = 0; //base address
    int count = 0; //instruction count
    int modulenum = 1;

    vector<Symbol> is_def_used = symbtable; //Rule4 : to check unused defs in symbol table

    while(idx < tokenlist.size()){
        //createModule
        int defcount;
        int usecount;
        int instcount;
	vector<char*> uselist;
	vector<char*> is_use_used; //Rule7: to check symbols in the uselist are referred.
	if(defcount = readInt(idx)){
		for(int i=0; i < defcount; i++){
	    		char* sym = tokenlist[idx + 2*i + 1].token;
			int val = readInt(idx + 2*i + 2);
		}
    	}

	idx += defcount * 2 + 1;
	if(usecount = readInt(idx)){
		for(int i=0; i < usecount; i++){
	    		char* sym = tokenlist[idx + i + 1].token;
			uselist.push_back(sym);
			is_use_used.push_back(sym);

			//update used symbols
			if(findSymbol(sym) >= 0){
				Symbol target_sym = symbtable[findSymbol(sym)];
				for(int j=0; j < is_def_used.size(); j++){
					if(strcmp(is_def_used[j].token, sym) == 0){
						is_def_used.erase(is_def_used.begin() + j);	
					}
				}
			}
		}
    	}

    	idx += usecount + 1;
	if(instcount = readInt(idx)){
		for(int i=0; i < instcount; i++){
	    		char* addressmode = tokenlist[idx + 2*i + 1].token;
	    		int instr = readInt(idx + 2*i + 2);
	    		int operand = instr % 1000;
			int opcode = instr / 1000;
			int absaddr;
			char errormsg[1024] = "";

			//various checks
	    		if(strcmp(addressmode, "R") == 0){
				//Rule 11 : illegal opcode
				if(opcode >= 10){
					absaddr = 9999;
                                	strcpy(errormsg, "Error: Illegal opcode; treated as 9999");
				}
				else{	
					//Rule9 : if relative address exceeds module size
					if(operand > modulesizes[modulenum]){
						absaddr = opcode * 1000 + base; //map with relative value "0"
						strcpy(errormsg, "Error: Relative address exceeds module size; zero used"); 
					}
					else absaddr = opcode * 1000 + operand + base;
				}
			}
			if(strcmp(addressmode, "E") == 0){
				//Rule 11 : illegal opcode
                                if(opcode >= 10){
                                        absaddr = 9999;
                                        strcpy(errormsg, "Error: Illegal opcode; treated as 9999");
                                }	
				else if(operand >= uselist.size()){//Rule6 : external address larger than the uselist
					strcpy(addressmode, "I");	
					strcpy(errormsg, "Error: External address exceeds length of uselist; treated as immediate");
				}	
				else{
					int isdefined = findSymbol(uselist[operand]);
					//Rule3 : symbol not defined
					if(isdefined < 0){
						absaddr = opcode * 1000;
						char* symb = uselist[operand];
						char tmp[1024] = "Error: ";
						strcat(tmp, symb);
						strcat(tmp, " is not defined; zero used");  
						strcpy(errormsg, tmp); 
					}
					else{
						Symbol target_sym = symbtable[isdefined];
						absaddr = opcode * 1000 + target_sym.addr;
					}
				
					//update used uselist symbols
					for(int j = 0; j < is_use_used.size(); j++){
						if(strcmp(is_use_used[j], uselist[operand]) == 0){
							is_use_used.erase(is_use_used.begin() + j);
						}
					}	
				}
			}
			if(strcmp(addressmode, "I") == 0){
				//an immediate operand is unchanged
				//Rule10 : illegal value
				if(instr >= 10000){
					absaddr = 9999;
					strcpy(errormsg, "Error: Illegal immediate value; treated as 9999");
				}
				else{
					absaddr = instr;
				}
			}
			if(strcmp(addressmode, "A") == 0){
				//Rule 11 : illegal opcode
                                if(opcode >= 10){
                                        absaddr = 9999;
                                        strcpy(errormsg, "Error: Illegal opcode; treated as 9999");
                                }
				//Rule8: if absolute address exceeds the machine size
				else if(operand > 512){
					absaddr = opcode * 1000;
					strcpy(errormsg, "Error: Absolute address exceeds machine size; zero used");
				}
				else{
					absaddr = instr;
				}
			}
			char absaddr_digit[1024];
			strcpy(absaddr_digit, convertTOdigit(4, absaddr));
			char count_digit[1024];
			strcpy(count_digit, convertTOdigit(3, count)); 
			cout << count_digit << ": " << absaddr_digit << " " << errormsg << endl;
			count++;
		}
    	}
	for(int i = 0; i < is_use_used.size(); i++){
        	cout << "Warning: Module " << modulenum << ": " << is_use_used[i] << " appeared in the uselist but was not actually used\n";
    	}

    	idx += instcount * 2 + 1;
    	base += instcount;
	modulenum++;
    }
    cout << endl;
    //print warnings
    //Rule4 : symbols defined but not used
    for(int i = 0; i < is_def_used.size(); i++){
	cout << "Warning: Module " << is_def_used[i].modulenum << ": " << is_def_used[i].token << " was defined but never used\n";	
    }
    
}


int main(){

	getToken();
/**	
	for(int i = 0; i < tokenlist.size(); i++){
		cout << tokenlist[i].token << " " << "line " << tokenlist[i].line << " offset " << tokenlist[i].offset << endl;
	}	
**/
	
	if(!pass1()) return -1;

	cout << "Symbol Table" << endl;
	for(int i = 0; i < symbtable.size(); i++){
        	cout << symbtable[i].token << "=" << symbtable[i].addr << symbtable[i].errormsg << endl;
        }	

	cout << endl;
	cout << "Memory Map" << endl;
	pass2();
	cout << endl;

}
