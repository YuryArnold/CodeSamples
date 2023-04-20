#pragma once

#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdint.h>
#include "logger.h"

#define EPOLL_SIZE 500
#define BUF_SIZE 0xFFFF

static void changeFdMod(int epollfd, int fd, uint32_t _mod)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = _mod;
    if (epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev) < 0) {
        std::cerr << "Failed to modify handler to epoll\n" <<"Please stop running server"<<std::endl;
    }
}

static void regEpollFd( int epollfd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        std::cerr << "Failed to insert handler to epoll\n" <<"Please stop running server"<<std::endl;
        return;
    }
    // Set socket to non blocking mode
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0)| O_NONBLOCK);
    std::cout<<"fd added to epoll!\n"<<std::endl;
}
 
static void remEpollFd(int epollfd, int fd)
{
     epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}


