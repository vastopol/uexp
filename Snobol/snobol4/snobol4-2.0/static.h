static int AUGATL __P((ret_t));
static int CODSKP __P((ret_t));
static int DTREP __P((ret_t));
static int FINDEX __P((ret_t));
static int BLOCK __P((ret_t));
static int GENVAR __P((ret_t));
static int GNVARI __P((ret_t));
static int GENVUP __P((ret_t));
static int CONVAR __P((ret_t));
static int GNVARS __P((ret_t));
static int GC __P((ret_t));
static int GCM __P((ret_t));
static int SPLIT __P((ret_t));
static int BINOP __P((ret_t));
static int CMPILE __P((ret_t));
static int ELEMNT __P((ret_t));
static int ELEARG __P((ret_t));
static int EXPR __P((ret_t));
static int EXPR1 __P((ret_t));
static int NULNOD __P((ret_t));
static int FORWRD __P((ret_t));
static int FORBLK __P((ret_t));
static int NEWCRD __P((ret_t));
static int TREPUB __P((ret_t));
static int UNOP __P((ret_t));
static int INTERP __P((ret_t));
static int INVOKE __P((ret_t));
static int ARGVAL __P((ret_t));
static int EXPVAL __P((ret_t));
static int EXPEVL __P((ret_t));
static int INTVAL __P((ret_t));
static int PATVAL __P((ret_t));
static int VARVAL __P((ret_t));
static int VARVUP __P((ret_t));
static int VPXPTR __P((ret_t));
static int XYARGS __P((ret_t));
static int SCNR __P((ret_t));
static int SCIN __P((ret_t));
static int SCIN1 __P((ret_t));
static int ASSOCE __P((ret_t));
static int PUTIN __P((ret_t));
static int PUTOUT __P((ret_t));
static int TRACEP __P((ret_t));
static int STOPTP __P((ret_t));
static int FENTR2 __P((ret_t));
static int TRPHND __P((ret_t));
static int FNEXT2 __P((ret_t));
static int KEYT __P((ret_t));
static int NMD __P((ret_t));
static int ARGINT __P((ret_t));
static int CONVE __P((ret_t));
static int ICNVTA __P((ret_t));
static int DUMP __P((ret_t));
static int DMK __P((ret_t));
static int COMP5 __P((ret_t));
static int COMP3 __P((ret_t));
static int COMP7 __P((ret_t));
static int FTLEN2 __P((ret_t));
static int MAIN1 __P((ret_t));
static int A5RTN __P((ret_t));
static int RTN1 __P((ret_t));
static int SIZERR __P((ret_t));
static int ALOC2 __P((ret_t));
static int RT1NUL __P((ret_t));
static int LOCA1 __P((ret_t));
static int FAIL __P((ret_t));
static int RTN2 __P((ret_t));
static int RTZPTR __P((ret_t));
static int RTN3 __P((ret_t));
static int COMP9 __P((ret_t));
static int ELEICH __P((ret_t));
static int ELEILI __P((ret_t));
static int ELEINT __P((ret_t));
static int ELEDEC __P((ret_t));
static int ELECMA __P((ret_t));
static int ELEMN9 __P((ret_t));
static int EXPR2 __P((ret_t));
static int RTXNAM __P((ret_t));
static int EXPR7 __P((ret_t));
static int FORJRN __P((ret_t));
static int FORRUN __P((ret_t));
static int RTNUL3 __P((ret_t));
static int INTR5 __P((ret_t));
static int INTR4 __P((ret_t));
static int EXEX __P((ret_t));
static int EXPVJN __P((ret_t));
static int RTXPTR __P((ret_t));
static int INTR1 __P((ret_t));
static int EXPVJ2 __P((ret_t));
static int GENVIX __P((ret_t));
static int ARITH __P((ret_t));
static int AERROR __P((ret_t));
static int ARTN __P((ret_t));
static int RETNUL __P((ret_t));
static int CHARZ __P((ret_t));
static int ABNSND __P((ret_t));
static int NONAME __P((ret_t));
static int LPRTND __P((ret_t));
static int PATNOD __P((ret_t));
static int LENERR __P((ret_t));
static int NEMO __P((ret_t));
static int NAM5 __P((ret_t));
static int RTYPTR __P((ret_t));
static int INTR8 __P((ret_t));
static int INTR31 __P((ret_t));
static int INTR13 __P((ret_t));
static int PROTER __P((ret_t));
static int UNDFFE __P((ret_t));
static int GENVRZ __P((ret_t));
static int INTR27 __P((ret_t));
static int NONARY __P((ret_t));
static int UNTERR __P((ret_t));
static int IOOP __P((ret_t));
static int INTR30 __P((ret_t));
static int FXOVR __P((ret_t));
static int FENTR3 __P((ret_t));
static int KEYTR3 __P((ret_t));
static int FNEXT1 __P((ret_t));
static int VXOVR __P((ret_t));
static int VALTR2 __P((ret_t));
static int VALTR3 __P((ret_t));
static int DEFDT __P((ret_t));
static int VALTR1 __P((ret_t));
static int TRPRT __P((ret_t));
static int TRI __P((ret_t));
static int TRI2 __P((ret_t));
static int TRV __P((ret_t));
static int VALTR4 __P((ret_t));
static int SORT1 __P((ret_t));
static int GENVSZ __P((ret_t));
static int UNKNKW __P((ret_t));
static int KEYN __P((ret_t));
static int KEYC __P((ret_t));
static int RPAD0 __P((ret_t));
static int ARG1 __P((ret_t));
static int ARG2 __P((ret_t));
static int CNVIV __P((ret_t));
static int CONVR __P((ret_t));
static int CONRI __P((ret_t));
static int CONIR __P((ret_t));
static int CNVVI __P((ret_t));
static int CNVAT __P((ret_t));
static int CNVTA __P((ret_t));
static int CNVRTS __P((ret_t));
static int CONVEX __P((ret_t));
static int RECOMP __P((ret_t));
static int RECOMJ __P((ret_t));
static int RECOMZ __P((ret_t));
static int ENDALL __P((ret_t));
static int FTLTST __P((ret_t));
static int FTLEND __P((ret_t));
static int FTERST __P((ret_t));
static int SCERST __P((ret_t));
static int CTLADV __P((ret_t));
static int FILCHK __P((ret_t));
static int COMP1 __P((ret_t));
static int LOAD2 __P((ret_t));
static int COMP6 __P((ret_t));
static int XITHND __P((ret_t));
static int CNTERR __P((ret_t));
static int USRINT __P((ret_t));
static int END0 __P((ret_t));
static int ADD_NP __P((ret_t));
static int AF_MERGE __P((ret_t));
static int AFRAME __P((ret_t));
static int B_PB __P((ret_t));
static int BMORG4 __P((ret_t));
static int BMORG5 __P((ret_t));
static int BMORG6 __P((ret_t));
static int BMORG7 __P((ret_t));
static int BHEAD __P((ret_t));
static int BLAND __P((ret_t));
static int BLANK __P((ret_t));
static int BLS2 __P((ret_t));
static int BLS1 __P((ret_t));
static int BLOKVAL __P((ret_t));
static int BOXIN __P((ret_t));
static int BTAIL __P((ret_t));
static int CAE __P((ret_t));
static int CIR __P((ret_t));
static int CLASS __P((ret_t));
static int COMPFR __P((ret_t));
static int DISTR __P((ret_t));
static int DUMP_B __P((ret_t));
static int DUMP_A __P((ret_t));
static int DUPE __P((ret_t));
static int E_ATTACH __P((ret_t));
static int EMB_PHY __P((ret_t));
static int FICOM __P((ret_t));
static int FIX1 __P((ret_t));
static int FIXINL __P((ret_t));
static int FORCING __P((ret_t));
static int F_JOIN __P((ret_t));
static int GR1 __P((ret_t));
static int IDENT_SB __P((ret_t));
static int INIT_SUB __P((ret_t));
static int INSERT __P((ret_t));
static int JE_LONGI __P((ret_t));
static int JE_ORTHO __P((ret_t));
static int JOIN __P((ret_t));
static int LSOHN __P((ret_t));
static int MIDREG __P((ret_t));
static int MINGLE __P((ret_t));
static int NRMZ_REG __P((ret_t));
static int N_REG __P((ret_t));
static int OPS1 __P((ret_t));
static int CCATB __P((ret_t));
static int PAR_CONG __P((ret_t));
static int PRE_SUF __P((ret_t));
static int P_BLOCK __P((ret_t));
static int NP_BLOCK __P((ret_t));
static int REPL __P((ret_t));
static int SUBBLOCK __P((ret_t));
static int STRIP_F __P((ret_t));
static int T_LEAF __P((ret_t));
static int UDCOM __P((ret_t));
static int UNITS __P((ret_t));
static int WARNING __P((ret_t));
static int NR99 __P((ret_t));
static int PRERR __P((ret_t));
static int BM99 __P((ret_t));
static int BMNULL __P((ret_t));
static int FIX99 __P((ret_t));
static int FIX2 __P((ret_t));
static int JELCO __P((ret_t));
static int JEL99 __P((ret_t));
static int CONVV __P((ret_t));
