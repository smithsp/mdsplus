#include <STATICdef.h>
#include "stdio.h"
#define U(x) ((x)&0377)
#define NLSTATE yyprevious=YYNEWLINE
#define BEGIN yybgin = yysvec + 1 +
#define INITIAL 0
#define YYLERR yysvec
#define YYSTATE (yyestate-yysvec-1)
#define YYOPTIM 1
#define YYLMAX 200
#define output(c) putc(c,yyout)
#define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
#define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
#define yymore() (yymorfg=1)
#define ECHO fprintf(yyout, "%s",yytext)
#define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng;
extern unsigned char yytext[];
int yymorfg;
extern unsigned char *yysptr, yysbuf[];
int yytchar;
/*
FILE *yyin = {stdin}, *yyout = {stdout};
*/
FILE *yyin = 0, *yyout = 0;
extern int yylineno;
/* forward declaration for yywork necessary for c++*/
struct yywork;
struct yysvf {
  struct yywork *yystoff;
  struct yysvf *yyother;
  int *yystops;
};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
/*define YY_NOPROTO to disable function prototypes */
#ifndef YY_NOPROTO
#if defined (__STDC__) || defined  (__cplusplus)
/* GNU C always defines __STDC__. Must test if zero */
#if defined (__GNUC__) && !__STDC__
#define YYVOID
#else
#define YYVOID void
#endif /*__GNUC__ */
#else				/* __STDC__ */
#define YYVOID
#endif				/* __STDC__ */
#if defined  (__cplusplus)
extern "C" {
#endif /*__cplusplus_ */
  int yylook(YYVOID);
  int yylex(YYVOID);
  int yywrap(YYVOID);
#if defined  (__cplusplus)
}
#endif /*__cplusplus_ */
#endif				/* YY_NOPROTO */
	/*      TdiLex
	   Lexical analysis to parse tokens for TdiYacc.y.
	   Definition section precedes rule section.
	   Lex regular expression operators:
	   \x "x"               as is unless "" or []
	   [xyz] [x-z]  chars x through z
	   x?           0 or 1 x
	   x*           0 or more x
	   x+           1 or more x
	   x{4,7}               4 to 7 x
	   x|y          either x or y
	   (x)          grouping
	   x/y          x if y follows
	   .            any non-newline char
	   ---------------------------------------
	   Adjustable table sizes used/default.
	   %e   /1000   nodes
	   %p   /2500   positions
	   %n   /500    6095 transitions
	   %k   /1000   packed classes
	   %a   /~2000  packed transitions
	   %o   /3000   output slots
	   Ken Klare, LANL P-4  (c)1989,1990,1991
	   NEED to handle 6..8 and 6...8 and 6...8.9, should use spaces.
	   Limitations:
	   Floating requires (1) digit before exponent
	   (2) either decimal point or exponent, (.E3) looks like path.
	   (3) 6..8 is 6 .. 8 and not 6. .8, 6...8 is ambiguous?
	   (4) prefix + or - on numbers handled elsewhere, a-6 would be tokens a -6.
	   Pathname apostrophe required (1) with wildcards (% or *),
	   (2) without leading \ . or : (path:member looks like file),
	   (3) with son or member starting with number (.7E6 is float not son),
	   (4) with up-tree minus requires leading period. .-.over
	   Filename double quote required for non-simple names.
	   "node::node::device:[--.dir.sub.sub]file.extension;version".
	   Input, nlpos, output, pos, unput, and yylex defined by include file.
	   Floating for exponent but no decimal would fall into integer.
	 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mdsdescrip.h>
#include <mdsshr.h>
#include "tdirefzone.h"
#include "tdilexdef.h"
#include "tdireffunction.h"
#include <strroutines.h>
#include <tdishr_messages.h>
#include <treeshr.h>

#ifdef WIN32
#pragma warning (disable : 4013 4102 4035)	/* LEX produces code with no forward declarations of yylook and yyback. Also has unreferenced label yyfussy. And two int functions: yyoutput and yyunput do not return a value. */
#endif

#ifdef output
#undef output
#endif
#define output(c)               (c)

#define YYLEX                   TdiLex

extern int TdiConvert();
extern struct marker *TdiYylvalPtr;

extern unsigned short Opcdollar, OpcZero,
    OpcAdd, OpcAnd, OpcConcat, OpcDivide, OpcEq,
    OpcGe, OpcGt, OpcIand, OpcInot, OpcIor,
    OpcLe, OpcLt, OpcMultiply, OpcNe, OpcNot, OpcOr,
    OpcPower, OpcPreDec, OpcPreInc, OpcPromote, OpcDtypeRange,
    OpcShiftLeft, OpcShiftRight, OpcSubtract;

extern
int LEX_ERROR, LEX_IDENT, LEX_VBL, LEX_TEXT, LEX_VALUE,
    LEX_IN, LEX_LAND, LEX_LEQ, LEX_LEQV, LEX_LGE, LEX_LOR, LEX_MUL,
    LEX_ADD, LEX_CONCAT, LEX_IAND, LEX_INC, LEX_IOR,
    LEX_POINT, LEX_POWER, LEX_PROMO, LEX_RANGE, LEX_SHIFT, LEX_BINEQ,
    LEX_LANDS, LEX_LEQS, LEX_LGES, LEX_LORS, LEX_MULS, LEX_UNARYS;

#define _MOVC3(a,b,c) memcpy(c,b,a)
STATIC_ROUTINE int TdiLexBinEq(int token);

extern void TdiYyReset()
{
/*
  yy_reset();
*/
}

extern int TdiHash();

STATIC_ROUTINE void upcase(unsigned char *str, int str_len)
{
  unsigned char *pc;

  for (pc = str; pc < str + str_len; ++pc)
    if (*pc >= 'a' && *pc <= 'z')
      *pc += (unsigned char)('A' - 'a');
}

/*--------------------------------------------------------
        Remove comment from the Lex input stream.
        Nested comments allowed. Len is not used.
        Limitation:     Not ANSI C standard use of delimiters.
*/
STATIC_ROUTINE int TdiLexComment(int len, unsigned char *str, struct marker *mark_ptr)
{
  char c, c1;
  int count = 1;

  while (count) {
    if ((c = input()) == '/') {
      if ((c1 = input()) == '*')
	++count;
      else
	unput(c1);
    } else if (c == '*') {
      if ((c1 = input()) == '/')
	--count;
      else
	unput(c1);
    } else if (c == '\0') {
      TdiRefZone.l_status = TdiUNBALANCE;
      return 1;
    }
  }
  return 0;
}

