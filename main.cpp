#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	w.setWindowTitle("Voronoi Drawer");
	w.setWindowIcon(QIcon(":/icon/small_icon"));

	return a.exec();
}
