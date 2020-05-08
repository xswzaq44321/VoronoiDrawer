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
}

void MainWindow::on_actionSave_Json_triggered()
{
	if(scene != nullptr && scene->voronoiGen.vmap != nullptr){
		QString json_map = scene->voronoiGen.vmap->toJson(4).c_str();
		qDebug() << json_map;
		QString filename = QFileDialog::getSaveFileName(this, "save json", "/", "voronoi map(*.json)");
		QFile file(filename);
		if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
			QTextStream stream(&file);
			stream << json_map;
		}
		file.close();
	}
}

void MainWindow::on_actionLoad_Json_triggered()
{
	QString json_map;
	QString filename = QFileDialog::getOpenFileName(this, "load json", "/", "voronoi map(*.json)");
	QFile file(filename);
	if(file.open(QIODevice::ReadOnly)){
		QTextStream stream(&file);
		json_map = stream.readAll();

		Voronoi* vmap = Voronoi::fromJson(json_map.toStdString());
		if(scene == nullptr){
			scene = new MyScene();
			ui->graphicsView->setScene(scene);
		}
		scene->setVmap(vmap);
		ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
	}
	file.close();
}

void MainWindow::on_actionRun_Fortune_Algorithm_triggered(bool checked)
{
	if(checked){
		scene->voronoiGen.performFortune();
		scene->syncVmap();
	}
	scene->setAutoFortune(checked);
}

void MainWindow::on_actionStep_Fortune_Algorithm_triggered()
{
	scene->voronoiGen.stepFortune();
	scene->syncFortune();
}

void MainWindow::on_actionRun_Lloyd_triggered()
{
	scene->voronoiGen.performLloyd();
	scene->syncVmap();
}

void MainWindow::on_actionRun_Mamemaki_triggered()
{
	scene->voronoiGen.mamemaki(Rectangle(0, 0, scene->voronoiGen.vmap->width, scene->voronoiGen.vmap->height), 0.99);
	scene->syncVmap();
}
