#include "syntax_analysis.h"

//�������*
void costa() {
	if (!HandleError) {
		if (symbol == "CONSTTK")cs();
		if (symbol == "INTTK" || symbol == "CHARTK")vs();
		stas();
		printsyn("<�������>");
	}
	//else {
	//	hasreturn = false;
	//	if (symbol == "CONSTTK")cs(p);
	//	if (symbol == "INTTK" || symbol == "CHARTK")vs(p);
	//	stas(p);
	//	if (p.sym[0].ret_type != "VOIDTK" && !hasreturn) {
	//		syntax_error('h', 1); //�з���ֵ����û��return���
	//	}
	//	printsyn("<�������>");
	//}
}


//���*
int sta() {
	if(!HandleError){
		if (symbol == "IDENFR") {
			if (preload(1) == "LPARENT") {  //��������
				callrfun();
				printlex(); //��ӡ�ֺ�
				getsym();
			}
			else {	//��ֵ���
				assta();
				printlex(); //��ӡ�ֺ�
				getsym();
			}
		}
		else if (symbol == "WHILETK" || symbol == "FORTK" || symbol == "DOTK") {
			lpsta();
		}
		else if (symbol == "IFTK") {
			cdsta();
		}
		else if (symbol == "RETURNTK") {
			rsta();
			printlex(); //��ӡ�ֺ�
			getsym();
		}
		else if (symbol == "SCANFTK") {
			rdsta();
			printlex(); //��ӡ�ֺ�
			getsym();
		}
		else if (symbol == "PRINTFTK") {
			wtsta();
			printlex(); //��ӡ�ֺ�
			getsym();
		}
		else if (symbol == "LBRACE") {
			printlex(); //��ӡ {
			getsym();
			stas();
			printlex(); //��ӡ }
			getsym();
		}
		else { //�����
			printlex(); //��ӡ�ֺ�
			getsym();
		}
		printsyn("<���>");
	}
	//else {
	//	int ret_v = 0; //��ʶ������Ƿ��Ƿ������
	//	if (symbol == "IDENFR") {
	//		if (preload(1) == "LPARENT") {  //��������
	//			/* ������ */
	//			if (global.search_nfun(token) != -1) callnfun(p);
	//			else if (global.search_rfun(token) != -1) callrfun(p);
	//			else {
	//				syntax_error('c', 1); //δ����ĺ���
	//				while (symbol != "RPARENT")getsym(); //���������������β��
	//				getsym();
	//			}
	//			/* ������ */
	//			if (symbol != "SEMICN") {
	//				syntax_error('k', 1); //ӦΪ�ֺ�
	//			}
	//			else {
	//				printlex(); //��ӡ�ֺ�
	//				getsym();
	//			}
	//		}
	//		else {	//��ֵ���
	//			assta(p);
	//			/* ������ */
	//			if (symbol != "SEMICN") {
	//				syntax_error('k', 1); //ӦΪ�ֺ�
	//			}
	//			else {
	//				printlex(); //��ӡ�ֺ�
	//				getsym();
	//			}
	//		}
	//	}
	//	else if (symbol == "WHILETK" || symbol == "FORTK" || symbol == "DOTK") {
	//		lpsta(p);
	//	}
	//	else if (symbol == "IFTK") {
	//		cdsta(p);
	//	}
	//	else if (symbol == "RETURNTK") {
	//		rsta(p);
	//		/* ������ */
	//		if (symbol != "SEMICN") {
	//			syntax_error('k', 1); //ӦΪ�ֺ�
	//		}
	//		else {
	//			printlex(); //��ӡ�ֺ�
	//			getsym();
	//		}
	//		ret_v = 1;
	//	}
	//	else if (symbol == "SCANFTK") {
	//		rdsta(p);
	//		/* ������ */
	//		if (symbol != "SEMICN") {
	//			syntax_error('k', 1); //ӦΪ�ֺ�
	//		}
	//		else {
	//			printlex(); //��ӡ�ֺ�
	//			getsym();
	//		}
	//	}
	//	else if (symbol == "PRINTFTK") {
	//		wtsta(p);
	//		/* ������ */
	//		if (symbol != "SEMICN") {
	//			syntax_error('k', 1); //ӦΪ�ֺ�
	//		}
	//		else {
	//			printlex(); //��ӡ�ֺ�
	//			getsym();
	//		}
	//	}
	//	else if (symbol == "LBRACE") {
	//		printlex(); //��ӡ {
	//		getsym();
	//		stas(p);
	//		printlex(); //��ӡ }
	//		getsym();
	//	}
	//	else { //�����
	//		/* ������ */
	//		if (symbol != "SEMICN") {
	//			syntax_error('k', 1); //ӦΪ�ֺ�
	//		}
	//		else {
	//			printlex(); //��ӡ�ֺ�
	//			getsym();
	//		}
	//	}
	//	printsyn("<���>");
	//	return ret_v;
	//}
}

