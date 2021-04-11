#include "widget.h"
#include <QApplication>
#include <vector>

bool CheckUpdate(void)
{
	std::string str = FileRead("UpdateList.json");
	CMvJson oldMj = CMvJson(str);
	std::string strUrl = oldMj["AutoUpdater"]["Updater"]["Url"]();
	CMvJson& oMvFiles = oldMj["AutoUpdater"]["Files"];

	if (strUrl.empty())
	{
		strUrl = "http://www.guxunw.com/stock/v2/";
	}
	std::string strFileName = "UpdateList.json";
	std::string strPath = strUrl + strFileName;
	HttpDownload(strPath.c_str(), "UpdateListTmp.json");

	std::vector<std::string> vecUpdateFileName;
	str = FileRead("UpdateListTmp.json");
	CMvJson newMj = CMvJson(str);
	CMvJson& oNewFiles = newMj["AutoUpdater"]["Files"];
	for (int i = 0; i < oNewFiles.size(); ++i)
	{
		bool needNotUpdate = false;
		std::string strName1 = oNewFiles[i]["Name"]();
		std::string strVer1 = oNewFiles[i]["Ver"]();
		for (int j = 0; j < oMvFiles.size(); ++j)
		{
			std::string strName2 = oMvFiles[j]["Name"]();
			std::string strVer2 = oMvFiles[j]["Ver"]();
			if (strName1 == strName2 && strVer1 == strVer2)
			{
				needNotUpdate = true;
				break;
			}
		}
		if (!needNotUpdate)
		{
			vecUpdateFileName.push_back(oNewFiles[i]["Name"]());
		}
	}
	if (vecUpdateFileName.size() > 0)
	{
		vecUpdateFileName.push_back("UpdateList.json");
		Widget::m_vecUpdateFileName = vecUpdateFileName;
		Widget::m_strPath = strUrl;
		return true;
	}
	return false;
}

int main(int argc, char *argv[])
{
	if (CheckUpdate())
	{

	}
	else
	{
		return 0;
	}
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
