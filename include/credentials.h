#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <Arduino.h>

#define NODE_ID 2

struct credentials{
    String appEUI;
    String appKey;
};

//garden arduino
#if NODE_ID == 1
    const credentials cr ={
        "3bf8ecd12035842d",
        "6ae5c0019c43801923e969f98bb8299d"
    };

//tank arduino
#elif NODE_ID == 2
    const credentials cr ={
        "dbba2b39cbbdfcbf",
        "b388de6914bfdc1f6f473deb8f60930c"
    };

#else
    #error "NODE_ID is not available"

#endif
#endif