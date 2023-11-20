#include "../support/logger.h"
#include "../support/memory-manager.h"
#include "generator.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/**
 * Implementación de "generator.h".
 */

char *aggregationsToPrint[] = {"SUM", "AVG", "MAX", "MIN", "COUNT"};
char *expressionsToPrint[] = {"+", "/", "*", "-"};
char *fromToPrint[] = {"$\\theta$_{", "$\\ltimes$_{", "$\\rtimes$_{", "$\\bowtie$_{","$\\ltimes$_{", "$\\rtimes$_{"};
char *operatorsToPrint[] = {"\\geq", ">", "\\leq", "<", "<>", "="};
char *logicalOperatorsToPrint[] = {"AND", "OR"};

void Generator(int result,const char * fileName) {
	
	FILE * latexFile;
	struct stat st = {0};

	if (stat("output", &st) == -1) {
   		mkdir("output", 0700);	
	}
	if(fileName != NULL){
		char * path = (char *)mm_malloc(sizeof(char) * (strlen(fileName) + strlen("output/") ) + 1);
		path = strcpy(path, "output/");
		path = strcat(path, fileName);
		latexFile = fopen(path, "w");
	}else{
		latexFile = fopen("output/output.tex", "w");
	}


	if (latexFile == NULL) {
		mm_freeAll();
		exit(EXIT_FAILURE);
	}

	fprintf(latexFile, "\\documentclass{article}\n");
	fprintf(latexFile, "\\usepackage[utf8]{inputenc}\n");
	fprintf(latexFile, "\\usepackage[spanish]{babel}\n");
	fprintf(latexFile, "\\usepackage{graphicx}\n");
	fprintf(latexFile, "\\usepackage{float}\n");
	fprintf(latexFile, "\\usepackage{amsmath}\n");
	fprintf(latexFile, "\\usepackage{amssymb}\n");
	fprintf(latexFile, "\\usepackage{amsfonts}\n");
	fprintf(latexFile, "\\usepackage{listings}\n");
	fprintf(latexFile, "\\usepackage{color}\n");
	fprintf(latexFile, "\\usepackage{hyperref}\n");
	fprintf(latexFile, "\\usepackage{tikz}\n");
	fprintf(latexFile, "\\usepackage{pgfplots}\n");
	fprintf(latexFile, "\\usepackage{pgfplotstable}\n");
	fprintf(latexFile, "\\usepackage{booktabs}\n");
	fprintf(latexFile, "\\usepackage{multirow}\n");
	fprintf(latexFile, "\\usepackage{array}\n");
	fprintf(latexFile, "\\usepackage{longtable}\n");
	fprintf(latexFile, "\\usepackage{tabularx}\n");
	fprintf(latexFile, "\\usepackage{pdflscape}\n");
	fprintf(latexFile, "\\usepackage{fancyhdr}\n");
	fprintf(latexFile, "\\date{}\n");
	fprintf(latexFile, "\\usepackage[margin=1in]{geometry}\n");
	fprintf(latexFile, "\\usepackage{varwidth}\n");
	fprintf(latexFile, "\\usepackage{setspace}\n");
	fprintf(latexFile, "\\newcommand{\\multiline}[1]{\\begin{varwidth}{\\linewidth}$\\displaystyle #1$\\end{varwidth}}\n");
	fprintf(latexFile, "\\begin{document}\n");
	fprintf(latexFile, "\\begin{landscape}\n");
	fprintf(latexFile, "\\pagestyle{fancy}\n\\fancyhf{}\n\\renewcommand{\\headrulewidth}{0pt}\n");
	fprintf(latexFile, "\\begin{flushleft}\n");
	fprintf(latexFile, "\\doublespacing\n");
	fprintf(latexFile, "{\\fontsize{14}{12}\\selectfont\n");
	fprintf(latexFile, "\\textbf{\\huge{Algebra Relacional}}\\newline \\\\\n");

	generateProgram(state.program, latexFile);
	
	fprintf(latexFile, "}\n");
	fprintf(latexFile, "\\end{flushleft}\n");
	fprintf(latexFile, "\\vfill\n\\begin{center}\n\\parbox{\\linewidth}{\\raggedright PiBot License v0.2.0\\hfill Copyright © 2023 PiBot}\n\\end{center}\n");
	fprintf(latexFile, "\\end{landscape}\n");
	fprintf(latexFile, "\\end{document}\n");
	fclose(latexFile);
}

