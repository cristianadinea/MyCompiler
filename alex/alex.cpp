#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct ATOM
{
	char *nume;
	int cod_lexical;
	int atribut;
	int linie;
	struct ATOM *urm;
}atom;

typedef struct TAB_CONST
{
	int tip;
	double valoare;
}tab_const;

typedef struct TAB_CHEIE
{
	char *cheie;
}tab_cheie;

int stare,linie,n=0;//n va tine minte prima pozitie libera din tabela de const
atom *first=NULL,*last=NULL;
tab_const tab[100];

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
	f=fopen("sursa.pas","r");
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
								if(((buffer[i]<48)||(buffer[i]>57-(10-baza_int))))
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

void main()
{
	atom *q=NULL;
	int i;
	first=alex(first);
	q=first;
	printf("Fisierul sursa contine %d linii.\n",linie);
	printf("Lista de atomi:\n");
	while (q!=NULL)
	{
		printf("  %s %d %d %d \n",q->nume,q->cod_lexical,q->atribut,q->linie);
		q=q->urm;
	}
	printf("Tabela de constante: \n");
	for(i=0;i<n;i++)
		printf("  %d %d %4.4lf \n",i+1,tab[i].tip,tab[i].valoare);
	
	getch();
}