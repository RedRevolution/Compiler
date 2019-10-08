#include "lexical_analysis.h"
#include "syntax_analysis.h"
#include <stdlib.h>
using namespace std;

string nfunID[300];
int nfuncnt;

void error() {
	printf("error");
	system("pause");
}

void printsyn(const char s[]) {
	fputs(s, out);
	fputs("\n", out);
}

//无符号整数*
void usINT() {
	if (symbol != "INTCON") error();
	printlex(); //打印整数
	printsyn("<无符号整数>");
	getsym();
}

//整数*
void INT() {
	if (symbol != "PLUS" && symbol != "MINU" && symbol != "INTCON") error();
	if (symbol == "PLUS" || symbol == "MINU") {
		printlex(); //打印+-
		getsym();
	}
	usINT();
	printsyn("<整数>");
}

//查找无返回值函数标识符表
int find(string s) {
	for (int i = 0; i < nfuncnt; i++) {
		if (nfunID[i] == s) {
			return 1;
		}
	}
	return 0;
}

//字符串*
void str() {
	if (symbol != "STRCON") error();
	printlex();
	printsyn("<字符串>");
	getsym();
}

//常量定义*
void cd() {
	if (symbol != "INTTK" && symbol != "CHARTK") error();
	printlex(); //打印int|char
	if (symbol == "INTTK") {
		while (1) {
			getsym(); //标识符
			printlex();
			getsym(); // =
			printlex();
			getsym();
			INT();
			if (symbol != "COMMA") break; //分号退出
			printlex(); //打印逗号
		}
	}
	else {
		while (1) {
			getsym(); //标识符
			printlex();
			getsym(); // =
			printlex();
			getsym(); //字符
			printlex();
			getsym();
			if (symbol != "COMMA") break;
			printlex(); //打印逗号
		}
	}
	printsyn("<常量定义>");
}

//常量说明*
void cs() {
	if (symbol != "CONSTTK") error();
	printlex();
	while (1) {
		getsym();
		cd();
		printlex(); //打印分号
		getsym();
		if (symbol != "CONSTTK")break;
		printlex(); //打印const
	}
	printsyn("<常量说明>");
}

//声明头部*
void dh() {
	if (symbol != "INTTK" && symbol != "CHARTK") error();
	printlex();
	getsym(); //标识符
	printlex(); //打印标识符
	printsyn("<声明头部>");
	getsym();
}

//变量定义*
void vd() {
	if (symbol != "INTTK" && symbol != "CHARTK") error();
	printlex();
	while (1) {
		getsym(); //读入一个标识符
		printlex();
		getsym(); //可能是逗号，分号或者[
		// [
		if (symbol == "LBRACK") {
			printlex(); //打印[
			getsym(); //读入无符号整数
			usINT();
			printlex(); //打印]
			getsym(); // 读入逗号|分号
		}
		if (symbol == "SEMICN")break; //分号退出
		printlex(); //打印逗号
	}
	printsyn("<变量定义>");
}

//变量说明*
void vs() {
	if (symbol != "INTTK" && symbol != "CHARTK") error();
	while (1) {
		vd();
		printlex(); //打印分号
		getsym();
		if (symbol != "INTTK" && symbol != "CHARTK")break;
		if (preload(2))break; //函数定义而非变量定义
	}
	printsyn("<变量说明>");
}

//参数表*
void pt() {
	//参数不为空
	if (symbol == "INTTK" || symbol == "CHARTK") {
		printlex(); //打印 int|char
		while (1) {
			getsym(); //读入标识符
			printlex(); //打印标识符
			getsym(); //读入逗号或者）
			if (symbol != "COMMA")break;
			printlex(); //打印逗号
			getsym(); //读入 int|char
			printlex(); //打印 int|char
		}
	}
	printsyn("<参数表>");
}

//复合语句*
void costa() {
	if (symbol == "CONSTTK")cs();
	if (symbol == "INTTK" || symbol == "CHARTK")vs();
	stas();
	printsyn("<复合语句>");
}

//有返回值函数定义*
void rfun() {
	if (symbol != "INTTK" && symbol != "CHARTK") error();
	dh();
	printlex(); //打印（
	getsym();
	pt();
	printlex(); //打印）
	getsym();
	printlex(); //打印{
	getsym();
	costa();
	printlex(); //打印}
	printsyn("<有返回值函数定义>");
	getsym();
}

