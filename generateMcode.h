#pragma once
#include <iostream>
using namespace std;

typedef struct {
	string op;
	string arg1;
	string arg2;
	string result;
}McodeUnit;

void emit(string opno, string a, string b, string c);
string newReg();
void resetRegNum(int num);
int getRegNum();
string newLab();
void printMidCode();