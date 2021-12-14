%{
#include "AcExpr.h"
#include "scanner.h"
#include "parser.h"
#include <math.h>
%}

%define api.pure full
%locations
%defines "parser.h"
%debug
%output "parser.c"
%expect 1

%lex-param { yyscan_t yyscanner }
%parse-param { yyscan_t yyscanner }
%lex-param { AcExprEvalResult* ptr }
%parse-param { AcExprEvalResult* ptr }

%code provides {
   YY_DECL;
   void yyerror (YYLTYPE* loc, yyscan_t yyscanner, AcExprEvalResult* ptr, const char* msg);
}

%token FLOAT_VALUE
%token NUMBER
%token LITERAL
%token OBJ_ID CELL_ID
%token UMINUS OBJECT TABLE EVALUATE PI E EXP10 PLD PLT DPL DPP ROT ILP
%token NOR VEC VEC1 ILL DIST ANG
%left '-' '+' 
%left '*' '/' '&'
%right '^'
%left '.'
%left ','
%nonassoc UMINUS

%%

start: Expr { ptr->result = acexprCopyValue($1, ptr); }

Expr: LITERAL '(' Expr ')' 			{ $$ = Functor($1, $3, ptr); if (!ptr->success) YYERROR; }
  | EXP10 '(' Expr ')' 				{ $$ = Exp10($3, ptr); if (!ptr->success) YYERROR; }
  | DIST '(' Expr ',' Expr ')'      { $$ = Distance($3, $5, ptr); if (!ptr->success) YYERROR; }
  | ANG '(' Expr ')'       			{ $$ = Angle1($3, ptr); if (!ptr->success) YYERROR;}
  | ANG '(' Expr ',' Expr ')'       { $$ = Angle2($3, $5, ptr); if (!ptr->success) YYERROR;}
  | ANG '(' Expr ',' Expr ',' Expr ')' { $$ = Angle3($3, $5, $7, ptr); if (!ptr->success) YYERROR;}
  | ANG '(' Expr ',' Expr ',' Expr ',' Expr ')' { $$ = Angle4($3, $5, $7, $9, ptr); if (!ptr->success) YYERROR;}
  | ROT '(' Expr ',' Expr ',' Expr ')' { $$ = Rot3($3, $5, $7, ptr); if (!ptr->success) YYERROR;}
  | ROT '(' Expr ',' Expr ',' Expr ',' Expr ')' { $$ = Rot4($3, $5, $7, $9, ptr); if (!ptr->success) YYERROR;}
  | PI                              { $$ = acexprCreateDouble2(3.14159265358979323846, ptr); }
  | E                               { $$ = acexprCreateDouble2(0.0, ptr); /*E is interpreted as 0.0E+0*/}
  | FLOAT_VALUE                     { $$ = $1; }
  | OBJECT '(' OBJ_ID ')' '.' LITERAL 
    { $$ = ObjectEval($3, $6, ptr); if (!ptr->success) YYERROR;}
  | '-' Expr %prec UMINUS           { $$ = Neg($2, ptr); if (!ptr->success) YYERROR;}
  | '+' Expr %prec UMINUS           { $$ = $2; }
  | '(' Expr ')'                    { $$ = $2; }
  | Expr '*' Expr                   { $$ = Mul($1, $3, ptr); if (!ptr->success) YYERROR;}
  | Expr '/' Expr                   { $$ = Div($1, $3, ptr); if (!ptr->success) YYERROR;}
  | Expr '+' Expr                   { $$ = Add($1, $3, ptr); if (!ptr->success) YYERROR;}
  | Expr '-' Expr                   { $$ = Sub($1, $3, ptr); if (!ptr->success) YYERROR;}
  | Expr '^' Expr					{ $$ = Pow($1, $3, ptr); if (!ptr->success) YYERROR;}
  | TABLE '(' OBJ_ID ')' '.' EVALUATE { acexprPushImplicitTable($3, ptr); } '(' Expr ')' 
    { $$ = $9; acexprPopImplicitTable(ptr); if (!ptr->success) YYERROR;}
  | TABLE '(' OBJ_ID ')' '.' CELL_ID
    { $$ = TableCell($3, $6, ptr); if (!ptr->success) YYERROR;}
  | LITERAL '(' TableSequence ')' 
    { $$ = ImplicitTableEval($1, $3, ptr); if (!ptr->success) YYERROR;}
  | CELL_ID {$$ = ImplicitTableCell($1, ptr); if (!ptr->success) YYERROR;}
  | NUMBER                          { $$ = $1; }

  | VEC '(' Expr ',' Expr ')'   { $$ = Vec($3, $5, ptr); if (!ptr->success) YYERROR; }
  | VEC1 '(' Expr ',' Expr ')'  { $$ = UnitVec($3, $5, ptr); if (!ptr->success) YYERROR; }
  | NOR '(' Expr ',' Expr ')'   { $$ = Normal($3, $5, ptr); if (!ptr->success) YYERROR; }
  | PLD '(' Expr ',' Expr ',' Expr ')' { $$ = Pld($3, $5, $7, ptr); if (!ptr->success) YYERROR; }
  | PLT '(' Expr ',' Expr ',' Expr ')' { $$ = Plt($3, $5, $7, ptr); if (!ptr->success) YYERROR; }
  | DPL '(' Expr ',' Expr ',' Expr ')' { $$ = Dpl($3, $5, $7, ptr); if (!ptr->success) YYERROR; }
  | DPP '(' Expr ',' Expr ',' Expr ',' Expr ')' { $$ = Dpp($3, $5, $7, $9, ptr); if (!ptr->success) YYERROR; }
  | ILL '(' Expr ',' Expr ',' Expr ',' Expr ')' { $$ = Ill($3, $5, $7, $9, ptr); if (!ptr->success) YYERROR;}
  | ILP '(' Expr ',' Expr ',' Expr ',' Expr ',' Expr ')' { $$ = Ilp($3, $5, $7, $9, $11, ptr); if (!ptr->success) YYERROR;}
  | Vector { $$ = $1; }
  | Expr '&' Expr { $$ = CrossProduct($1, $3, ptr); if (!ptr->success) YYERROR; }
  ;

