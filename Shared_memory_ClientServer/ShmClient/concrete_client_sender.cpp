#include "concrete_client_sender.h"

/**
 * @brief concrete_client_sender::concrete_client_sender
 */
concrete_client_sender::concrete_client_sender()
{

}

/**
 * @brief concrete_client_sender::concrete_client_sender
 * @param _shared_mutex
 * @param _buffer
 * @param _id
 */
concrete_client_sender::concrete_client_sender(const std::shared_ptr<ShmCircularBuffer>& _buffer,
                                               int _id) :buffer(_buffer),id(_id)
{

}

/**
 * @brief concrete_client_sender::~concrete_client_sender
 */
concrete_client_sender::~concrete_client_sender()
{
    if(thread_for_sending.joinable())
        thread_for_sending.join();
    if(file.isOpen())
        file.close();
}

/**
 * @brief concrete_client_sender::set_id
 * @param _id
 */
void concrete_client_sender::set_id(int _id)
{
    id = _id;
}

/**
 * @brief concrete_client_sender::set_path_to_file
 * @param _path_to_file
 * @return
 */
int concrete_client_sender::set_path_to_file(const char* _path_to_file)
{
    path_to_file = _path_to_file;
    if (0 != open_file())
        return -1;
    start();
    return 0;
}

/**
 * @brief concrete_client_sender::set_ptr_to_shared_buffer
 * @param _buffer
 */
void concrete_client_sender::set_ptr_to_shared_buffer(const std::shared_ptr<ShmCircularBuffer>& _buffer)
{
    buffer = _buffer;
}

void concrete_client_sender::write_data()
{
    message tmp;
    tmp.id = id;
    tmp.is_over = false;
    tmp.msg_number = 0;
    tmp.client_run_time = std::chrono::system_clock::now().time_since_epoch().count();
    while(file.isOpen() && !file.atEnd() && !force_quit){
       QByteArray raw_data = file.read(size_of_message);
       std::copy(raw_data.begin(),raw_data.end(),tmp.data);
       tmp.data_size = raw_data.size();
       file_size -= tmp.data_size;
       {
            if(file_size <= 0)
                tmp.is_over = true;
            buffer->set_message(tmp);
       }
       tmp.msg_number++;
    }
    if(file.atEnd() || force_quit)
        file.close();
}


void concrete_client_sender::set_force_quit(bool _quit)
{
    force_quit = _quit;
}

void concrete_client_sender::start()
{
    int sleep_period = 600;
    force_quit = false;
    thread_for_sending = std::thread(&concrete_client_sender::write_data,this);
    /*Waiting for thread started*/
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_period));
}

const char *concrete_client_sender::get_file_name()
{
    return path_to_file;
}

int concrete_client_sender::open_file()
{
    if(file.isOpen()){
        force_quit = true;
        if(thread_for_sending.joinable())
            thread_for_sending.join();
         file.close();
    }

    force_quit = false;
    file.setFileName(path_to_file);
    if(!file.open(QIODevice::ReadWrite)){
        file.close();
        return -1;
    }
    file_size = file.size();
    return 0;
}

