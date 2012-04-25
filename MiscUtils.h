#ifndef MISCUTILS_H
#define MISCUTILS_H

#include <map>

template <typename T>
class ReadOnly {
private:
    T m_data;
public:
    ReadOnly() {}
    ReadOnly(const ReadOnly& other) {}
    ReadOnly(const T& data) : m_data(data) {}
    operator T&() { return this->m_data; }
};

class BitArray
{
public:
	BitArray(int size)
	{
		_size = size;
		dataSize = (_size / 32) + 1;
		data = (unsigned int*)malloc(sizeof(unsigned int) * dataSize);
		memset(data, 0, dataSize * sizeof(unsigned int));
	}
	void Set(int index, bool value)
	{
		int dataIndex = index / 32;
		int mask = 1 << (index % 32);
		data[dataIndex] |= mask;
	}
	bool operator[] (int index)
	{
		int dataIndex = index / 32;
		int mask = 1 << (index % 32);
		return data[dataIndex] & mask;
	}
	int size()
	{
		return _size;
	}
	void CopyTo(BitArray& other)
	{
		if (other.size() < _size)
			return;
	}
private:
	unsigned int* data;	
	int dataSize;
	int _size;
};

template <typename keyType, typename valueType>
class MapWrapper
{
private:
	std::map<keyType, valueType>& map;
	MapWrapper& operator= (const MapWrapper& other);
public:
	valueType& operator[] (keyType index)
	{
		return map[index];
	}
	valueType& operator[] (int index)
	{
		std::map<keyType, valueType>::iterator iter = map.begin();
		return (iter + index)->second;
	}
	int size()
	{
		return map.size();
	}
	MapWrapper(std::map<keyType, valueType>& map)
	 : map(map)
	{}
};

template <typename listType, typename valueType>
class ListWrapper
{
private:
	listType& list;
	ListWrapper& operator= (const ListWrapper& other) {}
public:
	valueType& operator[] (int index)
	{
		return list[index];
	}
	int size()
	{
		return list.size();
	}
	ListWrapper(listType& list)
		: list(list)
	{}
};

#endif