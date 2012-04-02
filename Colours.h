#include <svl\SVL.h>

class Colours
{
public:
	static const Vec4& Red() { return instance->red; }
	static const Vec4& Blue() { return instance->blue; }
	static const Vec4& Green() { return instance->green; }
	static const Vec4& White() { return instance->white; }
	static const Vec4& Black() { return instance->black; }
	static const Vec4& Purple() { return instance->purple; }
	static const Vec4& Yellow() { return instance->yellow; }
	static const Vec4& Cyan() { return instance->cyan; }
	Colours()
	{
		red = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
		blue = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
		white = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
		black = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		green = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
		purple = Vec4(1.0f, 0.0f, 1.0f, 1.0f);
		yellow = Vec4(1.0f, 1.0f, 0.0f, 1.0f);
		cyan = Vec4(0.0f, 1.0f, 1.0f, 1.0f);
	}
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
private:
	static Colours* instance;
	Vec4 red;
	Vec4 blue;
	Vec4 green;
	Vec4 purple;
	Vec4 yellow;
	Vec4 black;
	Vec4 white;
	Vec4 cyan;
};