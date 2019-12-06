#include "generateObjcode.h"

int globalcnt;  //全局常变量计数器
map<string, int> global2bias;
int Index; //当前讨论的指令序列计数器
map<string, int> func2funcNo;
int curfun, paraNo;
Func_Syt func_syt[N], global;
int paraStack[N], top;

void init() {
	//字符串
	cout << ".data" << endl;
	cout << ".align 4" << endl;
	cout << "Enter: .asciiz \"\\n\"" << endl;
	int stringIndex = 0;
	for (int i = 0; i < midCodeNum; i++) {
		if (midCode[i].op == "Print" && midCode[i].arg1 != "") {
			cout << ".align 4" << endl;
			string temp;
			for (int j = 0; j < midCode[i].arg1.length(); j++) {
				temp += midCode[i].arg1[j];
				if (midCode[i].arg1[j] == '\\') {
					temp += '\\';
				}
			}
			cout << "string_" << stringIndex << ": .asciiz " << "\"" << temp << "\"" << endl;
			midCode[i].arg1 = "string_" + to_string(stringIndex++);
		}
	}

	//代码区
	cout << ".text" << endl;
	//全局常量和变量, 全局指针gp开始存储
	for (int i = 0; i < midCodeNum; i++) {
		if (midCode[i].op == "Con") {
			global.add_con(midCode[i].arg2, midCode[i].arg1, midCode[i].result);
			if (midCode[i].arg1 == "INTTK") printmips("li", "$t0", midCode[i].result, "");
			else printmips("li", "$t0", "\'"+midCode[i].result+"\'", "");
			printmips("sw", "$t0", to_string(globalcnt * 4) + "($gp)", "");
			global2bias[midCode[i].arg2] = globalcnt * 4;
			globalcnt++;
		}
		else if (midCode[i].op == "Var") {
			global.add_var(midCode[i].arg2, midCode[i].arg1);
			//printmips("sw", "$zero", to_string(globalcnt * 4) + "($gp)", ""); //初始化
			global2bias[midCode[i].arg2] = globalcnt * 4;
			globalcnt++;
		}
		else if (midCode[i].op == "Arr") {
			global.add_arr(midCode[i].arg2, midCode[i].arg1, midCode[i].result);
			//printmips("sw", "$zero", to_string(globalcnt * 4) + "($gp)", ""); //初始化
			global2bias[midCode[i].arg2] = globalcnt * 4;
			globalcnt += stoi(midCode[i].result);
		}
		else {
			Index = i;
			break;
		}
	}
	
	//函数注册
	for (int i = 0; i < midCodeNum; i++) {
		if (midCode[i].op == "Func") {
			func2funcNo[midCode[i].arg2] = curfun;
			i++;
			while (midCode[i].op == "Para") {
				func_syt[curfun].add_para(midCode[i].arg2, midCode[i].arg1);
				i++;
			}
			while (midCode[i].op == "Con") {
				func_syt[curfun].add_con(midCode[i].arg2, midCode[i].arg1, midCode[i].result);
				i++;
			}
			while (midCode[i].op == "Var" || midCode[i].op == "Arr") {
				if (midCode[i].op == "Var") 
					func_syt[curfun].add_var(midCode[i].arg2, midCode[i].arg1);
				else
					func_syt[curfun].add_arr(midCode[i].arg2, midCode[i].arg1, midCode[i].result);
				i++; 
			}
			int maxRegNum = 0;
			while (midCode[i].op != "End") {
				Max(maxRegNum, midCode[i].arg1, midCode[i].arg2, midCode[i].result);
				i++;
			}
			func_syt[curfun].setMaxRegNum(maxRegNum);
			func_syt[curfun].init();
			curfun++;
		}
	}
	
	//注册完信息后跳转到main函数
	curfun = 0;
	printmips("move", "$fp", "$sp", "");
	printmips("j", "main", "", "");
}

void Max(int& a,string b, string c, string d) {
	if (b[0] == '$' && b[1] == 't') {
		int bnum = stoi(b.substr(2)); //删去$t
		if (a < bnum) a = bnum;
	}
	if (c[0] == '$' && c[1] == 't') {
		int cnum = stoi(c.substr(2)); //删去$t
		if (a < cnum) a = cnum;
	}
	if (d[0] == '$' && d[1] == 't') {
		int dnum = stoi(d.substr(2)); //删去$t
		if (a < dnum) a = dnum;
	}
}

