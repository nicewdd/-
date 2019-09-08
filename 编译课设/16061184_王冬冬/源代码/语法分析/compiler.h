#pragma once
#define max_sym 1000
#define max_mid 1000
#define max_func 130
#define max_var 130
#define con_int 1
#define con_char 2
#define var_int 3
#define var_char 4
#define int_array 5
#define char_array 6
#define para_int 7
#define para_char 8
#define re_func_int 9
#define re_func_char 10
#define no_func 11
#define plus 12
#define mins 13
#define mult 14
#define div 15
#define Printf_s 16
#define Printf_int 17
#define Scanf 18
#define Return 19
#define not_equ 20
#define iden_equ 21
#define less 22
#define less_equ 23
#define greater 24
#define greater_equ 25
#define assign 26
#define assign_int 27
#define assign_char 28
#define assign_array 29
#define label 30
#define GOTO 31
#define PUSH 32
#define RET 33//函数调用后
#define CALL 34
#define BZ 35
#define BNZ 36
#define SYM 37
#define FUNC 38
#define not_equ_zero 39
#define array_assign 40
#define Printf_char 41
#define Return_main 42
#define assign_sym 43
struct tab1 {
	char name[130];
	int type;
	int num;
	int addr;
	int level;
};//符号表
struct tab2 {
	int id;
	char argu1[130];
	char argu2[130];
	char result[130];
	int value;
};//中间代码表
struct tab3 {
	char name[130];
	int type;
	int array_num;
	int front_addr = 0;
	int quote = 0;
	int addr;//相对位移
};//变量信息表
struct tab4 {
	char name[130];
	int para_num;
	int var_num;
	struct tab3 var_info[max_var];
	char para_name[100][130];
	int final_addr = 0;
};//函数信息表
extern struct tab1 sym_tab[max_sym];
extern struct tab2 mid_tab[max_mid];
extern struct tab3 var_info[max_func];
extern struct tab4 func_info[max_var];
extern char tostr[130];
extern int basic[500];
extern int var_num ;
extern int func_num;
extern int para_num ;
extern int var_addr ;
extern int var_level ;
extern int tab_num;
extern int mid_num;