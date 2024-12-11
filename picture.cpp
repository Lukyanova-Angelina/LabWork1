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
    }
    else
    {
    	rowSize = getWidth() * 4;
    }
    imageData = new uint8_t[rowSize * _height];



	loadimage();
}
Picture::~Picture(){
	delete imageData;
}

void Picture::loadimage(){
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
	return 0;
}