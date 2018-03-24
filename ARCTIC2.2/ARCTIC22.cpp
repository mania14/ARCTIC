#include "ARCTIC22.h"
#include "ArcticView.h"
#include "../Client/Client.h"

ARCTIC22::ARCTIC22(QWidget *parent)
	: QMainWindow(parent)
{
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
}

ARCTIC22::~ARCTIC22()
{
	delete pView;
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

void ARCTIC22::Init()
{
	pView->Initialize();
}
