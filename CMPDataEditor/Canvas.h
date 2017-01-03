#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QKeyEvent>
#include <opencv2/opencv.hpp>
#include <QDomNode>

class LabelObject {
public:
	int label;
	QString labelname;
	float x1;
	float y1;
	float x2;
	float y2;
	bool deleted;

public:
	LabelObject(int label, const QString& labelname, float x1, float y1, float x2, float y2) : label(label), labelname(labelname), x1(x1), y1(y1), x2(x2), y2(y2), deleted(false) {}
};

class Operation {
public:
	static enum { TYPE_MOVE = 0, TYPE_ADD, TYPE_DELETE };

public:
	int type;
	int objectIndex;
	float x1;
	float y1;
	float x2;
	float y2;

public:
	Operation(int type, int objectIndex) : type(type), objectIndex(objectIndex) {}
	Operation(int type, int objectIndex, float x1, float y1, float x2, float y2) : type(type), objectIndex(objectIndex), x1(x1), y1(y1), x2(x2), y2(y2) {}
};

class OperationHistory {
public:
	int cursor;
	std::vector<Operation> operations;

public:
	OperationHistory() : cursor(-1) {}
	
	void addDeleteOperation(int index);
	void addAddOperation(int index);
	void addMoveOperation(int index, float x1, float y1, float x2, float y2);
	Operation pop();
	int size();
};

class Canvas : public QWidget {
private:
	static enum { TOP_LEFT = 0, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };

private:
	QImage image;
	QImage resizedImage;
	std::vector<LabelObject> objects;
	OperationHistory history;
	QPoint prev_pos;

	int selectedIndex;
	int selectedVertex;

	bool ctrlPressed;
	bool shiftPressed;
	
public:
	Canvas(QWidget *parent = NULL);
    ~Canvas();
	
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	void resizeEvent(QResizeEvent *e);

public:
	void load(const QString& basename);
	void Canvas::loadAnnotation(const QString& filename, int width, int height);
	QString getNodeValue(const QDomNode& node);
	void save(const QString& filename);
	void undo();
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
};

#endif // CANVAS_H
