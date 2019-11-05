#pragma once
#include <stdio.h>
#include <ctype.h>
#include <string>
#define N 300
#define DEBUG 0
using namespace std;

extern char a;
extern string symbol, token;
extern FILE* in, * out;
extern int line_no;

extern char error_code[N];
extern int error_line[N];
extern int error_num;


void reserver();
int isLetter();
int isDigit();
void strcatAchar();
void reserver2();
int getsym();
string preload(int x);
void printlex();
void lexi_error(char code, int codeline = 0);
void add_error(char code, int line);