#include <iostream>
#include <fstream>
#include "ramdiskWriter.h"

using namespace std;

RamdiskWriter::RamdiskWriter(string filename, string path)
{
    cout << path+filename<<endl;
    RamdiskWriter::ramdiskFile = new ofstream;
    RamdiskWriter::ramdiskFile->open(path+filename);
    RamdiskWriter::ramdiskFile->close();   
}

RamdiskWriter::~RamdiskWriter()
{
    RamdiskWriter::ramdiskFile->close();
}

void RamdiskWriter::writeRamFile()
{
    
}