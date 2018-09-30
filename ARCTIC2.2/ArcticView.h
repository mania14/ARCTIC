#pragma once
#include <QtWidgets>

class ArcticView : public QWidget
{
public:
	ArcticView();
	QPaintEngine* paintEngine() const { return NULL; };
	void		  Initialize();

	int			  getViewFPSInfo();
protected:
	void resizeEvent(QResizeEvent *event) override {

		setAttribute(Qt::WA_PaintOnScreen, true);
		setAttribute(Qt::WA_NativeWindow, true);

		//QGraphicsView::resizeEvent(event);
	}

	void paintEvent(QPaintEvent* evt) override;

private:
	QTime m_time;
	QTimer *m_timer;
};