//
// Created by jonny on 01.02.2023.
//

#ifndef RINGBUFFER_JONATHAN_H
#define RINGBUFFER_JONATHAN_H
#include <iostream>


class Jonathan {
private:
    int alder {28};
    std::string navn {"jonathan"};
    std::string by {"Drammen"};
public:
    Jonathan() = default;
    void bytte_by(std::string ny_by);


};


#endif //RINGBUFFER_JONATHAN_H
