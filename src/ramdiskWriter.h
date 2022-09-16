#ifndef RAMDISK_WRITER
#define RAMDISK_WRITER

#include <filesystem>
#include <fstream>
#include <string>
#include "positioningData.h"

class RamdiskWriter {

public:
    RamdiskWriter(std::string filename="measurements.json", std::string path="/mnt/tmp/");
    ~RamdiskWriter();
    void writeRamFile(PositioningData& data);

private:
    std::ofstream *ramdiskFile;
    std::string filepath;

};

#endif