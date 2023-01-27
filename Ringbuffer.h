#ifndef RINGBUFFER_RINGBUFFER_H
#define RINGBUFFER_RINGBUFFER_H
#include <mutex>
#include <iostream>
#include <thread>

// RINGBUFFER WHICH TAKES ONE ARGUMENT "int buffer_length" to ALLOCATE "int buffer_length" amount of memory places.
// THE RINGBUFFER HAS THE FOLLOWING PROPERTIES:
// 1. IT WILL NOT READ FROM EMPTY MEMORY PLACES
// 2. IT WILL NOT WRITE TO NON-EMPTY MEMORY PLACES
// 3. STORES ANY OBJECT DATATYPE (PROBABLY)
// By Jonathan Bergius 27.01.23
template <typename T>
class Ringbuffer{
private:

    // PROPERTIES
    T* m_buffer;
    int m_buffer_length;
    int m_number_of_elements{0};

    int m_write_cursor {0};
    int m_read_cursor {0};

public:

    // CONSTRUCTOR
    Ringbuffer(int buffer_length){
        m_buffer_length = buffer_length;
        m_buffer = new T[buffer_length];
        for (int i {0}; i < buffer_length; ++i){
            m_buffer[i] = '\0';
        }
    }

    // DESTRUCTOR
    ~Ringbuffer(){
        delete[] m_buffer;
        m_buffer = nullptr;
    }

    // MAIN FUNCTIONS:
    // WRITE TO BUFFER
    bool write_to_buffer(T& keyboard_input, std::mutex& locker){

        // If buffer[cursor] is available --> input into buffer[cursor]
        // Using mutex lock() to ensure that only one thread can access a memory location at a time
        locker.lock();
        if (m_buffer[m_write_cursor % m_buffer_length] == NULL){
            m_buffer[m_write_cursor++ % m_buffer_length] = keyboard_input;
            ++m_number_of_elements;
            locker.unlock();
            return true;
        }
        else{
            locker.unlock();
            return false;
        }
    }

    // READ FROM BUFFER
    T read_from_buffer(std::mutex& locker){
        // If data exists in buffer[cursor]--> return and reset data
        // Using mutex lock() to ensure that only one thread can access a memory location at a time
        locker.lock();
        if (m_buffer[m_read_cursor % m_buffer_length] != NULL){
            char object_to_return = m_buffer[m_read_cursor % m_buffer_length];
            m_buffer[m_read_cursor++ % m_buffer_length] = NULL;
            --m_number_of_elements;
            locker.unlock();
            return object_to_return;
        }
        else{
            locker.unlock();
            return NULL;
        }
    }
};

#endif //RINGBUFFER_RINGBUFFER_H
