#ifndef CONNECTED_COMPONENT_H
#define CONNECTED_COMPONENT_H

#include "stdHdr.h"

namespace ESSDAA001 {
    class ConnectedComponent {
    private:
        int id;
        int numPixels;
        std::vector<std::pair<int, int>> pixels;
    
    public:
        ConnectedComponent(int id);

        ~ConnectedComponent();

        ConnectedComponent(ConnectedComponent &other);

        ConnectedComponent(ConnectedComponent &&other);

        ConnectedComponent &operator=(ConnectedComponent &other);

        ConnectedComponent &operator=(ConnectedComponent &&other);

        void addPixel(std::pair<int, int> pixelPos);

        std::vector<std::pair<int, int>> getPixels();

        int getID() const;
        int getSize() const;

        void setNumPixels(int pixelCount);
    };
}

#endif