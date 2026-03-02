#ifndef COMPILER_H
#define COMPILER_H

#include "buffer.h"

typedef enum { META_COMMAND_SUCCESS,META_COMMAND_UNRECOGNIZED_COMMAND } MetaCommandResult;
typedef enum { PREPARE_SUCCESS,PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;
typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;
typedef struct { StatementType type; } Statement;

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);
void execute_command(Statement* statement);

#endif