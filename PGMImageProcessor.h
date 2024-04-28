#ifndef PGM_IMAGE_PROCESSOR_H
#define PGM_IMAGE_PROCESSOR_H

#include "ConnectedComponent.h"

namespace ESSDAA001 {
    class PGMImageProcessor {
    public:
        PGMImage img;
        int idNum;
        std::vector<std::unique_ptr<ConnectedComponent>> components;

        int minSize, maxSize;

        PGMImageProcessor();

        ~PGMImageProcessor();

        PGMImageProcessor(PGMImageProcessor &other);

        PGMImageProcessor(PGMImageProcessor &&other);

        PGMImageProcessor &operator=(PGMImageProcessor &other);

        PGMImageProcessor &operator=(PGMImageProcessor &&other); 

        void readPGM(std::string &filename);

        int extractComponents(unsigned char threshold, int minValidSize);

        int filterComponentsBySize(int minSize, int maxSize);

        bool writeComponents(const std::string & outFileName);

        int getComponentCount() const;

        int getLargestSize() const;

        int getSmallestSize() const;

        void printComponentData(const ConnectedComponent &theComponent) const;

    };
}

#endif