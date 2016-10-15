
#ifndef _FILE_PROVIDER_110809_H_
#define _FILE_PROVIDER_110809_H_


//--------------------------------------------------------------------------------------------------------
// ���� C �Ļ����ļ�������������Դ
class CFileProvider
{
	CFileProvider(const CFileProvider& );
	CFileProvider& operator = (const CFileProvider&);
	void CloseFile();
public:
	CFileProvider() : mhFile(0), mSize(0){}
	~CFileProvider();

	bool Is_Open()const{return (0 != mhFile);}							// true : ��Ч��
	bool OpenResourceProvider(const char* pzfile);						// ����Դ, ����������

	bool GetDataBuffer(void*& pBf, size_t fsize, size_t nOffset);		// ��ȡ����, �ⲿ�����ڴ�ռ�
	bool GetDataBuffer(void*& pBf, size_t fsize, size_t nOffset, DWORD);// ��ȡ����
	void CloseDataBuffer(void*& pBf){if(pBf){delete [] pBf; pBf = 0;}}	// ��������
	size_t GetBufferSzie()const{return mSize;}							// ��ӦBuffer�Ĵ�С
	const char* GetResFile()const{return mFile.c_str();}				// !!! ���뷵����Ч��ַ !!!

private:
	DWORD		mSize;		// �ļ���С
	FILE*		mhFile;		// �ļ�ָ��
	std::string	mFile;		// ��¼��������дLog
};

#endif