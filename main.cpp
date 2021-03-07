#include "WatermarkPipeline.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WatermarkPipeline w;
    w.show();
    return a.exec();
}
