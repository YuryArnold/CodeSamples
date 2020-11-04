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
 * sending and receiveing data.
 * It sending or receiveing data
 * asyncrounuosly.
 */

class concrete_client_sender
{
public:
    concrete_client_sender();
    concrete_client_sender(const std::shared_ptr<ShmCircularBuffer>& _buffer, int _id);
    ~concrete_client_sender();
    void set_id(int _id);
    int set_path_to_file(const char* path_to_file);
    void set_ptr_to_shared_buffer(const std::shared_ptr<ShmCircularBuffer>& _bufer);
    void set_force_quit(bool _quit);
    const char* get_file_name();

private:
    int open_file();
    void start();
    void write_data();

private:
    std::shared_ptr<ShmCircularBuffer> buffer;
    const char* path_to_file;
    int id;
    QFile file;
    uint32_t file_size;
    std::atomic<bool> force_quit{false};
    uint16_t size_of_message = 4096;
    std::thread thread_for_sending;
};

#endif // CONCRETE_CLIENT_SENDER_H
