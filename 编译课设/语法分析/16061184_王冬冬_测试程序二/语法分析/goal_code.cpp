#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "compiler.h"
int var_num = 0;
int func_num = 0;
int para_num = 0;
int var_addr = 0;
int var_level = 0;
int addr_var(char func_name[], char var_name[]) {
	int i, j;
	for (i = 0; i < func_num; i++) {
		if (!strcmp(func_info[i].name, func_name)) {
			for (j = 0; j < func_info[i].var_num; j++) {
				if (!strcmp(var_name, func_info[i].var_info[j].name)) {
					return func_info[i].var_info[j].addr;
				}
			}
		}
	}
}
int var_type(char func_name[],char var_name[]) {
	int i,j;
	for (i = 0; i < tab_num; i++) {
		if (!strcmp(func_name, sym_tab[i].name)) {
			for (j = i + 1; j < tab_num; j++) {
				if (!strcmp(var_name, sym_tab[j].name)) {
					return sym_tab[j].type;
				}
			}
		}
	}
	return -1;
}
int strtonum(char w[]) {
	int result = 0;
	int i;
	if (w[0] == '+' || w[0] == '-') {
		for (i = 1; w[i] != '\0'; i++) {
			result = result * 10 + w[i] - '0';
		}
		if (w[0] == '-') {
			result = -result;
			return result;
		}
	}
	else if (w[0] == '\''){
		return w[1];
	}
	else {
		for (i = 0; w[i] != '\0'; i++) {
			result = result * 10 + w[i] - '0';
		}
		return result;
	}
}
int var_pos1(char func_name[], char var_name[]) {
	int i, j;
	for (i = 0; i < func_num; i++) {
		if (!strcmp(func_info[i].name, func_name)) {
			for (j = 0; j < var_num ; j++) {
				if (!strcmp(var_name, func_info[i].var_info[j].name)) {
					return j;
				}
			}
		}
	}
	for (i = 0; mid_tab[i].id >=1&& mid_tab[i].id<=6; i++) {
		if (!strcmp(mid_tab[i].argu1, var_name) && mid_tab[i].id >= 3 && mid_tab[i].id <= 6) {
			return -2;
		}
	}
	return -1;
}//返回变量在该函数中的位置(建表时查)
int var_pos2(char func_name[], char var_name[]) {
	int i, j;
	for (i = 0; i < func_num; i++) {
		if (!strcmp(func_info[i].name, func_name)) {
			for (j = 0; j < func_info[i].var_num; j++) {
				if (!strcmp(var_name, func_info[i].var_info[j].name)) {
					return j;
				}
			}
		}
	}
	for (i = 0; mid_tab[i].id >= 1 && mid_tab[i].id <= 6; i++) {
		if (!strcmp(mid_tab[i].argu1, var_name)&&mid_tab[i].id >= 3&&mid_tab[i].id <= 6) {
			return -2;
		}
	}
	return -1;
}//返回变量在该函数中的位置(建完表时查)
int func_pos(char func_name[]) {
	int i;
	for (i = 0; i < func_num; i++) {
		if (!strcmp(func_info[i].name, func_name)) {
			return i;
		}
	}
	return -1;
}
void enter_func_tab(char func_name[]) {
	strcpy(func_info[func_num++].name, func_name);
}
int local_var_type(char func_name[], char var_name[]) {
	int i,j;
	for (i = 0; i < tab_num; i++) {
		if (!strcmp(func_name, sym_tab[i].name)) {
			for (j = i+1; sym_tab[j].type != 9 && sym_tab[j].type != 10 && sym_tab[j].type != 11&&j<tab_num; j++) {
				if (!strcmp(var_name, sym_tab[j].name)) {
					return sym_tab[j].type;
				}
			}
		}
	}
	return -1;
}
int over_con_type(char var_name[]) {
	int i;
	for (i = 0; sym_tab[i].level != 1; i++) {
		if (!strcmp(var_name, sym_tab[i].name)) {
			return sym_tab[i].type;
		}
	}
}
void enter_var_tab(char var_name[],int type, int addr) {
	strcpy(func_info[func_num - 1].var_info[var_num].name, var_name);
	func_info[func_num - 1].var_info[var_num].type = type;
	func_info[func_num - 1].var_info[var_num++].addr = addr;
}
void enter_tab(int mid_num) {
	int i = 0;
	int k;
	for (i = 0; i < mid_num; i++) {
		if (mid_tab[i].id == 9 || mid_tab[i].id == 10 || mid_tab[i].id == 11) {
			var_level = 1;
			if (func_num > 0) {
				func_info[func_num-1].para_num = para_num;
				func_info[func_num-1].var_num = var_num;
			}
			enter_func_tab(mid_tab[i].result);
			para_num = 0;
			var_addr = 0;
			var_num = 0;
		}
		else if (mid_tab[i].id >= 3 && mid_tab[i].id <= 8 && var_level == 1) {
			if (mid_tab[i].id == 3 || mid_tab[i].id == 4) {
				var_addr += 4;
				enter_var_tab(mid_tab[i].argu1, mid_tab[i].id, var_addr);
			}
			else if (mid_tab[i].id == 5 || mid_tab[i].id == 6) {
				func_info[func_num - 1].var_info[var_num].array_num = mid_tab[i].value;
				var_addr = var_addr + 4 * mid_tab[i].value;
				enter_var_tab(mid_tab[i].argu1, mid_tab[i].id,var_addr);

			}
			else {
				var_addr += 4;
				enter_var_tab(mid_tab[i].argu1, mid_tab[i].id, var_addr);
				para_num++;
			}
		}
		else {
			if (mid_tab[i].argu1[0] == '@'&&mid_tab[i].argu1[1] == 't') {
				k = var_pos1(func_info[func_num - 1].name, mid_tab[i].argu1);
				if (k == -1) {
					var_addr += 4;
					enter_var_tab(mid_tab[i].argu1, mid_tab[i].id, var_addr);
				}
			}
			if (mid_tab[i].argu2[0] == '@'&&mid_tab[i].argu2[1] == 't') {
				k = var_pos1(func_info[func_num - 1].name, mid_tab[i].argu2);
				if (k == -1) {
					var_addr += 4;
					enter_var_tab(mid_tab[i].argu2, mid_tab[i].id, var_addr);
				}
			}
			if (mid_tab[i].result[0] == '@'&&mid_tab[i].result[1]=='t') {
				k = var_pos1(func_info[func_num - 1].name, mid_tab[i].result);
				if (k == -1) {
					var_addr += 4;
					enter_var_tab(mid_tab[i].result, mid_tab[i].id, var_addr);
				}
			}
		}
	}
	func_info[func_num - 1].para_num = para_num;
	func_info[func_num - 1].var_num = var_num;
}
void creat_data(int mid_num, int sym_num, FILE *goal) {
	int i;
	for (i = 0; sym_tab[i].level == 0; i++) {
		if (sym_tab[i].type == 3 || sym_tab[i].type == 4) {
			fprintf(goal, "\t%s: .word 0\n", sym_tab[i].name);
		}
		else if (sym_tab[i].type == 5 || sym_tab[i].type == 6) {
			fprintf(goal, "\t%s: .space %d\n", sym_tab[i].name, 4*sym_tab[i].num);
		}
	}
	for (i = 0; i < mid_num; i++) {
		if (mid_tab[i].id == 16) {
			fprintf(goal, "\t%s: .asciiz \"%s\"\n", mid_tab[i].argu1, mid_tab[i].result);
		}
	}
}
void creat_text(int mid_num,FILE *goal) {
	int i,j,k,h,g,result;
	int s1 ;
	int s2 ;
	int s3 ;
	char curr_func[130];
	int push_num = 0;
	int sp_addr = 0;
	int array_addr = 0;
	int func_position = 0;
	fprintf(goal, "\tmove $fp, $sp\n");
	fprintf(goal, "\tj main\n");
	for (i = 0; i < mid_num; i++) {
		switch (mid_tab[i].id) {
		case 9:
			fprintf(goal, "%s:\n", mid_tab[i].result);
			strcpy(curr_func, mid_tab[i].result);
			j = func_pos(curr_func);
			if (func_info[j].para_num <= 8) {
				for (k = 0; k < func_info[j].para_num; k++) {
					fprintf(goal, "\tlw $s%d, -%d($fp)\n", k, addr_var(curr_func, func_info[j].var_info[k].name)-4);
				}
			}
			else {
				for (k = 0; k < 8; k++) {
					fprintf(goal, "\tlw $s%d, -%d($fp)\n", k, addr_var(curr_func, func_info[j].var_info[k].name) - 4);
				}
			}
			sp_addr = func_info[j].var_num;
			fprintf(goal, "\tsubi $sp, $sp, %d\n", func_info[j].var_info[sp_addr - 1].addr);
			break;
		case 10:
			fprintf(goal, "%s:\n", mid_tab[i].result);
			strcpy(curr_func, mid_tab[i].result);
			j = func_pos(curr_func);
			if (func_info[j].para_num <= 8) {
				for (k = 0; k < func_info[j].para_num; k++) {
					fprintf(goal, "\tlw $s%d, -%d($fp)\n", k, addr_var(curr_func, func_info[j].var_info[k].name) - 4);
				}
			}
			else {
				for (k = 0; k < 8; k++) {
					fprintf(goal, "\tlw $s%d, -%d($fp)\n", k, addr_var(curr_func, func_info[j].var_info[k].name) - 4);
				}
			}
			sp_addr = func_info[j].var_num;
			fprintf(goal, "\tsubi $sp, $sp, %d\n", func_info[j].var_info[sp_addr - 1].addr);
			break;
		case 11:
			fprintf(goal, "%s:\n", mid_tab[i].result);
			strcpy(curr_func, mid_tab[i].result);
			j = func_pos(curr_func);
			if (func_info[j].para_num <= 8) {
				for (k = 0; k < func_info[j].para_num; k++) {
					fprintf(goal, "\tlw $s%d, -%d($fp)\n", k, addr_var(curr_func, func_info[j].var_info[k].name) - 4);
				}
			}
			else {
				for (k = 0; k < 8; k++) {
					fprintf(goal, "\tlw $s%d, -%d($fp)\n", k, addr_var(curr_func, func_info[j].var_info[k].name) - 4);
				}
			}
			sp_addr = func_info[j].var_num;
			fprintf(goal, "\tsubi $sp, $sp, %d\n", func_info[j].var_info[sp_addr - 1].addr);
			break;
		case 12:
			s2 = 0;
			s3 = 0;
			k = 0;
			h = 0;
			g = 0;
			result = 0;
			k = var_pos2(curr_func, mid_tab[i].result);
			h = var_pos2(curr_func, mid_tab[i].argu1);
			g = var_pos2(curr_func, mid_tab[i].argu2);
			if (h >= 0) {
				if (h < 8) {
					s2 = 1;
				}
				else {
					fprintf(goal, "\tlw $t1,-%d($fp)\n", addr_var(curr_func, mid_tab[i].argu1) - 4);
				}
			}
			else {
				if (h == -1) {
					result = strtonum(mid_tab[i].argu1);
					fprintf(goal, "\tli $t1, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t1, %s\n", mid_tab[i].argu1);
				}
			}
			if (g >= 0) {
				if (g < 8) {
					s3 = 1;
				}
				else {
					fprintf(goal, "\tlw $t2,-%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
				}
			}
			else {
				if (g == -1) {
					result = strtonum(mid_tab[i].argu2);
					fprintf(goal, "\tli $t2, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t2, %s\n", mid_tab[i].argu2);
				}
			}
			if (k >= 8 || k == -2) {
				fprintf(goal, "\tadd $t0, ");
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
				else {
					fprintf(goal, "$s%d, ", h);
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
				if (k >= 8) {
					fprintf(goal, "\tsw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
				}
				else {
					fprintf(goal, "\tsw $t0, %s\n", mid_tab[i].result);
				}
			}
			else {
				fprintf(goal, "\tadd $s%d, ", k);
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
				else {
					fprintf(goal, "$s%d, ", h);
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
			}
			break;
		case 13:
			s2 = 0;
			s3 = 0;
			k = 0;
			h = 0;
			g = 0;
			result = 0;
			k = var_pos2(curr_func, mid_tab[i].result);
			h = var_pos2(curr_func, mid_tab[i].argu1);
			g = var_pos2(curr_func, mid_tab[i].argu2);
			if (h >= 0) {
				if (h < 8) {
					s2 = 1;
				}
				else {
					fprintf(goal, "\tlw $t1,-%d($fp)\n", addr_var(curr_func, mid_tab[i].argu1) - 4);
				}
			}
			else {
				if (h == -1) {
					result = strtonum(mid_tab[i].argu1);
					fprintf(goal, "\tli $t1, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t1, %s\n", mid_tab[i].argu1);
				}
			}
			if (g >= 0) {
				if (g < 8) {
					s3 = 1;
				}
				else {
					fprintf(goal, "\tlw $t2,-%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
				}
			}
			else {
				if (g == -1) {
					result = strtonum(mid_tab[i].argu2);
					fprintf(goal, "\tli $t2, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t2, %s\n", mid_tab[i].argu2);
				}
			}
			if (k >= 8 || k == -2) {
				fprintf(goal, "\tsub $t0, ");
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
				else {
					fprintf(goal, "$s%d, ", h);
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
				if (k >= 8) {
					fprintf(goal, "\tsw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
				}
				else {
					fprintf(goal, "\tsw $t0, %s\n", mid_tab[i].result);
				}
			}
			else {
				fprintf(goal, "\tsub $s%d, ", k);
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
				else {
					fprintf(goal, "$s%d, ", h);
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
			}
			break;
		case 14:
			s2 = 0;
			s3 = 0;
			k = 0;
			h = 0;
			g = 0;
			result = 0;
			k = var_pos2(curr_func, mid_tab[i].result);
			h = var_pos2(curr_func, mid_tab[i].argu1);
			g = var_pos2(curr_func, mid_tab[i].argu2);
			if (h >= 0) {
				if (h < 8) {
					s2 = 1;
				}
				else {
					fprintf(goal, "\tlw $t1,-%d($fp)\n", addr_var(curr_func, mid_tab[i].argu1) - 4);
				}
			}
			else {
				if (h == -1) {
					result = strtonum(mid_tab[i].argu1);
					fprintf(goal, "\tli $t1, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t1, %s\n", mid_tab[i].argu1);
				}
			}
			if (g >= 0) {
				if (g < 8) {
					s3 = 1;
				}
				else {
					fprintf(goal, "\tlw $t2,-%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
				}
			}
			else {
				if (g == -1) {
					result = strtonum(mid_tab[i].argu2);
					fprintf(goal, "\tli $t2, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t2, %s\n", mid_tab[i].argu2);
				}
			}
			if (k>=8 || k==-2) {
				fprintf(goal, "\tmul $t0, ");
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
				else {
					fprintf(goal, "$s%d, ", h);
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
				if (k >= 8) {
					fprintf(goal, "\tsw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
				}
				else {
					fprintf(goal, "\tsw $t0, %s\n", mid_tab[i].result);
				}
			}
			else {
				fprintf(goal, "\tmul $s%d, ", k);
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
				else {
					fprintf(goal, "$s%d, ", h);
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
			}
			break;
		case 15:
			s2 = 0;
			s3 = 0;
			k = 0;
			h = 0;
			g = 0;
			result = 0;
			k = var_pos2(curr_func, mid_tab[i].result);
			h = var_pos2(curr_func, mid_tab[i].argu1);
			g = var_pos2(curr_func, mid_tab[i].argu2);
			if (h >= 0) {
				if (h < 8) {
					s2 = 1;
				}
				else {
					fprintf(goal, "\tlw $t1,-%d($fp)\n", addr_var(curr_func, mid_tab[i].argu1) - 4);
				}
			}
			else {
				if (h == -1) {
					result = strtonum(mid_tab[i].argu1);
					fprintf(goal, "\tli $t1, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t1, %s\n", mid_tab[i].argu1);
				}
			}
			if (g >= 0) {
				if (g < 8) {
					s3 = 1;
				}
				else {
					fprintf(goal, "\tlw $t2,-%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
				}
			}
			else {
				if (g == -1) {
					result = strtonum(mid_tab[i].argu2);
					fprintf(goal, "\tli $t2, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t2, %s\n", mid_tab[i].argu2);
				}
			}
			if (k >= 8 || k == -2) {
				fprintf(goal, "\tdiv $t0, ");
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
				else {
					fprintf(goal, "$s%d, ", h);
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
				if (k >= 8) {
					fprintf(goal, "\tsw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
				}
				else {
					fprintf(goal, "\tsw $t0, %s\n", mid_tab[i].result);
				}
			}
			else {
				fprintf(goal, "\tdiv $s%d, ", k);
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
				else {
					fprintf(goal, "$s%d, ", h);
					if (s3 == 0) {
						fprintf(goal, "$t2\n");
					}
					else {
						fprintf(goal, "$s%d\n", g);
					}
				}
			}
			break;
		case 16:
			fprintf(goal, "\tla $a0, %s\n", mid_tab[i].argu1);
			fprintf(goal, "\tli $v0, 4\n");
			fprintf(goal, "\tsyscall\n");
			break;
		case 17:
			k = var_pos2(curr_func,mid_tab[i].result);
			if (k >= 0) {
				if (k < 8) {
					fprintf(goal, "\tmove $a0, $s%d\n", k);
				}
				else {
					fprintf(goal, "\tlw $a0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
				}
			}
			else {
				if (k == -1) {
					result = strtonum(mid_tab[i].result);
					fprintf(goal, "\tli $t0, %d\n", result);
					fprintf(goal, "\tmove $a0, $t0\n");
				}
				else {
					fprintf(goal, "\tlw $a0, %s\n", mid_tab[i].result);
				}
			}
			fprintf(goal, "\tli $v0, 1\n");
			fprintf(goal, "\tsyscall\n");
			break;
		case 18:
			k = var_pos2(curr_func, mid_tab[i].result);
			if (local_var_type(curr_func, mid_tab[i].result) != -1) {
				if (local_var_type(curr_func, mid_tab[i].result) == 4 || local_var_type(curr_func, mid_tab[i].result) == 8) {
					fprintf(goal, "\tli $v0, 12\n");
				}
				else {
					fprintf(goal, "\tli $v0, 5\n");
				}
			}
			else {
				if (over_con_type(mid_tab[i].result) == 4) {
					fprintf(goal, "\tli $v0, 12\n");
				}
				else {
					fprintf(goal, "\tli $v0, 5\n");
				}
			}
			fprintf(goal, "\tsyscall\n");
			if (k >= 0 && k < 8) {
				fprintf(goal, "\tmove $s%d, $v0\n", k);
			}
			else if (k >= 8) {
				fprintf(goal, "\tsw $v0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
			}
			
			else if(k == -2){
				fprintf(goal, "\tsw $v0, %s\n", mid_tab[i].result);
			}
			else {
				fprintf(goal, "\tsw $v0, %s\n", mid_tab[i].result);
			}
			break;
		case 19:
			if (!strcmp(mid_tab[i].result, "")) {
				fprintf(goal, "\tjr $ra\n");
			}
			else {
				k = var_pos2(curr_func, mid_tab[i].result);
				if (k >= 0 && k < 8) {
					fprintf(goal, "\tmove $v0, $s%d\n", k);
					fprintf(goal, "\tjr $ra\n");
				}
				else if (k >= 8) {
					fprintf(goal, "\tlw $v0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
					fprintf(goal, "\tjr $ra\n");
				}
				else if (k == -1) {
					fprintf(goal, "\tli $t0, %d\n", strtonum(mid_tab[i].result));
					fprintf(goal, "\tmove $v0, $t0\n");
					fprintf(goal, "\tjr $ra\n");
				}
				else {
					fprintf(goal, "\tlw $v0, %s\n", mid_tab[i].result);
					fprintf(goal, "\tjr $ra\n");
				}
			}
			break;
		case 20:
			s1 = 0;//左侧是否为幸运儿
			s2 = 0;//右侧是否为幸运儿
			k = var_pos2(curr_func, mid_tab[i].argu1);
			h = var_pos2(curr_func, mid_tab[i].argu2);
			if (k >= 0) {
				if (k < 8) {
					s1 = 1;
				}
				else {
					fprintf(goal, "\tlw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu1) - 4);
				}
			}
			else {
				if (k == -1) {
					result = strtonum(mid_tab[i].argu1);
					fprintf(goal, "\tli $t0, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t0, %s\n", mid_tab[i].argu1);
				}
			}
			if (h >= 0) {
				if (h < 8) {
					s2 = 1;
				}
				else {
					fprintf(goal, "\tlw $t1, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
				}
			}
			else {
				if (h == -1) {
					result = strtonum(mid_tab[i].argu2);
					fprintf(goal, "\tli $t1, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t1, %s\n", mid_tab[i].argu2);
				}
			}
			if (s1 == 0) {
				fprintf(goal, "\tbeq $t0, ");
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
				}
				else {
					fprintf(goal, "$s%d, ", h);
				}
			}
			else {
				fprintf(goal, "\tbeq $s%d, ", k);
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
				}
				else {
					fprintf(goal, "$s%d, ", h);
				}
			}
			i++;
			fprintf(goal, "%s\n", mid_tab[i].result);
			break;
		case 21:
			s1 = 0;//左侧是否为幸运儿
			s2 = 0;//右侧是否为幸运儿
			k = var_pos2(curr_func, mid_tab[i].argu1);
			h = var_pos2(curr_func, mid_tab[i].argu2);
			if (k >= 0) {
				if (k < 8) {
					s1 = 1;
				}
				else {
					fprintf(goal, "\tlw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu1) - 4);
				}
			}
			else {
				if (k == -1) {
					result = strtonum(mid_tab[i].argu1);
					fprintf(goal, "\tli $t0, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t0, %s\n", mid_tab[i].argu1);
				}
			}
			if (h >= 0) {
				if (h < 8) {
					s2 = 1;
				}
				else {
					fprintf(goal, "\tlw $t1, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
				}
			}
			else {
				if (h == -1) {
					result = strtonum(mid_tab[i].argu2);
					fprintf(goal, "\tli $t1, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t1, %s\n", mid_tab[i].argu2);
				}
			}
			if (s1 == 0) {
				fprintf(goal, "\tbne $t0, ");
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
				}
				else {
					fprintf(goal, "$s%d, ", h);
				}
			}
			else {
				fprintf(goal, "\tbne $s%d, ", k);
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
				}
				else {
					fprintf(goal, "$s%d, ", h);
				}
			}
			i++;
			fprintf(goal, "%s\n", mid_tab[i].result);
			break;
		case 22:
			s1 = 0;//左侧是否为幸运儿
			s2 = 0;//右侧是否为幸运儿
			k = var_pos2(curr_func, mid_tab[i].argu1);
			h = var_pos2(curr_func, mid_tab[i].argu2);
			if (k >= 0) {
				if (k < 8) {
					s1 = 1;
				}
				else {
					fprintf(goal, "\tlw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu1) - 4);
				}
			}
			else {
				if (k == -1) {
					result = strtonum(mid_tab[i].argu1);
					fprintf(goal, "\tli $t0, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t0, %s\n", mid_tab[i].argu1);
				}
			}
			if (h >= 0) {
				if (h < 8) {
					s2 = 1;
				}
				else {
					fprintf(goal, "\tlw $t1, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
				}
			}
			else {
				if (h == -1) {
					result = strtonum(mid_tab[i].argu2);
					fprintf(goal, "\tli $t1, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t1, %s\n", mid_tab[i].argu2);
				}
			}
			if (s1 == 0) {
				fprintf(goal, "\tbge $t0, ");
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
				}
				else {
					fprintf(goal, "$s%d, ", h);
				}
			}
			else {
				fprintf(goal, "\tbge $s%d, ", k);
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
				}
				else {
					fprintf(goal, "$s%d, ", h);
				}
			}
			i++;
			fprintf(goal, "%s\n", mid_tab[i].result);
			break;
		case 23:
			s1 = 0;//左侧是否为幸运儿
			s2 = 0;//右侧是否为幸运儿
			k = var_pos2(curr_func, mid_tab[i].argu1);
			h = var_pos2(curr_func, mid_tab[i].argu2);
			if (k >= 0) {
				if (k < 8) {
					s1 = 1;
				}
				else {
					fprintf(goal, "\tlw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu1) - 4);
				}
			}
			else {
				if (k == -1) {
					result = strtonum(mid_tab[i].argu1);
					fprintf(goal, "\tli $t0, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t0, %s\n", mid_tab[i].argu1);
				}
			}
			if (h >= 0) {
				if (h < 8) {
					s2 = 1;
				}
				else {
					fprintf(goal, "\tlw $t1, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
				}
			}
			else {
				if (h == -1) {
					result = strtonum(mid_tab[i].argu2);
					fprintf(goal, "\tli $t1, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t1, %s\n", mid_tab[i].argu2);
				}
			}
			if (s1 == 0) {
				fprintf(goal, "\tbgt $t0, ");
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
				}
				else {
					fprintf(goal, "$s%d, ", h);
				}
			}
			else {
				fprintf(goal, "\tbgt $s%d, ", k);
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
				}
				else {
					fprintf(goal, "$s%d, ", h);
				}
			}
			i++;
			fprintf(goal, "%s\n", mid_tab[i].result);
			break;
		case 24:
			s1 = 0;//左侧是否为幸运儿
			s2 = 0;//右侧是否为幸运儿
			k = var_pos2(curr_func, mid_tab[i].argu1);
			h = var_pos2(curr_func, mid_tab[i].argu2);
			if (k >= 0) {
				if (k < 8) {
					s1 = 1;
				}
				else {
					fprintf(goal, "\tlw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu1) - 4);
				}
			}
			else {
				if (k == -1) {
					result = strtonum(mid_tab[i].argu1);
					fprintf(goal, "\tli $t0, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t0, %s\n", mid_tab[i].argu1);
				}
			}
			if (h >= 0) {
				if (h < 8) {
					s2 = 1;
				}
				else {
					fprintf(goal, "\tlw $t1, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
				}
			}
			else {
				if (h == -1) {
					result = strtonum(mid_tab[i].argu2);
					fprintf(goal, "\tli $t1, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t1, %s\n", mid_tab[i].argu2);
				}
			}
			if (s1 == 0) {
				fprintf(goal, "\tble $t0, ");
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
				}
				else {
					fprintf(goal, "$s%d, ", h);
				}
			}
			else {
				fprintf(goal, "\tble $s%d, ", k);
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
				}
				else {
					fprintf(goal, "$s%d, ", h);
				}
			}
			i++;
			fprintf(goal, "%s\n", mid_tab[i].result);
			break;
		case 25:
			s1 = 0;//左侧是否为幸运儿
			s2 = 0;//右侧是否为幸运儿
			k = var_pos2(curr_func, mid_tab[i].argu1);
			h = var_pos2(curr_func, mid_tab[i].argu2);
			if (k >= 0) {
				if (k < 8) {
					s1 = 1;
				}
				else {
					fprintf(goal, "\tlw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu1) - 4);
				}
			}
			else {
				if (k == -1) {
					result = strtonum(mid_tab[i].argu1);
					fprintf(goal, "\tli $t0, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t0, %s\n", mid_tab[i].argu1);
				}
			}
			if (h >= 0) {
				if (h < 8) {
					s2 = 1;
				}
				else {
					fprintf(goal, "\tlw $t1, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
				}
			}
			else {
				if (h == -1) {
					result = strtonum(mid_tab[i].argu2);
					fprintf(goal, "\tli $t1, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t1, %s\n", mid_tab[i].argu2);
				}
			}
			if (s1 == 0) {
				fprintf(goal, "\tblt $t0, ");
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
				}
				else {
					fprintf(goal, "$s%d, ", h);
				}
			}
			else {
				fprintf(goal, "\tblt $s%d, ", k);
				if (s2 == 0) {
					fprintf(goal, "$t1, ");
				}
				else {
					fprintf(goal, "$s%d, ", h);
				}
			}
			i++;
			fprintf(goal, "%s\n", mid_tab[i].result);
			break;
		case 26:
			s1 = 0;//右侧是否为幸运儿
			result = 0;
			k = var_pos2(curr_func, mid_tab[i].result);
			h = var_pos2(curr_func, mid_tab[i].argu1);
			if (h >= 0) {
				if (h < 8) {
					s1 = 1;
				}
				else {
					fprintf(goal, "\tlw $t0,-%d($fp)\n", addr_var(curr_func, mid_tab[i].argu1) - 4);
				}
			}
			else {
				if (h == -1) {
					result = strtonum(mid_tab[i].argu1);
					fprintf(goal, "\tli $t0, %d\n", result);
				}
				else {
					fprintf(goal, "\tlw $t0, %s\n", mid_tab[i].argu1);
				}
			}
			if (k >= 0) {
				if (k < 8) {
					if (s1 == 1) {
						fprintf(goal, "\tmove $s%d, $s%d\n", k, h);
					}
					else {
						fprintf(goal, "\tmove $s%d, $t0\n", k);
					}
				}
				else {
					fprintf(goal, "\tsw $t0,-%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
				}
			}
			else {
				if (s1 == 1) {
					fprintf(goal, "\tsw $s%d, %s\n", h, mid_tab[i].result);
				}
				else {
					fprintf(goal, "\tsw $t0, %s\n", mid_tab[i].result);
				}
			}
			break;
		case 27:
			k = var_pos2(curr_func, mid_tab[i].result);
			if (k >= 0) {
				if (k < 8) {
					fprintf(goal, "\tli $s%d, %d\n", k, mid_tab[i].value);
				}
				else {
					fprintf(goal, "\tli $t0, %d\n", mid_tab[i].value);
					fprintf(goal, "\tsw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
				}
			}
			else {
				fprintf(goal, "\tli $t0, %d\n", mid_tab[i].value);
				fprintf(goal, "\tsw $t0, %s\n", mid_tab[i].result);
			}
			break;
		case 28:
			k = var_pos2(curr_func, mid_tab[i].result);
			if (k >= 0) {
				if (k < 8) {
					fprintf(goal, "\tli $s%d, %d\n", k, mid_tab[i].value);
				}
				else {
					fprintf(goal, "\tli $t0, %d\n", mid_tab[i].value);
					fprintf(goal, "\tsw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
				}
			}
			else {
				fprintf(goal, "\tli $t0, %d\n", mid_tab[i].value);
				fprintf(goal, "\tsw $t0, %s\n", mid_tab[i].result);
			}
			break;
		case 29:
			k = var_pos2(curr_func, mid_tab[i].argu2);//a[]中的下标
			h = var_pos2(curr_func, mid_tab[i].argu1);//a在变量信息表中的位置
			g = var_pos2(curr_func, mid_tab[i].result);//左侧临时变量在信息表中的位置
			array_addr = 0;
			result = 0;
			func_position = func_pos(curr_func);
			if (h >= 0) {
				if (h == 0) {
					array_addr = 0;
				}
				else {
					array_addr = func_info[func_position].var_info[h - 1].addr;
				}
				if (k >= 0 && k < 8) {
					fprintf(goal, "\tmul $t0, $s%d, 4\n", k);
					fprintf(goal, "\taddi $t1, $t0, %d\n", array_addr);
					fprintf(goal, "\tsub $t0, $fp, $t1\n");
					fprintf(goal, "\tlw $t1, ($t0)\n");
				}
				else if (k > 8) {
					fprintf(goal, "\tlw $t1, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
					fprintf(goal, "\tmul $t0, $t1, 4\n");
					fprintf(goal, "\taddi $t1, $t0, %d\n", array_addr);
					fprintf(goal, "\tsub $t0, $fp, $t1\n");
					fprintf(goal, "\tlw $t1, ($t0)\n");
				}
				else if (k == -1) {
					result = strtonum(mid_tab[i].argu2);
					fprintf(goal, "\tli $t1, %d\n", result);
					fprintf(goal, "\tmul $t0, $t1, 4\n", k);
					fprintf(goal, "\taddi $t1, $t0, %d\n", array_addr);
					fprintf(goal, "\tsub $t0, $fp, $t1\n");
					fprintf(goal, "\tlw $t1, ($t0)\n");
				}
				else {
					fprintf(goal, "\tlw $t1, %s\n", mid_tab[i].argu2);
					fprintf(goal, "\tmul $t0, $t1, 4\n", k);
					fprintf(goal, "\taddi $t1, $t0, %d\n", array_addr);
					fprintf(goal, "\tsub $t0, $fp, $t1\n");
					fprintf(goal, "\tlw $t1, ($t0)\n");
				}
			}
			else {
				if (k >= 0 && k < 8) {
					fprintf(goal, "\tmul $t0, $s%d, 4\n", k);
					fprintf(goal, "\tlw $t1, %s($t0)\n", mid_tab[i].argu1);
				}
				else if (k > 8) {
					fprintf(goal, "\tlw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
					fprintf(goal, "\tmul $t0, $t0, 4\n");
					fprintf(goal, "\tlw $t1, %s($t0)\n", mid_tab[i].argu1);
				}
				else if (k == -1) {
					result = strtonum(mid_tab[i].argu2);
					fprintf(goal, "\tli $t0, %d\n", result*4);
					fprintf(goal, "\tlw $t1, %s($t0)\n", mid_tab[i].argu1);
				}
				else {
					fprintf(goal, "\tlw $t0, %s\n", mid_tab[i].argu2);
					fprintf(goal, "\tmul $t0, $t0, 4\n");
					fprintf(goal, "\tlw $t1, %s($t0)\n", mid_tab[i].argu1);
				}
			}
			if (g >= 0) {
				if (g < 8) {
					fprintf(goal, "\tmove $s%d, $t1\n", g);
				}
				else {
					fprintf(goal, "\tsw $t1, -%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
				}
			}
			else {
				fprintf(goal, "\tsw $t1, %s\n", mid_tab[i].result);
			}
			break;
		case 30:
			fprintf(goal, "%s:\n", mid_tab[i].result);
			break;
		case 31:
			fprintf(goal, "\tj %s\n", mid_tab[i].result);
			break;
		case 32:
			if (push_num == 0) {
				fprintf(goal, "\tsw $fp, 0($sp)\n");
				fprintf(goal, "\tsw $ra, -4($sp)\n");
				fprintf(goal, "\tsubi $sp, $sp, 8\n");
			}
			k = var_pos2(curr_func, mid_tab[i].result);
			if (k >= 0 && k < 8) {
				fprintf(goal, "\tsw $s%d, -%d($sp)\n", k, push_num*4);
			}
			else if (k > 7) {
				fprintf(goal, "\tlw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
				fprintf(goal, "\tsw $t0, -%d($sp)\n", push_num * 4);
			}
			else if (k == -1) {
				fprintf(goal, "\tli $t0, %d\n", strtonum(mid_tab[i].result));
				fprintf(goal, "\tsw $t0, -%d($sp)\n", push_num * 4);
			}
			else {
				fprintf(goal, "\tlw $t0, %s\n", mid_tab[i].result);
				fprintf(goal, "\tsw $t0, -%d($sp)\n", push_num * 4);
			}
			push_num++;
			break;
		case 33:
			k = var_pos2(curr_func, mid_tab[i].result);
			h = func_pos(curr_func);
			if (k >= 0&& k < 8) {
				fprintf(goal, "\tmove $s%d, $v0\n", k);
			}
			else if(k >= 8){
				fprintf(goal, "\tsw $v0, -%d($fp)\n", addr_var(curr_func, func_info[h].var_info[k].name) - 4);
			}
			else if (k == -1) {
				fprintf(goal, "\tli $t0, %d\n", strtonum(mid_tab[i].result));
				fprintf(goal, "\tmove $t0, $v0\n");
			}
			else {
				fprintf(goal, "\tsw $v0, %s\n", mid_tab[i].result);
			}
			break;
		case 34:
			k = 0;
			h = 0;
			char temp_func[130];
			strcpy(temp_func, curr_func);
			if (push_num == 0) {
				fprintf(goal, "\tsw $fp, 0($sp)\n");
				fprintf(goal, "\tsw $ra, -4($sp)\n");
				fprintf(goal, "\tsubi $sp, $sp, 8\n");
			}
			h = func_pos(curr_func);
			if (func_info[h].var_num <= 8&& func_info[h].var_num >= 0) {
				for (k = 0; k < func_info[h].var_num ; k++) {
					if (var_type(curr_func, func_info[h].var_info[k].name) != 5 && var_type(curr_func, func_info[h].var_info[k].name) != 6) {
						fprintf(goal, "\tsw $s%d, -%d($fp)\n", k, addr_var(curr_func, func_info[h].var_info[k].name) - 4);
					}
				}
			}
			else {
				for (k = 0; k < 8; k++) {
					if (var_type(curr_func, func_info[h].var_info[k].name) != 5 && var_type(curr_func, func_info[h].var_info[k].name) != 6) {
						fprintf(goal, "\tsw $s%d, -%d($fp)\n", k, addr_var(curr_func, func_info[h].var_info[k].name) - 4);
					}
				}
			}
			push_num = 0;
			fprintf(goal, "\tmove $fp, $sp\n");
			fprintf(goal, "\tjal %s\n", mid_tab[i].result);
			fprintf(goal, "\tlw $ra, 4($fp)\n");
			fprintf(goal, "\tmove $sp, $fp\n");
			fprintf(goal, "\tlw $fp, 8($sp)\n");
			fprintf(goal, "\tadd $sp, $sp, 8\n");
			strcpy(curr_func, temp_func);
			h = func_pos(curr_func);
			if (func_info[h].var_num <= 8 && func_info[h].var_num >= 0) {
				for (k = 0; k < func_info[h].var_num; k++) {
					if (var_type(curr_func, func_info[h].var_info[k].name) != 5 && var_type(curr_func, func_info[h].var_info[k].name) != 6) {
						fprintf(goal, "\tlw $s%d, -%d($fp)\n", k, addr_var(curr_func, func_info[h].var_info[k].name) - 4);
					}	
				}
			}
			else {
				for (k = 0; k < 8; k++) {
					if (var_type(curr_func, func_info[h].var_info[k].name) != 5 && var_type(curr_func, func_info[h].var_info[k].name) != 6) {
						fprintf(goal, "\tlw $s%d, -%d($fp)\n", k, addr_var(curr_func, func_info[h].var_info[k].name) - 4);
					}
				}
			}
			break;
		case 39:
			k = var_pos2(curr_func, mid_tab[i].result);
			if (k >= 0) {
				if (k < 8) {
					fprintf(goal, "\tbeqz $s%d, ", k);
				}
				else {
					fprintf(goal, "lw $t0,-%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
					fprintf(goal, "\tbeqz $t0, ");
				}
				i++;
				fprintf(goal, "%s\n", mid_tab[i].result);
			}
			else {
				if (k == -2) {
					fprintf(goal, "\tlw $t0, %s\n", mid_tab[i].result);
					fprintf(goal, "\tbeqz $t0, ");
					i++;
					fprintf(goal, "%s\n", mid_tab[i].result);
				}
				else {
					result = strtonum(mid_tab[i].result);
					if (result != 0) {
						printf("error:Endless loop!\n");
					}
					else {
						i++;
						fprintf(goal, "\tj %s\n", mid_tab[i].result);
					}
				}
			}

			break;
		case 40:
			k = 0;
			h = 0;
			g = 0;
			func_position = 0;//函数的位置
			s1 = 0;//判断下标是否为幸运儿
			s2 = 0;//判断右侧是否为幸运儿
			result = 0;
			func_position = func_pos(curr_func);
			k = var_pos2(curr_func, mid_tab[i].result);//判断数组是全局变量还是局部变量
			h = var_pos2(curr_func, mid_tab[i].argu1);//a[]中下标的位置
			g = var_pos2(curr_func, mid_tab[i].argu2);//右侧临时变量在信息表中的位置
			if (h >= 0) {
				if (h < 8) {
					s1 = 1;
				}
				else {
					fprintf(goal, "\tlw $t0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu1) - 4);
				}
			}
			else if(h == -2){
				fprintf(goal, "\tlw $t0, %s\n", mid_tab[i].argu1);
			}
			else {
				result = strtonum(mid_tab[i].argu1);
				fprintf(goal, "\tli $t0, %d\n", result);
			}
			if (g >= 0) {
				if (g < 8) {
					s2 = 1;
				}
				else {
					fprintf(goal, "\tlw $t1, -%d($fp)\n", addr_var(curr_func, mid_tab[i].argu2) - 4);
				}
			}
			else if (g == -2) {
				fprintf(goal, "\tlw $t1, %s\n", mid_tab[i].argu2);
			}
			else {
				result = strtonum(mid_tab[i].argu2);
				fprintf(goal, "\tli $t1, %d\n", result);
			}
			if (k >= 0) {
				if (k == 0) {
					if (s1 == 1) {
						fprintf(goal, "\tmul $t4, $s%d, 4\n", h);
					}
					else {
						fprintf(goal, "\tmul $t4, $t0, 4\n");
					}
				}
				else {
					if (s1 == 1) {
						fprintf(goal, "\tmul $t3, $s%d, 4\n", h);
						fprintf(goal, "\tadd $t4, $t3, %d\n", func_info[func_position].var_info[k - 1].addr);
					}
					else {
						fprintf(goal, "\tmul $t3, $t0, 4\n");
						fprintf(goal, "\tadd $t4, $t3, %d\n", func_info[func_position].var_info[k - 1].addr);
					}
				}
				fprintf(goal, "\tsub $t5, $fp, $t4\n");
				if (s2 == 0) {
					fprintf(goal, "\tsw  $t1, 0($t5)\n");
				}
				else {
					fprintf(goal, "\tsw $s%d, 0($t5)\n", g);
				}
			}//数组为局部变量
			else {
				if (s1 == 1) {
					if (s2 == 0) {
						fprintf(goal, "\tmul $t0, $s%d, 4\n",h);
						fprintf(goal, "\tsw $t1, %s($t0)\n", mid_tab[i].result);
					}
					else {
						fprintf(goal, "\tmul $t0, $s%d, 4\n", h);
						fprintf(goal, "\tsw $s%d, %s($t0)\n", g, mid_tab[i].result);
					}
				}
				else {
					if (s2 == 0) {
						fprintf(goal, "\tmul $t0, $t0, 4\n");
						fprintf(goal, "\tsw $t1, %s($t0)\n", mid_tab[i].result);
					}
					else {
						fprintf(goal, "\tmul $t0, $t0, 4\n");
						fprintf(goal, "\tsw $s%d, %s($t0)\n", g, mid_tab[i].result);
					}
				}
			}//数组为全局变量
			break;
		case 41:
			k = var_pos2(curr_func, mid_tab[i].result);
			if (k >= 0) {
				if (k < 8) {
					fprintf(goal, "\tmove $a0, $s%d\n", k);
				}
				else {
					fprintf(goal, "\tlw $a0, -%d($fp)\n", addr_var(curr_func, mid_tab[i].result) - 4);
				}
			}
			else {
				if (k == -1) {
					result = strtonum(mid_tab[i].result);
					fprintf(goal, "\tli $t0, %d\n", result);
					fprintf(goal, "\tmove $a0, $t0\n");
				}
				else {
					fprintf(goal, "\tlw $a0, %s\n", mid_tab[i].result);
				}
			}
			fprintf(goal, "\tli $v0, 11\n");
			fprintf(goal, "\tsyscall\n");
			break;
		case 42:
			fprintf(goal, "\tli $v0, 10\n");
			fprintf(goal, "\tsyscall\n");
			break;
		}
	}
}
void goal_code(int mid_num, int sym_num) {
	FILE *goal;
	int i,j;
	goal = fopen("goal_code.asm", "w");
	enter_tab(mid_num);
	/*for (i = 0; i < func_num; i++) {
		printf("%s\n",func_info[i].name);
		printf("%d\n", func_info[i].para_num);
		printf("%d\n", func_info[i].var_num);
		for (j = 0; j < func_info[i].var_num; j++) {
			printf("%s    type:%d    addr:%d\n", func_info[i].var_info[j].name, func_info[i].var_info[j].type, func_info[i].var_info[j].addr);
		}
	}*/
	fprintf(goal, ".data\n");
	creat_data(mid_num, sym_num, goal);
	fprintf(goal, ".text\n");
	creat_text(mid_num, goal);
	fclose(goal);
}