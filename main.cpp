#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <string>

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{0x4D42};      // Магические символы 'BM' (0x424D)
    uint32_t file_size{0};           // Общий размер файла в байтах
    uint16_t reserved1{0};           // Зарезервировано (обычно 0)
    uint16_t reserved2{0};           // Зарезервировано (обычно 0)
    uint32_t offset_data{54};        // Смещение до начала данных изображения
};

struct BMPInfoHeader {
    uint32_t size{40};               // Размер структуры BMPInfoHeader
    int32_t width{0};                // Ширина изображения в пикселях
    int32_t height{0};               // Высота изображения в пикселях
    uint16_t planes{1};              // Количество плоскостей (обычно 1)
    uint16_t bit_count{0};           // Глубина цвета (количество бит на пиксель)
    uint32_t compression{0};         // Метод сжатия (0 = отсутствие сжатия)
    uint32_t size_image{0};          // Размер изображения в байтах
    int32_t x_pixels_per_meter{0};   // Горизонтальное разрешение (необязательно)
    int32_t y_pixels_per_meter{0};   // Вертикальное разрешение (необязательно)
    uint32_t colors_used{0};         // Количество цветов в палитре (если 0, то используется максимальное количество цветов)
    uint32_t colors_important{0};    // Важные цвета (если 0, то все цвета важны)
};
#pragma pack(pop)

void loadBMP(std::string filename, uint8_t** imageData, int& width, int& height, int& bitDepth) {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

    if (fileHeader.file_type != 0x4D42) {
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

    *imageData = new uint8_t[rowSize * height];

    file.seekg(fileHeader.offset_data);
    for (size_t y = 0; y < static_cast<size_t>(height); ++y) {
        file.read(reinterpret_cast<char*>(&(*imageData)[(height - y - 1) * rowSize]), rowSize);
    }

    file.close();
}

void rotateImageData(uint8_t* imageData, int& width, int& height, int& bitDepth) {
    const size_t bytesPerPixel = bitDepth / 8;
    uint8_t* rotatedImageData = new uint8_t[width * height * bytesPerPixel];

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int srcIndex = (y * width + x) * bytesPerPixel;
            int destX = height - 1 - y;
            int destY = x;
            int destIndex = (destY * height + destX) * bytesPerPixel;

            for (int c = 0; c < bytesPerPixel; ++c) {
                rotatedImageData[destIndex + c] = imageData[srcIndex + c];
            }
        }
    }
    imageData = rotatedImageData;
    int temp = height;
    height = width;
    width = temp;


}
void saveBMP(const std::string& filename, const uint8_t* imageData, int width, int height, int bitDepth) {
    std::ofstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Не удалось создать файл." << std::endl;
        return;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    const size_t bytesPerPixel = bitDepth / 8;
    
    const size_t rowSize = ((bytesPerPixel * width + 3) & (~3));
    const size_t padding = rowSize - (width * bytesPerPixel);
    const size_t totalImageSize = rowSize * height;

    fileHeader.file_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + totalImageSize;
    infoHeader.size_image = totalImageSize;
    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.bit_count = bitDepth;
    
    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(BMPFileHeader));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(BMPInfoHeader));

    

    for (size_t y = 0; y < static_cast<size_t>(height); ++y) {
        file.write(reinterpret_cast<const char*>(&imageData[(height - y - 1) * rowSize]), rowSize);
    }

    file.close();
}

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