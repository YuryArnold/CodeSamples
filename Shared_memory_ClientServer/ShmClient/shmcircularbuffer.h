#ifndef SHMCIRCULARBUFFER_H
#define SHMCIRCULARBUFFER_H
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <QByteArray>
#include <iostream>
#include <exception>
#include <memory>
#include "messagestructures.h"

enum TypeOfBuffer {
    ServerBuffer,
    ClientBuffer
};

class ShmCircularBuffer
{
public:
    ShmCircularBuffer(TypeOfBuffer _bufType);
    ~ShmCircularBuffer();
    void set_message(const message _msg);
    message get_message();
    void open();
    void close();

private:
    shared_memory *shared_mem_ptr;
    sem_t *mutex_sem, *buffer_count_sem, *spool_signal_sem;
    int fd_shm;
    TypeOfBuffer bufType;
    uint32_t id;
    uint32_t msg_cnt;

};

#endif // SHMCIRCULARBUFFER_H
