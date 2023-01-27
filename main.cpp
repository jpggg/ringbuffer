#include "Ringbuffer.h"
#include <thread>
#include <chrono>

// ALLOWS THE USER TO ENTER KEYBOARD INPUT TO THE BUFFER
template <typename T>
void human_start_writing(Ringbuffer<T>& buffy, std::mutex& locker){
    T input;
    while (std::cin >> input){
        buffy.write_to_buffer(input, locker);
    }
}

// A BOT WHICH CONTINUOUSLY INPUTS TO THE BUFFER
template <typename T>
void bot_start_writing(Ringbuffer<T>& buffy, std::mutex& locker){
    char sample[20] = "abcdefghijklmnopqrs";
    for (int i {0};;){
        locker.lock();
        std::cout << "BOT INPUT = " << sample[i%20] << std::endl;
        locker.unlock();
        buffy.write_to_buffer(sample[(i++)%20], locker);
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

// A BOT WHICH OUTPUTS AND PRINTS FROM THE BUFFER EVERY 2 SECONDS
template <typename T>
void start_reading(Ringbuffer<T>& buffy, std::mutex& locker){
    while (true){
        T reading {buffy.read_from_buffer(locker)};
        if (reading != NULL){
            locker.lock();
            std::cout << "OUTPUT = " << reading << std::endl;
            locker.unlock();
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}


int main() {

    // CONSTRUCTING BUFFER OF CHARS WITH 9 MEMORY PLACES
    Ringbuffer<char> buffy(9);

    // CONSTRUCTING MUTEX FOR SYNCHRONIZING REASONS
    std::mutex locker;

    // STARTING THREADS THAT RUN THE DIFFERENT PROCESSES
    std::thread human_write_thread(human_start_writing<char>, std::ref(buffy), std::ref(locker));
    std::thread bot_write_thread(bot_start_writing<char>, std::ref(buffy), std::ref(locker));
    std::thread read_thread(start_reading<char>, std::ref(buffy), std::ref(locker));

    // JOINING THE THREADS
    human_write_thread.join();
    bot_write_thread.join();
    read_thread.join();

    return 0;
}
