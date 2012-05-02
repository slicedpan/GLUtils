#ifndef FBOMANAGER_H
#define FBOMANAGER_H

#include "FrameBufferObject.h"
#include <map>

typedef std::map<std::string, FrameBufferObject*>::iterator FBOIterator;
typedef std::pair<std::string, FrameBufferObject*> FBOEntry;

class FBOManager
{
public:
	FrameBufferObject* operator[] (std::string name) { return FBOs[name]; }
	static FBOManager* GetSingletonPtr() 
	{ 
		if (!instance)
			instance = new FBOManager();
		return instance; 
	}
	static FBOManager& GetSingleton()
	{
			return *(GetSingletonPtr());
	}
	void AddFBO(FrameBufferObject* fbo)
	{
		FBOs.insert(FBOEntry(fbo->GetName(), fbo));	
	}
	bool Contains(std::string name)
	{
		return (FBOs.count(name) > 0);
	}
	void Clear()
	{
		FBOIterator it = FBOs.begin();
		while (it != FBOs.end())
		{
			delete it->second;
			++it;
		}
		FBOs.clear();
	}
	FBOTexture* GetTexture(unsigned int index)
	{
		FBOTexture* current;
		FBOIterator it = FBOs.begin();
		while (index >= 0)
		{
			if (it == FBOs.end())
				break;
			if (it->second->GetNumTextures() > index)
				return it->second->GetTexture(index);
			index -= it->second->GetNumTextures();
			++it;
		}
		return 0;
	}
private:
	std::map<std::string, FrameBufferObject*> FBOs;
	static FBOManager* instance;
};

#endif
