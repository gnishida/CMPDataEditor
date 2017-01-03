#include "Canvas.h"
#include <QPainter>
#include <iostream>
#include <QFileInfoList>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QDomDocument>
#include <QResizeEvent>

#ifndef SQR
#define SQR(x)	((x) * (x))
#endif

void OperationHistory::addDeleteOperation(int index) {
	operations.push_back(Operation(Operation::TYPE_DELETE, index));
}

void OperationHistory::addAddOperation(int index) {
	operations.push_back(Operation(Operation::TYPE_ADD, index));
}

void OperationHistory::addMoveOperation(int index, float x1, float y1, float x2, float y2) {
	operations.push_back(Operation(Operation::TYPE_MOVE, index, x1, y1, x2, y2));
}

Operation OperationHistory::pop() {
	Operation oper = operations.back();
	operations.pop_back();
	return oper;
}

int OperationHistory::size() {
	return operations.size();
}

Canvas::Canvas(QWidget *parent) : QWidget(parent) {
	selectedIndex = -1;
	ctrlPressed = false;
	shiftPressed = false;
}

Canvas::~Canvas() {
}

void Canvas::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.drawImage(0, 0, resizedImage);

	for (int i = 0; i < objects.size(); ++i) {
		if (objects[i].deleted) continue;

		if (objects[i].labelname == "facade" || objects[i].labelname == "window" || objects[i].labelname == "door" || objects[i].labelname == "shop") {
			if (selectedIndex == i) {
				painter.setPen(QPen(QColor(255, 255, 0), 3));
			}
			else {
				painter.setPen(QPen(QColor(255, 255, 0), 1));
			}
			painter.drawRect(objects[i].x1 * resizedImage.width(), objects[i].y1 * resizedImage.height(), (objects[i].x2 - objects[i].x1) * resizedImage.width(), (objects[i].y2 - objects[i].y1) * resizedImage.height());
		}
	}
}

void Canvas::mousePressEvent(QMouseEvent* e) {
	if (ctrlPressed) {
		objects.push_back(LabelObject(2, "window", (float)e->x() / resizedImage.width(), (float)e->y() / resizedImage.height(), (float)(e->x() + 10) / resizedImage.width(), (float)(e->y() + 10) / resizedImage.height()));
		history.addAddOperation(objects.size() - 1);
	}
	else {
		selectedIndex = -1;
		float min_dist = std::numeric_limits<float>::max();

		const float delta = 5.0f;

		for (int i = 0; i < objects.size(); ++i) {
			if (objects[i].deleted) continue;

			if (objects[i].labelname == "facade" || objects[i].labelname == "window" || objects[i].labelname == "door" || objects[i].labelname == "shop") {
				if (e->x() < objects[i].x1 * resizedImage.width() - delta || e->x() > objects[i].x2 * resizedImage.width() + delta) continue;
				if (e->y() < objects[i].y1 * resizedImage.height() - delta || e->y() > objects[i].y2 * resizedImage.height() + delta) continue;

				// check top left corner
				float dist = SQR(e->x() - objects[i].x1 * resizedImage.width()) + SQR(e->y() - objects[i].y1 * resizedImage.height());
				if (dist < min_dist) {
					min_dist = dist;
					selectedIndex = i;
					selectedVertex = TOP_LEFT;
				}

				// check top right corner
				dist = SQR(e->x() - objects[i].x2 * resizedImage.width()) + SQR(e->y() - objects[i].y1 * resizedImage.height());
				if (dist < min_dist) {
					min_dist = dist;
					selectedIndex = i;
					selectedVertex = TOP_RIGHT;
				}

				// check bottom left corner
				dist = SQR(e->x() - objects[i].x1 * resizedImage.width()) + SQR(e->y() - objects[i].y2 * resizedImage.height());
				if (dist < min_dist) {
					min_dist = dist;
					selectedIndex = i;
					selectedVertex = BOTTOM_LEFT;
				}

				// check bottom right corner
				dist = SQR(e->x() - objects[i].x2 * resizedImage.width()) + SQR(e->y() - objects[i].y2 * resizedImage.height());
				if (dist < min_dist) {
					min_dist = dist;
					selectedIndex = i;
					selectedVertex = BOTTOM_RIGHT;
				}
			}
		}

		// for facade, we check the distance more strictly
		if (selectedIndex >= 0 && selectedIndex < objects.size()) {
			if (objects[selectedIndex].labelname == "facade") {
				if (min_dist > SQR(delta)) {
					selectedIndex = -1;
				}
			}
		}

		// add history
		if (selectedIndex >= 0 && selectedIndex < objects.size()) {
			prev_pos = e->pos();
			history.addMoveOperation(selectedIndex, objects[selectedIndex].x1, objects[selectedIndex].y1, objects[selectedIndex].x2, objects[selectedIndex].y2);
		}
	}

	update();
}

