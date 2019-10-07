#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define N 300

char token[N], symbol[10], a;
FILE* in, * out;

int isLetter() {
	return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || a == '_';
}

int isDigit() {
	return (a >= '0' && a <= '9');
}

void reserver() {
	if (strcmp(token, "const") == 0)strcpy(symbol, "CONSTTK");
	else if (strcmp(token, "int") == 0)strcpy(symbol, "INTTK");
	else if (strcmp(token, "char") == 0)strcpy(symbol, "CHARTK");
	else if (strcmp(token, "void") == 0)strcpy(symbol, "VOIDTK");
	else if (strcmp(token, "main") == 0)strcpy(symbol, "MAINTK");
	else if (strcmp(token, "if") == 0)strcpy(symbol, "IFTK");
	else if (strcmp(token, "else") == 0)strcpy(symbol, "ELSETK");
	else if (strcmp(token, "do") == 0)strcpy(symbol, "DOTK");
	else if (strcmp(token, "while") == 0)strcpy(symbol, "WHILETK");
	else if (strcmp(token, "for") == 0)strcpy(symbol, "FORTK");
	else if (strcmp(token, "scanf") == 0)strcpy(symbol, "SCANFTK");
	else if (strcmp(token, "printf") == 0)strcpy(symbol, "PRINTFTK");
	else if (strcmp(token, "return") == 0)strcpy(symbol, "RETURNTK");
	else strcpy(symbol, "IDENFR");
}

void strcatAchar() {
	char temp[N];
	temp[0] = a;
	temp[1] = '\0';
	strcat(token, temp);
}

void reserver2() {
	if (a == '+') strcpy(symbol, "PLUS");
	else if (a == '-') strcpy(symbol, "MINU");
	else if (a == '*') strcpy(symbol, "MULT");
	else if (a == '/') strcpy(symbol, "DIV");
	else if (a == ';') strcpy(symbol, "SEMICN");
	else if (a == ',') strcpy(symbol, "COMMA");
	else if (a == '(') strcpy(symbol, "LPARENT");
	else if (a == ')') strcpy(symbol, "RPARENT");
	else if (a == '[') strcpy(symbol, "LBRACK");
	else if (a == ']') strcpy(symbol, "RBRACK");
	else if (a == '{') strcpy(symbol, "LBRACE");
	else if (a == '}') strcpy(symbol, "RBRACE");
}

int scan()
{
	strcpy(token, "");  //清空字符串
	if ((a = getc(in)) == EOF) return 0; //先读入一个字符，防止isspace报错
	while (isspace(a)) {	 /*读取字符，跳过空格、换行和Tab,\t\f等*/
		if ((a = getc(in)) == EOF) return 0;
	}
	if (isLetter()) {
		while (isLetter() || isDigit()) {
			strcatAchar();		/*将字符拼接成字符串*/
			a = getc(in);
		}
		ungetc(a, in);         /*指针后退一个字符*/
		reserver();
	}
	//数字
	else if (isDigit()) {
		while (isDigit()) {
			strcatAchar();
			a = getc(in);
		}
		ungetc(a, in);
		strcpy(symbol, "INTCON");
	}
	//字符串
	else if (a == '"') {
		a = getc(in);
		while (a != '"') {
			strcatAchar();
			a = getc(in);
		}
		strcpy(symbol, "STRCON");
	}
	//字符常量
	else if (a == '\'') {
		a = getc(in);
		strcatAchar();
		a = getc(in);  //执行完后a = ‘‘’
		strcpy(symbol, "CHARCON");
	}
	//大于等于和大于
	else if (a == '>') {
		strcatAchar();
		a = getc(in);
		if (a == '=') {
			strcatAchar();
			strcpy(symbol, "GEQ");
		}
		else {
			ungetc(a, in);
			strcpy(symbol, "GRE");
		}
	}
	//小于等于和小于
	else if (a == '<') {
		strcatAchar();
		a = getc(in);
		if (a == '=') {
			strcatAchar();
			strcpy(symbol, "LEQ");
		}
		else {
			ungetc(a, in);
			strcpy(symbol, "LSS");
		}
	}
	//双等号和赋值
	else if (a == '=') {
		strcatAchar();
		a = getc(in);
		if (a == '=') {
			strcatAchar();
			strcpy(symbol, "EQL");
		}
		else {
			ungetc(a, in);
			strcpy(symbol, "ASSIGN");
		}
	}
	//不等于
	else if (a == '!') {
		strcatAchar();
		a = getc(in);
		strcatAchar();
		strcpy(symbol, "NEQ");
	}
	//其他符号
	else {
		reserver2();
		strcatAchar();
	}
	return 1;
}

int main() {
	in = fopen("testfile.txt", "rt+");
	out = fopen("output.txt", "w");
	while (1) {
		if (!scan()) break;
		fputs(symbol, out);
		fputs(" ", out);
		fputs(token, out);
		fputs("\n", out);
	}
	fclose(in);
	fclose(out);
	return 0;
}