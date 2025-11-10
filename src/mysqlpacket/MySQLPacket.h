//
// Created by shenfeng on 11/3/25.
//

#ifndef PACKET_H
#define PACKET_H
#include <cstdint>
#include <vector>


class MySQLPacket {
protected:
    uint8_t id;
    int fd;
    std::vector<uint8_t> *buf = nullptr;
    int write_index;

public:
    MySQLPacket(int fd_, uint8_t id_, std::vector<uint8_t> *buf_) {
        fd = fd_;
        id = id_;
        buf = buf_;
    }
    MySQLPacket(int fd_, uint8_t id_) {
        fd = fd_;
        id = id_;
        buf = new std::vector<uint8_t>;
        buf->insert(buf->end(), 4, 0x00);
    }

    bool isLastPacket() {
        return true;
    }

    ~MySQLPacket() {
        delete buf;
    }

    void update_payload();

    int write();

    int getSize() {
        return buf->size();
    }
};


#endif //PACKET_H
