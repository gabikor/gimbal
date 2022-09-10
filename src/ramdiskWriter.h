#include <filesystem>
#include <fstream>
#include <string>

class RamdiskWriter {

public:
    RamdiskWriter(std::string filename="measurements.json", std::string path="/mnt/tmp/");
    ~RamdiskWriter();
    void writeRamFile();

private:
    std::ofstream *ramdiskFile;

};