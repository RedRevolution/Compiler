#pragma once
#include <iostream>
using namespace std;

typedef struct mcode{
	string op;
	string arg1;
	string arg2;
	string result;
}McodeUnit;

extern int regNum,labNum;
extern McodeUnit midCode[1000];
extern int midCodeNum = 0;


void emit(string opno, string a, string b, string c);
string newReg();
void resetRegNum(int num);
int getRegNum();
string newLab();
void printMidCode();