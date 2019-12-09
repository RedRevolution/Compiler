#include "syntax_analysis.h"

//复合语句*
void costa() {
	if (!HandleError) {
		if (symbol == "CONSTTK")cs();
		if (symbol == "INTTK" || symbol == "CHARTK")vs();
		stas();
		printsyn("<复合语句>");
	}
	else {
		hasreturn = false;
		if (symbol == "CONSTTK")cs();
		if (symbol == "INTTK" || symbol == "CHARTK")vs();
		stas();
		if (syt[level].sym[0].ret_type != "VOIDTK" && !hasreturn) {
			syntax_error('h', 1); //有返回值函数没有return语句
		}
		printsyn("<复合语句>");
	}
}


//语句*
int sta() {
	if(!HandleError){
		if (symbol == "IDENFR") {
			if (preload(1) == "LPARENT") {  //函数调用
				callrfun();
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
		return 0;
	}
	else {
		int ret_v = 0; //标识该语句是否是返回语句
		if (symbol == "IDENFR") {
			if (preload(1) == "LPARENT") {  //函数调用
				/* 错误处理 */
				int index = syt[0].search_name(token);
				if (index == -1) {
					syntax_error('c', 1); //未定义的函数
					while (symbol != "RPARENT")getsym(); //跳到函数调用语句尾部
					getsym();
				}
				else if (syt[0].sym[index].ret_type == "VOIDTK") callnfun();
				else callrfun();

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
				assta();
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
			lpsta();
		}
		else if (symbol == "IFTK") {
			cdsta();
		}
		else if (symbol == "RETURNTK") {
			rsta();
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
			rdsta();
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
			wtsta();
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
			stas();
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
}

//赋值语句*
void assta() {
	if (!HandleError) {
		if (symbol != "IDENFR") error();
		string l_name, r_name;
		l_name = token;
		printlex(); //打印标识符
		getsym();
		if (symbol == "ASSIGN") {
			printlex();
			getsym();
			expr(r_name);
			emit("=", l_name, r_name, ""); 
		}
		else {
			string arr_index;
			printlex(); //打印[
			getsym();
			expr(arr_index);
			printlex(); //打印]
			getsym();
			printlex(); //打印=
			getsym();
			expr(r_name);
			emit("[]=", l_name, arr_index, r_name);
		}
		printsyn("<赋值语句>");
	}
	else {
		if (symbol != "IDENFR") error();
		/* 错误处理 */
		int index_l = syt[level].search_name(token);
		int index_g = syt[0].search_name(token);
		if (index_l == -1 && index_g == -1) {
			syntax_error('c', 3);  //名字未定义
		}
		if (index_l != -1) {
			if (syt[level].sym[index_l].type == "CONSTTK") {
				syntax_error('j', 1); //改变常量的值
			}
		}
		if (index_l == -1 && index_g != -1) {
			if (syt[0].sym[index_g].type == "CONSTTK") {
				syntax_error('j', 2); //改变常量的值
			}
		}
		printlex(); //打印标识符
		getsym();
		if (symbol == "ASSIGN") {
			printlex();
			getsym();
			string temp;
			expr(temp);
		}
		else {
			printlex(); //打印[
			getsym();
			string temp;
			if (expr(temp) != "INTTK") {
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
			expr(temp);
		}
		printsyn("<赋值语句>");
	}
}

//条件语句*
void cdsta() {
	if (!HandleError) {
		string jum_lab = "", last_lab = "",temp;
		if (symbol != "IFTK")error();
		printlex();
		getsym();
		printlex(); //打印（
		getsym();
		condition(jum_lab,false,temp);
		printlex(); //打印）
		getsym();
		sta();
		last_lab = jum_lab;

		if (symbol == "ELSETK") {
			jum_lab = newLab();
			printlex();
			emit("Jmp", jum_lab, "", "");
			emit("Lab", last_lab, "", "");

			getsym();
			sta();
			emit("Lab", jum_lab, "", "");
		}
		else {
			emit("Lab", jum_lab, "", "");
		}
		printsyn("<条件语句>");
	}
	else {
		if (symbol != "IFTK")error();
		printlex();
		getsym();
		printlex(); //打印（
		getsym();
		string temp1, temp3;
		bool temp2;
		condition(temp1,temp2,temp3);
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //应为右小括号
		}
		else {
			printlex(); //打印)
			getsym();
		}
		sta();
		if (symbol == "ELSETK") {
			printlex(); //打印else
			getsym();
			sta();
		}
		printsyn("<条件语句>");
	}
}

//条件*
void condition(string& jumlab1, bool isFor, string& jumlab2) {
	if (!HandleError){
		string l_name, op, r_name;
		expr(l_name);
		if (symbol == "LSS" || symbol == "LEQ"
			|| symbol == "GRE" || symbol == "GEQ"
			|| symbol == "NEQ" || symbol == "EQL") {
			op = symbol;
			printlex();
			getsym();
			expr(r_name);
			jumlab1 = newLab();
			if (op == "NEQ") {
				emit("==", l_name, r_name, jumlab1);
				if (isFor) emit("!=", l_name, r_name, jumlab2);
			}
			else if (op == "EQL") {
				emit("!=", l_name, r_name, jumlab1);
				if (isFor) emit("==", l_name, r_name, jumlab2);
			}
			else if (op == "LSS") {
				emit(">=", l_name, r_name, jumlab1);
				if (isFor) emit("<", l_name, r_name, jumlab2);
			}
			else if (op == "LEQ") {
				emit(">", l_name, r_name, jumlab1);
				if (isFor) emit("<=", l_name, r_name, jumlab2);
			}
			else if (op == "GRE") {
				emit("<=", l_name, r_name, jumlab1);
				if (isFor) emit(">", l_name, r_name, jumlab2);
			}
			else if (op == "GEQ") {
				emit("<", l_name, r_name, jumlab1);
				if (isFor) emit(">=", l_name, r_name, jumlab2);
			}
		}
		else {
			jumlab1 = newLab();
			emit("==", l_name, "0", jumlab1);
		}
		printsyn("<条件>");
	}
	else {
		string l_type = "INTTK";
		string r_type = "INTTK";
		string temp;
		l_type = expr(temp);
		if (symbol == "LSS" || symbol == "LEQ"
			|| symbol == "GRE" || symbol == "GEQ"
			|| symbol == "NEQ" || symbol == "EQL") {
			printlex(); //打印<>=
			getsym();
			r_type = expr(temp);
		}
		/* 错误处理 */
		if (l_type != "INTTK" || r_type != "INTTK") {
			syntax_error('f', 1);
		}
		printsyn("<条件>");
	}
}

//循环语句*
void lpsta() {
	if (!HandleError) {
		if (symbol == "WHILETK") {
			string jum_lab = newLab();
			string top_lab = jum_lab;
			string temp;
			emit("Lab", top_lab, "", "");
			printlex();
			getsym();
			printlex(); //打印（
			getsym();
			condition(jum_lab, false, temp);
			printlex(); //打印）
			getsym();
			sta();
			emit("Jmp", top_lab, "", "");
			emit("Lab", jum_lab, "", "");
		}
		else if (symbol == "DOTK") {
			string jum_lab = newLab();
			string top_lab = jum_lab;
			string temp;
			emit("Lab", top_lab, "", "");
			printlex();
			getsym();
			sta();
			printlex(); //打印while
			getsym();
			printlex();//打印（
			getsym();
			condition(jum_lab,false,temp);
			printlex(); //打印）
			getsym();
			emit("Jmp", top_lab, "", "");
			emit("Lab", jum_lab, "", "");
		}
		else if (symbol == "FORTK") {
			printlex();
			getsym();
			printlex(); //打印（

			string con_entry, add_entry;
			string lp_entry, lp_end;
			con_entry = newLab();
			add_entry = newLab();
			lp_entry = newLab();
			lp_end = newLab();

			//part1
			string l_name, r_name;
			getsym();
			l_name = token;
			printlex(); //打印标识符
			getsym();
			printlex(); //打印=
			getsym();
			expr(r_name);
			emit("=", l_name, r_name, "");
			//emit("Jmp", lp_entry, "", "");
			emit("Lab", con_entry, "", "");
			printlex(); //打印分号

			//part2
			getsym();
			condition(lp_end,true,lp_entry);
			printlex();//打印分号

			//part3
			string ll_name, rr_name, op, num;
			emit("Lab", add_entry, "", "");
			getsym();
			printlex(); //打印标识符
			ll_name = token;
			getsym();
			printlex(); //打印=
			getsym();
			rr_name = token;
			printlex(); //打印标识符
			getsym();
			printlex(); //打印+-
			op = token;
			getsym();
			steplen(num);
			emit(op, rr_name, num, ll_name);
			emit("Jmp", con_entry, "", "");
			emit("Lab", lp_entry, "", "");
			printlex();//打印）

			//part4
			getsym();
			sta();
			emit("Jmp", add_entry, "", "");
			emit("Lab", lp_end, "", "");
		}
		printsyn("<循环语句>");
	}
	else {
		if (symbol == "WHILETK") {
			printlex(); //打印while
			getsym();
			printlex(); //打印（
			getsym();
			string temp1, temp3;
			bool temp2;
			condition(temp1,temp2,temp3);
			if (symbol != "RPARENT") {
				syntax_error('l', 1); //应为右小括号
			}
			else {
				printlex(); //打印)
				getsym();
			}
			sta();
		}
		else if (symbol == "DOTK") {
			printlex(); //打印do
			getsym();
			sta();
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
				string temp1, temp3;
				bool temp2;
				condition(temp1,temp2,temp3);
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
			int index_l = syt[level].search_name(token);
			int index_g = syt[0].search_name(token);
			if (index_l == -1 && index_g == -1) {
				syntax_error('c', 3);  //名字未定义
			}
			if (index_l != -1) {
				if (syt[level].sym[index_l].type == "CONSTTK") {
					syntax_error('j', 1); //改变常量的值
				}
			}
			if (index_l == -1 && index_g != -1) {
				if (syt[0].sym[index_g].type == "CONSTTK") {
					syntax_error('j', 2); //改变常量的值
				}
			}
			printlex(); //打印标识符
			getsym();
			printlex(); //打印=
			getsym();
			string temp;
			expr(temp);
			/* 错误处理 */
			if (symbol != "SEMICN") {
				syntax_error('k', 0); //应为分号
			}
			else {
				printlex(); //打印分号
				getsym();
			}
			string temp1, temp3;
			bool temp2;
			condition(temp1, temp2, temp3);
			/* 错误处理 */
			if (symbol != "SEMICN") {
				syntax_error('k', 0); //应为分号
			}
			else {
				printlex(); //打印分号
				getsym();
			}
			/* 错误处理 */
			index_l = syt[level].search_name(token);
			index_g = syt[0].search_name(token);
			if (index_l == -1 && index_g == -1) {
				syntax_error('c', 3);  //名字未定义
			}
			if (index_l != -1) {
				if (syt[level].sym[index_l].type == "CONSTTK") {
					syntax_error('j', 1); //改变常量的值
				}
			}
			if (index_l == -1 && index_g != -1) {
				if (syt[0].sym[index_g].type == "CONSTTK") {
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
			steplen(temp);
			/* 错误处理 */
			if (symbol != "RPARENT") {
				syntax_error('l', 1); //应为右小括号
			}
			else {
				printlex(); //打印)
				getsym();
			}
			sta();
		}
		printsyn("<循环语句>");
	}
}

//步长*
void steplen(string& num) {
	if (!HandleError) {
		num = "";
		if (symbol != "INTCON") error();
		usINT(num);
		printsyn("<步长>");
	}
	else {
		num = "";
		if (symbol != "INTCON") error();
		usINT(num);
		printsyn("<步长>");
	}
}

//语句列*
void stas() {
	if (!HandleError) {
		int curRegNum = getRegNum();
		while (symbol != "RBRACE") sta();
		resetRegNum(curRegNum);
		printsyn("<语句列>");
	}
	else {
		int hasreturn = 0;
		while (symbol != "RBRACE") {
			int temp = sta();
			if (temp == 1)hasreturn = 1;
		}
		printsyn("<语句列>");
	}
}

//读语句*
void rdsta() {
	if (!HandleError) {
		if (symbol != "SCANFTK") error();
		string name;
		printlex(); //打印scanf
		getsym();
		printlex(); //打印（
		getsym();
		name = token;
		printlex(); //打印标识符
		emit("Scan", name, "", "");
		while (1) {
			getsym();
			if (symbol == "RPARENT")break;
			printlex(); //打印逗号
			getsym();
			name = token;
			emit("Scan", name, "", "");
			printlex(); //打印标识符
		}
		printlex(); //打印）
		printsyn("<读语句>");
		getsym();
	}
	else {
		if (symbol != "SCANFTK") error();
		printlex(); //打印scanf
		getsym();
		printlex(); //打印（
		getsym();
		/* 错误处理 */
		if (syt[level].search_name(token) == -1 && syt[0].search_name(token) == -1) {
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
			if (syt[level].search_name(token) == -1 && syt[0].search_name(token) == -1) {
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
}

//写语句*
void wtsta() {
	if (!HandleError) {
		string print_str="", reg_name, ret_type;
		if (symbol != "PRINTFTK") error();
		printlex(); //打印printf
		getsym();
		printlex(); //打印（
		getsym();
		if (symbol == "STRCON") {
			print_str = token;
			str();
			if (symbol == "COMMA") {
				printlex(); //打印逗号
				getsym();
				ret_type = expr(reg_name);
				emit("Print", print_str, ret_type, reg_name);
			}
			else {
				emit("Print", print_str, "", "");
			}
		}
		else {
			ret_type = expr(reg_name);
			emit("Print", print_str, ret_type, reg_name);
		}
		printlex(); // 打印）
		printsyn("<写语句>");
		getsym();
	}
	else {
		string temp;
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
				expr(temp);
			}
		}
		else expr(temp);
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //应为右小括号
		}
		else {
			printlex(); //打印)
			getsym();
		}
		printsyn("<写语句>");
	}
}

//返回语句*
void rsta() {
	if (!HandleError) {
		if (symbol != "RETURNTK") error();
		printlex();
		getsym();
		if (symbol != "SEMICN") {
			string reg_name;
			printlex(); //打印（
			getsym();
			expr(reg_name);
			emit("Ret", reg_name, "", "$End_" + syt[level].sym[0].name);
			printlex(); //打印）
			getsym();
		}
		else {
			emit("Ret", "", "", "$End_" + syt[level].sym[0].name);
		}
		printsyn("<返回语句>");
	}
	else {
		hasreturn = true;
		string ret_type;
		string temp;
		if (symbol != "RETURNTK") error();
		printlex(); //打印return
		getsym();
		if (symbol == "SEMICN") {
			if (syt[level].sym[0].ret_type != "VOIDTK") {
				syntax_error('h', 1); //有返回值函数存在不匹配的return语句
			}
		}
		if (symbol == "LPARENT") {
			/* 错误处理 */
			if (syt[level].sym[0].ret_type == "VOIDTK") {
				syntax_error('g', 1); //无返回值函数存在不匹配的return语句
			}
			printlex(); //打印（
			getsym();
			ret_type = expr(temp);
			/* 错误处理 */
			if (ret_type != syt[level].sym[0].ret_type && syt[level].sym[0].ret_type != "VOIDTK") {
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
}