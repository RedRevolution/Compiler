#include "syntax_analysis.h"

//�������*
void costa(Symtable &p) {
	hasreturn = false;
	if (symbol == "CONSTTK")cs(p);
	if (symbol == "INTTK" || symbol == "CHARTK")vs(p);
	stas(p);
	if (p.sym[0].ret_type != "VOIDTK" && !hasreturn) {
		syntax_error('h', 1); //�з���ֵ����û��return���
	}
	printsyn("<�������>");
}


//���*
int sta(Symtable &p) {
	int ret_v = 0; //��ʶ������Ƿ��Ƿ������
	if (symbol == "IDENFR") {
		if (preload(1) == "LPARENT") {  //��������
			/* ������ */
			if (global.search_nfun(token)!=-1) callnfun(p);
			else if (global.search_rfun(token)!=-1) callrfun(p);
			else {
				syntax_error('c', 1); //δ����ĺ���
				while (symbol != "RPARENT")getsym(); //���������������β��
				getsym();
			}
			/* ������ */
			if (symbol != "SEMICN") {
				syntax_error('k', 1); //ӦΪ�ֺ�
			}
			else {
				printlex(); //��ӡ�ֺ�
				getsym();
			}
		}
		else {	//��ֵ���
			assta(p);
			/* ������ */
			if (symbol != "SEMICN") {
				syntax_error('k', 1); //ӦΪ�ֺ�
			}
			else {
				printlex(); //��ӡ�ֺ�
				getsym();
			}
		}
	}
	else if (symbol == "WHILETK" || symbol == "FORTK" || symbol == "DOTK") {
		lpsta(p);
	}
	else if (symbol == "IFTK") {
		cdsta(p);
	}
	else if (symbol == "RETURNTK") {
		rsta(p);
		/* ������ */
		if (symbol != "SEMICN") {
			syntax_error('k', 1); //ӦΪ�ֺ�
		}
		else {
			printlex(); //��ӡ�ֺ�
			getsym();
		}
		ret_v = 1;
	}
	else if (symbol == "SCANFTK") {
		rdsta(p);
		/* ������ */
		if (symbol != "SEMICN") {
			syntax_error('k', 1); //ӦΪ�ֺ�
		}
		else {
			printlex(); //��ӡ�ֺ�
			getsym();
		}
	}
	else if (symbol == "PRINTFTK") {
		wtsta(p);
		/* ������ */
		if (symbol != "SEMICN") {
			syntax_error('k', 1); //ӦΪ�ֺ�
		}
		else {
			printlex(); //��ӡ�ֺ�
			getsym();
		}
	}
	else if (symbol == "LBRACE") {
		printlex(); //��ӡ {
		getsym();
		stas(p);
		printlex(); //��ӡ }
		getsym();
	}
	else { //�����
		/* ������ */
		if (symbol != "SEMICN") {
			syntax_error('k', 1); //ӦΪ�ֺ�
		}
		else {
			printlex(); //��ӡ�ֺ�
			getsym();
		}
	}
	printsyn("<���>");
	return ret_v;
}

//��ֵ���*
void assta(Symtable &p) {
	if (symbol != "IDENFR") error();
	/* ������ */
	int index_l = p.search_vname(token);
	int index_g = global.search_vname(token);
	if (index_l == -1 && index_g == -1) {
		syntax_error('c', 3);  //����δ����
	}
	if (index_l != -1) {
		if (p.sym[index_l].type == "CONSTTK") {
			syntax_error('j', 1); //�ı䳣����ֵ
		}
	}
	if (index_l == -1 && index_g != -1) {
		if (global.sym[index_g].type == "CONSTTK") {
			syntax_error('j', 2); //�ı䳣����ֵ
		}
	}
	printlex(); //��ӡ��ʶ��
	getsym();
	if (symbol == "ASSIGN") {
		printlex();
		getsym();
		expr(p);
	}
	else {
		printlex(); //��ӡ[
		getsym();
		if (expr(p) != "INTTK") {
			syntax_error('i', 1);
		}
		/* ������ */
		if (symbol != "RBRACK") {
			syntax_error('m', 1); //ӦΪ��������
		}
		else {
			printlex(); //��ӡ]
			getsym(); 
		}
		printlex(); //��ӡ=
		getsym();
		expr(p);
	}
	printsyn("<��ֵ���>");
}

