#include "image.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: ./imageProcessing <input.png> <operation> <output.png>\n";
        std::cerr << "Operations: grayscale | blur | edge\n";
        return 1;
    }

    try {
        Image img(argv[1]);

        std::string op = argv[2];
        if (op == "grayscale") img.to_grayscale();
        else if (op == "blur") img.blur();
        else if (op == "edge") img.edge_detect();
        else throw std::runtime_error("Unknown operation: " + op);

        img.save(argv[3]);
        std::cout << "Saved result to " << argv[3] << "\n";
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
