//
// Created by shenfeng on 11/4/25.
//

#include "Connection.h"

#include <sys/epoll.h>

#include "../mysqlpacket/MySQLHandshakePacket.h"

void Connection::createHandshakePacket() {
    response = new MySQLHandshakePacket(client_fd, client_seq_id, connectionId);
}

void Connection::writeToClient() {
    if (response == nullptr) {
        return;
    }
    int write = response->write();
    //写完删除可写监听
    if (write == response->getSize() && response->isLastPacket()) {
        epoll_event ev;
        ev.events = EPOLLIN | EPOLLERR;
        ev.data.ptr = this;
        epoll_ctl(getClientEpFd(), EPOLL_CTL_MOD, getFd(), &ev);
        delete response;
        response = nullptr;
    }
}

MySQLPacket *Connection::decodePacket() {
    uint8_t newseqId = buffer[0];
    setSeqId(newseqId);
    return new MySQLPacket(client_fd, newseqId, &buffer);
}


MySQLPacket *Connection::readClientPacket() {
    if (payloadLength == -1) {
        ssize_t ssize = read(client_fd, &payload[payloadIndex], 3 - payloadIndex);
        if (ssize == 0) {
            closed = true;
            return NULL;
        }
        if (ssize < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // 数据暂时不可读，等待下一次 EPOLLIN
                return nullptr;
            }
            closed = true;
            return nullptr;
        }
        if (3 - payloadIndex == ssize) {
            payloadLength = payload[0] + (payload[1] << 8) + (payload[2] << 16);
        } else {
            payloadIndex += ssize;
            return NULL;
        }
        buffer.resize(payloadLength);
    }

    // 循环读到buffer
    ssize_t ssize = read(client_fd, &buffer[payloadIndex], payloadLength - payloadIndex);
    if (ssize == 0) {
        closed = true;
        return nullptr;
    }
    if (ssize < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return nullptr;
        }
        closed = true;
        return nullptr;
    }

    payloadIndex += ssize;

    // 还没读满 payload，继续等下次 EPOLLIN
    if (payloadIndex < payloadLength) {
        return nullptr;
    }

    // ✅ payload 已完整读取
    payloadIndex = 0;
    int len = payloadLength;
    payloadLength = -1; // 重置，迎接下一包

    // 此处你可以根据 payload 解析返回不同的 MySQLPacket 子类
    MySQLPacket *packet = decodePacket();
    return packet;
}
