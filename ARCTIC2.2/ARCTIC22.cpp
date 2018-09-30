#include "ARCTIC22.h"
#include "ArcticView.h"
#include "../Client/Client.h"

ARCTIC22::ARCTIC22(QWidget *parent)
	: QMainWindow(parent)
{
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(refreshCaption()));
	m_timer->start();

	m_time.start();

	pTerrainView = nullptr;

	ui.setupUi(this);

	pView = new ArcticView();
	pView->setObjectName(QStringLiteral("PlayGraphicsView"));
	pView->sizePolicy().setHeightForWidth(pView->sizePolicy().hasHeightForWidth());
	pView->setSizePolicy(pView->sizePolicy());

	ui.gridLayout_2->addWidget(pView, 0, 1, 1, 1);

	//signal
	QObject::connect(ui.actionTransformPosition, SIGNAL(triggered()), this, SLOT(ToolBarPosition_Click()));
	QObject::connect(ui.actionTransformRotation, SIGNAL(triggered()), this, SLOT(ToolBarRotation_Click()));
	QObject::connect(ui.actionTransformScale, SIGNAL(triggered()), this, SLOT(ToolBarScale_Click()));

	QObject::connect(ui.menuTerrainTool, SIGNAL(triggered()), this, SLOT(ShowTerrainTool()));
}

ARCTIC22::~ARCTIC22()
{
	delete pView;
	delete pTerrainView;
}

void ARCTIC22::ToolBarPosition_Click()
{
	Command(ToolCommand_TransformPositionMode);
}

void ARCTIC22::ToolBarRotation_Click()
{
	Command(ToolCommand_TransformRotationMode);
}

void ARCTIC22::ToolBarScale_Click()
{
	Command(ToolCommand_TransformScaleMode);
}

void ARCTIC22::ShowTerrainTool()
{
	if (nullptr == pTerrainView)
	{
		pTerrainView = new TerrainView();
	}
	ui.MenuDock->setWidget(pTerrainView);
	pTerrainView->show();
}

void ARCTIC22::Init()
{
	pView->Initialize();
}

void ARCTIC22::refreshCaption()
{
	int fps = pView->getViewFPSInfo();

	QString strCaption;
	strCaption.sprintf("ARCTIC 2.2 Engine / FPS : %d", fps);
	this->setWindowTitle(strCaption);
}