//��ֵ���*
void assta() {
	if (!HandleError) {
		if (symbol != "IDENFR") error();
		string l_name, r_name;
		l_name = token;
		printlex(); //��ӡ��ʶ��
		getsym();
		if (symbol == "ASSIGN") {
			printlex();
			getsym();
			expr(r_name);
			emit("=", l_name, r_name, ""); 
		}
		else {
			string arr_index;
			printlex(); //��ӡ[
			getsym();
			expr(arr_index);
			printlex(); //��ӡ]
			getsym();
			printlex(); //��ӡ=
			getsym();
			expr(r_name);
			emit("[]=", l_name, arr_index, r_name);
		}
		printsyn("<��ֵ���>");
	}
	//else {
	//	if (symbol != "IDENFR") error();
	//	/* ������ */
	//	int index_l = p.search_vname(token);
	//	int index_g = global.search_vname(token);
	//	if (index_l == -1 && index_g == -1) {
	//		syntax_error('c', 3);  //����δ����
	//	}
	//	if (index_l != -1) {
	//		if (p.sym[index_l].type == "CONSTTK") {
	//			syntax_error('j', 1); //�ı䳣����ֵ
	//		}
	//	}
	//	if (index_l == -1 && index_g != -1) {
	//		if (global.sym[index_g].type == "CONSTTK") {
	//			syntax_error('j', 2); //�ı䳣����ֵ
	//		}
	//	}
	//	printlex(); //��ӡ��ʶ��
	//	getsym();
	//	if (symbol == "ASSIGN") {
	//		printlex();
	//		getsym();
	//		expr(p);
	//	}
	//	else {
	//		printlex(); //��ӡ[
	//		getsym();
	//		if (expr(p) != "INTTK") {
	//			syntax_error('i', 1);
	//		}
	//		/* ������ */
	//		if (symbol != "RBRACK") {
	//			syntax_error('m', 1); //ӦΪ��������
	//		}
	//		else {
	//			printlex(); //��ӡ]
	//			getsym();
	//		}
	//		printlex(); //��ӡ=
	//		getsym();
	//		expr(p);
	//	}
	//	printsyn("<��ֵ���>");
	//}
}

//�������*
void cdsta() {
	if (!HandleError) {
		string jum_lab = "", last_lab = "",temp;
		//int curRegNum = getRegNum();
		if (symbol != "IFTK")error();
		printlex();
		getsym();
		/* debug */
		emit("Debug", "ifstart", "" , "");
		printlex(); //��ӡ��
		getsym();
		condition(jum_lab,false,temp);
		printlex(); //��ӡ��
		getsym();
		sta();
		last_lab = jum_lab;
		//resetRegNum(curRegNum);
		/* debug */
		emit("Debug", "ifend", "", "");
		if (symbol == "ELSETK") {
			/* debug */
			emit("Debug", "elsestart", "", "");
			jum_lab = newReg();
			printlex();
			emit("Jmp", jum_lab, "", "");
			emit("Lab", last_lab, "", "");

			getsym();
			sta();
			emit("Lab", jum_lab, "", "");
			/* debug */
			emit("Debug", "elseend", "", "");
		}
		else {
			emit("Lab", jum_lab, "", "");
		}
		printsyn("<�������>");
	}
	//else {
	//	if (symbol != "IFTK")error();
	//	printlex();
	//	getsym();
	//	printlex(); //��ӡ��
	//	getsym();
	//	condition(p);
	//	if (symbol != "RPARENT") {
	//		syntax_error('l', 1); //ӦΪ��С����
	//	}
	//	else {
	//		printlex(); //��ӡ)
	//		getsym();
	//	}
	//	sta(p);
	//	if (symbol == "ELSETK") {
	//		printlex(); //��ӡelse
	//		getsym();
	//		sta(p);
	//	}
	//	printsyn("<�������>");
	//}
}