/*--------------------------------------------------------
        Convert floating point values with the follow
ing
        syntax to internal representation via descriptors:
                [sign] decimal [fraction] exponent
                or [sign] [decimal] fraction [exponent]
        where:  decimal         [+|-] 0-9...
                fraction        . 0-9...
                exponent        [E|F|D|G|H|S|T] [+|-] 0-9...
        NEED to size based on exponent range and number of digits.
*/
STATIC_CONSTANT DESCRIPTOR(dfghst_dsc, "DFGHSTVdfghstv");
STATIC_CONSTANT DESCRIPTOR(e_dsc, "E");
STATIC_CONSTANT DESCRIPTOR(valid_dsc, "+-.0123456789DEFGHSTV \t");

STATIC_ROUTINE int ConvertFloating(struct descriptor_s *str, struct descriptor_r *out_d)
{
  char str_c[64];
  int len = str->length > 63 ? 63 : str->length;
  strncpy(str_c, str->pointer, len);
  str_c[len] = 0;
  if (out_d->length == sizeof(double)) {
    double tmp;
    struct descriptor tmp_d = { sizeof(double), DTYPE_NATIVE_DOUBLE, CLASS_S, 0 };
    tmp_d.pointer = (char *)&tmp;
    tmp = atof(str_c);
    return TdiConvert(&tmp_d, out_d);
  } else {
    float tmp;
    struct descriptor tmp_d = { sizeof(float), DTYPE_NATIVE_FLOAT, CLASS_S, 0 };
    tmp_d.pointer = (char *)&tmp;
    sscanf(str_c, "%g", &tmp);
    return TdiConvert(&tmp_d, out_d);
  }
}

STATIC_ROUTINE int TdiLexFloat(int str_len, unsigned char *str, struct marker *mark_ptr)
{
  struct descriptor_s str_dsc = { 0, DTYPE_T, CLASS_S, 0 };
  int bad, idx, status, tst, type;
  STATIC_CONSTANT struct {
    unsigned short length;
    unsigned char dtype;
  } table[] = {
    {
    4, DTYPE_NATIVE_FLOAT}, {
    8, DTYPE_NATIVE_DOUBLE}, {
    8, DTYPE_D}, {
    8, DTYPE_G}, {
    16, DTYPE_H}, {
    4, DTYPE_F}, {
    4, DTYPE_FS}, {
    8, DTYPE_FT}
  };
  str_dsc.length = str_len;
  str_dsc.pointer = (char *)str;
  upcase(str, str_len);
	/*******************
        Find final location.
        *******************/
  bad = StrFindFirstNotInSet(&str_dsc, &valid_dsc);
  if (bad > 0)
    str_dsc.length = bad - 1;

	/**********************
        Find special exponents.
        **********************/
  idx = StrFindFirstInSet(&str_dsc, &dfghst_dsc);
  if (idx) {
    switch (tst = str[idx - 1]) {
    case 'D':
      type = 1;
      break;
    case 'V':
      type = 2;
      break;
    case 'G':
      type = 3;
      break;
    case 'H':
      type = 4;
      break;
    case 'F':
      type = 5;
      break;
    case 'S':
      type = 6;
      break;
    case 'T':
      type = 7;
      break;
    }
    str[idx - 1] = 'E';
  } else
    type = 0;

  MAKE_S(table[type].dtype, table[type].length, mark_ptr->rptr);

  status = ConvertFloating(&str_dsc, mark_ptr->rptr);
  if (status & 1 && bad > 0 && str[bad - 1] != '\0')
    status = TdiEXTRANEOUS;

  mark_ptr->builtin = -1;
  if (status & 1)
    return (LEX_VALUE);
  TdiRefZone.l_status = status;
  return (LEX_ERROR);
}

/*--------------------------------------------------------
        Convert Lex input to identifier name or builtin.
        Clobbers string with upcase. IDENT token returns name.
        Note, Lex strings are NUL terminated.
*/
STATIC_ROUTINE int compare(char *s1, struct TdiFunctionStruct *s2)
{
  return strcmp(s1, s2->name);
}

STATIC_ROUTINE int TdiLexIdent(int len, unsigned char *str, struct marker *mark_ptr)
{
  struct descriptor_s sd = { 0, DTYPE_T, CLASS_S, 0 };
  int j, token;
  unsigned char *str_l;

  sd.length = len;
  sd.pointer = (char *)str;
/*
        upcase(str,len);
*/
  mark_ptr->builtin = -1;
  MAKE_S(DTYPE_T, len, mark_ptr->rptr);
  _MOVC3(mark_ptr->rptr->length, str, (char *)mark_ptr->rptr->pointer);

	/*****************************
        $ marks next compile argument.
        $nnn marks nnn-th argument.
        *****************************/
  if (str[0] == '$') {
    for (j = len; --j > 0;)
      if (str[j] < '0' || str[j] > '9')
	break;
    if (j == 0) {
      mark_ptr->builtin = Opcdollar;
      return (LEX_IDENT);
    }
  } else if (str[0] == '_')
    return (LEX_VBL);

	/**********************
        Search of initial list.
        **********************/
  str_l = (unsigned char *)strncpy((char *)malloc(len + 1), (char *)str, len);
  str_l[len] = 0;
  j = TdiHash(len, str_l);
  free(str_l);
  if (j < 0) {
    if (str[0] == '$')
      return (LEX_VBL);
    return (LEX_IDENT);
  }

	/**********************************************
        Note difference of pointers is divided by step.
        Standard function gives number. Token if named.
        **********************************************/
  mark_ptr->builtin = (short)j;
  token = TdiRefFunction[j].token;
  if ((token & LEX_K_NAMED) != 0) {
    token = token & LEX_M_TOKEN;
    if (token == LEX_IN
	|| token == LEX_LAND
	|| token == LEX_LEQ
	|| token == LEX_LEQV || token == LEX_LGE || token == LEX_LOR || token == LEX_MUL)
      return TdiLexBinEq(token);
    return token;
  }
  return (LEX_IDENT);
}

