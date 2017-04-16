#include "ResourceManager.h"
#ifdef _MSC_VER
#include <io.h>
#else
#include<unistd.h>
#include <sys/stat.h>
#endif

inline bool _FileExist(const X::String & file)
{
	return _access(file.c_str(), 0) != -1;
}

namespace X {

	template<> ResourceManager * Singleton<ResourceManager>::msInstance = NULL;

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
	}

	void ResourceManager::AddFloder(const String & floder)
	{
		mResourceFloders.push_back(floder);
	}

	DataStream::Ptr ResourceManager::OpenResource(const String & source)
	{
		DataStream::Ptr stream = NULL;

		for (int i = 0; i < (int)mResourceFloders.size(); ++i)
		{
			String path = mResourceFloders[i] + "/" + source;
			if (_FileExist(path))
			{
				stream = new FileStream(path);
				break;
			}
		}

		return stream;
	}

	DataStream::Ptr ResourceManager::OpenFile(const String & filename)
	{
		if (_FileExist(filename))
			return DataStream::Ptr(new FileStream(filename));
		else
			return NULL;
	}

	bool ResourceManager::IsExist(const String & source)
	{
		for (int i = 0; i < (int)mResourceFloders.size(); ++i)
		{
			String path = mResourceFloders[i] + "/" + source;
			if (_FileExist(path))
			{
				return true;
			}
		}

		return false;
	}

}
