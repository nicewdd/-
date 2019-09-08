#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "compiler.h"
void numtostr(int num);
int strtonum(char w[]);
int judge(char s[]) {
	if (s[0] == '-' || s[0] == '+' || s[0] == '\'' || s[0] >= 48 && s[0] <= 57) {
		return 1;
	}
	else {
		return 0;
	}
	
}
void update(int i) {
	int j;
	for (j = i; j < mid_num - 1; j++) {
		mid_tab[j].id = mid_tab[j + 1].id;
		strcpy(mid_tab[j].argu1, mid_tab[j+1].argu1);
		strcpy(mid_tab[j].argu2, mid_tab[j+1].argu2);
		strcpy(mid_tab[j].result, mid_tab[j+1].result);
		mid_tab[j].value = mid_tab[j + 1].value;
	}
	mid_tab[j].id = 0;
	mid_num--;
}
void optimize(){
	int i,j,k,result;
	char func_name[130];
	for (i = 0; i < mid_num; i++) {
		if (mid_tab[i].id == 9 || mid_tab[i].id == 10 || mid_tab[i].id == 11) {
			strcpy(func_name, mid_tab[i].result);
		}
		else if (mid_tab[i].id == assign_int) {
			numtostr(mid_tab[i].value);
			for (j = i + 1; j < mid_num; j++) {
				if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
					strcpy(mid_tab[j].argu1, tostr);
				}
				if (!strcmp(mid_tab[j].argu2, mid_tab[i].result)) {
					strcpy(mid_tab[j].argu2, tostr);
				}
				if (!strcmp(mid_tab[j].result, mid_tab[i].result)) {
					strcpy(mid_tab[j].result, tostr);
				}
			}
			update(i);
			i = i - 1;
		}
		else if (mid_tab[i].id == assign_char) {
			char c[130];
			c[0] = '\'';
			c [1]= mid_tab[i].value;
			c[2] = '\'';
			c[3] = '\0';
			for (j = i + 1; j < mid_num; j++) {
				if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
					strcpy(mid_tab[j].argu1, c);
				}
				if (!strcmp(mid_tab[j].argu2, mid_tab[i].result)) {
					strcpy(mid_tab[j].argu2, c);
				}
				if (!strcmp(mid_tab[j].result, mid_tab[i].result)) {
					strcpy(mid_tab[j].result, c);
				}
			}
			update(i);
			i = i - 1;
		}
		else if (mid_tab[i].id == assign) {
			if (judge(mid_tab[i].argu1)) {
				for (j = i + 1; j < mid_num && mid_tab[j].id != BZ && mid_tab[j].id!=BNZ && mid_tab[j].id!=CALL && strcmp(mid_tab[j].result,mid_tab[i].result) && mid_tab[j].id != 9 && mid_tab[j].id != 10 && mid_tab[j].id != 11 && mid_tab[j].id != 30; j++) {
					if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu1, mid_tab[i].argu1);
					}
					if (!strcmp(mid_tab[j].argu2, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu2, mid_tab[i].argu1);
					}
					if (!strcmp(mid_tab[j].result, mid_tab[i].result)) {
						strcpy(mid_tab[j].result, mid_tab[i].argu1);
					}
				}
			}
			else if (mid_tab[i].result[0] == '@') {
				for (j = i + 1; j < mid_num; j++) {
					if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu1, mid_tab[i].argu1);
					}
					if (!strcmp(mid_tab[j].argu2, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu2, mid_tab[i].argu1);
					}
					if (!strcmp(mid_tab[j].result, mid_tab[i].result)) {
						strcpy(mid_tab[j].result, mid_tab[i].argu1);
					}
				}
				update(i);
				i = i - 1;
			}
		}
		else if (mid_tab[i].id == assign_sym) {
			int sym_flag = 0;
			if (judge(mid_tab[i].argu1)) {
				for (j = i + 1; j < mid_num; j++) {
					if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu1, mid_tab[i].argu1);
					}
					if (!strcmp(mid_tab[j].argu2, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu2, mid_tab[i].argu1);
					}
					if (!strcmp(mid_tab[j].result, mid_tab[i].result)) {
						strcpy(mid_tab[j].result, mid_tab[i].argu1);
					}
				}
				update(i);
				i = i - 1;
			}
			else {
				for (j = i+1; j < mid_num; j++) {
					if (!strcmp(mid_tab[j].argu1, mid_tab[i].result) || !strcmp(mid_tab[j].argu2, mid_tab[i].result)|| !strcmp(mid_tab[j].result, mid_tab[i].result)) {
						if (mid_tab[j].id >= 20 && mid_tab[j].id <= 26 || mid_tab[j].id >= 12 && mid_tab[j].id <= 15 ) {
							if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
								if (mid_tab[j].argu2[0] != '@') {
									sym_flag = 1;
									strcpy(mid_tab[j].argu1, mid_tab[i].argu1);
								}
							}
							if (!strcmp(mid_tab[j].argu2, mid_tab[i].result)) {
								if (mid_tab[j].argu1[0] != '@') {
									sym_flag = 1;
									strcpy(mid_tab[j].argu2, mid_tab[i].argu1);
								}
							}
						}
						else if (mid_tab[j].id == 17 || mid_tab[j].id == 41) {
							if (!strcmp(mid_tab[j].result, mid_tab[i].result)) {
								sym_flag = 1;
								strcpy(mid_tab[j].result, mid_tab[i].argu1);
							}
						}
					}
				}
				if (sym_flag) {
					update(i);
					i = i - 1;
				}
			}
		}
		else if (mid_tab[i].id == RET && mid_tab[i].result[0]=='@') {
			int RET_flag = 0;
			for (j = i + 1; j < mid_num; j++) {
				if (mid_tab[j].id == assign && !strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
					RET_flag = 1;
					strcpy(mid_tab[j].argu1, "");
					mid_tab[j].id = RET;
				}
			}
			if (RET_flag) {
				update(i);
				i = i - 1;
			}
		}
		else if (mid_tab[i].id == label) {
			if (mid_tab[i + 1].id == label) {
				for (j = 0; j < mid_num; j++) {
					if (mid_tab[j].id == GOTO || mid_tab[j].id == BZ || mid_tab[j].id == BNZ) {
						if (!strcmp(mid_tab[j].result, mid_tab[i + 1].result)) {
							strcpy(mid_tab[j].result, mid_tab[i].result);
						}
					}
				}
				update(i + 1);
			}
		}
		else if (mid_tab[i].id == GOTO) {
			if (mid_tab[i + 1].id == label && !strcmp(mid_tab[i].result, mid_tab[i + 1].result)) {
				update(i);
				i = i - 1;
			}
		}
		else if (mid_tab[i].id == plus) {
			int plus_flag = 0;
			if (judge(mid_tab[i].argu1) && judge(mid_tab[i].argu2) || !strcmp("", mid_tab[i].argu1) && judge(mid_tab[i].argu2)) {
				result = strtonum(mid_tab[i].argu1) + strtonum(mid_tab[i].argu2);
				numtostr(result);
				for (j = i + 1; j < mid_num; j++) {
					if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu1, tostr);
					}
					if (!strcmp(mid_tab[j].argu2, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu2, tostr);
					}
					if (!strcmp(mid_tab[j].result, mid_tab[i].result)) {
						strcpy(mid_tab[j].result, tostr);
					}
				}
				update(i);
				i = i - 1;
			}
			else if(mid_tab[i].result[0]=='@'){
				for (j = i + 1; j < mid_num; j++) {
					if (mid_tab[j].id == assign) {
						if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
							plus_flag = 1;
							mid_tab[j].id = plus;
							strcpy(mid_tab[j].argu1, mid_tab[i].argu1);
							strcpy(mid_tab[j].argu2, mid_tab[i].argu2);
						}
					}
				}
				if (plus_flag) {
					update(i);
					i = i - 1;
				}
			}
		}
		else if (mid_tab[i].id == mins) {
			int mins_flag = 0;
			if (judge(mid_tab[i].argu1) && judge(mid_tab[i].argu2)||!strcmp("",mid_tab[i].argu1)&& judge(mid_tab[i].argu2)) {
				result = strtonum(mid_tab[i].argu1) - strtonum(mid_tab[i].argu2);
				numtostr(result);
				for (j = i + 1; j < mid_num; j++) {
					if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu1, tostr);
					}
					if (!strcmp(mid_tab[j].argu2, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu2, tostr);
					}
					if (!strcmp(mid_tab[j].result, mid_tab[i].result)) {
						strcpy(mid_tab[j].result, tostr);
					}
				}
				update(i);
				i = i - 1;
			}
			else if (mid_tab[i].result[0] == '@') {
				for (j = i + 1; j < mid_num; j++) {
					if (mid_tab[j].id == assign) {
						if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
							mins_flag = 1;
							mid_tab[j].id = mins;
							strcpy(mid_tab[j].argu1, mid_tab[i].argu1);
							strcpy(mid_tab[j].argu2, mid_tab[i].argu2);
						}
					}
				}
				if (mins_flag) {
					update(i);
					i = i - 1;
				}
			}
		}
		else if (mid_tab[i].id == mult) {
			int mult_flag = 0;
			if (judge(mid_tab[i].argu1) && judge(mid_tab[i].argu2)) {
				result = strtonum(mid_tab[i].argu1) * strtonum(mid_tab[i].argu2);
				numtostr(result);
				for (j = i + 1; j < mid_num; j++) {
					if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu1, tostr);
					}
					if (!strcmp(mid_tab[j].argu2, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu2, tostr);
					}
					if (!strcmp(mid_tab[j].result, mid_tab[i].result)) {
						strcpy(mid_tab[j].result, tostr);
					}
				}
				update(i);
				i = i - 1;
			}
			else if (mid_tab[i].result[0] == '@') {
				for (j = i + 1; j < mid_num; j++) {
					if (mid_tab[j].id == assign) {
						if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
							mult_flag = 1;
							mid_tab[j].id = mult;
							strcpy(mid_tab[j].argu1, mid_tab[i].argu1);
							strcpy(mid_tab[j].argu2, mid_tab[i].argu2);
						}
					}
				}
				if (mult_flag) {
					update(i);
					i = i - 1;
				}
			}
		}
		else if (mid_tab[i].id == div) {
			int div_flag = 0;
			if (judge(mid_tab[i].argu1) && judge(mid_tab[i].argu2)) {
				result = strtonum(mid_tab[i].argu1) / strtonum(mid_tab[i].argu2);
				numtostr(result);
				for (j = i + 1; j < mid_num; j++) {
					if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu1, tostr);
					}
					if (!strcmp(mid_tab[j].argu2, mid_tab[i].result)) {
						strcpy(mid_tab[j].argu2, tostr);
					}
					if (!strcmp(mid_tab[j].result, mid_tab[i].result)) {
						strcpy(mid_tab[j].result, tostr);
					}
				}
				update(i);
				i = i - 1;
			}
			else if (mid_tab[i].result[0] == '@') {
				for (j = i + 1; j < mid_num; j++) {
					if (mid_tab[j].id == assign) {
						if (!strcmp(mid_tab[j].argu1, mid_tab[i].result)) {
							div_flag = 1;
							mid_tab[j].id = div;
							strcpy(mid_tab[j].argu1, mid_tab[i].argu1);
							strcpy(mid_tab[j].argu2, mid_tab[i].argu2);
						}
					}
				}
				if (div_flag) {
					update(i);
					i = i - 1;
				}
			}
		}
	}
}