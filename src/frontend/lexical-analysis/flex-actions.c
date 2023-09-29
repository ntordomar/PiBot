#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <stdlib.h>
#include <string.h>

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

char * copyLexeme(const char * lexeme, const int length) {
	char * lexemeCopy = (char *) calloc(length + 1, sizeof(char));
	strncpy(lexemeCopy, lexeme, length);
	return lexemeCopy;
}

void BeginCommentPatternAction() {
	LogDebug("[Flex] [COMMENT] BeginCommentPatternAction............................");
}

void EndCommentPatternAction() {
	LogDebug("[Flex] [COMMENT] EndCommentPatternAction..............................");
}

token CloseParenthesisPatternAction() {
	LogDebug("[Flex] CloseParenthesisPatternAction: ')'.");
	yylval.token = CLOSE_PARENTHESIS;
	return CLOSE_PARENTHESIS;
}

token IntegerPatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] IntegerPatternAction: '%s' (length = %d).", lexeme, length);
	char * lexemeCopy = copyLexeme(lexeme, length);
	yylval.integer = atoi(lexemeCopy);
	free(lexemeCopy);
	return INTEGER;
}

token LetterPatternAction(const char * lexeme, const int length){
	LogDebug("[Flex] LetterPatternAction '%s' (len = %d).",lexeme,length);
	return VAR;
}


token OpenParenthesisPatternAction() {
	LogDebug("[Flex] OpenParenthesisPatternAction: '('.");
	yylval.token = OPEN_PARENTHESIS;
	return OPEN_PARENTHESIS;
}

token UnknownPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] UnknownPatternAction: '%s' (length = %d).", lexemeCopy, length);
	free(lexemeCopy);
	yylval.token = ERROR;
	// Al emitir este token, el compilador aborta la ejecución.
	return ERROR;
}

void IgnoredPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogRaw("[DEBUG] [Flex] IgnoredPatternAction: '");
	LogText(lexemeCopy, length);
	LogRaw("' (length = %d).\n", length);
	free(lexemeCopy);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
	// No se emite ningún token.
}

token SelectPatternAction() {
    LogDebug("[Flex] SelectPatternAction: 'SELECT'.");
    yylval.token = SELECT;
    return SELECT;
}

token FromPatternAction() {
    LogDebug("[Flex] FromPatternAction: 'FROM'.");
    yylval.token = FROM;
    return FROM;
}

token WherePatternAction() {
    LogDebug("[Flex] WherePatternAction: 'WHERE'.");
    yylval.token = WHERE;
    return WHERE;
}

token GoEPatternAction() {
    LogDebug("[Flex] GoEPatternAction: '>='.");
    yylval.token = GREATER_EQUAL;
    return GREATER_EQUAL;
}

token GreaterPatternAction() {
    LogDebug("[Flex] GreaterPatternAction: '>'.");
    yylval.token = GREATER_THAN;
    return GREATER_THAN;
}

token LoEPatternAction() {
    LogDebug("[Flex] LoEPatternAction: '<='.");
    yylval.token = LESS_EQUAL;
    return LESS_EQUAL;
}

token LessPatternAction() {
    LogDebug("[Flex] LessPatternAction: '<'.");
    yylval.token = LESS_THAN;
    return LESS_THAN;
}

token NotEqualPatternAction() {
    LogDebug("[Flex] NotEqualPatternAction: '<>'.");
    yylval.token = NOT_EQUAL;
    return NOT_EQUAL;
}

token AndPatternAction() {
    LogDebug("[Flex] AndPatternAction: 'AND'.");
    yylval.token = AND;
    return AND;
}

token OrPatternAction() {
    LogDebug("[Flex] OrPatternAction: 'OR'.");
    yylval.token = OR;
    return OR;
}

token NotPatternAction() {
    LogDebug("[Flex] NotPatternAction: 'NOT'.");
    yylval.token = NOT;
    return NOT;
}

token InPatternAction() {
    LogDebug("[Flex] InPatternAction: 'IN'.");
    yylval.token = IN;
    return IN;
}

