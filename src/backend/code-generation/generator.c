#include "../support/logger.h"
#include "generator.h"
#include <stdio.h>
#include <string.h>
// incluime el del File
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


/**
 * Implementación de "generator.h".
 */

char *aggregationsToPrint[] = {"SUM", "AVG", "MAX", "MIN", "COUNT"};
char *expressionsToPrint[] = {"+", "/", "*", "-"};
char *fromToPrint[] = {"$\\times$", "$\\ltimes$", "$\\rtimes$", "$\\bowtie$","$\\ltimes$", "$\\rtimes$"};

void generateOrderBy(Order_by_statement *order_by, FILE * latexFile);
void generateSelect(Select_statement *select, FILE * latexFile);
void printColumns(Columns *columns, FILE * latexFile);
void printColumn(Column *column, FILE * latexFile);
void generateGroupBy(Group_by_statement* group_by, FILE * latexFile);
void printTables(Tables * tables, FILE * latexFile);
void generateFrom(From_statement* From_statement, FILE * latexFile);

void Generator(int result) {
	LogInfo("El resultado de la expresion computada es: '%d'.", result);
	// LogInfo("El programa es: '%s'.", state.program->select_statement->columns->column->constant->firstVar);
	FILE * latexFile = fopen("output/index.tex", "w");

	if (latexFile == NULL) {
		LogErrorRaw("Error: No se pudo abrir el archivo de salida.\n");
		exit(EXIT_FAILURE);
	}

	fprintf(latexFile, "\\documentclass{article}\n");
	fprintf(latexFile, "\\usepackage[utf8]{inputenc}\n");fprintf(latexFile, "\\usepackage[utf8]{inputenc}\n");
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
	// arrancame el documento aca
	fprintf(latexFile, "\\begin{document}\n");
	fprintf(latexFile, "\\title{Informe de la consulta}\n");
	fprintf(latexFile, "\\author{Grupo 2}\n");
	fprintf(latexFile, "\\maketitle\n");
	fprintf(latexFile, "\\textbf{\\huge{");

	//LLAMAR A LOS STATEMENTS
	if(state.program->order_by_statement != NULL) {
	generateOrderBy(state.program->order_by_statement, latexFile);
	}

	fprintf(latexFile,"\\newline");

	if(state.program->group_by_statement != NULL) {
		generateGroupBy(state.program->group_by_statement, latexFile);
	}
	fprintf(latexFile,"\\newline");
	if(state.program->select_statement != NULL) {
		generateSelect(state.program->select_statement, latexFile);
	}

	fprintf(latexFile,"\\newline");
	if(state.program->from_statement != NULL) {
		generateFrom(state.program->from_statement, latexFile);
	}

	fprintf(latexFile, "}\n}\n");
	// terminame el socumento aca 
	fprintf(latexFile, "\\end{document}\n");
	//cerra los recutross!
	fclose(latexFile);
}

void generateOrderBy(Order_by_statement* order_by, FILE * latexFile){
	//generar el codigo para el order by
	fprintf(latexFile, "$\\tau$_{");
	printColumns(order_by->columns, latexFile);
	fprintf(latexFile, "}\n");
}


void generateGroupBy(Group_by_statement* group_by, FILE * latexFile){
	//generar el codigo para el group by
	fprintf(latexFile, "$\\gamma$_{");
	printColumns(group_by->columns, latexFile);
	fprintf(latexFile, "}\n");

}


void generateSelect(Select_statement *select, FILE * latexFile){
	//generar el codigo para el select
	fprintf(latexFile, "$\\pi$_{");
	printColumns(select->columns, latexFile);
	fprintf(latexFile, "}\n");
}

void generateFrom(From_statement *from, FILE * latexFile){
	//generar el codigo para el from
	fprintf(latexFile, "$\\sigma$_{");
	printTables(from->tables, latexFile);
	fprintf(latexFile, "}\n");
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
		fprintf(latexFile, fromToPrint[tables->joinOnType]); // fixme el array de types
		printTables(tables->rightTables, latexFile);
		break;

		case NATURAL_JOIN_TABLES:
		printTables(tables->leftTables, latexFile);
		fprintf(latexFile, "$\\bowtie$"); // fixme el array de types
		printTables(tables->rightTables, latexFile);
		break;

		case PARENTHESIS_TABLES:
		fprintf(latexFile, "(");
		printTables(tables->leftTables, latexFile);
		fprintf(latexFile, ")");
		break;

			case ASSIGNMENT_TABLES:
		fprintf(latexFile, "(");
		printTables(tables->leftTables, latexFile);
		fprintf(latexFile, ","); // fixme el array de types
		printTables(tables->rightTables, latexFile);
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
		if(column->constant->type == TABLE_COLUMN_CONST){
			fprintf(latexFile, "%s.%s", column->constant->firstVar, column->constant->secondVar);
		}
		else{
			fprintf(latexFile, "%s", column->constant->firstVar);
		}
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
		//todo
	default:
		break;
	}
}


//gnerator de codigo para la estructura select_statement


