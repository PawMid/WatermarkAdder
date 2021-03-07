#pragma once
#include <QObject>
#include <QString>
#include <QWidget>
#include <QListWidget>
#include <vector>
#include <filesystem>

using std::vector;
using std::filesystem::directory_iterator;
using std::string;

class DirectoryViewer : public QObject
{
	Q_OBJECT
private:
	QString* directoryPath;
	vector<QString> images;
	QListWidget* dirContents;
	
	void setListWidget();
public:
	DirectoryViewer(QString directory = "C:\\");
	QWidget* getWidget();
	void loadImages(QString* path);
	vector<string> getImagesInDir();
	string getSelectedItem();
	bool empty();
};