void printmips(string a, string b, string c, string d) {
	if (a == "") { 
		cout << endl;
		return; 
	}
	cout << "	" << a;
	if (b == "") {
		cout << endl;
		return;
	}
	cout << " " << b;
	if (c == "") {
		cout << endl;
		return;
	}
	cout << "," << c;
	if (d == "") {
		cout << endl;
		return;
	}
	cout << "," << d <<endl;
}

void Con_local() {
	string name = midCode[Index].arg2;
	string retType = midCode[Index].arg1;
	string value = midCode[Index].result;

	if (func_syt[curfun].getVarReg(name)!="") {  //局部常量保存在s0-s7中
		if(retType == "INTTK") printmips("li", func_syt[curfun].getVarReg(name), value, "");
		else printmips("li", func_syt[curfun].getVarReg(name), "\'" + value + "\'", "");
	}   
	else {  //局部常量保存在运行栈里
		if (retType == "INTTK") {
			printmips("li", "$v0", value, "");
			printmips("sw", "$v0", func_syt[curfun].getVarBias(name) + "($fp)", "");
		}
		else{
			printmips("li", "$v0", "\'"+value+"\'", "");
			printmips("sw", "$v0", func_syt[curfun].getVarBias(name) + "($fp)", "");
		}
	}
}

string preloadvar(string varname,string regname) {  //若没有寄存器对应，则将栈(运行/全局)中的值加载回regname
	if (isNum(varname)) return varname;
	int type;
	string value = loadvar(varname, type);
	if (type == 1) {
		return value;
	}
	else if (type == 2) {
		printmips("lw", regname, value + "($fp)", "");
		return regname;
	}
	else if (type == 3) {
		printmips("lw", regname, value + "($gp)", "");
		return regname;
	}
	return "";
}

string loadvar(string varname,int& type) {
	if (varname == "$zero" || func_syt[curfun].getVarReg(varname) != "") {  //局部变量分配有寄存器
		type = 1;
		if (varname == "$zero") return varname;
		else return func_syt[curfun].getVarReg(varname);
	}
	else if (func_syt[curfun].getVarBias(varname) != "") { //局部变量未分配寄存器,参数
		type = 2;
		return func_syt[curfun].getVarBias(varname);
	}
	map<string, int>::iterator iter;  //全局变量
	iter = global2bias.find(varname);
	if (iter != global2bias.end()) {
		type = 3;
		return to_string(iter->second);
	}
	type = 0;
	return "";
}

void swap(string& a, string& b) {
	string temp;
	temp = b;
	b = a;
	a = temp;
}

