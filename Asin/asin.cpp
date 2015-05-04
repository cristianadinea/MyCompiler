#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "asin.h"


int stare,linie,n=0;//n va tine minte prima pozitie libera din tabela de constante
atom *first=NULL,*last=NULL;
tab_const tab[100];
atom *q=NULL;
FILE *rez,*rezalex;

atom* alex(atom *first)
{

	FILE *f,*g;
	tab_cheie cuvch[31];
	int poz,i,id_cuvch,atr,er,pozitie,baza_int,val,cifra,u,exponent,eroare=0;
	atom *q=NULL;
	char c,buffer[256];
	char baza[256],semn,nr_real_sir[256];
	double nr_real;
	
	//creez tabela de cuv cheie
	g=fopen("cheie.txt","r");
	for(i=0;i<31;i++)
	{
		fscanf(g,"%s",buffer);
		cuvch[i].cheie=(char*)malloc(sizeof(buffer)+1);
		strcpy(cuvch[i].cheie,buffer);
	}
	stare=0;
	f=fopen("asin-test2.pas","r");
	linie=1;
	while(!feof(f))
	{
		switch(stare)
		{
			case 0:	for(i=0;i<poz;i++)
						buffer[i]='\0';
					eroare=0;
					poz=0;
					c=fgetc(f);
					if (((c>='a')&&(c<='z'))||((c>='A')&&(c<='Z'))) stare=1;
					if ((c>='0')&&(c<='9')) stare=3;
					if ((c=='+')||(c=='-')||(c=='*')||(c=='/')||(c=='(')||(c==')')||(c=='[')||(c==']')||(c=='=')) stare=18;
					if (c==':') stare=19;
					if (c=='>') stare=20;
					if (c=='<') stare=21;
					if ((c==';')||(c==',')) stare=22;
					if (c=='.') stare=23;
					if (c=='"') stare=24;
					if (c=='\'') stare=26;
					if (c=='{') stare=30;
					if ((c=='\n')||(c=='\t')||(c==' ')) stare=0;
					if (c=='\n') linie++;
					if (stare!=0) 
						buffer[poz++]=c; 
					break;

			case 1: c=fgetc(f);
					while (((c>='a')&&(c<='z'))||((c>='A')&&(c<='Z'))||((c>='0')&&(c<='9')))
					{ 
						buffer[poz++]=c;
						c=fgetc(f);
					}
					ungetc(c,f);
					stare=2;
					break;

			case 2:	buffer[poz]='\0';
					id_cuvch=0;  //id_cuvch=0 - identificator,id_cuvch =1 - cuvant cheie
					atr=-1;	
					for (i=0;i<31;i++)
						if (strcmp(buffer,cuvch[i].cheie)==0) 
						{ 
							id_cuvch=1; 
							atr=i;//al catelea cuvant cheie este
						}
					//creare nou atom
					q=new atom;
					q->nume=(char*)malloc(sizeof(buffer)+1);
					strcpy(q->nume,buffer);
					q->linie=linie;
					q->cod_lexical=id_cuvch;//0-identificator, 1-cuvant cheie
					if (id_cuvch==1) 
						q->atribut=atr;//atributul pt cuvantul cheie
					else q->atribut=-1;//atributul pentru identificator (nu are)
					
					q->urm=NULL;
					if (first==NULL) 
					{ 
						first=q;
						last=first;
					}
					else 
					{ 
						last->urm=q;
						last=q;
					}
					stare=0;
					break;

			case 3:	
					c=fgetc(f);
					while ((c>='0') && (c<='9'))
					{ 
						buffer[poz++]=c;
						c=fgetc(f);
					}
					if (c=='.') 
						stare=9;
					else if (c=='@') 
					{ 
						buffer[poz++]=c; 
						stare=5;
					}
					else if (c=='e' || c=='E')
					{
						buffer[poz++]=c;
						stare=12;
					}
					else
					{
						ungetc(c,f);
						stare=4;
					}
					break;
		
			case 4://atr ->linie tabel //trebuie creata tabela 
					//transformarea din buffer in nr
					buffer[poz]='\0';
					tab[n].tip=1;//nr intreg
					tab[n].valoare=atoi(buffer);
					n++;
					//creare atom nou
					q=new atom;
					q->nume=(char*)malloc(sizeof(buffer)+1);
					strcpy(q->nume,buffer);
					q->linie=linie;
					q->cod_lexical=2;//constanta numerica
					q->atribut=n-1;//linie tabel
					q->urm=NULL;
					if (first==NULL) 
					{ 
						first=q;
						last=first;
					}
					else 
					{ 
						last->urm=q;
						last=q;
					}
					stare=0;
					break;

			case 5:	c=fgetc(f);
					if (((c>='0')&&(c<='9'))||((c>='a')&&(c<='f'))||((c>='A')&&(c<='F')))
					{
						buffer[poz++]=c;
						stare=7;
					}
					else stare=6;//eroare
					break;
			
			case 6:	printf("\nEroare pe linia : %d, dupa @ trebuie sa urmeze cifra(0-9,a-f)\n",linie);
					stare=0;
					break;

			case 7:
					c=fgetc(f);
					while (((c>='0')&&(c<='9'))||((c>='a')&&(c<='f'))||((c>='A')&&(c<='F')))
					{
						buffer[poz++]=c;
						c=fgetc(f);
					}
					ungetc(c,f);
								
				
					for (i=0;i<strlen(buffer);i++)
						if (buffer[i]=='@')
						{
							//strcat(baza,buffer[i]);
							pozitie=i;
						}
					for(i=0;i<pozitie;i++)
						baza[i]=buffer[i];
					baza[pozitie]='\0';
					baza_int=atoi(baza);
					if (baza_int>16)
					{
						printf("\n Eroare la linia: %d!baza e mai mare ca 16 \n",linie);
						eroare=1;
					}
					else
						if (baza_int<10)
						{
							for(i=pozitie+1;i<strlen(buffer);i++)
								if(((buffer[i]<48)&&(buffer[i]>57-(10-baza_int))))
								{
									printf("\nEroare la linia %d!cifrele sunt mai mari decat baza \n",linie);
									eroare=1;
									break;
								}
						}
						else if (baza_int>=10)
							for(i=pozitie+1;i<strlen(buffer);i++)
								if (((buffer[i]<'a')&&(buffer[i]>'a'+(baza_int-11)||((buffer[i]<48)&&(buffer[i]>57)))))	
								{
									printf("\nEroare la linia %d!cifrele sunt mai mari decat baza \n",linie);
									eroare=1;
									break;
								}
					if (eroare==1)
						stare=0;
					else
					{
						val=0;
						u=1;
						buffer[poz]='\0';
						for(i=strlen(buffer)-1;i>pozitie;i--)
						{
							if (buffer[i]=='a'||buffer[i]=='A')
								cifra=10;
							else if (buffer[i]=='b'||buffer[i]=='B')
								cifra=11;
							else if (buffer[i]=='c'||buffer[i]=='C')
								cifra=12;
							else if (buffer[i]=='d'||buffer[i]=='D')
								cifra=13;
							else if (buffer[i]=='e'||buffer[i]=='E')
								cifra=14;
							else if (buffer[i]=='f'||buffer[i]=='F')
								cifra=15;
							else
								cifra=buffer[i]-'0';
							val=val+cifra*u;
							u=u*baza_int;
						}
						stare=8;//nr intreg (  baza@cifra(cifra)*)
					}
					break;

			case 8 :
					//pun in tabela de const 
					tab[n].tip=1;//nr intreg
					tab[n].valoare=val;
					n++;
				
					//creare atom nou
					q=new atom;
					q->nume=(char*)malloc(sizeof(buffer)+1);
					strcpy(q->nume,buffer);
					q->linie=linie;
					q->cod_lexical=2;//constanta numerica
					q->atribut=n-1;//linie tabel pt nr intreg
					q->urm=NULL;
					if (first==NULL) 
					{ 
						first=q;
						last=first;
					}
					else 
					{ 
						last->urm=q;
						last=q;
					}
					stare=0;
					break;

			case 9:	c=fgetc(f);
					if (c=='.') 
					{ 
							ungetc(c,f); 
							ungetc(c,f);
							stare=4;
					}
					else 
					if ((c>='0') && (c<='9'))
					{ 
							buffer[poz++]='.';//pun '.'
							buffer[poz++]=c;//pun cifra
							stare=11;
					}
					else stare=10;//eroare
					break;

			case 10:printf("\nEroare la linia : %d , este asteptata o cifra dupa ',' in numarul real",linie);
					stare=0;
					break;
			
			case 11:c=fgetc(f);
					while ((c>='0') && (c<='9'))
					{ 
						buffer[poz++]=c;
						c=fgetc(f);
					}
					if (c=='e' || c=='E')
					{
						buffer[poz++]=c;
						stare=12;
					}
					else
					{
						ungetc(c,f);
						stare=17;//nr real de forma cifra(cifra)*.cifra(cifra)*
					}
					break;

			case 12:c=fgetc(f);
					semn='0';
					if ((c=='+') || (c=='-'))
					{
						semn=c;
						buffer[poz++]=c;
						stare=14;
					}
					else if ((c>='0') && (c<='9'))
					{
						buffer[poz++]=c;
						stare=15;
					}
					else
					{
						stare=13;//eroare
						er=0;
					}
					break;

			case 13:if (er==0)
						printf("\nEroare la linia : %d ,intr-un numar real dupa e sau E urmeaza + sau - sau cifra ",linie);
					else if (er==1)
						printf("\nEroare la linia : %d, intr-un numar real dupa + sau - urmeaza cel putin o cifra",linie);
					stare=0;
					break;

			case 14:c=fgetc(f);
					if ((c>='0') && (c<='9'))
					{
						buffer[poz++]=c;
						stare=15;
					}
					else
					{
						stare=13;//eroare
						er=1;
					}
					break;

			case 15:c=fgetc(f); 
					while((c>='0') && (c<='9'))
					{
						buffer[poz++]=c;
						c=fgetc(f);
					}
					for(i=0;i<strlen(buffer);i++)
						if ((buffer[i]=='e')||(buffer[i]=='E'))
							pozitie=i;
					//nr_real_sir=strncpy(buffer,nr_real_sir,pozitie);
					for(i=0;i<pozitie;i++)
						nr_real_sir[i]=buffer[i];
					nr_real=atof(nr_real_sir);
						
					pozitie++;
					if ((buffer[pozitie]=='+')||(buffer[pozitie]=='-'))
						pozitie++;
					else
						semn='+';
					exponent=buffer[pozitie]-'0';
					pozitie++;
					for(i=pozitie;i<strlen(buffer);i++)
					{
						exponent=exponent*10+(buffer[i]-'0');
					}
					val=1;
					while(exponent!=0)
					{
						val=val*10;
						exponent--;
					}

					if(semn=='+')
						nr_real=nr_real*val;
					else
						nr_real=nr_real/val;

					ungetc(c,f);
					stare=16;
					break;
			case 16: 
					buffer[poz]='\0';
					//pun in tabela de const 
					tab[n].tip=2;//nr real
					tab[n].valoare=nr_real;
					n++;
					//creare atom nou
					q=new atom;
					q->nume=(char*)malloc(sizeof(buffer)+1);
					strcpy(q->nume,buffer);
					q->linie=linie;
					q->cod_lexical=2;//constanta numerica
					q->atribut=n-1;//linie tabel pt nr real cu exponent
					q->urm=NULL;
					if (first==NULL) 
					{ 
						first=q;
						last=first;
					}
					else 
					{ 
						last->urm=q;
						last=q;
					}
					stare=0;
					break;

			case 17:
					//nr  real de forma cifra(cifra)*.cifra(cifra)*
					buffer[poz]='\0';
					//pun in tabela de const 
					tab[n].tip=2;//nr real
					tab[n].valoare=atof(buffer);
					n++;
					//creare atom nou
					q=new atom;
					q->nume=(char*)malloc(sizeof(buffer)+1);
					strcpy(q->nume,buffer);
					q->linie=linie;
					q->cod_lexical=2;//constanta numerica
					q->atribut=n-1;//linie tabel pt nr real
					q->urm=NULL;
					if (first==NULL) 
					{ 
						first=q;
						last=first;
					}
					else 
					{ 
						last->urm=q;
						last=q;
					}
					stare=0;
					break;

			case 18:buffer[poz]='\0';
			        atr=-1;
					if ((buffer[0]==':')&&(buffer[1]=='=')) 
						atr=14;
					if (buffer[0]=='<') 
					{ 
						if (buffer[1]=='=') atr=9;
						else 
						{ 
							if (buffer[1]=='>') atr=12;
							else atr=8;
						}
					}
					if (buffer[0]=='>') 
					{ 
						if (buffer[1]=='=') atr=11;
					    else atr=10;
					}
					if (buffer[0]=='+') atr=0;
					if (buffer[0]=='-') atr=1;
					if (buffer[0]=='*') atr=2;
					if (buffer[0]=='/') atr=3;
					if (buffer[0]=='(') atr=4;
					if (buffer[0]==')') atr=5;
					if (buffer[0]=='[') atr=6;
					if (buffer[0]==']') atr=7;
					if (buffer[0]=='=') atr=13;

					//creare atom nou
					q=new atom;
					q->nume=(char*)malloc(sizeof(buffer)+1);
					strcpy(q->nume,buffer);
					q->linie=linie;
					q->cod_lexical=3;//operator
					q->atribut=atr;
					q->urm=NULL;
					if (first==NULL) 
					{ 
						first=q;
						last=first;
					}
					else 
					{ 
						last->urm=q;
						last=q;
					}
					stare=0;
					break;

			case 19:c=fgetc(f);
					if (c=='=') 
					{
						buffer[poz++]=c; 
						stare=18;//:=
					}
					else
					{
						stare=22;//:
						ungetc(c,f);
					}
					break;
			
			case 20:c=fgetc(f);
					if (c=='=') 
						buffer[poz++]=c;
					else
						ungetc(c,f);
					stare=18;
					break;

			case 21:c=fgetc(f);
					if ((c=='>')||(c=='=')) 
						buffer[poz++]=c; 
					else
						ungetc(c,f);
					stare=18;
					break;

			case 22:buffer[poz]='\0';
					if (buffer[0]==';') atr=0;
					if (buffer[0]==':') atr=3;
					if (buffer[0]==',') atr=2;
					if ((buffer[0]=='.')&&(buffer[1]=='\0')) atr=1;	
					if ((buffer[0]=='.')&&(buffer[1]=='.')) atr=4;
					
					//creare atom nou
					q=new atom;
					q->nume=(char*)malloc(sizeof(buffer)+1);
					strcpy(q->nume,buffer);
					q->linie=linie;
					q->cod_lexical=4;//delimitator
					q->atribut=atr;
					q->urm=NULL;
					if (first==NULL) 
					{ 
						first=q;
						last=first;
					}
					else 
					{ 
						last->urm=q;
						last=q;
					}
					stare=0;
					break;

			case 23:c=fgetc(f);
					if (c=='.') 
						buffer[poz++]=c; 
					else 
						ungetc(c,f);
					stare=22;
					break;

			case 24:c=fgetc(f);
					while (((c!='\n')&&(c!='\t'))&&(c!='"'))
					{
						buffer[poz++]=c;
						c=fgetc(f);
					}
					if (c=='"') 
					{
						stare=25;
						buffer[poz++]=c;
					}
					else
					{
						printf("\nEroare la sir de caractere pe linia: %d",linie);
						stare=0;
					}
					break;

			case 25:buffer[poz]='\0';

					//creare atom nou
					q=new atom;
					q->nume=(char*)malloc(sizeof(buffer)+1);
					strcpy(q->nume,buffer);
					q->linie=linie;
					q->cod_lexical=5;//constanta sir
					q->atribut=-2;
					q->urm=NULL;
					if (first==NULL) 
					{ 
						first=q;
						last=first;
					}
					else 
					{ 
						last->urm=q;
						last=q;
					}
					stare=0;
					break;

			case 26:c=fgetc(f);
					buffer[poz++]=c;
					stare=27;
					val=c;//-'0';
					break;

			case 27:c=fgetc(f);
					if (c=='\'') 
					{	
						buffer[poz++]=c;
						stare=28;
					}
					else
					{
						stare=29;//eroare
						ungetc(c,f);
					}
					break;

			case 28:buffer[poz]='\0';
					//pun in tabela de const 
					tab[n].tip=3;//car
					tab[n].valoare=val;
					n++;
					//creare atom nou
					q=new atom;
					q->nume=(char*)malloc(sizeof(buffer)+1);
					strcpy(q->nume,buffer);
					q->linie=linie;
					q->cod_lexical=6;
					q->atribut=n-1;
					q->urm=NULL;
					if (first==NULL) 
					{ 
						first=q;
						last=first;
					}
					else 
					{ 
						last->urm=q;
						last=q;
					}
					stare=0;
					break;
			
			case 29:printf("\nEroare pe linia :%d,dupa un caracter tiparibil sau blank urmeaza '",linie);
					stare=0;
					break;
				 
			case 30:c=fgetc(f);
					while ((c!='}')&&(c!=feof(f))) 
					{
						if (c=='\n') 
						linie++;
						c=fgetc(f);
					}
					if (c=='}') stare=0;
					break;

			default:printf("\n Nu exista starea respectiva!!!");					
		}//switch
	}//while
	return first;	 
}//alex

