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
	BitArray(const BitArray& other)
	{
		_size = other._size;
		dataSize = (_size / 32) + 1;
		data = (unsigned int*)malloc(sizeof(unsigned int) * dataSize);
		CopyFrom(other);
	}
	~BitArray()
	{
		free(data);
	}
	void Set(int index, bool value)
	{
		int dataIndex = index / 32;
		int mask = 1 << (index % 32);
		if (value)
			data[dataIndex] |= mask;
		if (!value && data[dataIndex])
			data[dataIndex] ^= mask;
	}
	const bool operator[] (int index)
	{
		int dataIndex = index / 32;
		int mask = 1 << (index % 32);
		return (data[dataIndex] & mask);
	}
	const int size()
	{
		return _size;
	}
	const void CopyTo(BitArray& other)
	{
		if (other.size() < _size)
			return;
		if (other.size() == _size || _size % 32 == 0)
			memcpy(other.data, data, dataSize * sizeof(unsigned int));
		//TODO copy to a larger array, without overwriting entries
	}
	void CopyFrom (const BitArray& other)
	{
		if (other._size == _size)
			memcpy(data, other.data, dataSize * sizeof(unsigned int));
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
		for (int i = 0; i < index; ++i)
			++iter;
		return iter->second;
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
	valueType operator[] (int index)
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
	operator const listType& () 
	{
		return list;
	}
};

#endif