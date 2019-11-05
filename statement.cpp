#include "syntax_analysis.h"

//复合语句*
void costa(Symtable &p) {
	hasreturn = false;
	if (symbol == "CONSTTK")cs(p);
	if (symbol == "INTTK" || symbol == "CHARTK")vs(p);
	stas(p);
	if (p.sym[0].ret_type != "VOIDTK" && !hasreturn) {
		syntax_error('h', 1); //有返回值函数没有return语句
	}
	printsyn("<复合语句>");
}


//语句*
int sta(Symtable &p) {
	int ret_v = 0; //标识该语句是否是返回语句
	if (symbol == "IDENFR") {
		if (preload(1) == "LPARENT") {  //函数调用
			/* 错误处理 */
			if (global.search_nfun(token)!=-1) callnfun(p);
			else if (global.search_rfun(token)!=-1) callrfun(p);
			else {
				syntax_error('c', 1); //未定义的函数
				while (symbol != "RPARENT")getsym(); //跳到函数调用语句尾部
				getsym();
			}
			/* 错误处理 */
			if (symbol != "SEMICN") {
				syntax_error('k', 1); //应为分号
			}
			else {
				printlex(); //打印分号
				getsym();
			}
		}
		else {	//赋值语句
			assta(p);
			/* 错误处理 */
			if (symbol != "SEMICN") {
				syntax_error('k', 1); //应为分号
			}
			else {
				printlex(); //打印分号
				getsym();
			}
		}
	}
	else if (symbol == "WHILETK" || symbol == "FORTK" || symbol == "DOTK") {
		lpsta(p);
	}
	else if (symbol == "IFTK") {
		cdsta(p);
	}
	else if (symbol == "RETURNTK") {
		rsta(p);
		/* 错误处理 */
		if (symbol != "SEMICN") {
			syntax_error('k', 1); //应为分号
		}
		else {
			printlex(); //打印分号
			getsym();
		}
		ret_v = 1;
	}
	else if (symbol == "SCANFTK") {
		rdsta(p);
		/* 错误处理 */
		if (symbol != "SEMICN") {
			syntax_error('k', 1); //应为分号
		}
		else {
			printlex(); //打印分号
			getsym();
		}
	}
	else if (symbol == "PRINTFTK") {
		wtsta(p);
		/* 错误处理 */
		if (symbol != "SEMICN") {
			syntax_error('k', 1); //应为分号
		}
		else {
			printlex(); //打印分号
			getsym();
		}
	}
	else if (symbol == "LBRACE") {
		printlex(); //打印 {
		getsym();
		stas(p);
		printlex(); //打印 }
		getsym();
	}
	else { //空语句
		/* 错误处理 */
		if (symbol != "SEMICN") {
			syntax_error('k', 1); //应为分号
		}
		else {
			printlex(); //打印分号
			getsym();
		}
	}
	printsyn("<语句>");
	return ret_v;
}

//赋值语句*
void assta(Symtable &p) {
	if (symbol != "IDENFR") error();
	/* 错误处理 */
	int index_l = p.search_vname(token);
	int index_g = global.search_vname(token);
	if (index_l == -1 && index_g == -1) {
		syntax_error('c', 3);  //名字未定义
	}
	if (index_l != -1) {
		if (p.sym[index_l].type == "CONSTTK") {
			syntax_error('j', 1); //改变常量的值
		}
	}
	if (index_l == -1 && index_g != -1) {
		if (global.sym[index_g].type == "CONSTTK") {
			syntax_error('j', 2); //改变常量的值
		}
	}
	printlex(); //打印标识符
	getsym();
	if (symbol == "ASSIGN") {
		printlex();
		getsym();
		expr(p);
	}
	else {
		printlex(); //打印[
		getsym();
		if (expr(p) != "INTTK") {
			syntax_error('i', 1);
		}
		/* 错误处理 */
		if (symbol != "RBRACK") {
			syntax_error('m', 1); //应为右中括号
		}
		else {
			printlex(); //打印]
			getsym(); 
		}
		printlex(); //打印=
		getsym();
		expr(p);
	}
	printsyn("<赋值语句>");
}

//条件语句*
void cdsta(Symtable &p) {
	if (symbol != "IFTK")error();
	printlex();
	getsym();
	printlex(); //打印（
	getsym();
	condition(p);
	if (symbol != "RPARENT") {
		syntax_error('l', 1); //应为右小括号
	}
	else {
		printlex(); //打印)
		getsym();
	}
	sta(p);
	if (symbol == "ELSETK") {
		printlex(); //打印else
		getsym();
		sta(p);
	}
	printsyn("<条件语句>");
}

//条件*
void condition(Symtable &p) {
	string l_type = "INTTK";
	string r_type = "INTTK";
	l_type = expr(p);
	if (symbol == "LSS" || symbol == "LEQ"
		|| symbol == "GRE" || symbol == "GEQ"
		|| symbol == "NEQ" || symbol == "EQL") {
		printlex(); //打印<>=
		getsym();
		r_type = expr(p);
	}
	/* 错误处理 */
	if (l_type != "INTTK" || r_type != "INTTK") {
		syntax_error('f', 1);
	}
	printsyn("<条件>");
}

