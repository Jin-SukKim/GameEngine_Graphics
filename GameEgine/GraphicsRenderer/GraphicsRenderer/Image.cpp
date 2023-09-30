#include "Image.h"

#include <memory>
int width = 0, height = 0, channels = 0;

void ReadFromFile(const char* fileName)
{
	// 1차원처럼 이미지에서 데이터를 가져온다. (색의 범위는 [0, 255]이므로 8bit만 있으면 된다)
	unsigned char* img = stbi_load(fileName, &width, &height, &channels, 0);

	delete img;
}