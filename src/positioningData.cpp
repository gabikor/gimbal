#include "positioningData.h"
#include <string>

using namespace std;

void PositioningData::fillGyroscopeData(array<int16_t,METER_AXES_NO>& gyros)
{
    this->gyros = gyros;
}

void PositioningData::fillAccelerometerData(array<int16_t,METER_AXES_NO>& accels)
{
    this->accels = accels;
}

int16_t* PositioningData::getGyroDataPtr()
{
    return((int16_t*)(this->gyros.data()));
}
int16_t* PositioningData::getAccelDataPtr()
{
    return((int16_t*)(this->accels.data()));
}

float PositioningData::getGyroScaled(int16_t axis_no)
{
    return((float) this->gyros[axis_no] / this->k);
}

float PositioningData::getAccelScaled(int16_t axis_no)
{
    return((float) this->accels[axis_no] / this->k);
}