/*--------------------------------------------------------
        Convert integer values with the following syntax
        to internal representation via descriptors:
                [space]...[sign][radix][digit]...[type]
        where:  sign    +       ignored
                        -       negative
                radix   0B b    binary          digit   0-1
        (digit          0O o    octal                   0-7
        required)       0D d    decimal(float)          0-9
                        0X x    hexadecimal             0-9A-Fa-f
                type    SB b    signed byte
                        SW w    signed word
                        SL l    signed long
                        SQ q    signed quadword
                        SO o    signed octaword
                        UB bu   unsigned byte
                        UW wu   unsigned word
                        UL lu   unsigned long
                        UQ qu   unsigned quadword
                        UO ou   unsigned octaword
        CAUTION must use unsigned char to avoid sign extend in hex.
        WARNING without following digit B and O radix become byte and octaword.
        WARNING hexadecimal strings must use SB or UB for bytes.

        Limitations:
        Depends on contiguous character set, 0-9 A-F a-f.
        Depends on right-to-left byte assignment.
        Depends on ones- or twos-complement.
        NEED size based on range, thus no overflow.
*/
#define len1 8			/*length of a word in bits */
#define num1 16			/*number of words to accumulate, octaword */

STATIC_ROUTINE int TdiLexInteger(int str_len, unsigned char *str, struct marker *mark_ptr)
{
  STATIC_ROUTINE struct {
    unsigned short length;
    unsigned char udtype, sdtype;
  } table[] = {
    {
    1, DTYPE_BU, DTYPE_B}, {
    2, DTYPE_WU, DTYPE_W}, {
    4, DTYPE_LU, DTYPE_L}, {
    8, DTYPE_QU, DTYPE_Q}, {
  16, DTYPE_OU, DTYPE_O},};
  unsigned char sign, *now = str, *end = &str[str_len];
  unsigned char *qptr, qq[num1], qtst;
  int carry = 0, radix;
  int length, is_signed, status = 1, tst, type;

	/******************************
        Remove leading blanks and tabs.
        ******************************/
  while (now < end && (*now == ' ' || *now == '\t'))
    ++now;
  upcase(now, str_len);

	/*********
        Save sign.
        *********/
  if (now >= end)
    sign = '+';
  else if ((sign = *now) == '-' || sign == '+')
    ++now;

	/***************************************************
        Select radix. Must be followed by appropriate digit.
        Leading zero is required in our LEX.
        Watch, 0bu a proper unsigned byte is good and
        0bub a binary with no digits is bad. Reject 0b33.
        ***************************************************/
  if (now < end && *now == '0')
    ++now;
  if (now + 1 < end) {
    switch (tst = *now++) {
    case 'B':
      radix = 2;
      break;
    case 'O':
      radix = 8;
      break;
      /*      case 'D' :      radix = 10;     break; */
    case 'X':
      radix = 16;
      break;
    default:
      --now;
      radix = 0;
      break;
    }
    if ((carry = radix) == 0) {
      radix = 10;
    } else {
      if ((tst = *now) >= '0' && tst <= '9')
	carry = tst - '0';
      else if (tst >= 'A' && tst <= 'F')
	carry = tst - 'A' + 10;
      if (carry >= radix)
	--now;
    }
  } else
    radix = 10;

	/**************
        Convert number.
        **************/
  for (qptr = &qq[0]; qptr < &qq[num1]; ++qptr)
    *qptr = 0;
  for (; now < end; ++now) {
    if ((tst = *now) >= '0' && tst <= '9')
      carry = tst - '0';
    else if (tst >= 'A' && tst <= 'F')
      carry = tst - 'A' + 10;
    else
      break;
    if (carry >= radix) {
      carry = 0;
      break;
    }
    for (qptr = &qq[0]; qptr < &qq[num1]; ++qptr) {
      *qptr = (char)(carry += (int)*qptr * radix);
      carry >>= len1;
    }
  }

	/***********************************************************
        Negative numbers do negation until nonzero, then complement.
        Works for 1 or 2's complement, not sign and magnitude.
        Unsigned overflow: carry | sign, signed: carry | wrong sign.
        ***********************************************************/
  if (sign == '-') {
    for (qptr = &qq[0]; qptr < &qq[num1]; ++qptr)
      if ((*qptr = (char)(-*qptr)) != 0)
	break;
    for (; ++qptr < &qq[num1];)
      *qptr = (char)(~*qptr);
  }

	/*******************************
        Find output size and signedness.
        *******************************/
  is_signed = -1;
  type = 2;
  if (now < end)
    switch (tst = *now++) {
    case 'U':
      is_signed = 0;
      break;
    case 'S':
      is_signed = 1;
      break;
    case 'B':
      type = 0;
      break;
    case 'W':
      type = 1;
      break;
    case 'L':
      type = 2;
      break;
    case 'Q':
      type = 3;
      break;
    case 'O':
      type = 4;
      break;
    default:
      --now;
      break;
    }

  if (now >= end) {
  } else if (is_signed < 0)
    switch (tst = *now++) {
    case 'U':
      is_signed = 0;
      break;
    case 'S':
      is_signed = 1;
      break;
    default:
      --now;
      break;
  } else
    switch (tst = *now++) {
    case 'B':
      type = 0;
      break;
    case 'W':
      type = 1;
      break;
    case 'L':
      type = 2;
      break;
    case 'Q':
      type = 3;
      break;
    case 'O':
      type = 4;
      break;
    default:
      --now;
      break;
    }

	/*******************************
        Remove trailing blanks and tabs.
        *******************************/
  while (now < end && (*now == ' ' || *now == '\t'))
    ++now;

  length = table[type].length;
  MAKE_S((unsigned char)(is_signed ? table[type].sdtype : table[type].udtype),
	 (unsigned short)length, mark_ptr->rptr);
  mark_ptr->builtin = -1;
  _MOVC3(length, (char *)qq, (char *)mark_ptr->rptr->pointer);

	/*************************
        Check the high order bits.
        *************************/
  if (now < end && *now != '\0')
    status = TdiEXTRANEOUS;

  qtst = (unsigned char)((is_signed && (char)qq[length - 1] < 0) ? -1 : 0);
  if (carry != 0 || (is_signed && ((qtst != 0) ^ (sign == '-'))))
    status = TdiTOO_BIG;
  else
    for (qptr = &qq[length]; qptr < &qq[num1]; ++qptr)
      if (*qptr != qtst)
	status = TdiTOO_BIG;

  if (status & 1) {
#ifdef WORDS_BIGENDIAN
    int i;
    unsigned char *ptr = mark_ptr->rptr->pointer;
    for (i = 0; i < length / 2; i++) {
      unsigned char sav = ptr[i];
      ptr[i] = ptr[length - i - 1];
      ptr[length - i - 1] = sav;
    }
#endif
    return (LEX_VALUE);
  }
  TdiRefZone.l_status = status;
  return (LEX_ERROR);
}

