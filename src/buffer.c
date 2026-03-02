#include "buffer.h"
#include <stdio.h>

InputBuffer* new_input_buffer(void) {
    InputBuffer* new_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    new_buffer->buffer = NULL;
}

void read_buffer_line(InputBuffer* input_buffer) {
    ssize_t read_bytes = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if(read_bytes <= 0) {
        printf("Error, no lines read.");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = read_bytes - 1;
    input_buffer->buffer[read_bytes - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}