void Calculate() {
 	string op = midCode[Index].op;
	string arg1 = midCode[Index].arg1;
	string arg2 = midCode[Index].arg2;
	string result = midCode[Index].result;
	int type;
	string value = loadvar(result, type);

	if (arg1 == "varchar") {
		int i;
		i = 0;
	}

	if (op == "+") {
		if (isNum(arg1) && isNum(arg2)) {
			int temp = stoi(arg1) + stoi(arg2);
			if (type == 1) {
				printmips("li", value, to_string(temp), "");
			}
			else if (type == 2) {
				printmips("li", "$v0", to_string(temp), "");
				printmips("sw", "$v0", value + "($fp)", "");
			}
			else if (type == 3) {
				printmips("li", "$v0", to_string(temp), "");
				printmips("sw", "$v0", value + "($gp)", "");
			}
		}
		else {
			string arg1regname = preloadvar(arg1, "$v0");
			string arg2regname = preloadvar(arg2, "$v1");
			if (isNum(arg1regname))swap(arg1regname, arg2regname);

			if (type == 1) {
				printmips("add", value, arg1regname, arg2regname);
			}
			else if (type == 2) {
				printmips("add", "$v0", arg1regname, arg2regname);
				printmips("sw", "$v0", value + "($fp)", "");
			}
			else if (type == 3) {
				printmips("add", "$v0", arg1regname, arg2regname);
				printmips("sw", "$v0", value + "($gp)", "");
			}
		}
	}
	else if (op == "-") {
		if (isNum(arg1) && isNum(arg2)) {
			int temp = stoi(arg1) - stoi(arg2);
			if (type == 1) {
				printmips("li", value, to_string(temp), "");
			}
			else if (type == 2) {
				printmips("li", "$v0", to_string(temp), "");
				printmips("sw", "$v0", value + "($fp)", "");
			}
			else if (type == 3) {
				printmips("li", "$v0", to_string(temp), "");
				printmips("sw", "$v0", value + "($gp)", "");
			}
		}
		else {
			string arg1regname = preloadvar(arg1, "$v0");
			string arg2regname = preloadvar(arg2, "$v1");
			if (isNum(arg1regname) && !isNum(arg2regname)) {
				printmips("sub", arg2regname, "$zero", arg2regname);
				int temp = stoi(arg1regname);
				arg1regname = to_string(-1 * temp);
				swap(arg1regname, arg2regname);
			}

			if (type == 1) {
				printmips("sub", value, arg1regname, arg2regname);
			}
			else if (type == 2) {
				printmips("sub", "$v0", arg1regname, arg2regname);
				printmips("sw", "$v0", value + "($fp)", "");
			}
			else if (type == 3) {
				printmips("sub", "$v0", arg1regname, arg2regname);
				printmips("sw", "$v0", value + "($gp)", "");
			}
		}
	}
	else if (op == "*") {
		string arg1regname = preloadvar(arg1, "$v0");
		string arg2regname = preloadvar(arg2, "$v1");
		if (isNum(arg1regname)) {
			printmips("li", "$v0", arg1regname, "");
			arg1regname = "$v0";
		}
		if (isNum(arg2regname)) {
			printmips("li", "$v1", arg2regname, "");
			arg2regname = "$v1";
		}
		printmips("mult", arg1regname, arg2regname,"");

		if (type == 1) {
			printmips("mflo", value,"","");
		}
		else if (type == 2) {
			printmips("mflo", "$v0", "", "");
			printmips("sw", "$v0", value + "($fp)", "");
		}
		else if (type == 3) {
			printmips("mflo", "$v0", "", "");
			printmips("sw", "$v0", value + "($gp)", "");
		}
	}
	else if (op == "/") {
		string arg1regname = preloadvar(arg1, "$v0");
		string arg2regname = preloadvar(arg2, "$v1");
		if (isNum(arg1regname)) {
			printmips("li", "$v0", arg1regname, "");
			arg1regname = "$v0";
		}

		if (type == 1) {
			printmips("div", value, arg1regname, arg2regname);
		}
		else if (type == 2) {
			printmips("div", "$v0", arg1regname, arg2regname);
			printmips("sw", "$v0", value + "($fp)", "");
		}
		else if (type == 3) {
			printmips("div", "$v0", arg1regname, arg2regname);
			printmips("sw", "$v0", value + "($gp)", "");
		}
	}
}

bool isNum(string a) {
	return isdigit(a[0]) || a[0] == '+' || a[0] == '-';
}

void Branch() {
	string op = midCode[Index].op;
	string arg1 = midCode[Index].arg1;
	string arg2 = midCode[Index].arg2;
	string result = midCode[Index].result;
	string arg1regname = preloadvar(arg1, "$v0");
	string arg2regname = preloadvar(arg2, "$v1");

	if (isNum(arg1regname)) {
		printmips("li", "$v0", arg1regname, "");
		arg1regname = "$v0";
	}
	
	if (op == "==") {
		printmips("beq", arg1regname, arg2regname, result);
	}
	else if (op == "!=") {
		printmips("bne", arg1regname, arg2regname, result);
	}
	else if (op == ">") {
		printmips("bgt", arg1regname, arg2regname, result);
	}
	else if (op == "<") {
		printmips("blt", arg1regname, arg2regname, result);
	}
	else if (op == ">=") {
		printmips("bge", arg1regname, arg2regname, result);
	}
	else if (op == "<=") {
		printmips("ble", arg1regname, arg2regname, result);
	}
}

void Fetch() {
	int type;
	string result = midCode[Index].result;
	string value = loadvar(result, type);
	
	if (type == 1) {
		printmips("move", value, "$v0", "");
	}
	else if (type == 2) {
		printmips("sw", "$v0", value + "($fp)", "");
	}
	else if (type == 3) {
		printmips("sw", "$v0", value + "($gp)", "");
	}
}

