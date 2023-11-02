#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Se realiza este tipo de definiciones cuando el tipo de dato debe
* auto-referenciarse, como es el caso de una "Expression", que está compuesta
* de otras 2 expresiones.
*/
typedef struct Expression Expression;
typedef struct Array Array;
typedef struct Column Column;
typedef struct Columns Columns;
typedef struct Where_condition Where_condition;
typedef struct Tables Tables;
typedef struct Having_condition Having_condition;

typedef struct Program Program;


/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/

/**
* En caso de que un no-terminal ("Factor" en este caso), posea más de una
* regla de producción asociada, se crea además de la estructura un enumerado
* que identitifque cada reducción posible (cada regla de producción). Luego,
* se agrega una instancia de este enumerado dentro del nodo del no-terminal.
*
* De este modo, al recorrer el AST, es posible determinar qué nodos hijos
* posee según el valor de este enumerado.
*/

//Enums

typedef enum {
	UNIQUE_COLUMN,
	AGGREGATION_COLUMN,
	EXPRESSION_COLUMN,
	ASSIGNMENT_COLUMN
} ColumnType;

typedef enum {
	ADDITION,
	DIVISION,
	MULTIPLICATION,
	SUBSTRACTION,
} ExpressionType;

typedef enum {
	SUM_AGG,
	AVG_AGG,
	MAX_AGG,
	MIN_AGG,
	COUNT_AGG,
} AggregationType;

typedef enum {
	MULTIPLE_TABLES,
	UNIQUE_TABLES,
	JOIN_ON_TABLES,
	NATURAL_JOIN_TABLES,
	PARENTHESIS_TABLES,
	NESTED_QUERY_TABLES,
	ASSIGNMENT_TABLES,
} TablesType;

typedef enum {
	JOIN_ON,
	LEFT_OUTER_ON,
	RIGHT_OUTER_ON,
	OUTER_ON,
	LEFT_ON,
	RIGHT_ON,
} JoinOnType;

typedef enum {
	OPERATOR_WHERE,
	OPERATOR_NESTED_QUERY_WHERE,
	IS_NULL_WHERE,
	IS_NOT_NULL_WHERE,
	IN_NESTED_QUERY_WHERE,
	IN_ARRAY_WHERE,
	NOT_IN_NESTED_QUERY_WHERE,
	NOT_IN_ARRAY_WHERE,
	LOGICAL_OP_WHERE,
	PARENTHESIS_WHERE,
	PARENTHESIS_NESTED_QUERY_WHERE,
} WhereConditionType;

typedef enum {
	OPERATOR_HAVING,
	IS_NULL_HAVING,
	IS_NOT_NULL_HAVING,
	IN_NESTED_QUERY_HAVING,
	NOT_IN_NESTED_QUERY_HAVING,
	LOGICAL_OP_HAVING,
	OPERATOR_NESTED_QUERY_HAVING,
} HavingConditionType;

typedef enum {
	INTEGER_CONST,
	APOST_CONST,
	VAR_CONST,
	TABLE_COLUMN_CONST,
	ALL_CONST
} ConstantType;

typedef enum {
	GE_OP,
	GT_OP,
	LE_OP,
	LT_OP,
	NE_OP,
	EQ_OP,
} OperatorType;

typedef enum {
	AND_OP,
	OR_OP,
} LogicalOperator;


// %type <constant> constant
typedef struct {
	ConstantType type;
	int integer;
	char * firstVar;
	char * secondVar;
} Constant;

// %type <table> table
typedef struct {
	char * var;
} Table;


// %type <array> array
struct Array {
	Array* left_array;
	Array* right_array;
	Constant* constant;
};

// %type <columns> columns
struct Columns{
	Column * column;
	Columns * columns;
};

// %type <column> column
struct Column{
	ColumnType type;
	Constant* constant;
	AggregationType aggregation;
	Column* leftColumn;
	Column* rightColumn;
	ExpressionType expression;
	char * varname;
};

// %type <select_statement> select_statement
typedef struct {
	Columns * columns;
} Select_statement;

// %type <where_condition> where_condition
struct Where_condition {
	WhereConditionType type;
	Constant* rightConstant;
	Constant* leftConstant;
	OperatorType operator;
	Program* program;
	Array* array;
	Where_condition* leftWhere;
	Where_condition* rightWhere;
	LogicalOperator logicalOp;
};

// %type <where_statement> where_statement
typedef struct {
	Where_condition * where_condition;
} Where_statement;

//%type <tables> tables
struct Tables{
	TablesType type;
	JoinOnType joinOnType;
	Table * table;
	Where_condition * where_condition;
	Tables * leftTables;
	Tables * rightTables;
	Program * program;
	char * varname;
};

//%type <from_statement> from_statement
typedef struct {
	Tables * tables;
} From_statement;


// %type <having_statement> having_statement
typedef struct {
	Having_condition * Having_condition;
} Having_statement;

// %type <having_condition> having_condition
struct Having_condition {
	HavingConditionType type;
	Column * column;
	OperatorType operator;
	Constant * constant;
	Program * program;
	Having_condition * conditionRight;
	Having_condition * conditionLeft;
	LogicalOperator logicalOp;
};

// %type <group_by_statement> group_by_statement
typedef struct {
	Columns * columns;
} Group_by_statement;

// %type <order_by_statement> order_by_statement
typedef struct {
	Columns * columns;
} Order_by_statement;


struct Program {
	Select_statement * select_statement;
	From_statement * from_statement;
	Where_statement * where_statement;
	Group_by_statement * group_by_statement;
	Having_statement * having_statement;
	Order_by_statement * order_by_statement;
};


#endif