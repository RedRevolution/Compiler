#include "syntax_analysis.h"

Symtable global;

bool hasreturn;

void error() {
	printf("error");
	system("pause");
}

void syntax_error(char code, int index) {
	/*if (DEBUG) fprintf(out, "%d %c   %d\n", line_no, code, index); //打印对应error处理的代码行
	else {
		if(code == 'k'&&index == 1)fprintf(out, "%d %c\n", line_no-1, code);
		else fprintf(out, "%d %c\n", line_no, code);
	}*/


	//找到前面有几个换行符
	long nowaddr = ftell(in);
	int count = 0;
	while (1) {
		fseek(in, -1, SEEK_CUR);
		a = getc(in);
		if (isspace(a))break;
		ungetc(a, in);
	}
	while (1) {
		fseek(in, -1, SEEK_CUR);
		a = getc(in);
		if (!isspace(a))break;
		ungetc(a, in);
	}
	while (ftell(in) < nowaddr) {
		a = getc(in);
		if (a == '\n')count++;
	}
	if (code == 'k' && index == 1)add_error(code, line_no - count);
	else add_error(code, line_no);
}

void printsyn(const char s[]) {
	//fputs(s, out);
	//fputs("\n", out);
}

//无符号整数*
int usINT() {
	if (symbol != "INTCON") {
		//error();
		return 0;
	}
	printlex(); //打印整数
	printsyn("<无符号整数>");
	getsym();
	return 1;
}

//整数*
int INT() {
	int ret_v = 1;
	if (symbol != "PLUS" && symbol != "MINU" && symbol != "INTCON") {
		//error();
		return 0;
	}
	if (symbol == "PLUS" || symbol == "MINU") {
		printlex(); //打印+-
		getsym();
	}
	ret_v = usINT();
	printsyn("<整数>");
	return ret_v;
}

//字符串*
void str() {
	if (symbol != "STRCON") error();
	printlex();
	printsyn("<字符串>");
	getsym();
}


//程序*
void program() {
	//常量说明
	if (symbol == "CONSTTK") {
		cs(global);
	}
	//变量说明
	if ((symbol == "INTTK" || symbol == "CHARTK") &&
		(preload(2) != "LPARENT" || preload(2) == "LPARENT"&&preload(3)=="SEMICN")) {
		vs(global);
	}
	//有|无返回值函数定义
	while (!(symbol == "VOIDTK" && preload(1) == "MAINTK")) {
		if ((symbol == "INTTK" || symbol == "CHARTK") && preload(2) == "LPARENT")rfun();
		else nfun();
	}
	mfun();
	printsyn("<程序>");
}

void print() {
	/*
	for (int i = 0; i < error_num - 1; i++) {
		int index = i;
		int j;
		// 找出最小值得元素下标
		for (j = i + 1; j < error_num; j++) {
			if (error_line[j] < error_line[index]) {
				index = j;
			}
		}
		int tmp = error_line[index];
		error_line[index] = error_line[i];
		error_line[i] = tmp;

		char tp = error_code[index];
		error_code[index] = error_code[i];
		error_code[i] = tp;
	}
	if (error_code[0] != 'h') {
		fprintf(out, "%d %c\n", error_line[0], error_code[0]);
	}*/
	for (int i = 0; i < error_num; i++) {
		fprintf(out, "%d %c\n", error_line[i], error_code[i]);
	}
}

int main() {
	in = fopen("testfile.txt", "rt+");
	out = fopen("error.txt", "w");
	getsym();
	program();
	print();
	fclose(in);
	fclose(out);
	return 0;
}