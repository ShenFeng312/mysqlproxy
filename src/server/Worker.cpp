//
// Created by shenfeng on 11/4/25.
//

#include "Worker.h"

#include <numbers>
#include <thread>
#include <sys/epoll.h>

#include "MainServer.h"
#include "../connection/Connection.h"

class Connection;

namespace server {
    void Worker::start(const int fd_) {
        fd = fd_;
        thread = std::thread(&Worker::innerLoop, this);
    }

    void Worker::innerLoop() {
        epoll_event events[MAX_EVENTS];
        while (true) {
            int nfds = epoll_wait(fd, events, MAX_EVENTS, -1);
            for (int i = 0; i < nfds; ++i) {
                if (void *ptr = events[i].data.ptr; ptr == &fd) {
                    // // 新连接
                    // int conn_fd = accept(listen_fd, NULL, NULL);
                    // if (conn_fd == -1) {
                    //     perror("accept");
                    //     continue;
                    // }
                    // int epfd = workers[index%workerCount].getFd();
                    // Connection *connection = new Connection(conn_fd,epfd);
                    // set_nonblocking(conn_fd);
                    // ev.events =  EPOLLOUT;
                    // ev.data.ptr = connection;
                    //
                    // epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev);
                    // printf("Accepted new connection: fd=%d\n", conn_fd);
                    // print_client_addr(conn_fd);
                    // connection->createHandshakePacket();
                    // epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev);
                    // connection->setHandler(*connection->writeToClient);
                    // connection->invokeHandler();
                } else if (events[i].events & EPOLLIN) {
                    // 可读事件
                    Connection *connection = (Connection *) events[i].data.ptr;
                    MySQLPacket *client_packet = connection->readClientPacket();
                    if (client_packet == nullptr) {
                        int a = 1;
                    } else {
                        int a = 1;
                    }
                    // char buf[BUF_SIZE];
                    // int fd = connection->getFd();
                    // ssize_t count = read(fd, buf, sizeof(buf));
                    // if (count <= 0) {
                    //     if (count < 0) perror("read");
                    //     printf("Client disconnected: fd=%d\n", fd);
                    //     close(fd);
                    //     epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                    // } else {
                    //     // 回显
                    //     write(fd, buf, count);
                    //     buf[count] = '\0';
                    //     printf("Received: %s", buf);
                    // }
                } else if (events[i].events & EPOLLOUT) {
                    Connection *connection = (Connection *) events[i].data.ptr;
                    connection->writeToClient();
                }
            }
        }
    }
} // server