//�������*
void cdsta(Symtable &p) {
	if (symbol != "IFTK")error();
	printlex();
	getsym();
	printlex(); //��ӡ��
	getsym();
	condition(p);
	if (symbol != "RPARENT") {
		syntax_error('l', 1); //ӦΪ��С����
	}
	else {
		printlex(); //��ӡ)
		getsym();
	}
	sta(p);
	if (symbol == "ELSETK") {
		printlex(); //��ӡelse
		getsym();
		sta(p);
	}
	printsyn("<�������>");
}

//����*
void condition(Symtable &p) {
	string l_type = "INTTK";
	string r_type = "INTTK";
	l_type = expr(p);
	if (symbol == "LSS" || symbol == "LEQ"
		|| symbol == "GRE" || symbol == "GEQ"
		|| symbol == "NEQ" || symbol == "EQL") {
		printlex(); //��ӡ<>=
		getsym();
		r_type = expr(p);
	}
	/* ������ */
	if (l_type != "INTTK" || r_type != "INTTK") {
		syntax_error('f', 1);
	}
	printsyn("<����>");
}

//ѭ�����*
void lpsta(Symtable &p) {
	if (symbol == "WHILETK") {
		printlex(); //��ӡwhile
		getsym();
		printlex(); //��ӡ��
		getsym();
		condition(p);
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //ӦΪ��С����
		}
		else {
			printlex(); //��ӡ)
			getsym();
		}
		sta(p);
	}
	else if (symbol == "DOTK") {
		printlex(); //��ӡdo
		getsym();
		sta(p);
		/* ������ */
		if (symbol != "WHILETK") {
			syntax_error('n', 1);  //ȱ��while
		}
		else {
			printlex(); //��ӡwhile
			getsym();
		}
		if (symbol == "LPARENT") {
			printlex();//��ӡ��
			getsym();
			condition(p);
			if (symbol != "RPARENT") {
				syntax_error('l', 1); //ӦΪ��С����
			}
			else {
				printlex(); //��ӡ)
				getsym();
			}
		}
	}
	else if (symbol == "FORTK") {
		printlex(); //��ӡfor
		getsym();
		printlex(); //��ӡ��
		getsym();

		/* ������ */
		int index_l = p.search_vname(token);
		int index_g = global.search_vname(token);
		if (index_l == -1 && index_g == -1) {
			syntax_error('c', 3);  //����δ����
		}
		if (index_l != -1) {
			if (p.sym[index_l].type == "CONSTTK") {
				syntax_error('j', 1); //�ı䳣����ֵ
			}
		}
		if (index_l == -1 && index_g != -1) {
			if (global.sym[index_g].type == "CONSTTK") {
				syntax_error('j', 2); //�ı䳣����ֵ
			}
		}
		
		printlex(); //��ӡ��ʶ��
		getsym();
		printlex(); //��ӡ=
		getsym();
		expr(p);
		/* ������ */
		if (symbol != "SEMICN") {
			syntax_error('k', 0); //ӦΪ�ֺ�
		}
		else {
			printlex(); //��ӡ�ֺ�
			getsym();
		}
		condition(p);
		/* ������ */
		if (symbol != "SEMICN") {
			syntax_error('k', 0); //ӦΪ�ֺ�
		}
		else {
			printlex(); //��ӡ�ֺ�
			getsym();
		}

		/* ������ */
		index_l = p.search_vname(token);
		index_g = global.search_vname(token);
		if (index_l == -1 && index_g == -1) {
			syntax_error('c', 3);  //����δ����
		}
		if (index_l != -1) {
			if (p.sym[index_l].type == "CONSTTK") {
				syntax_error('j', 1); //�ı䳣����ֵ
			}
		}
		if (index_l == -1 && index_g != -1) {
			if (global.sym[index_g].type == "CONSTTK") {
				syntax_error('j', 2); //�ı䳣����ֵ
			}
		}
		
		printlex(); //��ӡ��ʶ��
		getsym();
		printlex(); //��ӡ=
		getsym();
		printlex(); //��ӡ��ʶ��
		getsym();
		printlex(); //��ӡ+-
		getsym();
		steplen(p);
		/* ������ */
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //ӦΪ��С����
		}
		else {
			printlex(); //��ӡ)
			getsym();
		}
		sta(p);
	}
	printsyn("<ѭ�����>");
}

