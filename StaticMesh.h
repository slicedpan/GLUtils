#ifndef STATICMESH_H
#define STATICMESH_H

#include <map>
#include <string>
#include <vector>
#include "SubMesh.h"
#include "NamedCollection.h"
#include "MiscUtils.h"

typedef std::pair<std::string, SubMesh> meshEntry;

class StaticMesh
{
private:
	std::map<std::string, SubMesh> subMeshes;
public:
	StaticMesh();
	void LoadObj(std::string filename, bool smoothNormals, bool generateNormals, bool loadMaterial = true);
	~StaticMesh(void);
	MapWrapper<std::string, SubMesh> SubMeshes;
	char* filename;
};

struct MeshCacheInfo
{
	size_t headerSize;
	int numSubMeshes;
	size_t* subMeshSize;
};

#endif

