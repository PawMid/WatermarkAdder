#include "DirectoryViewer.h"
#include <QVBoxLayout>
#include <QLabel>

using std::string;
using std::stringstream;
using std::getline;
/* PUBLIC */

DirectoryViewer::DirectoryViewer(QString directory) : QObject()
{
	this->directoryPath = new QString(directory);
	this->dirContents = new QListWidget();
	loadImages(directoryPath);
}

QWidget* DirectoryViewer::getWidget() {
	QWidget* widget = new QWidget();
	QVBoxLayout* widgetLayout = new QVBoxLayout();
	widgetLayout->setAlignment(Qt::AlignTop);
	widget->setLayout(widgetLayout);

	widgetLayout->addWidget(new QLabel("Images in input directory."));
	widgetLayout->addWidget(dirContents);

	return widget;
}

vector<string> DirectoryViewer::getImagesInDir()
{
	vector<string> vect;

	for (QString s : images) {
		vect.push_back(s.toStdString());
	}

	return vect;
}

string DirectoryViewer::getSelectedItem() {
	QListWidgetItem* item = dirContents->currentItem();
	if (item == nullptr) {
		item = dirContents->item(0);
	}

	return item->text().toStdString();
}

bool DirectoryViewer::empty()
{
	if(images.size() > 0)
		return false;
	return true;
}

/* PRIVATE */
void DirectoryViewer::loadImages(QString* path) {
	namespace fs = std::filesystem;
	images.clear();
	for (const auto& entry : directory_iterator(path->toStdString())) {
		string file = fs::absolute(entry.path()).string();
		if (file.find(".png") != string::npos || file.find(".jpg") != string::npos){
			stringstream ss(file);
			while(getline(ss, file, '\\')) {}
			images.push_back(QString::fromStdString(file));
		}
	}
	setListWidget();
}

void DirectoryViewer::setListWidget() {
	dirContents->clear();
	for (QString s : images) {
		dirContents->addItem(s);
	}
}