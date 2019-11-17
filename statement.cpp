#include "syntax_analysis.h"

//复合语句*
void costa() {
	if (!HandleError) {
		if (symbol == "CONSTTK")cs();
		if (symbol == "INTTK" || symbol == "CHARTK")vs();
		stas();
		printsyn("<复合语句>");
	}
	//else {
	//	hasreturn = false;
	//	if (symbol == "CONSTTK")cs(p);
	//	if (symbol == "INTTK" || symbol == "CHARTK")vs(p);
	//	stas(p);
	//	if (p.sym[0].ret_type != "VOIDTK" && !hasreturn) {
	//		syntax_error('h', 1); //有返回值函数没有return语句
	//	}
	//	printsyn("<复合语句>");
	//}
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
	//else {
	//	int ret_v = 0; //标识该语句是否是返回语句
	//	if (symbol == "IDENFR") {
	//		if (preload(1) == "LPARENT") {  //函数调用
	//			/* 错误处理 */
	//			if (global.search_nfun(token) != -1) callnfun(p);
	//			else if (global.search_rfun(token) != -1) callrfun(p);
	//			else {
	//				syntax_error('c', 1); //未定义的函数
	//				while (symbol != "RPARENT")getsym(); //跳到函数调用语句尾部
	//				getsym();
	//			}
	//			/* 错误处理 */
	//			if (symbol != "SEMICN") {
	//				syntax_error('k', 1); //应为分号
	//			}
	//			else {
	//				printlex(); //打印分号
	//				getsym();
	//			}
	//		}
	//		else {	//赋值语句
	//			assta(p);
	//			/* 错误处理 */
	//			if (symbol != "SEMICN") {
	//				syntax_error('k', 1); //应为分号
	//			}
	//			else {
	//				printlex(); //打印分号
	//				getsym();
	//			}
	//		}
	//	}
	//	else if (symbol == "WHILETK" || symbol == "FORTK" || symbol == "DOTK") {
	//		lpsta(p);
	//	}
	//	else if (symbol == "IFTK") {
	//		cdsta(p);
	//	}
	//	else if (symbol == "RETURNTK") {
	//		rsta(p);
	//		/* 错误处理 */
	//		if (symbol != "SEMICN") {
	//			syntax_error('k', 1); //应为分号
	//		}
	//		else {
	//			printlex(); //打印分号
	//			getsym();
	//		}
	//		ret_v = 1;
	//	}
	//	else if (symbol == "SCANFTK") {
	//		rdsta(p);
	//		/* 错误处理 */
	//		if (symbol != "SEMICN") {
	//			syntax_error('k', 1); //应为分号
	//		}
	//		else {
	//			printlex(); //打印分号
	//			getsym();
	//		}
	//	}
	//	else if (symbol == "PRINTFTK") {
	//		wtsta(p);
	//		/* 错误处理 */
	//		if (symbol != "SEMICN") {
	//			syntax_error('k', 1); //应为分号
	//		}
	//		else {
	//			printlex(); //打印分号
	//			getsym();
	//		}
	//	}
	//	else if (symbol == "LBRACE") {
	//		printlex(); //打印 {
	//		getsym();
	//		stas(p);
	//		printlex(); //打印 }
	//		getsym();
	//	}
	//	else { //空语句
	//		/* 错误处理 */
	//		if (symbol != "SEMICN") {
	//			syntax_error('k', 1); //应为分号
	//		}
	//		else {
	//			printlex(); //打印分号
	//			getsym();
	//		}
	//	}
	//	printsyn("<语句>");
	//	return ret_v;
	//}
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
	//else {
	//	if (symbol != "IDENFR") error();
	//	/* 错误处理 */
	//	int index_l = p.search_vname(token);
	//	int index_g = global.search_vname(token);
	//	if (index_l == -1 && index_g == -1) {
	//		syntax_error('c', 3);  //名字未定义
	//	}
	//	if (index_l != -1) {
	//		if (p.sym[index_l].type == "CONSTTK") {
	//			syntax_error('j', 1); //改变常量的值
	//		}
	//	}
	//	if (index_l == -1 && index_g != -1) {
	//		if (global.sym[index_g].type == "CONSTTK") {
	//			syntax_error('j', 2); //改变常量的值
	//		}
	//	}
	//	printlex(); //打印标识符
	//	getsym();
	//	if (symbol == "ASSIGN") {
	//		printlex();
	//		getsym();
	//		expr(p);
	//	}
	//	else {
	//		printlex(); //打印[
	//		getsym();
	//		if (expr(p) != "INTTK") {
	//			syntax_error('i', 1);
	//		}
	//		/* 错误处理 */
	//		if (symbol != "RBRACK") {
	//			syntax_error('m', 1); //应为右中括号
	//		}
	//		else {
	//			printlex(); //打印]
	//			getsym();
	//		}
	//		printlex(); //打印=
	//		getsym();
	//		expr(p);
	//	}
	//	printsyn("<赋值语句>");
	//}
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
	//else {
	//	if (symbol != "IFTK")error();
	//	printlex();
	//	getsym();
	//	printlex(); //打印（
	//	getsym();
	//	condition(p);
	//	if (symbol != "RPARENT") {
	//		syntax_error('l', 1); //应为右小括号
	//	}
	//	else {
	//		printlex(); //打印)
	//		getsym();
	//	}
	//	sta(p);
	//	if (symbol == "ELSETK") {
	//		printlex(); //打印else
	//		getsym();
	//		sta(p);
	//	}
	//	printsyn("<条件语句>");
	//}
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
	//else {
	//	string l_type = "INTTK";
	//	string r_type = "INTTK";
	//	l_type = expr(p);
	//	if (symbol == "LSS" || symbol == "LEQ"
	//		|| symbol == "GRE" || symbol == "GEQ"
	//		|| symbol == "NEQ" || symbol == "EQL") {
	//		printlex(); //打印<>=
	//		getsym();
	//		r_type = expr(p);
	//	}
	//	/* 错误处理 */
	//	if (l_type != "INTTK" || r_type != "INTTK") {
	//		syntax_error('f', 1);
	//	}
	//	printsyn("<条件>");
	//}
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
			emit("Jmp", lp_entry, "", "");
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
	//else {
	//	if (symbol == "whiletk") {
	//		printlex(); //打印while
	//		getsym();
	//		printlex(); //打印（
	//		getsym();
	//		condition(p);
	//		if (symbol != "rparent") {
	//			syntax_error('l', 1); //应为右小括号
	//		}
	//		else {
	//			printlex(); //打印)
	//			getsym();
	//		}
	//		sta(p);
	//	}
	//	else if (symbol == "dotk") {
	//		printlex(); //打印do
	//		getsym();
	//		sta(p);
	//		/* 错误处理 */
	//		if (symbol != "whiletk") {
	//			syntax_error('n', 1);  //缺少while
	//		}
	//		else {
	//			printlex(); //打印while
	//			getsym();
	//		}
	//		if (symbol == "lparent") {
	//			printlex();//打印（
	//			getsym();
	//			condition(p);
	//			if (symbol != "rparent") {
	//				syntax_error('l', 1); //应为右小括号
	//			}
	//			else {
	//				printlex(); //打印)
	//				getsym();
	//			}
	//		}
	//	}
	//	else if (symbol == "fortk") {
	//		printlex(); //打印for
	//		getsym();
	//		printlex(); //打印（
	//		getsym();
	//		/* 错误处理 */
	//		int index_l = p.search_vname(token);
	//		int index_g = global.search_vname(token);
	//		if (index_l == -1 && index_g == -1) {
	//			syntax_error('c', 3);  //名字未定义
	//		}
	//		if (index_l != -1) {
	//			if (p.sym[index_l].type == "consttk") {
	//				syntax_error('j', 1); //改变常量的值
	//			}
	//		}
	//		if (index_l == -1 && index_g != -1) {
	//			if (global.sym[index_g].type == "consttk") {
	//				syntax_error('j', 2); //改变常量的值
	//			}
	//		}
	//		printlex(); //打印标识符
	//		getsym();
	//		printlex(); //打印=
	//		getsym();
	//		expr(p);
	//		/* 错误处理 */
	//		if (symbol != "semicn") {
	//			syntax_error('k', 0); //应为分号
	//		}
	//		else {
	//			printlex(); //打印分号
	//			getsym();
	//		}
	//		condition(p);
	//		/* 错误处理 */
	//		if (symbol != "semicn") {
	//			syntax_error('k', 0); //应为分号
	//		}
	//		else {
	//			printlex(); //打印分号
	//			getsym();
	//		}
	//		/* 错误处理 */
	//		index_l = p.search_vname(token);
	//		index_g = global.search_vname(token);
	//		if (index_l == -1 && index_g == -1) {
	//			syntax_error('c', 3);  //名字未定义
	//		}
	//		if (index_l != -1) {
	//			if (p.sym[index_l].type == "consttk") {
	//				syntax_error('j', 1); //改变常量的值
	//			}
	//		}
	//		if (index_l == -1 && index_g != -1) {
	//			if (global.sym[index_g].type == "consttk") {
	//				syntax_error('j', 2); //改变常量的值
	//			}
	//		}
	//		printlex(); //打印标识符
	//		getsym();
	//		printlex(); //打印=
	//		getsym();
	//		printlex(); //打印标识符
	//		getsym();
	//		printlex(); //打印+-
	//		getsym();
	//		steplen(p);
	//		/* 错误处理 */
	//		if (symbol != "rparent") {
	//			syntax_error('l', 1); //应为右小括号
	//		}
	//		else {
	//			printlex(); //打印)
	//			getsym();
	//		}
	//		sta(p);
	//	}
	//	printsyn("<循环语句>");
	//}
}

