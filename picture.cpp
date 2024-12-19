/* 
Lukyanova Angelina st128743@student.spbu.ru
Labwork1
*/
#include "picture.h"

// Constructor to get data from image
Picture::Picture(std::string filename) {
	std::ifstream file(filename, std::ios::binary);
	// If doesn't work
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

	if (infoheader.bitCount != 24) {
		std::cerr << "Умею работать пока только с глубиной 24" << std::endl;
	}

	_width = infoheader.width;
	_height = abs(infoheader.height);
	_bitCount = infoheader.bitCount;

	// Read data of pixels
	file.seekg(fileheader.offsetData, std::ios::beg);
	Data = new Pixel[getWidth() * getHeight()];
	const int padding = (4 - (getWidth() * 3) % 4) % 4;

	for (int y = 0; y < getHeight(); y++) {
		for (int x = 0; x < getWidth(); x++) {
			Pixel& pixel = Data[y * _width + x];
			unsigned char colorByte;

			// Read RGB to Pixel struct and convert them to float [0, 1]
			file.read(reinterpret_cast<char*>(&colorByte), 1);
			pixel.r = static_cast<float>(colorByte) / 255.0f;

			file.read(reinterpret_cast<char*>(&colorByte), 1);
			pixel.g = static_cast<float>(colorByte) / 255.0f;

			file.read(reinterpret_cast<char*>(&colorByte), 1);
			pixel.b = static_cast<float>(colorByte) / 255.0f;
		}
		file.ignore(padding); // Doesn't read 0 bytes because of byte alignment
	}

	file.close();
}

// Constructor if we have prepared data
Picture::Picture(Picture& other, int32_t w, int32_t h, uint16_t bc) {
	infoheader = other.infoheader;
	fileheader = other.fileheader;
	setWidth(w);
	setHeight(h);
	setBit_count(bc);
	Data = new Pixel[getWidth() * getHeight()];
}

Picture::~Picture() {
	delete[] Data;
}

void Picture::rotateRight(const Picture& other) {
	for (int i = 0; i < getHeight(); i++) {
		for (int j = 0; j < getWidth(); j++) {
			int index = (i * getWidth() + j);
			other.Data[(getWidth() - j - 1) * getHeight() + i] = Data[index];
		}
	}
}

void Picture::rotateLeft(const Picture& other) {
	for (int i = 0; i < getHeight(); i++) {
		for (int j = 0; j < getWidth(); j++) {
			int index = (i * getWidth() + j);
			other.Data[j * getHeight() + (getHeight() - 1 - i)] = Data[index];
		}
	}
}

// Gauss filter's generation
void Picture::generateGaussianFilter(float* filter, int radius, float sigma) {
	int size = 2 * radius + 1;
	float sum = 0;

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			int x = i - radius;
			int y = j - radius;
			float value = 1 / (2 * PI * sigma * sigma) * std::exp(-(x * x + y * y) / (2 * sigma * sigma));
			filter[i * size + j] = value;
			sum += value;
		}
	}

	for (int i = 0; i < size * size; ++i) {
		filter[i] /= sum; // Normalization
	}
}

void Picture::Gauss(const Picture& other, int radius, float sigma) {
	float* filter = new float[(2 * radius + 1) * (2 * radius + 1)];
	generateGaussianFilter(filter, radius, sigma);

	for (int i = 0; i < getHeight(); i++) {
		for (int j = 0; j < getWidth(); j++) {
			int index = i * getWidth() + j;
			Pixel mainp{0.0f, 0.0f, 0.0f};
			float totalWeight = 0.0f;

			for (int k = -radius; k <= radius; k++) {
				for (int l = -radius; l <= radius; l++) {
					int x = j + l; // Coords of neighbor pixel
					int y = i + k;
					if (x >= 0 && x < getWidth() && y >= 0 && y < getHeight()) {
						float weight = filter[(k + radius) * (2 * radius + 1) + (l + radius)]; // Weight of neighbor pixel
						mainp.r += Data[y * getWidth() + x].r * weight;
						mainp.g += Data[y * getWidth() + x].g * weight;
						mainp.b += Data[y * getWidth() + x].b * weight;
						totalWeight += weight;
					}
				}
			}

			// To remove dark frame because of borders
			mainp.r /= totalWeight;
			mainp.g /= totalWeight;
			mainp.b /= totalWeight;

			other.Data[index] = mainp;
		}
	}

	delete[] filter;
}

void Picture::saveImage(std::string filename) {
	std::ofstream output(filename, std::ios::binary);

	if (!output.is_open()) {
		std::cerr << "Не удалось создать файл." << std::endl;
		return;
	}

	// Reinitialization
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

			// Convert to [0, 255]
			unsigned char r = static_cast<unsigned char>(std::round(pixel.r * 255.0f));
			unsigned char g = static_cast<unsigned char>(std::round(pixel.g * 255.0f));
			unsigned char b = static_cast<unsigned char>(std::round(pixel.b * 255.0f));

			// Write in file
			output.write(reinterpret_cast<char*>(&r), 1);
			output.write(reinterpret_cast<char*>(&g), 1);
			output.write(reinterpret_cast<char*>(&b), 1);
		}

		for (int i = 0; i < ((4 - (getWidth() * 3) % 4) & 3); i++) {
			output.put(0);
		}
	}

	output.close();
}

// Getters
int32_t Picture::getHeight() {
	return _height;
}

int32_t Picture::getWidth() {
	return _width;
}

uint16_t Picture::getBit_count() {
	return _bitCount;
}

// Setters
void Picture::setHeight(int32_t h) {
	_height = h;
}

void Picture::setWidth(int32_t w) {
	_width = w;
}

void Picture::setBit_count(uint16_t b) {
	_bitCount = b;
}

int main() {
	int radius, sigma;
	std::string input;

	std::cout << "Print filePath: ";
	std::getline(std::cin, input); // Read line

	Picture* a = new Picture(input);

	std::cout << std::endl << "And radius: ";
	std::getline(std::cin, input); // Read line
	radius = std::stoi(input);

	std::cout << std::endl << "And sigma: ";
	std::getline(std::cin, input); // Read line
	sigma = std::stoi(input);
	std::cout << std::endl << "Wait a bit...";

	Picture* b = new Picture(*a, a->getHeight(), a->getWidth(), a->getBit_count());
	a->rotateRight(*b);
	Picture* c = new Picture(*a, a->getHeight(), a->getWidth(), a->getBit_count());
	a->rotateLeft(*c);
	delete a;
	b->saveImage("rotateRight.bmp");
	c->saveImage("rotateLeft.bmp");

	Picture* d = new Picture(*b, b->getWidth(), b->getHeight(), b->getBit_count());
	b->Gauss(*d, radius, sigma);
	delete b;

	Picture* e = new Picture(*c, c->getWidth(), c->getHeight(), c->getBit_count());
	c->Gauss(*e, radius, sigma);
	delete c;

	d->saveImage("gaussRight.bmp");
	e->saveImage("gaussLeft.bmp");
	delete d;
	delete e;

	std::cout << std::endl << "Pictures were saved as:" << std::endl<< "rotateRight.bmp" << std::endl<< "rotateLeft.bmp" << std::endl<< "gaussRight.bmp" << std::endl<< "gaussLeft.bmp" << std::endl << std::endl;

	return 0;
}