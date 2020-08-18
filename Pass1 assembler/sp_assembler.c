#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char mot_arr[15][10]={"MOVEM","ADD","MOVER","SUB","MULT","PRINT","LOAD","STOP","DC","DS"};
char pot_arr[6][10]={"START","LTORG","ORIGIN","EQU","END"};
char reg[10][10]={"areg","breg","creg","dreg"};
char data[40][50];
int lcount=0;
int curptr=0,line=0;
char str[10];
int lc,ic=0;
//symbol and literal table index
int s_count=0,l_count=0;
int l_index=0;

//pot table 
struct pot
{
	
	char mneum[10];
	char class[5];
	int opcode;

}pobj[10];

//mot table 
struct mot
{
	
	char mneum[10];
	char class[5];
	int opcode;

}mobj[10];




//symbol table 
struct symbol_table
{
	int serial;
	char symbol[10];
	int addr;
}sobj[10];

// literal table
struct literal_table
{
	int serial;
	char literal[10];
	int addr;
}lobj[10];

//intermediate code
struct ic_table
{
	int lc;
	char class[2];
	int opcode;
	int reg;
	char tab;
	int no;
}ic_code[30];

void read_data();
void setcode();
void read();
void first_pass();
void getstring();
void opcode();
int search_mot();
void display();
void serach_reg();
void search_symbol();
void search_literal();
void ls_display();
void assign_symbol();
int addr_of_symbol();
int main()
{

	setcode();
	read();
	first_pass();
	return 0;
}

void setcode()
{
	FILE *f;
	f=fopen("input","r");
	int i=0,j=0;
	// for IS
	for(i=0;i<8;i++)
	{
		strcpy(mobj[i].mneum,mot_arr[i]);
		strcpy(mobj[i].class,"IS");
		mobj[i].opcode=i+1;
		
	}
	//for DL
	for(;i<10;i++)
	{
		
		strcpy(mobj[i].mneum,mot_arr[i]);
		strcpy(mobj[i].class,"DL");
		mobj[i].opcode=i+1;
	}
	//for AD
	for(i=0;i<5;i++)
	{
		strcpy(pobj[i].mneum,pot_arr[i]);
		strcpy(pobj[i].class,"AD");
		pobj[i].opcode=i+1;
	}
	//for pot
	for(i=0;i<5;i++)
	{
		printf("%s\t",pobj[i].mneum);
		printf("%s\t",pobj[i].class);
		printf("%d\n",pobj[i].opcode);

	}
	//for mot
	for(i=0;i<10;i++)
	{
		printf("%s\t",mobj[i].mneum);
		printf("%s\t",mobj[i].class);
		printf("%d\n",mobj[i].opcode);

	}
}

void read()
{
	char buf[50];
	FILE *fp;
	int len=0;
	fp=fopen("input1.txt","r");
	while(fgets(buf,50,fp))
	{
		strcpy(data[lcount],buf);
		lcount++;
	}
	len=strlen(data[lcount-1]);
	data[lcount-1][len+1]='\0';
	fclose(fp);
}

void first_pass()
{
	line=0;
	while(line<lcount)
	{
		if(line==15)
			break;

		curptr=0;
		getstring();	
		opcode();
		line++;
	}
	display();
	ls_display();

}
void getstring()
{
	
	int j=0;
	while(data[line][curptr]!='\0')
	{
		if(data[line][curptr]==' '  || data[line][curptr]== ',' || data[line][curptr]=='\t' )
		{	
			curptr++;
		}
		else
		{
			j=0;
			
			while(data[line][curptr]!=' '  && data[line][curptr]!= ',' && data[line][curptr]!='\t' && data[line][curptr]!='\0' && data[line][curptr]!='\n')
			{
				str[j]=data[line][curptr];
				
				j++;
				curptr++;
			}
			str[j]='\0'; 
			
			break;
		}
	}	
}

void opcode()
{
	int flag=0,i=0,jump=0,addr=0;
	if (strcmp(str,"start")==0)
	{
		strcpy(ic_code[line].class,"AD");
		ic_code[line].opcode= 1;
		ic_code[line].tab='C';
		getstring();
		lc=atoi(str);
		
		ic_code[line].no= atoi(str);		
	}
	else if (strcmp(str,"ltorg")==0)
	{
		strcpy(ic_code[line].class,"AD");
		ic_code[line].opcode=2;
		for(i=0;i<l_count;i++)
		{
			lobj[i].addr=lc;
			lc++;
		}
		l_index=l_count;					
	}
	else if (strcmp(str,"origin")==0)
	{
		strcpy(ic_code[line].class,"AD");
		ic_code[line].opcode=3;
		getstring();
		for(i=0;i<s_count;i++)
		{
			if(strcmp(str,sobj[i].symbol)==0)
			{	
				addr=sobj[i].addr;
				break;
			}
		}
		getstring();
		if(str[0]=='+')
		{
			getstring();
			jump=atoi(str);
			lc=addr+jump;	
		}				
	}
	else if (strcmp(str,"end")==0)
	{
		strcpy(ic_code[line].class,"AD");
		ic_code[line].opcode=5;
		for(i=l_index;i<l_count;i++)
		{
			lobj[i].addr=lc;
			lc++;
		}
	}
	else
	{
		flag=search_mot();
		if (flag!=1)
		{
			assign_symbol();
			getstring();
			flag=search_mot();
			if (strcasecmp(str,"ds")==0)
			{
				ic_code[line].lc=lc;
				getstring();
				lc+=atoi(str);
				ic_code[line].tab='C';
				ic_code[line].no= atoi(str);	
				return ;			
		 	}
			else if (strcasecmp(str,"dc")==0)
			{
				ic_code[line].lc=lc;
				getstring();
				lc+=1;
				ic_code[line].tab='C';
				ic_code[line].no= (int)str[1]-'0';
				return ;			
		 	}
			else if (strcasecmp(str,"equ")==0)
			{
				strcpy(ic_code[line].class,"AD");				
				ic_code[line].opcode= 4;
				getstring();
				sobj[s_count-1].addr=addr_of_symbol();
				search_symbol();
				return ;
			}
				
		}

		ic_code[line].lc=lc;
		if(strcasecmp(str,"stop")==0)
		{
			lc++;		
			return;	
		}
			
		getstring();
		serach_reg();
		getstring();
		lc++;
		if(str[0]=='=')
			search_literal();
		else
		{
			search_symbol();
		}
	}
}

