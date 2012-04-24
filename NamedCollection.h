#pragma once

#include <string>
#include <vector>

template <class T>
class NamedCollection
{
public:
	NamedCollection(void) {}
	
	~NamedCollection(void) {}
	T operator[] (const int index)
	{
		return byIndex[index];
	}
	T operator[] (const std::string name)
	{

	}
	void Add(const std::string name, const T item)
	{
		names.push_back(name);
		byIndex.push_back(item);
	}
	void Remove(const T item)
	{
		for (int i = 0; i < byIndex.size(); ++i)
		{
			if (byIndex[i] == item)
			{
				byIndex.erase(byIndex.begin() + i);
				names.erase(names.begin() + i);
				return;
			}
		}
	}
	void Remove(const int index)
	{
		byIndex.erase(byIndex.begin() + index);
		names.erase(names.begin() + index);
	}
	void Remove(const std::string name)
	{
		for (int i = 0; i < byIndex.size(); ++i)
		{
			if (names[i] == name)
			{
				byIndex.erase(byIndex.begin() + i);
				names.erase(names.begin() + i);
			}
		}
	}
	int Count() { return byIndex.size(); }
private:	
	std::vector<std::string> names;
	std::vector<T> byIndex;
};

