#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// Programa.
Program * ProgramGrammarAction(Select_statement * select, From_statement * from, Where_statement * where, Group_by_statement * groupby, Having_statement * having, Order_by_statement * orderby);

Constant * ConstantTreeConstruction(ConstantType type, int integer, char * firstVar, char * secondVar);
Constant * IntegerConstantGrammarAction(int integer);
Constant * ApostopheConstantGrammarAction(char * var);
Constant * VarConstantGrammarAction(char * var);
Constant * TableColumnConstantGrammarAction(char * firstVar, char * secondVar);
Constant * AllConstantGrammarAction();

Select_statement * SelectStatementGrammarAction(Columns * columns);

Columns * ColumnsGrammarAction(Column * singleCol, Columns * multipleCols);

Column * ColumnTreeCreation(ColumnType type, Constant * constant, AggregationType aggregation, Column * leftCol, Column * rightCol, ExpressionType expression, char * var);
Column * UniqueColumnGrammarAction(Constant * column);
Column * AggregationColumnGrammarAction(AggregationType aggregation, Constant * column);
Column * AdditionColumnGrammarAction(Column * leftCol, Column * rightCol);
Column * DivisionColumnGrammarAction(Column * leftCol, Column * rightCol);
Column * MultiplicationColumnGrammarAction(Column * leftCol, Column * rightCol);
Column * SubstractionColumnGrammarAction(Column * leftCol, Column * rightCol);
Column * AssignmentColumnGrammarAction(Column * column, char * var);

From_statement * FormStatementGrammarAction(Tables * tables);

Tables * TablesTreeCreation(TablesType type, JoinOnType join, Table * table, Where_condition * where, Tables * leftTables, Tables * rightTables, Program * program, char * var);
Tables * NaturalJoinGrammarAction(Tables * leftTables, Tables * rightTables);
Tables * JoinOnGrammarAction(Tables * leftTables, Tables * rightTables, Where_condition * where);
Tables * LeftOuterJoinOnGrammarAction(Tables * leftTables, Tables * rightTables, Where_condition * where);
Tables * RightOuterJoinOnGrammarAction(Tables * leftTables, Tables * rightTables, Where_condition * where);
Tables * OuterJoinOnGrammarAction(Tables * leftTables, Tables * rightTables, Where_condition * where);
Tables * LeftOnGrammarAction(Tables * leftTables, Tables * rightTables, Where_condition * where);
Tables * RightOnGrammarAction(Tables * leftTables, Tables * rightTables, Where_condition * where);
Tables * MultipleTablesGrammarAction(Tables * leftTables, Tables * rightTables);
Tables * UniqueTablesGrammarAction(Table * table);
Tables * ParenthesisTablesGrammarAction(Tables * tables);
Tables * NestedQueryTablesGrammarAction(Program * program);
Tables * AssignmentTablesGrammarAction(Tables * tables, char * var);

Table * TableGrammarAction(char * var);

Where_statement * WhereStatementGrammarAction(Where_condition * where);

Where_condition * WhereTreeCreation(WhereConditionType type, Constant * leftConstant, Constant * rightConstant, OperatorType operator, Program * program, Array * array, Where_condition * rightWhere, Where_condition * leftWhere, LogicalOperator logicalOp);
Where_condition * OperatorWhereGrammarAction(Constant * leftConstant, Constant * rightConstant, OperatorType operator);
Where_condition * OperatorNestedQueryWhereGrammarAction(Constant * constant, OperatorType operator, Program * program);
Where_condition * IsNullWhereGrammarAction(Constant * constant);
Where_condition * IsNotNullWhereGrammarAction(Constant * constant);
Where_condition * InNestedQueryWhereGrammarAction(Constant * constant, Program * program);
Where_condition * InArrayWhereGrammarAction(Constant * constant, Array * array);
Where_condition * NotInNestedQueryWhereGrammarAction(Constant * constant, Program * program);
Where_condition * NotInArrayWhereGrammarAction(Constant * constant, Array * array);
Where_condition * LogicalOperatorWhereGrammarAction(Where_condition * leftWhere, Where_condition * rightWhere, LogicalOperator logicalOp);
Where_condition * ParenthesisWhereGrammarAction(Where_condition * where);
Where_condition * ParenthesisNestedQueryWhereGrammarAction(Program * program);

Having_statement * HavingStatementGrammarAction(Having_condition * having);

Having_condition * HavingTreeCreation(HavingConditionType type, Column * column, OperatorType operator, Constant * constant, Program * program, Having_condition * rightHaving, Having_condition * leftHaving, LogicalOperator logicalOp);
Having_condition * OperatorHavingGrammarAction(Column * column, OperatorType operator, Constant * constant);
Having_condition * IsNullHavingGrammarAction(Column * column);
Having_condition * IsNotNullHavingGrammarAction(Column * column);
Having_condition * InNestedQueryHavingGrammarAction(Column * column, Program * program);
Having_condition * NotInNestedQueryHavingGrammarAction(Column * column, Program * program);
Having_condition * LogicalOperatorHavingGrammarAction(Having_condition * rightHaving, Having_condition * leftHaving, LogicalOperator logicalOp);
Having_condition * OperatorNestedQueryHavingGrammarAction(Column * column, OperatorType operator, Program * program);
Group_by_statement * GroupByGrammarAction(Columns * columns);
Order_by_statement * OrderByGrammarAction(Columns * columns);
Array * ArrayGrammarAction(Array * leftArray, Array * rightArray, Constant * constant);



// Expresión.
int AdditionExpressionGrammarAction(const int leftValue, const int rightValue);
int SubtractionExpressionGrammarAction(const int leftValue, const int rightValue);
int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue);
int DivisionExpressionGrammarAction(const int leftValue, const int rightValue);
int FactorExpressionGrammarAction(const int value);

// Factores.
int ExpressionFactorGrammarAction(const int value);
int ConstantFactorGrammarAction(const int value);

// Constantes.
// int IntegerConstantGrammarAction(const int value);

#endif
