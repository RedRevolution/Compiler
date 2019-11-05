#include "syntax_analysis.h"

//����ͷ��*
void dh(string& func_name) {  //����ַ
	string ret_type;
	if (symbol != "INTTK" && symbol != "CHARTK") error();
	ret_type = symbol;
	printlex(); //��ӡint|char
	getsym(); //��ʶ��
	func_name = token;
	/*������*/
	if (!global.add_entry(func_name, "FUNC", ret_type)) {
		syntax_error('b', 11);
	}
	printlex(); //��ӡ��ʶ��
	printsyn("<����ͷ��>");
	getsym();
}


//������*
void pt(string func_name,Symtable &p) {
	string pr_name;
	string ret_type;
	//������Ϊ��
	if (symbol == "INTTK" || symbol == "CHARTK") {
		ret_type = symbol;
		printlex(); //��ӡ int|char
		while (1) {
			getsym(); //�����ʶ��
			pr_name = token;
			//���뺯��func_name�Ĳ�����
			if (!global.add_pt_entry(func_name, pr_name, ret_type)) {
				/*������*/  
				syntax_error('b', 12);  //�������ض���
			}
			p.add_entry(pr_name, "VAR", ret_type); //���������еĲ�������ֲ�������
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

//�з���ֵ��������*
void rfun() {
	Symtable p;
	string func_name;
	string func_type;
	if (symbol != "INTTK" && symbol != "CHARTK") error();
	func_type = symbol;
	dh(func_name);
	printlex(); //��ӡ��
	getsym();
	//�ֲ����ű�ĵ�һ��ע��ú����Ļ�����Ϣ
	p.add_entry(func_name, "FUNC", func_type);
	pt(func_name,p);
	/* ������ */
	if (symbol != "RPARENT") {
		syntax_error('l', 1); //ӦΪ��С����
	}
	else {
		printlex(); //��ӡ)
		getsym();
	}
	printlex(); //��ӡ{
	getsym();
	costa(p);
	printlex(); //��ӡ}
	printsyn("<�з���ֵ��������>");
	getsym();
}

//�޷���ֵ��������*
void nfun() {
	Symtable p;
	string func_name;
	if (symbol != "VOIDTK") error();
	printlex(); //��ӡvoid
	getsym(); //�����ʶ��
	func_name = token;
	//�ֲ����ű�ĵ�һ��ע��ú����Ļ�����Ϣ
	p.add_entry(func_name, "FUNC", "VOIDTK");
	/*������*/
	if (!global.add_entry(func_name, "FUNC", "VOIDTK")) {
		syntax_error('b', 13);
	}
	printlex(); //��ӡ��ʶ��
	getsym();
	printlex(); //��ӡ��
	getsym();
	pt(func_name,p);
	/* ������ */
	if (symbol != "RPARENT") {
		syntax_error('l', 1); //ӦΪ��С����
	}
	else {
		printlex(); //��ӡ)
		getsym();
	}
	printlex(); //��ӡ{
	getsym();
	costa(p);
	printlex(); //��ӡ}
	printsyn("<�޷���ֵ��������>");
	getsym();
}

//������*
void mfun() {
	Symtable p;
	if (symbol != "VOIDTK") error();
	printlex(); //��ӡvoid
	getsym(); //�����ʶ��main
	/*������*/
	if (!global.add_entry("main", "FUNC", "VOIDTK")) {
		syntax_error('b', 14);
	}
	p.add_entry("main", "FUNC", "VOIDTK");
	printlex(); //��ӡ��ʶ��main
	getsym();
	printlex(); //��ӡ��
	getsym();
	/* ������ */
	if (symbol != "RPARENT") {
		syntax_error('l', 1); //ӦΪ��С����
	}
	else {
		printlex(); //��ӡ)
		getsym();
	}
	printlex(); //��ӡ{
	getsym();
	costa(p);
	printlex(); //��ӡ}
	printsyn("<������>"); //��������������getsym
}