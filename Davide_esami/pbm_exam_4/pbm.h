#pragma once

#include <vector>
#include <string>

struct BinaryImage {
    int W;
    int H;
    std::vector<uint8_t> ImageData;

    bool ReadFromPBM(const std::string& filename);

    auto begin() const { return ImageData.begin(); }
    auto end() const { return ImageData.end(); }
    auto begin(){ return ImageData.begin(); }
    auto end(){ return ImageData.end(); }

};

struct Image {
    int W;
    int H;
    std::vector<uint8_t> ImageData;
};

Image BinaryImageToImage(const BinaryImage& bimg);
