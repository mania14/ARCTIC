#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include "ui_ARCTIC22.h"
#include "TerrainView.h"

class ArcticView;
class ARCTIC22 : public QMainWindow
{
	Q_OBJECT

public:
	ARCTIC22(QWidget *parent = Q_NULLPTR);
	~ARCTIC22();

	void			Init();

private slots:
	void			ToolBarPosition_Click();
	void			ToolBarRotation_Click();
	void			ToolBarScale_Click();
	void			ShowTerrainTool();
	void			refreshCaption();

private:
	Ui::ARCTIC22Class ui;
	ArcticView* pView;
	TerrainView* pTerrainView;

	QTime m_time;
	QTimer *m_timer;
};
