#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	canvas = new Canvas(this);
	setCentralWidget(canvas);

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.actionNext, SIGNAL(triggered()), this, SLOT(onNext()));
	connect(ui.actionPrevious, SIGNAL(triggered()), this, SLOT(onPrevious()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(onSave()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionUndo, SIGNAL(triggered()), this, SLOT(onUndo()));
}

MainWindow::~MainWindow() {
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
	canvas->keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
	canvas->keyReleaseEvent(e);
}

void MainWindow::onOpen() {
	QString new_filename = QFileDialog::getOpenFileName(this, tr("Open Image file..."), "", tr("Image Files (*.jpg)"));
	if (new_filename.isEmpty()) return;

	filename = new_filename;
	int index = filename.lastIndexOf(".");
	if (index < 0) return;

	QString basename = filename.mid(0, index);

	canvas->load(basename);
	this->setWindowTitle("CMP Data Editor - " + filename);
}

void MainWindow::onSave() {
	int index = filename.lastIndexOf(".");
	if (index < 0) return;

	QString basename = filename.mid(0, index);

	canvas->save(basename + ".xml");
}

void MainWindow::onNext() {
	int index = filename.lastIndexOf(".");
	if (index < 0) return;

	QString basename = filename.mid(0, index);

	QString prefix = basename.mid(0, basename.length() - 4);
	index = basename.mid(basename.length() - 4).toInt() + 1;

	basename = prefix + QString("%1").arg(index, 4, 10, QChar('0'));

	if (QFileInfo(basename + ".jpg").exists()) {
		filename = basename + ".jpg";
		canvas->load(basename);
		this->setWindowTitle("CMP Data Editor - " + filename);
	}
	else {
		QMessageBox msg;
		msg.setText("No more next file exists.");
		msg.exec();
	}
}

void MainWindow::onPrevious() {
	int index = filename.lastIndexOf(".");
	if (index < 0) return;

	QString basename = filename.mid(0, index);

	QString prefix = basename.mid(0, basename.length() - 4);
	index = basename.mid(basename.length() - 4).toInt() - 1;

	basename = prefix + QString("%1").arg(index, 4, 10, QChar('0'));

	if (QFileInfo(basename + ".jpg").exists()) {
		filename = basename + ".jpg";
		canvas->load(basename);
		this->setWindowTitle("CMP Data Editor - " + filename);
	}
	else {
		QMessageBox msg;
		msg.setText("No more previous file exists.");
		msg.exec();
	}
}

void MainWindow::onUndo() {
	canvas->undo();
}