//循环语句*
void lpsta(Symtable &p) {
	if (symbol == "WHILETK") {
		printlex(); //打印while
		getsym();
		printlex(); //打印（
		getsym();
		condition(p);
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //应为右小括号
		}
		else {
			printlex(); //打印)
			getsym();
		}
		sta(p);
	}
	else if (symbol == "DOTK") {
		printlex(); //打印do
		getsym();
		sta(p);
		/* 错误处理 */
		if (symbol != "WHILETK") {
			syntax_error('n', 1);  //缺少while
		}
		else {
			printlex(); //打印while
			getsym();
		}
		if (symbol == "LPARENT") {
			printlex();//打印（
			getsym();
			condition(p);
			if (symbol != "RPARENT") {
				syntax_error('l', 1); //应为右小括号
			}
			else {
				printlex(); //打印)
				getsym();
			}
		}
	}
	else if (symbol == "FORTK") {
		printlex(); //打印for
		getsym();
		printlex(); //打印（
		getsym();

		/* 错误处理 */
		int index_l = p.search_vname(token);
		int index_g = global.search_vname(token);
		if (index_l == -1 && index_g == -1) {
			syntax_error('c', 3);  //名字未定义
		}
		if (index_l != -1) {
			if (p.sym[index_l].type == "CONSTTK") {
				syntax_error('j', 1); //改变常量的值
			}
		}
		if (index_l == -1 && index_g != -1) {
			if (global.sym[index_g].type == "CONSTTK") {
				syntax_error('j', 2); //改变常量的值
			}
		}
		
		printlex(); //打印标识符
		getsym();
		printlex(); //打印=
		getsym();
		expr(p);
		/* 错误处理 */
		if (symbol != "SEMICN") {
			syntax_error('k', 0); //应为分号
		}
		else {
			printlex(); //打印分号
			getsym();
		}
		condition(p);
		/* 错误处理 */
		if (symbol != "SEMICN") {
			syntax_error('k', 0); //应为分号
		}
		else {
			printlex(); //打印分号
			getsym();
		}

		/* 错误处理 */
		index_l = p.search_vname(token);
		index_g = global.search_vname(token);
		if (index_l == -1 && index_g == -1) {
			syntax_error('c', 3);  //名字未定义
		}
		if (index_l != -1) {
			if (p.sym[index_l].type == "CONSTTK") {
				syntax_error('j', 1); //改变常量的值
			}
		}
		if (index_l == -1 && index_g != -1) {
			if (global.sym[index_g].type == "CONSTTK") {
				syntax_error('j', 2); //改变常量的值
			}
		}
		
		printlex(); //打印标识符
		getsym();
		printlex(); //打印=
		getsym();
		printlex(); //打印标识符
		getsym();
		printlex(); //打印+-
		getsym();
		steplen(p);
		/* 错误处理 */
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //应为右小括号
		}
		else {
			printlex(); //打印)
			getsym();
		}
		sta(p);
	}
	printsyn("<循环语句>");
}

//步长*
void steplen(Symtable &p) {
	if (symbol != "INTCON") error();
	usINT();
	printsyn("<步长>");
}


//语句列*
void stas(Symtable &p) {
	int hasreturn = 0;
	while (symbol != "RBRACE") {
		int temp = sta(p);
		if (temp == 1)hasreturn = 1;
	}
	printsyn("<语句列>");
}

//读语句*
void rdsta(Symtable &p) {
	if (symbol != "SCANFTK") error();
	printlex(); //打印scanf
	getsym();
	printlex(); //打印（
	getsym();
	/* 错误处理 */
	if (p.search_vname(token)==-1 && global.search_vname(token)==-1) {
		syntax_error('c', 4);
	}
	printlex(); //打印标识符
	while (1) {
		getsym();
		if (symbol == "RPARENT")break;
		if (symbol == "SEMICN") {
			syntax_error('l', 1);
			break;
		}
		printlex(); //打印逗号
		getsym();
		/* 错误处理 */
		if (p.search_vname(token)==-1 && global.search_vname(token)==-1) {
			syntax_error('c', 4);
		}
		printlex(); //打印标识符
	}
	if (symbol != "RPARENT") {
		syntax_error('l', 1); //应为右小括号
	}
	else {
		printlex(); //打印)
		getsym();
	}
	printsyn("<读语句>");
}

//写语句*
void wtsta(Symtable &p) {
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
			expr(p);
		}
	}
	else expr(p);
	if (symbol != "RPARENT") {
		syntax_error('l', 1); //应为右小括号
	}
	else {
		printlex(); //打印)
		getsym();
	}
	printsyn("<写语句>");
}

//返回语句*
void rsta(Symtable &p) {
	hasreturn = true;
	string ret_type;
	if (symbol != "RETURNTK") error();
	printlex(); //打印return
	getsym();
	if (symbol == "SEMICN") {
		if (p.sym[0].ret_type != "VOIDTK") {
			syntax_error('h', 1); //有返回值函数存在不匹配的return语句
		}
	}
	if (symbol == "LPARENT") {
		/* 错误处理 */
		if (p.sym[0].ret_type == "VOIDTK") {
			syntax_error('g', 1); //无返回值函数存在不匹配的return语句
		}
		printlex(); //打印（
		getsym();
		ret_type = expr(p);
		/* 错误处理 */
		if (ret_type != p.sym[0].ret_type&& p.sym[0].ret_type != "VOIDTK") {
			syntax_error('h', 1); //有返回值函数存在不匹配的return语句
		}
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //应为右小括号
		}
		else {
			printlex(); //打印)
			getsym();
		}
	}
	printsyn("<返回语句>");
}