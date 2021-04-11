#include "HttpDownload.h"

// ConsoleApplication2.cpp : 定义控制台应用程序的入口点。
//

//HTTP DownloadToFile
//特别注意：引用头文件和lib库  不然会报错和编译不过
#include <tchar.h>
#include <string>
#include <iostream>
#pragma comment(lib,"urlmon.lib")
#pragma comment(lib,"ws2_32.lib")
#include <afxinet.h>


using namespace std;


int UrlEncodeUtf8(LPCSTR pszUrl, LPSTR pszEncode, int nEncodeLen)
{
	int nRes = 0;
	wchar_t* pWString = NULL;
	char* pString = NULL;
	char* pResult = NULL;

	do
	{
		if (pszUrl == NULL)
			break;

		int nLength = MultiByteToWideChar(CP_ACP, 0, pszUrl, -1, NULL, 0);

		pWString = new wchar_t[nLength + 1];
		if (pWString == NULL)
			break;

		memset(pWString, 0, (nLength + 1)*sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, pszUrl, -1, pWString, nLength);

		nLength = WideCharToMultiByte(CP_UTF8, 0, pWString, -1, NULL, 0, NULL, NULL);
		pString = new char[nLength + 1];
		if (pString == NULL)
			break;

		memset(pString, 0, nLength + 1);
		nLength = WideCharToMultiByte(CP_UTF8, 0, pWString, -1, pString, nLength, NULL, NULL);

		pResult = new char[nLength * 2];
		if (pResult == NULL)
			break;

		memset(pResult, 0, nLength * 2);
		char* pTmp = pResult;
		static char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

		for (int i = 0; i < nLength; i++)
		{
			unsigned char c = pString[i];
			if (c == 0)
			{
				break;
			}

			if (c > 0x20 && c < 0x7f)
			{
				*pTmp++ = c;
			}
			else if (c == 0x20)
			{
				*pTmp++ = '%';
				*pTmp++ = hex[c / 16];
				*pTmp++ = hex[c % 16];
			}
			else
			{
				*pTmp++ = '%';
				*pTmp++ = hex[c / 16];
				*pTmp++ = hex[c % 16];
			}
		}
		nLength = strlen(pResult);
		nRes = nLength;
		if (pszEncode == NULL || nEncodeLen < nLength)
			break;

		memcpy(pszEncode, pResult, nLength);
	} while (0);

	if (pWString != NULL)
		delete[]pWString;

	if (pString != NULL)
		delete[]pString;

	if (pResult != NULL)
		delete[]pResult;

	return nRes;
}

int UrlEncodeUtf8(LPCWSTR pszUrl, LPWSTR pszEncode, int nEncodeLen)
{
	int nRes = 0;
	const int nBufferSize = 4096;
	char* pStringA = new char[nBufferSize];
	char* pCodeA = new char[nBufferSize];
	wchar_t* pCodeW = new wchar_t[nBufferSize];

	do
	{
		if (pszUrl == NULL)
			break;

		if (pStringA == NULL || pCodeA == NULL || pCodeW == NULL)
			break;

		memset(pStringA, 0, nBufferSize);
		memset(pCodeA, 0, nBufferSize);
		memset(pCodeW, 0, nBufferSize * 2);

		WideCharToMultiByte(CP_ACP, 0, pszUrl, -1, pStringA, nBufferSize, 0, 0);
		nRes = UrlEncodeUtf8(pStringA, pCodeA, nBufferSize);
		if (nRes == 0)
			break;

		nRes = MultiByteToWideChar(CP_ACP, 0, pCodeA, -1, pCodeW, nBufferSize);
		if (nRes >= nEncodeLen)
			break;

		if (pszEncode != NULL)
		{
			memset(pszEncode, 0, nEncodeLen * 2);
			memcpy(pszEncode, pCodeW, nRes * 2);
		}
	} while (0);

	if (pStringA != NULL)
		delete[]pStringA;

	if (pCodeA != NULL)
		delete[]pCodeA;

	if (pCodeW != NULL)
		delete[]pCodeW;

	return nRes;
}

static std::string GetRand(void)
{
	int nRand = time(NULL); // 用时间作为随机数

	CString szUrl;
	szUrl.Format(L"?abc=%d", nRand); // 生成随机URL
	return (char*)szUrl.GetBuffer(0);
}

int HttpDownload(const std::string& strPath, const std::string& strFileName)
{
	std::string strPathRand = strPath + GetRand();
	CString strURL = strPath.c_str();
	const int nBufferSize = 4096;
	TCHAR szURL[nBufferSize] = { 0 };
	UrlEncodeUtf8(strURL, szURL, nBufferSize);
	strURL = szURL;

	string url = CW2A(strURL.GetString());

	size_t len = url.length();//获取字符串长度
	int nmlen = MultiByteToWideChar(CP_ACP, 0, url.c_str(), len + 1, NULL, 0);//如果函数运行成功，并且cchWideChar为零，
	//返回值是接收到待转换字符串的缓冲区所需求的宽字符数大小。
	wchar_t* buffer = new wchar_t[nmlen];

	MultiByteToWideChar(CP_ACP, 0, url.c_str(), len + 1, buffer, nmlen);
	DeleteUrlCacheEntry(buffer);
	HRESULT hr = URLDownloadToFile(NULL, buffer, CA2W(strFileName.c_str()), 0, NULL);
	if (hr == S_OK)
	{
		cout << "Download File Is Success" << endl;
	}
	return 0;
}

std::string FileRead(const std::string& file)
{
	std::string strRes;
	const int MAX_LINE = 1024 * 10;
	char buf[MAX_LINE] = { 0 };
	FILE *fp;
	int len;
	if ((fp = fopen(file.c_str(), "r")) == NULL)
	{
		perror("fail to read");
		return strRes;
	}
	while (fgets(buf, MAX_LINE, fp) != NULL)
	{
		len = strlen(buf);
		//buf[len - 1] = '\0'; 
		strRes += buf;
	}
	fclose(fp);
	return strRes;
}