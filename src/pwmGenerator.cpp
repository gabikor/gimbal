#include <chrono>
#include <iostream>
#include <unistd.h>
#include "pwmGenerator.h"

using namespace std;
using namespace chrono;

PwmGenerator::PwmGenerator(outputChangeCallback_t cb, int16_t freq) 
: outputChangeCallback(cb), freq(freq)
{
    this->state = true;
    this->maxFreqPwmCounter = 0;
    this->period_us = (microseconds)((microseconds)(1000000)/((microseconds)this->freq));
    this->_stateChange();
    cout << "Period " << this->period_us.count() << endl;
}

PwmGenerator::PwmGenerator()
{
    this->state = true;
}

PwmGenerator::~PwmGenerator()
{
}

void PwmGenerator::_stateChange()
{
    time_point< system_clock > now = system_clock::now();
    auto duration = now.time_since_epoch();
    this->state = !this->state;
    this->last_change_time = duration_cast< microseconds >( duration );
}

microseconds PwmGenerator::_getTimeInUs()
{
    time_point< system_clock > now = system_clock::now();
    return(duration_cast< microseconds >( now.time_since_epoch() ));
}


void PwmGenerator::run(int16_t duty)
{
    microseconds level_duration;
    microseconds current_time = _getTimeInUs();

    if(this->duty != duty) {
        this->duty = duty;
        this->high_time = (microseconds)(( ((microseconds)duty).count() * this->period_us.count() ) / 1000);
        this->low_time = this->period_us - this->high_time;
    }

    level_duration = this->state ? this->high_time : this->low_time;

    if((this->last_change_time.count() + level_duration.count()) <= current_time.count()) {
        this->last_change_time = current_time;
        this->state = !this->state;
        this->outputChangeCallback(this->state);
    }
}

bool PwmGenerator::runMaxFreq(int duty)
{
    if(duty > 100) duty = 100;
    if(duty < 0) duty = 0;

    bool new_state = (this->maxFreqPwmCounter++ <= duty);
    if(this->maxFreqPwmCounter == 100) {
        this->maxFreqPwmCounter = 0;
    }

    return(new_state);
}