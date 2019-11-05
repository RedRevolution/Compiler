#pragma once
#include "lexical_analysis.h"
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
	Paratable pt;
};

typedef struct st {
	int length;
	struct Entry sym[N];
	st() : length(0) {}
	bool add_entry(string name, string type, string ret_type) {
		for (int i = 0; i < length; i++) {
			if (sym[i].name == name)return false;
		}
		sym[length].name = name;
		sym[length].type = type;
		sym[length++].ret_type = ret_type;
		return true;
	}
	bool add_pt_entry(string func_name, string pr_name, string ret_type) {
		for (int i = 0; i < length; i++) {
			if (sym[i].name == func_name) {
				return sym[i].pt.add(pr_name, ret_type);
			}
		}
	}
	int search_nfun(string s) {
		for (int i = 0; i < length; i++) {
			if (sym[i].type == "FUNC"
				&& sym[i].ret_type == "VOIDTK"
				&& sym[i].name == s)return i;
		}
		return -1;
	}
	int search_rfun(string s) {
		for (int i = 0; i < length; i++) {
			if (sym[i].type == "FUNC"
				&& sym[i].ret_type != "VOIDTK"
				&& sym[i].name == s)return i;
		}
		return -1;
	}
	int search_vname(string s) {
		for (int i = 0; i < length; i++) {
			if (sym[i].name == s)return i;
		}
		return -1;
	}
}Symtable;

extern Symtable global;
extern bool hasreturn;

//syn
void error();
void printsyn(const char s[]);
int usINT();
int INT();
void str();
void syntax_error(char code, int index = 0);
void program();

//declaration
void cd(Symtable& syt);
void cs(Symtable& syt);
void vd(Symtable& syt);
void vs(Symtable& syt);

//function
void dh(string& func_name);
void pt(string func_name, Symtable& p);
void nfun();
void mfun();
void rfun();

//sta_callfunc
void callrfun(Symtable& syt);
void callnfun(Symtable& syt);
void vpt(string func_name, Symtable& syt);

//expression
string expr(Symtable &syt);
string term(Symtable &syt);
string factor(Symtable &syt);

//statement
void assta(Symtable &syt);
void cdsta(Symtable &syt);
void condition(Symtable &syt);
void costa(Symtable& syt);
void lpsta(Symtable& syt);
void rdsta(Symtable& syt);
void rsta(Symtable& syt);
int sta(Symtable &syt);
void stas(Symtable &syt);
void steplen(Symtable &syt);
void wtsta(Symtable &syt);
