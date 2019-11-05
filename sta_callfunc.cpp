#include "syntax_analysis.h"

//�з���ֵ�����������*
void callrfun(Symtable &syt) {
	string func_name;
	if (symbol != "IDENFR") error();
	func_name = token;
	printlex(); //��ӡ��ʶ��
	getsym();
	printlex(); //��ӡ��
	getsym();
	vpt(func_name, syt);
	if (symbol != "RPARENT") {
		syntax_error('l', 1); //ӦΪ��С����
	}
	else {
		printlex(); //��ӡ)
		getsym();
	}
	printsyn("<�з���ֵ�����������>");
}

//�޷���ֵ�����������*
void callnfun(Symtable &syt) {
	string func_name;
	if (symbol != "IDENFR") error();
	func_name = token;
	printlex(); //��ӡ��ʶ��
	getsym();
	printlex(); //��ӡ��
	getsym();
	vpt(func_name, syt);
	if (symbol != "RPARENT") {
		syntax_error('l', 1); //ӦΪ��С����
	}
	else {
		printlex(); //��ӡ)
		getsym();
	}
	printsyn("<�޷���ֵ�����������>");
}

//ֵ������*
void vpt(string func_name,Symtable &syt) {
	int count = 0;
	string ret_types[N];
	if (symbol != "RPARENT"&&symbol != "SEMICN") {
		/* ������ */
		//����syt��global��������δ����
		ret_types[count++] = expr(syt);
		
		while (1) {
			if (symbol != "COMMA")break;
			printlex(); //��ӡ����
			getsym();
			ret_types[count++] = expr(syt);
		}
	}
	int index = global.search_vname(func_name);
	/* ������ */
	//��������������ƥ��
	if (!global.sym[index].pt.check_length(count)) {
		syntax_error('d', 1);
	}
	else {
		//������������0�Һ����������Ͳ�ƥ��
		if (count&&!global.sym[index].pt.check_type(ret_types)) {
			syntax_error('e', 1);
		}
	}
	
	printsyn("<ֵ������>");
}