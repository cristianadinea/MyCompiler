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
tab_simb tabela[100];
int nr=0; //nr tine minte prima pozitie libera din tabela de simboluri
int iui=0,ad=1,oper=4;//initializez prima operatie pe adunare pt a aduna cu 0
tab_const aux;
int nivel=1;//nivelul curent
int sursa=0;//pt tip simplu
int rec=0;//imi spune daca sunt in record(1) sau nu(0)
int valadr=-1;//par prin val(1)sau adresa(0)
int pr=1,pr1=1;//pt lista id,resp record
int nrparam=0,x=0;
int h=0,ret=0,kk=0;//pt record
int lim1=-1,lim2=-1;//limitele pt tablou
lista_par *par=NULL,*first1=NULL,*last1=NULL;
lista_rec *reco=NULL,*first2=NULL,*last2=NULL;
int error=0;

typedef struct Lista
{
	double v;//val
	int t;//tip pt nr(1/2/3)
	int op;//operator(0/1)
	struct Lista *urm;
	struct Lista *ant;
}lista; 
lista *list=NULL,*ult=NULL;

void add_lista(double v,int t,int o)
{
lista *p=NULL;
p=new lista; 
p->v=v;
p->t=t;
p->op=o;
p->urm=NULL;
p->ant=NULL;
if(list==NULL)
{
	list=p;
	ult=p;
}
else
{
	ult->urm=p;
	p->ant=ult;
	ult=p;
}
}

lista* calc2(double val1,int tip1,double val2,int tip2,int ope)
{
lista *p;
p=new lista;
p->urm=NULL;
p->ant=NULL;
p->op=0;

if((tip1==3)||(tip2==3))
	{
	printf("Eroare!Un char nu poate fii implicat intr-o operatie!\n");
	return NULL;
	}

if(ope==0)//inmultire
	{
	if((tip1==1)&&(tip2==1))
		p->t=1;
	else
		p->t=2;
	p->v=val1*val2;
	}
if(ope==1)//impartire
	{
	if((tip1==1)&&(tip2==1))
		if(((int)val1 % (int)val2)==0)
			p->t=1;
		else
			p->t=2;
	p->v=val1/val2;
	}
if(ope==2)//div
	{
	if((tip1!=1)||(tip2!=1))
		{
		printf("Eroare!Tipul operanzilor trebuie sa fie intreg pt a putea realiza div!\n");
		return 0;
		}
	p->t=1;
	p->v=(double)((int)(val1/val2));
	}
if(ope==3)//mod
	{
	if((tip1!=1)||(tip2!=1))
		{
		printf("Eroare!Tipul operanzilor trebuie sa fie intreg pt a putea realiza mod!\n");
		return 0;
		}
	p->t=1;
	p->v=(double)((int)val1%(int)val2);
	}
if(ope==4)//adunare
	{
	if((tip1==1)&&(tip2==1))
		p->t=1;
	else
		p->t=2;
	p->v=val1+val2;
	}
if(ope==5)//scadere
	{
	if((tip1==1)&&(tip2==1))
		p->t=1;
	else
		p->t=2;
	p->v=val1-val2;
	}
return p;
}

void afis_lista(lista* l)
{
lista *p;
p=l;
while(p!=NULL)
{
	if(p->op==0)
		if((p->t==0)&&(p->v==1))
			printf("(");
		else if((p->t==0)&&(p->v==2))
			printf(")");
		else 
			printf(" %lf ",p->v);
	else
	{
		if(p->v==4)
			printf(" + ");
		if(p->v==5)
			printf(" - ");
		if(p->v==0)
			printf(" * ");
		if(p->v==1)
			printf(" / ");
		if(p->v==2)
			printf(" div ");
		if(p->v==3)
			printf(" mod ");
				
	}
	p=p->urm;
}
printf("\n");
}