/*--------------------------------------------------------
        Convert Lex input to NID or absolute PATH.
*/
int TdiLexPath(int len, unsigned char *str, struct marker *mark_ptr)
{
  int nid, token = LEX_VALUE;
  unsigned char *str_l;
  str_l = (unsigned char *)strncpy((char *)malloc(len + 1), (char *)str, len);
  str_l[len] = 0;
  upcase(str_l, len);
  mark_ptr->builtin = -1;
  if (TdiRefZone.l_rel_path) {
    MAKE_S(DTYPE_PATH, (unsigned short)len, mark_ptr->rptr);
    _MOVC3(len, str, (char *)mark_ptr->rptr->pointer);
  } else if (TreeFindNode((char *)str_l, &nid) & 1) {
    MAKE_S(DTYPE_NID, (unsigned short)sizeof(nid), mark_ptr->rptr);
    *(int *)mark_ptr->rptr->pointer = nid;
  } else {
    struct descriptor abs_dsc = { 0, DTYPE_T, CLASS_D, 0 };
    char *apath = TreeAbsPath((char *)str_l);
    if (apath != NULL) {
      unsigned short alen = (unsigned short)strlen(apath);
      StrCopyR(&abs_dsc, &alen, apath);
      TreeFree(apath);
      MAKE_S(DTYPE_PATH, abs_dsc.length, mark_ptr->rptr);
      _MOVC3(abs_dsc.length, abs_dsc.pointer, (char *)mark_ptr->rptr->pointer);
      StrFree1Dx(&abs_dsc);
    } else {
      TdiRefZone.l_status = 0;
      token = LEX_ERROR;
    }
  }
  free(str_l);
  return token;
}

/*--------------------------------------------------------
        Remove arrow and trailing punctation.
*/
STATIC_ROUTINE int TdiLexPoint(int len, unsigned char *str, struct marker *mark_ptr)
{
  int lenx = len - 2;
  while (str[lenx + 1] == '.' || str[lenx + 1] == ':')
    --lenx;
  mark_ptr->builtin = -1;
  MAKE_S(DTYPE_T, lenx, mark_ptr->rptr);
  _MOVC3((unsigned short)lenx, &str[2], (char *)mark_ptr->rptr->pointer);
  return LEX_POINT;
}

/*--------------------------------------------------------
        Recognize some graphic punctuation Lex symbols for YACC.
        Note must be acceptable in written form also: a<=b, a LE b, LE(a,b).
        Binary a<=(b,c) is OK, but unary <=(b,c) should not be.
*/
STATIC_ROUTINE int TdiLexBinEq(int token)
{
  char cx;

  while ((cx = input()) == ' ' || cx == '\t') ;
  if (cx == '=')
    return (LEX_BINEQ);
  unput(cx);
  return token;
}

STATIC_ROUTINE int TdiLexPunct(int len, unsigned char *str, struct marker *mark_ptr)
{
  char c0 = str[0], c1 = input();

  mark_ptr->rptr = 0;

	/********************
        Two graphic operator.
        ********************/
  switch (c0) {
  case '!':
    if (c1 == '=') {
      mark_ptr->builtin = OpcNe;
      return TdiLexBinEq(LEX_LEQS);
    }
    break;
  case '&':
    if (c1 == '&') {
      mark_ptr->builtin = OpcAnd;
      return TdiLexBinEq(LEX_LANDS);
    }
    break;
  case '*':
    if (c1 == '*') {
      mark_ptr->builtin = OpcPower;
      return TdiLexBinEq(LEX_POWER);
    }
    break;
  case '+':
    if (c1 == '+') {
      mark_ptr->builtin = OpcPreInc;
      return (LEX_INC);
    }
    break;
  case '-':
    if (c1 == '-') {
      mark_ptr->builtin = OpcPreDec;
      return (LEX_INC);
    }
    break;
/***                    if (c1 == '>') {                                        return                  (LEX_POINT);} break;***/
  case '.':
    if (c1 == '.') {
      mark_ptr->builtin = OpcDtypeRange;
      return (LEX_RANGE);
    }
    break;
  case '/':
    if (c1 == '/') {
      mark_ptr->builtin = OpcConcat;
      return TdiLexBinEq(LEX_CONCAT);
    }
    break;
/***                     else if (c1 == '*') return (TdiLexComment(len, str, mark_ptr) == 0) ? input() : 0; break; ***/
/***                    if (c1 == '=') {mark_ptr->builtin = OpcNe;              return TdiLexBinEq      (LEX_LEQS);}*/
/***                    if (c1 == ')') {                                        return                  ']';} break;***/
/***    case '(' :      if (c1 == '/') {                                        return                  '[';} break;***/
  case '<':
    if (c1 == '<') {
      mark_ptr->builtin = OpcShiftLeft;
      return TdiLexBinEq(LEX_SHIFT);
    }
    if (c1 == '=') {
      mark_ptr->builtin = OpcLe;
      return TdiLexBinEq(LEX_LGES);
    }
    if (c1 == '>') {
      mark_ptr->builtin = OpcNe;
      return TdiLexBinEq(LEX_LEQS);
    }
    break;
  case '=':
    if (c1 == '=') {
      mark_ptr->builtin = OpcEq;
      return TdiLexBinEq(LEX_LEQS);
    }
    break;
  case '>':
    if (c1 == '=') {
      mark_ptr->builtin = OpcGe;
      return TdiLexBinEq(LEX_LGES);
    }
    if (c1 == '>') {
      mark_ptr->builtin = OpcShiftRight;
      return TdiLexBinEq(LEX_SHIFT);
    }
    break;
  case '|':
    if (c1 == '|') {
      mark_ptr->builtin = OpcOr;
      return TdiLexBinEq(LEX_LORS);
    }
    break;
  }
  unput(c1);

	/********************
        One graphic operator.
        ********************/
  switch (c0) {
  case '!':
    mark_ptr->builtin = OpcNot;
    return (LEX_UNARYS);
/****   case '%' :      mark_ptr->builtin = OpcMod;             return TdiLexBinEq      (LEX_MULS);****/
  case '&':
    mark_ptr->builtin = OpcIand;
    return TdiLexBinEq(LEX_IAND);
  case '*':
    mark_ptr->builtin = OpcMultiply;
    return TdiLexBinEq('*');
  case '+':
    mark_ptr->builtin = OpcAdd;
    return TdiLexBinEq(LEX_ADD);
  case '-':
    mark_ptr->builtin = OpcSubtract;
    return TdiLexBinEq(LEX_ADD);
  case '/':
    mark_ptr->builtin = OpcDivide;
    return TdiLexBinEq(LEX_MULS);
  case ':':
    mark_ptr->builtin = OpcDtypeRange;
    return (LEX_RANGE);
  case '<':
    mark_ptr->builtin = OpcLt;
    return TdiLexBinEq(LEX_LGES);
  case '>':
    mark_ptr->builtin = OpcGt;
    return TdiLexBinEq(LEX_LGES);
  case '@':
    mark_ptr->builtin = OpcPromote;
    return (LEX_PROMO);
  case '^':
    mark_ptr->builtin = OpcPower;
    return TdiLexBinEq(LEX_POWER);
  case '|':
    mark_ptr->builtin = OpcIor;
    return TdiLexBinEq(LEX_IOR);
  case '~':
    mark_ptr->builtin = OpcInot;
    return (LEX_UNARYS);
  }
  mark_ptr->builtin = -1;
  return (c0);
}