token GroupPatternAction() {
    LogDebug("[Flex] GroupPatternAction: 'GROUP BY'.");
    yylval.token = GROUPBY;
    return GROUPBY;
}

token OrderPatternAction() {
    LogDebug("[Flex] OrderPatternAction: 'ORDER BY'.");
    yylval.token = ORDERBY;
    return ORDERBY;
}

token JoinPatternAction() {
    LogDebug("[Flex] JoinPatternAction: 'JOIN'.");
    yylval.token = JOIN;
    return JOIN;
}

token NaturalPatternAction() {
    LogDebug("[Flex] NaturalPatternAction: 'NATURAL'.");
    yylval.token = NATURAL;
    return NATURAL;
}

token LeftPatternAction() {
    LogDebug("[Flex] LeftPatternAction: 'LEFT'.");
    yylval.token = LEFT;
    return LEFT;
}

token RightPatternAction() {
    LogDebug("[Flex] RightPatternAction: 'RIGHT'.");
    yylval.token = RIGHT;
    return RIGHT;
}

token OuterPatternAction() {
    LogDebug("[Flex] OuterPatternAction: 'OUTER'.");
    yylval.token = OUTER;
    return OUTER;
}

token AllPatternAction() {
    LogDebug("[Flex] AllPatternAction: 'ALL'.");
    yylval.token = ALL;
    return ALL;
}

token SumPatternAction() {
    LogDebug("[Flex] SumPatternAction: 'SUM'.");
    yylval.token = SUM;
    return SUM;
}

token CountPatternAction() {
    LogDebug("[Flex] CountPatternAction: 'COUNT'.");
    yylval.token = COUNT;
    return COUNT;
}

token AvgPatternAction() {
    LogDebug("[Flex] AvgPatternAction: 'AVG'.");
    yylval.token = AVG;
    return AVG;
}

token MaxPatternAction() {
    LogDebug("[Flex] MaxPatternAction: 'MAX'.");
    yylval.token = MAX;
    return MAX;
}

token MinPatternAction() {
    LogDebug("[Flex] MinPatternAction: 'MIN'.");
    yylval.token = MIN;
    return MIN;
}

token NullPatternAction() {
    LogDebug("[Flex] NullPatternAction: 'NULL'.");
    yylval.token = NULL_VAL;
    return NULL_VAL;
}

token IsPatternAction() {
    LogDebug("[Flex] IsPatternAction: 'IS'.");
    yylval.token = IS;
    return IS;
}

token AsPatternAction() {
    LogDebug("[Flex] AsPatternAction: 'AS'.");
    yylval.token = AS;
    return AS;
}


token CommaPatternAction() {
    LogDebug("[Flex] CommaPatternAction: ','.");
    yylval.token = COMMA;
    return COMMA;
}

token EqualPatternAction(){
    LogDebug("[Flex] EqualPatternAction: '='.");
    yylval.token = EQUAL;
    return EQUAL;
}

token AsteriskPatternAction(){
    LogDebug("[Flex] MulPatternAction: '*'.");
    yylval.token = AST;
    return AST;
}

token DivPatternAction(){
    LogDebug("[Flex] DivPatternAction: '/'.");
    yylval.token = DIV;
    return DIV;
}

token AddPatternAction(){
    LogDebug("[Flex] AddPatternAction: '+'.");
    yylval.token = ADD;
    return ADD;
}

token SubPatternAction(){
    LogDebug("[Flex] AddPatternAction: '-'.");
    yylval.token = SUB;
    return SUB;
}

token  ApstrophePatternAction(){
    LogDebug("[Flex] ApostrophePatterAction: '''.");
    yylval.token = APOSTROPHE;
    return APOSTROPHE; 
}

token DotPatternAction() {
    LogDebug("[Flex] ApostrophePatterAction: '.'.");
    yylval.token = DOT;
    return DOT; 
}

token OnPatternAction(){
    LogDebug("[Flex] OnPatternAction: 'ON'.");
    yylval.token = ON;
    return ON;
}

token HavingPatternAction(){
    LogDebug("[Flex] HavingPatternAction: 'HAVING'.");
    yylval.token = HAVING;
    return HAVING;
}
