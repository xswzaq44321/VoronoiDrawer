#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace voronoiMap;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->graphicsView->setBackgroundBrush(QBrush(QColor(135, 206, 235)));

	{
		Point bar;
		Edge bar2;
		Polygon bar3;
		Voronoi bar4;
	}
}

MainWindow::~MainWindow()
{
	delete ui;
	delete scene;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	ui->graphicsView->setGeometry(1, 1,
								  this->centralWidget()->width() - 2,
								  this->centralWidget()->height() - 4);

	if(scene != nullptr)
		ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_actionNew_Map_triggered()
{
	NewMapDialog nmd;
	nmd.exec();

	delete scene;
	scene = new MyScene(nmd.size);
	scene->addRect(scene->sceneRect(), QPen(Qt::white), QBrush(Qt::white))->setZValue(-100);
	ui->graphicsView->setScene(scene);
	ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
	delete vmap;
	vmap = new Voronoi(nmd.size.width(), nmd.size.height());
	scene->vmap = vmap;
}

void MainWindow::on_actionSave_Json_triggered()
{
	qDebug() << vmap->toJson(4).c_str();
}
