#include "syntax_analysis.h"

//表达式*
string expr(string& regName) {
	if (!HandleError) {
		bool charflag = true;
		bool negFlag = false;
		if (symbol == "PLUS" || symbol == "MINU") {
			printlex();
			if (symbol == "MINU") negFlag = true;
			charflag = false;
			getsym();
		}
		string op, reg_1, reg_2, reg_3;
		if (term(reg_1) == "INTTK") charflag = false;
		if (negFlag) {
			reg_3 = newReg();
			emit("-", "0", reg_1, reg_3);
			reg_1 = reg_3;
		}
		while (1) {
			if (symbol != "PLUS" && symbol != "MINU")break;
			charflag = false;
			op = token;
			printlex(); //打印正负号
			getsym();
			term(reg_2);
			reg_3 = newReg();
			emit(op, reg_1, reg_2, reg_3);
			reg_1 = reg_3;
		}
		regName = reg_1;
		printsyn("<表达式>");
		if (charflag) return "CHARTK";
		else return "INTTK";
	}
	else {
		string ret_v;
		int flag = 0;
		int count = 0;
		if (symbol == "PLUS" || symbol == "MINU") {
			printlex(); //打印正负号
			flag = 1;
			getsym();
		}
		while (1) {
			string temp;
			ret_v = term(temp);
			count++;
			if (symbol != "PLUS" && symbol != "MINU") break;
			printlex(); //打印正负号
			getsym();
		}
		printsyn("<表达式>");
		if (count > 1 || flag)ret_v = "INTTK";
		return ret_v;
	}
}

//项*
string term(string& regName) {
	if (!HandleError) {
		string ret_type;
		bool charflag = true;
		string op, reg_1, reg_2, reg_3;
		if (factor(reg_1) == "INTTK")charflag = false;
		while (symbol == "MULT" || symbol == "DIV") {
			charflag = false;
			op = token;
			printlex();
			getsym();
			factor(reg_2);
			reg_3 = newReg();
			emit(op, reg_1, reg_2, reg_3);
			reg_1 = reg_3;
		}
		regName = reg_1;
		printsyn("<项>");
		if (charflag)return "CHARTK";
		else return "INTTK";
	}
	else {
		string ret_v = "INTTK";
		int count = 0;
		string temp;
		ret_v = factor(temp);
		count++;
		while (symbol == "MULT" || symbol == "DIV") {
			printlex(); //打印乘除号
			getsym();
			factor(temp);
			count++;
		}
		printsyn("<项>");
		if (count > 1)ret_v = "INTTK";
		return ret_v;
	}
}

//因子*
string factor(string& regName) {
	if (!HandleError) {
		string ret_type;
		if (symbol == "IDENFR") {
			string name = token;
			//有返回值调用函数
			if (preload(1) == "LPARENT") {
				ret_type = syt[0].sym[syt[0].search_name(name)].ret_type;
				if (ret_type == "CHARTK"||ret_type == "INTTK") callrfun();
				regName = newReg();
				emit("Fetch", "", "", regName);
				printsyn("<因子>");
			}
			else if (preload(1) == "LBRACK") { //＜标识符＞‘[’＜表达式＞‘]’
				if (syt[level].search_name(name) != -1) ret_type = syt[level].sym[syt[level].search_name(name)].ret_type;
				else ret_type = syt[0].sym[syt[0].search_name(name)].ret_type;
				printlex(); //打印标识符
				getsym();
				printlex(); //打印[
				getsym();
				string reg_index;
				expr(reg_index);
				regName = newReg();
				emit("[]", name, reg_index, regName);
				printlex(); //打印]
				getsym();
				printsyn("<因子>");
			}
			else {   //变量名
				if (syt[level].search_name(name) != -1) ret_type = syt[level].sym[syt[level].search_name(name)].ret_type;
				else ret_type = syt[0].sym[syt[0].search_name(name)].ret_type;
				printlex(); //打印标识符
				regName = name;
				getsym();
			}
		}
		else if (symbol == "LPARENT") {
			printlex(); //打印（
			getsym();
			ret_type = "INTTK";
			expr(regName);
			printlex(); //打印）
			getsym();
		}
		else if (symbol == "PLUS" || symbol == "MINU" || symbol == "INTCON") {
			INT(regName);
			ret_type = "INTTK";
		}
		else if (symbol == "CHARCON") {
			printlex();
			regName = getAscill();
			getsym();
			ret_type = "CHARTK";
		}
		printsyn("<因子>");
		return ret_type;
	}
	else {
		string ret_v = "INTTK";
		if (symbol == "IDENFR") {
			//有返回值调用函数
			if (preload(1) == "LPARENT") {
				int index = syt[0].search_name(token);
				/* 错误处理 */
				if (index == -1) {
					syntax_error('c', 3);
					while (symbol != "RPARENT")getsym();
					getsym();
					return ret_v;
				}
				ret_v = syt[0].sym[index].ret_type;
				callrfun();
				printsyn("<因子>");
				return ret_v;
			}
			//这里出现的标识符可能是变量常量或数组
			int index_g = syt[0].search_name(token);
			int index_l = syt[level].search_name(token);
			/* 错误处理 */
			if (index_g == -1 && index_l == -1) {
				syntax_error('c', 2);
			}
			if (index_g != -1) ret_v = syt[0].sym[index_g].ret_type;
			if (index_l != -1) ret_v = syt[level].sym[index_l].ret_type;
			printlex(); //打印标识符
			getsym();
			//标识符[表达式]
			if (symbol == "LBRACK") {
				printlex(); //打印[
				getsym();
				/* 错误处理 */
				string temp;
				if (expr(temp) != "INTTK") {
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
			string temp;
			expr(temp);
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
			string temp;
			INT(temp);
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
}