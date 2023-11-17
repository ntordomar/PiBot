#include "../../backend/domain-specific/calculator.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"
#include "../../backend/semantic-analysis/symbol_table.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Implementación de "bison-actions.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogErrorRaw("[ERROR] Mensaje: '%s', debido a '", string);
	for (int i = 0; i < yyleng; ++i) {
		switch (yytext[i]) {
			case '\n':
				LogErrorRaw("\\n");
			default:
				LogErrorRaw("%c", yytext[i]);
		}
	}
	LogErrorRaw("' (length = %d, linea %d).\n\n", yyleng, yylineno);
}

bool checkIsValidReference(Columns * columns) {
	return true;
	// if(columns == NULL) return true;
	// Column * column = columns->column;
	// // printf("la columna es : %s\n", column->constant->firstVar);
	// if(column->type == UNIQUE_COLUMN) {
	// 	if(column->constant->type == TABLE_COLUMN_CONST) {
			
	// 		if(column->constant->secondVar != NULL){
	// 			if(!symbolTableFindTable(column->constant->firstVar)) {
	// 				return false;
	// 			}
	// 		}
	// 	}
	// }
	// return checkIsValidReference(columns->columns); si desconmentas esta tira seg fault!
}

void checkValidTableReferences(Program * program) {

	// tengo que chequear las del select, from , where , group by, having, order by
	if(program->group_by_statement == NULL) return;
	Columns * columns = program->select_statement->columns;
	if ( checkIsValidReference(columns) == false ){
		printf("ERROR: la columna %s no existe en la tabla %s\n", columns->column->varname, columns->column->varname);
		exit(1);
	} 

}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
Program * ProgramGrammarAction(Select_statement * select, From_statement * from, Where_statement * where, Group_by_statement * groupby, Having_statement * having, Order_by_statement * orderby) {
	Program * program = calloc(1, sizeof(Program));
	int value = 0;
	LogDebug("[Bison] ProgramGrammarAction(%d)", value);

	
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	/*
	* Por otro lado, "result" contiene el resultado de aplicar el análisis
	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	* la expresión se computa on-the-fly, y es la razón por la cual esta
	* variable es un simple entero, en lugar de un nodo.
	*/
	
	program->select_statement = select;
	program->from_statement = from;
	program->where_statement = where;
	program->group_by_statement = groupby;
	program->having_statement = having;
	program->order_by_statement = orderby;
	program = state.program;
	state.result = value; //todo preguntar
	printTableList();

	// the following function checks that every column reference of the type "table.column" is valid
	checkValidTableReferences(program);

	// the following functoin  checks that every column in the select clause is in the group by clause
	// checkValidGroupByClause(program);
	return program;
}


Constant * ConstantTreeConstruction(ConstantType type, int integer, char * firstVar, char * secondVar) {
	Constant * constant = calloc(1, sizeof(Constant));
	constant->type = type;
	constant->integer = integer;
	constant->firstVar = firstVar;
	constant->secondVar = secondVar;
	return constant;
}

Constant * IntegerConstantGrammarAction(int integer) {
	return ConstantTreeConstruction(INTEGER_CONST, integer, NULL, NULL);
}

Constant * ApostopheConstantGrammarAction(char * var) {
	return ConstantTreeConstruction(VAR_CONST, 0, var, NULL);
}

Constant * VarConstantGrammarAction(char * var) {
	return ConstantTreeConstruction(TABLE_COLUMN_CONST, 0, var, NULL);
}

Constant * TableColumnConstantGrammarAction(char * firstVar, char * secondVar) {
	return ConstantTreeConstruction(TABLE_COLUMN_CONST, 0, firstVar, secondVar);
}

Constant * AllConstantGrammarAction() {
	return ConstantTreeConstruction(ALL_CONST, 0, NULL, NULL);
}

Select_statement * SelectStatementGrammarAction(Columns * columns) {
	Select_statement * selectStatment = calloc(1, sizeof(Select_statement));
	selectStatment->columns = columns;
	return selectStatment;
}

