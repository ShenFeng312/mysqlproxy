//
// Created by shenfeng on 11/4/25.
//

#ifndef MAINSERVER_H
#define MAINSERVER_H
#include "Worker.h"

namespace server {
#define MAX_EVENTS 10
#define PORT 8080
#define BUF_SIZE 16*1024*1024

    class MainServer {
    private:
        Worker *workers;
        size_t workerCount;

        void startMainLoop();

    public:
        MainServer(int workerCount): workerCount(workerCount) {
            workers = new Worker[workerCount];
        }

        void start();

        void stop();
    };
} // server

#endif //MAINSERVER_H
