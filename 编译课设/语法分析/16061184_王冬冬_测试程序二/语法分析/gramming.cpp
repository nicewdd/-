#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "compiler.h"
/*
const 1
int 2
char 3
void 4
if 5
else 6
while 7
for 8
scanf 9
printf 10
return 11
main 12
+ 13
- 14
* 15
/ 16
, 17
; 18
( 19
) 20
[ 21
] 22
{ 23
} 24
= 25
< 26
<= 27
> 28
>= 29
!= 30
== 31
��ʶ�� 32
�޷������� 33
�ַ��� 34
�ַ� 35
������ 36
˫���� 37
*/
FILE *fp;
char c;
int m;//���������͵ķ���ֵ
int ret_value;//����ص�ֵ
int temp_plus_mins;//��ʱ�������
int int_char;//0��ʾint��1��ʾchar
int tab_num = 0;//���ű�����ĵ�ǰλ��
int mid_num = 0;//�м���������ĵ�ǰλ��
int label_num = 1;
int t_num = 1;
int func_type = 0;
int return_num = 0;
char sym_name[130];//���ű��з��ŵ�����
char label_name[130];
int str_num = 1;
char str_name[130];
char t_name[130];
char mid_argu1[130];
char mid_argu2[130];
char mid_result[130];
int main_begin = 0;//main������ʼ�ı�־
int sym_type;//���ű��з��ŵ�����
int mid_id;//�м������е�����
int sym_num;//���ű��з��ŵ�ֵ
int mid_value;//�м������е�����ֵ
int sym_addr = 0;//���ű��з��ŵĵ�ַ
int sym_level = 0;
int para_type;//���ú�������������
int left_type;//��ֵ�������ʶ��������
int right_type;//��ֵ����Ҳ���ʽ������
char word[130];
char lowsym[130];
int line = 1;
int charnum = 0;
int printf_flag = 0;//��¼printf�б��ʽ������
int flAg1 = 0, flag_2 = 0;//flag1Ϊ������ƥ���жϣ�flag_2Ϊ˫����ƥ���ж�
int varfinish = 0;
int funcfinish = 0;
int func_judge = 0;
void label_creat();
void t_creat();
void str_creat();
int getword(FILE *fp, char *w);
void err_mes(int n);
int tra_num(int a, char b[]);
int var_type(char func_name[], char var_name[]);
void enter_sym_tab(char sym_name[], int sym_type, int sym_num, int sym_addr, int level);
void enter_mid_tab(int mid_id, char mid_argu1[], char mid_argu2[], char mid_result[], int mid_num);
void enter_func_tab(char func_name[]);
void enter_var_tab(char var_name[], int type, int addr);
void enter_tab(int mid_num);
int look_tab(char sym_name[]);
int look_func(char sym_name[]);
int over_con_type(char var_name[]);
int local_var_type(char func_name[], char var_name[]);
int var_pos1(char func_name[], char var_name[]);
int var_pos2(char func_name[], char var_name[]);
int func_pos(char func_name[]);
int addr_var(char func_name[], char var_name[]);//���ұ�����Ϣ���б����ĵ�ַ
void int_ass();
void char_ass();
void array_def();
void con_def();
void var_def();
void comp_state();
void statement();
void while_state();
void for_state();
void if_statement();
void scanf_state();
void printf_state();
void return_state();
void condition();
void expression();
void item();
void factor();
void value_para_tab();
void para_tab();
void return_value();
void  no_return_value();
void main_fun();
void mid_code(int mid_num);
void numtostr(int num);
void mid_update(int mid_num);
void goal_code(int mid_num,int sym_num);
void creat_data(int mid_num,int sym_num,FILE *goal);
void creat_text(int mid_num, FILE *goal);
int tolow_com(char w[]);
void low_sym(char w[]);
void low_sym(char w[]) {
	char t[130];
	int i;
	for (i = 0; w[i] != '\0'; i++) {
		t[i] = tolower(w[i]);
	}
	t[i] = '\0';
	strcpy(lowsym, t);
}
int tolow_com(char w[]) {
	char t[130];
	int i;
	for (i = 0; w[i] != '\0'; i++) {
		t[i] = tolower(w[i]);
	}
	t[i] = '\0';
	if (!strcmp(t, "const")) {
		return 1;
	}
	else if (!strcmp(t, "int")) {
		return 2;
	}
	else if (!strcmp(t, "char")) {
		return 3;
	}
	else if (!strcmp(t, "void")) {
		return 4;
	}
	else if (!strcmp(t, "if")) {
		return 5;
	}
	else if (!strcmp(t, "else")) {
		return 6;
	}
	else if (!strcmp(t, "while")) {
		return 7;
	}
	else if (!strcmp(t, "for")) {
		return 8;
	}
	else if (!strcmp(t, "scanf")) {
		return 9;
	}
	else if (!strcmp(t, "printf")) {
		return 10;
	}
	else if (!strcmp(t, "return")) {
		return 11;
	}
	else if (!strcmp(t, "main")) {
		return 12;
	}
	else {
		return 32;
	}
}
char *stcat(char *s, char *t)
{
	char *pt = s;
	while (*s != '\0') s++;
	while (*t != '\0') *s++ = *t++;
	*s = '\0';
	return pt;
}
void label_creat() {
	char s[130];
	char t[130] = { 'l','a','b','e','l','\0' };
	int num;
	int i = 0;
	int j;
	char c;
	num = label_num;
	label_num++;
	while (num) {
		s[i++] = num % 10 + '0';
		num = num / 10;
	}
	s[i] = '\0';
	for (j = 0; j <= (i-1) / 2; j++) {
		c = s[j];
		s[j] = s[i - j - 1];
		s[i - j - 1] = c;
	}
	stcat(t, s);
	strcpy(label_name, t);
}
void t_creat() {
	char s[130];
	char t[130] = { '@','t','\0' };
	int num;
	int i = 0;
	int j;
	char c;
	num = t_num;
	t_num++;
	while (num) {
		s[i++] = num % 10 + '0';
		num = num / 10;
	}
	s[i] = '\0';
	for (j = 0; j <= (i - 1) / 2; j++) {
		c = s[j];
		s[j] = s[i - j - 1];
		s[i - j - 1] = c;
	}
	stcat(t, s);
	strcpy(t_name, t);
}
void str_creat() {
	char s[130];
	char t[130] = { 's','t','r','\0' };
	int num;
	int i = 0;
	int j;
	char c;
	num = str_num;
	str_num++;
	while (num) {
		s[i++] = num % 10 + '0';
		num = num / 10;
	}
	s[i] = '\0';
	for (j = 0; j <= (i - 1) / 2; j++) {
		c = s[j];
		s[j] = s[i - j - 1];
		s[i - j - 1] = c;
	}
	stcat(t, s);
	strcpy(str_name, t);
}
int getword(FILE *fp, char *w) {
	int c;
	int state = 1;
	while ((c = fgetc(fp)) != EOF) {
		if (flag_2 == 1) {
			state = 7;
		}
		if (flAg1 == 1) {
			state = 8;
		}
		switch (state) {
		case 1:
			if (isalpha(c) || c == 95) {
				*w++ = c;
				state = 2;
			}
			else if (isdigit(c)) {
				*w++ = c;
				state = 3;
			}
			else if (c == 60 || c == 62) {
				*w++ = c;
				state = 4;
			}
			else if (c == 61) {
				*w++ = c;
				state = 5;
			}
			else if (c == 33) {
				*w++ = c;
				state = 6;
			}
			else if (c == 42 || c == 43 || c == 45 || c == 47) {
				*w++ = c;
				*w = '\0';
				if (!strcmp(word, "+")) {
					return 13;
				}
				else if (!strcmp(word, "-")) {
					return 14;
				}
				else if (!strcmp(word, "*")) {
					return 15;
				}
				else {
					return 16;
				}
			}
			else if (c == 34) {//˫����
				if (!flag_2) {
					flag_2 = 1;
					return 37;
				}
				else {
					flag_2 = 0;
					return 37;
				}
			}
			else if (c == 39) {//������
				if (!flAg1) {
					flAg1 = 1;
					return 36;
				}
				else if(flAg1 == 2){
					flAg1 = 0;
					return 36;
				}
			}
			else if (c == 44 || c == 59) {
				*w++ = c;
				*w = '\0';
				if (!strcmp(word, ",")) {
					return 17;
				}
				else {
					return 18;
				}
			}
			else if (c == 40 || c == 41 || c == 91 || c == 93 || c == 123 || c == 125) {
				*w++ = c;
				*w = '\0';
				if (!strcmp(word, "(")) {
					return 19;
				}
				else if (!strcmp(word, ")")) {
					return 20;
				}
				else if (!strcmp(word, "[")) {
					return 21;
				}
				else if (!strcmp(word, "]")) {
					return 22;
				}
				else if (!strcmp(word, "{")) {
					return 23;
				}
				else {
					return 24;
				}
			}
			else if (c == ' '|| c == '\t') {
				state = 1;
			}
			else if (c == '\n') {
				state = 1;
				line++;
			}
			else {
				printf("�Ƿ��ַ�\n");
				return 0;
			}
			break;
		case 2:
			if (isalpha(c) || c == 95 || isdigit(c)) {
				*w++ = c;
				state = 2;
			}
			else {
				*w = '\0';
				ungetc(c, fp);
				if (tolow_com(word)==1) {
					return 1;
				}
				else if (tolow_com(word) == 2) {
					return 2;
				}
				else if (tolow_com(word) == 3) {
					return 3;
				}
				else if (tolow_com(word) == 4) {
					return 4;
				}
				else if (tolow_com(word) == 5) {
					return 5;
				}
				else if (tolow_com(word) == 6) {
					return 6;
				}
				else if (tolow_com(word) == 7) {
					return 7;
				}
				else if (tolow_com(word) == 8) {
					return 8;
				}
				else if (tolow_com(word) == 9) {
					return 9;
				}
				else if (tolow_com(word) == 10) {
					return 10;
				}
				else if (tolow_com(word) == 11) {
					return 11;
				}
				else if (tolow_com(word) == 12) {
					return 12;
				}
				else {
					return 32;
				}
			}
			break;
		case 3:
			if (isdigit(c)) {
				*w++ = c;
				state = 3;
			}
			else {
				*w = '\0';
				ungetc(c, fp);
				return 33;
			}
			break;
		case 4:
			if (c == 61) {
				*w++ = c;
				*w = '\0';
				if (!strcmp(word, "<=")) {
					return 27;
				}
				else{
					return 29;
				}
			}
			else {
				*w = '\0';
				ungetc(c, fp);
				if (!strcmp(word, "<")) {
					return 26;
				}
				else{
					return 28;
				}
			}
			break;
		case 5:
			if (c == 61) {
				*w++ = c;
				*w = '\0';
				return 31;
			}
			else {
				*w = '\0';
				ungetc(c, fp);
				return 25;
			}
			break;
		case 6:
			if (c == 61) {
				*w++ = c;
				*w = '\0';
				return 30;
			}
			else {
				printf("Line %d:�����ĸ�̾��\n",line);
				return 0;
			}
			break;
		case 7:
			if (c == '\"') {
				flag_2 = 0;
				return 37;
			}
			else {
				if (c == 32 || c == 33 || c >= 35 && c <= 126) {
					while (c == 32 || c == 33 || c >= 35 && c <= 126) {
						if (c == 92) {
							*w++ = c;
							*w++ = c;
							c = fgetc(fp);
						}
						else {
							*w++ = c;
							c = fgetc(fp);
						}
					}
					if (c == '\"') {
						flag_2 = 2;
						*w = '\0';
						ungetc(c, fp);
						return 34;
					}
					else if(c == '\n'){
						*w = '\0';
						flag_2 = 0;
						return -1;
					}
					else {
						err_mes(22);//�ַ����Ƿ�
						while (c != '\n' || c != '\"') {
							*w++ = c;
							c = fgetc(fp);
						}
						if (c == '\"') {
							*w = '\0';
							ungetc(c, fp);
							return -1;
						}
						else {
							*w = '\0';
							flag_2 = 0;
							return -1;
						}
					}
				}
				else {
					err_mes(22);//�ַ����Ƿ�
					while (c != '\n' || c != '\"') {
						*w++ = c;
						c = fgetc(fp);
					}
					if (c == '\"') {
						*w = '\0';
						ungetc(c, fp);
						return -1;
					}
					else {
						*w = '\0';
						flag_2 = 0;
						return -1;
					}
				}
			}
			break;
		case 8:
			if (c == 39) {
				ungetc(c, fp);
				err_mes(21);//���ַ�
				return -1;
			}
			else {
				if (c == 42 || c == 43 || c == 45 || c == 47 || c == 95 || isalpha(c) || isdigit(c)) {
					while (c == 42 || c == 43 || c == 45 || c == 47 || c == 95 || isalpha(c) || isdigit(c)) {
						*w++ = c;
						charnum++;
						c = fgetc(fp);
					}
					if (charnum ==1) {
						charnum = 0;
						if (c == '\'') {
							*w = '\0';
							flAg1 = 2;
							ungetc(c, fp);
							return 35;
						}
						else if (c == '\n') {
							flAg1 = 0;
							*w = '\0';
							return 35;
						}
						else {
							err_mes(8);//���ַ�
							*w++ = c;
							c = fgetc(fp);
							while (c != '\''&&c != '\n') {
								*w++ = c;
								c = fgetc(fp);
							}
							if (c == '\'') {
								*w = '\0';
								ungetc(c, fp);
								return -1;
							}
							else {
								flAg1 = 0;
								*w = '\0';
								return 0;
							}
						}
					}
					else {
						charnum = 0;
						err_mes(8);//���ַ�
						c = fgetc(fp);
						while (c != '\''&&c != '\n') {
							*w++ = c;
							c = fgetc(fp);
						}
						if (c == '\'') {
							*w = '\0';
							ungetc(c, fp);
							return -1;
						}
						else {
							flAg1 = 0;
							*w = '\0';
							return 0;
						}

					}
				}
				else {
					err_mes(7);//�Ƿ��ַ�
					*w++ = c;
					c = fgetc(fp);
					while (c != '\''&&c != '\n') {
						*w++ = c;
						c = fgetc(fp);
					}
					if (c == '\'') {
						*w = '\0';
						ungetc(c, fp);
						return -1;
					}
					else {
						flAg1 = 0;
						*w = '\0';
						return 0;
					}
				}
			}
			break;
		}
	}
	if (flAg1 == 1) {
		printf("Line %d:˫���Ų�ƥ��\n",line);
		return 0;
	}
	if (c == EOF) {
		return -1;
	}
}//�ʷ���������
void err_mes(int n) {
	switch (n) {
	case 1:
		printf("Line %d:Error--------Missing identifier!\n",line);//ȱʧ��ʶ��
		break;
	case 2:
		printf("Line %d:Error--------Missing semicolon!\n", line);//ȱʧ�ֺ�
		break;
	case 3:
		printf("Line %d:Error--------Missing type identifier!\n", line);//ȱʧ���ͱ�ʶ��
		break;
	case 4:
		printf("Line %d:Error--------Missing assignment symbol!\n", line);//ȱʧ��ֵ����
		break;
	case 5:
		printf("Line %d:Error--------Missing unsigned integer!\n", line);//ȱʧ�޷�������
		break;
	case 6:
		printf("Line %d:Error--------Missing apostrophe!\n", line);//ȱʧ������
		break;
	case 7:
		printf("Line %d:Error--------invalid symbol!\n", line);//�Ƿ��ַ�
		break;
	case 8:
		printf("Line %d:Error--------Multiple characters!\n", line);//���ַ�
		break;
	case 9:
		printf("Line %d:Error--------Missing array number!\n", line);//����ȱʧԪ�ظ���
		break;
	case 10:
		printf("Line %d:Error--------Array number is zero!\n", line);//����Ԫ�ظ���Ϊ0
		break;
	case 11:
		printf("Line %d:Error--------Missing ']'!\n", line);//ȱ���ҷ�����
		break;
	case 12:
		printf("Line %d:Error--------Missing ')'!\n", line);//ȱʧ��С����
		break;
	case 13:
		printf("Line %d:Error--------Missing '{'!\n", line);//ȱʧ�������
		break;
	case 14:
		printf("Line %d:Error--------Missing '('!\n", line);//ȱʧ��С����
		break;
	case 15:
		printf("Line %d:Error--------Missing '='!\n", line);//ȱʧ�Ⱥ�
		break;
	case 16:
		printf("Line %d:Error--------Missing addition operator!\n", line);//ȱʧ�ӷ������
		break;
	case 17:
		printf("Line %d:Error--------Missing string!\n", line);//���ַ���
		break;
	case 18:
		printf("Line %d:Error--------Double quotes do not match!\n", line);//˫���Ų�ƥ��
		break;
	case 19:
		printf("Line %d:Error--------Missing '}'!\n", line);//ȱʧ�Ҵ�����
		break;
	case 20:
		printf("Line %d:Error--------Missing main function!\n", line);//ȱʧ������
		break;
	case 21:
		printf("Line %d:Error--------Null character!\n", line);//���ַ�
		break;
	case 22:
		printf("Line %d:Error--------Illegal string!\n", line);//�ַ����Ƿ�
		break;
	case 23:
		printf("Line %d:Error--------Type mismatch!\n",line);//���Ͳ�ƥ��
		break;
	case 24:
		printf("Line %d:Error--------Parameter num mismatch!\n",line);//������Ŀ��ƥ��
		break;
	case 25:
		printf("Line %d:Error--------Undefined identifier!\n", line);//��ʶ��δ����
		break;
	case 26:
		printf("Line %d:Error--------Redefine identifier!\n", line);//�ض����ʶ��
		break;
	case 27:
		printf("Line %d:Error--------Undefined function!\n", line);//����δ����
		break;
	case 28:
		printf("Line %d:Error--------The factor is no return function!\n", line);//����Ϊ�޷���ֵ����
		break;
	case 29:
		printf("Line %d:Error--------The assign statement is illegal!\n", line);//��ֵ������Ϊ���Ǳ���
		break;
	case 30:
		printf("Line %d:Error--------The array is not existent!\n", line);//���鲻����
		break;
	case 31:
		printf("Line %d:Error--------The return value function lack return value!\n", line);//�з���ֵ����ȱ�ٷ���ֵ
		break;
	case 32:
		printf("Line %d:Error--------The no return value function has return value!\n", line);//�޷���ֵ�����з���ֵ
		break;
	}
}//������Ϣ
int tra_num(int a, char b[]) {
	int i;
	int result = 0;
	for (i = 0; word[i] != '\0'; i++) {
		result = result * 10 + word[i] - '0';
	}
	if (a == 0) {
		result = -result;
	}
	return result;
}
void enter_sym_tab(char sym_name[], int sym_type, int sym_num, int sym_addr, int level) {
	char t[130];
	int i;
	for (i = 0; sym_name[i] != '\0'; i++) {
		t[i] = tolower(sym_name[i]);
	}
	t[i] = '\0';
	strcpy(sym_tab[tab_num].name, t);
	sym_tab[tab_num].type = sym_type;
	sym_tab[tab_num].num = sym_num;
	sym_tab[tab_num].addr = sym_addr;
	if (sym_level == 0||level == 0) {
		sym_tab[tab_num].level = 0;
	}
	else {
		sym_tab[tab_num].level = 1;
	}
	tab_num++;
}
void enter_mid_tab(int mid_id, char mid_argu1[], char mid_argu2[], char mid_result[], int mid_value) {
	mid_tab[mid_num].id = mid_id;
	if (mid_argu1 == NULL) {
		strcpy(mid_tab[mid_num].argu1, "");
	}
	else {
		strcpy(mid_tab[mid_num].argu1, mid_argu1);
	}
	if (mid_argu2 == NULL) {
		strcpy(mid_tab[mid_num].argu2, "");
	}
	else {
		strcpy(mid_tab[mid_num].argu2, mid_argu2);
	}
	if (mid_result == NULL) {
		strcpy(mid_tab[mid_num].result, "");
	}
	else {
		strcpy(mid_tab[mid_num].result, mid_result);
	}
	mid_tab[mid_num].value = mid_value;
	mid_num++;
}
int look_tab(char sym_name[]) {
	int num,i,j;
	num = tab_num-1;
	for (i = num;i>=0; i--) {
		if (!strcmp(sym_tab[i].name, sym_name)) {
			return sym_tab[i].type;
		}
		if (sym_tab[i].type == re_func_int || sym_tab[i].type == re_func_char || sym_tab[i].type == no_func) {
			for (j = 0; j<tab_num; j++) {
				if (!strcmp(sym_tab[j].name, sym_name)&&sym_tab[j].level==0) {
					return sym_tab[j].type;
				}
			}
			return -1;
		}
	}
	return -1;
}
int define_look(char sym_name[],int level) {
	int num, i, j;
	num = tab_num - 1;
	if (level == 0) {
		for (i = num; i >= 0; i--) {
			if (!strcmp(sym_tab[i].name, sym_name) && sym_tab[i].level == 0) {
				return sym_tab[i].type;
			}
		}
		return -1;
	}
	else {
		for (i = num; i >= 0; i--) {
			if (!strcmp(sym_tab[i].name, sym_name)) {
				return sym_tab[i].type;
			}
			if (sym_tab[i].type == re_func_int || sym_tab[i].type ==  re_func_char || sym_tab[i].type == no_func||sym_tab[i].level==0) {
				return -1;
			}
		}
		return -1;
	}
}
int look_func(char sym_name[]) {
	int num, i;
	num = tab_num - 1;
	char t[130];
	for (i = 0; sym_name[i] != '\0'; i++) {
		t[i] = tolower(sym_name[i]);
	}
	t[i] = '\0';
	for (i = num; i >= 0; i--) {
		if (!strcmp(sym_tab[i].name, t)&&sym_tab[i].level==0) {
			return i;
		}
	}
	return -1;
}
void int_ass() {
	sym_type = con_int;
	mid_id = con_int;
	m = getword(fp, word);
	if (m == 32) {//ȷ�ϱ�ʶ��
		low_sym(word);
		if (define_look(lowsym,sym_level) != -1) {
			err_mes(26);//��ʶ���ض���
		}
		strcpy(sym_name, lowsym);
		strcpy(mid_argu1, lowsym);
		m = getword(fp, word);
		if (m == 25) {//ȷ�ϸ�ֵ����
			m = getword(fp, word);
			if (m == 13 || m == 14) {//ȷ��+��-
				if (m == 13) {
					temp_plus_mins = 1;
				}
				else {
					temp_plus_mins = 0;
				}
				m = getword(fp, word);
				if (m != 33) {
					err_mes(5);
				}
				else {
					sym_num = tra_num(temp_plus_mins, word);
					mid_value = sym_num;
					enter_sym_tab(sym_name, sym_type, sym_num, sym_addr, sym_level);
					enter_mid_tab(mid_id, mid_argu1, NULL, NULL, mid_value);
					sym_addr += 4;
				}
			}
			else if (m == 33) {
				sym_num = tra_num(1, word);
				mid_value = sym_num;
				enter_sym_tab(sym_name, sym_type, sym_num, sym_addr, sym_level);
				enter_mid_tab(mid_id, mid_argu1, NULL, NULL, mid_value);
				sym_addr += 4;
			}
			else {
				err_mes(5);
			}
		}
		else {
			err_mes(4);
		}
	}
	else {
		err_mes(1);
	}
}//������ֵ���
void char_ass() {
	sym_type = con_char;
	mid_id = con_char;
	m = getword(fp, word);
	if (m == 32) {//ȷ�ϱ�ʶ��
		low_sym(word);
		if (define_look(lowsym, sym_level) != -1) {
			err_mes(26);//��ʶ���ض���
		}
		strcpy(sym_name, lowsym);
		strcpy(mid_argu1, lowsym);
		m = getword(fp, word);
		if (m == 25) {//ȷ�ϸ�ֵ����
			m = getword(fp, word);
			if (m != 36) {
				err_mes(6);//ȱʧ������
			}
			else {
				m = getword(fp, word);
				sym_num = word[0];
				mid_value = sym_num;
				m = getword(fp, word);
				if (m != 36) {
					while (m != 36) {
						m = getword(fp, word);
					}
					err_mes(8);
				}
				else {
					enter_sym_tab(sym_name, sym_type, sym_num, sym_addr, sym_level);
					sym_addr += 4;
					enter_mid_tab(mid_id, mid_argu1, NULL, NULL, mid_value);
				}
			}
		}
		else {
			err_mes(4);
		}
	}
	else {
		err_mes(1);
	}
}//�ַ���ֵ���
void array_def() {
	if (int_char) {
		sym_type = char_array;
		mid_id = char_array;
	}
	else {
		sym_type = int_array;
		mid_id = int_array;
	}
	int num;
	int result = 0;
	m = getword(fp, word);
	if (m == 33) {
		for (num = 0; word[num] != '\0'; num++) {
			result = result * 10 + word[num] - '0';
		}
		if (result == 0) {
			err_mes(10);//����Ԫ�ظ���Ϊ0
		}
		else {
			sym_num = result;
			mid_value = result;
			m = getword(fp, word);
			if (m == 22) {
				if (define_look(sym_name, sym_level) != -1) {
					err_mes(26);//��ʶ���ض���
				}
				enter_sym_tab(sym_name, sym_type, sym_num, sym_addr, sym_level);
				sym_addr += 4;
				low_sym(sym_name);
				strcpy(mid_argu1, lowsym);
				enter_mid_tab(mid_id, mid_argu1, NULL, NULL, mid_value);
				m = getword(fp, word);
			}
			else {
				err_mes(11);
			}
		}
	}
	else {
		err_mes(9);
	}
}//���鶨��
void con_def(){
	printf("Line %d:This is constant description.\n",line);
	m = getword(fp, word);
	if (m == 2) {
		int_ass();
		m = getword(fp, word);
		while (m == 17) {
			int_ass();
			m = getword(fp, word);
		}
	}
	else if (m == 3) {
		char_ass();
		m = getword(fp, word);
		while (m == 17) {
			char_ass();
			m = getword(fp, word);
		}
	}
	else {
		err_mes(3);
	}
}//������˵���� ::=  const���������壾;{ const���������壾;}
void var_def() {
	printf("Line %d:This is variable description.\n",line);//���Ǳ���˵��
	if (m == 18) {
		if (int_char) {
			sym_type = var_char;
			mid_id = var_char;
		}
		else {
			sym_type = var_int;
			mid_id = var_int;
		}
		if (define_look(lowsym, sym_level) != -1) {
			err_mes(26);//��ʶ���ض���
		}
		enter_sym_tab(sym_name, sym_type, 0, sym_addr, sym_level);
		sym_addr += 4;
		low_sym(sym_name);
		enter_mid_tab(mid_id, lowsym, NULL, NULL, 0);
		m = getword(fp, word);
	}
	else if (m == 17) {
		if (int_char) {
			sym_type = var_char;
			mid_id = var_char;
		}
		else {
			sym_type = var_int;
			mid_id = var_int;
		}
		if (define_look(lowsym, sym_level) != -1) {
			err_mes(26);//��ʶ���ض���
		}
		enter_sym_tab(sym_name, sym_type, 0, sym_addr, sym_level);
		sym_addr += 4;
		low_sym(sym_name);
		enter_mid_tab(mid_id,lowsym, NULL, NULL, 0);
		while (m == 17) {
			m = getword(fp, word);
			if (m == 32) {
				low_sym(word);
				if (define_look(lowsym, sym_level) != -1) {
					err_mes(26);//��ʶ���ض���
				}
				strcpy(sym_name, lowsym);
				strcpy(mid_argu1, lowsym);
				m = getword(fp, word);
				if (m == 21) {
					array_def();
				}
				else {
					if (int_char) {
						sym_type = var_char;
						mid_id = var_char;
					}
					else {
						sym_type = var_int;
						mid_id = var_int;
					}
					enter_sym_tab(sym_name, sym_type, 0, sym_addr, sym_level);
					sym_addr += 4;
					enter_mid_tab(mid_id, mid_argu1, NULL, NULL, 0);
				}
			}
			else {
				err_mes(1);//ȱʧ��ʶ��
			}
		}
		if (m != 18) {
			err_mes(2);
		}
		m = getword(fp, word);
	}
	else {
		array_def();
		while (m == 17) {
			m = getword(fp, word);
			if (m == 32) {
				low_sym(word);
				if (define_look(lowsym, sym_level) != -1) {
					err_mes(26);//��ʶ���ض���
				}
				strcpy(sym_name, lowsym);
				m = getword(fp, word);
				if (m == 21) {
					array_def();
				}
				else {
					if (int_char) {
						sym_type = var_char;
					}
					else {
						sym_type = var_int;
					}
					enter_sym_tab(sym_name, sym_type, 0, sym_addr, sym_level);
					sym_addr += 4;
				}
			}
			else {
				err_mes(1);//ȱʧ��ʶ��
			}
		}
		if (m != 18) {
			err_mes(2);
		}
		m = getword(fp, word);
	}
}//������˵����  ::= ���������壾;{���������壾;}ֻ������������
void comp_state() {
	printf("Line %d:This is compound statement.\n",line);
	if (m == 1) {
		while (m == 1) {
			con_def();
			if (m != 18) {
				err_mes(2);
			}
			m = getword(fp, word);
		}
	}//����˵������
	if (m == 2 || m == 3) {
		while (m == 2 || m == 3) {
			if (m == 2) {
				int_char = 0;
			}
			else {
				int_char = 1;
			}
			m = getword(fp, word);
			if (m == 32) {
				low_sym(word);
				strcpy(sym_name, lowsym);
				m = getword(fp, word);
				if (m == 17 || m == 18 || m == 21) {
					var_def();
				}
				else {
					err_mes(2);
				}
			}
			else {
				err_mes(1);
			}
		}
	}//����˵������
	while (m == 5 || m == 7 || m == 8 || m == 23 || m == 32 || m == 9 || m == 10 || m == 11||m == 18) {
		statement();
	}//����в���
}//��������䣾   :: = �ۣ�����˵�����ݣۣ�����˵�����ݣ�����У�
void statement() {
	printf("Line %d:This is statement.\n",line);
	if (m == 5) {
		if_statement();
	}//��������䣾
	else if (m == 7) {
		while_state();
	}//��ѭ����䣾while
	else if (m == 8) {
		for_state();
	}//��ѭ����䣾for
	else if(m == 9){
		scanf_state();
		if (m == 18) {
			m = getword(fp, word);
		}
		else {
			err_mes(2);//ȱʧ�ֺ�
		}
	}//������䣾;
	else if (m == 10) {
		printf_state();
		if (m == 18) {
			m = getword(fp, word);
		}
		else {
			err_mes(2);//ȱʧ�ֺ�
		}
	}//��д��䣾;
	else if (m == 11) {
		return_state();
		if (m == 18) {
			m = getword(fp, word);
		}
		else {
			err_mes(2);//ȱʧ�ֺ�
		}
	}//��������䣾;
	else if (m == 23) {//�������
		m = getword(fp, word);
		while (m == 5 || m == 7 || m == 8 || m == 23 || m == 32 || m == 9 || m == 10 || m == 11||m == 18) {
			statement();
		}
		if (m == 24) {
			m = getword(fp, word);
		}
		else {
			err_mes(19);//ȱʧ�Ҵ�����
		}
	}//��{ ��������У��� }��
	else if (m == 32) {//��ʶ��

		char assign_temp[130];
		char assign_temp2[130];
		low_sym(word);
		if (look_tab(lowsym) == -1) {
			err_mes(25);
		}
		strcpy(sym_name, lowsym);
		strcpy(mid_argu1, lowsym);
		m = getword(fp, word);
		if (m == 19) {//��С����
			printf("Line %d:This is Function call statement.\n", line);
			ret_value = look_func(sym_name);
			strcpy(assign_temp, sym_tab[ret_value].name);
			m = getword(fp, word);
			value_para_tab();
			mid_id = CALL;
			strcpy(mid_result, assign_temp);
			enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
			ret_value = look_func(sym_name);
			if (sym_tab[ret_value].type == 9 || sym_tab[ret_value].type == 10) {
				mid_id = RET;
				t_creat();
				strcpy(mid_result, t_name);
				enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
			}
			if (m == 20) {//��С����
				m = getword(fp, word);
			}
			else {
				err_mes(12);//ȱʧ��С����
			}
			if (m == 18) {
				m = getword(fp, word);
			}
			else {
				err_mes(2);//ȱʧ�ֺ�
			}
		}//���з���ֵ����������䣾 :: = ����ʶ������(����ֵ��������)�� | <��ʶ��>
		else if (m == 25) {//�Ⱥ�
			if (look_tab(sym_name) == 3 || look_tab(sym_name) == 7) {
				left_type = 0;
			}
			else if (look_tab(sym_name) == 4 || look_tab(sym_name) == 8) {
				left_type = 1;
			}
			else {
				err_mes(29);//��ֵ�����಻�Ǳ���
			}
			printf("Line %d:This is assignment statement.\n", line);
			strcpy(assign_temp, mid_argu1);
			m = getword(fp, word);
			expression();
			if (left_type != right_type) {
				err_mes(23);
			}
			mid_id = assign;
			strcpy(mid_argu1, mid_tab[mid_num - 1].result);
			strcpy(mid_result, assign_temp);
			enter_mid_tab(mid_id, mid_argu1, NULL, mid_result, 0);
			if (m == 18) {
				m = getword(fp, word);
			}
			else {
				err_mes(2);//ȱʧ�ֺ�
			}
		}//����ֵ��䣾   ::=  ����ʶ�����������ʽ��
		else if (m == 21) {//������
			printf("Line %d:This is assignment statement.\n", line);
			strcpy(assign_temp, mid_argu1);
			if (look_tab(sym_name) == 5) {
				left_type = 0;
			}
			else if (look_tab(sym_name) == 6) {
				left_type = 1;
			}
			else {
				err_mes(29);//��ֵ�����಻�Ǳ���
			}
			m = getword(fp, word);
			expression();
			strcpy(assign_temp2, mid_tab[mid_num - 1].result);
			if (m == 22) {//�ҷ�����
				m = getword(fp, word);
				if (m == 25) {//�Ⱥ�
					m = getword(fp, word);
					expression();
					if (left_type != right_type) {
						err_mes(23);
					}
					strcpy(mid_argu2, mid_tab[mid_num - 1].result);
					mid_id = array_assign;
					strcpy(mid_result, assign_temp);
					strcpy(mid_argu1, assign_temp2);
					enter_mid_tab(mid_id, mid_argu1, mid_argu2, mid_result, 0);
				}
				else {
					err_mes(4);//ȱʧ��ֵ����
				}
			}
			else {
				err_mes(11);//ȱʧ�ҷ�����
			}
			if (m == 18) {
				m = getword(fp, word);
			}
			else {
				err_mes(2);//ȱʧ�ֺ�
			}
		}//����ֵ��䣾   ::=  ����ʶ������[�������ʽ����]��=�����ʽ��
		else {
			printf("Line %d:This is Function call statement.\n", line);
			ret_value = look_func(sym_name);
			mid_id = CALL;
			strcpy(mid_result, mid_argu1);
			enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
			if (sym_tab[ret_value].type == 9 || sym_tab[ret_value].type == 10) {
				mid_id = RET;
				t_creat();
				strcpy(mid_result, t_name);
				enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
			}
			if (m == 18) {
				m = getword(fp, word);
			}
			else {
				err_mes(2);//ȱʧ�ֺ�
			}
		}
	}// ���з���ֵ����������䣾;| ���޷���ֵ����������䣾; ������ֵ��䣾; 
	else if (m == 18) {
		m = getword(fp, word);
	}
	else {
		err_mes(2);//ȱʧ�ֺ�
	}
}//����䣾::= ��������䣾����ѭ����䣾|��{��������У���}��| ���з���ֵ����������䣾;| ���޷���ֵ����������䣾; ������ֵ��䣾; ��������䣾; ����д��䣾; �����գ�; | ��������䣾;
void while_state() {
	char while_first[130];
	char while_second[130];
	printf("Line %d:This is loop statement.\n", line);
	m = getword(fp, word);
	if (m == 19) {
		label_creat();
		strcpy(while_first, label_name);
		mid_id = label;
		strcpy(mid_result, while_first);
		enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
		m = getword(fp, word);
		condition();
		mid_id = BZ;
		label_creat();
		strcpy(while_second, label_name);
		strcpy(mid_result, while_second);
		enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
		if (m == 20) {
			m = getword(fp, word);
			statement();
			mid_id = GOTO;
			strcpy(mid_result, while_first);
			enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
			mid_id = label;
			strcpy(mid_result, while_second);
			enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
		}
		else {
			err_mes(12);
		}
	}
	else {
		err_mes(14);//ȱʧ��С����
	}
}//while ��(������������)������䣾
void for_state() {
	char iden_first[130];
	char iden_second[130];
	char iden_third[130];
	char for_first[130];
	char for_second[130];
	int plus_mins;
	int value_for;
	int i, j,for1, for2, for3, for4, for5, id_temp, value_temp;
	char argu1_temp[130], argu2_temp[130], result_temp[130];
	printf("Line %d:This is loop statement.\n", line);
	m = getword(fp, word);
	if (m == 19) {//��С����
		m = getword(fp, word);
		if (m == 32) {//��ʶ��
			low_sym(word);
			if (look_tab(lowsym) == -1) {
				err_mes(25);
			}
			strcpy(iden_first, lowsym);
			m = getword(fp, word);
			if (m == 25) {//�Ⱥ�
				m = getword(fp, word);
				expression();
				mid_id = assign;
				strcpy(mid_argu1, mid_tab[mid_num - 1].result);
				strcpy(mid_result, iden_first);
				enter_mid_tab(mid_id, mid_argu1, NULL, mid_result, 0);
				if (m == 18) {//�ֺ�
					m = getword(fp, word);
					mid_id = label;
					label_creat();
					strcpy(for_first, label_name);
					strcpy(mid_result, for_first);
					for1 = mid_num;
					enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
					condition();
					mid_id = BZ;
					label_creat();
					strcpy(for_second, label_name);
					strcpy(mid_result, for_second);
					for2 = mid_num;
					enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
					if (m == 18) {//�ֺ�
						m = getword(fp, word); 
						if (m == 32) {//��ʶ��
							low_sym(word);
							if (look_tab(lowsym) == -1) {
								err_mes(25);
							}
							strcpy(iden_second, lowsym);
							m = getword(fp, word);
							if (m == 25) {//�Ⱥ�
								m = getword(fp, word);
								if (m == 32) {//��ʶ��
									low_sym(word);
									if (look_tab(lowsym) == -1) {
										err_mes(25);
									}
									strcpy(iden_third, lowsym);
									m = getword(fp, word);
									if (m == 13 || m == 14) {//+��-
										if (m == 13) {
											plus_mins = 1;
										}
										else {
											plus_mins = 0;
										}
										m = getword(fp, word);
										if (m == 33) {//�޷�������
											value_for = tra_num(1, word);
											m = getword(fp, word);
											if (m == 20) {//��С����
												m = getword(fp, word);
												statement();
												mid_id = assign_int;
												t_creat();
												strcpy(mid_result, t_name);
												mid_value = value_for;
												enter_mid_tab(mid_id, NULL, NULL, mid_result, mid_value);
												if (plus_mins) {
													enter_mid_tab(plus, iden_third, mid_tab[mid_num - 1].result, iden_second, 0);
												}
												else {
													enter_mid_tab(mins, iden_third, mid_tab[mid_num - 1].result, iden_second, 0);
												}
												mid_id = GOTO;
												strcpy(mid_result, for_first);
												for3 = mid_num;
												enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
												mid_id = label;
												strcpy(mid_result, for_second);
												enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
											}
											else {
												err_mes(12);
											}
										}
										else {
											err_mes(5);//ȱʧ�޷�������
										}
									}
									else {
										err_mes(16);//ȱʧ�ӷ������
									}
								}
								else {
									err_mes(1);//ȱʧ��ʶ��
								}
							}
							else {
								err_mes(15);//ȱʧ�Ⱥ�
							}
						}
						else {
							err_mes(1);//ȱʧ��ʶ��
						}
					}
					else {
						err_mes(2);//ȱʧ�ֺ�
					}
				}
				else {
					err_mes(2);//ȱʧ�ֺ�
				}
			}
			else {
				err_mes(15);
			}
		}
		else {
			err_mes(1);
		}
	}
	else {
		err_mes(14);
	}
	for4 = for2 + 1;
	for5 = for3;
	for (i = for2; i > for1; i--) {
		id_temp = mid_tab[i].id;
		strcpy(argu1_temp, mid_tab[i].argu1);
		strcpy(argu2_temp, mid_tab[i].argu2);
		strcpy(result_temp, mid_tab[i].result);
		value_temp = mid_tab[i].value;
		for (j = for4; j < for5; j++) {
			mid_tab[j - 1].id = mid_tab[j].id;
			strcpy(mid_tab[j - 1].argu1, mid_tab[j].argu1);
			strcpy(mid_tab[j - 1].argu2, mid_tab[j].argu2);
			strcpy(mid_tab[j - 1].result, mid_tab[j].result);
			mid_tab[j - 1].value = mid_tab[j].value;
		}
		mid_tab[for5 - 1].id = id_temp;
		strcpy(mid_tab[for5 - 1].argu1, argu1_temp);
		strcpy(mid_tab[for5 - 1].argu2, argu2_temp);
		strcpy(mid_tab[for5 - 1].result, result_temp);
		mid_tab[for5 - 1].value = value_temp;
		for4--;
		for5--;
	}
}//for'('����ʶ�����������ʽ��;��������;����ʶ����������ʶ����(+|-)��������')'����䣾
void if_statement() {
	char if_first[130];
	char if_second[130];
	printf("Line %d:This is conditional statement.\n", line);
	m = getword(fp, word);
	if (m == 19) {//��С����
		m = getword(fp, word);
		condition();
		mid_id = BZ;
		label_creat();
		strcpy(if_first, label_name); 
		strcpy(mid_result, if_first);
		enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
		if (m == 20) {
			m = getword(fp, word);
			statement();
			mid_id = GOTO;
			label_creat();
			strcpy(if_second, label_name);
			strcpy(mid_result, if_second);
			enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
			mid_id = label;
			strcpy(mid_result, if_first);
			enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
			if (m == 6) {
				printf("Line %d:This is else statement.\n",line);
				m = getword(fp, word);
				statement();
			}
			mid_id = label;
			strcpy(mid_result, if_second);
			enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
		}
		else {
			err_mes(12);
		}
	}
	else {
		err_mes(14);//ȱ����С����
	}
}//��������䣾:: = if ��(������������)������䣾[else����䣾]
void scanf_state() {
	printf("Line %d:This is read statement.\n", line);
	m = getword(fp, word);
	if (m == 19) {//��С����
		m = getword(fp, word);
		if (m == 32) {//��ʶ��
			low_sym(word);
			if (look_tab(lowsym) == -1) {
				err_mes(25);
			}
			strcpy(mid_result, lowsym);
			mid_id = Scanf;
			enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
			m = getword(fp, word);
			if (m == 17) {//����
				while (m == 17) {
					m = getword(fp, word);
					if (m != 32) {
						err_mes(1);//ȱʧ��ʶ��
					}
					else {
						low_sym(word);
						if (look_tab(lowsym) == -1) {
							err_mes(25);
						}
						strcpy(mid_result, lowsym);
						mid_id = Scanf;
						enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
						m = getword(fp, word);
					}
				}
			}
			if (m != 20) {//��С����
				err_mes(12);
			}
			else {
				m = getword(fp, word);
			}
		}
		else {
			err_mes(1);//ȱʧ��ʶ��
		}
	}
	else {
		err_mes(14);//ȱʧ��С����
	}
}//������䣾    :: = scanf ��(������ʶ����{ ,����ʶ���� }��)��
void printf_state() {
	printf_flag = 1;
	printf("Line %d:This is write statement.\n", line);
	m = getword(fp, word);
	if (m == 19) {
		m = getword(fp, word);
		if (m == 37) {
			m = getword(fp, word);
			if (m == 34) {
				mid_id = Printf_s;
				str_creat();
				strcpy(mid_argu1, str_name);
				strcpy(mid_result, word);
				enter_mid_tab(mid_id, mid_argu1, NULL, mid_result, 0);
				m = getword(fp, word);
				if (m == 37) {//˫����
					m = getword(fp, word);
					if (m == 17) {//����
						m = getword(fp, word);
						expression();
						if (printf_flag == 2) {
							mid_id = Printf_char;
						}
						else {
							mid_id = Printf_int;
						}
						strcpy(mid_result, mid_tab[mid_num - 1].result);
						enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
					}
					if (m == 20) {
						m = getword(fp, word);
					}
					else {
						err_mes(12);//ȱʧ��С����
					}
				}
				else {
					err_mes(18);
				}
			}
			else if (m == 37) {
				m = getword(fp, word);
				if (m == 17) {//����
					m = getword(fp, word);
					expression();
					if (printf_flag == 2) {
						mid_id = Printf_char;
					}
					else {
						mid_id = Printf_int;
					}
					strcpy(mid_result, mid_tab[mid_num - 1].result);
					enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
				}
				if (m == 20) {
					m = getword(fp, word);
				}
				else {
					err_mes(12);//ȱʧ��С����
				}
			}
		}
		else {
			expression();
			if (printf_flag == 2) {
				mid_id = Printf_char;
			}
			else {
				mid_id = Printf_int;
			}
			strcpy(mid_result, mid_tab[mid_num - 1].result);
			enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
			if (m == 20) {
				m = getword(fp, word);
			}
			else {
				err_mes(12);//ȱʧ��С����
			}
		}
	}
	else {
		err_mes(14);
	}
	printf_flag = 0;
}//��д��䣾    ::= printf ��(�� ���ַ�����,�����ʽ�� ��)��| printf ��(�����ַ����� ��)��| printf ��(�������ʽ����)��
void return_state() {
	printf("Line %d:This is return statement.\n", line);
	m = getword(fp, word);
	if (m == 19) {
		return_num++;
		if (func_type == 0) {
			err_mes(32);//�޷���ֵ�����з���ֵ
		}
		m = getword(fp, word);
		expression();
		if (main_begin == 0) {
			mid_id = Return;
			strcpy(mid_result, mid_tab[mid_num - 1].result);
			enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
		}
		else {
			mid_id = Return_main;
			strcpy(mid_result, mid_tab[mid_num - 1].result);
			enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
		}
		if (m == 20) {
			m = getword(fp, word);
		}
		else {
			err_mes(12);//ȱʧ��С����
		}
	}
	else {
		if (func_type == 1 || func_type == 2) {
			err_mes(31);//�з���ֵ����ȱ�ٷ���ֵ
		}
		if (main_begin == 0) {
			enter_mid_tab(Return, NULL, NULL, NULL, 0);
		}
		else {
			enter_mid_tab(Return_main, NULL, NULL, NULL, 0);
		}
	}
}//��������䣾   ::=  return[��(�������ʽ����)��]
void condition() {
	int relate;
	char mid_temp3[40];
	printf("Line %d:This is condition.\n", line);
	expression();
	strcpy(mid_temp3, mid_tab[mid_num - 1].result);
	if (m == 26 || m == 27 || m == 28 || m == 29 || m == 30 || m == 31) {
		if (m == 26) relate = less;
		else if (m == 27) relate = less_equ;
		else if (m == 28) relate = greater;
		else if (m == 29) relate = greater_equ;
		else if (m == 30) relate = not_equ;
		else if (m == 31) relate = iden_equ;
		m = getword(fp, word);
		expression();
		mid_id = relate;
		strcpy(mid_argu1, mid_temp3);
		strcpy(mid_argu2, mid_tab[mid_num - 1].result);
		enter_mid_tab(mid_id, mid_argu1, mid_argu2, NULL, 0);
	}
	else {
		mid_id = not_equ_zero;
		strcpy(mid_result, mid_temp3);
		enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
	}
 }//��������    :: = �����ʽ������ϵ������������ʽ���������ʽ��
