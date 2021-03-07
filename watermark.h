#ifndef WATERMARK_H
#define WATERMARK_H

#include <string>
#include <QImage>
#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QSpinBox>

using std::string;

class watermark : public QObject
{
   Q_OBJECT
   QString watermarkPath;
   QLabel * img;
   QSpinBox ** watermarkSize;
   QRadioButton ** position;
   QSpinBox ** offsetEdit;
   QSpinBox* opacity;

   void setImg();
   void addRadio(QLayout * layout);
   void addOffsetEdit(QLayout * layout);
   void addWatermarkSizeEdit(QLayout *layout);

private slots:
   void loadImg();
   void clearImg();
public:
    watermark();
    string getWatermarkPath();
    QWidget * getWidget();
    int * getWatermarkSize();
    int * getWatermarkOffset();
    QString getWatermarkPosition();
    int getOpacity();
};

#endif // WATERMARK_H