/*--------------------------------------------------------
        C-style text in matching quotes. Strict: must end in quote. Continuation: \ before newline.
        Limitation: Text is ASCII dependent in quotes.
        Limitation: No wide characters. L'\xabc'
        Code all ANSI C escapes.
        On \ followed by non-standard we remove \.
        NEED overflow check on octal and hex. Watch sign extend of char.
*/

int TdiLexQuote(int len, unsigned char *str, struct marker *mark_ptr)
{
  char c, c1, *cptr = TdiRefZone.a_cur;
  int cur = 0, limit;

  while ((c = input()) && c != str[0])
    if (c == '\\')
      input();
  limit = TdiRefZone.a_cur - cptr - 1;
  TdiRefZone.a_cur = cptr;
  MAKE_S(DTYPE_T, limit, mark_ptr->rptr);
  mark_ptr->builtin = -1;
  while (cur <= limit) {
    if ((c = input()) == str[0]) {
      mark_ptr->rptr->length = cur;
      return (LEX_TEXT);
    } else if (c == '\\') {
      c = input();
      switch (c) {
      default:
	break;			/*non-standard */
      case '\n':
	continue;		/*continuation ignores end of line */
      case 'n':
	c = '\n';
	break;			/*NL/LF newline */
      case 't':
	c = '\t';
	break;			/*HT    horizontal tab */
      case 'v':
	c = '\v';
	break;			/*VT    vertical tab */
      case 'b':
	c = '\b';
	break;			/*BS    backspace */
      case 'r':
	c = '\r';
	break;			/*CR    carriage return */
      case 'f':
	c = '\f';
	break;			/*FF    formfeed */
      case 'a':
	c = '\007';
	break;			/*BEL   audible alert */
      case '\\':
	break;			/*backslash */
      case '\?':
	break;			/*question mark */
      case '\'':
	break;			/*single quote */
      case '\"':
	break;			/*double quote */
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
	c = c - '0';		/*octal number, 1-3 digits */
	if ((c1 = input()) >= '0' && c1 <= '7')
	  c = (c << 3) | (c1 - '0');
	else
	  unput(c1);
	if ((c1 = input()) >= '0' && c1 <= '7')
	  c = (c << 3) | (c1 - '0');
	else
	  unput(c1);
	break;
      case 'x':
	c = 0;			/*hex number, any number of digits */
	while ((c1 = input())) {
	  if (c1 >= '0' && c1 <= '9')
	    c = (c << 4) | (c1 - '0');
	  else if (c1 >= 'A' && c1 <= 'F')
	    c = (c << 4) | (c1 - 'A' + 10);
	  else if (c1 >= 'a' && c1 <= 'f')
	    c = (c << 4) | (c1 - 'a' + 10);
	  else
	    break;
	}
	unput(c1);
      }
    } else if (c == '\0')
      break;
    mark_ptr->rptr->pointer[cur++] = c;
  }
  TdiRefZone.l_status = TdiUNBALANCE;
  return (LEX_ERROR);
}

#define YYNEWLINE 10
int yylex()
{
  int nstr;
  extern int yyprevious;
  while ((nstr = yylook()) >= 0)
 yyfussy:switch (nstr) {
    case 0:
      if (yywrap())
	return (0);
      break;
    case 1:
      {;
      }
      break;
    case 2:
      {
	nlpos();
      }
      break;
    case 3:
      {
	pos();
	return (TdiLexFloat(yyleng, (unsigned char *)yytext, TdiYylvalPtr));
      }
      break;
    case 4:
      {
	pos();
	return (TdiLexFloat(yyleng, (unsigned char *)yytext, TdiYylvalPtr));
      }
      break;
    case 5:
      {
	pos();
	return (TdiLexInteger(yyleng, (unsigned char *)yytext, TdiYylvalPtr));
      }
      break;
    case 6:
      {
	pos();
	return (TdiLexIdent(yyleng, (unsigned char *)yytext, TdiYylvalPtr));
      }
      break;
    case 7:
      {
	pos();
	return (TdiLexPath(yyleng, (unsigned char *)yytext, TdiYylvalPtr));
      }
      break;
    case 8:
      {
	pos();
	return (TdiLexQuote(yyleng, (unsigned char *)yytext, TdiYylvalPtr));
      }
      break;
    case 9:
      {
	pos();
	return (TdiLexPoint(yyleng, (unsigned char *)yytext, TdiYylvalPtr));
      }
      break;
    case 10:
      {
	pos();
	if (TdiLexComment(yyleng, (unsigned char *)yytext, TdiYylvalPtr))
	  return (LEX_ERROR);
      }
      break;
    case 11:
      {
	pos();
	return (TdiLexPunct(yyleng, (unsigned char *)yytext, TdiYylvalPtr));
      }
      break;
    case -1:
      break;
    default:
      fprintf(yyout, "bad switch yylook %d", nstr);
    }
  return (0);
}

/* end of yylex */

int yyvstop[] = {
  0,

  11,
  0,

  1,
  11,
  0,

  2,
  0,

  8,
  11,
  0,

  6,
  7,
  11,
  0,

  11,
  0,

  11,
  0,

  11,
  0,

  5,
  11,
  0,

  11,
  0,

  11,
  0,

  6,
  7,
  0,

  7,
  0,

  7,
  0,

  4,
  0,

  10,
  0,

  -3,
  0,

  5,
  0,

  5,
  0,

  5,
  0,

  7,
  0,

  7,
  0,

  9,
  0,

  3,
  0,

  4,
  0,

  4,
  5,
  0,

  9,
  0,

  4,
  0,

  4,
  0,
  0
};

