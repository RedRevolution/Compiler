#pragma once
#include "lexical_analysis.h"
#include "generateMcode.h"
using namespace std;

typedef struct prt {
	int length;
	string name[N / 10]; //最多30个参数 
	string ret_type[N / 10]; //INT CHAR
	prt() : length(0) {}
	bool add(string pr_name, string ret_type) {
		for (int i = 0; i < length; i++) {
			if (name[i] == pr_name)return false;
		}
		this->name[length] = pr_name;
		this->ret_type[length++] = ret_type;
		return true;
	}
	bool check_length(int len) {
		return this->length == len;
	}
	bool check_type(string* ret_types) {
		for (int i = 0; i < length; i++) {
			if (ret_types[i] != this->ret_type[i]) {
				return false;
			}
		}
		return true;
	}
}Paratable;

struct Entry {
	string name;
	string type; //CONSTTK VAR FUNC
	string ret_type; //CHARTK INTTK VOIDTK
	string val; 
	Paratable pt;
};

typedef struct st {
	int length;
	struct Entry sym[N];
	st() : length(0) {}
	//"type == FUNC|CONSTTK|VAR|ARRAY"  ret_type = "CHARTK|INTTK|VOIDTK" value = "num|char|size|Level"
	bool add_entry(string name, string type, string ret_type, string valueOrSizeOrLevel) {
		for (int i = 0; i < length; i++) {
			if (sym[i].name == name)return false;
		}
		sym[length].name = name;
		sym[length].type = type;
		sym[length].ret_type = ret_type;
		sym[length++].val = valueOrSizeOrLevel;
		return true;
	}
	bool add_pt_entry(string func_name, string pr_name, string ret_type) {
		for (int i = 0; i < length; i++) {
			if (sym[i].name == func_name) {
				return sym[i].pt.add(pr_name, ret_type);
			}
		}
	}
	int search_name(string s) {
		for (int i = 0; i < length; i++) {
			if (sym[i].name == s)return i;
		}
		return -1;
	}
}Symtable;

extern bool HandleError;
extern Symtable syt[N];
extern int level;
extern bool hasreturn;

//syn
void error();
void printsyn(const char s[]);
int usINT(string& num);
int INT(string& num);
void str();
void syntax_error(char code, int index = 0);
void program();
void printError();

//declaration
void cd();
void cs();
void vd();
void vs();

//expression
//返回值类型:INTTK|CHARTK,regName是变量名,$tx和num(常数或字符的ascill)
string expr(string& regName);
string term(string& regName);
string factor(string& regName);

//function
void dh(string& func_name);
void pt(string func_name);
void nfun();
void mfun();
void rfun();

//sta_callfunc
void callrfun();
void callnfun();
void vpt(string func_name);

//statement
void assta();
void cdsta();
void condition(string& jumlab1, bool isFor, string& jumlab2);
void costa();
void lpsta();
void rdsta();
void rsta();
int sta();
void stas();
void steplen(string& num);
void wtsta();