void display()
{
	printf("\n");
	for(int i=0;i<15;i++)
	{
		if(ic_code[i].lc  )
			printf("%d\t%s%d\t%d\t%c%d\n",ic_code[i].lc,ic_code[i].class,ic_code[i].opcode,ic_code[i].reg,ic_code[i].tab,ic_code[i].no);
		else
			printf("\t%s%d\t\t%c%d\n",ic_code[i].class,ic_code[i].opcode,ic_code[i].tab,ic_code[i].no);

	}

}

int search_mot()
{
	int flag=-1;
	for(int i=0;i<10;i++)
	{
		if(strcasecmp(str,mobj[i].mneum)==0)
		{
		
			strcpy(ic_code[line].class,mobj[i].class);
			ic_code[line].opcode= mobj[i].opcode;
			
			flag=1;	
		}
	}
	return flag;

}

void serach_reg()
{	
			if(strcasecmp(str,"areg")==0)
				ic_code[line].reg=1;
			else if(strcasecmp(str,"breg")==0)
				ic_code[line].reg=2;
			else if(strcasecmp(str,"creg")==0)
				ic_code[line].reg=3;
			else if(strcasecmp(str,"dreg")==0)
				ic_code[line].reg=4;		
}

void search_symbol()
{
	int i=0,flag=-1;
	for(i=0;i<s_count;i++)
	{
		if(strcasecmp(sobj[i].symbol,str)==0)
		{
			
			flag=1;
			ic_code[line].tab='S';
			ic_code[line].no=i+1;
			break;			
		}
	}
	if(flag==-1)
	{
		sobj[s_count].serial=s_count+1;
		strcpy(sobj[s_count].symbol,str);
		ic_code[line].tab='S';
		ic_code[line].no=sobj[s_count].serial;
		s_count++;
	}
}

void search_literal()
{
	int i=0,flag=-1;
	for(i=l_index;i<l_count;i++)
	{
		if(strcasecmp(lobj[i].literal,str)==0)
		{
			flag=1;
			break;	
		}
	}
	if(flag==-1)
	{
		lobj[l_count].serial=l_count+1;
		strcpy(lobj[l_count].literal,str);
		ic_code[line].tab='L';
		ic_code[line].no=lobj[l_count].serial;
		l_count++;
	}
}

void ls_display()
{
	int i=0;
	printf("\nliteral table\n");
	for(i=0;i<l_count;i++)
	{
		printf("%d\t",lobj[i].serial);
		printf("%s\t",lobj[i].literal);
		printf("%d\n",lobj[i].addr);
	}
	printf("\nsymbol table\n");
	
	for(i=0;i<s_count;i++)
	{
		printf("%d\t",sobj[i].serial);
		printf("%s\t",sobj[i].symbol);
		printf("%d\n",sobj[i].addr);
	}
}

//assign in symbol table 
void assign_symbol()
{
	int i=0,flag=-1;
	for(i=0;i<s_count;i++)
	{		
		if(strcasecmp(sobj[i].symbol,str)==0)
		{		
			sobj[i].addr=lc;
			flag=1;	
			break;
		}
	}
	if(flag==-1)
	{	
		sobj[i].addr=lc;
		sobj[s_count].serial=s_count+1;
		strcpy(sobj[s_count].symbol,str);
		s_count++;
	}
}

int addr_of_symbol()
{
	int i=0,flag=-1;
	for(i=0;i<s_count;i++)
	{
		if(strcasecmp(sobj[i].symbol,str)==0)
		{
			return sobj[i].addr;	
		}
	}
}


/*output

START	AD	1
LTORG	AD	2
ORIGIN	AD	3
EQU	AD	4
END	AD	5
MOVEM	IS	1
ADD	IS	2
MOVER	IS	3
SUB	IS	4
MULT	IS	5
PRINT	IS	6
LOAD	IS	7
STOP	IS	8
DC	DL	9
DS	DL	10

	AD1		C200
200	IS3	1	L1
201	IS1	1	S1
202	IS3	1	S1
203	IS3	3	S3
204	IS2	3	L2
	AD2		0
207	IS4	1	L3
	AD3		0
212	IS5	3	S3
213	IS8	0	0
214	DL10	0	C2
216	DL9	0	C3
	AD4		S2
	AD5		0

literal table
1	='5'	205
2	='1'	206
3	='1'	217

symbol table
1	a	214
2	loop	202
3	b	216
4	next1	207
5	next2	202

*/