//a = b[i]
void Arr_Brack() {
	string arrname = midCode[Index].arg1;
	string arrindex = midCode[Index].arg2;
	string varname = midCode[Index].result;
	int type_bias, type_index;
	string arr_bias = loadvar(arrname, type_bias);
	string index = preloadvar(arrindex, "$v0");

	if (type_bias == 2) {  //局部数组变量
		if (isNum(index)) {  //i是数字
			int temp = stoi(arr_bias) - stoi(index) * 4;
			printmips("lw", "$v0", to_string(temp) + "($fp)","");
		}
		else {  //i是寄存器
			printmips("sll", "$v0", index, "2");
			printmips("sub", "$v0", "$zero", "$v0");
			printmips("add", "$v0",	"$v0", arr_bias);
			printmips("add", "$v0", "$fp", "$v0");
			printmips("lw", "$v0", "($v0)","");
		}
	}
	else if (type_bias == 3) {  //全局数组变量
		if (isNum(index)) {  //i是数字
			int temp = stoi(arr_bias) + stoi(index) * 4;
			printmips("lw", "$v0", to_string(temp) + "($gp)", "");
		}
		else {  //i是寄存器
			printmips("sll", "$v0", index, "2");
			printmips("add", "$v0", "$v0", arr_bias);
			printmips("add", "$v0", "$gp", "$v0");
			printmips("lw", "$v0", "($v0)", "");
		}
	}

	int var_type;
	string value = loadvar(varname, var_type);
	if (var_type == 1) {
		printmips("move", value, "$v0", "");
	}
	else if (var_type == 2) {  
		printmips("sw", "$v0", value + "($fp)","");
	}
	else if (var_type == 3) {
		printmips("sw", "$v0", value + "($gp)","");
	}
}

//b[i] = a;
void Arr_Brack_Assi() {
	string arrname = midCode[Index].arg1;
	string arrindex = midCode[Index].arg2;
	string varname = midCode[Index].result;
	string var2reg = preloadvar(varname,"$v1");
	if (isNum(var2reg)) {
		printmips("li", "$v1", var2reg,"");
		var2reg = "$v1";
	}
	
	int type_bias, type_index;
	string arr_bias = loadvar(arrname, type_bias);
	string index = preloadvar(arrindex, "$v0");

	if (type_bias == 2) {  //局部数组变量
		if (isNum(index)) {  //i是数字
			int temp = stoi(arr_bias) - stoi(index) * 4;
			printmips("sw", var2reg, to_string(temp) + "($fp)", "");
		}
		else {  //i是寄存器
			printmips("sll", "$v0", index, "2");
			printmips("sub", "$v0", "$zero", "$v0");
			printmips("add", "$v0", "$v0", arr_bias);
			printmips("add", "$v0", "$fp", "$v0");
			printmips("sw", var2reg, "($v0)", "");
		}
	}
	else if (type_bias == 3) {  //全局数组变量
		if (isNum(index)) {  //i是数字
			int temp = stoi(arr_bias) + stoi(index) * 4;
			printmips("sw", var2reg, to_string(temp) + "($gp)", "");
		}
		else {  //i是寄存器
			printmips("sll", "$v0", index, "2");
			printmips("add", "$v0", "$v0", arr_bias);
			printmips("add", "$v0", "$gp", "$v0");
			printmips("sw", var2reg, "($v0)", "");
		}
	}

}

void Func_Def() {
	string funcname = midCode[Index].arg2;
	cout << funcname << ":" << endl;
	map<string, int>::iterator iter; 
	iter = func2funcNo.find(funcname);
	if (iter != func2funcNo.end()) {
		curfun = iter->second;
		if(funcname == "main"){
			printmips("sub", "$sp", "$sp", to_string(func_syt[curfun].blocksize * 4));
		}
	}
}

void End_Func() {
	cout << midCode[Index].result << ":" << endl;
	if(midCode[Index].result != "$End_main")printmips("jr", "$ra", "", "");
}

