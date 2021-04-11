#ifndef __HTTP_DOWNLOAD__
#define __HTTP_DOWNLOAD__

#include <afx.h>
#include <string>
#include <windows.h>


int HttpDownload(const std::string& strPath, const std::string& strFileName);

std::string FileRead(const std::string& file);

#endif