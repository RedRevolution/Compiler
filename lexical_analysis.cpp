#include "lexical_analysis.h"

char a;
string symbol, token;
FILE* in, * out;

int isLetter() {
	return isalpha(a) || a == '_';
}

int isDigit() {
	return isdigit(a);
}

void reserver() {
	if (token == "const")symbol = "CONSTTK";
	else if (token == "int")symbol = "INTTK";
	else if (token == "char")symbol = "CHARTK";
	else if (token == "void")symbol = "VOIDTK";
	else if (token == "main")symbol = "MAINTK";
	else if (token == "if")symbol = "IFTK";
	else if (token == "else")symbol = "ELSETK";
	else if (token == "do")symbol = "DOTK";
	else if (token == "while")symbol = "WHILETK";
	else if (token == "for")symbol = "FORTK";
	else if (token == "scanf")symbol = "SCANFTK";
	else if (token == "printf")symbol = "PRINTFTK";
	else if (token == "return")symbol = "RETURNTK";
	else symbol = "IDENFR";
}

void strcatAchar() {
	token += a;
}

void reserver2() {
	if (a == '+') symbol = "PLUS";
	else if (a == '-') symbol = "MINU";
	else if (a == '*') symbol = "MULT";
	else if (a == '/') symbol = "DIV";
	else if (a == ';') symbol = "SEMICN";
	else if (a == ',') symbol = "COMMA";
	else if (a == '(') symbol = "LPARENT";
	else if (a == ')') symbol = "RPARENT";
	else if (a == '[') symbol = "LBRACK";
	else if (a == ']') symbol = "RBRACK";
	else if (a == '{') symbol = "LBRACE";
	else if (a == '}') symbol = "RBRACE";
}

int getsym() {
	token = "";  //清空字符串
	if ((a = getc(in)) == EOF) return 0; //先读入一个字符，防止isspace报错
	while (isspace(a)) {	 /*读取字符，跳过空格、换行和Tab,\t\f等 */
		if ((a = getc(in)) == EOF) return 0;
	}
	if (isLetter()) {
		while (isLetter() || isDigit()) {
			strcatAchar();		/*将字符拼接成字符串 */
			a = getc(in);
		}
		ungetc(a, in);         /*指针后退一个字符 */
		reserver();
	}
	//数字
	else if (isDigit()) {
		while (isDigit()) {
			strcatAchar();
			a = getc(in);
		}
		ungetc(a, in);
		symbol = "INTCON";
	}
	//字符串
	else if (a == '"') {
		a = getc(in);
		while (a != '"') {
			strcatAchar();
			a = getc(in);
		}
		symbol = "STRCON";
	}
	//字符常量
	else if (a == '\'') {
		a = getc(in);
		strcatAchar();
		a = getc(in);  //执行完后a = ‘‘’
		symbol = "CHARCON";
	}
	//大于等于和大于
	else if (a == '>') {
		strcatAchar();
		a = getc(in);
		if (a == '=') {
			strcatAchar();
			symbol = "GEQ";
		}
		else {
			ungetc(a, in);
			symbol = "GRE";
		}
	}
	//小于等于和小于
	else if (a == '<') {
		strcatAchar();
		a = getc(in);
		if (a == '=') {
			strcatAchar();
			symbol = "LEQ";
		}
		else {
			ungetc(a, in);
			symbol = "LSS";
		}
	}
	//双等号和赋值
	else if (a == '=') {
		strcatAchar();
		a = getc(in);
		if (a == '=') {
			strcatAchar();
			symbol = "EQL";
		}
		else {
			ungetc(a, in);
			symbol = "ASSIGN";
		}
	}
	//不等于
	else if (a == '!') {
		strcatAchar();
		a = getc(in);
		strcatAchar();
		symbol = "NEQ";
	}
	//其他符号
	else {
		reserver2();
		strcatAchar();
	}
	return 1;
}

//指针移回到当前已读出单词的开头位置
void ungetsym() {
	fseek(in, -token.length() , SEEK_CUR); 
}


//预读  int|char+标识符 下一个单词(字符)  x = 2
//预读下一个单词   x = 1
int preload(int x) {
	int value = 0, count;
	string ttmp, stmp; //对当前单词进行备份
	ttmp = token;
	stmp = symbol;
	
	if (x == 1) {
		getsym();
		if (symbol == "LPARENT")value = 1;
		if (symbol == "MAINTK")value = 2;
		ungetsym();
		token = ttmp;//还原备份
		symbol = stmp;
		return value;
	}

	getsym(); //取得标识符
	count = token.length(); //标识符的长度

	//读下一个单词，空格也需计数
	a = getc(in);
	count++;
	while (isspace(a)) {
		a = getc(in);
		count++;
	}
	if (a == '(')value = 1; //如果为"(" 表示是函数，返回1
	
	fseek(in, -count, SEEK_CUR); //回退count个长度的字符

	token = ttmp;//还原备份
	symbol = stmp;
	return value;
}

//打印当前单词
void printlex() {
	fputs(symbol.c_str(), out);
	fputs(" ", out);
	fputs(token.c_str(), out);
	fputs("\n", out);
}