//����*
void condition(string& jumlab1, bool isFor, string& jumlab2) {
	if (!HandleError){
		string l_name;
		expr(l_name);
		if (symbol == "LSS" || symbol == "LEQ"
			|| symbol == "GRE" || symbol == "GEQ"
			|| symbol == "NEQ" || symbol == "EQL") {
			string op = symbol;
			string r_name;
			printlex();
			getsym();
			expr(r_name);
			jumlab1 = newLab();
			if (op == "NEQ") {
				emit("==", l_name, r_name, jumlab1);
				if (isFor) emit("!=", l_name, r_name, jumlab2);
			}
			else if (op == "EQL") {
				emit("!=", l_name, r_name, jumlab1);
				if (isFor) emit("==", l_name, r_name, jumlab2);
			}
			else if (op == "LSS") {
				emit(">=", l_name, r_name, jumlab1);
				if (isFor) emit("<", l_name, r_name, jumlab2);
			}
			else if (op == "LEQ") {
				emit(">", l_name, r_name, jumlab1);
				if (isFor) emit("<=", l_name, r_name, jumlab2);
			}
			else if (op == "GTR") {
				emit("<=", l_name, r_name, jumlab1);
				if (isFor) emit(">", l_name, r_name, jumlab2);
			}
			else if (op == "GEQ") {
				emit("<", l_name, r_name, jumlab1);
				if (isFor) emit(">=", l_name, r_name, jumlab2);
			}
		}
		else {
			jumlab1 = newLab();
			emit("==", l_name, "0", jumlab1);
		}
		printsyn("<����>");
	}
	//else {
	//	string l_type = "INTTK";
	//	string r_type = "INTTK";
	//	l_type = expr(p);
	//	if (symbol == "LSS" || symbol == "LEQ"
	//		|| symbol == "GRE" || symbol == "GEQ"
	//		|| symbol == "NEQ" || symbol == "EQL") {
	//		printlex(); //��ӡ<>=
	//		getsym();
	//		r_type = expr(p);
	//	}
	//	/* ������ */
	//	if (l_type != "INTTK" || r_type != "INTTK") {
	//		syntax_error('f', 1);
	//	}
	//	printsyn("<����>");
	//}
}

