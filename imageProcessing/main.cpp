#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "image.h"
#include <iostream>
#include <vector>
#include <cmath>

Image::Image(const std::string &filename) {
    data = {};
    unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!img) throw std::runtime_error("Failed to load image: " + filename);
    data.assign(img, img + width * height * channels);
    stbi_image_free(img);
}

void Image::save(const std::string &filename) {
    if (!stbi_write_png(filename.c_str(), width, height, channels, data.data(), width * channels)) {
        throw std::runtime_error("Failed to save image: " + filename);
    }
}

// Convert to grayscale
void Image::to_grayscale() {
    if (channels < 3) return;
    for (int i = 0; i < width * height; i++) {
        int r = data[i * channels + 0];
        int g = data[i * channels + 1];
        int b = data[i * channels + 2];
        unsigned char gray = static_cast<unsigned char>(0.299*r + 0.587*g + 0.114*b);
        data[i * channels + 0] = gray;
        data[i * channels + 1] = gray;
        data[i * channels + 2] = gray;
    }
}

// Simple box blur
void Image::blur(int kernel_size) {
    std::vector<unsigned char> output(data.size());
    int offset = kernel_size / 2;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sum[3] = {0, 0, 0};
            int count = 0;
            for (int ky = -offset; ky <= offset; ky++) {
                for (int kx = -offset; kx <= offset; kx++) {
                    int nx = x + kx, ny = y + ky;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        int idx = (ny * width + nx) * channels;
                        sum[0] += data[idx + 0];
                        sum[1] += data[idx + 1];
                        sum[2] += data[idx + 2];
                        count++;
                    }
                }
            }
            int idx = (y * width + x) * channels;
            output[idx + 0] = sum[0] / count;
            output[idx + 1] = sum[1] / count;
            output[idx + 2] = sum[2] / count;
            if (channels == 4) output[idx + 3] = data[idx + 3]; // alpha
        }
    }
    data = output;
}

// Sobel edge detection
void Image::edge_detect() {
    to_grayscale(); // ensure grayscale first
    std::vector<unsigned char> output(data.size());
    int gx[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
    int gy[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sumX = 0, sumY = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int idx = ((y+ky) * width + (x+kx)) * channels;
                    int gray = data[idx];
                    sumX += gray * gx[ky+1][kx+1];
                    sumY += gray * gy[ky+1][kx+1];
                }
            }
            int mag = std::min(255, static_cast<int>(std::sqrt(sumX*sumX + sumY*sumY)));
            int idx = (y * width + x) * channels;
            output[idx] = output[idx+1] = output[idx+2] = mag;
            if (channels == 4) output[idx+3] = data[idx+3];
        }
    }
    data = output;
}
