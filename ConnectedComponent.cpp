#include "ConnectedComponent.h"

namespace ESSDAA001 {
    ConnectedComponent::ConnectedComponent(int id) : id(id) {}

    ConnectedComponent::~ConnectedComponent() {};

    ConnectedComponent::ConnectedComponent(ConnectedComponent &other): id(other.id), numPixels(other.numPixels)  {
        for (int i = 0; i < other.pixels.size(); i++) {
            std::pair<int, int> pixelCopy(other.pixels[i].first, other.pixels[i].second);
            pixels.push_back(pixelCopy);
        }
    };

    ConnectedComponent::ConnectedComponent(ConnectedComponent &&other): id(other.id), numPixels(other.numPixels) {
        pixels = std::move(other.pixels);
    };

    ConnectedComponent& ConnectedComponent::operator=(ConnectedComponent &other) {
        id = other.id;
        numPixels = other.numPixels;

        for (int i = 0; i < other.pixels.size(); i++) {
            std::pair<int, int> pixelCopy(other.pixels[i].first, other.pixels[i].second);
            pixels.push_back(pixelCopy);
        }

        return *this;
    };

    ConnectedComponent& ConnectedComponent::operator=(ConnectedComponent &&other) {
        id = other.id;
        numPixels = other.numPixels;

        pixels = std::move(other.pixels);

        return *this;
    };

    // Pushes pixel to the pixels vector
    void ConnectedComponent::addPixel(std::pair<int, int> pixelPos) {
        pixels.push_back(pixelPos);
    }

    // Returns the pixels vector
    std::vector<std::pair<int, int>> ConnectedComponent::getPixels() {
        return pixels;
    }

    // Returns the id
    int ConnectedComponent::getID() const {
        return id;
    }

    // Returns the size of the pixels vector
    int ConnectedComponent::getSize() const {
        return pixels.size();
    }

    // Sets the number of pixels
    void ConnectedComponent::setNumPixels(int pixelCount) {
        numPixels = pixelCount;
    }
}