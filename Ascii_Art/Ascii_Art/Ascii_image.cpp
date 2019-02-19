#include "Ascii_image.hpp"
#include <string>
#include <queue>
#include <fstream>
#include <array>
#include <tuple>
#include <cmath>

using std::abs;

Ascii_image::Ascii_image()
{

}
void Ascii_image::convert(std::ofstream& writer, const std::string & savename, bitmap_image image)
{
	image.convert_to_grayscale();
	unsigned int pic_width = image.width();
	unsigned int pic_height = image.height();
	unsigned int BLOCK_WIDTH = 2;
	unsigned int BLOCK_HEIGHT = 2;

	writer.open(savename);

	for (size_t i = 0; i < pic_height; i += BLOCK_HEIGHT)
	{
		for (size_t j = 0; j < pic_width; j += BLOCK_WIDTH)
		{
			auto vals = average_pixels(BLOCK_HEIGHT, BLOCK_WIDTH, j, i, image);
			unsigned int red = std::get<0>(vals);
			unsigned int grn = std::get<1>(vals);
			unsigned int blue = std::get<2>(vals);

			writer << choose_map_char(red, grn, blue);
		}
		writer << "\n";
	}
	writer.close();
}
void Ascii_image::outline(std::ofstream & writer, const std::string & savename, bitmap_image image)
{
	bitmap_image _image = image;
	unsigned int pic_width = _image.width();
	unsigned int pic_height = _image.height();
	unsigned int BLOCK_WIDTH = 5;
	unsigned int BLOCK_HEIGHT = 5;
	pixelate(_image);
	int threshold = 100;

	writer.open(savename);

	for (int i = 0; i < pic_height; i += BLOCK_HEIGHT)
	{
		for (int j = 0; j < pic_width; j += BLOCK_WIDTH)
		{
			rgb_t pixel = _image.get_pixel(j, i);
			char p_avg = (pixel.red + pixel.green + pixel.blue) / 3;
			char l_avg = 0, r_avg = 0, b_avg = 0, t_avg = 0;

			if (j - 1 > 0)
			{
				rgb_t left = _image.get_pixel(j - 1, i);
				l_avg = (left.red + left.green + left.blue) / 3;
			}
			if (j + BLOCK_WIDTH < pic_width)
			{
				rgb_t right = _image.get_pixel(j + BLOCK_WIDTH, i);
				r_avg = (right.red + right.green + right.blue) / 3;
			}
			if (i - 1 > 0)
			{
				rgb_t top = _image.get_pixel(j, i - 1);
				t_avg = (top.red + top.green + top.blue) / 3;
			}
			if (i + BLOCK_HEIGHT < pic_height)
			{
				rgb_t bottom = _image.get_pixel(j, i + BLOCK_HEIGHT);
				b_avg = (bottom.red + bottom.green + bottom.blue) / 3;
			}


			if (abs(p_avg - l_avg) < threshold && abs(p_avg - r_avg) < threshold &&
				abs(p_avg - t_avg) < threshold && abs(p_avg - b_avg) < threshold)
			{
				if (p_avg < threshold)
				{
					writer << "`";
				}
				else
				{
					writer << " ";
				}
			}
			else if (p_avg - l_avg > threshold && p_avg - r_avg > threshold &&
				p_avg - t_avg > threshold && p_avg - b_avg > threshold)
			{
				writer << ".";
			}
			else if (p_avg - l_avg > threshold && p_avg - r_avg < threshold &&
				p_avg - t_avg > threshold && p_avg - b_avg < threshold)
			{
				writer << "/";
			}
			else if (abs(p_avg - r_avg) > threshold && abs(p_avg - l_avg) < threshold &&
				abs(p_avg - t_avg) > threshold && abs(p_avg - b_avg) < threshold)
			{
				writer << "\\";
			}
			else if (abs(p_avg - b_avg) < threshold && abs(p_avg - t_avg) < threshold)
			{
				writer << "|";
			}
			else if (abs(p_avg - r_avg) < threshold && abs(p_avg - t_avg) < threshold)
			{
				writer << "_";
			}

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
			avg_grn += image.get_pixel(xpos + i, ypos + j).green;
			avg_blue += image.get_pixel(xpos + i, ypos + j).blue;
		}
	}
	avg_red /= (BLOCK_HEIGHT * BLOCK_WIDTH);
	avg_grn /= (BLOCK_HEIGHT * BLOCK_WIDTH);
	avg_blue /= (BLOCK_HEIGHT * BLOCK_WIDTH);
	return std::make_tuple(avg_red, avg_grn, avg_blue);
}
char Ascii_image::choose_map_char(unsigned int r, unsigned int g, unsigned int b)
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

void Ascii_image::pixelate(bitmap_image& image)
{
	image.convert_to_grayscale();
	unsigned int pic_width = image.width();
	unsigned int pic_height = image.height();
	unsigned int BLOCK_WIDTH = 5;
	unsigned int BLOCK_HEIGHT = 5;

	for (size_t i = 0; i < pic_height; i += BLOCK_HEIGHT)
	{
		for (size_t j = 0; j < pic_width; j += BLOCK_WIDTH)
		{
			auto vals = average_pixels(BLOCK_HEIGHT, BLOCK_WIDTH, j, i, image);
			image.set_region(j, i, BLOCK_WIDTH, BLOCK_HEIGHT, std::get<0>(vals),
				std::get<1>(vals), std::get<2>(vals));
		}
	}
	image.save_image("boop.bmp");
}
