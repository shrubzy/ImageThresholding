#include "PGMImageProcessor.cpp"
#include "ConnectedComponent.cpp"

namespace ESSDAA001 {

    void findcomp(int argc, char *argv[]) {
        // Initialise variables
        int minSize = -1;
        int maxSize = -1;
        int threshold = -1;
        bool shouldPrint = false;
        bool shouldWrite = false;
        std::string inputFileName = "";
        std::string outputFileName = "";

        for (int i = 0; i < argc; i++) {
            // Skip argv[0]
            if (!i) {continue;}

            // Parameter '-s'
            if(!(std::strcmp(argv[i], "-s"))) {
                try {
                    if (i + 1 >= argc || i + 2 >= argc) {
                        throw std::out_of_range("Invalid argument. Parameter '-s' must be specified as follows '-s <min> <max>', where min and max are of type int.");
                    }

                    minSize = std::stoi(argv[i+1]);
                    maxSize = std::stoi(argv[i+2]);

                    if (minSize <= 0) {
                        throw std::invalid_argument("Invalid argument. The minimum value must be at least one pixel.");
                    }

                    if (maxSize < minSize) {
                        throw std::invalid_argument("The maximum value must be greater than or equal to the minimum value.");
                    }

                } catch (std::invalid_argument &e) {
                    if (!std::strcmp(e.what(), "stoi")) {
                        throw std::invalid_argument("Invalid argument. Parameter '-s' must be specified as follows '-s <min> <max>', where min and max are of type int.");
                    } else {
                        throw std::invalid_argument(e.what());
                    }

                } catch (std::out_of_range &e) {
                     if (!std::strcmp(e.what(), "stoi")) {
                        throw std::out_of_range("Argument out of range. Parameter '-s' is out of the range of an int.");
                    } else {
                        throw std::out_of_range(e.what());
                    }

                }
            }

            // Parameter '-t'
            if(!(std::strcmp(argv[i], "-t"))) {
                try {
                    if (i + 1 >= argc) {
                        throw std::out_of_range("Invalid argument. Parameter '-t' must be specified as follows '-t <threshold>', where threshold is of type int.");
                    }

                    threshold = std::stoi(argv[i+1]);

                    if (threshold < 0 || threshold > 255) {
                        throw std::invalid_argument("Invalid argument. The threshold value should be between 0 and 255.");
                    }
                } catch (std::invalid_argument &e) {
                    if (!std::strcmp(e.what(), "stoi")) {
                        throw std::invalid_argument("Invalid argument. Parameter '-t' must be of type int.");
                    } else {
                        throw std::invalid_argument(e.what());
                    }

                } catch (std::out_of_range &e) {
                    if (!std::strcmp(e.what(), "stoi")) {
                        throw std::out_of_range("Argument out of range. Parameter '-t' is out of the range of an int.");
                    } else {
                        throw std::out_of_range(e.what());
                    }
                }
            }

            // Parameter '-p'
            if(!(std::strcmp(argv[i], "-p"))) {
                shouldPrint = true;
            }

            // Parameter '-w'
            if(!(std::strcmp(argv[i], "-w"))) {
                shouldWrite = true;

                try {
                    if (i + 1 >= argc) {
                        throw std::out_of_range("Invalid argument. Parameter '-w' must be specified as follows '-w <outPGMFile>', where outPGMFile is of type string.");
                    }

                    outputFileName = argv[i+1];
                } catch (std::out_of_range &e) {
                    throw std::out_of_range(e.what());
                }
            }

            // Input file
            if (
                i == (argc - 1) &&
                std::strcmp(argv[i-1], "-s") &&
                std::strcmp(argv[i-1], "-t") &&
                std::strcmp(argv[i-1], "-w")
            ) {
                inputFileName = argv[i];
            }
        }

        PGMImageProcessor imgProcessor;

        imgProcessor.readPGM(inputFileName);

        // Default min and max sizes if '-s' not specified
        if (minSize == -1) {
            minSize = 3;
            maxSize = imgProcessor.img.width * imgProcessor.img.height;
        }

        // Default threshold value if '-t' not specified
        if (threshold == -1) {
            threshold = 128;
        }

        // Set image processor avlues
        imgProcessor.minSize = minSize;
        imgProcessor.maxSize = maxSize;

        imgProcessor.extractComponents(threshold, minSize);

        if (shouldPrint) {
            std::cout << "Component Data:\n";

            // Print the id and size of each component
            for (int i = 0; i < imgProcessor.components.size(); i++) {
                imgProcessor.printComponentData((*imgProcessor.components[i]));
            }

            std::cout << "\nThe total number of components is: " << imgProcessor.getComponentCount() << "\n";
            std::cout << "The smallest component has " << imgProcessor.getSmallestSize() << " pixels\n";
            std::cout << "The largest component has " << imgProcessor.getLargestSize() << " pixels\n";
        }

        if (shouldWrite) {
            imgProcessor.writeComponents(outputFileName);
        }
    }
}