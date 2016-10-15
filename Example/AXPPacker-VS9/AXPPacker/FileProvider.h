
#ifndef _FILE_PROVIDER_110809_H_
#define _FILE_PROVIDER_110809_H_


//--------------------------------------------------------------------------------------------------------
// 利用 C 的基本文件操作来处理资源
class CFileProvider
{
	CFileProvider(const CFileProvider& );
	CFileProvider& operator = (const CFileProvider&);
	void CloseFile();
public:
	CFileProvider() : mhFile(0), mSize(0){}
	~CFileProvider();

	bool Is_Open()const{return (0 != mhFile);}							// true : 有效的
	bool OpenResourceProvider(const char* pzfile);						// 打开资源, 构建索引表

	bool GetDataBuffer(void*& pBf, size_t fsize, size_t nOffset);		// 读取数据, 外部给定内存空间
	bool GetDataBuffer(void*& pBf, size_t fsize, size_t nOffset, DWORD);// 读取数据
	void CloseDataBuffer(void*& pBf){if(pBf){delete [] pBf; pBf = 0;}}	// 销毁数据
	size_t GetBufferSzie()const{return mSize;}							// 对应Buffer的大小
	const char* GetResFile()const{return mFile.c_str();}				// !!! 必须返回有效地址 !!!

private:
	DWORD		mSize;		// 文件大小
	FILE*		mhFile;		// 文件指针
	std::string	mFile;		// 记录下来方便写Log
};

#endif