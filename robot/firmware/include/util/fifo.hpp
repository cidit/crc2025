#pragma once
#include <Arduino.h>

/**
 * Fifo (first in, first out) arena based queue. 
 */
template<size_t length, typename T>
class Fifo {
public:
    T[length] arena;
    size_t ptr;

    Fifo(): ptr(0) {}

    void push(T newobj) {
        arena[ptr] = newobj;
        ptr = ptr+1 % length; 
    }
};