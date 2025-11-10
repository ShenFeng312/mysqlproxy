//
// Created by shenfeng on 11/3/25.
//

#pragma once
#include <iomanip>
#include <random>
#include <netinet/in.h>

#include "MySQLPacket.h"


class MySQLHandshakePacket : public MySQLPacket {
    int connection_id;

public:
    MySQLHandshakePacket(const int fd_, uint8_t id_, const int connectionId_): MySQLPacket(fd_, id_) {
        connection_id = connectionId_;
        // 协议版本
        buf->push_back(0x0A); // protocol version 10

        // server version 字符串 + \0
        const std::string server_version = "8.0.33";

        buf->insert(buf->end(), server_version.begin(), server_version.end());
        buf->push_back('\0');


        // connection id (4 bytes, little-endian)
        push_uint32_le(connection_id);

        // auth_plugin_data_part1 (8 bytes random)
        std::vector<uint8_t> auth_plugin_data_part1 = random_bytes(8);
        buf->insert(buf->end(), auth_plugin_data_part1.begin(), auth_plugin_data_part1.end());

        buf->push_back('\0'); // filler byte

        // capability flags lower 2 bytes (16-bit, little-endian)
        uint16_t capability_flags_lower = 0xB74F; // 举例
        push_uint16_le(capability_flags_lower);

        // character set
        buf->push_back(0x2D); // utf8_general_ci

        // status flags
        uint16_t status_flags = 0x0002; // SERVER_STATUS_AUTOCOMMIT
        push_uint16_le(status_flags);

        // capability flags upper 2 bytes
        uint16_t capability_flags_upper = 0x000F; // 可根据需求设置
        push_uint16_le(capability_flags_upper);

        // auth_plugin_data length (if CLIENT_PLUGIN_AUTH)
        buf->push_back(21); // 8 + 13

        // reserved 10 bytes
        buf->insert(buf->end(), 10, 0x00);

        // auth_plugin_data_part2 (12 bytes + \0)
        std::vector<uint8_t> auth_plugin_data_part2 = random_bytes(12);
        buf->insert(buf->end(), auth_plugin_data_part2.begin(), auth_plugin_data_part2.end());
        buf->push_back('\0');
        // auth plugin name (optional)
        const std::string auth_plugin_name = "caching_sha2_password";
        buf->insert(buf->end(), auth_plugin_name.begin(), auth_plugin_name.end());
        buf->push_back('\0');
    }

private:
    void push_uint16_le(const uint16_t val) {
        buf->push_back(val & 0xFF);
        buf->push_back((val >> 8) & 0xFF);
    }

    void push_uint32_le(const uint32_t val) {
        buf->push_back(val & 0xFF);
        buf->push_back((val >> 8) & 0xFF);
        buf->push_back((val >> 16) & 0xFF);
        buf->push_back((val >> 24) & 0xFF);
    }

    static std::vector<uint8_t> random_bytes(const size_t n) {
        thread_local std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, 255);

        std::vector<uint8_t> bytes(n);
        for (size_t i = 0; i < n; ++i) {
            bytes[i] = static_cast<uint8_t>(dist(gen));
        }
        return bytes;
    }
};
