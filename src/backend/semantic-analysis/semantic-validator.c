#include "semantic-validator.h"
#include <stdarg.h>
#include "../support/memory-manager.h"

int errorIndex = 0;
void addErrorToArray(char * error, ...){

    va_list args;
    va_start(args, error);
    char * errorWithArgs = mm_malloc(MAX_ERROR_LEN);
    vsprintf(errorWithArgs, error, args);
    va_end(args);

    if(errorIndex < MAX_ERRORS){
        errorsName[errorIndex] = errorWithArgs;
        errorIndex++;
    }

}

void printErrors(){
    for(int i = 0; i < errorIndex; i++){
        printf("[ERROR] [Bison] %s\n", errorsName[i]);
    }
}



void createSymbolEntry(KeyStruct * key, ValueStruct * val, char * tableName, char * columnName, TypeColumn type) {
	key->columnName = mm_malloc(strlen(columnName)+1);
	strcpy(key->columnName, columnName);
	val->type = type;
	if(tableName != NULL) {
		key->tableName = mm_malloc(strlen(tableName)+1);
		strcpy(key->tableName, tableName);
	} else {
		key->tableName = NULL;
	}
}

void checkTableExists(Constant * constant){
	if(constant!= NULL && constant->type == TABLE_COLUMN_CONST && !symbolTableFindTable(constant->firstVar)){
		addErrorToArray("La tabla %s no existe", constant->firstVar);
	}
}

void checkValidTableReference(Column *column){
    if(column == NULL) return;
	if(column->type == EXPRESSION_COLUMN) {
        checkValidTableReference(column->leftColumn);
        checkValidTableReference(column->rightColumn);
    }
	if(column->type == ASSIGNMENT_COLUMN) checkValidTableReference(column->leftColumn);
	if(column->type == UNIQUE_COLUMN || column->type == AGGREGATION_COLUMN ) {
		if(column->constant->type == TABLE_COLUMN_CONST) {
			if(!symbolTableFindTable(column->constant->firstVar)) {
                addErrorToArray("La tabla %s no existe", column->constant->firstVar);
            }
		}
	}
}

void checkValidTableReferences(Columns * columns) {
	if(columns == NULL) return;
	Column * column = columns->column; 
	if(column == NULL) return;
	if((column) == false) return;
	checkValidTableReference(column);
	checkValidTableReferences(columns->columns);
}


void checkValidTablesInProgram(Program * program) {

	Columns * columns = program->select_statement->columns;
	checkValidTableReferences(columns);

	columns = program->group_by_statement->columns;
	checkValidTableReferences(columns);
	
	columns = program->order_by_statement->columns;
    checkValidTableReferences(columns);
		
}

bool checkContainsTable(char * table, Columns * sLista) {

	Columns * segundaLista = sLista;
    while (segundaLista != NULL) {
		char * iter = segundaLista->column->constant->firstVar;
		if(segundaLista->column->constant->type == TABLE_COLUMN_CONST) {
			iter = segundaLista->column->constant->secondVar;
		}
        if (strcmp(iter, table) == 0) {
            return true;
        }
        segundaLista = segundaLista->columns;
    }
    return false; 
}

bool checkListInsideOther(Columns* pLista, Columns * segundaLista) {
	Columns * primeraLista = pLista;
    while (primeraLista != NULL) {
        if (primeraLista->column->type == UNIQUE_COLUMN && ( primeraLista->column->constant->type == TABLE_COLUMN_CONST || primeraLista->column->constant->type == VAR_CONST ) ) {
			char * table = primeraLista->column->constant->firstVar;
			if(primeraLista->column->constant->type == TABLE_COLUMN_CONST){
				table = primeraLista->column->constant->secondVar;
			}
			else{
				table = primeraLista->column->constant->firstVar;
			}
			if (checkContainsTable(table, segundaLista) == false) return false; 
        }
        primeraLista = primeraLista->columns;
    }
    return true;
}

void checkValidGroupByClause(Program * program){
	if(program->group_by_statement->columns == NULL) return;
	if(checkListInsideOther(program->select_statement->columns, program->group_by_statement->columns) == false){
		addErrorToArray("No se puede agrupar por una columna que no esta en el select");
	}	
}

