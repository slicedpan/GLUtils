#include "StaticMesh.h"
#include "ObjLoader\objLoader.h"

StaticMesh::StaticMesh()
	: SubMeshes(subMeshes)
{
}

StaticMesh::~StaticMesh(void)
{
}

void StaticMesh::LoadObj(std::string filename, bool smoothNormals, bool generateNormals, bool loadMaterial)
{
	objLoader* obj = new objLoader();
	obj->load(filename.c_str());

	if (loadMaterial == false || obj->materialCount <= 1)
	{
		SubMesh subMesh;
		subMesh.LoadObj(obj, smoothNormals, generateNormals);
		subMeshes.insert(meshEntry("submesh0", subMesh));		
		return;
	}
}
