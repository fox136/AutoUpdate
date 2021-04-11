#include "widget.h"
#include "ui_widget.h"
#include <string>
#include <QTimer> //瀹氭椂鍣ㄧ被
#include <QMouseEvent>
#include <QDebug>
#include <QLabel>
#include <QPixmap>
#include <QMainWindow>
#include <QPainter> //鐢诲锟?
#include <fcntl.h>
#include <iostream>
#include <cassert>
#include <tchar.h>
#include <string>
#include <iostream>
#include <QProcess>
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup" )
#include "mv_json.h"  // json瑙ｆ瀽锟?

#pragma comment(lib,"urlmon.lib")
#pragma comment(lib,"ws2_32.lib")

std::vector<std::string> Widget::m_vecUpdateFileName;
std::string Widget::m_strPath;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
	
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(192, 253, 123));
	palette.setBrush(QPalette::Background, QBrush(QPixmap(":/AutoUpdate/1.png")));
	this->setPalette(palette);
	this->resize(641, 416);

	connect(this, SIGNAL(clicked()), this, SLOT(mouseClicked()));

	m_strCurrentImg = "1";

	this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);	//灞忚斀鏍囬锟?
	this->setProperty("canMove", true);	//璁剧疆鑷畾涔夊睘鎬у彲绉诲姩

	QWidget::setWindowTitle("AutoUpdate.exe");
	QWidget::setWindowIcon(QIcon(":/AutoUpdate/red.ico"));
}

void Widget::mouseClicked()
{
	//澶勭悊浠ｇ爜  
}

void Widget::ChangeBgImg(const std::string& strName)
{
	std::string str = std::string(":/AutoUpdate/") + strName;
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QPixmap(str.c_str())));
	this->setPalette(palette);
	this->resize(QPixmap(str.c_str()).size());
	QWidget::repaint();
}

static char* Utf8ToGb2312(const char *p){
	DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, p, -1, NULL, 0);
	char *psText;
	wchar_t *pwText = (wchar_t*)malloc(dwNum*sizeof(wchar_t));
	dwNum = MultiByteToWideChar(CP_UTF8, 0, p, -1, pwText, dwNum);
	dwNum = WideCharToMultiByte(CP_ACP, 0, pwText, -1, NULL, 0, NULL, NULL);
	psText = (char*)malloc(dwNum*sizeof(char));
	dwNum = WideCharToMultiByte(CP_ACP, 0, pwText, -1, psText, dwNum, NULL, NULL);
	free(pwText);
	return psText;
}

void Widget::GengXinWenJian(void)
{
	for (int i = 0; i < m_vecUpdateFileName.size(); ++i)
	{
		std::string& strFileName = m_vecUpdateFileName[i];
		std::string& strPath = m_strPath + strFileName;
		HttpDownload(Utf8ToGb2312(strPath.c_str()), Utf8ToGb2312(strFileName.c_str()));
		m_nProgress = i;
		QWidget::repaint();
	}
}

void Widget::mousePressEvent(QMouseEvent *event)
{
	int x = event->x();
	int y = event->y();
	if (m_strCurrentImg == "1")
	{
		if (x > 220 && x < 425 && y > 350 && y < 400)
		{
			system(("taskkill /F /im 股讯大数据.exe   /fi \"pid gt 0\""));
			m_strCurrentImg = "2";
			ChangeBgImg("2.png");
			GengXinWenJian();
			m_strCurrentImg = "3";
			ChangeBgImg("3.png");
		}
	}
	else if (m_strCurrentImg == "3")
	{
		if (x > 220 && x < 425 && y > 350 && y < 400)
		{
			system("start /b 股讯大数据.exe");
			exit(0);
		}
	}
	if (y > 10 && y < 35 && x > 600 && x < 625)
	{
		exit(0);
	}
	qDebug() << tr("%1,%2").arg(event->pos().x()).arg(event->pos().y());
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
	if (mousePos == QPoint(event->x(), event->y())) emit clicked();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
	if (m_strCurrentImg != "2")
	{
		return;
	}
	QPainter painter(this);

	QPen pen(QColor(255,0,0));

	pen.setWidth(3);

	//pen.setStyle(Qt::DotLine);

	painter.setPen(pen);

	int xBegin = 62;
	int nEnd = (602.0 - 62.0) / m_vecUpdateFileName.size() * (m_nProgress + 1);
	static int i = 0;
	if (i++ > 0)
	{
		painter.drawLine(QPoint(xBegin, 368), QPoint(nEnd, 368)); // (60,368) -> (602,368);
	}
}