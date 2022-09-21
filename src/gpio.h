#ifndef GPIO_HANDLER
#define GPIO_HANDLER

#include <vector>
#include <string>
#include <linux/gpio.h>

namespace nanoPi_gpio {

    class GpioHandler {

    public:

        GpioHandler(std::vector<int16_t>&, std::string dev_name = "/dev/gpiochip0");

        ~GpioHandler();

        int16_t writeGpioPins(std::vector<bool>& values);

    private:
        int16_t fd;
        std::string dev_name;
        std::vector<int16_t> pins;
        struct gpiohandle_request request;
    };
} //namespace nanoPi_gpio


#endif