Columns * ColumnsGrammarAction(Column * singleCol, Columns * multipleCols) {
	Columns * columns = calloc(1, sizeof(Columns));
	columns->column = singleCol;
	columns->columns = multipleCols;
	return columns;
}

Column * ColumnTreeCreation(ColumnType type, Constant * constant, AggregationType aggregation, Column * leftCol, Column * rightCol, ExpressionType expression, char * var) {
	Column * column = calloc(1, sizeof(Column));
	column->type = type;
	column->constant = constant;
	column->aggregation = aggregation;
	column->leftColumn = leftCol;
	column->rightColumn = rightCol;
	column->expression = expression;
	column->varname = var;

	return column;
}

void createSymbolEntry(KeyStruct * key, ValueStruct * val, char * tableName, char * columnName, TypeColumn type) {
	key->columnName = malloc(strlen(columnName)+1);
	strcpy(key->columnName, columnName);
	val->type = type;
	if(tableName != NULL) {
		printf("no es null la tabla\n");
		key->tableName = malloc(strlen(tableName)+1);
		strcpy(key->tableName, tableName);
	} else {
		printf(" es null la tabla\n");
		key->tableName = NULL;
	}
}

Column * UniqueColumnGrammarAction(Constant * column) {

	// if(column->type == VAR_CONST || column->type == TABLE_COLUMN_CONST) {

	// 	KeyStruct *key;
	// 	ValueStruct *val;

	// 	if(column->type == TABLE_COLUMN_CONST) {
	// 		createSymbolEntry(key,val, column->firstVar,column->secondVar,UNDEFINED);
	// 	} else {
	// 		createSymbolEntry(key,val, column->firstVar,NULL,UNDEFINED);
	// 	}
	// 	symbolTableInsert(key, val);
	// }

	return ColumnTreeCreation(UNIQUE_COLUMN, column, SUM_AGG, NULL, NULL, ADDITION, NULL);
}

Column * AggregationColumnGrammarAction(AggregationType aggregation, Constant * column) {
	return ColumnTreeCreation(AGGREGATION_COLUMN, column, aggregation, NULL, NULL, ADDITION, NULL);
}

Column * AdditionColumnGrammarAction(Column * leftCol, Column * rightCol) {
	return ColumnTreeCreation(EXPRESSION_COLUMN, NULL, SUM_AGG, leftCol, rightCol, ADDITION, NULL);
}

Column * DivisionColumnGrammarAction(Column * leftCol, Column * rightCol) {
	return ColumnTreeCreation(EXPRESSION_COLUMN, NULL, SUM_AGG, leftCol, rightCol, DIVISION, NULL);
}

Column * MultiplicationColumnGrammarAction(Column * leftCol, Column * rightCol) {
	return ColumnTreeCreation(EXPRESSION_COLUMN, NULL, SUM_AGG, leftCol, rightCol, MULTIPLICATION, NULL);
}

Column * SubstractionColumnGrammarAction(Column * leftCol, Column * rightCol) {
	return ColumnTreeCreation(EXPRESSION_COLUMN, NULL, SUM_AGG, leftCol, rightCol, SUBSTRACTION, NULL);
}

Column * AssignmentColumnGrammarAction(Column * column, char * var) {
	return ColumnTreeCreation(ASSIGNMENT_COLUMN, NULL, SUM_AGG, column, NULL, ADDITION, var);
}



From_statement * FormStatementGrammarAction(Tables * tables) {
	From_statement * formStatement = calloc(1, sizeof(From_statement));
	formStatement->tables = tables;
	return formStatement;
}

Tables * TablesTreeCreation(TablesType type, JoinOnType join, Table * table, Where_condition * where, Tables * leftTables, Tables * rightTables, Program * program, char * var) {
	Tables * tables = calloc(1, sizeof(Tables));
	tables->type = type;
	tables->joinOnType = join;
	tables->table = table;
	tables->where_condition = where;
	tables->leftTables = leftTables;
	tables->rightTables = rightTables;
	tables->program = program;
	tables->varname = var;
	return tables;
}

