//
// Created by shenfeng on 11/4/25.
//

#ifndef WORKER_H
#define WORKER_H
#include <thread>

namespace server {
    class Worker {
        int fd =-1;
        std::thread thread;

    private:
        void innerLoop();
    public:
        int getFd() const {
            return fd;
        }

        void start(const int fd_);

        void stop();
    };
} // server

#endif //WORKER_H
