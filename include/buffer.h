#ifndef BUFFER_H
#define BUFFER_H

#include <unistd.h>
#include <stdlib.h>

typedef struct {
    char* buffer;
    ssize_t buffer_length;
    ssize_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer(void);
void read_buffer_line(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer);

#endif