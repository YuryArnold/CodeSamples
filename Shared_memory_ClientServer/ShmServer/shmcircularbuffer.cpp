#include "shmcircularbuffer.h"


ShmCircularBuffer::ShmCircularBuffer(TypeOfBuffer _bufType)
{
    bufType = _bufType;
}

ShmCircularBuffer::~ShmCircularBuffer()
{
    // first, I have to free allocated memory
    munmap (NULL, sizeof (struct shared_memory));

    //after it I have to release memory mapper
    shm_unlink (SHARED_MEM_NAME);
}

/**
 * @brief ShmCircularBuffer::set_message
 * @param _msg
 */
void ShmCircularBuffer::set_message(const message _msg)
{
    if(bufType == ServerBuffer)
        throw "Can't set message in this type of circular buffer";

    if (sem_wait (buffer_count_sem) == -1)
        throw "error in semaphore for counting messages.";

    if (sem_wait (mutex_sem) == -1)
        throw "error in waiting semaphore for writing message from concrete client";

        shared_mem_ptr -> buf [shared_mem_ptr -> buffer_in_index] = _msg;

        (shared_mem_ptr -> buffer_in_index)++;
        if (shared_mem_ptr -> buffer_in_index == MAX_BUFFERS)
            shared_mem_ptr -> buffer_in_index = 0;

    if (sem_post (mutex_sem) == -1)
        throw "error in waiting semaphore for writing message from concrete client";

    if (sem_post (spool_signal_sem) == -1)
        throw "error in semaphore for counting messages.";

}

/**
 * @brief ShmCircularBuffer::get_message
 * @return
 */
message ShmCircularBuffer::get_message()
{

    if(bufType == ClientBuffer)
        throw "Can't receive message for this kind of buffer.";

    message tmpRecMes;
    if (sem_wait (spool_signal_sem) == -1)
        throw "error in waiting semaphore for getting message from buffer.";

     tmpRecMes = shared_mem_ptr->buf[shared_mem_ptr->buffer_out_index];

    (shared_mem_ptr -> buffer_out_index)++;
    if (shared_mem_ptr -> buffer_out_index == MAX_BUFFERS)
       shared_mem_ptr -> buffer_out_index = 0;

    if (sem_post (buffer_count_sem) == -1)
        throw "error in semaphore for alerting produsers of messages.";

    return tmpRecMes;
}


void ShmCircularBuffer::open()
{
    if ((mutex_sem = sem_open (SEM_MUTEX_NAME, O_CREAT, 0660, 0)) == SEM_FAILED)
        throw "error in intializing named shared semophore";

    // Get shared memory
    if ((fd_shm = shm_open (SHARED_MEM_NAME, O_RDWR | O_CREAT, 0660)) == -1)
         throw "error in opening shared memory.";

    if (ftruncate (fd_shm, sizeof (struct shared_memory)) == -1)
       throw "error in truncating shared memory";

    if ((shared_mem_ptr = (shared_memory *)mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,
            fd_shm, 0)) == MAP_FAILED)
       throw "error in initializing shared momory";

    // Initialize the shared memory
    shared_mem_ptr -> buffer_in_index = shared_mem_ptr -> buffer_out_index = 0;

    switch(bufType){
        case ServerBuffer:
            // counting semaphore, indicating the number of available buffers. Initial value = MAX_BUFFERS
            if ((buffer_count_sem = sem_open (SEM_BUFFER_COUNT_NAME, O_CREAT, 0660, MAX_BUFFERS)) == SEM_FAILED)
                throw  "error in opening semaphores for counting number of messages.";

            // counting semaphore, Initial value = 0
            if ((spool_signal_sem = sem_open (SEM_SPOOL_SIGNAL_NAME, O_CREAT, 0660, 0)) == SEM_FAILED)
                throw  "error in opening semaphores for countig of number readable messages.";

            // Initialization complete; now we can set mutex semaphore as 1 to
            // indicate shared memory segment is available
            if (sem_post (mutex_sem) == -1)
                throw  "error in sending shared mutex into ";

        break;

        case ClientBuffer:
            // counting semaphore, indicating the number of available buffers.
            if ((buffer_count_sem = sem_open (SEM_BUFFER_COUNT_NAME, 0, 0, 0)) == SEM_FAILED)
                throw  "error in opening semaphores for counting number of messages.";

            // counting semaphore, indicating the number of strings to be printed. Initial value = 0
            if ((spool_signal_sem = sem_open (SEM_SPOOL_SIGNAL_NAME, 0, 0, 0)) == SEM_FAILED)
                throw  "error in opening semaphores for countig of number readable messages.";

        break;

        default:
            break;

    }


}

void ShmCircularBuffer::close()
{
    // first, I have to free allocated memory
    munmap (NULL, sizeof (struct shared_memory));

    //after it I have to release memory mapper
    shm_unlink (SHARED_MEM_NAME);
}


