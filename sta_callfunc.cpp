#include "syntax_analysis.h"

//有返回值函数调用语句*
void callrfun() {
	if (!HandleError) {
		string funcname;
		if (symbol != "IDENFR") error();
		emit("Fcall", "", "", "");
		printlex(); //打印标识符
		funcname = token;
		getsym();
		printlex(); //打印（
		getsym();
		vpt(funcname);
		printlex(); //打印）
		emit("Call", "", funcname, "");
		printsyn("<有返回值函数调用语句>");
		getsym();
	}
	//else {
	//	string func_name;
	//	if (symbol != "IDENFR") error();
	//	func_name = token;
	//	printlex(); //打印标识符
	//	getsym();
	//	printlex(); //打印（
	//	getsym();
	//	vpt(func_name, syt);
	//	if (symbol != "RPARENT") {
	//		syntax_error('l', 1); //应为右小括号
	//	}
	//	else {
	//		printlex(); //打印)
	//		getsym();
	//	}
	//	printsyn("<有返回值函数调用语句>");
	//}
}

//无返回值函数调用语句*
void callnfun() {
	if (!HandleError) {
		string funcname;
		if (symbol != "IDENFR") error();
		emit("Fcall", "", "", "");
		printlex(); //打印标识符
		funcname = token;
		getsym();
		printlex(); //打印（
		getsym();
		vpt(funcname);
		printlex(); //打印） 
		emit("Call", "", funcname, "");
		printsyn("<无返回值函数调用语句>");
		getsym();
	}
	//else {
	//	string func_name;
	//	if (symbol != "IDENFR") error();
	//	func_name = token;
	//	printlex(); //打印标识符
	//	getsym();
	//	printlex(); //打印（
	//	getsym();
	//	vpt(func_name, syt);
	//	if (symbol != "RPARENT") {
	//		syntax_error('l', 1); //应为右小括号
	//	}
	//	else {
	//		printlex(); //打印)
	//		getsym();
	//	}
	//	printsyn("<无返回值函数调用语句>");
	//}
}

//值参数表*
void vpt(string func_name) {
	if (!HandleError) {
		if (symbol != "RPARENT") {
			string regName;
			expr(regName);
			emit("Push", regName, "", "");
			while (1) {
				if (symbol != "COMMA")break;
				printlex(); //打印逗号
				getsym();
				expr(regName);
				emit("Push", regName, "", "");
			}
		}
		printsyn("<值参数表>");
	}
	//else {
	//	int count = 0;
	//	string ret_types[N];
	//	if (symbol != "RPARENT" && symbol != "SEMICN") {
	//		/* 错误处理 */
	//		//根据syt和global处理名字未定义
	//		ret_types[count++] = expr(syt);
	//		while (1) {
	//			if (symbol != "COMMA")break;
	//			printlex(); //打印逗号
	//			getsym();
	//			ret_types[count++] = expr(syt);
	//		}
	//	}
	//	int index = global.search_vname(func_name);
	//	/* 错误处理 */
	//	//函数参数个数不匹配
	//	if (!global.sym[index].pt.check_length(count)) {
	//		syntax_error('d', 1);
	//	}
	//	else {
	//		//参数个数大于0且函数参数类型不匹配
	//		if (count && !global.sym[index].pt.check_type(ret_types)) {
	//			syntax_error('e', 1);
	//		}
	//	}
	//	printsyn("<值参数表>");
	//}
}