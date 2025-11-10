//
// Created by shenfeng on 11/3/25.
//

#include "MySQLPacket.h"

#include <cstdio>
#include <iostream>
#include <unistd.h>

void MySQLPacket::update_payload() {
    std::size_t payload_size = buf->size() - 4;
    (*buf)[0] = (payload_size) & 0xFF;
    (*buf)[1] = (payload_size >> 8) & 0xFF;
    (*buf)[2] = (payload_size >> 16) & 0xFF;
    (*buf)[3] = id;
}

int MySQLPacket::write() {
    if (!fd) {
        return -1;
    }
    // std::size_t size;
    update_payload();

    ssize_t n = ::write(fd, buf->data() + write_index, buf -> size() - write_index);
    std::cout << "need: " << buf -> size() << " bytes written\n";
    if (n == -1) {
        // 出错，查看 errno
        perror("write failed");
    } else {
        write_index += n;
    }
    return write_index;
}
