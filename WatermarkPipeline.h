#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "directoryselector.h"
#include "watermark.h"
#include "DirectoryViewer.h"

#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QtWidgets/QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

using std::fstream;
using std::string;
using cv::Mat;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class WatermarkPipeline : public QMainWindow
{
    Q_OBJECT

public:
    WatermarkPipeline(QWidget* parent = nullptr);
    ~WatermarkPipeline();

private slots:
    void on_processImages_clicked();
    void on_preview_clicked();

private:
    DirectorySelector* inputDirectory;
    DirectorySelector* outputDirectory;
    DirectoryViewer* viewer;
    watermark* mark;
    QWidget* mainWidget;
    QGridLayout* mainLayout;
    QPushButton* processImages;
    QPushButton* showPreview;

    void rescaleWatermark(Mat & watermark, int* size);
    int* calculateWatermarkPosition(Mat image, Mat wateramrk, string position, int* offset);
    bool checkXBound(int posX, int wtmX, int imgX);
    bool checkYBound(int posY, int wtmY, int imgY);
    void overlayImage(Mat* src, Mat* overlay, const cv::Rect& location, double opac, string name);
    
};
#endif // MAINWINDOW_H
