#include "syntax_analysis.h"

//常量定义*
void cd() {
	if (!HandleError) {
		string name, ret_type;
		if (symbol != "INTTK" && symbol != "CHARTK") error();
		printlex(); //打印int|char
		ret_type = symbol;
		if (symbol == "INTTK") {
			while (1) {
				getsym(); //标识符
				name = token;
				printlex();
				getsym(); // =
				printlex();
				getsym();
				string num;
				INT(num);
				syt[level].add_entry(name, "CONSTTK", ret_type, num);
				emit("Con", ret_type, name, num);
				if (symbol != "COMMA") break; //分号退出
				printlex(); //打印逗号
			}
		}
		else {
			while (1) {
				getsym(); //标识符
				name = token;
				printlex();
				getsym(); // =
				printlex();
				getsym(); //字符
				syt[level].add_entry(name, "CONSTTK", ret_type, token);
				emit("Con", ret_type, name, token);
				printlex();
				getsym();
				if (symbol != "COMMA") break;
				printlex(); //打印逗号
			}
		}
		printsyn("<常量定义>");
	}
	else {
		string ret_type;
		string name;
		if (symbol != "INTTK" && symbol != "CHARTK") error();
		ret_type = symbol;
		printlex(); //打印int|char
		if (symbol == "INTTK") {
			while (1) {
				getsym(); //标识符	
				if (symbol != "IDENFR") {
					lexi_error('a', 1);
					while (symbol != "SEMICN" && symbol != "COMMA")getsym();
				}
				else {
					name = token;
					/*错误处理*/
					if (!syt[level].add_entry(name, "CONSTTK", ret_type,"")) {
						syntax_error('b', 1); //名字重定义
					}
					printlex(); //打印标识符
					getsym(); // =
					if (symbol != "ASSIGN") {
						lexi_error('a', 1);
						while (symbol != "ASSIGN")getsym();
					}
					printlex(); //打印=
					getsym();
					/* 错误处理 */
					//判断是不是表达式
					int preline_no = line_no;
					if (symbol == "CHARCON") {
						getsym();
					}
					else {
						string num = "";
						if (!INT(num)) {
							syntax_error('o', 1); //没有跟整形常量
							while (symbol != "COMMA" && symbol != "SEMICN")getsym();
						}
					}
					if (symbol != "COMMA" && symbol != "SEMICN") { //接下来没有跟，或者；
						if (line_no != preline_no) break;  //缺少结尾的分号
						syntax_error('k', 0);
						while (symbol != "COMMA" && symbol != "SEMICN") getsym();
					}
				}
				if (symbol != "COMMA") break; //分号退出
				printlex(); //打印逗号
			}
		}
		else {
			while (1) {
				getsym(); //标识符
				if (symbol != "IDENFR") {
					lexi_error('a', 1);
					while (symbol != "SEMICN" && symbol != "COMMA")getsym();
				}
				else {
					name = token;
					printlex(); //打印标识符
					getsym(); // =
					printlex(); //打印 =
					getsym(); //字符
					/* 错误处理 */
					int preline_no = line_no;
					if (symbol == "CHARCON") {
						printlex(); //打印字符
						getsym();
					}
					else {
						string num = "";
						if (!INT(num)) {
							syntax_error('o', 3); //没有跟字符常量
						}
					}
					if (symbol != "COMMA" && symbol != "SEMICN") { //接下来没有跟，或者；
						if (line_no == preline_no) {
							while (symbol != "COMMA" && symbol != "SEMICN") getsym();
						}
						else break;
					}
					else {
						if (!syt[level].add_entry(name, "CONSTTK", ret_type, "")) syntax_error('b', 2); //名字重定义
					}
				}
				if (symbol != "COMMA") break;
				printlex(); //打印逗号
			}
		}
		printsyn("<常量定义>");
	}
}

//常量说明*
void cs() {
	if (!HandleError) {
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
	else {
		if (symbol != "CONSTTK") error();
		printlex(); //打印const
		while (1) {
			getsym();
			cd();
			/* 错误处理 */
			if (symbol != "SEMICN") {
				syntax_error('k', 1); //应为分号
			}
			else {
				printlex(); //打印分号
				getsym();
			}
			if (symbol != "CONSTTK")break;
			printlex(); //打印const
		}
		printsyn("<常量说明>");
	}
}

//变量定义*
void vd() {
	if (!HandleError) {
		string name, ret_type;
		if (symbol != "INTTK" && symbol != "CHARTK") error();
		ret_type = symbol;
		printlex();
		while (1) {
			getsym(); //读入一个标识符
			name = token;
			printlex();
			getsym(); //可能是逗号，分号或者[
			// [
			if (symbol == "LBRACK") {
				printlex(); //打印[
				getsym(); //读入无符号整数
				string size = "";
				usINT(size);
				emit("Arr", ret_type, name, size);
				syt[level].add_entry(name, "ARRAY", ret_type, size);
				printlex(); //打印]
				getsym(); // 读入逗号|分号
			}
			else {
				emit("Var", ret_type, name, "");
				syt[level].add_entry(name, "VAR", ret_type, "");
			}
			if (symbol == "SEMICN")break; //分号退出
			printlex(); //打印逗号
		}
		printsyn("<变量定义>");
	}
	else {
		string ret_type;
		string name;
		if (symbol != "INTTK" && symbol != "CHARTK") error();
		ret_type = symbol;
		printlex(); //打印int|char
		while (1) {
			int preline_no;
			getsym(); //读入一个标识符
			/* 错误处理 */
			if (symbol != "IDENFR") {
				lexi_error('a', 1);
				while (symbol != "SEMICN" && symbol != "COMMA")getsym();
			}
			else {
				name = token;
				printlex(); //打印标识符
				preline_no = line_no;
				getsym(); //可能是逗号，分号或者[
				// [
				if (symbol == "LBRACK") {
					printlex(); //打印[
					getsym(); //读入无符号整数
					string num;
					usINT(num);
					/* 错误处理 */
					if (symbol != "RBRACK") {
						syntax_error('m', 1); //应为右中括号
					}
					else {
						printlex(); //打印]
						getsym(); // 读入逗号|分号
					}
				}
				if (symbol != "COMMA" && symbol != "SEMICN") {
					if (preline_no == line_no) {
						syntax_error('a', 0);
						while (symbol != "SEMICN" && symbol != "COMMA")getsym();
					}
					else break;
				}
				else {
					if (!syt[level].add_entry(name, "VAR", ret_type,"")) syntax_error('b', 3); //名字重定义
				}
			}
			if (symbol == "SEMICN")break; //分号退出
			printlex(); //打印逗号
		}
		printsyn("<变量定义>");
	}
}

//变量说明*
void vs() {
	if (!HandleError) {
		if (symbol != "INTTK" && symbol != "CHARTK") error();
		while (1) {
			vd();
			printlex(); //打印分号
			getsym();
			if (symbol != "INTTK" && symbol != "CHARTK")break;
			if (preload(2) == "LPARENT")break; //函数定义而非变量定义
		}
		printsyn("<变量说明>");
	}
	else {
		if (symbol != "INTTK" && symbol != "CHARTK") error();
		while (1) {
			vd();
			/* 错误处理 */
			int preline_no = line_no;
			if (symbol != "SEMICN") {
				syntax_error('k', 1); //应为分号
			}
			else {
				printlex(); //打印分号
				getsym();
			}
			if (symbol != "INTTK" && symbol != "CHARTK")break;
			if (preload(2) == "LPARENT")break; //函数定义而非变量定义
		}
		printsyn("<变量说明>");
	}
}