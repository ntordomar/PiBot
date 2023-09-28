%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT

/* Patrones reutilizables. */
digit			[0-9]
letter          [A-Za-z]
endline			[\n]
whitespace		[ \f\n\r\t\v]

%%

"--"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.       							{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}							{ EndCommentPatternAction(); BEGIN(INITIAL); }

"SELECT"                                    { return SelectPatternAction(); }
"FROM"                                      { return FromPatternAction(); }
"WHERE"                                     { return WherePatternAction(); }
">="                                        { return GoEPatternAction(); }
">"                                         { return GreaterPatternAction(); }
"<="                                        { return LoEPatternAction(); }
"<"                                         { return LessPatternAction(); }
"<>"                                        { return NotEqualPatternAction();}
"="                                         { return EqualPatternAction();}
"/"                                         { return DivPatternAction(); }
"*"                                         { return MulPatternAction(); }
"+"                                         { return AddPatternAction(); }
"-"                                         { return SubPatternAction(); }
"AND"                                       { return AndPatternAction(); }
"OR"                                        { return OrPatternAction(); }
"NOT"                                       { return NotPatternAction(); }
"IN"                                        { return InPatternAction(); }
"GROUP"                                     { return GroupPatternAction(); }
"ORDER"                                     { return OrderPatternAction(); }
"BY"                                        { return ByPatternAction(); }
"JOIN"                                      { return JoinPatternAction(); }
"NATURAL"                                   { return NaturalPatternAction(); }
"LEFT"                                      { return LeftPatternAction(); }
"RIGHT"                                     { return RightPatternAction(); }
"OUTER"                                     { return OuterPatternAction(); }
"ALL"                                       { return AllPatternAction(); }
"SUM"                                       { return SumPatternAction(); }
"COUNT"                                     { return CountPatternAction(); }
"AVG"                                       { return AvgPatternAction(); }
"MAX"                                       { return MaxPatternAction(); }
"MIN"                                       { return MinPatternAction(); }
"NULL"                                      { return NullPatternAction(); }
"IS"                                        { return IsPatternAction(); }
"AS"                                        { return AsPatternAction(); }
"*"                                         { return EverythingPatternAction(); }
","                                         { return CommaPatternAction(); }
"("                                         { return OpenParenthesisPatternAction(); }
")"                                         { return CloseParenthesisPatternAction();}
"'"                                         { return ApstrophePatternAction();}

{digit}*									{ return IntegerPatternAction(yytext, yyleng); }
{letter}+                                   { return LetterPatternAction(yytext, yyleng); }
{whitespace}+                               { IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