void generateProgram(Program * program, FILE * latexFile){
	float x = 0.5;

	if(state.program->order_by_statement != NULL && state.program->order_by_statement->columns != NULL) {
		generateOrderBy(state.program->order_by_statement, latexFile);
		if(hasSelect(state.program->select_statement) || hasHaving(state.program->having_statement) || hasGroupBy(state.program->group_by_statement) || hasWhere(state.program->where_statement)) {
			printf("entro al if\n");
			fprintf(latexFile,"\\\\\n");
			fprintf(latexFile,"\\hspace{%.1fem}", x);
			x++;
		}
	}
	
	if(state.program->select_statement != NULL) {
		generateSelect(state.program->select_statement, latexFile);
		if(hasSelect(state.program->select_statement) && (hasHaving(state.program->having_statement) || hasGroupBy(state.program->group_by_statement) || hasWhere(state.program->where_statement))) {
			printf("entro al if 2\n");
			fprintf(latexFile,"\\\\\n");
			fprintf(latexFile,"\\hspace{%.1fem}", x);
			x++;
		}
	}
	if(state.program->having_statement != NULL && state.program->having_statement->Having_condition != NULL) {
		generateHaving(state.program->having_statement, latexFile);
		if(hasGroupBy(state.program->group_by_statement) || hasWhere(state.program->where_statement)) {
			fprintf(latexFile,"\\\\\n");
			fprintf(latexFile,"\\hspace{%.1fem}", x);
			x++;
		}
	}

	if(state.program->group_by_statement != NULL && state.program->group_by_statement->columns != NULL) {
		generateGroupBy(state.program->group_by_statement, latexFile);
		if(hasWhere(state.program->where_statement)) {
			fprintf(latexFile,"\\\\\n");
			fprintf(latexFile,"\\hspace{%.1fem}", x);
			x++;
		}
	}

	if(state.program->where_statement != NULL) {
		generateWhere(state.program->where_statement, latexFile);
	}

	if(state.program->from_statement != NULL) {
		generateFrom(state.program->from_statement, latexFile);
	}

}


bool hasSelect(Select_statement *select){
	return select != NULL && !(hasAllColumns(select->columns));
}

bool hasHaving(Having_statement *having){
	return having != NULL && having->Having_condition != NULL;
}

bool hasGroupBy(Group_by_statement *group_by){
	return group_by != NULL && group_by->columns != NULL;
}

bool hasWhere(Where_statement *where){
	return where != NULL && where->where_condition != NULL;
}

void generateOrderBy(Order_by_statement* order_by, FILE * latexFile){
	fprintf(latexFile, "$\\tau_{");
	printColumns(order_by->columns, latexFile);
	fprintf(latexFile, "}$\n");
}


void generateGroupBy(Group_by_statement* group_by, FILE * latexFile){
	fprintf(latexFile, "$\\gamma_{");
	printColumns(group_by->columns, latexFile);
	fprintf(latexFile, "}$\n");
}

bool hasAllColumns(Columns * columns){
	Columns * current = columns;
	while(current != NULL){
		if(current->column != NULL && current->column->constant != NULL && current->column->constant->type == ALL_CONST) return true;
		current = current->columns;
	}
	return false;
}


void generateSelect(Select_statement *select, FILE * latexFile){
	if(!hasAllColumns(select->columns)){
		fprintf(latexFile, "$\\pi_{");
		printColumns(select->columns, latexFile);
		fprintf(latexFile, "}$\n");
	}
}

void generateFrom(From_statement *from, FILE * latexFile){
	fprintf(latexFile, "\\textbf{(");
	printTables(from->tables, latexFile);
	fprintf(latexFile, ")}\n");
}

void generateHaving(Having_statement *having, FILE * latexFile){
	Having_condition * having_condition = having->Having_condition;
	fprintf(latexFile, "$\\sigma_{");
	printHavingCondition(having_condition, latexFile);
	fprintf(latexFile, "}$\n");
}

void generateWhere(Where_statement* where, FILE * latexFile){
	Where_condition * where_condition = where->where_condition;
	fprintf(latexFile, "\\fontsize{14}{12}\\selectfont$\\sigma_{\\fontsize{10}{5}\\multiline{");
	printWhereCondition(where_condition, latexFile);
	fprintf(latexFile, "}}$\n");
}


