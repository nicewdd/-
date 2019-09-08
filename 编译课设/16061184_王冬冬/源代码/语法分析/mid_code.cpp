#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "compiler.h"
struct tab1 sym_tab[max_sym];
struct tab2 mid_tab[max_mid];
struct tab4 func_info[max_func];
struct tab3 var_info[max_var];
char tostr[130];
int basic[500];
void optimize();
int look_con(char func_name[], char sym_name[]) {
	int i,j;
	for (i = 0; i < tab_num; i++) {
		if (!strcmp(sym_tab[i].name, func_name)) {
			for (j = i + 1; sym_tab[j].type >= 1 && sym_tab[j].type <= 8 ; j++) {
				if (!strcmp(sym_tab[j].name, sym_name) && sym_tab[i].type != 5 && sym_tab[i].type != 6) {
					return 1;
				}
			}
		}
	}
	return -1;
}
void numtostr(int num) {
	char s[130];
	int i = 0;
	int j = 0;
	char c;
	if (num == 0) {
		s[0] = '0';
		s[1] = '\0';
		strcpy(tostr, s);
	}
	else if (num >= 0) {
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
		strcpy(tostr, s);
	}
	else {
		s[i++] = '-';
		num = -num;
		while (num) {
			s[i++] = num % 10 + '0';
			num = num / 10;
		}
		s[i] = '\0';
		for (j = 1; j <= i / 2; j++) {
			c = s[j];
			s[j] = s[i - j ];
			s[i - j ] = c;
		}
		strcpy(tostr, s);
	}
}
void mid_update(int mid_num) {
	int i;
	char temp1[130];
	char temp2[130];
	char func_temp[130];
	int level = 0;
	for (i = 0; i < mid_num; i++) {
		int j;
		if (mid_tab[i].id == 9 || mid_tab[i].id == 10 || mid_tab[i].id == 11) {
			level = 1;
		}
		if (mid_tab[i].id == 1) {
			strcpy(temp1, mid_tab[i].argu1);
			numtostr(mid_tab[i].value);
			strcpy(temp2, tostr);
			if (level) {
				for (j = i+1; mid_tab[j].id != 9 && mid_tab[j].id != 10 && mid_tab[j].id != 11&& j<mid_num; j++) {
					if (!strcmp(mid_tab[j].argu1, temp1)) {
						strcpy(mid_tab[j].argu1, temp2);
					}
					if (!strcmp(mid_tab[j].argu2, temp1)) {
						strcpy(mid_tab[j].argu2, temp2);
					}
					if (!strcmp(mid_tab[j].result, temp1)) {
						strcpy(mid_tab[j].result, temp2);
					}
				}
			}
		}
		else if (mid_tab[i].id == 2) {
			strcpy(temp1, mid_tab[i].argu1);
			temp2[0] = '\'';
			temp2[1] = mid_tab[i].value;
			temp2[2]= '\'';
			temp2[3] = '\0';
			if (level) {
				for (j = i+1; mid_tab[j].id != 9 && mid_tab[j].id != 10 && mid_tab[j].id != 11&& j<mid_num; j++) {
					if (!strcmp(mid_tab[j].argu1, temp1)) {
						strcpy(mid_tab[j].argu1, temp2);
					}
					if (!strcmp(mid_tab[j].argu2, temp1)) {
						strcpy(mid_tab[j].argu2, temp2);
					}
					if (!strcmp(mid_tab[j].result, temp1)) {
						strcpy(mid_tab[j].result, temp2);
					}
				}
			}
		}
	}//局部常量替换
	for (i = 0,level = 0; mid_tab[i].id == 1||mid_tab[i].id == 2; i++) {
		int j;
		if (mid_tab[i].id == 1) {
			strcpy(temp1, mid_tab[i].argu1);
			numtostr(mid_tab[i].value);
			strcpy(temp2, tostr);
			for (j = i + 1; j < mid_num; j++) {
				if (mid_tab[j].id == 9 || mid_tab[j].id == 10 || mid_tab[j].id == 11) {
					strcpy(func_temp, mid_tab[j].result);
					level = 1;
				}
				if (level) {
					if (mid_tab[j].id > 8) {
						if (!strcmp(mid_tab[j].argu1, temp1)) {
							if (look_con(func_temp, mid_tab[j].argu1) == -1) {
								strcpy(mid_tab[j].argu1, temp2);
							}
						}
						if (!strcmp(mid_tab[j].argu2, temp1)) {
							if (look_con(func_temp, mid_tab[j].argu2) == -1) {
								strcpy(mid_tab[j].argu2, temp2);
							}
						}
						if (!strcmp(mid_tab[j].result, temp1)) {
							if (look_con(func_temp, mid_tab[j].result) == -1) {
								strcpy(mid_tab[j].result, temp2);
							}
						}
					}
				}
			}
		}
		else {
			strcpy(temp1, mid_tab[i].argu1);
			temp2[0] = '\'';
			temp2[1] = mid_tab[i].value;
			temp2[2] = '\'';
			temp2[3] = '\0';
			for (j = i + 1; j < mid_num; j++) {
				if (mid_tab[j].id == 9 || mid_tab[j].id == 10 || mid_tab[j].id == 11) {
					strcpy(func_temp, mid_tab[j].result);
					level = 1;
				}
				if (level) {
					if (mid_tab[j].id > 8) {
						if (!strcmp(mid_tab[j].argu1, temp1)) {
							if (look_con(func_temp, mid_tab[j].argu1) == -1) {
								strcpy(mid_tab[j].argu1, temp2);
							}
						}
						if (!strcmp(mid_tab[j].argu2, temp1)) {
							if (look_con(func_temp, mid_tab[j].argu2) == -1) {
								strcpy(mid_tab[j].argu2, temp2);
							}
						}
						if (!strcmp(mid_tab[j].result, temp1)) {
							if (look_con(func_temp, mid_tab[j].result) == -1) {
								strcpy(mid_tab[j].result, temp2);
							}
						}
					}
				}
			}
		}
	}//全局常量替换
}
void basic_block() {
	int i;
	int BAS = 0;
	for (i = 0; i < 500; i++) {
		basic[i] = -1;
	}
	for (i = 0; i < mid_num; i++) {
		if (i == 0) {
			basic[BAS++] = 0;
		}
		else {

		}
	}
}
void mid_code(int mid_num) {
	FILE *mid;
	char isopt;
	mid = fopen("mid_code.txt", "w");
	mid_update(mid_num);
	printf("是否进行优化");
	scanf("%c", &isopt);
	if (isopt == 'Y') {
		optimize();
		optimize();
		optimize();
		optimize();
	}
	int i;
	for (i = 0; i < mid_num+1; i++) {
		switch (mid_tab[i].id) {
		case 1:
			fprintf(mid, "const int %s = %d\n", mid_tab[i].argu1, mid_tab[i].value);
			break;
		case 2:
			fprintf(mid, "const char %s = '%c'\n", mid_tab[i].argu1, mid_tab[i].value);
			break;
		case 3:
			fprintf(mid, "var int %s\n", mid_tab[i].argu1);
			break;
		case 4:
			fprintf(mid, "var char %s\n", mid_tab[i].argu1);
			break;
		case 5:
			fprintf(mid, "var int %s[%d]\n", mid_tab[i].argu1, mid_tab[i].value);
			break;
		case 6:
			fprintf(mid, "var char %s[%d]\n", mid_tab[i].argu1, mid_tab[i].value);
			break;
		case 7:
			fprintf(mid, "para int %s\n", mid_tab[i].argu1);
			break;
		case 8:
			fprintf(mid, "para char %s\n", mid_tab[i].argu1);
			break;
		case 9:
			fprintf(mid, "int %s()\n", mid_tab[i].result);
			break;
		case 10:
			fprintf(mid, "char %s()\n", mid_tab[i].result);
			break;
		case 11:
			fprintf(mid, "void %s()\n", mid_tab[i].result);
			break;
		case 12:
			fprintf(mid, "%s = %s + %s\n", mid_tab[i].result, mid_tab[i].argu1, mid_tab[i].argu2);
			break;
		case 13:
			fprintf(mid, "%s = %s - %s\n", mid_tab[i].result, mid_tab[i].argu1, mid_tab[i].argu2);
			break;
		case 14:
			fprintf(mid, "%s = %s * %s\n", mid_tab[i].result, mid_tab[i].argu1, mid_tab[i].argu2);
			break;
		case 15:
			fprintf(mid, "%s = %s / %s\n", mid_tab[i].result, mid_tab[i].argu1, mid_tab[i].argu2);
			break;
		case 16:
			fprintf(mid, "printf \"%s\"\n", mid_tab[i].result);
			break;
		case 17:
			fprintf(mid, "printf %s\n", mid_tab[i].result);
			break;
		case 18:
			fprintf(mid, "scanf %s\n", mid_tab[i].result);
			break;
		case 19:
			if (mid_tab[i].result == NULL) {
				fprintf(mid, "ret\n");
			}
			else {
				fprintf(mid, "ret %s\n", mid_tab[i].result);
			}
			break;
		case 20:
			fprintf(mid, "%s != %s\n", mid_tab[i].argu1, mid_tab[i].argu2);
			break;
		case 21:
			fprintf(mid, "%s == %s\n", mid_tab[i].argu1, mid_tab[i].argu2);
			break;
		case 22:
			fprintf(mid, "%s < %s\n", mid_tab[i].argu1, mid_tab[i].argu2);
			break;
		case 23:
			fprintf(mid, "%s <= %s\n", mid_tab[i].argu1, mid_tab[i].argu2);
			break;
		case 24:
			fprintf(mid, "%s > %s\n", mid_tab[i].argu1, mid_tab[i].argu2);
			break;
		case 25:
			fprintf(mid, "%s >= %s\n", mid_tab[i].argu1, mid_tab[i].argu2);
			break;
		case 26:
			fprintf(mid, "%s = %s\n", mid_tab[i].result, mid_tab[i].argu1);
			break;
		case 27:
			fprintf(mid, "%s = %d\n", mid_tab[i].result, mid_tab[i].value);
			break;
		case 28:
			fprintf(mid, "%s = '%c'\n", mid_tab[i].result, mid_tab[i].value);
			break;
		case 29:
			fprintf(mid, "%s = %s[%s]\n", mid_tab[i].result, mid_tab[i].argu1, mid_tab[i].argu2);
			break;
		case 30:
			fprintf(mid, "%s:\n", mid_tab[i].result);
			break;
		case 31:
			fprintf(mid, "GOTO %s\n", mid_tab[i].result);
			break;
		case 32:
			fprintf(mid, "push %s\n", mid_tab[i].result);
			break;
		case 33:
			fprintf(mid, "%s = RET\n", mid_tab[i].result);
			break;
		case 34:
			fprintf(mid, "call %s\n", mid_tab[i].result);
			break;
		case 35:
			fprintf(mid, "BZ %s\n", mid_tab[i].result);
			break;
		case 36:
			fprintf(mid, "BNZ %s\n", mid_tab[i].result);
			break;
		case 39:
			fprintf(mid, "%s != 0\n", mid_tab[i].result);
			break;
		case 40:
			fprintf(mid, "%s[%s] = %s\n", mid_tab[i].result, mid_tab[i].argu1, mid_tab[i].argu2);
			break;
		case 41:
			fprintf(mid, "printf %s\n", mid_tab[i].result);
			break;
		case 42:
			fprintf(mid, "ret\n");
			break;
		case 43:
			fprintf(mid, "%s = %s\n", mid_tab[i].result, mid_tab[i].argu1);
			break;
		}
	}
	fclose(mid);
}