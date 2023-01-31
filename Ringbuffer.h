#ifndef RINGBUFFER_RINGBUFFER_H
#define RINGBUFFER_RINGBUFFER_H
#include <mutex>
#include <iostream>
#include <thread>

// RINGBUFFER WHICH TAKES ONE ARGUMENT "int buffer_length" to ALLOCATE "int buffer_length" amount of memory places.
// THE RINGBUFFER HAS THE FOLLOWING PROPERTIES:
// 1. THE BUFFER STORES DATA IN A FIRST ON FIRST OUT (FIFO) PRINCIPLE
// 2. THE BUFFER WILL STORE ADDITIONAL INPUT UNTIL IT'S FULL
// 3. THE BUFFER WILL OVERWRITE PREVIOUSLY STORED DATA IF SUCH DATA HAS BEEN OUTPUT
// 4. STORES ANY OBJECT DATATYPE (PROBABLY)
// By Jonathan Bergius 27.01.23
template <typename T>
class Ringbuffer{
private:

    // PROPERTIES
    T* m_buffer;
    int m_buffer_length;

    int m_write_cursor {0};
    int m_read_cursor {0};

public:

    // CONSTRUCTOR
    explicit Ringbuffer(int buffer_length){
        m_buffer_length = buffer_length;
        m_buffer = new T[buffer_length];
        for (int i {0}; i < buffer_length; ++i){
            m_buffer[i] = NULL;
        }
    }

    // DESTRUCTOR
    ~Ringbuffer(){
        delete[] m_buffer;
        m_buffer = nullptr;
    }

    // SUPPORT FUNCTIONS:
    // FULL() RETURNS TRUE IF FULL
    bool full(){
        if ((m_write_cursor > m_read_cursor) && (m_write_cursor % m_buffer_length) == (m_read_cursor % m_buffer_length)){
            return true;
        }
        else return false;
    }

    // EMPTY() RETURNS TRUE IF EMPTY
    bool empty(){
        if (m_read_cursor == m_write_cursor) return true;
        else return false;
    }

    // PRINT (for debugging)
    void print_buffer(){
        for (int i{0}; i < m_buffer_length; ++i){
            std::cout << "buffer[" << i << "]=" << m_buffer[i] << " ";
        }
        std::cout << std::endl;
    }

    // MAIN FUNCTIONS:
    // WRITE TO BUFFER
    void write_to_buffer(T& keyboard_input, std::mutex& locker) {
        if (not full()){
            locker.lock();
            m_buffer[m_write_cursor++ % m_buffer_length] = keyboard_input;
            locker.unlock();
        }
    }

    // READ FROM BUFFER
    T* read_from_buffer(std::mutex& locker) {
        if (not empty()){
            locker.lock();
            T* object_to_return = &m_buffer[m_read_cursor++ % m_buffer_length];
            locker.unlock();
            return object_to_return;
        }
        else return nullptr;
    }







    // ARCHIVE (OLD VERSIONS OF FUNCTIONS)
    /*
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
    T read_from_buffer2(std::mutex& locker){
        // If data exists in buffer[cursor]--> return and reset data
        // Using mutex lock() to ensure that only one thread can access a memory location at a time
        locker.lock();
        if (m_buffer[m_read_cursor % m_buffer_length] != NULL){
            T object_to_return = m_buffer[m_read_cursor % m_buffer_length];
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
     */
};

#endif //RINGBUFFER_RINGBUFFER_H