lista* calcul(lista *listt)
{
lista *p,*q,*t,*scos,*auxi;
double v1,v2;
int t1,t2,op;

//adaug 0+ ... +0
p=new lista;
p->ant=NULL;
p->op=0;
p->t=1;
p->urm=NULL;
p->v=0;

q=new lista;
q->ant=NULL;
q->op=1;
q->t=0;
q->urm=NULL;
q->v=4;

q->urm=listt;
listt->ant=q;
p->urm=q;
q->ant=p;
listt=p;

p=new lista;
p->ant=NULL;
p->op=0;
p->t=1;
p->urm=NULL;
p->v=0;

q=new lista;
q->ant=NULL;
q->op=1;
q->t=0;
q->urm=NULL;
q->v=4;

t=listt;
while(t->urm!=NULL)
	t=t->urm;
t->urm=q;
q->ant=t;
q->urm=p;
p->ant=q;


//ma ocup de operatiile prioritare: * / mod div
p=listt->urm;
q=listt;
while(p!=NULL)
{
	if (p->op==1)
		if(p->v<=3)
		{
			scos=q;
			t=scos->ant;
			q=q->urm;
			q=q->urm;
			q=q->urm;
			t->urm=q;
			q->ant=t;
			//trimit spre calc ce am scos din lista
			auxi=scos;
			v1=auxi->v;
			t1=auxi->t; 
			auxi=auxi->urm;
			op=(int)auxi->v;
			auxi=auxi->urm;
			v2=auxi->v;
			t2=auxi->t;
			scos=NULL;
			scos=calc2(v1,t1,v2,t2,op);
			//adaug la lista rezultatul
			t->urm=scos;
			scos->ant=t;
			q->ant=scos;
			scos->urm=q;
			p=scos;
		}
	q=p;
	p=p->urm;
	}
//am terminat cu operatorii prioritari
//mai am doar adunari/scaderi in lista
p=listt->urm;
q=listt;
while(p!=NULL)
{
	if((p->urm)->urm==NULL)
	{
		scos=q;
		q=q->urm;
		q=q->urm;
		q=q->urm;
		
		auxi=scos;
		v1=auxi->v;
		t1=auxi->t; 
		auxi=auxi->urm;
		op=(int)auxi->v;
		auxi=auxi->urm;
		v2=auxi->v;
		t2=auxi->t;
		scos=NULL;
		scos=calc2(v1,t1,v2,t2,op);

		listt=scos;
		p=NULL;
	}
	else
	{
		scos=q;
		q=q->urm;
		q=q->urm;
		q=q->urm;
		
		auxi=scos;
		v1=auxi->v;
		t1=auxi->t; 
		auxi=auxi->urm;
		op=(int)auxi->v;
		auxi=auxi->urm;
		v2=auxi->v;
		t2=auxi->t;
		scos=NULL;
		scos=calc2(v1,t1,v2,t2,op);
		//adaug la lista rezultatul
		q->ant=scos;
		scos->urm=q;
		listt=scos;
		p=scos;

		q=p;
		p=p->urm;
	}
}
//acum in lista mai am un singur nod,rezultatul calculului
return listt;
}


void calc_lista()
{
lista *p,*q,*t,*scos,*auxi,*auxi2,*paran;
double v1,v2;
int t1,t2,op,flag=0,pd=0,pi=0,i,c;
double x;


p=new lista;
p->ant=NULL;
p->op=0;
p->t=1;
p->urm=NULL;
p->v=0;

q=new lista;
q->ant=NULL;
q->op=1;
q->t=0;
q->urm=NULL;
q->v=4;

q->urm=list;
list->ant=q;
p->urm=q;
q->ant=p;
list=p;

p=new lista;
p->ant=NULL;
p->op=0;
p->t=1;
p->urm=NULL;
p->v=0;

q=new lista;
q->ant=NULL;
q->op=1;
q->t=0;
q->urm=NULL;
q->v=4;

t=list;
while(t->urm!=NULL)
	t=t->urm;
t->urm=q;
q->ant=t;
q->urm=p;
p->ant=q;


//ma ocup de paranteze
p=list;
while(p!=NULL)
{
	//numar parantezele deschise pd si inchise pi
	if((p->op==0)&&(p->t==0)&&(p->v==1))
		pd++;
	if((p->op==0)&&(p->t==0)&&(p->v==2))
		pi++;
	p=p->urm;
}

q=NULL;
t=NULL;

for(i=0;i<pd;i++)//pentru cate paranteze am parcurg lista si inlocuiesc paranteza cu rezultatul calculului
{
p=list;
while(p!=NULL)
{
	if((p->op==0)&&(p->t==0)&&(p->v==2))
	{
		t=p;
		p=NULL;
	}
	else
		p=p->urm;
}
//caut prima ( mergand inapoi de la t; ( ramane in q
q=t;
auxi=NULL;
while (q!=NULL)
{
	if ((q->op==0)&&(q->t==0)&&(q->v==1))
	{
		auxi=q;
		q=NULL;
	}
	else
		q=q->ant;
}
q=auxi;

//creez lista pt continutul parantezei 
paran=new lista;

paran=q->urm;
paran->ant=NULL;

auxi=paran->urm;
auxi2=paran;

while(auxi!=NULL)
{
	if((auxi->op==0)&&(auxi->t==0)&&(auxi->v==2))
	{
		auxi=NULL;
	}
	else
	{
		auxi2=auxi;
		auxi=auxi->urm;
	}
}
auxi2->urm=NULL;
//calculez continutul parantezei
paran=calcul(paran);

//inlocuiesc paranteza din lista cu rezultatul
q=q->ant;
t=t->urm;
paran->ant=q;
q->urm=paran;
t->ant=paran;
paran->urm=t;
}
//am terminat cu parantezele
//calculez ce mi-a ramas in lista(doar operatii)
list=calcul(list);

aux.tip=list->t;
aux.valoare=list->v;
//daca rezultatul este un intreg ii setez tipul pe 1
if(aux.tip==2)
	{
	x=aux.valoare;
	c=(int)aux.valoare;
	if((double)(c)==x)
		aux.tip=1;
	}
}


