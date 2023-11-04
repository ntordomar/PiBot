%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	Program * program;
	Constant * constant;
	Select_statement * select_statement;
	Columns * columns;
	Column * column;
	AggregationType aggregation;
	From_statement * from_statement;
	Tables * tables;
	Table * table;
	Where_statement * where_statement;
	Where_condition * where_condition;
	Having_statement * having_statement;
	Having_condition * having_condition;
	Group_by_statement * group_by_statement;
	Order_by_statement * order_by_statement;
	OperatorType operator;
	Array * array;

	// No-terminales (frontend).
//	int program;
//	int constant;
//	int select_statement;
//	int columns;
//	int column;
//	int aggregation;
//	int from_statement;
//	int tables;
//	int table;
//	int where_statement;
//	int where_condition;
//	int having_statement;
//	int having_condition;
//	int group_by_statement;
//	int order_by_statement;
//	int operator;
//	int array;
	


	// Terminales.
	token token;
	int integer;
	char * varname;

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
%token <varname> VAR
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
%type <array> array


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

program: select_statement[select] from_statement[from] where_statement[where] group_by_statement[groupby] having_statement[having] order_by_statement[orderby]	{ $$ = ProgramGrammarAction($select, $from, $where, $groupby, $having, $orderby); }
		;															
	

select_statement: SELECT columns[cols] 														{ $$ = SelectStatementGrammarAction($cols); }
		; 

columns: column[col] COMMA columns[cols]													{ $$ = ColumnsGrammarAction($col, $cols); }
		| column[col] 																		{ $$ = ColumnsGrammarAction($col, NULL); }
	;

column:  	constant[con]										                            { $$ = UniqueColumnGrammarAction($con); }					
		|	aggregation[agg] OPEN_PARENTHESIS constant[con] CLOSE_PARENTHESIS				{ $$ = AggregationColumnGrammarAction($agg, $con); }
		|   column[left] ADD column[right]													{ $$ = AdditionColumnGrammarAction($left, $right); }
		|   column[left] DIV column[right]													{ $$ = DivisionColumnGrammarAction($left, $right); }
		|   column[left] AST column[right]													{ $$ = MultiplicationColumnGrammarAction($left, $right); }
		|   column[left] SUB column[right]													{ $$ = SubstractionColumnGrammarAction($left, $right); }
		| 	OPEN_PARENTHESIS column[col] CLOSE_PARENTHESIS AS VAR[var]						{ $$ = AssignmentColumnGrammarAction($col, $var); }
		;

		
aggregation: 	SUM																			{ $$ = SUM_AGG; }
				|AVG																		{ $$ = AVG_AGG; }
				|MAX																		{ $$ = MAX_AGG; }
				|MIN																		{ $$ = MIN_AGG; }
				|COUNT																		{ $$ = COUNT_AGG; }
	;




from_statement: FROM tables[tbls]																{ $$ = FormStatementGrammarAction($tbls); }
		;

tables: tables[left] COMMA tables[right]														{ $$ = MultipleTablesGrammarAction($left, $right); }
		| table[tbl]																			{ $$ = UniqueTablesGrammarAction($tbl); }
		| tables[left] JOIN tables[right] ON where_condition[where]								{ $$ = JoinOnGrammarAction($left, $right, $where); }
		| tables[left] NATURAL JOIN tables[right]												{ $$ = NaturalJoinGrammarAction($left, $right); }
		| tables[left] LEFT OUTER JOIN tables[right] ON where_condition[where]					{ $$ = LeftOuterJoinOnGrammarAction($left, $right, $where); }
		| tables[left] RIGHT OUTER JOIN tables[right] ON where_condition[where]					{ $$ = RightOuterJoinOnGrammarAction($left, $right, $where); }
		| tables[left] OUTER JOIN tables[right] ON where_condition[where]						{ $$ = OuterJoinOnGrammarAction($left, $right, $where); }	
		| tables[left] LEFT JOIN tables[right] ON where_condition[where]						{ $$ = LeftOnGrammarAction($left, $right, $where); }
		| tables[left] RIGHT JOIN tables[right] ON where_condition[where]						{ $$ = RightOnGrammarAction($left, $right, $where); }
		| OPEN_PARENTHESIS tables[tbls] CLOSE_PARENTHESIS										{ $$ = ParenthesisTablesGrammarAction($tbls); }
		| OPEN_PARENTHESIS program[prog] CLOSE_PARENTHESIS										{ $$ = NestedQueryTablesGrammarAction($prog); }
		| tables[tbls] AS VAR[var]																{ $$ = AssignmentTablesGrammarAction($tbls, $var); }
	;

table: VAR[var]																					{ $$ = TableGrammarAction($var); }
	;


where_statement: WHERE where_condition[cond]													{ $$ = WhereStatementGrammarAction($cond); }
				|																				{ $$ = WhereStatementGrammarAction(NULL); }
		;