atom *getNextAtom(atom *a)
{
	if (a->urm!=NULL)
		return a->urm;
	else
	{ 
		printf("Unexpected end of file \n"); 
		exit(1);
	}
}

int ProgramSursa()
{
	if ((q->cod_lexical==1)&&(q->atribut==19))//cuvantul cheie: program
	{
		q=getNextAtom(q);
		if (q->cod_lexical==0)//identificator
		{
			q=getNextAtom(q);
			if ((q->cod_lexical==4)&&(q->atribut==0))//delimitatorul: ';'
			{
				q=getNextAtom(q);
				if (Bloc())
				{
					if ((q->cod_lexical==4)&&(q->atribut==1))//delimitatorul: '.' 
						return 1;
					else 
					{ 
						printf("Eroare, se asteapta . pe linia: %d\n",q->linie);
						fprintf(rez,"Eroare, se asteapta . pe linia: %d\n",q->linie);
						return 0; 
					}
				}
				else 
				{
					printf("Eroare la Bloc\n");
					fprintf(rez,"Eroare la Bloc\n");
					return 0;
				}
			}
			else 
			{
				printf("Eroare, se asteapta ; pe linia %d \n",q->linie);
				fprintf(rez,"Eroare, se asteapta ; pe linia %d \n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare, lipseste un identificator pe linia: %d \n",q->linie); 
			fprintf(rez,"Eroare, lipseste un identificator pe linia: %d \n",q->linie); 
			return 0;
		}
	}
	else 
	{
		printf("Eroare lipseste cuvantul cheie: program pe linia: %d \n",q->linie);
		fprintf(rez,"Eroare lipseste cuvantul cheie: program pe linia: %d \n",q->linie);
		return 0;
	}
}//ProgramSursa



int Bloc()
{
	if (SectiuneConst())
		if (SectiuneVar())
			if (SectiuneDeclSubprog())
				if (InstrComp())
					return 1;
				else 
				{
					printf("Eroare la InstrComp la linia: %d\n",q->linie);
					fprintf(rez,"Eroare la InstrComp la linia: %d\n",q->linie);
					return 0;
				}
			else
			{
				printf("Eroare la SectiuneDeclSubprog la linia: %d\n",q->linie);
				fprintf(rez,"Eroare la SectiuneDeclSubprog la linia: %d\n",q->linie);
				return 0;
			}
		else
		{
			printf("Eroare la SectiuneVar la linia: %d\n",q->linie);
			fprintf(rez,"Eroare la SectiuneVar la linia: %d\n",q->linie);
			return 0;
		}
	else
	{
		printf("Eroare la SectiuneConst la linia: %d\n",q->linie);
		fprintf(rez,"Eroare la SectiuneConst la linia: %d\n",q->linie);
		return 0;
	}
}//Bloc


int SectiuneConst()
{
	if ((q->cod_lexical==1)&&(q->atribut==4))//cuvantul cheie: const 
	{
		q=getNextAtom(q);
		if (ListaDeclConst())
			return 1;
		else 
		{
			printf("Eroare la ListaDeclConst la linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaDeclConst la linia: %d\n",q->linie);
			return 0;
		}
	}
	else
		return 1;//ramura cu epsilon
}//SectiuneConst

int ListaDeclConst()
{
	if (DeclarConst()) 
	{ 
		if (ListaDeclConst1()) 
			return 1;
		else 
		{
			printf("Eroare la ListaDeclConst1 la linia: %d \n",q->linie);
			fprintf(rez,"Eroare la ListaDeclConst1 la linia: %d \n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//ListaDeclConst

int ListaDeclConst1()
{
	if (DeclarConst())
	{
		if (ListaDeclConst1()) 
			return 1;
	    else
			return 0;
	}
	else 
		return 1;//ramura cu epsilon
}//ListaDeclConst1

int SectiuneVar()
{
	if ((q->cod_lexical==1)&&(q->atribut==24))//cuvantul cheie: var
    {
		q=getNextAtom(q);
		if (ListaDeclVar()) 
			return 1;
		else 
		{
			printf("Eroare la ListaDeclVar la linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaDeclVar la linia: %d\n",q->linie);
			return 0;
		}
    }
	else 
		return 1;//ramura cu epsilon
}//SectiuneVar

int ListaDeclVar()
{
	if (DeclarVar())
	{
		if (ListaDeclVar1())
			return 1;
		else 
		{
			printf("Eroare la ListaDeclVar1 la linia: %d\n", q->linie);
			fprintf(rez,"Eroare la ListaDeclVar1 la linia: %d\n", q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//ListaDeclVar


int ListaDeclVar1()
{
	if (DeclarVar())
	{
		if (ListaDeclVar1()) 
			return 1;
		else
			return 0;
	}
	else 
		return 1;//ramura cu epsilon
}//ListaDeclVar1

int SectiuneDeclSubprog()
{
	if (ListaDeclSubprog())
		return 1;
	else
		return 1;//ramura cu epsilon
}//SectiuneDeclSubprog

int ListaDeclSubprog()
{
	if (DeclSubprog())
	{
		if (ListaDeclSubprog1()) 
			return 1;
		else 
		{
			printf("Eroare la ListaDeclSubprog1 la linia: %d \n",q->linie);
			fprintf(rez,"Eroare la ListaDeclSubprog1 la linia: %d \n",q->linie);
			return 0;
		}
	}
	else
		return 0;
}//ListaDeclSubprog

int ListaDeclSubprog1()
{
	if (DeclSubprog())
	{
		if (ListaDeclSubprog1()) 
			return 1;
		else
			return 0;
	}
	else 
		return 1;//ramura cu epsilon
}//ListaDeclSubprog1

int DeclSubprog()
{
	if (DeclarFunctie()) 
		return 1;
	else
	{
		if (DeclarProcedura()) 
			return 1;
		else 
			return 0;
	}
}//DeclSubprog


int DeclarConst()
{
	if (q->cod_lexical==0)//identificator
	{
		q=getNextAtom(q);
		if ((q->cod_lexical==3)&&(q->atribut==13))//operatorul =
		{
			q=getNextAtom(q);
			if (ExpresieStatica()) 
			{
				if ((q->cod_lexical==4)&&(q->atribut==0))//delimitatorul ;
				{
					q=getNextAtom(q);
					return 1;
				}
				else
				{
					printf("Eroare, se asteapta ; pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare, se asteapta ; pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else 
			{
				printf("Eroare la ExpresieStatica la linia: %d\n",q->linie);
				fprintf(rez,"Eroare la ExpresieStatica la linia: %d\n",q->linie);
				return 0;
			}
		}
		else
		{
			printf("Eroare, se asteapta = pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare, se asteapta = pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else
		return 0;
}//DeclarConst

int DeclarVar()
{
	if (ListaId())
	{
		if ((q->cod_lexical==4)&&(q->atribut==3))//delimitatorul :
		{
			q=getNextAtom(q);
			if (Tip())
			{
				if ((q->cod_lexical==4)&&(q->atribut==0))//delimitatorul ;
				{
					q=getNextAtom(q);
					return 1;
				}
				else 
				{
					printf("Eroare se asteapta ; pe linia :%d\n",q->linie);
					fprintf(rez,"Eroare se asteapta ; pe linia :%d\n",q->linie);
					return 0;
				}
			}
			else 
			{
				printf("Eroare la Tip la linia: %d\n",q->linie);
				fprintf(rez,"Eroare la Tip la linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare se asteapta : pe linia :%d\n",q->linie);
			fprintf(rez,"Eroare se asteapta : pe linia :%d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//DeclarVar


int ListaId()
{
	if (q->cod_lexical==0)//identificator
	{
		q=getNextAtom(q);
		if (ListaId1()) 
			return 1;
		else 
		{
			printf("Eroare la ListaId1 la linia: %d\n",q->linie); 
			fprintf(rez,"Eroare la ListaId1 la linia: %d\n",q->linie); 
			return 0;
		}
	}
	else 
		return 0;
}//ListaId

int ListaId1()
{
	if ((q->cod_lexical==4)&&(q->atribut==2))//delimitatorul : ','
	{
		q=getNextAtom(q);
		if (q->cod_lexical==0)//identificator
		{
			q=getNextAtom(q);
			if (ListaId1()) 
				return 1;
			else
				return 0;
		}
		else 
		{
			printf("Eroare, se asteapta un identificator la linia: %d\n",q->linie);
			fprintf(rez,"Eroare, se asteapta un identificator la linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 1;//ramura cu epsilon
}//ListaId1

int Tip()
{
	if (TipSimplu())
		return 1;
	else 
	{
		if (TipTablou())
			return 1;
		else
		{
			if (TipStruct())
				return 1;
			else
			{
				printf("Eroare la TipStruct la linia: %d \n",q->linie);				
				fprintf(rez,"Eroare la TipStruct la linia: %d \n",q->linie);				
				return 0;
			}
			printf("Eroare la TipTablou la linia: %d \n",q->linie);
			fprintf(rez,"Eroare la TipTablou la linia: %d \n",q->linie);
			return 0;
		}
		printf("Eroare la TipSimplu la linia: %d \n",q->linie);
		fprintf(rez,"Eroare la TipSimplu la linia: %d \n",q->linie);
		return 0;
	}
}//Tip

int TipSimplu()
{
	if ((q->cod_lexical==1)&& (((q->atribut==13)||(q->atribut==20)||(q->atribut==3))))//integer,real,char
	{
		q=getNextAtom(q);
		return 1;
		
	}
	else
		return 0;
}//TipSimplu

int TipTablou()
{
	if ((q->cod_lexical==1)&&(q->atribut==26))//cuvantul cheie array
	{
		q=getNextAtom(q);
		if ((q->cod_lexical==3)&&(q->atribut==6))//operatorul [
		{
			q=getNextAtom(q);
			if (ExpresieStatica())
			{
				if ((q->cod_lexical==4)&&(q->atribut==4))//delimitatorul ..
				{
					q=getNextAtom(q);
					if (ExpresieStatica())
					{
						if ((q->cod_lexical==3)&&(q->atribut==7))//operatorul ]
						{
							q=getNextAtom(q);
							if ((q->cod_lexical==1)&&(q->atribut==16))//cuvantul cheie of
							{
								q=getNextAtom(q);
								if (TipSimplu()) 
									return 1;
								else
								{
									printf("Eroare la TipSimplu la linia: %d\n",q->linie);
									fprintf(rez,"Eroare la TipSimplu la linia: %d\n",q->linie);
									return 0;
								}
							}
							else 
							{
								printf("Eroare, lipseste cuvantul cheie of, la linia: %d\n",q->linie);
								fprintf(rez,"Eroare, lipseste cuvantul cheie of, la linia: %d\n",q->linie);
								return 0;
							}
						}
						else
						{
							printf("Eroare, lipseste ] la linia %d\n",q->linie);
							fprintf(rez,"Eroare, lipseste ] la linia %d\n",q->linie);
							return 0;
						}
					}
					else
					{
						printf("Eroare la ExpresieStatica la linia: %d\n",q->linie);
						fprintf(rez,"Eroare la ExpresieStatica la linia: %d\n",q->linie);
						return 0;
					}
				}
				else
				{
					printf("Eroare ,lipseste delimitatorul .. pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare ,lipseste delimitatorul .. pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else 
			{
				printf("Eroare la ExpresieStatica pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare la ExpresieStatica pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare lipseste [ pe linia: %d\n", q->linie);
			fprintf(rez,"Eroare lipseste [ pe linia: %d\n", q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//TipTablou

int TipStruct()
{
	if ((q->cod_lexical==1)&&(q->atribut==27))//cuvantul cheie record
	{
		q=getNextAtom(q);
		if (ListaCamp())
		{
			if ((q->cod_lexical==1)&&(q->atribut==9))//cuvantul cheie end
			{
				q=getNextAtom(q);
				return 1;
			}
			else 
			{
				printf("Eroare lipseste cuvantul cheie end pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare lipseste cuvantul cheie end pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else
		{
			printf("Eroare la ListaCamp la linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaCamp la linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//TipStruct

int ListaCamp()
{
	if (DeclSimpla())
	{
		if (ListaCamp1()) 
			return 1;
		else 
		{
			printf("Eroare la ListaCamp1 la linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaCamp1 la linia: %d\n",q->linie);
			return 0;
		}
	}
	else
		return 0;
}//ListaCamp

int ListaCamp1()
{
	if ((q->cod_lexical==4)&&(q->atribut==0))//delimitatorul ;
	{
		q=getNextAtom(q);
		if (DeclSimpla())
		{
			if (ListaCamp1()) 
				return 1;
			else
				return 0;
		}
		else
			return 1;//sa fie asta ramura cu epsilon
	}
	else
	{
		printf("Eroare, lipseste ; pe linia: %d\n",q->linie);
		fprintf(rez,"Eroare, lipseste ; pe linia: %d\n",q->linie);
		return 0;
	}
}//ListaCamp1

int DeclSimpla()
{
	if (ListaId())
	{
		if ((q->cod_lexical==4)&&(q->atribut==3))//delimitatorul :
		{
			q=getNextAtom(q);
			if (TipSimplu())
				return 1;
			else
			{
				printf("Eroare la TipSimplu, pe linia: %d \n",q->linie);
				fprintf(rez,"Eroare la TipSimplu, pe linia: %d \n",q->linie);
				return 0;
			}
		}
		else
		{
			printf("Eroare lipseste : pe linia: %d \n",q->linie);
			fprintf(rez,"Eroare lipseste : pe linia: %d \n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//DeclSimpla


int DeclarFunctie()
{
	if ((q->cod_lexical==1)&&(q->atribut==11))//cuvantul cheie function
	{
		q=getNextAtom(q);
		if (AntetSubprog())
		{
			if ((q->cod_lexical==4)&&(q->atribut==3))//delimitatorul :
			{
				q=getNextAtom(q);
				if (TipSimplu())
				{
					if ((q->cod_lexical==4)&&(q->atribut==0))//delimitatorul ;
					{
						q=getNextAtom(q);
						if (Bloc())
						{
							if ((q->cod_lexical==4)&&(q->atribut==0))//delimitatorul ;
							{
								q=getNextAtom(q);
								return 1;
							}
							else
							{
								printf("Eroare, se astepta ; pe linia: %d\n",q->linie);
								fprintf(rez,"Eroare, se astepta ; pe linia: %d\n",q->linie);
								return 0;
							}
						}
						else 
						{
							printf("Eroare la Bloc pe linia: %d\n",q->linie);
							fprintf(rez,"Eroare la Bloc pe linia: %d\n",q->linie);
							return 0;
						}
					}
					else
					{
						printf("Eroare, se astepta ; pe linia: %d\n",q->linie);
						fprintf(rez,"Eroare, se astepta ; pe linia: %d\n",q->linie);
						return 0;
					}
				}
				else 
				{
					printf("Eroare la TipSimplu ; pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare la TipSimplu ; pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else
			{
				printf("Eroare, se astepta : pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare, se astepta : pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare la AntetSubprog  pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la AntetSubprog  pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//DeclarFunctie


int DeclarProcedura()
{
	if ((q->cod_lexical==1)&&(q->atribut==18))//cuvantul cheie procedure
	{
		q=getNextAtom(q);
		if (AntetSubprog())
		{
			if ((q->cod_lexical==4)&&(q->atribut==0))//delimitatorul ;
			{
				q=getNextAtom(q);
				if (Bloc())
				{
					if ((q->cod_lexical==4)&(q->atribut==0))//delimitatorul ;
					{
						q=getNextAtom(q);
						return 1;
					}
					else
					{
						printf("Eroare, se astepta ; pe linia: %d",q->linie);
						fprintf(rez,"Eroare, se astepta ; pe linia: %d",q->linie);
						return 0;
					}
				}
				else 
				{
					printf("Eroare la Bloc pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare la Bloc pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else 
			{
				printf("Eroare, se asteapta ; pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare, se asteapta ; pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare la AntetSubprog pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la AntetSubprog pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//DeclarProcedura


int AntetSubprog()
{
	if (q->cod_lexical==0)//identificator
	{
		q=getNextAtom(q);
		if (ParamForm()) 
			return 1;
		else
		{
			printf("Eroare la ParamForm pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ParamForm pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else
	{
		printf("Eroare, se astepta identificator pe linia: %d\n",q->linie);
		fprintf(rez,"Eroare, se astepta identificator pe linia: %d\n",q->linie);
		return 0;
	}
}//AntetSubprog

int ParamForm()
{
	if ((q->cod_lexical==3)&&(q->atribut==4))//operatorul (
	{
		q=getNextAtom(q);
		if (ListaParamForm())
		{
			if ((q->cod_lexical==3)&&(q->atribut==5))//operatorul )
			{
				q=getNextAtom(q);
				return 1;
			}
			else
			{
				printf("Eroare, se ateapta ) pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare, se ateapta ) pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare la ListaParamForm pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaParamForm pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 1; // ramura epsilon
}//ParamForm


int ListaParamForm()
{
	if (DeclarPar())
	{
		if (ListaParamForm1()) 
			return 1;
		else 
		{
			printf("Eroare la ListaParamForm1 pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaParamForm1 pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//ListaParamForm

int ListaParamForm1()
{
	if ((q->cod_lexical==4)&&(q->atribut==0))//delimitatorul ';'
	{
		q=getNextAtom(q);
		if (DeclarPar())
		{
			if (ListaParamForm1()) 
				return 1;
			else
				return 0;
		}
		else 
			return 0;
	}
	else 
		return 1;//ramura epsilon
}//ListaParamForm1

int DeclarPar()
{
	if ((q->cod_lexical==1)&&(q->atribut==24))//cuvantul cheie var 
	{
		q=getNextAtom(q);
		if (DeclSimpla())
			return 1;
		else
		{
			printf("Eroare la DeclSimpla pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la DeclSimpla pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else
	{
		if (DeclSimpla())
			return 1;
		else 
		{
			printf("Eroare la DeclSimpla pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la DeclSimpla pe linia: %d\n",q->linie);
			return 0;
		}
	}
}//DeclarPar

int ExpresieStatica()
{
	if (TermenStatic())
	{
		if (ExpresieStatica1())
			return 1;
		else 
		{
			printf("Eroare la ExpresieStatica1 pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ExpresieStatica1 pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//ExpresieStatica


int ExpresieStatica1()
{
	if (((q->cod_lexical==3)&&(q->atribut==0))||((q->cod_lexical==3)&&(q->atribut==1)))//operatorii:+ -
	{
		q=getNextAtom(q);
		if (TermenStatic()) 
		{
			if (ExpresieStatica1())
				return 1;
			else 
				return 0;
		}
		else 
			return 0;
	}
	else 
		return 1;//ramura cu epsilon
}//ExpresieStatica1

int TermenStatic()
{
	if (FactorStatic())
	{
		if (TermenStatic1())
			return 1;
		else 
		{
			printf("Eroare la TermenStatic1 pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la TermenStatic1 pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//TermenStatic

int TermenStatic1()
{
	if (((q->cod_lexical==3)&&((q->atribut==2)||(q->atribut==3))) ||((q->cod_lexical==1)&&(q->atribut==5)) ||((q->cod_lexical==1)&&(q->atribut==14)))//operatorii: *,/ si cuvintele cheie: div, mod
    {
		q=getNextAtom(q);
	    if (FactorStatic())
		{
			if (TermenStatic1()) 
				return 1;
			else
				return 0;
		}
		else 
			return 0;
	}
	else 
		return 1;//ramura cu epsilon
}//TermenStatic1

int FactorStatic()
{
	if (q->cod_lexical==0)//identificator
	{
		q=getNextAtom(q);
		return 1;
	}
	else
	{
		  if (Constanta())
			  return 1;
		  else   
		  {
			  if ((q->cod_lexical==3)&&(q->atribut==4))//operatorul (
			  {
				  q=getNextAtom(q);
				  if (ExpresieStatica()) 
				  {	
					  if ((q->cod_lexical==3)&&(q->atribut==5)) //operatorul )
					  {
							q=getNextAtom(q);
							return 1;
					  }
				 	  else
					  {
						    printf("Eroare, se astepta ) pe linia: %d\n",q->linie);
							fprintf(rez,"Eroare, se astepta ) pe linia: %d\n",q->linie);
							return 0;
					  }
				  }
				  else 
				  {
						printf("Eroare la ExpresieStatica pe linia: %d\n",q->linie);
						fprintf(rez,"Eroare la ExpresieStatica pe linia: %d\n",q->linie);
						return 0;
				  }
			}
			else 
				return 0;
			return 0;
		}
		return 0;
	}
}//FactorStatic


int Constanta()
{
	if ((q->cod_lexical==2)||(q->cod_lexical==6))// constanta numerica , constanta caracter
	{
		q=getNextAtom(q);
		return 1;
	}
	else
		return 0;
}//Constanta


int InstrComp()
{
	if ((q->cod_lexical==1)&&(q->atribut==1))//cuvantul cheie: begin
	{
		q=getNextAtom(q);
		if (ListaInstr())
		{
			if ((q->cod_lexical==4)&&(q->atribut==0))// delimitatorul ;
			{
				q=getNextAtom(q);
				if ((q->cod_lexical==1)&&(q->atribut==9))//cuvantul cheie: end
				{
					q=getNextAtom(q);
					return 1;
				}
				else
				{
					printf("Eroare, lipseste end pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare, lipseste end pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else 
			{
				if ((q->cod_lexical==1)&&(q->atribut==9))//cuvantul cheie: end
				{
					q=getNextAtom(q);
					return 1;
				}
				else 
				{
					printf("Eroare, se asteapta end pe linie: %d\n",q->linie);
					fprintf(rez,"Eroare, se asteapta end pe linie: %d\n",q->linie);
					return 0;
				}
				printf("Eroare, lipseste delimitatorul ; pe linia: %d",q->linie);
				fprintf(rez,"Eroare, lipseste delimitatorul ; pe linia: %d",q->linie);
				return 0;
			}	
		}
		else 
		{
			printf("Eroare la ListaInstr pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaInstr pe linia: %d\n",q->linie);
			return 0;
		}

	}
	else
		return 0;
}//InstrComp

int ListaInstr()
{
	if (Instr())
	{
		if (ListaInstr1())
			return 1;
		else 
		{
			printf("Eroare la ListaInstr1\n");
			fprintf(rez,"Eroare la ListaInstr1\n");
			return 0;
		}
	}
	else
		return 0;
}//ListaInstr

int ListaInstr1()
{
	if ((q->cod_lexical==4)&&(q->atribut==0))//delimitatorul ';'
	{
		q=getNextAtom(q);
		if (Instr())
		{
			if (ListaInstr1())
				return 1;
			else
				return 0;
		}
		else 
			return 1;//modificat 
	}
	else 
	{
		printf("Eroare, lipseste ; pe linia: %d\n",q->linie);
		fprintf(rez,"Eroare, lipseste ; pe linia: %d\n",q->linie);
		return 0;
	}
}//ListaInstr1


int Instr()
{
	atom *test=NULL;
	if (InstrCase())
		return 1;
	else 
	{
		if (InstrFor())
			return 1;
		else 
		{
			if (InstrRepeat())
				return 1;
			else
			{
				if (InstrIf())
					return 1;
				else
				{
					if (InstrWhile())
						return 1;
					else
					{
						if (InstrPrint())
							return 1;
						else
						{
							if (InstrRead())
								return 1;
							else
							{
								if (InstrComp())
									return 1;
								else
								{
									if (q->cod_lexical==0)//identificator
									{
										test=q->urm;
										if (((test->cod_lexical==3)&&(test->atribut==6))||((test->cod_lexical==4)&&(test->atribut==1)))//operatorul [, delimitatorul .
										{
											if (InstrAtrib())
												return 1;
											else
												return 0;
										}
										else
										{
											if (((test->cod_lexical==3)&&(test->atribut==4)))//operatorul (
											{
												if (ApelProc())
													return 1;
												else
													return 0;
											}
											else//identificator
											{
												if ((test->cod_lexical==3) &&(test->atribut==14))//operatorul :=
												{
													if (InstrAtrib())
														return 1;
													else
														return 0;
												}
												else
												{	
													if (ApelProc())
														return 1;
													else
														return 0;
												}
											}
										}
									}//identificator
									return 0;
								}//else
								return 0;
							}
							return 0;
						}
						return 0;
					}
					return 0;
				}
				return 0;
			}
			return 0;
		}
		return 0;
	}
	return 0;
}//Instr
//cand sunt intr-o functie recursiva..si dau de un atom care nu mai verifica recurvitatea pot iesi fara nici o eroare

int InstrAtrib()
{
	if (Variabila())
	{
		if ((q->cod_lexical==3)&&(q->atribut==14))//operatorul :=
		{
			q=getNextAtom(q);
			if (Expresie())
				return 1;
			else
			{
				printf("Eroare la Expresie pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare la Expresie pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else
		{
			printf("Eroare, lipseste := pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare, lipseste := pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//InstrAtrib


int Variabila()//factorizare
{
	if (q->cod_lexical==0)//identificator
	{
		q=getNextAtom(q);
		if (Variabila1())
			return 1;
		else
		{
			printf("Eroare la Variabila1 pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la Variabila1 pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else
		return 0;
}//Variabila

int Variabila1()
{
	if ((q->cod_lexical==4)&&(q->atribut==1))//delimitatorul '.'
	{
		q=getNextAtom(q);
		if (q->cod_lexical==0)//identificator
		{
			q=getNextAtom(q);
			return 1;
		}
		else 
		{
			printf("Eroare, se asteapta identificator pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare, se asteapta identificator pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else
	{
		if ((q->cod_lexical==3)&&(q->atribut==6))//operatorul [
		{
			q=getNextAtom(q);
			if (Expresie())
			{
				if ((q->cod_lexical==3)&&(q->atribut==7))//operatorul ]
				{
					q=getNextAtom(q);
					return 1;
				}
				else
				{
					printf("Eroare, lipseste ] pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare, lipseste ] pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else 
			{
				printf("Eroare la Expresie pe  linia: %d\n",q->linie);
				fprintf(rez,"Eroare la Expresie pe  linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
			return 1;//ramura cu identificator
	}	
}//Variabila1

int Expresie()
{
	if (Termen())
	{
		if (Expresie1())
			return 1;
		else 
		{
			printf("Eroare la Expresie1 pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la Expresie1 pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else
		return 0;
}//Expresie

int Expresie1()
{
	if ((q->cod_lexical==3)&&((q->atribut==0)||(q->atribut==1)))//operatorii:+ -
	{
		q=getNextAtom(q);
		if (Termen())
		{
			if (Expresie1())
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}
	else 
		return 1;//ramura cu epsilon
}//Expresie1

int Termen()
{
	if (Factor())
	{
		if (Termen1())
			return 1;
		else
		{
			printf("Eroare la Termen1 pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la Termen1 pe linia: %d\n",q->linie);
			return 0; 
		}
	}
	else
		return 0;
}//Termen

int Termen1()
{
	if (((q->cod_lexical==3)&&(q->atribut==2)) || ((q->cod_lexical==3)&&(q->atribut==3)) ||((q->cod_lexical==1)&&(q->atribut==5)) ||((q->cod_lexical==1)&&(q->atribut==14)))// operatorii: * / si cuvintele cheie: div si mod
	{
		q=getNextAtom(q);
		if (Factor())
		{
			if (Termen1())
				return 1;
			else 
				return 0;
		}
		else 
			return 0;
	}
	else 
		return 1;//ramura cu epsilon
}//Termen1

int Factor()
{
		if (Constanta())
			return 1;
		else
		{	
			if ((q->cod_lexical==3)&&(q->atribut==4))//operatorul (
			{
				q=getNextAtom(q);
				if (Expresie())
				{
					if ((q->cod_lexical==3)&&(q->atribut==5))//operatorul )
					{
						q=getNextAtom(q);
						return 1;
					}
					else
					{
						printf("Eroare, se asteapta ) pe linia: %d\n",q->linie);
						fprintf(rez,"Eroare, se asteapta ) pe linia: %d\n",q->linie);
						return 0;
					}
				}
				else
				{
					printf("Eroare la Expresie pe linia: %d \n",q->linie);
					fprintf(rez,"Eroare la Expresie pe linia: %d \n",q->linie);
					return 0;
				}
			}
			else
			{
				if (q->cod_lexical==0)//identificator
				{
					q=getNextAtom(q);
					if ((q->cod_lexical==3)&&(q->atribut==6))//operatorul [
					{
						q=getNextAtom(q);
						if (Expresie())
						{
							if ((q->cod_lexical==3)&&(q->atribut==7))//operatorul ]
							{
								q=getNextAtom(q);
								return 1;
							}
							else
							{
								printf("\nEroare, se asteapta ] pe linia: %d\n",q->linie);
								fprintf(rez,"\nEroare, se asteapta ] pe linia: %d\n",q->linie);
								return 0;
							}
						}
						else 
						{
							printf("Eroare la Expresie pe linia: %d\n",q->linie);
							fprintf(rez,"Eroare la Expresie pe linia: %d\n",q->linie);
							return 0;
						}

					}
					else
					{
						if ((q->cod_lexical==4)&&(q->atribut==1))//delimitatorul .
						{
							q=getNextAtom(q);
							if (q->cod_lexical==0)//identificator
							{
								q=getNextAtom(q);
								return 1;
							}
							else
							{
								printf("Eroare, se asteapta identificator pe linia: %d\n",q->linie);
								fprintf(rez,"Eroare, se asteapta identificator pe linia: %d\n",q->linie);
								return 0;
							}
						}
						else 
						{
							if ((q->cod_lexical==3)&&(q->atribut==4))//operatorul (
							{
								q=getNextAtom(q);
								if (ListaExpresii())
								{
									if ((q->cod_lexical==3)&&(q->atribut==5))//operatorul )
									{
										q=getNextAtom(q);
										return 1;
									}
									else
									{
										printf("Eroare, se asteapta ) pe linia: %d\n",q->linie);
										fprintf(rez,"Eroare, se asteapta ) pe linia: %d\n",q->linie);
										return 0;
									}
								}
								else
								{
									printf("Eroare la ListaExpresii pe linia: %d\n",q->linie);
									fprintf(rez,"Eroare la ListaExpresii pe linia: %d\n",q->linie);
									return 0;
								}
							}
							else 
								return 1;//pentru cazul cand ramura=identificator
						}
						printf("Eroare, se asteapta ] pe linia: %d\n",q->linie);
						fprintf(rez,"Eroare, se asteapta ] pe linia: %d\n",q->linie);
						return 0;
					}
				}
				else
					return 0;
				return 0;
			}
			return 0;
		}
}//Factor

int ListaExpresii()
{
	if (Expresie())
	{
		if (ListaExpresii1())
			return 1;
		else
		{
			printf("Eroare la ListaExpresii1 pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaExpresii1 pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else
		return 0;
}//ListaExpresii1

int ListaExpresii1()
{
	if ((q->cod_lexical==4)&&(q->atribut==2))//delimitatorul ,
	{
		q=getNextAtom(q);
		if (Expresie())
		{
			if (ListaExpresii1())
				return 1;
			else
				return 0;
		}
		else
			return 0;
	}
	else 
		return 1;//ramura cu epsilon
}//ListaExpresii1

int InstrIf()
{
	if ((q->cod_lexical==1)&&(q->atribut==12))//cuvantul cheie if
	{
		q=getNextAtom(q);
		if (Conditie())
		{
			if ((q->cod_lexical==1)&&(q->atribut==22))//cuvantul cheie then
			{
				q=getNextAtom(q);
				if (Instr())
				{
					if (RamuraElse())
						return 1;
					else 
					{
						printf("Eroare la RamuraElse pe linia: %d\n",q->linie);
						fprintf(rez,"Eroare la RamuraElse pe linia: %d\n",q->linie);
						return 0;
					}
				}
				else
				{
					printf("Eroare la Instr pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare la Instr pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else
			{
				printf("Eroare, se asteapta then pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare, se asteapta then pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare la Conditie pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la Conditie pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//InstrIf

int RamuraElse()
{
	if ((q->cod_lexical==1)&&(q->atribut==8))//cuvantul cheie else
	{
		q=getNextAtom(q);
		if (Instr())
			return 1;
		else 
		{
			printf("Eroare la Instr pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la Instr pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 1;//cazul cu epsilon
}//RamuraElse

int Conditie()
{
	if ((q->cod_lexical==1)&&(q->atribut==15))//cuvantul cheie not	
	{
		 q=getNextAtom(q);
		 if (ExprLogica())
		 	 return 1;
		 else
			 return 0;
	}
    else
	{
		if (ExprLogica())
			return 1;
		else 
			return 0;
	}
}//Conditie

int ExprLogica()
{
	if (ExprRel())
	{
		if (ExprLogica1())
			return 1;
		else 
		{
			printf("Eroare la ExprLogica1 la linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ExprLogica1 la linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//ExprLogica

int ExprLogica1()
{
	if ((q->cod_lexical==1)&&((q->atribut==0)||(q->atribut==17)))//cuvintele cheie: and or
	{
		q=getNextAtom(q);
		if (ExprRel())
		{
			if (ExprLogica1())
				return 1;
			else
				return 0;
		}
		else 
			return 0;
	}
	else 
		return 1;//ramura cu epsilon
}//ExprLogica1

int ExprRel()
{
	if ((q->cod_lexical==3)&&(q->atribut==4))//operatorul (
	{
		q=getNextAtom(q);
		if (Conditie())
		{
			if ((q->cod_lexical==3)&&(q->atribut==5))//operatorul )
			{
				q=getNextAtom(q);
				return 1;
			}
			else 
			{
				printf("Eroare, se asteapta ) pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare, se asteapta ) pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
			return 0;
	}
	else
	{
		if (Expresie())
		{
			if ((q->cod_lexical==3) &&(q->atribut>=8)&&(q->atribut<=13))//operatorii:<,<=,>,>=,<>,=
			{
				q=getNextAtom(q);
				if (Expresie())
					return 1;
				else 
				{
					printf("Eroare la Expresie pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare la Expresie pe linia: %d\n",q->linie);
					return 0;
				}
				
			}
			else
				return 0;
		}
		else 
			return 0;
	}
}//ExprRel

int InstrWhile()
{
	if ((q->cod_lexical==1)&&(q->atribut==25))//cuvantul cheie while
	{
		q=getNextAtom(q);
		if (Conditie())
		{
			if ((q->cod_lexical==1)&&(q->atribut==6))//cuvantul cheie do
			{
				q=getNextAtom(q);
				if (Instr())
					return 1;
				else 
				{
					printf("Eroare la Instr pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare la Instr pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else 
			{
				printf("Eroare, se asteapta do pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare, se asteapta do pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare la Conditie pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la Conditie pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//InstrWhile

int InstrRepeat()
{
	if ((q->cod_lexical==1)&&(q->atribut==21))//cuvantul cheie repeat
	{
		q=getNextAtom(q);
		if (Instr())
		{
			if ((q->cod_lexical==1)&&(q->atribut==23))//cuvantul cheie until
			{
				q=getNextAtom(q);
				if (Conditie())
					return 1;
				else 
				{
					printf("Eroare la Conditie pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare la Conditie pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else
			{
				printf("Eroare, se asteapta until pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare, se asteapta until pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare la Instr pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la Instr pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//InstrRepeat

int InstrFor()
{
	if ((q->cod_lexical==1)&&(q->atribut==10))//cuvantul cheie for
	{
		q=getNextAtom(q);
		if (Variabila())
		{
			if ((q->cod_lexical==3)&&(q->atribut==14))//operatorul :=
			{
				q=getNextAtom(q);
				if (Expresie())
				{
					if (Sens())
					{
						if (Expresie())
						{
							if (Pas())
							{
								if ((q->cod_lexical==1)&&(q->atribut==6))//cuvantul cheie do
								{
									q=getNextAtom(q);
									if (Instr())
										return 1;
									else 
									{
										printf("Eroare la Instr pe linia: %d\n",q->linie);
										fprintf(rez,"Eroare la Instr pe linia: %d\n",q->linie);
										return 0;
									}
								}
								else
								{
									printf("Eroare, se asteapta do pe linia: %d\n",q->linie);
									fprintf(rez,"Eroare, se asteapta do pe linia: %d\n",q->linie);
									return 0;
								}
							}
							else 
							{
								printf("Eroare la Pas pe linia: %d\n",q->linie);
								fprintf(rez,"Eroare la Pas pe linia: %d\n",q->linie);
								return 0;
							}
						}
						else 
						{
							printf("Eroare la Expresie pe linia: %d\n",q->linie);
							fprintf(rez,"Eroare la Expresie pe linia: %d\n",q->linie);
							return 0;
						}
					}
					else 
					{
						printf("Eroare la Sens pe linia: %d\n",q->linie);
						fprintf(rez,"Eroare la Sens pe linia: %d\n",q->linie);
						return 0;
					}
				}
				else 
				{
					printf("Eroare la Expresie pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare la Expresie pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else
			{
				printf("Eroare, se asteapta := pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare, se asteapta := pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare la Variabila pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la Variabila pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//InstrFor

int Sens()
{
	if ((q->cod_lexical==1)&&((q->atribut==28)||(q->atribut==7)))//cuvintele cheie downto si to
	{
		q=getNextAtom(q);
		return 1;
	}
	else
	{
		printf("Eroare, se asteapta to sau downto pe linia: %d\n",q->linie);
		fprintf(rez,"Eroare, se asteapta to sau downto pe linia: %d\n",q->linie);
		return 0;
	}
}//Sens

int Pas()
{
	if ((q->cod_lexical==1)&&(q->atribut==31))//cuvantul cheie step
	{
		q=getNextAtom(q);
		if (Expresie())
			return 1;
		else 
		{
			printf("Eroare la Expresie pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la Expresie pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 1;//epsilon
}//Pas

int InstrCase()
{
	if ((q->cod_lexical==1)&&(q->atribut==2))//cuvantul cheie case
	{
		q=getNextAtom(q);
		if (Expresie())
		{
			if ((q->cod_lexical==1)&&(q->atribut==16))//cuvantul cheie of
			{
				q=getNextAtom(q);
				if (ListaAltern())
				{
					if ((q->cod_lexical==1)&&(q->atribut==9))//cuvantul cheie end
					{
						q=getNextAtom(q);
						return 1;
					}
					else 
					{
						printf("Eroare, se asteapta end pe linia: %d\n",q->linie);
						fprintf(rez,"Eroare, se asteapta end pe linia: %d\n",q->linie);
						return 0;
					}
				}
				else 
				{
					printf("Eroare la ListaAltern pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare la ListaAltern pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else
			{
				printf("Eroare, se asteapta of, pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare, se asteapta of, pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare la Expresie pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la Expresie pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//InstrCase

int ListaAltern()
{
	if (ListaRamuri())
	{
		if (ListaAltern1())
			return 1;
		else
		{
			printf("Eroare la ListaAltern1 pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaAltern1 pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//ListaAltern

int ListaAltern1()
{
	if ((q->cod_lexical==1)&&(q->atribut==32))//cuvantul cheie otherwise
	{
		q=getNextAtom(q);
		if ((q->cod_lexical==4)&&(q->atribut==3))//operatorul :
		{
			q=getNextAtom(q);
			if (Instr())
				return 1;
			else 
			{
				printf("Eroare la Instr pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare la Instr pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare, lipseste : pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare, lipseste : pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 1;//ramura cu listaramuri
}//ListaAltern1

int ListaRamuri()
{
	if (Ramura())
	{
		if (ListaRamuri1())
			return 1;
		else 
		{
			printf("Eroare la ListaRamuri1 la linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaRamuri1 la linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//ListaRamuri

int ListaRamuri1()
{
	if ((q->cod_lexical==2)||(q->cod_lexical==6))//constanta numerica sau constanta caracter 
	{
		if (Ramura())
		{
			if (ListaRamuri1())
				return 1;
			else
				return 0;
		}
		else 
			return 0;
	}
	else 
		return 1;//ramura cu epsilon
}//ListaRamuri1

int Ramura()
{
	if (ListaVal())
	{
		if ((q->cod_lexical==4)&&(q->atribut==3))//operatorul :
		{
			q=getNextAtom(q);
			if (Instr())
			{
				if ((q->cod_lexical==4)&&(q->atribut==0))//operatorul ;
				{
					q=getNextAtom(q);
					return 1;
				}
			    else
				{
					printf("Eroare, lipseste ; pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare, lipseste ; pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else 
			{
				printf("Eroare la Instr pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare la Instr pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else
		{
			printf("Eroare, se asteapta : pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare, se asteapta : pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//Ramura

int ListaVal()
{
	if (Constanta())
	{
		if (ListaVal1())
			return 1;
		else 
		{
			printf("Eroare la ListaVal1 la linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaVal1 la linia: %d\n",q->linie);			
			return 0;
		}
	}
	else 
		return 0;
}//ListaVal

int ListaVal1()
{
	if ((q->cod_lexical==4)&&(q->atribut==2))//delimitatorul ,
	{
		q=getNextAtom(q);
		if (Constanta())
		{
			if (ListaVal1())
				return 1;
			else 
				return 0;
		}
		else 
			return 0;
	}
	else 
		return 1;//epsilon
}//ListaVal1

int InstrPrint()
{
	if ((q->cod_lexical==1)&&(q->atribut==29))//cuvantul cheie write
	{
		q=getNextAtom(q);
		if ((q->cod_lexical==3)&&(q->atribut==4))//operatorul (
		{
			q=getNextAtom(q);
			if (ListaElem())
			{
				if ((q->cod_lexical==3)&&(q->atribut==5))//operatorul )
				{
					q=getNextAtom(q);
					return 1;
				}
				else
				{
					printf("Eroare, se asteapta ) pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare, se asteapta ) pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else 
			{
				printf("Eroare la ListaElem pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare la ListaElem pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else
		{
			printf("Eroare, se asteapta ( pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare, se asteapta ( pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//InstrPrint

int ListaElem()
{
	if (Element())
	{
		if (ListaElem1())
			return 1;
		else 
		{
			printf("Eroare la ListaElem1 la linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaElem1 la linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//ListaElem

int ListaElem1()
{
	if ((q->cod_lexical==4)&&(q->atribut==2))//delimitatorul ,
	{
		q=getNextAtom(q);
		if (Element())
		{
			if (ListaElem1())
				return 1;
			else
				return 0;
		}
		else 
			return 0;
	}
	else 
		return 1;//ramura cu epsilon
}//ListaElem1

int Element()
{
	if (q->cod_lexical==5)//constanta sir de caractere
	{
		q=getNextAtom(q);
		return 1;
	}
	else
	{	
		if (Expresie())
			return 1;
		else 
			return 0;
	}
}//Element

int InstrRead()
{
	if ((q->cod_lexical==1)&&(q->atribut==30))//cuvantul cheie read
	{
		q=getNextAtom(q);
		if ((q->cod_lexical==3)&&(q->atribut==4))//operatorul (
		{
			q=getNextAtom(q);
			if (ListaVariab())
			{
				if ((q->cod_lexical==3)&&(q->atribut==5))//operatorul )
				{
					q=getNextAtom(q);
					return 1;
				}
				else
				{
					printf("Eroare, se asteapta ) pe linia: %d\n",q->linie);
					fprintf(rez,"Eroare, se asteapta ) pe linia: %d\n",q->linie);
					return 0;
				}
			}
			else 
			{
				printf("Eroare la ListaVariab la linia: %d\n",q->linie);
				fprintf(rez,"Eroare la ListaVariab la linia: %d\n",q->linie);
				return 0;
			}
		}
		else
		{
			printf("Eroare, se asteapta ( pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare, se asteapta ( pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//InstrRead

int ListaVariab()
{
	if (Variabila())
	{
		if (ListaVariab1())
			return 1;
		else 
		{
			printf("Eroare la ListaVariab1 la linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaVariab1 la linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 0;
}//ListaVariab

int ListaVariab1()
{
	if ((q->cod_lexical==4)&&(q->atribut==2))//delimitatorul ,
	{
		q=getNextAtom(q);
		if (Variabila())
		{
			if (ListaVariab1())
				return 1;
			else 
				return 0;
		}
		else 
			return 0;
	}
	else 
		return 1;//ramura epsilon
}//ListaVariab1

int ApelProc()
{
	if (q->cod_lexical==0)//identificator
	{
		q=getNextAtom(q);
		if (ApelProc1())
			return 1;
		else
		{
			printf("Eroare la ApelProc1 pe linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ApelProc1 pe linia: %d\n",q->linie);
			return 0;
		}
	}
	else
		return 0;
}//ApelProc

int ApelProc1()
{
	if ((q->cod_lexical==3)&&(q->atribut==4))//operatorul (
	{
		q=getNextAtom(q);
		if (ListaExpresii())
		{
			if ((q->cod_lexical==3)&&(q->atribut==5))//operatorul )
			{
				q=getNextAtom(q);
				return 1;
			}
			else
			{
				printf("Eroare, lipseste ) pe linia: %d\n",q->linie);
				fprintf(rez,"Eroare, lipseste ) pe linia: %d\n",q->linie);
				return 0;
			}
		}
		else 
		{
			printf("Eroare la ListaExpresii pe  linia: %d\n",q->linie);
			fprintf(rez,"Eroare la ListaExpresii pe  linia: %d\n",q->linie);
			return 0;
		}
	}
	else 
		return 1;//ramura cu identificator
}//ApelProc1

void main()
{
    int i;

	rez=fopen("rezultat.txt","w");
	rezalex=fopen("rezultatalex.txt","w");
	first=alex(first);
	
	printf("\nLISTA DE ATOMI:\n");
	fprintf(rezalex,"\nLISTA DE ATOMI:\n");
	q=first;
	while (q!=NULL)
	{
		printf("%s %d %d %d \n",q->nume,q->cod_lexical,q->atribut,q->linie);
		fprintf(rezalex,"%s %d %d %d \n",q->nume,q->cod_lexical,q->atribut,q->linie);
		q=q->urm;
	}

	printf("\nTabela de constante\n");
	fprintf(rezalex,"\nTabela de constante\n");
    for (i=0;i<n;i++)
	{
		printf(" %d %f\n",tab[i].tip,tab[i].valoare);
		fprintf(rezalex," %d %f\n",tab[i].tip,tab[i].valoare);
	}

    q=first;
    if (ProgramSursa())
    	 printf("\nProgramul este corect sintactic\n");
	else 
		 printf("\nEroare la Analiza Sintactica\n");
	fclose(rez);
	getch();
}//main

