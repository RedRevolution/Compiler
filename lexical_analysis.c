#include <stdio.h>
#include <string.h>
#define N 300

char token[N], symbol[10], a;
FILE* in, * out;

int isLetter() {
	return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || a == '_';
}

int isDigit() {
	return (a >= '0' && a <= '9');
}

void reserver() {
	if (strcmp(token, "const") == 0)strcpy(symbol, "CONSTTK");
	else if (strcmp(token, "int") == 0)strcpy(symbol, "INTTK");
	else if (strcmp(token, "char") == 0)strcpy(symbol, "CHARTK");
	else if (strcmp(token, "void") == 0)strcpy(symbol, "VOIDTK");
	else if (strcmp(token, "main") == 0)strcpy(symbol, "MAINTK");
	else if (strcmp(token, "if") == 0)strcpy(symbol, "IFTK");
	else if (strcmp(token, "else") == 0)strcpy(symbol, "ELSETK");
	else if (strcmp(token, "do") == 0)strcpy(symbol, "DOTK");
	else if (strcmp(token, "while") == 0)strcpy(symbol, "WHILETK");
	else if (strcmp(token, "for") == 0)strcpy(symbol, "FORTK");
	else if (strcmp(token, "scanf") == 0)strcpy(symbol, "SCANFTK");
	else if (strcmp(token, "printf") == 0)strcpy(symbol, "PRINTFTK");
	else if (strcmp(token, "return") == 0)strcpy(symbol, "RETURNTK");
	else strcpy(symbol, "IDENFR");
}

void strcatAchar() {
	char temp[N];
	temp[0] = a;
	temp[1] = '\0';
	strcat(token, temp);
}

void reserver2() {
	if (a == '+') strcpy(symbol, "PLUS");
	else if (a == '-') strcpy(symbol, "MINU");
	else if (a == '*') strcpy(symbol, "MULT");
	else if (a == '/') strcpy(symbol, "DIV");
	else if (a == ';') strcpy(symbol, "SEMICN");
	else if (a == ',') strcpy(symbol, "COMMA");
	else if (a == '(') strcpy(symbol, "LPARENT");
	else if (a == ')') strcpy(symbol, "RPARENT");
	else if (a == '[') strcpy(symbol, "LBRACK");
	else if (a == ']') strcpy(symbol, "RBRACK");
	else if (a == '{') strcpy(symbol, "LBRACE");
	else if (a == '}') strcpy(symbol, "RBRACE");
}

int getsym(){
	strcpy(token, "");  //����ַ���
	if ((a = getc(in)) == EOF) return 0; //�ȶ���һ���ַ�����ֹisspace����
	while (isspace(a)) {	 /*��ȡ�ַ��������ո񡢻��к�Tab,\t\f��*/
		if ((a = getc(in)) == EOF) return 0;
	}
	if (isLetter()) {
		while (isLetter() || isDigit()) {
			strcatAchar();		/*���ַ�ƴ�ӳ��ַ���*/
			a = getc(in);
		}
		ungetc(a, in);         /*ָ�����һ���ַ�*/
		reserver();
	}
	//����
	else if (isDigit()) {
		while (isDigit()) {
			strcatAchar();
			a = getc(in);
		}
		ungetc(a, in);
		strcpy(symbol, "INTCON");
	}
	//�ַ���
	else if (a == '"') {
		a = getc(in);
		while (a != '"') {
			strcatAchar();
			a = getc(in);
		}
		strcpy(symbol, "STRCON");
	}
	//�ַ�����
	else if (a == '\'') {
		a = getc(in);
		strcatAchar();
		a = getc(in);  //ִ�����a = ������
		strcpy(symbol, "CHARCON");
	}
	//���ڵ��ںʹ���
	else if (a == '>') {
		strcatAchar();
		a = getc(in);
		if (a == '=') {
			strcatAchar();
			strcpy(symbol, "GEQ");
		}
		else {
			ungetc(a, in);
			strcpy(symbol, "GRE");
		}
	}
	//С�ڵ��ں�С��
	else if (a == '<') {
		strcatAchar();
		a = getc(in);
		if (a == '=') {
			strcatAchar();
			strcpy(symbol, "LEQ");
		}
		else {
			ungetc(a, in);
			strcpy(symbol, "LSS");
		}
	}
	//˫�Ⱥź͸�ֵ
	else if (a == '=') {
		strcatAchar();
		a = getc(in);
		if (a == '=') {
			strcatAchar();
			strcpy(symbol, "EQL");
		}
		else {
			ungetc(a, in);
			strcpy(symbol, "ASSIGN");
		}
	}
	//������
	else if (a == '!') {
		strcatAchar();
		a = getc(in);
		strcatAchar();
		strcpy(symbol, "NEQ");
	}
	//��������
	else {
		reserver2();
		strcatAchar();
	}
	return 1;
}

//ָ���ƻص���ǰ�Ѷ������ʵĿ�ͷλ��
int ungetsym() {
	int i = strlen(token);
	while (i--) unget(a, in);
}


//Ԥ��  int|char+��ʶ�� ��һ������(�ַ�)  x = 2
//Ԥ����һ������   x = 1
int preload(int x) {
	int value = 0, count;
	char ttmp[N], stmp[10]; //�Ե�ǰ���ʽ��б���
	strcpy(ttmp, token);  
	strcpy(stmp, symbol);
	
	if (x == 1) {
		getsym();
		if (strcmp(symbol, "LPARENT") == 0)value = 1;
		if (strcmp(symbol, "MAINTK") == 0)value = 2; 
		ungetsym();
		strcpy(token, ttmp);//��ԭ����
		strcpy(symbol, stmp);
		return value;
	}

	getsym(); //ȡ�ñ�ʶ��
	count = strlen(token); //��ʶ���ĳ���

	//����һ�����ʣ��ո�Ҳ�����
	a = getc(in);
	count++;
	while (isspace(a)) {
		a = getc(in);
		count++;
	}
	if (a == '(')value = 1; //���Ϊ"(" ��ʾ�Ǻ���������1
	
	while (count--)ungetc(a, in); //���˵���ʶ����ͷλ��
	strcpy(token, ttmp);//��ԭ����
	strcpy(symbol, stmp);
	return value;
}

//��ӡ��ǰ����
void printlex() {
	fputs(symbol, out);
	fputs(" ", out);
	fputs(token, out);
	fputs("\n", out);
}