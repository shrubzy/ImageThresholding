#ifndef STD_HDR_H
#define STD_HDR_H

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <bit>
#include <memory>
#include <algorithm>

namespace ESSDAA001 {
    struct PGMImage {
        int width, height, maxValue;
        std::vector<unsigned char> pixelData;
    };

    void findcomp(int argc, char *argv[]);
}

#endif