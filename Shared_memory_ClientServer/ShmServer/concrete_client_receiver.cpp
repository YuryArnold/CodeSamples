#include "concrete_client_receiver.h"

concrete_client_receiver::concrete_client_receiver()
{

}

concrete_client_receiver::~concrete_client_receiver()
{
    force_quit = true;
    if(thread_for_receiver.joinable())
        thread_for_receiver.join();

    if(file.isOpen())
        file.close();
}


int concrete_client_receiver::set_path_to_file(const char* _path_to_file)
{
    path_to_file = _path_to_file;
    if (0 != open_file())
        return -1;
    start();
    return 0;
}

void concrete_client_receiver::set_message_to_container(message &_incoming_message)
{
    std::lock_guard<std::mutex> lock(m_mtx_for_container);
    m_messages.push(_incoming_message);
}

void concrete_client_receiver::read_data()
{
    message rec_mes;
    forever{
        if(!m_messages.empty()){
            {
                std::lock_guard<std::mutex> lock(m_mtx_for_container);
                rec_mes = m_messages.front();
                m_messages.pop();
                std::cout<<rec_mes.msg_number<<" Block size "<<rec_mes.data_size<<std::endl;
            }
             file.write(QByteArray(reinterpret_cast<const char*>(rec_mes.data),rec_mes.data_size));

            if(rec_mes.is_over || force_quit)
                break;

        } else {
             std::this_thread::sleep_for(std::chrono::milliseconds(m_sleep_period));
        }
    }
    file.close();
    std::cout<<"Writing file has accomplished!"<<std::endl;
}

void concrete_client_receiver::set_force_quit(bool _quit)
{
    force_quit = _quit;
}

void concrete_client_receiver::start()
{
     thread_for_receiver = std::thread(&concrete_client_receiver::read_data,this);
    /*Waiting for thread started*/
     std::this_thread::sleep_for(std::chrono::milliseconds(m_sleep_period));
}

int concrete_client_receiver::open_file()
{
    if(file.isOpen()){
        force_quit = true;
        if(thread_for_receiver.joinable())
            thread_for_receiver.join();
        file.close();
    }

    force_quit = false;
    file.setFileName(path_to_file);
    if(!file.open(QIODevice::ReadWrite)){
        file.close();
        return -1;
    }
   return 0;
}