//步长*
void steplen(string& num) {
	if (!HandleError) {
		num = "";
		if (symbol != "INTCON") error();
		usINT(num);
		printsyn("<步长>");
	}
	/*else {
		num = "";
		if (symbol != "INTCON") error();
		usINT(num);
		printsyn("<步长>");
	}*/
}

//语句列*
void stas() {
	if (!HandleError) {
		int curRegNum = getRegNum();
		while (symbol != "RBRACE") sta();
		resetRegNum(curRegNum);
		printsyn("<语句列>");
	}
	/*else {
		int hasreturn = 0;
		while (symbol != "RBRACE") {
			int temp = sta(p);
			if (temp == 1)hasreturn = 1;
		}
		printsyn("<语句列>");
	}*/
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
			emit("Scan", name, "", "");
			printlex(); //打印标识符
		}
		printlex(); //打印）
		printsyn("<读语句>");
		getsym();
	}
	//else {
	//	if (symbol != "SCANFTK") error();
	//	printlex(); //打印scanf
	//	getsym();
	//	printlex(); //打印（
	//	getsym();
	//	/* 错误处理 */
	//	if (p.search_vname(token) == -1 && global.search_vname(token) == -1) {
	//		syntax_error('c', 4);
	//	}
	//	printlex(); //打印标识符
	//	while (1) {
	//		getsym();
	//		if (symbol == "RPARENT")break;
	//		if (symbol == "SEMICN") {
	//			syntax_error('l', 1);
	//			break;
	//		}
	//		printlex(); //打印逗号
	//		getsym();
	//		/* 错误处理 */
	//		if (p.search_vname(token) == -1 && global.search_vname(token) == -1) {
	//			syntax_error('c', 4);
	//		}
	//		printlex(); //打印标识符
	//	}
	//	if (symbol != "RPARENT") {
	//		syntax_error('l', 1); //应为右小括号
	//	}
	//	else {
	//		printlex(); //打印)
	//		getsym();
	//	}
	//	printsyn("<读语句>");
	//}
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
	//else {
	//	if (symbol != "PRINTFTK") error();
	//	printlex(); //打印printf
	//	getsym();
	//	printlex(); //打印（
	//	getsym();
	//	if (symbol == "STRCON") {
	//		str();
	//		if (symbol == "COMMA") {
	//			printlex(); //打印逗号
	//			getsym();
	//			expr(p);
	//		}
	//	}
	//	else expr(p);
	//	if (symbol != "RPARENT") {
	//		syntax_error('l', 1); //应为右小括号
	//	}
	//	else {
	//		printlex(); //打印)
	//		getsym();
	//	}
	//	printsyn("<写语句>");
	//}
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
	//else {
	//	hasreturn = true;
	//	string ret_type;
	//	if (symbol != "RETURNTK") error();
	//	printlex(); //打印return
	//	getsym();
	//	if (symbol == "SEMICN") {
	//		if (p.sym[0].ret_type != "VOIDTK") {
	//			syntax_error('h', 1); //有返回值函数存在不匹配的return语句
	//		}
	//	}
	//	if (symbol == "LPARENT") {
	//		/* 错误处理 */
	//		if (p.sym[0].ret_type == "VOIDTK") {
	//			syntax_error('g', 1); //无返回值函数存在不匹配的return语句
	//		}
	//		printlex(); //打印（
	//		getsym();
	//		ret_type = expr(p);
	//		/* 错误处理 */
	//		if (ret_type != p.sym[0].ret_type && p.sym[0].ret_type != "VOIDTK") {
	//			syntax_error('h', 1); //有返回值函数存在不匹配的return语句
	//		}
	//		if (symbol != "RPARENT") {
	//			syntax_error('l', 1); //应为右小括号
	//		}
	//		else {
	//			printlex(); //打印)
	//			getsym();
	//		}
	//	}
	//	printsyn("<返回语句>");
	//}
}