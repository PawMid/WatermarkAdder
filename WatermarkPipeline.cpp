#include "WatermarkPipeline.h"

#include <vector>
#include <QDialog>
#include <QWidget>

using std::vector;
using std::string;

WatermarkPipeline::WatermarkPipeline(QWidget* parent) : QMainWindow(parent)
{
    setWindowIcon(QIcon("./414-4140061_library-of-broken-water-pipe-png-black-and.png"));
    mainWidget = new QWidget();
    viewer = new DirectoryViewer();
    inputDirectory = new DirectorySelector("Selected input directory:", viewer);
    outputDirectory = new DirectorySelector("Selected output directory:");
    processImages = new QPushButton("Add watermark");
    showPreview = new QPushButton("Show preview");
    connect(processImages, &QPushButton::clicked, this, &WatermarkPipeline::on_processImages_clicked);
    connect(showPreview, &QPushButton::clicked, this, &WatermarkPipeline::on_preview_clicked);
    
    mark = new watermark();
    mainLayout = new QGridLayout();
    mainWidget->setLayout(mainLayout);
    
    mainLayout->addWidget(inputDirectory->getWidget());
    mainLayout->addWidget(outputDirectory->getWidget(), 0, 1, 1, 1);
    mainLayout->addWidget(mark->getWidget(), 1, 1, 1, 1);
    mainLayout->addWidget(viewer->getWidget(), 1, 0, 1, 1);
    mainLayout->addWidget(processImages, 2, 0, 1, 1);
    mainLayout->addWidget(showPreview, 2, 1, 1, 1);
    layout()->setAlignment(Qt::AlignTop);

    QMainWindow::setWindowTitle("Water Pipe");
    setCentralWidget(mainWidget);

    fstream read;
    read.open(".\\style.qss");
    string style = "";
    string line;

    if (read.is_open()) {
        while (getline(read, line)) {
            style += line;
        }
    }

    centralWidget()->setStyleSheet(QString::fromStdString(style));
}

WatermarkPipeline::~WatermarkPipeline()
{
    delete inputDirectory;
    delete outputDirectory;
    delete mainWidget;
}

void WatermarkPipeline::on_processImages_clicked()
{
    vector<string> images = viewer->getImagesInDir();
    string watermarkPath = mark->getWatermarkPath();
    if (images.size() > 0 && watermarkPath.size() > 0) {
        string destinationDir = outputDirectory->getDirectory()->toStdString();
        string sourceDir = inputDirectory->getDirectory()->toStdString();
        
        string position = mark->getWatermarkPosition().toStdString();
        double opacity = double(mark->getOpacity()) / 100.;
        double beta = (1.0 - opacity);
        int* size = mark->getWatermarkSize();
        int* offset = mark->getWatermarkOffset();

        Mat watermark = cv::imread(watermarkPath, cv::IMREAD_UNCHANGED);
        if (watermark.channels() < 3) {
            cv::cvtColor(watermark, watermark, cv::COLOR_GRAY2BGR);
        }

        rescaleWatermark(watermark, size);
        int watermarkY = watermark.size().height;
        int watermarkX = watermark.size().width;

        for (string s : images) {
            string path = sourceDir + "/" + s;
            Mat img = cv::imread(path, cv::IMREAD_UNCHANGED);
            if (img.channels() < 3) {
                cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
            }
            
            int* calculatedPos = calculateWatermarkPosition(img, watermark, position, offset);
            cv::Rect pos = cv::Rect(calculatedPos[0], calculatedPos[1], watermarkX, watermarkY);
            overlayImage(&img, &watermark, pos, opacity, watermarkPath);

            cv::imwrite(destinationDir + "/" + s, img);
        }
    }
}

