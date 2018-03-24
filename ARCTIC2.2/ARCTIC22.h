#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ARCTIC22.h"

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

private:
	Ui::ARCTIC22Class ui;
	ArcticView* pView;
};
