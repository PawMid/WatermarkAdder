#include "watermark.h"

#include <QGridLayout>
#include <QPixmap>
#include <QFrame>
#include <QPushButton>
#include <QFileDialog>
#include <QSize>
#include <QString>

/* PUBLIC */
watermark::watermark() : QObject()
{
    this->watermarkPath = "";
    this->img = new QLabel();
    this->offsetEdit = new QSpinBox*[2];
    this->position = new QRadioButton*[5];
    this->watermarkSize = new QSpinBox*[2];
    this->opacity = new QSpinBox();

    opacity->setMinimum(0);
    opacity->setMaximum(100);
    opacity->setValue(100);

    QString positions [] = {"Top right", "Top left", "Bottom right", "Bottom left", "Center"};

    for (int i = 0; i < 2; i++){
        watermarkSize[i] = new QSpinBox();
        watermarkSize[i]->setMaximum(10e4);
        offsetEdit[i] = new QSpinBox();
        //offsetEdit[i]->setFixedHeight(26);
        offsetEdit[i]->setMaximum(10e5);
        offsetEdit[i]->setMinimum(-10e3);
    }



    for(int i = 0; i < 5; i++){
        position[i] = new QRadioButton(positions[i]);
    }
    position[2]->setChecked(true);

}

string watermark::getWatermarkPath(){
    return watermarkPath.toStdString();
}

int * watermark::getWatermarkSize(){
    int * size = new int[2];
    for(int i = 0; i < 2; i++){
        size[i] = watermarkSize[i]->value();
    }
    return size;
}
int * watermark::getWatermarkOffset(){
    int * offset = new int[2];
    for(int i = 0; i < 2; i++){
        offset[i] = offsetEdit[i]->value();
    }
    return offset;
}
QString watermark::getWatermarkPosition(){
    for(int i = 0; i < 5; i++){
        if(position[i]->isChecked()){
            return position[i]->text();
        }
    }
    return NULL;
}

int watermark::getOpacity()
{
    return this->opacity->value();
}

QWidget* watermark::getWidget() {
    QWidget* toReturn = new QWidget();
    QGridLayout* widgetLayout = new QGridLayout();

    toReturn->setLayout(widgetLayout);

    QLabel* title = new QLabel("Watermark preview");
    title->setFixedHeight(30);
    widgetLayout->addWidget(title, 0, 0, 1, 2);

    QFrame* frame = new QFrame();
    QGridLayout* frameLayout = new QGridLayout();
    frameLayout->setContentsMargins(0, 0, 0, 0);
    frameLayout->addWidget(img);
    frame->setLayout(frameLayout);

    frameLayout->addWidget(this->img);
    frame->setFixedSize(250, 250);
    widgetLayout->addWidget(frame, 1, 0, 2, 2);

    QWidget* watermarkPositionSelector = new QWidget();
    QVBoxLayout* selectorLayout = new QVBoxLayout();
    selectorLayout->setContentsMargins(0, 0, 0, 0);
    selectorLayout->setAlignment(Qt::AlignTop);
    watermarkPositionSelector->setLayout(selectorLayout);
    
    QLabel* watermarkPositionLabel = new QLabel("Watermark position:");
    selectorLayout->addWidget(watermarkPositionLabel);
    addRadio(selectorLayout);
    
    selectorLayout->addWidget(new QLabel("Offset"));
    addOffsetEdit(selectorLayout);
    
    selectorLayout->addWidget(new QLabel("Watermark size"));
    addWatermarkSizeEdit(selectorLayout);
    
    selectorLayout->addWidget(new QLabel("Opacity"));
    QWidget* opacityWidget = new QWidget();
    QHBoxLayout* opacityLayout = new QHBoxLayout();
    opacityWidget->setLayout(opacityLayout);
    opacityLayout->addWidget(this->opacity);
    opacityLayout->addWidget(new QLabel("%"));
    selectorLayout->addWidget(opacityWidget);
    
    widgetLayout->addWidget(watermarkPositionSelector, 1, 2, 1, 1);

    
    QPushButton* load = new QPushButton("Load watermark");
    load->setFixedHeight(30);
    connect(load, &QPushButton::clicked, this, &watermark::loadImg);
    widgetLayout->addWidget(load, 3, 0, 1, 1);

    QPushButton* clear = new QPushButton("Clear watermark");
    clear->setFixedHeight(30);
    connect(clear, &QPushButton::clicked, this, &watermark::clearImg);
    widgetLayout->addWidget(clear, 3, 1, 1, 1);

    return toReturn;
}

/* PRIVATE */

void watermark::setImg(){
    QPixmap im;
    im.load(watermarkPath);
    QSize size = im.size();
    watermarkSize[0]->setValue( size.width());
    watermarkSize[1]->setValue(size.height());
    im = im.scaled(250,250);
    this->img->setPixmap(im);
}

void watermark::loadImg(){
    watermarkPath = QFileDialog::getOpenFileName(nullptr, "Open watermark", "C:\\", "Image file (*.png *.jpg)");
    setImg();
}

void watermark::clearImg(){
    this->watermarkPath = "";
    this->img->clear();
}

void watermark::addRadio(QLayout * layout){
    for ( int i = 0; i < 5; i++){
        layout->addWidget(this->position[i]);
    }
}

void watermark::addOffsetEdit(QLayout *layout){
    QString labels [] = {"X:", "Y:"};

    for(int i = 0; i < 2; i++){
        QWidget * widget = new QWidget();
        QHBoxLayout * widgetLayout = new QHBoxLayout();
        widgetLayout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(widgetLayout);
        QLabel * label = new QLabel(labels[i]);
        label->setFixedSize(20,20);
        widgetLayout->addWidget(label);
        widgetLayout->addWidget(offsetEdit[i]);
        widgetLayout->addWidget(new QLabel("px"));
        layout->addWidget(widget);
    }
}

void watermark::addWatermarkSizeEdit(QLayout *layout){
    QString labels [] = {"X:", "Y:"};

    for(int i = 0; i < 2; i++){
        QWidget * widget = new QWidget();
        QHBoxLayout * widgetLayout = new QHBoxLayout();
        widgetLayout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(widgetLayout);
        QLabel * label = new QLabel(labels[i]);
        label->setFixedSize(20,20);
        widgetLayout->addWidget(label);
        widgetLayout->addWidget(watermarkSize[i]);
        widgetLayout->addWidget(new QLabel("px"));
        layout->addWidget(widget);
    }
}