//无返回值函数定义*
void nfun() {
	if (symbol != "VOIDTK") error();
	printlex();
	getsym(); //读入标识符
	nfunID[nfuncnt++] = token; //打表
	printlex(); //打印标识符
	getsym();
	printlex(); //打印（
	getsym();
	pt();
	printlex(); //打印）
	getsym();
	printlex(); //打印{
	getsym();
	costa();
	printlex(); //打印}
	printsyn("<无返回值函数定义>");
	getsym();
}

//主函数*
void mfun() {
	if (symbol != "VOIDTK") error();
	printlex(); //打印void
	getsym(); //读入标识符main
	printlex(); //打印标识符main
	getsym();
	printlex(); //打印（
	getsym();
	printlex(); //打印）
	getsym();
	printlex(); //打印{
	getsym();
	costa();
	printlex(); //打印}
	printsyn("<主函数>"); //主函数结束无须getsym
}

//表达式*
void expr() {
	if (symbol == "PLUS" || symbol == "MINU") {
		printlex();
		getsym();
	}
	while (1) {
		term();
		if (symbol != "PLUS" && symbol != "MINU")break;
		printlex(); //打印正负号
		getsym();
	}
	printsyn("<表达式>");
}

//项*
void term() {
	factor();
	while (symbol == "MULT" || symbol == "DIV") {
		printlex();
		getsym();
		factor();
	}
	printsyn("<项>");
}

//因子*
void factor() {
	if (symbol == "IDENFR") {
		//有返回值调用函数
		if (preload(1)) {
			callrfun();
			printsyn("<因子>");
			return;
		}

		printlex(); //打印标识符
		getsym();
		//标识符[表达式]
		if (symbol == "LBRACK") {
			printlex(); //打印[
			getsym();
			expr();
			printlex(); //打印]
			getsym();
		}
	}
	else if (symbol == "LPARENT") {
		printlex(); //打印（
		getsym();
		expr();
		printlex(); //打印）
		getsym();
	}
	else if (symbol == "PLUS" || symbol == "MINU" || symbol == "INTCON") {
		INT();
	}
	else if (symbol == "CHARCON") {
		printlex();
		getsym();
	}
	printsyn("<因子>");
}

//语句*
void sta() {
	if (symbol == "IDENFR") {
		if (preload(1)) {  //函数调用
			if (find(token)) callnfun();
			else callrfun();
			printlex(); //打印分号
			getsym();
		}
		else {	//赋值语句
			assta();
			printlex(); //打印分号
			getsym();
		}
	}
	else if (symbol == "WHILETK" || symbol == "FORTK" || symbol == "DOTK") {
		lpsta();
	}
	else if (symbol == "IFTK") {
		cdsta();
	}
	else if (symbol == "RETURNTK") {
		rsta();
		printlex(); //打印分号
		getsym();
	}
	else if (symbol == "SCANFTK") {
		rdsta();
		printlex(); //打印分号
		getsym();
	}
	else if (symbol == "PRINTFTK") {
		wtsta();
		printlex(); //打印分号
		getsym();
	}
	else if (symbol == "LBRACE") {
		printlex(); //打印 {
		getsym();
		stas();
		printlex(); //打印 }
		getsym();
	}
	else { //空语句
		printlex(); //打印分号
		getsym();
	}
	printsyn("<语句>");
}

//赋值语句*
void assta() {
	if (symbol != "IDENFR") error();
	printlex(); //打印标识符
	getsym();
	if (symbol == "ASSIGN") {
		printlex();
		getsym();
		expr();
	}
	else {
		printlex(); //打印[
		getsym();
		expr();
		printlex(); //打印]
		getsym();
		printlex(); //打印=
		getsym();
		expr();
	}
	printsyn("<赋值语句>");
}

//条件语句*
void cdsta() {
	if (symbol != "IFTK")error();
	printlex();
	getsym();
	printlex(); //打印（
	getsym();
	condition();
	printlex(); //打印）
	getsym();
	sta();
	if (symbol == "ELSETK") {
		printlex();
		getsym();
		sta();
	}
	printsyn("<条件语句>");
}

