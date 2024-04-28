#include "PGMImageProcessor.h"

namespace ESSDAA001 {

    PGMImageProcessor::PGMImageProcessor() : idNum(0) {};

    PGMImageProcessor::~PGMImageProcessor() {};

    PGMImageProcessor::PGMImageProcessor(PGMImageProcessor &other): idNum(other.idNum), img(other.img), minSize(other.minSize), maxSize(other.maxSize) {
        for (int i = 0; i < other.components.size(); i++) {
            *components[i] = *other.components[i]; 
        }
    };

    PGMImageProcessor::PGMImageProcessor(PGMImageProcessor &&other): idNum(other.idNum), img(std::move(other.img)), minSize(other.minSize), maxSize(other.maxSize) {
        components = std::move(other.components);
    };

    PGMImageProcessor& PGMImageProcessor::operator=(PGMImageProcessor &other) {
        idNum = other.idNum;
        img = other.img;
        minSize = other.minSize;
        maxSize = other.maxSize;

        for (int i = 0; i < other.components.size(); i++) {
            *components[i] = *other.components[i]; 
        }

        return *this;
    };

    PGMImageProcessor& PGMImageProcessor::operator=(PGMImageProcessor &&other) {
        idNum = other.idNum;
        minSize = other.minSize;
        maxSize = other.maxSize;

        img = std::move(other.img);
        components = std::move(other.components);

        return *this;
    };


    // Reads in a pgm image
    void PGMImageProcessor::readPGM(std::string &filename) {
        int width, height, maxValue;

        std::ifstream file(filename, std::ios::binary);
        std::string line;

        if (!file.is_open()) {
            throw std::ios_base::failure("Error: " + filename + " not found");
        }

        while (std::getline(file, line)) {
            if (!(line == "P5" || line[0] == '#')) {
                std::istringstream lineStream(line);
                lineStream >> width >> height;
                break;
            }
        }

        file >> maxValue;

        std::vector<unsigned char> pixelData(width * height);
        file.read(std::bit_cast<char*>(pixelData.data()), width * height);

        file.close();

        img.width = width;
        img.height = height;
        img.maxValue = maxValue;
        img.pixelData = pixelData;
    }

    // Extracts the components from an image
    int PGMImageProcessor::extractComponents(unsigned char threshold, int minValidSize) {
        
        // For every pixel
        for (int i = 0; i < img.width * img.height; i++) {
            if (img.pixelData[i] < threshold) {continue;}   // Continue if pixel below threshold

            // Create a pointer to a new component
            std::unique_ptr<ConnectedComponent> componentPtr = std::make_unique<ConnectedComponent>(idNum);
            idNum++;

            std::pair<int, int> pixelCoords;  // x, y coordinates of the pixel
            std::pair<unsigned char, std::pair<int, int>> pixelMap;  // value mapped to the pixel coordinates
            std::vector<std::pair<unsigned char, std::pair<int, int>>> pixelsToCheck;  // Unchecked neighbouring pixels

            int position = i;
            pixelCoords.first = position % img.width;  // x value
            pixelCoords.second = position / img.width;  // y value

            unsigned char value = img.pixelData[i];
            pixelMap.first = value;
            pixelMap.second = pixelCoords;
            
            pixelsToCheck.push_back(pixelMap);  // Push map into vector


            for (int j = 0; j < pixelsToCheck.size(); j++) {
                if (int(pixelsToCheck[j].first) < threshold) {continue;}  // Continue if pixel below threshold


                position = pixelsToCheck[j].second.first + pixelsToCheck[j].second.second * img.width;  // Calculate pixel's 1D position


                if (pixelsToCheck[j].first != img.pixelData[position]) {continue;}  // Continue if this pixel was already checked

                (*componentPtr).addPixel(pixelsToCheck[j].second);  // Add pixel to component

                img.pixelData[position] = 0;  // Set value to 0 to prevent infinite loop

                // Check neighbouring pixels
                // Left pixel
                if (position > 0) {
                    if (img.pixelData[position - 1] >= threshold) {
                        pixelCoords.first = (position - 1) % img.width;
                        pixelCoords.second = (position - 1) / img.width;

                        pixelMap.first = img.pixelData[position - 1];
                        pixelMap.second = pixelCoords;

                        pixelsToCheck.push_back(pixelMap);
                    }
                }

                // Right Pixel
                if (position < img.width * img.height + 1) {
                    if (img.pixelData[position + 1] >= threshold) {
                        pixelCoords.first = (position + 1) % img.width;
                        pixelCoords.second = (position + 1) / img.width;

                        pixelMap.first = img.pixelData[position + 1];
                        pixelMap.second = pixelCoords;

                        pixelsToCheck.push_back(pixelMap);
                    }
                }

                // Up Pixel
                if (position > img.width) {
                    if (img.pixelData[position - img.width] >= threshold) {
                        pixelCoords.first = (position - img.width) % img.width;
                        pixelCoords.second = (position - img.width) / img.width;

                        pixelMap.first = img.pixelData[position - img.width];
                        pixelMap.second = pixelCoords;

                        pixelsToCheck.push_back(pixelMap);
                    }
                }

                // Down pixel
                if (img.pixelData[position + img.width] >= threshold) {
                    if (position < img.width * img.height - img.width - 1) {
                        pixelCoords.first = (position + img.width) % img.width;
                        pixelCoords.second = (position + img.width) / img.width;

                        pixelMap.first = img.pixelData[position + img.width];
                        pixelMap.second = pixelCoords;

                        pixelsToCheck.push_back(pixelMap);
                    }
                }

            }
            components.push_back(std::move(componentPtr));  // Add pointer to components vector
            pixelsToCheck.resize(0);  // Clear pixelsToCheck
        }

        filterComponentsBySize(minSize, maxSize);
        return getComponentCount();
    }

