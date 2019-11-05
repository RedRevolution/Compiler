#include "syntax_analysis.h"

//���ʽ*
string expr(Symtable &p) {
	string ret_v;
	int flag = 0;
	int count = 0;
	if (symbol == "PLUS" || symbol == "MINU") {
		printlex(); //��ӡ������
		flag = 1;
		getsym();
	}
	while (1) {
		ret_v = term(p);
		count++;
		if (symbol != "PLUS" && symbol != "MINU") break;
		printlex(); //��ӡ������
		getsym();
	}
	printsyn("<���ʽ>");
	if (count > 1||flag)ret_v = "INTTK";
	return ret_v;
}

//��*
string term(Symtable &p) {
	string ret_v = "INTTK";
	int count = 0;
	ret_v = factor(p);
	count++;
	while (symbol == "MULT" || symbol == "DIV") {
		printlex(); //��ӡ�˳���
		getsym();
		factor(p);
		count++;
	}
	printsyn("<��>");
	if (count > 1)ret_v = "INTTK";
	return ret_v;
}

//����*
string factor(Symtable &p) {
	string ret_v = "INTTK";
	if (symbol == "IDENFR") {
		//�з���ֵ���ú���
		if (preload(1) == "LPARENT") {
			int index = global.search_rfun(token);
			/* ������ */
			if (index == -1) {
				syntax_error('c', 3);
				while (symbol != "RPARENT")getsym();
				getsym();
				return ret_v;
			}
			ret_v = global.sym[index].ret_type;
			callrfun(p);
			printsyn("<����>");
			return ret_v;
		}

		//������ֵı�ʶ�������Ǳ�������������
		int index_g = global.search_vname(token);
		int index_l = p.search_vname(token);
		/* ������ */
		if (index_g == -1&& index_l == -1) {
			syntax_error('c', 2);
		}
		if (index_g != -1) ret_v = global.sym[index_g].ret_type;
		if (index_l != -1) ret_v = p.sym[index_l].ret_type;
		printlex(); //��ӡ��ʶ��
		getsym();
		//��ʶ��[���ʽ]
		if (symbol == "LBRACK") {
			printlex(); //��ӡ[
			getsym();
			/* ������ */
			if (expr(p) != "INTTK") {
				syntax_error('i', 1); //�����±�ֻ��������
			}
			/* ������ */
			if (symbol != "RBRACK") {
				syntax_error('m', 1); //ӦΪ��������
			}
			else {
				printlex(); //��ӡ]
				getsym(); 
			}
		}
	}
	else if (symbol == "LPARENT") {
		printlex(); //��ӡ��
		getsym();
		expr(p);
		/* ������ */
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //ӦΪ��С����
		}
		else {
			printlex(); //��ӡ)
			getsym();
		}
		ret_v = "INTTK";
	}
	else if (symbol == "PLUS" || symbol == "MINU" || symbol == "INTCON") {
		INT();
		ret_v = "INTTK";
	}
	else if (symbol == "CHARCON") {
		printlex(); //��ӡchar
		getsym();
		ret_v = "CHARTK";
	}
	printsyn("<����>");
	return ret_v;
}