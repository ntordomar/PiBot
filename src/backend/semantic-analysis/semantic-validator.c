#include "semantic-validator.h"
#include <stdarg.h>

int errorIndex = 0;
void addErrorToArray(char * error, ...){

    va_list args;
    va_start(args, error);
    char * errorWithArgs = malloc(1000);
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
	key->columnName = malloc(strlen(columnName)+1);
	strcpy(key->columnName, columnName);
	val->type = type;
	if(tableName != NULL) {
		key->tableName = malloc(strlen(tableName)+1);
		strcpy(key->tableName, tableName);
	} else {
		key->tableName = NULL;
	}
}

// checkTableExists

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
			// todo falta chequear que la tabla exista en la lista
			if(!symbolTableFindTable(column->constant->firstVar)) {
                addErrorToArray("La tabla %s no existe", column->constant->firstVar);
            }
		}
	}
}

void checkValidTableReferences(Columns * columns) {
	if(columns == NULL) return;
	Column * column = columns->column; 
	// select chau,hola,avg(num)/sum(hola)*2
	if(column == NULL) return;
	if((column) == false) return;
	checkValidTableReference(column);
	checkValidTableReferences(columns->columns);
}


void checkValidTablesInProgram(Program * program) {

	// tengo que chequear las del select, from , where , group by, having, order by
	// if(program->group_by_statement == NULL) return;
	Columns * columns = program->select_statement->columns;
	checkValidTableReferences(columns);

	columns = program->group_by_statement->columns;
	checkValidTableReferences(columns);
	
	columns = program->order_by_statement->columns;
    checkValidTableReferences(columns);
		
}

//estaEnSegundaLista

bool checkContainsTable(char * table, Columns * sLista) {

	Columns * segundaLista = sLista;
    while (segundaLista != NULL) {
		char * iter = segundaLista->column->constant->firstVar;
		if(segundaLista->column->constant->type == TABLE_COLUMN_CONST) {
			iter = segundaLista->column->constant->secondVar;
		}
        if (strcmp(iter, table) == 0) {
            return true; // El elemento está en la segunda lista
        }
        segundaLista = segundaLista->columns;
    }
    return false; // El elemento no está en la segunda lista
}

// Función para verificar si todos los elementos de la primera lista están en la segunda lista
//todosEnSegundaLista
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
			if (checkContainsTable(table, segundaLista) == false) return false; // Al menos un elemento no está en la segunda lista
        }
        primeraLista = primeraLista->columns;
    }
    return true; // Todos los elementos de la primera lista están en la segunda lista
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
		
		// C1 ambas son col
		if((leftConstant->type == TABLE_COLUMN_CONST ||  leftConstant->type == VAR_CONST) && (rightConstant->type == TABLE_COLUMN_CONST || rightConstant->type == VAR_CONST)){
			
			KeyStruct *key = malloc(sizeof(KeyStruct));
			ValueStruct* val = malloc(sizeof(ValueStruct));
			if(leftConstant->type == VAR_CONST) createSymbolEntry(key,val, NULL,leftConstant->firstVar,UNDEFINED);
			else createSymbolEntry(key,val, leftConstant->firstVar,leftConstant->secondVar,UNDEFINED);
			EntryStruct * entry1 = symbolTableFind(key,val);

			KeyStruct *key2 = malloc(sizeof(KeyStruct));
			ValueStruct* val2 = malloc(sizeof(ValueStruct));
			if(rightConstant->type == VAR_CONST) createSymbolEntry(key2,val2, NULL,rightConstant->firstVar,UNDEFINED);
			else createSymbolEntry(key2,val2, rightConstant->firstVar,rightConstant->secondVar,UNDEFINED);
			EntryStruct * entry2 = symbolTableFind(key2,val2);
			//c1.1 las dos estan en la tabla de simbolos
			if(entry1 != NULL && entry2 != NULL){
				if(entry1->value.type != UNDEFINED && entry2->value.type != UNDEFINED && entry1->value.type != entry2->value.type){
						//ERROR Y EXIT
						addErrorToArray("Las columnas %s y %s no son del mismo tipo", leftConstant->firstVar, rightConstant->firstVar);
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
		else if (leftConstant->type == TABLE_COLUMN_CONST || leftConstant->type == VAR_CONST){
			KeyStruct *key = malloc(sizeof(KeyStruct));
			ValueStruct* val = malloc(sizeof(ValueStruct));
			if(leftConstant->type == VAR_CONST){
				createSymbolEntry(key,val, NULL,leftConstant->firstVar,UNDEFINED);
			}else{
				createSymbolEntry(key,val,leftConstant->firstVar,leftConstant->secondVar,UNDEFINED);
			}
			EntryStruct * entry = symbolTableFind(key,val);
			
			// c2.1 esta en la tabla de simbolos
			if(entry != NULL && entry->value.type != UNDEFINED && entry->value.type != rightConstant->type){
					//ERROR Y EXIT
					addErrorToArray("La columna %s no es del mismo tipo que la constante", leftConstant->firstVar);
			}
			else{
					//c2.1.1 no tiene type asignado
					if(rightConstant->type == INTEGER_CONST){
						val->type = INTEGER_CONST;
					}else if(rightConstant->type == APOST_CONST){
						val->type = APOST_CONST;
					}
					// insert en la tabla de simbolos
					symbolTableInsert(key, val);
			}
		}

		// C3 solo la der es col
		else if (rightConstant->type == TABLE_COLUMN_CONST || rightConstant->type == VAR_CONST){
			KeyStruct *key = malloc(sizeof(KeyStruct));
			ValueStruct* val = malloc(sizeof(ValueStruct));
			if(rightConstant->type == VAR_CONST){
				createSymbolEntry(key,val, NULL,rightConstant->firstVar,UNDEFINED);
			}else{
			createSymbolEntry(key,val, rightConstant->firstVar,rightConstant->secondVar,UNDEFINED);
			}
			EntryStruct * entry = symbolTableFind(key,val);
			//c2.1 esta en la tabla de simbolos
			if(entry != NULL && entry->value.type != UNDEFINED && entry->value.type != leftConstant->type){
					//ERROR Y EXIT
					addErrorToArray("La columna %s no es del mismo tipo que la constante", rightConstant->firstVar);
			}else{
					//c2.1.1 no tiene type asignado
					if(leftConstant->type == INTEGER_CONST){
						val->type = INTEGER_CONST;
					}else if(leftConstant->type == APOST_CONST){
						val->type =  APOST_CONST;
					}
					// insert en la tabla de simbolos
					symbolTableInsert(key, val);
			}
		}
	}
}