void Call_Func() {
	string callfuncname = midCode[Index].arg2;
	int savedfunNo = curfun;
	//维护现场
	saveReg();
	//更新fp
	printmips("sub", "$fp", "$sp", to_string(4 * paraCnt()));
	//更新curfun
	curfun = func2funcNo[callfuncname];
	//更新sp
	printmips("sub", "$sp", "$fp", to_string(4 * func_syt[curfun].blocksize));
	//跳转到函数执行
	printmips("jal", callfuncname, "", "");
	//重置sp
	printmips("move", "$sp", "$fp", "");
	printmips("add", "$sp", "$sp", to_string(4 * paraCnt()));
	//重置curfun
	curfun = savedfunNo;
	//重置fp
	printmips("add", "$fp", "$sp", to_string(4 * func_syt[curfun].blocksize));
	//重置寄存器
	restoreReg();

	paraNo = paraStack[--top];
}

void saveReg() {
	for (int i = 0; i < func_syt[curfun].sregNo; i++) {
		printmips("sw", "$s"+to_string(i), to_string(-4 * (func_syt[curfun].index + i)) + "($fp)", "");
	}
	for (int i = 0; i < func_syt[curfun].tregNo; i++) {
		printmips("sw", "$t" + to_string(i), to_string(-4 * (func_syt[curfun].index + func_syt[curfun].sregNo + i)) + "($fp)", "");
	}
	printmips("sw", "$ra", to_string(-4 * (func_syt[curfun].index + func_syt[curfun].sregNo + func_syt[curfun].tregNo)) + "($fp)", "");
	//printmips("sw", "$fp", to_string(-4 * (func_syt[curfun].blocksize - 2)) + "($fp)", "");
	//printmips("sw", "$ra", to_string(-4 * (func_syt[curfun].blocksize - 1)) + "($fp)", "");
}

void restoreReg() {
	for (int i = 0; i < func_syt[curfun].sregNo; i++) {
		printmips("lw", "$s" + to_string(i), to_string(-4 * (func_syt[curfun].index + i)) + "($fp)", "");
	}
	for (int i = 0; i < func_syt[curfun].tregNo; i++) {
		printmips("lw", "$t" + to_string(i), to_string(-4 * (func_syt[curfun].index + func_syt[curfun].sregNo + i)) + "($fp)", "");
	}
	printmips("lw", "$ra", to_string(-4 * (func_syt[curfun].index + func_syt[curfun].sregNo + func_syt[curfun].tregNo)) + "($fp)", "");
	//printmips("lw", "$fp", to_string(-4 * (func_syt[curfun].blocksize - 2)) + "($fp)", "");
	//printmips("lw", "$ra", to_string(-4 * (func_syt[curfun].blocksize - 1)) + "($fp)", "");
}

void Fcall() {
	paraStack[top++] = paraNo;
	paraNo = 0;
}

int paraCnt() {
	int sum = 0;
	for (int i = 0; i < top; i++) sum += paraStack[i];
	return sum;
}

void Push() {
	string para = midCode[Index].arg1;
	string regname = preloadvar(para, "$v0");
	if (isNum(regname)) {
		printmips("li", "$v0", regname, "");
		regname = "$v0";
	}
	printmips("sw", regname, to_string(-4 * (paraNo+paraCnt())) + "($sp)", "");
	paraNo++;
}   

void Ret() {
	string ret_v = midCode[Index].arg1;
	string label = midCode[Index].result;
	if (ret_v != "") {
		string regname = preloadvar(ret_v, "$v1");
		if (isNum(regname)) {
			printmips("li", "$v0", regname, "");
		}
		else {
			printmips("move", "$v0", regname, "");
		}
	}
	printmips("j", label, "", "");
}

void Scan() {
	string varname = midCode[Index].arg1;

	if (global.search_var_type(varname) == "") {
		if (func_syt[curfun].search_var_type(varname) == "INTTK") {
			printmips("li", "$v0", "5", "");
		}
		else {
			printmips("li", "$v0", "12", "");
		}
		cout << "	syscall" << endl;
		int type;
		string varReg = loadvar(varname, type);
		if (type == 1) {
			printmips("move", varReg, "$v0", "");
		}
		if (type == 2) {
			printmips("sw", "$v0", varReg + "($fp)", "");
		}
	}
	else {
		if (global.search_var_type(varname) == "INTTK") {
			printmips("li", "$v0", "5", "");
		}
		else {
			printmips("li", "$v0", "12", "");
		}
		cout << "	syscall" << endl;
		int type;
		string var_bias = loadvar(varname, type);
		if (type == 3) {
			printmips("sw", "$v0", var_bias+"($gp)", "");
		}
	}
}

