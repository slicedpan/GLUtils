#pragma once
#include <map>
#include <string>
#include <vector>
#include "SubMesh.h"
#include "NamedCollection.h"

class StaticMesh
{
private:
	class meshCollection : public NamedCollection<SubMesh*>
	{
	};	//this will prevnt people from reassigning SubMeshes (maybe)

public:
	StaticMesh();
	void LoadObj(std::string filename, bool smoothNormals, bool generateNormals, bool loadMaterial = true);
	~StaticMesh(void);
	meshCollection SubMeshes;
	char* filename;
};

