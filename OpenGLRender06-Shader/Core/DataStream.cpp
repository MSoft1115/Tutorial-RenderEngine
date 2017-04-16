#include "DataStream.h"

namespace X {

	int DataStream::ReadString(char * str, int buffSize, char end)
	{
		const int TEMP_SIZE = 128;
		char tmpBuf[TEMP_SIZE] = { 0 };
		int count = 0;
		bool flag = false;

		buffSize -= 1;
		while (!flag && count < buffSize && !IsEOF())
		{
			int nreads = Read(tmpBuf, TEMP_SIZE);

			int i = 0;
			while (i < nreads && count < buffSize)
			{
				if (tmpBuf[i] == end || tmpBuf[i] == 0)
				{
					flag = true;
					i += 1;
					break;
				}

				str[count++] = tmpBuf[i++];
			}

			if (i < nreads)
			{
				Seek(-(nreads - i), SEEK_CUR);
			}
		}

		str[count] = 0;

		return count;
	}

	//
	MemoryStream::MemoryStream(byte * data, int size, bool managed)
		: mSize(size)
		, mCursor(0)
		, mManaged(managed)
	{
		d_assert(data != NULL && size > 0);
		mData = data;
	}

	MemoryStream::MemoryStream(int size)
		: mSize(size)
		, mCursor(0)
		, mManaged(true)
	{
		mData = new byte[size + 1];
		mData[mSize] = 0;
	}

	MemoryStream::~MemoryStream()
	{
		Close();
	}

	DataStream * MemoryStream::Clone()
	{
		MemoryStream * p = new MemoryStream(mSize);

		memcpy(p->mData, mData, mSize);

		return p;
	}

	bool MemoryStream::IsOpen()
	{
		return true;
	}

	void MemoryStream::Close()
	{
		if (mManaged)
			safe_delete_array(mData);

		mData = NULL;
		mSize = 0;
		mCursor = 0;
	}

	void * MemoryStream::GetData()
	{
		return mData;
	}

	int MemoryStream::Read(void * data, int size)
	{
		d_assert(size > 0);

		if (!IsEOF())
		{
			if (size > mSize - mCursor)
				size = mSize - mCursor;

			memcpy(data, &mData[mCursor], size);

			mCursor += size;

			return size;
		}

		return 0;
	}

	void MemoryStream::Seek(int off, int orig)
	{
		switch (orig)
		{
			case SEEK_CUR:
				mCursor += off;
				break;

			case SEEK_SET:
				mCursor = off;
				break;

			case SEEK_END:
				mCursor = mSize - off;
				break;
		}

		mCursor = clamp(mCursor, 0, Size());
	}

	int MemoryStream::Tell()
	{
		return mCursor;
	}

	int MemoryStream::Size() const
	{
		return mSize;
	}

	bool MemoryStream::IsEOF() const
	{
		return mSize - mCursor <= 0;
	}

	//
	FileStream::FileStream(const String & file)
		: DataStream(file)
	{
		mData = NULL;
		mFileHandle = fopen(file.c_str(), "rb");
		mManaged = true;
	}

	FileStream::FileStream(const String & name, FILE * fp, bool managed)
		: DataStream(name)
	{
		mData = NULL;
		mFileHandle = fp;
		mManaged = managed;
	}

	FileStream::~FileStream()
	{
		Close();
	}

	DataStream * FileStream::Clone()
	{
		return new FileStream(mSource);
	}

	bool FileStream::IsOpen()
	{
		return mFileHandle != NULL;
	}

	void FileStream::Close()
	{
		if (mFileHandle)
		{
			if (mManaged)
				fclose(mFileHandle);

			mFileHandle = NULL;
		}

		safe_delete_array(mData);
	}

	int FileStream::Read(void * data, int size)
	{
		if (mFileHandle)
			return fread(data, 1, size, mFileHandle);

		return 0;
	}

	void FileStream::Seek(int off, int orig)
	{
		if (mFileHandle)
			fseek(mFileHandle, off, orig);
	}

	int FileStream::Tell()
	{
		if (mFileHandle)
			return ftell(mFileHandle);

		return 0;
	}

	bool FileStream::IsEOF() const
	{
		return mFileHandle == NULL || feof(mFileHandle) != 0;
	}

	void * FileStream::GetData()
	{
		if (!mData)
		{
			int size = Size();
			mData = new char[size + 1];
			Read(mData, size);
			mData[size] = 0;
		}

		return mData;
	}

	int FileStream::Size() const
	{
		FILE * fp = mFileHandle;
		if (fp)
		{
			int size = 0;
			int cur = ftell(fp);

			fseek(fp, 0, SEEK_END);
			size = ftell(fp);
			fseek(fp, cur, SEEK_SET);

			return size;
		}

		return 0;
	}
}
