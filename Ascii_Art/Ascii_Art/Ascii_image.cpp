#include "Ascii_image.hpp"
#include <string>
#include <queue>
#include <fstream>
#include <array>
#include <tuple>
Ascii_image::Ascii_image()
{

}
void Ascii_image::convert(std::ofstream& writer, const std::string & savename, bitmap_image image)
{
	image.convert_to_grayscale();
	unsigned int pic_width = image.width();
	unsigned int pic_height = image.height();
	unsigned int BLOCK_WIDTH = 5;
	unsigned int BLOCK_HEIGHT = 5;

	writer.open(savename);

	for (size_t i = 0; i < pic_height; i += BLOCK_HEIGHT)
	{
		for (size_t j = 0; j < pic_width; j += BLOCK_WIDTH)
		{
			auto vals = average_pixels(BLOCK_HEIGHT, BLOCK_WIDTH, j, i, image);
			unsigned int red = std::get<0>(vals);
			unsigned int blue = std::get<1>(vals);
			unsigned int grn = std::get<2>(vals);

			//image.set_region(j, i, BLOCK_WIDTH, BLOCK_HEIGHT, red, grn, blue);
			writer << choose_char(red, grn, blue);
		}
		writer << "\n";
	}
	writer.close();
}
std::queue<unsigned int> Ascii_image::getDivisors(unsigned int num)
{
	unsigned int lim = sqrt(num);
	std::queue<unsigned int> ret = std::queue<unsigned int>();
	for (size_t i = 0; i < lim; i++)
	{
		if (i % num == 0)
		{
			ret.push(i);
		}
	}
	return ret;
}
std::tuple<unsigned int, unsigned int, unsigned int> Ascii_image::average_pixels(
	unsigned int BLOCK_HEIGHT, unsigned int BLOCK_WIDTH, unsigned int xpos,
	unsigned int ypos, const bitmap_image & image)
{
	unsigned int avg_red = 0, avg_blue = 0, avg_grn = 0;

	for (size_t i = 0; i < BLOCK_WIDTH; i++)
	{
		for (size_t j = 0; j < BLOCK_HEIGHT; j++)
		{
			avg_red += image.get_pixel(xpos + i, ypos + j).red;
			avg_blue += image.get_pixel(xpos + i, ypos + j).blue;
			avg_grn += image.get_pixel(xpos + i, ypos + j).green;
		}
	}
	avg_red /= (BLOCK_HEIGHT * BLOCK_WIDTH);
	avg_blue /= (BLOCK_HEIGHT * BLOCK_WIDTH);
	avg_grn /= (BLOCK_HEIGHT * BLOCK_WIDTH);
	return std::make_tuple(avg_red, avg_blue, avg_grn);
}
char Ascii_image::choose_char(unsigned int r, unsigned int g, unsigned int b)
{
	unsigned int avg = (r + g + b) / 3;

	if (avg < 32)
	{
		return map_chars[0];
	}
	else if (avg < 64)
	{
		return map_chars[1];
	}
	else if (avg < 96)
	{
		return map_chars[2];
	}
	else if (avg < 128)
	{
		return map_chars[3];
	}
	else if (avg < 160)
	{
		return map_chars[4];
	}
	else if (avg < 192)
	{
		return map_chars[5];
	}
	else if (avg < 224)
	{
		return map_chars[6];
	}
	else if (avg < 256)
	{
		return map_chars[7];
	}
	return NULL;
}
