#include "syntax_analysis.h"
#include "generateMcode.h"

//����ͷ��*
void dh(string& func_name) { 
	if (!HandleError) {
		string ret_type, funcname;
		if (symbol != "INTTK" && symbol != "CHARTK") error();
		ret_type = symbol;
		printlex();
		getsym(); //��ʶ��
		funcname = token;
		printlex(); //��ӡ��ʶ��
		syt[0].add_entry(funcname, "FUNC", ret_type, to_string(level));
		printsyn("<����ͷ��>");
		getsym();
	}
	//else {
	//	string ret_type;
	//	if (symbol != "INTTK" && symbol != "CHARTK") error();
	//	ret_type = symbol;
	//	printlex(); //��ӡint|char
	//	getsym(); //��ʶ��
	//	func_name = token;
	//	/*������*/
	//	if (!global.add_entry(func_name, "FUNC", ret_type)) {
	//		syntax_error('b', 11);
	//	}
	//	printlex(); //��ӡ��ʶ��
	//	printsyn("<����ͷ��>");
	//	getsym();
	//}
}


//������*
void pt(string func_name) {
	if (!HandleError) {
		string ret_type, pr_name;
		//������Ϊ��
		if (symbol == "INTTK" || symbol == "CHARTK") {
			ret_type = symbol;
			printlex(); //��ӡ int|char
			while (1) {
				getsym(); //�����ʶ��
				pr_name = token;
				syt[0].add_pt_entry(func_name, pr_name, ret_type);
				syt[level].add_entry(pr_name, "VAR", ret_type, "");
				emit("Para", ret_type, pr_name, "");
				printlex(); //��ӡ��ʶ��
				getsym(); //���붺�Ż��ߣ�
				if (symbol != "COMMA")break;
				printlex(); //��ӡ����
				getsym(); //���� int|char
				ret_type = symbol;
				printlex(); //��ӡ int|char
			}
		}
		printsyn("<������>");
	}
	//else {
	//	string pr_name;
	//	string ret_type;
	//	//������Ϊ��
	//	if (symbol == "INTTK" || symbol == "CHARTK") {
	//		ret_type = symbol;
	//		printlex(); //��ӡ int|char
	//		while (1) {
	//			getsym(); //�����ʶ��
	//			pr_name = token;
	//			//���뺯��func_name�Ĳ�����
	//			if (!global.add_pt_entry(func_name, pr_name, ret_type)) {
	//				/*������*/
	//				syntax_error('b', 12);  //�������ض���
	//			}
	//			p.add_entry(pr_name, "VAR", ret_type); //���������еĲ�������ֲ�������
	//			printlex(); //��ӡ��ʶ��
	//			getsym(); //���붺�Ż��ߣ�
	//			if (symbol != "COMMA")break;
	//			printlex(); //��ӡ����
	//			getsym(); //���� int|char
	//			ret_type = symbol;
	//			printlex(); //��ӡ int|char
	//		}
	//	}
	//	printsyn("<������>");
	//}
}

//�з���ֵ��������*
void rfun() {
	if (!HandleError) {
		string funcname, ret_type;
		resetRegNum(0);
		if (symbol != "INTTK" && symbol != "CHARTK") error();
		ret_type = symbol;
		dh(funcname);
		printlex(); //��ӡ��
		syt[level].add_entry(funcname, "FUNC", ret_type,to_string(level));
		emit("Func", "", funcname, "");
		getsym();
		pt(funcname);
		printlex(); //��ӡ��
		getsym();
		printlex(); //��ӡ{
		getsym();
		costa();
		printlex(); //��ӡ}
		emit("End", "", "", "$End_" + funcname);
		printsyn("<�з���ֵ��������>");
		getsym();
	}
	//else {
	//	Symtable p;
	//	string func_name;
	//	string func_type;
	//	if (symbol != "INTTK" && symbol != "CHARTK") error();
	//	func_type = symbol;
	//	dh(func_name);
	//	printlex(); //��ӡ��
	//	getsym();
	//	//�ֲ����ű�ĵ�һ��ע��ú����Ļ�����Ϣ
	//	p.add_entry(func_name, "FUNC", func_type);
	//	pt(func_name, p);
	//	/* ������ */
	//	if (symbol != "RPARENT") {
	//		syntax_error('l', 1); //ӦΪ��С����
	//	}
	//	else {
	//		printlex(); //��ӡ)
	//		getsym();
	//	}
	//	printlex(); //��ӡ{
	//	getsym();
	//	costa(p);
	//	printlex(); //��ӡ}
	//	printsyn("<�з���ֵ��������>");
	//	getsym();
	//}
}

