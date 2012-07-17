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

	std::string cachedName = filename + ".cache";
	FILE* cached = fopen(cachedName.c_str(), "rb");

	bool readFromCache = false;
	MeshCacheInfo mci;

	if (cached)
	{
		size_t fileSize;
		fseek(cached, 0L, SEEK_END);
		fileSize = ftell(cached);
		fseek(cached, 0L, SEEK_SET);
		if (fileSize > sizeof(int))
		{	
			fread(&mci.numSubMeshes, sizeof(int), 1, cached);
			if (fileSize > sizeof(int) + mci.numSubMeshes * sizeof(size_t))
			{
				mci.subMeshSize = (size_t*)malloc(sizeof(size_t) * mci.numSubMeshes);
				fread(mci.subMeshSize, sizeof(size_t), mci.numSubMeshes, cached);
				size_t subMeshDataSize = 0;
				for (int i = 0; i < mci.numSubMeshes; ++i)
				{
					subMeshDataSize += mci.subMeshSize[i];
				}
				subMeshDataSize += sizeof(int) + mci.numSubMeshes * sizeof(size_t);
				if (fileSize == subMeshDataSize)
					readFromCache = true;
			}
		}
	}	

	if (!readFromCache)
	{
		if (cached)
		{
			fclose(cached);
			cached = 0;
		}
		objLoader* obj = new objLoader();
		printf("Parsing file: %s...", filename.c_str());
		obj->load(filename.c_str());
		printf(" done.\n");

		if (loadMaterial == false || obj->materialCount <= 1)
		{
			SubMesh subMesh;
			subMesh.LoadObj(obj, smoothNormals, generateNormals);
			subMeshes.insert(meshEntry("submesh0", subMesh));			
		}
		else
			return;	//need support for multiple materials etc.

		printf("Caching...");

		cached = fopen(cachedName.c_str(), "wb");
		MeshCacheInfo mci;
		mci.numSubMeshes = subMeshes.size();
		mci.subMeshSize = (size_t*)malloc(sizeof(size_t) * mci.numSubMeshes);
		std::vector<SubMeshBlock> smbs;
		std::map<std::string, SubMesh>::iterator iter;

		int counter = 0;
	
		for (iter = subMeshes.begin(); iter != subMeshes.end(); ++iter)
		{			
			smbs.push_back(iter->second.GenerateCache());
			mci.subMeshSize[counter] = smbs[counter].size;
			++counter;
		}

		fwrite(&mci.numSubMeshes, sizeof(int), 1, cached);
		fwrite(mci.subMeshSize, sizeof(size_t), mci.numSubMeshes, cached);

		long bytePosition = ftell(cached);

		for (int i = 0; i < mci.numSubMeshes; ++i)
		{
			fwrite(smbs[i].data, smbs[i].size, 1, cached);
		}

		SubMeshBlock* smb = &smbs[0];

		bytePosition = ftell(cached);
		
		fclose(cached);

		printf(" done.\n");

	}
	else
	{

		printf("Loading from cached file: %s...\n", cachedName.c_str());
		
		for (int i = 0; i < mci.numSubMeshes; ++i)
		{
			unsigned char* data = (unsigned char*)malloc(mci.subMeshSize[i]);
			long bytePosition = ftell(cached);
			size_t bytesRead = fread(data, sizeof(unsigned char), mci.subMeshSize[i], cached);	
			bytePosition = ftell(cached);
			
			SubMesh subMesh;
			subMesh.LoadCached(data);
			char name[16];
			sprintf(name, "submesh%d", i);
			subMeshes.insert(meshEntry(name, subMesh));
			free(data);
		}

		fclose(cached);

	}
}
