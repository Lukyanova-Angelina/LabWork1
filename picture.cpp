#include "picture.h"
#include <cmath>
const double PI = acos(-1.0);
Picture::Picture(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return;
    }
    file.read(reinterpret_cast<char*>(&fileheader), sizeof(BMPFileHeader));
    if (fileheader.fileType != 0x4D42) {
        std::cerr << "Неверный формат файла." << fileheader.fileType << std::endl;
        return;
    }    
    if (!file.good()) {
        std::cerr << "Ошибка чтения заголовка файла." << std::endl;
        return;
    }
    file.read(reinterpret_cast<char*>(&infoheader), sizeof(BMPInfoHeader));
    if (!file.good()) {
        std::cerr << "Ошибка чтения информации о файле." << std::endl;
        return;
    }

    _width = infoheader.width;
    _height = abs(infoheader.height);
    _bitCount = infoheader.bitCount;

    file.seekg(fileheader.offsetData, std::ios::beg);
    Data = new Pixel[getWidth() * getHeight()];
    const int padding = (4 - (getWidth() * 3) % 4) % 4;
    for (int y = 0; y < getHeight(); y++) {
        for (int x = 0; x < getWidth(); x++) {
            Pixel& pixel = Data[y * getWidth() + x];
            file.read(reinterpret_cast<char*>(&pixel.r), 1);
            file.read(reinterpret_cast<char*>(&pixel.g), 1);
            file.read(reinterpret_cast<char*>(&pixel.b), 1);
        }
        file.ignore(padding);
    }

    file.close();
}
Picture::Picture(Picture& other, int32_t w, int32_t h, uint16_t bc)
{

    infoheader = other.infoheader;
    fileheader = other.fileheader;
    setWidth(w);
    setHeight(h);
    setBit_count(bc);
    Data = new Pixel[getWidth() * getHeight()];

}

Picture::~Picture(){
    delete[] Data;
}




void Picture::rotateRight(const Picture& other){
        for (int i = 0; i < getHeight(); i++) { 
        for (int j = 0; j < getWidth(); j++) {
            int index = (i * getWidth() + j);
            other.Data[(getWidth() - j - 1) * getHeight() + i] = Data[index];
        }
    }

}



void Picture::rotateLeft(const Picture& other){
    for (int i = 0; i < getHeight(); i++) { 
        for (int j = 0; j < getWidth(); j++) {
            int index = (i * getWidth() + j);
            other.Data[j * getHeight() + (getHeight() - 1 - i)] = Data[index];
        }
    }

}







float* Picture::generateGaussianFilter(int radius, float sigma) {

    int size = 2 * radius + 1;
    //double PI = 3.141592653589793;
    float* filter = new float[size * size];
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int x = i - radius;
            int y = j - radius;
            float value = 1 /(2 * PI * sigma * sigma) * std::exp(-(x * x + y * y) / (2 * sigma * sigma));
            filter[i * size + j] = value;
            sum += value;
        }
    }
    float Temp = 0;
    for (int i = 0; i < size * size; ++i) {
        filter[i] /= sum;
        Temp += filter[i];
    }
    ;

    return filter;


}
void Picture::Gauss(int radius, float sigma) {
    float *filter = generateGaussianFilter(radius, sigma);
    Pixel *mass = new Pixel[getWidth() * getHeight()];

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            int index = i * getWidth() + j;
            Pixel mainp {0, 0, 0};
            float totalWeight = 0.0f;

            for (int k = -radius; k <= radius; k++) {
                for (int l = -radius; l <= radius; l++) {
                    int x = j + l;
                    int y = i + k;

                    if (x >= 0 && x < getWidth() && y >= 0 && y < getHeight()) {
                        float weight = filter[(k + radius) * (2 * radius + 1) + (l + radius)];
                        mainp.r += Data[y * getWidth() + x].r * weight;
                        mainp.g += Data[y * getWidth() + x].g * weight;
                        mainp.b += Data[y * getWidth() + x].b * weight;
                        totalWeight += weight;
                    }
                }
            }

            // Нормализация цвета
            if (totalWeight > 0) {
                mainp.r /= totalWeight;
                mainp.g /= totalWeight;
                mainp.b /= totalWeight;
            }

            mass[index] = mainp;
        }
    }

    for (int i = 0; i < getHeight(); ++i) {
        for (int j = 0; j < getWidth(); ++j) {
            Data[i * getWidth() + j] = mass[i * getWidth() + j];
        }
    }

    delete[] mass; 
    delete[] filter; 
}
void Picture::saveImage(std::string filename){
    std::ofstream output(filename, std::ios::binary);

    if (!output.is_open()) {
        std::cerr << "Не удалось создать файл." << std::endl;
        return;
    }
    const int dataSize = (getWidth() * 3 + (4 - (getWidth() * 3) % 4) % 4) * getHeight();
    fileheader.fileType = 0x4D42;
    fileheader.fileSize = sizeof(fileheader) + sizeof(infoheader) + dataSize;
    fileheader.offsetData = sizeof(fileheader) + sizeof(infoheader);
    infoheader.size = sizeof(infoheader);
    infoheader.width = getWidth();
    infoheader.height = getHeight();
    infoheader.planes = 1;
    infoheader.bitCount = 24;
    infoheader.imageSize = dataSize;
    output.write(reinterpret_cast<const char*>(&fileheader), sizeof(BMPFileHeader));
    output.write(reinterpret_cast<const char*>(&infoheader), sizeof(BMPInfoHeader));

    for (int y = 0; y < getHeight(); y++) {
        for (int x = 0; x < getWidth(); x++) {
            Pixel& pixel = Data[y * getWidth() + x];
            output.write(reinterpret_cast<char*>(&pixel.r), 1);
            output.write(reinterpret_cast<char*>(&pixel.g), 1);
            output.write(reinterpret_cast<char*>(&pixel.b), 1);
        }
        for (int i = 0; i < ((4 - (getWidth() * 3) % 4) & 3); i++){
            output.put(0);
        }
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
    Picture* b = new Picture(a, a.getHeight(), a.getWidth(), a.getBit_count());
    //b->Data;
    a.rotateRight(*b);
    b->saveImage("rotateRight.bmp");
    delete b;
    Picture* c = new Picture(a, a.getHeight(), a.getWidth(), a.getBit_count());
    a.rotateLeft(*c);
    c->saveImage("rotateLeft.bmp");
    delete c;
    Picture* d = new Picture(a, a.getWidth(), a.getHeight(), a.getBit_count());
    a.Gauss(10, 5);
    delete d;
    a.saveImage("mqwerthgfghjkkkkkkkkkkkkkkkkkkkkkk.bmp");
    return 0;
}