void expression() {
	right_type = 0;
	int plus_mins;
	char mid_temp1[130];//��ʱ���mid_argu1��mid_argu2��mid_temp
	printf("Line %d:This is expression.\n", line);
	if (m == 13 || m == 14) {
		if (printf_flag != 0) {
			printf_flag = 3;
		}
		if (m == 13) {
			plus_mins = 1;
		}
		else {
			plus_mins = 0;
		}
		m = getword(fp, word);
		item();
		right_type = 0;
		strcpy(mid_argu2, mid_tab[mid_num - 1].result);
		t_creat();
		strcpy(mid_result, t_name);
		if (plus_mins) {
			enter_mid_tab(plus, NULL, mid_argu2, mid_result, 0);
		}
		else {
			enter_mid_tab(mins, NULL, mid_argu2, mid_result, 0);
		}
		if (m == 13 || m == 14) {
			while (m == 13 || m == 14) {
				if (m == 13) {
					plus_mins = 1;
				}
				else {
					plus_mins = 0;
				}
				strcpy(mid_temp1, mid_tab[mid_num - 1].result);
				m = getword(fp, word);
				item();
				right_type = 0;
				strcpy(mid_argu1, mid_temp1);
				strcpy(mid_argu2, mid_tab[mid_num - 1].result);
				t_creat();
				strcpy(mid_result, t_name);
				if (plus_mins) {
					enter_mid_tab(plus, mid_argu1, mid_argu2, mid_result, 0);
				}
				else {
					enter_mid_tab(mins, mid_argu1, mid_argu2, mid_result, 0);
				}
			}
		}
	}
	else {
		item();
		strcpy(mid_temp1, mid_tab[mid_num - 1].result);
		if (m == 13 || m == 14) {
			if (printf_flag != 0) {
				printf_flag = 3;
			}
			while (m == 13 || m == 14) {
				if (m == 13) {
					plus_mins = 1;
				}
				else {
					plus_mins = 0;
				}
				m = getword(fp, word);
				item();
				right_type = 0;
				strcpy(mid_argu1, mid_temp1);
				strcpy(mid_argu2, mid_tab[mid_num - 1].result);
				t_creat();
				strcpy(mid_result, t_name);
				if (plus_mins) {
					enter_mid_tab(plus, mid_argu1, mid_argu2, mid_result, 0);
				}
				else {
					enter_mid_tab(mins, mid_argu1, mid_argu2, mid_result, 0);
				}
				strcpy(mid_temp1, mid_tab[mid_num - 1].result);
			}
		}
	}
}//�����ʽ��    :: = �ۣ������ݣ��{ ���ӷ����������� }
void item() {
	int mult_div;
	char mid_temp2[130];
	printf("Line %d:This is item.\n", line);
	factor();
	if (m == 15 || m == 16) {
		if (printf_flag != 0) {
			printf_flag = 3;
		}
		while (m == 15 || m == 16) {
			if (m == 15) {
				mult_div = 1;
			}
			else {
				mult_div = 0;
			}
			strcpy(mid_temp2, mid_tab[mid_num - 1].result);
			m = getword(fp, word);
			factor();
			right_type = 0;
			strcpy(mid_argu1, mid_temp2);
			strcpy(mid_argu2, mid_tab[mid_num - 1].result);
			t_creat();
			strcpy(mid_result, t_name);
			if (mult_div) {
				enter_mid_tab(mult, mid_argu1, mid_argu2, mid_result, 0);
			}
			else {
				enter_mid_tab(div, mid_argu1, mid_argu2, mid_result, 0);
			}
		}	
	}
}//���     :: = �����ӣ�{ ���˷�������������ӣ� }
void factor() {
	char factor_temp[130];
	printf("Line %d:This is factor.\n", line);
	if (m == 32) {
		low_sym(word);
		if (look_tab(lowsym) == -1) {
			err_mes(25);
		}
		strcpy(mid_argu1, lowsym);
		strcpy(factor_temp, lowsym);
		m = getword(fp, word);
		if (m == 21) {//'['
			m = getword(fp, word);
			expression();
			if (look_tab(factor_temp) == 5) {
				printf_flag = 3;
			}
			else {
				printf_flag = 2;
			}
			if (look_tab(factor_temp) == 5) {
				right_type = 0;
			}
			else if (look_tab(factor_temp) == 6){
				right_type = 1;
			}
			else {
				err_mes(30);//���鲻����
			}
			mid_id = assign_array;
			strcpy(mid_argu2, mid_tab[mid_num - 1].result);
			t_creat();
			strcpy(mid_result, t_name);
			strcpy(mid_argu1, factor_temp);
			enter_mid_tab(mid_id, mid_argu1, mid_argu2, mid_result,0);
			if (m == 22) {
				m = getword(fp, word);
			}
			else {
				err_mes(11);
			}
		}
		else if (m == 19) {//'('
			ret_value = look_func(mid_argu1);
			if (sym_tab[ret_value].type == 9) {
				right_type = 0;
			}
			else if (sym_tab[ret_value].type == 10) {
				right_type = 1;
			}
			strcpy(factor_temp, mid_argu1);
			m = getword(fp, word);
			value_para_tab();
			mid_id = CALL;
			strcpy(mid_result, factor_temp);
			enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
			ret_value = look_func(factor_temp);
			if (sym_tab[ret_value].type == 9 || sym_tab[ret_value].type == 10) {
				if (printf_flag != 0) {
					if (sym_tab[ret_value].type == 10) {
						if (printf_flag == 1) {
							printf_flag = 2;
						}
					}
					else {
						printf_flag = 3;
					}
				}
				mid_id = RET;
				t_creat();
				strcpy(mid_result, t_name);
				enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
			}
			if (m == 20) {
				m = getword(fp, word);
			}
			else {
				err_mes(12);//ȱ����С����
			}
		}
		else {
			if (look_tab(mid_argu1) == 9 || look_tab(mid_argu1) == 10 || look_tab(mid_argu1) == 11) {
				if (look_tab(mid_argu1) == 9 || look_tab(mid_argu1) == 10) {
					if (look_tab(mid_argu1) == 9) {
						right_type = 0;
					}
					else {
						right_type = 1;
					}
					if (printf_flag != 0) {
						if (look_tab(mid_argu1) == 10) {
							if (printf_flag == 1) {
								printf_flag = 2;
							}
						}
						else {
							printf_flag = 3;
						}
					}
					mid_id = CALL;
					enter_mid_tab(mid_id, NULL, NULL, mid_argu1, 0);
					mid_id = RET;
					t_creat();
					strcpy(mid_result, t_name);
					enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
				}
				else {
					err_mes(28);//����Ϊ�޷���ֵ����
				}
			}
			else {
				if (look_tab(mid_argu1) == 1 || look_tab(mid_argu1) == 3 || look_tab(mid_argu1) == 7) {
					right_type = 0;
				}
				else if(look_tab(mid_argu1) == 2 || look_tab(mid_argu1) == 4 || look_tab(mid_argu1) == 8){
					right_type = 1;
				}
				if (printf_flag == 1) {
					if (look_tab(mid_argu1) == 2 || look_tab(mid_argu1) == 4 || look_tab(mid_argu1) == 8) {
						printf_flag = 2;
					}
				}
				mid_id = SYM;
				enter_mid_tab(mid_id, NULL, NULL, mid_argu1, 0);
			}
		}
	}//����ʶ����������ʶ������[�������ʽ����]��|���з���ֵ����������䣾
	else if (m == 19) {//'('
		m = getword(fp, word);
		expression();
		if (printf_flag != 0) {
			printf_flag = 3;
		}
		right_type = 0;
		if (m == 20) {
			m = getword(fp, word);
		}
		else {
			err_mes(12);//ȱʧ��С����
		}
	}//��(�������ʽ����)��
	else if (m == 13 || m == 14) {
		right_type = 0;
		if (m == 13) {
			temp_plus_mins = 1;
		}
		else {
			temp_plus_mins = 0;
		}
		m = getword(fp, word);
		if (m != 33) {
			err_mes(5);//ȱʧ�޷�������
		}
		else {
			mid_value = tra_num(temp_plus_mins, word);
			mid_id = assign_int;
			t_creat();
			strcpy(mid_result, t_name);
			enter_mid_tab(mid_id, NULL, NULL,mid_result, mid_value);
			m = getword(fp, word);
		}
	}//�з�������
	else if (m == 33) {
		right_type = 0;
		temp_plus_mins = 1;
		mid_value = tra_num(temp_plus_mins, word);
		mid_id = assign_int;
		t_creat();
		strcpy(mid_result, t_name);
		enter_mid_tab(mid_id, NULL, NULL, mid_result, mid_value);
		m = getword(fp, word);
	}//�޷�������
	else if (m == 36) {//������
		right_type = 1;
		if (printf_flag == 1) {
			printf_flag = 2;
		}
		m = getword(fp, word);
		mid_id = assign_char;
		t_creat();
		strcpy(mid_result, t_name);
		mid_value = word[0];
		enter_mid_tab(mid_id, NULL, NULL, mid_result, mid_value);
		m = getword(fp, word);
		if (m != 36) {
			while (m != 36) {
				m = getword(fp, word);
			}
			err_mes(8);
		}
		m = getword(fp, word);//��ȡ�ַ���ĵ���
	}
	else {
		err_mes(2);
	}
}//�����ӣ�    :: = ����ʶ����������ʶ������[�������ʽ����]�� | ��(�������ʽ����)������������ | ���ַ��������з���ֵ����������䣾
void value_para_tab() {
	char push_stack[100][130];
	char push_num = 0;
	int para_temp;
	int i = 0;
	para_temp = ret_value;
	printf("Line %d:This is value parameter table.\n", line);
	para_type = sym_tab[++para_temp].type;
	if (para_type == 7) {
		left_type = 0;
	}
	else {
		left_type = 1;
	}
	expression();
	if (left_type != right_type) {
		err_mes(23);
	}
	strcpy(push_stack[push_num++], mid_tab[mid_num - 1].result);
	if (m == 17) {//����
		while (m == 17) {
			m = getword(fp, word);
			para_type = sym_tab[++para_temp].type;
			if (para_type == 7) {
				left_type = 0;
			}
			else {
				left_type = 1;
			}
			if (para_type == 7 || para_type == 8) {
			}
			else {
				err_mes(24);//������Ŀ��ƥ��
			}
			expression();
			if (left_type != right_type) {
				err_mes(23);
			}
			strcpy(push_stack[push_num++], mid_tab[mid_num - 1].result);
		}
	}
	for (i = 0; i < push_num; i++) {
		mid_id = PUSH;
		strcpy(mid_result, push_stack[i]);
		enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
	}
}//��ֵ������   ::= �����ʽ��{,�����ʽ��}
void para_tab() {
	printf("Line %d:This is parameter table.\n", line);
	if (m == 2||m == 3) {//int��char
		if (m == 2) {
			sym_type = para_int;
			mid_id = para_int;
		}
		else {
			sym_type = para_char;
			mid_id = para_char;
		}
		m = getword(fp, word);
		if (m == 32) {//��ʶ��
			low_sym(word);
			strcpy(sym_name, lowsym);
			strcpy(mid_argu1, lowsym);
			if (define_look(lowsym, sym_level) != -1) {
				err_mes(26);//��ʶ���ض���
			}
			enter_sym_tab(sym_name, sym_type, 0, sym_addr, sym_level);
			sym_addr += 4;
			enter_mid_tab(mid_id, mid_argu1, NULL, NULL, 0);
			m = getword(fp, word);
			if (m == 17) {//����
				while (m == 17) {//����
					m = getword(fp, word);
					if (m == 2 || m == 3) {//int��char
						if (m == 2) {
							sym_type = para_int;
							mid_id = para_int;
						}
						else {
							sym_type = para_char;
							mid_id = para_char;
						}
						m = getword(fp, word);
						if (m == 32) {//��ʶ��
							low_sym(word);
							strcpy(sym_name, lowsym);
							strcpy(mid_argu1, lowsym);
							if (define_look(lowsym, sym_level) != -1) {
								err_mes(26);//��ʶ���ض���
							}
							enter_sym_tab(sym_name, sym_type, 0, sym_addr, sym_level);
							sym_addr += 4;
							enter_mid_tab(mid_id, mid_argu1, NULL, NULL, 0);
							m = getword(fp, word);
						}
						else {
							err_mes(1);//ȱʧ��ʶ��
						}
					}
					else {
						err_mes(3);//ȱ�����ͱ�ʶ��
					}
				}
			}
		}
		else {
			err_mes(1);//ȱʧ��ʶ��
		}
	}
	else {
		err_mes(3);//ȱ�����ͱ�ʶ��
	}
}//��������    :: = �����ͱ�ʶ��������ʶ����{ ,�����ͱ�ʶ��������ʶ���� }
void return_value() {
	printf("Line %d:This is return value function.\n", line);
	strcpy(mid_result, mid_argu1);
	strcpy(sym_name, mid_result);
	if (define_look(lowsym, 0) != -1) {
		err_mes(26);//��ʶ���ض���
	}
	if (func_judge == 2) {
		mid_id = re_func_int;
		sym_type = re_func_int;
		enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
		enter_sym_tab(sym_name, sym_type, 0, sym_addr, 0);
		sym_addr += 4;
	}
	else {
		mid_id = re_func_char;
		sym_type = re_func_char;
		enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
		enter_sym_tab(sym_name, sym_type, 0, sym_addr,0);
		sym_addr += 4;
	}
	if (m == 19) {//��С����
		m = getword(fp, word);
		para_tab();//���ò�����
		if (m == 20) {
			m = getword(fp, word);
			if (m == 23) {//�������
				m = getword(fp, word);
				comp_state();
				if (return_num == 0) {
					err_mes(31);//�з���ֵ����ȱ�ٷ���ֵ
				}
				return_num = 0;
				if (m == 24) {//�Ҵ�����
					m = getword(fp, word);
				}
				else {
					err_mes(19);//ȱʧ�Ҵ�����
				}
			}
			else {
				err_mes(13);//ȱʧ�������
			}
		}
		else {
			err_mes(12);//ȱʧ��С����
		}
	}
	else if(m == 23){//�������
		m = getword(fp, word);
		comp_state();
		if (return_num == 0) {
			err_mes(31);//�з���ֵ����ȱ�ٷ���ֵ
		}
		return_num = 0;
		if (m == 24) {//�Ҵ�����
			m = getword(fp, word);
		}
		else {
			err_mes(19);//ȱʧ�Ҵ�����
		}
	}
	else {
		err_mes(13);
	}
}//���з���ֵ�������壾  ::=  ������ͷ������(������������)�� ��{����������䣾��}��|������ͷ������{����������䣾��}��
void  no_return_value() {
	printf("Line %d:This is no return value function.\n", line);
	strcpy(mid_result, mid_argu1);
	strcpy(sym_name, mid_result);
	mid_id = no_func;
	sym_type = no_func;
	if (define_look(lowsym, 0) != -1) {
		err_mes(26);//��ʶ���ض���
	}
	enter_sym_tab(sym_name, sym_type, 0, sym_addr, 0);
	sym_addr += 4;
	enter_mid_tab(mid_id, NULL, NULL, mid_result, 0);
	if (m == 19) {//��С����
		m = getword(fp, word);
		para_tab();//���ò�����
		if (m == 20) {//��С����
			m = getword(fp, word);
			if (m == 23) {//�������
				m = getword(fp, word);
				comp_state();
				return_num = 0;
				if (m == 24) {//�Ҵ�����
					mid_id = Return;
					enter_mid_tab(mid_id, NULL, NULL, NULL, 0);
					m = getword(fp, word);
				}
				else {
					err_mes(19);//ȱʧ�Ҵ�����
				}
			}
			else {
				err_mes(13);//ȱʧ�������
			}
		}
		else {
			err_mes(12);//ȱʧ��С����
		}
	}
	else if (m == 23) {//�������
		m = getword(fp, word);
		comp_state();
		return_num = 0;
		if (m == 24) {//�Ҵ�����
			mid_id = Return;
			enter_mid_tab(mid_id, NULL, NULL, NULL, 0);
			m = getword(fp, word);
		}
		else {
			err_mes(19);//ȱʧ�Ҵ�����
		}
	}
	else {
		err_mes(13);
	}
}//���޷���ֵ�������壾  ::= void����ʶ����(������������)����{����������䣾��}��| void����ʶ����{����������䣾��}��
void main_fun() {
	printf("This is main function.\n");
	if (m == 19) {//��С����
		m = getword(fp, word);
		if (m == 20) {//��С����
			m = getword(fp, word);
			if (m == 23) {//�������
				m = getword(fp, word);
				comp_state();
				return_num = 0;
				if (m == 24) {//�Ҵ�����
					m = getword(fp, word);
				}
				else {
					err_mes(19);//ȱʧ�Ҵ�����
				}
			}
			else {
				err_mes(13);//ȱʧ�������
			}
		}
		else {
			err_mes(12);//ȱʧ��С����
		}
	}
	else {
		err_mes(14);//ȱʧ��С����
	}
}//����������    ::= void main��(����)����{����������䣾��}��
int main()
{
	int i;
	char road[130];//�洢�ļ�·��������
	for (i = 0; i<130; i++) {
		road[i] = '\0';
	}
	for (i = 0; i<130; i++) {
		word[i] = '\0';
	}
	printf("�������ļ�·����\n");
	scanf("%s", road);
	fp = fopen(road, "r");
 	m = getword(fp, word);
	if (m == 1) {
		while (m == 1) {
			con_def();
			if (m != 18) {
				err_mes(2);
			}
			m = getword(fp, word);
		}
	}
	if (m == 2 || m == 3) {
		while (m == 2 || m == 3) {
			if (m == 2) {
				int_char = 0;
			}
			else {
				int_char = 1;
			}
			m = getword(fp, word);
			if (m == 32) {
				low_sym(word);
				strcpy(sym_name, lowsym);
				strcpy(mid_argu1, lowsym);
				m = getword(fp, word);
				if (m == 19||m == 23) {//������
					varfinish = 1;//����˵������
					sym_level = 1;
					break;
				}
				else if (m == 17 || m == 18 || m == 21) {
					var_def();
				}
				else {
					err_mes(2);
				}
			}
			else {
				err_mes(1);
			}
		}
	}
	if (varfinish) {
		if (int_char) {
			func_type = 2;
			sym_type = re_func_char;
			mid_id = re_func_char;
		}
		else {
			func_type = 1;
			sym_type = re_func_int;
			mid_id = re_func_int;
		}
		enter_mid_tab(mid_id, NULL, NULL, sym_name, 0);
		if (define_look(sym_name, 0) != -1) {
			err_mes(26);//��ʶ���ض���
		}
		enter_sym_tab(sym_name, sym_type, 0, sym_addr, 0);
		sym_addr += 4;
		printf("Line %d:This is return value function.\n",line);
		if (m == 19) {//��С����
			m = getword(fp, word);
			para_tab();//���ò�����
			if (m == 20) {
				m = getword(fp, word);
				if (m == 23) {//�������
					m = getword(fp, word);
					comp_state();
					if (return_num == 0) {
						err_mes(31);//�з���ֵ����ȱ�ٷ���ֵ
					}
					return_num = 0;
					if (m == 24) {//�Ҵ�����
						m = getword(fp, word);
					}
					else {
						err_mes(19);//ȱʧ�Ҵ�����
					}
				}
				else {
					err_mes(13);//ȱʧ�������
				}
			}
			else {
				err_mes(12);//ȱʧ��С����
			}
		}//������ͷ������(������������)�� �� { ����������䣾�� }��
		else {//�������
			m = getword(fp, word);
			comp_state();//�������
			if (m == 24) {//�Ҵ�����
				m = getword(fp, word);
			}
			else {
				err_mes(19);//ȱʧ�Ҵ�����
			}
		}//������ͷ������{����������䣾��}��
	}
	if (m == 2 || m == 3 || m == 4) {//int��char��void
		sym_level = 1;
		while (m == 2 || m == 3 || m == 4) {
			if (m == 2) {
				func_type = 1;
			}
			else if (m == 3) {
				func_type = 2;
			}
			else {
				func_type = 0;
			}
			func_judge = m;
			m = getword(fp, word);
			low_sym(word);
			strcpy(mid_argu1, lowsym);
			if (m == 12) {//main
				funcfinish = 1;
				break;
			}
			else if(m == 32){//��ʶ��
				m = getword(fp, word);
				if (func_judge == 2 || func_judge == 3) {
					return_value();
				}
				else {
					no_return_value();
				}
			}
			else {
				err_mes(1);//ȱʧ��ʶ��
			}
		}
	}
	if (funcfinish) {
		main_begin = 1;
		strcpy(sym_name, mid_argu1);
		mid_id = no_func;
		sym_type = no_func;
		low_sym(word);
		strcpy(mid_result, lowsym);
		if (define_look(lowsym, 0) != -1) {
			err_mes(26);//��ʶ���ض���
		}
		enter_sym_tab(sym_name, sym_type, 0, 0, 0);
		sym_addr += 4;
		enter_mid_tab(mid_id,NULL,NULL,mid_result,0);
		m = getword(fp, word);
		main_fun();
		if (m == -1) {
			printf("Line %d:The program is finished!\n",line);
		}
		else {
			printf("Line %d:The main function is finished,but the program isn't finished!\n", line);
		}
	}
	else {
		err_mes(20);//ȱʧ������
	}
	for (i = 0; i < tab_num; i++) {
		printf("%s %d %d %d %d\n", sym_tab[i].name, sym_tab[i].type, sym_tab[i].num, sym_tab[i].addr, sym_tab[i].level);
	}//������ű�
	fclose(fp);

	mid_code(mid_num);
	/*for (i = 0; i < mid_num; i++) {
		printf("id:%d    %s    %s    %s    %d\n ", mid_tab[i].id, mid_tab[i].argu1, mid_tab[i].argu2, mid_tab[i].result,mid_tab[i].value);
	}*///����м�����
	goal_code(mid_num, sym_num);
	system("pause");
	return 0;
}