void printWhereCondition(Where_condition * where_condition, FILE * latexFile){
	if(where_condition == NULL) return;
	switch (where_condition->type)
	{
	case OPERATOR_WHERE:
		printConstant(where_condition->leftConstant, latexFile);
		fprintf(latexFile, " %s ", operatorsToPrint[where_condition->operator]);
		printConstant(where_condition->rightConstant, latexFile);
		break;

	case IS_NULL_WHERE:
		printNullValuesConstant(where_condition->leftConstant, latexFile, true);
		break;
		
	case IS_NOT_NULL_WHERE:
		printNullValuesConstant(where_condition->leftConstant, latexFile, false);
		break;

	case LOGICAL_OP_WHERE:
		printWhereCondition(where_condition->leftWhere, latexFile);
		fprintf(latexFile, "\\ %s\\ ", logicalOperatorsToPrint[where_condition->logicalOp]);
		printWhereCondition(where_condition->rightWhere, latexFile);
		break;

	case IN_ARRAY_WHERE:
		printInArray(where_condition->leftConstant,where_condition->array, latexFile);
		break;

	case NOT_IN_ARRAY_WHERE:
		fprintf(latexFile, "\\text{NOT} (");
		printInArray(where_condition->leftConstant,where_condition->array, latexFile);
		fprintf(latexFile, ")");
		break;

	case PARENTHESIS_WHERE:
		fprintf(latexFile, "(");
		printWhereCondition(where_condition->rightWhere, latexFile);
		fprintf(latexFile, ")");
		break;

	case OPERATOR_NESTED_QUERY_WHERE:
		printConstant(where_condition->leftConstant, latexFile);
		fprintf(latexFile, "%s", operatorsToPrint[where_condition->operator]);
		fprintf(latexFile, "(");
		generateProgram(where_condition->program, latexFile);
		fprintf(latexFile, ")");
		break;
	case PARENTHESIS_NESTED_QUERY_WHERE:
		fprintf(latexFile, "(");
		generateProgram(where_condition->program, latexFile);
		fprintf(latexFile, ")");
		break;

	case IN_NESTED_QUERY_WHERE:
	printConstant(where_condition->leftConstant,latexFile);
		fprintf(latexFile, "\\ \\text{IN}\\ ");
		fprintf(latexFile, "(");
		generateProgram(where_condition->program, latexFile);
		fprintf(latexFile, ")");
		break;

	case NOT_IN_NESTED_QUERY_WHERE:
		printConstant(where_condition->leftConstant,latexFile);
		fprintf(latexFile, "\\ \\text{NOT IN}\\ ");
		fprintf(latexFile, "(");
		generateProgram(where_condition->program, latexFile);
		fprintf(latexFile, ")");
		break;

	default:
		break;
	};
	
}


void printInArray(Constant * constant,Array * array, FILE * latexFile){
	if(array->left_array != NULL){
		printInArray(constant,array->left_array, latexFile);
	}
	if(array->constant!= NULL){
		printConstant(constant, latexFile);
		fprintf(latexFile, " = ");
		printConstant(array->constant, latexFile);
	}
	if(array->right_array != NULL){
		fprintf(latexFile, "\\ OR\\ ");
		printInArray(constant,array->right_array, latexFile);
	}
	
}


void printHavingCondition(Having_condition * having_condition, FILE * latexFile){
	switch (having_condition->type)
	{
	case OPERATOR_HAVING:
		printColumn(having_condition->column, latexFile);
		fprintf(latexFile, "%s", operatorsToPrint[having_condition->operator]);
		printConstant(having_condition->constant, latexFile);
		break;

	case IS_NULL_HAVING:
		printNullValues(having_condition->column, latexFile, true);
		break;
		
	case IS_NOT_NULL_HAVING:
		printNullValues(having_condition->column, latexFile, false);
		break;

	case LOGICAL_OP_HAVING:
		printHavingCondition(having_condition->conditionLeft, latexFile);
		fprintf(latexFile, "\\ %s \\ ", logicalOperatorsToPrint[having_condition->logicalOp]);
		printHavingCondition(having_condition->conditionRight, latexFile);
		break;

	case OPERATOR_NESTED_QUERY_HAVING:
		printColumn(having_condition->column, latexFile);
		fprintf(latexFile, "%s", operatorsToPrint[having_condition->operator]);
		fprintf(latexFile, "(");
		generateProgram(having_condition->program, latexFile);
		fprintf(latexFile, ")");
		break;

	case IN_NESTED_QUERY_HAVING:
		printColumn(having_condition->column, latexFile);
		fprintf(latexFile, "\\ IN\\ ");
		fprintf(latexFile, "(");
		generateProgram(having_condition->program, latexFile);
		fprintf(latexFile, ")");
		break;
	case NOT_IN_NESTED_QUERY_HAVING:
		printColumn(having_condition->column, latexFile);
		fprintf(latexFile, "\\ NOT IN\\ ");
		fprintf(latexFile, "(");
		generateProgram(having_condition->program, latexFile);
		fprintf(latexFile, ")");
		break;


	default:
		break;
	};
}


