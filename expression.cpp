#include "syntax_analysis.h"

//表达式*
string expr(Symtable &p) {
	string ret_v;
	int flag = 0;
	int count = 0;
	if (symbol == "PLUS" || symbol == "MINU") {
		printlex(); //打印正负号
		flag = 1;
		getsym();
	}
	while (1) {
		ret_v = term(p);
		count++;
		if (symbol != "PLUS" && symbol != "MINU") break;
		printlex(); //打印正负号
		getsym();
	}
	printsyn("<表达式>");
	if (count > 1||flag)ret_v = "INTTK";
	return ret_v;
}

//项*
string term(Symtable &p) {
	string ret_v = "INTTK";
	int count = 0;
	ret_v = factor(p);
	count++;
	while (symbol == "MULT" || symbol == "DIV") {
		printlex(); //打印乘除号
		getsym();
		factor(p);
		count++;
	}
	printsyn("<项>");
	if (count > 1)ret_v = "INTTK";
	return ret_v;
}

//因子*
string factor(Symtable &p) {
	string ret_v = "INTTK";
	if (symbol == "IDENFR") {
		//有返回值调用函数
		if (preload(1) == "LPARENT") {
			int index = global.search_rfun(token);
			/* 错误处理 */
			if (index == -1) {
				syntax_error('c', 3);
				while (symbol != "RPARENT")getsym();
				getsym();
				return ret_v;
			}
			ret_v = global.sym[index].ret_type;
			callrfun(p);
			printsyn("<因子>");
			return ret_v;
		}

		//这里出现的标识符可能是变量常量或数组
		int index_g = global.search_vname(token);
		int index_l = p.search_vname(token);
		/* 错误处理 */
		if (index_g == -1&& index_l == -1) {
			syntax_error('c', 2);
		}
		if (index_g != -1) ret_v = global.sym[index_g].ret_type;
		if (index_l != -1) ret_v = p.sym[index_l].ret_type;
		printlex(); //打印标识符
		getsym();
		//标识符[表达式]
		if (symbol == "LBRACK") {
			printlex(); //打印[
			getsym();
			/* 错误处理 */
			if (expr(p) != "INTTK") {
				syntax_error('i', 1); //数组下标只能是整形
			}
			/* 错误处理 */
			if (symbol != "RBRACK") {
				syntax_error('m', 1); //应为右中括号
			}
			else {
				printlex(); //打印]
				getsym(); 
			}
		}
	}
	else if (symbol == "LPARENT") {
		printlex(); //打印（
		getsym();
		expr(p);
		/* 错误处理 */
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //应为右小括号
		}
		else {
			printlex(); //打印)
			getsym();
		}
		ret_v = "INTTK";
	}
	else if (symbol == "PLUS" || symbol == "MINU" || symbol == "INTCON") {
		INT();
		ret_v = "INTTK";
	}
	else if (symbol == "CHARCON") {
		printlex(); //打印char
		getsym();
		ret_v = "CHARTK";
	}
	printsyn("<因子>");
	return ret_v;
}