void Canvas::mouseMoveEvent(QMouseEvent* e) {
	if (selectedIndex >= 0 && selectedIndex < objects.size()) {
		if (selectedVertex == TOP_LEFT) {
			objects[selectedIndex].x1 = (float)e->x() / resizedImage.width();
			objects[selectedIndex].y1 = (float)e->y() / resizedImage.height();
		}
		else if (selectedVertex == TOP_RIGHT) {
			objects[selectedIndex].x2 = (float)e->x() / resizedImage.width();
			objects[selectedIndex].y1 = (float)e->y() / resizedImage.height();
		}
		else if (selectedVertex == BOTTOM_LEFT) {
			objects[selectedIndex].x1 = (float)e->x() / resizedImage.width();
			objects[selectedIndex].y2 = (float)e->y() / resizedImage.height();
		}
		else if (selectedVertex == BOTTOM_RIGHT) {
			objects[selectedIndex].x2 = (float)e->x() / resizedImage.width();
			objects[selectedIndex].y2 = (float)e->y() / resizedImage.height();
		}

		// check the boundary condition
		if (objects[selectedIndex].x1 < 0) objects[selectedIndex].x1 = 0;
		if (objects[selectedIndex].y1 < 0) objects[selectedIndex].y1 = 0;
		if (objects[selectedIndex].x2 > 1) objects[selectedIndex].x2 = 1;
		if (objects[selectedIndex].y2 > 1) objects[selectedIndex].y2 = 1;

		update();
	}
}

void Canvas::mouseReleaseEvent(QMouseEvent* e) {
	if (selectedIndex >= 0 && selectedIndex < objects.size()) {
		if (e->pos() == prev_pos) {
			history.pop();
		}
	}
}

void Canvas::resizeEvent(QResizeEvent *e) {
	if (!image.isNull()) {
		float scale = std::min((float)width() / image.width(), (float)height() / image.height());
		resizedImage = image.scaled(image.width() * scale, image.height() * scale);
	}
}

void Canvas::load(const QString& basename) {
	image = QImage(basename + ".jpg");
	float scale = std::min((float)width() / image.width(), (float)height() / image.height());
	resizedImage = image.scaled(image.width() * scale, image.height() * scale);

	// load annotation
	loadAnnotation(basename + ".xml", image.width(), image.height());

	selectedIndex = -1;

	update();
}

void Canvas::loadAnnotation(const QString& filename, int width, int height) {
	objects.clear();

	QFile file(filename);

	QDomDocument doc;
	doc.setContent(&file, true);
	QDomElement root = doc.documentElement();
	QDomNode object_node = root.firstChild();
	while (!object_node.isNull()) {
		if (object_node.toElement().tagName() == "object") {
			int label;
			QString label_name;
			std::vector<float> xs;
			std::vector<float> ys;

			QDomNode data_node = object_node.firstChild();
			while (!data_node.isNull()) {
				if (data_node.toElement().tagName() == "points") {
					QDomNode coord_node = data_node.firstChild();
					while (!coord_node.isNull()) {
						if (coord_node.toElement().tagName() == "x") {
							xs.push_back(getNodeValue(coord_node).toFloat());
						}
						else if (coord_node.toElement().tagName() == "y") {
							ys.push_back(getNodeValue(coord_node).toFloat());
						}

						coord_node = coord_node.nextSibling();
					}
				}
				else if (data_node.toElement().tagName() == "label") {
					label = getNodeValue(data_node).toInt();
				}
				else if (data_node.toElement().tagName() == "labelname") {
					label_name = getNodeValue(data_node);
				}

				data_node = data_node.nextSibling();
			}

			objects.push_back(LabelObject(label, label_name, xs[0], ys[0], xs[1], ys[1]));


		}

		object_node = object_node.nextSibling();
	}

}

