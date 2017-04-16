#pragma once

#include "SharedPtr.h"

namespace X {

	class DataStream
	{
		DECLARE_REF_COUNTER();

	public:
		typedef SharedPtr<DataStream> Ptr;

	public:
		DataStream() {}
		DataStream(const String & source) : mSource(source) {}
		virtual ~DataStream() {}

		const String &
			GetSource() { return mSource; }

		virtual void
			Release() { delete this; }
		
		virtual DataStream *
			Clone() = 0;

		virtual bool
			IsOpen() = 0;
		virtual bool
			IsEOF() const = 0;
		virtual void
			Close() = 0;
		
		virtual int
			Read(void * data, int size) = 0;
		int
			ReadString(char * str, int buffSize, char end = '\0');

		virtual void
			Seek(int off, int orig) = 0;
		virtual int
			Tell() = 0;
		virtual int
			Size() const = 0;

		virtual void *
			GetData() = 0;

		template<class T>
		int
			ReadT(T & data) { return Read(&data, sizeof(T)); }

		template <class T> DataStream &
			operator >>(T & data) { ReadT(data); return *this; }

	protected:
		String mSource;
	};

	template<> 
	inline int DataStream::ReadT<String>(String & data)
	{
		char buff[1024];
		int count = ReadString(buff, 1024);
		data = buff;
		return count;
	}
	
	//
	class MemoryStream : public DataStream
	{
	public:
		typedef SharedPtr<MemoryStream> Ptr;

	public:
		MemoryStream(byte * data, int size, bool managed);
		MemoryStream(int size);
		virtual ~MemoryStream();

		virtual DataStream *
			Clone();

		virtual bool
			IsOpen();
		virtual bool
			IsEOF() const;
		virtual void
			Close();
		
		virtual int 
			Read(void * data, int size);

		virtual void
			Seek(int off, int orig);
		virtual int
			Tell();
		virtual int
			Size() const;

		virtual void *
			GetData();

	protected:
		int mSize;
		byte * mData;
		int mCursor;
		bool mManaged;
	};

	//
	class FileStream : public DataStream
	{
	public:
		typedef SharedPtr<FileStream> Ptr;

	public:
		FileStream(const String & file);
        FileStream(const String & name, FILE * fp, bool managed);
		virtual ~FileStream();

		virtual DataStream *
			Clone();
		
		virtual bool
			IsOpen();
		virtual bool
			IsEOF() const;
		virtual void
			Close();
		
		virtual int
			Read(void * data, int size);

		virtual void
			Seek(int off, int orig);
		virtual int
			Tell();
		virtual int
			Size() const;

		virtual void *
			GetData();

	protected:
		FILE * mFileHandle;
		char * mData;
        bool mManaged;
	};

}
