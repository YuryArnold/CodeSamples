#ifndef MESSAGESTRUCTURES_H
#define MESSAGESTRUCTURES_H
#include "macro_defines.h"
#include <stdint.h>

struct message{
    uint32_t id;
    uint32_t msg_number;
    uint64_t client_run_time;
    uint32_t data_size;
    uint8_t data[4096];
    bool is_over;
};

struct shared_memory {
    message buf [MAX_BUFFERS];
    int buffer_in_index;
    int buffer_out_index;
};


#endif // MESSAGESTRUCTURES_H
