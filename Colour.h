#ifndef COLOURS_H
#define COLOURS_H

#include <svl\SVL.h>
#include "MiscUtils.h"

class Colour
{
public:
	static ReadOnly<Vec4> Red;
	static ReadOnly<Vec4> Blue;
	static ReadOnly<Vec4> Green;
	static ReadOnly<Vec4> White;
	static ReadOnly<Vec4> Black;
	static ReadOnly<Vec4> Yellow;
	static ReadOnly<Vec4> Cyan;
	static ReadOnly<Vec4> Magenta;

	static void Copy3(float* arr, const Vec3& source)
	{
		arr[0] = source[0];
		arr[1] = source[1];
		arr[2] = source[2];
	}
	static void Copy3(float* arr, const Vec4& source)
	{
		arr[0] = source[0];
		arr[1] = source[1];
		arr[2] = source[2];
	}
	static void Copy4(float* arr, const Vec4& source)
	{
		arr[0] = source[0];
		arr[1] = source[1];
		arr[2] = source[2];
		arr[3] = source[3];
	}
	static Vec4 Lerp(Vec4 colour1, Vec4 colour2, float factor)
	{
		return Vec4(colour1 * factor + colour2 * (1.0f - factor));
	}
	static Vec3 Lerp(Vec3 colour1, Vec3 colour2, float factor)
	{
		return Vec3(colour1 * factor + colour2 * (1.0f - factor));
	}
	static Vec4 FromByte(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		return Vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	template <typename T>
	static T AsBytes(Vec4& colour)
	{
		T* retData;
		unsigned char byteData[4];
		for (int i = 0; i < 4; ++i)
		{
			byteData[i] = (unsigned char)colour[i] * 255;
		}
		retData = (T*)byteData;
		return (*retData);
	}
};

#endif