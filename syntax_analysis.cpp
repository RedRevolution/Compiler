#include "lexical_analysis.c"
#include <stdlib.h>

char nfunID[][N];
int nfuncnt;

//�����޷���ֵ������ʶ����
int find(char s[]) {
	for (int i = 0; i < nfuncnt; i++) {
		if (strcmp(nfunID[i], s) == 0) {
			return 1;
		}
	}
	return 0;
}

void error() {
	printf("error");
	system("pause");
}

void printsyn(const char s[]) {
	fputs(s, out);
	fputs("\n", out);
}

//����*
void program() {
	//����˵��
	if (symbol == "CONSTTK") {
		cs();
	}
	//����˵��
	if (symbol == "INTTK" || symbol == "CHARTK" && !preload(2)) {
		vs();
	}
	//��|�޷���ֵ��������
	while (!(symbol == "VOIDTK" && preload(1) == 2)) {
		if (symbol == "INTTK" || symbol == "CHARTK" && preload(2))rfun();
		else nfun();
	}
	mfun();
	printsyn("<����>");
}

//�ַ���*
void str() {
	if (symbol != "STRCON") error();
	printlex();
	printsyn("<�ַ���>");
	getsym();
}

//����˵��*
void cs() {
	if (symbol != "CONSTTK") error();
	printlex(); 
	while (1) {
		getsym();
		cd();
		printlex(); //��ӡ�ֺ�
		getsym();
		if (symbol != "CONSTTK")break;
		printlex(); //��ӡconst
	}
	printsyn("<����˵��>");
}

//��������*
void cd() {
	if (symbol != "INTTK" && symbol != "CHARTK") error();
	printlex(); //��ӡint|char
	if (symbol == "INTTK") {
		while(1){
			getsym(); //��ʶ��
			printlex();
			getsym(); // =
			printlex();
			getsym();
			INT();
			if (symbol != "COMMA") break; //�ֺ��˳�
			printlex(); //��ӡ����
		}
	}
	else {
		while (1) {
			getsym(); //��ʶ��
			printlex();
			getsym(); // =
			printlex();
			getsym(); //�ַ�
			printlex();
			getsym();
			if (symbol != "COMMA") break;
			printlex(); //��ӡ����
		}
	}
	printsyn("<��������>");
}

//����*
void INT() {
	if (symbol != "PLUS" && symbol != "MINU" && symbol != "INTCON") error();
	if (symbol == "PLUS" || symbol == "MINU") {
		printlex(); //��ӡ+-
		getsym();
	}
	usINT();
	printsyn("<����>");
}

//�޷�������*
void usINT() {
	if (symbol != "INTCON") error();
	printlex(); //��ӡ����
	printsyn("<�޷�������>");
	getsym();
}


//����ͷ��*
void dh() {
	if (symbol != "INTTK" && symbol != "CHARTK") error();
	printlex();
	getsym(); //��ʶ��
	printlex(); //��ӡ��ʶ��
	printsyn("<����ͷ��>");
	getsym();
}

//����˵��*
void vs() {
	if (symbol != "INTTK" && symbol != "CHARTK") error();
	while (1) {
		vd();
		printlex(); //��ӡ�ֺ�
		getsym();
		if (symbol != "INTTK" && symbol != "CHARTK")break;
		if (preload(2))break; //����������Ǳ�������
	}
	printsyn("<����˵��>");
}

//��������*
void vd() {
	if (symbol != "INTTK" && symbol != "CHARTK") error();
	printlex();
	while (1) {
		getsym(); //����һ����ʶ��
		printlex();
		getsym(); //�����Ƕ��ţ��ֺŻ���[
		// [
		if (symbol == "LBRACK") {  
			printlex(); //��ӡ[
			getsym(); //�����޷�������
			usINT();
			printlex(); //��ӡ]
			getsym(); // ���붺��|�ֺ�
		}
		if (symbol == "SEMICN")break; //�ֺ��˳�
		printlex(); //��ӡ����
	}
	printsyn("<��������>");
}