Tables * NaturalJoinGrammarAction(Tables * leftTables, Tables * rightTables) {
	return TablesTreeCreation(NATURAL_JOIN_TABLES, JOIN_ON, NULL, NULL, leftTables, rightTables, NULL, NULL);
}

Tables * JoinOnGrammarAction(Tables * leftTables, Tables * rightTables, Where_condition * where) {
	return TablesTreeCreation(JOIN_ON_TABLES, JOIN_ON, NULL, where, leftTables, rightTables, NULL, NULL);
}

Tables * LeftOuterJoinOnGrammarAction(Tables * leftTables, Tables * rightTables, Where_condition * where) {
	return TablesTreeCreation(JOIN_ON_TABLES, LEFT_OUTER_ON, NULL, where, leftTables, rightTables, NULL, NULL);
}

Tables * RightOuterJoinOnGrammarAction(Tables * leftTables, Tables * rightTables, Where_condition * where) {
	return TablesTreeCreation(JOIN_ON_TABLES, RIGHT_OUTER_ON, NULL, where, leftTables, rightTables, NULL, NULL);
}

Tables * OuterJoinOnGrammarAction(Tables * leftTables, Tables * rightTables, Where_condition * where) {
	return TablesTreeCreation(JOIN_ON_TABLES, OUTER_ON, NULL, where, leftTables, rightTables, NULL, NULL);
}

Tables * LeftOnGrammarAction(Tables * leftTables, Tables * rightTables, Where_condition * where) {
	return TablesTreeCreation(JOIN_ON_TABLES, LEFT_ON, NULL, where, leftTables, rightTables, NULL, NULL);
}

Tables * RightOnGrammarAction(Tables * leftTables, Tables * rightTables, Where_condition * where) {
	return TablesTreeCreation(JOIN_ON_TABLES, RIGHT_ON, NULL, where, leftTables, rightTables, NULL, NULL);
}

Tables * MultipleTablesGrammarAction(Tables * leftTables, Tables * rightTables) {
	return TablesTreeCreation(MULTIPLE_TABLES, JOIN_ON, NULL, NULL, leftTables, rightTables, NULL, NULL);
}

Tables * UniqueTablesGrammarAction(Table * table) {
	return TablesTreeCreation(UNIQUE_TABLES, JOIN_ON, table, NULL, NULL, NULL, NULL, NULL);
}

Tables * ParenthesisTablesGrammarAction(Tables * tables) {
	return TablesTreeCreation(PARENTHESIS_TABLES, JOIN_ON, NULL, NULL, tables, NULL, NULL, NULL);
}

Tables * NestedQueryTablesGrammarAction(Program * program) {
	return TablesTreeCreation(NESTED_QUERY_TABLES, JOIN_ON, NULL, NULL, NULL, NULL, program, NULL);
}

Tables * AssignmentTablesGrammarAction(Tables * tables, char * var) {
	return TablesTreeCreation(ASSIGNMENT_TABLES, JOIN_ON, NULL, NULL, tables, NULL, NULL, var);
}

Table * TableGrammarAction(char * var) {
	if(!symbolTableFindTable(var)) symbolTableInsertTable(var);
	Table * table = calloc(1, sizeof(Table));
	table->var = var;
	return table;
}

Where_statement * WhereStatementGrammarAction(Where_condition * where) {
	Where_statement * whereStatement = calloc(1, sizeof(Where_statement));
	whereStatement->where_condition = where;
	return whereStatement;
}

Where_condition * WhereTreeCreation(WhereConditionType type, Constant * leftConstant, Constant * rightConstant, OperatorType operator, Program * program, Array * array, Where_condition * rightWhere, Where_condition * leftWhere, LogicalOperator logicalOp) {
	Where_condition * whereCondition = calloc(1, sizeof(Where_condition));
	whereCondition->type = type;
	whereCondition->leftConstant = leftConstant;
	whereCondition->rightConstant = rightConstant;
	whereCondition->operator = operator;
	whereCondition->program = program;
	whereCondition->array = array;
	whereCondition->leftWhere = leftWhere;
	whereCondition->rightWhere = rightWhere;
	whereCondition->logicalOp = logicalOp;
	return whereCondition;
}

