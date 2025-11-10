//
// Created by shenfeng on 11/4/25.
//

#include "MainServer.h"

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <arpa/inet.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include "../mysqlpacket/MySQLHandshakePacket.h"
#include "../connection/Connection.h"


namespace server {
    int set_nonblocking(int fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }

    void print_client_addr(int conn_fd) {
        sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        if (getpeername(conn_fd, (sockaddr *) &addr, &addr_len) == 0) {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip)); // 转为点分十进制
            printf("Client connected from %s:%d\n", ip, ntohs(addr.sin_port));
        } else {
            perror("getpeername failed");
        }
    }


    void MainServer::start() {
        for (int i = 0; i < workerCount; i++) {
            int epoll_fd = epoll_create1(0);
            workers[i].start(epoll_fd);
        }
        startMainLoop();
    }

    void MainServer::startMainLoop() {
        uint64_t index = 0;
        int listen_fd, epoll_fd;
        sockaddr_in addr;

        // 创建监听 socket
        listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_fd < 0) {
            perror("socket");
            exit(1);
        }

        int opt = 1;
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(PORT);

        if (bind(listen_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("bind");
            exit(1);
        }

        listen(listen_fd, 5);
        set_nonblocking(listen_fd);

        // 创建 epoll 实例
        epoll_fd = epoll_create1(0);
        if (epoll_fd == -1) {
            perror("epoll_create1");
            exit(1);
        }
        epoll_event ev{}, events[MAX_EVENTS];

        ev.events = EPOLLIN; // 可读事件
        ev.data.ptr = &listen_fd;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

        printf("Server listening on port %d...\n", PORT);

        while (1) {
            int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
            if (nfds == -1) {
                perror("epoll_wait");
                int errno_location = errno;
                // if (errno_location == EINTR) continue;
                // exit(1);
                continue;
            }

            for (int i = 0; i < nfds; ++i) {
                if (void *ptr = events[i].data.ptr; ptr == &listen_fd) {
                    // 新连接
                    int conn_fd = accept(listen_fd, NULL, NULL);
                    if (conn_fd == -1) {
                        perror("accept");
                        continue;
                    }
                    int epfd = workers[index % workerCount].getFd();
                    Connection *connection = new Connection(conn_fd, epfd);
                    set_nonblocking(conn_fd);
                    ev.events = EPOLLOUT;
                    ev.data.ptr = connection;

                    epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev);
                    printf("Accepted new connection: fd=%d\n", conn_fd);
                    print_client_addr(conn_fd);
                    connection->createHandshakePacket();
                    epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev);
                    // connection->setHandler(*connection->writeToClient);
                    // connection->invokeHandler();
                } else if (events[i].events & EPOLLIN) {
                    // 可读事件
                    Connection *connection = (Connection *) events[i].data.ptr;
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
                }
            }
        }

        close(listen_fd);
        close(epoll_fd);
    }
} // server
