#include "picture.h"
Picture::Picture(std::string filename)
{
	std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return;
    }
    file.read(reinterpret_cast<char*>(&fileheader), sizeof(BMPFileHeader));
    file.read(reinterpret_cast<char*>(&infoheader), sizeof(BMPInfoHeader));
    if (fileheader.fileType != 0x4D42) {
        std::cerr << "Неверный формат файла." << fileheader.fileType << std::endl;
        return;
    }
    _width = infoheader.width;
    _height = abs(infoheader.height);
    _bitCount = infoheader.bitCount;
    int32_t rowSize;
    if(getBit_count() == 24)
    {
    	rowSize = ((getWidth() * 3 + 3) & ~3);
        std::cout<<"24";
    }
    else
    {
    	rowSize = getWidth() * 4;
        std::cout<<"32";
    }
    imageData = new uint8_t[rowSize * _height];
    file.seekg(fileheader.offsetData);
    for (int i = 0; i < _height; ++i) {
        file.read(reinterpret_cast<char*>(imageData + i * rowSize), rowSize);
    }
    file.close();
    loadimage();
    saveImage(infoheader.width, infoheader.height);
	
}
Picture::~Picture(){
	delete[] imageData;
}

void Picture::loadimage() {
    Data = new Pixel[getWidth() * getHeight()];
    
    for (int i = 0; i < getHeight(); i++) { 
        for (int j = 0; j < getWidth(); j++) {
            int index = (i * getWidth() + j);
            Data[index].b = imageData[index * 3];
            Data[index].g = imageData[index * 3 + 1];
            Data[index].r = imageData[index * 3 + 2];
        }
    }
    saveData();
}

void Picture::saveData() {
    int rowSize;
    if (getBit_count() == 24) {
        rowSize = ((getWidth() * 3 + 3) & ~3); 
    } else {
        rowSize = getWidth() * 4; 
    }
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            int index = (i * getWidth() + j);
            int pixelIndex = i * rowSize + j * ((getBit_count() == 24) ? 3 : 4);
            imageData[pixelIndex]     = Data[index].b; 
            imageData[pixelIndex + 1] = Data[index].g; 
            imageData[pixelIndex + 2] = Data[index].r; 
            
            if (getBit_count() == 32) {
                imageData[pixelIndex + 3] = 255; 
            }
        }
    }
}
void Picture::rotateRight(){

}
void Picture::saveImage(int32_t width, int32_t height) {
    std::ofstream output("output.bmp", std::ios::binary);

    if (!output.is_open()) {
        std::cerr << "Не удалось создать файл." << std::endl;
        return;
    }

    BMPFileHeader fileHeader_output;
    BMPInfoHeader infoHeader_output;
    fileHeader_output = fileheader;
    infoHeader_output = infoheader;
    infoHeader_output.width = width;
    infoHeader_output.height = height;
    output.write(reinterpret_cast<const char*>(&fileHeader_output), sizeof(BMPFileHeader));
    output.write(reinterpret_cast<const char*>(&infoHeader_output), sizeof(BMPInfoHeader));
    int rowSize;
    if(getBit_count() == 24)
    {
        rowSize = ((width * 3 + 3) & ~3);
    }
    else
    {
        rowSize = width * 4;
    }
    for (int i = 0; i < abs(height); ++i) {
        output.write(reinterpret_cast<const char*>(imageData + i * rowSize), rowSize);
    }

    output.close();
}
    //getters
int32_t Picture::getHeight()
{
	return _height;
}
int32_t Picture::getWidth()
{
	return _width;
}
uint16_t Picture::getBit_count()
{
	return _bitCount;
}

    //setters
void Picture::setHeight(int32_t h)
{
	_height = h;
}
void Picture::setWidth(int32_t w)
{
	_width = w;
}
void Picture::setBit_count(uint16_t b)
{
	_bitCount = b;
}

int main()
{
    Picture a = Picture("hello.bmp");
	return 0;
}