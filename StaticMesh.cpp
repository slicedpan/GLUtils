#include "StaticMesh.h"
#include "ObjLoader\objLoader.h"

StaticMesh::StaticMesh()
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
		SubMesh* subMesh = new SubMesh();
		subMesh->LoadObj(obj, smoothNormals, generateNormals);
		SubMeshes.Add("submesh0", subMesh);
		return;
	}
}