//�з���ֵ��������*
void rfun() {
	if (symbol != "INTTK" && symbol != "CHARTK") error();
	dh();
	printlex(); //��ӡ��
	getsym(); 
	pt();
	printlex(); //��ӡ��
	getsym();
	printlex(); //��ӡ{
	getsym();
	costa();
	printlex(); //��ӡ}
	printsyn("<�з���ֵ��������>");
	getsym();
}

//�޷���ֵ��������*
void nfun() {
	if (symbol != "VOIDTK") error();
	getsym(); //�����ʶ��
	strcpy(nfunID[nfuncnt++], token); //���
	printlex(); //��ӡ��ʶ��
	getsym();
	printlex(); //��ӡ��
	getsym();
	pt();
	printlex(); //��ӡ��
	getsym();
	printlex(); //��ӡ{
	getsym();
	costa();
	printlex(); //��ӡ}
	printsyn("<�޷���ֵ��������>");
	getsym();
}

//�������*
void costa() {
	if (symbol == "CONSTTK")cs();
	if (symbol == "INTTK" || symbol == "CHARTK")vs();
	stas();
	printsyn("�������");
}

//������*
void pt() {
	//������Ϊ��
	if (symbol == "INTTK" || symbol == "CHARTK") {
		printlex(); //��ӡ int|char
		while (1) {
			getsym(); //�����ʶ��
			printlex(); //��ӡ��ʶ��
			getsym(); //���붺�Ż��ߣ�
			if (symbol != "COMMA")break;
			printlex(); //��ӡ����
			getsym(); //���� int|char
			printlex(); //��ӡ int|char
		}
	}
	printsyn("<������>");
}

//������*
void mfun() {
	if (symbol != "VOIDTK") error();
	getsym(); //�����ʶ��main
	printlex(); //��ӡ��ʶ��main
	getsym();
	printlex(); //��ӡ��
	getsym();
	printlex(); //��ӡ��
	getsym();
	printlex(); //��ӡ{
	getsym();
	costa();
	printlex(); //��ӡ}
	printsyn("<������>"); //��������������getsym
}

//���ʽ*
void expr() {
	if (symbol == "PLUS" || symbol == "MINU") {
		printlex();
		getsym();
	}
	while (1) {
		term();
		if (symbol != "PLUS" && symbol != "MINU")break;
		printlex(); //��ӡ������
		getsym();
	}
	printsyn("<���ʽ>");
}

//��*
void term() {
	factor();
	while (symbol == "MUL" || symbol == "DIV") {
		printlex();
		getsym();
		factor();
	}
	printsyn("<��>");
}

//����*
void factor() {
	if (symbol == "IDENFR") {
		//�з���ֵ���ú���
		if (preload(1)) {
			callrfun();
			printsyn("<����>");
			return;
		}

		printlex(); //��ӡ��ʶ��
		getsym();
		//��ʶ��[���ʽ]
		if (symbol == "LBRACK") {
			printlex(); //��ӡ[
			getsym();
			expr();
			printlex(); //��ӡ]
			getsym();
		}
	}
	else if (symbol == "LPARENT") {
		printlex(); //��ӡ��
		getsym();
		expr();
		printlex(); //��ӡ��
		getsym();
	}
	else if (symbol == "PLUS" || symbol == "MINU" || symbol == "INTCON") {
		INT();
	}
	else if (symbol == "CHARCON") {
		printlex();
		getsym();
	}
	printsyn("<����>");
}

