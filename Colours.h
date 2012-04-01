#include <svl\SVL.h>

class Colours
{
public:
	static const Vec3& Red() { return instance->red; }
	static const Vec3& Blue() { return instance->blue; }
	static const Vec3& Green() { return instance->green; }
	static const Vec3& White() { return instance->white; }
	static const Vec3& Black() { return instance->black; }
	static const Vec3& Purple() { return instance->purple; }
	static const Vec3& Yellow() { return instance->yellow; }
	static const Vec3& Cyan() { return instance->cyan; }
	Colours()
	{
		red = Vec3(1.0f, 0.0f, 0.0f);
		blue = Vec3(0.0f, 0.0f, 1.0f);
		white = Vec3(1.0f, 1.0f, 1.0f);
		black = Vec3(0.0f, 0.0f, 0.0f);
		green = Vec3(0.0f, 1.0f, 0.0f);
		purple = Vec3(1.0f, 0.0f, 1.0f);
		yellow = Vec3(1.0f, 1.0f, 0.0f);
		cyan = Vec3(0.0f, 1.0f, 1.0f);
	}
	static void Copy(float* arr, const Vec3& source)
	{
		arr[0] = source[0];
		arr[1] = source[1];
		arr[2] = source[2];
	}
private:
	static Colours* instance;
	Vec3 red;
	Vec3 blue;
	Vec3 green;
	Vec3 purple;
	Vec3 yellow;
	Vec3 black;
	Vec3 white;
	Vec3 cyan;
};