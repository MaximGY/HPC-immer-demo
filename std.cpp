//
// Created by mgy on 15.06.23.
//

#include <csignal>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

#define WORKLOAD 5
#define RUNTIME 2

bool done = false;
long num_read = 0;
long num_write = 0;

std::vector<long> global_list;
std::mutex mutex;
std::vector<long> read_elements;

void read_some(std::vector<long> & list) {
    // Read a random element
    if (!list.empty()) {
        auto index = (size_t) rand() % list.size();
        auto value = list[index];
        read_elements.push_back(value);
    }

    // Emulates some workload...
    usleep(WORKLOAD);
}

void write_some(std::vector<long> & list) {
    list.push_back(num_write);

    // Emulates some workload...
    usleep(WORKLOAD);
}

void read_loop() {
    for (; !done; ++num_read) {
        // Lock has to be held during the entire process...
        mutex.lock();
        read_some(global_list);
        mutex.unlock();
    }
}

void write_loop() {
    for (; !done; ++num_write) {
        // Lock has to be held during the entire process...
        mutex.lock();
        write_some(global_list);
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