void afisare()
{
	int i;
	printf(" Nr nume cls niv tip val adrel deplrec imin imax nrpar dimvar listarec listapar \n");
	for(i=0;i<nr;i++)
	{
		printf(" %2d %2s %29s %d %2d %2d %3d %2d %2d %2d %2d %2d ",tabela[i].nr,tabela[i].nume,tabela[i].clasa,tabela[i].nivel,tabela[i].tip,tabela[i].val,tabela[i].adrel,tabela[i].deplrec,tabela[i].imin,tabela[i].imax,tabela[i].nrpar,tabela[i].dimvar);
		if (tabela[i].listarec==NULL)
			printf(" NULL ");
		else
		{
			reco=tabela[i].listarec;
			while(reco!=NULL)
			{
				if(reco->urm==NULL)
					printf("%d",reco->poz);
				else
					printf("%d,",reco->poz);
				reco=reco->urm;
			}
		}
		if (tabela[i].listapar==NULL)
			printf(" NULL ");
		else
		{
			par=tabela[i].listapar;
			while(par!=NULL)
			{
				if (par->next==NULL)
					printf("%d,%d",par->tip1,par->tip2);
				else
					printf("%d,%d;",par->tip1,par->tip2);
				par=par->next;
			}
		}
		printf("\n");
	}
	printf("\n\n");
}

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
	f=fopen("tab-test.pas","r");
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

int verificare(char *nume,int nivel)
{
	int i;
	//verific ca nu mai exista in tabela niciun simbol cu acelasi nume
	for(i=0;i<nr;i++) 
		if((tabela[i].nivel==nivel)&&(strcmp(tabela[i].nume,nume)==0))
			if((tabela[i].clasa==NULL)||(strcmp(tabela[i].clasa,"nume camp structura")!=0))
			{
				printf("eroare!simbolul %s este deja in tabela pe nivelul curent!\n",q->nume);
				error=1;
				return 0;
			}
			//else
				return 1;

}

int verificare_rec(char *nume,int num)
{
	int i;
	int nnn=nr-h;
	//verificare in cadrul unui record
	for(i=nnn;i<nnn+h;i++)
		if(strcmp(tabela[i].nume,nume)==0)
		{
			printf("eroare!sunt 2 var cu acelasi nume(%s) in record\n",q->nume);
			return 0;
		}
	return 1;
}

