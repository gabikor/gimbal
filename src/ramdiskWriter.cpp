#include <iostream>
#include <fstream>
#include "ramdiskWriter.h"
#include "positioningData.h"

using namespace std;

RamdiskWriter::RamdiskWriter(string filename, string path)
{
    cout << path+filename<<endl;
    this->filepath = path+filename;
    this->ramdiskFile = new ofstream;
    this->ramdiskFile->open(this->filepath);
    this->ramdiskFile->close();   
}

RamdiskWriter::~RamdiskWriter()
{
    this->ramdiskFile->close();
}

void RamdiskWriter::writeRamFile(PositioningData& data)
{
    this->ramdiskFile->open(this->filepath);
    *this->ramdiskFile << "{";
    *this->ramdiskFile << "\"Acc\":{\"X\":" + to_string(data.getAccelScaled(0)) 
                                + ",\"Y\":" + to_string(data.getAccelScaled(1))
                                + ",\"Z\":" + to_string(data.getAccelScaled(2))
                                + "},";
    *this->ramdiskFile << "\"Gyro\":{\"X\":" + to_string(data.getGyroScaled(0)) 
                                 + ",\"Y\":" + to_string(data.getGyroScaled(1))
                                 + ",\"Z\":" + to_string(data.getGyroScaled(2))
                                 + "}";
    *this->ramdiskFile << "}"; //data;
    this->ramdiskFile->close();
}