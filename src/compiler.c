#include "compiler.h"

#include <stdio.h>
#include <string.h>

const uint32_t ROW_SIZE = size_of_field(Row, id) + size_of_field(Row, username) + size_of_field(Row, email);
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if(strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;

        int args_assinged = sscanf(input_buffer->buffer, "insert %d %s %s", &(statement->inserting_row.id), &(statement->inserting_row.username), &(statement->inserting_row.email));

        if(args_assinged < 3) return PREPARE_SYNTAX_ERROR;

        return PREPARE_SUCCESS;
    }
    if(strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

MetaCommandResult meta_handler(InputBuffer* input_buffer) {
    if(strcmp(input_buffer->buffer, ".exit") == 0) {
        printf("bye.\n");
        exit(EXIT_SUCCESS);
    }

    else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

void serialize_row(Row* src, void* dst) {
    memcpy(dst, &src->id, sizeof(src->id));
    memcpy(dst + sizeof(src->id), &src->username, sizeof(src->username));
    memcpy(dst + sizeof(src->id) + sizeof(src->username), &src->email, sizeof(src->email));
}

void deserialize_row(void* src, Row* dst) {
    memcpy(&dst->id, src, sizeof(dst->id));
    memcpy(&dst->username, src + sizeof(dst->id), sizeof(dst->username));
    memcpy(&dst->email, src + sizeof(dst->id) + sizeof(dst->username), sizeof(dst->email));
}

void* row_slot(Table* table, uint32_t row_num) {
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void* page = table->pages[page_num];

    if(page == NULL) {
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }

    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;

    return page + byte_offset;
}

Table* new_table() {
    Table* table = (Table*)malloc(sizeof(Table));
    table->num_rows = 0;

    for(uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        table->pages[i] = NULL;
    }

    return table;
}

void free_table(Table* table) {
    for (uint32_t i = 0; table->pages[i]; i++) {
        free(table->pages[i]);
    }
}

ExecuteResult exec_insert(Statement* statement, Table* table) {
    if(table->num_rows >= TABLE_MAX_PAGES) {
        return EXECUTE_TABLE_FULL;
    }

    Row* row_to_insert = &statement->inserting_row;
    serialize_row(row_to_insert, row_slot(table, table->num_rows));
    table->num_rows += 1;

    return EXECUTE_SUCCESS;
}  
ExecuteResult exec_select(Statement* statement, Table* table) {
    Row row;

    for(uint32_t i = 0; i < table->num_rows; i++) {
        deserialize_row(row_slot(table, i), &row);

        printf("(%d, %s, %s)\n", row.id, row.username, row.email);
    }

    return EXECUTE_SUCCESS;
}

ExecuteResult exec_statement(Statement* statement, Table* table) {
    switch(statement->type) {
        case STATEMENT_INSERT:
            return exec_insert(statement, table);

        case STATEMENT_SELECT:
            return exec_select(statement, table);
    }
}