//ѭ�����*
void lpsta() {
	if (!HandleError) {
		if (symbol == "WHILETK") {
			string jum_lab = newLab();
			string top_lab = jum_lab;
			string temp;
			emit("Lab", top_lab, "", "");
			printlex();
			getsym();
			printlex(); //��ӡ��
			getsym();
			condition(jum_lab, false, temp);
			printlex(); //��ӡ��
			getsym();
			sta();
			emit("Jmp", top_lab, "", "");
			emit("Lab", jum_lab, "", "");
		}
		else if (symbol == "DOTK") {
			string jum_lab = newLab();
			string top_lab = jum_lab;
			string temp;
			emit("Lab", top_lab, "", "");
			printlex();
			getsym();
			sta();
			printlex(); //��ӡwhile
			getsym();
			printlex();//��ӡ��
			getsym();
			condition(jum_lab,false,temp);
			printlex(); //��ӡ��
			getsym();
			emit("Jmp", top_lab, "", "");
			emit("Lab", jum_lab, "", "");
		}
		else if (symbol == "FORTK") {
			printlex();
			getsym();
			printlex(); //��ӡ��

			string con_entry, add_entry;
			string lp_entry, lp_end;
			con_entry = newLab();
			add_entry = newLab();
			lp_entry = newLab();
			lp_end = newLab();

			//part1
			string l_name, r_name;
			getsym();
			l_name = token;
			printlex(); //��ӡ��ʶ��
			getsym();
			printlex(); //��ӡ=
			getsym();
			expr(r_name);
			emit("=", l_name, r_name, "");
			emit("Jmp", lp_entry, "", "");
			emit("Lab", con_entry, "", "");
			printlex(); //��ӡ�ֺ�

			//part2
			getsym();
			condition(lp_end,true,lp_entry);
			printlex();//��ӡ�ֺ�

			//part3
			string ll_name, rr_name, op, num;
			emit("Lab", add_entry, "", "");
			getsym();
			printlex(); //��ӡ��ʶ��
			ll_name = token;
			getsym();
			printlex(); //��ӡ=
			getsym();
			rr_name = token;
			printlex(); //��ӡ��ʶ��
			getsym();
			printlex(); //��ӡ+-
			op = token;
			getsym();
			steplen(num);
			emit(op, rr_name, num, ll_name);
			emit("Jml", con_entry, "", "");
			emit("Lab", lp_entry, "", "");
			printlex();//��ӡ��

			//part4
			getsym();
			sta();
			emit("Jml", add_entry, "", "");
			emit("Lab", lp_entry, "", "");
		}
		printsyn("<ѭ�����>");
	}
	//else {
	//	if (symbol == "whiletk") {
	//		printlex(); //��ӡwhile
	//		getsym();
	//		printlex(); //��ӡ��
	//		getsym();
	//		condition(p);
	//		if (symbol != "rparent") {
	//			syntax_error('l', 1); //ӦΪ��С����
	//		}
	//		else {
	//			printlex(); //��ӡ)
	//			getsym();
	//		}
	//		sta(p);
	//	}
	//	else if (symbol == "dotk") {
	//		printlex(); //��ӡdo
	//		getsym();
	//		sta(p);
	//		/* ������ */
	//		if (symbol != "whiletk") {
	//			syntax_error('n', 1);  //ȱ��while
	//		}
	//		else {
	//			printlex(); //��ӡwhile
	//			getsym();
	//		}
	//		if (symbol == "lparent") {
	//			printlex();//��ӡ��
	//			getsym();
	//			condition(p);
	//			if (symbol != "rparent") {
	//				syntax_error('l', 1); //ӦΪ��С����
	//			}
	//			else {
	//				printlex(); //��ӡ)
	//				getsym();
	//			}
	//		}
	//	}
	//	else if (symbol == "fortk") {
	//		printlex(); //��ӡfor
	//		getsym();
	//		printlex(); //��ӡ��
	//		getsym();
	//		/* ������ */
	//		int index_l = p.search_vname(token);
	//		int index_g = global.search_vname(token);
	//		if (index_l == -1 && index_g == -1) {
	//			syntax_error('c', 3);  //����δ����
	//		}
	//		if (index_l != -1) {
	//			if (p.sym[index_l].type == "consttk") {
	//				syntax_error('j', 1); //�ı䳣����ֵ
	//			}
	//		}
	//		if (index_l == -1 && index_g != -1) {
	//			if (global.sym[index_g].type == "consttk") {
	//				syntax_error('j', 2); //�ı䳣����ֵ
	//			}
	//		}
	//		printlex(); //��ӡ��ʶ��
	//		getsym();
	//		printlex(); //��ӡ=
	//		getsym();
	//		expr(p);
	//		/* ������ */
	//		if (symbol != "semicn") {
	//			syntax_error('k', 0); //ӦΪ�ֺ�
	//		}
	//		else {
	//			printlex(); //��ӡ�ֺ�
	//			getsym();
	//		}
	//		condition(p);
	//		/* ������ */
	//		if (symbol != "semicn") {
	//			syntax_error('k', 0); //ӦΪ�ֺ�
	//		}
	//		else {
	//			printlex(); //��ӡ�ֺ�
	//			getsym();
	//		}
	//		/* ������ */
	//		index_l = p.search_vname(token);
	//		index_g = global.search_vname(token);
	//		if (index_l == -1 && index_g == -1) {
	//			syntax_error('c', 3);  //����δ����
	//		}
	//		if (index_l != -1) {
	//			if (p.sym[index_l].type == "consttk") {
	//				syntax_error('j', 1); //�ı䳣����ֵ
	//			}
	//		}
	//		if (index_l == -1 && index_g != -1) {
	//			if (global.sym[index_g].type == "consttk") {
	//				syntax_error('j', 2); //�ı䳣����ֵ
	//			}
	//		}
	//		printlex(); //��ӡ��ʶ��
	//		getsym();
	//		printlex(); //��ӡ=
	//		getsym();
	//		printlex(); //��ӡ��ʶ��
	//		getsym();
	//		printlex(); //��ӡ+-
	//		getsym();
	//		steplen(p);
	//		/* ������ */
	//		if (symbol != "rparent") {
	//			syntax_error('l', 1); //ӦΪ��С����
	//		}
	//		else {
	//			printlex(); //��ӡ)
	//			getsym();
	//		}
	//		sta(p);
	//	}
	//	printsyn("<ѭ�����>");
	//}
}

//����*
void steplen(string& num) {
	if (!HandleError) {
		num = "";
		if (symbol != "INTCON") error();
		usINT(num);
		printsyn("<����>");
	}
	/*else {
		num = "";
		if (symbol != "INTCON") error();
		usINT(num);
		printsyn("<����>");
	}*/
}

//�����*
void stas() {
	if (!HandleError) {
		while (symbol != "RBRACE") sta();
		printsyn("<�����>");
	}
	/*else {
		int hasreturn = 0;
		while (symbol != "RBRACE") {
			int temp = sta(p);
			if (temp == 1)hasreturn = 1;
		}
		printsyn("<�����>");
	}*/
}

