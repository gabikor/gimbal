#ifndef POSITIONING_DATA
#define POSITIONING_DATA

#include <array>
#include <stdint.h>

using namespace std;

#define METER_AXES_NO 3

class PositioningData {
    public:
        PositioningData(float k_coef = 16000.0F) : k(k_coef) {}
        ~PositioningData();
        void fillGyroscopeData(array<int16_t,METER_AXES_NO>& gyros);
        void fillAccelerometerData(array<int16_t,METER_AXES_NO>& accels);

        float getGyroScaled(int16_t axis_no);
        float getAccelScaled(int16_t axis_no);

        int16_t* getGyroDataPtr();
        int16_t* getAccelDataPtr();
    
    private:
        array<int16_t,METER_AXES_NO> gyros;
        array<int16_t,METER_AXES_NO> accels;
        float k;
};

#endif