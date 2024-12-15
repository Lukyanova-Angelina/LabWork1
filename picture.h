#ifndef PICTURE_H
#define PICTURE_H
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <string>

#pragma pack(push, 1)

struct BMPFileHeader {
    uint16_t fileType{};
    uint32_t fileSize{};
    uint16_t reserved1{};
    uint16_t reserved2{};
    uint32_t offsetData{};
};

struct BMPInfoHeader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression{};
    uint32_t imageSize{};
    int32_t xPixelsPerMeter{};
    int32_t yPixelsPerMeter{};
    uint32_t colorsUsed{};
    uint32_t colorsImportant{};
};

#pragma pack(pop)
struct Pixel{
    uint8_t r{0};
    uint8_t g{0};
    uint8_t b{0};
};


class Picture
{
private:
    int32_t _width;
    int32_t _height;
    uint16_t _bitCount;
    Pixel* Data;
    BMPFileHeader fileheader;
    BMPInfoHeader infoheader;

public:
    Picture(std::string filename);
    ~Picture();
    void saveImage();
    void Gauss(int r, float s);
    float* generateGaussianFilter(int radius, float sigma);
    
    //void loadimage();
    //void saveData();
    
    void rotateRight();    
    void rotateLeft();

    //getters
    int32_t getHeight();
    int32_t getWidth();
    uint16_t getBit_count();

    //setters
    void setHeight(int32_t h);
    void setWidth(int32_t w);
    void setBit_count(uint16_t b);        

};
#endif
