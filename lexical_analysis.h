#pragma once
#include <stdio.h>
#include <ctype.h>
#include <string>
using namespace std;

extern char a;
extern string symbol, token;
extern FILE* in, * out;

void reserver();
int isLetter();
int isDigit();
void strcatAchar();
void reserver2();
int getsym();
void ungetsym();
int preload(int x);
void printlex();