void checkColumnsType(Constant * leftConstant, Constant * rightConstant){
    if(leftConstant->type == TABLE_COLUMN_CONST || rightConstant->type == TABLE_COLUMN_CONST || leftConstant->type == VAR_CONST || rightConstant->type == VAR_CONST){
		if(leftConstant->type == TABLE_COLUMN_CONST) {
			if(!symbolTableFindTable(leftConstant->firstVar)) {
				addErrorToArray("La tabla %s no existe",  leftConstant->firstVar);
			}
		}
		if(rightConstant->type == TABLE_COLUMN_CONST) {
			if(!symbolTableFindTable(rightConstant->firstVar)) {
				addErrorToArray("La tabla %s no existe",  leftConstant->firstVar);
			}
		}
		
		if((leftConstant->type == TABLE_COLUMN_CONST ||  leftConstant->type == VAR_CONST) && (rightConstant->type == TABLE_COLUMN_CONST || rightConstant->type == VAR_CONST)){
			
			KeyStruct *key = mm_malloc(sizeof(KeyStruct));
			ValueStruct* val = mm_malloc(sizeof(ValueStruct));
			if(leftConstant->type == VAR_CONST) createSymbolEntry(key,val, NULL,leftConstant->firstVar,UNDEFINED);
			else createSymbolEntry(key,val, leftConstant->firstVar,leftConstant->secondVar,UNDEFINED);
			EntryStruct * entry1 = symbolTableFind(key,val);

			KeyStruct *key2 = mm_malloc(sizeof(KeyStruct));
			ValueStruct* val2 = mm_malloc(sizeof(ValueStruct));
			if(rightConstant->type == VAR_CONST) createSymbolEntry(key2,val2, NULL,rightConstant->firstVar,UNDEFINED);
			else createSymbolEntry(key2,val2, rightConstant->firstVar,rightConstant->secondVar,UNDEFINED);
			EntryStruct * entry2 = symbolTableFind(key2,val2);
			if(entry1 != NULL && entry2 != NULL){
				if(entry1->value.type != UNDEFINED && entry2->value.type != UNDEFINED && entry1->value.type != entry2->value.type){
						addErrorToArray("Las columnas %s y %s no son del mismo tipo", leftConstant->firstVar, rightConstant->firstVar);
				}else if(entry1->value.type == UNDEFINED && entry2->value.type != UNDEFINED){
					symbolTableInsert(key, val2);
				}else if (entry1->value.type != UNDEFINED && entry2->value.type == UNDEFINED){
					symbolTableInsert(key2, val);
				}
			}
			else if(entry1 != NULL){	
					symbolTableInsert(key2, val);
			}
			else if(entry2 != NULL){
					symbolTableInsert(key, val2);
			}
		}
		else if (leftConstant->type == TABLE_COLUMN_CONST || leftConstant->type == VAR_CONST){
			KeyStruct *key = mm_malloc(sizeof(KeyStruct));
			ValueStruct* val = mm_malloc(sizeof(ValueStruct));
			if(leftConstant->type == VAR_CONST){
				createSymbolEntry(key,val, NULL,leftConstant->firstVar,UNDEFINED);
			}else{
				createSymbolEntry(key,val,leftConstant->firstVar,leftConstant->secondVar,UNDEFINED);
			}
			EntryStruct * entry = symbolTableFind(key,val);
			
			if(entry != NULL && entry->value.type != UNDEFINED && entry->value.type != rightConstant->type){
					addErrorToArray("La columna %s no es del mismo tipo que la constante", leftConstant->firstVar);
			}
			else{
					if(rightConstant->type == INTEGER_CONST){
						val->type = INTEGER_CONST;
					}else if(rightConstant->type == APOST_CONST){
						val->type = APOST_CONST;
					}
					symbolTableInsert(key, val);
			}
		}

		else if (rightConstant->type == TABLE_COLUMN_CONST || rightConstant->type == VAR_CONST){
			KeyStruct *key = mm_malloc(sizeof(KeyStruct));
			ValueStruct* val = mm_malloc(sizeof(ValueStruct));
			if(rightConstant->type == VAR_CONST){
				createSymbolEntry(key,val, NULL,rightConstant->firstVar,UNDEFINED);
			}else{
			createSymbolEntry(key,val, rightConstant->firstVar,rightConstant->secondVar,UNDEFINED);
			}
			EntryStruct * entry = symbolTableFind(key,val);
			if(entry != NULL && entry->value.type != UNDEFINED && entry->value.type != leftConstant->type){
					addErrorToArray("La columna %s no es del mismo tipo que la constante", rightConstant->firstVar);
			}else{
					if(leftConstant->type == INTEGER_CONST){
						val->type = INTEGER_CONST;
					}else if(leftConstant->type == APOST_CONST){
						val->type =  APOST_CONST;
					}
					symbolTableInsert(key, val);
			}
		}
	}
}