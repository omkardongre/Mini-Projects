******************************   Implementation of  Lexical Analyzer for subset of C   ******************************


Compiler is responsible for converting high level language in machine language. 

There are several phases involved in this and lexical analysis is the first phase.

Lexical analyzer reads the characters from source code and convert it into tokens/lexemes 

Different tokens or lexemes are:

1.Keywords

2.Identifiers

3.Operators

4.Constants


==================================================================================================================

Input  -  File containg C code

Output - > Uniform Symbol Table (UST) 
         
         > Symbol/Identifier table
         
         > Literal table


==================================================================================================================




#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include <ctype.h>

typedef struct table{
	int num;
	char name[20];
}table;

typedef struct UST {
	
	int num;
	char name[20];
	char class[20];
} UST ;

table st[10];
table lt[10];
UST ust[10];

int is_keyword(char* buffer,char keyword[][10],int n);
int is_operator(char ch,char operator[][10],int n);
int symbol_exist(char* buffer, int nst);
void print(int nust,int nst,int nlt);

int main()
{
	char operator[][10]={"+","-","*","/",";",",","{","}","(",")","^","<",">","[","]","!","=","%","\""};
	char keyword[][10]={"int","char","float","double","main","for","while","do","return","if","else","printf"};
	
	FILE *fp;
	int i=0,k=0,flag=0,litflag=0,ist=0,ilt=0,iust=0;
	char file[10],ch,buffer[60];
	printf("\n Emter name of file : ");
	scanf("%s",file);
	fp=fopen(file,"r");
	int left=0,right=0;
	int nkeyword=sizeof(keyword)/10;
	int noperator=sizeof(operator)/10;
	while(!feof(fp))
	{
		ch=fgetc(fp);
		if(isalnum(ch))
			buffer[i++]=ch;

		if(!isalnum(ch) )
		{ 
			buffer[i]='\0';
			if(is_keyword(buffer,keyword,nkeyword))
			{      
				strcpy(ust[iust].name,buffer);
				strcpy(ust[iust].class,"key");
				ust[iust].num=iust+1;
				++iust;	
				strcpy(buffer,"\0\0\0\0");
				i=0;
			}
			else if(buffer[0]!='\0' && !(buffer[0]>47 && buffer[0]<58) )
			{
				if(symbol_exist(buffer,ist)==0)
				{
					st[ist].num=ist+1;
					strcpy(st[ist].name,buffer);
					++ist;
				}
				strcpy(buffer,"\0\0\0\0");
				i=0;	
			}
			else if(buffer[0]>47 && buffer[0]<58)	
			{
				lt[ilt].num=ilt+1;
				strcpy(lt[ilt].name,buffer);
				strcpy(buffer,"\0\0\0\0");
				++ilt;
				i=0;
			}

			if(is_operator(ch,operator,noperator))
			{
				ust[iust].name[0]=ch;
				strcpy(ust[iust].class,"op");
				ust[iust].num=iust+1;
				++iust;
			}	
		}
	}
	print(iust,ist,ilt);
	printf("\n");
	return 0;
}

int is_keyword(char* buffer,char keyword[][10],int n)
{			
	for(int i=0;i<n;i++)
	{
		if(strcmp(buffer,keyword[i])==0)
			return 1;
	}
	return 0;
}

void print(int nust,int nst,int nlt )
{
	printf("\n\tUST table\n");
	for(int i=0;i<nust;i++)
	{
		printf("\n%d\t",ust[i].num);
		printf("%s\t",ust[i].class);
		printf("%s\t",ust[i].name); 		
	}

	printf("\n\n\tSymbol table\n ");
	for(int i=0;i<nst;i++)
	{
		printf("\n%d\t",st[i].num);
		printf("%s\t",st[i].name);	
	}

	printf("\n\n\tLiteral table\n ");
	for(int i=0;i<nlt;i++)
	{
		printf("\n%d\t",lt[i].num);
		printf("%s\t",lt[i].name);		
	}
}
int is_operator(char ch,char operator[][10],int n)
{
	for(int i=0;i<n;i++)
	{
		if(ch==operator[i][0])
		{
			return 1;
		}	
	}
	return 0;
}
int symbol_exist(char* buffer, int nst)
{
	for(int i=0;i<nst;i++)
	{
		if(strcmp(st[i].name,buffer)==0)
			return 1;
	}
	return 0;
}

/*

********Output********


om@om-Vostro-3558:~/Documents/SP/Assignment5$ gcc assign5.c 
om@om-Vostro-3558:~/Documents/SP/Assignment5$ ./a.out

 Emter name of file : omkar

	UST table

1	key	int	
2	key	main	
3	op	(	
4	op	)	
5	op	{	
6	key	int	
7	op	=	
8	op	,	
9	op	=	
10	op	,	
11	op	=	
12	op	;	
13	op	=	
14	op	+	
15	op	;	
16	key	printf	
17	op	(	
18	op	"	
19	op	=	
20	op	%	
21	op	"	
22	op	,	
23	op	)	
24	op	;	
25	key	return	
26	op	;	
27	op	}	

	Symbol table
 
1	a	
2	b	
3	sum	
4	d	

	Literal table
 
1	10	
2	10	
3	0	
4	0	

*/
