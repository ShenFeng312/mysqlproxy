#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>

#include "src/mysqlpacket/MySQLHandshakePacket.h"
#include "src/server/MainServer.h"

#define MAX_EVENTS 10
#define PORT 8080
#define BUF_SIZE 1024

// // 设置非阻塞模式
// int set_nonblocking(int fd) {
//     int flags = fcntl(fd, F_GETFL, 0);
//     return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
// }
//
// void print_client_addr(int conn_fd) {
//     struct sockaddr_in addr;
//     socklen_t addr_len = sizeof(addr);
//     if (getpeername(conn_fd, (struct sockaddr*)&addr, &addr_len) == 0) {
//         char ip[INET_ADDRSTRLEN];
//         inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip)); // 转为点分十进制
//         printf("Client connected from %s:%d\n", ip, ntohs(addr.sin_port));
//     } else {
//         perror("getpeername failed");
//     }
// }

int main() {
    server::MainServer main_server(8);
    main_server.start();
    // int listen_fd, conn_fd, epoll_fd;
    // sockaddr_in addr;
    // epoll_event ev, events[MAX_EVENTS];
    //
    // // 创建监听 socket
    // listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    // if (listen_fd < 0) {
    //     perror("socket");
    //     exit(1);
    // }
    //
    // int opt = 1;
    // setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    //
    // memset(&addr, 0, sizeof(addr));
    // addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = INADDR_ANY;
    // addr.sin_port = htons(PORT);
    //
    // if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    //     perror("bind");
    //     exit(1);
    // }
    //
    // listen(listen_fd, 5);
    // set_nonblocking(listen_fd);
    //
    // // 创建 epoll 实例
    // epoll_fd = epoll_create1(0);
    // if (epoll_fd == -1) {
    //     perror("epoll_create1");
    //     exit(1);
    // }
    //
    // ev.events = EPOLLIN;  // 可读事件
    // ev.data.fd = listen_fd;
    // epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);
    //
    // printf("Server listening on port %d...\n", PORT);
    //
    // while (1) {
    //     int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    //     if (nfds == -1) {
    //         perror("epoll_wait");
    //         exit(1);
    //     }
    //
    //     for (int i = 0; i < nfds; ++i) {
    //         int fd = events[i].data.fd;
    //
    //         if (fd == listen_fd) {
    //             // 新连接
    //             conn_fd = accept(listen_fd, NULL, NULL);
    //             if (conn_fd == -1) {
    //                 perror("accept");
    //                 continue;
    //             }
    //             set_nonblocking(conn_fd);
    //             ev.events = EPOLLIN | EPOLLET;  // 边缘触发
    //             ev.data.fd = conn_fd;
    //             epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev);
    //             printf("Accepted new connection: fd=%d\n", conn_fd);
    //             print_client_addr(conn_fd);
    //             uint8_t id = 0;
    //             // MySQLHandshakePacket packet = MySQLHandshakePacket(conn_fd,id,);
    //             // packet.write();
    //
    //         } else if (events[i].events & EPOLLIN) {
    //             // 可读事件
    //             char buf[BUF_SIZE];
    //             ssize_t count = read(fd, buf, sizeof(buf));
    //             if (count <= 0) {
    //                 if (count < 0) perror("read");
    //                 printf("Client disconnected: fd=%d\n", fd);
    //                 close(fd);
    //                 epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    //             } else {
    //                 // 回显
    //                 write(fd, buf, count);
    //                 buf[count] = '\0';
    //                 printf("Received: %s", buf);
    //             }
    //         }
    //     }
    // }
    //
    // close(listen_fd);
    // close(epoll_fd);
    // return 0;
}