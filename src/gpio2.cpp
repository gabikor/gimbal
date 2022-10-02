#include <linux/gpio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "gpio2.h"

namespace nanoPi_gpio2 {
    
    using namespace std;

    GpioHandler::GpioHandler(vector<int16_t>& pins, string dev_name)
    {
        int fd = open("/sys/class/gpio/export", O_WRONLY);
        if (fd == -1) {
            perror("Unable to open /sys/class/gpio/export");
            //exit(1);
        }

        for(auto i = pins.begin(); i != pins.end(); ++i) {
            this->pinsy.push_back(*i);
            this->values.push_back(0);
            
            string pin_no = to_string(*i);
            cout << "checking pins " << pin_no << " size " << pin_no.size() << endl;
            if (write(fd, pin_no.c_str(), pin_no.size()) != pin_no.size()) {
                perror("Error writing to /sys/class/gpio/export");
                //exit(1);
            } else {
                cout << "GPIO " + pin_no + " exported" <<endl;
            }
        }


        close(fd);

        for(auto i = pins.begin(); i != pins.end(); ++i) {
            fd = open(("/sys/class/gpio/gpio" + to_string(*i) + "/direction").c_str(), O_WRONLY);
            if (fd == -1) {
                perror("Unable to open /sys/class/gpio/gpio24/direction");
                //exit(1);
            }

            if (write(fd, "out", 3) != 3) {
                perror("Error writing to /sys/class/gpio/gpio24/direction");
                //exit(1);
            }
            else {
                cout << "GPIO " + to_string(*i) + " set as output" <<endl;
            }

            close(fd);
        }

        for(auto i = pins.begin(); i != pins.end(); ++i) {
            fd = open(("/sys/class/gpio/gpio" + to_string(*i) + "/value").c_str(), O_WRONLY);
            if (fd == -1) {
                perror("Unable to open /sys/class/gpio/gpio24/value");
                //exit(1);
            } else {
                cout << "GPIO " + to_string(*i) + " value fd opened" <<endl;
            }
            this->fdy.push_back(fd);
            if (write(fd, "0", 1) != 1) {
                perror("Error writing to /sys/class/gpio/gpio24/value");
                //exit(1);
            }
        }

    }

    GpioHandler::~GpioHandler() 
    {
        for(auto i = this->fdy.begin(); i != this->fdy.end(); ++i) {
            close(*i);
        }

        int fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (fd == -1) {
            perror("Unable to open /sys/class/gpio/unexport");
            exit(1);
        }
        for(auto i = this->pinsy.begin(); i != this->pinsy.end(); ++i) {
            string pin_no = to_string(*i);
            if (write(fd, pin_no.c_str(), pin_no.size()) != pin_no.size()) {
                perror("Error writing to /sys/class/gpio/unexport");
                //exit(1);
            }
        }

        close(fd);
    }

    void GpioHandler::getPins(void)
    {
        for(auto i = this->pinsy.begin(); i != pinsy.end(); ++i) {
            cout << "PIN " <<*i << endl;
        }
    } 

    int16_t GpioHandler::writeGpioPins(std::vector<bool>& values) 
    {
        for(int i = 0; i < this->pinsy.size(); ++i) {
            if(this->values[i] != values[i]) {
                
                if (write(this->fdy[i], (values[i] ? "1" : "0"), 1) != 1) {
                    perror("Error writing to /sys/class/gpio/gpio24/value");
                    //exit(1);
                } else {
                    this->values[i] = values[i];
                }
                //cout << "writing " << (values[i] ? "1 " : "0 ") << "to "<< this->pinsy[i] << endl;
            }
        }
    }
}






