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
    return 0;
}



Image::Image(std::string filename){  
    loadBMP(filename);
}

Image::~Image(){
    delete[] _imageData;
}
void Image::setWidth(int width){ // чтобы красивее было
    _ImageWidth = width;
}
void Image::setHeight(int height){ // чтобы красивее было
    _ImageHeight = height;
}
void Image::setBitDepth(int bitDepth){ // чтобы красивее было
    _ImageBitDepth = bitDepth;
}
int Image::getWidth(){
    return _ImageWidth;
}
int Image::getHeight(){
    return _ImageHeight;
}
int Image::getBitDepth(){
    return _ImageBitDepth;
}
uint8_t* Image::getImageData(){
    return _imageData;
}


void Image::loadBMP(std::string filename){  // загружаем изображение
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) { // файлик не открылся :(
        std::cerr << "Не удалось открыть файл." << std::endl;
        return;
    }
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

    if (fileHeader.file_type != 0x4D42) { // если не битмап, тоше ошибка :(
        std::cerr << "Неверный формат файла." << fileHeader.file_type << std::endl;
        return;
    }

    setWidth(infoHeader.width);
    setHeight(abs(infoHeader.height));
    setBitDepth(infoHeader.bit_count);
    size_t bytesPerPixel =  getBitDepth() / 8;
    size_t padding = ((getWidth() * bytesPerPixel + 3) & (~3));
    size_t rowSize = getWidth() * bytesPerPixel + padding;
    size_t totalImageSize = rowSize * getHeight();

    fileHeader.file_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + totalImageSize;
    infoHeader.size_image = totalImageSize;

    _imageData = new uint8_t[rowSize * getHeight()];

    file.seekg(fileHeader.offset_data);
    for (size_t y = 0; y < static_cast<size_t>(getHeight()); ++y) {
        file.read(reinterpret_cast<char*>(_imageData + y * rowSize), rowSize);
    }

    file.close();

}

        
void Image::saveBMP(std::string filenameoutput, uint8_t* ImageChangedData, int width, int height, int bitDepth) {
    std::ofstream file(filenameoutput, std::ios::binary);

    if (!file.is_open()) {
        std::cout << "Не удалось создать файл." << std::endl;
        return;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    size_t bytesPerPixel = bitDepth / 8;

    size_t rowSize = ((bytesPerPixel * width + 3) & (~3));
    size_t padding = rowSize - (width * bytesPerPixel);
    size_t totalImageSize = rowSize * height;

    fileHeader.file_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + totalImageSize;
    infoHeader.size_image = totalImageSize;
    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.bit_count = bitDepth;
    
    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(BMPFileHeader));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(BMPInfoHeader));

    

    for (size_t y = 0; y < static_cast<size_t>(height); ++y) {
        file.write(reinterpret_cast<const char*>(&ImageChangedData[(y) * rowSize]), rowSize);
    }

    file.close();
}



int main() {
    Image img1("l.bmp");
    img1.saveBMP("hello.bmp", img1.getImageData(), img1.getWidth(), img1.getHeight(), img1.getBitDepth());
    return 0;
}