//
// Created by shenfeng on 11/3/25.
//

#pragma once

#include <atomic>
#include <climits>


namespace utils {
    class IDGenerator {
        inline static int currentId = 0;
        inline static std::mutex mtx;

    public:
        static int nextId() {
            std::lock_guard lock(mtx);
            if (currentId == INT_MAX) currentId = 0;
            return ++currentId;
        }
    };
};



