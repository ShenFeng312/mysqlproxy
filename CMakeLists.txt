cmake_minimum_required(VERSION 3.31)
project(mysqlproxy)

set(CMAKE_CXX_STANDARD 20)

add_executable(mysqlproxy main.cpp
        packet.h
        src/utils/IDGenerator.h
        src/mysqlpacket/MySQLPacket.cpp
        src/mysqlpacket/MySQLPacket.h
        src/mysqlpacket/MySQLHandshakePacket.cpp
        src/mysqlpacket/MySQLHandshakePacket.h
        src/connection/Connection.cpp
        src/connection/Connection.h
        src/server/MainServer.cpp
        src/server/MainServer.h
        src/server/Worker.cpp
        src/server/Worker.h)