    // Filters out components that are outside of the size bounds
    int PGMImageProcessor::filterComponentsBySize(int minSize, int maxSize) {
        for (auto it = components.begin(); it != components.end(); ) {
            if ((*it)->getSize() < minSize || (*it)->getSize() > maxSize) {  // If components is not the correct size
                it = components.erase(it);
            } else {
                it++;  // Move iterator if nothing was erased
            }
        }

        return 0;
    }

    // Writes the components to pgm file
    bool PGMImageProcessor::writeComponents(const std::string & outFileName) {
        int position = -1;
        std::vector<int> pixelPositions;

        // Calculate the pixel positions and push them to a vector
        for (int i = 0; i < components.size(); i++) {
            for (int j = 0; j < (*components[i]).getSize(); j++) {
                position = (*components[i]).getPixels()[j].first + (*components[i]).getPixels()[j].second * img.width;
                pixelPositions.push_back(position);
            }
        }

        std::vector<unsigned char> dataToWrite(img.width * img.height, 0);  // Initialise dataToWrite with all values set to 0
        std::ofstream file(outFileName, std::ios::binary);
        int numPixels = 0;

        file << "P5" << "\n";
        file << img.width << " " << img.height << "\n";
        file << 255 << "\n";

        // Set pixel positions in the components to 255
        for (int i = 0; i < pixelPositions.size(); i++) {
            dataToWrite[pixelPositions[i]] = 255;
        }

        // Cast data to char*
        const char* dataAsChar = std::bit_cast<const char*>(dataToWrite.data());
        file.write(dataAsChar, img.width * img.height);

        file.close();

        return 0;
    }

    // Returns the number of components
    int PGMImageProcessor::getComponentCount() const {
        return components.size();
    }

    // Returns the size of the largest component
    int PGMImageProcessor::getLargestSize() const {
        int max = -1;
        for (int i = 0; i < components.size(); i++) {
            if ((*components[i]).getSize() > max) {max = (*components[i]).getSize();}
        }

        return max;
    }

    // Returns the size of the smallest component
    int PGMImageProcessor::getSmallestSize() const {
        int min = img.width * img.height + 1;
        for (int i = 0; i < components.size(); i++) {
            if ((*components[i]).getSize() < min) {min = (*components[i]).getSize();}
        }

        return min;
    }
    // Print a summary of the component data
    void PGMImageProcessor::printComponentData(const ConnectedComponent &theComponent) const {
        std::cout << theComponent.getID() << ", " << theComponent.getSize() << "\n";
    }
    
}