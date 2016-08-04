// TwoDee.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Buffer.h"
#include <complex>
#include <iostream>
#include <vector>

#define rgb(x,y,z) x.f/255.f, y.f/255.f, z.f/255.f

#define WIDTH 1080
#define HEIGHT 1920

typedef float FLOAT_FORMAT;

template<class MainType, class FloatType>
MainType lerp(MainType start, MainType end, FloatType percent)
{
	return (start + percent * (end - start));
}

void deux(std::vector<Color>& palette)
{
	palette.clear();

	std::vector<Color> ranges;

	// Full saturation spectrum
	ranges.push_back(Color(rgb(255, 0, 0)));			// Red
	ranges.push_back(Color(rgb(255, 255, 0)));			// Yellow
	ranges.push_back(Color(rgb(0, 255, 0)));			// Green
	ranges.push_back(Color(rgb(0, 255, 255)));			// Cyan
	ranges.push_back(Color(rgb(0, 0, 255)));			// Blue
	ranges.push_back(Color(rgb(255, 0, 255)));			// Fuschia
	ranges.push_back(Color(rgb(255, 0, 0)));			// Red

	Buffer b(Size(WIDTH, HEIGHT), Color());

	// Don't create too large of a palette (~ less iterations) if you want to create more jagged forms and "nerves".
	int max_iter = 256;

	// Ranges will depend on the current and the next. So divide by total - 1
	int div = ranges.size()-1;

	// Sections are then created.
	int sections = max_iter / div;

	for (int r=0; r<div; r++)
	{
		Color c1 = ranges[r], c2 = ranges[r+1];

		for (int s=0; s < sections; s++)
		{
			// lerp between color range and store it into the palette.
			palette.push_back(lerp<Color, float>(c1, c2, (float)s / (float)sections));
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<Color> palette;

	deux(palette);

	const FLOAT_FORMAT scale = 0.00003f;
	const FLOAT_FORMAT center_x = -0.1592f, center_y = -1.0350f; //0.001643721971153, center_y = 0.822467633298876;

	const FLOAT_FORMAT left_keep = center_x - (FLOAT_FORMAT)WIDTH / 2 * scale;
	FLOAT_FORMAT top  = center_y - (FLOAT_FORMAT)HEIGHT / 2 * scale;

	std::complex<FLOAT_FORMAT> c, z;

	Buffer b(Size(WIDTH, HEIGHT), Color(rgb(219,112,147)));

	for (int j=0; j<HEIGHT; j++, top += scale)
	{
		FLOAT_FORMAT left = left_keep;

		for (int i=0; i<WIDTH; i++, left += scale)
		{
			c.real(left);
			c.imag(top);

			z.real(0);
			z.imag(0);

			size_t h = 0;

			for (; h<palette.size(); h++)
			{
				z = z * z + c;

				// sqrt(z.real() ^ 2 + z.imag() ^ 2) > 2 ==> slower
				if ((z.real() * z.real() + z.imag() * z.imag()) > 4)
					break;
			}

			if (h == palette.size())
				b.Set(Point(i,j), Color());
			else
				b.Set(Point(i,j), palette[h]);
		}

		std::cout << "Row " << j << std::endl;
	}

	b.SaveAsTGA("test.tga");
	return 0;
}

