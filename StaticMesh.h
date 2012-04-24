#pragma once
#include <map>
#include <string>
#include <vector>
#include "SubMesh.h"
#include "NamedCollection.h"

template <typename T>
class readonly {
private:
    T m_data;
public:
	readonly() {}
	readonly(const readonly& other) {}
    readonly(const T& data) : m_data(data) {}
    operator T&() { return this->m_data; } 
};

class StaticMesh
{
private:
	class meshCollection : public NamedCollection<SubMesh*>
	{
		meshCollection& operator=(const meshCollection& rhs) {}
	};	//this will prevent people from reassigning SubMeshes (maybe)
public:
	StaticMesh();
	void LoadObj(std::string filename, bool smoothNormals, bool generateNormals, bool loadMaterial = true);
	~StaticMesh(void);
	meshCollection SubMeshes;
	char* filename;
};

