#include "directoryselector.h"



DirectorySelector::DirectorySelector(QString selectorLabel, DirectoryViewer* viewer, QString buttonLabel, QString defaultDirectory)
{
    this->buttonLabel = buttonLabel;
    this->selectorLabel = selectorLabel;
    this->selectedDirectory = defaultDirectory;
    this->viewer = viewer;

    if (this->selectedDirectory == "."){
        this->directoryLabel = new QLabel("Current directory");
    } else {
        this->directoryLabel = new QLabel(this->selectedDirectory);
    }

}

DirectorySelector::~DirectorySelector(){}

QWidget * DirectorySelector::getWidget(){
    QWidget * toReturn = new QWidget();
    QGridLayout * widgetLayout = new QGridLayout();

    QLabel * widgetLabel = new QLabel(this->selectorLabel);
    QPushButton * widgetBtn = new QPushButton(this->buttonLabel);
    connect(widgetBtn, &QPushButton::clicked, this, &DirectorySelector::on_button_clicked);


    widgetLayout->addWidget(widgetLabel);

    widgetLayout->addWidget(directoryLabel, 1, 0, 1, 2);
    widgetLayout->addWidget(widgetBtn);
    toReturn->setLayout(widgetLayout);
    toReturn->setFixedHeight(100);
    return toReturn;

}

QString* DirectorySelector::getDirectory()
{
    return &selectedDirectory;
}

QString DirectorySelector::trimLabel(){
    QString location = selectedDirectory;
    if(location.length() > 20){
        location = location.left(17) + "...";
    }
    return location;
}

QString DirectorySelector::expandLabel(){
    return selectedDirectory;
}

void DirectorySelector::on_button_clicked(){
    this->selectedDirectory = QFileDialog::getExistingDirectory(nullptr, "Select dir", this->selectedDirectory);

    if(this->selectedDirectory.isEmpty()){
        selectedDirectory = "C:\\";
    }

    this->directoryLabel->setText(selectedDirectory);
    if (viewer != nullptr)
        viewer->loadImages(getDirectory());
}

void DirectorySelector::on_mouse_enter(){
    directoryLabel->setText(expandLabel());
    directoryLabel->raise();
}


void DirectorySelector::on_mouse_leave(){
    directoryLabel->setText(trimLabel());
    directoryLabel->lower();
}
