#include "Image.h"

#include <memory>
int width = 0, height = 0, channels = 0;

void ReadFromFile(const char* fileName)
{
	// 1����ó�� �̹������� �����͸� �����´�. (���� ������ [0, 255]�̹Ƿ� 8bit�� ������ �ȴ�)
	unsigned char* img = stbi_load(fileName, &width, &height, &channels, 0);

	delete img;
}