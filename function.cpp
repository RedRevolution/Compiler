#include "syntax_analysis.h"
#include "generateMcode.h"

//声明头部*
void dh(string& func_name) { 
	if (!HandleError) {
		string ret_type;
		if (symbol != "INTTK" && symbol != "CHARTK") error();
		ret_type = symbol;
		printlex();
		getsym(); //标识符
		func_name = token;
		printlex(); //打印标识符
		syt[0].add_entry(func_name, "FUNC", ret_type, to_string(level));
		printsyn("<声明头部>");
		getsym();
	}
	else {
		string ret_type;
		if (symbol != "INTTK" && symbol != "CHARTK") error();
		ret_type = symbol;
		printlex(); //打印int|char
		getsym(); //标识符
		func_name = token;
		/*错误处理*/
		if (!syt[0].add_entry(func_name, "FUNC", ret_type, to_string(level))) {
			syntax_error('b', 11);
		}
		printlex(); //打印标识符
		printsyn("<声明头部>");
		getsym();
	}
}


//参数表*
void pt(string func_name) {
	if (!HandleError) {
		string ret_type, pr_name;
		//参数不为空
		if (symbol == "INTTK" || symbol == "CHARTK") {
			ret_type = symbol;
			printlex(); //打印 int|char
			while (1) {
				getsym(); //读入标识符
				pr_name = token;
				syt[0].add_pt_entry(func_name, pr_name, ret_type);
				syt[level].add_entry(pr_name, "VAR", ret_type, "");
				emit("Para", ret_type, pr_name, "");
				printlex(); //打印标识符
				getsym(); //读入逗号或者）
				if (symbol != "COMMA")break;
				printlex(); //打印逗号
				getsym(); //读入 int|char
				ret_type = symbol;
				printlex(); //打印 int|char
			}
		}
		printsyn("<参数表>");
	}
	else {
		string pr_name;
		string ret_type;
		//参数不为空
		if (symbol == "INTTK" || symbol == "CHARTK") {
			ret_type = symbol;
			printlex(); //打印 int|char
			while (1) {
				getsym(); //读入标识符
				pr_name = token;
				//加入函数func_name的参数表
				if (!syt[0].add_pt_entry(func_name, pr_name, ret_type)) {
					/*错误处理*/
					syntax_error('b', 12);  //参数名重定义
				}
				syt[level].add_entry(pr_name, "VAR", ret_type,""); //将参数表中的参数加入局部变量区
				printlex(); //打印标识符
				getsym(); //读入逗号或者）
				if (symbol != "COMMA")break;
				printlex(); //打印逗号
				getsym(); //读入 int|char
				ret_type = symbol;
				printlex(); //打印 int|char
			}
		}
		printsyn("<参数表>");
	}
}

//有返回值函数定义*
void rfun() {
	if (!HandleError) {
		string funcname, ret_type;
		resetRegNum(0);
		if (symbol != "INTTK" && symbol != "CHARTK") error();
		ret_type = symbol;
		dh(funcname);
		printlex(); //打印（
		syt[level].add_entry(funcname, "FUNC", ret_type,to_string(level));
		emit("Func", "", funcname, "");
		getsym();
		pt(funcname);
		printlex(); //打印）
		getsym();
		printlex(); //打印{
		getsym();
		costa();
		printlex(); //打印}
		emit("End", "", "", "$End_" + funcname);
		printsyn("<有返回值函数定义>");
		getsym();
	}
	else {
		string func_name, func_type;
		if (symbol != "INTTK" && symbol != "CHARTK") error();
		func_type = symbol;
		dh(func_name);
		printlex(); //打印（
		//局部符号表的第一项注册该函数的基本信息
		syt[level].add_entry(func_name, "FUNC", func_type, to_string(level));
		getsym();
		pt(func_name);
		/* 错误处理 */
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //应为右小括号
		}
		else {
			printlex(); //打印)
			getsym();
		}
		printlex(); //打印{
		getsym();
		costa();
		printlex(); //打印}
		printsyn("<有返回值函数定义>");
		getsym();
	}
}

//无返回值函数定义*
void nfun() {
	if (!HandleError) {
		string funcname;
		if (symbol != "VOIDTK") error();
		printlex();
		getsym(); //读入标识符
		funcname = token;
		printlex(); //打印标识符
		syt[0].add_entry(funcname, "FUNC", "VOIDTK", to_string(level));
		syt[level].add_entry(funcname, "FUNC", "VOIDTK", to_string(level));
		emit("Func", "", funcname, "");
		getsym();
		printlex(); //打印（
		getsym();
		pt(funcname);
		printlex(); //打印）
		getsym();
		printlex(); //打印{
		getsym();
		costa();
		printlex(); //打印}
		emit("End", "", "", "$End_" + funcname);
		printsyn("<无返回值函数定义>");
		getsym();
	}
	else {
		string func_name;
		if (symbol != "VOIDTK") error();
		printlex(); //打印void
		getsym(); //读入标识符
		func_name = token;
		//局部符号表的第一项注册该函数的基本信息
		syt[level].add_entry(func_name, "FUNC", "VOIDTK", to_string(level));
		/*错误处理*/
		if (!syt[0].add_entry(func_name, "FUNC", "VOIDTK", to_string(level))) {
			syntax_error('b', 13);
		}
		printlex(); //打印标识符
		getsym();
		printlex(); //打印（
		getsym();
		pt(func_name);
		/* 错误处理 */
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //应为右小括号
		}
		else {
			printlex(); //打印)
			getsym();
		}
		printlex(); //打印{
		getsym();
		costa();
		printlex(); //打印}
		printsyn("<无返回值函数定义>");
		getsym();
	}
}

//主函数*
void mfun() {
	if (!HandleError) {
		if (symbol != "VOIDTK") error();
		printlex(); //打印void
		getsym(); //读入标识符main
		printlex(); //打印标识符main
		syt[0].add_entry("main", "FUNC", "VOIDTK", to_string(level));
		syt[level].add_entry("main", "FUNC", "VOIDTK", to_string(level));
		emit("Func", "", "main", "");
		getsym();
		printlex(); //打印（
		getsym();
		printlex(); //打印）
		getsym();
		printlex(); //打印{
		getsym();
		costa();
		printlex(); //打印}
		emit("End", "", "", "$End_main");
		printsyn("<主函数>"); //主函数结束无须getsym
	}
	else {
		if (symbol != "VOIDTK") error();
		printlex(); //打印void
		getsym(); //读入标识符main
		/*错误处理*/
		syt[level].add_entry("main", "FUNC", "VOIDTK", to_string(level));
		if (!syt[0].add_entry("main", "FUNC", "VOIDTK", to_string(level))) {
			syntax_error('b', 14);
		}
		printlex(); //打印标识符main
		getsym();
		printlex(); //打印（
		getsym();
		/* 错误处理 */
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //应为右小括号
		}
		else {
			printlex(); //打印)
			getsym();
		}
		printlex(); //打印{
		getsym();
		costa();
		printlex(); //打印}
		printsyn("<主函数>"); //主函数结束无须getsym
	}
}