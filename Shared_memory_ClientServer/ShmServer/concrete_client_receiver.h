#ifndef CONCRETE_CLIENT_H
#define CONCRETE_CLIENT_H
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
class concrete_client_receiver
{
public:
    concrete_client_receiver();
    ~concrete_client_receiver();
    int set_path_to_file(const char* path_to_file);
    void set_message_to_container(message& _incoming_message);
    void set_force_quit(bool _quit);

private:
    int open_file();
    void start();
    void read_data();

private:
    std::thread thread_for_receiver;
    std::mutex m_mtx_for_container;
    const char* path_to_file;
    std::queue<message> m_messages;
    QFile file;
    std::atomic<bool> force_quit{false};
    int m_sleep_period = 200;
};

#endif // CONCRETE_CLIENT_H