where_condition: constant[left] operator[op] constant[right]										{ $$ = OperatorWhereGrammarAction($left, $right, $op); }
			| 	constant[con] operator[op] ALL OPEN_PARENTHESIS program[prog] CLOSE_PARENTHESIS		{ $$ = OperatorNestedQueryWhereGrammarAction($con, $op, $prog); }
			|	constant[con] IS NULL_VAL															{ $$ = IsNullWhereGrammarAction($con); }
			|	constant[con] IS NOT NULL_VAL														{ $$ = IsNotNullWhereGrammarAction($con); }
			|	constant[con] IN OPEN_PARENTHESIS program[prog] CLOSE_PARENTHESIS					{ $$ = InNestedQueryWhereGrammarAction($con, $prog); }
			|	constant[con] IN OPEN_PARENTHESIS array[arr] CLOSE_PARENTHESIS						{ $$ = InArrayWhereGrammarAction($con, $arr); }
			|	constant[con] NOT IN OPEN_PARENTHESIS program[prog] CLOSE_PARENTHESIS				{ $$ = NotInNestedQueryWhereGrammarAction($con, $prog); }
			|	constant[con] NOT IN OPEN_PARENTHESIS array[arr] CLOSE_PARENTHESIS					{ $$ = NotInArrayWhereGrammarAction($con, $arr); }
			| 	where_condition[left] AND where_condition[right]									{ $$ = LogicalOperatorWhereGrammarAction($left, $right, AND_OP); }
			| 	where_condition[left] OR where_condition[right]										{ $$ = LogicalOperatorWhereGrammarAction($left, $right, OR_OP); }
			| 	OPEN_PARENTHESIS where_condition[where] CLOSE_PARENTHESIS							{ $$ = ParenthesisWhereGrammarAction($where); }
			| 	OPEN_PARENTHESIS program[prog] CLOSE_PARENTHESIS									{ $$ = ParenthesisNestedQueryWhereGrammarAction($prog); }
	;

operator: GREATER_EQUAL																	{ $$ = GE_OP; }
		| GREATER_THAN																	{ $$ = GT_OP; }
		| LESS_EQUAL																	{ $$ = LE_OP; }
		| LESS_THAN																		{ $$ = LT_OP; }
		| NOT_EQUAL																		{ $$ = NE_OP; }
		| EQUAL																			{ $$ = EQ_OP; }	
	;
	
having_statement: HAVING having_condition[cond]											{ $$ = HavingStatementGrammarAction($cond); }
				| 																		{ $$ = HavingStatementGrammarAction(NULL); }
		;
	
having_condition:column[col] operator[op] constant[cons]											{ $$ = OperatorHavingGrammarAction($col, $op, $cons); }
			|	column[col] IS NULL_VAL																{ $$ = IsNullHavingGrammarAction($col); }
			|	column[col] IS NOT NULL_VAL															{ $$ = IsNotNullHavingGrammarAction($col); }
			|	column[col] IN OPEN_PARENTHESIS program[prog] CLOSE_PARENTHESIS						{ $$ = InNestedQueryHavingGrammarAction($col, $prog); }
			|	column[col] NOT IN OPEN_PARENTHESIS program[prog] CLOSE_PARENTHESIS					{ $$ = NotInNestedQueryHavingGrammarAction($col, $prog); }
			| 	having_condition[left] AND having_condition[right]									{ $$ = LogicalOperatorHavingGrammarAction($right, $left, AND_OP); }
			| 	having_condition[left] OR having_condition[right]									{ $$ = LogicalOperatorHavingGrammarAction($right, $left, OR_OP); }
			| 	column[col] operator[op] ALL OPEN_PARENTHESIS program[prog] CLOSE_PARENTHESIS		{ $$ = OperatorNestedQueryHavingGrammarAction($col, $op, $prog); }
	;

group_by_statement: GROUPBY columns[cols]												{ $$ = GroupByGrammarAction($cols); }
				| 																		{ $$ = GroupByGrammarAction(NULL); }
		;


order_by_statement: ORDERBY columns[cols]												{ $$ = OrderByGrammarAction($cols); }
				| 																		{ $$ = OrderByGrammarAction(NULL); }
		;

array: array[left] COMMA array[right]													{ $$ = ArrayGrammarAction($left, $right, NULL); }
		| constant[cons]																{ $$ = ArrayGrammarAction(NULL, NULL, $cons); }
	; 


constant: INTEGER[integer]																{ $$ = IntegerConstantGrammarAction($integer); }
		| APOSTROPHE VAR[var] APOSTROPHE												{ $$ = ApostopheConstantGrammarAction($var); }
		| VAR[var]																		{ $$ = VarConstantGrammarAction($var); }
		| VAR[first] DOT VAR[second]													{ $$ = TableColumnConstantGrammarAction($first, $second); }
		| AST																			{ $$ = AllConstantGrammarAction(); }	
	;

%%
