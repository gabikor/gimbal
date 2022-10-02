#include <iostream>
#include "signal.h"
#include "I2cPort.h"
#include "MPU6050.h"
#include "ramdiskWriter.h"
#include "positioningData.h"
#include "pwmGenerator.h"
#include "motorControl.h"
#include "gpio2.h"
#include <vector>
#include <thread>
#include <sys/timerfd.h>

using namespace cacaosd_i2cport;
using namespace cacaosd_mpu6050;

int ctrl;
int g_speed = 30;

std::vector<int16_t> motor1_pins {64,65,66};
std::vector<int16_t> motor2_pins {0,2,3};
std::vector<int16_t> motor3_pins {199,200,201};

nanoPi_gpio2::GpioHandler gpio_motor1(motor1_pins);
nanoPi_gpio2::GpioHandler gpio_motor2(motor2_pins);
nanoPi_gpio2::GpioHandler gpio_motor3(motor3_pins);

// void a(bool state){
//     std::vector<bool> pin_values {state,1,0};
//     gpio_motor1.writeGpioPins(pin_values);
// }

void ab1(std::vector<bool>& states){
    gpio_motor1.writeGpioPins(states);
}

void ab2(std::vector<bool>& states){
    gpio_motor2.writeGpioPins(states);
}

void ab3(std::vector<bool>& states){
    gpio_motor3.writeGpioPins(states);
}

void pwm_thread()
{
    moto::MotorControl motorCtrl1(ab1);
    //moto::MotorControl motorCtrl2(ab2);
    //moto::MotorControl motorCtrl3(ab3);
    
    while (ctrl) {  
        motorCtrl1.runWithPwm(100);
        // motorCtrl2.runWithPwm(g_speed);
        // motorCtrl3.runWithPwm(-50);
    }
    delete &gpio_motor1;
    // delete &gpio_motor2;
    // delete &gpio_motor3;
}

void pwm_thread2()
{
    moto::MotorControl motorCtrl2(ab2);
    
    while (ctrl) {  
        motorCtrl2.runWithPwm(g_speed);
    }
    delete &gpio_motor2;
}

void pwm_thread3()
{
    moto::MotorControl motorCtrl3(ab3);
    
    while (ctrl) {  
        motorCtrl3.runWithPwm(-50);
    }
    delete &gpio_motor3;
}

int main() {

    ctrl = 1;
    signal(SIGINT, signal_handler);

    std::thread pwmThread(pwm_thread);
    pwmThread.detach();

        std::thread pwmThread2(pwm_thread2);
    pwmThread2.detach();

        std::thread pwmThread3(pwm_thread3);
    pwmThread3.detach();

    char a;
    while (ctrl) {
        std::cin >> a;
        if('[' == a ) {
            g_speed +=3;
            std::cout << "speed is " << g_speed << endl;
        }
        if(']' == a ) {
            g_speed -=3;
            std::cout << "speed is " << g_speed << endl;
        }

        usleep(10000);
    }

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