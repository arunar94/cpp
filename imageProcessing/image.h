#pragma once
#include <string>
#include <vector>

class Image {
public:
    int width, height, channels;
    std::vector<unsigned char> data;

    Image(const std::string &filename);
    void save(const std::string &filename);

    void to_grayscale();
    void blur(int kernel_size = 3);
    void edge_detect();
};
