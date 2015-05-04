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

typedef struct LISTA_PAR
{
	int tip1; //1-intreg;2-real;3-char
	int tip2; //1-prin valoare;2-prin adresa
	struct LISTA_PAR *next;
}lista_par;

typedef struct LISTA_REC
{
	int poz;
	struct LISTA_REC *urm;
}lista_rec;

typedef struct TAB_SIMB
{
	int nr;
	char *nume;
	char *clasa;
	int nivel;
	int tip;
	int val;
	int adrel;
	int deplrec;
	int imin;
	int imax;
	int nrpar;
	int dimvar;
	lista_rec *listarec;
	lista_par *listapar;
}tab_simb;

atom *getNextAtom(atom *a);
int ProgramSursa();
int Bloc();
int SectiuneConst();
int ListaDeclConst();
int ListaDeclConst1();
int SectiuneVar();
int ListaDeclVar();
int ListaDeclVar1();
int SectiuneDeclSubprog();
int ListaDeclSubprog();
int ListaDeclSubprog1();
int DeclSubprog();
int DeclarConst();
int DeclarVar();
int ListaId();
int ListaId1();
int Tip();
int TipSimplu();
int TipTablou();
int TipStruct();
int ListaCamp();
int ListaCamp1();
int DeclSimpla();
int DeclarFunctie();
int DeclarProcedura();
int AntetSubprog();
int ParamForm();
int ListaParamForm();
int ListaParamForm1();
int DeclarPar();
int ExpresieStatica();
int ExpresieStatica1();
int TermenStatic();
int TermenStatic1();
int FactorStatic();
int Constanta();
int InstrComp();
int ListaInstr();
int ListaInstr1();
int Instr();
int InstrAtrib();
int Variabila();
int Variabila1();
int Expresie();
int Expresie1();
int Termen();
int Termen1();
int Factor();
int ListaExpresii();
int ListaExpresii1();
int InstrIf();
int RamuraElse();
int Conditie();
int ExprLogica();
int ExprLogica1();
int ExprRel();
int InstrWhile();
int InstrRepeat();
int InstrFor();
int Sens();
int Pas();
int InstrCase();
int ListaAltern();
int ListaAltern1();
int ListaRamuri();
int ListaRamuri1();
int Ramura();
int ListaVal();
int ListaVal1();
int InstrPrint();
int ListaElem();
int ListaElem1();
int Element();
int InstrRead();
int ListaVariab();
int ListaVariab1();
int ApelProc();
int ApelProc1();


