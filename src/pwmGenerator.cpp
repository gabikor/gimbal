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
    this->period_us = (microseconds)((microseconds)(1000000)/((microseconds)this->freq));
    this->_stateChange();
    cout << "Period " << this->period_us.count() << endl;
}

PwmGenerator::~PwmGenerator()
{
}

void PwmGenerator::_stateChange()
{
    time_point< system_clock > now = system_clock::now();
    auto duration = now.time_since_epoch();
    this->state = !this->state;
    //this->last_change_time = now.time_since_epoch();
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
        cout<<"H " << this->high_time.count() << " L " << this->low_time.count()  << endl;  
    }

    level_duration = this->state ? this->high_time : this->low_time;

    if((this->last_change_time.count() + level_duration.count()) <= current_time.count()) {
        //cout<<"l " << last_change_time.count() << " d " << level_duration.count() << " c " << current_time.count() << endl;  
        this->last_change_time = current_time;
        this->state = !this->state;
        this->outputChangeCallback(this->state);
        //cout << this->state << endl;
    }
}