Where_condition * OperatorWhereGrammarAction(Constant * leftConstant, Constant * rightConstant, OperatorType operator) {
	if(leftConstant->type == TABLE_COLUMN_CONST || rightConstant->type == TABLE_COLUMN_CONST){
		
		// C1 ambas son col
		if(leftConstant->type == TABLE_COLUMN_CONST && rightConstant->type == TABLE_COLUMN_CONST){
			KeyStruct *key = malloc(sizeof(KeyStruct));
			ValueStruct* val = malloc(sizeof(ValueStruct));
			if(leftConstant->secondVar == NULL)
				createSymbolEntry(key,val, NULL,leftConstant->firstVar,UNDEFINED);
			else
			createSymbolEntry(key,val, leftConstant->firstVar,leftConstant->secondVar,UNDEFINED);
			EntryStruct * entry1 = symbolTableFind(key,val);

			KeyStruct *key2 = malloc(sizeof(KeyStruct));
			ValueStruct* val2 = malloc(sizeof(ValueStruct));
			if(rightConstant->secondVar == NULL)
				createSymbolEntry(key2,val2, NULL,rightConstant->firstVar,UNDEFINED);
			else
				createSymbolEntry(key2,val2, rightConstant->firstVar,rightConstant->secondVar,UNDEFINED);
			EntryStruct * entry2 = symbolTableFind(key2,val2);

			
			//c1.1 las dos estan en la tabla de simbolos
			if(entry1 != NULL && entry2 != NULL){
				if(entry1->value.type != UNDEFINED && entry2->value.type != UNDEFINED && entry1->value.type != entry2->value.type){
						//ERROR Y EXIT
						printf("hola!!!!");
						printf("ERROR: las columnas %s y %s no son del mismo tipo\n", leftConstant->firstVar, rightConstant->firstVar);
						exit(1);
				}else if(entry1->value.type == UNDEFINED && entry2->value.type != UNDEFINED){
					//c1.1.1 la primera no tiene type asignado
					symbolTableInsert(key, val2);
				}else if (entry1->value.type != UNDEFINED && entry2->value.type == UNDEFINED){
					//c1.1.2 la segunda no tiene type asignado
					symbolTableInsert(key2, val);
				}
			}
			//c1.2 solo la primera esta en la tabla de simbolos
			else if(entry1 != NULL){	
					// agrego a la tabla la segunda columna con el tipo de la priemra
					symbolTableInsert(key2, val);
			}
			//c1.3 solo la segunda esta en la tabla de simbolos
			else if(entry2 != NULL){
					// agrego a la tabla la primera columna con el tipo de la segunda
					symbolTableInsert(key, val2);
			}
		}
		// C2 solo la izq es col
		else if (leftConstant->type == TABLE_COLUMN_CONST){
			printf("aca entre una tabla izquerxa\n\n\n\n");
			KeyStruct *key = malloc(sizeof(KeyStruct));
			ValueStruct* val = malloc(sizeof(ValueStruct));
			if(leftConstant->secondVar == NULL){
				createSymbolEntry(key,val, NULL,leftConstant->firstVar,UNDEFINED);
			}else{
				createSymbolEntry(key,val,leftConstant->firstVar,leftConstant->secondVar,UNDEFINED);
			}
			EntryStruct * entry = symbolTableFind(key,val);
			
			// c2.1 esta en la tabla de simbolos
			if(entry != NULL && entry->value.type != UNDEFINED && entry->value.type != rightConstant->type){
					//ERROR Y EXIT
					printf("ERROR: la columna %s no es del mismo tipo que la constante\n", leftConstant->firstVar);
					exit(1);
			}
			else{
					printf("el valor de tipo de la constante es %d\n", rightConstant->type);
					//c2.1.1 no tiene type asignado
					if(rightConstant->type == INTEGER_CONST){
						val->type = INTEGER_CONST;
					}else if(rightConstant->type == VAR_CONST){
						val->type = VAR_CONST;
					}
					// insert en la tabla de simbolos
					symbolTableInsert(key, val);
					symbolTablePrint();
					printf("\n\n\n\n");
			}
		}

		// C3 solo la der es col
		else if (rightConstant->type == TABLE_COLUMN_CONST){
			printf("aca entre una tabla detecja");
			KeyStruct *key = malloc(sizeof(KeyStruct));
			ValueStruct* val = malloc(sizeof(ValueStruct));
			createSymbolEntry(key,val, rightConstant->firstVar,rightConstant->secondVar,UNDEFINED);
			EntryStruct * entry = symbolTableFind(key,val);
			//c2.1 esta en la tabla de simbolos
			if(entry != NULL && entry->value.type != UNDEFINED && entry->value.type != leftConstant->type){
					//ERROR Y EXIT
					printf("ERROR: la columna %s no es del mismo tipo que la constante\n", leftConstant->firstVar);
					exit(1);
			}else{
					//c2.1.1 no tiene type asignado
					if(leftConstant->type == INTEGER_CONST){
						entry->value.type = INTEGER_CONST;
					}else if(leftConstant->type == VAR_CONST){
						entry->value.type = VAR_CONST;
					}
					// insert en la tabla de simbolos
					symbolTableInsert(key, val);
			}
		}




	}
	return WhereTreeCreation(OPERATOR_WHERE, leftConstant, rightConstant, operator, NULL, NULL, NULL, NULL, AND_OP);
}

