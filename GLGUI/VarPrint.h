
#include <string>
#include <sstream>
#include <svl\svl.h>
#include <cstdio>

inline std::string FormatFloat(float value, int numDigits)
{
	std::ostringstream buf;
	buf.precision(numDigits);
	buf.setf(std::ios::fixed, std::ios::floatfield);
	buf << value;
	if (buf.tellp() >= numDigits)
	{
		buf.seekp(8, std::ios_base::beg);
		buf.put(0);
	}
	return std::string(buf.str().c_str());
}

template<typename T>
class IVarPrint
{
public:
	virtual std::string Print(T* value) = 0;
	virtual int MaxChars() = 0;
};

template<typename T>
class VarPrint : public IVarPrint<T>
{
private: 
	VarPrint() {}	//this should never be instantiated
public:
	std::string Print(T* value)
	{
		throw;	//this should (hopefully) never happen
	}
	int MaxChars()
	{
		throw;	//ditto
	}
};

template<>
class VarPrint<float> : public IVarPrint<float>
{
public:
	VarPrint()
	{
		buf.setf(std::ios::fixed, std::ios::floatfield);
		buf.precision(8);
	}
	std::string Print(float * value)
	{
		buf.str("");
		buf << *(value);
		buf.seekp(0, std::ios_base::end);
		if (buf.tellp() >= 8)
		{
			buf.seekp(8, std::ios_base::beg);
			buf.put(0);
		}
		return buf.str();
	}
	int MaxChars() { return 8; }
private:
	std::ostringstream buf;
};

template<>
class VarPrint<int> : public IVarPrint<int>
{
public:
	VarPrint() {}
	std::string Print(int * value)
	{
		buf.str("");
		buf << *(value);
		return buf.str();
	}
	int MaxChars() { return 8; }
private:
	std::ostringstream buf;
};

template<>
class VarPrint<std::string> : public IVarPrint<std::string>
{
public:
	VarPrint() {}
	std::string Print(std::string * value)
	{
		return *value;
	}
	int MaxChars() { return -1; }
};

template<>
class VarPrint<char*> : public IVarPrint<char*>
{
public:
	VarPrint() {}
	std::string Print(char** value)
	{
		return std::string(*(value));
	}
	int MaxChars() { return -1; }
};

template<>
class VarPrint<Vec2> : public IVarPrint<Vec2>
{
public:
	VarPrint() 
	{
		buf.setf(std::ios::scientific, std::ios::floatfield);
		buf.precision(8);
	}
	std::string Print(Vec2* value)
	{
		std::string retStr;
		float* ptr = value->Ref();
		retStr = "(";
		for (int i = 0; i < value->Elts(); ++i)
		{
			buf.str("");
			buf << *(ptr + i);
			buf.seekp(0, std::ios_base::end);
			if (buf.tellp() >= 8)
			{
				buf.seekp(8, std::ios_base::beg);
				buf.put(0);
			}
			retStr.append(buf.str().c_str());
			if (i + 1 != value->Elts())
				retStr.append(", ");
		}
		retStr.append(")");
		return retStr;
	}
	int MaxChars() { return 20; }
private:
	std::ostringstream buf;
};

template<>
class VarPrint<Vec3> : public IVarPrint<Vec3>
{
public:
	VarPrint() 
	{
		buf.setf(std::ios::fixed, std::ios::floatfield);
		buf.precision(8);
	}
	std::string Print(Vec3* value)
	{
		std::string retStr;
		float* ptr = value->Ref();
		retStr = "(";
		for (int i = 0; i < value->Elts(); ++i)
		{
			buf.str("");
			buf << *(ptr + i);
			buf.seekp(0, std::ios_base::end);
			if (buf.tellp() >= 8)
			{
				buf.seekp(8, std::ios_base::beg);
				buf.put(0);
			}
			retStr.append(buf.str().c_str());
			if (i + 1 != value->Elts())
				retStr.append(", ");
		}
		retStr.append(")");
		return retStr;
	}
	int MaxChars() { return 30; }
private:
	std::ostringstream buf;
};

template<>
class VarPrint<Vec4> : public IVarPrint<Vec4>
{
public:
	VarPrint() 
	{
		buf.setf(std::ios::fixed, std::ios::floatfield);
		buf.precision(8);
	}
	std::string Print(Vec4* value)
	{
		std::string retStr;
		float* ptr = value->Ref();
		retStr = "(";
		for (int i = 0; i < value->Elts(); ++i)
		{
			buf.str("");
			buf << *(ptr + i);
			buf.seekp(0, std::ios_base::end);
			if (buf.tellp() >= 8)
			{
				buf.seekp(8, std::ios_base::beg);
				buf.put(0);
			}
			retStr.append(buf.str().c_str());
			if (i + 1 != value->Elts())
				retStr.append(", ");
		}
		retStr.append(")");
		return retStr;
	}
	int MaxChars() { return 40; }
private:
	std::ostringstream buf;
};