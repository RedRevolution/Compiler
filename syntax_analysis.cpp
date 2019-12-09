#include "syntax_analysis.h"
#include "generateObjcode.h"

bool HandleError;
Symtable syt[N];
int level;
bool hasreturn;

void error() {
	printf("error");
	system("pause");
}

void syntax_error(char code, int index) {
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

void printError() {
	//for (int i = 0; i < error_num; i++) {
	//	fprintf(out, "%d %c\n", error_line[i], error_code[i]);
	//}
}

void printsyn(const char s[]) {
	//fputs(s, out);
	//fputs("\n", out);
}

//无符号整数*
int usINT(string& num) {
	if (!HandleError) {
		if (symbol != "INTCON") error();
		printlex(); //打印整数
		num += token;
		printsyn("<无符号整数>");
		getsym();
		return 1;
	}
	else {
		if (symbol != "INTCON") {
			//error();
			return 0;
		}
		printlex(); //打印整数
		printsyn("<无符号整数>");
		getsym();
		return 1;
	}
}

//整数*
int INT(string& num) {
	if (!HandleError) {
		num = "";
		if (symbol != "PLUS" && symbol != "MINU" && symbol != "INTCON") error();
		if (symbol == "PLUS" || symbol == "MINU") {
			printlex(); //打印+-
			num += token;
			getsym();
		}
		usINT(num);
		printsyn("<整数>");
		return 0;
	}
	else {
		int ret_v = 1;
		if (symbol != "PLUS" && symbol != "MINU" && symbol != "INTCON") {
			//error();
			return 0;
		}
		if (symbol == "PLUS" || symbol == "MINU") {
			printlex(); //打印+-
			getsym();
		}
		ret_v = usINT(num);
		printsyn("<整数>");
		return ret_v;
	}
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
	level = 0;
	//常量说明
	if (symbol == "CONSTTK") {
		cs();
	}
	//变量说明
	if ((symbol == "INTTK" || symbol == "CHARTK") &&
		(preload(2) != "LPARENT" || preload(2) == "LPARENT"&&preload(3)=="SEMICN")) {
		vs();
	}
	//有|无返回值函数定义
	while (!(symbol == "VOIDTK" && preload(1) == "MAINTK")) {
		level++;
		if ((symbol == "INTTK" || symbol == "CHARTK") && preload(2) == "LPARENT")rfun();
		else nfun();
	}
	level++;
	mfun();
	printsyn("<程序>");
}

void syntax_analyse(const char* fileName, bool handleError) {
	in = fopen(fileName, "rt+");
	HandleError = handleError;
	getsym();
	program();
	fclose(in);
	return;
}