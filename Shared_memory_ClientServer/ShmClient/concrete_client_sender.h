#ifndef CONCRETE_CLIENT_SENDER_H
#define CONCRETE_CLIENT_SENDER_H
#include "shmcircularbuffer.h"
#include <QFile>
#include <queue>
#include <chrono>
#include <atomic>
#include <thread>
#include <mutex>

/**
 * @brief The concrete_client class
 * This class represents logick for
 * sending and receiveing data with
 * inherited qthread future in it.
 * It sending or receiveing data
 * asyncrounuosly.
 */
class concrete_client_sender
{
public:
    concrete_client_sender();
    concrete_client_sender(std::mutex* _shared_mutex, ShmCircularBuffer* _buffer, int _id);
    ~concrete_client_sender();
    void set_shared_mutex(std::mutex* _mtx);
    void set_id(int _id);
    int set_path_to_file(const char* path_to_file);
    void set_ptr_to_shared_buffer(ShmCircularBuffer* _bufer);
    void set_force_quit(bool _quit);
    const char* get_file_name();

private:
    int open_file();
    void start();
    void write_data();

private:
    std::mutex* shared_mutex;
    ShmCircularBuffer* buffer;
    const char* path_to_file;
    int id;
    QFile file;
    uint32_t file_size;
    uint16_t size_of_message;
    std::thread thread_for_sending;
    std::atomic<bool> force_quit;

};

#endif // CONCRETE_CLIENT_SENDER_H
