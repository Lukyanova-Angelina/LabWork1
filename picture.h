/* 
Lukyanova Angelina st128743@student.spbu.ru
Labwork1
*/
#ifndef PICTURE_H
#define PICTURE_H
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <string>
#include <cmath>

#pragma pack(push, 1)
// bmp structure
							

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

// struct for pixels
struct Pixel {
	float r{0.0f};
	float g{0.0f};
	float b{0.0f};
};

const double PI = acos(-1.0);

class Picture {
private:
	int32_t _width;
	int32_t _height;
	uint16_t _bitCount;
	Pixel* Data;
	BMPFileHeader fileheader;
	BMPInfoHeader infoheader;

public:
	Picture(std::string filename);
	Picture(Picture& other, int32_t w, int32_t h, uint16_t bc);
	~Picture();

	void saveImage(std::string filename = "output.bmp");
	// functions for gauss filter
	void Gauss(const Picture& other, int r, float s);
	void generateGaussianFilter(float* filter, int radius, float sigma);

	// functions for rotation
	void rotateRight(const Picture& other);
	void rotateLeft(const Picture& other);

	// getters
	int32_t getHeight();
	int32_t getWidth();
	uint16_t getBit_count();

	// setters
	void setHeight(int32_t h);
	void setWidth(int32_t w);
	void setBit_count(uint16_t b);
};
#endif