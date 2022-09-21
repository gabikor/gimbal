#include <iostream>
#include "signal.h"
#include "I2cPort.h"
#include "MPU6050.h"
#include "ramdiskWriter.h"
#include "positioningData.h"
#include "gpio.h"
#include <vector>

using namespace cacaosd_i2cport;
using namespace cacaosd_mpu6050;
using namespace nanoPi_gpio;

int ctrl;

int main() {

    ctrl = 1;
    signal(SIGINT, signal_handler);

    std::vector<int16_t> motor1_pins {64,65,66};
    std::vector<int16_t> motor2_pins {0,2,3};
    std::vector<int16_t> motor3_pins {199,200,201};
    std::vector<bool> pin_values {0,1,0};
    GpioHandler gpio_motor1(motor1_pins);
    GpioHandler gpio_motor2(motor2_pins);
    GpioHandler gpio_motor3(motor3_pins);
    gpio_motor1.writeGpioPins(pin_values);
    gpio_motor2.writeGpioPins(pin_values);
    gpio_motor3.writeGpioPins(pin_values);

    //exit(SIGINT);
    RamdiskWriter *ramdiskWriter = new RamdiskWriter();

    I2cPort *i2c = new I2cPort(0x68, 0);
    i2c->openConnection();

    MPU6050 *mpu6050 = new MPU6050(i2c);
    mpu6050->initialize();

    PositioningData *positioningData = new PositioningData(16000.0);

    while (ctrl) {
        std::cout << "MPU6050" << std::endl;

        mpu6050->getAccelerations(positioningData->getAccelDataPtr());
        std::cout << "Accel X: " << positioningData->getAccelScaled(0) << std::endl;
        std::cout << "Accel Y: " << positioningData->getAccelScaled(1) << std::endl;
        std::cout << "Accel Z: " << positioningData->getAccelScaled(2) << std::endl;

        mpu6050->getAngularVelocities(positioningData->getGyroDataPtr());
        std::cout << "Gyro X: " << positioningData->getGyroScaled(0) << std::endl;
        std::cout << "Gyro Y: " << positioningData->getGyroScaled(1) << std::endl;
        std::cout << "Gyro Z: " << positioningData->getGyroScaled(2) << std::endl;

        std::cout << "----------------------" << std::endl;

        ramdiskWriter->writeRamFile(*positioningData);
        usleep(200000);
    }

    i2c->closeConnection();
    delete i2c, mpu6050, positioningData, ramdiskWriter;

    return 0;
}