//�޷���ֵ��������*
void nfun() {
	if (!HandleError) {
		string funcname;
		if (symbol != "VOIDTK") error();
		printlex();
		getsym(); //�����ʶ��
		funcname = token;
		printlex(); //��ӡ��ʶ��
		syt[0].add_entry(funcname, "FUNC", "VOIDTK", to_string(level));
		syt[level].add_entry(funcname, "FUNC", "VOIDTK", to_string(level));
		emit("Func", "", funcname, "");
		getsym();
		printlex(); //��ӡ��
		getsym();
		pt(funcname);
		printlex(); //��ӡ��
		getsym();
		printlex(); //��ӡ{
		getsym();
		costa();
		printlex(); //��ӡ}
		emit("End", "", "", "$End_" + funcname);
		printsyn("<�޷���ֵ��������>");
		getsym();
	}
	//else {
	//	Symtable p;
	//	string func_name;
	//	if (symbol != "VOIDTK") error();
	//	printlex(); //��ӡvoid
	//	getsym(); //�����ʶ��
	//	func_name = token;
	//	//�ֲ����ű�ĵ�һ��ע��ú����Ļ�����Ϣ
	//	p.add_entry(func_name, "FUNC", "VOIDTK");
	//	/*������*/
	//	if (!global.add_entry(func_name, "FUNC", "VOIDTK")) {
	//		syntax_error('b', 13);
	//	}
	//	printlex(); //��ӡ��ʶ��
	//	getsym();
	//	printlex(); //��ӡ��
	//	getsym();
	//	pt(func_name, p);
	//	/* ������ */
	//	if (symbol != "RPARENT") {
	//		syntax_error('l', 1); //ӦΪ��С����
	//	}
	//	else {
	//		printlex(); //��ӡ)
	//		getsym();
	//	}
	//	printlex(); //��ӡ{
	//	getsym();
	//	costa(p);
	//	printlex(); //��ӡ}
	//	printsyn("<�޷���ֵ��������>");
	//	getsym();
	//}
}

//������*
void mfun() {
	if (!HandleError) {
		if (symbol != "VOIDTK") error();
		printlex(); //��ӡvoid
		getsym(); //�����ʶ��main
		printlex(); //��ӡ��ʶ��main
		syt[0].add_entry("main", "FUNC", "VOIDTK", to_string(level));
		syt[level].add_entry("main", "FUNC", "VOIDTK", to_string(level));
		emit("Func", "", "main", "");
		getsym();
		printlex(); //��ӡ��
		getsym();
		printlex(); //��ӡ��
		getsym();
		printlex(); //��ӡ{
		getsym();
		costa();
		printlex(); //��ӡ}
		emit("End", "", "", "$End_main");
		printsyn("<������>"); //��������������getsym
	}
	//else {
	//	Symtable p;
	//	if (symbol != "VOIDTK") error();
	//	printlex(); //��ӡvoid
	//	getsym(); //�����ʶ��main
	//	/*������*/
	//	if (!global.add_entry("main", "FUNC", "VOIDTK")) {
	//		syntax_error('b', 14);
	//	}
	//	p.add_entry("main", "FUNC", "VOIDTK");
	//	printlex(); //��ӡ��ʶ��main
	//	getsym();
	//	printlex(); //��ӡ��
	//	getsym();
	//	/* ������ */
	//	if (symbol != "RPARENT") {
	//		syntax_error('l', 1); //ӦΪ��С����
	//	}
	//	else {
	//		printlex(); //��ӡ)
	//		getsym();
	//	}
	//	printlex(); //��ӡ{
	//	getsym();
	//	costa(p);
	//	printlex(); //��ӡ}
	//	printsyn("<������>"); //��������������getsym
	//}
}