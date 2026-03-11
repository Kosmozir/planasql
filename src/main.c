#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "buffer.h"
#include "compiler.h"

void print_prompt(void) {
    printf("planasql > ");
}

int main(void) {
    Table* table = new_table();
    InputBuffer* input_buffer = new_input_buffer();
    
    while(1) {
        print_prompt();
        read_buffer_line(input_buffer);

        if(input_buffer->buffer[0] == '.') {
            switch (meta_handler(input_buffer)) {
                case META_COMMAND_SUCCESS:
                    continue;;
                
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement))
        {
        case PREPARE_SUCCESS:
            break;
        
        case PREPARE_SYNTAX_ERROR:
            printf("Syntax error, could not prepare statement\n");
            continue;
        
        case PREPARE_UNRECOGNIZED_STATEMENT:
            printf("Unrecognized keyword at start of '%s'\n", input_buffer->buffer);
            continue;
        }

        switch(exec_statement(&statement, table)) {
            case EXECUTE_SUCCESS:
                printf("Execution successful\n");
                break;
            
            case EXECUTE_TABLE_FULL:
                printf("ERROR: Table full\n");
                break;
        }
    }
}