QString Canvas::getNodeValue(const QDomNode& node) {
	QDomNode text_node = node.firstChild();
	while (!text_node.isNull()) {
		if (text_node.nodeType() == QDomNode::TextNode) {
			return text_node.toText().data().trimmed();
		}

		text_node = text_node.nextSibling();
	}

	return "";
}

void Canvas::save(const QString& filename) {
	QFile file(filename);
	file.open(QFile::WriteOnly);

	QDomDocument doc;
	QDomElement root = doc.createElement("objects");
	doc.appendChild(root);

	for (int i = 0; i < objects.size(); ++i) {
		if (objects[i].deleted) continue;

		QDomElement object_node = doc.createElement("object");

		// points node
		QDomElement points_node = doc.createElement("points");

		QDomElement x_node1 = doc.createElement("x");
		QDomText x_value1 = doc.createTextNode(QString::number(objects[i].x1));
		x_node1.appendChild(x_value1);
		QDomElement x_node2 = doc.createElement("x");
		QDomText x_value2 = doc.createTextNode(QString::number(objects[i].x2));
		x_node2.appendChild(x_value2);
		QDomElement y_node1 = doc.createElement("y");
		QDomText y_value1 = doc.createTextNode(QString::number(objects[i].y1));
		y_node1.appendChild(y_value1);
		QDomElement y_node2 = doc.createElement("y");
		QDomText y_value2 = doc.createTextNode(QString::number(objects[i].y2));
		y_node2.appendChild(y_value2);
			
		points_node.appendChild(x_node1);
		points_node.appendChild(x_node2);
		points_node.appendChild(y_node1);
		points_node.appendChild(y_node2);
		object_node.appendChild(points_node);

		// label node
		QDomElement label_node = doc.createElement("label");
		QDomText label_value = doc.createTextNode(QString::number(objects[i].label));
		label_node.appendChild(label_value);
		object_node.appendChild(label_node);

		// labelname node
		QDomElement labelname_node = doc.createElement("labelname");
		QDomText labelname_value = doc.createTextNode(objects[i].labelname);
		labelname_node.appendChild(labelname_value);
		object_node.appendChild(labelname_node);
			
		root.appendChild(object_node);
	}

	QTextStream out(&file);
	doc.save(out, 4);
}

void Canvas::undo() {
	if (history.size() == 0) return;

	Operation oper = history.pop();
	if (oper.type == Operation::TYPE_DELETE) {
		objects[oper.objectIndex].deleted = false;
	}
	else if (oper.type == Operation::TYPE_MOVE) {
		objects[oper.objectIndex].x1 = oper.x1;
		objects[oper.objectIndex].y1 = oper.y1;
		objects[oper.objectIndex].x2 = oper.x2;
		objects[oper.objectIndex].y2 = oper.y2;
	}
	else if (oper.type == Operation::TYPE_ADD) {
		objects[oper.objectIndex].deleted = true;
	}

	update();
}

void Canvas::keyPressEvent(QKeyEvent* e) {
	ctrlPressed = false;
	shiftPressed = false;

	if (e->modifiers() & Qt::ControlModifier) {
		ctrlPressed = true;
	}
	if (e->modifiers() & Qt::ShiftModifier) {
		shiftPressed = true;
	}

	switch (e->key()) {
	case Qt::Key_Space:
		break;
	case Qt::Key_Delete:
		if (selectedIndex >= 0 && selectedIndex < objects.size()) {
			objects[selectedIndex].deleted = true;
			history.addDeleteOperation(selectedIndex);
			selectedIndex = -1;
		}
		break;
	}

	update();
}

void Canvas::keyReleaseEvent(QKeyEvent* e) {
	switch (e->key()) {
	case Qt::Key_Control:
		ctrlPressed = false;
		break;
	case Qt::Key_Shift:
		shiftPressed = false;
		break;
	default:
		break;
	}
}

