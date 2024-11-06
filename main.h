#ifndef MAIN
#define MAIN


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


struct Pixel{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
}



class Matrix{
private:
    int _MatrixWidth, _MatrixHeight;
    Pixel** MatrixPixels; // это будет двумерный массив с указателями на пиксели
public:
    Matrix(int width, int height, uint8_t imageData);
    ~Matrix();
    // надо придумать как переделать дату в нормальный массив !!!!!!!!!!!!!!!!!!
    uint8_t returnImageData();
}

class Image{
private: 
	BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    uint8_t* imageData;
    int _ImageWidth, _ImageHeight, _ImageBitDepth;
public:
	Image(std::string filename);
	~Image();
	void loadBMP(std::string filename);
	void saveBMP(std::string filename, uint8_t ImageChangedData, int width, int height);


	// setters 
	void setWidth(int width);
	void setHeight(int height);
}

#endif