Where_condition * OperatorNestedQueryWhereGrammarAction(Constant * constant, OperatorType operator, Program * program) {
	return WhereTreeCreation(OPERATOR_NESTED_QUERY_WHERE, constant, NULL, operator, program, NULL, NULL, NULL, AND_OP);
}

Where_condition * IsNullWhereGrammarAction(Constant * constant) {
	return WhereTreeCreation(IS_NULL_WHERE, constant, NULL, GE_OP, NULL, NULL, NULL, NULL, AND_OP);
}

Where_condition * IsNotNullWhereGrammarAction(Constant * constant) {
	return WhereTreeCreation(IS_NOT_NULL_WHERE, constant, NULL, GE_OP, NULL, NULL, NULL, NULL, AND_OP);
}

Where_condition * InNestedQueryWhereGrammarAction(Constant * constant, Program * program) {
	return WhereTreeCreation(IN_NESTED_QUERY_WHERE, constant, NULL, GE_OP, program, NULL, NULL, NULL, AND_OP);
}

Where_condition * InArrayWhereGrammarAction(Constant * constant, Array * array) {
	return WhereTreeCreation(IN_ARRAY_WHERE, constant, NULL, GE_OP, NULL, array, NULL, NULL, AND_OP);
}

Where_condition * NotInNestedQueryWhereGrammarAction(Constant * constant, Program * program) {
	return WhereTreeCreation(NOT_IN_NESTED_QUERY_WHERE, constant, NULL, GE_OP, program, NULL, NULL, NULL, AND_OP);
}

Where_condition * NotInArrayWhereGrammarAction(Constant * constant, Array * array) {
	return WhereTreeCreation(NOT_IN_ARRAY_WHERE, constant, NULL, GE_OP, NULL, array, NULL, NULL, AND_OP);
}

Where_condition * LogicalOperatorWhereGrammarAction(Where_condition * leftWhere, Where_condition * rightWhere, LogicalOperator logicalOp) {
	return WhereTreeCreation(LOGICAL_OP_WHERE, NULL, NULL, GE_OP, NULL, NULL, leftWhere, rightWhere, logicalOp);
}

Where_condition * ParenthesisWhereGrammarAction(Where_condition * where) {
	return WhereTreeCreation(PARENTHESIS_WHERE, NULL, NULL, GE_OP, NULL, NULL, where, NULL, AND_OP);
}

Where_condition * ParenthesisNestedQueryWhereGrammarAction(Program * program) {
	return WhereTreeCreation(PARENTHESIS_NESTED_QUERY_WHERE, NULL, NULL, GE_OP, program, NULL, NULL, NULL, AND_OP);
}

