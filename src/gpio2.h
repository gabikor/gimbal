#ifndef GPIO2_HANDLER
#define GPIO2_HANDLER

#include <vector>
#include <string>
#include <linux/gpio.h>

namespace nanoPi_gpio2 {

    class GpioHandler {

    public:

        GpioHandler(std::vector<int16_t>&, std::string dev_name = "/dev/gpiochip0");

        ~GpioHandler();

        int16_t writeGpioPins(std::vector<bool>& values);
        void getPins(void);

    private:
        std::string dev_name;
        std::vector<int16_t> pinsy;
        std::vector<bool> values;
        std::vector<int> fdy;
        struct gpiohandle_request request;
    };
} //namespace nanoPi_gpio


#endif