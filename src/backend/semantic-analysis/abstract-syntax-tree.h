#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Se realiza este tipo de definiciones cuando el tipo de dato debe
* auto-referenciarse, como es el caso de una "Expression", que está compuesta
* de otras 2 expresiones.
*/
typedef struct Expression Expression;

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


typedef struct {
	Select_statement * select_statement;
	From_statement * from_statement;
	Where_statement * where_statement;
	Group_by_statement * group_by_statement;
	Having_statement * having_statement;
	Order_by_statement * order_by_statement;
} Program;


typedef struct {
	Columns columns;
}Select_statement;
// %type <constant> constant
typedef struct {
	int rightValue;
	int leftValue;
} Constant;

// %type <select_statement> select_statement
typedef struct {
	Columns * columns;
} Select_statement;

typedef struct {
	Tables * tables;
}From_statement;

typedef struct{
	Table * table;
	Where_condition * where_condition;
	Tables * leftTables;
	Tables * rightTables;
	Program * program;
}Tables;

typedef struct {
	int value;
} Operator;


typedef struct {
	Columns * columns;
}Group_by_statement;



typedef struct {
	Having_condition * Having_condition;
}Having_statement;

typedef struct {
	Columns * columns;
}Order_by_statement;
// %type <columns> columns
typedef struct {
	Column * column;
	Columns * columns;
} Columns;

// %type <column> column
typedef struct {
	Constant* constant;
	Aggregation* aggregation;
	Column* leftColumn;
	Column* rightColumn;
	int token;
} Column;

// %type <aggregation> aggregation
typedef struct {
	int token;
} Aggregation;

// %type <from_statement> from_statement
typedef struct {
	int token;
} From_statement;

// %type <tables> tables


// %type <table> table
typedef struct {
	int token;
} Table;




// %type <where_statement> where_statement
typedef struct {
	Where_condition * where_condition;
} Where_statement;

typedef struct {
	Constant* constant;
	Operator* operator;
	Program* program;
	Array* array;
	Where_condition* leftWhere_condition;
	Where_condition* rightWhere_condition;
	int token;
} Were_condition;

// %type <where_condition> where_condition

// %type <having_statement> having_statement

// %type <having_condition> having_condition
typedef struct {
	Column * column;
	Operator * operator;
	Constant * constant;
	Program * program;
	Having_condition * conditionRight;
	Having_condition * conditionLeft;
} Having_condition;

// %type <group_by_statement> group_by_statement
// %type <order_by_statement> order_by_statement
// %type <operator> operator
// %type <array> array
typedef struct {
	
};



typedef enum {
	EXPRESSION,
	CONSTANT
} FactorType;

typedef struct {
	FactorType type;
	Expression * expression;
} Factor;

typedef enum {
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION,
	FACTOR
} ExpressionType;

struct Expression {
	ExpressionType type;
	Expression * leftExpression;
	Expression * rightExpression;
};


typedef struct {
	Constant left_constant;
	Constant right_constant;
	Operator operator;
	Where_condition * left_where_condition;
	Where_condition * right_where_condition;
	Program * program;
	Array * array;
	

}Where_condition;
#endif