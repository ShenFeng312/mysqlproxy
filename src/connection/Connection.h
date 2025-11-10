//
// Created by shenfeng on 11/4/25.
//

#ifndef CONNECTION_H
#define CONNECTION_H

#include "../mysqlpacket/MySQLPacket.h"
#include "../utils/IDGenerator.h"


class Connection {
    int client_fd;
    int client_ep_fd;
    int connectionId = utils::IDGenerator::nextId();
    MySQLPacket *response = nullptr;
    uint8_t client_seq_id = 0;
    uint8_t payload[3];
    std::vector<uint8_t> buffer;
    int payloadIndex = 0;
    int payloadLength = -1;
    bool closed = false;

    void (*handler)() = nullptr;
    MySQLPacket* decodePacket();

public:
    Connection(int fd, int client_ep_fd): client_fd(fd), client_ep_fd(client_ep_fd) {
    }

    int getFd() {
        return client_fd;
    }

    int getClientEpFd() {
        return client_ep_fd;
    }

    void createHandshakePacket();

    uint8_t newseqId() {
        return client_seq_id++;
    }

    void setSeqId(uint8_t newId) {
        client_seq_id = newId;
    }

    void writeToClient();



    MySQLPacket *readClientPacket();


    void setHandler(void (*handler_)()) {
        handler = handler_;
    }

    void invokeHandler() {
        if (handler) {
            handler();
        }
    }
};


#endif //CONNECTION_H