#define YYTYPE unsigned int
struct yywork {
  YYTYPE verify, advance;
} yycrank[] = {
0, 0, 0, 0, 1, 3, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 4, 1, 5,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 6,
      0, 0, 1, 7, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 10, 20,
      1, 3, 0, 0, 1, 8, 1, 9,
      1, 10, 1, 11, 2, 8, 28, 18,
      2, 10, 27, 36, 18, 18, 18, 28,
      36, 37, 0, 0, 0, 0, 1, 12,
      35, 41, 0, 0, 0, 0, 8, 16,
      0, 0, 27, 37, 1, 7, 18, 15,
      0, 0, 1, 7, 7, 14, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      7, 15, 0, 0, 7, 14, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 15, 1, 13, 0, 0, 0, 0,
      0, 0, 2, 13, 0, 0, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 0, 0, 0, 0, 0, 0,
      0, 0, 7, 14, 0, 0, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 7, 14, 7, 14, 7, 14,
      7, 14, 9, 17, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 9, 18, 0, 0,
      0, 0, 9, 19, 9, 19, 9, 19,
      9, 19, 9, 19, 9, 19, 9, 19,
      9, 19, 9, 19, 9, 19, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 9, 17, 9, 17,
      9, 17, 9, 17, 9, 17, 9, 17,
      9, 17, 9, 17, 9, 17, 9, 17,
      9, 17, 9, 17, 9, 17, 9, 17,
      9, 17, 9, 17, 9, 17, 9, 17,
      9, 17, 9, 17, 9, 17, 9, 17,
      9, 17, 9, 17, 9, 17, 9, 17,
      0, 0, 0, 0, 0, 0, 0, 0,
      9, 17, 0, 0, 9, 17, 9, 17,
      9, 17, 9, 17, 9, 17, 9, 17,
      9, 17, 9, 17, 9, 17, 9, 17,
      9, 17, 9, 17, 9, 17, 9, 17,
      9, 17, 9, 17, 9, 17, 9, 17,
      9, 17, 9, 17, 9, 17, 9, 17,
      9, 17, 9, 17, 9, 17, 9, 17,
      11, 21, 0, 0, 11, 22, 11, 22,
      11, 22, 11, 22, 11, 22, 11, 22,
      11, 22, 11, 22, 11, 22, 11, 22,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 11, 23,
      11, 23, 11, 23, 11, 24, 11, 24,
      11, 24, 11, 24, 11, 24, 11, 23,
      11, 23, 11, 23, 11, 23, 11, 23,
      11, 23, 11, 23, 11, 23, 11, 23,
      11, 23, 11, 24, 11, 24, 11, 23,
      11, 24, 11, 23, 11, 23, 11, 23,
      11, 23, 0, 0, 0, 0, 0, 0,
      12, 17, 0, 0, 0, 0, 11, 23,
      11, 23, 11, 23, 11, 24, 11, 24,
      11, 24, 11, 24, 11, 24, 11, 23,
      11, 23, 11, 23, 11, 23, 11, 23,
      11, 23, 11, 23, 11, 23, 11, 23,
      11, 23, 11, 24, 11, 24, 11, 23,
      11, 24, 11, 23, 11, 23, 11, 23,
      11, 23, 12, 17, 12, 17, 12, 17,
      12, 17, 12, 17, 12, 17, 12, 17,
      12, 17, 12, 17, 12, 17, 12, 17,
      12, 17, 12, 17, 12, 17, 12, 17,
      12, 17, 12, 17, 12, 17, 12, 17,
      12, 17, 12, 17, 12, 17, 12, 17,
      12, 17, 12, 17, 12, 17, 0, 0,
      0, 0, 0, 0, 13, 25, 12, 17,
      0, 0, 12, 17, 12, 17, 12, 17,
      12, 17, 12, 17, 12, 17, 12, 17,
      12, 17, 12, 17, 12, 17, 12, 17,
      12, 17, 12, 17, 12, 17, 12, 17,
      12, 17, 12, 17, 12, 17, 12, 17,
      12, 17, 12, 17, 12, 17, 12, 17,
      12, 17, 12, 17, 12, 17, 13, 25,
      13, 25, 13, 25, 13, 25, 13, 25,
      13, 25, 13, 25, 13, 25, 13, 25,
      13, 25, 13, 25, 13, 25, 13, 25,
      13, 25, 13, 25, 13, 25, 13, 25,
      13, 25, 13, 25, 13, 25, 13, 25,
      13, 25, 13, 25, 13, 25, 13, 25,
      13, 25, 0, 0, 0, 0, 0, 0,
      15, 26, 13, 25, 0, 0, 13, 25,
      13, 25, 13, 25, 13, 25, 13, 25,
      13, 25, 13, 25, 13, 25, 13, 25,
      13, 25, 13, 25, 13, 25, 13, 25,
      13, 25, 13, 25, 13, 25, 13, 25,
      13, 25, 13, 25, 13, 25, 13, 25,
      13, 25, 13, 25, 13, 25, 13, 25,
      13, 25, 15, 26, 15, 26, 15, 26,
      15, 26, 15, 26, 15, 26, 15, 26,
      15, 26, 15, 26, 15, 26, 15, 26,
      15, 26, 15, 26, 15, 26, 15, 26,
      15, 26, 15, 26, 15, 26, 15, 26,
      15, 26, 15, 26, 15, 26, 15, 26,
      15, 26, 15, 26, 15, 26, 0, 0,
      0, 0, 0, 0, 0, 0, 15, 26,
      0, 0, 15, 26, 15, 26, 15, 26,
      15, 26, 15, 26, 15, 26, 15, 26,
      15, 26, 15, 26, 15, 26, 15, 26,
      15, 26, 15, 26, 15, 26, 15, 26,
      15, 26, 15, 26, 15, 26, 15, 26,
      15, 26, 15, 26, 15, 26, 15, 26,
      15, 26, 15, 26, 15, 26, 16, 27,
      33, 38, 33, 38, 33, 38, 33, 38,
      33, 38, 33, 38, 33, 38, 33, 38,
      33, 38, 33, 38, 0, 0, 16, 27,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 0, 0, 0, 0,
      0, 0, 0, 0, 16, 27, 0, 0,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 16, 27, 16, 27,
      16, 27, 16, 27, 17, 15, 0, 0,
      17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 15, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 19, 29,
      19, 29, 19, 29, 19, 29, 19, 29,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 21, 30, 0, 0,
      0, 0, 0, 0, 19, 29, 19, 29,
      0, 0, 19, 29, 21, 30, 21, 30,
      39, 40, 39, 40, 39, 40, 39, 40,
      39, 40, 39, 40, 39, 40, 39, 40,
      39, 40, 39, 40, 0, 0, 19, 29,
      19, 29, 19, 29, 19, 29, 19, 29,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 21, 30,
      0, 0, 21, 30, 19, 29, 19, 29,
      0, 0, 19, 29, 0, 0, 0, 0,
      21, 30, 0, 0, 0, 0, 21, 0,
      0, 0, 21, 31, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 21, 30,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 21, 30, 0, 0,
      0, 0, 21, 32, 23, 23, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 23, 23, 23, 23, 23, 23,
      23, 23, 24, 33, 0, 0, 24, 33,
      0, 0, 0, 0, 24, 34, 24, 34,
      24, 34, 24, 34, 24, 34, 24, 34,
      24, 34, 24, 34, 24, 34, 24, 34,
      25, 15, 0, 0, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25,
      25, 25, 25, 25, 25, 25, 25, 25,
      25, 35, 26, 15, 0, 0, 26, 26,
      26, 26, 26, 26, 26, 26, 26, 26,
      26, 26, 26, 26, 26, 26, 26, 26,
      26, 26, 26, 15, 29, 33, 0, 0,
      29, 33, 0, 0, 0, 0, 29, 38,
      29, 38, 29, 38, 29, 38, 29, 38,
      29, 38, 29, 38, 29, 38, 29, 38,
      29, 38, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 31, 32, 31, 32, 31, 32,
      31, 32, 31, 32, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      31, 32, 31, 32, 32, 39, 31, 32,
      32, 39, 0, 0, 0, 0, 32, 40,
      32, 40, 32, 40, 32, 40, 32, 40,
      32, 40, 32, 40, 32, 40, 32, 40,
      32, 40, 31, 32, 31, 32, 31, 32,
      31, 32, 31, 32, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 32, 31, 32, 0, 0, 31, 32, 0, 0};
