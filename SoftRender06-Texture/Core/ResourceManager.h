#pragma once

#include "Singleton.h"
#include "DataStream.h"

namespace X {

	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		ResourceManager();
		~ResourceManager();

		void
			AddFloder(const String & floder);

		virtual DataStream::Ptr 
			OpenResource(const String & source);
		virtual DataStream::Ptr
			OpenFile(const String & filename);
		virtual bool
			IsExist(const String & source);

	protected:
		std::vector<String> mResourceFloders;
	};

}
