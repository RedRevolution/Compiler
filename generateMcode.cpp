﻿#include "generateMcode.h"

int regNum = 0, labNum = 0;
McodeUnit midCode[1000];
int midCodeNum = 0;

void emit(string op, string a, string b, string c) {
	midCode[midCodeNum].op = op;
	midCode[midCodeNum].arg1 = a;
	midCode[midCodeNum].arg2 = b;
	midCode[midCodeNum++].result = c;
}

void printMidCode() {
	FILE* out;
	out = fopen("midcode.txt", "w");
	for (int i = 0; i < midCodeNum;i++) {
		fprintf(out, "%s,%s,%s,%s\n", midCode[i].op.c_str(), midCode[i].arg1.c_str(), midCode[i].arg2.c_str(), midCode[i].result.c_str());
	}
	fclose(out);
}

string newReg() {
	string ret_v;
	char reg[300];
	sprintf(reg,"%d", regNum);
	regNum++;
	ret_v = reg;
	ret_v = "$t" + ret_v;
	return ret_v;
}

void resetRegNum(int num) {
	regNum = num;
}

int getRegNum() {
	return regNum;
}

string newLab() {
	string ret_v;
	char lab[300];
	sprintf(lab, "%d", labNum);
	labNum++;
	ret_v = lab;
	ret_v = "$label" + ret_v;
	return ret_v;
}