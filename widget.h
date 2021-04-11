#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "mv_json.h"
#include "HttpDownload.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

	void paintEvent(QPaintEvent *);

signals:
	void clicked();

	public slots:
	void mouseClicked();

protected:
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);

	void ChangeBgImg(const std::string& strPath);
	void GengXinWenJian(void);

private:
	QPoint mousePos;

private:
    Ui::Widget *ui;
	std::string m_strCurrentImg;
	int m_nProgress;

public:
	static std::vector<std::string> m_vecUpdateFileName;
	static std::string m_strPath;
};

#endif // WIDGET_H
