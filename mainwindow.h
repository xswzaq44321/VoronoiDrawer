#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include <QTimer>

#include <memory>
#include "mywidget/myscene.h"
#include "dialogs/newmap/newmapdialog.h"
#include "voronoi/voronoimap.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	MyScene *scene = nullptr;

	// QWidget interface
protected:
	void resizeEvent(QResizeEvent *event);
private slots:
	void on_actionNew_Map_triggered();
	void on_actionSave_Json_triggered();
	void on_actionLoad_Json_triggered();
};

#endif // MAINWINDOW_H
