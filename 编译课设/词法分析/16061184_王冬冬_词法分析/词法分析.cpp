#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
int getword(FILE *fp,char *w){
	int c;
	int state=1;
	int flag1=0;
	while((c=fgetc(fp))!=EOF){
		switch(state){
			case 1:
				if(isalpha(c)||c==95){
					w++=c;
					state=2;
				}
				else if(isdigit(c)){
					w++=c;
					state=3;
				}
				else if(c==60||c==62){
					*w++=c;
					state=4;
				}
				else if(c==61){
					*w++=c;
					state=5;
				}
				else if(c==33){
					*w++=c;
					state=6;
				}
				else if(c==42||c==43||c==45||c==47){
					*w++=c;
					*w='\0';
					return 1;
				}
				else if(c==34){
					flag1=1;
					state=7;
				}
				else if(c==39){
					state=8;
				}
				else if(c==44||c==59){
					*w++=c;
					*w='\0';
					return 2;
				}
				else if(c==40||c==41||c==91||c==93||c==123||c==125){
					*w++=c;
					*w='\0';
					return 3;
				}
				else if(c==' '||c=='\n'||c=='\t'){
					return 0;
				}
				else{
					printf("·Ç·¨×Ö·û\n"); 
					return 0; 
				}
			break;
			case 2:
				if(isalpha(c)||c==95||isdigit(c)){
					*w++=c;
					state=2;
				}
				else{
					*w='\0';
					ungetc(c,fp);
					return 4;
				}
			break;
			case 3:
				if(isdigit(c)){
					*w++=c;
					state=3;
				}
				else{
					*w='\0';
					ungetc(c,fp);
					return 5;
				}
			break;
			case 4:
				if(c==61){
					*w++=c;
					*w='\0';
					return 6;
				}
				else{
					*w='\0';
					ungetc(c,fp);
					return 6;
				}
			break;
			case 5:
				if(c==61){
					*w++=c;
					*w='\0';
					return 6;
				}
				else{
					*w='\0';
					ungetc(c,fp);
					return 7;
				}
			break;
			case 6:
				if(c==61){
					*w++=c;
					*w='\0';
					return 6;
				}
				else{
					printf("µ¥¶ÀµÄ¸ÐÌ¾ºÅ\n");
					return 0;
				}
			break;
			case 7:
				if(c=='\"'){
					flag1=0;
					*w='\0';
					return 8;
				}
				else if(c==32||c==33||c>=35&&c<=126){
					*w++=c;
					state=7;
				}
				else{
					printf("×Ö·û´®º¬ÓÐ·Ç·¨×Ö·û\n");
					state=9;
				}
			break;
			case 8:
				if(c=='\''){
					printf("¿Õ×Ö·û\n");
					return 0;
				}
				if(c==43||c==45||c==42||c==47||c==95||c>=97&&c<=122||c>=65&&c<=90||c>=48&&c<=57){
					*w++=c;
					c=fgetc(fp);
					if(c=='\''){
						*w='\0';
						return 9;
					}
					else{
						while(c!='\''){
							c=fgetc(fp);
						}
						printf("¶à×Ö·û\n");
						return 0; 
					}
				}
				else{
					printf("×Ö·ûÎª·Ç·¨×Ö·û\n");
					c=fgetc(fp);
					if(c=='\''){
						return 0;
					}
					else{
						while(c!='\''){
							c=fgetc(fp);
						}
						printf("¶à×Ö·û\n");
						return 0; 
					}
				}
			break;
			case 9:
				if(c=='\"'){
					flag1=0;
					return 0;
				}
				else{
					state=9;
				}
			break;
		}
	}
	if(flag1==1){
		printf("Ë«ÒýºÅ²»Æ¥Åä\n");
	}
	if(c==EOF){
		return c;
	}
}
void print(int n,char w[]){
	int result=0,num;
	char a[6]="const";
	char b[4]="int";
	char c[5]="char";
	char d[5]="void";
	char e[3]="if";
	char f[5]="else";
	char g[6]="while";
	char h[4]="for";
	char i[6]="scanf";
	char j[7]="printf";
	char k[7]="return";
	char l[5]="main"; 
	switch(n){
		case 1:
			printf("OPE %s\n",w);
		break;
		case 2:
			printf("PUN %s\n",w);
		break;
		case 3:
			printf("BRA %s\n",w);
		break;
		case 4:
			if(!strcmp(w,a)){
				printf("CON %s\n",w);
			}
			else if(!strcmp(w,b)){
				printf("INT %s\n",w);
			}
			else if(!strcmp(w,c)){
				printf("CHAR %s\n",w);
			}
			else if(!strcmp(w,d)){
				printf("VOID %s\n",w);
			}
			else if(!strcmp(w,e)){
				printf("IF %s\n",w);
			}
			else if(!strcmp(w,f)){
				printf("ELSE %s\n",w);
			}
			else if(!strcmp(w,g)){
				printf("WHILE %s\n",w);
			}
			else if(!strcmp(w,h)){
				printf("FOR %s\n",w);
			}
			else if(!strcmp(w,i)){
				printf("SCA %s\n",w);
			}
			else if(!strcmp(w,j)){
				printf("PRI %s\n",w);
			}
			else if(!strcmp(w,k)){
				printf("RET %s\n",w);
			}
			else if(!strcmp(w,l)){
				printf("MAIN %s\n",w);
			}
			else{
				printf("IDEN %s\n",w);
			}
		break;
		case 5:
			for(num=0;w[num]!='\0';num++){
				result=result*10+w[num]-'0';
			}
			printf("INTE %d\n",result);
		break;
		case 6:
			printf("COM %s\n",w);
		break;
		case 7:
			printf("EQU %s\n",w);
		break;
		case 8:
			printf("ALP %s\n",w);
		break;
		case 9:
			printf("CHARA %s\n",w);
	}
}
int main()
{
	char c;
	FILE *fp;
	int i,m;
	char word[40];
	for(i=0;i<40;i++){
		word[i]='\0';
	} 
	fp=fopen("16061184_test.txt","r");
	while((m=getword(fp,word))!=EOF){
		print(m,word);
	}
	fclose(fp);
	return 0;
} 
