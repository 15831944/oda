%top{
#if (_MSC_VER >= 1600) || !defined(_MSC_VER)
#include <stdint.h>
#endif
}
%{
#include "AcExpr.h"
#include "parser.h"
%}
%option reentrant noyywrap never-interactive
%option outfile="scanner.c" header-file="scanner.h"

digit [0-9]
letter [a-z]
whitespace [' ' '\t' '\n' '\r']
exponent [Ee][+-]?{digit}+
float_value ({digit}*\.{digit}+{exponent})|({digit}+\.{exponent})|({digit}+{exponent})|({digit}*\.{digit}+)|({digit}+\.{digit}*)
simple_float {digit}+(\.{digit}*)?
int_value {digit}+
value {float_value}|{int_value}
arch_value {int_value}\'(\-?|{whitespace}?){value}({whitespace}+{int_value}+\/{int_value})?\"
%%

{whitespace}                    {}
"."                             { return *yytext; }
"object"                        { return OBJECT; }
"table"                         { return TABLE; }
"nor"                           { return NOR;}
"vec"                           { return VEC;}
"vec1"                          { return VEC1;}
"ill"                           { return ILL;}
"dist"                          { return DIST;}
"ang"                           { return ANG;}
"pi"							{ return PI;}
"evaluate"                      { return EVALUATE;}
"pld"							{ return PLD;}
"plt"							{ return PLT;}
"dpl"							{ return DPL;}
"dpp"							{ return DPP;}
"exp10"							{ return EXP10;}
"rot"							{ return ROT;}
"ilp"							{ return ILP;}
\$*{letter}+\$*{digit}+         { *yylval = acexprParseLiteral(yytext, yyleng, ptr); return CELL_ID; }
({letter}|\_)+({letter}|{digit}|\_)*	{ *yylval = acexprParseLiteral(yytext, yyleng, ptr); return LITERAL;}
{float_value}                   { *yylval = acexprParseDouble(yytext, yyleng, ptr); return FLOAT_VALUE;}
{arch_value}					{ *yylval = acexprParseArch(yytext, yyleng, ptr); return FLOAT_VALUE;}
{int_value}                     { *yylval = acexprParseInteger(yytext, yyleng, ptr); return NUMBER;}
[NnSs]{whitespace}?{simple_float}([dD]({simple_float}\'({simple_float}\")?)?)?{whitespace}?[WwEe] { *yylval = acexprParseAngle(yytext, yyleng, ptr); return FLOAT_VALUE; }
{value}[dD]({value}\'({value}\")?)?                                                               { *yylval = acexprParseAngle(yytext, yyleng, ptr); return FLOAT_VALUE; }
{value}[gGrR]                                                                                     { *yylval = acexprParseAngle(yytext, yyleng, ptr); return FLOAT_VALUE; }
[+\-*/\^\,:\(\)\[\]\<\@\%\>\#\&\=]  { return *yytext; }
"%<\\_objid"{whitespace}*{int_value}{whitespace}*">%"     { *yylval = acexprParseObjRef(yytext, yyleng, ptr); return OBJ_ID; }
%%