void WatermarkPipeline::on_preview_clicked() {
    string watermarkPath = mark->getWatermarkPath();
    if (!viewer->empty() && watermarkPath.size() > 0) {
        string imgToPrev = viewer->getSelectedItem();
        string sourceDir = inputDirectory->getDirectory()->toStdString();
    
        string position = mark->getWatermarkPosition().toStdString();
        double opacity = double(mark->getOpacity()) / 100.;
        double beta = (1.0 - opacity);
        int* size = mark->getWatermarkSize();
        int* offset = mark->getWatermarkOffset();

        Mat watermark = cv::imread(watermarkPath, cv::IMREAD_UNCHANGED);
        if (watermark.channels() < 3) {
            cv::cvtColor(watermark, watermark, cv::COLOR_GRAY2BGR);
        }

        rescaleWatermark(watermark, size);
        int watermarkY = watermark.size().height;
        int watermarkX = watermark.size().width;

        string path = sourceDir + "/" + imgToPrev;
        Mat img = cv::imread(path, cv::IMREAD_UNCHANGED);
        if (img.channels() < 3) {
            cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
        }

        int* calculatedPos = calculateWatermarkPosition(img, watermark, position, offset);
        cv::Rect pos = cv::Rect(calculatedPos[0], calculatedPos[1], watermarkX, watermarkY);
        overlayImage(&img, &watermark, pos, opacity, watermarkPath);

        cv::imshow("Preview", img);
        cv::waitKey(0);
    
    }
}

void WatermarkPipeline::rescaleWatermark(Mat& watermark, int* size) {
    int width = watermark.size().width;
    int height = watermark.size().height;
    float* percentage = new float[2];
    percentage[0] = float(size[0]) / width;
    percentage[1] = float(size[0]) / height;
    Mat dst;

    cv::Size s(width * percentage[0], height * percentage[1]);
    

    try {
        cv::resize(watermark, watermark, s);
    }
    catch (cv::Exception e) {
        QDialog* dial = new QDialog();
        QHBoxLayout* lay = new QHBoxLayout();
        std::cerr << e.err;
        dial->setLayout(lay);
        lay->addWidget(new QLabel(QString::fromStdString(e.err)));
        dial->show();
    }
}

int* WatermarkPipeline::calculateWatermarkPosition(Mat image, Mat watermark, string position, int* offset){
    int* xy = new int[2];
    int imageY = image.size().height;
    int imageX = image.size().width;
    int watermarkY = watermark.size().height;
    int watermarkX = watermark.size().width;

    if (position == "Top left") {
        xy[0] = offset[0];
        xy[1] = offset[1];
    }
    else if (position == "Bottom left") {
        xy[0] = offset[0];
        xy[1] = imageY - watermarkY + offset[1];
    } 
    else if (position == "Top right") {
        xy[0] = imageX - watermarkX + offset[0];
        xy[1] = offset[1];
    }
    else if (position == "Bottom right") {
        xy[0] = imageX - watermarkX + offset[0];
        xy[1] = imageY - watermarkY + offset[1];
    } 
    else if (position == "Center") {
        xy[0] = int(imageX /2 ) - watermarkX / 2  + offset[0];
        xy[1] = int(imageY / 2) - watermarkY / 2 + offset[1];
    }

    if (!checkXBound(xy[0], watermarkX, imageX))
        xy[0] -= offset[0];

    if (!checkYBound(xy[1], watermarkY, imageY))
        xy[1] -= offset[1];

    return xy;
}

bool WatermarkPipeline::checkXBound(int posX, int wtmX, int imgX) {
    if (posX + wtmX > imgX || posX < 0)
        return false;
    return true;
}

bool WatermarkPipeline::checkYBound(int posY, int wtmY, int imgY) {
    if (posY + wtmY > imgY || posY < 0)
        return false;
    return true;
}

void WatermarkPipeline::overlayImage(Mat* src, Mat* overlay, const cv::Rect& location, double opac, string name)
{
    for (int y = std::max(location.y, 0); y < src->rows; ++y)
    {
        int fY = y - location.y;

        if (fY >= overlay->rows)
            break;

        for (int x = std::max(location.x, 0); x < src->cols; ++x)
        {
            int fX = x - location.x;

            if (fX >= overlay->cols)
                break;
            
            double opacity;
            if (name.find(".jpg") != string::npos) {
                opacity = opac;
            }
            else {
                opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) * opac / 255;
            }
            
            for (int c = 0; opacity > 0 && c < src->channels(); ++c)
            {
                unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
                src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
            }
        }
    }
}