struct yysvf yysvec[] = {
  0, 0, 0,
  yycrank + -1, 0, 0,
  yycrank + -5, yysvec + 1, 0,
  yycrank + 0, 0, yyvstop + 1,
  yycrank + 0, 0, yyvstop + 3,
  yycrank + 0, 0, yyvstop + 6,
  yycrank + 0, 0, yyvstop + 8,
  yycrank + 34, 0, yyvstop + 11,
  yycrank + 1, 0, yyvstop + 15,
  yycrank + 121, 0, yyvstop + 17,
  yycrank + 1, 0, yyvstop + 19,
  yycrank + 198, 0, yyvstop + 21,
  yycrank + 256, 0, yyvstop + 24,
  yycrank + 314, 0, yyvstop + 26,
  yycrank + 0, yysvec + 7, yyvstop + 28,
  yycrank + 372, 0, 0,
  yycrank + 459, 0, 0,
  yycrank + 536, yysvec + 12, yyvstop + 31,
  yycrank + 9, 0, yyvstop + 33,
  yycrank + 547, 0, yyvstop + 35,
  yycrank + 0, 0, yyvstop + 37,
  yycrank + -625, 0, yyvstop + 39,
  yycrank + 0, yysvec + 11, yyvstop + 41,
  yycrank + 646, 0, yyvstop + 43,
  yycrank + 726, yysvec + 23, yyvstop + 45,
  yycrank + 738, yysvec + 13, yyvstop + 47,
  yycrank + 751, yysvec + 15, yyvstop + 49,
  yycrank + 7, yysvec + 16, yyvstop + 51,
  yycrank + 6, yysvec + 15, 0,
  yycrank + 767, 0, 0,
  yycrank + 0, 0, yyvstop + 53,
  yycrank + 777, 0, yyvstop + 55,
  yycrank + 819, 0, 0,
  yycrank + 448, 0, 0,
  yycrank + 557, yysvec + 23, yyvstop + 57,
  yycrank + 2, yysvec + 15, 0,
  yycrank + 10, 0, 0,
  yycrank + 0, 0, yyvstop + 60,
  yycrank + 0, yysvec + 33, yyvstop + 62,
  yycrank + 588, 0, 0,
  yycrank + 0, yysvec + 39, yyvstop + 64,
  yycrank + 0, yysvec + 12, 0,
  0, 0, 0
};

struct yywork *yytop = yycrank + 895;
struct yysvf *yybgin = yysvec + 1;
const unsigned int yymatch[] = {
  00, 01, 01, 01, 01, 01, 01, 01,
  01, 011, 012, 01, 01, 011, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  011, 01, '"', 01, '$', 01, 01, '"',
  01, 01, 01, '+', 01, '+', '.', 01,
  '0', '0', '0', '0', '0', '0', '0', '0',
  '0', '0', ':', 01, 01, 01, 01, 01,
  01, 'A', 'A', 'A', 'D', 'D', 'D', 'D',
  'D', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
  'A', 'A', 'A', 'D', 'D', 'A', 'D', 'A',
  'A', 'A', 'A', 01, 01, 01, 01, '$',
  01, 'A', 'A', 'A', 'D', 'D', 'D', 'D',
  'D', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
  'A', 'A', 'A', 'D', 'D', 'A', 'D', 'A',
  'A', 'A', 'A', 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  01, 01, 01, 01, 01, 01, 01, 01,
  0
};

const unsigned char yyextra[] = {
  0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0
};

/*
 * *****************************************************************
 * *                                                               *
 * *    Copyright (c) Digital Equipment Corporation, 1991, 1999    *
 * *                                                               *
 * *   All Rights Reserved.  Unpublished rights  reserved  under   *
 * *   the copyright laws of the United States.                    *
 * *                                                               *
 * *   The software contained on this media  is  proprietary  to   *
 * *   and  embodies  the  confidential  technology  of  Digital   *
 * *   Equipment Corporation.  Possession, use,  duplication  or   *
 * *   dissemination of the software and media is authorized only  *
 * *   pursuant to a valid written license from Digital Equipment  *
 * *   Corporation.                                                *
 * *                                                               *
 * *   RESTRICTED RIGHTS LEGEND   Use, duplication, or disclosure  *
 * *   by the U.S. Government is subject to restrictions  as  set  *
 * *   forth in Subparagraph (c)(1)(ii)  of  DFARS  252.227-7013,  *
 * *   or  in  FAR 52.227-19, as applicable.                       *
 * *                                                               *
 * *****************************************************************
 */
/*
 * HISTORY
 */
/*
 * @(#)$RCSfile$ $Revision$ (DEC) $Date$
 */
/*
 ** C Language Skeleton for lex output - yytext array
 */
