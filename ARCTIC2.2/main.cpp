#include "ARCTIC22.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ARCTIC22 w;
	w.show();
	w.Init();
	return a.exec();
}
