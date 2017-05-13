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

#include <thread>
using std::thread;

// Coordinate System:
//   0 1 2 3 - XPos - width
// 0 x x x x
// 1 x x x x
// 2 x x x x
// YPos - height

inline void borderLeft(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height, unsigned int posY)
{
	multiset<unsigned char> pixel;

	// | # #
	// | X #
	// | # #

	pixel.insert(input[0][posY - 1]);
	pixel.insert(input[0][posY + 1]);
	pixel.insert(input[1][posY - 1]);
	pixel.insert(input[1][posY]);
	pixel.insert(input[1][posY + 1]);

	output[0][posY] = *std::next(pixel.begin(), 2);
}

inline void borderRight(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height, unsigned int posY)
{
	multiset<unsigned char> pixel;

	// # # |
	// # X |
	// # # |

	pixel.insert(input[width - 1][posY - 1]);
	pixel.insert(input[width - 1][posY + 1]);
	pixel.insert(input[width - 2][posY - 1]);
	pixel.insert(input[width - 2][posY]);
	pixel.insert(input[width - 2][posY + 1]);

	output[width - 1][posY] = *std::next(pixel.begin(), 2);
}

inline void borderTop(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height, unsigned int posX)
{
	multiset<unsigned char> pixel;

	// -----
	// # X #
	// # # #

	pixel.insert(input[posX - 1][0]);
	pixel.insert(input[posX + 1][0]);
	pixel.insert(input[posX - 1][0]);
	pixel.insert(input[posX][1]);
	pixel.insert(input[posX + 1][1]);

	output[posX][0] = *std::next(pixel.begin(), 2);
}

inline void borderBottom(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height, unsigned int posX)
{
	multiset<unsigned char> pixel;

	// # # #
	// # X #
	// -----

	pixel.insert(input[posX - 1][height - 2]);
	pixel.insert(input[posX + 1][height - 2]);
	pixel.insert(input[posX - 1][height - 1]);
	pixel.insert(input[posX][height - 1]);
	pixel.insert(input[posX + 1][height - 1]);

	output[posX][height - 1] = *std::next(pixel.begin(), 2);
}

inline void cornerTopLeft(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height)
{
	multiset<unsigned char> pixel;

	// .----
	// | X #
	// | # #

	pixel.insert(input[0][1]);
	pixel.insert(input[1][0]);
	pixel.insert(input[1][1]);

	output[0][0] = *std::next(pixel.begin(), 1);
}

inline void cornerTopRight(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height)
{
	multiset<unsigned char> pixel;

	// ----.
	// # X |
	// # # |

	pixel.insert(input[width - 2][0]);
	pixel.insert(input[width - 1][1]);
	pixel.insert(input[width - 2][1]);

	output[width - 1][0] = *std::next(pixel.begin(), 1);
}

inline void cornerBottomLeft(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height)
{
	multiset<unsigned char> pixel;

	// | # #
	// | X #
	// .----

	pixel.insert(input[0][height - 2]);
	pixel.insert(input[1][height - 1]);
	pixel.insert(input[1][height - 2]);

	output[0][height - 1] = *std::next(pixel.begin(), 1);
}

inline void cornerBottomRight(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height)
{
	multiset<unsigned char> pixel;

	// # # |
	// # X |
	// ----.

	pixel.insert(input[width - 1][height - 2]);
	pixel.insert(input[width - 2][height - 1]);
	pixel.insert(input[width - 2][height - 2]);

	output[width - 1][height - 1] = *std::next(pixel.begin(), 1);
}

inline void mid(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height, unsigned int posX, unsigned int posY)
{
	multiset<unsigned char> pixel;

	// # # #
	// # X #
	// # # #

	pixel.insert(input[posX - 1][posY - 1]);
	pixel.insert(input[posX - 1][posY]);
	pixel.insert(input[posX - 1][posY + 1]);
	pixel.insert(input[posX][posY - 1]);
	pixel.insert(input[posX][posY]);
	pixel.insert(input[posX][posY + 1]);
	pixel.insert(input[posX + 1][posY - 1]);
	pixel.insert(input[posX + 1][posY]);
	pixel.insert(input[posX + 1][posY + 1]);

	output[posX][posY] = *std::next(pixel.begin(), 4);
}

void TopLine(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height)
{
	// First one is border left
	cornerTopLeft(input, output, width, height);

	// All in the middle
	for (unsigned i = 1; i < width - 1; i++)
		borderTop(input, output, width, height, i);

	// Last one is border right
	cornerTopRight(input, output, width, height);
}

void midLine(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height, unsigned int posY)
{
	// First one is border left
	borderLeft(input, output, width, height, posY);

	// All in the middle
	for (unsigned i = 1; i < width - 1; i++)
		mid(input, output, width, height, i, posY);

	// Last one is border right
	borderRight(input, output, width, height, posY);

}

void BottomLine(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height)
{
	// First one is border left
	cornerBottomLeft(input, output, width, height);

	// All in the middle
	for (unsigned i = 1; i < width - 1; i++)
		borderBottom(input, output, width, height, i);

	// Last one is border right
	cornerBottomRight(input, output, width, height);

}

void medianFilter(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height)
{
	thread t[HEIGHT];

	// Mid threads
	for (unsigned i = 1; i < height - 1; i++)
	{
		t[i] = thread(midLine, input, output, width, height, i);
	}

	// Top & Bottom threads
	t[height - 1] = thread(BottomLine, input, output, width, height);
	t[0] = thread(TopLine, input, output, width, height);

	// Wait for all threads
	for (unsigned i = 0; i < height; i++)
		t[i].join();
}

