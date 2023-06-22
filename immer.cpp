//
// Created by mgy on 15.06.23.
//

#include <csignal>
#include <iostream>
#include <vector>
#include <immer/vector.hpp>
#include <mutex>
#include <thread>

#define WORKLOAD 5
#define RUNTIME 2

bool done = false;
long num_read = 0;
long num_write = 0;

immer::vector<long> global_list;
std::mutex mutex;
std::vector<long> read_elements;

void read_some(immer::vector<long> list) {
    // Read a random element
    if (!list.empty()) {
        auto index = (size_t) rand() % list.size();
        auto value = list[index];
        read_elements.push_back(value);
    }

    // Emulates some workload...
    usleep(WORKLOAD);
}

immer::vector<long> write_some(immer::vector<long> list) {
    // Emulates some workload...
    usleep(WORKLOAD);
    return list.push_back(num_write);
}

void read_loop() {
    for (; !done; ++num_read) {
        mutex.lock();
        auto list = global_list;
        mutex.unlock();

        // No need to lock during reading!
        read_some(list);
    }
}

void write_loop() {
    for (; !done; ++num_write) {
        // No need to lock during writing!
        auto list = write_some(global_list);

        mutex.lock();
        global_list = list;
        mutex.unlock();
    }
}

int main() {
    // Start both loops in a separate thread
    std::thread read_thread(read_loop);
    std::thread write_thread(write_loop);

    // Let them run for some time...
    sleep(RUNTIME);
    done = true;

    // Join the threads and report the findings!
    read_thread.join();
    write_thread.join();

    std::cout << "num_read: " << num_read << std::endl;
    std::cout << "num_write: " << num_write << std::endl;
    std::cout << "global_list.size(): " << global_list.size() << std::endl;

    return 0;
}