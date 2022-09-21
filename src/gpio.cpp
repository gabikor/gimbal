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

#include "gpio.h"

namespace nanoPi_gpio {
    
    using namespace std;

    GpioHandler::GpioHandler(vector<int16_t>& pins, string dev_name)
    {
        struct gpiochip_info info;
        struct gpioline_info line_info;

        this->pins = pins;
        this->dev_name = dev_name;

        int ret;
        this->fd = open(dev_name.c_str(), O_RDONLY);

        if (this->fd < 0) {
            cout << "Unabled to open " << dev_name <<  " : " << strerror(errno) << endl;
            return;
        }

        ret = ioctl(this->fd, GPIO_GET_CHIPINFO_IOCTL, &info);
        if (ret == -1) {
            cout << "Unable to get chip info from ioctl: " << strerror(errno) << endl;
            close(this->fd);
            return;
        }
        close(this->fd);

        if(pins.size() <= sizeof(this->request.lineoffsets)/sizeof(this->request.lineoffsets[0]))
        {
            copy(pins.begin(), pins.end(), this->request.lineoffsets);
            this->request.lines = pins.size();
            this->request.flags = GPIOHANDLE_REQUEST_OUTPUT;
        }

    }

    GpioHandler::~GpioHandler() 
    {
        close(this->fd);
        close(this->request.fd);
    }

    int16_t GpioHandler::writeGpioPins(std::vector<bool>& values) 
    {
        struct gpiohandle_data data;
        int16_t status = 0;
        int ret;

        if(values.size() == this->pins.size())
        {
            this->fd = open(this->dev_name.c_str(), O_RDONLY);
            ret = ioctl(this->fd, GPIO_GET_LINEHANDLE_IOCTL, &(this->request));
            close(this->fd);
            if (ret == -1)
            {
                printf("Unable to set line value using ioctl : %s\r\n", strerror(errno));
            }
            else {
                copy(values.begin(), values.end(), data.values);
                ret = ioctl(this->request.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
                close(this->request.fd);
                if (ret == -1) {
                    printf("Unable to line handle from ioctl : %s\t\n", strerror(errno));
                }
                else {
                    status = 1;
                }
            } 
        } 
        else 
        {
            printf("values size does not match pins size\n");
        }
        return(status);
    }
}






