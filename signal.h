#ifndef SIGNAL_H
#define SIGNAL_H

#include <csignal>
#include <iostream>

extern int ctrl;

void signal_handler(int signal) {
    std::cout << "Interrupt signal "<< signal <<" received." << std::endl;
    ctrl = 0;
    exit(signal);
}

#endif /*SIGNAL_H*/