//����*
void steplen(Symtable &p) {
	if (symbol != "INTCON") error();
	usINT();
	printsyn("<����>");
}


//�����*
void stas(Symtable &p) {
	int hasreturn = 0;
	while (symbol != "RBRACE") {
		int temp = sta(p);
		if (temp == 1)hasreturn = 1;
	}
	printsyn("<�����>");
}

//�����*
void rdsta(Symtable &p) {
	if (symbol != "SCANFTK") error();
	printlex(); //��ӡscanf
	getsym();
	printlex(); //��ӡ��
	getsym();
	/* ������ */
	if (p.search_vname(token)==-1 && global.search_vname(token)==-1) {
		syntax_error('c', 4);
	}
	printlex(); //��ӡ��ʶ��
	while (1) {
		getsym();
		if (symbol == "RPARENT")break;
		if (symbol == "SEMICN") {
			syntax_error('l', 1);
			break;
		}
		printlex(); //��ӡ����
		getsym();
		/* ������ */
		if (p.search_vname(token)==-1 && global.search_vname(token)==-1) {
			syntax_error('c', 4);
		}
		printlex(); //��ӡ��ʶ��
	}
	if (symbol != "RPARENT") {
		syntax_error('l', 1); //ӦΪ��С����
	}
	else {
		printlex(); //��ӡ)
		getsym();
	}
	printsyn("<�����>");
}

//д���*
void wtsta(Symtable &p) {
	if (symbol != "PRINTFTK") error();
	printlex(); //��ӡprintf
	getsym();
	printlex(); //��ӡ��
	getsym();
	if (symbol == "STRCON") {
		str();
		if (symbol == "COMMA") {
			printlex(); //��ӡ����
			getsym();
			expr(p);
		}
	}
	else expr(p);
	if (symbol != "RPARENT") {
		syntax_error('l', 1); //ӦΪ��С����
	}
	else {
		printlex(); //��ӡ)
		getsym();
	}
	printsyn("<д���>");
}

//�������*
void rsta(Symtable &p) {
	hasreturn = true;
	string ret_type;
	if (symbol != "RETURNTK") error();
	printlex(); //��ӡreturn
	getsym();
	if (symbol == "SEMICN") {
		if (p.sym[0].ret_type != "VOIDTK") {
			syntax_error('h', 1); //�з���ֵ�������ڲ�ƥ���return���
		}
	}
	if (symbol == "LPARENT") {
		/* ������ */
		if (p.sym[0].ret_type == "VOIDTK") {
			syntax_error('g', 1); //�޷���ֵ�������ڲ�ƥ���return���
		}
		printlex(); //��ӡ��
		getsym();
		ret_type = expr(p);
		/* ������ */
		if (ret_type != p.sym[0].ret_type&& p.sym[0].ret_type != "VOIDTK") {
			syntax_error('h', 1); //�з���ֵ�������ڲ�ƥ���return���
		}
		if (symbol != "RPARENT") {
			syntax_error('l', 1); //ӦΪ��С����
		}
		else {
			printlex(); //��ӡ)
			getsym();
		}
	}
	printsyn("<�������>");
}