void printNullValuesConstant(Constant * constant, FILE * latexFile, bool isNULL){
	
	if(isNULL){
		printConstant(constant, latexFile);
		fprintf(latexFile, " = NULL");
	}
	else{
		fprintf(latexFile, "NOT (");
		printConstant(constant, latexFile);
		fprintf(latexFile, " = NULL");
		fprintf(latexFile, ")");
	}

}



void printTables(Tables * tables, FILE * latexFile){
	if(tables == NULL) return;
	switch (tables->type)
	{
	case UNIQUE_TABLES:
		fprintf(latexFile, "%s", tables->table->var);
		break;
	case MULTIPLE_TABLES:
		printTables(tables->leftTables, latexFile);
		fprintf(latexFile, "$\\times$");
		printTables(tables->rightTables, latexFile);
		break;

		case JOIN_ON_TABLES:
		printTables(tables->leftTables, latexFile);
		fprintf(latexFile, fromToPrint[tables->joinOnType]);
		printWhereCondition(tables->where_condition, latexFile);
		fprintf(latexFile, "}");
		printTables(tables->rightTables, latexFile);
		break;

		case NATURAL_JOIN_TABLES:
		printTables(tables->leftTables, latexFile);
		fprintf(latexFile, "$\\bowtie$");
		printTables(tables->rightTables, latexFile);
		break;

		case PARENTHESIS_TABLES:
		fprintf(latexFile, "(");
		printTables(tables->leftTables, latexFile);
		fprintf(latexFile, ")");
		break;

			case ASSIGNMENT_TABLES:
		fprintf(latexFile, "\\rho_{");
		fprintf(latexFile, "%s", tables->varname);
		fprintf(latexFile, "} \\ ");
		fprintf(latexFile, "(");
		printTables(tables->leftTables, latexFile);
		fprintf(latexFile, ")");
		break;
		case (NESTED_QUERY_TABLES):
		fprintf(latexFile, "(");
		generateProgram(tables->program, latexFile);
		fprintf(latexFile, ")");
		break;
	default:
		break;
	}

	
}



void printColumns(Columns * columns, FILE * latexFile){
	Columns * current = columns;
	if(current == NULL) printf("esto es joda?");
	while(current != NULL){
		printColumn(current->column, latexFile);
		current = current->columns;
		if(current != NULL) fprintf(latexFile, ",");
	}
}

void printColumn(Column * column, FILE * latexFile){
	switch (column->type)
	{
	case UNIQUE_COLUMN:
		printConstant(column->constant, latexFile);
		break;

	case AGGREGATION_COLUMN:
		if(column->constant->type == TABLE_COLUMN_CONST) {
			fprintf(latexFile, "%s(%s.%s)", aggregationsToPrint[column->aggregation], column->constant->firstVar, column->constant->secondVar);	
		} else {
			fprintf(latexFile, "%s(%s)", aggregationsToPrint[column->aggregation], column->constant->firstVar);	
		}
		break;
	case EXPRESSION_COLUMN:
		printColumn(column->leftColumn, latexFile);
		fprintf(latexFile, "%s", expressionsToPrint[column->expression]);
		printColumn(column->rightColumn, latexFile);
		break;
	case ASSIGNMENT_COLUMN:
		printColumn(column->leftColumn, latexFile);
		fprintf(latexFile, " \\ \\rightarrow \\ ");
		fprintf(latexFile, "%s", column->varname);
	default:
		break;
	}
}

void printConstant(Constant * constant, FILE * latexFile){
	if(constant == NULL) return;
	switch (constant->type)
	{
	case INTEGER_CONST:
		fprintf(latexFile, "%d", constant->integer);
		break;
	case APOST_CONST:
		fprintf(latexFile, "\\text{''%s''}", constant->firstVar);
		break;
	case VAR_CONST:
		fprintf(latexFile, "\\text{%s}", constant->firstVar);
		break;
	case TABLE_COLUMN_CONST:
		fprintf(latexFile, "\\text{%s.%s}", constant->firstVar, constant->secondVar);
		break;
	default:
		break;
	}
}


void printNullValues(Column * column, FILE * latexFile, bool isNULL){
	
	if(isNULL){
		printColumn(column, latexFile);
		fprintf(latexFile, " = NULL ");
	}
	else{
		fprintf(latexFile, "NOT ( ");
		printColumn(column, latexFile);
		fprintf(latexFile, " = NULL ");
		fprintf(latexFile, " ) ");
	}

}