Vector: '[' VectorFlags VectorComponent ',' VectorComponent ',' VectorComponent ']'   
    { $$ = Vector($3, $5, $7, $2, ptr); if (!ptr->success) YYERROR; }
  | '[' VectorFlags VectorComponent ',' VectorComponent ']'       
    { $$ = Vector($3, $5, acexprCreateDouble2(0.0, ptr), $2, ptr); if (!ptr->success) YYERROR; }
  | '[' VectorFlags VectorComponent  ']'                          
    { $$ = Vector($3, acexprCreateDouble2(0.0, ptr), acexprCreateDouble2(0.0, ptr), $2, ptr); }
  | '[' VectorFlags VectorComponent '<' VectorComponent ']'       
    { $$ = PolarVector($3, $5, acexprCreateDouble2(0.0, ptr), $2, ptr); if (!ptr->success) YYERROR; }
  | '[' VectorFlags VectorComponent '<' VectorComponent ',' VectorComponent ']'      
    { $$ = PolarVector($3, $5, $7, $2, ptr); if (!ptr->success) YYERROR; }
  | '[' VectorFlags VectorComponent '<' VectorComponent '<' VectorComponent ']'      
    { $$ = SphereVector($3, $5, $7, $2, ptr); if (!ptr->success) YYERROR; }
;

TableRange: CELL_ID ':' CELL_ID { $$ = acexprCreateTableRange($1, $3, ptr); if (!ptr->success) YYERROR; }

TableSequenceElement: TableRange
   | Expr { $$ = acexprConvertToRange($1, ptr); }

TableSequence: TableSequenceElement
   | TableSequence ',' TableSequenceElement { $$ = acexprCreateTableSequence($1, $3, ptr); if (!ptr->success) YYERROR; }

VectorComponent:                  { $$ = acexprCreateDouble2(0.0, ptr); }
  | Expr                          { $$ = $1; }
  ;
VectorFlags:                      { $$ = acexprCreateInteger2(0, ptr); }
  | '@'                           { $$ = acexprCreateInteger2(1, ptr); }
  | '*'                           { $$ = acexprCreateInteger2(2, ptr); }
  | '@' '*'                       { $$ = acexprCreateInteger2(3, ptr); }
  ;

%%

void yyerror (YYLTYPE* loc, yyscan_t yyscanner, AcExprEvalResult* ptr, const char* msg)
{
#ifdef _DEBUG
  printf("Parse error %s, at %d", msg, loc->first_column);
#endif
}
