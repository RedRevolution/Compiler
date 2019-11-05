#include "lexical_analysis.h"

char a;
string symbol, token;
FILE* in, * out;
int line_no = 1;

char error_code[N];
int error_line[N];
int error_num = 0;

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
	else {
		lexi_error('a'); /*错误处理*/
		getsym();
	}
}

int getsym() {
	token = "";  //清空字符串
	if ((a = getc(in)) == EOF) return 0; //先读入一个字符，防止isspace报错
	while (isspace(a)) {	 /*读取字符，跳过空格、换行和Tab,\t\f等 */
		if (a == '\n') line_no++;
		if ((a = getc(in)) == EOF) return 0;
	}
	if (isLetter()) {
		while (isLetter() || isDigit()) {
			token += a;		/*将字符拼接成字符串 */
			a = getc(in);
		}
		ungetc(a, in);         /*指针后退一个字符 */
		reserver();
	}
	//数字
	else if (isDigit()) {
		char temp = a;
		while (isDigit()) {
			token += a;
			a = getc(in);
		}
		if (token.length() > 1&& temp == '0') {
			lexi_error('a', 1);
		}
		ungetc(a, in);
		symbol = "INTCON";
	}
	//字符串
	else if (a == '"') {
		a = getc(in);
		while (a != '"') {
			/*错误处理*/
			//注意先判是不是换行符再判非法
			//字符串同一行中没有配对的右双引号,语法中字符串只在print语句中出现
			//printf '(' ＜字符串＞,＜表达式＞ ')'| printf '('＜字符串＞ ')'
			if (a == '\n') {
				lexi_error('a');
				ungetc(a, in); //回退换行符
				//回退到printf结束右括号的前一个字符
				fseek(in, -1, SEEK_CUR);
				while (preload(1) != "RPARENT") fseek(in, -1, SEEK_CUR);
				break;
			}
			//字符串中出现非法字符
			if (!(a == 32 || a == 33 || (a >= 35 && a <= 126)))lexi_error('a');
			token += a;
			a = getc(in);
		}
		symbol = "STRCON";
	}
	//字符常量
	else if (a == '\'') {
		a = getc(in);
		/*错误处理*/
		if(!(a=='+'||a=='-'||a=='*'||a=='/'||isDigit()||isLetter()))lexi_error('a');
		token += a;
		a = getc(in); //读右单引号或错误处理 
		/*错误处理*/
		//字符常量缺右单引号
		if (a != '\'') {
			lexi_error('a');
			ungetc(a, in);
		}
		symbol = "CHARCON";
	}
	//大于等于和大于
	else if (a == '>') {
		token += a;
		a = getc(in);
		if (a == '=') {
			token += a;
			symbol = "GEQ";
		}
		else {
			ungetc(a, in);
			symbol = "GRE";
		}
	}
	//小于等于和小于
	else if (a == '<') {
		token += a;
		a = getc(in);
		if (a == '=') {
			token += a;
			symbol = "LEQ";
		}
		else {
			ungetc(a, in);
			symbol = "LSS";
		}
	}
	//双等号和赋值
	else if (a == '=') {
		token += a;
		a = getc(in);
		if (a == '=') {
			token += a;
			symbol = "EQL";
		}
		else {
			ungetc(a, in);
			symbol = "ASSIGN";
		}
	}
	//不等于
	else if (a == '!') {
		token += a;
		a = getc(in);
		if (a != '=') {
			lexi_error('a', 1);
		}
		token += a;
		symbol = "NEQ";
	}
	//其他符号
	else {
		reserver2();
		token += a;
	}
	return 1;
}

//预读接下来第x个单词的类型
string preload(int x) {
	long preaddr,nowaddr;
	string ttmp, stmp, type; //对当前单词进行备份
	int line_no_pre;

	ttmp = token;
	stmp = symbol;
	line_no_pre = line_no;
	preaddr = ftell(in);
	
	for (int i = 0; i < x; i++) getsym();
	
	type = symbol;

	nowaddr = ftell(in);
	fseek(in, preaddr - nowaddr, SEEK_CUR); 
	//fseek(in, -count, SEEK_CUR); //回退count个长度的字符
	
	token = ttmp;//还原备份
	symbol = stmp;
	line_no = line_no_pre;
	return type;
}


//打印当前单词
void printlex() {
	//fputs(symbol.c_str(), out);
	//fputs(" ", out);
	//fputs(token.c_str(), out);
	//fputs("\n", out);
}

void add_error(char code,int line) {
	for (int i = 0; i < error_num; i++) {
		if (error_code[i] == code && error_line[i] == line) {
			return;
		}
	}
	error_code[error_num] = code;
	error_line[error_num++] = line;
}


void lexi_error(char code,int codeline) {
	//if (DEBUG) fprintf(out, "%d %c   %d\n", line_no, code, codeline); //打印对应error处理的代码行
	add_error(code, line_no);
}