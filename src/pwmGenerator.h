#ifndef PWM_GENERATOR
#define PWM_GENERATOR

#include <stdint.h>
#include <chrono>

typedef void (*outputChangeCallback_t)(bool);

class PwmGenerator
{
private:
   outputChangeCallback_t outputChangeCallback;
   int16_t duty;
   int16_t freq;
  
   std::chrono::microseconds last_change_time;
   //std::chrono::time_point<std::chrono::high_resolution_clock> last_change_time;
   std::chrono::microseconds low_time;
   std::chrono::microseconds high_time;
   std::chrono::microseconds period_us;
   bool state;
   void _stateChange();
   std::chrono::microseconds _getTimeInUs();
public:
    PwmGenerator(outputChangeCallback_t cb, int16_t freq);
    ~PwmGenerator();
    void run(int16_t duty);
};


#endif