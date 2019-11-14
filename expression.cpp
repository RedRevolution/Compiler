#include "syntax_analysis.h"

//���ʽ*
string expr(string& regName) {
	if (!HandleError) {
		bool charflag = true;
		bool negFlag = false;
		if (symbol == "PLUS" || symbol == "MINU") {
			printlex();
			if (symbol == "MINU") negFlag = true;
			charflag = false;
			getsym();
		}
		string op, reg_1, reg_2, reg_3;
		if (term(reg_1) == "INTTK") charflag = false;
		if (negFlag) {
			reg_3 = newReg();
			emit("-", "$zero", reg_1, reg_3);
			reg_1 = reg_3;
		}
		while (1) {
			if (symbol != "PLUS" && symbol != "MINU")break;
			charflag = false;
			op = token;
			printlex(); //��ӡ������
			getsym();
			term(reg_2);
			reg_3 = newReg();
			emit(op, reg_1, reg_2, reg_3);
			reg_1 = reg_3;
		}
		regName = reg_1;
		printsyn("<���ʽ>");
		if (charflag)return "CHARTK";
		else return "INTTK";
	}
	//else {
	//	string ret_v;
	//	int flag = 0;
	//	int count = 0;
	//	if (symbol == "PLUS" || symbol == "MINU") {
	//		printlex(); //��ӡ������
	//		flag = 1;
	//		getsym();
	//	}
	//	while (1) {
	//		ret_v = term(p);
	//		count++;
	//		if (symbol != "PLUS" && symbol != "MINU") break;
	//		printlex(); //��ӡ������
	//		getsym();
	//	}
	//	printsyn("<���ʽ>");
	//	if (count > 1 || flag)ret_v = "INTTK";
	//	return ret_v;
	//}
}

//��*
string term(string& regName) {
	if (!HandleError) {
		string ret_type;
		bool charflag = true;
		string op, reg_1, reg_2, reg_3;
		if (factor(reg_1) == "INTTK")charflag = false;
		while (symbol == "MULT" || symbol == "DIV") {
			charflag = false;
			op = token;
			printlex();
			getsym();
			factor(reg_2);
			reg_3 = newReg();
			emit(op, reg_1, reg_2, reg_3);
			reg_1 = reg_3;
		}
		regName = reg_3;
		printsyn("<��>");
		if (charflag)return "CHARTK";
		else return "INTTK";
	}
	//else {
	//	string ret_v = "INTTK";
	//	int count = 0;
	//	ret_v = factor(p);
	//	count++;
	//	while (symbol == "MULT" || symbol == "DIV") {
	//		printlex(); //��ӡ�˳���
	//		getsym();
	//		factor(p);
	//		count++;
	//	}
	//	printsyn("<��>");
	//	if (count > 1)ret_v = "INTTK";
	//	return ret_v;
	//}
}

//����*
string factor(string& regName) {
	if (!HandleError) {
		string ret_type;
		if (symbol == "IDENFR") {
			string name = token;
			//�з���ֵ���ú���
			if (preload(1) == "LPARENT") {
				ret_type = syt[0].sym[syt[0].search_name(name)].ret_type;
				if (ret_type == "CHARTK"||ret_type == "INTTK") callrfun();
				regName = newReg();
				emit("Fetch", "", "", regName);
				printsyn("<����>");
			}
			else if (preload(1) == "LBRACK") { //����ʶ������[�������ʽ����]��
				if (syt[level].search_name[name] != -1) ret_type = syt[level].sym[syt[level].search_name(name)].ret_type;
				else ret_type = syt[0].sym[syt[0].search_name(name)].ret_type;
				printlex(); //��ӡ��ʶ��
				getsym();
				printlex(); //��ӡ[
				getsym();
				string reg_index;
				expr(reg_index);
				regName = newReg();
				emit("[]", name, reg_index, regName);
				printlex(); //��ӡ]
				getsym();
				printsyn("<����>");
			}
			else {   //������
				if (syt[level].search_name[name] != -1) ret_type = syt[level].sym[syt[level].search_name(name)].ret_type;
				else ret_type = syt[0].sym[syt[0].search_name(name)].ret_type;
				printlex(); //��ӡ��ʶ��
				regName = name;
				getsym();
			}
		}
		else if (symbol == "LPARENT") {
			printlex(); //��ӡ��
			getsym();
			ret_type = expr(regName);
			printlex(); //��ӡ��
			getsym();
		}
		else if (symbol == "PLUS" || symbol == "MINU" || symbol == "INTCON") {
			INT(regName);
			ret_type = "INTTK";
		}
		else if (symbol == "CHARCON") {
			printlex();
			regName = getAscill();
			getsym();
			ret_type = "CHARTK";
		}
		printsyn("<����>");
		return ret_type;
	}
	//else {
	//	string ret_v = "INTTK";
	//	if (symbol == "IDENFR") {
	//		//�з���ֵ���ú���
	//		if (preload(1) == "LPARENT") {
	//			int index = global.search_rfun(token);
	//			/* ������ */
	//			if (index == -1) {
	//				syntax_error('c', 3);
	//				while (symbol != "RPARENT")getsym();
	//				getsym();
	//				return ret_v;
	//			}
	//			ret_v = global.sym[index].ret_type;
	//			callrfun(p);
	//			printsyn("<����>");
	//			return ret_v;
	//		}
	//		//������ֵı�ʶ�������Ǳ�������������
	//		int index_g = global.search_vname(token);
	//		int index_l = p.search_vname(token);
	//		/* ������ */
	//		if (index_g == -1 && index_l == -1) {
	//			syntax_error('c', 2);
	//		}
	//		if (index_g != -1) ret_v = global.sym[index_g].ret_type;
	//		if (index_l != -1) ret_v = p.sym[index_l].ret_type;
	//		printlex(); //��ӡ��ʶ��
	//		getsym();
	//		//��ʶ��[���ʽ]
	//		if (symbol == "LBRACK") {
	//			printlex(); //��ӡ[
	//			getsym();
	//			/* ������ */
	//			if (expr(p) != "INTTK") {
	//				syntax_error('i', 1); //�����±�ֻ��������
	//			}
	//			/* ������ */
	//			if (symbol != "RBRACK") {
	//				syntax_error('m', 1); //ӦΪ��������
	//			}
	//			else {
	//				printlex(); //��ӡ]
	//				getsym();
	//			}
	//		}
	//	}
	//	else if (symbol == "LPARENT") {
	//		printlex(); //��ӡ��
	//		getsym();
	//		expr(p);
	//		/* ������ */
	//		if (symbol != "RPARENT") {
	//			syntax_error('l', 1); //ӦΪ��С����
	//		}
	//		else {
	//			printlex(); //��ӡ)
	//			getsym();
	//		}
	//		ret_v = "INTTK";
	//	}
	//	else if (symbol == "PLUS" || symbol == "MINU" || symbol == "INTCON") {
	//		INT();
	//		ret_v = "INTTK";
	//	}
	//	else if (symbol == "CHARCON") {
	//		printlex(); //��ӡchar
	//		getsym();
	//		ret_v = "CHARTK";
	//	}
	//	printsyn("<����>");
	//	return ret_v;
	//}
}