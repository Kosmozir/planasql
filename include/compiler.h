#ifndef COMPILER_H
#define COMPILER_H

#include "buffer.h"

#include <stdint.h>

typedef enum { META_COMMAND_SUCCESS,META_COMMAND_UNRECOGNIZED_COMMAND } MetaCommandResult;
typedef enum { PREPARE_SUCCESS,PREPARE_UNRECOGNIZED_STATEMENT, PREPARE_SYNTAX_ERROR } PrepareResult;
typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;
typedef enum { EXECUTE_SUCCESS, EXECUTE_TABLE_FULL } ExecuteResult;

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define PAGE_SIZE 4096
#define TABLE_MAX_PAGES 100

typedef struct { 
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} Row;

typedef struct {
    StatementType type;
    Row inserting_row;
} Statement;

typedef struct {
    uint32_t num_rows;
    void* pages[TABLE_MAX_PAGES];
} Table; 

#define size_of_field(Struct, Attribute) sizeof(((Struct*)NULL)->Attribute)

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);
MetaCommandResult meta_handler(InputBuffer* input_buffer);
void serialize_row(Row* source, void* destination);
void deserialize_row(void* source, Row* destination);
void* row_slot(Table* table, uint32_t num_rows);
Table* new_table();
void free_table(Table* table);
ExecuteResult exec_insert(Statement* statement, Table* table);
ExecuteResult exec_statement(Statement* statement, Table* table);

#endif