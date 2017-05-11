// Copyright (C) 2014 University of Kaiserslautern
// Microelectronic System Design Research Group
//
// This file is part of the FPGAHS Course
// de.uni-kl.eit.course.fpgahs
//
// Matthias Jung <jungma@eit.uni-kl.de>
// Christian De Schryver <schryver@eit.uni-kl.de>

#include<image.h>

#include <set>
using std::multiset;

void medianFilter(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height)
{
	multiset<unsigned char> pixel;
	for (unsigned x = 1; x < (width-1); x++)
	{
		for (unsigned y = 1; y < (height-1); y++)
		{
			// clear set
			pixel.clear();

			// insert all pixel values
			pixel.insert(input[x-1][y-1]);
			pixel.insert(input[x-1][y]);
			pixel.insert(input[x-1][y+1]);
			pixel.insert(input[x][y-1]);
			pixel.insert(input[x][y]);
			pixel.insert(input[x][y+1]);
			pixel.insert(input[x+1][y-1]);
			pixel.insert(input[x+1][y]);
			pixel.insert(input[x+1][y+1]);

			// Get value in the mid of set
			output[x][y] = *std::next(pixel.begin(), 4);
		}
	}
}

