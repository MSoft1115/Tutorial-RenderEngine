#pragma once

#include "XCore.h"
#include "Image.h"

namespace X {

	class Texture
	{
		DECLARE_REF_COUNTER();

	public:
		typedef SharedPtr<Texture> Ptr;

	public:
		Texture(const String & name, const String & source);
		virtual ~Texture();

		virtual void
			Release();

		virtual void *
			Lock(int flag) = 0;
		virtual void
			Unlock() = 0;

		const String &
			GetName() const { return mName;  }
		const String &
			GetSource() const { return mSource;  }
		int
			GetFormat() const { return mFormat; }
		int
			GetWidth() const { return mWidth; }
		int
			GetHeight() const { return mHeight; }
		int
			GetMipmaps() const { return mMipmaps; }

	protected:
		String mName;
		String mSource;
		int mFormat;
		int mWidth;
		int mHeight;
		int mMipmaps;
	};
}