int yylineno = 1;
#define YYU(x) x
#define NLSTATE yyprevious=YYNEWLINE
unsigned char yytext[YYLMAX];
struct yysvf *yylstate[YYLMAX], **yylsp, **yyolsp;
unsigned char yysbuf[YYLMAX];
unsigned char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
/*
 * Define YY_NOPROTO to suppress the prototype declarations
 * GNUC and DECC define __STDC__ differently
 */
#ifdef __GNUC__
#if !__STDC__
#define YY_NOPROTO
#endif				/* __STDC__ */
#elif !defined(__STDC__) &&  !defined (__cplusplus)
#define YY_NOPROTO
#endif				/* __STDC__ */

#ifndef YY_NOPROTO
int yyback(int *, int);
#ifdef LEXDEBUG
#if defined (__cplusplus)
extern "C" {
#endif				/* __cplusplus */
  int allprint(char);
  int sprint(unsigned char *);
#if defined (__cplusplus)
}
#endif				/* __cplusplus */
#endif				/* LEXDEBUG */
#endif				/* YY_NOPROTO */
int yylook()
{
  register struct yysvf *yystate, **lsp;
  register struct yywork *yyt;
  struct yysvf *yyz;
  int yych, yyfirst;
  struct yywork *yyr;
#ifdef LEXDEBUG
  int debug;
#endif
  unsigned char *yylastch;
  /* start off machines */
#ifdef LEXDEBUG
  debug = 0;
#endif
  yyfirst = 1;
  if (!yymorfg)
    yylastch = yytext;
  else {
    yymorfg = 0;
    yylastch = yytext + yyleng;
  }
  for (;;) {
    lsp = yylstate;
    yyestate = yystate = yybgin;
    if (yyprevious == YYNEWLINE)
      yystate++;
    for (;;) {
#ifdef LEXDEBUG
      if (debug)
	fprintf(yyout, "state %d\n", yystate - yysvec - 1);
#endif
      yyt = yystate->yystoff;
      if (yyt == yycrank && !yyfirst) {	/* may not be any transitions */
	yyz = yystate->yyother;
	if (yyz == 0)
	  break;
	if (yyz->yystoff == yycrank)
	  break;
      }
      *yylastch++ = yych = input();
      if (yylastch >= yytext + (YYLMAX - 1)) {
	fprintf(yyout, "Maximum token length exceeded\n");
	yytext[YYLMAX - 1] = 0;
	return 0;
      }
      yyfirst = 0;
 tryagain:
#ifdef LEXDEBUG
      if (debug) {
	fprintf(yyout, "char ");
	allprint(yych);
	putchar('\n');
      }
#endif
      yyr = yyt;
      if (yyt > yycrank) {
	yyt = yyr + yych;
	if (yyt <= yytop && yyt->verify + yysvec == yystate) {
	  if (yyt->advance + yysvec == YYLERR) {	/* error transitions */
	    unput(*--yylastch);
	    break;
	  }
	  *lsp++ = yystate = yyt->advance + yysvec;
	  goto contin;
	}
      }
#ifdef YYOPTIM
      else if (yyt < yycrank) {	/* r < yycrank */
	yyt = yyr = yycrank + (yycrank - yyt);
#ifdef LEXDEBUG
	if (debug)
	  fprintf(yyout, "compressed state\n");
#endif
	yyt = yyt + yych;
	if (yyt <= yytop && yyt->verify + yysvec == yystate) {
	  if (yyt->advance + yysvec == YYLERR) {	/* error transitions */
	    unput(*--yylastch);
	    break;
	  }
	  *lsp++ = yystate = yyt->advance + yysvec;
	  goto contin;
	}
	yyt = yyr + YYU(yymatch[yych]);
#ifdef LEXDEBUG
	if (debug) {
	  fprintf(yyout, "try fall back character ");
	  allprint(YYU(yymatch[yych]));
	  putchar('\n');
	}
#endif
	if (yyt <= yytop && yyt->verify + yysvec == yystate) {
	  if (yyt->advance + yysvec == YYLERR) {	/* error transition */
	    unput(*--yylastch);
	    break;
	  }
	  *lsp++ = yystate = yyt->advance + yysvec;
	  goto contin;
	}
      }
      if ((yystate = yystate->yyother)
	  && (yyt = yystate->yystoff) != yycrank) {
#ifdef LEXDEBUG
	if (debug)
	  fprintf(yyout, "fall back to state %d\n", yystate - yysvec - 1);
#endif
	goto tryagain;
      }
#endif
      else {
	unput(*--yylastch);
	break;
      }
 contin:
#ifdef LEXDEBUG
      if (debug) {
	fprintf(yyout, "state %d char ", yystate - yysvec - 1);
	allprint(yych);
	putchar('\n');
      }
#endif
      ;
    }
#ifdef LEXDEBUG
    if (debug) {
      fprintf(yyout, "stopped at %d with ", *(lsp - 1) - yysvec - 1);
      allprint(yych);
      putchar('\n');
    }
#endif
    while (lsp-- > yylstate) {
      *yylastch-- = 0;
      if (*lsp != 0 && (yyfnd = (*lsp)->yystops) && *yyfnd > 0) {
	yyolsp = lsp;
	if (yyextra[*yyfnd]) {	/* must backup */
	  while (yyback((*lsp)->yystops, -*yyfnd) != 1 && lsp > yylstate) {
	    lsp--;
	    unput(*yylastch--);
	  }
	}
	yyprevious = YYU(*yylastch);
	yylsp = lsp;
	yyleng = yylastch - yytext + 1;
	if (yyleng >= (YYLMAX - 1)) {
	  fprintf(yyout, "Maximum token length exceeded\n");
	  yytext[YYLMAX - 1] = 0;
	  return 0;
	}
	yytext[yyleng] = 0;
#ifdef LEXDEBUG
	if (debug) {
	  fprintf(yyout, "\nmatch ");
	  sprint(yytext);
	  fprintf(yyout, " action %d\n", *yyfnd);
	}
#endif
	return (*yyfnd++);
      }
      unput(*yylastch);
    }
    if (yytext[0] == 0 /* && feof(yyin) */ ) {
      yysptr = yysbuf;
      return (0);
    }
    yyprevious = yytext[0] = input();
    if (yyprevious > 0)
      output(yyprevious);
    yylastch = yytext;
#ifdef LEXDEBUG
    if (debug)
      putchar('\n');
#endif
  }
}

int yyback(int *p, int m)
{
  if (p == 0)
    return (0);
  while (*p) {
    if (*p++ == m)
      return (1);
  }
  return (0);
}

	/* the following are only used in the lex library */
int yyinput()
{
  return (input());
}

void yyoutput(int c)
{
  output(c);
}

void yyunput(int c)
{
  unput(c);
}