int ProgramSursa()
{
	if ((q->cod_lexical==1)&&(q->atribut==19))//cuvantul cheie: program
	{
		q=getNextAtom(q);
		if (q->cod_lexical==0)//identificator
		{
			//setez nivelul curent pe 1
			nivel=1;
			//adaug in tabela de simboluri
			tabela[nr].nr=nr;
			tabela[nr].nume=(char*)malloc(sizeof(q->nume)+1);
			tabela[nr].nume=q->nume;
			tabela[nr].clasa=(char*)malloc(sizeof("nume program")+1);
			tabela[nr].clasa="nume program";
			tabela[nr].nivel=nivel;
			tabela[nr].tip=-1;
			tabela[nr].val=-1;
			tabela[nr].adrel=-1; 
			tabela[nr].deplrec=-1;
			tabela[nr].imin=-1;
			tabela[nr].imax=-1;
			tabela[nr].nrpar=-1;
			tabela[nr].dimvar=-1;
			tabela[nr].listarec=NULL;
			tabela[nr].listapar=NULL;
			nr++;

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
		if (ad==1) 
			{
			//completez campul adrel al primei intrari libere din tab de simboluri cu 0
			tabela[nr].adrel=0;
			ad++;
			}
		else
			if(tabela[nr-1].adrel==-2)
				tabela[nr].adrel=tabela[nr-1].adrel+2;
			else
				tabela[nr].adrel=tabela[nr-1].adrel+1;

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
	{
		if (error!=0)
			return 0;
		else
			return 1;//ramura cu epsilon
	}
}//ListaDeclVar1

int SectiuneDeclSubprog()
{
	int i,j;//am ajuns la partea de decl subprog,deci cunosc dimvar pt prog principal/proc/functie
	i=nr-1;
	j=tabela[nr].adrel;
	while((strcmp(tabela[i].clasa,"nume program")!=0)&&(strcmp(tabela[i].clasa,"nume functie")!=0)&&(strcmp(tabela[i].clasa,"nume procedura")!=0))
		i--;
	if(tabela[i].nrpar!=-1)
		if (j>tabela[i].nrpar)
			tabela[i].dimvar=j-tabela[i].nrpar;
		else
			tabela[i].dimvar=j;
	else
		tabela[i].dimvar=j;
	//if(tabela[i].nrpar==-1)
	//	tabela[i].nrpar=0;
	
	//am completat dimvar 

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
	int i;
	if (q->cod_lexical==0)//identificator
	{
		//verific ca nu mai exista in tabela niciun simbol cu acelasi nume
		if (verificare(q->nume,nivel))
		{
			//adaug in tabela
			tabela[nr].nr=nr;
			tabela[nr].nume=(char*)malloc(sizeof(q->nume)+1);
			tabela[nr].nume=q->nume;
			tabela[nr].clasa=(char*)malloc(sizeof("nume constanta")+1);
			tabela[nr].clasa="nume constanta";
			tabela[nr].nivel=nivel;
			tabela[nr].tip=-1;
			tabela[nr].val=-1;
			tabela[nr].adrel=-1; 
			tabela[nr].deplrec=-1;
			tabela[nr].imin=-1;
			tabela[nr].imax=-1;
			tabela[nr].nrpar=-1;
			tabela[nr].dimvar=-1;
			tabela[nr].listarec=NULL;
			tabela[nr].listapar=NULL;
			nr++;
		}
		else
			return 0;
		q=getNextAtom(q);
		if ((q->cod_lexical==3)&&(q->atribut==13))//operatorul =
		{
			aux.valoare=0;
			q=getNextAtom(q);

			if (ExpresieStatica()) 
			{
				if ((q->cod_lexical==4)&&(q->atribut==0))//delimitatorul ;
				{
					calc_lista();
					list=NULL;
					ult=NULL;
					
					if(aux.tip==1)
						aux.valoare=(int)(aux.valoare);
					tab[n].valoare=aux.valoare;
					tab[n].tip=aux.tip;
					n++;

					i=nr-1;
					while(strcmp(tabela[i].clasa,"nume constanta")!=0)
						i--;

					tabela[i].tip=aux.tip;
					tabela[i].val=n-1;

					aux.valoare=0;//reinitializez
					aux.tip=1;
					oper=4;

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
	valadr=-1;
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
			{	error=1;
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
		if(rec==0)//nu sunt in record
		{
			if ((valadr==1)||(valadr==0))
			{
				//verific ca nu mai exista in tabela niciun simbol cu acelasi nume
				if (verificare(q->nume,nivel))
				{	
					//adaug in tabela de simboluri 		
					tabela[nr].nr=nr;
					tabela[nr].nume=(char*)malloc(sizeof(q->nume)+1);
					tabela[nr].nume=q->nume;
					tabela[nr].nivel=nivel;
					//tabela[nr].tip=-1;
					tabela[nr].val=-1;
					tabela[nr].deplrec=-1;
					tabela[nr].imin=-1;
					tabela[nr].imax=-1;
					tabela[nr].nrpar=-1;
					tabela[nr].dimvar=-1;
					tabela[nr].listarec=NULL;
					tabela[nr].listapar=NULL;
					if(valadr==1)//prin valoare
					{
						tabela[nr].clasa=(char*)malloc(sizeof("nume parametru formal valoare")+1);
						tabela[nr].clasa="nume parametru formal valoare";
					}
					else
					{
						tabela[nr].clasa=(char*)malloc(sizeof("nume parametru formal adresa")+1);
						tabela[nr].clasa="nume parametru formal adresa";
					}
				    if(tabela[nr-1].adrel==-2)
						tabela[nr].adrel=tabela[nr-1].adrel+2;
					else
						tabela[nr].adrel=tabela[nr-1].adrel+1;
					nr++;
					nrparam++;
				}
				else//verificare
					return 0;
			}
			else//var simple (fara val sau adr)
			{
				if (verificare(q->nume,nivel))
				{
					//adaug in tabela de simboluri 		
					tabela[nr].nr=nr;
					tabela[nr].nume=(char*)malloc(sizeof(q->nume)+1);
					tabela[nr].nume=q->nume;
					tabela[nr].nivel=nivel;
				//	tabela[nr].tip=-1;
					tabela[nr].val=-1;
					tabela[nr].deplrec=-1;
					tabela[nr].imin=-1;
					tabela[nr].imax=-1;
					tabela[nr].nrpar=-1;
					tabela[nr].dimvar=-1;
					tabela[nr].listarec=NULL;
					tabela[nr].listapar=NULL;
					nr++;
					
				}
				else
					return 0;
			}
		}
		else //sunt in record
		{
			if (verificare_rec(q->nume,kk+1))
				{	
					//adaug in tabela de simboluri 		
					tabela[nr].nr=nr;
					tabela[nr].nume=(char*)malloc(sizeof(q->nume)+1);
					tabela[nr].nume=q->nume;
					tabela[nr].clasa=(char*)malloc(sizeof("nume camp structura")+1);
					tabela[nr].clasa="nume camp structura";
					tabela[nr].nivel=nivel;
					//tabela[nr].tip=-1;
					tabela[nr].val=-1;
					tabela[nr].adrel=-1;
					tabela[nr].deplrec=tabela[nr-1].deplrec+1;
					tabela[nr].imin=-1;
					tabela[nr].imax=-1;
					tabela[nr].nrpar=-1;
					tabela[nr].dimvar=-1;
					tabela[nr].listarec=NULL;
					tabela[nr].listapar=NULL;
					nr++;
					h++;
				}
				else
					return 0;
		}
			
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
	//int i;
	if ((q->cod_lexical==4)&&(q->atribut==2))//delimitatorul : ','
	{
		q=getNextAtom(q);
		if (q->cod_lexical==0)//identificator
		{
		if(rec==0)//nu sunt in record
		{
			if ((valadr==1)||(valadr==0))
			{
				//verific ca nu mai exista in tabela niciun simbol cu acelasi nume
				if (verificare(q->nume,nivel))
				{	
					//adaug in tabela de simboluri 		
					tabela[nr].nr=nr;
					tabela[nr].nume=(char*)malloc(sizeof(q->nume)+1);
					tabela[nr].nume=q->nume;
					tabela[nr].nivel=nivel;
					//tabela[nr].tip=-1;
					tabela[nr].val=-1;
					tabela[nr].deplrec=-1;
					tabela[nr].imin=-1;
					tabela[nr].imax=-1;
					tabela[nr].nrpar=-1;
					tabela[nr].dimvar=-1;
					tabela[nr].listarec=NULL;
					tabela[nr].listapar=NULL;
					if(valadr==1)//prin valoare
					{
						tabela[nr].clasa=(char*)malloc(sizeof("nume parametru formal valoare")+1);
						tabela[nr].clasa="nume parametru formal valoare";
					}
					else
					{
						tabela[nr].clasa=(char*)malloc(sizeof("nume parametru formal adresa")+1);
						tabela[nr].clasa="nume parametru formal adresa";
					}
					tabela[nr].adrel=tabela[nr-1].adrel+1;
					nr++;
					nrparam++;
				}
				else//verificare
					return 0;
			}
			else//var simple (fara val sau adr)
			{
				if (verificare(q->nume,nivel))
				{
					//adaug in tabela de simboluri 		
					tabela[nr].nr=nr;
					tabela[nr].nume=(char*)malloc(sizeof(q->nume)+1);
					tabela[nr].nume=q->nume;
					tabela[nr].nivel=nivel;
					//tabela[nr].tip=-1;
					tabela[nr].val=-1;
					tabela[nr].deplrec=-1;
					tabela[nr].imin=-1;
					tabela[nr].imax=-1;
					tabela[nr].nrpar=-1;
					tabela[nr].dimvar=-1;
					tabela[nr].listarec=NULL;
					tabela[nr].listapar=NULL;
					nr++;
				
				}
				else
					return 0;
			}
		}
		else //sunt in record
		{
			if (verificare_rec(q->nume,kk+1))
				{	
					//adaug in tabela de simboluri 	
					tabela[nr].nr=nr;
					tabela[nr].nume=(char*)malloc(sizeof(q->nume)+1);
					tabela[nr].nume=q->nume;
					tabela[nr].clasa=(char*)malloc(sizeof("nume camp structura")+1);
					tabela[nr].clasa="nume camp structura";
					tabela[nr].nivel=nivel;
					//tabela[nr].tip=-1;
					tabela[nr].val=-1;
					tabela[nr].adrel=-1;
					tabela[nr].deplrec=tabela[nr-1].deplrec+1;
					tabela[nr].imin=-1;
					tabela[nr].imax=-1;
					tabela[nr].nrpar=-1;
					tabela[nr].dimvar=-1;
					tabela[nr].listarec=NULL;
					tabela[nr].listapar=NULL;
					nr++;
					h++;
				}
				else
					return 0;
		}

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
	sursa=1;
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
	//in tip simplu pot ajunge din mai multe functii (tip(1),tiptablou(2),declsimpla(3/5),declarfunctie(4))
	//folosesc o var globala sursa care sa-mi spuna de unde vin
	int tip,i;
	if ((q->cod_lexical==1)&& (((q->atribut==13)||(q->atribut==20)||(q->atribut==3))))//integer,real,char
	{
		if (q->atribut==13)
			tip=1;
		if (q->atribut==20)
			tip=2;
		if (q->atribut==3)
			tip=3;
		i=nr-1;
		if(sursa==1)
		{
			//vin din tip
			while((tabela[i].tip==0)&&(tabela[i].clasa==NULL))
				i--;
			i++;
		
			while(i<nr)
			{
				tabela[i].clasa=(char*)malloc(sizeof("nume variabila simpla")+1);
				tabela[i].clasa="nume variabila simpla";
				tabela[i].tip=tip;
				tabela[i].deplrec=-1;
				tabela[i+1].adrel=tabela[i].adrel+1;
				i++;
			}
		}

		if(sursa==2)
		{
			//vin din tip tablou
			while(tabela[i].tip==0)
				i--;
			i++;
			while(i<nr)
			{
				tabela[i].clasa=(char*)malloc(sizeof("nume variabila tablou")+1);
				tabela[i].clasa="nume variabila tablou";
				tabela[i].tip=tip;
				tabela[i].imin=lim1;
				tabela[i].imax=lim2;
				tabela[i+1].adrel=tabela[i].adrel+(lim2-lim1)+1;
				i++;
			}
			lim1=0;
			lim2=0;
		}

		if(sursa==3)
		{
			//vin din decl simpla cu val/adr
			while(tabela[i].tip==0)
				i--;
			i++;
			
			while(i<nr)
			{
				tabela[i].tip=tip;
				par=new lista_par;
				par->tip1=tip;
				if(valadr==1)
					par->tip2=1;
				else
					par->tip2=2;
				par->next=NULL;
				if(first1==NULL)
				{
					first1=par;
					last1=first1;
				}
				else
				{
					last1->next=par;
					last1=par;
				}
				i++;
			}

		}
		
		if(sursa==4)
		{
			//vin din functie
			while(tabela[i].tip!=-2)
				i--;
			tabela[i].tip=tip;
		}

		if(sursa==5)
		{
			//vin din declsimpla fara val/adr
			while((tabela[i].tip==0)&&(strcmp(tabela[i].clasa,"nume camp structura")==0))
			{
				tabela[i].tip=tip;
				i--;
			}
		}

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
		oper=4;//initializari pt calcul
		aux.valoare=0;
		aux.tip=1;

		q=getNextAtom(q);
		if ((q->cod_lexical==3)&&(q->atribut==6))//operatorul [
		{
			q=getNextAtom(q);
			if (ExpresieStatica())
			{
				calc_lista();
				list=NULL;
				ult=NULL;
				
				if (aux.tip!=1)
				{
					printf("Eroare,limita trebuie sa fie intreaga!\n");
					error=1; 
					return 0;
				}
				else
				{
					lim1=(int)aux.valoare;
					oper=4;
					aux.valoare=0;
					aux.tip=1;
				}
				if ((q->cod_lexical==4)&&(q->atribut==4))//delimitatorul ..
				{
					q=getNextAtom(q);
					if (ExpresieStatica())
					{
						calc_lista();
						list=NULL;
						ult=NULL;

						if (aux.tip!=1)
						{
							printf("Eroare,limita trebuie sa fie intreaga!\n");
							return 0;
						}
						else
						{
							lim2=(int)aux.valoare;
							if((lim2-lim1)<0)
							{
								printf("Limite incorecte!Lim1:%d Lim2:%d\n",lim1,lim2);
								return 0;
							}
							else if((lim1<0)||(lim2<0))
							{
								printf("Eroare!Limita negativa!Lim1:%d Lim2:%d\n",lim1,lim2
									);
								return 0;
							}
							else
							{
								oper=4;
								aux.valoare=0;
								aux.tip=1;
							}
						}
						if ((q->cod_lexical==3)&&(q->atribut==7))//operatorul ]
						{
							q=getNextAtom(q);
							if ((q->cod_lexical==1)&&(q->atribut==16))//cuvantul cheie of
							{
								q=getNextAtom(q);
								sursa=2;
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
	int i,k=0,k1=0;
	if ((q->cod_lexical==1)&&(q->atribut==27))//cuvantul cheie record
	{
		rec=1;//decl record
		i=nr-1;
		kk=i;
		
		tabela[i+1].deplrec=0;
		while(tabela[i].tip==0)
		{
			tabela[i].clasa=(char*)malloc(sizeof("nume variabila structura")+1);
			tabela[i].clasa="nume variabila structura";
			tabela[i].tip=-1;
			i--;
		}

		q=getNextAtom(q);
		if (ListaCamp())
		{
			if ((q->cod_lexical==1)&&(q->atribut==9))//cuvantul cheie end
			{
				h=0;
				i=nr-1;
				while(strcmp(tabela[i].clasa,"nume camp structura")==0)
				{
					tabela[i].listarec=NULL;
					i--;
					k++;
				}
				while(strcmp(tabela[i].clasa,"nume variabila structura")==0)
				{
					i--;
					k1++;
				}
				i++;
				k1=i+k1;
				while(i<k1)//parcurgere (var struct) in sens direct
				{
					reco=new lista_rec;
					reco->poz=tabela[i].nr;
					reco->urm=NULL;
					if(first2==NULL)
					{
						first2=reco;
						last2=first2;
					}
					else
					{
						last2->urm=reco;
						last2=reco;
					}
					tabela[i+1].adrel=tabela[i].adrel+k;
					i++;
				}
				tabela[i+k].adrel=tabela[i].adrel;
				tabela[i].adrel=-1;
				while(i<nr)
				{
					tabela[i].listarec=first2;
					i++;
				}
				rec=0;
				first2=NULL;
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
	valadr=-1;
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
		valadr=-1;
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

			if (valadr==-1)
			{
				sursa=5;
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
				sursa=3;
				if (TipSimplu())
					return 1;
				else
				{
					printf("Eroare la TipSimplu, pe linia: %d \n",q->linie);
					fprintf(rez,"Eroare la TipSimplu, pe linia: %d \n",q->linie);
					return 0;
				}
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
		valadr=1;
		if (AntetSubprog())
		{
			if ((q->cod_lexical==4)&&(q->atribut==3))//delimitatorul :
			{
				q=getNextAtom(q);
				sursa=4;
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
		valadr=0;
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
	//int i;
	if (q->cod_lexical==0)//identificator
	{
		//adaug in tabela de simboluri
		tabela[nr].nr=nr;
		tabela[nr].nume=(char*)malloc(sizeof(q->nume)+1);
		tabela[nr].nume=q->nume;
		tabela[nr].nivel=nivel;
		tabela[nr].val=-1;
		tabela[nr].deplrec=-1;
		tabela[nr].imin=-1;
		tabela[nr].imax=-1;
		tabela[nr].nrpar=-1;
		tabela[nr].dimvar=-1;
		tabela[nr].listarec=NULL;
		tabela[nr].listapar=NULL;
		nivel++;
		
		if(valadr==1)
		{
			tabela[nr].adrel=-2;
			tabela[nr].clasa=(char*)malloc(sizeof("nume functie")+1);
			tabela[nr].clasa="nume functie";
			tabela[nr].tip=-2;
		}
		else
		{
			tabela[nr].adrel=-1;
			tabela[nr].clasa=(char*)malloc(sizeof("nume procedura")+1);
			tabela[nr].clasa="nume procedura";
			tabela[nr].tip=-2;
		}
		nr++;

		getch();
		printf("Tabela de simboluri la intrarea in functie/procedura\n");
		afisare();
		getch();
		tabela[nr].adrel=0;

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
	int i;
	if ((q->cod_lexical==3)&&(q->atribut==4))//operatorul (
	{
		q=getNextAtom(q);
		if (ListaParamForm())
		{
			if ((q->cod_lexical==3)&&(q->atribut==5))//operatorul )
			{
				i=nr-1;
				while(tabela[i].tip!=-2)
					i--;
				tabela[i].nrpar=nrparam;
				nrparam=0;
				tabela[i].listapar=first1;
				first1=NULL;

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
		valadr=0;
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
		valadr=1;
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
		if(q->atribut==0)
		{
			oper=4;//+
			add_lista(oper,0,1);
		}
		else
		{
			oper=5;//-
			add_lista(oper,0,1);
		}
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
		if (q->cod_lexical==3)
			if (q->atribut==2)
			{
				oper=0;//*
				add_lista(oper,0,1);
			}
			else
			{
				oper=1;// /
				add_lista(oper,0,1);
			}
		else
			if (q->atribut==5)
			{
				oper=2;//div
				add_lista(oper,0,1);
			}
			else
			{
				oper=3;//mod
				add_lista(oper,0,1);
			}

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

/*int calculeaza(double val,int tip,int ope)
{
	double x;
	int c;
if(tip==3)
	{
	printf("Eroare!Un char nu poate fii implicat intr-o operatie!\n");
	return 0;
	}
if(aux.valoare==0)
	if(tip==1)
		aux.tip=1;
	else
		aux.tip=2;
if(oper==0)//inmultire
	{
	if((aux.tip==1)&&(tip==1))
		aux.tip=1;
	else
		aux.tip=2;
	aux.valoare=aux.valoare*val;
	}
if(oper==1)//impartire
	{
	if((aux.tip==1)&&(tip==1))
		if(((int)aux.valoare % (int)val)==0)
			aux.tip=1;
		else
			aux.tip=2;
	aux.valoare=aux.valoare/val;
	}
if(oper==2)//div
	{
	if((aux.tip!=1)||(tip!=1))
		{
		printf("Eroare!Tipul operanzilor trebuie sa fie intreg pt a putea realiza div!\n");
		return 0;
		}
	aux.tip=1;
	aux.valoare=(double)((int)(aux.valoare/val));
	}
if(oper==3)//mod
	{
	if((aux.tip!=1)||(tip!=1))
		{
		printf("Eroare!Tipul operanzilor trebuie sa fie intreg pt a putea realiza mod!\n");
		return 0;
		}
	aux.tip=1;
	aux.valoare=(double)((int)aux.valoare%(int)val);
	}
if(oper==4)//adunare
	{
	if((aux.tip==1)&&(tip==1))
		aux.tip=1;
	else
		aux.tip=2;
	aux.valoare=aux.valoare+val;
	}
if(oper==5)//scadere
	{
	if((aux.tip==1)&&(tip==1))
		aux.tip=1;
	else
		aux.tip=2;
	aux.valoare=aux.valoare-val;
	}
if(aux.tip==2)
	{
	x=aux.valoare;
	c=(int)aux.valoare;
	if((double)(c)==x)
		aux.tip=1;
	}
return 1;
}
*/
int FactorStatic()
{
	int i,j,tip,ok=0;
	double val;

	if (q->cod_lexical==0)//identificator
	{
		for(i=0;i<nr;i++)
			if(strcmp(tabela[i].nume,q->nume)==0)
				if(strcmp(tabela[i].clasa,"nume constanta")==0)
				{
					ok=1;
					j=i;
					break;
				}
		if(ok==0)
		{
			error=1;
			printf("Eroare!identificatorul din declaratia constantei trebuie sa fie constanta!\n");
			return 0;
		}
		if (ok==1)
		{
			val=tab[tabela[j].val].valoare;
			tip=tab[tabela[j].val].tip;
		//	calculeaza(val,tip,oper);
			
			//add_lista(oper,0,1);
			add_lista(val,tip,0);
		}

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
				  //aici tre sa pun in lista
				  add_lista(1,0,0);

				  q=getNextAtom(q);
				  if (ExpresieStatica()) 
				  {	
					  if ((q->cod_lexical==3)&&(q->atribut==5)) //operatorul )
					  {
							//aici tre sa pun in lista
							add_lista(2,0,0);

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
int tip;
double val;
	if ((q->cod_lexical==2)||(q->cod_lexical==6))// constanta numerica , constanta caracter
	{
		val=tab[q->atribut].valoare;
		tip=tab[q->atribut].tip;
	//	calculeaza(val,tip,oper);
//		add_lista(oper,0,1);
		add_lista(val,tip,0);
		
		q=getNextAtom(q);
		return 1;
	}
	else
		return 0;
}//Constanta


int InstrComp()
{
	int j;
	if ((q->cod_lexical==1)&&(q->atribut==1))//cuvantul cheie: begin
	{
		printf("Tabela de simboluri inainte de begin\n");
		afisare();
		getch();
		
		q=getNextAtom(q);
		if (ListaInstr())
		{
			if ((q->cod_lexical==4)&&(q->atribut==0))// delimitatorul ;
			{
				q=getNextAtom(q);
				if ((q->cod_lexical==1)&&(q->atribut==9))//cuvantul cheie: end
				{
					j=nr-1;
					while(tabela[j].nivel==nivel)
					{
						tabela[j].listapar=NULL;
						tabela[j].listarec=NULL;
						tabela[j].nume=NULL;
						tabela[j].clasa=NULL;
						tabela[j].tip=0;
						tabela[nr].nivel=-1;
						tabela[nr].val=-1;
						tabela[nr].adrel=-1; 
						tabela[nr].deplrec=-1;
						tabela[nr].imin=-1;
						tabela[nr].imax=-1;
						tabela[nr].nrpar=-1;
						tabela[nr].dimvar=-1;
						j--;
					}
					nr=j+1;
					nivel--;
					getch();
					printf("Tabela de simboluri dupa end:\n");
					afisare();
					getch();

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
					j=nr-1;
					while(tabela[j].nivel==nivel)
					{
						tabela[j].listapar=NULL;
						tabela[j].listarec=NULL;
						tabela[j].nume=NULL;
						tabela[j].clasa=NULL;
						tabela[j].tip=0;
						tabela[nr].nivel=-1;
						tabela[nr].val=-1;
						tabela[nr].adrel=-1; 
						tabela[nr].deplrec=-1;
						tabela[nr].imin=-1;
						tabela[nr].imax=-1;
						tabela[nr].nrpar=-1;
						tabela[nr].dimvar=-1;
						j--;
					}
					nr=j+1;
					nivel--;
					getch();
					printf("Tabela de simboluri dupa end:\n");
					afisare();
					getch();

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
	fclose(rezalex);

	printf("\nTabela de constante\n");
	for (i=0;i<n;i++)
		printf(" %d %f\n",tab[i].tip,tab[i].valoare);
	getch();

}//main

