%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	int program;
	int select_statement;
	int columns;
	int column;
	int aggregation;
	int from_statement;
	int tables;
	int table;
	int where_statement;
	int where_condition;
	int having_statement;
	int having_condition;
	int group_by_statement;
	int order_by_statement;
	int constant;
	int operator;
    // int table_list;
	// int condition;
	
	// int select_columns;
	// int table_name;
	


	// Terminales.
	token token;
	int integer;


}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> SUB
%token <token> AST
%token <token> DIV
%token <token> ADD


%token <token> SELECT
%token <token> FROM
%token <token> WHERE
%token <token> GREATER_EQUAL
%token <token> GREATER_THAN
%token <token> LESS_EQUAL
%token <token> LESS_THAN
%token <token> NOT_EQUAL
%token <token> AND
%token <token> OR
%token <token> NOT
%token <token> IN
%token <token> GROUPBY
%token <token> ORDERBY
%token <token> JOIN
%token <token> NATURAL
%token <token> LEFT
%token <token> RIGHT
%token <token> OUTER
%token <token> ALL
%token <token> SUM
%token <token> COUNT
%token <token> AVG
%token <token> MAX
%token <token> MIN
%token <token> NULL_VAL
%token <token> IS
%token <token> AS
%token <token> COMMA
%token <token> VAR
%token <token> EQUAL
%token <token> APOSTROPHE
%token <token> DOT
%token <token> ON
%token <token> HAVING



%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

%token <integer> INTEGER

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <constant> constant
%type <select_statement> select_statement
%type <columns> columns
%type <column> column
%type <aggregation> aggregation
%type <from_statement> from_statement
%type <tables> tables
%type <table> table
%type <where_statement> where_statement
%type <where_condition> where_condition
%type <having_statement> having_statement
%type <having_condition> having_condition
%type <group_by_statement> group_by_statement
%type <order_by_statement> order_by_statement
%type <operator> operator


// %type <table_list> table_list
// %type <table_name> table_name
// %type <condition> condition

// %type <select_columns> select_columns

// Reglas de asociatividad y precedencia (menor a mayor)
%left ADD SUB 
%left AST DIV
%left AND OR
%left GREATER_EQUAL GREATER_THAN LESS_EQUAL LESS_THAN NOT_EQUAL EQUAL
%left IN
%left NOT
%left IS
%left ALL
%left SUM COUNT AVG MAX MIN
%left COMMA
%left OPEN_PARENTHESIS CLOSE_PARENTHESIS
%left ON
%left AS
%left JOIN NATURAL LEFT RIGHT OUTER 
%left GROUPBY ORDERBY
%left HAVING
%left DOT







// El símbolo inicial de la gramatica.
%start program

%%

program: select_statement from_statement where_statement group_by_statement having_statement order_by_statement				{ $$ = ProgramGrammarAction($1); }
		;															
	
	

select_statement: SELECT columns 														{ $$ = 0; }
		; 

columns:	column COMMA columns														{ $$ = 0; }
		   | column 																	{ $$ = 0; }
	;

column:  	constant										                            { $$ = 0; }					
		|	aggregation OPEN_PARENTHESIS constant CLOSE_PARENTHESIS						{ $$ = 0; }
		|   column ADD column														{ $$ = 0; }
		|   column DIV column														{ $$ = 0; }
		|   column AST column														{ $$ = 0; }
		|   column SUB column														{ $$ = 0; }
		| 	OPEN_PARENTHESIS column CLOSE_PARENTHESIS AS VAR							{ $$ = 0; }
		;

		
aggregation: 	SUM																		{ $$ = 0; }
				|AVG																	{ $$ = 0; }
				|MAX																	{ $$ = 0; }
				|MIN																	{ $$ = 0; }
				|COUNT																	{ $$ = 0; }
	;




from_statement: FROM tables																{ $$ = 0; }
		;

tables: table COMMA tables																{ $$ = 0; }
		| table																			{ $$ = 0; }
		| table JOIN tables ON where_condition											{ $$ = 0; }
		| table NATURAL JOIN tables														{ $$ = 0; }
		| table LEFT OUTER JOIN tables ON where_condition								{ $$ = 0; }
		| table RIGHT OUTER JOIN tables ON where_condition								{ $$ = 0; }
		| table OUTER JOIN tables ON where_condition									{ $$ = 0; }	
		| table LEFT JOIN tables ON where_condition										{ $$ = 0; }
		| table RIGHT JOIN tables ON where_condition									{ $$ = 0; }
		| OPEN_PARENTHESIS tables CLOSE_PARENTHESIS										{ $$ = 0; }
	;

table: VAR																				{ $$ = 0; }
	;


where_statement: WHERE where_condition													{ $$ = 0; }
				|																{ $$ = 0; }
		;

operator: GREATER_EQUAL																	{ $$ = 0; }
		| GREATER_THAN																	{ $$ = 0; }
		| LESS_EQUAL																	{ $$ = 0; }
		| LESS_THAN																		{ $$ = 0; }
		| NOT_EQUAL																		{ $$ = 0; }
		| EQUAL																			{ $$ = 0; }	
	;

where_condition: constant operator constant												{ $$ = 0; }
			| 	constant operator ALL OPEN_PARENTHESIS program CLOSE_PARENTHESIS		{ $$ = 0; }
			|	constant IS NULL_VAL													{ $$ = 0; }
			|	constant IS NOT NULL_VAL												{ $$ = 0; }
			|	constant IN OPEN_PARENTHESIS program CLOSE_PARENTHESIS					{ $$ = 0; }
			|	constant NOT IN OPEN_PARENTHESIS program CLOSE_PARENTHESIS				{ $$ = 0; }
			| 	where_condition AND where_condition										{ $$ = 0; }
			| 	where_condition OR where_condition										{ $$ = 0; }
	;
	
having_condition:column operator constant											{ $$ = 0; }
			|	column IS NULL_VAL														{ $$ = 0; }
			|	column IS NOT NULL_VAL													{ $$ = 0; }
			|	column IN OPEN_PARENTHESIS program CLOSE_PARENTHESIS			{ $$ = 0; }
			|	column NOT IN OPEN_PARENTHESIS program CLOSE_PARENTHESIS		{ $$ = 0; }
			| 	having_condition AND having_condition									{ $$ = 0; }
			| 	having_condition OR having_condition									{ $$ = 0; }
			| 	column operator ALL OPEN_PARENTHESIS program CLOSE_PARENTHESIS		{ $$ = 0; }
	;

group_by_statement: GROUPBY columns													{ $$ = 0; }
				| 																{ $$ = 0; }
		;

having_statement: HAVING having_condition												{ $$ = 0; }
				| 																{ $$ = 0; }
		;

order_by_statement: ORDERBY columns													{ $$ = 0; }
				| 																{ $$ = 0; }
		;


constant: INTEGER																			{ $$ = 0; }
		| APOSTROPHE VAR APOSTROPHE															{ $$ = 0; }
		| VAR																				{ $$ = 0; }
		| VAR DOT VAR																		{ $$ = 0; }
		| AST																				{ $$ = 0; }	
	;

%%