//���*
void sta() {
	if (symbol == "IDENFR") {
		if (preload(1)) {  //��������
			if (find(token)) callnfun();
			else callrfun();
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
		stas();
		printlex(); //��ӡ }
		getsym();
	}
	printsyn("<���>");
}

//��ֵ���*
void assta() {
	if (symbol != "IDENFR") error();
	printlex(); //��ӡ��ʶ��
	getsym();
	if (symbol == "ASSIGN") {
		printlex();
		getsym();
		expr();
	}
	else {
		printlex(); //��ӡ[
		getsym();
		expr();
		printlex(); //��ӡ]
		getsym();
		printlex(); //��ӡ=
		getsym();
		expr();
	}
	printsyn("<��ֵ���>");
}

//�������*
void cdsta() {
	if (symbol != "IFTK")error();
	printlex();
	getsym();
	printlex(); //��ӡ��
	getsym();
	condition();
	printlex(); //��ӡ��
	getsym();
	sta();
	if (symbol == "ELSETK") {
		printlex();
		getsym();
		sta();
	}
	printsyn("<�������>");
}

//����*
void condition() {
	expr();
	if (symbol == "LSS" || symbol == "LEQ"
			|| symbol == "GRE" || symbol == "GEQ"
			|| symbol == "NEQ" || symbol == "EQL") {
		printlex();
		getsym();
		expr();
	}
	printsyn("<����>");
}

//ѭ�����*
void lpsta() {
	if (symbol == "WHILETK") {
		printlex();
		getsym();
		printlex(); //��ӡ��
		getsym();
		condition();
		printlex(); //��ӡ��
		getsym();
		sta();
	}
	else if (symbol == "DOTK") {
		printlex();
		getsym();
		sta();
		printlex(); //��ӡwhile
		getsym();
		printlex();//��ӡ��
		getsym();
		condition();
		printlex(); //��ӡ��
		getsym();
	}
	else if (symbol == "FORTK") {
		printlex();
		getsym();
		printlex(); //��ӡ��
		getsym();
		printlex(); //��ӡ��ʶ��
		getsym();
		printlex(); //��ӡ=
		getsym();
		expr();
		printlex(); //��ӡ�ֺ�
		getsym();
		condition();
		printlex();//��ӡ�ֺ�
		getsym();
		printlex(); //��ӡ��ʶ��
		getsym();
		printlex(); //��ӡ=
		getsym();
		printlex(); //��ӡ��ʶ��
		getsym();
		printlex(); //��ӡ+-
		getsym();
		steplen();
		printlex();//��ӡ��
		getsym();
		sta();
	}
	printsyn("<ѭ�����>");
}

//����*
void steplen() {
	if (symbol != "INTCON") error();
	usINT();
	printsyn("<����>");
}

//�з���ֵ�����������*
void callrfun() {
	if (symbol != "IDENFR") error();
	printlex(); //��ӡ��ʶ��
	getsym(); 
	printlex(); //��ӡ��
	getsym();
	vpt();
	printlex(); //��ӡ�� 
	printsyn("<�з���ֵ�����������>");
	getsym();
}

//�޷���ֵ�����������*
void callnfun() {
	if (symbol != "IDENFR") error();
	printlex(); //��ӡ��ʶ��
	getsym();
	printlex(); //��ӡ��
	getsym();
	vpt();
	printlex(); //��ӡ�� 
	printsyn("<�޷���ֵ�����������>");
	getsym();
}

//ֵ������*
void vpt() {
	if (symbol != "RPARENT") {
		expr();
		while (1) {
			if (symbol != "COMMA")break;
			printlex(); //��ӡ����
			getsym();
			expr();
		}
	}
	printsyn("<ֵ������>");
}

//�����*
void stas() {
	while (symbol != "RBRACE") sta();
	printsyn("<�����>");
}

//�����*
void rdsta() {
	if (symbol != "SCANFTK") error();
	printlex(); //��ӡscanf
	getsym();
	printlex(); //��ӡ��
	getsym();
	printlex(); //��ӡ��ʶ��
	while (1) {
		getsym();
		if (symbol == "RPARENT")break;
		printlex(); //��ӡ����
		getsym();
		printlex(); //��ӡ��ʶ��
	}
	printlex(); //��ӡ��
	printsyn("<�����>");
	getsym();
}

//д���*
void wtsta() {
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
			expr();
		}
	}
	else expr();
	printlex(); // ��ӡ��
	printsyn("<д���>");
	getsym();
}

//�������*
void rsta() {
	if (symbol != "RETURNTK") error();
	printlex();
	getsym();
	if (symbol != "SEMICN") {
		printlex(); //��ӡ��
		getsym();
		expr();
		printlex(); //��ӡ��
		getsym();
	}
	printsyn("<�������>");
}


int main() {
	in = fopen("testfile.txt", "rt+");
	out = fopen("output.txt", "w");
	getsym();
	program();
	fclose(in);
	fclose(out);
	return 0;
}