#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <string>
#include "main.h"

// хочу написать 2 класса: класс, который загружает изображение, выгружает его, а вот обрабатывать массив пикселей будет другой класс
// класс матрицы будет из непонятного формата переводить imageData в более понятный динамический двумерный массив с указателями и обратно тоже запаковывать будет




Matrix::Matrix(int width, int height,uint8_t imageData): _MatrixWidth(width), _MatrixHeight(height){
    // !!!!!!!!!!! здесь должна быть функция для обработки imageData
}

Matrix::~Matrix(){
    delete[] MatrixPixels;
}

uint8_t Matrix::returnImageData(){
    // как-то обрабатываем 
}



Image::Image(std::string filename){  
    loadBMP(filename);
}

Image::~Image(){
    delete[] imageData;
}
void Image::setWidth(int width){ // чтобы красивее было
    _ImageWidth = width;
}
void Image::setHeight(int height){ // чтобы красивее было
    _ImageHeight = height;
}
void Image::loadBMP(std::string filename){  // загружаем изображение
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) { // файлик не открылся :(
        std::cerr << "Не удалось открыть файл." << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

    if (fileHeader.file_type != 0x4D42) { // если не битмап, тоше ошибка :(
        std::cerr << "Неверный формат файла." << fileHeader.file_type << std::endl;
        return;
    }

    width = infoHeader.width;
    height = abs(infoHeader.height);
    bitDepth = infoHeader.bit_count;

    const size_t bytesPerPixel = bitDepth / 8;
    const size_t padding = (4 - ((width * bytesPerPixel) % 4)) & 3;
    const size_t rowSize = width * bytesPerPixel + padding;
    const size_t totalImageSize = rowSize * height;

    fileHeader.file_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + totalImageSize;
    infoHeader.size_image = totalImageSize;

    imageData = new uint8_t[rowSize * height];

    file.seekg(fileHeader.offset_data);
    for (size_t y = 0; y < static_cast<size_t>(height); ++y) {
        file.read(reinterpret_cast<char*>(&(*imageData)[(height - y - 1) * rowSize]), rowSize);
    }

    file.close();

}

void saveBMP(std::string filename, uint8_t ImageChangedData, int width, int height);




int main() {
    std::string inputFilename = "m.bmp";
    std::string outputFilename = "output.bmp";

    uint8_t* imageData = nullptr;
    int width = 0, height = 0, bitDepth = 0;

    loadBMP(inputFilename, &imageData, width, height, bitDepth);
    std::cout<<sizeof(imageData);
    rotateImageData(imageData, width, height, bitDepth);
    std::cout<<sizeof(imageData);
    saveBMP(outputFilename, imageData, width, height, bitDepth);

    delete[] imageData;

    return 0;
}