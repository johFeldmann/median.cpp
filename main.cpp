// Copyright (C) 2014 University of Kaiserslautern
// Microelectronic System Design Research Group
// 
// This file is part of the FPGAHS Course
// de.uni-kl.eit.course.fpgahs
// 
// Matthias Jung <jungma@eit.uni-kl.de>
// Christian De Schryver <schryver@eit.uni-kl.de>

#include <iostream>
#include "profiler.h"
#include "median.h"
#include "EasyBMP/EasyBMP.h"

// define IMAGE, WIDTH, HEIGHT
#include "image.h"

using namespace std;


int main(void)
{
	unsigned char OUT[WIDTH][HEIGHT];

	// Do a profiling of this block:
	{
		Profiler p;
		medianFilter(IMAGE, OUT, WIDTH, HEIGHT);
	
	}

	// Test image
	unsigned changes = medianTest(IMAGE, OUT, WIDTH, HEIGHT);

	cout << "Changes done: " << changes << endl;

	// Save Image:
        BMP image;
        image.SetSize(WIDTH,HEIGHT);
        int x;
        for(x = 0; x < WIDTH; x++)
        {
                int y;
                for(y = 0; y < HEIGHT; y++)
                {
                        image(x,y)->Red   = OUT[x][y];
                        image(x,y)->Green = OUT[x][y];
                        image(x,y)->Blue  = OUT[x][y];
                        image(x,y)->Alpha = 0;
                }
        }
        image.WriteToFile("Output.bmp");
}