//条件*
void condition() {
	expr();
	if (symbol == "LSS" || symbol == "LEQ"
		|| symbol == "GRE" || symbol == "GEQ"
		|| symbol == "NEQ" || symbol == "EQL") {
		printlex();
		getsym();
		expr();
	}
	printsyn("<条件>");
}

//循环语句*
void lpsta() {
	if (symbol == "WHILETK") {
		printlex();
		getsym();
		printlex(); //打印（
		getsym();
		condition();
		printlex(); //打印）
		getsym();
		sta();
	}
	else if (symbol == "DOTK") {
		printlex();
		getsym();
		sta();
		printlex(); //打印while
		getsym();
		printlex();//打印（
		getsym();
		condition();
		printlex(); //打印）
		getsym();
	}
	else if (symbol == "FORTK") {
		printlex();
		getsym();
		printlex(); //打印（
		getsym();
		printlex(); //打印标识符
		getsym();
		printlex(); //打印=
		getsym();
		expr();
		printlex(); //打印分号
		getsym();
		condition();
		printlex();//打印分号
		getsym();
		printlex(); //打印标识符
		getsym();
		printlex(); //打印=
		getsym();
		printlex(); //打印标识符
		getsym();
		printlex(); //打印+-
		getsym();
		steplen();
		printlex();//打印）
		getsym();
		sta();
	}
	printsyn("<循环语句>");
}

//步长*
void steplen() {
	if (symbol != "INTCON") error();
	usINT();
	printsyn("<步长>");
}

//有返回值函数调用语句*
void callrfun() {
	if (symbol != "IDENFR") error();
	printlex(); //打印标识符
	getsym();
	printlex(); //打印（
	getsym();
	vpt();
	printlex(); //打印） 
	printsyn("<有返回值函数调用语句>");
	getsym();
}

//无返回值函数调用语句*
void callnfun() {
	if (symbol != "IDENFR") error();
	printlex(); //打印标识符
	getsym();
	printlex(); //打印（
	getsym();
	vpt();
	printlex(); //打印） 
	printsyn("<无返回值函数调用语句>");
	getsym();
}

//值参数表*
void vpt() {
	if (symbol != "RPARENT") {
		expr();
		while (1) {
			if (symbol != "COMMA")break;
			printlex(); //打印逗号
			getsym();
			expr();
		}
	}
	printsyn("<值参数表>");
}

//语句列*
void stas() {
	while (symbol != "RBRACE") sta();
	printsyn("<语句列>");
}

//读语句*
void rdsta() {
	if (symbol != "SCANFTK") error();
	printlex(); //打印scanf
	getsym();
	printlex(); //打印（
	getsym();
	printlex(); //打印标识符
	while (1) {
		getsym();
		if (symbol == "RPARENT")break;
		printlex(); //打印逗号
		getsym();
		printlex(); //打印标识符
	}
	printlex(); //打印）
	printsyn("<读语句>");
	getsym();
}

//写语句*
void wtsta() {
	if (symbol != "PRINTFTK") error();
	printlex(); //打印printf
	getsym();
	printlex(); //打印（
	getsym();
	if (symbol == "STRCON") {
		str();
		if (symbol == "COMMA") {
			printlex(); //打印逗号
			getsym();
			expr();
		}
	}
	else expr();
	printlex(); // 打印）
	printsyn("<写语句>");
	getsym();
}

//返回语句*
void rsta() {
	if (symbol != "RETURNTK") error();
	printlex();
	getsym();
	if (symbol != "SEMICN") {
		printlex(); //打印（
		getsym();
		expr();
		printlex(); //打印）
		getsym();
	}
	printsyn("<返回语句>");
}

//程序*
void program() {
	//常量说明
	if (symbol == "CONSTTK") {
		cs();
	}
	//变量说明
	if ((symbol == "INTTK" || symbol == "CHARTK") && !preload(2)) {
		vs();
	}
	//有|无返回值函数定义
	while (!(symbol == "VOIDTK" && preload(1) == 2)) {
		if ((symbol == "INTTK" || symbol == "CHARTK") && preload(2))rfun();
		else nfun();
	}
	mfun();
	printsyn("<程序>");
}

int main() {
	in = fopen("testfile.txt", "rt+");
	out = fopen("output.txt", "w");
	getsym();
	program();
	fclose(in);
	fclose(out);
	return 0;
}