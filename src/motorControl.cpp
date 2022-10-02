#include <iostream>
#include "motorControl.h"

using namespace std;
using namespace chrono;

namespace moto {

    MotorControl::MotorControl(outputChangeCallback_t cb)
    {
        this->state_ptr = 0;
        this->pwm_pin_is_ahead = false;
        this->high_pin_idx = 0;
        this->outputChangeCB = cb;
        this->last_change_time = _getTimeInUs();
        this->pwm_generator = new PwmGenerator();
    }

    MotorControl::~MotorControl() {
        delete this->pwm_generator;
    }

    microseconds MotorControl::_getTimeInUs()
    {
        time_point< system_clock > now = system_clock::now();
        return(duration_cast< microseconds >( now.time_since_epoch() ));
    }

    void MotorControl::run(int speed)
    {
        microseconds current_time = _getTimeInUs();
        this->direction = true;

        if(speed < 0) {
            this->direction = false;
            speed = speed*(-1);
        }
        
        this->period_us = (microseconds)(1000000/speed);

        if((this->last_change_time.count() + this->period_us.count()) <= current_time.count()) {
            this->last_change_time = current_time;
            if(direction) {
                ++this->state_ptr %= OUTPUT_STATES_NO;
            } else {
                this->state_ptr = (this->state_ptr == 0) ? (OUTPUT_STATES_NO-1) : (this->state_ptr-1);
            }

            vector<bool> s {output_states[this->state_ptr],
                        output_states[(this->state_ptr+2)%OUTPUT_STATES_NO],
                        output_states[(this->state_ptr+4)%OUTPUT_STATES_NO]};
            this->outputChangeCB(s);
        }
        
    }

    void MotorControl::runWithPwm(int speed)
    {
        microseconds current_time = _getTimeInUs();
        this->direction = true;

        if(speed < 0) {
            this->direction = false;
            speed = speed*(-1);
        }

        if(speed > this->max_speed)
        {
            speed = this->max_speed;
        }
        
        if(speed == 0) {
            this->_clearOutputStates();
        } 
        else 
        {            
            this->period_us = (microseconds)(1000000/(2*speed));

            if((this->last_change_time.count() + this->period_us.count()) <= current_time.count()) {

                this->_rotate_pins();
                this->last_change_time = current_time;
            }
            _sendOutputStates(pwm_generator->runMaxFreq(_calc_pwm_duty(current_time)));
        }

    }

    void MotorControl::_rotate_pins()
    {
        if(this->direction) {
            if(this->pwm_pin_is_ahead) {
                ++this->high_pin_idx %= MOTOR_PINS_NO;
                this->pwm_pin_is_ahead = false;
            } else {
                this->pwm_pin_is_ahead = true;
            }
        }
        else {
            if(!this->pwm_pin_is_ahead) {
                this->high_pin_idx = (this->high_pin_idx==0) ? MOTOR_PINS_NO-1 : this->high_pin_idx-1;
                this->pwm_pin_is_ahead = true;
            } else {
                this->pwm_pin_is_ahead = false;
            }
        }
    }

    int MotorControl::_calc_pwm_duty(microseconds current_time)
    {
        int duty = ((current_time.count() - this->last_change_time.count())*100)/this->period_us.count();

        /* descending order */
        if (!this->pwm_pin_is_ahead) {
            duty = 100 - duty;
        }
        if(!this->direction) {
            duty = 100 - duty;
        }
        return(duty);
    }

    void MotorControl::_sendOutputStates(bool pwm_state)
    {
        vector<bool> s {0,0,0};

        s[this->high_pin_idx] = 1;
        if(this->pwm_pin_is_ahead) {
            s[(this->high_pin_idx+1)%MOTOR_PINS_NO] = pwm_state;
        } else {
            int idx = (this->high_pin_idx == 0) ? MOTOR_PINS_NO-1 : this->high_pin_idx-1;
            s[idx] = pwm_state;
        }
        this->outputChangeCB(s);
    }

    void MotorControl::_clearOutputStates()
    {
        vector<bool> s {0,0,0};
        this->outputChangeCB(s);
    }

} //namespace