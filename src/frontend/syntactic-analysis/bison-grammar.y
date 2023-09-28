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
	int select_list;
    int table_list;
	int condition;
	int table_column;
	int operation;
	int select_columns;
	int table_name;
	int constant;

	// Terminales.
	token token;
	int integer;


}

// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> SUB
%token <token> MUL
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
%token <token> GROUP
%token <token> ORDER
%token <token> BY
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
%token <token> EVERYTHING
%token <token> COMMA
%token <token> VAR
%token <token> EQUAL
%token <token> APOSTROPHE


%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

%token <integer> INTEGER

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <constant> constant
%type <select_statement> select_statement
%type <select_list> select_list
%type <table_list> table_list
%type <table_name> table_name
%type <condition> condition
%type <table_column> table_column
%type <operation> operation
%type <select_columns> select_columns

// El símbolo inicial de la gramatica.
%start program

%%

program: select_statement																	{ $$ = ProgramGrammarAction($1); }
	;

select_statement: SELECT select_columns FROM table_list WHERE condition						{ $$ = 0; }
				| SELECT select_list FROM table_list										{ $$ = 0; }
		; 



condition: 		table_column GREATER_EQUAL constant											{ $$ = 0; }
			|	table_column GREATER_THAN constant											{ $$ = 0; }
			|	table_column LESS_EQUAL constant											{ $$ = 0; }
			| 	table_column LESS_THAN constant												{ $$ = 0; }
			|	table_column NOT_EQUAL constant												{ $$ = 0; }
			|	table_column EQUAL constant													{ $$ = 0; }
			// | 	condition AND condition														{ $$ = 0; }
			// | 	condition OR condition														{ $$ = 0; }
	;

select_list: select_columns																	{ $$ = 0; }
           | select_columns COMMA select_columns 											{ $$ = 0; }
		   | EVERYTHING																		{ $$ = 0; }
	;

table_list: table_name																		{ $$ = 0; }
          | table_name COMMA table_list 													{ $$ = 0; }
	;

table_column: VAR																			{ $$ = 0; }
	;

operation: select_columns DIV select_columns												{ $$ = 0; }
			| select_columns MUL select_columns												{ $$ = 0; }
			| select_columns ADD select_columns												{ $$ = 0; }
			| select_columns SUB select_columns												{ $$ = 0; }
			| OPEN_PARENTHESIS select_columns CLOSE_PARENTHESIS								{ $$ = 0; }
			| OPEN_PARENTHESIS operation CLOSE_PARENTHESIS									{ $$ = 0; }
			| OPEN_PARENTHESIS select_columns CLOSE_PARENTHESIS	AS VAR						{ $$ = 0; }
			| select_columns AS VAR															{ $$ = 0; }
	;

select_columns: table_column 																{ $$ = 0; }
	| AVG OPEN_PARENTHESIS table_column CLOSE_PARENTHESIS									{ $$ = 0; }
	| SUM OPEN_PARENTHESIS table_column CLOSE_PARENTHESIS									{ $$ = 0; }
	| COUNT OPEN_PARENTHESIS table_column CLOSE_PARENTHESIS									{ $$ = 0; }
	| MAX OPEN_PARENTHESIS table_column CLOSE_PARENTHESIS									{ $$ = 0; }
	| MIN OPEN_PARENTHESIS table_column CLOSE_PARENTHESIS									{ $$ = 0; }
	;

table_name: VAR 																			{ $$ = 0; }
	;

constant: INTEGER																			{ $$ = 0; }
		| APOSTROPHE constant APOSTROPHE													{ $$ = 0; }
	;

%%
