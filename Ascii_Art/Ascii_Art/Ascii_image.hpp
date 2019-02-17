#pragma once
#include <string>
#include <array>
#include <queue>
#include <fstream>
#include "bitmap_image.hpp"
class Ascii_image
{
public:
	Ascii_image();
	void convert(std::ofstream& writer, const std::string& savename, bitmap_image image);

private:
	std::array<const char, 8> map_chars = { '@', '&', '#', 'B', 'x', '+', ':', '`' };
	std::queue<unsigned int> getDivisors(unsigned int num);
	std::tuple<unsigned int, unsigned int, unsigned int> average_pixels(unsigned int BLOCK_HEIGHT,
		unsigned int BLOCK_WIDTH, unsigned int xpos, unsigned int ypos,
		const bitmap_image& image);
	char choose_char(unsigned int r, unsigned int g, unsigned int b);
};
