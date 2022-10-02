#ifndef MOTOR_CONTROL
#define MOTOR_CONTROL

#include <cstdint>
#include <vector>
#include <chrono>
#include "pwmGenerator.h"

namespace moto {

    #define OUTPUT_STATES_NO 6
    #define MOTOR_PINS_NO 3

    enum output_pin_mode {
        OUTPUT_MODE_LOW,
        OUTPUT_MODE_HIGH,
        OUTPUT_MODE_PWM,
        OUTPUT_MODE_MAX
    };

    typedef void (*outputChangeCallback_t)(std::vector<bool>&);

    class MotorControl {

    public:

        MotorControl(outputChangeCallback_t);
        ~MotorControl();
        void run(int speed);
        void runWithPwm(int speed);

    private:
        const bool output_states[OUTPUT_STATES_NO] = {true,true,true,false,false,false};
        const int max_freq = 2000;
        unsigned int state_ptr;
        outputChangeCallback_t outputChangeCB;

        PwmGenerator * pwm_generator;

        const int max_speed = 150;
        bool direction;
        bool pwm_pin_is_ahead;   /* if not ahead, it is beforehead high level pin */
        unsigned int high_pin_idx;

        void _pwmStateChangeCb(bool state);
        void _sendOutputStates(bool pwm_state);
        void _clearOutputStates();
        void _rotate_pins();
        int _calc_pwm_duty(std::chrono::microseconds current_time);
        static std::chrono::microseconds _getTimeInUs();

        std::chrono::microseconds last_change_time;
        std::chrono::microseconds period_us;
    };
}

#endif