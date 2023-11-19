#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../syntactic-analysis/bison-parser.h"

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 *
 * Debería existir una acción por cada patrón disponible en Flex (es decir,
 * por cada regla).
 *
 * Solo aquellas acciones que tengan interés en conocer el largo del lexema
 * emplean el parámetro adicional "length". Esto es útil cuando se desea
 * asignar memoria dinámica, y por lo tanto, se debe saber exactamente el
 * largo de aquello que se va a almacenar.
 */

// Bloque de comentarios 
void BeginCommentPatternAction();
void EndCommentPatternAction();

void IgnoredPatternAction(const char* text, int length);
token SelectPatternAction();
token FromPatternAction();
token WherePatternAction();
token GoEPatternAction();
token GreaterPatternAction();
token LoEPatternAction();
token LessPatternAction();
token NotEqualPatternAction();
token AndPatternAction();
token OrPatternAction();
token NotPatternAction();
token InPatternAction();
token GroupPatternAction();
token OrderPatternAction();
token JoinPatternAction();
token NaturalPatternAction();
token LeftPatternAction();
token RightPatternAction();
token OuterPatternAction();
token AllPatternAction();
token SumPatternAction();
token CountPatternAction();
token AvgPatternAction();
token MaxPatternAction();
token MinPatternAction();
token NullPatternAction();
token IsPatternAction();
token AsPatternAction();
token CommaPatternAction();
token OpenParenthesisPatternAction();
token CloseParenthesisPatternAction();
token EqualPatternAction();
token IntegerPatternAction(const char* text, int length);
token LetterPatternAction(const char* text, int length);
token AsteriskPatternAction();
token DivPatternAction();
token AddPatternAction();
token SubPatternAction();
token ApstrophePatternAction();
token DotPatternAction();
token OnPatternAction();
token HavingPatternAction();
token ByPatternAction();

// Patrón desconocido, permite abortar debido a un error de sintaxis.
token UnknownPatternAction(const char * lexeme, const int length);

// Regla que no hace nada, permite ignorar parte de la entrada.
void IgnoredPatternAction(const char * lexeme, const int length);


#endif