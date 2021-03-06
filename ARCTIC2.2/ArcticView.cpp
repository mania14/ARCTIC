#include "ArcticView.h"
#include "../Client/Client.h"

ArcticView::ArcticView()
{
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
	m_timer->start();

	m_time.start();

	Init((HWND)winId(), nullptr);
}

void ArcticView::Initialize()
{
}

int ArcticView::getViewFPSInfo()
{
	return GetFPSInfo();
}



void ArcticView::paintEvent(QPaintEvent * evt)
{
	UpdateLoop();
}