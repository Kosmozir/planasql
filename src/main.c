#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "buffer.h"
#include "compiler.h"

void print_prompt(void) {
    printf("PlanaDB> ");
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

int main(void) {
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
        
        case PREPARE_UNRECOGNIZED_STATEMENT:
            printf("Unrecognized keyword at start of '%s'\n", input_buffer->buffer);
            continue;
        }

        execute_command(&statement);
        printf("Executed command.\n");
    }
}