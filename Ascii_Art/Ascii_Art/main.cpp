#include <iostream>
#include <fstream>
#include <filesystem>
#include "bitmap_image.hpp"
#include "Ascii_image.hpp"

using namespace std;

int main()
{
	ofstream txt_output;
	bitmap_image image = bitmap_image("foo.bmp");
	Ascii_image converter = Ascii_image();
	converter.convert(txt_output, "foo_ascii.txt", image);
	system("pause");
	return 0;
}