/* generated from v311.sil by genc.sno on 01/01/2015 23:54:50 */

# include "mlink.h"
# include "parms.h"
int
BEGIN(retval)
    ret_t retval;
{
    ENTRY(BEGIN)
    init();
    ISTACK();
    if (D_A(BANRCL) == 0)
	goto L_NOBANR;
    io_printf(D_A(PUNCH),TITLEF);
    io_printf(D_A(PUNCH),TITLEF1);
    io_printf(D_A(PUNCH),SOURCF);
L_NOBANR:
    D_RV(TIMECL) = mstime();
    D_F(TIMECL) = D_V(TIMECL) = 0;
    SAVSTK();
    PUSH(OCALIM);
    BLOCK(SCBSCL);
    D(OCSVCL) = D(SCBSCL);
    D_F(SCBSCL) &= ~(PTR);
    D_A(YCL) = D_V(D_A(INITLS));
    D_F(YCL) = D_V(YCL) = 0;
L_SPCNVT:
    D(XPTR) = D(D_A(INITLS) + D_A(YCL));
    D_A(XCL) = D_V(D_A(XPTR));
    D_F(XCL) = D_V(XCL) = 0;
L_SPCNV1:
    D(ZPTR) = D(D_A(XPTR) + D_A(XCL));
    if (D_A(ZPTR) == 0)
	goto L_SPCNV2;
    SAVSTK();
    PUSH(ZPTR);
    GENVAR(ZPTR);
    D(D_A(XPTR) + D_A(XCL)) = D(ZPTR);
L_SPCNV2:
    D_A(XCL) -= 2*DESCR;
    if (D_A(XCL) > 0)
	goto L_SPCNV1;
    D_A(YCL) -= DESCR;
    if (D_A(YCL) > 0)
	goto L_SPCNVT;
L_INITD1:
    D(XPTR) = D(D_A(INITB));
    SAVSTK();
    PUSH(XPTR);
    GENVAR(YPTR);
    D(ZPTR) = D(D_A(INITB) + DESCR);
    D(D_A(ZPTR)) = D(YPTR);
    D_A(INITB) += 2*DESCR;
    if (D_PTR(INITB) < D_PTR(INITE))
	goto L_INITD1;
    GETPARM(XSP);
    SAVSTK();
    PUSH(XSPPTR);
    GENVAR(PARMVL);
    D(D_A(ABRTKY) + DESCR) = D(ABOPAT);
    D(D_A(ARBKY) + DESCR) = D(ARBPAT);
    D(D_A(BALKY) + DESCR) = D(BALPAT);
    D(D_A(FAILKY) + DESCR) = D(FALPAT);
    D(D_A(FNCEKY) + DESCR) = D(FNCPAT);
    D(D_A(REMKY) + DESCR) = D(REMPAT);
    D(D_A(SUCCKY) + DESCR) = D(SUCPAT);
    D_A(VARSYM) = 0;
    SAVSTK();
    PUSH(NMOVER);
    BLOCK(NBSPTR);
    D(CMBSCL) = D(SCBSCL);
    D(UNIT) = D(INPUT);
    D(OCBSCL) = D(CMBSCL);
    D(OCLIM) = D(CMBSCL);
    D_A(OCLIM) += D_A(OCALIM);
    D_A(OCLIM) -= 7*DESCR;
    D_A(INICOM) = 1;
    IO_FILE(UNIT,XSP);
    SAVSTK();
    PUSH(XSPPTR);
    GENVAR(FILENM);
    D_A(TMVAL) = 0;
L_AUTLOP:
    if (!GETPMPROTO(XSP,TMVAL))
	goto L_XLATRN;
    SAVSTK();
    PUSH(XSPPTR);
    GENVAR(XPTR);
    D(WPTR) = D(NULVCL);
    D_A(ERRLCL)++;
    SAVSTK();
    LOAD2(NORET);
    D_A(ERRLCL) = 0;
    D_A(TMVAL)++;
    goto L_AUTLOP;
    /*_*/
L_XLATRD:
    if (D_A(LISTCL) == 0)
	goto L_XLATRN;
    IO_PRINT(IOKEY, OUTBLK, LNBFSP);
L_XLATRN:
    _SPEC(TEXTSP) = _SPEC(NEXTSP);
    switch (IO_READ(UNIT, TEXTSP)) {
    case IO_EOF:
	goto L_XLATIN;
    case IO_ERR:
	BRANCH(COMP1)
    }
    D_A(TMVAL) = S_L(TEXTSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D_A(TMVAL) += STNOSZ;
    S_L(LNBFSP) = D_A(TMVAL);
    D_A(LNNOCL)++;
L_XLATNX:
    switch (STREAM(XSP, TEXTSP, &CARDTB)) {
    case ST_ERROR:
	BRANCH(COMP3)
    case ST_EOS:
	goto L_XLATRD;
    }
    SAVSTK();
    switch (NEWCRD(NORET)) {
    case 1:
	goto L_XLATRD;
    }
    SAVSTK();
    switch (CMPILE(NORET)) {
    case 1:
	BRANCH(COMP3)
    case 3:
	goto L_XLATNX;
    }
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(ENDCL);
    if (D_A(LISTCL) == 0)
	goto L_XLATP;
    IO_PRINT(IOKEY, OUTBLK, LNBFSP);
L_XLATP:
    if (D_A(STYPE) == EOSTYP)
	goto L_XLAEND;
    switch (STREAM(XSP, TEXTSP, &IBLKTB)) {
    case ST_ERROR:
	BRANCH(COMP3)
    case ST_EOS:
	goto L_XLAEND;
    }
    if (D_A(STYPE) == EOSTYP)
	goto L_XLAEND;
    if (D_A(STYPE) != NBTYP)
	BRANCH(COMP7)
    switch (STREAM(XSP, TEXTSP, &LBLTB)) {
    case ST_ERROR:
    case ST_EOS:
	BRANCH(COMP7)
    }
    SAVSTK();
    PUSH(XSPPTR);
    GENVUP(XPTR);
    D(OCBSCL) = D(D_A(XPTR) + ATTRIB);
    if (D_A(OCBSCL) == 0)
	BRANCH(COMP7)
    if (D_A(STYPE) == EOSTYP)
	goto L_XLAEND;
    switch (STREAM(XSP, TEXTSP, &IBLKTB)) {
    case ST_ERROR:
    case ST_STOP:
	BRANCH(COMP7)
    }
L_XLAEND:
    if (D_A(ESAICL) == 0)
	goto L_XLATSC;
    io_printf(D_A(PUNCH),ERRCF);
    if (D_A(NERRCL) == 0)
	goto L_XLATND;
    D_A(RETCOD) = 1;
    BRANCH(FTLEN2)
    /*_*/
L_XLATIN:
    SAVSTK();
    FILCHK(NORET);
    goto L_XLATRN;
    /*_*/
L_XLATSC:
    if (D_A(BANRCL) == 0)
	goto L_XLATND;
    io_printf(D_A(PUNCH),SUCCF);
L_XLATND:
    if (D_A(EXECCL) == 0)
	BRANCH(END)
    io_ecomp();
    D_A(UNIT) = 0;
    D_A(LPTR) = 0;
    D_A(OCLIM) = 0;
    D_A(LNNOCL) = 0;
    ZERBLK(D_A(COMREG),D_A(COMDCT));
    D(XCL) = D(CMBSCL);
    D_A(XCL) += D_A(CMOFCL);
    SAVSTK();
    PUSH(XCL);
    SPLIT(NORET);
    D_A(LISTCL) = 0;
    D_A(COMPCL) = 0;
    D_RV(ETMCL) = mstime();
    D_F(ETMCL) = D_V(ETMCL) = 0;
    D_RV(TIMECL) = D_RV(ETMCL) - D_RV(TIMECL);
    D_F(TIMECL) = D_F(ETMCL);
    D_V(TIMECL) = D_V(ETMCL);
    D_A(CNSLCL) = 1;
    SAVSTK();
    switch (INTERP(NORET)) {
    case 1:
    case 2:
    case 3:
	BRANCH(MAIN1)
    }
    /*_*/
}
static int
AUGATL(retval)
    ret_t retval;
{
    ENTRY(AUGATL)
    POP(A1PTR);
    POP(A2PTR);
    POP(A3PTR);
    if (!LOCAPT(A4PTR,A1PTR,ZEROCL))
	goto L_AUG1;
    D(D_A(A4PTR) + DESCR) = D(A2PTR);
    D(D_A(A4PTR) + 2*DESCR) = D(A3PTR);
    D(A5PTR) = D(A1PTR);
    BRANCH(A5RTN)
    /*_*/
L_AUG1:
    D_A(A4PTR) = D_V(D_A(A1PTR));
    D_F(A4PTR) = D_V(A4PTR) = 0;
    D_A(A4PTR) += 2*DESCR;
    D_V(A4PTR) = B;
    SAVSTK();
    PUSH(A4PTR);
    BLOCK(A5PTR);
    D(D_A(A5PTR) + D_A(A4PTR)) = D(A3PTR);
    D_A(A4PTR) -= DESCR;
    D(D_A(A5PTR) + D_A(A4PTR)) = D(A2PTR);
    D_A(A4PTR) -= DESCR;
    MOVBLK(D_A(A5PTR),D_A(A1PTR),D_A(A4PTR));
    BRANCH(A5RTN)
    /*_*/
}
static int
CODSKP(retval)
    ret_t retval;
{
    ENTRY(CODSKP)
    POP(YCL);
L_CODCNT:
    D_A(OCICL) += DESCR;
    D(XCL) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(XCL) & FNC))
	goto L_CODFNC;
L_CODECR:
    D_A(YCL)--;
    if (D_A(YCL) < 0)
	BRANCH(INTR10)
    if (D_A(YCL) == 0)
	BRANCH(RTN1)
    goto L_CODCNT;
    /*_*/
L_CODFNC:
    PUSH(YCL);
    D_A(YCL) = D_V(XCL);
    D_F(YCL) = D_V(YCL) = 0;
    SAVSTK();
    PUSH(YCL);
    CODSKP(NORET);
    POP(YCL);
    goto L_CODECR;
    /*_*/
}
static int
DTREP(retval)
    ret_t retval;
{
    ENTRY(DTREP)
    POP(A2PTR);
    if (D_V(A2PTR) == A)
	goto L_DTARRY;
    if (D_V(A2PTR) == T)
	goto L_DTABLE;
    if (D_V(A2PTR) != R)
	goto L_DTREP1;
    REALST(DPSP,A2PTR);
    goto L_DTREPR;
    /*_*/
L_DTARRY:
    D(A3PTR) = D(D_A(A2PTR) + DESCR);
    X_LOCSP(ZSP,A3PTR);
    D_A(A3PTR) = S_L(ZSP);
    D_F(A3PTR) = D_V(A3PTR) = 0;
    if (D_A(A3PTR) > ARRLEN)
	goto L_DTREP1;
    S_L(DTARSP) = 0;
    APDSP(DTARSP,ARRSP);
    APDSP(DTARSP,LPRNSP);
    APDSP(DTARSP,QTSP);
    APDSP(DTARSP,ZSP);
    APDSP(DTARSP,QTSP);
L_DTARTB:
    APDSP(DTARSP,RPRNSP);
    _SPEC(DPSP) = _SPEC(DTARSP);
    goto L_DTREPR;
    /*_*/
L_DTABLE:
    D_A(A3PTR) = D_V(D_A(A2PTR));
    D_F(A3PTR) = D_V(A3PTR) = 0;
    D(A1PTR) = D(D_A(A2PTR) + D_A(A3PTR));
    D_A(A3PTR) -= DESCR;
    D(A2PTR) = D(D_A(A2PTR) + D_A(A3PTR));
L_DTABL1:
    if (D_A(A1PTR) == 1)
	goto L_DTABL2;
    D_A(A3PTR) += D_A(A2PTR);
    D_A(A3PTR) -= 2*DESCR;
    D(A1PTR) = D(D_A(A1PTR) + D_A(A2PTR));
    goto L_DTABL1;
    /*_*/
L_DTABL2:
    D_A(A3PTR) -= DESCR;
    D_A(A2PTR) -= 2*DESCR;
    D_A(A3PTR) /= D_A(DSCRTW);
    INTSPC(ZSP,A3PTR);
    S_L(DTARSP) = 0;
    APDSP(DTARSP,ASSCSP);
    APDSP(DTARSP,LPRNSP);
    APDSP(DTARSP,ZSP);
    APDSP(DTARSP,CMASP);
    D_A(A2PTR) /= D_A(DSCRTW);
    INTSPC(ZSP,A2PTR);
    APDSP(DTARSP,ZSP);
    goto L_DTARTB;
    /*_*/
L_DTREP1:
    D_V(DT1CL) = D_V(A2PTR);
    if (!LOCAPT(A3PTR,DTATL,DT1CL))
	goto L_DTREPE;
    D(A3PTR) = D(D_A(A3PTR) + 2*DESCR);
    X_LOCSP(DPSP,A3PTR);
L_DTREPR:
    D(retval) = D(DPSPTR);
    RETURN(1)
    /*_*/
L_DTREPE:
    _SPEC(DPSP) = _SPEC(EXDTSP);
    goto L_DTREPR;
    /*_*/
}
static int
FINDEX(retval)
    ret_t retval;
{
    ENTRY(FINDEX)
    POP(F1PTR);
    if (!LOCAPV(F2PTR,FNCPL,F1PTR))
	goto L_FATNF;
    D(F2PTR) = D(D_A(F2PTR) + DESCR);
L_FATBAK:
    D(retval) = D(F2PTR);
    RETURN(1)
    /*_*/
L_FATNF:
    D_A(NEXFCL) += 2*DESCR;
    if (D_A(NEXFCL) > FBLKSZ)
	goto L_FATBLK;
L_FATNXT:
    D(F2PTR) = D(FBLOCK);
    D_A(F2PTR) += D_A(NEXFCL);
    SAVSTK();
    PUSH(F1PTR);
    PUSH(F2PTR);
    PUSH(FNCPL);
    AUGATL(FNCPL);
    D(D_A(F2PTR)) = D(UNDFCL);
    D(D_A(F2PTR) + DESCR) = D(F1PTR);
    goto L_FATBAK;
    /*_*/
L_FATBLK:
    SAVSTK();
    PUSH(FBLKRQ);
    BLOCK(FBLOCK);
    D_F(FBLOCK) |= FNC;
    D_V(FBLOCK) = 0;
    D_A(NEXFCL) = (int_t) DESCR;
    goto L_FATNXT;
    /*_*/
}
static int
BLOCK(retval)
    ret_t retval;
{
    ENTRY(BLOCK)
    POP(ARG1CL);
    if (D_A(ARG1CL) >= D_A(SIZLMT))
	BRANCH(SIZERR)
L_BLOCK1:
    D(BLOCL) = D(FRSGPT);
    D_V(BLOCL) = D_V(ARG1CL);
    D_A(FRSGPT) += DESCR;
    D_A(FRSGPT) += D_A(ARG1CL);
    if (D_PTR(TLSGP1) < D_PTR(FRSGPT))
	goto L_BLOGC;
    ZERBLK(D_A(BLOCL),D_A(ARG1CL));
    D_A(D_A(BLOCL)) = D_A(BLOCL);
    D_F(D_A(BLOCL)) |= TTL;
    D_V(D_A(BLOCL)) = D_A(ARG1CL);
    D(retval) = D(BLOCL);
    RETURN(1)
    /*_*/
L_BLOGC:
    D_A(FRSGPT) = D_A(BLOCL);
    SAVSTK();
    PUSH(ARG1CL);
    switch (GC(NORET)) {
    case 1:
	BRANCH(ALOC2)
    case 2:
	goto L_BLOCK1;
    }
    /*_*/
}
static int
GENVAR(retval)
    ret_t retval;
{
    ENTRY(GENVAR)
    D_A(CONVSW) = 0;
    POP(AXPTR);
    _SPEC(SPECR1) = _SPEC(D_A(AXPTR));
    if (S_L(SPECR1) == 0)
	BRANCH(RT1NUL)
    /* FALL */
    BRANCH(LOCA1)
}
static int
LOCA1(retval)
    ret_t retval;
{
    ENTRY(LOCA1)
    HASH(EQUVCL,SPECR1);
    D(BUKPTR) = D(OBPTR);
    D_A(BUKPTR) += D_A(EQUVCL);
L_LOCA2:
    D(LSTPTR) = D(BUKPTR);
    D_A(BUKPTR) = D_A(D_A(BUKPTR) + LNKFLD);
    if (D_A(BUKPTR) == 0)
	goto L_LOCA5;
    if (D_V(D_A(BUKPTR) + LNKFLD) < D_V(EQUVCL))
	goto L_LOCA2;
    if (D_V(D_A(BUKPTR) + LNKFLD) > D_V(EQUVCL))
	goto L_LOCA5;
    X_LOCSP(SPECR2,BUKPTR);
    if (!LEXEQ(SPECR1,SPECR2))
	goto L_LOCA2;
    D(LCPTR) = D(BUKPTR);
    goto L_LOCRET;
    /*_*/
L_LOCA5:
    D_A(AXPTR) = S_L(SPECR1);
    D_F(AXPTR) = D_V(AXPTR) = 0;
    D_A(BKLTCL) = X_GETLTH(AXPTR);
    D_F(BKLTCL) = D_V(BKLTCL) = 0;
    if (D_A(BKLTCL) > D_A(SIZLMT))
	BRANCH(SIZERR)
L_LOCA7:
    D(LCPTR) = D(FRSGPT);
    D_V(LCPTR) = S;
    D_A(FRSGPT) += DESCR;
    D_A(FRSGPT) += D_A(BKLTCL);
    if (D_PTR(TLSGP1) < D_PTR(FRSGPT))
	goto L_LOCA4;
    D(D_A(LCPTR)) = D(ZEROCL);
    D_A(D_A(LCPTR)) = D_A(LCPTR);
    D_F(D_A(LCPTR)) |= TTL+STTL;
    D_V(D_A(LCPTR)) = D_A(AXPTR);
    if (D_A(CONVSW) != 0)
	goto L_LOCA6;
    D(D_A(LCPTR) + DESCR) = D(NULVCL);
    D(D_A(LCPTR) + ATTRIB) = D(ZEROCL);
    X_LOCSP(SPECR2,LCPTR);
    S_L(SPECR2) = 0;
    APDSP(SPECR2,SPECR1);
L_LOCA6:
    D_V(D_A(LCPTR) + LNKFLD) = D_V(EQUVCL);
    D_A(D_A(LCPTR) + LNKFLD) = D_A(BUKPTR);
    D_A(D_A(LSTPTR) + LNKFLD) = D_A(LCPTR);
    D_A(VARSYM)++;
L_LOCRET:
    D(retval) = D(LCPTR);
    RETURN(1)
    /*_*/
L_LOCA4:
    D_A(FRSGPT) = D_A(LCPTR);
    SAVSTK();
    PUSH(BKLTCL);
    switch (GC(NORET)) {
    case 1:
	BRANCH(ALOC2)
    case 2:
	goto L_LOCA7;
    }
    /*_*/
}
static int
GNVARI(retval)
    ret_t retval;
{
    ENTRY(GNVARI)
    D_A(CONVSW) = 0;
    POP(AXPTR);
    INTSPC(SPECR1,AXPTR);
    BRANCH(LOCA1)
    /*_*/
}
static int
GENVUP(retval)
    ret_t retval;
{
    ENTRY(GENVUP)
    if (D_A(CASECL) == 0)
	BRANCH(GENVAR)
    POP(AXPTR);
    PUSH(AXPTR);
    _SPEC(SPECR1) = _SPEC(D_A(AXPTR));
    RAISE1(SPECR1);
    BRANCH(GENVAR)
    /*_*/
}
static int
CONVAR(retval)
    ret_t retval;
{
    ENTRY(CONVAR)
    POP(AXPTR);
    if (D_A(AXPTR) == 0)
	BRANCH(RT1NUL)
    D_A(CONVSW) = 1;
    D_A(BKLTCL) = X_GETLTH(AXPTR);
    D_F(BKLTCL) = D_V(BKLTCL) = 0;
    if (D_A(BKLTCL) > D_A(SIZLMT))
	BRANCH(SIZERR)
    D(TEMPCL) = D(FRSGPT);
    D_A(TEMPCL) += D_A(BKLTCL);
    D_A(TEMPCL) += DESCR;
    if (D_A(TLSGP1) < D_A(TEMPCL))
	goto L_CONVR4;
L_CONVR5:
    D(D_A(FRSGPT)) = D(ZEROCL);
    D_A(D_A(FRSGPT)) = D_A(FRSGPT);
    D_F(D_A(FRSGPT)) |= TTL+STTL;
    D_V(D_A(FRSGPT)) = D_A(AXPTR);
    D(D_A(FRSGPT) + DESCR) = D(NULVCL);
    D(D_A(FRSGPT) + ATTRIB) = D(ZEROCL);
    D_A(BKLTCL) = D_A(FRSGPT);
    D(retval) = D(BKLTCL);
    RETURN(1)
    /*_*/
L_CONVR4:
    SAVSTK();
    PUSH(BKLTCL);
    switch (GC(NORET)) {
    case 1:
	BRANCH(ALOC2)
    case 2:
	goto L_CONVR5;
    }
    /*_*/
}
static int
GNVARS(retval)
    ret_t retval;
{
    ENTRY(GNVARS)
    POP(AXPTR);
    if (D_A(AXPTR) == 0)
	BRANCH(RT1NUL)
    X_LOCSP(SPECR1,FRSGPT);
    S_L(SPECR1) = D_A(AXPTR);
    D_V(D_A(FRSGPT)) = D_A(AXPTR);
    BRANCH(LOCA1)
    /*_*/
}
static int
GC(retval)
    ret_t retval;
{
    ENTRY(GC)
    D_RV(GCTMCL) = mstime();
    D_F(GCTMCL) = D_V(GCTMCL) = 0;
    vm_gc_advise(1);
    POP(GCREQ);
    PSTACK(BLOCL);
    D_A(BLOCL) -= D_A(STKPTR);
    D_V(D_A(STKPTR)) = D_A(BLOCL);
    D(BKDXU) = D(PRMDX);
L_GCT:
    D(GCMPTR) = D(D_A(PRMPTR) + D_A(BKDXU));
    if (D_A(GCMPTR) == 0)
	goto L_GCTDWN;
    SAVSTK();
    PUSH(GCMPTR);
    GCM(NORET);
L_GCTDWN:
    D_A(BKDXU) -= DESCR;
    if (D_A(BKDXU) != 0)
	goto L_GCT;
    D_A(BKPTR) = (int_t) (OBLIST-DESCR);
L_GCBA1:
    if (D_PTR(BKPTR) > D_PTR(OBEND))
	goto L_GCLAD;
    D_A(BKPTR) += DESCR;
    D(ST1PTR) = D(BKPTR);
L_GCBA2:
    D_A(ST1PTR) = D_A(D_A(ST1PTR) + LNKFLD);
    if (D_A(ST1PTR) == 0)
	goto L_GCBA1;
    if ((D_F(D_A(ST1PTR)) & MARK))
	goto L_GCBA2;
    D(ST2PTR) = D(D_A(ST1PTR) + DESCR);
    if (!DCMP(ST2PTR, NULVCL))
	goto L_GCBA4;
    if (D_A(D_A(ST1PTR) + ATTRIB) == (int_t)0)
	goto L_GCBA2;
L_GCBA4:
    D(D_A(GCBLK) + DESCR) = D(ST1PTR);
    SAVSTK();
    PUSH(GCBLK);
    switch (GCM(NORET)) {
    case 1:
	goto L_GCBA2;
    }
    /*_*/
L_GCLAD:
    D(CPYCL) = D(HDSGPT);
    D(TTLCL) = D(HDSGPT);
L_GCLAD0:
    D_A(BKDX) = X_BKSIZE(D_A(TTLCL));
    D_F(BKDX) = D_V(BKDX) = 0;
    if (!(D_F(D_A(TTLCL)) & MARK))
	goto L_GCLAD7;
    D_A(CPYCL) += D_A(BKDX);
    D_A(TTLCL) += D_A(BKDX);
    if (D_A(TTLCL) == D_A(FRSGPT))
	goto L_GCBB1;
    else
	goto L_GCLAD0;
    /*_*/
L_GCLAD7:
    D(MVSGPT) = D(TTLCL);
L_GCLAD4:
    D_A(TTLCL) += D_A(BKDX);
    if (D_A(TTLCL) == D_A(FRSGPT))
	goto L_GCBB1;
    D_A(BKDX) = X_BKSIZE(D_A(TTLCL));
    D_F(BKDX) = D_V(BKDX) = 0;
    if (!(D_F(D_A(TTLCL)) & MARK))
	goto L_GCLAD4;
    D_A(D_A(TTLCL)) = D_A(CPYCL);
    D_A(CPYCL) += D_A(BKDX);
    goto L_GCLAD4;
    /*_*/
L_GCBB1:
    D_A(BKPTR) = (int_t) (OBLIST-DESCR);
    D_A(NODPCL) = 1;
L_GCBB2:
    if (D_PTR(BKPTR) > D_PTR(OBEND))
	goto L_GCLAP;
    D_A(BKPTR) += DESCR;
    D(ST1PTR) = D(BKPTR);
L_GCBB3:
    D(ST2PTR) = D(ST1PTR);
L_GCBB4:
    D_A(ST1PTR) = D_A(D_A(ST1PTR) + LNKFLD);
    if (D_A(ST1PTR) == 0)
	goto L_GCBB5;
    if (!(D_F(D_A(ST1PTR)) & MARK))
	goto L_GCBB4;
    D_A(BLOCL) = D_A(D_A(ST1PTR));
    D_A(D_A(ST2PTR) + LNKFLD) = D_A(BLOCL);
    goto L_GCBB3;
    /*_*/
L_GCBB5:
    D_A(D_A(ST2PTR) + LNKFLD) = D_A(ZEROCL);
    goto L_GCBB2;
    /*_*/
L_GCLAP:
    D(TTLCL) = D(HDSGPT);
L_GCLAP0:
    D_A(BKDXU) = X_BKSIZE(D_A(TTLCL));
    D_F(BKDXU) = D_V(BKDXU) = 0;
    if ((D_F(D_A(TTLCL)) & STTL))
	goto L_GCLAP1;
    D(BKDX) = D(BKDXU);
    goto L_GCLAP2;
    /*_*/
L_GCLAP1:
    D_A(BKDX) = (int_t) (3*DESCR);
L_GCLAP2:
    if (!(D_F(D_A(TTLCL)) & MARK))
	goto L_GCLAP5;
    D_A(BKDX) -= DESCR;
L_GCLAP3:
    D(DESCL) = D(D_A(TTLCL) + D_A(BKDX));
    if (!(D_F(DESCL) & PTR))
	goto L_GCLAP4;
    if (D_PTR(DESCL) < D_PTR(MVSGPT))
	goto L_GCLAP4;
    TOP(TOPCL,OFSET,DESCL);
    D_A(DESCL) = D_A(OFSET) + D_A(D_A(TOPCL));
    D(D_A(TTLCL) + D_A(BKDX)) = D(DESCL);
L_GCLAP4:
    D_A(BKDX) -= DESCR;
    if (D_A(BKDX) != 0)
	goto L_GCLAP3;
L_GCLAP5:
    D_A(TTLCL) += D_A(BKDXU);
    if (D_A(TTLCL) != D_A(FRSGPT))
	goto L_GCLAP0;
    D(BKDXU) = D(PRMDX);
L_GCLAT1:
    D(TTLCL) = D(D_A(PRMPTR) + D_A(BKDXU));
    if (D_A(TTLCL) == 0)
	goto L_GCLAT4;
    D_A(BKDX) = D_V(D_A(TTLCL));
    D_F(BKDX) = D_V(BKDX) = 0;
L_GCLAT2:
    D(DESCL) = D(D_A(TTLCL) + D_A(BKDX));
    if (!(D_F(DESCL) & PTR))
	goto L_GCLAT3;
    if (D_PTR(DESCL) < D_PTR(MVSGPT))
	goto L_GCLAT3;
    TOP(TOPCL,OFSET,DESCL);
    D_A(DESCL) = D_A(OFSET) + D_A(D_A(TOPCL));
    D(D_A(TTLCL) + D_A(BKDX)) = D(DESCL);
L_GCLAT3:
    D_A(BKDX) -= DESCR;
    if (D_A(BKDX) != 0)
	goto L_GCLAT2;
L_GCLAT4:
    D_A(BKDXU) -= DESCR;
    if (D_A(BKDXU) != 0)
	goto L_GCLAT1;
    D(TTLCL) = D(HDSGPT);
L_GCLAM0:
    D_A(BKDXU) = X_BKSIZE(D_A(TTLCL));
    D_F(BKDXU) = D_V(BKDXU) = 0;
    if (D_PTR(TTLCL) >= D_PTR(MVSGPT))
	goto L_GCLAM5;
    D_A(TOPCL) = D_A(D_A(TTLCL));
    D(D_A(TOPCL)) = D(D_A(TTLCL));
    D_F(D_A(TOPCL)) &= ~(MARK);
    goto L_GCLAM4;
    /*_*/
L_GCLAM5:
    D_A(BKDX) = D_A(BKDXU);
    D_A(BKDX) -= DESCR;
    if (!(D_F(D_A(TTLCL)) & MARK))
	goto L_GCLAM4;
    D_A(TOPCL) = D_A(D_A(TTLCL));
    D(D_A(TOPCL)) = D(D_A(TTLCL));
    D_F(D_A(TOPCL)) &= ~(MARK);
    MOVBLK(D_A(TOPCL),D_A(TTLCL),D_A(BKDX));
L_GCLAM4:
    D_A(TTLCL) += D_A(BKDXU);
    if (D_A(TTLCL) != D_A(FRSGPT))
	goto L_GCLAM0;
    vm_gc_advise(0);
    D_A(GCNO)++;
    D_A(NODPCL) = 0;
    D_A(BKDX) = X_BKSIZE(D_A(TOPCL));
    D_F(BKDX) = D_V(BKDX) = 0;
    D(FRSGPT) = D(TOPCL);
    D_A(FRSGPT) += D_A(BKDX);
    D_F(FRSGPT) &= ~(FNC);
    D(GCGOT) = D(TLSGP1);
    D_A(GCGOT) -= D_A(FRSGPT);
    D_A(GCGOT) -= DESCR;
    D_F(GCGOT) &= ~(PTR);
    D_RV(TEMPCL) = mstime();
    D_F(TEMPCL) = D_V(TEMPCL) = 0;
    D_RV(TEMPCL) -= D_RV(GCTMCL);
    D_RV(GCTTTL) += D_RV(TEMPCL);
    if (D_A(GCTRCL) < 0)
	goto L_GCTRC;
    if (D_A(GCTRCL) == 0)
	goto L_NOGCTR;
    D_A(GCTRCL)--;
L_GCTRC:
    io_printf(D_A(PUNCH),GCFMT,FILENM,LNNOCL,TEMPCL,GCGOT);
L_NOGCTR:
    if (D_A(GCREQ) > D_A(GCGOT))
	BRANCH(FAIL)
    D(retval) = D(GCGOT);
    RETURN(2)
    /*_*/
}
static int
GCM(retval)
    ret_t retval;
{
    ENTRY(GCM)
    POP(BK1CL);
    PUSH(ZEROCL);
L_GCMA1:
    D_A(BKDX) = D_V(D_A(BK1CL));
    D_F(BKDX) = D_V(BKDX) = 0;
L_GCMA2:
    D(DESCL) = D(D_A(BK1CL) + D_A(BKDX));
    if (!(D_F(DESCL) & PTR))
	goto L_GCMA3;
    if (D_A(DESCL) == 0)
	goto L_GCMA3;
    TOP(TOPCL,OFSET,DESCL);
    if (!(D_F(D_A(TOPCL)) & MARK))
	goto L_GCMA4;
L_GCMA3:
    D_A(BKDX) -= DESCR;
    if (D_A(BKDX) != 0)
	goto L_GCMA2;
    POP(BK1CL);
    if (D_A(BK1CL) == 0)
	BRANCH(RTN1)
    D_A(BKDX) = D_V(BK1CL);
    D_F(BKDX) = D_V(BKDX) = 0;
    goto L_GCMA2;
    /*_*/
L_GCMA4:
    D_A(BKDX) -= DESCR;
    if (D_A(BKDX) == 0)
	goto L_GCMA9;
    D_V(BK1CL) = D_A(BKDX);
    PUSH(BK1CL);
L_GCMA9:
    D(BK1CL) = D(TOPCL);
    D_F(D_A(BK1CL)) |= MARK;
    if (!(D_F(D_A(BK1CL)) & STTL))
	goto L_GCMA1;
    D(BKDX) = D(TWOCL);
    goto L_GCMA2;
    /*_*/
}
static int
SPLIT(retval)
    ret_t retval;
{
    ENTRY(SPLIT)
    POP(A4PTR);
    TOP(A5PTR,A6PTR,A4PTR);
    if (D_A(A6PTR) == 0)
	BRANCH(RTN1)
    D_A(A7PTR) = D_V(D_A(A5PTR));
    D_F(A7PTR) = D_V(A7PTR) = 0;
    D_A(A7PTR) -= D_A(A6PTR);
    D_A(A7PTR) -= DESCR;
    if (D_A(A7PTR) <= 0)
	BRANCH(RTN1)
    D_V(D_A(A5PTR)) = D_A(A6PTR);
    D_A(A4PTR) += DESCR;
    D(D_A(A4PTR)) = D(ZEROCL);
    D_A(D_A(A4PTR)) = D_A(A4PTR);
    D_F(D_A(A4PTR)) |= TTL;
    D_V(D_A(A4PTR)) = D_A(A7PTR);
    BRANCH(RTN1)
    /*_*/
}
static int
BINOP(retval)
    ret_t retval;
{
    ENTRY(BINOP)
    SAVSTK();
    switch (FORBLK(NORET)) {
    case 1:
	goto L_BINOP1;
    }
    if (D_A(BRTYPE) == EQTYP)
	goto L_BINOP2;
L_BINOP4:
    if (D_A(BRTYPE) != NBTYP)
	BRANCH(RTN2)
    if (D_A(SPITCL) != 0)
	goto L_BINOP5;
    if (D_A(BLOKCL) != 0)
	goto L_BINOP7;
    switch (STREAM(XSP, TEXTSP, &BIOPTB)) {
    case ST_ERROR:
	goto L_BINCON;
    }
L_BINOP3:
    D(ZPTR) = D(STYPE);
    BRANCH(RTZPTR)
    /*_*/
L_BINOP2:
    if (D_A(SPITCL) == 0)
	goto L_BINOP4;
    D_A(STYPE) = (int_t) BIEQFN;
    goto L_BINOP3;
    /*_*/
L_BINOP1:
    SAVSTK();
    switch (FORWRD(NORET)) {
    case 1:
	BRANCH(COMP3)
    }
    switch (D_A(BRTYPE)) {
    case 2:
    case 3:
    case 6:
    case 7:
	BRANCH(RTN2)
    }
    D_A(EMSGCL) = (int_t) ILLBIN;
    BRANCH(RTN1)
    /*_*/
L_BINCON:
    D(ZPTR) = D(CONCL);
    BRANCH(RTZPTR)
    /*_*/
L_BINOP5:
    if (D_A(BLOKCL) != 0)
	goto L_BINOP6;
    switch (STREAM(XSP, TEXTSP, &SBIPTB)) {
    case ST_ERROR:
	goto L_BINCON;
    }
    goto L_BINOP3;
    /*_*/
L_BINOP6:
    switch (STREAM(XSP, TEXTSP, &BSBIPTB)) {
    case ST_ERROR:
	goto L_BINCON;
    }
    goto L_BINOP3;
    /*_*/
L_BINOP7:
    switch (STREAM(XSP, TEXTSP, &BBIOPTB)) {
    case ST_ERROR:
	goto L_BINCON;
    }
    goto L_BINOP3;
    /*_*/
}
static int
CMPILE(retval)
    ret_t retval;
{
    ENTRY(CMPILE)
    D_A(BRTYPE) = 0;
    D(BOSCL) = D(CMOFCL);
    if (D_A(HIDECL) != 0)
	goto L_CMPIL0;
    D_A(CSTNCL)++;
L_CMPIL0:
    switch (STREAM(XSP, TEXTSP, &LBLTB)) {
    case ST_ERROR:
	goto L_CERR1;
    }
    if (S_L(XSP) == 0)
	goto L_CMPILA;
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(BASECL);
    D_A(CMBSCL) += D_A(CMOFCL);
    if (D_PTR(CMBSCL) < D_PTR(OCLIM))
	goto L_CMPILO;
    SAVSTK();
    PUSH(CODELT);
    BLOCK(XCL);
    D(D_A(CMBSCL)) = D(GOTGCL);
    D(D_A(CMBSCL) + DESCR) = D(LIT1CL);
    D(D_A(CMBSCL) + 2*DESCR) = D(XCL);
    D(CMBSCL) = D(XCL);
    D(OCLIM) = D(CMBSCL);
    D_A(OCLIM) += D_A(CODELT);
    D_A(OCLIM) -= 7*DESCR;
    D(D_A(CMBSCL) + DESCR) = D(BASECL);
    D_A(CMBSCL) += DESCR;
L_CMPILO:
    D_A(CMOFCL) = 0;
    D_A(BOSCL) = 0;
    SAVSTK();
    PUSH(XSPPTR);
    GENVUP(LPTR);
    if (D_A(D_A(LPTR) + ATTRIB) == (int_t)0)
	goto L_CMPILC;
    if (D_A(CNSLCL) == 0)
	goto L_CERR2;
L_CMPILC:
    D(D_A(LPTR) + ATTRIB) = D(CMBSCL);
    if (DCMP(LPTR, ENDPTR))
	BRANCH(RTN2)
L_CMPILA:
    SAVSTK();
    switch (FORBLK(NORET)) {
    case 1:
	goto L_CERR12;
    }
    if (D_A(BRTYPE) == EOSTYP)
	BRANCH(RTN3)
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(INITCL);
    D_A(CMOFCL) += DESCR;
    D(FRNCL) = D(CMOFCL);
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(LNNOCL);
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(FILENM);
    if (D_A(BRTYPE) == NBTYP)
	goto L_CMPSUB;
    if (D_A(BRTYPE) == CLNTYP)
	goto L_CMPGO;
    else
	goto L_CERR3;
    /*_*/
L_CMPSUB:
    SAVSTK();
    switch (ELEMNT(SUBJND)) {
    case 1:
	goto L_CDIAG;
    case 2:
	BRANCH(COMP3)
    }
    SAVSTK();
    switch (FORBLK(NORET)) {
    case 1:
	goto L_CERR5;
    }
    if (D_A(BRTYPE) != NBTYP)
	goto L_CMPSB1;
    if (D_A(SPITCL) == 0)
	goto L_CMPAT2;
    {
		/* Sigh; this was in assembler in SNOBOL4+		[PLB32] */
		register char *cp;		/*			[PLB33] */
		cp = S_SP(TEXTSP);		/*			[PLB33] */
		if (*cp == '?' &&		/* is break char '?'	[PLB32] */
		    (S_L(TEXTSP) == 1 ||	/* yes; is last char?	[PLB32] */
		     cp[1] == ' ' || cp[1] == '\t')) { /* or next is white? [PLB33] */
		    S_O(TEXTSP)++;		/* binary ? found.	[PLB32] */
		    S_L(TEXTSP)--;		/* remove from TEXTSP	[PLB32] */
		}				/*			[PLB32] */
    }
    goto L_CMPATN;
    /*_*/
L_CMPSB1:
    if (D_A(BRTYPE) == EQTYP)
	goto L_CMPFRM;
    SAVSTK();
    PUSH(SUBJND);
    TREPUB(NORET);
    if (D_A(BRTYPE) == CLNTYP)
	goto L_CMPGO;
    if (D_A(BRTYPE) == EOSTYP)
	goto L_CMPNGO;
    else
	goto L_CERR5;
    /*_*/
L_CMPATN:
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(EXOPND);
    D(D_A(EXOPND) + CODE) = D(SCANCL);
    D_V(D_A(EXOPND) + CODE) = D_V(ZEROCL);
    ADDSON(EXOPND,SUBJND);
    D(EXPRND) = D(SUBJND);
    SAVSTK();
    switch (EXPR1(PATND)) {
    case 1:
	goto L_CDIAG;
    case 2:
	goto L_CMPAT1;
    }
    /*_*/
L_CMPAT2:
    SAVSTK();
    switch (EXPR(PATND)) {
    case 1:
	goto L_CDIAG;
    }
    if (D_A(BRTYPE) == EQTYP)
	goto L_CMPASP;
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(SCANCL);
    SAVSTK();
    PUSH(SUBJND);
    TREPUB(NORET);
L_CMPAT1:
    SAVSTK();
    PUSH(PATND);
    TREPUB(NORET);
L_CMPTGO:
    if (D_A(BRTYPE) == EOSTYP)
	goto L_CMPNGO;
    if (D_A(BRTYPE) == CLNTYP)
	goto L_CMPGO;
    else
	goto L_CERR5;
    /*_*/
L_CMPFRM:
    SAVSTK();
    switch (EXPR(FORMND)) {
    case 1:
	goto L_CDIAG;
    }
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(ASGNCL);
    SAVSTK();
    PUSH(SUBJND);
    TREPUB(NORET);
    goto L_CMPFT;
    /*_*/
L_CMPASP:
    SAVSTK();
    switch (EXPR(FORMND)) {
    case 1:
	goto L_CDIAG;
    }
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(SJSRCL);
    SAVSTK();
    PUSH(SUBJND);
    TREPUB(NORET);
    SAVSTK();
    PUSH(PATND);
    TREPUB(NORET);
L_CMPFT:
    SAVSTK();
    PUSH(FORMND);
    switch (TREPUB(NORET)) {
    case 1:
	goto L_CMPTGO;
    }
    /*_*/
L_CMPNGO:
    D_V(CSTNCL) = D_A(CMOFCL);
    D(D_A(CMBSCL) + D_A(FRNCL)) = D(CSTNCL);
    D_V(CSTNCL) = I;
    BRANCH(RTN3)
    /*_*/
L_CMPGO:
    SAVSTK();
    switch (FORWRD(NORET)) {
    case 1:
	BRANCH(COMP3)
    }
    if (D_A(BRTYPE) == EOSTYP)
	goto L_CMPNGO;
    if (D_A(BRTYPE) != NBTYP)
	goto L_CERR11;
    switch (STREAM(XSP, TEXTSP, &GOTOTB)) {
    case ST_ERROR:
	goto L_CERR11;
    case ST_EOS:
	goto L_CERR12;
    }
    D(GOGOCL) = D(GOTLCL);
    D_A(GOBRCL) = (int_t) RPTYP;
    if (D_A(STYPE) <= D_A(GTOCL))
	goto L_CMPGG;
    D(GOGOCL) = D(GOTGCL);
    D_A(GOBRCL) = (int_t) RBTYP;
L_CMPGG:
    switch (D_A(STYPE)) {
    case 2:
	goto L_CMPSGO;
    case 3:
	goto L_CMPFGO;
    case 5:
	goto L_CMPSGO;
    case 6:
	goto L_CMPFGO;
    }
L_CMPUGO:
    D_V(CSTNCL) = D_A(CMOFCL);
    D(D_A(CMBSCL) + D_A(FRNCL)) = D(CSTNCL);
    D_V(CSTNCL) = I;
    SAVSTK();
    switch (EXPR(GOTOND)) {
    case 1:
	goto L_CDIAG;
    }
    if (D_A(BRTYPE) != D_A(GOBRCL))
	goto L_CERR11;
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(GOGOCL);
    SAVSTK();
    PUSH(GOTOND);
    TREPUB(NORET);
    SAVSTK();
    switch (FORWRD(NORET)) {
    case 1:
	BRANCH(COMP3)
    }
    if (D_A(BRTYPE) == EOSTYP)
	BRANCH(RTN3)
    else
	goto L_CERR11;
    /*_*/
L_CMPSGO:
    SAVSTK();
    switch (EXPR(SGOND)) {
    case 1:
	goto L_CDIAG;
    }
    if (D_A(BRTYPE) != D_A(GOBRCL))
	goto L_CERR11;
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(GOGOCL);
    SAVSTK();
    PUSH(SGOND);
    TREPUB(NORET);
    SAVSTK();
    switch (FORWRD(NORET)) {
    case 1:
	BRANCH(COMP3)
    }
    if (D_A(BRTYPE) != EOSTYP)
	goto L_CMPILL;
    D_V(CSTNCL) = D_A(CMOFCL);
    D(D_A(CMBSCL) + D_A(FRNCL)) = D(CSTNCL);
    D_V(CSTNCL) = I;
    BRANCH(RTN3)
    /*_*/
L_CMPILL:
    if (D_A(BRTYPE) != NBTYP)
	goto L_CERR11;
    switch (STREAM(XSP, TEXTSP, &GOTOTB)) {
    case ST_ERROR:
	goto L_CERR11;
    case ST_EOS:
	goto L_CERR12;
    }
    if (D_A(STYPE) != FGOTYP)
	goto L_CMPFTC;
    D(GOGOCL) = D(GOTLCL);
    D_A(GOBRCL) = (int_t) RPTYP;
    goto L_CMPUGO;
    /*_*/
L_CMPFTC:
    if (D_A(STYPE) != FTOTYP)
	goto L_CERR11;
    D(GOGOCL) = D(GOTGCL);
    D_A(GOBRCL) = (int_t) RBTYP;
    goto L_CMPUGO;
    /*_*/
L_CMPFGO:
    SAVSTK();
    switch (EXPR(FGOND)) {
    case 1:
	goto L_CDIAG;
    }
    if (D_A(BRTYPE) != D_A(GOBRCL))
	goto L_CERR11;
    SAVSTK();
    switch (FORWRD(NORET)) {
    case 1:
	BRANCH(COMP3)
    }
    if (D_A(BRTYPE) != EOSTYP)
	goto L_CMPILM;
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(GOTOCL);
    D_A(CMOFCL) += DESCR;
    D(SRNCL) = D(CMOFCL);
    D_V(CSTNCL) = D_A(CMOFCL);
    D(D_A(CMBSCL) + D_A(FRNCL)) = D(CSTNCL);
    D_V(CSTNCL) = I;
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(GOGOCL);
    SAVSTK();
    PUSH(FGOND);
    TREPUB(NORET);
    D(D_A(CMBSCL) + D_A(SRNCL)) = D(CMOFCL);
    BRANCH(RTN3)
    /*_*/
L_CMPILM:
    if (D_A(BRTYPE) != NBTYP)
	goto L_CERR11;
    switch (STREAM(XSP, TEXTSP, &GOTOTB)) {
    case ST_ERROR:
	goto L_CERR11;
    case ST_EOS:
	goto L_CERR12;
    }
    if (D_A(STYPE) != SGOTYP)
	goto L_CMPSTC;
    PUSH(GOTLCL);
    D_A(GOBRCL) = (int_t) RPTYP;
    goto L_CMPILN;
    /*_*/
L_CMPSTC:
    if (D_A(STYPE) != STOTYP)
	goto L_CERR11;
    PUSH(GOTGCL);
    D_A(GOBRCL) = (int_t) RBTYP;
L_CMPILN:
    SAVSTK();
    switch (EXPR(SGOND)) {
    case 1:
	goto L_CDIAG;
    }
    if (D_A(BRTYPE) != D_A(GOBRCL))
	goto L_CERR11;
    SAVSTK();
    switch (FORWRD(NORET)) {
    case 1:
	BRANCH(COMP3)
    }
    if (D_A(BRTYPE) != EOSTYP)
	goto L_CERR11;
    D_A(CMOFCL) += DESCR;
    POP(WCL);
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(WCL);
    SAVSTK();
    PUSH(SGOND);
    TREPUB(NORET);
    D_V(CSTNCL) = D_A(CMOFCL);
    D(D_A(CMBSCL) + D_A(FRNCL)) = D(CSTNCL);
    D_V(CSTNCL) = I;
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(GOGOCL);
    SAVSTK();
    PUSH(FGOND);
    switch (TREPUB(NORET)) {
    case 1:
	BRANCH(RTN3)
    }
    /*_*/
L_CERR1:
    D_A(EMSGCL) = (int_t) EMSG1;
    goto L_CDIAG;
    /*_*/
L_CERR2:
    D_A(EMSGCL) = (int_t) EMSG2;
    goto L_CDIAG;
    /*_*/
L_CERR3:
    D_A(EMSGCL) = (int_t) EMSG3;
    goto L_CDIAG;
    /*_*/
L_CERR5:
    D_A(EMSGCL) = (int_t) ILLBRK;
    goto L_CDIAG;
    /*_*/
L_CERR12:
    D_A(EMSGCL) = (int_t) ILLEOS;
    goto L_CDIAG;
    /*_*/
L_CERR11:
    D_A(EMSGCL) = (int_t) EMSG14;
L_CDIAG:
    D_A(BOSCL) += DESCR;
    D(D_A(CMBSCL) + D_A(BOSCL)) = D(ERORCL);
    D_A(BOSCL) += DESCR;
    D(D_A(CMBSCL) + D_A(BOSCL)) = D(CSTNCL);
    D_A(BOSCL) += DESCR;
    D(D_A(CMBSCL) + D_A(BOSCL)) = D(LNNOCL);
    D_A(BOSCL) += DESCR;
    D(D_A(CMBSCL) + D_A(BOSCL)) = D(FILENM);
    D(CMOFCL) = D(BOSCL);
    D_A(ESAICL) += DESCR;
    if (D_A(ESAICL) > D_A(ESALIM))
	BRANCH(COMP9)
    if (D_A(COMPCL) == 0)
	goto L_CDIAG4;
    {
		char *sp, *dp;		/* src ptr, desr ptr		[PLB40] */
		int len;		/* src char count		[PLB40] */
		sp = INBUF + STNOSZ;	/* get start of statement buffer [PLB40] */
		len = S_O(TEXTSP);	/* number of spaces/tabs to skip [PLB77] */
		S_L(ERRSP) = len + 1;	/* set pointer line length	[PLB77] */
		dp = ERRBUF + STNOSZ;	/* get start of marker buffer	[PLB40] */
		while (len-- > STNOSZ) { /* for all chars in statement	[PLB77] */
		    if (*sp++ == '\t')	/* if tab			[PLB77] */
			*dp++ = '\t';	/*	put tab in errbuf	[PLB40] */
		    else		/* else				[PLB40] */
			*dp++ = ' ';	/*	put space		[PLB40] */
		}			/*				[PLB40] */
		*dp++ = '^';		/* store pointer		[PLB40] */
    }
    IO_PRINT(IOKEY, ERRBLK, LNBFSP);
    IO_PRINT(IOKEY, ERRBLK, ERRSP);
    if (D_A(LISTCL) == 0)
	goto L_CDIAG2;
    IO_PRINT(IOKEY, OUTBLK, LNBFSP);
    IO_PRINT(IOKEY, OUTBLK, ERRSP);
L_CDIAG2:
    _SPEC(TSP) = _SPEC(D_A(EMSGCL));
    S_L(CERRSP) = 0;
    X_LOCSP(XSP,FILENM);
    APDSP(CERRSP,XSP);
    APDSP(CERRSP,COLSP);
    INTSPC(XSP,LNNOCL);
    APDSP(CERRSP,XSP);
    APDSP(CERRSP,COLSP);
    APDSP(CERRSP,SPCSP);
    APDSP(CERRSP,STARSP);
    APDSP(CERRSP,TSP);
    if (D_A(LISTCL) == 0)
	goto L_CDIAG1;
    IO_PRINT(IOKEY, OUTBLK, CERRSP);
    IO_PRINT(IOKEY, OUTBLK, BLSP);
L_CDIAG1:
    IO_PRINT(IOKEY, ERRBLK, CERRSP);
    IO_PRINT(IOKEY, ERRBLK, BLSP);
L_CDIAG4:
    SAVSTK();
    PUSH(EMSGCL);
    GENVAR(ERRTXT);
    if (D_A(UNIT) == 0)
	BRANCH(RTN1)
    if (D_A(BRTYPE) == EOSTYP)
	BRANCH(RTN3)
    switch (STREAM(XSP, TEXTSP, &EOSTB)) {
    case ST_ERROR:
	BRANCH(COMP3)
    case ST_STOP:
	BRANCH(RTN3)
    }
L_DIAGRN:
    _SPEC(TEXTSP) = _SPEC(NEXTSP);
    switch (IO_READ(UNIT, TEXTSP)) {
    case IO_EOF:
	goto L_DIAGIN;
    case IO_ERR:
	BRANCH(COMP1)
    }
    D_A(TMVAL) = S_L(TEXTSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D_A(TMVAL) += STNOSZ;
    S_L(LNBFSP) = D_A(TMVAL);
    D_A(LNNOCL)++;
    switch (STREAM(XSP, TEXTSP, &CARDTB)) {
    case ST_ERROR:
	BRANCH(COMP3)
    case ST_EOS:
	goto L_CDIAG7;
    }
    SAVSTK();
    switch (NEWCRD(NORET)) {
    case 3:
	BRANCH(RTN3)
    }
L_CDIAG7:
    if (D_A(LISTCL) == 0)
	goto L_DIAGRN;
    IO_PRINT(IOKEY, OUTBLK, LNBFSP);
    goto L_DIAGRN;
    /*_*/
L_DIAGIN:
    SAVSTK();
    FILCHK(NORET);
    goto L_DIAGRN;
    /*_*/
}
static int
ELEMNT(retval)
    ret_t retval;
{
    ENTRY(ELEMNT)
    SAVSTK();
    switch (UNOP(ELEMND)) {
    case 1:
	BRANCH(RTN2)
    }
    switch (STREAM(XSP, TEXTSP, &ELEMTB)) {
    case ST_ERROR:
	BRANCH(ELEICH)
    case ST_EOS:
	BRANCH(ELEILI)
    }
    /* FALL */
    BRANCH(ELEMN9)
}
static int
ELEMN9(retval)
    ret_t retval;
{
    ENTRY(ELEMN9)
    switch (D_A(STYPE)) {
    case 2:
	goto L_ELEILT;
    case 3:
	goto L_ELEVBL;
    case 4:
	goto L_ELENST;
    case 5:
	goto L_ELEFNC;
    case 6:
	goto L_ELEFLT;
    case 7:
	goto L_ELEARY;
    }
    S_L(XSP)--;
    S_O(XSP)++;
    S_L(XSP)--;
    SAVSTK();
    PUSH(XSPPTR);
    GENVAR(XPTR);
L_ELEMN5:
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(ELEXND);
    D(D_A(ELEXND) + CODE) = D(LITCL);
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(ELEYND);
    D(D_A(ELEYND) + CODE) = D(XPTR);
    ADDSON(ELEXND,ELEYND);
L_ELEMN1:
    if (D_A(ELEMND) != 0)
	goto L_ELEMN6;
    D(ZPTR) = D(ELEXND);
    goto L_ELEMRR;
    /*_*/
L_ELEMN6:
    ADDSON(ELEMND,ELEXND);
L_ELEMNR:
    D(ZPTR) = D(ELEMND);
L_ELEMRR:
    if (D_A(D_A(ZPTR) + FATHER) == (int_t)0)
	goto L_ELEM10;
    D(ZPTR) = D(D_A(ZPTR) + FATHER);
    goto L_ELEMRR;
    /*_*/
L_ELEM10:
    switch (STREAM(XSP, TEXTSP, &GOTSTB)) {
    case ST_ERROR:
    case ST_EOS:
	BRANCH(RTZPTR)
    }
    if (D_A(STYPE) == SGOTYP)
	BRANCH(ELECMA)
    D(ELEMND) = D(ZEROCL);
    D(ELEXND) = D(ZPTR);
    goto L_ELEAR2;
    /*_*/
L_ELEILT:
    if (SPCINT(XPTR,XSP))
	goto L_ELEMN5;
    else
	BRANCH(ELEINT)
    /*_*/
L_ELEFLT:
    if (SPREAL(XPTR,XSP))
	goto L_ELEMN5;
    else
	BRANCH(ELEDEC)
    /*_*/
L_ELEVBL:
    SAVSTK();
    PUSH(XSPPTR);
    GENVUP(XPTR);
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(ELEXND);
    D(D_A(ELEXND) + CODE) = D(XPTR);
    goto L_ELEMN1;
    /*_*/
L_ELENST:
    PUSH(ELEMND);
    SAVSTK();
    switch (EXPR(ELEXND)) {
    case 1:
	BRANCH(RTN1)
    }
    POP(ELEMND);
    if (D_A(BRTYPE) == RPTYP)
	goto L_ELEMN1;
    if (D_A(SPITCL) == 0)
	BRANCH(ELECMA)
    if (D_A(BRTYPE) != CMATYP)
	BRANCH(ELECMA)
    SAVSTK();
    PUSH(CMACL);
    switch (ELEARG(NORET)) {
    case 1:
	BRANCH(RTN1)
    }
    if (D_A(BRTYPE) == RPTYP)
	goto L_ELEMNR;
    else
	BRANCH(ELECMA)
    /*_*/
L_ELEFNC:
    S_L(XSP)--;
    SAVSTK();
    PUSH(XSPPTR);
    GENVUP(XPTR);
    SAVSTK();
    PUSH(XPTR);
    FINDEX(XCL);
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(ELEXND);
    D(D_A(ELEXND) + CODE) = D(XCL);
    if (D_A(ELEMND) == 0)
	goto L_ELEMN7;
    ADDSON(ELEMND,ELEXND);
L_ELEMN7:
    PUSH(ELEXND);
    SAVSTK();
    switch (EXPR(ELEXND)) {
    case 1:
	BRANCH(RTN1)
    }
    POP(ELEMND);
    ADDSON(ELEMND,ELEXND);
    D(ELEMND) = D(ELEXND);
L_ELEMN2:
    if (D_A(BRTYPE) == RPTYP)
	goto L_ELEMN3;
    if (D_A(BRTYPE) != CMATYP)
	BRANCH(ELECMA)
    PUSH(ELEMND);
    SAVSTK();
    switch (EXPR(ELEXND)) {
    case 1:
	BRANCH(RTN1)
    }
    POP(ELEMND);
    ADDSIB(ELEMND,ELEXND);
    D(ELEMND) = D(ELEXND);
    goto L_ELEMN2;
    /*_*/
L_ELEMN3:
    D(ELEXND) = D(D_A(ELEMND) + FATHER);
    D(XCL) = D(D_A(ELEXND) + CODE);
    D(YCL) = D(D_A(XCL));
    if ((D_F(YCL) & FNC))
	goto L_ELEMNR;
    D_A(XCL) = D_V(XCL);
    D_F(XCL) = D_V(XCL) = 0;
    D_A(YCL) = D_V(YCL);
    D_F(YCL) = D_V(YCL) = 0;
L_ELEMN4:
    if (D_A(XCL) >= D_A(YCL))
	goto L_ELEMNR;
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(ELEYND);
    D(D_A(ELEYND) + CODE) = D(LITCL);
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(ELEXND);
    D(D_A(ELEXND) + CODE) = D(NULVCL);
    ADDSON(ELEYND,ELEXND);
    ADDSIB(ELEMND,ELEYND);
    D(ELEMND) = D(ELEYND);
    D_A(XCL)++;
    goto L_ELEMN4;
    /*_*/
L_ELEARY:
    S_L(XSP)--;
    SAVSTK();
    PUSH(XSPPTR);
    GENVUP(XPTR);
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(ELEXND);
    D(D_A(ELEXND) + CODE) = D(XPTR);
L_ELEAR2:
    SAVSTK();
    PUSH(ITEMCL);
    switch (ELEARG(NORET)) {
    case 1:
	BRANCH(RTN1)
    }
    if (D_A(BRTYPE) == RBTYP)
	goto L_ELEMNR;
    else
	BRANCH(ELECMA)
    /*_*/
}
static int
ELEARG(retval)
    ret_t retval;
{
    ENTRY(ELEARG)
    POP(XCL);
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(XPTR);
    D(D_A(XPTR) + CODE) = D(XCL);
    if (D_A(ELEMND) == 0)
	goto L_ELEMN8;
    ADDSON(ELEMND,XPTR);
L_ELEMN8:
    ADDSON(XPTR,ELEXND);
    D(ELEMND) = D(ELEXND);
L_ELEAR1:
    PUSH(ELEMND);
    SAVSTK();
    switch (EXPR(ELEXND)) {
    case 1:
	BRANCH(RTN1)
    }
    POP(ELEMND);
    ADDSIB(ELEMND,ELEXND);
    D(ELEMND) = D(ELEXND);
    if (D_A(BRTYPE) == CMATYP)
	goto L_ELEAR1;
    else
	BRANCH(RTN2)
    /*_*/
}
static int
ELEICH(retval)
    ret_t retval;
{
    ENTRY(ELEICH)
    D_A(EMSGCL) = (int_t) ILCHAR;
    BRANCH(RTN1)
    /*_*/
}
static int
ELEILI(retval)
    ret_t retval;
{
    ENTRY(ELEILI)
    if (D_A(STYPE) == 0)
	BRANCH(ELEICH)
    if (D_A(STYPE) != QLITYP)
	BRANCH(ELEMN9)
    D_A(EMSGCL) = (int_t) OPNLIT;
    BRANCH(RTN1)
    /*_*/
}
static int
ELEINT(retval)
    ret_t retval;
{
    ENTRY(ELEINT)
    D_A(EMSGCL) = (int_t) ILLINT;
    BRANCH(RTN1)
    /*_*/
}
static int
ELEDEC(retval)
    ret_t retval;
{
    ENTRY(ELEDEC)
    D_A(EMSGCL) = (int_t) ILLDEC;
    BRANCH(RTN1)
    /*_*/
}
static int
ELECMA(retval)
    ret_t retval;
{
    ENTRY(ELECMA)
    D_A(EMSGCL) = (int_t) ILLBRK;
    BRANCH(RTN1)
    /*_*/
}
static int
EXPR(retval)
    ret_t retval;
{
    ENTRY(EXPR)
    SAVSTK();
    switch (ELEMNT(EXELND)) {
    case 1:
	BRANCH(RTN1)
    case 2:
	goto L_EXPNUL;
    }
    D_A(EXPRND) = 0;
    BRANCH(EXPR2)
    /*_*/
L_EXPNUL:
    SAVSTK();
    switch (NULNOD(XPTR)) {
    case 1:
	BRANCH(RTXNAM)
    }
    /*_*/
}
static int
EXPR1(retval)
    ret_t retval;
{
    ENTRY(EXPR1)
    PUSH(EXPRND);
    SAVSTK();
    switch (ELEMNT(EXELND)) {
    case 1:
	BRANCH(RTN1)
    case 2:
	goto L_EXPR12;
    }
    POP(EXPRND);
    BRANCH(EXPR2)
    /*_*/
L_EXPR12:
    if (D_A(SPITCL) == 0)
	goto L_EXPERR;
    if (D_A(D_A(EXOPND) + CODE) == (int_t)BISRFN)
	goto L_EXPR17;
    if (D_A(D_A(EXOPND) + CODE) != (int_t)BIEQFN)
	goto L_EXPERR;
L_EXPR17:
    SAVSTK();
    switch (NULNOD(EXELND)) {
    case 1:
	goto L_EXPR13;
    }
    /*_*/
L_EXPR13:
    POP(EXPRND);
    BRANCH(EXPR7)
    /*_*/
L_EXPERR:
    D_A(EMSGCL) = (int_t) ILLEOS;
    BRANCH(RTN1)
    /*_*/
}
static int
EXPR2(retval)
    ret_t retval;
{
    ENTRY(EXPR2)
    SAVSTK();
    switch (BINOP(EXOPCL)) {
    case 1:
	BRANCH(RTN1)
    case 2:
	BRANCH(EXPR7)
    }
    if (D_A(SPITCL) == 0)
	goto L_EXPR14;
    if (D_A(EXOPCL) != D_A(ASGNCL))
	goto L_EXPR14;
    if (D_A(EXPRND) == 0)
	goto L_EXPR14;
    D(EXOPND) = D(EXPRND);
L_EXPR15:
    D(EXEXND) = D(EXOPND);
    if (D_A(D_A(EXEXND) + FATHER) == (int_t)0)
	goto L_EXPR14;
    D(EXOPND) = D(D_A(EXEXND) + FATHER);
    if (D_A(D_A(EXOPND) + CODE) != (int_t)BISNFN)
	goto L_EXPR15;
    D_A(D_A(EXOPND) + CODE) = D_A(SJSRCL);
    ADDSIB(EXPRND,EXELND);
    D(EXPRND) = D(EXELND);
L_EXPR16:
    D(EXOPND) = D(D_A(EXPRND) + FATHER);
    if (D_A(D_A(EXOPND) + FATHER) == (int_t)0)
	BRANCH(EXPR1)
    D(EXPRND) = D(EXOPND);
    goto L_EXPR16;
    /*_*/
L_EXPR14:
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(EXOPND);
    D(D_A(EXOPND) + CODE) = D(EXOPCL);
    if (D_A(EXPRND) != 0)
	goto L_EXPR3;
    ADDSON(EXOPND,EXELND);
    D(EXPRND) = D(EXELND);
    BRANCH(EXPR1)
    /*_*/
L_EXPR3:
    D(EXOPCL) = D(D_A(EXOPCL) + 2*DESCR);
    D_A(EXOPCL) = D_V(EXOPCL);
    D_F(EXOPCL) = D_V(EXOPCL) = 0;
    D(EXEXND) = D(D_A(EXPRND) + FATHER);
    D(XPTR) = D(D_A(EXEXND) + CODE);
    D(XPTR) = D(D_A(XPTR) + 2*DESCR);
    if (D_A(XPTR) > D_A(EXOPCL))
	goto L_EXPR4;
    ADDSIB(EXPRND,EXOPND);
    D(EXPRND) = D(EXOPND);
    ADDSON(EXPRND,EXELND);
    D(EXPRND) = D(EXELND);
    BRANCH(EXPR1)
    /*_*/
L_EXPR4:
    ADDSIB(EXPRND,EXELND);
L_EXPR5:
    if (D_A(D_A(EXPRND) + FATHER) == (int_t)0)
	goto L_EXPR11;
    D(EXPRND) = D(D_A(EXPRND) + FATHER);
    if (D_A(D_A(EXPRND) + FATHER) == (int_t)0)
	goto L_EXPR11;
    D(EXEXND) = D(D_A(EXPRND) + FATHER);
    D(XPTR) = D(D_A(EXEXND) + CODE);
    D(XPTR) = D(D_A(XPTR) + 2*DESCR);
    if (D_A(XPTR) > D_A(EXOPCL))
	goto L_EXPR5;
    X_INSERT(EXPRND,EXOPND);
    BRANCH(EXPR1)
    /*_*/
L_EXPR11:
    ADDSON(EXOPND,EXPRND);
    BRANCH(EXPR1)
    /*_*/
}
static int
EXPR7(retval)
    ret_t retval;
{
    ENTRY(EXPR7)
    if (D_A(EXPRND) != 0)
	goto L_EXPR10;
    D(XPTR) = D(EXELND);
    goto L_EXPR9;
    /*_*/
L_EXPR10:
    ADDSIB(EXPRND,EXELND);
    D(XPTR) = D(EXPRND);
L_EXPR9:
    if (D_A(D_A(XPTR) + FATHER) == (int_t)0)
	BRANCH(RTXNAM)
    D(XPTR) = D(D_A(XPTR) + FATHER);
    goto L_EXPR9;
    /*_*/
}
static int
NULNOD(retval)
    ret_t retval;
{
    ENTRY(NULNOD)
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(EXPRND);
    D(D_A(EXPRND) + CODE) = D(LITCL);
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(EXEXND);
    D(D_A(EXEXND) + CODE) = D(NULVCL);
    ADDSON(EXPRND,EXEXND);
    D(retval) = D(EXPRND);
    RETURN(1)
    /*_*/
}
static int
FORWRD(retval)
    ret_t retval;
{
    ENTRY(FORWRD)
    switch (STREAM(XSP, TEXTSP, &FRWDTB)) {
    case ST_ERROR:
	BRANCH(COMP3)
    case ST_EOS:
	BRANCH(FORRUN)
    }
    /* FALL */
    BRANCH(FORJRN)
}
static int
FORJRN(retval)
    ret_t retval;
{
    ENTRY(FORJRN)
    D(BRTYPE) = D(STYPE);
    BRANCH(RTN2)
    /*_*/
}
static int
FORRUN(retval)
    ret_t retval;
{
    ENTRY(FORRUN)
L_FORRN0:
    if (D_A(UNIT) == 0)
	goto L_FOREOS;
    if (D_A(LISTCL) == 0)
	goto L_FORRUR;
    IO_PRINT(IOKEY, OUTBLK, LNBFSP);
L_FORRUR:
    _SPEC(TEXTSP) = _SPEC(NEXTSP);
    switch (IO_READ(UNIT, TEXTSP)) {
    case IO_EOF:
	goto L_FORINC;
    case IO_ERR:
	BRANCH(COMP1)
    }
    D_A(TMVAL) = S_L(TEXTSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D_A(TMVAL) += STNOSZ;
    S_L(LNBFSP) = D_A(TMVAL);
    D_A(LNNOCL)++;
    switch (STREAM(XSP, TEXTSP, &CARDTB)) {
    case ST_ERROR:
	BRANCH(COMP3)
    case ST_EOS:
	goto L_FORRN0;
    }
    SAVSTK();
    switch (NEWCRD(NORET)) {
    case 1:
	goto L_FORRN0;
    case 2:
	BRANCH(FORWRD)
    }
L_FOREOS:
    D(BRTYPE) = D(EOSCL);
    BRANCH(RTN2)
    /*_*/
L_FORINC:
    SAVSTK();
    FILCHK(NORET);
    goto L_FORRUR;
    /*_*/
}
static int
FORBLK(retval)
    ret_t retval;
{
    ENTRY(FORBLK)
    switch (STREAM(XSP, TEXTSP, &IBLKTB)) {
    case ST_ERROR:
	BRANCH(RTN1)
    case ST_EOS:
	BRANCH(FORRUN)
    case ST_STOP:
	BRANCH(FORJRN)
    }
    /*_*/
}
static int
FILCHK(retval)
    ret_t retval;
{
    ENTRY(FILCHK)
    if (D_A(INCSTK) == 0)
	goto L_FILCK2;
    D(LNNOCL) = D(D_A(INCSTK) + 2*DESCR);
    D(FILENM) = D(D_A(INCSTK) + 3*DESCR);
    D(INCSTK) = D(D_A(INCSTK) + DESCR);
    BRANCH(RTN2)
    /*_*/
L_FILCK2:
    if (!IO_FILE(UNIT,XSP))
	BRANCH(RTN1)
    SAVSTK();
    PUSH(XSPPTR);
    GENVAR(FILENM);
    D_A(LNNOCL) = 0;
    BRANCH(RTN1)
    /*_*/
}
static int
NEWCRD(retval)
    ret_t retval;
{
    ENTRY(NEWCRD)
    switch (D_A(STYPE)) {
    case 2:
	goto L_CMTCRD;
    case 3:
	goto L_CTLCRD;
    case 4:
	goto L_CNTCRD;
    }
    if (D_A(LISTCL) == 0)
	BRANCH(RTN3)
    D(XCL) = D(CSTNCL);
    D_A(XCL)++;
    INTSPC(TSP,XCL);
    if (D_A(LLIST) != 0)
	goto L_CARDL;
    IO_PAD(LNBFSP,CARDSZ+DSTSZ);
    S_L(RNOSP) = 0;
    APDSP(RNOSP,TSP);
    BRANCH(RTN3)
    /*_*/
L_CARDL:
    S_L(LNOSP) = 0;
    APDSP(LNOSP,TSP);
    BRANCH(RTN3)
    /*_*/
L_CMTCRD:
    if (D_A(LISTCL) == 0)
	BRANCH(RTN1)
L_CMTCLR:
    S_L(LNOSP) = 0;
    S_L(RNOSP) = 0;
    APDSP(LNOSP,BLNSP);
    APDSP(RNOSP,BLNSP);
    BRANCH(RTN1)
    /*_*/
L_CNTCRD:
    S_L(TEXTSP)--;
    S_O(TEXTSP)++;
    if (D_A(LISTCL) == 0)
	BRANCH(RTN2)
    INTSPC(TSP,CSTNCL);
    if (D_A(LLIST) != 0)
	goto L_CARDLL;
    IO_PAD(LNBFSP,CARDSZ+DSTSZ);
    S_L(RNOSP) = 0;
    APDSP(RNOSP,TSP);
    BRANCH(RTN2)
    /*_*/
L_CARDLL:
    S_L(LNOSP) = 0;
    APDSP(LNOSP,TSP);
    BRANCH(RTN2)
    /*_*/
L_CTLCRD:
    S_L(TEXTSP)--;
    S_O(TEXTSP)++;
    switch (STREAM(XSP, TEXTSP, &FRWDTB)) {
    case ST_ERROR:
	BRANCH(COMP3)
    case ST_EOS:
	goto L_CMTCRD;
    }
    if (D_A(STYPE) != NBTYP)
	goto L_CMTCRD;
    switch (STREAM(XSP, TEXTSP, &LBLXTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_CMTCLR;
    }
    RAISE1(XSP);
    if (LEXEQ(XSP,UNLSP))
	goto L_UNLIST;
    if (LEXEQ(XSP,LISTSP))
	goto L_LIST;
    if (LEXEQ(XSP,EJCTSP))
	goto L_EJECT;
    if (LEXEQ(XSP,ERORSP))
	goto L_ERRORS;
    if (LEXEQ(XSP,NERRSP))
	goto L_NOERRS;
    if (LEXEQ(XSP,CASESP))
	goto L_CASE;
    if (LEXEQ(XSP,INCLSP))
	goto L_INCLUD;
    if (LEXEQ(XSP,COPYSP))
	goto L_INCLUD;
    if (LEXEQ(XSP,SPITSP))
	goto L_PLSOPS;
    if (LEXEQ(XSP,EXECSP))
	goto L_EXEC;
    if (LEXEQ(XSP,NEXESP))
	goto L_NOEXEC;
    if (LEXEQ(XSP,LINESP))
	goto L_LINE;
    if (LEXEQ(XSP,HIDESP))
	goto L_HIDE;
    if (LEXEQ(XSP,BLKSSP))
	goto L_BLOCKS;
    if (LEXEQ(XSP,NOBLSP))
	goto L_NOBLKS;
    goto L_CMTCLR;
    /*_*/
L_HIDE:
    D_A(HIDECL) = 1;
L_UNLIST:
    D_A(LISTCL) = 0;
    BRANCH(RTN1)
    /*_*/
L_LIST:
    D_A(LISTCL) = 1;
    D_A(HIDECL) = 0;
    switch (STREAM(XSP, TEXTSP, &FRWDTB)) {
    case ST_ERROR:
	BRANCH(COMP3)
    case ST_EOS:
	goto L_CMTCLR;
    }
    if (D_A(STYPE) != NBTYP)
	goto L_CMTCLR;
    switch (STREAM(XSP, TEXTSP, &LBLXTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_CMTCLR;
    }
    RAISE1(XSP);
    if (!LEXEQ(XSP,LEFTSP))
	goto L_CTLCR2;
    D_A(LLIST) = 1;
    goto L_CMTCLR;
    /*_*/
L_CTLCR2:
    D_A(LLIST) = 0;
    goto L_CMTCLR;
    /*_*/
L_EJECT:
    if (D_A(LISTCL) == 0)
	goto L_CMTCLR;
    io_printf(D_A(OUTPUT),EJECTF);
    goto L_CMTCLR;
    /*_*/
L_ERRORS:
    D_A(NERRCL) = 0;
    goto L_CMTCLR;
    /*_*/
L_NOERRS:
    D_A(NERRCL) = 1;
    goto L_CMTCLR;
    /*_*/
L_CASE:
    switch (STREAM(XSP, TEXTSP, &FRWDTB)) {
    case ST_ERROR:
	BRANCH(COMP3)
    case ST_EOS:
	goto L_CASE1;
    }
    if (D_A(STYPE) != NBTYP)
	goto L_CMTCLR;
    switch (STREAM(XSP, TEXTSP, &ELEMTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_CMTCLR;
    }
    if (D_A(STYPE) != ILITYP)
	goto L_CMTCLR;
    SPCINT(CASECL,XSP);
    goto L_CMTCLR;
    /*_*/
L_CASE1:
    D_A(CASECL) = 0;
    goto L_CMTCLR;
    /*_*/
L_INCLUD:
    SAVSTK();
    switch (CTLADV(NORET)) {
    case 1:
    case 2:
	goto L_COMP10;
    }
    switch (IO_INCLUDE(UNIT, XSP)) {
    case INC_FAIL:
	goto L_COMP11;
    case INC_SKIP:
	goto L_CMTCLR;
    }
    TRIMSP(XSP,XSP);
    SAVSTK();
    PUSH(INCLSZ);
    BLOCK(XCL);
    D(D_A(XCL) + DESCR) = D(INCSTK);
    D(D_A(XCL) + 2*DESCR) = D(LNNOCL);
    D(D_A(XCL) + 3*DESCR) = D(FILENM);
    D(INCSTK) = D(XCL);
    SAVSTK();
    PUSH(XSPPTR);
    GENVAR(FILENM);
    D_A(LNNOCL) = 0;
    goto L_CMTCLR;
    /*_*/
L_COMP10:
    D_A(ERRTYP) = 29;
    BRANCH(FTLEND)
    /*_*/
L_COMP11:
    D_A(ERRTYP) = 30;
    BRANCH(FTLEND)
    /*_*/
L_PLSOPS:
    switch (STREAM(XSP, TEXTSP, &FRWDTB)) {
    case ST_ERROR:
	BRANCH(COMP3)
    case ST_EOS:
	goto L_PLSOP2;
    }
    if (D_A(STYPE) != NBTYP)
	goto L_CMTCLR;
    switch (STREAM(XSP, TEXTSP, &INTGTB)) {
    case ST_ERROR:
	goto L_CMTCLR;
    case ST_EOS:
	goto L_PLSOP1;
    }
    if (D_A(STYPE) != ILITYP)
	goto L_CMTCLR;
L_PLSOP1:
    SPCINT(SPITCL,XSP);
    goto L_PLSOP3;
    /*_*/
L_PLSOP2:
    D_A(SPITCL) = 0;
L_PLSOP3:
    goto L_CMTCLR;
    /*_*/
L_EXEC:
    D_A(EXECCL) = 1;
    goto L_CMTCLR;
    /*_*/
L_NOEXEC:
    D_A(EXECCL) = 0;
    goto L_CMTCLR;
    /*_*/
L_LINE:
    switch (STREAM(XSP, TEXTSP, &FRWDTB)) {
    case ST_ERROR:
	BRANCH(COMP3)
    case ST_EOS:
	goto L_CMTCLR;
    }
    if (D_A(STYPE) != NBTYP)
	goto L_CMTCLR;
    switch (STREAM(XSP, TEXTSP, &INTGTB)) {
    case ST_ERROR:
	goto L_COMP12;
    case ST_EOS:
	goto L_LINE1;
    }
    if (D_A(STYPE) != ILITYP)
	goto L_COMP12;
L_LINE1:
    if (!SPCINT(LNNOCL,XSP))
	goto L_COMP12;
    D_A(LNNOCL)--;
    SAVSTK();
    switch (CTLADV(NORET)) {
    case 1:
	goto L_CMTCLR;
    case 2:
	goto L_COMP12;
    }
    SAVSTK();
    PUSH(XSPPTR);
    GENVAR(FILENM);
    goto L_CMTCLR;
    /*_*/
L_COMP12:
    D_A(ERRTYP) = 31;
    BRANCH(FTLEND)
    /*_*/
L_BLOCKS:
    D_A(BLOKCL) = 1;
    goto L_CMTCLR;
    /*_*/
L_NOBLKS:
    D_A(BLOKCL) = 0;
    goto L_CMTCLR;
    /*_*/
}
static int
CTLADV(retval)
    ret_t retval;
{
    ENTRY(CTLADV)
    switch (STREAM(XSP, TEXTSP, &FRWDTB)) {
    case ST_ERROR:
	BRANCH(COMP3)
    case ST_EOS:
	BRANCH(RTN1)
    }
    if (D_A(STYPE) != NBTYP)
	BRANCH(RTN1)
    switch (STREAM(XSP, TEXTSP, &ELEMTB)) {
    case ST_ERROR:
    case ST_EOS:
	BRANCH(RTN2)
    }
    if (D_A(STYPE) != QLITYP)
	BRANCH(RTN2)
    S_L(XSP)--;
    S_O(XSP)++;
    S_L(XSP)--;
    BRANCH(RTN3)
    /*_*/
}
static int
TREPUB(retval)
    ret_t retval;
{
    ENTRY(TREPUB)
    POP(YPTR);
L_TREPU1:
    D(XPTR) = D(D_A(YPTR) + CODE);
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(XPTR);
    D(ZPTR) = D(CMBSCL);
    D_A(ZPTR) += D_A(CMOFCL);
    if (D_PTR(ZPTR) > D_PTR(OCLIM))
	goto L_TREPU5;
L_TREPU4:
    if (D_A(D_A(YPTR) + LSON) == (int_t)0)
	goto L_TREPU2;
    D(YPTR) = D(D_A(YPTR) + LSON);
    goto L_TREPU1;
    /*_*/
L_TREPU2:
    if (D_A(D_A(YPTR) + RSIB) == (int_t)0)
	goto L_TREPU3;
    D(YPTR) = D(D_A(YPTR) + RSIB);
    goto L_TREPU1;
    /*_*/
L_TREPU3:
    if (D_A(D_A(YPTR) + FATHER) == (int_t)0)
	BRANCH(RTN1)
    D(YPTR) = D(D_A(YPTR) + FATHER);
    goto L_TREPU2;
    /*_*/
L_TREPU5:
    D(ZPTR) = D(CMOFCL);
    D_A(ZPTR) += D_A(CODELT);
    D_V(ZPTR) = C;
    SAVSTK();
    PUSH(ZPTR);
    BLOCK(XCL);
    if (D_A(LPTR) == 0)
	goto L_TREPU6;
    D(D_A(LPTR) + ATTRIB) = D(XCL);
L_TREPU6:
    MOVBLK(D_A(XCL),D_A(CMBSCL),D_A(CMOFCL));
    D(D_A(CMBSCL) + DESCR) = D(GOTGCL);
    D(D_A(CMBSCL) + 2*DESCR) = D(LIT1CL);
    D(D_A(CMBSCL) + 3*DESCR) = D(XCL);
    D_A(CMBSCL) += 3*DESCR;
    SAVSTK();
    PUSH(CMBSCL);
    SPLIT(NORET);
    D(CMBSCL) = D(XCL);
    D(OCLIM) = D(CMBSCL);
    D_A(OCLIM) += D_A(ZPTR);
    D_A(OCLIM) -= 7*DESCR;
    goto L_TREPU4;
    /*_*/
}
static int
UNOP(retval)
    ret_t retval;
{
    ENTRY(UNOP)
    SAVSTK();
    switch (FORWRD(NORET)) {
    case 1:
	BRANCH(COMP3)
    }
    D_A(XPTR) = 0;
    if (D_A(BRTYPE) != NBTYP)
	BRANCH(RTN1)
L_UNOPA:
    switch (STREAM(XSP, TEXTSP, &UNOPTB)) {
    case ST_ERROR:
    case ST_EOS:
	BRANCH(RTXNAM)
    }
    SAVSTK();
    PUSH(CNDSIZ);
    BLOCK(YPTR);
    D(D_A(YPTR) + CODE) = D(STYPE);
    if (D_A(XPTR) == 0)
	goto L_UNOPB;
    ADDSON(XPTR,YPTR);
L_UNOPB:
    D(XPTR) = D(YPTR);
    goto L_UNOPA;
    /*_*/
}
int
BASE(retval)
    ret_t retval;
{
    ENTRY(BASE)
    D_A(OCBSCL) += D_A(OCICL);
    D_A(OCICL) = 0;
    BRANCH(RTNUL3)
    /*_*/
}
int
GOTG(retval)
    ret_t retval;
{
    ENTRY(GOTG)
    SAVSTK();
    switch (ARGVAL(OCBSCL)) {
    case 1:
	BRANCH(INTR5)
    }
    if (D_V(OCBSCL) != C)
	BRANCH(INTR4)
    D_A(OCICL) = 0;
    BRANCH(RTNUL3)
    /*_*/
}
int
GOTL(retval)
    ret_t retval;
{
    ENTRY(GOTL)
    D_A(OCICL) += DESCR;
    D(XPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(XPTR) & FNC))
	goto L_GOTLC;
L_GOTLV:
    if (D_A(TRAPCL) <= 0)
	goto L_GOTLV1;
    if (!LOCAPT(ATPTR,TLABL,XPTR))
	goto L_GOTLV1;
    PUSH(XPTR);
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
    POP(XPTR);
L_GOTLV1:
    if (!DCMP(XPTR, RETCL))
	goto L_GOTL1;
    RETURN(6)
    /*_*/
L_GOTL1:
    if (!DCMP(XPTR, FRETCL))
	goto L_GOTL2;
    RETURN(4)
    /*_*/
L_GOTL2:
    if (!DCMP(XPTR, NRETCL))
	goto L_GOTL3;
    RETURN(5)
    /*_*/
L_GOTL3:
    if (!DCMP(XPTR, ABORCL))
	goto L_GOTL4;
    if (D_A(XOCBSC) == 0)
	BRANCH(CNTERR)
    D(ERRTYP) = D(XERRTY);
    D(FILENM) = D(XFILEN);
    D(LNNOCL) = D(XLNNOC);
    D(STNOCL) = D(XSTNOC);
    BRANCH(FTLEND)
    /*_*/
L_GOTL4:
    if (!DCMP(XPTR, SCNTCL))
	goto L_GOTL5;
    D(FRTNCL) = D(XOCICL);
    goto L_GOTL5B;
    /*_*/
L_GOTL5:
    if (!DCMP(XPTR, CONTCL))
	goto L_GOTL6;
    D(FRTNCL) = D(XFRTNC);
L_GOTL5B:
    if (D_A(XOCBSC) == 0)
	BRANCH(CNTERR)
    D(OCBSCL) = D(XOCBSC);
    D(FILENM) = D(XFILEN);
    D(LNNOCL) = D(XLNNOC);
    D(STNOCL) = D(XSTNOC);
    D(LSFLNM) = D(XLSFLN);
    D(LSLNCL) = D(XLSLNC);
    D(LSTNCL) = D(XLNNOC);
    D_A(XOCBSC) = 0;
    if (D_A(ERRTYP) == 0)
	BRANCH(END0)
    D(ERRTYP) = D(XERRTY);
    BRANCH(FAIL)
    /*_*/
L_GOTL6:
    D(OCBSCL) = D(D_A(XPTR) + ATTRIB);
    if (D_A(OCBSCL) == 0)
	BRANCH(INTR4)
    D_A(OCICL) = 0;
    BRANCH(RTNUL3)
    /*_*/
L_GOTLC:
    SAVSTK();
    PUSH(XPTR);
    switch (INVOKE(XPTR)) {
    case 1:
	BRANCH(INTR5)
    case 3:
	BRANCH(INTR4)
    }
    if (D_V(XPTR) == S)
	goto L_GOTLV;
    else
	BRANCH(INTR4)
    /*_*/
}
int
GOTO(retval)
    ret_t retval;
{
    ENTRY(GOTO)
    D_A(OCICL) += DESCR;
    D(OCICL) = D(D_A(OCBSCL) + D_A(OCICL));
    BRANCH(RTNUL3)
    /*_*/
}
int
INIT(retval)
    ret_t retval;
{
    ENTRY(INIT)
    D(LSTNCL) = D(STNOCL);
    D_A(LSFLNM) = D_A(FILENM);
    D_A(LSLNCL) = D_A(LNNOCL);
    if (D_A(UINTCL) != 0)
	BRANCH(USRINT)
    D_A(OCICL) += DESCR;
    D(XCL) = D(D_A(OCBSCL) + D_A(OCICL));
    D_A(STNOCL) = D_A(XCL);
    D_A(FRTNCL) = D_V(XCL);
    D_F(FRTNCL) = D_V(FRTNCL) = 0;
    D_A(OCICL) += DESCR;
    D(LNNOCL) = D(D_A(OCBSCL) + D_A(OCICL));
    D_A(OCICL) += DESCR;
    D(FILENM) = D(D_A(OCBSCL) + D_A(OCICL));
    D_A(EXN2CL)++;
    if (D_A(EXLMCL) < 0)
	BRANCH(RTNUL3)
    if (D_A(EXNOCL) >= D_A(EXLMCL))
	BRANCH(EXEX)
    D_A(EXNOCL)++;
    if (D_A(TRAPCL) <= 0)
	BRANCH(RTNUL3)
    if (!chk_break(0))
	goto L_INIT1;
    if (!LOCAPT(ATPTR,TKEYL,STNOKY))
	goto L_INIT1;
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
L_INIT1:
    if (!LOCAPT(ATPTR,TKEYL,STCTKY))
	BRANCH(RTNUL3)
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
    BRANCH(RTNUL3)
    /*_*/
}
static int
INTERP(retval)
    ret_t retval;
{
    ENTRY(INTERP)
L_INTRP0:
    D_A(OCICL) += DESCR;
    D(XPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if (!(D_F(XPTR) & FNC))
	goto L_INTRP0;
    SAVSTK();
    PUSH(XPTR);
    switch (INVOKE(XPTR)) {
    case 2:
    case 3:
	goto L_INTRP0;
    case 4:
	BRANCH(RTN1)
    case 5:
	BRANCH(RTN2)
    case 6:
	BRANCH(RTN3)
    }
    D(OCICL) = D(FRTNCL);
    D_A(FALCL)++;
    if (D_A(TRAPCL) <= 0)
	goto L_INTRP0;
    if (!LOCAPT(ATPTR,TKEYL,FALKY))
	goto L_INTRP0;
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
    goto L_INTRP0;
    /*_*/
}
static int
INVOKE(retval)
    ret_t retval;
{
    ENTRY(INVOKE)
    POP(INCL);
    D(XPTR) = D(D_A(INCL));
    if (D_V(INCL) != D_V(XPTR))
	goto L_INVK2;
L_INVK1:
    BRANCH( ((func_t) (D_A(D_A(INCL)))))
    /*_*/
L_INVK2:
    if ((D_F(XPTR) & FNC))
	goto L_INVK1;
    else
	BRANCH(ARGNER)
    /*_*/
}
static int
ARGVAL(retval)
    ret_t retval;
{
    ENTRY(ARGVAL)
    D_A(OCICL) += DESCR;
    D(XPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(XPTR) & FNC))
	goto L_ARGVC;
L_ARGV1:
    if (D_A(INSW) == 0)
	goto L_ARGV2;
    if (!LOCAPV(ZPTR,INATL,XPTR))
	goto L_ARGV2;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    SAVSTK();
    PUSH(XPTR);
    PUSH(ZPTR);
    switch (PUTIN(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	BRANCH(RTXNAM)
    }
    /*_*/
L_ARGVC:
    SAVSTK();
    PUSH(XPTR);
    switch (INVOKE(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_ARGV1;
    case 3:
	BRANCH(RTXNAM)
    }
    /*_*/
L_ARGV2:
    D(XPTR) = D(D_A(XPTR) + DESCR);
    BRANCH(RTXNAM)
    /*_*/
}
static int
EXPVAL(retval)
    ret_t retval;
{
    ENTRY(EXPVAL)
    D_A(SCL) = 1;
    /* FALL */
    BRANCH(EXPVJN)
}
static int
EXPVJN(retval)
    ret_t retval;
{
    ENTRY(EXPVJN)
    POP(XPTR);
    /* FALL */
    BRANCH(EXPVJ2)
}
static int
EXPVJ2(retval)
    ret_t retval;
{
    ENTRY(EXPVJ2)
    PUSH(OCBSCL);
    PUSH(OCICL);
    PUSH(PATBCL);
    PUSH(PATICL);
    PUSH(WPTR);
    PUSH(XCL);
    PUSH(YCL);
    PUSH(TCL);
    PUSH(MAXLEN);
    PUSH(LENFCL);
    PUSH(PDLPTR);
    PUSH(PDLHED);
    PUSH(NAMICL);
    PUSH(NHEDCL);
    SPUSH(HEADSP);
    SPUSH(TSP);
    SPUSH(TXSP);
    SPUSH(XSP);
    D(OCBSCL) = D(XPTR);
    D_A(OCICL) = (int_t) DESCR;
    D(PDLHED) = D(PDLPTR);
    D(NHEDCL) = D(NAMICL);
    D(XPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(XPTR) & FNC))
	goto L_EXPVC;
L_EXPV11:
    if (D_A(SCL) == 0)
	goto L_EXPV6;
    if (D_A(INSW) == 0)
	goto L_EXPV4;
    if (!LOCAPV(ZPTR,INATL,XPTR))
	goto L_EXPV4;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    SAVSTK();
    PUSH(XPTR);
    PUSH(ZPTR);
    switch (PUTIN(XPTR)) {
    case 1:
	goto L_EXPV1;
    case 2:
	goto L_EXPV6;
    }
    /*_*/
L_EXPV4:
    D(XPTR) = D(D_A(XPTR) + DESCR);
L_EXPV6:
    D_A(SCL) = 2;
    goto L_EXPV7;
    /*_*/
L_EXPV9:
    POP(SCL);
L_EXPV1:
    D_A(SCL) = 1;
L_EXPV7:
    SPOP(XSP);
    SPOP(TXSP);
    SPOP(TSP);
    SPOP(HEADSP);
    POP(NHEDCL);
    POP(NAMICL);
    POP(PDLHED);
    POP(PDLPTR);
    POP(LENFCL);
    POP(MAXLEN);
    POP(TCL);
    POP(YCL);
    POP(XCL);
    POP(WPTR);
    POP(PATICL);
    POP(PATBCL);
    POP(OCICL);
    POP(OCBSCL);
    switch (D_A(SCL)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	BRANCH(RTXNAM)
    case 3:
	BRANCH(RTZPTR)
    }
    /*_*/
L_EXPVC:
    PUSH(SCL);
    SAVSTK();
    PUSH(XPTR);
    switch (INVOKE(XPTR)) {
    case 1:
	goto L_EXPV9;
    case 2:
	goto L_EXPV5;
    }
    POP(SCL);
    if (D_A(SCL) != 0)
	goto L_EXPV6;
    D_A(SCL) = 3;
    D(ZPTR) = D(XPTR);
    goto L_EXPV7;
    /*_*/
L_EXPV5:
    POP(SCL);
    goto L_EXPV11;
    /*_*/
}
static int
EXPEVL(retval)
    ret_t retval;
{
    ENTRY(EXPEVL)
    D_A(SCL) = 0;
    BRANCH(EXPVJN)
    /*_*/
}
int
EVAL(retval)
    ret_t retval;
{
    ENTRY(EVAL)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(XPTR) == E)
	goto L_EVAL1;
    if (D_V(XPTR) == I)
	BRANCH(RTXPTR)
    if (D_V(XPTR) == R)
	BRANCH(RTXPTR)
    if (D_V(XPTR) != S)
	BRANCH(INTR1)
    X_LOCSP(XSP,XPTR);
    if (S_L(XSP) == 0)
	BRANCH(RTXPTR)
    if (SPCINT(XPTR,XSP))
	BRANCH(RTXPTR)
    if (SPREAL(XPTR,XSP))
	BRANCH(RTXPTR)
    D(ZPTR) = D(XPTR);
    SAVSTK();
    switch (CONVE(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	BRANCH(INTR10)
    }
L_EVAL1:
    D_A(SCL) = 0;
    BRANCH(EXPVJ2)
    /*_*/
}
static int
INTVAL(retval)
    ret_t retval;
{
    ENTRY(INTVAL)
    D_A(OCICL) += DESCR;
    D(XPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(XPTR) & FNC))
	goto L_INTVC;
L_INTV1:
    if (D_A(INSW) == 0)
	goto L_INTV3;
    if (!LOCAPV(ZPTR,INATL,XPTR))
	goto L_INTV3;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    SAVSTK();
    PUSH(XPTR);
    PUSH(ZPTR);
    switch (PUTIN(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
L_INTV:
    X_LOCSP(XSP,XPTR);
    if (SPCINT(XPTR,XSP))
	BRANCH(RTXNAM)
    if (!SPREAL(XPTR,XSP))
	BRANCH(INTR1)
L_INTRI:
    CLR_MATH_ERROR();
    D_A(XPTR) = (int_t) D_RV(XPTR);
    D_F(XPTR) = 0;
    D_V(XPTR) = I;
    if (MATH_ERROR())
	BRANCH(INTR1)
    BRANCH(RTXNAM)
    /*_*/
L_INTV3:
    D(XPTR) = D(D_A(XPTR) + DESCR);
L_INTV2:
    if (D_V(XPTR) == I)
	BRANCH(RTXNAM)
    if (D_V(XPTR) == R)
	goto L_INTRI;
    if (D_V(XPTR) == S)
	goto L_INTV;
    else
	BRANCH(INTR1)
    /*_*/
L_INTVC:
    SAVSTK();
    PUSH(XPTR);
    switch (INVOKE(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_INTV1;
    case 3:
	goto L_INTV2;
    }
    /*_*/
}
static int
PATVAL(retval)
    ret_t retval;
{
    ENTRY(PATVAL)
    D_A(OCICL) += DESCR;
    D(XPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(XPTR) & FNC))
	goto L_PATVC;
L_PATV1:
    if (D_A(INSW) == 0)
	goto L_PATV2;
    if (!LOCAPV(ZPTR,INATL,XPTR))
	goto L_PATV2;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    SAVSTK();
    PUSH(XPTR);
    PUSH(ZPTR);
    switch (PUTIN(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	BRANCH(RTXNAM)
    }
    /*_*/
L_PATVC:
    SAVSTK();
    PUSH(XPTR);
    switch (INVOKE(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_PATV1;
    case 3:
	goto L_PATV3;
    }
    /*_*/
L_PATV2:
    D(XPTR) = D(D_A(XPTR) + DESCR);
L_PATV3:
    if (D_V(XPTR) == P)
	BRANCH(RTXNAM)
    if (D_V(XPTR) == S)
	BRANCH(RTXNAM)
    if (D_V(XPTR) == I)
	BRANCH(GENVIX)
    if (D_V(XPTR) == R)
	goto L_PATVR;
    if (D_V(XPTR) != E)
	BRANCH(INTR1)
    SAVSTK();
    PUSH(STARSZ);
    BLOCK(TPTR);
    MOVBLK(D_A(TPTR),D_A(STRPAT),D_A(STARSZ));
    D(D_A(TPTR) + 4*DESCR) = D(XPTR);
    D(XPTR) = D(TPTR);
    BRANCH(RTXNAM)
    /*_*/
L_PATVR:
    REALST(XSP,XPTR);
    SAVSTK();
    PUSH(XSPPTR);
    switch (GENVAR(XPTR)) {
    case 1:
	BRANCH(RTXNAM)
    }
    /*_*/
}
static int
VARVAL(retval)
    ret_t retval;
{
    ENTRY(VARVAL)
    D_A(OCICL) += DESCR;
    D(XPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(XPTR) & FNC))
	goto L_VARVC;
L_VARV1:
    if (D_A(INSW) == 0)
	goto L_VARV4;
    if (!LOCAPV(ZPTR,INATL,XPTR))
	goto L_VARV4;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    SAVSTK();
    PUSH(XPTR);
    PUSH(ZPTR);
    switch (PUTIN(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	BRANCH(RTXNAM)
    }
    /*_*/
L_VARV4:
    D(XPTR) = D(D_A(XPTR) + DESCR);
L_VARV2:
    if (D_V(XPTR) == S)
	BRANCH(RTXNAM)
    if (D_V(XPTR) == I)
	BRANCH(GENVIX)
    else
	BRANCH(INTR1)
    /*_*/
L_VARVC:
    SAVSTK();
    PUSH(XPTR);
    switch (INVOKE(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_VARV1;
    case 3:
	goto L_VARV2;
    }
    /*_*/
}
static int
VARVUP(retval)
    ret_t retval;
{
    ENTRY(VARVUP)
    SAVSTK();
    switch (VARVAL(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_A(CASECL) == 0)
	BRANCH(RTXNAM)
    else
	BRANCH(VPXPTR)
    /*_*/
}
static int
VPXPTR(retval)
    ret_t retval;
{
    ENTRY(VPXPTR)
    X_LOCSP(SPECR1,XPTR);
    if (S_L(SPECR1) == 0)
	BRANCH(RTXNAM)
    D_A(AXPTR) = S_L(SPECR1);
    D_F(AXPTR) = D_V(AXPTR) = 0;
    SAVSTK();
    PUSH(AXPTR);
    CONVAR(NORET);
    X_LOCSP(XSP,FRSGPT);
    if (!RAISE2(SPECR1,XSP))
	BRANCH(RTXNAM)
    D(D_A(FRSGPT) + DESCR) = D(D_A(XPTR) + DESCR);
    D(D_A(FRSGPT) + ATTRIB) = D(D_A(XPTR) + ATTRIB);
    SAVSTK();
    PUSH(AXPTR);
    switch (GNVARS(XPTR)) {
    case 1:
	BRANCH(RTXNAM)
    }
    BRANCH(RTXNAM)
    /*_*/
}
static int
XYARGS(retval)
    ret_t retval;
{
    ENTRY(XYARGS)
    D_A(SCL) = 0;
L_XYN:
    D_A(OCICL) += DESCR;
    D(YPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(YPTR) & FNC))
	goto L_XYC;
L_XY1:
    if (D_A(INSW) == 0)
	goto L_XY2;
    if (!LOCAPV(ZPTR,INATL,YPTR))
	goto L_XY2;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    SAVSTK();
    PUSH(YPTR);
    PUSH(ZPTR);
    switch (PUTIN(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
L_XY3:
    if (D_A(SCL) != 0)
	BRANCH(RTN2)
    D_A(SCL) = 1;
    D(XPTR) = D(YPTR);
    goto L_XYN;
    /*_*/
L_XY2:
    D(YPTR) = D(D_A(YPTR) + DESCR);
    goto L_XY3;
    /*_*/
L_XYC:
    PUSH(SCL);
    PUSH(XPTR);
    SAVSTK();
    PUSH(YPTR);
    switch (INVOKE(YPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_XY4;
    }
    POP(XPTR);
    POP(SCL);
    goto L_XY3;
    /*_*/
L_XY4:
    POP(XPTR);
    POP(SCL);
    goto L_XY1;
    /*_*/
}
int
ADD(retval)
    ret_t retval;
{
    ENTRY(ADD)
    D_A(SCL) = 1;
    BRANCH(ARITH)
    /*_*/
}
int
DIV(retval)
    ret_t retval;
{
    ENTRY(DIV)
    D_A(SCL) = 2;
    BRANCH(ARITH)
    /*_*/
}
int
EXPOP(retval)
    ret_t retval;
{
    ENTRY(EXPOP)
    D_A(SCL) = 3;
    BRANCH(ARITH)
    /*_*/
}
int
MPY(retval)
    ret_t retval;
{
    ENTRY(MPY)
    D_A(SCL) = 4;
    BRANCH(ARITH)
    /*_*/
}
int
SUB(retval)
    ret_t retval;
{
    ENTRY(SUB)
    D_A(SCL) = 5;
    BRANCH(ARITH)
    /*_*/
}
int
EQ(retval)
    ret_t retval;
{
    ENTRY(EQ)
    D_A(SCL) = 6;
    BRANCH(ARITH)
    /*_*/
}
int
GE(retval)
    ret_t retval;
{
    ENTRY(GE)
    D_A(SCL) = 7;
    BRANCH(ARITH)
    /*_*/
}
int
GT(retval)
    ret_t retval;
{
    ENTRY(GT)
    D_A(SCL) = 8;
    BRANCH(ARITH)
    /*_*/
}
int
LE(retval)
    ret_t retval;
{
    ENTRY(LE)
    D_A(SCL) = 9;
    BRANCH(ARITH)
    /*_*/
}
int
LT(retval)
    ret_t retval;
{
    ENTRY(LT)
    D_A(SCL) = 10;
    BRANCH(ARITH)
    /*_*/
}
int
NE(retval)
    ret_t retval;
{
    ENTRY(NE)
    D_A(SCL) = 11;
    BRANCH(ARITH)
    /*_*/
}
int
REMDR(retval)
    ret_t retval;
{
    ENTRY(REMDR)
    D_A(SCL) = 12;
    BRANCH(ARITH)
    /*_*/
}
static int
ARITH(retval)
    ret_t retval;
{
    ENTRY(ARITH)
    PUSH(SCL);
    SAVSTK();
    switch (XYARGS(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(SCL);
    D_A(DTCL) = D_V(XPTR);
    D_F(DTCL) = D_V(DTCL) = 0;
    D_V(DTCL) = D_V(YPTR);
    if (DCMP(DTCL, IIDTP))
	goto L_ARTHII;
    if (DCMP(DTCL, IVDTP))
	goto L_ARTHIV;
    if (DCMP(DTCL, VIDTP))
	goto L_ARTHVI;
    if (DCMP(DTCL, VVDTP))
	goto L_ARTHVV;
    if (DCMP(DTCL, RRDTP))
	goto L_ARTHRR;
    if (DCMP(DTCL, IRDTP))
	goto L_ARTHIR;
    if (DCMP(DTCL, RIDTP))
	goto L_ARTHRI;
    if (DCMP(DTCL, VRDTP))
	goto L_ARTHVR;
    if (DCMP(DTCL, RVDTP))
	goto L_ARTHRV;
    else
	BRANCH(INTR1)
    /*_*/
L_ARTHII:
    switch (D_A(SCL)) {
    case 1:
	goto L_AD;
    case 2:
	goto L_DV;
    case 3:
	goto L_EX;
    case 4:
	goto L_MP;
    case 5:
	goto L_SB;
    case 6:
	goto L_CEQ;
    case 7:
	goto L_CGE;
    case 8:
	goto L_CGT;
    case 9:
	goto L_CLE;
    case 10:
	goto L_CLT;
    case 11:
	goto L_CNE;
    case 12:
	goto L_RM;
    }
    /*_*/
L_ARTHVI:
    X_LOCSP(XSP,XPTR);
    if (SPCINT(XPTR,XSP))
	goto L_ARTHII;
    if (SPREAL(XPTR,XSP))
	goto L_ARTHRI;
    else
	BRANCH(INTR1)
    /*_*/
L_ARTHIV:
    X_LOCSP(YSP,YPTR);
    if (SPCINT(YPTR,YSP))
	goto L_ARTHII;
    if (SPREAL(YPTR,YSP))
	goto L_ARTHIR;
    else
	BRANCH(INTR1)
    /*_*/
L_ARTHVV:
    X_LOCSP(XSP,XPTR);
    if (SPCINT(XPTR,XSP))
	goto L_ARTHIV;
    if (SPREAL(XPTR,XSP))
	goto L_ARTHRV;
    else
	BRANCH(INTR1)
    /*_*/
L_ARTHRR:
    switch (D_A(SCL)) {
    case 1:
	goto L_AR;
    case 2:
	goto L_DR;
    case 3:
	goto L_EXR;
    case 4:
	goto L_MR;
    case 5:
	goto L_SR;
    case 6:
	goto L_REQ;
    case 7:
	goto L_RGE;
    case 8:
	goto L_RGT;
    case 9:
	goto L_RLE;
    case 10:
	goto L_RLT;
    case 11:
	goto L_RNE;
    case 12:
	BRANCH(INTR1)
    }
    /*_*/
L_ARTHIR:
    D_RV(XPTR) = (real_t)D_A(XPTR);
    D_F(XPTR) = 0;
    D_V(XPTR) = R;
    goto L_ARTHRR;
    /*_*/
L_ARTHRI:
    D_RV(YPTR) = (real_t)D_A(YPTR);
    D_F(YPTR) = 0;
    D_V(YPTR) = R;
    goto L_ARTHRR;
    /*_*/
L_ARTHVR:
    X_LOCSP(XSP,XPTR);
    if (SPCINT(XPTR,XSP))
	goto L_ARTHIR;
    if (SPREAL(XPTR,XSP))
	goto L_ARTHRR;
    else
	BRANCH(INTR1)
    /*_*/
L_ARTHRV:
    X_LOCSP(YSP,YPTR);
    if (SPCINT(YPTR,YSP))
	goto L_ARTHRI;
    if (SPREAL(YPTR,YSP))
	goto L_ARTHRR;
    else
	BRANCH(INTR1)
    /*_*/
L_AD:
    CLR_MATH_ERROR();
    D(ZPTR) = D(XPTR);
    D_A(ZPTR) += D_A(YPTR);
    if (MATH_ERROR())
	BRANCH(AERROR)
    BRANCH(ARTN)
    /*_*/
L_DV:
    if (D_A(YPTR) == 0)
	BRANCH(AERROR)
    CLR_MATH_ERROR();
    D(ZPTR) = D(XPTR);
    D_A(ZPTR) /= D_A(YPTR);
    if (MATH_ERROR())
	BRANCH(AERROR)
    BRANCH(ARTN)
    /*_*/
L_EX:
    if (EXPINT(ZPTR,XPTR,YPTR))
	BRANCH(ARTN)
    else
	BRANCH(AERROR)
    /*_*/
L_MP:
    CLR_MATH_ERROR();
    D(ZPTR) = D(XPTR);
    D_A(ZPTR) *= D_A(YPTR);
    if (MATH_ERROR())
	BRANCH(AERROR)
    BRANCH(ARTN)
    /*_*/
L_SB:
    CLR_MATH_ERROR();
    D(ZPTR) = D(XPTR);
    D_A(ZPTR) -= D_A(YPTR);
    if (MATH_ERROR())
	BRANCH(AERROR)
    BRANCH(ARTN)
    /*_*/
L_CEQ:
    if (D_A(XPTR) == D_A(YPTR))
	BRANCH(RETNUL)
    else
	BRANCH(FAIL)
    /*_*/
L_CGE:
    if (D_A(XPTR) < D_A(YPTR))
	BRANCH(FAIL)
    else
	BRANCH(RETNUL)
    /*_*/
L_CGT:
    if (D_A(XPTR) <= D_A(YPTR))
	BRANCH(FAIL)
    else
	BRANCH(RETNUL)
    /*_*/
L_CLE:
    if (D_A(XPTR) <= D_A(YPTR))
	BRANCH(RETNUL)
    else
	BRANCH(FAIL)
    /*_*/
L_CLT:
    if (D_A(XPTR) < D_A(YPTR))
	BRANCH(RETNUL)
    else
	BRANCH(FAIL)
    /*_*/
L_CNE:
    if (D_A(XPTR) == D_A(YPTR))
	BRANCH(FAIL)
    else
	BRANCH(RETNUL)
    /*_*/
L_AR:
    CLR_MATH_ERROR();
    D(ZPTR) = D(XPTR);
    D_RV(ZPTR) += D_RV(YPTR);
    if (RMATH_ERROR(D_RV(ZPTR)))
	BRANCH(AERROR)
    BRANCH(ARTN)
    /*_*/
L_DR:
    CLR_MATH_ERROR();
    D(ZPTR) = D(XPTR);
    D_RV(ZPTR) /= D_RV(YPTR);
    if (RMATH_ERROR(D_RV(ZPTR)))
	BRANCH(AERROR)
    BRANCH(ARTN)
    /*_*/
L_EXR:
    if (EXREAL(ZPTR,XPTR,YPTR))
	BRANCH(ARTN)
    else
	BRANCH(AERROR)
    /*_*/
L_MR:
    CLR_MATH_ERROR();
    D(ZPTR) = D(XPTR);
    D_RV(ZPTR) *= D_RV(YPTR);
    if (RMATH_ERROR(D_RV(ZPTR)))
	BRANCH(AERROR)
    BRANCH(ARTN)
    /*_*/
L_SR:
    CLR_MATH_ERROR();
    D(ZPTR) = D(XPTR);
    D_RV(ZPTR) -= D_RV(YPTR);
    if (RMATH_ERROR(D_RV(ZPTR)))
	BRANCH(AERROR)
    BRANCH(ARTN)
    /*_*/
L_REQ:
    if (D_RV(XPTR) == D_RV(YPTR))
	BRANCH(RETNUL)
    else
	BRANCH(FAIL)
    /*_*/
L_RGE:
    if (D_RV(XPTR) < D_RV(YPTR))
	BRANCH(FAIL)
    else
	BRANCH(RETNUL)
    /*_*/
L_RGT:
    if (D_RV(XPTR) <= D_RV(YPTR))
	BRANCH(FAIL)
    else
	BRANCH(RETNUL)
    /*_*/
L_RLE:
    if (D_RV(XPTR) <= D_RV(YPTR))
	BRANCH(RETNUL)
    else
	BRANCH(FAIL)
    /*_*/
L_RLT:
    if (D_RV(XPTR) < D_RV(YPTR))
	BRANCH(RETNUL)
    else
	BRANCH(FAIL)
    /*_*/
L_RNE:
    if (D_RV(XPTR) == D_RV(YPTR))
	BRANCH(FAIL)
    else
	BRANCH(RETNUL)
    /*_*/
L_RM:
    if (D_A(YPTR) == 0)
	BRANCH(AERROR)
    CLR_MATH_ERROR();
    D(ZPTR) = D(XPTR);
    D_A(ZPTR) /= D_A(YPTR);
    if (MATH_ERROR())
	BRANCH(AERROR)
    D(WPTR) = D(ZPTR);
    D_A(WPTR) *= D_A(YPTR);
    D(ZPTR) = D(XPTR);
    D_A(ZPTR) -= D_A(WPTR);
    BRANCH(ARTN)
    /*_*/
}
int
INTGER(retval)
    ret_t retval;
{
    ENTRY(INTGER)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(XPTR) == I)
	BRANCH(RETNUL)
    if (D_V(XPTR) != S)
	BRANCH(FAIL)
    X_LOCSP(XSP,XPTR);
    if (SPCINT(XPTR,XSP))
	BRANCH(RETNUL)
    else
	BRANCH(FAIL)
    /*_*/
}
int
MNS(retval)
    ret_t retval;
{
    ENTRY(MNS)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(XPTR) == I)
	goto L_MNSM;
    if (D_V(XPTR) == S)
	goto L_MNSV;
    if (D_V(XPTR) == R)
	goto L_MNSR;
    else
	BRANCH(INTR1)
    /*_*/
L_MNSM:
    CLR_MATH_ERROR();
    D(ZPTR) = D(XPTR);
    D_A(ZPTR) = -D_A(ZPTR);
    if (MATH_ERROR())
	BRANCH(AERROR)
    BRANCH(ARTN)
    /*_*/
L_MNSV:
    X_LOCSP(XSP,XPTR);
    if (SPCINT(XPTR,XSP))
	goto L_MNSM;
    if (!SPREAL(XPTR,XSP))
	BRANCH(INTR1)
L_MNSR:
    D(ZPTR) = D(XPTR);
    D_RV(ZPTR) = -D_RV(ZPTR);
    BRANCH(ARTN)
    /*_*/
}
int
PLS(retval)
    ret_t retval;
{
    ENTRY(PLS)
    SAVSTK();
    switch (ARGVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(ZPTR) == I)
	BRANCH(ARTN)
    if (D_V(ZPTR) == S)
	goto L_PLSV;
    if (D_V(ZPTR) == R)
	BRANCH(ARTN)
    else
	BRANCH(INTR1)
    /*_*/
L_PLSV:
    X_LOCSP(XSP,ZPTR);
    if (SPCINT(ZPTR,XSP))
	BRANCH(ARTN)
    if (SPREAL(ZPTR,XSP))
	BRANCH(ARTN)
    else
	BRANCH(INTR1)
    /*_*/
}
int
ANY(retval)
    ret_t retval;
{
    ENTRY(ANY)
    PUSH(ANYCCL);
    BRANCH(CHARZ)
    /*_*/
}
int
BREAKX(retval)
    ret_t retval;
{
    ENTRY(BREAKX)
    PUSH(BRXCCL);
    PUSH(ZEROCL);
    BRANCH(ABNSND)
    /*_*/
}
int
BREAK(retval)
    ret_t retval;
{
    ENTRY(BREAK)
    PUSH(BRKCCL);
    PUSH(ZEROCL);
    BRANCH(ABNSND)
    /*_*/
}
int
NOTANY(retval)
    ret_t retval;
{
    ENTRY(NOTANY)
    PUSH(NNYCCL);
    BRANCH(CHARZ)
    /*_*/
}
int
SPAN(retval)
    ret_t retval;
{
    ENTRY(SPAN)
    PUSH(SPNCCL);
    /* FALL */
    BRANCH(CHARZ)
}
static int
CHARZ(retval)
    ret_t retval;
{
    ENTRY(CHARZ)
    PUSH(CHARCL);
    /* FALL */
    BRANCH(ABNSND)
}
static int
ABNSND(retval)
    ret_t retval;
{
    ENTRY(ABNSND)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(ZCL);
    POP(YCL);
    if (D_V(XPTR) == S)
	BRANCH(PATNOD)
    if (D_V(XPTR) == E)
	BRANCH(PATNOD)
    if (D_V(XPTR) != I)
	BRANCH(INTR1)
    SAVSTK();
    PUSH(XPTR);
    GNVARI(XPTR);
    /* FALL */
    BRANCH(PATNOD)
}
static int
PATNOD(retval)
    ret_t retval;
{
    ENTRY(PATNOD)
    if (DCMP(XPTR, NULVCL))
	BRANCH(NONAME)
    SAVSTK();
    PUSH(LNODSZ);
    BLOCK(TPTR);
    MAKNOD(ZPTR,TPTR,ZCL,ZEROCL,YCL,XPTR);
    BRANCH(RTZPTR)
    /*_*/
}
int
LEN(retval)
    ret_t retval;
{
    ENTRY(LEN)
    PUSH(LNTHCL);
    BRANCH(LPRTND)
    /*_*/
}
int
POS(retval)
    ret_t retval;
{
    ENTRY(POS)
    PUSH(POSICL);
    BRANCH(LPRTND)
    /*_*/
}
int
RPOS(retval)
    ret_t retval;
{
    ENTRY(RPOS)
    PUSH(RPSICL);
    BRANCH(LPRTND)
    /*_*/
}
int
RTAB(retval)
    ret_t retval;
{
    ENTRY(RTAB)
    PUSH(RTBCL);
    BRANCH(LPRTND)
    /*_*/
}
int
TAB(retval)
    ret_t retval;
{
    ENTRY(TAB)
    PUSH(TBCL);
    /* FALL */
    BRANCH(LPRTND)
}
static int
LPRTND(retval)
    ret_t retval;
{
    ENTRY(LPRTND)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(YCL);
    D(ZCL) = D(ZEROCL);
    if (D_V(XPTR) == I)
	goto L_LPRTNI;
    if (D_V(XPTR) == E)
	BRANCH(PATNOD)
    if (D_V(XPTR) != S)
	BRANCH(INTR1)
    X_LOCSP(ZSP,XPTR);
    if (!SPCINT(XPTR,ZSP))
	BRANCH(INTR1)
L_LPRTNI:
    if (D_A(XPTR) < 0)
	BRANCH(LENERR)
    if (!DCMP(YCL, LNTHCL))
	BRANCH(PATNOD)
    D_A(ZCL) = D_A(XPTR);
    BRANCH(PATNOD)
    /*_*/
}
int
ARBNO(retval)
    ret_t retval;
{
    ENTRY(ARBNO)
    SAVSTK();
    switch (PATVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(XPTR) == P)
	goto L_ARBP;
    if (D_V(XPTR) != S)
	BRANCH(INTR1)
    X_LOCSP(TSP,XPTR);
    D_A(TMVAL) = S_L(TSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    SAVSTK();
    PUSH(LNODSZ);
    BLOCK(TPTR);
    MAKNOD(XPTR,TPTR,TMVAL,ZEROCL,CHRCL,XPTR);
L_ARBP:
    D_A(XSIZ) = D_V(D_A(XPTR));
    D_F(XSIZ) = D_V(XSIZ) = 0;
    D(TSIZ) = D(XSIZ);
    D_A(TSIZ) += D_A(ARBSIZ);
    D_V(TSIZ) = P;
    SAVSTK();
    PUSH(TSIZ);
    BLOCK(TPTR);
    D(ZPTR) = D(TPTR);
    D_A(TSIZ) = D_V(D_A(ARHEAD));
    D_F(TSIZ) = D_V(TSIZ) = 0;
    CPYPAT(TPTR,ARHEAD,ZEROCL,ZEROCL,ZEROCL,TSIZ);
    D(ZSIZ) = D(XSIZ);
    D_A(ZSIZ) += D_A(TSIZ);
    CPYPAT(TPTR,XPTR,ZEROCL,TSIZ,ZSIZ,XSIZ);
    D(TSIZ) = D(NODSIZ);
    D_A(TSIZ) += D_A(NODSIZ);
    CPYPAT(TPTR,ARTAIL,ZEROCL,ZSIZ,ZEROCL,TSIZ);
    D_A(ZSIZ) = D_A(TSIZ) + D_A(ZSIZ);
    D_F(ZSIZ) = D_F(TSIZ);
    D_V(ZSIZ) = D_V(TSIZ);
    CPYPAT(TPTR,ARBACK,ZEROCL,ZSIZ,TSIZ,TSIZ);
    BRANCH(RTZPTR)
    /*_*/
}
int
ATOP(retval)
    ret_t retval;
{
    ENTRY(ATOP)
    D_A(OCICL) += DESCR;
    D(YPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if (!(D_F(YPTR) & FNC))
	goto L_ATOP1;
    SAVSTK();
    PUSH(YPTR);
    switch (INVOKE(YPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_ATOP1;
    }
    if (D_V(YPTR) != E)
	BRANCH(NEMO)
L_ATOP1:
    SAVSTK();
    PUSH(LNODSZ);
    BLOCK(TPTR);
    MAKNOD(ZPTR,TPTR,ZEROCL,ZEROCL,ATOPCL,YPTR);
    BRANCH(RTZPTR)
    /*_*/
}
int
NAM(retval)
    ret_t retval;
{
    ENTRY(NAM)
    PUSH(ENMECL);
    BRANCH(NAM5)
    /*_*/
}
int
DOL(retval)
    ret_t retval;
{
    ENTRY(DOL)
    PUSH(ENMICL);
    /* FALL */
    BRANCH(NAM5)
}
static int
NAM5(retval)
    ret_t retval;
{
    ENTRY(NAM5)
    SAVSTK();
    switch (PATVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    D_A(OCICL) += DESCR;
    D(YPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(YPTR) & FNC))
	goto L_NAMC2;
L_NAM3:
    if (D_V(XPTR) == S)
	goto L_NAMV;
    if (D_V(XPTR) == P)
	goto L_NAMP;
    else
	BRANCH(INTR1)
    /*_*/
L_NAMC2:
    PUSH(XPTR);
    SAVSTK();
    PUSH(YPTR);
    switch (INVOKE(YPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_NAM4;
    }
    if (D_V(YPTR) != E)
	BRANCH(NEMO)
L_NAM4:
    POP(XPTR);
    goto L_NAM3;
    /*_*/
L_NAMV:
    X_LOCSP(TSP,XPTR);
    D_A(TMVAL) = S_L(TSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    SAVSTK();
    PUSH(LNODSZ);
    BLOCK(TPTR);
    MAKNOD(XPTR,TPTR,TMVAL,ZEROCL,CHRCL,XPTR);
L_NAMP:
    SAVSTK();
    PUSH(SNODSZ);
    BLOCK(TPTR);
    MAKNOD(WPTR,TPTR,ZEROCL,ZEROCL,NMECL,NULL);
    SAVSTK();
    PUSH(LNODSZ);
    BLOCK(TPTR);
    POP(TVAL);
    MAKNOD(YPTR,TPTR,ZEROCL,ZEROCL,TVAL,YPTR);
    D_A(XSIZ) = D_V(D_A(XPTR));
    D_F(XSIZ) = D_V(XSIZ) = 0;
    D(YSIZ) = D(XSIZ);
    D_A(YSIZ) += D_A(NODSIZ);
    D_A(TSIZ) = D_V(D_A(YPTR));
    D_F(TSIZ) = D_V(TSIZ) = 0;
    D(ZSIZ) = D(YSIZ);
    D_A(ZSIZ) += D_A(TSIZ);
    D_V(ZSIZ) = P;
    SAVSTK();
    PUSH(ZSIZ);
    BLOCK(TPTR);
    D(ZPTR) = D(TPTR);
    LVALUE(TVAL,XPTR);
    CPYPAT(TPTR,WPTR,TVAL,ZEROCL,NODSIZ,NODSIZ);
    CPYPAT(TPTR,XPTR,ZEROCL,NODSIZ,YSIZ,XSIZ);
    CPYPAT(TPTR,YPTR,ZEROCL,YSIZ,ZEROCL,TSIZ);
    BRANCH(RTZPTR)
    /*_*/
}
int
OR(retval)
    ret_t retval;
{
    ENTRY(OR)
    SAVSTK();
    switch (PATVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (PATVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    D_A(DTCL) = D_V(XPTR);
    D_F(DTCL) = D_V(DTCL) = 0;
    D_V(DTCL) = D_V(YPTR);
    if (DCMP(DTCL, VVDTP))
	goto L_ORVV;
    if (DCMP(DTCL, VPDTP))
	goto L_ORVP;
    if (DCMP(DTCL, PVDTP))
	goto L_ORPV;
    if (DCMP(DTCL, PPDTP))
	goto L_ORPP;
    else
	BRANCH(INTR1)
    /*_*/
L_ORVV:
    X_LOCSP(XSP,XPTR);
    D_A(TMVAL) = S_L(XSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    SAVSTK();
    PUSH(LNODSZ);
    BLOCK(TPTR);
    MAKNOD(XPTR,TPTR,TMVAL,ZEROCL,CHRCL,XPTR);
L_ORPV:
    X_LOCSP(YSP,YPTR);
    D_A(TMVAL) = S_L(YSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    SAVSTK();
    PUSH(LNODSZ);
    BLOCK(TPTR);
    MAKNOD(YPTR,TPTR,TMVAL,ZEROCL,CHRCL,YPTR);
L_ORPP:
    D_A(XSIZ) = D_V(D_A(XPTR));
    D_F(XSIZ) = D_V(XSIZ) = 0;
    D_A(YSIZ) = D_V(D_A(YPTR));
    D_F(YSIZ) = D_V(YSIZ) = 0;
    D(TSIZ) = D(XSIZ);
    D_A(TSIZ) += D_A(YSIZ);
    D_V(TSIZ) = P;
    SAVSTK();
    PUSH(TSIZ);
    BLOCK(TPTR);
    D(ZPTR) = D(TPTR);
    CPYPAT(TPTR,XPTR,ZEROCL,ZEROCL,ZEROCL,XSIZ);
    CPYPAT(TPTR,YPTR,ZEROCL,XSIZ,ZEROCL,YSIZ);
    LINKOR(ZPTR,XSIZ);
    BRANCH(RTZPTR)
    /*_*/
L_ORVP:
    X_LOCSP(XSP,XPTR);
    D_A(TMVAL) = S_L(XSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    SAVSTK();
    PUSH(LNODSZ);
    BLOCK(TPTR);
    MAKNOD(XPTR,TPTR,TMVAL,ZEROCL,CHRCL,XPTR);
    goto L_ORPP;
    /*_*/
}
int
SCAN(retval)
    ret_t retval;
{
    ENTRY(SCAN)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (PATVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    D_A(DTCL) = D_V(XPTR);
    D_F(DTCL) = D_V(DTCL) = 0;
    D_V(DTCL) = D_V(YPTR);
    D_A(SCNCL)++;
    if (DCMP(DTCL, VVDTP))
	goto L_SCANVV;
    if (DCMP(DTCL, VPDTP))
	goto L_SCANVP;
    if (DCMP(DTCL, IVDTP))
	goto L_SCANIV;
    if (DCMP(DTCL, RVDTP))
	goto L_SCANRV;
    if (DCMP(DTCL, RPDTP))
	goto L_SCANRP;
    if (DCMP(DTCL, IPDTP))
	goto L_SCANIP;
    else
	BRANCH(INTR1)
    /*_*/
L_SCANVV:
    X_LOCSP(XSP,XPTR);
    X_LOCSP(YSP,YPTR);
L_SCANVB:
    if (S_L(XSP) < S_L(YSP))
	BRANCH(FAIL)
    _SPEC(TSP) = _SPEC(XSP);
    S_L(TSP) = S_L(YSP);
    if (LEXEQ(TSP,YSP))
	BRANCH(RTYPTR)
    if (D_A(ANCCL) != 0)
	BRANCH(FAIL)
    S_L(XSP)--;
    S_O(XSP)++;
    goto L_SCANVB;
    /*_*/
L_SCANIV:
    SAVSTK();
    PUSH(XPTR);
    GNVARI(XPTR);
    goto L_SCANVV;
    /*_*/
L_SCANVP:
    X_LOCSP(XSP,XPTR);
    SAVSTK();
    switch (SCNR(NORET)) {
    case 1:
    case 3:
	BRANCH(FAIL)
    }
    SAVSTK();
    switch (NMD(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    if (S_L(TXSP) >= S_L(HEADSP))
	goto L_SCANV1;
    X_REMSP(XSP,HEADSP,TXSP);
    goto L_SCANV2;
    /*_*/
L_SCANV1:
    X_REMSP(XSP,TXSP,HEADSP);
L_SCANV2:
    SAVSTK();
    PUSH(XSPPTR);
    switch (GENVAR(YPTR)) {
    case 1:
	BRANCH(RTYPTR)
    }
    /*_*/
L_SCANIP:
    SAVSTK();
    PUSH(XPTR);
    GNVARI(XPTR);
    goto L_SCANVP;
    /*_*/
L_SCANRV:
    REALST(XSP,XPTR);
    SAVSTK();
    PUSH(XSPPTR);
    switch (GENVAR(XPTR)) {
    case 1:
	goto L_SCANVV;
    }
    /*_*/
L_SCANRP:
    REALST(XSP,XPTR);
    SAVSTK();
    PUSH(XSPPTR);
    switch (GENVAR(XPTR)) {
    case 1:
	goto L_SCANVP;
    }
    /*_*/
}
int
SJSR(retval)
    ret_t retval;
{
    ENTRY(SJSR)
    D_A(OCICL) += DESCR;
    D(WPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(WPTR) & FNC))
	goto L_SJSRC1;
L_SJSR1:
    if (D_A(INSW) == 0)
	goto L_SJSR1A;
    if (!LOCAPV(ZPTR,INATL,WPTR))
	goto L_SJSR1A;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    SAVSTK();
    PUSH(WPTR);
    PUSH(ZPTR);
    switch (PUTIN(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_SJSR1B;
    }
    /*_*/
L_SJSR1A:
    D(XPTR) = D(D_A(WPTR) + DESCR);
L_SJSR1B:
    PUSH(WPTR);
    PUSH(XPTR);
    SAVSTK();
    switch (PATVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    D_A(DTCL) = D_V(XPTR);
    D_F(DTCL) = D_V(DTCL) = 0;
    D_V(DTCL) = D_V(YPTR);
    D_A(SCNCL)++;
    if (DCMP(DTCL, VVDTP))
	goto L_SJSSVV;
    if (DCMP(DTCL, VPDTP))
	goto L_SJSSVP;
    if (DCMP(DTCL, IVDTP))
	goto L_SJSSIV;
    if (DCMP(DTCL, RVDTP))
	goto L_SJSSRV;
    if (DCMP(DTCL, RPDTP))
	goto L_SJSSRP;
    if (DCMP(DTCL, IPDTP))
	goto L_SJSSIP;
    else
	BRANCH(INTR1)
    /*_*/
L_SJSRC1:
    SAVSTK();
    PUSH(WPTR);
    switch (INVOKE(WPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_SJSR1;
    case 3:
	BRANCH(NEMO)
    }
    /*_*/
L_SJSSVP:
    X_LOCSP(XSP,XPTR);
    SAVSTK();
    switch (SCNR(NORET)) {
    case 1:
    case 3:
	BRANCH(FAIL)
    }
    D_A(NAMGCL) = 1;
    X_REMSP(TAILSP,XSP,TXSP);
    goto L_SJSS1;
    /*_*/
L_SJSSIP:
    SAVSTK();
    PUSH(XPTR);
    GNVARI(XPTR);
    goto L_SJSSVP;
    /*_*/
L_SJSSIV:
    SAVSTK();
    PUSH(XPTR);
    GNVARI(XPTR);
    goto L_SJSSVV;
    /*_*/
L_SJSSRV:
    REALST(XSP,XPTR);
    SAVSTK();
    PUSH(XSPPTR);
    switch (GENVAR(XPTR)) {
    case 1:
	goto L_SJSSVV;
    }
    /*_*/
L_SJSSRP:
    REALST(XSP,XPTR);
    SAVSTK();
    PUSH(XSPPTR);
    switch (GENVAR(XPTR)) {
    case 1:
	goto L_SJSSVP;
    }
    /*_*/
L_SJVVON:
    if (D_A(ANCCL) != 0)
	BRANCH(FAIL)
    S_L(HEADSP) += D_A(ONECL);
    S_L(XSP)--;
    S_O(XSP)++;
    goto L_SJSSV2;
    /*_*/
L_SJSSVV:
    X_LOCSP(XSP,XPTR);
    X_LOCSP(YSP,YPTR);
    _SPEC(HEADSP) = _SPEC(XSP);
    S_L(HEADSP) = 0;
L_SJSSV2:
    if (S_L(XSP) < S_L(YSP))
	BRANCH(FAIL)
    _SPEC(TSP) = _SPEC(XSP);
    S_L(TSP) = S_L(YSP);
    if (!LEXEQ(TSP,YSP))
	goto L_SJVVON;
    D_A(NAMGCL) = 0;
    X_REMSP(TAILSP,XSP,TSP);
L_SJSS1:
    SPUSH(TAILSP);
    SPUSH(HEADSP);
    if (D_A(NAMGCL) == 0)
	goto L_SJSS1A;
    SAVSTK();
    switch (NMD(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
L_SJSS1A:
    SAVSTK();
    switch (ARGVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    SPOP(HEADSP);
    SPOP(TAILSP);
    POP(WPTR);
    if (S_L(HEADSP) != 0)
	goto L_SJSSDT;
    if (S_L(TAILSP) == 0)
	goto L_SJSRV1;
L_SJSSDT:
    if (D_V(ZPTR) == S)
	goto L_SJSRV;
    if (D_V(ZPTR) == P)
	goto L_SJSRP;
    if (D_V(ZPTR) == I)
	goto L_SJSRI;
    if (D_V(ZPTR) == R)
	goto L_SJSRR;
    if (D_V(ZPTR) != E)
	BRANCH(INTR1)
    SAVSTK();
    PUSH(STARSZ);
    BLOCK(TPTR);
    MOVBLK(D_A(TPTR),D_A(STRPAT),D_A(STARSZ));
    D(D_A(TPTR) + 4*DESCR) = D(ZPTR);
    D(ZPTR) = D(TPTR);
L_SJSRP:
    _SPEC(XSP) = _SPEC(HEADSP);
    SAVSTK();
    PUSH(XSPPTR);
    GENVAR(XPTR);
    D_A(TMVAL) = S_L(HEADSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    SAVSTK();
    PUSH(LNODSZ);
    BLOCK(TPTR);
    MAKNOD(XPTR,TPTR,TMVAL,ZEROCL,CHRCL,XPTR);
    _SPEC(YSP) = _SPEC(TAILSP);
    SAVSTK();
    PUSH(YSPPTR);
    GENVAR(YPTR);
    D_A(TMVAL) = S_L(TAILSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    SAVSTK();
    PUSH(LNODSZ);
    BLOCK(TPTR);
    MAKNOD(YPTR,TPTR,TMVAL,ZEROCL,CHRCL,YPTR);
    D_A(XSIZ) = D_V(D_A(XPTR));
    D_F(XSIZ) = D_V(XSIZ) = 0;
    D_A(YSIZ) = D_V(D_A(YPTR));
    D_F(YSIZ) = D_V(YSIZ) = 0;
    D_A(ZSIZ) = D_V(D_A(ZPTR));
    D_F(ZSIZ) = D_V(ZSIZ) = 0;
    D(TSIZ) = D(XSIZ);
    D_A(TSIZ) += D_A(ZSIZ);
    D_A(TSIZ) += D_A(YSIZ);
    D_V(TSIZ) = P;
    SAVSTK();
    PUSH(TSIZ);
    BLOCK(TPTR);
    D(VVAL) = D(TPTR);
    LVALUE(TVAL,ZPTR);
    CPYPAT(TPTR,XPTR,TVAL,ZEROCL,XSIZ,XSIZ);
    LVALUE(TVAL,YPTR);
    D(TSIZ) = D(XSIZ);
    D_A(TSIZ) += D_A(ZSIZ);
    CPYPAT(TPTR,ZPTR,TVAL,XSIZ,TSIZ,ZSIZ);
    CPYPAT(TPTR,YPTR,ZEROCL,TSIZ,ZEROCL,YSIZ);
    D(ZPTR) = D(VVAL);
    goto L_SJSRV1;
    /*_*/
L_SJSRV:
    X_LOCSP(ZSP,ZPTR);
L_SJSRS:
    D_A(XPTR) = S_L(TAILSP);
    D_F(XPTR) = D_V(XPTR) = 0;
    D_A(YPTR) = S_L(HEADSP);
    D_F(YPTR) = D_V(YPTR) = 0;
    D_A(ZPTR) = S_L(ZSP);
    D_F(ZPTR) = D_V(ZPTR) = 0;
    D_A(XPTR) += D_A(YPTR);
    D_A(XPTR) += D_A(ZPTR);
    if (D_A(XPTR) > D_A(MLENCL))
	BRANCH(INTR8)
    SAVSTK();
    PUSH(XPTR);
    CONVAR(ZPTR);
    X_LOCSP(TSP,ZPTR);
    S_L(TSP) = 0;
    APDSP(TSP,HEADSP);
    APDSP(TSP,ZSP);
    APDSP(TSP,TAILSP);
    SAVSTK();
    PUSH(XPTR);
    GNVARS(ZPTR);
L_SJSRV1:
    D(D_A(WPTR) + DESCR) = D(ZPTR);
    if (D_A(OUTSW) == 0)
	goto L_SJSRV2;
    if (!LOCAPV(YPTR,OUTATL,WPTR))
	goto L_SJSRV2;
    D(YPTR) = D(D_A(YPTR) + DESCR);
    SAVSTK();
    PUSH(ZPTR);
    PUSH(YPTR);
    PUTOUT(NORET);
L_SJSRV2:
    if (D_A(TRAPCL) <= 0)
	BRANCH(RTZPTR)
    if (!LOCAPT(ATPTR,TVALL,WPTR))
	BRANCH(RTZPTR)
    SAVSTK();
    PUSH(ATPTR);
    switch (TRPHND(NORET)) {
    case 1:
	BRANCH(RTZPTR)
    }
    /*_*/
L_SJSRI:
    INTSPC(ZSP,ZPTR);
    goto L_SJSRS;
    /*_*/
L_SJSRR:
    REALST(ZSP,ZPTR);
    goto L_SJSRS;
    /*_*/
}
static int
SCNR(retval)
    ret_t retval;
{
    ENTRY(SCNR)
    D_A(MAXLEN) = S_L(XSP);
    D_F(MAXLEN) = D_V(MAXLEN) = 0;
    LVALUE(YSIZ,YPTR);
    if (D_A(FULLCL) != 0)
	goto L_SCNR1;
    if (D_A(YSIZ) > D_A(MAXLEN))
	BRANCH(FAIL)
L_SCNR1:
    _SPEC(TXSP) = _SPEC(XSP);
    S_L(TXSP) = 0;
    D(PDLPTR) = D(PDLHED);
    D(NAMICL) = D(NHEDCL);
    if (D_A(ANCCL) != 0)
	goto L_SCNR3;
    if (D_A(FULLCL) == 0)
	goto L_SCNR4;
    D(YSIZ) = D(MAXLEN);
    goto L_SCNR5;
    /*_*/
L_SCNR4:
    D_A(YSIZ) = D_A(MAXLEN) - D_A(YSIZ);
    D_F(YSIZ) = D_F(MAXLEN);
    D_V(YSIZ) = D_V(MAXLEN);
L_SCNR5:
    D_A(YSIZ) += D_A(CHARCL);
    PUSH(YPTR);
    PUSH(YSIZ);
    _SPEC(HEADSP) = _SPEC(TXSP);
    D_A(PDLPTR) += 3*DESCR;
    if (D_A(PDLPTR) > D_A(PDLEND))
	BRANCH(INTR31)
    D_A(LENFCL) = 1;
    D(D_A(PDLPTR) + DESCR) = D(SCONCL);
    D_A(TMVAL) = S_L(TXSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D(D_A(PDLPTR) + 2*DESCR) = D(TMVAL);
    D(D_A(PDLPTR) + 3*DESCR) = D(LENFCL);
    BRANCH(SCIN)
    /*_*/
L_SCNR3:
    D_A(PDLPTR) += 3*DESCR;
    if (D_A(PDLPTR) > D_A(PDLEND))
	BRANCH(INTR31)
    S_L(HEADSP) = 0;
    D(D_A(PDLPTR) + DESCR) = D(SCFLCL);
    D_A(TMVAL) = S_L(TXSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D(D_A(PDLPTR) + 2*DESCR) = D(TMVAL);
    D(D_A(PDLPTR) + 3*DESCR) = D(LENFCL);
    BRANCH(SCIN)
    /*_*/
}
static int
SCIN(retval)
    ret_t retval;
{
    ENTRY(SCIN)
    D_A(UNSCCL) = 0;
    /* FALL */
    BRANCH(SCIN1)
}
static int
SCIN1(retval)
    ret_t retval;
{
    ENTRY(SCIN1)
L_SCIN1A:
    D(PATBCL) = D(YPTR);
    if (D_A(UNSCCL) != 0)
	goto L_UNSC;
    D_A(PATICL) = 0;
L_SCIN2:
    D_A(LENFCL) = 1;
L_SCIN3:
    D_A(PATICL) += DESCR;
    D(ZCL) = D(D_A(PATBCL) + D_A(PATICL));
    D_A(PATICL) += DESCR;
    D(XCL) = D(D_A(PATBCL) + D_A(PATICL));
    D_A(PATICL) += DESCR;
    D(YCL) = D(D_A(PATBCL) + D_A(PATICL));
    D_A(PDLPTR) += 3*DESCR;
    if (D_A(PDLPTR) > D_A(PDLEND))
	BRANCH(INTR31)
    D(D_A(PDLPTR) + DESCR) = D(XCL);
    D_A(TMVAL) = S_L(TXSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D_V(TMVAL) = D_V(YCL);
    D(D_A(PDLPTR) + 2*DESCR) = D(TMVAL);
    D(D_A(PDLPTR) + 3*DESCR) = D(LENFCL);
    if (D_A(FULLCL) != 0)
	goto L_SCIN4;
    if (S_L(TXSP) + D_A(YCL) > D_A(MAXLEN))
	goto L_SALT1;
L_SCIN4:
    D(PTBRCL) = D(D_A(ZCL));
    goto L_PATBRA;
    /*_*/
L_SALF:
L_TSALF:
L_SALF1:
    D_A(LENFCL) = 0;
    goto L_SALT2;
    /*_*/
L_SALT:
L_TSALT:
L_SALT1:
    D(LENFCL) = D(D_A(PDLPTR) + 3*DESCR);
L_SALT2:
    D(XCL) = D(D_A(PDLPTR) + DESCR);
    D(YCL) = D(D_A(PDLPTR) + 2*DESCR);
    D_A(PDLPTR) -= 3*DESCR;
    D(PATICL) = D(XCL);
    if (D_A(PATICL) == 0)
	goto L_SALT3;
    S_L(TXSP) = D_A(YCL);
    if (!(D_F(PATICL) & FNC))
	goto L_SCIN3;
    D(PTBRCL) = D(D_A(PATICL));
L_PATBRA:
    switch (D_A(PTBRCL)) {
    case 1:
	goto L_ANYC;
    case 2:
	goto L_ARBF;
    case 3:
	goto L_ARBN;
    case 4:
	goto L_ATP;
    case 5:
	goto L_CHR;
    case 6:
	goto L_BAL;
    case 7:
	goto L_BALF;
    case 8:
	goto L_BRKC;
    case 9:
	goto L_BRKX;
    case 10:
	goto L_BRKXF;
    case 11:
	goto L_DNME;
    case 12:
	goto L_DNME1;
    case 13:
	goto L_EARB;
    case 14:
	goto L_DSAR;
    case 15:
	goto L_ENME;
    case 16:
	goto L_ENMI;
    case 17:
	goto L_FARB;
    case 18:
	goto L_FNME;
    case 19:
	goto L_LNTH;
    case 20:
	goto L_NME;
    case 21:
	goto L_NNYC;
    case 22:
	goto L_ONAR;
    case 23:
	goto L_ONRF;
    case 24:
	goto L_POSI;
    case 25:
	goto L_RPSI;
    case 26:
	goto L_RTB;
    case 27:
	BRANCH(FAIL)
    case 28:
	goto L_SALF;
    case 29:
	goto L_SCOK;
    case 30:
	goto L_SCON;
    case 31:
	goto L_SPNC;
    case 32:
	goto L_STAR;
    case 33:
	goto L_TB;
    case 34:
	BRANCH(RTNUL3)
    case 35:
	goto L_FNCE;
    case 36:
	goto L_SUCF;
    }
    BRANCH(INTR13)
    /*_*/
L_UNSC:
    D_A(UNSCCL) = 0;
    D(PATBCL) = D(YPTR);
L_SALT3:
    if (D_A(LENFCL) != 0)
	goto L_SALT1;
    goto L_SALF1;
    /*_*/
L_SCOK:
L_TSCOK:
    D_A(PATICL) = D_V(XCL);
    D_F(PATICL) = D_V(PATICL) = 0;
    if (D_A(PATICL) == 0)
	BRANCH(RTN2)
    else
	goto L_SCIN2;
    /*_*/
L_SCON:
    if (D_A(FULLCL) != 0)
	goto L_SCON1;
    if (D_A(LENFCL) != 0)
	BRANCH(FAIL)
L_SCON1:
    POP(YSIZ);
    POP(YPTR);
    D_A(YSIZ)--;
    if (D_A(YSIZ) < 0)
	BRANCH(INTR13)
    if (D_A(YSIZ) == 0)
	BRANCH(FAIL)
    S_L(TXSP) += D_A(ONECL);
    PUSH(YPTR);
    PUSH(YSIZ);
    _SPEC(HEADSP) = _SPEC(TXSP);
    D_A(PDLPTR) += 3*DESCR;
    if (D_A(PDLPTR) > D_A(PDLEND))
	BRANCH(INTR31)
    D_A(LENFCL) = 1;
    D(D_A(PDLPTR) + DESCR) = D(SCONCL);
    D_A(TMVAL) = S_L(TXSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D(D_A(PDLPTR) + 2*DESCR) = D(TMVAL);
    D(D_A(PDLPTR) + 3*DESCR) = D(LENFCL);
    goto L_SCIN1A;
    /*_*/
L_BRKC:
    D_A(SCL) = 2;
    goto L_ABNS;
    /*_*/
L_BRKX:
    D_A(SCL) = 5;
    goto L_ABNS;
    /*_*/
L_NNYC:
    D_A(SCL) = 3;
    goto L_ABNS;
    /*_*/
L_SPNC:
    D_A(SCL) = 4;
    goto L_ABNS;
    /*_*/
L_ANYC:
    D_A(SCL) = 1;
L_ABNS:
    D_A(PATICL) += DESCR;
    D(XPTR) = D(D_A(PATBCL) + D_A(PATICL));
L_ABNS1:
    if (D_V(XPTR) == S)
	goto L_ABNSV;
    if (D_V(XPTR) == E)
	goto L_ABNSE;
    if (D_V(XPTR) == I)
	goto L_ABNSI;
    else
	goto L_SCDTER;
    /*_*/
L_ABNSE:
    PUSH(SCL);
    SAVSTK();
    PUSH(XPTR);
    switch (EXPVAL(XPTR)) {
    case 1:
	goto L_ABNSF;
    case 2:
	goto L_ABNS2;
    }
L_ABNSF:
    POP(SCL);
    goto L_SALF;
    /*_*/
L_ABNS2:
    POP(SCL);
    goto L_ABNS1;
    /*_*/
L_ABNSI:
    SAVSTK();
    PUSH(XPTR);
    GNVARI(XPTR);
L_ABNSV:
    if (D_A(XPTR) == 0)
	goto L_SCNAME;
    switch (D_A(SCL)) {
    case 1:
    case 3:
	goto L_ANYC3;
    case 4:
	goto L_SPNV;
    }
    if (DCMP(XPTR, TBLBCS))
	goto L_ANYC3;
    clertb(&BRKTB,AC_CONTIN);
    X_LOCSP(YSP,XPTR);
    PLUGTB(&BRKTB,AC_STOPSH,YSP);
    D(TBLBCS) = D(XPTR);
L_ANYC3:
    _SPEC(VSP) = _SPEC(XSP);
    if (D_A(FULLCL) != 0)
	goto L_ANYC4;
    S_L(VSP) = D_A(MAXLEN);
    if (S_L(VSP) < S_L(TXSP))
	goto L_SALT;
    if (S_L(XSP) + D_A(ZEROCL) <= D_A(MAXLEN))
	goto L_ANYC4;
    S_L(VSP) += D_A(ONECL);
L_ANYC4:
    X_REMSP(YSP,VSP,TXSP);
    switch (D_A(SCL)) {
    case 1:
    case 3:
	goto L_ANYC7;
    }
    switch (STREAM(ZSP, YSP, &BRKTB)) {
    case ST_ERROR:
	goto L_SALF;
    case ST_EOS:
	goto L_SALT;
    }
    D_A(XPTR) = S_L(ZSP);
    D_F(XPTR) = D_V(XPTR) = 0;
    S_L(TXSP) += D_A(XPTR);
    if (D_A(SCL) != 5)
	goto L_SCOK;
    D_A(PDLPTR) += 3*DESCR;
    if (D_A(PDLPTR) > D_A(PDLEND))
	BRANCH(INTR31)
    D(D_A(PDLPTR) + DESCR) = D(ZEROCL);
    D(D_A(PDLPTR) + 2*DESCR) = D(PATICL);
    D(D_A(PDLPTR) + 3*DESCR) = D(LENFCL);
    D_A(TVAL) = S_L(TXSP);
    D_F(TVAL) = D_V(TVAL) = 0;
    D_A(TMVAL) = D_A(TVAL);
    D_A(PDLPTR) += 3*DESCR;
    if (D_A(PDLPTR) > D_A(PDLEND))
	BRANCH(INTR31)
    D(D_A(PDLPTR) + DESCR) = D(BRXFCL);
    D(D_A(PDLPTR) + 2*DESCR) = D(TMVAL);
    D(D_A(PDLPTR) + 3*DESCR) = D(LENFCL);
    goto L_SCOK;
    /*_*/
L_ANYC7:
    if (S_L(YSP) == 0)
	goto L_SALT;
    if (XANY(YSP,XPTR))
	goto L_ANYC5;
    if (D_A(SCL) == 1)
	goto L_SALF;
L_ANYC6:
    S_L(TXSP) += D_A(ONECL);
    goto L_SCOK;
L_ANYC5:
    if (D_A(SCL) == 3)
	goto L_SALF;
    else
	goto L_ANYC6;
    /*_*/
L_BRKXF:
    D(PATICL) = D(D_A(PDLPTR) + 2*DESCR);
    D_A(PATICL) -= DESCR;
    D_A(PDLPTR) -= 3*DESCR;
    if (D_A(FULLCL) == 0)
	goto L_BRXF1;
    D_A(NVAL) = 0;
    goto L_BRXF3;
    /*_*/
L_BRXF1:
    if (D_A(LENFCL) != 0)
	goto L_SALT;
    D_A(NVAL) = D_V(YCL);
    D_F(NVAL) = D_V(NVAL) = 0;
L_BRXF3:
    if (S_L(TXSP) + D_A(NVAL) >= D_A(MAXLEN))
	goto L_SALT;
    D(XCL) = D(D_A(PDLPTR) + DESCR);
    S_L(TXSP) += D_A(ONECL);
    goto L_BRKX;
    /*_*/
L_SPNV:
    if (DCMP(XPTR, TBLSCS))
	goto L_SPNC3;
    clertb(&SPANTB,AC_STOPSH);
    X_LOCSP(YSP,XPTR);
    PLUGTB(&SPANTB,AC_CONTIN,YSP);
    D(TBLSCS) = D(XPTR);
L_SPNC3:
    if (S_L(XSP) <= S_L(TXSP))
	goto L_SALT;
    X_REMSP(YSP,XSP,TXSP);
    switch (STREAM(ZSP, YSP, &SPANTB)) {
    case ST_ERROR:
	goto L_SALF;
    }
    if (S_L(ZSP) == 0)
	goto L_SALF;
    D_A(XPTR) = S_L(ZSP);
    D_F(XPTR) = D_V(XPTR) = 0;
    if (D_A(FULLCL) != 0)
	goto L_SPNC5;
    if (S_L(TXSP) + D_A(XPTR) > D_A(MAXLEN))
	goto L_SALT;
L_SPNC5:
    S_L(TXSP) += D_A(XPTR);
    goto L_SCOK;
    /*_*/
L_LNTH:
    D_A(SCL) = 1;
L_LPRRT:
    D_A(PATICL) += DESCR;
    D(XPTR) = D(D_A(PATBCL) + D_A(PATICL));
    PUSH(SCL);
L_LPRRT1:
    if (D_V(XPTR) == I)
	goto L_LPRRTI;
    if (D_V(XPTR) == E)
	goto L_LPRRTE;
    if (D_V(XPTR) == S)
	goto L_LPRRTV;
    POP(SCL);
    goto L_SCDTER;
L_LPRRTE:
    SAVSTK();
    PUSH(XPTR);
    switch (EXPVAL(XPTR)) {
    case 2:
	goto L_LPRRT1;
    }
    POP(SCL);
    goto L_TSALF;
    /*_*/
L_LPRRTV:
    X_LOCSP(ZSP,XPTR);
    if (!SPCINT(XPTR,ZSP))
	goto L_SCDTER;
L_LPRRTI:
    POP(SCL);
    switch (D_A(SCL)) {
    case 2:
	goto L_POSII;
    case 3:
	goto L_RPSII;
    case 4:
	goto L_RTBI;
    case 5:
	goto L_TBI;
    }
    if (D_A(XPTR) < 0)
	goto L_SCLENR;
    if (S_L(TXSP) + D_A(XPTR) > D_A(MAXLEN))
	goto L_TSALT;
    S_L(TXSP) += D_A(XPTR);
    goto L_SCOK;
    /*_*/
L_POSII:
    if (D_A(XPTR) < 0)
	goto L_SCLENR;
    D_A(NVAL) = S_L(TXSP);
    D_F(NVAL) = D_V(NVAL) = 0;
    if (D_A(XPTR) > D_A(MAXLEN))
	goto L_TSALT;
    if (D_A(XPTR) == D_A(NVAL))
	goto L_TSCOK;
    if (D_A(XPTR) > D_A(NVAL))
	goto L_TSALF;
    goto L_SALT;
    /*_*/
L_RPSII:
    if (D_A(XPTR) < 0)
	goto L_SCLENR;
    D_A(NVAL) = S_L(XSP);
    D_F(NVAL) = D_V(NVAL) = 0;
    D(TVAL) = D(NVAL);
    D_A(TVAL) -= D_A(XPTR);
    D_A(NVAL) = S_L(TXSP);
    D_F(NVAL) = D_V(NVAL) = 0;
    if (D_A(NVAL) < D_A(TVAL))
	goto L_TSALF;
    if (D_A(NVAL) == D_A(TVAL))
	goto L_TSCOK;
    goto L_TSALT;
    /*_*/
L_RTBI:
    if (D_A(XPTR) < 0)
	goto L_SCLENR;
    D_A(NVAL) = S_L(XSP);
    D_F(NVAL) = D_V(NVAL) = 0;
    D(TVAL) = D(NVAL);
    D_A(TVAL) -= D_A(XPTR);
    D_A(NVAL) = S_L(TXSP);
    D_F(NVAL) = D_V(NVAL) = 0;
    if (D_A(NVAL) > D_A(TVAL))
	goto L_TSALT;
    if (D_A(FULLCL) != 0)
	goto L_RTBII;
    D_A(NVAL) = D_V(YCL);
    D_F(NVAL) = D_V(NVAL) = 0;
    D_A(NVAL) = D_A(MAXLEN) - D_A(NVAL);
    D_F(NVAL) = D_F(MAXLEN);
    D_V(NVAL) = D_V(MAXLEN);
    if (D_A(NVAL) < D_A(TVAL))
	goto L_TSALT;
L_RTBII:
    S_L(TXSP) = D_A(TVAL);
    goto L_SCOK;
    /*_*/
L_TBI:
    if (D_A(XPTR) < 0)
	goto L_SCLENR;
    D_A(NVAL) = S_L(TXSP);
    D_F(NVAL) = D_V(NVAL) = 0;
    if (D_A(NVAL) > D_A(XPTR))
	goto L_TSALT;
    if (D_A(XPTR) > D_A(MAXLEN))
	goto L_TSALT;
    S_L(TXSP) = D_A(XPTR);
    goto L_SCOK;
    /*_*/
L_POSI:
    D_A(SCL) = 2;
    goto L_LPRRT;
    /*_*/
L_RPSI:
    D_A(SCL) = 3;
    goto L_LPRRT;
    /*_*/
L_RTB:
    D_A(SCL) = 4;
    goto L_LPRRT;
    /*_*/
L_TB:
    D_A(SCL) = 5;
    goto L_LPRRT;
    /*_*/
L_ARBN:
    D_A(TMVAL) = S_L(TXSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    PUSH(TMVAL);
    goto L_SCOK;
    /*_*/
L_ARBF:
    POP(TMVAL);
    goto L_ONAR2;
    /*_*/
L_EARB:
    POP(TMVAL);
    D(D_A(PDLPTR) + DESCR) = D(TMVAL);
    D_A(TMVAL) = S_L(TXSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D(D_A(PDLPTR) + 2*DESCR) = D(TMVAL);
    D(D_A(PDLPTR) + 3*DESCR) = D(ZEROCL);
    goto L_SCOK;
    /*_*/
L_ONAR:
    if (D_A(FULLCL) != 0)
	goto L_TSCOK;
    D(TVAL) = D(ZEROCL);
    D_A(TVAL) = D_A(D_A(PDLPTR) + -2*DESCR);
    D_A(TMVAL) = S_L(TXSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    if (D_A(TVAL) != D_A(TMVAL))
	goto L_TSCOK;
L_ONAR1:
    PUSH(TVAL);
    D_A(PDLPTR) -= 6*DESCR;
L_ONAR2:
    if (D_A(LENFCL) != 0)
	goto L_TSALT;
    goto L_SALF;
    /*_*/
L_ONRF:
    D(TVAL) = D(ZEROCL);
    D_A(TVAL) = D_A(D_A(PDLPTR) + -2*DESCR);
    goto L_ONAR1;
    /*_*/
L_FARB:
    if (D_A(FULLCL) == 0)
	goto L_FARB2;
    D_A(NVAL) = 0;
    goto L_FARB3;
    /*_*/
L_FARB2:
    if (D_A(LENFCL) != 0)
	goto L_FARB1;
    D_A(NVAL) = D_V(YCL);
    D_F(NVAL) = D_V(NVAL) = 0;
L_FARB3:
    D_A(TVAL) = S_L(TXSP);
    D_F(TVAL) = D_V(TVAL) = 0;
    D_A(TVAL) += D_A(NVAL);
    if (D_A(TVAL) >= D_A(MAXLEN))
	goto L_FARB1;
    S_L(TXSP) += D_A(ONECL);
    D_A(TVAL) = S_L(TXSP);
    D_F(TVAL) = D_V(TVAL) = 0;
    D_A(D_A(PDLPTR) + 2*DESCR) = D_A(TVAL);
    goto L_SCOK;
    /*_*/
L_FARB1:
    D_A(PDLPTR) -= 3*DESCR;
    goto L_SALT;
    /*_*/
L_ATP:
    D_A(PATICL) += DESCR;
    D(XPTR) = D(D_A(PATBCL) + D_A(PATICL));
L_ATP1:
    if (D_V(XPTR) == E)
	goto L_ATPEXN;
    D_A(NVAL) = S_L(TXSP);
    D_F(NVAL) = D_V(NVAL) = 0;
    D_V(NVAL) = I;
    D(D_A(XPTR) + DESCR) = D(NVAL);
    if (D_A(OUTSW) == 0)
	goto L_ATP2;
    if (!LOCAPV(ZPTR,OUTATL,XPTR))
	goto L_ATP2;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    SAVSTK();
    PUSH(NVAL);
    PUSH(ZPTR);
    PUTOUT(NORET);
L_ATP2:
    if (D_A(TRAPCL) == 0)
	goto L_TSCOK;
    if (!LOCAPT(ATPTR,TVALL,XPTR))
	goto L_TSCOK;
    PUSH(PATBCL);
    PUSH(PATICL);
    PUSH(WPTR);
    PUSH(XCL);
    PUSH(YCL);
    PUSH(MAXLEN);
    PUSH(LENFCL);
    PUSH(PDLPTR);
    PUSH(PDLHED);
    PUSH(NAMICL);
    PUSH(NHEDCL);
    SPUSH(HEADSP);
    SPUSH(TSP);
    SPUSH(TXSP);
    SPUSH(XSP);
    D(PDLHED) = D(PDLPTR);
    D(NHEDCL) = D(NAMICL);
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
    SPOP(XSP);
    SPOP(TXSP);
    SPOP(TSP);
    SPOP(HEADSP);
    POP(NHEDCL);
    POP(NAMICL);
    POP(PDLHED);
    POP(PDLPTR);
    POP(LENFCL);
    POP(MAXLEN);
    POP(YCL);
    POP(XCL);
    POP(WPTR);
    POP(PATICL);
    POP(PATBCL);
    goto L_SCOK;
    /*_*/
L_ATPEXN:
    SAVSTK();
    PUSH(XPTR);
    switch (EXPEVL(XPTR)) {
    case 1:
	goto L_TSALF;
    case 2:
	goto L_ATP1;
    case 3:
	goto L_SCNEMO;
    }
    /*_*/
L_BAL:
L_BALF1:
    if (D_A(FULLCL) == 0)
	goto L_BALF4;
    D_A(NVAL) = 0;
    goto L_BALF2;
    /*_*/
L_BALF4:
    D_A(NVAL) = D_V(YCL);
    D_F(NVAL) = D_V(NVAL) = 0;
L_BALF2:
    D_A(TVAL) = S_L(TXSP);
    D_F(TVAL) = D_V(TVAL) = 0;
    D_A(TVAL) += D_A(NVAL);
    if (D_A(TVAL) >= D_A(MAXLEN))
	goto L_BAL1;
    D_A(TVAL) = D_A(MAXLEN) - D_A(TVAL);
    D_F(TVAL) = D_F(MAXLEN);
    D_V(TVAL) = D_V(MAXLEN);
    if (!GETBAL(TXSP,TVAL))
	goto L_BAL1;
    D_A(TVAL) = S_L(TXSP);
    D_F(TVAL) = D_V(TVAL) = 0;
    D_A(D_A(PDLPTR) + 2*DESCR) = D_A(TVAL);
    goto L_SCOK;
    /*_*/
L_BAL1:
    D_A(PDLPTR) -= 3*DESCR;
    if (D_PTR(PDLPTR) < D_PTR(PDLHED))
	BRANCH(INTR13)
    else
	goto L_TSALF;
    /*_*/
L_BALF:
    if (D_A(FULLCL) == 0)
	goto L_BALF3;
    D_A(NVAL) = 0;
    goto L_BALF2;
    /*_*/
L_BALF3:
    if (D_A(LENFCL) == 0)
	goto L_BALF1;
    else
	goto L_BAL1;
    /*_*/
L_CHR:
    D_A(PATICL) += DESCR;
    D(YPTR) = D(D_A(PATBCL) + D_A(PATICL));
L_CHR1:
    X_LOCSP(TSP,YPTR);
L_CHR2:
    X_REMSP(VSP,XSP,TXSP);
    if (S_L(VSP) < S_L(TSP))
	goto L_TSALT;
    S_L(VSP) = S_L(TSP);
    if (!LEXEQ(VSP,TSP))
	goto L_TSALF;
    D_A(YPTR) = S_L(TSP);
    D_F(YPTR) = D_V(YPTR) = 0;
    S_L(TXSP) += D_A(YPTR);
    goto L_SCOK;
    /*_*/
L_STAR:
    D_A(PATICL) += DESCR;
    D(YPTR) = D(D_A(PATBCL) + D_A(PATICL));
L_STAR2:
    SAVSTK();
    PUSH(YPTR);
    switch (EXPVAL(YPTR)) {
    case 1:
	goto L_TSALF;
    }
    if (D_V(YPTR) == E)
	goto L_STAR2;
    D(XPTR) = D(PATBCL);
    D_A(XPTR) += D_A(PATICL);
    D(D_A(XPTR) + 7*DESCR) = D(YPTR);
    if (D_V(YPTR) == S)
	goto L_CHR1;
    if (D_V(YPTR) == P)
	goto L_STARP;
    if (D_V(YPTR) != I)
	goto L_SCDTER;
    INTSPC(TSP,YPTR);
    goto L_CHR2;
    /*_*/
L_STARP:
    if (D_A(FULLCL) == 0)
	goto L_STARP1;
    D_A(NVAL) = 0;
    goto L_STARP4;
    /*_*/
L_STARP1:
    D_A(NVAL) = D_V(YCL);
    D_F(NVAL) = D_V(NVAL) = 0;
L_STARP4:
    D_A(NVAL) = D_A(MAXLEN) - D_A(NVAL);
    D_F(NVAL) = D_F(MAXLEN);
    D_V(NVAL) = D_V(MAXLEN);
    if (D_A(NVAL) < 0)
	goto L_TSALT;
    LVALUE(TSIZ,YPTR);
    if (D_A(FULLCL) != 0)
	goto L_STARP6;
    if (D_A(TSIZ) > D_A(NVAL))
	goto L_TSALT;
L_STARP6:
    D_A(PDLPTR) += 3*DESCR;
    if (D_A(PDLPTR) > D_A(PDLEND))
	BRANCH(INTR31)
    D(D_A(PDLPTR) + DESCR) = D(SCFLCL);
    D_A(TMVAL) = S_L(TXSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D(D_A(PDLPTR) + 2*DESCR) = D(TMVAL);
    D(D_A(PDLPTR) + 3*DESCR) = D(LENFCL);
    PUSH(MAXLEN);
    PUSH(PATBCL);
    PUSH(PATICL);
    PUSH(XCL);
    PUSH(YCL);
    D(MAXLEN) = D(NVAL);
    SAVSTK();
    switch (SCIN(NORET)) {
    case 1:
	goto L_STARP5;
    case 3:
	BRANCH(RTNUL3)
    }
L_STARP2:
    POP(YCL);
    POP(XCL);
    POP(PATICL);
    POP(PATBCL);
    POP(MAXLEN);
    goto L_SCOK;
    /*_*/
L_STARP5:
    POP(YCL);
    POP(XCL);
    POP(PATICL);
    POP(PATBCL);
    POP(MAXLEN);
L_STARP3:
    if (D_A(LENFCL) != 0)
	goto L_TSALT;
    goto L_SALF;
    /*_*/
L_DSAR:
    D_A(PATICL) += DESCR;
    D(YPTR) = D(D_A(PATBCL) + D_A(PATICL));
    if (D_V(YPTR) == S)
	goto L_STARP3;
    if (D_V(YPTR) == P)
	goto L_DSARP;
    if (D_V(YPTR) == I)
	goto L_STARP3;
    else
	goto L_SCDTER;
    /*_*/
L_DSARP:
    if (D_A(FULLCL) == 0)
	goto L_DSARP1;
    D_A(NVAL) = 0;
    goto L_DSARP2;
    /*_*/
L_DSARP1:
    D_A(NVAL) = D_V(YCL);
    D_F(NVAL) = D_V(NVAL) = 0;
L_DSARP2:
    D_A(NVAL) = D_A(MAXLEN) - D_A(NVAL);
    D_F(NVAL) = D_F(MAXLEN);
    D_V(NVAL) = D_V(MAXLEN);
    PUSH(MAXLEN);
    PUSH(PATBCL);
    PUSH(PATICL);
    PUSH(XCL);
    PUSH(YCL);
    D(MAXLEN) = D(NVAL);
    D_A(UNSCCL) = 1;
    SAVSTK();
    switch (SCIN1(NORET)) {
    case 1:
	goto L_STARP5;
    case 2:
	goto L_STARP2;
    case 3:
	BRANCH(RTNUL3)
    }
    /*_*/
L_FNCE:
    D_A(PDLPTR) += 3*DESCR;
    if (D_A(PDLPTR) > D_A(PDLEND))
	BRANCH(INTR31)
    D(D_A(PDLPTR) + DESCR) = D(FNCFCL);
    D_A(TMVAL) = S_L(TXSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D(D_A(PDLPTR) + 2*DESCR) = D(TMVAL);
    D(D_A(PDLPTR) + 3*DESCR) = D(LENFCL);
    D_A(LENFCL) = 1;
    goto L_SCOK;
    /*_*/
L_NME:
    D_A(PDLPTR) += 3*DESCR;
    if (D_A(PDLPTR) > D_A(PDLEND))
	BRANCH(INTR31)
    D(D_A(PDLPTR) + DESCR) = D(FNMECL);
    D_A(TMVAL) = S_L(TXSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D(D_A(PDLPTR) + 2*DESCR) = D(TMVAL);
    D(D_A(PDLPTR) + 3*DESCR) = D(LENFCL);
    PUSH(TMVAL);
    D_A(LENFCL) = 1;
    goto L_SCOK;
    /*_*/
L_FNME:
    POP(TVAL);
L_FNME1:
    if (D_A(LENFCL) == 0)
	goto L_TSALF;
    else
	goto L_TSALT;
    /*_*/
L_ENME:
    D_A(PATICL) += DESCR;
    D(YPTR) = D(D_A(PATBCL) + D_A(PATICL));
    POP(NVAL);
    D_V(YCL) = D_A(NVAL);
    _SPEC(TSP) = _SPEC(TXSP);
    S_L(TSP) = D_A(NVAL);
    X_REMSP(TSP,TXSP,TSP);
    D(TPTR) = D(NBSPTR);
    D_A(TPTR) += D_A(NAMICL);
    _SPEC(D_A(TPTR) + DESCR) = _SPEC(TSP);
    D(D_A(TPTR) + DESCR+SPEC) = D(YPTR);
    D_A(NAMICL) += DESCR+SPEC;
    if (D_A(NAMICL) == D_A(NMOVER))
	goto L_ENME1;
    if (D_A(NAMICL) > D_A(NMOVER))
	BRANCH(INTR13)
L_ENME2:
    D_A(PDLPTR) += DESCR+SPEC;
    if (D_A(PDLPTR) > D_A(PDLEND))
	BRANCH(INTR31)
    D(D_A(PDLPTR) + DESCR) = D(DNMECL);
L_ENME3:
    D_A(TMVAL) = S_L(TXSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D_V(TMVAL) = D_V(YCL);
    D(D_A(PDLPTR) + 2*DESCR) = D(TMVAL);
    D(D_A(PDLPTR) + 3*DESCR) = D(LENFCL);
    D_A(LENFCL) = 1;
    goto L_SCOK;
    /*_*/
L_ENME1:
    D(WCL) = D(NMOVER);
    D_A(NMOVER) += NAMLSZ*SPDR;
    SAVSTK();
    PUSH(NMOVER);
    BLOCK(TPTR);
    MOVBLK(D_A(TPTR),D_A(NBSPTR),D_A(WCL));
    D(NBSPTR) = D(TPTR);
    goto L_ENME2;
    /*_*/
L_DNME:
    D_A(NAMICL) -= DESCR+SPEC;
    D(TPTR) = D(NBSPTR);
    D_A(TPTR) += D_A(NAMICL);
L_DNME1:
    D_A(VVAL) = D_V(YCL);
    D_F(VVAL) = D_V(VVAL) = 0;
    PUSH(VVAL);
    goto L_FNME1;
    /*_*/
L_ENMI:
    D_A(PATICL) += DESCR;
    D(YPTR) = D(D_A(PATBCL) + D_A(PATICL));
    POP(NVAL);
    D_V(YCL) = D_A(NVAL);
    _SPEC(TSP) = _SPEC(TXSP);
    S_L(TSP) = D_A(NVAL);
    X_REMSP(TSP,TXSP,TSP);
    D_A(ZCL) = S_L(TSP);
    D_F(ZCL) = D_V(ZCL) = 0;
    if (D_A(ZCL) > D_A(MLENCL))
	goto L_SCLNOR;
    if (D_V(YPTR) == E)
	goto L_ENMEXN;
L_ENMI5:
    if (D_V(YPTR) == K)
	goto L_ENMIC;
    SAVSTK();
    PUSH(TSPPTR);
    GENVAR(VVAL);
L_ENMI3:
    D(D_A(YPTR) + DESCR) = D(VVAL);
    if (D_A(OUTSW) == 0)
	goto L_ENMI4;
    if (!LOCAPV(ZPTR,OUTATL,YPTR))
	goto L_ENMI4;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    SAVSTK();
    PUSH(VVAL);
    PUSH(ZPTR);
    PUTOUT(NORET);
L_ENMI4:
    if (D_A(TRAPCL) <= 0)
	goto L_ENMI2;
    if (!LOCAPT(ATPTR,TVALL,YPTR))
	goto L_ENMI2;
    PUSH(PATBCL);
    PUSH(PATICL);
    PUSH(WPTR);
    PUSH(XCL);
    PUSH(YCL);
    PUSH(MAXLEN);
    PUSH(LENFCL);
    PUSH(PDLPTR);
    PUSH(PDLHED);
    PUSH(NAMICL);
    PUSH(NHEDCL);
    SPUSH(HEADSP);
    SPUSH(TSP);
    SPUSH(TXSP);
    SPUSH(XSP);
    D(PDLHED) = D(PDLPTR);
    D(NHEDCL) = D(NAMICL);
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
    SPOP(XSP);
    SPOP(TXSP);
    SPOP(TSP);
    SPOP(HEADSP);
    POP(NHEDCL);
    POP(NAMICL);
    POP(PDLHED);
    POP(PDLPTR);
    POP(LENFCL);
    POP(MAXLEN);
    POP(YCL);
    POP(XCL);
    POP(WPTR);
    POP(PATICL);
    POP(PATBCL);
L_ENMI2:
    D_A(PDLPTR) += 3*DESCR;
    if (D_A(PDLPTR) > D_A(PDLEND))
	BRANCH(INTR31)
    D(D_A(PDLPTR) + DESCR) = D(DNMICL);
    goto L_ENME3;
    /*_*/
L_ENMIC:
    if (SPCINT(VVAL,TSP))
	goto L_ENMI3;
    else
	goto L_SCDTER;
    /*_*/
L_ENMEXN:
    PUSH(ZEROCL);
    SAVSTK();
    PUSH(YPTR);
    switch (EXPEVL(YPTR)) {
    case 1:
	goto L_TSALF;
    case 3:
	goto L_SCNEMO;
    }
    POP(ZEROCL);
    goto L_ENMI5;
    /*_*/
L_SUCE:
    D_A(PDLPTR) += 3*DESCR;
    if (D_A(PDLPTR) > D_A(PDLEND))
	BRANCH(INTR31)
    D(D_A(PDLPTR) + DESCR) = D(SUCFCL);
    D_A(TMVAL) = S_L(TXSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    D(D_A(PDLPTR) + 2*DESCR) = D(TMVAL);
    D(D_A(PDLPTR) + 3*DESCR) = D(LENFCL);
    D_A(LENFCL) = 1;
    goto L_SCOK;
    /*_*/
L_SUCF:
    D(XCL) = D(D_A(PDLPTR) + DESCR);
    D(YCL) = D(D_A(PDLPTR) + 2*DESCR);
    goto L_SUCE;
    /*_*/
L_SCDTER:
    D_A(ERRTYP) = 1;
    goto L_SCERSX;
    /*_*/
L_SCLENR:
    D_A(ERRTYP) = 14;
    goto L_SCERSX;
    /*_*/
L_SCLNOR:
    D_A(ERRTYP) = 15;
    goto L_SCERSX;
    /*_*/
L_SCNAME:
    D_A(ERRTYP) = 4;
    goto L_SCERSX;
    /*_*/
L_SCNEMO:
    D_A(ERRTYP) = 8;
L_SCERSX:
    SAVSTK();
    SCERST(NORET);
    goto L_TSALF;
    /*_*/
}
int
DEFINE(retval)
    ret_t retval;
{
    ENTRY(DEFINE)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVUP(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    X_LOCSP(XSP,XPTR);
    switch (STREAM(YSP, XSP, &VARATB)) {
    case ST_ERROR:
    case ST_EOS:
	BRANCH(PROTER)
    }
    if (D_A(STYPE) != LPTYP)
	BRANCH(PROTER)
    SAVSTK();
    PUSH(YSPPTR);
    GENVUP(XPTR);
    SAVSTK();
    PUSH(XPTR);
    FINDEX(ZCL);
    if (!DCMP(YPTR, NULVCL))
	goto L_DEFIN3;
    D(YPTR) = D(XPTR);
L_DEFIN3:
    PUSH(YPTR);
    D(YCL) = D(ZEROCL);
    PUSH(XPTR);
L_DEFIN4:
    S_L(XSP)--;
    S_O(XSP)++;
    switch (STREAM(YSP, XSP, &VARATB)) {
    case ST_ERROR:
    case ST_EOS:
	BRANCH(PROTER)
    }
    switch (D_A(STYPE)) {
    case 1:
	BRANCH(PROTER)
    case 3:
	goto L_DEFIN6;
    }
    if (S_L(YSP) == 0)
	goto L_DEFIN4;
    SAVSTK();
    PUSH(YSPPTR);
    GENVUP(XPTR);
    PUSH(XPTR);
    D_A(YCL)++;
    goto L_DEFIN4;
    /*_*/
L_DEFIN6:
    if (S_L(YSP) == 0)
	goto L_DEFIN9;
    D_A(YCL)++;
    SAVSTK();
    PUSH(YSPPTR);
    GENVUP(XPTR);
    PUSH(XPTR);
L_DEFIN9:
    D_V(DEFCL) = D_A(YCL);
L_DEFIN8:
    S_L(XSP)--;
    S_O(XSP)++;
    switch (STREAM(YSP, XSP, &VARATB)) {
    case ST_ERROR:
	BRANCH(PROTER)
    case ST_EOS:
	goto L_DEF10;
    }
    if (D_A(STYPE) != CMATYP)
	BRANCH(PROTER)
    if (S_L(YSP) == 0)
	goto L_DEFIN8;
    SAVSTK();
    PUSH(YSPPTR);
    GENVUP(XPTR);
    PUSH(XPTR);
    D_A(YCL)++;
    goto L_DEFIN8;
    /*_*/
L_DEF10:
    if (S_L(YSP) == 0)
	goto L_DEF11;
    SAVSTK();
    PUSH(YSPPTR);
    GENVUP(XPTR);
    PUSH(XPTR);
    D_A(YCL)++;
L_DEF11:
    D_A(YCL) += 2;
    D_A(XCL) = D_A(YCL);
    D_A(XCL) *= DESCR;
    D_F(XCL) = D_V(XCL) = 0;
    D_V(XCL) = B;
    SAVSTK();
    PUSH(XCL);
    BLOCK(XPTR);
    D(D_A(ZCL)) = D(DEFCL);
    D(D_A(ZCL) + DESCR) = D(XPTR);
    D_A(XPTR) += D_A(XCL);
L_DEF12:
    D_A(XPTR) -= DESCR;
    POP(YPTR);
    D(D_A(XPTR) + DESCR) = D(YPTR);
    D_A(YCL)--;
    if (D_A(YCL) == 0)
	BRANCH(RETNUL)
    else
	goto L_DEF12;
    /*_*/
}
int
DEFFNC(retval)
    ret_t retval;
{
    ENTRY(DEFFNC)
    D_A(XCL) = D_V(INCL);
    D_F(XCL) = D_V(XCL) = 0;
    D(WCL) = D(XCL);
    D(YCL) = D(INCL);
    PSTACK(YPTR);
    PUSH(NULVCL);
L_DEFF1:
    D_A(OCICL) += DESCR;
    D(XPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(XPTR) & FNC))
	goto L_DEFFC;
L_DEFF2:
    if (D_A(INSW) == 0)
	goto L_DEFF14;
    if (!LOCAPV(ZPTR,INATL,XPTR))
	goto L_DEFF14;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    PUSH(XCL);
    PUSH(WCL);
    PUSH(YCL);
    PUSH(YPTR);
    SAVSTK();
    PUSH(XPTR);
    PUSH(ZPTR);
    switch (PUTIN(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(YPTR);
    POP(YCL);
    POP(WCL);
    POP(XCL);
    goto L_DEFF3;
    /*_*/
L_DEFF14:
    D(XPTR) = D(D_A(XPTR) + DESCR);
L_DEFF3:
    PUSH(XPTR);
    D_A(XCL)--;
    if (D_A(XCL) < 0)
	BRANCH(INTR10)
    if (D_A(XCL) > 0)
	goto L_DEFF1;
    D(XCL) = D(D_A(YCL));
    D_A(XCL) = D_V(XCL);
    D_F(XCL) = D_V(XCL) = 0;
L_DEFF4:
    if (D_A(WCL) == D_A(XCL))
	goto L_DEFF5;
    if (D_A(WCL) > D_A(XCL))
	goto L_DEFF9;
    PUSH(NULVCL);
    D_A(WCL)++;
    goto L_DEFF4;
    /*_*/
L_DEFF9:
    POP(ZCL);
    D_A(WCL)--;
    goto L_DEFF4;
    /*_*/
L_DEFF5:
    D(ZCL) = D(D_A(YCL) + DESCR);
    D(XPTR) = D(ZCL);
    D_A(WCL) = D_V(D_A(ZCL));
    D_F(WCL) = D_V(WCL) = 0;
    D(WPTR) = D(ZCL);
    D_A(WPTR) += D_A(WCL);
    D_A(XCL)++;
L_DEFF8:
    D_A(XPTR) += DESCR;
    D_A(YPTR) += DESCR;
    D(ZPTR) = D(D_A(XPTR) + DESCR);
    D(TPTR) = D(D_A(ZPTR) + DESCR);
    D(ATPTR) = D(D_A(YPTR) + DESCR);
    D(D_A(ZPTR) + DESCR) = D(ATPTR);
    D(D_A(YPTR) + DESCR) = D(TPTR);
    D_A(XCL)--;
    if (D_A(XCL) < 0)
	BRANCH(INTR10)
    if (D_A(XCL) > 0)
	goto L_DEFF8;
L_DEFF10:
    D_A(XPTR) += DESCR;
    if (D_A(XPTR) == D_A(WPTR))
	goto L_DEFFGO;
    D(ZPTR) = D(D_A(XPTR) + DESCR);
    D(TPTR) = D(D_A(ZPTR) + DESCR);
    PUSH(TPTR);
    D(D_A(ZPTR) + DESCR) = D(NULVCL);
    goto L_DEFF10;
    /*_*/
L_DEFFGO:
    PUSH(FILENM);
    PUSH(LNNOCL);
    PUSH(FRTNCL);
    PUSH(STNOCL);
    PUSH(OCICL);
    PUSH(OCBSCL);
    PUSH(ZCL);
    PUSH(ZCL);
    D(XCL) = D(D_A(ZCL) + DESCR);
    if (D_A(D_A(XCL) + ATTRIB) == (int_t)0)
	BRANCH(UNDFFE)
    D(OCBSCL) = D(D_A(XCL) + ATTRIB);
    if (D_A(TRACL) <= 0)
	goto L_DEFF18;
    D_A(TRACL)--;
    D(ATPTR) = D(D_A(ZCL) + 2*DESCR);
    PUSH(ZCL);
    SAVSTK();
    PUSH(ATPTR);
    switch (FENTR2(NORET)) {
    case 1:
    case 2:
	BRANCH(INTR10)
    }
    POP(ZCL);
L_DEFF18:
    if (D_A(TRAPCL) <= 0)
	goto L_DEFF19;
    D(ATPTR) = D(D_A(ZCL) + 2*DESCR);
    if (!LOCAPT(ATPTR,TFENTL,ATPTR))
	goto L_DEFF19;
    PUSH(OCBSCL);
    PUSH(ZCL);
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
    POP(ZCL);
    POP(OCBSCL);
L_DEFF19:
    D_A(LVLCL)++;
    if (D_A(TRAPCL) <= 0)
	goto L_DEFF15;
    if (!LOCAPT(ATPTR,TKEYL,FNCLKY))
	goto L_DEFF15;
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
L_DEFF15:
    D_A(OCICL) = 0;
    SAVSTK();
    switch (INTERP(NORET)) {
    case 1:
	goto L_DEFFF;
    case 2:
	goto L_DEFFNR;
    }
    D(RETPCL) = D(RETCL);
L_DEFFS1:
    POP(ZCL);
    if (D_A(TRACL) <= 0)
	goto L_DEFF20;
    D_A(TRACL)--;
    D(ATPTR) = D(D_A(ZCL) + 2*DESCR);
    PUSH(ZCL);
    SAVSTK();
    PUSH(ATPTR);
    switch (FNEXT2(NORET)) {
    case 1:
    case 2:
	BRANCH(INTR10)
    }
    POP(ZCL);
L_DEFF20:
    if (D_A(TRAPCL) <= 0)
	goto L_DEFFS2;
    D(ATPTR) = D(D_A(ZCL) + 2*DESCR);
    if (!LOCAPT(ATPTR,TFEXTL,ATPTR))
	goto L_DEFFS2;
    PUSH(RETPCL);
    PUSH(ZCL);
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
    POP(ZCL);
    POP(RETPCL);
L_DEFFS2:
    D_A(LVLCL)--;
    if (D_A(TRAPCL) <= 0)
	goto L_DEFF17;
    if (!LOCAPT(ATPTR,TKEYL,FNCLKY))
	goto L_DEFF17;
    PUSH(RETPCL);
    PUSH(ZCL);
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
    POP(ZCL);
    POP(RETPCL);
L_DEFF17:
    POP(ZCL);
    POP(OCBSCL);
    POP(OCICL);
    POP(STNOCL);
    POP(FRTNCL);
    POP(LNNOCL);
    POP(FILENM);
    D_A(WCL) = D_V(D_A(ZCL));
    D_F(WCL) = D_V(WCL) = 0;
    D_A(WCL) -= DESCR;
    if (D_A(WCL) <= 0)
	BRANCH(INTR10)
    D(WPTR) = D(ZCL);
    D_A(WPTR) += D_A(WCL);
    D(YPTR) = D(ZCL);
    D_A(YPTR) += DESCR;
    D(ZPTR) = D(D_A(YPTR) + DESCR);
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
L_DEFF6:
    POP(XPTR);
    D(YPTR) = D(D_A(WPTR) + DESCR);
    D(D_A(YPTR) + DESCR) = D(XPTR);
    D_A(WPTR) -= DESCR;
    if (D_A(WPTR) != D_A(ZCL))
	goto L_DEFF6;
    if (DCMP(RETPCL, FRETCL))
	BRANCH(FAIL)
    if (!DCMP(RETPCL, NRETCL))
	BRANCH(RTZPTR)
    D(XPTR) = D(ZPTR);
    if (D_V(XPTR) == S)
	goto L_DEFFVX;
    if (D_V(XPTR) == I)
	BRANCH(GENVIX)
    if (D_V(XPTR) == N)
	BRANCH(RTXNAM)
    if (D_V(XPTR) == K)
	BRANCH(RTXNAM)
    else
	BRANCH(NONAME)
L_DEFFVX:
    if (D_A(XPTR) == 0)
	BRANCH(NONAME)
    else
	BRANCH(RTXNAM)
    /*_*/
L_DEFFF:
    D(RETPCL) = D(FRETCL);
    goto L_DEFFS1;
    /*_*/
L_DEFFC:
    PUSH(XCL);
    PUSH(WCL);
    PUSH(YCL);
    PUSH(YPTR);
    SAVSTK();
    PUSH(XPTR);
    switch (INVOKE(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_DEFFN;
    }
    POP(YPTR);
    POP(YCL);
    POP(WCL);
    POP(XCL);
    goto L_DEFF3;
    /*_*/
L_DEFFN:
    POP(YPTR);
    POP(YCL);
    POP(WCL);
    POP(XCL);
    goto L_DEFF2;
    /*_*/
L_DEFFNR:
    D(RETPCL) = D(NRETCL);
    goto L_DEFFS1;
    /*_*/
}
int
LOAD(retval)
    ret_t retval;
{
    ENTRY(LOAD)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVAL(WPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    /* FALL */
    BRANCH(LOAD2)
}
static int
LOAD2(retval)
    ret_t retval;
{
    ENTRY(LOAD2)
    X_LOCSP(VSP,WPTR);
    X_LOCSP(XSP,XPTR);
    switch (STREAM(YSP, XSP, &VARATB)) {
    case ST_ERROR:
    case ST_EOS:
	BRANCH(PROTER)
    }
    if (D_A(STYPE) != LPTYP)
	BRANCH(PROTER)
    SAVSTK();
    PUSH(YSPPTR);
    GENVUP(XPTR);
    SAVSTK();
    PUSH(XPTR);
    FINDEX(ZCL);
    D(YCL) = D(ZEROCL);
L_LOAD4:
    S_L(XSP)--;
    S_O(XSP)++;
    switch (STREAM(ZSP, XSP, &VARATB)) {
    case ST_ERROR:
	goto L_LOAD1;
    case ST_EOS:
	BRANCH(PROTER)
    }
    switch (D_A(STYPE)) {
    case 1:
	BRANCH(PROTER)
    case 3:
	goto L_LOAD6;
    }
    SAVSTK();
    PUSH(ZSPPTR);
    GENVUP(XPTR);
    if (!LOCAPV(XPTR,DTATL,XPTR))
	goto L_LOAD9;
    D(XPTR) = D(D_A(XPTR) + DESCR);
    PUSH(XPTR);
L_LOAD10:
    D_A(YCL)++;
    goto L_LOAD4;
    /*_*/
L_LOAD6:
    D_A(YCL)++;
    SAVSTK();
    PUSH(ZSPPTR);
    GENVAR(XPTR);
    if (!LOCAPV(XPTR,DTATL,XPTR))
	goto L_LOAD11;
    D(XPTR) = D(D_A(XPTR) + DESCR);
    PUSH(XPTR);
L_LOAD13:
    S_L(XSP)--;
    S_O(XSP)++;
    SAVSTK();
    PUSH(XSPPTR);
    GENVAR(XPTR);
    if (!LOCAPV(XPTR,DTATL,XPTR))
	goto L_LOAD7;
    D(XPTR) = D(D_A(XPTR) + DESCR);
    PUSH(XPTR);
L_LOAD8:
    D_V(LODCL) = D_A(YCL);
    D_A(YCL)++;
    D_A(XCL) = D_A(YCL);
    D_A(XCL) *= DESCR;
    D_F(XCL) = D_V(XCL) = 0;
    D_A(XCL) += DESCR;
    D_V(XCL) = B;
    SAVSTK();
    PUSH(XCL);
    BLOCK(ZPTR);
    D(XPTR) = D(ZPTR);
    D_A(XPTR) += D_A(XCL);
L_LOAD12:
    D_A(XPTR) -= DESCR;
    POP(YPTR);
    D(D_A(XPTR) + DESCR) = D(YPTR);
    D_A(YCL)--;
    if (D_A(YCL) > 0)
	goto L_LOAD12;
    if (!_LOAD(YPTR,YSP,VSP))
	BRANCH(FAIL)
    D(D_A(XPTR)) = D(YPTR);
    D(D_A(ZCL)) = D(LODCL);
    D(D_A(ZCL) + DESCR) = D(ZPTR);
    BRANCH(RETNUL)
    /*_*/
L_LOAD7:
    PUSH(ZEROCL);
    goto L_LOAD8;
    /*_*/
L_LOAD9:
    PUSH(ZEROCL);
    goto L_LOAD10;
    /*_*/
L_LOAD1:
    PUSH(ZEROCL);
    _SPEC(TSP) = _SPEC(XSP);
    S_L(TSP) = 1;
    D_A(YCL)++;
    if (LEXEQ(TSP,RPRNSP))
	goto L_LOAD13;
    else
	goto L_LOAD4;
    /*_*/
L_LOAD11:
    PUSH(ZEROCL);
    goto L_LOAD13;
    /*_*/
}
int
UNLOAD(retval)
    ret_t retval;
{
    ENTRY(UNLOAD)
    SAVSTK();
    switch (VARVUP(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    SAVSTK();
    PUSH(XPTR);
    FINDEX(ZCL);
    D(D_A(ZCL)) = D(UNDFCL);
    X_LOCSP(XSP,XPTR);
    _UNLOAD(XSP);
    BRANCH(RETNUL)
    /*_*/
}
int
LNKFNC(retval)
    ret_t retval;
{
    ENTRY(LNKFNC)
    D_A(XCL) = D_V(INCL);
    D_F(XCL) = D_V(XCL) = 0;
    D(YCL) = D(INCL);
    D(WCL) = D(D_A(YCL));
    D_A(WCL) = D_V(WCL);
    D_F(WCL) = D_V(WCL) = 0;
    D(WPTR) = D(ZEROCL);
    D(ZCL) = D(D_A(YCL) + DESCR);
    PSTACK(YPTR);
    D_A(TCL) = (int_t) (2*DESCR);
L_LNKF1:
    PUSH(XCL);
    PUSH(ZCL);
    PUSH(TCL);
    PUSH(YPTR);
    PUSH(WCL);
    PUSH(YCL);
    PUSH(WPTR);
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(WPTR);
    POP(YCL);
    POP(WCL);
    POP(YPTR);
    POP(TCL);
    POP(ZCL);
    POP(XCL);
    D_A(WCL)--;
    if (D_A(WCL) < 0)
	goto L_LNKF6;
    D(ZPTR) = D(D_A(ZCL) + D_A(TCL));
    if (D_V(ZPTR) == 0)
	goto L_LNKF6;
    if (D_V(ZPTR) == D_V(XPTR))
	goto L_LNKF6;
    D_A(DTCL) = D_V(XPTR);
    D_F(DTCL) = D_V(DTCL) = 0;
    D_V(DTCL) = D_V(ZPTR);
    if (DCMP(DTCL, VIDTP))
	goto L_LNKVI;
    if (DCMP(DTCL, IVDTP))
	goto L_LNKIV;
    if (DCMP(DTCL, RIDTP))
	goto L_LNKRI;
    if (DCMP(DTCL, IRDTP))
	goto L_LNKIR;
    if (DCMP(DTCL, RVDTP))
	goto L_LNKRV;
    if (DCMP(DTCL, VRDTP))
	goto L_LNKVR;
    else
	BRANCH(INTR1)
L_LNKIV:
    SAVSTK();
    PUSH(XPTR);
    switch (GNVARI(XPTR)) {
    case 1:
	goto L_LNKF6;
    }
    /*_*/
L_LNKRI:
    CLR_MATH_ERROR();
    D_A(XPTR) = (int_t) D_RV(XPTR);
    D_F(XPTR) = 0;
    D_V(XPTR) = I;
    if (MATH_ERROR())
	BRANCH(INTR1)
    goto L_LNKF6;
    /*_*/
L_LNKIR:
    D_RV(XPTR) = (real_t)D_A(XPTR);
    D_F(XPTR) = 0;
    D_V(XPTR) = R;
    goto L_LNKF6;
    /*_*/
L_LNKVR:
    X_LOCSP(XSP,XPTR);
    if (SPCINT(XPTR,XSP))
	goto L_LNKIR;
    if (SPREAL(XPTR,XSP))
	goto L_LNKF6;
    else
	BRANCH(INTR1)
    /*_*/
L_LNKRV:
    REALST(XSP,XPTR);
    SAVSTK();
    PUSH(XSPPTR);
    switch (GENVAR(XPTR)) {
    case 1:
	goto L_LNKF6;
    }
    /*_*/
L_LNKVI:
    X_LOCSP(XSP,XPTR);
    if (SPCINT(XPTR,XSP))
	goto L_LNKF6;
    if (SPREAL(XPTR,XSP))
	goto L_LNKRI;
    else
	BRANCH(INTR1)
L_LNKF6:
    D_A(TCL) += DESCR;
    PUSH(XPTR);
    D_A(WPTR)++;
    D_A(XCL)--;
    if (D_A(XCL) > 0)
	goto L_LNKF1;
L_LNKF4:
    if (D_A(WCL) <= 0)
	goto L_LNKF5;
    PUSH(NULVCL);
    D_A(WPTR)++;
    D_A(WCL)--;
    goto L_LNKF4;
    /*_*/
L_LNKF5:
    D_A(WCL) = D_V(D_A(ZCL));
    D_F(WCL) = D_V(WCL) = 0;
    D(XPTR) = D(ZCL);
    D_A(XPTR) += D_A(WCL);
    D(ZPTR) = D(D_A(XPTR));
    D(ZCL) = D(D_A(ZCL) + DESCR);
    D_A(YPTR) += 2*DESCR;
    if (!CALLX(ZPTR,YPTR,WPTR,ZCL))
	BRANCH(FAIL)
    if (D_V(ZPTR) != L)
	BRANCH(RTZPTR)
    _SPEC(ZSP) = _SPEC(D_A(ZPTR));
    BRANCH(GENVRZ)
    /*_*/
}
int
ARRAY(retval)
    ret_t retval;
{
    ENTRY(ARRAY)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (ARGVAL(TPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    D_A(ARRMRK) = 0;
    D(WCL) = D(ZEROCL);
    D(XCL) = D(ONECL);
    X_LOCSP(XSP,XPTR);
    PUSH(XPTR);
L_ARRAY1:
    switch (STREAM(YSP, XSP, &NUMBTB)) {
    case ST_ERROR:
	BRANCH(PROTER)
    case ST_EOS:
	goto L_ARROT1;
    }
    if (!SPCINT(YCL,YSP))
	BRANCH(PROTER)
    switch (D_A(STYPE)) {
    case 2:
	goto L_ARRAY3;
    }
    S_L(XSP)--;
    S_O(XSP)++;
    switch (STREAM(ZSP, XSP, &NUMBTB)) {
    case ST_ERROR:
	BRANCH(PROTER)
    case ST_EOS:
	goto L_ARROT2;
    }
    if (!SPCINT(ZCL,ZSP))
	BRANCH(PROTER)
    switch (D_A(STYPE)) {
    case 1:
	BRANCH(PROTER)
    case 2:
	goto L_ARRAY5;
    }
    /*_*/
L_ARRAY3:
    if (D_A(YCL) <= 0)
	BRANCH(PROTER)
    D(ZCL) = D(YCL);
    D_A(YCL) = 1;
    goto L_ARRAY6;
    /*_*/
L_ARRAY5:
    D_A(ZCL) -= D_A(YCL);
    D_A(ZCL) += D_A(ONECL);
    if (D_A(ZCL) < 0)
	BRANCH(PROTER)
L_ARRAY6:
    D_V(YCL) = D_A(ZCL);
    PUSH(YCL);
    CLR_MATH_ERROR();
    D_A(XCL) *= D_A(ZCL);
    if (MATH_ERROR())
	BRANCH(PROTER)
    D_A(WCL)++;
    if (D_A(ARRMRK) != 0)
	goto L_ARRAY7;
    S_L(XSP)--;
    S_O(XSP)++;
    goto L_ARRAY1;
    /*_*/
L_ARROT1:
    D_A(ARRMRK) = 1;
    if (SPCINT(YCL,YSP))
	goto L_ARRAY3;
    else
	BRANCH(PROTER)
    /*_*/
L_ARROT2:
    D_A(ARRMRK) = 1;
    if (SPCINT(ZCL,ZSP))
	goto L_ARRAY5;
    else
	BRANCH(PROTER)
    /*_*/
L_ARRAY7:
    D(ZCL) = D(XCL);
    D_A(ZCL) += D_A(WCL);
    D_A(ZCL) += 2;
    D_A(ZCL) *= DESCR;
    D_F(ZCL) = D_V(ZCL) = 0;
    D_V(ZCL) = A;
    SAVSTK();
    PUSH(ZCL);
    BLOCK(ZPTR);
    D(XPTR) = D(ZPTR);
    D(WPTR) = D(XPTR);
    D_A(WPTR) += D_A(ZCL);
    D(D_A(ZPTR) + 2*DESCR) = D(WCL);
    D_A(XPTR) += DESCR;
L_ARRAY8:
    D_A(XPTR) += DESCR;
    POP(YPTR);
    D(D_A(XPTR) + DESCR) = D(YPTR);
    D_A(WCL)--;
    if (D_A(WCL) == 0)
	goto L_ARRFIL;
    if (D_A(WCL) > 0)
	goto L_ARRAY8;
L_ARRAY9:
    D(D_A(XPTR) + DESCR) = D(TPTR);
L_ARRFIL:
    D_A(XPTR) += DESCR;
    if (D_PTR(XPTR) < D_PTR(WPTR))
	goto L_ARRAY9;
    if (D_PTR(XPTR) > D_PTR(WPTR))
	BRANCH(INTR10)
    POP(WPTR);
    D(D_A(ZPTR) + DESCR) = D(WPTR);
    BRANCH(RTZPTR)
    /*_*/
}
int
ASSOC(retval)
    ret_t retval;
{
    ENTRY(ASSOC)
    SAVSTK();
    switch (INTVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (INTVAL(WPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    CLR_MATH_ERROR();
    D(ZPTR) = D(WPTR);
    D_A(ZPTR) *= D_A(DSCRTW);
    if (MATH_ERROR())
	BRANCH(SIZERR)
    D_A(ZPTR) += 2*DESCR;
    if (D_A(ZPTR) >= D_A(SIZLMT))
	BRANCH(SIZERR)
    POP(XPTR);
    if (D_A(XPTR) < 0)
	BRANCH(LENERR)
    if (D_A(XPTR) > 0)
	goto L_ASSOC1;
    D_A(XPTR) = (int_t) EXTSIZ;
L_ASSOC1:
    D_A(XPTR)++;
    D_A(XPTR) *= 2*DESCR;
    D_F(XPTR) = D_V(XPTR) = 0;
    if (D_A(WPTR) < 0)
	BRANCH(LENERR)
    if (D_A(WPTR) > 0)
	goto L_ASSOC4;
    D_A(WPTR) = (int_t) EXTSIZ;
L_ASSOC4:
    D_A(WPTR)++;
    D_A(WPTR) *= 2*DESCR;
    D_F(WPTR) = D_V(WPTR) = 0;
    D_V(XPTR) = T;
    /* FALL */
    BRANCH(ASSOCE)
}
static int
ASSOCE(retval)
    ret_t retval;
{
    ENTRY(ASSOCE)
    SAVSTK();
    PUSH(XPTR);
    BLOCK(ZPTR);
    D(D_A(ZPTR) + D_A(XPTR)) = D(ONECL);
    D_A(XPTR) -= DESCR;
    D(D_A(ZPTR) + D_A(XPTR)) = D(WPTR);
L_ASSOC2:
    D_A(XPTR) -= 2*DESCR;
    D(D_A(ZPTR) + D_A(XPTR)) = D(NULVCL);
    if (D_A(XPTR) == DESCR)
	BRANCH(RTZPTR)
    else
	goto L_ASSOC2;
    /*_*/
}
int
DATDEF(retval)
    ret_t retval;
{
    ENTRY(DATDEF)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    D_A(DATACL) = 0;
    X_LOCSP(XSP,XPTR);
    switch (STREAM(YSP, XSP, &VARATB)) {
    case ST_ERROR:
    case ST_EOS:
	BRANCH(PROTER)
    }
    if (D_A(STYPE) != LPTYP)
	BRANCH(PROTER)
    SAVSTK();
    PUSH(YSPPTR);
    GENVUP(XPTR);
    SAVSTK();
    PUSH(XPTR);
    FINDEX(ZCL);
    D_V(DATSEG)++;
    if (D_V(DATSEG) == DATSIZ)
	BRANCH(INTR27)
    D(YCL) = D(ZEROCL);
    SAVSTK();
    PUSH(XPTR);
    PUSH(DATSEG);
    PUSH(DTATL);
    AUGATL(DTATL);
    PSTACK(WPTR);
    PUSH(DATSEG);
    PUSH(XPTR);
L_DATA3:
    S_L(XSP)--;
    S_O(XSP)++;
    if (D_A(DATACL) != 0)
	goto L_DAT5;
    switch (STREAM(YSP, XSP, &VARATB)) {
    case ST_ERROR:
    case ST_EOS:
	BRANCH(PROTER)
    }
    switch (D_A(STYPE)) {
    case 1:
	BRANCH(PROTER)
    case 3:
	goto L_DATA6;
    }
L_DATA4:
    if (S_L(YSP) == 0)
	goto L_DATA3;
    SAVSTK();
    PUSH(YSPPTR);
    GENVUP(XPTR);
    PUSH(XPTR);
    SAVSTK();
    PUSH(XPTR);
    FINDEX(XCL);
    D(WCL) = D(D_A(XCL));
    if (!DCMP(WCL, FLDCL))
	goto L_DAT6;
    D(ZPTR) = D(D_A(XCL) + DESCR);
    D_A(TCL) = D_A(YCL);
    D_A(TCL) *= DESCR;
    D_F(TCL) = D_V(TCL) = 0;
    SAVSTK();
    PUSH(TCL);
    PUSH(DATSEG);
    PUSH(ZPTR);
    AUGATL(ZPTR);
L_DAT7:
    D(D_A(XCL) + DESCR) = D(ZPTR);
    D_A(YCL)++;
    goto L_DATA3;
    /*_*/
L_DATA6:
    D_A(DATACL) = 1;
    goto L_DATA4;
    /*_*/
L_DAT5:
    if (S_L(XSP) != 0)
	BRANCH(PROTER)
    if (D_A(YCL) == 0)
	BRANCH(PROTER)
    D_V(DATCL) = D_A(YCL);
    D(D_A(ZCL)) = D(DATCL);
    D_A(YCL) *= DESCR;
    D_F(YCL) = D_V(YCL) = 0;
    D_A(YCL) += 2*DESCR;
    D_V(YCL) = D_V(DATSEG);
    SAVSTK();
    PUSH(YCL);
    BLOCK(ZPTR);
    D_A(WPTR) += DESCR;
    MOVBLK(D_A(ZPTR),D_A(WPTR),D_A(YCL));
    D(D_A(ZCL) + DESCR) = D(ZPTR);
    BRANCH(RETNUL)
    /*_*/
L_DAT6:
    D(D_A(XCL)) = D(FLDCL);
    SAVSTK();
    PUSH(TWOCL);
    BLOCK(ZPTR);
    D(D_A(ZPTR) + DESCR) = D(DATSEG);
    D_A(TCL) = D_A(YCL);
    D_A(TCL) *= DESCR;
    D_F(TCL) = D_V(TCL) = 0;
    D(D_A(ZPTR) + 2*DESCR) = D(TCL);
    goto L_DAT7;
    /*_*/
}
int
PROTO(retval)
    ret_t retval;
{
    ENTRY(PROTO)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(XPTR) != A)
	BRANCH(NONARY)
    D(ZPTR) = D(D_A(XPTR) + DESCR);
    BRANCH(RTZPTR)
    /*_*/
}
int
FREEZE(retval)
    ret_t retval;
{
    ENTRY(FREEZE)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(XPTR) != T)
	BRANCH(NONARY)
    D_F(D_A(XPTR)) |= FRZN;
    BRANCH(RETNUL)
    /*_*/
}
int
THAW(retval)
    ret_t retval;
{
    ENTRY(THAW)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(XPTR) != T)
	BRANCH(NONARY)
    D_F(D_A(XPTR)) &= ~(FRZN);
    BRANCH(RETNUL)
    /*_*/
}
int
ITEM(retval)
    ret_t retval;
{
    ENTRY(ITEM)
    D_A(XCL) = D_V(INCL);
    D_F(XCL) = D_V(XCL) = 0;
    D_A(XCL)--;
    PUSH(XCL);
    SAVSTK();
    switch (ARGVAL(YCL)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XCL);
    if (D_V(YCL) == A)
	goto L_ARYAD3;
    if (D_V(YCL) == T)
	goto L_ASSCR;
    else
	BRANCH(NONARY)
L_ARYAD3:
    D(WCL) = D(XCL);
L_ARYAD1:
    if (D_A(XCL) <= 0)
	goto L_ARYAD2;
    PUSH(XCL);
    PUSH(WCL);
    PUSH(YCL);
    SAVSTK();
    switch (INTVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(YCL);
    POP(WCL);
    POP(XCL);
    PUSH(XPTR);
    D_A(XCL)--;
    goto L_ARYAD1;
    /*_*/
L_ARYAD2:
    D(ZPTR) = D(ZEROCL);
    D(ZCL) = D(D_A(YCL) + 2*DESCR);
    D_A(YPTR) = D_A(ZCL);
    D_A(YPTR) *= DESCR;
    D_F(YPTR) = D_V(YPTR) = 0;
    D_A(YPTR) = D_A(YCL) + D_A(YPTR);
    D_F(YPTR) = D_F(YCL);
    D_V(YPTR) = D_V(YCL);
    D_A(YPTR) += 2*DESCR;
L_ARYAD7:
    if (D_A(WCL) == D_A(ZCL))
	goto L_ARYAD9;
    if (D_A(WCL) > D_A(ZCL))
	BRANCH(ARGNER)
    PUSH(ZEROCL);
    D_A(WCL)++;
    goto L_ARYAD7;
    /*_*/
L_ARYAD9:
    D_A(YCL) += 2*DESCR;
    D(WPTR) = D(D_A(YCL) + DESCR);
    D_A(TPTR) = D_V(WPTR);
    D_F(TPTR) = D_V(TPTR) = 0;
L_ARYA11:
    POP(XPTR);
    D_A(XPTR) -= D_A(WPTR);
    if (D_A(XPTR) < 0)
	BRANCH(FAIL)
    if (D_A(XPTR) >= D_A(TPTR))
	BRANCH(FAIL)
    D_A(XPTR) = D_A(ZPTR) + D_A(XPTR);
    D_F(XPTR) = D_F(ZPTR);
    D_V(XPTR) = D_V(ZPTR);
    D_A(ZCL)--;
    if (D_A(ZCL) == 0)
	goto L_ARYA12;
    D_A(YCL) += DESCR;
    D(WPTR) = D(D_A(YCL) + DESCR);
    D_A(TPTR) = D_V(WPTR);
    D_F(TPTR) = D_V(TPTR) = 0;
    D(ZPTR) = D(XPTR);
    D_A(ZPTR) *= D_A(TPTR);
    goto L_ARYA11;
    /*_*/
L_ARYA12:
    D_A(XPTR) *= DESCR;
    D_F(XPTR) = D_V(XPTR) = 0;
    D_A(XPTR) = D_A(YPTR) + D_A(XPTR);
    D_F(XPTR) = D_F(YPTR);
    D_V(XPTR) = D_V(YPTR);
L_ARYA10:
    D_V(XPTR) = N;
    BRANCH(RTXNAM)
    /*_*/
L_ASSCR:
    if (D_A(XCL) != 1)
	BRANCH(ARGNER)
    PUSH(YCL);
    SAVSTK();
    switch (ARGVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    D(YCL) = D(XPTR);
L_ASSCR5:
    if (LOCAPV(XPTR,XPTR,YPTR))
	goto L_ARYA10;
    D_A(TCL) = D_V(D_A(XPTR));
    D_F(TCL) = D_V(TCL) = 0;
    D(ZPTR) = D(D_A(XPTR) + D_A(TCL));
    if (D_A(ZPTR) == 1)
	goto L_ASSCR6;
    D(XPTR) = D(ZPTR);
    goto L_ASSCR5;
    /*_*/
L_ASSCR6:
    if ((D_F(D_A(YCL)) & FRZN))
	BRANCH(RETNUL)
    if (!LOCAPV(XPTR,XPTR,ZEROCL))
	goto L_ASSCR3;
L_ASSCR8:
    D(D_A(XPTR) + 2*DESCR) = D(YPTR);
    goto L_ARYA10;
    /*_*/
L_ASSCR3:
    D_A(TCL) -= DESCR;
    D(WPTR) = D(D_A(XPTR) + D_A(TCL));
    D(ZCL) = D(XPTR);
    SAVSTK();
    PUSH(WPTR);
    BLOCK(ZPTR);
    D(XPTR) = D(WPTR);
    SAVSTK();
    switch (ASSOCE(XPTR)) {
    case 1:
    case 2:
	BRANCH(INTR10)
    }
    D_A(TCL) += DESCR;
    D(D_A(ZCL) + D_A(TCL)) = D(XPTR);
    goto L_ASSCR8;
    /*_*/
}
int
DEFDAT(retval)
    ret_t retval;
{
    ENTRY(DEFDAT)
    D_A(XCL) = D_V(INCL);
    D_F(XCL) = D_V(XCL) = 0;
    D(WCL) = D(XCL);
    D(YCL) = D(INCL);
    PSTACK(YPTR);
L_DEFD1:
    D_A(OCICL) += DESCR;
    D(XPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(XPTR) & FNC))
	goto L_DEFDC;
L_DEFD2:
    if (D_A(INSW) == 0)
	goto L_DEFD8;
    if (!LOCAPV(ZPTR,INATL,XPTR))
	goto L_DEFD8;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    PUSH(XCL);
    PUSH(WCL);
    PUSH(YCL);
    PUSH(YPTR);
    SAVSTK();
    PUSH(XPTR);
    PUSH(ZPTR);
    switch (PUTIN(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(YPTR);
    POP(YCL);
    POP(WCL);
    POP(XCL);
    goto L_DEFD3;
    /*_*/
L_DEFD8:
    D(XPTR) = D(D_A(XPTR) + DESCR);
L_DEFD3:
    PUSH(XPTR);
    D_A(XCL)--;
    if (D_A(XCL) < 0)
	BRANCH(INTR10)
    if (D_A(XCL) > 0)
	goto L_DEFD1;
    D(XCL) = D(D_A(YCL));
    D_A(XCL) = D_V(XCL);
    D_F(XCL) = D_V(XCL) = 0;
L_DEFD4:
    if (D_A(WCL) >= D_A(XCL))
	goto L_DEFD5;
    PUSH(NULVCL);
    D_A(WCL)++;
    goto L_DEFD4;
    /*_*/
L_DEFD5:
    D(WCL) = D(D_A(YCL) + DESCR);
    D_A(XCL) *= DESCR;
    D_F(XCL) = D_V(XCL) = 0;
    D_V(XCL) = D_V(WCL);
    SAVSTK();
    PUSH(XCL);
    BLOCK(ZPTR);
    D_A(YPTR) += DESCR;
    MOVBLK(D_A(ZPTR),D_A(YPTR),D_A(XCL));
    BRANCH(RTZPTR)
    /*_*/
L_DEFDC:
    PUSH(XCL);
    PUSH(WCL);
    PUSH(YCL);
    PUSH(YPTR);
    SAVSTK();
    PUSH(XPTR);
    switch (INVOKE(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_DEFDN;
    }
    POP(YPTR);
    POP(YCL);
    POP(WCL);
    POP(XCL);
    goto L_DEFD3;
    /*_*/
L_DEFDN:
    POP(YPTR);
    POP(YCL);
    POP(WCL);
    POP(XCL);
    goto L_DEFD2;
    /*_*/
}
int
FIELD(retval)
    ret_t retval;
{
    ENTRY(FIELD)
    PUSH(INCL);
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (DCMP(XPTR, NULVCL))
	BRANCH(NONAME)
    POP(YCL);
    if (D_V(XPTR) == I)
	goto L_FIELD2;
    if (D_V(XPTR) != S)
	goto L_FIELD1;
    if (D_A(CASECL) == 0)
	goto L_FIELD1;
    SAVSTK();
    switch (VPXPTR(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
L_FIELD1:
    D_V(DT1CL) = D_V(XPTR);
    D(YPTR) = D(D_A(YCL) + DESCR);
    if (!LOCAPT(ZCL,YPTR,DT1CL))
	BRANCH(INTR1)
    D(ZCL) = D(D_A(ZCL) + 2*DESCR);
    D_A(XPTR) += D_A(ZCL);
    D_V(XPTR) = N;
    BRANCH(RTXNAM)
    /*_*/
L_FIELD2:
    SAVSTK();
    PUSH(XPTR);
    switch (GNVARI(XPTR)) {
    case 1:
	goto L_FIELD1;
    }
    /*_*/
}
int
RSORT(retval)
    ret_t retval;
{
    ENTRY(RSORT)
    D_A(SCL) = 1;
    BRANCH(SORT1)
    /*_*/
}
int
SORT(retval)
    ret_t retval;
{
    ENTRY(SORT)
    D_A(SCL) = 0;
    /* FALL */
    BRANCH(SORT1)
}
static int
SORT1(retval)
    ret_t retval;
{
    ENTRY(SORT1)
    D_A(WCL) = D_V(INCL);
    D_F(WCL) = D_V(WCL) = 0;
    PUSH(WCL);
    PUSH(SCL);
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    D(WPTR) = D(XPTR);
    if (D_V(XPTR) == A)
	goto L_SORT2;
    if (D_V(XPTR) != T)
	BRANCH(NONARY)
    SAVSTK();
    PUSH(XPTR);
    switch (ICNVTA(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
L_SORT2:
    POP(SCL);
    POP(WCL);
    D(XCL) = D(D_A(XPTR) + 2*DESCR);
    D_A(YPTR) = D_A(XCL);
    D_A(YPTR) *= DESCR;
    D_F(YPTR) = D_V(YPTR) = 0;
    D_A(YPTR) = D_A(XPTR) + D_A(YPTR);
    D_F(YPTR) = D_F(XPTR);
    D_V(YPTR) = D_V(XPTR);
    D_A(YPTR) += 2*DESCR;
    D(YCL) = D(ONECL);
    if (D_A(XCL) < 2)
	goto L_SORT3;
    if (D_A(XCL) > 2)
	BRANCH(INTR30)
    D(YCL) = D(D_A(XPTR) + 3*DESCR);
    D_A(YCL) = D_V(YCL);
    D_F(YCL) = D_V(YCL) = 0;
L_SORT3:
    D(ZCL) = D(D_A(YPTR));
    D_A(ZCL) = D_V(ZCL);
    D_F(ZCL) = D_V(ZCL) = 0;
    D_A(ZCL) *= DESCR;
    D_F(ZCL) = D_V(ZCL) = 0;
    D(ZPTR) = D(YPTR);
    if (D_A(WCL) < 2)
	goto L_SORT5;
    if (D_A(WCL) > 2)
	BRANCH(ARGNER)
    PUSH(WPTR);
    PUSH(XPTR);
    PUSH(YPTR);
    PUSH(ZPTR);
    PUSH(XCL);
    PUSH(YCL);
    PUSH(ZCL);
    PUSH(SCL);
    SAVSTK();
    switch (ARGVAL(WCL)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(SCL);
    POP(ZCL);
    POP(YCL);
    POP(XCL);
    POP(ZPTR);
    POP(YPTR);
    POP(XPTR);
    POP(WPTR);
    D(A3PTR) = D(ZEROCL);
    if (D_V(WCL) == I)
	goto L_SORT3C;
    if (D_V(WCL) == S)
	goto L_SORT3B;
    if (D_V(WCL) == R)
	goto L_SORT3A;
    else
	BRANCH(INTR30)
    /*_*/
L_SORT3A:
    CLR_MATH_ERROR();
    D_A(WCL) = (int_t) D_RV(WCL);
    D_F(WCL) = 0;
    D_V(WCL) = I;
    if (MATH_ERROR())
	BRANCH(INTR30)
    goto L_SORT3C;
L_SORT3B:
    X_LOCSP(XSP,WCL);
    if (SPCINT(WCL,XSP))
	goto L_SORT3C;
    if (SPREAL(WCL,XSP))
	goto L_SORT3A;
    if (D_A(XCL) != 1)
	BRANCH(INTR30)
    if (!LOCAPV(WCL,FNCPL,WCL))
	BRANCH(INTR30)
    D(WCL) = D(D_A(WCL) + DESCR);
    D(TCL) = D(D_A(WCL));
    if (D_A(TCL) != D_A(FLDCL))
	BRANCH(INTR30)
    D(A3PTR) = D(D_A(WCL) + DESCR);
    D(WCL) = D(ONECL);
L_SORT3C:
    if (D_A(XCL) != 1)
	goto L_SORT4;
    if (D_A(WCL) == 1)
	goto L_SORT5;
    else
	BRANCH(INTR30)
    /*_*/
L_SORT4:
    D(TCL) = D(D_A(XPTR) + 3*DESCR);
    CLR_MATH_ERROR();
    D_A(WCL) -= D_A(TCL);
    if (MATH_ERROR())
	BRANCH(INTR30)
    goto L_SORT4A;
L_SORT4A:
    if (D_A(WCL) < 0)
	BRANCH(INTR30)
    if (D_A(WCL) >= D_A(YCL))
	BRANCH(INTR30)
    CLR_MATH_ERROR();
    D_A(WCL) *= D_A(ZCL);
    if (MATH_ERROR())
	BRANCH(INTR30)
    D(ZPTR) = D(YPTR);
    D_A(ZPTR) += D_A(WCL);
L_SORT5:
    D_A(XCL) = 0;
    if (D_V(WPTR) == A)
	goto L_SORTA;
    D_A(TCL) = D_V(D_A(XPTR));
    D_F(TCL) = D_V(TCL) = 0;
    D(TPTR) = D(XPTR);
    D_A(TPTR) += D_A(TCL);
    D_A(TPTR) -= D_A(ZCL);
    D_A(A4PTR) = (int_t) (-DESCR);
    D_A(A5PTR) = 0;
    if (D_A(ZPTR) == D_A(YPTR))
	goto L_SORTT1;
    D_A(A5PTR) = (int_t) (-DESCR);
L_SORTT1:
    D_A(WCL) = D_V(D_A(WPTR));
    D_F(WCL) = D_V(WCL) = 0;
    D_A(WCL) -= 2*DESCR;
    D_A(WCL) = D_A(WPTR) + D_A(WCL);
    D_F(WCL) = D_F(WPTR);
    D_V(WCL) = D_V(WPTR);
L_SORTT2:
    D(TCL) = D(D_A(WPTR) + DESCR);
    if (DCMP(TCL, NULVCL))
	goto L_SORTT3;
    D_A(XCL) += DESCR;
    D(A6PTR) = D(WPTR);
    D_A(A6PTR) += D_A(A5PTR);
    D_A(A6PTR) += 2*DESCR;
    D(D_A(TPTR) + D_A(XCL)) = D(A6PTR);
    if (D_A(XCL) == D_A(ZCL))
	goto L_SORTGO;
L_SORTT3:
    D_A(WPTR) += 2*DESCR;
    if (D_A(WCL) != D_A(WPTR))
	goto L_SORTT2;
    D(WPTR) = D(D_A(WCL) + 2*DESCR);
    goto L_SORTT1;
    /*_*/
L_SORTA:
    D_A(WCL) = D_V(D_A(WPTR));
    D_F(WCL) = D_V(WCL) = 0;
    D_V(WCL) = A;
    SAVSTK();
    PUSH(WCL);
    BLOCK(XPTR);
    D_A(A4PTR) = (int_t) (4*DESCR);
    MOVBLK(D_A(XPTR),D_A(WPTR),D_A(A4PTR));
    D(TPTR) = D(XPTR);
    D_A(TPTR) += D_A(WCL);
    D_A(TPTR) -= D_A(ZCL);
    D(A5PTR) = D(ZPTR);
    D_A(A5PTR) -= D_A(YPTR);
    D_F(A5PTR) &= ~(PTR);
    D_A(A4PTR) = D_A(ZCL);
L_SORTA1:
    D_A(XCL) += DESCR;
    D_A(ZPTR) += DESCR;
    D(D_A(TPTR) + D_A(XCL)) = D(ZPTR);
    if (D_A(XCL) != D_A(ZCL))
	goto L_SORTA1;
L_SORTGO:
    D_A(A6PTR) = D_A(ZCL);
L_SORT6:
    if (D_A(A6PTR) <= DESCR)
	goto L_SORT12;
    D_A(A6PTR) /= D_A(TWOCL);
    D_A(A6PTR) *= DESCR;
    D_F(A6PTR) = D_V(A6PTR) = 0;
    D(XCL) = D(ZCL);
    D_A(XCL) -= D_A(A6PTR);
L_SORT7:
    D(TCL) = D(ZEROCL);
    D_A(A7PTR) = (int_t) DESCR;
L_SORT8:
    D(LPTR) = D(A7PTR);
    D_A(LPTR) += D_A(A6PTR);
    D(F1PTR) = D(D_A(TPTR) + D_A(A7PTR));
    D(F2PTR) = D(D_A(TPTR) + D_A(LPTR));
    D(A1PTR) = D(D_A(F1PTR));
    D(A2PTR) = D(D_A(F2PTR));
L_SORT9:
    D_A(DTCL) = D_V(A1PTR);
    D_F(DTCL) = D_V(DTCL) = 0;
    D_V(DTCL) = D_V(A2PTR);
    if (DCMP(DTCL, VVDTP))
	goto L_CVV;
    if (DCMP(DTCL, IIDTP))
	goto L_CII;
    if (DCMP(DTCL, RIDTP))
	goto L_CRI;
    if (DCMP(DTCL, IRDTP))
	goto L_CIRX;
    if (DCMP(DTCL, RRDTP))
	goto L_CRR;
    goto L_COTH;
    /*_*/
L_CVV:
    X_LOCSP(XSP,A1PTR);
    X_LOCSP(YSP,A2PTR);
    {
	int x = LEXCMP(XSP,YSP);
	if (x < 0)
	    goto L_CMPLT;
	if (x == 0)
	    goto L_CMPEQ;
	goto L_CMPGT;
    }
L_CIRX:
    D_RV(A1PTR) = (real_t)D_A(A1PTR);
    D_F(A1PTR) = 0;
    D_V(A1PTR) = R;
    goto L_CRR;
    /*_*/
L_CRI:
    D_RV(A2PTR) = (real_t)D_A(A2PTR);
    D_F(A2PTR) = 0;
    D_V(A2PTR) = R;
    goto L_CRR;
    /*_*/
L_CII:
    if (D_A(A1PTR) < D_A(A2PTR))
	goto L_CMPLT;
    if (D_A(A1PTR) == D_A(A2PTR))
	goto L_CMPEQ;
    goto L_CMPGT;
    /*_*/
L_CRR:
    if (isnan(D_RV(A1PTR)))
	goto L_CRR1;
    if (isnan(D_RV(A2PTR)))
	goto L_CMPLT;
    if (D_RV(A1PTR) < D_RV(A2PTR))
	goto L_CMPLT;
    if (D_RV(A1PTR) == D_RV(A2PTR))
	goto L_CMPEQ;
    goto L_CMPGT;
    /*_*/
L_CRR1:
    if (isnan(D_RV(A2PTR)))
	goto L_CMPEQ;
    else
	goto L_CMPGT;
    /*_*/
L_CMPEQ:
    if (D_PTR(F1PTR) <= D_PTR(F2PTR))
	goto L_CMPNXT;
    else
	goto L_SWAP;
L_CMPGT:
    if (D_A(SCL) == 0)
	goto L_SWAP;
    else
	goto L_CMPNXT;
L_CMPLT:
    if (D_A(SCL) == 0)
	goto L_CMPNXT;
L_SWAP:
    D(D_A(TPTR) + D_A(A7PTR)) = D(F2PTR);
    D(D_A(TPTR) + D_A(LPTR)) = D(F1PTR);
    D_A(TCL)++;
L_CMPNXT:
    if (D_A(A7PTR) >= D_A(XCL))
	goto L_SORT11;
    D_A(A7PTR) += DESCR;
    goto L_SORT8;
    /*_*/
L_SORT11:
    if (D_A(TCL) == 0)
	goto L_SORT6;
    else
	goto L_SORT7;
    /*_*/
L_COTH:
    if (D_A(A3PTR) == 0)
	goto L_COTH2;
    if (D_V(A1PTR) < DATSTA)
	goto L_COTH0;
    D_V(DT1CL) = D_V(A1PTR);
    if (!LOCAPT(ZPTR,A3PTR,DT1CL))
	BRANCH(INTR1)
    D(ZPTR) = D(D_A(ZPTR) + 2*DESCR);
    D_A(A1PTR) += D_A(ZPTR);
    D(A1PTR) = D(D_A(A1PTR) + DESCR);
    goto L_SORT9;
    /*_*/
L_COTH0:
    if (D_V(A2PTR) < DATSTA)
	goto L_COTH2;
    D_V(DT1CL) = D_V(A2PTR);
    if (!LOCAPT(ZPTR,A3PTR,DT1CL))
	BRANCH(INTR1)
    D(ZPTR) = D(D_A(ZPTR) + 2*DESCR);
    D_A(A2PTR) += D_A(ZPTR);
    D(A2PTR) = D(D_A(A2PTR) + DESCR);
    goto L_SORT9;
L_COTH2:
    if (D_V(A1PTR) == D_V(A2PTR))
	goto L_COTH1;
    D_A(A1PTR) = D_V(A1PTR);
    D_F(A1PTR) = D_V(A1PTR) = 0;
    D_A(A2PTR) = D_V(A2PTR);
    D_F(A2PTR) = D_V(A2PTR) = 0;
L_COTH1:
    if (D_PTR(A1PTR) < D_PTR(A2PTR))
	goto L_CMPLT;
    if (D_PTR(A1PTR) == D_PTR(A2PTR))
	goto L_CMPEQ;
    goto L_CMPGT;
    /*_*/
L_SORT12:
    D(XCL) = D(D_A(XPTR) + 2*DESCR);
    D_A(WPTR) = D_A(XCL);
    D_A(WPTR) *= DESCR;
    D_F(WPTR) = D_V(WPTR) = 0;
    D_A(WPTR) = D_A(XPTR) + D_A(WPTR);
    D_F(WPTR) = D_F(XPTR);
    D_V(WPTR) = D_V(XPTR);
    D_A(WPTR) += 3*DESCR;
    D(LPTR) = D(ONECL);
    D_A(LPTR) *= DESCR;
    D_F(LPTR) = D_V(LPTR) = 0;
L_SORT13:
    D(ZPTR) = D(D_A(TPTR) + D_A(LPTR));
    D_A(ZPTR) -= D_A(A5PTR);
    D(YPTR) = D(WPTR);
    D_A(A6PTR) = D_A(YCL);
    D_A(A6PTR) *= DESCR;
    D_F(A6PTR) = D_V(A6PTR) = 0;
L_SORT14:
    D(D_A(YPTR)) = D(D_A(ZPTR));
    D_A(ZPTR) += D_A(A4PTR);
    D_A(YPTR) += D_A(ZCL);
    D_A(A6PTR) -= DESCR;
    if (D_A(A6PTR) != 0)
	goto L_SORT14;
    D_A(WPTR) += DESCR;
    D_A(LPTR) += DESCR;
    if (D_PTR(LPTR) <= D_PTR(ZCL))
	goto L_SORT13;
    PUSH(ZEROCL);
    PUSH(ZEROCL);
    PUSH(ZEROCL);
    PUSH(ZEROCL);
    POP(YPTR);
    POP(ZPTR);
    POP(F1PTR);
    POP(F2PTR);
    BRANCH(RTXPTR)
    /*_*/
}
int
READ(retval)
    ret_t retval;
{
    ENTRY(READ)
    SAVSTK();
    switch (IND(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (INTVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(YPTR);
    SAVSTK();
    switch (VARVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(YPTR);
    if (D_A(YPTR) < 0)
	BRANCH(UNTERR)
    if (D_A(YPTR) == 0)
	goto L_READ5;
L_READ6:
    PUSH(YPTR);
    PUSH(ZPTR);
    SAVSTK();
    switch (VARVAL(TPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    X_LOCSP(XSP,TPTR);
    POP(ZPTR);
    POP(YPTR);
    POP(XPTR);
    X_LOCSP(ZSP,ZPTR);
    D(ZPTR) = D(ZEROCL);
    if (!IO_OPENI(YPTR,XSP,ZSP,ZPTR))
	BRANCH(FAIL)
    if (D_A(ZPTR) < 0)
	BRANCH(LENERR)
    if (D_A(ZPTR) > 0)
	goto L_READ2;
    if (!LOCAPT(TPTR,INSATL,YPTR))
	goto L_READ4;
L_READ3:
    if (!LOCAPV(ZPTR,INATL,XPTR))
	goto L_READ1;
    D(D_A(ZPTR) + DESCR) = D(TPTR);
    BRANCH(RETNUL)
    /*_*/
L_READ1:
    SAVSTK();
    PUSH(XPTR);
    PUSH(TPTR);
    PUSH(INATL);
    switch (AUGATL(INATL)) {
    case 1:
	BRANCH(RETNUL)
    }
    /*_*/
L_READ4:
    D(ZPTR) = D(DFLSIZ);
L_READ2:
    SAVSTK();
    PUSH(IOBLSZ);
    BLOCK(TPTR);
    D(D_A(TPTR) + DESCR) = D(YPTR);
    D(D_A(TPTR) + 2*DESCR) = D(ZPTR);
    goto L_READ3;
    /*_*/
L_READ5:
    D_A(YPTR) = (int_t) UNITI;
    goto L_READ6;
    /*_*/
}
int
PRINT(retval)
    ret_t retval;
{
    ENTRY(PRINT)
    SAVSTK();
    switch (IND(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (INTVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(YPTR);
    SAVSTK();
    switch (VARVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(ZPTR);
    SAVSTK();
    switch (VARVAL(TPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(ZPTR);
    POP(YPTR);
    POP(XPTR);
    X_LOCSP(XSP,TPTR);
    X_LOCSP(ZSP,ZPTR);
    if (!IO_OPENO(YPTR,XSP,ZSP))
	BRANCH(FAIL)
    if (D_A(YPTR) < 0)
	BRANCH(UNTERR)
    if (D_A(YPTR) == 0)
	goto L_PRINT5;
L_PRINT6:
    if (D_A(ZPTR) != 0)
	goto L_PRINT2;
    if (!LOCAPT(TPTR,OTSATL,YPTR))
	goto L_PRINT4;
L_PRINT3:
    if (!LOCAPV(ZPTR,OUTATL,XPTR))
	goto L_PRINT1;
    D(D_A(ZPTR) + DESCR) = D(TPTR);
    BRANCH(RETNUL)
    /*_*/
L_PRINT1:
    SAVSTK();
    PUSH(XPTR);
    PUSH(TPTR);
    PUSH(OUTATL);
    switch (AUGATL(OUTATL)) {
    case 1:
	BRANCH(RETNUL)
    }
    /*_*/
L_PRINT4:
    D(ZPTR) = D(DFLFST);
L_PRINT2:
    SAVSTK();
    PUSH(IOBLSZ);
    BLOCK(TPTR);
    D(D_A(TPTR) + DESCR) = D(YPTR);
    D(D_A(TPTR) + 2*DESCR) = D(ZPTR);
    goto L_PRINT3;
    /*_*/
L_PRINT5:
    D_A(YPTR) = (int_t) UNITO;
    goto L_PRINT6;
    /*_*/
}
int
BKSPCE(retval)
    ret_t retval;
{
    ENTRY(BKSPCE)
    D_A(SCL) = 1;
    BRANCH(IOOP)
    /*_*/
}
int
ENDFIL(retval)
    ret_t retval;
{
    ENTRY(ENDFIL)
    D_A(SCL) = 2;
    BRANCH(IOOP)
    /*_*/
}
int
REWIND(retval)
    ret_t retval;
{
    ENTRY(REWIND)
    D_A(SCL) = 3;
    BRANCH(IOOP)
    /*_*/
}
int
SET(retval)
    ret_t retval;
{
    ENTRY(SET)
    D_A(SCL) = 4;
    /* FALL */
    BRANCH(IOOP)
}
static int
IOOP(retval)
    ret_t retval;
{
    ENTRY(IOOP)
    PUSH(SCL);
    SAVSTK();
    switch (INTVAL(XCL)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_A(XCL) <= 0)
	BRANCH(UNTERR)
    POP(SCL);
    switch (D_A(SCL)) {
    case 2:
	goto L_EOP;
    case 3:
	goto L_ROP;
    case 4:
	goto L_SOP;
    }
    io_backspace(D_A(XCL));
    BRANCH(RETNUL)
    /*_*/
L_EOP:
    if (!io_endfile(D_A(XCL)))
	BRANCH(COMP6)
    BRANCH(RETNUL)
    /*_*/
L_ROP:
    io_rewind(D_A(XCL));
    BRANCH(RETNUL)
    /*_*/
L_SOP:
    PUSH(XCL);
    SAVSTK();
    switch (INTVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (INTVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    POP(XCL);
    if (IO_SEEK(XCL,XPTR,YPTR))
	BRANCH(RTXPTR)
    else
	BRANCH(FAIL)
    /*_*/
}
int
DETACH(retval)
    ret_t retval;
{
    ENTRY(DETACH)
    SAVSTK();
    switch (IND(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (!LOCAPV(ZPTR,INATL,XPTR))
	goto L_DTCH1;
    D(D_A(ZPTR) + DESCR) = D(ZEROCL);
    D(D_A(ZPTR) + 2*DESCR) = D(ZEROCL);
L_DTCH1:
    if (!LOCAPV(ZPTR,OUTATL,XPTR))
	BRANCH(RETNUL)
    D(D_A(ZPTR) + DESCR) = D(ZEROCL);
    D(D_A(ZPTR) + 2*DESCR) = D(ZEROCL);
    BRANCH(RETNUL)
    /*_*/
}
static int
PUTIN(retval)
    ret_t retval;
{
    ENTRY(PUTIN)
    POP(IO1PTR);
    POP(IO2PTR);
    D(IO3PTR) = D(D_A(IO1PTR) + DESCR);
    D(IO1PTR) = D(D_A(IO1PTR) + 2*DESCR);
    SAVSTK();
    PUSH(IO1PTR);
    CONVAR(IO4PTR);
    X_LOCSP(IOSP,IO4PTR);
    D_A(RSTAT)++;
    switch (IO_READ(IO3PTR, IOSP)) {
    case IO_EOF:
	BRANCH(FAIL)
    case IO_ERR:
	BRANCH(COMP5)
    }
    if (D_A(TRIMCL) == 0)
	goto L_PUTIN1;
    TRIMSP(IOSP,IOSP);
L_PUTIN1:
    D_A(IO1PTR) = S_L(IOSP);
    D_F(IO1PTR) = D_V(IO1PTR) = 0;
    if (D_A(IO1PTR) > D_A(MLENCL))
	BRANCH(INTR8)
    if (D_V(IO2PTR) == K)
	goto L_PUTIN3;
    SAVSTK();
    PUSH(IO1PTR);
    GNVARS(IO1PTR);
L_PUTIN2:
    D(D_A(IO2PTR) + DESCR) = D(IO1PTR);
    D(retval) = D(IO1PTR);
    RETURN(2)
L_PUTIN3:
    X_LOCSP(XSP,IO1PTR);
    if (SPCINT(IO1PTR,XSP))
	goto L_PUTIN2;
    else
	BRANCH(INTR1)
    /*_*/
}
static int
PUTOUT(retval)
    ret_t retval;
{
    ENTRY(PUTOUT)
    POP(IO1PTR);
    POP(IO2PTR);
    if (D_V(IO2PTR) == S)
	goto L_PUTV;
    if (D_V(IO2PTR) == I)
	goto L_PUTI;
    SAVSTK();
    PUSH(IO2PTR);
    DTREP(IO2PTR);
    _SPEC(IOSP) = _SPEC(D_A(IO2PTR));
    goto L_PUTVU;
    /*_*/
L_PUTV:
    X_LOCSP(IOSP,IO2PTR);
L_PUTVU:
    IO_PRINT(IOKEY, IO1PTR, IOSP);
    if (D_A(IOKEY) == 0)
	BRANCH(COMP6)
    D_A(WSTAT)++;
    BRANCH(RTN1)
    /*_*/
L_PUTI:
    INTSPC(IOSP,IO2PTR);
    goto L_PUTVU;
    /*_*/
}
int
TRACE(retval)
    ret_t retval;
{
    ENTRY(TRACE)
    SAVSTK();
    switch (IND(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVUP(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(YPTR);
    SAVSTK();
    switch (ARGVAL(WPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(WPTR);
    SAVSTK();
    switch (VARVUP(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(WPTR);
    POP(YPTR);
    POP(XPTR);
    if (!DCMP(YPTR, NULVCL))
	goto L_TRAC5;
    D(YPTR) = D(VALTRS);
L_TRAC5:
    if (!LOCAPV(YPTR,TRATL,YPTR))
	goto L_TRAC1;
    D(YPTR) = D(D_A(YPTR) + DESCR);
    BRANCH(TRACEP)
    /*_*/
L_TRAC1:
    if (DCMP(YPTR, FUNTCL))
	goto L_TRACF;
    if (!DCMP(YPTR, EFFCL))
	BRANCH(INTR30)
L_TRACF:
    D(YPTR) = D(TFNCLP);
    SAVSTK();
    switch (TRACEP(NORET)) {
    case 1:
    case 2:
	BRANCH(INTR10)
    }
    D(YPTR) = D(TFNRLP);
    BRANCH(TRACEP)
    /*_*/
}
static int
TRACEP(retval)
    ret_t retval;
{
    ENTRY(TRACEP)
    D(TPTR) = D(D_A(YPTR) + DESCR);
    if (DCMP(ZPTR, NULVCL))
	goto L_TRAC2;
    SAVSTK();
    PUSH(ZPTR);
    FINDEX(TPTR);
L_TRAC2:
    D_A(XSIZ) = (int_t) (5*DESCR);
    D_V(XSIZ) = C;
    SAVSTK();
    PUSH(XSIZ);
    BLOCK(XCL);
    MOVBLK(D_A(XCL),D_A(TRCBLK),D_A(XSIZ));
    D_V(TPTR) = 2;
    D(D_A(XCL) + 1*DESCR) = D(TPTR);
    D(D_A(XCL) + 3*DESCR) = D(XPTR);
    D(D_A(XCL) + 5*DESCR) = D(WPTR);
    D(TPTR) = D(D_A(YPTR));
    if (D_A(TPTR) == 0)
	goto L_TRAC4;
    if (!LOCAPT(TPTR,TPTR,XPTR))
	goto L_TRAC3;
    D(D_A(TPTR) + 2*DESCR) = D(XCL);
    BRANCH(RETNUL)
    /*_*/
L_TRAC3:
    SAVSTK();
    PUSH(XCL);
    PUSH(XPTR);
    PUSH(TPTR);
    AUGATL(TPTR);
L_TRAC6:
    D(D_A(YPTR)) = D(TPTR);
    BRANCH(RETNUL)
    /*_*/
L_TRAC4:
    SAVSTK();
    PUSH(TWOCL);
    BLOCK(TPTR);
    D(D_A(TPTR) + DESCR) = D(XPTR);
    D(D_A(TPTR) + 2*DESCR) = D(XCL);
    goto L_TRAC6;
    /*_*/
}
int
STOPTR(retval)
    ret_t retval;
{
    ENTRY(STOPTR)
    SAVSTK();
    switch (IND(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVUP(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    if (!DCMP(YPTR, NULVCL))
	goto L_STOPT2;
    D(YPTR) = D(VALTRS);
L_STOPT2:
    if (!LOCAPV(YPTR,TRATL,YPTR))
	goto L_STOPT1;
    D(YPTR) = D(D_A(YPTR) + DESCR);
    BRANCH(STOPTP)
    /*_*/
L_STOPT1:
    if (DCMP(YPTR, FUNTCL))
	goto L_STOPTF;
    if (!DCMP(YPTR, EFFCL))
	BRANCH(INTR30)
L_STOPTF:
    D(YPTR) = D(TFNCLP);
    SAVSTK();
    switch (STOPTP(NORET)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	BRANCH(INTR10)
    }
    D(YPTR) = D(TFNRLP);
    BRANCH(STOPTP)
    /*_*/
}
static int
STOPTP(retval)
    ret_t retval;
{
    ENTRY(STOPTP)
    D(YPTR) = D(D_A(YPTR));
    if (!LOCAPT(YPTR,YPTR,XPTR))
	BRANCH(FAIL)
    D(D_A(YPTR) + DESCR) = D(ZEROCL);
    D(D_A(YPTR) + 2*DESCR) = D(ZEROCL);
    BRANCH(RETNUL)
    /*_*/
}
int
FENTR(retval)
    ret_t retval;
{
    ENTRY(FENTR)
    SAVSTK();
    switch (VARVAL(WPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    /* FALL */
    BRANCH(FENTR3)
}
static int
FENTR3(retval)
    ret_t retval;
{
    ENTRY(FENTR3)
    S_L(PROTSP) = 0;
    X_LOCSP(XSP,FILENM);
    APDSP(PROTSP,XSP);
    APDSP(PROTSP,COLSP);
    INTSPC(XSP,LNNOCL);
    APDSP(PROTSP,XSP);
    APDSP(PROTSP,TRSTSP);
    INTSPC(XSP,STNOCL);
    APDSP(PROTSP,XSP);
    APDSP(PROTSP,COLSP);
    APDSP(PROTSP,SPCSP);
    APDSP(PROTSP,TRLVSP);
    INTSPC(XSP,LVLCL);
    APDSP(PROTSP,XSP);
    APDSP(PROTSP,TRCLSP);
    X_LOCSP(XSP,WPTR);
    D_A(TCL) = S_L(XSP);
    D_F(TCL) = D_V(TCL) = 0;
    if (D_A(TCL) >= BUFLEN)
	BRANCH(FXOVR)
    APDSP(PROTSP,XSP);
    APDSP(PROTSP,LPRNSP);
    D_A(WCL) = 0;
L_FNTRLP:
    D_A(WCL)++;
    SAVSTK();
    PUSH(WCL);
    PUSH(WPTR);
    switch (ARGINT(ZPTR)) {
    case 1:
	goto L_FENTR4;
    case 2:
	BRANCH(INTR10)
    }
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    if (D_V(ZPTR) == S)
	goto L_DEFTV;
    if (D_V(ZPTR) == I)
	goto L_DEFTI;
    SAVSTK();
    PUSH(ZPTR);
    DTREP(A2PTR);
    _SPEC(XSP) = _SPEC(D_A(A2PTR));
    D_A(SCL) = S_L(XSP);
    D_F(SCL) = D_V(SCL) = 0;
    D_A(TCL) += D_A(SCL);
    if (D_A(TCL) >= BUFLEN)
	BRANCH(FXOVR)
L_DEFTIA:
    APDSP(PROTSP,XSP);
    goto L_DEFDTT;
    /*_*/
L_DEFTI:
    INTSPC(XSP,ZPTR);
    goto L_DEFTIA;
    /*_*/
L_DEFTV:
    X_LOCSP(XSP,ZPTR);
    D_A(SCL) = S_L(XSP);
    D_F(SCL) = D_V(SCL) = 0;
    D_A(TCL) += D_A(SCL);
    if (D_A(TCL) >= BUFLEN)
	BRANCH(FXOVR)
    APDSP(PROTSP,QTSP);
    APDSP(PROTSP,XSP);
    APDSP(PROTSP,QTSP);
L_DEFDTT:
    APDSP(PROTSP,CMASP);
    goto L_FNTRLP;
    /*_*/
L_FENTR4:
    if (D_A(WCL) == 1)
	goto L_FENTR5;
    S_L(PROTSP)--;
L_FENTR5:
    APDSP(PROTSP,RPRNSP);
    D_RV(ZPTR) = mstime();
    D_F(ZPTR) = D_V(ZPTR) = 0;
    D_RV(ZPTR) -= D_RV(ETMCL);
    REALST(XSP,ZPTR);
    APDSP(PROTSP,ETIMSP);
    APDSP(PROTSP,XSP);
    IO_PRINT(IOKEY, OUTBLK, PROTSP);
    BRANCH(RTNUL3)
    /*_*/
}
static int
FENTR2(retval)
    ret_t retval;
{
    ENTRY(FENTR2)
    POP(WPTR);
    BRANCH(FENTR3)
    /*_*/
}
static int
FXOVR(retval)
    ret_t retval;
{
    ENTRY(FXOVR)
    io_printf(D_A(OUTPUT),PRTOVF);
    BRANCH(RTNUL3)
    /*_*/
}
int
KEYTR(retval)
    ret_t retval;
{
    ENTRY(KEYTR)
    D_A(FNVLCL) = 1;
    SAVSTK();
    switch (VARVAL(WPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    X_LOCSP(XSP,WPTR);
    SAVSTK();
    PUSH(WPTR);
    switch (KEYT(YCL)) {
    case 1:
	BRANCH(INTR10)
    }
    /* FALL */
    BRANCH(KEYTR3)
}
static int
KEYTR3(retval)
    ret_t retval;
{
    ENTRY(KEYTR3)
    S_L(PROTSP) = 0;
    X_LOCSP(TSP,FILENM);
    APDSP(PROTSP,TSP);
    APDSP(PROTSP,COLSP);
    INTSPC(TSP,LNNOCL);
    APDSP(PROTSP,TSP);
    APDSP(PROTSP,TRSTSP);
    INTSPC(TSP,STNOCL);
    APDSP(PROTSP,TSP);
    APDSP(PROTSP,COLSP);
    APDSP(PROTSP,SPCSP);
    if (D_A(FNVLCL) == 0)
	goto L_KEYTR4;
    APDSP(PROTSP,AMPSP);
L_KEYTR4:
    APDSP(PROTSP,XSP);
    APDSP(PROTSP,BLSP);
    if (D_A(FNVLCL) == 0)
	goto L_KEYTR5;
    INTSPC(YSP,YCL);
    APDSP(PROTSP,EQLSP);
L_KEYTR5:
    APDSP(PROTSP,YSP);
    D_RV(YPTR) = mstime();
    D_F(YPTR) = D_V(YPTR) = 0;
    D_RV(YPTR) -= D_RV(ETMCL);
    REALST(XSP,YPTR);
    APDSP(PROTSP,ETIMSP);
    APDSP(PROTSP,XSP);
    IO_PRINT(IOKEY, OUTBLK, PROTSP);
    BRANCH(RTN2)
    /*_*/
}
int
LABTR(retval)
    ret_t retval;
{
    ENTRY(LABTR)
    D_A(FNVLCL) = 0;
    SAVSTK();
    switch (VARVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    X_LOCSP(YSP,YPTR);
    _SPEC(XSP) = _SPEC(XFERSP);
    BRANCH(KEYTR3)
    /*_*/
}
static int
TRPHND(retval)
    ret_t retval;
{
    ENTRY(TRPHND)
    POP(ATPTR);
    D_A(TRAPCL)--;
    PUSH(FILENM);
    PUSH(LNNOCL);
    PUSH(LSTNCL);
    PUSH(STNOCL);
    PUSH(FRTNCL);
    PUSH(OCBSCL);
    PUSH(OCICL);
    PUSH(TRAPCL);
    PUSH(TRACL);
    PUSH(LSFLNM);
    PUSH(LSLNCL);
    D(OCBSCL) = D(D_A(ATPTR) + 2*DESCR);
    D_A(OCICL) = (int_t) DESCR;
    D(XPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    D_A(TRAPCL) = 0;
    D_A(TRACL) = 0;
    SAVSTK();
    PUSH(XPTR);
    INVOKE(NORET);
    POP(LSLNCL);
    POP(LSFLNM);
    POP(TRACL);
    POP(TRAPCL);
    POP(OCICL);
    POP(OCBSCL);
    POP(FRTNCL);
    POP(STNOCL);
    POP(LSTNCL);
    POP(LNNOCL);
    POP(FILENM);
    BRANCH(RTN1)
    /*_*/
}
int
VALTR(retval)
    ret_t retval;
{
    ENTRY(VALTR)
    D_A(FNVLCL) = 1;
    /* FALL */
    BRANCH(VALTR2)
}
static int
VALTR2(retval)
    ret_t retval;
{
    ENTRY(VALTR2)
    SAVSTK();
    switch (IND(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    /* FALL */
    BRANCH(VALTR4)
}
static int
VALTR4(retval)
    ret_t retval;
{
    ENTRY(VALTR4)
    S_L(TRACSP) = 0;
    X_LOCSP(XSP,FILENM);
    APDSP(TRACSP,XSP);
    APDSP(TRACSP,COLSP);
    INTSPC(XSP,LNNOCL);
    APDSP(TRACSP,XSP);
    APDSP(TRACSP,TRSTSP);
    INTSPC(XSP,STNOCL);
    APDSP(TRACSP,XSP);
    APDSP(TRACSP,COLSP);
    APDSP(TRACSP,SPCSP);
    if (D_A(FNVLCL) == 0)
	BRANCH(FNEXT1)
    if (D_V(XPTR) != S)
	BRANCH(DEFDT)
    /* FALL */
    BRANCH(VALTR3)
}
static int
VALTR3(retval)
    ret_t retval;
{
    ENTRY(VALTR3)
    X_LOCSP(XSP,XPTR);
    D_A(TCL) = S_L(XSP);
    D_F(TCL) = D_V(TCL) = 0;
    if (D_A(TCL) >= BUFLEN)
	BRANCH(VXOVR)
    /* FALL */
    BRANCH(VALTR1)
}
static int
VALTR1(retval)
    ret_t retval;
{
    ENTRY(VALTR1)
    APDSP(TRACSP,XSP);
    APDSP(TRACSP,BLEQSP);
    D(YPTR) = D(D_A(XPTR) + DESCR);
    if (D_V(YPTR) == S)
	BRANCH(TRV)
    if (D_V(YPTR) == I)
	BRANCH(TRI)
    SAVSTK();
    PUSH(YPTR);
    DTREP(XPTR);
    _SPEC(XSP) = _SPEC(D_A(XPTR));
    /* FALL */
    BRANCH(TRI2)
}
static int
TRI2(retval)
    ret_t retval;
{
    ENTRY(TRI2)
    APDSP(TRACSP,XSP);
    BRANCH(TRPRT)
    /*_*/
}
static int
TRV(retval)
    ret_t retval;
{
    ENTRY(TRV)
    X_LOCSP(XSP,YPTR);
    D_A(SCL) = S_L(XSP);
    D_F(SCL) = D_V(SCL) = 0;
    D_A(TCL) += D_A(SCL);
    if (D_A(TCL) >= BUFLEN)
	BRANCH(VXOVR)
    APDSP(TRACSP,QTSP);
    APDSP(TRACSP,XSP);
    APDSP(TRACSP,QTSP);
    /* FALL */
    BRANCH(TRPRT)
}
static int
TRPRT(retval)
    ret_t retval;
{
    ENTRY(TRPRT)
    D_RV(YPTR) = mstime();
    D_F(YPTR) = D_V(YPTR) = 0;
    D_RV(YPTR) -= D_RV(ETMCL);
    REALST(XSP,YPTR);
    APDSP(TRACSP,ETIMSP);
    APDSP(TRACSP,XSP);
    IO_PRINT(IOKEY, OUTBLK, TRACSP);
    BRANCH(RTNUL3)
    /*_*/
}
static int
TRI(retval)
    ret_t retval;
{
    ENTRY(TRI)
    INTSPC(XSP,YPTR);
    BRANCH(TRI2)
    /*_*/
}
static int
DEFDT(retval)
    ret_t retval;
{
    ENTRY(DEFDT)
    X_LOCSP(XSP,ZPTR);
    BRANCH(VALTR1)
    /*_*/
}
int
FNEXTR(retval)
    ret_t retval;
{
    ENTRY(FNEXTR)
    D_A(FNVLCL) = 0;
    BRANCH(VALTR2)
    /*_*/
}
static int
FNEXT1(retval)
    ret_t retval;
{
    ENTRY(FNEXT1)
    APDSP(TRACSP,TRLVSP);
    D(XCL) = D(LVLCL);
    D_A(XCL)--;
    INTSPC(XSP,XCL);
    APDSP(TRACSP,XSP);
    APDSP(TRACSP,BLSP);
    X_LOCSP(XSP,RETPCL);
    APDSP(TRACSP,XSP);
    APDSP(TRACSP,OFSP);
    if (!DCMP(RETPCL, FRETCL))
	BRANCH(VALTR3)
    X_LOCSP(XSP,XPTR);
    D_A(TCL) = S_L(XSP);
    D_F(TCL) = D_V(TCL) = 0;
    if (D_A(TCL) >= BUFLEN)
	BRANCH(VXOVR)
    APDSP(TRACSP,XSP);
    BRANCH(TRPRT)
    /*_*/
}
static int
FNEXT2(retval)
    ret_t retval;
{
    ENTRY(FNEXT2)
    D_A(FNVLCL) = 0;
    POP(XPTR);
    BRANCH(VALTR4)
    /*_*/
}
static int
VXOVR(retval)
    ret_t retval;
{
    ENTRY(VXOVR)
    io_printf(D_A(OUTPUT),PRTOVF);
    BRANCH(RTNUL3)
    /*_*/
}
int
SETXIT(retval)
    ret_t retval;
{
    ENTRY(SETXIT)
    SAVSTK();
    switch (VARVUP(XPTR)) {
    case 1:
	BRANCH(INTR30)
    }
    if (D_A(XPTR) == 0)
	goto L_SETXI2;
    D(YPTR) = D(D_A(XPTR) + ATTRIB);
    if (D_A(YPTR) == 0)
	BRANCH(INTR30)
L_SETXI2:
    D(YPTR) = D(XITPTR);
    D(XITPTR) = D(XPTR);
    BRANCH(RTYPTR)
    /*_*/
}
static int
XITHND(retval)
    ret_t retval;
{
    ENTRY(XITHND)
    if (D_A(XITPTR) == 0)
	BRANCH(FAIL)
    D(XFILEN) = D(FILENM);
    D(XLNNOC) = D(LNNOCL);
    D(XSTNOC) = D(STNOCL);
    D(XLSFLN) = D(LSFLNM);
    D(XLSLNC) = D(LSLNCL);
    D(XLNNOC) = D(LNNOCL);
    D(XERRTY) = D(ERRTYP);
    D(XOCBSC) = D(OCBSCL);
    D(XFRTNC) = D(FRTNCL);
    D(XOCICL) = D(OCICL);
    D(OCBSCL) = D(D_A(XITPTR) + ATTRIB);
    if (D_A(OCBSCL) == 0)
	BRANCH(INTR4)
    D_A(FRTNCL) = 0;
    D_A(XITPTR) = 0;
    D(LSTNCL) = D(STNOCL);
    D_A(LSLNCL) = D_A(LNNOCL);
    D_A(LSFLNM) = D_A(FILENM);
    BRANCH(RETNUL)
    /*_*/
}
int
ASGN(retval)
    ret_t retval;
{
    ENTRY(ASGN)
    D_A(OCICL) += DESCR;
    D(XPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(XPTR) & FNC))
	goto L_ASGNC;
L_ASGNV:
    if (D_V(XPTR) == K)
	goto L_ASGNIC;
    D_A(OCICL) += DESCR;
    D(YPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(YPTR) & FNC))
	goto L_ASGNCV;
L_ASGNVN:
    if (D_A(INSW) == 0)
	goto L_ASGNV1;
    if (!LOCAPV(ZPTR,INATL,YPTR))
	goto L_ASGNV1;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    SAVSTK();
    PUSH(YPTR);
    PUSH(ZPTR);
    switch (PUTIN(YPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_ASGNVV;
    }
    /*_*/
L_ASGNV1:
    D(YPTR) = D(D_A(YPTR) + DESCR);
L_ASGNVV:
    D(D_A(XPTR) + DESCR) = D(YPTR);
    if (D_A(OUTSW) == 0)
	goto L_ASGN1;
    if (!LOCAPV(ZPTR,OUTATL,XPTR))
	goto L_ASGN1;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    SAVSTK();
    PUSH(YPTR);
    PUSH(ZPTR);
    PUTOUT(NORET);
L_ASGN1:
    if (D_A(TRAPCL) <= 0)
	BRANCH(RTYPTR)
    if (!LOCAPT(ATPTR,TVALL,XPTR))
	BRANCH(RTYPTR)
    PUSH(YPTR);
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
    POP(YPTR);
    BRANCH(RTYPTR)
    /*_*/
L_ASGNC:
    SAVSTK();
    PUSH(XPTR);
    switch (INVOKE(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_ASGNV;
    case 3:
	BRANCH(NEMO)
    }
    /*_*/
L_ASGNCV:
    PUSH(XPTR);
    SAVSTK();
    PUSH(YPTR);
    switch (INVOKE(YPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_ASGNVP;
    }
L_ASGNCJ:
    POP(XPTR);
    goto L_ASGNVV;
    /*_*/
L_ASGNVP:
    POP(XPTR);
    goto L_ASGNVN;
    /*_*/
L_ASGNIC:
    PUSH(XPTR);
    SAVSTK();
    switch (INTVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_ASGNCJ;
    }
    /*_*/
}
int
CONCAT(retval)
    ret_t retval;
{
    ENTRY(CONCAT)
    SAVSTK();
    switch (XYARGS(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    if (DCMP(XPTR, NULVCL))
	BRANCH(RTYPTR)
    if (DCMP(YPTR, NULVCL))
	BRANCH(RTXPTR)
    if (D_V(XPTR) == S)
	goto L_CON5;
    if (D_V(XPTR) == P)
	goto L_CON5;
    if (D_V(XPTR) == I)
	goto L_CON4I;
    if (D_V(XPTR) == R)
	goto L_CON4R;
    if (D_V(XPTR) == BL)
	goto L_CON5;
    if (D_V(XPTR) != E)
	BRANCH(INTR1)
    SAVSTK();
    PUSH(STARSZ);
    BLOCK(TPTR);
    MOVBLK(D_A(TPTR),D_A(STRPAT),D_A(STARSZ));
    D(D_A(TPTR) + 4*DESCR) = D(XPTR);
    D(XPTR) = D(TPTR);
    goto L_CON5;
    /*_*/
L_CON4R:
    REALST(REALSP,XPTR);
    _SPEC(XSP) = _SPEC(REALSP);
    SAVSTK();
    PUSH(XSPPTR);
    switch (GENVAR(XPTR)) {
    case 1:
	goto L_CON5;
    }
    /*_*/
L_CON4I:
    INTSPC(ZSP,XPTR);
    SAVSTK();
    PUSH(ZSPPTR);
    GENVAR(XPTR);
L_CON5:
    if (D_V(YPTR) == S)
	goto L_CON7;
    if (D_V(YPTR) == P)
	goto L_CON7;
    if (D_V(YPTR) == I)
	goto L_CON5I;
    if (D_V(YPTR) == R)
	goto L_CON5R;
    if (D_V(YPTR) == BL)
	goto L_CON7;
    if (D_V(YPTR) != E)
	BRANCH(INTR1)
    SAVSTK();
    PUSH(STARSZ);
    BLOCK(TPTR);
    MOVBLK(D_A(TPTR),D_A(STRPAT),D_A(STARSZ));
    D(D_A(TPTR) + 4*DESCR) = D(YPTR);
    D(YPTR) = D(TPTR);
    goto L_CON7;
    /*_*/
L_CON5R:
    REALST(REALSP,YPTR);
    _SPEC(YSP) = _SPEC(REALSP);
    SAVSTK();
    PUSH(YSPPTR);
    switch (GENVAR(YPTR)) {
    case 1:
	goto L_CON7;
    }
    /*_*/
L_CON5I:
    INTSPC(ZSP,YPTR);
    SAVSTK();
    PUSH(ZSPPTR);
    GENVAR(YPTR);
L_CON7:
    D_A(DTCL) = D_V(XPTR);
    D_F(DTCL) = D_V(DTCL) = 0;
    D_V(DTCL) = D_V(YPTR);
    if (DCMP(DTCL, VVDTP))
	BRANCH(CONVV)
    if (DCMP(DTCL, VPDTP))
	goto L_CONVP;
    if (DCMP(DTCL, PVDTP))
	goto L_CONPV;
    if (DCMP(DTCL, PPDTP))
	goto L_CONPP;
    else
	BRANCH(CCATB)
    /*_*/
L_CONVP:
    X_LOCSP(TSP,XPTR);
    D_A(TMVAL) = S_L(TSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    SAVSTK();
    PUSH(LNODSZ);
    BLOCK(TPTR);
    MAKNOD(XPTR,TPTR,TMVAL,ZEROCL,CHRCL,XPTR);
L_CONPP:
    D_A(XSIZ) = D_V(D_A(XPTR));
    D_F(XSIZ) = D_V(XSIZ) = 0;
    D_A(YSIZ) = D_V(D_A(YPTR));
    D_F(YSIZ) = D_V(YSIZ) = 0;
    D(TSIZ) = D(XSIZ);
    D_A(TSIZ) += D_A(YSIZ);
    D_V(TSIZ) = P;
    SAVSTK();
    PUSH(TSIZ);
    BLOCK(TPTR);
    D(ZPTR) = D(TPTR);
    LVALUE(TVAL,YPTR);
    CPYPAT(TPTR,XPTR,TVAL,ZEROCL,XSIZ,XSIZ);
    CPYPAT(TPTR,YPTR,ZEROCL,XSIZ,ZEROCL,YSIZ);
    BRANCH(RTZPTR)
    /*_*/
L_CONPV:
    X_LOCSP(TSP,YPTR);
    D_A(TMVAL) = S_L(TSP);
    D_F(TMVAL) = D_V(TMVAL) = 0;
    SAVSTK();
    PUSH(LNODSZ);
    BLOCK(TPTR);
    MAKNOD(YPTR,TPTR,TMVAL,ZEROCL,CHRCL,YPTR);
    goto L_CONPP;
    /*_*/
}
static int
CONVV(retval)
    ret_t retval;
{
    ENTRY(CONVV)
    X_LOCSP(XSP,XPTR);
    X_LOCSP(YSP,YPTR);
    D_A(XCL) = S_L(XSP);
    D_F(XCL) = D_V(XCL) = 0;
    D_A(YCL) = S_L(YSP);
    D_F(YCL) = D_V(YCL) = 0;
    D_A(XCL) += D_A(YCL);
    if (D_A(XCL) > D_A(MLENCL))
	BRANCH(INTR8)
    SAVSTK();
    PUSH(XCL);
    CONVAR(ZPTR);
    X_LOCSP(TSP,ZPTR);
    S_L(TSP) = 0;
    APDSP(TSP,XSP);
    APDSP(TSP,YSP);
    BRANCH(GENVSZ)
    /*_*/
}
int
IND(retval)
    ret_t retval;
{
    ENTRY(IND)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(XPTR) == S)
	goto L_INDV;
    if (D_V(XPTR) == N)
	BRANCH(RTXNAM)
    if (D_V(XPTR) == I)
	BRANCH(GENVIX)
    if (D_V(XPTR) == K)
	BRANCH(RTXNAM)
    else
	BRANCH(INTR1)
    /*_*/
L_INDV:
    if (D_A(XPTR) == 0)
	BRANCH(NONAME)
    if (D_A(CASECL) == 0)
	BRANCH(RTXNAM)
    else
	BRANCH(VPXPTR)
    /*_*/
}
int
KEYWRD(retval)
    ret_t retval;
{
    ENTRY(KEYWRD)
    D_A(OCICL) += DESCR;
    D(XPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if ((D_F(XPTR) & FNC))
	BRANCH(KEYC)
    /* FALL */
    BRANCH(KEYN)
}
static int
KEYN(retval)
    ret_t retval;
{
    ENTRY(KEYN)
    if (!LOCAPV(XPTR,KNATL,XPTR))
	goto L_KEYV;
    D(YPTR) = D(D_A(XPTR) + DESCR);
    if (D_V(YPTR) == I)
	goto L_KEYN1;
    D_V(XPTR) = N;
    BRANCH(RTXNAM)
L_KEYN1:
    D_V(XPTR) = K;
    BRANCH(RTXNAM)
    /*_*/
L_KEYV:
    if (!LOCAPV(ATPTR,KVATL,XPTR))
	BRANCH(UNKNKW)
    D(ZPTR) = D(D_A(ATPTR) + DESCR);
    BRANCH(RTZPTR)
    /*_*/
}
static int
KEYC(retval)
    ret_t retval;
{
    ENTRY(KEYC)
    SAVSTK();
    PUSH(XPTR);
    switch (INVOKE(XPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	BRANCH(KEYN)
    case 3:
	BRANCH(NEMO)
    }
    /*_*/
}
static int
KEYT(retval)
    ret_t retval;
{
    ENTRY(KEYT)
    POP(XPTR);
    BRANCH(KEYN)
    /*_*/
}
int
LIT(retval)
    ret_t retval;
{
    ENTRY(LIT)
    D_A(OCICL) += DESCR;
    D(ZPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    BRANCH(RTZPTR)
    /*_*/
}
int
NAME(retval)
    ret_t retval;
{
    ENTRY(NAME)
    D_A(OCICL) += DESCR;
    D(ZPTR) = D(D_A(OCBSCL) + D_A(OCICL));
    if (!(D_F(ZPTR) & FNC))
	BRANCH(RTZPTR)
    SAVSTK();
    PUSH(ZPTR);
    switch (INVOKE(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	BRANCH(RTZPTR)
    case 3:
	BRANCH(NEMO)
    }
    /*_*/
}
static int
NMD(retval)
    ret_t retval;
{
    ENTRY(NMD)
    D(TCL) = D(NHEDCL);
L_NMD1:
    if (D_A(TCL) == D_A(NAMICL))
	BRANCH(RTN2)
    if (D_A(TCL) > D_A(NAMICL))
	BRANCH(INTR13)
    D(TPTR) = D(NBSPTR);
    D_A(TPTR) += D_A(TCL);
    _SPEC(TSP) = _SPEC(D_A(TPTR) + DESCR);
    D(TVAL) = D(D_A(TPTR) + DESCR+SPEC);
    D_A(XCL) = S_L(TSP);
    D_F(XCL) = D_V(XCL) = 0;
    if (D_A(XCL) > D_A(MLENCL))
	BRANCH(INTR8)
    if (D_V(TVAL) == E)
	goto L_NAMEXN;
L_NMD5:
    if (D_V(TVAL) == K)
	goto L_NMDIC;
    SAVSTK();
    PUSH(TSPPTR);
    GENVAR(VVAL);
L_NMD4:
    D(D_A(TVAL) + DESCR) = D(VVAL);
    if (D_A(OUTSW) == 0)
	goto L_NMD3;
    if (!LOCAPV(ZPTR,OUTATL,TVAL))
	goto L_NMD3;
    D(ZPTR) = D(D_A(ZPTR) + DESCR);
    SAVSTK();
    PUSH(VVAL);
    PUSH(ZPTR);
    PUTOUT(NORET);
L_NMD3:
    if (D_A(TRAPCL) <= 0)
	goto L_NMD2;
    if (!LOCAPT(ATPTR,TVALL,TVAL))
	goto L_NMD2;
    PUSH(TCL);
    PUSH(NAMICL);
    PUSH(NHEDCL);
    D(NHEDCL) = D(NAMICL);
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
    POP(NHEDCL);
    POP(NAMICL);
    POP(TCL);
L_NMD2:
    D_A(TCL) += DESCR+SPEC;
    goto L_NMD1;
    /*_*/
L_NMDIC:
    if (SPCINT(VVAL,TSP))
	goto L_NMD4;
    else
	BRANCH(INTR1)
    /*_*/
L_NAMEXN:
    SAVSTK();
    PUSH(TVAL);
    switch (EXPEVL(TVAL)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	goto L_NMD5;
    case 3:
	BRANCH(NEMO)
    }
    /*_*/
}
int
STR(retval)
    ret_t retval;
{
    ENTRY(STR)
    D(ZPTR) = D(OCBSCL);
    D_A(ZPTR) += D_A(OCICL);
    SAVSTK();
    PUSH(ONECL);
    CODSKP(NORET);
    D_V(ZPTR) = E;
    BRANCH(RTZPTR)
    /*_*/
}
int
DIFFER(retval)
    ret_t retval;
{
    ENTRY(DIFFER)
    SAVSTK();
    switch (XYARGS(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    if (DCMP(XPTR, YPTR))
	BRANCH(FAIL)
    else
	BRANCH(RETNUL)
    /*_*/
}
int
FUNCTN(retval)
    ret_t retval;
{
    ENTRY(FUNCTN)
    SAVSTK();
    switch (VARVUP(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (!LOCAPV(XPTR,FNCPL,XPTR))
	BRANCH(FAIL)
    D(XPTR) = D(D_A(XPTR) + DESCR);
    D(XPTR) = D(D_A(XPTR));
    if (D_A(XPTR) == D_A(UNDFCL))
	BRANCH(FAIL)
    else
	BRANCH(RETNUL)
    /*_*/
}
int
IDENT(retval)
    ret_t retval;
{
    ENTRY(IDENT)
    SAVSTK();
    switch (XYARGS(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    if (DCMP(XPTR, YPTR))
	BRANCH(RETNUL)
    else
	BRANCH(FAIL)
    /*_*/
}
int
LABEL(retval)
    ret_t retval;
{
    ENTRY(LABEL)
    SAVSTK();
    switch (VARVUP(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_A(XPTR) == 0)
	BRANCH(FAIL)
    D(XPTR) = D(D_A(XPTR) + ATTRIB);
    if (D_A(XPTR) == 0)
	BRANCH(FAIL)
    else
	BRANCH(RETNUL)
    /*_*/
}
int
LEQ(retval)
    ret_t retval;
{
    ENTRY(LEQ)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    if (DCMP(XPTR, YPTR))
	BRANCH(RETNUL)
    else
	BRANCH(FAIL)
    /*_*/
}
int
LGE(retval)
    ret_t retval;
{
    ENTRY(LGE)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    if (DCMP(XPTR, YPTR))
	BRANCH(RETNUL)
    if (D_A(XPTR) == 0)
	BRANCH(FAIL)
    if (D_A(YPTR) == 0)
	BRANCH(RETNUL)
    X_LOCSP(XSP,XPTR);
    X_LOCSP(YSP,YPTR);
    if (LEXCMP(XSP,YSP) < 0)
	BRANCH(FAIL)
    else
	BRANCH(RETNUL)
    /*_*/
}
int
LGT(retval)
    ret_t retval;
{
    ENTRY(LGT)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    if (DCMP(XPTR, YPTR))
	BRANCH(FAIL)
    if (D_A(XPTR) == 0)
	BRANCH(FAIL)
    if (D_A(YPTR) == 0)
	BRANCH(RETNUL)
    X_LOCSP(XSP,XPTR);
    X_LOCSP(YSP,YPTR);
    if (LEXCMP(XSP,YSP) <= 0)
	BRANCH(FAIL)
    else
	BRANCH(RETNUL)
    /*_*/
}
int
LLE(retval)
    ret_t retval;
{
    ENTRY(LLE)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    if (DCMP(XPTR, YPTR))
	BRANCH(RETNUL)
    if (D_A(XPTR) == 0)
	BRANCH(RETNUL)
    if (D_A(YPTR) == 0)
	BRANCH(FAIL)
    X_LOCSP(XSP,XPTR);
    X_LOCSP(YSP,YPTR);
    if (LEXCMP(XSP,YSP) <= 0)
	BRANCH(RETNUL)
    else
	BRANCH(FAIL)
    /*_*/
}
int
LLT(retval)
    ret_t retval;
{
    ENTRY(LLT)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    if (DCMP(XPTR, YPTR))
	BRANCH(FAIL)
    if (D_A(XPTR) == 0)
	BRANCH(RETNUL)
    if (D_A(YPTR) == 0)
	BRANCH(FAIL)
    X_LOCSP(XSP,XPTR);
    X_LOCSP(YSP,YPTR);
    if (LEXCMP(XSP,YSP) < 0)
	BRANCH(RETNUL)
    else
	BRANCH(FAIL)
    /*_*/
}
int
LNE(retval)
    ret_t retval;
{
    ENTRY(LNE)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    if (DCMP(XPTR, YPTR))
	BRANCH(FAIL)
    else
	BRANCH(RETNUL)
    /*_*/
}
int
NEG(retval)
    ret_t retval;
{
    ENTRY(NEG)
    PUSH(OCBSCL);
    PUSH(OCICL);
    SAVSTK();
    switch (ARGVAL(NORET)) {
    case 2:
	BRANCH(FAIL)
    }
    POP(OCICL);
    POP(OCBSCL);
    SAVSTK();
    PUSH(ONECL);
    switch (CODSKP(NORET)) {
    case 1:
	BRANCH(RETNUL)
    }
    /*_*/
}
int
QUES(retval)
    ret_t retval;
{
    ENTRY(QUES)
    SAVSTK();
    switch (ARGVAL(NORET)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	BRANCH(RETNUL)
    }
    /*_*/
}
int
CHAR(retval)
    ret_t retval;
{
    ENTRY(CHAR)
    SAVSTK();
    switch (INTVAL(XCL)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_A(XCL) < 0)
	BRANCH(LENERR)
    if (D_A(XCL) >= 256)
	BRANCH(INTR30)
    SAVSTK();
    PUSH(ONECL);
    CONVAR(XPTR);
    X_LOCSP(XSP,XPTR);
    {
		*S_SP(XSP) = D_A(XCL);	   /* Store argument as char */
    }
    SAVSTK();
    PUSH(ONECL);
    GNVARS(XPTR);
    BRANCH(RTXPTR)
    /*_*/
}
int
LPAD(retval)
    ret_t retval;
{
    ENTRY(LPAD)
    D_A(SCL) = 0;
    BRANCH(RPAD0)
    /*_*/
}
int
RPAD(retval)
    ret_t retval;
{
    ENTRY(RPAD)
    D_A(SCL) = 1;
    /* FALL */
    BRANCH(RPAD0)
}
static int
RPAD0(retval)
    ret_t retval;
{
    ENTRY(RPAD0)
    PUSH(SCL);
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (INTVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(ZPTR);
    SAVSTK();
    switch (VARVAL(WPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(ZPTR);
    POP(XPTR);
    POP(SCL);
    X_LOCSP(VSP,WPTR);
    X_LOCSP(XSP,XPTR);
    if (D_A(ZPTR) > D_A(MLENCL))
	BRANCH(INTR8)
    if (D_A(ZEROCL) > D_A(ZPTR))
	BRANCH(LENERR)
    D_A(YPTR) = S_L(XSP);
    D_F(YPTR) = D_V(YPTR) = 0;
    if (D_A(YPTR) >= D_A(ZPTR))
	BRANCH(RTXPTR)
    D_A(XCL) = D_A(ZPTR);
    SAVSTK();
    PUSH(XCL);
    CONVAR(ZPTR);
    X_LOCSP(TSP,ZPTR);
    PAD(SCL,TSP,XSP,VSP);
    BRANCH(GENVSZ)
    /*_*/
}
int
APPLY(retval)
    ret_t retval;
{
    ENTRY(APPLY)
    D_A(XCL) = D_V(INCL);
    D_F(XCL) = D_V(XCL) = 0;
    D_A(XCL)--;
    if (D_A(XCL) < 1)
	BRANCH(ARGNER)
    PUSH(XCL);
    SAVSTK();
    switch (VARVUP(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XCL);
    if (!LOCAPV(XPTR,FNCPL,XPTR))
	BRANCH(UNDF)
    D(INCL) = D(D_A(XPTR) + DESCR);
    D_V(INCL) = D_A(XCL);
    SAVSTK();
    PUSH(INCL);
    switch (INVOKE(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    case 3:
	BRANCH(RTZPTR)
    }
    D(XPTR) = D(ZPTR);
    BRANCH(RTXNAM)
    /*_*/
}
int
ARG(retval)
    ret_t retval;
{
    ENTRY(ARG)
    PUSH(ONECL);
    PUSH(DEFCL);
    BRANCH(ARG1)
    /*_*/
}
static int
ARGINT(retval)
    ret_t retval;
{
    ENTRY(ARGINT)
    POP(XPTR);
    POP(XCL);
    PUSH(ONECL);
    PUSH(DEFCL);
    BRANCH(ARG2)
    /*_*/
}
int
LOCAL(retval)
    ret_t retval;
{
    ENTRY(LOCAL)
    PUSH(ONECL);
    PUSH(ZEROCL);
    PUSH(DEFCL);
    BRANCH(ARG1)
    /*_*/
}
int
FIELDS(retval)
    ret_t retval;
{
    ENTRY(FIELDS)
    PUSH(ZEROCL);
    PUSH(ZEROCL);
    PUSH(DATCL);
    /* FALL */
    BRANCH(ARG1)
}
static int
ARG1(retval)
    ret_t retval;
{
    ENTRY(ARG1)
    SAVSTK();
    switch (VARVUP(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (INTVAL(XCL)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_A(ZEROCL) >= D_A(XCL))
	BRANCH(FAIL)
    POP(XPTR);
    /* FALL */
    BRANCH(ARG2)
}
static int
ARG2(retval)
    ret_t retval;
{
    ENTRY(ARG2)
    if (!LOCAPV(XPTR,FNCPL,XPTR))
	BRANCH(INTR30)
    D(XPTR) = D(D_A(XPTR) + DESCR);
    D(YCL) = D(D_A(XPTR));
    D(XPTR) = D(D_A(XPTR) + DESCR);
    POP(ZCL);
    POP(ALCL);
    if (D_A(YCL) != D_A(ZCL))
	BRANCH(INTR30)
    D_A(XCL) *= DESCR;
    D_F(XCL) = D_V(XCL) = 0;
    D_A(XCL) += 2*DESCR;
    D_A(YCL) = D_V(YCL);
    D_F(YCL) = D_V(YCL) = 0;
    D_A(YCL) *= DESCR;
    D_F(YCL) = D_V(YCL) = 0;
    if (D_A(ALCL) == 0)
	goto L_ARG4;
    D_A(YCL) += 2*DESCR;
    D(ZCL) = D(YCL);
    goto L_ARG5;
    /*_*/
L_ARG4:
    D_A(ZCL) = D_V(D_A(XPTR));
    D_F(ZCL) = D_V(ZCL) = 0;
    POP(ALCL);
    if (D_A(ALCL) == 0)
	goto L_ARG5;
    D_A(XCL) += D_A(YCL);
L_ARG5:
    if (D_A(XCL) > D_A(ZCL))
	BRANCH(FAIL)
    D(ZPTR) = D(D_A(XPTR) + D_A(XCL));
    BRANCH(RTZPTR)
    /*_*/
}
int
CLEAR(retval)
    ret_t retval;
{
    ENTRY(CLEAR)
    SAVSTK();
    switch (ARGVAL(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    D_A(DMPPTR) = (int_t) (OBLIST-DESCR);
L_CLEAR1:
    if (D_PTR(DMPPTR) > D_PTR(OBEND))
	BRANCH(RETNUL)
    D_A(DMPPTR) += DESCR;
    D(YPTR) = D(DMPPTR);
L_CLEAR2:
    D_A(YPTR) = D_A(D_A(YPTR) + LNKFLD);
    if (D_A(YPTR) == 0)
	goto L_CLEAR1;
    D(D_A(YPTR) + DESCR) = D(NULVCL);
    goto L_CLEAR2;
    /*_*/
}
int
CMA(retval)
    ret_t retval;
{
    ENTRY(CMA)
    D_A(ZCL) = D_V(INCL);
    D_F(ZCL) = D_V(ZCL) = 0;
L_CMA1:
    if (D_A(ZCL) == 0)
	BRANCH(FAIL)
    PUSH(ZCL);
    PUSH(OCBSCL);
    PUSH(OCICL);
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	goto L_CMA2;
    }
    POP(OCICL);
    POP(OCBSCL);
    POP(ZCL);
    SAVSTK();
    PUSH(ZCL);
    switch (CODSKP(NORET)) {
    case 1:
	BRANCH(RTXPTR)
    }
L_CMA2:
    POP(OCICL);
    POP(OCBSCL);
    POP(ZCL);
    D_A(ZCL)--;
    SAVSTK();
    PUSH(ONECL);
    switch (CODSKP(NORET)) {
    case 1:
	goto L_CMA1;
    }
    /*_*/
}
int
COLECT(retval)
    ret_t retval;
{
    ENTRY(COLECT)
    SAVSTK();
    switch (INTVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_A(XPTR) < 0)
	BRANCH(LENERR)
    SAVSTK();
    PUSH(XPTR);
    switch (GC(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    D_V(ZPTR) = I;
    BRANCH(RTZPTR)
    /*_*/
}
int
COPY(retval)
    ret_t retval;
{
    ENTRY(COPY)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(XPTR) == S)
	BRANCH(INTR1)
    if (D_V(XPTR) == I)
	BRANCH(INTR1)
    if (D_V(XPTR) == R)
	BRANCH(INTR1)
    if (D_V(XPTR) == N)
	BRANCH(INTR1)
    if (D_V(XPTR) == K)
	BRANCH(INTR1)
    if (D_V(XPTR) == E)
	BRANCH(INTR1)
    if (D_V(XPTR) == T)
	BRANCH(INTR1)
    D_A(XCL) = D_V(D_A(XPTR));
    D_F(XCL) = D_V(XCL) = 0;
    D_V(XCL) = D_V(XPTR);
    SAVSTK();
    PUSH(XCL);
    BLOCK(ZPTR);
    MOVBLK(D_A(ZPTR),D_A(XPTR),D_A(XCL));
    BRANCH(RTZPTR)
    /*_*/
}
int
CNVRT(retval)
    ret_t retval;
{
    ENTRY(CNVRT)
    SAVSTK();
    switch (ARGVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(ZPTR);
    SAVSTK();
    switch (VARVUP(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(ZPTR);
    if (!LOCAPV(XPTR,DTATL,YPTR))
	goto L_CNV1;
    D(XPTR) = D(D_A(XPTR) + DESCR);
    D_A(DTCL) = D_V(ZPTR);
    D_F(DTCL) = D_V(DTCL) = 0;
    D_V(DTCL) = D_V(XPTR);
    if (DCMP(DTCL, IVDTP))
	BRANCH(CNVIV)
    if (DCMP(DTCL, VCDTP))
	BRANCH(RECOMP)
    if (DCMP(DTCL, VEDTP))
	BRANCH(CONVE)
    if (DCMP(DTCL, VRDTP))
	BRANCH(CONVR)
    if (DCMP(DTCL, RIDTP))
	BRANCH(CONRI)
    if (DCMP(DTCL, IRDTP))
	BRANCH(CONIR)
    if (DCMP(DTCL, VIDTP))
	BRANCH(CNVVI)
    if (DCMP(DTCL, ATDTP))
	BRANCH(CNVAT)
    if (DCMP(DTCL, TADTP))
	BRANCH(CNVTA)
    if (D_V(ZPTR) == D_V(XPTR))
	BRANCH(RTZPTR)
    if (D_V(XPTR) == S)
	BRANCH(CNVRTS)
    else
	BRANCH(FAIL)
    /*_*/
L_CNV1:
    X_LOCSP(YSP,YPTR);
    if (!LEXEQ(YSP,NUMSP))
	BRANCH(INTR1)
    if (D_V(ZPTR) == I)
	BRANCH(RTZPTR)
    if (D_V(ZPTR) == R)
	BRANCH(RTZPTR)
    if (D_V(ZPTR) != S)
	BRANCH(FAIL)
    X_LOCSP(ZSP,ZPTR);
    if (SPCINT(ZPTR,ZSP))
	BRANCH(RTZPTR)
    if (SPREAL(ZPTR,ZSP))
	BRANCH(RTZPTR)
    else
	BRANCH(FAIL)
    /*_*/
}
static int
RECOMP(retval)
    ret_t retval;
{
    ENTRY(RECOMP)
    D_A(SCL) = 1;
    /* FALL */
    BRANCH(RECOMJ)
}
static int
RECOMJ(retval)
    ret_t retval;
{
    ENTRY(RECOMJ)
    X_LOCSP(TEXTSP,ZPTR);
L_RECOMT:
    D_A(OCALIM) = S_L(TEXTSP);
    D_F(OCALIM) = D_V(OCALIM) = 0;
    if (D_A(OCALIM) == 0)
	goto L_RECOMN;
    D_A(OCALIM) *= DESCR;
    D_F(OCALIM) = D_V(OCALIM) = 0;
    D_A(OCALIM) += 6*DESCR;
    D_V(OCALIM) = C;
    SAVSTK();
    PUSH(OCALIM);
    BLOCK(CMBSCL);
    D(OCLIM) = D(CMBSCL);
    D_A(OCLIM) += D_A(OCALIM);
    D_A(OCLIM) -= 6*DESCR;
    D_A(CMOFCL) = 0;
    D_A(ESAICL) = 0;
    PUSH(CMBSCL);
    switch (D_A(SCL)) {
    case 2:
	BRANCH(CONVEX)
    }
L_RECOM1:
    if (S_L(TEXTSP) == 0)
	goto L_RECOM2;
    SAVSTK();
    switch (CMPILE(NORET)) {
    case 1:
	goto L_RECOMF;
    case 3:
	goto L_RECOM1;
    }
L_RECOM2:
    D_A(SCL) = 3;
L_RECOMQ:
    D_A(CMOFCL) += DESCR;
    D(D_A(CMBSCL) + D_A(CMOFCL)) = D(ENDCL);
    POP(ZPTR);
    BRANCH(RECOMZ)
    /*_*/
L_RECOMF:
    D_A(SCL) = 1;
    goto L_RECOMQ;
    /*_*/
L_RECOMN:
    _SPEC(TEXTSP) = _SPEC(BLSP);
    goto L_RECOMT;
    /*_*/
}
static int
RECOMZ(retval)
    ret_t retval;
{
    ENTRY(RECOMZ)
    D_A(CMBSCL) += D_A(CMOFCL);
    SAVSTK();
    PUSH(CMBSCL);
    SPLIT(NORET);
    D_A(OCLIM) = 0;
    D_A(LPTR) = 0;
    ZERBLK(D_A(COMREG),D_A(COMDCT));
    switch (D_A(SCL)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	BRANCH(INTR10)
    case 3:
	BRANCH(RTZPTR)
    }
    /*_*/
}
int
CODER(retval)
    ret_t retval;
{
    ENTRY(CODER)
    SAVSTK();
    switch (VARVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    case 2:
	BRANCH(RECOMP)
    }
    /*_*/
}
static int
CONVE(retval)
    ret_t retval;
{
    ENTRY(CONVE)
    D_A(SCL) = 2;
    BRANCH(RECOMJ)
    /*_*/
}
static int
CONVEX(retval)
    ret_t retval;
{
    ENTRY(CONVEX)
    SAVSTK();
    switch (EXPR(FORMND)) {
    case 1:
	BRANCH(FAIL)
    }
    if (S_L(TEXTSP) != 0)
	BRANCH(FAIL)
    SAVSTK();
    PUSH(FORMND);
    TREPUB(NORET);
    D(ZPTR) = D(CMBSCL);
    D_V(ZPTR) = E;
    D_A(SCL) = 3;
    BRANCH(RECOMZ)
    /*_*/
}
static int
CONVR(retval)
    ret_t retval;
{
    ENTRY(CONVR)
    X_LOCSP(ZSP,ZPTR);
    if (SPCINT(ZPTR,ZSP))
	BRANCH(CONIR)
    if (SPREAL(ZPTR,ZSP))
	BRANCH(RTZPTR)
    else
	BRANCH(FAIL)
    /*_*/
}
static int
CONIR(retval)
    ret_t retval;
{
    ENTRY(CONIR)
    D_RV(ZPTR) = (real_t)D_A(ZPTR);
    D_F(ZPTR) = 0;
    D_V(ZPTR) = R;
    BRANCH(RTZPTR)
    /*_*/
}
static int
CONRI(retval)
    ret_t retval;
{
    ENTRY(CONRI)
    CLR_MATH_ERROR();
    D_A(ZPTR) = (int_t) D_RV(ZPTR);
    D_F(ZPTR) = 0;
    D_V(ZPTR) = I;
    if (MATH_ERROR())
	BRANCH(FAIL)
    BRANCH(RTZPTR)
    /*_*/
}
static int
CNVIV(retval)
    ret_t retval;
{
    ENTRY(CNVIV)
    SAVSTK();
    PUSH(ZPTR);
    switch (GNVARI(ZPTR)) {
    case 1:
	BRANCH(RTZPTR)
    }
    /*_*/
}
static int
CNVVI(retval)
    ret_t retval;
{
    ENTRY(CNVVI)
    X_LOCSP(ZSP,ZPTR);
    if (SPCINT(ZPTR,ZSP))
	BRANCH(RTZPTR)
    if (SPREAL(ZPTR,ZSP))
	BRANCH(CONRI)
    else
	BRANCH(FAIL)
    /*_*/
}
static int
CNVRTS(retval)
    ret_t retval;
{
    ENTRY(CNVRTS)
    SAVSTK();
    PUSH(ZPTR);
    DTREP(XPTR);
    _SPEC(ZSP) = _SPEC(D_A(XPTR));
    BRANCH(GENVRZ)
    /*_*/
}
static int
CNVTA(retval)
    ret_t retval;
{
    ENTRY(CNVTA)
    D(WPTR) = D(ZPTR);
    SAVSTK();
    PUSH(ZPTR);
    switch (ICNVTA(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    D(YPTR) = D(ZPTR);
    D_A(YCL) *= DESCR;
    D_F(YCL) = D_V(YCL) = 0;
    D_A(YPTR) += 5*DESCR;
    D(TPTR) = D(YPTR);
    D_A(TPTR) += D_A(YCL);
L_CNVTA8:
    D_A(WCL) = D_V(D_A(WPTR));
    D_F(WCL) = D_V(WCL) = 0;
    D_A(WCL) -= 2*DESCR;
    D_A(WCL) = D_A(WPTR) + D_A(WCL);
    D_F(WCL) = D_F(WPTR);
    D_V(WCL) = D_V(WPTR);
L_CNVTA3:
    D(TCL) = D(D_A(WPTR) + DESCR);
    if (DCMP(TCL, NULVCL))
	goto L_CNVTA5;
    D(D_A(TPTR)) = D(TCL);
    D(D_A(YPTR)) = D(D_A(WPTR) + 2*DESCR);
    D_A(YPTR) += DESCR;
    D_A(TPTR) += DESCR;
L_CNVTA5:
    D_A(WPTR) += 2*DESCR;
    if (D_A(WCL) != D_A(WPTR))
	goto L_CNVTA3;
    D(WPTR) = D(D_A(WCL) + 2*DESCR);
    if (D_A(WPTR) != 1)
	goto L_CNVTA8;
    D_A(TPTR) = 0;
    BRANCH(RTZPTR)
    /*_*/
}
static int
ICNVTA(retval)
    ret_t retval;
{
    ENTRY(ICNVTA)
    POP(YPTR);
    D(YCL) = D(ZEROCL);
L_CNVTA7:
    D_A(XCL) = D_V(D_A(YPTR));
    D_F(XCL) = D_V(XCL) = 0;
    D(ZCL) = D(XCL);
    D_A(XCL) -= 3*DESCR;
L_CNVTA1:
    D(WCL) = D(D_A(YPTR) + D_A(XCL));
    if (DCMP(WCL, NULVCL))
	goto L_CNVTA2;
    D_A(YCL)++;
L_CNVTA2:
    if (D_A(XCL) == DESCR)
	goto L_CNVTA6;
    D_A(XCL) -= 2*DESCR;
    goto L_CNVTA1;
    /*_*/
L_CNVTA6:
    D(YPTR) = D(D_A(YPTR) + D_A(ZCL));
    if (D_A(YPTR) != 1)
	goto L_CNVTA7;
    if (D_A(YCL) == 0)
	BRANCH(FAIL)
    D_A(XCL) = D_A(YCL);
    D_A(XCL) *= 2*DESCR;
    D_F(XCL) = D_V(XCL) = 0;
    INTSPC(YSP,YCL);
    S_L(PROTSP) = 0;
    APDSP(PROTSP,YSP);
    APDSP(PROTSP,CMASP);
    D(WCL) = D(ZEROCL);
    D_A(WCL) = 2;
    INTSPC(XSP,WCL);
    APDSP(PROTSP,XSP);
    _SPEC(XSP) = _SPEC(PROTSP);
    SAVSTK();
    PUSH(XSPPTR);
    GENVAR(TPTR);
    D(ZCL) = D(XCL);
    D_A(XCL) += 4*DESCR;
    SAVSTK();
    PUSH(XCL);
    BLOCK(ZPTR);
    D_V(ZPTR) = A;
    D(ATPRCL) = D(TPTR);
    D_V(ATEXCL) = D_A(YCL);
    MOVBLK(D_A(ZPTR),D_A(ATRHD),D_A(FRDSCL));
    D(retval) = D(ZPTR);
    RETURN(2)
    /*_*/
}
static int
CNVAT(retval)
    ret_t retval;
{
    ENTRY(CNVAT)
    D(XCL) = D(D_A(ZPTR) + 2*DESCR);
    D(YPTR) = D(ZPTR);
    if (D_A(XCL) != 2)
	BRANCH(FAIL)
    D(XCL) = D(D_A(ZPTR) + 3*DESCR);
    if (D_V(XCL) != 2)
	BRANCH(FAIL)
    D_A(XCL) = D_V(D_A(ZPTR));
    D_F(XCL) = D_V(XCL) = 0;
    D_A(XCL) -= 2*DESCR;
    SAVSTK();
    PUSH(XCL);
    BLOCK(XPTR);
    D_V(XPTR) = T;
    D(YCL) = D(D_A(ZPTR) + 4*DESCR);
    D(ZPTR) = D(XPTR);
    D(D_A(XPTR) + D_A(XCL)) = D(ONECL);
    D_A(XCL) -= DESCR;
    D(TCL) = D(EXTVAL);
    D_A(TCL) += 2*DESCR;
    D(D_A(XPTR) + D_A(XCL)) = D(TCL);
    D_A(YCL) = D_V(YCL);
    D_F(YCL) = D_V(YCL) = 0;
    D_A(YCL) *= DESCR;
    D_F(YCL) = D_V(YCL) = 0;
    D_A(YPTR) += 5*DESCR;
    D(WPTR) = D(YPTR);
    D_A(WPTR) += D_A(YCL);
L_CNVAT2:
    D(D_A(XPTR) + DESCR) = D(D_A(WPTR));
    D(D_A(XPTR) + 2*DESCR) = D(D_A(YPTR));
    D_A(YCL) -= DESCR;
    if (D_A(YCL) == 0)
	BRANCH(RTZPTR)
    D_A(XPTR) += 2*DESCR;
    D_A(WPTR) += DESCR;
    D_A(YPTR) += DESCR;
    goto L_CNVAT2;
    /*_*/
}
int
DATE(retval)
    ret_t retval;
{
    ENTRY(DATE)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    _DATE(ZSP,XPTR);
    BRANCH(GENVRZ)
    /*_*/
}
int
DT(retval)
    ret_t retval;
{
    ENTRY(DT)
    SAVSTK();
    switch (ARGVAL(A2PTR)) {
    case 1:
	BRANCH(FAIL)
    }
    D_V(DT1CL) = D_V(A2PTR);
    if (!LOCAPT(A3PTR,DTATL,DT1CL))
	goto L_DTEXTN;
    D(A3PTR) = D(D_A(A3PTR) + 2*DESCR);
L_DTRTN:
    D(retval) = D(A3PTR);
    RETURN(3)
    /*_*/
L_DTEXTN:
    D(A3PTR) = D(EXTPTR);
    goto L_DTRTN;
    /*_*/
}
int
DMP(retval)
    ret_t retval;
{
    ENTRY(DMP)
    SAVSTK();
    switch (INTVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_A(XPTR) == 0)
	BRANCH(RETNUL)
    /* FALL */
    BRANCH(DUMP)
}
static int
DUMP(retval)
    ret_t retval;
{
    ENTRY(DUMP)
    D_A(WPTR) = (int_t) (OBLIST-DESCR);
L_DMPB:
    if (D_PTR(WPTR) > D_PTR(OBEND))
	BRANCH(RETNUL)
    D_A(WPTR) += DESCR;
    D(YPTR) = D(WPTR);
L_DMPA:
    D_A(YPTR) = D_A(D_A(YPTR) + LNKFLD);
    if (D_A(YPTR) == 0)
	goto L_DMPB;
    D(XPTR) = D(D_A(YPTR) + DESCR);
    if (DCMP(XPTR, NULVCL))
	goto L_DMPA;
    S_L(DMPSP) = 0;
    X_LOCSP(YSP,YPTR);
    D_A(YCL) = S_L(YSP);
    D_F(YCL) = D_V(YCL) = 0;
    if (D_A(YCL) >= BUFLEN)
	goto L_DMPOVR;
    APDSP(DMPSP,YSP);
    APDSP(DMPSP,BLEQSP);
    if (D_V(XPTR) == S)
	goto L_DMPV;
    if (D_V(XPTR) == I)
	goto L_DMPI;
    SAVSTK();
    PUSH(XPTR);
    DTREP(A1PTR);
    _SPEC(YSP) = _SPEC(D_A(A1PTR));
L_DMPX:
    D_A(XCL) = S_L(YSP);
    D_F(XCL) = D_V(XCL) = 0;
    D_A(YCL) += D_A(XCL);
    if (D_A(YCL) > BUFLEN)
	goto L_DMPOVR;
    APDSP(DMPSP,YSP);
    if (D_V(XPTR) != T)
	goto L_DMPRT;
    if (!(D_F(D_A(XPTR)) & FRZN))
	goto L_DMPRT;
    APDSP(DMPSP,FRZNSP);
    goto L_DMPRT;
    /*_*/
L_DMPV:
    X_LOCSP(YSP,XPTR);
    D_A(XCL) = S_L(YSP);
    D_F(XCL) = D_V(XCL) = 0;
    D_A(YCL) += D_A(XCL);
    if (D_A(YCL) > BUFLEN)
	goto L_DMPOVR;
    APDSP(DMPSP,QTSP);
    APDSP(DMPSP,YSP);
    APDSP(DMPSP,QTSP);
L_DMPRT:
    IO_PRINT(IOKEY, OUTBLK, DMPSP);
    goto L_DMPA;
    /*_*/
L_DMPI:
    INTSPC(YSP,XPTR);
    goto L_DMPX;
    /*_*/
L_DMPOVR:
    io_printf(D_A(OUTPUT),PRTOVF);
    goto L_DMPA;
    /*_*/
}
static int
DMK(retval)
    ret_t retval;
{
    ENTRY(DMK)
    io_printf(D_A(OUTPUT),PKEYF);
    D_A(XCL) = D_V(D_A(KNLIST));
    D_F(XCL) = D_V(XCL) = 0;
L_DMPK1:
    D(XPTR) = D(D_A(KNLIST) + D_A(XCL));
    D_A(XCL) -= DESCR;
    D(YPTR) = D(D_A(KNLIST) + D_A(XCL));
    INTSPC(YSP,YPTR);
    X_LOCSP(XSP,XPTR);
    S_L(DMPSP) = 0;
    APDSP(DMPSP,AMPSP);
    APDSP(DMPSP,XSP);
    APDSP(DMPSP,BLEQSP);
    if (D_V(YPTR) == S)
	goto L_DMPKV;
    APDSP(DMPSP,YSP);
L_DMPK2:
    IO_PRINT(IOKEY, OUTBLK, DMPSP);
    D_A(XCL) -= DESCR;
    if (D_A(XCL) == 0)
	BRANCH(RTN1)
    else
	goto L_DMPK1;
    /*_*/
L_DMPKV:
    X_LOCSP(YSP,YPTR);
    APDSP(DMPSP,QTSP);
    D_A(YCL) = S_L(YSP);
    D_F(YCL) = D_V(YCL) = 0;
    if (D_A(YCL) > BUFLEN-20)
	goto L_DMPK3;
    APDSP(DMPSP,YSP);
L_DMPK3:
    APDSP(DMPSP,QTSP);
    goto L_DMPK2;
    /*_*/
}
int
DUPL(retval)
    ret_t retval;
{
    ENTRY(DUPL)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (INTVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XPTR);
    if (D_A(YPTR) < 0)
	BRANCH(FAIL)
    if (D_A(YPTR) == 0)
	BRANCH(RETNUL)
    X_LOCSP(XSP,XPTR);
    D_A(XCL) = S_L(XSP);
    D_F(XCL) = D_V(XCL) = 0;
    CLR_MATH_ERROR();
    D_A(XCL) *= D_A(YPTR);
    if (MATH_ERROR())
	BRANCH(AERROR)
    if (D_A(XCL) > D_A(MLENCL))
	BRANCH(INTR8)
    SAVSTK();
    PUSH(XCL);
    CONVAR(ZPTR);
    X_LOCSP(TSP,ZPTR);
    S_L(TSP) = 0;
L_DUPL1:
    APDSP(TSP,XSP);
    D_A(YPTR)--;
    if (D_A(YPTR) == 0)
	BRANCH(GENVSZ)
    else
	goto L_DUPL1;
    /*_*/
}
int
OPSYN(retval)
    ret_t retval;
{
    ENTRY(OPSYN)
    SAVSTK();
    switch (VARVUP(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVUP(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(YPTR);
    SAVSTK();
    switch (INTVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(YPTR);
    POP(XPTR);
    if (D_A(XPTR) == 0)
	BRANCH(NONAME)
    if (D_A(ZPTR) == 1)
	goto L_UNYOP;
    if (D_A(ZPTR) == 2)
	goto L_BNYOP;
    if (D_A(ZPTR) != 0)
	BRANCH(INTR30)
    SAVSTK();
    PUSH(XPTR);
    FINDEX(XPTR);
L_UNBF:
    SAVSTK();
    PUSH(YPTR);
    FINDEX(YPTR);
L_OPPD:
    D(D_A(XPTR)) = D(D_A(YPTR));
    D(D_A(XPTR) + DESCR) = D(D_A(YPTR) + DESCR);
    BRANCH(RETNUL)
    /*_*/
L_UNYOP:
    X_LOCSP(XSP,XPTR);
    if (S_L(XSP) != 1)
	goto L_UNAF;
    _SPEC(ZSP) = _SPEC(PROTSP);
    S_L(ZSP) = 0;
    APDSP(ZSP,XSP);
    APDSP(ZSP,LPRNSP);
    switch (STREAM(TSP, ZSP, &UNOPTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_UNAF;
    }
    D(XPTR) = D(STYPE);
L_UNCF:
    X_LOCSP(YSP,YPTR);
    if (S_L(YSP) != 1)
	goto L_UNBF;
    _SPEC(ZSP) = _SPEC(PROTSP);
    S_L(ZSP) = 0;
    APDSP(ZSP,YSP);
    APDSP(ZSP,LPRNSP);
    switch (STREAM(TSP, ZSP, &UNOPTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_UNBF;
    }
    D(YPTR) = D(STYPE);
    goto L_OPPD;
    /*_*/
L_UNAF:
    SAVSTK();
    PUSH(XPTR);
    FINDEX(XPTR);
    goto L_UNCF;
    /*_*/
L_BNYOP:
    X_LOCSP(XSP,XPTR);
    if (S_L(XSP) > S_L(EQLSP))
	goto L_BNAF;
    _SPEC(ZSP) = _SPEC(PROTSP);
    S_L(ZSP) = 0;
    APDSP(ZSP,XSP);
    APDSP(ZSP,BLSP);
    if (D_A(SPITCL) != 0)
	goto L_BNYOP2;
    if (D_A(BLOKCL) != 0)
	goto L_BNYOP4;
    switch (STREAM(TSP, ZSP, &BIOPTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_BNAF;
    }
    goto L_BNYOP3;
    /*_*/
L_BNYOP4:
    switch (STREAM(TSP, ZSP, &BBIOPTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_BNAF;
    }
    goto L_BNYOP3;
    /*_*/
L_BNYOP2:
    if (D_A(BLOKCL) != 0)
	goto L_BNYOP5;
    switch (STREAM(TSP, ZSP, &SBIPTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_BNAF;
    }
    goto L_BNYOP3;
    /*_*/
L_BNYOP5:
    switch (STREAM(TSP, ZSP, &BSBIPTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_BNAF;
    }
L_BNYOP3:
    if (S_L(ZSP) != 0)
	goto L_BNAF;
    D(XPTR) = D(STYPE);
L_BNCF:
    X_LOCSP(YSP,YPTR);
    if (S_L(YSP) > S_L(EQLSP))
	goto L_BNBF;
    _SPEC(ZSP) = _SPEC(PROTSP);
    S_L(ZSP) = 0;
    APDSP(ZSP,YSP);
    APDSP(ZSP,BLSP);
    if (D_A(SPITCL) != 0)
	goto L_BNCF2;
    if (D_A(BLOKSP) != 0)
	goto L_BNCF4;
    switch (STREAM(TSP, ZSP, &BIOPTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_BNBF;
    }
    goto L_BNCF3;
    /*_*/
L_BNCF4:
    switch (STREAM(TSP, ZSP, &BBIOPTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_BNBF;
    }
    goto L_BNCF3;
    /*_*/
L_BNCF2:
    if (D_A(BLOKSP) != 0)
	goto L_BNCF5;
    switch (STREAM(TSP, ZSP, &SBIPTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_BNBF;
    }
    goto L_BNCF3;
    /*_*/
L_BNCF5:
    switch (STREAM(TSP, ZSP, &BSBIPTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_BNBF;
    }
L_BNCF3:
    if (S_L(ZSP) != 0)
	goto L_BNBF;
    D(YPTR) = D(STYPE);
    goto L_OPPD;
    /*_*/
L_BNAF:
    if (LEXEQ(XSP,BLSP))
	goto L_BNCN;
    SAVSTK();
    PUSH(XPTR);
    FINDEX(XPTR);
    goto L_BNCF;
    /*_*/
L_BNCN:
    D(XPTR) = D(CONCL);
    goto L_BNCF;
    /*_*/
L_BNBF:
    if (!LEXEQ(YSP,BLSP))
	goto L_UNBF;
    D(YPTR) = D(CONCL);
    goto L_OPPD;
    /*_*/
}
int
RPLACE(retval)
    ret_t retval;
{
    ENTRY(RPLACE)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (VARVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(YPTR);
    SAVSTK();
    switch (VARVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(YPTR);
    POP(XPTR);
    if (D_A(XPTR) == 0)
	BRANCH(RTXPTR)
    X_LOCSP(YSP,YPTR);
    X_LOCSP(ZSP,ZPTR);
    if (S_L(ZSP) != S_L(YSP))
	BRANCH(FAIL)
    if (D_A(YPTR) == 0)
	BRANCH(FAIL)
    X_LOCSP(XSP,XPTR);
    D_A(XCL) = S_L(XSP);
    D_F(XCL) = D_V(XCL) = 0;
    SAVSTK();
    PUSH(XCL);
    CONVAR(ZPTR);
    X_LOCSP(TSP,ZPTR);
    S_L(TSP) = 0;
    APDSP(TSP,XSP);
    _RPLACE(TSP,YSP,ZSP);
    BRANCH(GENVSZ)
    /*_*/
}
int
REVERS(retval)
    ret_t retval;
{
    ENTRY(REVERS)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    X_LOCSP(XSP,XPTR);
    D_A(ZPTR) = S_L(XSP);
    D_F(ZPTR) = D_V(ZPTR) = 0;
    if (D_A(ZPTR) == 0)
	BRANCH(RETNUL)
    D_A(XCL) = D_A(ZPTR);
    SAVSTK();
    PUSH(XCL);
    CONVAR(ZPTR);
    X_LOCSP(TSP,ZPTR);
    REVERSE(TSP,XSP);
    BRANCH(GENVSZ)
    /*_*/
}
int
SIZE(retval)
    ret_t retval;
{
    ENTRY(SIZE)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    X_LOCSP(XSP,XPTR);
    D_A(ZPTR) = S_L(XSP);
    D_F(ZPTR) = D_V(ZPTR) = 0;
    D_V(ZPTR) = I;
    BRANCH(RTZPTR)
    /*_*/
}
int
SUBSTR(retval)
    ret_t retval;
{
    ENTRY(SUBSTR)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(XPTR);
    SAVSTK();
    switch (INTVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(YPTR);
    SAVSTK();
    switch (INTVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(YPTR);
    POP(XPTR);
    if (D_A(YPTR) < 1)
	BRANCH(FAIL)
    D_A(YPTR)--;
    X_LOCSP(XSP,XPTR);
    D_A(WPTR) = S_L(XSP);
    D_F(WPTR) = D_V(WPTR) = 0;
    D_A(WPTR) -= D_A(YPTR);
    if (D_A(ZPTR) > D_A(WPTR))
	BRANCH(FAIL)
    if (D_A(ZPTR) < 0)
	BRANCH(FAIL)
    if (D_A(ZPTR) > 0)
	goto L_SSNOFX;
    D_A(ZPTR) = D_A(WPTR);
L_SSNOFX:
    if (D_A(ZPTR) < 0)
	BRANCH(FAIL)
    if (D_A(ZPTR) == 0)
	BRANCH(RETNUL)
    D_A(XCL) = D_A(ZPTR);
    SAVSTK();
    PUSH(XCL);
    CONVAR(ZPTR);
    X_LOCSP(TSP,ZPTR);
    XSUBSTR(TSP,XSP,YPTR);
    BRANCH(GENVSZ)
    /*_*/
}
int
TIME(retval)
    ret_t retval;
{
    ENTRY(TIME)
    SAVSTK();
    switch (ARGVAL(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    D_RV(ZPTR) = mstime();
    D_F(ZPTR) = D_V(ZPTR) = 0;
    D_RV(ZPTR) -= D_RV(ETMCL);
    D_V(ZPTR) = R;
    BRANCH(RTZPTR)
    /*_*/
}
int
TRIM(retval)
    ret_t retval;
{
    ENTRY(TRIM)
    SAVSTK();
    switch (VARVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    X_LOCSP(ZSP,XPTR);
    TRIMSP(ZSP,ZSP);
    BRANCH(GENVRZ)
    /*_*/
}
int
VDIFFR(retval)
    ret_t retval;
{
    ENTRY(VDIFFR)
    SAVSTK();
    switch (XYARGS(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    if (DCMP(XPTR, YPTR))
	BRANCH(FAIL)
    else
	BRANCH(RTXPTR)
    /*_*/
}
static int
ADD_NP(retval)
    ret_t retval;
{
    ENTRY(ADD_NP)
    POP(AN_EDGE);
    POP(AN_NODE);
    POP(AN_DIST);
    D_V(AN_NODE) = D_A(AN_DIST);
    if (D_V(AN_EDGE) == EDGDT)
	goto L_AN1;
    SAVSTK();
    PUSH(AN_RD);
    BLOCK(TEMP31);
    SAVSTK();
    PUSH(AN_ED);
    BLOCK(TEMP32);
    D(D_A(TEMP32) + RELS1_) = D(TEMP31);
    D(D_A(TEMP32) + NN1_) = D(AN_1);
    D(D_A(TEMP32) + TR1_) = D(AN_EDGE);
    D(D_A(TEMP31) + DESCR) = D(AN_NODE);
    goto L_AN99;
L_AN1:
    D(TEMP32) = D(D_A(AN_EDGE) + RELS1_);
    SAVSTK();
    PUSH(AN_EDGE);
    BCOPY(AN_EDGE);
    PUSH(AN_EDGE);
    SAVSTK();
    PUSH(TEMP32);
    BCOPY(TEMP32);
    POP(AN_EDGE);
    D_A(TEMP31) = D_V(D_A(TEMP32));
    D_F(TEMP31) = D_V(TEMP31) = 0;
    D(TEMP33) = D(D_A(AN_EDGE) + NN1_);
    D_A(TEMP33)++;
    D(D_A(AN_EDGE) + NN1_) = D(TEMP33);
    D_A(TEMP33) *= DESCR;
    D_F(TEMP33) = D_V(TEMP33) = 0;
    if (D_A(TEMP31) < D_A(TEMP33))
	goto L_AN2;
L_AN3:
    D(D_A(TEMP32) + D_A(TEMP33)) = D(AN_NODE);
    D(TEMP32) = D(AN_EDGE);
    goto L_AN99;
L_AN2:
    PUSH(AN_EDGE);
    PUSH(XPTR);
    SAVSTK();
    PUSH(TEMP32);
    MORE(TEMP32);
    POP(XPTR);
    POP(AN_EDGE);
    D(D_A(AN_EDGE) + RELS1_) = D(TEMP32);
    goto L_AN3;
L_AN99:
    D(retval) = D(TEMP32);
    RETURN(1)
}
static int
AF_MERGE(retval)
    ret_t retval;
{
    ENTRY(AF_MERGE)
    POP(TEMP41);
    POP(TEMP42);
    D(AM_DIR) = D(ZEROCL);
L_AM4:
    D_A(AM_DIR) += DESCR;
    if (D_A(AM_DIR) > FRSIZE_)
	BRANCH(RTN1)
    D(TEMP43) = D(D_A(TEMP41) + D_A(AM_DIR));
    if (D_V(TEMP43) == AEDGDT)
	goto L_AM1;
    SAVSTK();
    PUSH(AM_DE);
    BLOCK(AM_AE);
    D(D_A(AM_AE) + VAL_) = D(TEMP43);
    D(D_A(TEMP41) + D_A(AM_DIR)) = D(AM_AE);
    D(TEMP43) = D(AM_AE);
L_AM1:
    D(TEMP44) = D(D_A(TEMP42) + D_A(AM_DIR));
    if (D_V(TEMP44) == AEDGDT)
	goto L_AM2;
    SAVSTK();
    PUSH(TEMP44);
    PUSH(TEMP43);
    COAG(NORET);
    goto L_AM4;
L_AM2:
L_AM3:
    D(AM_E) = D(D_A(TEMP44) + VAL_);
    SAVSTK();
    PUSH(AM_E);
    PUSH(TEMP43);
    COAG(NORET);
    D(TEMP44) = D(D_A(TEMP44) + NEXT_);
    if (D_A(TEMP44) != 0)
	goto L_AM3;
    goto L_AM4;
}
static int
AFRAME(retval)
    ret_t retval;
{
    ENTRY(AFRAME)
    POP(TEMP1);
    D(TEMP2) = D(D_A(TEMP1) + VALUE_);
    if (D_V(TEMP2) == S)
	goto L_AFR4;
    D(TEMP39) = D(D_A(TEMP2) + ORG_);
    D_A(TEMP39) = D_V(TEMP39);
    D_F(TEMP39) = D_V(TEMP39) = 0;
    if (D_A(TEMP39) > OVY_)
	goto L_AFR4;
    SAVSTK();
    PUSH(AF_SIZE);
    BLOCK(TEMP3);
    D(TEMP4) = D(D_A(TEMP2) + ORG_);
    D_A(TEMP4) = D_V(TEMP4);
    D_F(TEMP4) = D_V(TEMP4) = 0;
    D_A(TEMP4) *= DESCR;
    D_F(TEMP4) = D_V(TEMP4) = 0;
    D(TEMP5) = D(D_A(TEMP1) + TNREG_);
    SAVSTK();
    PUSH(AF_SIZE);
    BLOCK(TEMP6);
    D(D_A(TEMP6) + D_A(TEMP4)) = D(AF_Z);
    SAVSTK();
    PUSH(TEMP1);
    LSOHN(TEMP7);
    PUSH(TEMP2);
    PUSH(TEMP3);
    PUSH(TEMP5);
    PUSH(TEMP1);
    PUSH(TEMP4);
    PUSH(TEMP6);
    D(AF_NF) = D(ZEROCL);
L_AFR3A:
    D(TEMP31) = D(D_A(TEMP7) + VALUE_);
    if (D_V(TEMP31) == S)
	goto L_T1;
    D(TEMP39) = D(D_A(TEMP31) + ORG_);
    if (D_V(TEMP39) == REP_)
	goto L_AFR1A;
L_T1:
    PUSH(TEMP7);
    PUSH(AF_NF);
    SAVSTK();
    PUSH(NULVCL);
    PUSH(TEMP7);
    B_PB(TEMP31);
    POP(AF_NF);
    POP(TEMP7);
    D(TEMP31) = D(D_A(TEMP31) + FRAME_);
    D(TEMP31) = D(D_A(TEMP31) + HT_);
    if (D_V(TEMP31) != EDGDT)
	goto L_AFR1A;
    D_A(AF_NF) = 1;
L_AFR1A:
    D(TEMP7) = D(D_A(TEMP7) + RSIB_);
    if (D_A(TEMP7) != 0)
	goto L_AFR3A;
    POP(TEMP6);
    POP(TEMP4);
    POP(TEMP1);
    PUSH(AF_NF);
    if (D_A(TEMP4)>= 3*DESCR) {
	D_A(TEMP4) = 0;
	D_F(TEMP4) = D_V(TEMP4) = 0;
    }
    D_A(TEMP4) += DESCR;
    PUSH(TEMP1);
    PUSH(TEMP4);
    PUSH(TEMP6);
    SAVSTK();
    PUSH(TEMP4);
    PUSH(TEMP1);
    FORCING(TEMP31);
    POP(TEMP6);
    POP(TEMP4);
    POP(TEMP1);
    D(D_A(TEMP6) + D_A(TEMP4)) = D(TEMP31);
    if (D_A(TEMP4)>= 3*DESCR) {
	D_A(TEMP4) = 0;
	D_F(TEMP4) = D_V(TEMP4) = 0;
    }
    D_A(TEMP4) += DESCR;
    PUSH(TEMP1);
    PUSH(TEMP4);
    PUSH(TEMP6);
    SAVSTK();
    PUSH(TEMP4);
    PUSH(TEMP1);
    FORCING(TEMP31);
    POP(TEMP6);
    POP(TEMP4);
    POP(TEMP1);
    D(D_A(TEMP6) + D_A(TEMP4)) = D(TEMP31);
    if (D_A(TEMP4)>= 3*DESCR) {
	D_A(TEMP4) = 0;
	D_F(TEMP4) = D_V(TEMP4) = 0;
    }
    D_A(TEMP4) += DESCR;
    POP(AF_NF);
    POP(TEMP5);
    POP(TEMP3);
    POP(TEMP2);
    D(TEMP7) = D(D_A(TEMP1) + LSO_);
L_AFR3:
    D(TEMP31) = D(D_A(TEMP7) + VALUE_);
    if (D_V(TEMP31) == S)
	goto L_T2;
    D(TEMP39) = D(D_A(TEMP31) + ORG_);
    if (D_V(TEMP39) == REP_)
	goto L_AFR1;
L_T2:
    D(TEMP8) = D(D_A(TEMP31) + FRAME_);
    if (D_A(AF_NF) != 0)
	goto L_AFR6;
    D(TEMP8) = D(D_A(TEMP8) + D_A(TEMP4));
    D(TEMP31) = D(D_A(TEMP6) + D_A(TEMP4));
    D_A(TEMP31) += D_A(TEMP8);
    D(D_A(TEMP6) + D_A(TEMP4)) = D(TEMP31);
    goto L_AFR2;
L_AFR6:
    SAVSTK();
    PUSH(TEMP5);
    PUSH(TEMP8);
    PUSH(TEMP6);
    PUSH(TEMP4);
    F_JOIN(TEMP6);
L_AFR2:
    D(TEMP7) = D(D_A(TEMP7) + RSIB_);
    if (D_A(TEMP7) != 0)
	goto L_AFR3;
    D(TEMP31) = D(D_A(TEMP3) + D_A(TEMP4));
    D(TEMP32) = D(D_A(TEMP6) + D_A(TEMP4));
    if (DCMP(TEMP31, ZEROCL))
	goto L_AFNOTA;
    SAVSTK();
    PUSH(TEMP32);
    PUSH(TEMP31);
    E_ATTACH(TEMP32);
    D(D_A(TEMP3) + D_A(TEMP4)) = D(TEMP32);
    if (D_A(TEMP4)>= 3*DESCR) {
	D_A(TEMP4) = 0;
	D_F(TEMP4) = D_V(TEMP4) = 0;
    }
    D_A(TEMP4) += DESCR;
    D(TEMP31) = D(D_A(TEMP6) + D_A(TEMP4));
    D(D_A(TEMP3) + D_A(TEMP4)) = D(TEMP31);
    if (D_A(TEMP4)>= 3*DESCR) {
	D_A(TEMP4) = 0;
	D_F(TEMP4) = D_V(TEMP4) = 0;
    }
    D_A(TEMP4) += DESCR;
    D(TEMP31) = D(D_A(TEMP6) + D_A(TEMP4));
    D(D_A(TEMP3) + D_A(TEMP4)) = D(TEMP31);
L_AFR21:
    SAVSTK();
    PUSH(NULVCL);
    PUSH(TEMP3);
    PUSH(TEMP2);
    NP_BLOCK(TEMP31);
    D(D_A(TEMP1) + VALUE_) = D(TEMP31);
    goto L_AFR99;
L_AFNOTA:
    D(TEMP3) = D(TEMP6);
    goto L_AFR21;
L_AFR1:
    D(TEMP31) = D(D_A(TEMP3) + D_A(TEMP4));
    D(TEMP32) = D(D_A(TEMP6) + D_A(TEMP4));
    if (DCMP(TEMP31, ZEROCL))
	goto L_AFR11;
    SAVSTK();
    PUSH(TEMP32);
    PUSH(TEMP31);
    E_ATTACH(TEMP32);
L_AFR11:
    D(D_A(TEMP3) + D_A(TEMP4)) = D(TEMP32);
    D(D_A(TEMP6) + D_A(TEMP4)) = D(AF_Z);
    goto L_AFR2;
L_AFR4:
    D(TEMP31) = D(D_A(TEMP2) + ORG_);
    if (D_V(TEMP31) != NODE_)
	goto L_AFR5;
    PUSH(TEMP1);
    SAVSTK();
    PUSH(TEMP1);
    B_PB(TEMP2);
    POP(TEMP1);
L_AFR5:
    if (D_V(TEMP2) == S)
	goto L_T3;
    D(TEMP39) = D(D_A(TEMP2) + ORG_);
    if (D_V(TEMP39) == PHY_)
	goto L_AFR51;
L_T3:
    D_A(ERRTYP) = (int_t) (ERRNO+3);
    BRANCH(FTLEND)
L_AFR51:
    D(TEMP3) = D(D_A(TEMP2) + FRAME_);
    goto L_AFR99;
L_AFR99:
    D(retval) = D(TEMP3);
    RETURN(1)
}
static int
B_PB(retval)
    ret_t retval;
{
    ENTRY(B_PB)
    POP(XPTR);
    D(TEMP1) = D(D_A(XPTR) + VALUE_);
    if (D_V(TEMP1) != S)
	goto L_BPNOST;
    if (D_A(TEMP1) != 0)
	goto L_BPNONL;
    SAVSTK();
    PUSH(NULVCL);
    PUSH(BP_0);
    PUSH(BP_0);
    PUSH(BP_0);
    PUSH(NULVCL);
    P_BLOCK(TEMP12);
    goto L_BPST1;
L_BPNONL:
    SAVSTK();
    PUSH(BP_ARY);
    BLOCK(BP_T);
    D(D_A(BP_T) + DESCR) = D(TEMP1);
    D_A(BP_L) = D_V(D_A(TEMP1));
    D_F(BP_L) = D_V(BP_L) = 0;
    D_V(BP_L) = I;
    SAVSTK();
    PUSH(BP_T);
    PUSH(BP_1);
    PUSH(BP_L);
    PUSH(BP_1);
    PUSH(TEMP1);
    P_BLOCK(TEMP12);
L_BPST1:
    D(D_A(XPTR) + VALUE_) = D(TEMP12);
    goto L_BP99;
L_BPNOST:
    D(BP_ORG) = D(D_A(TEMP1) + ORG_);
    D_A(BP_ORG) = D_V(BP_ORG);
    D_F(BP_ORG) = D_V(BP_ORG) = 0;
    switch (D_A(BP_ORG)) {
    case 1:
	goto L_BPORG1;
    case 2:
	goto L_BPORG2;
    case 3:
	goto L_BPORG3;
    case 4:
	goto L_BPORG4;
    case 5:
	goto L_BPORG5;
    case 6:
	goto L_BPORG6;
    case 7:
	goto L_BPORG7;
    case 8:
	goto L_BPORG8;
    case 9:
	goto L_BPORG9;
    }
L_BPORG1:
L_BPORG2:
L_BPORG3:
    PUSH(XPTR);
    PUSH(TEMP1);
    SAVSTK();
    PUSH(XPTR);
    AFRAME(TEMP3);
    POP(TEMP1);
    POP(XPTR);
    D(BP_ORG) = D(D_A(TEMP1) + ORG_);
    D_A(BP_ORG) = D_V(BP_ORG);
    D_F(BP_ORG) = D_V(BP_ORG) = 0;
    D_A(BP_DIR) = D_A(BP_ORG);
    D_A(BP_DIR) *= DESCR;
    D_F(BP_DIR) = D_V(BP_DIR) = 0;
    D(TEMP2) = D(D_A(XPTR) + TNREG_);
    D(TEMP4) = D(D_A(TEMP3) + D_A(BP_DIR));
    if (D_V(TEMP4) != AEDGEDT)
	goto L_BPCONT1;
    D(BP_N) = D(ZEROCL);
    D(BP_L) = D(ZEROCL);
L_BPCONT2:
    D_A(BP_N) += DESCR;
    D(BP_EDGE) = D(D_A(TEMP4) + VAL_);
    if (D_V(BP_EDGE) == EDGDT)
	D(XCL) = D(D_A(BP_EDGE) + TR1_);
    else
	D(XCL) = D(BP_EDGE);
    D_A(BP_L) += D_A(XCL);
    D(YPTR) = D(D_A(TEMP4) + NEXT_);
    if (D_A(YPTR) == 0)
	goto L_BPCONT3;
    D(TEMP4) = D(YPTR);
    goto L_BPCONT2;
L_BPCONT1:
    D_A(BP_N) = (int_t) DESCR;
    D(TEMP5) = D(TEMP3);
    D(BP_L) = D(D_A(TEMP3) + D_A(BP_DIR));
    if (D_V(BP_L) == EDGDT)
	D(BP_L) = D(D_A(BP_L) + TR1_);
    goto L_BPCONT4;
L_BPCONT3:
    PUSH(XPTR);
    PUSH(TEMP1);
    PUSH(TEMP2);
    PUSH(TEMP3);
    PUSH(BP_DIR);
    PUSH(BP_L);
    PUSH(BP_N);
    D(BP_A) = D(BP_L);
    SAVSTK();
    PUSH(XPTR);
    switch (PAR_CONG(YPTR)) {
    case 1:
	goto L_BPCONT31;
    }
    SAVSTK();
    PUSH(BP_DIR);
    PUSH(YPTR);
    FORCING(BP_A);
L_BPCONT31:
    POP(BP_N);
    POP(BP_L);
    POP(BP_DIR);
    POP(TEMP3);
    POP(TEMP2);
    POP(TEMP1);
    POP(XPTR);
    D_A(BP_A) -= D_A(BP_L);
    D_A(BP_N) -= DESCR;
    SAVSTK();
    PUSH(BP_N);
    PUSH(BP_A);
    DISTR(TEMP6);
    D_A(BP_N) = (int_t) DESCR;
    D(TEMP4) = D(D_A(TEMP3) + D_A(BP_DIR));
    D(TEMP7) = D(D_A(TEMP4) + VAL_);
    if (D_V(TEMP7) != EDGDT)
	goto L_BPCONT33;
    SAVSTK();
    PUSH(TEMP7);
    BCOPY(TEMP7);
L_BPCONT33:
    D(TEMP4) = D(D_A(TEMP4) + NEXT_);
    if (D_A(TEMP4) == 0)
	goto L_BPCONT34;
    D(BP_L) = D(D_A(TEMP6) + D_A(BP_N));
    D_A(BP_N) += DESCR;
    SAVSTK();
    PUSH(BP_L);
    PUSH(TEMP7);
    JE_LONGI(TEMP7);
    D(BP_L) = D(D_A(TEMP4) + VAL_);
    SAVSTK();
    PUSH(BP_L);
    PUSH(TEMP7);
    JE_LONGI(TEMP7);
    goto L_BPCONT33;
L_BPCONT34:
    D(BP_N) = D(ZEROCL);
    SAVSTK();
    PUSH(TEMP3);
    BCOPY(TEMP5);
    D(D_A(TEMP5) + D_A(BP_DIR)) = D(TEMP7);
L_BPCONT4:
    SAVSTK();
    PUSH(XPTR);
    LSOHN(TEMP8);
    SAVSTK();
    PUSH(TEMP5);
    STRIP_F(TEMP9);
    D(BP_H) = D(D_A(TEMP9) + HT_);
    D(BP_D) = D(D_A(TEMP9) + DTH_);
    D(BP_W) = D(D_A(TEMP9) + WTH_);
    PUSH(XPTR);
    SAVSTK();
    PUSH(BP_D);
    PUSH(BP_W);
    PUSH(BP_H);
    switch (BLAND(TEMP10)) {
    case 1:
	goto L_BPCONTNL;
    }
    POP(XPTR);
    SAVSTK();
    PUSH(TEMP9);
    IDENT_SB(TEMP11);
    SAVSTK();
    PUSH(TEMP10);
    PUSH(TEMP5);
    PUSH(TEMP1);
    NP_BLOCK(TEMP12);
L_BPCONT5:
    D(BP_T) = D(D_A(TEMP8) + VALUE_);
    if (D_V(BP_T) == S)
	goto L_T4;
    D(TEMP39) = D(D_A(BP_T) + ORG_);
    if (D_V(TEMP39) == REP_)
	goto L_BPCONT6;
L_T4:
    PUSH(TEMP9);
    PUSH(XPTR);
    PUSH(BP_DIR);
    PUSH(BP_N);
    PUSH(TEMP2);
    PUSH(TEMP5);
    PUSH(TEMP6);
    PUSH(TEMP8);
    PUSH(TEMP10);
    PUSH(TEMP11);
    PUSH(TEMP12);
    SAVSTK();
    PUSH(NULVCL);
    PUSH(TEMP8);
    B_PB(TEMP13);
    POP(TEMP12);
    POP(TEMP11);
    POP(TEMP10);
    POP(TEMP8);
    POP(TEMP6);
    POP(TEMP5);
    POP(TEMP2);
    POP(BP_N);
    POP(BP_DIR);
    POP(XPTR);
    POP(TEMP9);
    D(BP_T) = D(D_A(TEMP13) + FRAME_);
    SAVSTK();
    PUSH(BP_T);
    STRIP_F(TEMP14);
L_BPCONT51:
    SAVSTK();
    PUSH(TEMP2);
    PUSH(BP_DIR);
    PUSH(TEMP14);
    PUSH(TEMP11);
    INIT_SUB(TEMP15);
L_BPCONT52:
    D(BP_T) = D(D_A(TEMP13) + ARRAY_);
    SAVSTK();
    PUSH(TEMP15);
    PUSH(BP_T);
    PUSH(TEMP9);
    PUSH(TEMP10);
    INSERT(NORET);
    D(TEMP8) = D(D_A(TEMP8) + RSIB_);
    if (D_A(TEMP8) != 0)
	goto L_BPCONT5;
    D(D_A(XPTR) + LSO_) = D(NULVCL);
    D(D_A(XPTR) + VALUE_) = D(TEMP12);
    goto L_BP99;
L_BPCONT6:
    D_A(BP_N) += DESCR;
    D(BP_L) = D(D_A(TEMP6) + D_A(BP_N));
    PUSH(TEMP2);
    PUSH(TEMP5);
    PUSH(TEMP6);
    PUSH(TEMP8);
    PUSH(TEMP9);
    PUSH(TEMP10);
    PUSH(TEMP11);
    PUSH(TEMP12);
    PUSH(XPTR);
    PUSH(BP_L);
    PUSH(BP_DIR);
    PUSH(BP_N);
    SAVSTK();
    PUSH(BP_L);
    PUSH(TEMP8);
    B_PB(TEMP13);
    POP(BP_N);
    POP(BP_DIR);
    POP(BP_L);
    POP(XPTR);
    POP(TEMP12);
    POP(TEMP11);
    POP(TEMP10);
    POP(TEMP9);
    POP(TEMP8);
    POP(TEMP6);
    POP(TEMP5);
    POP(TEMP2);
    D(TEMP14) = D(D_A(TEMP13) + FRAME_);
    D(BP_T) = D(D_A(TEMP14) + D_A(BP_DIR));
    if (D_A(BP_T) == D_A(BP_L))
	goto L_BPCONT8;
    SAVSTK();
    PUSH(TEMP5);
    BCOPY(TEMP16);
    SAVSTK();
    PUSH(TEMP5);
    BCOPY(TEMP17);
    D(BP_DIF) = D(BP_L);
    D_A(BP_DIF) -= D_A(BP_T);
    if (D_A(BP_TWO) == 0)
	BRANCH(FAIL)
    CLR_MATH_ERROR();
    D(BP_PREF) = D(BP_DIF);
    D_A(BP_PREF) /= D_A(BP_TWO);
    if (MATH_ERROR())
	BRANCH(FAIL)
    D(D_A(TEMP16) + D_A(BP_DIR)) = D(BP_PREF);
    D(BP_T) = D(BP_DIF);
    D_A(BP_T) -= D_A(BP_PREF);
    D(D_A(TEMP17) + D_A(BP_DIR)) = D(BP_T);
    SAVSTK();
    PUSH(TEMP2);
    PUSH(BP_DIR);
    PUSH(TEMP16);
    PUSH(TEMP11);
    INIT_SUB(NORET);
    SAVSTK();
    PUSH(TEMP2);
    PUSH(BP_DIR);
    PUSH(TEMP14);
    PUSH(TEMP11);
    INIT_SUB(TEMP15);
    SAVSTK();
    PUSH(TEMP2);
    PUSH(BP_DIR);
    PUSH(TEMP17);
    PUSH(TEMP11);
    INIT_SUB(NORET);
    goto L_BPCONT52;
L_BPCONT8:
    goto L_BPCONT51;
L_BPCONTNL:
    SAVSTK();
    PUSH(NULVCL);
    PUSH(TEMP5);
    PUSH(TEMP1);
    NP_BLOCK(TEMP12);
    D(D_A(XPTR) + LSO_) = D(NULVCL);
    D(D_A(XPTR) + VALUE_) = D(TEMP12);
    goto L_BP99;
L_BPORG4:
    BRANCH(BMORG4)
L_BPORG5:
    BRANCH(BMORG5)
L_BPORG6:
    BRANCH(BMORG6)
L_BPORG7:
    BRANCH(BMORG7)
L_BP99:
    D(retval) = D(TEMP12);
    RETURN(1)
L_BPORG8:
    D_A(ERRTYP) = (int_t) (ERRNO+1);
    BRANCH(FTLEND)
L_BPORG9:
    D(TEMP12) = D(TEMP1);
    goto L_BP99;
}
static int
BMORG4(retval)
    ret_t retval;
{
    ENTRY(BMORG4)
    SAVSTK();
    PUSH(XPTR);
    LSOHN(TEMP8);
    PUSH(XPTR);
    PUSH(TEMP8);
    SAVSTK();
    PUSH(TEMP8);
    AFRAME(TEMP5);
    POP(TEMP8);
    POP(XPTR);
    SAVSTK();
    PUSH(TEMP5);
    BCOPY(TEMP5);
    D(TEMP32) = D(ZEROCL);
L_BPMRG51:
    D_A(TEMP32) += DESCR;
    if (D_A(TEMP32) > FRSIZE_)
	goto L_BPMRG52;
    D(TEMP31) = D(D_A(TEMP5) + D_A(TEMP32));
    SAVSTK();
    PUSH(TEMP31);
    CAE(TEMP31);
    D(D_A(TEMP5) + D_A(TEMP32)) = D(TEMP31);
    goto L_BPMRG51;
L_BPMRG52:
    D(TEMP31) = D(NULVCL);
L_BPMRG1:
    D(TEMP8) = D(D_A(TEMP8) + RSIB_);
    if (D_A(TEMP8) == 0)
	goto L_BPMRG2;
    PUSH(XPTR);
    PUSH(TEMP5);
    PUSH(TEMP8);
    SAVSTK();
    PUSH(TEMP8);
    AFRAME(TEMP31);
    POP(TEMP8);
    POP(TEMP5);
    POP(XPTR);
    SAVSTK();
    PUSH(TEMP31);
    PUSH(TEMP5);
    AF_MERGE(NORET);
    goto L_BPMRG1;
L_BPMRG2:
    SAVSTK();
    PUSH(TEMP5);
    COMPFR(NORET);
    SAVSTK();
    PUSH(TEMP5);
    STRIP_F(TEMP9);
    D(BM_H) = D(D_A(TEMP9) + HT_);
    D(BM_W) = D(D_A(TEMP9) + WTH_);
    D(BM_D) = D(D_A(TEMP9) + DTH_);
    PUSH(XPTR);
    SAVSTK();
    PUSH(BM_D);
    PUSH(BM_W);
    PUSH(BM_H);
    switch (BLAND(TEMP10)) {
    case 1:
	BRANCH(BMNULL)
    }
    POP(XPTR);
    D(TEMP1) = D(D_A(XPTR) + VALUE_);
    SAVSTK();
    PUSH(TEMP10);
    PUSH(TEMP5);
    PUSH(TEMP1);
    NP_BLOCK(TEMP12);
    D(D_A(XPTR) + VALUE_) = D(TEMP12);
    D(TEMP8) = D(D_A(XPTR) + LSO_);
L_BPMRG3:
    PUSH(XPTR);
    PUSH(TEMP8);
    PUSH(TEMP12);
    SAVSTK();
    PUSH(TEMP8);
    EMB_PHY(NORET);
    POP(TEMP12);
    POP(TEMP8);
    POP(XPTR);
    D(TEMP8) = D(D_A(TEMP8) + RSIB_);
    if (D_A(TEMP8) != 0)
	goto L_BPMRG3;
    D(D_A(XPTR) + LSO_) = D(NULVCL);
    BRANCH(BM99)
}
static int
BMORG5(retval)
    ret_t retval;
{
    ENTRY(BMORG5)
    D(TEMP31) = D(D_A(XPTR) + LSO_);
    PUSH(XPTR);
    SAVSTK();
    PUSH(TEMP31);
    B_PB(TEMP13);
    POP(XPTR);
    D(TEMP10) = D(D_A(TEMP13) + ARRAY_);
    D(TEMP31) = D(D_A(TEMP13) + FRAME_);
    SAVSTK();
    PUSH(TEMP31);
    STRIP_F(TEMP5);
    SAVSTK();
    PUSH(XPTR);
    switch (PAR_CONG(BM_PC)) {
    case 1:
	BRANCH(BMNULL)
    }
    D(BM_PCD) = D(D_A(BM_PC) + VALUE_);
    if (D_V(BM_PCD) == S)
	goto L_BPIT3;
    D(TEMP39) = D(D_A(BM_PCD) + ORG_);
    if (D_V(TEMP39) != PHY_)
	goto L_BPIT3;
    D(BM_PCD) = D(D_A(BM_PCD) + BL_);
L_BPIT3:
    D(BM_PCD) = D(D_A(BM_PCD) + ORG_);
    D_A(BM_PCD) = D_V(BM_PCD);
    D_F(BM_PCD) = D_V(BM_PCD) = 0;
    D_A(BM_PCD) *= DESCR;
    D_F(BM_PCD) = D_V(BM_PCD) = 0;
    D(BM_DIR) = D(BM_PCD);
    if (D_A(BM_DIR)>= 3*DESCR) {
	D_A(BM_DIR) = 0;
	D_F(BM_DIR) = D_V(BM_DIR) = 0;
    }
    D_A(BM_DIR) += DESCR;
L_BPIT1:
    PUSH(XPTR);
    PUSH(TEMP10);
    PUSH(TEMP5);
    PUSH(BM_DIR);
    PUSH(BM_PC);
    PUSH(BM_PCD);
    SAVSTK();
    PUSH(BM_DIR);
    PUSH(BM_PC);
    FORCING(TEMP6);
    POP(BM_PCD);
    POP(BM_PC);
    POP(BM_DIR);
    POP(TEMP5);
    POP(TEMP10);
    POP(XPTR);
    D(TEMP31) = D(D_A(TEMP5) + D_A(BM_DIR));
    if (D_A(TEMP31) == 0)
	goto L_BPIT2;
    if (D_A(TEMP31) == 0)
	BRANCH(FAIL)
    CLR_MATH_ERROR();
    D_A(TEMP6) /= D_A(TEMP31);
    if (MATH_ERROR())
	BRANCH(FAIL)
L_BPIT2:
    PUSH(BM_PC);
    SAVSTK();
    PUSH(TEMP6);
    PUSH(BM_DIR);
    PUSH(TEMP5);
    PUSH(TEMP10);
    REPL(TEMP10);
    POP(BM_PC);
    if (D_A(BM_DIR)>= 3*DESCR) {
	D_A(BM_DIR) = 0;
	D_F(BM_DIR) = D_V(BM_DIR) = 0;
    }
    D_A(BM_DIR) += DESCR;
    if (D_A(BM_DIR) != D_A(BM_PCD))
	goto L_BPIT1;
    D(TEMP31) = D(D_A(XPTR) + VALUE_);
    SAVSTK();
    PUSH(TEMP10);
    PUSH(TEMP5);
    PUSH(TEMP31);
    NP_BLOCK(TEMP12);
    D(D_A(XPTR) + VALUE_) = D(TEMP12);
    D(D_A(XPTR) + LSO_) = D(NULVCL);
    BRANCH(BM99)
}
static int
BMORG6(retval)
    ret_t retval;
{
    ENTRY(BMORG6)
    D(TEMP31) = D(D_A(XPTR) + FATHER_);
    D(BM_PC) = D(D_A(TEMP31) + VALUE_);
    D(TEMP32) = D(D_A(BM_PC) + BL_);
    D(BM_ORG) = D(D_A(TEMP32) + ORG_);
    D_A(BM_DIR) = D_V(BM_ORG);
    D_F(BM_DIR) = D_V(BM_DIR) = 0;
    D_A(BM_DIR) *= DESCR;
    D_F(BM_DIR) = D_V(BM_DIR) = 0;
    D(TEMP31) = D(D_A(XPTR) + LSO_);
    PUSH(BM_DIR);
    PUSH(XPTR);
    SAVSTK();
    PUSH(NULVCL);
    PUSH(TEMP31);
    B_PB(TEMP13);
    POP(XPTR);
    POP(BM_DIR);
    D(TEMP10) = D(D_A(TEMP13) + ARRAY_);
    D(TEMP31) = D(D_A(TEMP13) + FRAME_);
    SAVSTK();
    PUSH(TEMP31);
    STRIP_F(TEMP5);
    POP(BM_A);
    D(BM_L) = D(D_A(TEMP5) + D_A(BM_DIR));
    if (D_A(BM_L) == 0)
	goto L_BPREP1;
    CLR_MATH_ERROR();
    D_A(BM_A) /= D_A(BM_L);
    if (MATH_ERROR())
	goto L_BPREP1;
L_BPREP1:
    SAVSTK();
    PUSH(BM_A);
    PUSH(BM_DIR);
    PUSH(TEMP5);
    PUSH(TEMP10);
    REPL(TEMP10);
    SAVSTK();
    PUSH(TEMP10);
    PUSH(TEMP5);
    PUSH(NULVCL);
    NP_BLOCK(TEMP12);
    D(D_A(XPTR) + VALUE_) = D(TEMP12);
    D(D_A(XPTR) + LSO_) = D(NULVCL);
    BRANCH(BM99)
}
static int
BMORG7(retval)
    ret_t retval;
{
    ENTRY(BMORG7)
    D(BM_T) = D(D_A(TEMP1) + ID_);
    if (!LOCAPT(TEMP31,TEMP50,BM_T))
	goto L_BPND0;
    D(TEMP12) = D(D_A(TEMP31) + 2*DESCR);
    D(D_A(XPTR) + VALUE_) = D(TEMP12);
    BRANCH(BM99)
L_BPND0:
    D(TEMP31) = D(D_A(XPTR) + LSO_);
    PUSH(XPTR);
    PUSH(BM_T);
    PUSH(TEMP1);
    SAVSTK();
    PUSH(NULVCL);
    PUSH(TEMP31);
    B_PB(TEMP13);
    POP(TEMP1);
    POP(BM_T);
    POP(XPTR);
    D(TEMP5) = D(D_A(TEMP13) + FRAME_);
    SAVSTK();
    PUSH(TEMP5);
    BCOPY(TEMP5);
    D_A(BM_DIR) = 0;
L_BPND1:
    D_A(BM_DIR) += DESCR;
    if (D_A(BM_DIR) > DTH_)
	goto L_BPND2;
    D(TEMP31) = D(D_A(TEMP5) + D_A(BM_DIR));
    SAVSTK();
    PUSH(BM_0);
    PUSH(BM_T);
    PUSH(TEMP31);
    ADD_NP(TEMP31);
    D(D_A(TEMP5) + D_A(BM_DIR)) = D(TEMP31);
    goto L_BPND1;
L_BPND2:
    D(TEMP31) = D(D_A(TEMP13) + ARRAY_);
    SAVSTK();
    PUSH(TEMP31);
    PUSH(TEMP5);
    PUSH(TEMP1);
    NP_BLOCK(TEMP12);
    D(WCL) = D(TEMP50);
    SAVSTK();
    BTAIL(TEMP31);
    D(D_A(WCL) + D_A(TEMP31)) = D(BM_T);
    SAVSTK();
    BTAIL(TEMP31);
    D(D_A(WCL) + D_A(TEMP31)) = D(TEMP12);
    D(TEMP50) = D(WCL);
    D(D_A(XPTR) + VALUE_) = D(TEMP12);
    BRANCH(BM99)
}
static int
BMNULL(retval)
    ret_t retval;
{
    ENTRY(BMNULL)
    D(D_A(XPTR) + LSO_) = D(NULVCL);
    SAVSTK();
    PUSH(NULVCL);
    PUSH(BM_0);
    PUSH(BM_0);
    PUSH(BM_0);
    PUSH(NULVCL);
    P_BLOCK(TEMP12);
    D(D_A(XPTR) + VALUE_) = D(TEMP12);
    BRANCH(BM99)
}
static int
BM99(retval)
    ret_t retval;
{
    ENTRY(BM99)
    D(retval) = D(TEMP12);
    RETURN(1)
}
int
BCOPY(retval)
    ret_t retval;
{
    ENTRY(BCOPY)
    POP(BC_B1);
    D_A(BC_N) = D_V(D_A(BC_B1));
    D_F(BC_N) = D_V(BC_N) = 0;
    PUSH(BC_B1);
    D_V(BC_N) = D_V(BC_B1);
    SAVSTK();
    PUSH(BC_N);
    BLOCK(BC_B2);
    POP(BC_B1);
    MOVBLK(D_A(BC_B2),D_A(BC_B1),D_A(BC_N));
    D(retval) = D(BC_B2);
    RETURN(1)
}
static int
BHEAD(retval)
    ret_t retval;
{
    ENTRY(BHEAD)
L_BHEAD0:
    D_A(BH_SIZE) = D_V(D_A(WCL));
    D_F(BH_SIZE) = D_V(BH_SIZE) = 0;
    D(BH_TOP) = D(D_A(WCL) + TOP_);
    if (D_A(BH_TOP) >= D_A(BH_SIZE))
	goto L_BH_MORE;
    D(BH_PTR) = D(WCL);
    D_A(BH_PTR) += D_A(BH_TOP);
    D_A(BH_TOP) += DESCR;
    D(D_A(WCL) + TOP_) = D(BH_TOP);
    D(BH_1ST) = D(WCL);
    D_A(BH_1ST) += FIRST_;
L_BH1:
    D(BH_TEMP) = D(D_A(BH_PTR));
    D(D_A(BH_PTR) + DESCR) = D(BH_TEMP);
    D_A(BH_PTR) -= DESCR;
    if (D_A(BH_PTR) >= D_A(BH_1ST))
	goto L_BH1;
    RETURN(1)
L_BH_MORE:
    SAVSTK();
    PUSH(WCL);
    MORE(WCL);
    goto L_BHEAD0;
}
static int
BLAND(retval)
    ret_t retval;
{
    ENTRY(BLAND)
    POP(BL_H);
    POP(BL_W);
    POP(BL_D);
    D(BL_L) = D(BL_H);
    D_A(BL_L) *= D_A(BL_D);
    D_A(BL_L) *= DESCR;
    D_F(BL_L) = D_V(BL_L) = 0;
    if (D_A(BL_L) == 0)
	BRANCH(FAIL)
    if (D_A(BL_W) == 0)
	BRANCH(FAIL)
    D_V(BL_L) = A;
    SAVSTK();
    PUSH(BL_L);
    BLOCK(TEMP31);
    SAVSTK();
    PUSH(BL_W);
    BLANK(TEMP32);
    D(D_A(TEMP31) + DESCR) = D(TEMP32);
    D(TEMP33) = D(TEMP31);
    D_A(TEMP33) += DESCR;
    D_A(BL_L) -= DESCR;
    if (D_A(BL_L) == 0)
	goto L_BL1;
    MOVBLK2(D_A(TEMP33),D_A(TEMP31),D_A(BL_L));
L_BL1:
    D(retval) = D(TEMP31);
    RETURN(2)
}
static int
BLANK(retval)
    ret_t retval;
{
    ENTRY(BLANK)
    POP(BK_N);
L_BKB:
    S_L(YSP) = D_A(BK_N);
    if (D_A(TEMP49) == 0)
	goto L_BK3;
    if (DCMP(BKGNCL, TEMP26))
	goto L_BK2;
    if (D_V(BKGNCL) != S)
	goto L_BK2;
    if (D_A(BKGNCL) == 0)
	goto L_BK2;
    D(TEMP26) = D(BKGNCL);
    D(TEMP49) = D(BKGNCL);
L_BK2:
    X_LOCSP(ZSP,TEMP49);
    if (S_L(ZSP) < S_L(YSP))
	goto L_BK1;
    S_L(ZSP) = S_L(YSP);
    SAVSTK();
    PUSH(ZSPPTR);
    GENVAR(YPTR);
    D(retval) = D(YPTR);
    RETURN(1)
L_BK1:
    D(XPTR) = D(TEMP49);
    D(YPTR) = D(TEMP49);
    SAVSTK();
    switch (CONVV(TEMP49)) {
    case 1:
    case 2:
	BRANCH(FAIL)
    case 3:
	goto L_BKB;
    }
L_BK3:
    SAVSTK();
    PUSH(BK_PTR);
    GENVAR(TEMP49);
    D(TEMP26) = D(TEMP49);
    goto L_BKB;
}
int
HEIGHT(retval)
    ret_t retval;
{
    ENTRY(HEIGHT)
    D_A(BS_DIR) = (int_t) HT_;
    BRANCH(BLS2)
}
int
WIDTH(retval)
    ret_t retval;
{
    ENTRY(WIDTH)
    D_A(BS_DIR) = (int_t) WTH_;
    BRANCH(BLS2)
}
int
DEPTH(retval)
    ret_t retval;
{
    ENTRY(DEPTH)
    D_A(BS_DIR) = (int_t) DTH_;
    BRANCH(BLS2)
}
static int
BLS2(retval)
    ret_t retval;
{
    ENTRY(BLS2)
    if (D_V(INCL) != 1)
	goto L_BLS3;
    SAVSTK();
    switch (BLOKVAL(BS_B)) {
    case 1:
	BRANCH(FAIL)
    }
    BRANCH(BLS1)
L_BLS3:
    if (D_V(INCL) != 0)
	BRANCH(ARGNER)
    D(BS_B) = D(NULVCL);
    BRANCH(BLS1)
}
int
BLOCKSIZ(retval)
    ret_t retval;
{
    ENTRY(BLOCKSIZ)
    if (D_V(INCL) != 2)
	BRANCH(ARGNER)
    SAVSTK();
    switch (BLOKVAL(BS_B)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(BS_B);
    SAVSTK();
    switch (INTVAL(BS_DIR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(BS_B);
    D_A(BS_DIR)++;
    D_A(BS_DIR) *= DESCR;
    D_F(BS_DIR) = D_V(BS_DIR) = 0;
    BRANCH(BLS1)
}
static int
BLS1(retval)
    ret_t retval;
{
    ENTRY(BLS1)
    SAVSTK();
    PUSH(BS_B);
    PUSH(BS_B);
    FIXINL(BS_B);
    D(BS_B) = D(D_A(BS_B) + FRAME_);
    D(BS_B) = D(D_A(BS_B) + D_A(BS_DIR));
    D_V(BS_B) = I;
    D(retval) = D(BS_B);
    RETURN(3)
}
static int
BLOKVAL(retval)
    ret_t retval;
{
    ENTRY(BLOKVAL)
    SAVSTK();
    switch (ARGVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(XPTR) == BL)
	BRANCH(RTXNAM)
    if (D_V(XPTR) == S)
	BRANCH(RTXNAM)
    if (D_V(XPTR) != R)
	goto L_BLOKV1;
    REALST(REALSP,XPTR);
    _SPEC(XSP) = _SPEC(REALSP);
    SAVSTK();
    PUSH(XSPPTR);
    switch (GENVAR(XPTR)) {
    case 1:
	BRANCH(RTXNAM)
    }
L_BLOKV1:
    if (D_V(XPTR) == I)
	goto L_BLOKVI;
    else
	BRANCH(INTR1)
L_BLOKVI:
    SAVSTK();
    PUSH(XPTR);
    switch (GNVARI(XPTR)) {
    case 1:
	BRANCH(RTXNAM)
    }
    /* FALL */
    BRANCH(FRONT)
}
int
FRONT(retval)
    ret_t retval;
{
    ENTRY(FRONT)
    BRANCH(BOX)
}
int
VER(retval)
    ret_t retval;
{
    ENTRY(VER)
    BRANCH(BOX)
}
int
HOR(retval)
    ret_t retval;
{
    ENTRY(HOR)
    SAVSTK();
    switch (INTVAL(BX_W)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_A(BX_W) < 0)
	BRANCH(FAIL)
    D(BX_H) = D(BX_ZERO);
    D(BX_D) = D(BX_ZERO);
    BRANCH(BOXIN)
}
int
BOX(retval)
    ret_t retval;
{
    ENTRY(BOX)
    D_A(TEMP1) = D_V(INCL);
    D_F(TEMP1) = D_V(TEMP1) = 0;
    PUSH(TEMP1);
    SAVSTK();
    switch (INTVAL(BX_H)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(TEMP1);
    if (D_A(BX_H) < 0)
	BRANCH(LENERR)
    D(BX_W) = D(BX_ZERO);
    if (D_A(TEMP1) <= 1)
	goto L_BOX2;
    PUSH(TEMP1);
    PUSH(BX_H);
    SAVSTK();
    switch (INTVAL(BX_W)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(BX_H);
    POP(TEMP1);
    if (D_A(BX_W) < 0)
	BRANCH(LENERR)
L_BOX2:
    D(BX_D) = D(BX_ZERO);
    if (D_A(TEMP1) <= 2)
	goto L_BOX3;
    PUSH(BX_H);
    PUSH(BX_W);
    SAVSTK();
    switch (INTVAL(BX_D)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(BX_W);
    POP(BX_H);
    if (D_A(BX_D) < 0)
	BRANCH(LENERR)
L_BOX3:
    BRANCH(BOXIN)
}
static int
BOXIN(retval)
    ret_t retval;
{
    ENTRY(BOXIN)
    SAVSTK();
    PUSH(BX_D);
    PUSH(BX_W);
    PUSH(BX_H);
    switch (BLAND(XPTR)) {
    case 1:
	goto L_BOXNL;
    }
L_BOX4:
    SAVSTK();
    PUSH(XPTR);
    PUSH(BX_D);
    PUSH(BX_W);
    PUSH(BX_H);
    PUSH(NULVCL);
    P_BLOCK(XPTR);
    BRANCH(RTXPTR)
L_BOXNL:
    D(XPTR) = D(NULVCL);
    goto L_BOX4;
}
static int
BTAIL(retval)
    ret_t retval;
{
    ENTRY(BTAIL)
L_BTAIL0:
    D_A(BT_SIZE) = D_V(D_A(WCL));
    D_F(BT_SIZE) = D_V(BT_SIZE) = 0;
    D(BT_TOP) = D(D_A(WCL) + TOP_);
    if (D_A(BT_TOP) >= D_A(BT_SIZE))
	goto L_BT_MORE;
    D_A(BT_TOP) += DESCR;
    D(D_A(WCL) + TOP_) = D(BT_TOP);
    D(retval) = D(BT_TOP);
    RETURN(1)
L_BT_MORE:
    SAVSTK();
    PUSH(WCL);
    MORE(WCL);
    goto L_BTAIL0;
}
static int
CAE(retval)
    ret_t retval;
{
    ENTRY(CAE)
    POP(TEMP35);
    if (D_V(TEMP35) != AEDGDT)
	goto L_CA99;
    SAVSTK();
    PUSH(TEMP35);
    BCOPY(TEMP35);
    D(TEMP36) = D(D_A(TEMP35) + NEXT_);
    PUSH(TEMP35);
    SAVSTK();
    PUSH(TEMP36);
    CAE(TEMP36);
    POP(TEMP35);
    D(D_A(TEMP35) + NEXT_) = D(TEMP36);
L_CA99:
    D(retval) = D(TEMP35);
    RETURN(1)
}
int
BCHAR(retval)
    ret_t retval;
{
    ENTRY(BCHAR)
    if (D_V(INCL) != 1)
	goto L_CH1;
    SAVSTK();
    switch (BLOKVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    SAVSTK();
    PUSH(ZPTR);
    PUSH(ZPTR);
    FIXINL(ZPTR);
    D(CH_F) = D(D_A(ZPTR) + FRAME_);
    D(TEMP1) = D(D_A(ZPTR) + ARRAY_);
    D(CH_H) = D(D_A(CH_F) + HT_);
    D(CH_D) = D(D_A(CH_F) + DTH_);
    D_V(CH_H1) = D_A(CH_H);
    D_V(CH_D1) = D_A(CH_D);
    D(TEMP3) = D(CH_H);
    D_A(TEMP3) *= D_A(CH_D);
    D_A(TEMP2) = D_A(TEMP3);
    D_A(TEMP2) *= DESCR;
    D_F(TEMP2) = D_V(TEMP2) = 0;
    D_A(CH_LG) = D_A(TEMP2);
    D_A(TEMP2) += D_A(CH_DL);
    D_V(TEMP2) = A;
    SAVSTK();
    PUSH(TEMP2);
    BLOCK(YPTR);
    _SPEC(TSP) = _SPEC(CH_SPEC);
    S_L(TSP) = D_A(ZEROCL);
    INTSPC(XSP,CH_D);
    APDSP(TSP,XSP);
    APDSP(TSP,CMASP);
    INTSPC(XSP,CH_H);
    APDSP(TSP,XSP);
    SAVSTK();
    PUSH(TSPPTR);
    GENVAR(CH_PR);
    MOVBLK(D_A(YPTR),D_A(CH_DOPE),D_A(CH_DL));
    D(TPTR) = D(YPTR);
    D_A(TPTR) += D_A(CH_DL);
    if (D_A(TEMP1) == 0)
	goto L_CH2;
    MOVBLK(D_A(TPTR),D_A(TEMP1),D_A(CH_LG));
L_CH3:
    BRANCH(RTYPTR)
L_CH1:
    if (D_V(INCL) == 0)
	BRANCH(RETNUL)
    else
	BRANCH(ARGNER)
L_CH2:
    if (D_A(CH_LG) == 0)
	goto L_CH3;
    D(TEMP4) = D(CH_LG);
    D_A(TEMP4) -= DESCR;
    D(D_A(TPTR) + DESCR) = D(NULVCL);
    D(TEMP5) = D(TPTR);
    D_A(TEMP5) += DESCR;
    if (D_A(TEMP4) == 0)
	goto L_CH3;
    MOVBLK(D_A(TEMP5),D_A(TPTR),D_A(TEMP4));
    goto L_CH3;
}
static int
CIR(retval)
    ret_t retval;
{
    ENTRY(CIR)
    POP(CI_A1);
    POP(CI_N1);
    POP(CI_A2);
    POP(CI_N2);
    POP(CI_D);
    if (D_A(CI_N2) != 0)
	goto L_CIR11;
    D(CI_CI) = D(CI_A1);
    goto L_CI99;
L_CIR11:
    if (D_A(CI_N1) != 0)
	goto L_CIR12;
    if (D_A(CI_D) != 0)
	goto L_CIR12;
    D(CI_CI) = D(CI_A2);
    goto L_CI99;
L_CIR12:
    D_A(CI_N1) *= DESCR;
    D_F(CI_N1) = D_V(CI_N1) = 0;
    D_A(CI_N2) *= DESCR;
    D_F(CI_N2) = D_V(CI_N2) = 0;
    D(TEMP31) = D(CI_N1);
    D_A(TEMP31) += D_A(CI_N2);
    PUSH(CI_A1);
    PUSH(CI_A2);
    D_V(TEMP31) = A;
    SAVSTK();
    PUSH(TEMP31);
    BLOCK(CI_CI);
    POP(CI_A2);
    POP(CI_A1);
    MOVBLK(D_A(CI_CI),D_A(CI_A1),D_A(CI_N1));
    D_A(CI_J) = 0;
    D(TEMP31) = D(CI_CI);
    D_A(TEMP31) += D_A(CI_N1);
L_CIR1:
    if (D_A(CI_J) >= D_A(CI_N2))
	goto L_CI99;
    D_A(CI_J) += DESCR;
    D(TEMP32) = D(D_A(CI_A2) + D_A(CI_J));
    D_A(TEMP33) = D_V(TEMP32);
    D_F(TEMP33) = D_V(TEMP33) = 0;
    D_A(TEMP33) += D_A(CI_D);
    D_V(TEMP32) = D_A(TEMP33);
    D(D_A(TEMP31) + D_A(CI_J)) = D(TEMP32);
    goto L_CIR1;
L_CI99:
    D(retval) = D(CI_CI);
    RETURN(1)
}
static int
CLASS(retval)
    ret_t retval;
{
    ENTRY(CLASS)
    POP(CL_B);
    if (D_V(CL_B) != S)
	goto L_CLASS1;
    D_A(CL_CLASS) = 0;
L_CLASS2:
    D(retval) = D(CL_CLASS);
    RETURN(1)
L_CLASS1:
    D_A(CL_CLASS) = 1;
    D(CL_REG) = D(D_A(CL_B) + SET_);
    if (D_V(CL_REG) != 0)
	goto L_CLASS3;
    D(CL_B) = D(D_A(CL_B) + ORG_);
    if (D_V(CL_B) == D_V(TEMP1))
	goto L_CLASS2;
L_CLASS3:
    D_A(CL_CLASS)++;
    goto L_CLASS2;
}
int
COAG(retval)
    ret_t retval;
{
    ENTRY(COAG)
    POP(TEMP45);
    POP(TEMP46);
    D(TEMP47) = D(TEMP45);
    D_A(CO_HIT) = 0;
L_COAG2:
    D(TEMP31) = D(D_A(TEMP45) + VAL_);
    SAVSTK();
    PUSH(TEMP46);
    PUSH(TEMP31);
    switch (MINGLE(TEMP46)) {
    case 1:
	goto L_COAG1;
    }
    D_A(CO_HIT) = 1;
    D(D_A(TEMP45) + VAL_) = D(ZEROCL);
L_COAG1:
    if (D_A(D_A(TEMP45) + NEXT_) == (int_t)0)
	goto L_COAG11;
    D(TEMP45) = D(D_A(TEMP45) + NEXT_);
    goto L_COAG2;
L_COAG11:
    if (D_A(CO_HIT) != 0)
	goto L_COAG12;
    SAVSTK();
    PUSH(CO_DE);
    BLOCK(TEMP31);
    D(D_A(TEMP31) + VAL_) = D(TEMP46);
    D(D_A(TEMP45) + NEXT_) = D(TEMP31);
    goto L_COAG99;
L_COAG12:
    D(TEMP45) = D(TEMP47);
L_COAG3:
    if (D_A(D_A(TEMP45) + VAL_) == (int_t)0)
	goto L_COAG31;
    D(TEMP45) = D(D_A(TEMP45) + NEXT_);
    goto L_COAG3;
L_COAG31:
    D(D_A(TEMP45) + VAL_) = D(TEMP46);
L_COAG4:
    if (D_A(D_A(TEMP45) + NEXT_) == (int_t)0)
	goto L_COAG99;
    D(TEMP31) = D(D_A(TEMP45) + NEXT_);
    if (D_A(D_A(TEMP31) + VAL_) != (int_t)0)
	goto L_COAG41;
    D(TEMP31) = D(D_A(TEMP31) + NEXT_);
    D(D_A(TEMP45) + NEXT_) = D(TEMP31);
    goto L_COAG4;
L_COAG41:
    D(TEMP45) = D(D_A(TEMP45) + NEXT_);
    goto L_COAG4;
L_COAG99:
    BRANCH(RTN1)
}
static int
COMPFR(retval)
    ret_t retval;
{
    ENTRY(COMPFR)
    POP(CF_AF);
    D_A(CF_DIR) = (int_t) HT_;
L_CF2:
    D(CF_E) = D(D_A(CF_AF) + D_A(CF_DIR));
    if (D_V(CF_E) != AEDGDT)
	goto L_CF1;
    if (D_A(D_A(CF_E) + NEXT_) != (int_t)0)
	goto L_CF3;
    D(CF_E) = D(D_A(CF_E) + VAL_);
    D(D_A(CF_AF) + D_A(CF_DIR)) = D(CF_E);
    goto L_CF1;
L_CF3:
    io_printf(D_A(CF_UNIT),CF_FMT);
    SAVSTK();
    WARNING(NORET);
    D(TEMP31) = D(D_A(CF_E) + VAL_);
L_CF5:
    D(CF_E) = D(D_A(CF_E) + NEXT_);
    if (D_A(CF_E) == 0)
	goto L_CF4;
    D(TEMP32) = D(D_A(CF_E) + VAL_);
    PUSH(CF_AF);
    PUSH(CF_E);
    SAVSTK();
    PUSH(TEMP32);
    PUSH(TEMP31);
    JE_LONGI(TEMP31);
    POP(CF_E);
    POP(CF_AF);
    goto L_CF5;
L_CF4:
    D(D_A(CF_AF) + D_A(CF_DIR)) = D(TEMP31);
L_CF1:
    D_A(CF_DIR) += DESCR;
    if (D_A(CF_DIR) <= DTH_)
	goto L_CF2;
    else
	BRANCH(RTN1)
}
static int
DISTR(retval)
    ret_t retval;
{
    ENTRY(DISTR)
    POP(DI_A);
    POP(DI_N);
    D_V(DI_N) = A;
    SAVSTK();
    PUSH(DI_N);
    BLOCK(DI_DI);
L_DI1:
    if (D_A(DI_N) <= 0)
	goto L_DI99;
    if (D_A(DI_DSC) == 0)
	BRANCH(FAIL)
    CLR_MATH_ERROR();
    D(DI_M) = D(DI_N);
    D_A(DI_M) /= D_A(DI_DSC);
    if (MATH_ERROR())
	BRANCH(FAIL)
    if (D_A(DI_M) == 0)
	BRANCH(FAIL)
    CLR_MATH_ERROR();
    D(DI_V) = D(DI_A);
    D_A(DI_V) /= D_A(DI_M);
    if (MATH_ERROR())
	BRANCH(FAIL)
    D(D_A(DI_DI) + D_A(DI_N)) = D(DI_V);
    D_A(DI_A) -= D_A(DI_V);
    D_A(DI_N) -= DESCR;
    goto L_DI1;
L_DI99:
    D(retval) = D(DI_DI);
    RETURN(1)
}
static int
DUMP_B(retval)
    ret_t retval;
{
    ENTRY(DUMP_B)
    io_printf(D_A(DP_UNIT),DP_FT1);
    POP(DP_BLOCK);
    D_A(DP_TEMP) = D_A(D_A(DP_BLOCK));
    if (D_A(DP_TEMP) != D_A(DP_BLOCK))
	goto L_DP_NOBLK;
    D_A(DP_I) = 0;
    D_A(DP_SIZE) = D_V(D_A(DP_BLOCK));
    D_F(DP_SIZE) = D_V(DP_SIZE) = 0;
L_DP_LOOP:
    D(DP_TEMP) = D(D_A(DP_BLOCK) + D_A(DP_I));
    io_printf(D_A(DP_UNIT),DP_FT2,DP_TEMP,DP_TEMP,DP_TEMP);
    if (D_A(DP_I) >= D_A(DP_SIZE))
	goto L_DP_END;
    D_A(DP_I) += DESCR;
    goto L_DP_LOOP;
L_DP_NOBLK:
L_DP_END:
    RETURN(1)
}
static int
DUMP_A(retval)
    ret_t retval;
{
    ENTRY(DUMP_A)
    POP(DP_BLOCK);
    if (D_A(DP_BLOCK) != 0)
	goto L_DP1;
    io_printf(D_A(DP_UNIT),DP_FT3,ZEROCL);
    BRANCH(RTN1)
L_DP1:
    D_A(DP_SIZE) = D_V(D_A(DP_BLOCK));
    D_F(DP_SIZE) = D_V(DP_SIZE) = 0;
    if (D_A(DP_8) == 0)
	BRANCH(FAIL)
    CLR_MATH_ERROR();
    D(DP_TEMP) = D(DP_SIZE);
    D_A(DP_TEMP) /= D_A(DP_8);
    if (MATH_ERROR())
	BRANCH(FAIL)
    io_printf(D_A(DP_UNIT),DP_FT3,DP_TEMP);
    D_A(DP_I) = (int_t) DESCR;
L_DP_LP:
    D(DP_TEMP) = D(D_A(DP_BLOCK) + D_A(DP_I));
    X_LOCSP(YSP,DP_TEMP);
    FASTPR(IOKEY,DP_UNIT,DP_UNIT,YSP,BLSP);
    D_A(DP_I) += DESCR;
    if (D_A(DP_I) <= D_A(DP_SIZE))
	goto L_DP_LP;
    else
	BRANCH(RTN1)
}
int
DUP(retval)
    ret_t retval;
{
    ENTRY(DUP)
    D_A(DUP_NA) = D_V(INCL);
    D_F(DUP_NA) = D_V(DUP_NA) = 0;
    if (D_A(DUP_NA) != 3)
	BRANCH(ARGNER)
    SAVSTK();
    switch (BLOKVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(XPTR) == S)
	goto L_DUP2;
    D(TEMP39) = D(D_A(XPTR) + ORG_);
    if (D_V(TEMP39) != PHY_)
	goto L_DUP2;
    SAVSTK();
    PUSH(XPTR);
    BCOPY(XPTR);
L_DUP2:
    PUSH(XPTR);
    SAVSTK();
    switch (INTVAL(DUP_DIR)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(DUP_DIR);
    SAVSTK();
    switch (INTVAL(DUP_N)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(DUP_DIR);
    POP(XPTR);
    if (D_A(DUP_N) < 0)
	BRANCH(FAIL)
    if (D_V(XPTR) != S)
	goto L_DUP1;
    if (D_A(DUP_DIR) != 1)
	goto L_DUP1;
    SAVSTK();
    PUSH(DUP_N);
    PUSH(XPTR);
    switch (DUPE(XPTR)) {
    case 1:
	BRANCH(RTXPTR)
    }
L_DUP1:
    SAVSTK();
    PUSH(XPTR);
    PUSH(XPTR);
    FIXINL(XPTR);
    D_A(DUP_DIR)++;
    D_A(DUP_DIR) *= DESCR;
    D_F(DUP_DIR) = D_V(DUP_DIR) = 0;
    D(TEMP1) = D(D_A(XPTR) + ARRAY_);
    D(TEMP2) = D(D_A(XPTR) + FRAME_);
    PUSH(XPTR);
    SAVSTK();
    PUSH(DUP_N);
    PUSH(DUP_DIR);
    PUSH(TEMP2);
    PUSH(TEMP1);
    REPL(TEMP1);
    POP(XPTR);
    D(D_A(XPTR) + ARRAY_) = D(TEMP1);
    BRANCH(RTXPTR)
}
static int
DUPE(retval)
    ret_t retval;
{
    ENTRY(DUPE)
    POP(YPTR);
    POP(DU_N);
    if (D_A(DU_N) == 0)
	BRANCH(RT1NUL)
    X_LOCSP(YSP,YPTR);
    D_A(YCL) = S_L(YSP);
    D_F(YCL) = D_V(YCL) = 0;
    D_A(YCL) *= D_A(DU_N);
    if (D_A(YCL) > D_A(MLENCL))
	BRANCH(INTR8)
    SAVSTK();
    PUSH(YCL);
    CONVAR(ZPTR);
    X_LOCSP(TSP,ZPTR);
    S_L(TSP) = 0;
L_DUPE2:
    if (D_A(DU_N) <= 0)
	goto L_DUPE1;
    D_A(DU_N)--;
    APDSP(TSP,YSP);
    goto L_DUPE2;
L_DUPE1:
    SAVSTK();
    PUSH(YCL);
    GNVARS(ZPTR);
    D(retval) = D(ZPTR);
    RETURN(1)
}
static int
E_ATTACH(retval)
    ret_t retval;
{
    ENTRY(E_ATTACH)
    POP(EA_AE);
    POP(EA_E);
    if (D_V(EA_AE) == AEDGDT)
	goto L_EA1;
    PUSH(EA_AE);
    PUSH(EA_E);
    SAVSTK();
    PUSH(EA_DE);
    BLOCK(EA_T);
    POP(EA_E);
    POP(EA_AE);
    D(D_A(EA_T) + VAL_) = D(EA_AE);
    D(EA_AE) = D(EA_T);
L_EA1:
    D(EA_T) = D(EA_AE);
L_EA2:
    if (D_A(D_A(EA_T) + NEXT_) == (int_t)0)
	goto L_EA3;
    D(EA_T) = D(D_A(EA_T) + NEXT_);
    goto L_EA2;
L_EA3:
    PUSH(EA_AE);
    PUSH(EA_T);
    PUSH(EA_E);
    SAVSTK();
    PUSH(EA_DE);
    BLOCK(TEMP31);
    POP(EA_E);
    POP(EA_T);
    POP(EA_AE);
    D(D_A(EA_T) + NEXT_) = D(TEMP31);
    D(D_A(TEMP31) + VAL_) = D(EA_E);
    D(retval) = D(EA_AE);
    RETURN(1)
}
static int
EMB_PHY(retval)
    ret_t retval;
{
    ENTRY(EMB_PHY)
    POP(XPTR);
    D(TEMP31) = D(D_A(XPTR) + VALUE_);
    if (D_V(TEMP31) == S)
	goto L_EMERR;
    D(TEMP39) = D(D_A(TEMP31) + ORG_);
    if (D_V(TEMP39) != PHY_)
	goto L_EMERR;
    D(TEMP1) = D(D_A(TEMP31) + BL_);
    D(TEMP3) = D(D_A(TEMP31) + FRAME_);
    SAVSTK();
    PUSH(TEMP3);
    BCOPY(TEMP5);
    D(TEMP32) = D(D_A(XPTR) + FATHER_);
    D(TEMP33) = D(D_A(TEMP32) + VALUE_);
    D(TEMP18) = D(D_A(TEMP33) + FRAME_);
    D(TEMP10) = D(D_A(TEMP33) + ARRAY_);
    if (D_A(TEMP1) == 0)
	goto L_EM4;
    D(EM_DIR) = D(D_A(TEMP1) + ORG_);
    D_A(EM_DIR) = D_V(EM_DIR);
    D_F(EM_DIR) = D_V(EM_DIR) = 0;
    D_A(EM_DIR) *= DESCR;
    D_F(EM_DIR) = D_V(EM_DIR) = 0;
    D(TEMP4) = D(D_A(TEMP3) + D_A(EM_DIR));
    if (D_V(TEMP4) != AEDGDT)
	goto L_EM11;
    D(TEMP31) = D(D_A(TEMP4) + VAL_);
    D(D_A(TEMP5) + D_A(EM_DIR)) = D(TEMP31);
L_EM11:
    SAVSTK();
    PUSH(TEMP18);
    STRIP_F(TEMP9);
    SAVSTK();
    PUSH(TEMP5);
    PUSH(TEMP18);
    switch (SUBBLOCK(TEMP11)) {
    case 1:
	goto L_EMERR1;
    }
    if (D_V(TEMP1) == S)
	goto L_EM3;
    D(TEMP39) = D(D_A(TEMP1) + ORG_);
    D_A(TEMP39) = D_V(TEMP39);
    D_F(TEMP39) = D_V(TEMP39) = 0;
    if (D_A(TEMP39) > OVY_)
	goto L_EM3;
    SAVSTK();
    PUSH(TEMP11);
    BCOPY(TEMP20);
    D(TEMP2) = D(D_A(XPTR) + TNREG_);
    D(TEMP8) = D(D_A(XPTR) + LSO_);
L_EM1:
    D(TEMP31) = D(D_A(TEMP8) + VALUE_);
    D(TEMP32) = D(D_A(TEMP31) + FRAME_);
    SAVSTK();
    PUSH(TEMP32);
    STRIP_F(TEMP21);
    D(TEMP22) = D(D_A(TEMP31) + ARRAY_);
    SAVSTK();
    PUSH(TEMP2);
    PUSH(EM_DIR);
    PUSH(TEMP21);
    PUSH(TEMP11);
    INIT_SUB(TEMP15);
    SAVSTK();
    PUSH(TEMP15);
    PUSH(TEMP22);
    PUSH(TEMP9);
    PUSH(TEMP10);
    INSERT(NORET);
    D(TEMP8) = D(D_A(TEMP8) + RSIB_);
    if (D_A(TEMP8) == 0)
	goto L_EM99;
    D(TEMP31) = D(D_A(TEMP8) + VALUE_);
    if (D_V(TEMP31) == S)
	goto L_EM1;
    D(TEMP39) = D(D_A(TEMP31) + ORG_);
    if (D_V(TEMP39) != REP_)
	goto L_EM1;
    D(TEMP4) = D(D_A(TEMP4) + NEXT_);
    D(TEMP32) = D(D_A(TEMP4) + VAL_);
    D(D_A(TEMP5) + D_A(EM_DIR)) = D(TEMP32);
    SAVSTK();
    PUSH(TEMP5);
    PUSH(TEMP18);
    switch (SUBBLOCK(TEMP23)) {
    case 1:
	goto L_EMERR1;
    }
    SAVSTK();
    PUSH(EM_DIR);
    PUSH(TEMP23);
    PUSH(TEMP20);
    switch (MIDREG(TEMP11)) {
    case 2:
	goto L_EM2;
    }
    SAVSTK();
    PUSH(EM_DIR);
    PUSH(TEMP20);
    PUSH(TEMP23);
    switch (MIDREG(TEMP11)) {
    case 2:
	goto L_EM2;
    }
    D(TEMP11) = D(TEMP23);
    SAVSTK();
    PUSH(TEMP11);
    BCOPY(TEMP20);
    D(TEMP8) = D(D_A(TEMP8) + RSIB_);
    goto L_EM1;
L_EM2:
    D(TEMP31) = D(D_A(TEMP11) + D_A(EM_DIR));
    D_A(EM_A) = D_V(TEMP31);
    D_F(EM_A) = D_V(EM_A) = 0;
    PUSH(EM_DIR);
    PUSH(TEMP2);
    PUSH(TEMP4);
    PUSH(TEMP5);
    PUSH(TEMP8);
    PUSH(TEMP9);
    PUSH(TEMP10);
    PUSH(TEMP11);
    PUSH(TEMP18);
    PUSH(TEMP21);
    PUSH(TEMP23);
    SAVSTK();
    PUSH(EM_A);
    PUSH(TEMP8);
    B_PB(TEMP24);
    POP(TEMP23);
    POP(TEMP21);
    POP(TEMP18);
    POP(TEMP11);
    POP(TEMP10);
    POP(TEMP9);
    POP(TEMP8);
    POP(TEMP5);
    POP(TEMP4);
    POP(TEMP2);
    POP(EM_DIR);
    D(TEMP14) = D(D_A(TEMP24) + FRAME_);
    SAVSTK();
    PUSH(TEMP2);
    PUSH(EM_DIR);
    PUSH(TEMP14);
    PUSH(TEMP11);
    INIT_SUB(TEMP15);
    D(TEMP31) = D(D_A(TEMP24) + ARRAY_);
    SAVSTK();
    PUSH(TEMP15);
    PUSH(TEMP31);
    PUSH(TEMP9);
    PUSH(TEMP10);
    INSERT(NORET);
    D(TEMP11) = D(TEMP23);
    SAVSTK();
    PUSH(TEMP11);
    BCOPY(TEMP20);
    D(TEMP8) = D(D_A(TEMP8) + RSIB_);
    goto L_EM1;
L_EM4:
    SAVSTK();
    PUSH(TEMP18);
    STRIP_F(TEMP9);
    SAVSTK();
    PUSH(TEMP5);
    PUSH(TEMP18);
    switch (SUBBLOCK(TEMP11)) {
    case 1:
	goto L_EMERR1;
    }
L_EM3:
    D(TEMP1) = D(D_A(XPTR) + VALUE_);
    D(TEMP31) = D(D_A(TEMP1) + ARRAY_);
    SAVSTK();
    PUSH(TEMP11);
    PUSH(TEMP31);
    PUSH(TEMP9);
    PUSH(TEMP10);
    INSERT(NORET);
    goto L_EM99;
L_EMERR1:
    io_printf(D_A(EM_DIAG),EM_FMT2);
    SAVSTK();
    WARNING(NORET);
    goto L_EM99;
L_EMERR:
    io_printf(D_A(EM_DIAG),EM_FMT3);
    SAVSTK();
    WARNING(NORET);
    goto L_EM99;
L_EM99:
    RETURN(1)
}
static int
FICOM(retval)
    ret_t retval;
{
    ENTRY(FICOM)
    POP(FC_A1);
    POP(FC_N1);
    POP(FC_A2);
    POP(FC_N2);
    D(TEMP31) = D(ZEROCL);
    D(TEMP32) = D(ZEROCL);
    D_A(FC_N1) *= DESCR;
    D_F(FC_N1) = D_V(FC_N1) = 0;
    D_A(FC_N2) *= DESCR;
    D_F(FC_N2) = D_V(FC_N2) = 0;
L_FICOM2:
    D_A(TEMP31) += DESCR;
    if (D_A(TEMP31) > D_A(FC_N1))
	BRANCH(FAIL)
    D_A(TEMP32) = 0;
    D(FC_ND1) = D(D_A(FC_A1) + D_A(TEMP31));
L_FICOM1:
    D_A(TEMP32) += DESCR;
    if (D_A(TEMP32) > D_A(FC_N2))
	goto L_FICOM2;
    D(FC_ND2) = D(D_A(FC_A2) + D_A(TEMP32));
    if (D_A(FC_ND1) == D_A(FC_ND2))
	BRANCH(RTN2)
    else
	goto L_FICOM1;
}
int
FIX(retval)
    ret_t retval;
{
    ENTRY(FIX)
    PUSH(INCL);
    SAVSTK();
    switch (BLOKVAL(TEMP1)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(INCL);
    D(TEMP2) = D(NULVCL);
    if (D_V(INCL) == 1)
	goto L_FIXCL;
    PUSH(INCL);
    PUSH(TEMP1);
    SAVSTK();
    switch (ARGVAL(TEMP2)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(TEMP1);
    POP(INCL);
    if (D_A(TEMP2) != 1)
	goto L_FIXNDS;
L_FIXCL:
    SAVSTK();
    PUSH(TEMP2);
    PUSH(TEMP1);
    switch (FIXINL(ZPTR)) {
    case 1:
	BRANCH(RTZPTR)
    }
L_FIXNDS:
    PUSH(TEMP2);
    PUSH(INCL);
    SAVSTK();
    PUSH(NULVCL);
    PUSH(TEMP1);
    FIXINL(ZPTR);
    POP(INCL);
    POP(TEMP2);
    D_A(FX_ARGS) = D_V(INCL);
    D_F(FX_ARGS) = D_V(FX_ARGS) = 0;
    D_A(FX_ARGS) -= 2;
    D(TEMP10) = D(D_A(ZPTR) + FRAME_);
    SAVSTK();
    PUSH(TEMP10);
    STRIP_F(TEMP20);
    BRANCH(FIX1)
}
static int
FIX2(retval)
    ret_t retval;
{
    ENTRY(FIX2)
    if (D_A(FX_ARGS) <= 0)
	BRANCH(FIX99)
    D_A(FX_ARGS)--;
    PUSH(FX_ARGS);
    PUSH(TEMP10);
    PUSH(TEMP20);
    PUSH(ZPTR);
    SAVSTK();
    switch (BLOKVAL(TEMP2)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(ZPTR);
    POP(TEMP20);
    POP(TEMP10);
    POP(FX_ARGS);
    /* FALL */
    BRANCH(FIX1)
}
static int
FIX1(retval)
    ret_t retval;
{
    ENTRY(FIX1)
    D(FX_ND) = D(D_A(TEMP2) + ID_);
    D_A(FX_DIR) = (int_t) HT_;
L_FIX11:
    D(TEMP11) = D(D_A(TEMP10) + D_A(FX_DIR));
    if (D_V(TEMP11) == EDGDT)
	D(TEMP12) = D(D_A(TEMP11) + RELS1_);
    else
	D(TEMP12) = D(NULVCL);
    if (D_V(TEMP11) == EDGDT)
	D(TEMP13) = D(D_A(TEMP11) + NN1_);
    else
	D(TEMP13) = D(ZEROCL);
    SAVSTK();
    PUSH(TEMP13);
    PUSH(TEMP12);
    PUSH(FX_N1);
    PUSH(FX_A1);
    switch (FICOM(NORET)) {
    case 1:
	BRANCH(FIX2)
    }
    D(TEMP14) = D(D_A(TEMP12) + D_A(TEMP32));
    D_A(TEMP13) = D_V(TEMP14);
    D_F(TEMP13) = D_V(TEMP13) = 0;
    D(TEMP21) = D(D_A(TEMP20) + D_A(FX_DIR));
    SAVSTK();
    PUSH(TEMP13);
    PUSH(TEMP14);
    PUSH(TEMP21);
    ADD_NP(TEMP21);
    D(D_A(TEMP20) + D_A(FX_DIR)) = D(TEMP21);
    D_A(FX_DIR) += DESCR;
    if (D_A(FX_DIR) <= DTH_)
	goto L_FIX11;
    else
	BRANCH(FIX2)
}
static int
FIX99(retval)
    ret_t retval;
{
    ENTRY(FIX99)
    D(D_A(ZPTR) + FRAME_) = D(TEMP20);
    BRANCH(RTZPTR)
}
static int
FIXINL(retval)
    ret_t retval;
{
    ENTRY(FIXINL)
    POP(FIX_B);
    POP(FIX_FLAG);
    if (D_V(FIX_B) == S)
	goto L_T5;
    D(TEMP39) = D(D_A(FIX_B) + ORG_);
    if (D_V(TEMP39) == PHY_)
	goto L_FIXI1;
L_T5:
    D(TEMP50) = D(FX_NN);
    SAVSTK();
    PUSH(FIX_B);
    PUSH(NULVCL);
    PUSH(NULVCL);
    T_LEAF(FIX_B);
    SAVSTK();
    PUSH(FIX_B);
    B_PB(FIX_B);
    D(D_A(FIX_B) + BL_) = D(NULVCL);
    D(TEMP50) = D(FX_NN);
L_FIXI1:
    SAVSTK();
    PUSH(FIX_B);
    BCOPY(FIX_B);
    if (D_A(FIX_FLAG) == 0)
	goto L_FIXI99;
    D(TEMP31) = D(D_A(FIX_B) + FRAME_);
    PUSH(FIX_B);
    SAVSTK();
    PUSH(TEMP31);
    STRIP_F(TEMP31);
    POP(FIX_B);
    D(D_A(FIX_B) + FRAME_) = D(TEMP31);
L_FIXI99:
    D(retval) = D(FIX_B);
    RETURN(1)
}
static int
FORCING(retval)
    ret_t retval;
{
    ENTRY(FORCING)
    POP(FO_TN);
    POP(FO_DIR);
    D(FO_FO) = D(FO_ZERO);
    D(FO_B) = D(D_A(FO_TN) + VALUE_);
    if (D_V(FO_B) != S)
	goto L_FO1;
    if (D_A(FO_B) == 0)
	goto L_FO99;
    if (D_A(FO_DIR) == WTH_)
	goto L_FO2;
    D_A(FO_FO) = 1;
    goto L_FO99;
L_FO2:
    D_A(FO_FO) = D_V(D_A(FO_B));
    D_F(FO_FO) = D_V(FO_FO) = 0;
    D_V(FO_FO) = I;
    goto L_FO99;
L_FO1:
    D(TEMP1) = D(D_A(FO_B) + ORG_);
    D_A(TEMP1) = D_V(TEMP1);
    D_F(TEMP1) = D_V(TEMP1) = 0;
    switch (D_A(TEMP1)) {
    case 1:
	goto L_FORCE1;
    case 2:
	goto L_FORCE2;
    case 3:
	goto L_FORCE3;
    case 4:
	goto L_FORCE4;
    case 5:
	goto L_FORCE5;
    case 6:
	goto L_FORCE6;
    case 7:
	goto L_FORCE7;
    case 8:
	goto L_FORCE8;
    case 9:
	goto L_FORCE9;
    }
L_FORCE1:
L_FORCE2:
L_FORCE3:
    PUSH(FO_TN);
    SAVSTK();
    PUSH(FO_TN);
    LSOHN(FO_P);
    POP(FO_TN);
    D_A(FO_SW) = 1;
    D_A(TEMP1) *= DESCR;
    D_F(TEMP1) = D_V(TEMP1) = 0;
    if (D_A(TEMP1) != D_A(FO_DIR))
	goto L_FO3;
    D_A(FO_SW) = 2;
L_FO3:
    goto L_FO4;
L_FO31:
    D(FO_P) = D(D_A(FO_P) + RSIB_);
L_FO4:
    if (D_A(FO_P) == 0)
	goto L_FO99;
    PUSH(FO_P);
    PUSH(FO_DIR);
    PUSH(FO_SW);
    PUSH(FO_FO);
    SAVSTK();
    PUSH(FO_DIR);
    PUSH(FO_P);
    FORCING(TEMP1);
    POP(FO_FO);
    POP(FO_SW);
    POP(FO_DIR);
    POP(FO_P);
    switch (D_A(FO_SW)) {
    case 1:
	goto L_FO_ORTH;
    case 2:
	goto L_FO_LONGI;
    }
L_FO_ORTH:
    if (D_A(FO_FO) >= D_A(TEMP1))
	goto L_FO31;
    D_A(FO_FO) = D_A(TEMP1);
    goto L_FO31;
L_FO_LONGI:
    D_A(FO_FO) += D_A(TEMP1);
    goto L_FO31;
L_FORCE4:
    PUSH(FO_DIR);
    SAVSTK();
    PUSH(NULVCL);
    PUSH(FO_TN);
    B_PB(FO_B);
    POP(FO_DIR);
    D(FO_B) = D(D_A(FO_B) + FRAME_);
    D(FO_E) = D(D_A(FO_B) + D_A(FO_DIR));
    if (D_V(FO_E) == EDGDT)
	D(FO_FO) = D(D_A(FO_E) + TR1_);
    else
	D(FO_FO) = D(FO_E);
    goto L_FO99;
L_FORCE5:
L_FORCE6:
    SAVSTK();
    PUSH(FO_TN);
    switch (PAR_CONG(FO_P)) {
    case 1:
	goto L_FO99;
    }
    D(FO_P) = D(D_A(FO_P) + VALUE_);
    D(FO_P) = D(D_A(FO_P) + ORG_);
    D_A(FO_P) = D_V(FO_P);
    D_F(FO_P) = D_V(FO_P) = 0;
    D_A(FO_P) *= DESCR;
    D_F(FO_P) = D_V(FO_P) = 0;
    if (D_A(FO_P) != D_A(FO_DIR))
	goto L_FORCE52;
    if (D_A(TEMP1) == REP_)
	goto L_FO99;
    else
	goto L_FORCE51;
L_FORCE52:
    if (D_A(TEMP1) == IT_)
	goto L_FO99;
    else
	goto L_FORCE51;
L_FORCE51:
    SAVSTK();
    PUSH(FO_TN);
    LSOHN(FO_TN);
    SAVSTK();
    PUSH(FO_DIR);
    PUSH(FO_TN);
    switch (FORCING(FO_FO)) {
    case 1:
	goto L_FO99;
    }
L_FORCE7:
    goto L_FORCE51;
L_FORCE8:
    D_A(ERRTYP) = (int_t) (ERRNO+1);
    BRANCH(FTLEND)
L_FORCE9:
    D(FO_P) = D(D_A(FO_B) + FRAME_);
    D(FO_P) = D(D_A(FO_P) + D_A(FO_DIR));
    if (D_V(FO_P) == AEDGEDT)
	goto L_FORCE91;
    if (D_V(FO_P) == EDGDT)
	D(FO_FO) = D(D_A(FO_P) + TR1_);
    else
	D(FO_FO) = D(FO_P);
    goto L_FO99;
L_FORCE91:
    D(FO_E) = D(D_A(FO_P) + VAL_);
    if (D_V(FO_E) == EDGDT)
	D(TEMP1) = D(D_A(FO_E) + TR1_);
    else
	D(TEMP1) = D(FO_E);
    D_A(FO_FO) += D_A(TEMP1);
    D(FO_P) = D(D_A(FO_P) + NEXT_);
    if (D_A(FO_P) == 0)
	goto L_FO99;
    else
	goto L_FORCE91;
L_FO99:
    D(retval) = D(FO_FO);
    RETURN(1)
}
static int
F_JOIN(retval)
    ret_t retval;
{
    ENTRY(F_JOIN)
    POP(FJ_DIR);
    POP(FJ_F1);
    POP(FJ_F2);
    POP(FJ_REG);
    if (DCMP(FJ_F2, NULVCL))
	goto L_FJ99;
    D_A(FJ_I) = (int_t) HT_;
L_FJ6:
    D(FJ_T1) = D(D_A(FJ_F1) + D_A(FJ_I));
    D(FJ_T2) = D(D_A(FJ_F2) + D_A(FJ_I));
    D(FJ_RI) = D(D_A(FJ_REG) + D_A(FJ_I));
    PUSH(FJ_F1);
    PUSH(FJ_F2);
    PUSH(FJ_REG);
    if (D_A(FJ_I) != D_A(FJ_DIR))
	goto L_FJ3;
    SAVSTK();
    PUSH(FJ_T2);
    PUSH(FJ_T1);
    switch (JE_LONGI(FJ_T)) {
    case 1:
	goto L_FJ4;
    }
L_FJ3:
    SAVSTK();
    PUSH(FJ_RI);
    PUSH(FJ_T2);
    PUSH(FJ_T1);
    JE_ORTHO(FJ_T);
L_FJ4:
    POP(FJ_REG);
    POP(FJ_F2);
    POP(FJ_F1);
    D(D_A(FJ_F1) + D_A(FJ_I)) = D(FJ_T);
    if (D_A(FJ_I) == DTH_)
	goto L_FJ99;
    D_A(FJ_I) += DESCR;
    goto L_FJ6;
L_FJ99:
    D(retval) = D(FJ_F1);
    RETURN(1)
}
int
HOR_REG(retval)
    ret_t retval;
{
    ENTRY(HOR_REG)
    D_A(GR_DIR) = (int_t) (WTH_-DESCR);
    BRANCH(GR1)
}
int
VER_REG(retval)
    ret_t retval;
{
    ENTRY(VER_REG)
    D_A(GR_DIR) = (int_t) (HT_-DESCR);
    BRANCH(GR1)
}
int
NORM_REG(retval)
    ret_t retval;
{
    ENTRY(NORM_REG)
    D_A(GR_DIR) = (int_t) (DTH_-DESCR);
    BRANCH(GR1)
}
static int
GR1(retval)
    ret_t retval;
{
    ENTRY(GR1)
    if (D_V(INCL) != 1)
	goto L_GRERR;
    SAVSTK();
    switch (BLOKVAL(TEMP1)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(TEMP1) == S)
	BRANCH(FAIL)
    D(TEMP39) = D(D_A(TEMP1) + ORG_);
    D_A(TEMP39) = D_V(TEMP39);
    D_F(TEMP39) = D_V(TEMP39) = 0;
    if (D_A(TEMP39) > OVY_)
	BRANCH(FAIL)
    if (D_A(D_A(TEMP1) + REG_) != (int_t)0)
	goto L_GR2;
    SAVSTK();
    PUSH(GR_REG);
    BLOCK(TEMP2);
    D(D_A(TEMP1) + REG_) = D(TEMP2);
L_GR2:
    D(XPTR) = D(D_A(TEMP1) + REG_);
    D_A(XPTR) += D_A(GR_DIR);
    BRANCH(RTXNAM)
L_GRERR:
    BRANCH(ARGNER)
}
static int
IDENT_SB(retval)
    ret_t retval;
{
    ENTRY(IDENT_SB)
    SAVSTK();
    PUSH(ID_D);
    BLOCK(ID_ID);
    POP(ID_SF);
    D(ID_T) = D(D_A(ID_SF) + HT_);
    D_V(ID_T) = D_A(ID_T);
    D_V(D_A(ID_ID) + HT_) = D_V(ID_T);
    D(ID_T) = D(D_A(ID_SF) + WTH_);
    D_V(ID_T) = D_A(ID_T);
    D_V(D_A(ID_ID) + WTH_) = D_V(ID_T);
    D(ID_T) = D(D_A(ID_SF) + DTH_);
    D_V(ID_T) = D_A(ID_T);
    D_V(D_A(ID_ID) + DTH_) = D_V(ID_T);
    D(retval) = D(ID_ID);
    RETURN(1)
}
static int
INIT_SUB(retval)
    ret_t retval;
{
    ENTRY(INIT_SUB)
    SAVSTK();
    PUSH(IS_SBD);
    BLOCK(IS_IS);
    POP(IS_SB);
    POP(IS_SF);
    POP(IS_DIR);
    POP(IS_REG);
    D(IS_DS) = D(IS_DIR);
    D(TEMP31) = D(D_A(IS_SF) + D_A(IS_DIR));
    D(TEMP32) = D(D_A(IS_SB) + D_A(IS_DIR));
    D(TEMP33) = D(TEMP32);
    D_V(TEMP32) = D_A(TEMP31);
    D(D_A(IS_IS) + D_A(IS_DIR)) = D(TEMP32);
    D_A(TEMP34) = D_V(TEMP33);
    D_F(TEMP34) = D_V(TEMP34) = 0;
    D_A(TEMP34) -= D_A(TEMP31);
    D_A(TEMP33) += D_A(TEMP31);
    D_V(TEMP33) = D_A(TEMP34);
    D(D_A(IS_SB) + D_A(IS_DIR)) = D(TEMP33);
L_IS1:
    if (D_A(IS_DIR)>= 3*DESCR) {
	D_A(IS_DIR) = 0;
	D_F(IS_DIR) = D_V(IS_DIR) = 0;
    }
    D_A(IS_DIR) += DESCR;
    if (D_A(IS_DIR) == D_A(IS_DS))
	goto L_IS2;
    D(IS_ST) = D(D_A(IS_SB) + D_A(IS_DIR));
    D_A(TEMP31) = D_V(IS_ST);
    D_F(TEMP31) = D_V(TEMP31) = 0;
    D(IS_SFD) = D(D_A(IS_SF) + D_A(IS_DIR));
    D(TEMP33) = D(D_A(IS_REG) + D_A(IS_DIR));
    SAVSTK();
    PUSH(TEMP33);
    PUSH(IS_SFD);
    PUSH(TEMP31);
    PRE_SUF(NORET);
    D_A(IS_ST) += D_A(TEMP31);
    D_V(IS_ST) = D_A(IS_SFD);
    D(D_A(IS_IS) + D_A(IS_DIR)) = D(IS_ST);
    goto L_IS1;
L_IS2:
    D(retval) = D(IS_IS);
    RETURN(1)
}
static int
INSERT(retval)
    ret_t retval;
{
    ENTRY(INSERT)
    POP(TEMP35);
    POP(IN_GF);
    POP(TEMP36);
    POP(IN_SB);
    D(IN_HO) = D(D_A(IN_SB) + HT_);
    D_A(IN_H) = D_V(IN_HO);
    D_F(IN_H) = D_V(IN_H) = 0;
    D(IN_WO) = D(D_A(IN_SB) + WTH_);
    D_A(IN_W) = D_V(IN_WO);
    D_F(IN_W) = D_V(IN_W) = 0;
    D(IN_DO) = D(D_A(IN_SB) + DTH_);
    D_A(IN_D) = D_V(IN_DO);
    D_F(IN_D) = D_V(IN_D) = 0;
    D(IN_DG) = D(D_A(IN_GF) + DTH_);
    D(IN_WG) = D(D_A(IN_GF) + WTH_);
    S_L(IN_REM) = D_A(IN_WO);
    if (D_A(IN_W) == 0)
	goto L_IN99;
    D(IN_N) = D(IN_HO);
    D_A(IN_N) *= D_A(IN_DG);
    D(IN_INC) = D(IN_DG);
    D_A(IN_INC) -= D_A(IN_D);
    D_A(IN_N) -= D_A(IN_INC);
    D_A(IN_N) += D_A(IN_DO);
    D_A(IN_N) *= DESCR;
    D_F(IN_N) = D_V(IN_N) = 0;
    D_A(IN_INC) *= DESCR;
    D_F(IN_INC) = D_V(IN_INC) = 0;
    D_A(IN_I) = 0;
    D_A(IN_M) = 0;
    X_LOCSP(TXSP,BKGNCL);
L_IN1:
    D_A(IN_J) = 0;
    D_A(IN_I)++;
    if (D_A(IN_I) > D_A(IN_H))
	goto L_IN99;
    D_A(IN_N) += D_A(IN_INC);
L_IN2:
    D_A(IN_J)++;
    if (D_A(IN_J) > D_A(IN_D))
	goto L_IN1;
    D_A(IN_M) += DESCR;
    D_A(IN_N) += DESCR;
    D(TEMP31) = D(D_A(TEMP35) + D_A(IN_N));
    X_LOCSP(VSP,TEMP31);
    D(TEMP32) = D(D_A(TEMP36) + D_A(IN_M));
    X_LOCSP(YSP,TEMP32);
    SAVSTK();
    PUSH(IN_WG);
    CONVAR(ZPTR);
    X_LOCSP(XSP,ZPTR);
    S_L(XSP) = 0;
    APDSP(XSP,VSP);
    X_REMSP(ZSP,XSP,IN_REM);
    MERGSP(ZSP,YSP,TXSP);
    SAVSTK();
    PUSH(IN_WG);
    GNVARS(ZPTR);
    D(D_A(TEMP35) + D_A(IN_N)) = D(ZPTR);
    goto L_IN2;
L_IN99:
    if (D_A(DMPCL) != 50)
	BRANCH(RTN1)
    io_printf(D_A(IN_UNIT),IN_FT1);
    SAVSTK();
    PUSH(TEMP35);
    DUMP_A(NORET);
    SAVSTK();
    PUSH(IN_GF);
    DUMP_B(NORET);
    SAVSTK();
    PUSH(TEMP36);
    DUMP_A(NORET);
    SAVSTK();
    PUSH(IN_SB);
    DUMP_B(NORET);
    BRANCH(RTN1)
}
static int
JE_LONGI(retval)
    ret_t retval;
{
    ENTRY(JE_LONGI)
    POP(JEL_E1);
    POP(JEL_E2);
    if (D_V(JEL_E1) == EDGDT)
	goto L_JEL1;
    if (D_V(JEL_E2) == EDGDT)
	goto L_JEL1;
    D_A(JEL_E1) += D_A(JEL_E2);
    BRANCH(JEL99)
L_JEL1:
    if (D_V(JEL_E1) == EDGDT)
	D(JEL_T1) = D(D_A(JEL_E1) + TR1_);
    else
	D(JEL_T1) = D(JEL_E1);
    if (D_V(JEL_E2) == EDGDT)
	D(JEL_T2) = D(D_A(JEL_E2) + TR1_);
    else
	D(JEL_T2) = D(JEL_E2);
    D(JEL_T) = D(JEL_T1);
    D_A(JEL_T) += D_A(JEL_T2);
    if (D_V(JEL_E1) == EDGDT)
	D(JEL_N1) = D(D_A(JEL_E1) + NN1_);
    else
	D(JEL_N1) = D(ZEROCL);
    if (D_V(JEL_E2) == EDGDT)
	D(JEL_N2) = D(D_A(JEL_E2) + NN1_);
    else
	D(JEL_N2) = D(ZEROCL);
    D(JEL_N) = D(JEL_N1);
    D_A(JEL_N) += D_A(JEL_N2);
    if (D_V(JEL_E1) == EDGDT)
	D(JEL_R1) = D(D_A(JEL_E1) + RELS1_);
    else
	D(JEL_R1) = D(NULVCL);
    if (D_V(JEL_E2) == EDGDT)
	D(JEL_R2) = D(D_A(JEL_E2) + RELS1_);
    else
	D(JEL_R2) = D(NULVCL);
    BRANCH(JELCO)
}
static int
JE_ORTHO(retval)
    ret_t retval;
{
    ENTRY(JE_ORTHO)
    POP(JEL_E1);
    POP(JEL_E2);
    POP(JEL_REG);
    if (D_V(JEL_E1) == EDGDT)
	goto L_JEL3;
    if (D_V(JEL_E2) == EDGDT)
	goto L_JEL3;
    if (D_A(JEL_E1) > D_A(JEL_E2))
	D(JEL_E1) = D(JEL_E1);
    else
	D(JEL_E1) = D(JEL_E2);
    BRANCH(JEL99)
L_JEL3:
    if (D_V(JEL_E1) == EDGDT)
	D(JEL_T1) = D(D_A(JEL_E1) + TR1_);
    else
	D(JEL_T1) = D(JEL_E1);
    if (D_V(JEL_E2) == EDGDT)
	D(JEL_T2) = D(D_A(JEL_E2) + TR1_);
    else
	D(JEL_T2) = D(JEL_E2);
    if (D_V(JEL_E1) == EDGDT)
	D(JEL_N1) = D(D_A(JEL_E1) + NN1_);
    else
	D(JEL_N1) = D(ZEROCL);
    if (D_V(JEL_E2) == EDGDT)
	D(JEL_N2) = D(D_A(JEL_E2) + NN1_);
    else
	D(JEL_N2) = D(ZEROCL);
    if (D_V(JEL_E1) == EDGDT)
	D(JEL_R1) = D(D_A(JEL_E1) + RELS1_);
    else
	D(JEL_R1) = D(NULVCL);
    if (D_V(JEL_E2) == EDGDT)
	D(JEL_R2) = D(D_A(JEL_E2) + RELS1_);
    else
	D(JEL_R2) = D(NULVCL);
    if (D_A(JEL_T1) >= D_A(JEL_T2))
	goto L_JEL4;
    D(TEMP39) = D(JEL_T1);
    D(JEL_T1) = D(JEL_T2);
    D(JEL_T2) = D(TEMP39);
    D(TEMP39) = D(JEL_N1);
    D(JEL_N1) = D(JEL_N2);
    D(JEL_N2) = D(TEMP39);
    D(TEMP39) = D(JEL_R1);
    D(JEL_R1) = D(JEL_R2);
    D(JEL_R2) = D(TEMP39);
L_JEL4:
    D(JEL_N) = D(JEL_N1);
    D_A(JEL_N) += D_A(JEL_N2);
    D(JEL_T) = D(JEL_T1);
    SAVSTK();
    PUSH(JEL_REG);
    PUSH(JEL_T2);
    PUSH(JEL_T1);
    PRE_SUF(NORET);
    D(JEL_T1) = D(TEMP31);
    /* FALL */
    BRANCH(JELCO)
}
static int
JELCO(retval)
    ret_t retval;
{
    ENTRY(JELCO)
    PUSH(JEL_E1);
    SAVSTK();
    PUSH(JEL_T1);
    PUSH(JEL_N2);
    PUSH(JEL_R2);
    PUSH(JEL_N1);
    PUSH(JEL_R1);
    CIR(JEL_R);
    POP(JEL_E1);
    if (D_V(JEL_E1) == EDGDT)
	goto L_JEL2;
    PUSH(JEL_R);
    SAVSTK();
    PUSH(JEL_SIZE);
    BLOCK(JEL_E1);
    POP(JEL_R);
L_JEL2:
    D(D_A(JEL_E1) + TR1_) = D(JEL_T);
    D(D_A(JEL_E1) + NN1_) = D(JEL_N);
    D(D_A(JEL_E1) + RELS1_) = D(JEL_R);
    /* FALL */
    BRANCH(JEL99)
}
static int
JEL99(retval)
    ret_t retval;
{
    ENTRY(JEL99)
    D(retval) = D(JEL_E1);
    RETURN(1)
}
static int
JOIN(retval)
    ret_t retval;
{
    ENTRY(JOIN)
    POP(JN_B1);
    POP(JN_B2);
    if (!DCMP(JN_B1, NULVCL))
	goto L_JOIN1;
    D(WCL) = D(JN_B2);
    goto L_JOIN99;
L_JOIN1:
    if (!DCMP(JN_B2, NULVCL))
	goto L_JOIN2;
    D(WCL) = D(JN_B1);
    goto L_JOIN99;
L_JOIN2:
    SAVSTK();
    PUSH(JN_B1);
    CLASS(JN_CL1);
    D_A(JN_CL1) *= 3;
    D_F(JN_CL1) = D_V(JN_CL1) = 0;
    SAVSTK();
    PUSH(JN_B2);
    CLASS(JN_CL2);
    D_A(JN_CL1) += D_A(JN_CL2);
    D_A(JN_CL1)++;
    PUSH(JN_B1);
    PUSH(JN_B2);
    switch (D_A(JN_CL1)) {
    case 1:
	goto L_ST_ST;
    case 2:
	goto L_ST_BW;
    case 3:
	goto L_ST_BA;
    case 4:
	goto L_BW_ST;
    case 5:
	goto L_BW_BW;
    case 6:
	goto L_BW_BA;
    case 7:
	goto L_BA_ST;
    case 8:
	goto L_BA_BW;
    case 9:
	goto L_BA_BA;
    }
L_ST_BA:
L_BA_ST:
L_BA_BA:
    SAVSTK();
    PUSH(JN_SIZE);
    BLOCK(WCL);
    POP(JN_B2);
    POP(JN_B1);
    D_V(D_A(WCL) + ORG_) = D_V(TEMP1);
    D_A(TEMP1) = (int_t) (FIRST_+DESCR);
    D_A(D_A(WCL) + TOP_) = D_A(TEMP1);
    D(D_A(WCL) + FIRST_) = D(JN_B1);
    D(D_A(WCL) + FIRST_+DESCR) = D(JN_B2);
    goto L_JOIN99;
L_ST_BW:
L_BA_BW:
    SAVSTK();
    PUSH(JN_B2);
    BCOPY(WCL);
    SAVSTK();
    BHEAD(NORET);
    POP(JN_B2);
    POP(JN_B1);
    D(D_A(WCL) + FIRST_) = D(JN_B1);
    goto L_JOIN99;
L_BW_ST:
L_BW_BA:
    SAVSTK();
    PUSH(JN_B1);
    BCOPY(WCL);
    SAVSTK();
    BTAIL(JN_TOP);
    POP(JN_B2);
    POP(JN_B1);
    D(D_A(WCL) + D_A(JN_TOP)) = D(JN_B2);
    goto L_JOIN99;
L_BW_BW:
    SAVSTK();
    PUSH(JN_B1);
    BCOPY(WCL);
    POP(TEMP2);
    D(JN_TOP) = D(D_A(TEMP2) + TOP_);
    D_A(JN_I) = (int_t) FIRST_;
L_BW_BW_1:
    if (D_A(JN_I) > D_A(JN_TOP))
	goto L_JOIN99;
    SAVSTK();
    BTAIL(JN_K);
    D(TEMP22) = D(D_A(TEMP2) + D_A(JN_I));
    D(D_A(WCL) + D_A(JN_K)) = D(TEMP22);
    D_A(JN_I) += DESCR;
    goto L_BW_BW_1;
L_ST_ST:
    if (D_V(TEMP1) != PAR_)
	goto L_BA_BA;
    D(XPTR) = D(JN_B1);
    D(YPTR) = D(JN_B2);
    SAVSTK();
    switch (CONVV(WCL)) {
    case 1:
	BRANCH(FAIL)
    case 3:
	goto L_JOIN99;
    }
L_JOIN99:
    D(retval) = D(WCL);
    RETURN(1)
}
int
LOC(retval)
    ret_t retval;
{
    ENTRY(LOC)
    D_A(LO_AC) = D_V(INCL);
    D_F(LO_AC) = D_V(LO_AC) = 0;
    if (D_A(LO_AC) < 2)
	BRANCH(FAIL)
    PUSH(LO_AC);
    SAVSTK();
    switch (BLOKVAL(TEMP1)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(TEMP1);
    SAVSTK();
    switch (BLOKVAL(TEMP2)) {
    case 1:
	BRANCH(FAIL)
    }
    SAVSTK();
    PUSH(NULVCL);
    PUSH(TEMP2);
    FIXINL(TEMP2);
    POP(TEMP1);
    POP(LO_AC);
    D(TEMP3) = D(ZEROCL);
    if (D_A(LO_AC) != 3)
	goto L_LOC1;
    PUSH(TEMP1);
    PUSH(TEMP2);
    SAVSTK();
    switch (INTVAL(TEMP3)) {
    case 1:
	BRANCH(FAIL)
    }
L_LOC1:
    D(LO_ND) = D(D_A(TEMP1) + ID_);
    D_A(TEMP3)++;
    D_A(TEMP3) *= DESCR;
    D_F(TEMP3) = D_V(TEMP3) = 0;
    D(TEMP4) = D(D_A(TEMP2) + FRAME_);
    D(TEMP5) = D(D_A(TEMP4) + D_A(TEMP3));
    if (D_V(TEMP5) == EDGDT)
	D(TEMP6) = D(D_A(TEMP5) + RELS1_);
    else
	D(TEMP6) = D(NULVCL);
    if (D_V(TEMP5) == EDGDT)
	D(TEMP7) = D(D_A(TEMP5) + NN1_);
    else
	D(TEMP7) = D(ZEROCL);
    SAVSTK();
    PUSH(TEMP7);
    PUSH(TEMP6);
    PUSH(LO_N1);
    PUSH(LO_A1);
    switch (FICOM(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    D(ZPTR) = D(D_A(TEMP6) + D_A(TEMP32));
    D_A(ZPTR) = D_V(ZPTR);
    D_F(ZPTR) = D_V(ZPTR) = 0;
    D_V(ZPTR) = I;
    BRANCH(RTZPTR)
}
int
LRECL(retval)
    ret_t retval;
{
    ENTRY(LRECL)
    /* FALL */
    BRANCH(CC)
}
int
CC(retval)
    ret_t retval;
{
    ENTRY(CC)
    if (D_V(INCL) != 1)
	BRANCH(ARGNER)
    SAVSTK();
    switch (INTVAL(XPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    D(YPTR) = D(ONECL);
    D_V(YPTR) = I;
    if (D_A(XPTR) == 0)
	goto L_LRECL5;
    D_A(YPTR) = 0;
L_LRECL5:
    if (D_A(TEMP48) == 0)
	goto L_LRECL4;
L_LRECL3:
    if (!LOCAPT(XPTR,TEMP48,XPTR))
	goto L_LRECL2;
    D_A(XPTR) += DESCR;
    D_V(XPTR) = N;
    BRANCH(RTXNAM)
L_LRECL2:
    SAVSTK();
    PUSH(YPTR);
    PUSH(XPTR);
    PUSH(TEMP48);
    AUGATL(TEMP48);
    goto L_LRECL3;
L_LRECL4:
    D(TEMP48) = D(LR_1);
    goto L_LRECL2;
}
static int
LSOHN(retval)
    ret_t retval;
{
    ENTRY(LSOHN)
    POP(LS_TN);
    D(LS_LS) = D(D_A(LS_TN) + LSO_);
    if (D_A(LS_LS) != 0)
	goto L_LS99;
    D(LS_B) = D(D_A(LS_TN) + VALUE_);
    D(LS_I) = D(D_A(LS_B) + TOP_);
    D(LS_ORG) = D(D_A(LS_B) + ORG_);
L_LS1:
    if (D_A(LS_I) < FIRST_)
	goto L_LS2;
    D(LS_BI) = D(D_A(LS_B) + D_A(LS_I));
    D_A(LS_I) -= DESCR;
L_LS4:
    if (D_V(LS_BI) == S)
	goto L_T6;
    D(TEMP39) = D(D_A(LS_BI) + ORG_);
    if (D_V(TEMP39) == DEF_)
	goto L_LS3;
L_T6:
    PUSH(LS_TN);
    PUSH(LS_B);
    SAVSTK();
    PUSH(LS_BI);
    PUSH(LS_LS);
    PUSH(LS_TN);
    T_LEAF(LS_LS);
    POP(LS_B);
    POP(LS_TN);
    goto L_LS1;
L_LS2:
    D(D_A(LS_TN) + LSO_) = D(LS_LS);
    goto L_LS99;
L_LS3:
    D(LS_BI) = D(D_A(LS_BI) + NAME_);
    D(LS_BI) = D(D_A(LS_BI) + DESCR);
    if (D_V(LS_BI) == BL)
	goto L_LS7;
    if (D_V(LS_BI) == S)
	goto L_LS4;
    if (D_V(LS_BI) != I)
	goto L_LS8;
    SAVSTK();
    PUSH(LS_BI);
    switch (GNVARI(LS_BI)) {
    case 1:
	goto L_LS4;
    }
L_LS8:
    if (D_V(LS_BI) != R)
	BRANCH(INTR1)
    REALST(REALSP,LS_BI);
    _SPEC(YSP) = _SPEC(REALSP);
    SAVSTK();
    PUSH(YSPPTR);
    switch (GENVAR(YPTR)) {
    case 1:
	goto L_LS4;
    }
L_LS7:
    if (D_V(D_A(LS_BI) + ORG_) != D_V(LS_ORG))
	goto L_LS4;
    if (D_V(D_A(LS_BI) + SET_) != D_V(LS_NULL))
	goto L_LS4;
    D(LS_OLDP) = D(LS_LS);
    D(D_A(LS_TN) + VALUE_) = D(LS_BI);
    PUSH(LS_OLDP);
    PUSH(LS_TN);
    PUSH(LS_B);
    PUSH(LS_I);
    PUSH(LS_ORG);
    SAVSTK();
    PUSH(LS_TN);
    LSOHN(LS_LS);
    POP(LS_ORG);
    POP(LS_I);
    POP(LS_B);
    POP(LS_TN);
    POP(LS_OLDP);
    D(D_A(LS_TN) + VALUE_) = D(LS_B);
    D(LS_P) = D(LS_LS);
L_LS5:
    if (D_A(D_A(LS_P) + RSIB_) == (int_t)0)
	goto L_LS6;
    D(LS_P) = D(D_A(LS_P) + RSIB_);
    goto L_LS5;
L_LS6:
    D(D_A(LS_P) + RSIB_) = D(LS_OLDP);
    goto L_LS1;
L_LS99:
    D(retval) = D(LS_LS);
    RETURN(1)
}
static int
MIDREG(retval)
    ret_t retval;
{
    ENTRY(MIDREG)
    POP(MR_SB1);
    POP(MR_SB2);
    POP(MR_DIR);
    D(TEMP31) = D(D_A(MR_SB1) + D_A(MR_DIR));
    D_A(TEMP32) = D_V(TEMP31);
    D_F(TEMP32) = D_V(TEMP32) = 0;
    D(MR_NO) = D(TEMP32);
    D_A(MR_NO) += D_A(TEMP31);
    D(TEMP31) = D(D_A(MR_SB2) + D_A(MR_DIR));
    D(MR_NE) = D(TEMP31);
    D_A(MR_NE) -= D_A(MR_NO);
    if (D_A(MR_NE) < 0)
	BRANCH(FAIL)
    D_V(MR_NO) = D_A(MR_NE);
    SAVSTK();
    PUSH(MR_SB1);
    BCOPY(MR_MR);
    D(D_A(MR_MR) + D_A(MR_DIR)) = D(MR_NO);
    D(retval) = D(MR_MR);
    RETURN(2)
}
static int
MINGLE(retval)
    ret_t retval;
{
    ENTRY(MINGLE)
    POP(TEMP28);
    POP(TEMP29);
    if (D_V(TEMP28) == EDGDT)
	D(MI_R1) = D(D_A(TEMP28) + RELS1_);
    else
	D(MI_R1) = D(NULVCL);
    if (D_V(TEMP28) == EDGDT)
	D(MI_N1) = D(D_A(TEMP28) + NN1_);
    else
	D(MI_N1) = D(ZEROCL);
    if (D_V(TEMP29) == EDGDT)
	D(MI_R2) = D(D_A(TEMP29) + RELS1_);
    else
	D(MI_R2) = D(NULVCL);
    if (D_V(TEMP29) == EDGDT)
	D(MI_N2) = D(D_A(TEMP29) + NN1_);
    else
	D(MI_N2) = D(ZEROCL);
    SAVSTK();
    PUSH(MI_N2);
    PUSH(MI_R2);
    PUSH(MI_N1);
    PUSH(MI_R1);
    switch (FICOM(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    D(MI_D1) = D(D_A(MI_R1) + D_A(TEMP31));
    D_A(MI_D1) = D_V(MI_D1);
    D_F(MI_D1) = D_V(MI_D1) = 0;
    D(MI_D2) = D(D_A(MI_R2) + D_A(TEMP32));
    D_A(MI_D2) = D_V(MI_D2);
    D_F(MI_D2) = D_V(MI_D2) = 0;
    D_A(MI_D1) -= D_A(MI_D2);
    if (D_A(MI_D1) >= 0)
	goto L_MI1;
    CLR_MATH_ERROR();
    D_A(MI_D1) = -D_A(MI_D1);
    if (MATH_ERROR())
	goto L_MI62;
    goto L_MI62;
L_MI62:
    D(TEMP39) = D(TEMP28);
    D(TEMP28) = D(TEMP29);
    D(TEMP29) = D(TEMP39);
    D(TEMP39) = D(MI_R1);
    D(MI_R1) = D(MI_R2);
    D(MI_R2) = D(TEMP39);
    D(TEMP39) = D(MI_N1);
    D(MI_N1) = D(MI_N2);
    D(MI_N2) = D(TEMP39);
L_MI1:
    SAVSTK();
    PUSH(MI_D1);
    PUSH(MI_N2);
    PUSH(MI_R2);
    PUSH(MI_N1);
    PUSH(MI_R1);
    CIR(TEMP33);
    if (D_V(TEMP28) == EDGDT)
	D(MI_T1) = D(D_A(TEMP28) + TR1_);
    else
	D(MI_T1) = D(TEMP28);
    if (D_V(TEMP29) == EDGDT)
	D(MI_T2) = D(D_A(TEMP29) + TR1_);
    else
	D(MI_T2) = D(TEMP29);
    D_A(MI_T2) += D_A(MI_D1);
    D_A(MI_N1) += D_A(MI_N2);
    if (D_A(MI_T1) > D_A(MI_T2))
	D(MI_T1) = D(MI_T1);
    else
	D(MI_T1) = D(MI_T2);
    SAVSTK();
    PUSH(MI_ED);
    BLOCK(MI_MI);
    D(D_A(MI_MI) + TR1_) = D(MI_T1);
    D(D_A(MI_MI) + NN1_) = D(MI_N1);
    D(D_A(MI_MI) + RELS1_) = D(TEMP33);
    D(retval) = D(MI_MI);
    RETURN(2)
}
int
MORE(retval)
    ret_t retval;
{
    ENTRY(MORE)
    POP(MR_BLOCK);
    D_A(MR_SIZE1) = D_V(D_A(MR_BLOCK));
    D_F(MR_SIZE1) = D_V(MR_SIZE1) = 0;
    D_A(MR_SIZE2) = D_A(MR_SIZE1);
    D_A(MR_SIZE2) *= 2;
    D_F(MR_SIZE2) = D_V(MR_SIZE2) = 0;
    D_V(MR_SIZE2) = D_V(MR_BLOCK);
    SAVSTK();
    PUSH(MR_SIZE2);
    BLOCK(MR_X);
    MOVBLK(D_A(MR_X),D_A(MR_BLOCK),D_A(MR_SIZE1));
    D(retval) = D(MR_X);
    RETURN(1)
}
static int
NRMZ_REG(retval)
    ret_t retval;
{
    ENTRY(NRMZ_REG)
    POP(ZCL);
    D_A(NR_DIR) = (int_t) HT_;
    SAVSTK();
    PUSH(NR_VA);
    PUSH(NR_VF);
    N_REG(NORET);
    D_A(NR_DIR) = (int_t) WTH_;
    SAVSTK();
    PUSH(NR_HA);
    PUSH(NR_HF);
    N_REG(NORET);
    D_A(NR_DIR) = (int_t) DTH_;
    SAVSTK();
    PUSH(NR_NA);
    PUSH(NR_NF);
    N_REG(NORET);
    BRANCH(NR99)
}
static int
N_REG(retval)
    ret_t retval;
{
    ENTRY(N_REG)
    POP(NR_F);
    POP(NR_A);
    D(NR_VAL) = D(D_A(ZCL) + D_A(NR_DIR));
    if (D_V(NR_VAL) != S)
	BRANCH(NR99)
    if (D_A(NR_VAL) != 0)
	goto L_NR1;
L_NR0:
    D(D_A(ZCL) + D_A(NR_DIR)) = D(NR_SC);
    BRANCH(NR99)
L_NR1:
    X_LOCSP(TXSP,NR_VAL);
    S_L(TXSP) = 1;
    _SPEC(NR_SP) = _SPEC(D_A(NR_F));
    if (!LEXEQ(TXSP,NR_SP))
	goto L_NR2;
    D(D_A(ZCL) + D_A(NR_DIR)) = D(NR_SF);
    BRANCH(NR99)
L_NR2:
    _SPEC(NR_SP) = _SPEC(D_A(NR_A));
    if (!LEXEQ(TXSP,NR_SP))
	goto L_NR3;
    D(D_A(ZCL) + D_A(NR_DIR)) = D(NR_SA);
    BRANCH(NR99)
L_NR3:
    X_LOCSP(TXSP,NR_VAL);
    if (D_A(NR_DES) == 0)
	goto L_NR9;
    CLR_MATH_ERROR();
    D_A(NR_DIR) /= D_A(NR_DES);
    if (MATH_ERROR())
	goto L_NR9;
L_NR9:
    D_A(NR_DIR)--;
    io_printf(D_A(NR_UNIT),NR_FMT1);
    io_printf(D_A(NR_UNIT),NR_FMT2,NR_DIR);
    D_A(NR_DIR)++;
    D_A(NR_DIR) *= DESCR;
    D_F(NR_DIR) = D_V(NR_DIR) = 0;
    FASTPR(IOKEY,NR_UNIT,NR_UNIT,TXSP,BLSP);
    SAVSTK();
    WARNING(NORET);
    goto L_NR0;
}
static int
NR99(retval)
    ret_t retval;
{
    ENTRY(NR99)
    RETURN(1)
}
int
PAR(retval)
    ret_t retval;
{
    ENTRY(PAR)
    D(TEMP1) = D(ZEROCL);
    D_V(TEMP1) = PAR_;
    BRANCH(OPS1)
}
int
SER(retval)
    ret_t retval;
{
    ENTRY(SER)
    D(TEMP1) = D(ZEROCL);
    D_V(TEMP1) = SER_;
    BRANCH(OPS1)
}
int
OVY(retval)
    ret_t retval;
{
    ENTRY(OVY)
    D(TEMP1) = D(ZEROCL);
    D_V(TEMP1) = OVY_;
    BRANCH(OPS1)
}
int
MERGE(retval)
    ret_t retval;
{
    ENTRY(MERGE)
    D(TEMP1) = D(ZEROCL);
    D_V(TEMP1) = MERGE_;
    BRANCH(OPS1)
}
static int
OPS1(retval)
    ret_t retval;
{
    ENTRY(OPS1)
    D(ZPTR) = D(NULVCL);
    D_A(OP_N) = D_V(INCL);
    D_F(OP_N) = D_V(OP_N) = 0;
L_OPS4:
    if (D_A(OP_N) != 0)
	goto L_OPS2;
    BRANCH(RTZPTR)
L_OPS2:
    D_A(OP_N)--;
    PUSH(ZPTR);
    PUSH(TEMP1);
    PUSH(OP_N);
    SAVSTK();
    switch (BLOKVAL(YPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(OP_N);
    POP(TEMP1);
    POP(ZPTR);
    SAVSTK();
    PUSH(YPTR);
    PUSH(ZPTR);
    JOIN(ZPTR);
    goto L_OPS4;
}
static int
CCATB(retval)
    ret_t retval;
{
    ENTRY(CCATB)
    D(TEMP1) = D(ZEROCL);
    D_V(TEMP1) = PAR_;
    SAVSTK();
    PUSH(YPTR);
    PUSH(XPTR);
    JOIN(XPTR);
    BRANCH(RTXPTR)
}
static int
PAR_CONG(retval)
    ret_t retval;
{
    ENTRY(PAR_CONG)
    POP(PC_TN);
L_PC1:
    D(PC_PC) = D(D_A(PC_TN) + FATHER_);
    if (D_A(PC_PC) == 0)
	BRANCH(FAIL)
    D(PC_VAL) = D(D_A(PC_PC) + VALUE_);
    if (D_V(D_A(PC_VAL) + ORG_) <= D_V(PC_MG))
	goto L_PC3;
    if (D_V(PC_VAL) == S)
	goto L_PC2;
    D(TEMP39) = D(D_A(PC_VAL) + ORG_);
    if (D_V(TEMP39) != PHY_)
	goto L_PC2;
L_PC3:
    D(retval) = D(PC_PC);
    RETURN(2)
L_PC2:
    D(PC_TN) = D(PC_PC);
    goto L_PC1;
}
static int
PRE_SUF(retval)
    ret_t retval;
{
    ENTRY(PRE_SUF)
    POP(PS_OTR);
    POP(PS_INR);
    POP(PS_REG);
    D(TEMP33) = D(PS_OTR);
    D_A(TEMP33) -= D_A(PS_INR);
    if (D_A(PS_REG) != 0)
	goto L_PS1;
    if (D_A(PS_TWO) == 0)
	BRANCH(FAIL)
    CLR_MATH_ERROR();
    D(TEMP31) = D(TEMP33);
    D_A(TEMP31) /= D_A(PS_TWO);
    if (MATH_ERROR())
	BRANCH(FAIL)
    D(TEMP32) = D(TEMP33);
    D_A(TEMP32) -= D_A(TEMP31);
L_PS99:
    RETURN(1)
L_PS1:
    if (D_A(PS_REG) != 1)
	goto L_PS2;
    D(TEMP32) = D(TEMP33);
    D(TEMP31) = D(PS_ZR);
    goto L_PS99;
L_PS2:
    D(TEMP32) = D(PS_ZR);
    D(TEMP31) = D(TEMP33);
    goto L_PS99;
}
int
PRINTB(retval)
    ret_t retval;
{
    ENTRY(PRINTB)
    D_A(PR_NA) = D_V(INCL);
    D_F(PR_NA) = D_V(PR_NA) = 0;
    if (D_A(PR_NA) == 0)
	BRANCH(ARGNER)
    D_A(PR_NA)--;
    SAVSTK();
    switch (BLOKVAL(XCL)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_A(XCL) == 0)
	BRANCH(RETNUL)
    if (D_V(XCL) == S)
	goto L_PR6;
    SAVSTK();
    PUSH(ZEROCL);
    PUSH(XCL);
    FIXINL(XCL);
L_PR6:
    PUSH(XCL);
    SAVSTK();
    PUSH(PR_NA);
    switch (UNITS(PR_UF1)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(XCL);
    if (D_A(PR_UF1) == 0)
	BRANCH(RETNUL)
    D_A(PR_NA) = D_V(D_A(PR_UF1));
    D_F(PR_NA) = D_V(PR_NA) = 0;
    goto L_PR3;
L_PR4:
    D_A(PR_UF1) += 2*DESCR;
L_PR3:
    if (D_A(PR_NA) == 0)
	BRANCH(RETNUL)
    D_A(PR_NA) -= 2*DESCR;
    D(PR_UNIT) = D(D_A(PR_UF1) + DESCR);
    if (D_A(PR_UNIT) == 0)
	goto L_PR4;
    D(PR_FLAG) = D(D_A(PR_UF1) + 2*DESCR);
    if (D_V(XCL) == S)
	goto L_PRSTR;
    D(TEMP2) = D(D_A(XCL) + FRAME_);
    D(PR_HT) = D(D_A(TEMP2) + HT_);
    if (D_V(PR_HT) == EDGDT)
	D(PR_HT) = D(D_A(PR_HT) + TR1_);
    D(PR_DTH) = D(D_A(TEMP2) + DTH_);
    if (D_V(PR_DTH) == EDGDT)
	D(PR_DTH) = D(D_A(PR_DTH) + TR1_);
    D_A(PR_N) = 0;
    D_A(PR_I) = 0;
    D(TEMP1) = D(D_A(XCL) + ARRAY_);
    if (D_A(TEMP1) == 0)
	goto L_PRNOTH;
    D(PR_UF2) = D(ZEROCL);
L_PR1:
    D_A(PR_I)++;
    if (D_A(PR_I) > D_A(PR_HT))
	goto L_PR99;
    D_A(PR_J) = 1;
    D_A(PR_N) += DESCR;
    D(YCL) = D(D_A(TEMP1) + D_A(PR_N));
    X_LOCSP(YSP,YCL);
    FASTPR(IOKEY,PR_UNIT,PR_FLAG,YSP,BLSP);
    if (D_A(IOKEY) != 0)
	BRANCH(PRERR)
L_PR2:
    D_A(PR_J)++;
    if (D_A(PR_J) > D_A(PR_DTH))
	goto L_PR1;
    D_A(PR_N) += DESCR;
    D(YCL) = D(D_A(TEMP1) + D_A(PR_N));
    if (D_A(PR_UF2) != 0)
	goto L_PR21;
    X_LOCSP(YSP,YCL);
    switch (STREAM(XSP, YSP, &FRWDTB)) {
    case ST_ERROR:
    case ST_EOS:
	goto L_PR23;
    }
L_PR22:
    X_LOCSP(YSP,YCL);
    FASTPR(IOKEY,PR_UNIT,PR_FLAG,YSP,PR_PLUS);
    if (D_A(IOKEY) != 0)
	BRANCH(PRERR)
    goto L_PR2;
L_PR21:
    if (D_A(PR_UF2) == D_A(YCL))
	goto L_PR2;
    goto L_PR22;
L_PR23:
    D(PR_UF2) = D(YCL);
    goto L_PR2;
L_PR99:
    if (D_A(PR_FLAG) >= 0)
	goto L_PR4;
    FASTPR(IOKEY,PR_UNIT,PR_FLAG,PR_NULL,BLSP);
    if (D_A(IOKEY) != 0)
	BRANCH(PRERR)
    goto L_PR4;
L_PRSTR:
    X_LOCSP(YSP,XCL);
    FASTPR(IOKEY,PR_UNIT,PR_FLAG,YSP,BLSP);
    if (D_A(IOKEY) != 0)
	BRANCH(PRERR)
    goto L_PR99;
L_PRNOTH:
L_PRN1:
    D_A(PR_I)++;
    if (D_A(PR_I) > D_A(PR_HT))
	goto L_PR99;
    FASTPR(IOKEY,PR_UNIT,PR_FLAG,PR_NULL,BLSP);
    if (D_A(IOKEY) != 0)
	BRANCH(PRERR)
    goto L_PRN1;
}
int
NEW_PAGE(retval)
    ret_t retval;
{
    ENTRY(NEW_PAGE)
    D_A(PR_NA) = D_V(INCL);
    D_F(PR_NA) = D_V(PR_NA) = 0;
    SAVSTK();
    PUSH(PR_NA);
    switch (UNITS(PR_UF1)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_A(PR_UF1) == 0)
	BRANCH(RETNUL)
    D_A(PR_NA) = D_V(D_A(PR_UF1));
    D_F(PR_NA) = D_V(PR_NA) = 0;
L_NP1:
    if (D_A(PR_NA) == 0)
	BRANCH(RETNUL)
    D(PR_I) = D(D_A(PR_UF1) + DESCR);
    if (D_A(PR_I) == 0)
	goto L_NP2;
    D(PR_FLAG) = D(D_A(PR_UF1) + 2*DESCR);
    FASTPR(IOKEY,PR_I,PR_FLAG,PR_NULL,PR_1);
    if (D_A(IOKEY) != 0)
	BRANCH(PRERR)
L_NP2:
    D_A(PR_NA) -= 2*DESCR;
    D_A(PR_UF1) += 2*DESCR;
    goto L_NP1;
}
static int
PRERR(retval)
    ret_t retval;
{
    ENTRY(PRERR)
    BRANCH(COMP6)
}
static int
P_BLOCK(retval)
    ret_t retval;
{
    ENTRY(P_BLOCK)
    D_V(PB_FS) = A;
    SAVSTK();
    PUSH(PB_FS);
    BLOCK(NP_FR);
    POP(NP_BL);
    POP(PB_H);
    POP(PB_W);
    POP(PB_D);
    POP(NP_AR);
    MOVBLK(D_A(NP_FR),D_A(PB_FR),D_A(PB_FS));
    PUSH(NP_AR);
    PUSH(NP_FR);
    PUSH(NP_BL);
    BRANCH(NP_BLOCK)
}
static int
NP_BLOCK(retval)
    ret_t retval;
{
    ENTRY(NP_BLOCK)
    SAVSTK();
    PUSH(NP_DE);
    BLOCK(NP_NP);
    POP(NP_BL);
    POP(NP_FR);
    POP(NP_AR);
    MOVBLK(D_A(NP_NP),D_A(NP_BK),D_A(NP_DE));
    D(retval) = D(NP_NP);
    RETURN(1)
}
static int
REPL(retval)
    ret_t retval;
{
    ENTRY(REPL)
    POP(RE_A);
    POP(RE_F);
    POP(RE_DIR);
    POP(RE_N);
    D(RE_RE) = D(NULVCL);
    if (D_A(RE_A) == 0)
	goto L_RE99;
    if (D_A(RE_N) < 1)
	goto L_RESZ;
    if (D_A(RE_N) == 1)
	goto L_RES1;
    if (D_A(RE_DIR) == WTH_)
	goto L_REH;
    D(RE_H) = D(D_A(RE_F) + HT_);
    D(RE_D) = D(D_A(RE_F) + DTH_);
    D(RE_L) = D(RE_D);
    D_A(RE_L) *= D_A(RE_H);
    D_A(RE_L) *= DESCR;
    D_F(RE_L) = D_V(RE_L) = 0;
    D(RE_SIZE) = D(RE_L);
    D_A(RE_SIZE) *= D_A(RE_N);
    PUSH(RE_A);
    PUSH(RE_F);
    D_V(RE_SIZE) = A;
    SAVSTK();
    PUSH(RE_SIZE);
    BLOCK(RE_RE);
    POP(RE_F);
    POP(RE_A);
    D(RE_P) = D(RE_RE);
    if (D_A(RE_DIR) != HT_)
	goto L_REN;
    D_A(RE_H) *= D_A(RE_N);
    D(D_A(RE_F) + HT_) = D(RE_H);
L_REV1:
    if (D_A(RE_N) == 0)
	goto L_REV2;
    D_A(RE_N)--;
    MOVBLK(D_A(RE_P),D_A(RE_A),D_A(RE_L));
    D_A(RE_P) += D_A(RE_L);
    goto L_REV1;
L_REV2:
    goto L_RE99;
L_REN:
    D(RE_L) = D(RE_D);
    D_A(RE_L) *= D_A(RE_N);
    D(D_A(RE_F) + DTH_) = D(RE_L);
    D_A(RE_D) *= DESCR;
    D_F(RE_D) = D_V(RE_D) = 0;
    D(RE_Q) = D(RE_A);
    D(RE_I) = D(RE_N);
L_REN1:
    if (D_A(RE_H) == 0)
	goto L_RE99;
    D_A(RE_H)--;
L_REN2:
    if (D_A(RE_I) == 0)
	goto L_REN3;
    D_A(RE_I)--;
    MOVBLK(D_A(RE_P),D_A(RE_Q),D_A(RE_D));
    D_A(RE_P) += D_A(RE_D);
    goto L_REN2;
L_REN3:
    D_A(RE_Q) += D_A(RE_D);
    D(RE_I) = D(RE_N);
    goto L_REN1;
L_REH:
    SAVSTK();
    PUSH(RE_A);
    BCOPY(TEMP31);
    D_A(RE_SIZE) = D_V(D_A(TEMP31));
    D_F(RE_SIZE) = D_V(RE_SIZE) = 0;
    D(TEMP32) = D(TEMP31);
    D(TEMP33) = D(TEMP32);
    D_A(TEMP33) += D_A(RE_SIZE);
    D(RE_W) = D(D_A(RE_F) + WTH_);
    D_A(RE_W) *= D_A(RE_N);
    D(D_A(RE_F) + WTH_) = D(RE_W);
L_REH1:
    D_A(TEMP32) += DESCR;
    if (D_A(TEMP32) > D_A(TEMP33))
	goto L_REH99;
    D(RE_P) = D(D_A(TEMP32));
    SAVSTK();
    PUSH(RE_N);
    PUSH(RE_P);
    DUPE(RE_P);
    D(D_A(TEMP32)) = D(RE_P);
    goto L_REH1;
L_REH99:
    D(RE_RE) = D(TEMP31);
    goto L_RE99;
L_RES1:
    SAVSTK();
    PUSH(RE_A);
    BCOPY(RE_RE);
    goto L_RE99;
L_RESZ:
    D(RE_RE) = D(NULVCL);
    D(D_A(RE_F) + D_A(RE_DIR)) = D(RE_0);
    goto L_RE99;
L_RE99:
    D(retval) = D(RE_RE);
    RETURN(1)
}
int
SLAB(retval)
    ret_t retval;
{
    ENTRY(SLAB)
    D_A(SL_NA) = D_V(INCL);
    D_F(SL_NA) = D_V(SL_NA) = 0;
    if (D_A(SL_NA) != 4)
	BRANCH(ARGNER)
    SAVSTK();
    switch (BLOKVAL(ZPTR)) {
    case 1:
	BRANCH(FAIL)
    }
    SAVSTK();
    PUSH(NULVCL);
    PUSH(ZPTR);
    FIXINL(ZPTR);
    SAVSTK();
    PUSH(ZPTR);
    BCOPY(ZPTR);
    D(TEMP2) = D(D_A(ZPTR) + FRAME_);
    SAVSTK();
    PUSH(TEMP2);
    STRIP_F(TEMP2);
    D(D_A(ZPTR) + FRAME_) = D(TEMP2);
    PUSH(ZPTR);
    SAVSTK();
    switch (INTVAL(SL_DIR)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_A(SL_DIR) < 0)
	BRANCH(LENERR)
    PUSH(SL_DIR);
    SAVSTK();
    switch (INTVAL(SL_I)) {
    case 1:
	BRANCH(FAIL)
    }
    PUSH(SL_I);
    SAVSTK();
    switch (INTVAL(SL_L)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_A(SL_L) < 0)
	BRANCH(LENERR)
    POP(SL_I);
    POP(SL_DIR);
    POP(ZPTR);
    D_A(SL_DIR)++;
    D(TEMP1) = D(D_A(ZPTR) + ARRAY_);
    D(D_A(ZPTR) + ARRAY_) = D(NULVCL);
    D(TEMP2) = D(D_A(ZPTR) + FRAME_);
    D(SL_H) = D(D_A(TEMP2) + HT_);
    D(SL_W) = D(D_A(TEMP2) + WTH_);
    D(SL_D) = D(D_A(TEMP2) + DTH_);
    D(WPTR) = D(ZEROCL);
    D_A(WPTR) = D_A(SL_DIR);
    D_A(WPTR) *= DESCR;
    D_F(WPTR) = D_V(WPTR) = 0;
    D(SL_T) = D(D_A(TEMP2) + D_A(WPTR));
    if (D_A(SL_I) >= 1)
	goto L_SL1;
    D_A(SL_L)--;
    D_A(SL_L) += D_A(SL_I);
    D_A(SL_I) = 1;
    if (D_A(SL_L) > 0)
	goto L_SL1;
    D_A(SL_L) = 0;
    goto L_SL2;
L_SL1:
    if (D_A(SL_I) <= D_A(SL_T))
	goto L_SL11;
    D_A(SL_L) = 0;
    goto L_SL2;
L_SL11:
    D_A(SL_I)--;
    D_A(SL_T) -= D_A(SL_I);
    if (D_A(SL_L) <= D_A(SL_T))
	goto L_SL2;
    D_A(SL_L) = D_A(SL_T);
L_SL2:
    D(D_A(TEMP2) + D_A(WPTR)) = D(SL_L);
    if (D_A(SL_L) == 0)
	BRANCH(RTZPTR)
    if (D_A(TEMP1) == 0)
	BRANCH(RTZPTR)
    switch (D_A(SL_DIR)) {
    case 1:
	goto L_SLAB1;
    case 2:
	goto L_SLAB2;
    case 3:
	goto L_SLAB3;
    }
L_SLAB1:
    D_A(SL_D) *= DESCR;
    D_F(SL_D) = D_V(SL_D) = 0;
    D_A(SL_L) *= D_A(SL_D);
    D_V(SL_L) = A;
    SAVSTK();
    PUSH(SL_L);
    BLOCK(TEMP3);
    D_A(SL_I) *= D_A(SL_D);
    D_A(TEMP1) += D_A(SL_I);
    MOVBLK(D_A(TEMP3),D_A(TEMP1),D_A(SL_L));
    D(D_A(ZPTR) + ARRAY_) = D(TEMP3);
    BRANCH(RTZPTR)
L_SLAB2:
    SAVSTK();
    PUSH(TEMP1);
    BCOPY(TEMP1);
    D_A(SL_T) = D_V(D_A(TEMP1));
    D_F(SL_T) = D_V(SL_T) = 0;
    D_A(SL_H) = 0;
L_SLAB21:
    D_A(SL_H) += DESCR;
    if (D_A(SL_H) > D_A(SL_T))
	goto L_SLAB22;
    D(YPTR) = D(D_A(TEMP1) + D_A(SL_H));
    SAVSTK();
    PUSH(SL_L);
    CONVAR(XPTR);
    X_LOCSP(YSP,YPTR);
    X_LOCSP(VSP,XPTR);
    S_L(VSP) = 0;
    S_L(XSP) = D_A(SL_I);
    X_REMSP(YSP,YSP,XSP);
    S_L(YSP) = D_A(SL_L);
    APDSP(VSP,YSP);
    SAVSTK();
    PUSH(SL_L);
    GNVARS(XPTR);
    D(D_A(TEMP1) + D_A(SL_H)) = D(XPTR);
    goto L_SLAB21;
L_SLAB22:
    D(D_A(ZPTR) + ARRAY_) = D(TEMP1);
    BRANCH(RTZPTR)
L_SLAB3:
    D_A(SL_D) *= DESCR;
    D_F(SL_D) = D_V(SL_D) = 0;
    D_A(SL_L) *= DESCR;
    D_F(SL_L) = D_V(SL_L) = 0;
    D(SL_T) = D(SL_H);
    D_A(SL_T) *= D_A(SL_L);
    D_V(SL_T) = A;
    SAVSTK();
    PUSH(SL_T);
    BLOCK(TEMP3);
    D(TEMP11) = D(TEMP1);
    D(TEMP31) = D(TEMP3);
    D_A(SL_I) *= DESCR;
    D_F(SL_I) = D_V(SL_I) = 0;
    D_A(TEMP11) += D_A(SL_I);
    D_A(SL_T) = 0;
L_SLAB32:
    D_A(SL_T)++;
    if (D_A(SL_T) > D_A(SL_H))
	goto L_SLAB31;
    MOVBLK(D_A(TEMP31),D_A(TEMP11),D_A(SL_L));
    D_A(TEMP31) += D_A(SL_L);
    D_A(TEMP11) += D_A(SL_D);
    goto L_SLAB32;
L_SLAB31:
    D(D_A(ZPTR) + ARRAY_) = D(TEMP3);
    BRANCH(RTZPTR)
}
static int
SUBBLOCK(retval)
    ret_t retval;
{
    ENTRY(SUBBLOCK)
    SAVSTK();
    PUSH(SB_DSB);
    BLOCK(SB_SB);
    POP(SB_GF);
    POP(SB_LF);
    D_A(SB_DIR) = 0;
L_SB1:
    D_A(SB_DIR) += DESCR;
    if (D_A(SB_DIR) > FRSIZE_)
	goto L_SB99;
    D(SB_GE) = D(D_A(SB_GF) + D_A(SB_DIR));
    D(SB_LE) = D(D_A(SB_LF) + D_A(SB_DIR));
    if (D_V(SB_GE) == EDGDT)
	D(SB_GR) = D(D_A(SB_GE) + RELS1_);
    else
	D(SB_GR) = D(NULVCL);
    if (D_V(SB_LE) == EDGDT)
	D(SB_LR) = D(D_A(SB_LE) + RELS1_);
    else
	D(SB_LR) = D(NULVCL);
    if (D_V(SB_GE) == EDGDT)
	D(SB_GN) = D(D_A(SB_GE) + NN1_);
    else
	D(SB_GN) = D(ZEROCL);
    if (D_V(SB_LE) == EDGDT)
	D(SB_LN) = D(D_A(SB_LE) + NN1_);
    else
	D(SB_LN) = D(ZEROCL);
    SAVSTK();
    PUSH(SB_LN);
    PUSH(SB_LR);
    PUSH(SB_GN);
    PUSH(SB_GR);
    switch (FICOM(NORET)) {
    case 1:
	BRANCH(FAIL)
    }
    D(TEMP33) = D(D_A(SB_GR) + D_A(TEMP31));
    D(TEMP34) = D(D_A(SB_LR) + D_A(TEMP32));
    D_A(TEMP33) = D_V(TEMP33);
    D_F(TEMP33) = D_V(TEMP33) = 0;
    D_A(TEMP34) = D_V(TEMP34);
    D_F(TEMP34) = D_V(TEMP34) = 0;
    D(TEMP35) = D(TEMP33);
    D_A(TEMP35) -= D_A(TEMP34);
    if (D_V(SB_LE) == EDGDT)
	D(TEMP36) = D(D_A(SB_LE) + TR1_);
    else
	D(TEMP36) = D(SB_LE);
    D_V(TEMP35) = D_A(TEMP36);
    D(D_A(SB_SB) + D_A(SB_DIR)) = D(TEMP35);
    goto L_SB1;
L_SB99:
    D(retval) = D(SB_SB);
    RETURN(2)
}
static int
STRIP_F(retval)
    ret_t retval;
{
    ENTRY(STRIP_F)
    POP(SF_F);
    SAVSTK();
    PUSH(SF_F);
    BCOPY(SF_F);
    D_A(SF_DIR) = (int_t) HT_;
L_SF1:
    D(SF_E) = D(D_A(SF_F) + D_A(SF_DIR));
    if (D_V(SF_E) == EDGDT)
	D(SF_E) = D(D_A(SF_E) + TR1_);
    D(D_A(SF_F) + D_A(SF_DIR)) = D(SF_E);
    D_A(SF_DIR) += DESCR;
    if (D_A(SF_DIR) <= DTH_)
	goto L_SF1;
    D(retval) = D(SF_F);
    RETURN(1)
}
static int
T_LEAF(retval)
    ret_t retval;
{
    ENTRY(T_LEAF)
    SAVSTK();
    PUSH(TL_SIZE);
    BLOCK(TL_TL);
    POP(TL_F);
    POP(TL_RS);
    POP(TL_V);
    D(D_A(TL_TL) + FATHER_) = D(TL_F);
    D(D_A(TL_TL) + RSIB_) = D(TL_RS);
L_TLF0:
    D(D_A(TL_TL) + VALUE_) = D(TL_V);
    if (D_V(TL_V) == S)
	goto L_TLF99;
    D(TL_N) = D(D_A(TL_V) + ORG_);
    D_A(TL_N) = D_V(TL_N);
    D_F(TL_N) = D_V(TL_N) = 0;
    switch (D_A(TL_N)) {
    case 1:
	goto L_TLF1;
    case 2:
	goto L_TLF2;
    case 3:
	goto L_TLF3;
    case 4:
	goto L_TLF4;
    case 5:
	goto L_TLF5;
    case 6:
	goto L_TLF6;
    case 7:
	goto L_TLF7;
    case 8:
	goto L_TLF8;
    case 9:
	goto L_TLF9;
    }
L_TLF1:
L_TLF2:
L_TLF3:
    D(TL_REG) = D(D_A(TL_V) + REG_);
    if (D_A(TL_REG) != 0)
	goto L_TLF11;
    D(D_A(TL_TL) + TNREG_) = D(CTR_REG);
    goto L_TLF99;
L_TLF11:
    PUSH(TL_TL);
    SAVSTK();
    PUSH(TL_REG);
    BCOPY(TL_REG);
    POP(TL_TL);
    SAVSTK();
    PUSH(TL_REG);
    NRMZ_REG(NORET);
    D(D_A(TL_TL) + TNREG_) = D(TL_REG);
    goto L_TLF99;
L_TLF4:
    goto L_TLF99;
L_TLF5:
    D(TL_V) = D(D_A(TL_V) + ELEMENT_);
    PUSH(TL_TL);
    SAVSTK();
    PUSH(TL_V);
    PUSH(NULVCL);
    PUSH(TL_TL);
    T_LEAF(TL_TN2);
    POP(TL_TL);
    D(D_A(TL_TL) + LSO_) = D(TL_TN2);
    D(TL_V) = D(D_A(TL_TN2) + VALUE_);
    if (D_V(TL_V) == S)
	goto L_TLF99;
    D(TEMP39) = D(D_A(TL_V) + ORG_);
    if (D_V(TEMP39) != REP_)
	goto L_TLF99;
    D(TL_TEMP) = D(D_A(TL_TL) + VALUE_);
    D(D_A(TL_TN2) + VALUE_) = D(TL_TEMP);
    D(D_A(TL_TL) + VALUE_) = D(TL_V);
    goto L_TLF99;
L_TLF6:
L_TLF7:
    D(TL_V) = D(D_A(TL_V) + ELEMENT_);
    PUSH(TL_TL);
    SAVSTK();
    PUSH(TL_V);
    PUSH(NULVCL);
    PUSH(TL_TL);
    T_LEAF(TL_TN2);
    POP(TL_TL);
    D(D_A(TL_TL) + LSO_) = D(TL_TN2);
    goto L_TLF99;
L_TLF8:
    D(TL_V) = D(D_A(TL_V) + NAME_);
    D(TL_V) = D(D_A(TL_V) + DESCR);
    goto L_TLF0;
L_TLF9:
    goto L_TLF99;
L_TLF99:
    D(retval) = D(TL_TL);
    RETURN(1)
}
int
IT(retval)
    ret_t retval;
{
    ENTRY(IT)
    D(UD_TP) = D(UD_IT);
    BRANCH(UDCOM)
}
int
REP(retval)
    ret_t retval;
{
    ENTRY(REP)
    D(UD_TP) = D(UD_REP);
    BRANCH(UDCOM)
}
int
NODE(retval)
    ret_t retval;
{
    ENTRY(NODE)
    D_A(UD_ID)++;
    D(UD_TP) = D(UD_ID);
    BRANCH(UDCOM)
}
static int
UDCOM(retval)
    ret_t retval;
{
    ENTRY(UDCOM)
    PUSH(UD_TP);
    SAVSTK();
    switch (BLOKVAL(TEMP1)) {
    case 1:
	BRANCH(FAIL)
    }
    if (D_V(TEMP1) != S)
	goto L_UDCOM1;
    SAVSTK();
    PUSH(NULVCL);
    PUSH(TEMP1);
    FIXINL(TEMP1);
L_UDCOM1:
    POP(UD_TP);
    SAVSTK();
    PUSH(UD_SIZE);
    BLOCK(XPTR);
    D(D_A(XPTR) + ELEMENT_) = D(TEMP1);
    D(D_A(XPTR) + ORG_) = D(UD_TP);
    BRANCH(RTXPTR)
}
int
DEF(retval)
    ret_t retval;
{
    ENTRY(DEF)
    SAVSTK();
    switch (IND(TEMP1)) {
    case 1:
	BRANCH(FAIL)
    }
    SAVSTK();
    PUSH(UD_SIZE);
    BLOCK(XPTR);
    D(D_A(XPTR) + ORG_) = D(UD_DFT);
    D(D_A(XPTR) + NAME_) = D(TEMP1);
    BRANCH(RTXPTR)
}
static int
UNITS(retval)
    ret_t retval;
{
    ENTRY(UNITS)
    D(UN_I) = D(UN_0);
    D(UN_UN) = D(UN_UN1);
    ZERBLK(D_A(UN_UN3),D_A(UN_UN4));
    POP(UN_N);
    if (D_A(UN_N) == 0)
	goto L_UNITS2;
L_UNITS1:
    if (D_A(UN_N) == 0)
	goto L_UN99;
    D_A(UN_N)--;
    PUSH(UN_UN);
    PUSH(UN_N);
    SAVSTK();
    switch (INTVAL(UN_I)) {
    case 1:
	BRANCH(FAIL)
    }
    POP(UN_N);
    POP(UN_UN);
    if (D_A(UN_I) < 0)
	goto L_UNITS1;
L_UNITS2:
    if (D_A(TEMP48) == 0)
	goto L_UNNATL;
    if (!LOCAPT(XCL,TEMP48,UN_I))
	goto L_UNNATL;
    D(YCL) = D(D_A(XCL) + 2*DESCR);
    if (D_A(UN_I) != 0)
	goto L_UNITS5;
    D_A(UN_I) = (int_t) PRUNO;
L_UNITS5:
    SAVSTK();
    PUSH(YCL);
    PUSH(UN_I);
    PUSH(UN_UN);
    AUGATL(UN_UN);
    goto L_UNITS1;
L_UNNATL:
    D(YPTR) = D(ZEROCL);
    if (D_A(UN_I) != 0)
	goto L_UNITS4;
    D_A(YPTR) = 1;
    D_A(UN_I) = (int_t) PRUNO;
L_UNITS4:
    SAVSTK();
    PUSH(YPTR);
    PUSH(UN_I);
    PUSH(UN_UN);
    AUGATL(UN_UN);
    goto L_UNITS1;
L_UN99:
    D(retval) = D(UN_UN);
    RETURN(2)
}
static int
WARNING(retval)
    ret_t retval;
{
    ENTRY(WARNING)
    D_A(WA_N)++;
    io_printf(D_A(WA_DIAG),WA_FMT,FILENM,LNNOCL,WA_N,STNOCL);
    if (D_A(WA_N) > MAXW)
	goto L_WA1;
    BRANCH(RTN1)
L_WA1:
    D_A(ERRTYP) = (int_t) (ERRNO+2);
    BRANCH(FTLEND)
}
static int
RT1NUL(retval)
    ret_t retval;
{
    ENTRY(RT1NUL)
    D(retval) = D(NULVCL);
    RETURN(1)
    /*_*/
}
static int
RTN1(retval)
    ret_t retval;
{
    ENTRY(RTN1)
    /* FALL */
    BRANCH(FAIL)
}
static int
FAIL(retval)
    ret_t retval;
{
    ENTRY(FAIL)
    RETURN(1)
    /*_*/
}
static int
RETNUL(retval)
    ret_t retval;
{
    ENTRY(RETNUL)
    D(retval) = D(NULVCL);
    RETURN(3)
    /*_*/
}
static int
RTN2(retval)
    ret_t retval;
{
    ENTRY(RTN2)
    RETURN(2)
    /*_*/
}
static int
RTN3(retval)
    ret_t retval;
{
    ENTRY(RTN3)
    /* FALL */
    BRANCH(RTNUL3)
}
static int
RTNUL3(retval)
    ret_t retval;
{
    ENTRY(RTNUL3)
    RETURN(3)
    /*_*/
}
static int
RTXNAM(retval)
    ret_t retval;
{
    ENTRY(RTXNAM)
    D(retval) = D(XPTR);
    RETURN(2)
    /*_*/
}
static int
RTXPTR(retval)
    ret_t retval;
{
    ENTRY(RTXPTR)
    D(retval) = D(XPTR);
    RETURN(3)
    /*_*/
}
static int
RTYPTR(retval)
    ret_t retval;
{
    ENTRY(RTYPTR)
    D(retval) = D(YPTR);
    RETURN(3)
    /*_*/
}
static int
ARTN(retval)
    ret_t retval;
{
    ENTRY(ARTN)
    D_A(ARTHCL)++;
    /* FALL */
    BRANCH(RTZPTR)
}
static int
RTZPTR(retval)
    ret_t retval;
{
    ENTRY(RTZPTR)
    D(retval) = D(ZPTR);
    RETURN(3)
    /*_*/
}
static int
A5RTN(retval)
    ret_t retval;
{
    ENTRY(A5RTN)
    D(retval) = D(A5PTR);
    RETURN(1)
    /*_*/
}
static int
GENVSZ(retval)
    ret_t retval;
{
    ENTRY(GENVSZ)
    SAVSTK();
    PUSH(XCL);
    switch (GNVARS(ZPTR)) {
    case 1:
	BRANCH(RTZPTR)
    }
    /*_*/
}
static int
GENVRZ(retval)
    ret_t retval;
{
    ENTRY(GENVRZ)
    SAVSTK();
    PUSH(ZSPPTR);
    switch (GENVAR(ZPTR)) {
    case 1:
	BRANCH(RTZPTR)
    }
    /*_*/
}
static int
GENVIX(retval)
    ret_t retval;
{
    ENTRY(GENVIX)
    SAVSTK();
    PUSH(XPTR);
    switch (GNVARI(XPTR)) {
    case 1:
	BRANCH(RTXNAM)
    }
    /*_*/
}
int
END(retval)
    ret_t retval;
{
    ENTRY(END)
    if (D_A(ERRLCL) <= 0)
	BRANCH(END0)
    D_A(ERRLCL)--;
    D_A(ERRTYP) = 0;
    D_A(ERRTXT) = 0;
    D(FRTNCL) = D(OCICL);
    D_A(FRTNCL) -= DESCR;
    D(LSTNCL) = D(STNOCL);
    D_A(LSLNCL) = D_A(LNNOCL);
    D_A(LSFLNM) = D_A(FILENM);
    SAVSTK();
    switch (XITHND(NORET)) {
    case 3:
	BRANCH(RTNUL3)
    }
    /* FALL */
    BRANCH(END0)
}
static int
END0(retval)
    ret_t retval;
{
    ENTRY(END0)
    io_flushall(0);
    if (D_A(BANRCL) == 0)
	BRANCH(FTLEN2)
    io_printf(D_A(PUNCH),NRMEND,LVLCL);
    io_printf(D_A(PUNCH),LASTSF,FILENM,LNNOCL,STNOCL);
    BRANCH(FTLEN2)
    /*_*/
}
static int
FTLEND(retval)
    ret_t retval;
{
    ENTRY(FTLEND)
    io_flushall(0);
    io_printf(D_A(PUNCH),FTLCF,FILENM,LNNOCL,ERRTYP,STNOCL,LVLCL);
    D_A(RETCOD) = 1;
    if (D_A(INICOM) != 0)
	goto L_FTLEN3;
    io_printf(D_A(PUNCH),ALOCFL);
    BRANCH(ENDALL)
    /*_*/
L_FTLEN3:
    D_A(YCL) = D_A(ERRTYP);
    D_A(YCL) *= DESCR;
    D_F(YCL) = D_V(YCL) = 0;
    D(YCL) = D(D_A(MSGNO) + D_A(YCL));
    _SPEC(TSP) = _SPEC(D_A(YCL));
    IO_PRINT(IOKEY, ERRBLK, TSP);
    /* FALL */
    BRANCH(FTLEN2)
}
static int
FTLEN2(retval)
    ret_t retval;
{
    ENTRY(FTLEN2)
    ISTACK();
    if (D_A(COMPCL) == 0)
	goto L_FTLEN4;
    D_RV(ETMCL) = mstime();
    D_F(ETMCL) = D_V(ETMCL) = 0;
    D_RV(TIMECL) = D_RV(ETMCL) - D_RV(TIMECL);
    D_F(TIMECL) = D_F(ETMCL);
    D_V(TIMECL) = D_V(ETMCL);
    D_A(ETMCL) = 0;
    goto L_FTLEN1;
    /*_*/
L_FTLEN4:
    D_RV(XCL) = mstime();
    D_F(XCL) = D_V(XCL) = 0;
    D_RV(ETMCL) = D_RV(XCL) - D_RV(ETMCL);
    D_F(ETMCL) = D_F(XCL);
    D_V(ETMCL) = D_V(XCL);
L_FTLEN1:
    if (D_A(DMPCL) == 0)
	goto L_END1;
    if (D_A(NODPCL) != 0)
	goto L_DMPNO;
    ordvst();
    io_printf(D_A(OUTPUT),STDMP);
    io_printf(D_A(OUTPUT),NVARF);
    SAVSTK();
    switch (DUMP(NORET)) {
    case 1:
    case 2:
	BRANCH(INTR10)
    case 3:
	goto L_DMPK;
    }
    /*_*/
L_DMPNO:
    io_printf(D_A(OUTPUT),INCGCF);
    io_printf(D_A(OUTPUT),NODMPF);
    goto L_END1;
    /*_*/
L_DMPK:
    SAVSTK();
    DMK(NORET);
L_END1:
    if (D_A(STATCL) == 0)
	BRANCH(ENDALL)
    io_printf(D_A(PUNCH),STATHD);
    io_printf(D_A(PUNCH),CMTIME,TIMECL);
    io_printf(D_A(PUNCH),INTIME,ETMCL);
    io_printf(D_A(PUNCH),EXNO,EXN2CL,FALCL);
    io_printf(D_A(PUNCH),ARTHNO,ARTHCL);
    io_printf(D_A(PUNCH),SCANNO,SCNCL);
    io_printf(D_A(PUNCH),STGENO,GCNO);
    io_printf(D_A(PUNCH),STGETM,GCTTTL);
    io_printf(D_A(PUNCH),READNO,RSTAT);
    io_printf(D_A(PUNCH),WRITNO,WSTAT);
    if (D_A(EXN2CL) != 0)
	goto L_END2;
    D_RV(FCL) = (real_t)D_A(ZEROCL);
    D_F(FCL) = 0;
    D_V(FCL) = R;
    goto L_AVTIME;
    /*_*/
L_END2:
    D_RV(EXN2CL) = (real_t)D_A(EXN2CL);
    D_F(EXN2CL) = 0;
    D_V(EXN2CL) = R;
    D(FCL) = D(ETMCL);
    D_RV(FCL) /= D_RV(EXN2CL);
    D_RV(FCL) *= D_RV(R1MCL);
L_AVTIME:
    io_printf(D_A(PUNCH),TIMEPS,FCL);
    if (D_RV(ETMCL) == D_RV(RZERCL))
	BRANCH(ENDALL)
    CLR_MATH_ERROR();
    D(FCL) = D(EXN2CL);
    D_RV(FCL) /= D_RV(ETMCL);
    if (RMATH_ERROR(D_RV(FCL)))
	BRANCH(ENDALL)
    io_printf(D_A(PUNCH),KSTSF,FCL);
    /* FALL */
    BRANCH(ENDALL)
}
static int
ENDALL(retval)
    ret_t retval;
{
    ENTRY(ENDALL)
    endex(D_A(ABNDCL));
    return 0;
    /*_*/
}
int
SYSCUT(retval)
    ret_t retval;
{
    ENTRY(SYSCUT)
    io_flushall(0);
    io_printf(D_A(PUNCH),SYSCMT,FILENM,LNNOCL,SIGNCL,STNOCL,LVLCL);
    if (D_A(CUTNO) != 0)
	BRANCH(ENDALL)
    D_A(CUTNO) = 1;
    D_A(RETCOD) = 1;
    BRANCH(FTLEN2)
    /*_*/
}
static int
AERROR(retval)
    ret_t retval;
{
    ENTRY(AERROR)
    D_A(ERRTYP) = 2;
    BRANCH(FTLTST)
    /*_*/
}
static int
ALOC2(retval)
    ret_t retval;
{
    ENTRY(ALOC2)
    D_A(ERRTYP) = 20;
    BRANCH(FTLEND)
    /*_*/
}
int
ARGNER(retval)
    ret_t retval;
{
    ENTRY(ARGNER)
    D_A(ERRTYP) = 25;
    BRANCH(FTLEND)
    /*_*/
}
static int
COMP1(retval)
    ret_t retval;
{
    ENTRY(COMP1)
    D_A(ERRTYP) = 32;
    BRANCH(FTLTST)
    /*_*/
}
int
INTR10(retval)
    ret_t retval;
{
    ENTRY(INTR10)
    /* FALL */
    BRANCH(INTR13)
}
static int
INTR13(retval)
    ret_t retval;
{
    ENTRY(INTR13)
    /* FALL */
    BRANCH(COMP3)
}
static int
COMP3(retval)
    ret_t retval;
{
    ENTRY(COMP3)
    D_A(ERRTYP) = 17;
    BRANCH(FTLEND)
    /*_*/
}
static int
COMP5(retval)
    ret_t retval;
{
    ENTRY(COMP5)
    D_A(ERRTYP) = 11;
    BRANCH(FTLTST)
    /*_*/
}
static int
COMP6(retval)
    ret_t retval;
{
    ENTRY(COMP6)
    D_A(ERRTYP) = 33;
    BRANCH(FTLTST)
    /*_*/
}
static int
COMP7(retval)
    ret_t retval;
{
    ENTRY(COMP7)
    D_A(ERRTYP) = 27;
    BRANCH(FTLEND)
    /*_*/
}
static int
COMP9(retval)
    ret_t retval;
{
    ENTRY(COMP9)
    D_A(ERRTYP) = 26;
    D_A(ESAICL) -= DESCR;
    BRANCH(FTLEND)
    /*_*/
}
int
EROR(retval)
    ret_t retval;
{
    ENTRY(EROR)
    D_A(ERRTYP) = 28;
    D_A(OCICL) += DESCR;
    D(STNOCL) = D(D_A(OCBSCL) + D_A(OCICL));
    D_A(OCICL) += DESCR;
    D(LNNOCL) = D(D_A(OCBSCL) + D_A(OCICL));
    D_A(OCICL) += DESCR;
    D(FILENM) = D(D_A(OCBSCL) + D_A(OCICL));
    BRANCH(FTLEND)
    /*_*/
}
static int
EXEX(retval)
    ret_t retval;
{
    ENTRY(EXEX)
    D_A(ERRTYP) = 22;
    BRANCH(FTLEND)
    /*_*/
}
static int
INTR1(retval)
    ret_t retval;
{
    ENTRY(INTR1)
    D_A(ERRTYP) = 1;
    BRANCH(FTLTST)
    /*_*/
}
static int
INTR4(retval)
    ret_t retval;
{
    ENTRY(INTR4)
    D_A(ERRTYP) = 24;
    BRANCH(FTLEND)
    /*_*/
}
static int
INTR5(retval)
    ret_t retval;
{
    ENTRY(INTR5)
    D_A(ERRTYP) = 19;
    BRANCH(FTLEND)
    /*_*/
}
static int
INTR8(retval)
    ret_t retval;
{
    ENTRY(INTR8)
    D_A(ERRTYP) = 15;
    BRANCH(FTLTST)
    /*_*/
}
static int
INTR27(retval)
    ret_t retval;
{
    ENTRY(INTR27)
    D_A(ERRTYP) = 13;
    BRANCH(FTLTST)
    /*_*/
}
static int
INTR30(retval)
    ret_t retval;
{
    ENTRY(INTR30)
    D_A(ERRTYP) = 10;
    BRANCH(FTLTST)
    /*_*/
}
static int
INTR31(retval)
    ret_t retval;
{
    ENTRY(INTR31)
    D_A(ERRTYP) = 16;
    D_A(SCERCL) = 3;
    BRANCH(FTERST)
    /*_*/
}
static int
LENERR(retval)
    ret_t retval;
{
    ENTRY(LENERR)
    D_A(ERRTYP) = 14;
    BRANCH(FTLTST)
    /*_*/
}
static int
MAIN1(retval)
    ret_t retval;
{
    ENTRY(MAIN1)
    D_A(ERRTYP) = 18;
    BRANCH(FTLEND)
    /*_*/
}
static int
NEMO(retval)
    ret_t retval;
{
    ENTRY(NEMO)
    D_A(ERRTYP) = 8;
    BRANCH(FTLTST)
    /*_*/
}
static int
NONAME(retval)
    ret_t retval;
{
    ENTRY(NONAME)
    D_A(ERRTYP) = 4;
    BRANCH(FTLTST)
    /*_*/
}
static int
NONARY(retval)
    ret_t retval;
{
    ENTRY(NONARY)
    D_A(ERRTYP) = 3;
    BRANCH(FTLTST)
    /*_*/
}
int
OVER(retval)
    ret_t retval;
{
    ENTRY(OVER)
    D_A(ERRTYP) = 21;
    BRANCH(FTLEND)
    /*_*/
}
static int
PROTER(retval)
    ret_t retval;
{
    ENTRY(PROTER)
    D_A(ERRTYP) = 6;
    BRANCH(FTLTST)
    /*_*/
}
static int
SIZERR(retval)
    ret_t retval;
{
    ENTRY(SIZERR)
    D_A(ERRTYP) = 23;
    BRANCH(FTLEND)
    /*_*/
}
int
UNDF(retval)
    ret_t retval;
{
    ENTRY(UNDF)
    D_A(ERRTYP) = 5;
    BRANCH(FTLTST)
    /*_*/
}
static int
UNDFFE(retval)
    ret_t retval;
{
    ENTRY(UNDFFE)
    D_A(ERRTYP) = 9;
    BRANCH(FTLTST)
    /*_*/
}
static int
UNKNKW(retval)
    ret_t retval;
{
    ENTRY(UNKNKW)
    D_A(ERRTYP) = 7;
    BRANCH(FTLTST)
    /*_*/
}
static int
UNTERR(retval)
    ret_t retval;
{
    ENTRY(UNTERR)
    D_A(ERRTYP) = 12;
    BRANCH(FTLTST)
    /*_*/
}
static int
USRINT(retval)
    ret_t retval;
{
    ENTRY(USRINT)
    D_A(ERRTYP) = 34;
    D_A(UINTCL) = 0;
    BRANCH(FTLTST)
    /*_*/
}
static int
CNTERR(retval)
    ret_t retval;
{
    ENTRY(CNTERR)
    D_A(ERRTYP) = 35;
    BRANCH(FTLEND)
    /*_*/
}
static int
SCERST(retval)
    ret_t retval;
{
    ENTRY(SCERST)
    D_A(SCERCL) = 1;
    BRANCH(FTERST)
    /*_*/
}
static int
FTLTST(retval)
    ret_t retval;
{
    ENTRY(FTLTST)
    D_A(SCERCL) = 2;
    /* FALL */
    BRANCH(FTERST)
}
static int
FTERST(retval)
    ret_t retval;
{
    ENTRY(FTERST)
    if (D_A(ERRLCL) <= 0)
	BRANCH(FTLEND)
    D_A(ERRLCL)--;
    D_A(YCL) = D_A(ERRTYP);
    D_A(YCL) *= DESCR;
    D_F(YCL) = D_V(YCL) = 0;
    D(YCL) = D(D_A(MSGNO) + D_A(YCL));
    _SPEC(TSP) = _SPEC(D_A(YCL));
    SAVSTK();
    PUSH(TSPPTR);
    GENVAR(ERRTXT);
    if (D_A(TRAPCL) <= 0)
	goto L_FTERBR;
    if (!LOCAPT(ATPTR,TKEYL,ERRTKY))
	goto L_FTERBR;
    PUSH(SCERCL);
    SAVSTK();
    PUSH(ATPTR);
    TRPHND(NORET);
    POP(SCERCL);
L_FTERBR:
    SAVSTK();
    XITHND(NORET);
    switch (D_A(SCERCL)) {
    case 1:
    case 2:
	BRANCH(FAIL)
    case 3:
	BRANCH(RTNUL3)
    }
    /* PANIC? */
    ;
}
