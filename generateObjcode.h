#pragma once
#include "generateMcode.h"
#include <iostream>
#include <string>
#include <stack>
#include <map>
#define N 300
using namespace std;

typedef struct table_entry {
	string name; 
	string type; // Para, Con, Var, Arr
	string ret_type; //INTTK|CHARTK
	string val; //value, size 
}Tab_entry;

typedef struct Syt {
	int paraNum, conNum, varNum, arrNum, maxRegNum;
	int sregNo, tregNo, index, blocksize;
	Tab_entry para[N], con[N], var[N], arr[N];
	map<string, string> var2reg;
	map<string, int> var2bias;  //距fp的偏移
	Syt() : paraNum(0), conNum(0), varNum(0), arrNum(0), blocksize(0) {}
	void add_para(string name, string ret_type) {
		para[paraNum].name = name;
		para[paraNum++].ret_type = ret_type;
	}
	void add_con(string name, string ret_type, string val) {
		con[conNum].name = name;
		con[conNum].ret_type = ret_type;
		con[conNum++].val = val;
	}
	void add_var(string name, string ret_type) {
		var[varNum].name = name;
		var[varNum++].ret_type = ret_type;
	}
	void add_arr(string name, string ret_type, string size) {
		arr[arrNum].name = name;
		arr[arrNum].ret_type = ret_type;
		arr[arrNum++].val = size;
	}
	void setMaxRegNum(int n) {
		maxRegNum = n;
	}

	string getVarReg(string name) {
		map<string, string>::iterator iter;
		iter = var2reg.find(name);
		if (iter != var2reg.end()) {
			return iter->second;
		}
		return "";
	}

	string getVarBias(string name) {
		map<string, int>::iterator iter;
		iter = var2bias.find(name);
		if (iter != var2bias.end()) {
			return to_string(-1 * iter->second);
		}
		return "";
	}

	void init() {
		index = 0;
		sregNo = 0;
		tregNo = 0;
 		//初始化参数区
		for (int i = 0; i < paraNum; i++) {
			var2bias[para[i].name] = 4 * index;
			index++; 
		}

		//初始化局部常量区
		for (int i = 0; i < conNum; i++) {
			if (sregNo <= 7) {
				var2reg[con[i].name] = "$s" + to_string(sregNo);
				sregNo++;
			}
			else {
				var2bias[con[i].name] = 4 * index;
				index++;
			}
		}

		//初始化局部变量区
		for (int i = 0; i < varNum; i++) {
			if (sregNo <= 7) {
				var2reg[var[i].name] = "$s" + to_string(sregNo);
				sregNo++;
			}
			else {
				var2bias[var[i].name] = 4 * index;
				index++;
			}
		}

		//初始化局部数组区
		for (int i = 0; i < arrNum; i++) {
			var2bias[arr[i].name] = 4 * index;
			index += stoi(arr[i].val);
		}

		//初始化寄存器区
		for (int i = 0; i <= maxRegNum; i++) {
			if (tregNo <= 9) {
				var2reg["$t" + to_string(i)] = "$t" + to_string(tregNo);
				tregNo++;
			}
			else {
				var2bias["$t" + to_string(i)] = 4 * index;
				index++;
			}
		}

		//运行栈 blocksize							//+1为ra
		blocksize = index +  sregNo + tregNo + 1;  //sregNo + tregNo 用于维护寄存器现场
	}	
	
	string search_var_type(string name) {
		for (int i = 0; i < varNum; i++) {
			if (var[i].name == name) {
				return var[i].ret_type;
			}
		}
		return "";
	}
}Func_Syt;

extern int globalcnt;  
extern map<string, int> global2bias;
extern int index; 

void init();
void Max(int& a, string b, string c, string d);
void printmips(string a, string b, string c, string d);
void Con_local();
void Arr_local();
void Var_local();
string preloadvar(string varname, string regname);
string loadvar(string varname, int& type);
void swap(string& a, string& b);
void Calculate();
bool isNum(string a);
void Branch();
void Fetch();
void Arr_Brack();
void Arr_Brack_Assi();
void Func_Def();
void End_Func();
void Fcall();
void Call_Func();
void saveReg();
void restoreReg();
void Push();
void Jmp();
void Lab();
void Scan();
void Print();
void Ret();
void Assi();
void generateObjcode(const char* fileName);
int paraCnt();
