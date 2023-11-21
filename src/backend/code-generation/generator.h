#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER
#include "../support/shared.h"


void Generator(int result, const char * fileName);

void generateOrderBy(Order_by_statement *order_by, FILE * latexFile);
void generateSelect(Select_statement *select, FILE * latexFile);
void printColumns(Columns *columns, FILE * latexFile);
void printColumn(Column *column, FILE * latexFile);
void generateGroupBy(Group_by_statement* group_by, FILE * latexFile);
void printTables(Tables * tables, FILE * latexFile);
void generateFrom(From_statement* From_statement, FILE * latexFile);
void generateHaving(Having_statement* Having_statement, FILE * latexFile);
void generateProgram(Program * program, FILE * latexFile);
void printNullValues(Column * column, FILE * latexFile, bool isNULL);
void printConstant(Constant * constant, FILE * latexFile);
bool hasAllColumns(Columns * columns);
void printHavingCondition(Having_condition * having_condition, FILE * latexFile);
void generateWhere(Where_statement* where, FILE * latexFile);
void printWhereCondition(Where_condition * where_condition, FILE * latexFile);
void printInArray(Constant * constant,Array * array, FILE * latexFile);
void printNullValuesConstant(Constant * constant, FILE * latexFile, bool isNULL);

bool hasSelect(Select_statement *select);
bool hasHaving(Having_statement *having);
bool hasWhere(Where_statement *where);
bool hasGroupBy(Group_by_statement *group_by);

#endif