//�����*
void rdsta() {
	if (!HandleError) {
		if (symbol != "SCANFTK") error();
		string name;
		printlex(); //��ӡscanf
		getsym();
		printlex(); //��ӡ��
		getsym();
		name = token;
		printlex(); //��ӡ��ʶ��
		emit("Scan", name, "", "");
		while (1) {
			getsym();
			if (symbol == "RPARENT")break;
			printlex(); //��ӡ����
			getsym();
			emit("Scan", name, "", "");
			printlex(); //��ӡ��ʶ��
		}
		printlex(); //��ӡ��
		printsyn("<�����>");
		getsym();
	}
	//else {
	//	if (symbol != "SCANFTK") error();
	//	printlex(); //��ӡscanf
	//	getsym();
	//	printlex(); //��ӡ��
	//	getsym();
	//	/* ������ */
	//	if (p.search_vname(token) == -1 && global.search_vname(token) == -1) {
	//		syntax_error('c', 4);
	//	}
	//	printlex(); //��ӡ��ʶ��
	//	while (1) {
	//		getsym();
	//		if (symbol == "RPARENT")break;
	//		if (symbol == "SEMICN") {
	//			syntax_error('l', 1);
	//			break;
	//		}
	//		printlex(); //��ӡ����
	//		getsym();
	//		/* ������ */
	//		if (p.search_vname(token) == -1 && global.search_vname(token) == -1) {
	//			syntax_error('c', 4);
	//		}
	//		printlex(); //��ӡ��ʶ��
	//	}
	//	if (symbol != "RPARENT") {
	//		syntax_error('l', 1); //ӦΪ��С����
	//	}
	//	else {
	//		printlex(); //��ӡ)
	//		getsym();
	//	}
	//	printsyn("<�����>");
	//}
}

//д���*
void wtsta() {
	if (!HandleError) {
		string print_str="", reg_name, ret_type;
		if (symbol != "PRINTFTK") error();
		printlex(); //��ӡprintf
		getsym();
		printlex(); //��ӡ��
		getsym();
		if (symbol == "STRCON") {
			print_str = token;
			str();
			if (symbol == "COMMA") {
				printlex(); //��ӡ����
				getsym();
				ret_type = expr(reg_name);
				emit("Print", print_str, ret_type, reg_name);
			}
			emit("Print", print_str, "", "");
		}
		else {
			ret_type = expr(reg_name);
			emit("Print", print_str, ret_type, reg_name);
		}
		printlex(); // ��ӡ��
		printsyn("<д���>");
		getsym();
	}
	//else {
	//	if (symbol != "PRINTFTK") error();
	//	printlex(); //��ӡprintf
	//	getsym();
	//	printlex(); //��ӡ��
	//	getsym();
	//	if (symbol == "STRCON") {
	//		str();
	//		if (symbol == "COMMA") {
	//			printlex(); //��ӡ����
	//			getsym();
	//			expr(p);
	//		}
	//	}
	//	else expr(p);
	//	if (symbol != "RPARENT") {
	//		syntax_error('l', 1); //ӦΪ��С����
	//	}
	//	else {
	//		printlex(); //��ӡ)
	//		getsym();
	//	}
	//	printsyn("<д���>");
	//}
}

//�������*
void rsta() {
	if (!HandleError) {
		if (symbol != "RETURNTK") error();
		printlex();
		getsym();
		if (symbol != "SEMICN") {
			string reg_name;
			printlex(); //��ӡ��
			getsym();
			expr(reg_name);
			emit("Ret", reg_name, "", "$End_" + syt[level].sym[0].name);
			printlex(); //��ӡ��
			getsym();
		}
		else {
			emit("Ret", "", "", "$End_" + syt[level].sym[0].name);
		}
		printsyn("<�������>");
	}
	//else {
	//	hasreturn = true;
	//	string ret_type;
	//	if (symbol != "RETURNTK") error();
	//	printlex(); //��ӡreturn
	//	getsym();
	//	if (symbol == "SEMICN") {
	//		if (p.sym[0].ret_type != "VOIDTK") {
	//			syntax_error('h', 1); //�з���ֵ�������ڲ�ƥ���return���
	//		}
	//	}
	//	if (symbol == "LPARENT") {
	//		/* ������ */
	//		if (p.sym[0].ret_type == "VOIDTK") {
	//			syntax_error('g', 1); //�޷���ֵ�������ڲ�ƥ���return���
	//		}
	//		printlex(); //��ӡ��
	//		getsym();
	//		ret_type = expr(p);
	//		/* ������ */
	//		if (ret_type != p.sym[0].ret_type && p.sym[0].ret_type != "VOIDTK") {
	//			syntax_error('h', 1); //�з���ֵ�������ڲ�ƥ���return���
	//		}
	//		if (symbol != "RPARENT") {
	//			syntax_error('l', 1); //ӦΪ��С����
	//		}
	//		else {
	//			printlex(); //��ӡ)
	//			getsym();
	//		}
	//	}
	//	printsyn("<�������>");
	//}
}