void Print() {
	string str = midCode[Index].arg1;
	string regtype = midCode[Index].arg2;
	string regname = midCode[Index].result;
	string reg = preloadvar(regname, "$v1");
	if (isNum(reg)) {
		printmips("li", "$v1", reg, "");
		reg = "$v1";
	}

	if (str != "") {
		printmips("li", "$v0", "4", "");
		printmips("la", "$a0", str, "");
		cout << "	syscall" << endl;
	}

	if (regtype == "INTTK") {
		printmips("li", "$v0", "1", "");
		printmips("move", "$a0", reg , "");
		cout << "	syscall" << endl;
	}
	else if (regtype == "CHARTK") {
		printmips("li", "$v0", "11", "");
		printmips("move", "$a0", reg, "");
		cout << "	syscall" << endl;
	}

	printmips("li", "$v0", "4", "");
	printmips("la", "$a0", "Enter", "");
	cout << "	syscall" << endl;
	return;
}

void Lab() {
	cout << midCode[Index].arg1 << ":" << endl;
}

void Jmp() {
	printmips("j", midCode[Index].arg1, "", "");
}

void Assi() {
	string lname = midCode[Index].arg1;
	string rname = midCode[Index].arg2;
	string r_reg = preloadvar(rname, "$v0");
	int type;
	string l_reg = loadvar(lname, type);
	string l_reg_type;
	

	if (isNum(r_reg)) {
		if (type == 1) {
			printmips("li", l_reg, r_reg,"");
		}
		else if (type == 2) {
			printmips("li", "$v0", r_reg, "");
			printmips("sw", "$v0", l_reg+"($fp)", "");
		}
		else if (type == 3) {
			printmips("li", "$v0", r_reg, "");
			printmips("sw", "$v0", l_reg + "($gp)", "");
		}
	}
	else {
		if (type == 1) {
			printmips("move", l_reg, r_reg, "");
		}
		else if (type == 2) {
			printmips("sw", r_reg, l_reg + "($fp)", "");
		}
		else if (type == 3) {
			printmips("sw", r_reg, l_reg + "($gp)", "");
		}
	}
}

void generateObjcode() {
	freopen("mips.txt", "w", stdout);
	init();
	while (Index < midCodeNum) {
		if (midCode[Index].op == "Con") Con_local();
		else if (midCode[Index].op == "+" || midCode[Index].op == "-" ||
			midCode[Index].op == "*" || midCode[Index].op == "/") Calculate();
		else if (midCode[Index].op == "==" || midCode[Index].op == "!=" ||
			midCode[Index].op == ">" || midCode[Index].op == "<" ||
			midCode[Index].op == ">=" || midCode[Index].op == "<=") Branch();
		else if (midCode[Index].op == "=") Assi();
		else if (midCode[Index].op == "Fetch") Fetch();
		else if (midCode[Index].op == "[]") Arr_Brack();
		else if (midCode[Index].op == "[]=") Arr_Brack_Assi();
		else if (midCode[Index].op == "Func") Func_Def();
		else if (midCode[Index].op == "End") End_Func();
		else if (midCode[Index].op == "Call") Call_Func();
		else if (midCode[Index].op == "Jmp") Jmp();
		else if (midCode[Index].op == "Lab") Lab();
		else if (midCode[Index].op == "Scan") Scan();
		else if (midCode[Index].op == "Print") Print();
		else if (midCode[Index].op == "Ret") Ret();
		else if (midCode[Index].op == "Push") Push();
		else if (midCode[Index].op == "Fcall") Fcall();
		else if (midCode[Index].op == "Para") {}
		else if (midCode[Index].op == "Var") {}
		else if (midCode[Index].op == "Arr") {} //局部变量和数组无需初始化
		else printf("error");
		Index++;
	}
	printmips("li", "$v0", "10", "");
	cout << "	syscall" << endl;
	fclose(stdout);
}