Having_statement * HavingStatementGrammarAction(Having_condition * having) {
	Having_statement * havingStatement = calloc(1, sizeof(Having_statement));
	havingStatement->Having_condition = having;
	return havingStatement;
}

Having_condition * HavingTreeCreation(HavingConditionType type, Column * column, OperatorType operator, Constant * constant, Program * program, Having_condition * rightHaving, Having_condition * leftHaving, LogicalOperator logicalOp) {
	Having_condition * having = calloc(1, sizeof(Having_condition));
	having->type = type;
	having->column = column;
	having->operator = operator;
	having->constant = constant;
	having->program = program;
	having->conditionRight = rightHaving;
	having->conditionLeft = leftHaving;
	having->logicalOp = logicalOp;
	return having;
}

Having_condition * OperatorHavingGrammarAction(Column * column, OperatorType operator, Constant * constant) {
	return HavingTreeCreation(OPERATOR_HAVING, column, operator, constant, NULL, NULL, NULL, AND_OP);
}

Having_condition * IsNullHavingGrammarAction(Column * column) {
	return HavingTreeCreation(IS_NULL_HAVING, column, GE_OP, NULL, NULL, NULL, NULL, AND_OP);
}

Having_condition * IsNotNullHavingGrammarAction(Column * column) {
	return HavingTreeCreation(IS_NOT_NULL_HAVING, column, GE_OP, NULL, NULL, NULL, NULL, AND_OP);
}

Having_condition * InNestedQueryHavingGrammarAction(Column * column, Program * program) {
	return HavingTreeCreation(IN_NESTED_QUERY_HAVING, column, GE_OP, NULL, program, NULL, NULL, AND_OP);
}

Having_condition * NotInNestedQueryHavingGrammarAction(Column * column, Program * program) {
	return HavingTreeCreation(NOT_IN_NESTED_QUERY_HAVING, column, GE_OP, NULL, program, NULL, NULL, AND_OP);
}

Having_condition * LogicalOperatorHavingGrammarAction(Having_condition * rightHaving, Having_condition * leftHaving, LogicalOperator logicalOp) {
	return HavingTreeCreation(LOGICAL_OP_HAVING, NULL, GE_OP, NULL, NULL, rightHaving, leftHaving, logicalOp);
}

Having_condition * OperatorNestedQueryHavingGrammarAction(Column * column, OperatorType operator, Program * program) {
	return HavingTreeCreation(OPERATOR_NESTED_QUERY_HAVING, column, operator, NULL, program, NULL, NULL, AND_OP);
}

Group_by_statement * GroupByGrammarAction(Columns * columns) {
	Group_by_statement * groupByStatement = calloc(1, sizeof(Group_by_statement));
	groupByStatement->columns = columns;
	return groupByStatement;
}

Order_by_statement * OrderByGrammarAction(Columns * columns) {
	Order_by_statement * orderByStatement = calloc(1, sizeof(Order_by_statement));
	orderByStatement->columns = columns;
	return orderByStatement;
}

Array * ArrayGrammarAction(Array * leftArray, Array * rightArray, Constant * constant) {
	Array * array = calloc(1, sizeof(Array));
	array->left_array = leftArray;
	array->right_array = rightArray;
	array->constant = constant;
	return array;
}


int AdditionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] AdditionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Add(leftValue, rightValue);
}

int SubtractionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] SubtractionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Subtract(leftValue, rightValue);
}

int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] MultiplicationExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Multiply(leftValue, rightValue);
}

int DivisionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("[Bison] DivisionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Divide(leftValue, rightValue);
}

int FactorExpressionGrammarAction(const int value) {
	LogDebug("[Bison] FactorExpressionGrammarAction(%d)", value);
	return value;
}

int ExpressionFactorGrammarAction(const int value) {
	LogDebug("[Bison] ExpressionFactorGrammarAction(%d)", value);
	return value;
}

int ConstantFactorGrammarAction(const int value) {
	LogDebug("[Bison] ConstantFactorGrammarAction(%d)", value);
	return value;
}

// int IntegerConstantGrammarAction(const int value) {
// 	LogDebug("[Bison] IntegerConstantGrammarAction(%d)", value);
// 	return value;
// }
