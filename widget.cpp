#include "widget.h"
#include "ui_widget.h"
#include "stylehelper.h"
#include <QAction>
#include <QGraphicsPixmapItem>
#include <QColorDialog>
#include <QAbstractItemView>
#include <QListView>
#include <QStyledItemDelegate>
#include <QDebug>
#include <QPushButton>
#include "mapiconbutton.h"
#include <QGridLayout>
#include "mapsettings.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>
#include <QScreen>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("Orientir");
    QString iniFile = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    qDebug() << iniFile;
    QSettings settings(iniFile+"/settings.ini",QSettings::IniFormat);
    int w = settings.value("window/width", 800).toInt();
    int h = settings.value("window/height", 600).toInt();
    bool winMax = settings.value("window/max", false).toBool();
    if(winMax){
        showMaximized();
    }else{
        QRect rect = QApplication::primaryScreen()->geometry();
        setGeometry(rect.width()/2-w/2, rect.height()/2-h/2, w, h);
    }



    QFile output("exmample.xml");
    output.open(QIODevice::WriteOnly |QIODevice::Text);
    QXmlStreamWriter stream(&output);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("maps");
    stream.writeStartElement("map");

    stream.writeTextElement("title", "Первенство...");
    stream.writeTextElement("origin", "C:/123/1.png");
    stream.writeTextElement("small", "C:/123/1small.png");
    stream.writeTextElement("size", "1000");
    stream.writeTextElement("metrpx", "10");
    stream.writeTextElement("description", "sjslkdjfl skdfj\n slkdj\n fklsjdfl");
    stream.writeEndElement(); // map

    stream.writeStartElement("map");
    stream.writeTextElement("title", "Чемпионат...");
    stream.writeTextElement("origin", "C:/123/12.png");
    stream.writeTextElement("small", "C:/123/12small.png");
    stream.writeTextElement("size", "10000");
    stream.writeTextElement("metrpx", "20");
    stream.writeTextElement("description", "sjs33333lkdjfl skdfj\n slkdj\n fklsjdfl");
    stream.writeEndElement(); // map
    stream.writeEndElement(); // maps
    stream.writeEndDocument();
    output.close();


    QFile file("exmample.xml");
    file.open(QIODevice::ReadOnly |QIODevice::Text);
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();
    QString map;
    while(!xmlReader.atEnd())
    {
        if(xmlReader.isStartElement())
        {
            if(xmlReader.name().toString()=="map"){
                if(!map.isEmpty()){
                    qDebug() << map;
                }
                map.clear();

            }else if(xmlReader.name().toString() == "title"){
                map += xmlReader.readElementText();
            }
            else if(xmlReader.name().toString() == "size"){
                map += xmlReader.readElementText();
            }
        }
        xmlReader.readNext();
    }
    qDebug() << map;
    file.close();


    StyleHelper::setFonts();
    ui->mapPage->setStyleSheet(StyleHelper::getMapStyleDark());
    StyleHelper::setToolButtonStyleDark(ui->backButton, StyleHelper::MapIconsType::Back,false);
    StyleHelper::setToolButtonStyleDark(ui->pathButton, StyleHelper::MapIconsType::Path,false);
    StyleHelper::setToolButtonStyleDark(ui->rulerButton, StyleHelper::MapIconsType::Ruler,false);
    StyleHelper::setToolButtonStyleDark(ui->startButton, StyleHelper::MapIconsType::Start,false);
    StyleHelper::setToolButtonStyleDark(ui->moveButton, StyleHelper::MapIconsType::Move,false);

    ui->pathButton->setProperty("MapIconsType", StyleHelper::MapIconsType::Path);
    ui->rulerButton->setProperty("MapIconsType", StyleHelper::MapIconsType::Ruler);
    ui->moveButton->setProperty("MapIconsType", StyleHelper::MapIconsType::Move);

    connect(ui->pathButton, &QToolButton::clicked, this, &Widget::changCurrentToolSlot);
    connect(ui->rulerButton, &QToolButton::clicked, this, &Widget::changCurrentToolSlot);
    connect(ui->moveButton, &QToolButton::clicked, this, &Widget::changCurrentToolSlot);
    currentTool = nullptr;

    mapScene = new MapScene(this);
    mapScene->setBackgroundBrush(QColor(200,200,210));
    ui->mapView->setScene(mapScene);

    QPixmap pix(":/resourses/maps/map2.jpg");
    QGraphicsPixmapItem* imageItem = new QGraphicsPixmapItem(pix);

    ui->mapView->setSceneRect(0,0,pix.width(), pix.height());
    mapScene->setMapItem(imageItem);
    mapScene->addItem(imageItem);
    mapSizeValue = 100;
    connect(ui->plusButton,&QPushButton::clicked,this,&Widget::scaleSceneSlot);
    connect(ui->minusButton,&QPushButton::clicked,this,&Widget::scaleSceneSlot);
    mapScene->setStatistic(ui->TreeStatisticWidget);

    connect(ui->KPColorButton,    &QPushButton::clicked, this, &Widget::ColorButtonSlot);
    connect(ui->KPNumColorButton, &QPushButton::clicked, this, &Widget::ColorButtonSlot);
    connect(ui->StartColorButton, &QPushButton::clicked, this, &Widget::ColorButtonSlot);
    connect(ui->LineColorButton,  &QPushButton::clicked, this, &Widget::ColorButtonSlot);


    connect(mapScene, &MapScene::zoomSignal, ui->plusButton,&QPushButton::click);
    connect(mapScene, &MapScene::unzoomSignal, ui->minusButton,&QPushButton::click);

    QListView* listview = new QListView;
    QFont font = listview->font();
    font.setFamily("Comfortaa");
    font.setPointSize(10);
    listview->setFont(font);
    ui->SettingsComboBox->setView(listview);

    ui->moveButton->click();

    connect(ui->SettingsComboBox, &QComboBox::currentIndexChanged, this, &Widget::settingsComboBoxSlot);

    settingsComboBoxSlot(0);

    settingsInit();


    endPathButton = new QPushButton("Завершить",this);
    endPathButton->move(10,ui->topPanelWidget->height()+10);
    endPathButton->setStyleSheet(StyleHelper::getEndPathButtonStyle());
    endPathButton->hide();
    connect(mapScene, &MapScene::removeAllMapPointsSignal, endPathButton, &QWidget::hide);
    connect(endPathButton, &QPushButton::clicked, this, &Widget::endButtonPointSlot);

    endRulerButton = new QPushButton("Завершить измерения", this);
    endRulerButton->setGeometry(0, 0, 150, 30);
    endRulerButton->move(10, ui->topPanelWidget->height() + 10);
    endRulerButton->setStyleSheet(StyleHelper::getEndPathButtonStyle());
    endRulerButton->hide();
    connect(endRulerButton, &QPushButton::clicked, this, &Widget::endButtonRulerSlot);

    connect(mapScene, &MapScene::addStartPointSignal,
            this, &Widget::addStartPointSlot);
    //connect(endPathButton, &QPushButton::clicked, this, &Widget::endPathSlot);
    connect(ui->backButton, &QToolButton::clicked,
            this, &Widget::backButtonSlot);

    grid = new QGridLayout;
    ui->scrollAreaWidgetContents->setLayout(grid);
    for(int i=0;i<8;i++){
        MapIconButton* iconBtn = new MapIconButton;
        iconBtn->setStyleSheet(StyleHelper::getMapIconButtonStyle());
        iconBtn->setFixedSize(320,280);
        grid->addWidget(iconBtn, i/4, i%4);
        connect(iconBtn, &MapIconButton::openMap, this, &Widget::openMapSlot);

    }



    ui->KPNumSpinBox->setStyleSheet(StyleHelper::getSpinBoxStyle());
    ui->KPSizeSpinBox->setStyleSheet(StyleHelper::getSpinBoxStyle());
    ui->KPWidthSpinBox->setStyleSheet(StyleHelper::getSpinBoxStyle());
    ui->StartSizeSpinBox->setStyleSheet(StyleHelper::getSpinBoxStyle());
    ui->StartWidthSpinBox->setStyleSheet(StyleHelper::getSpinBoxStyle());
    ui->LineWidthSpinBox->setStyleSheet(StyleHelper::getSpinBoxStyle());
    ui->mapsListPage->setStyleSheet(StyleHelper::getMapListStyleDark());

    connect(ui->KPNumSpinBox, &QSpinBox::valueChanged, this, &Widget::SizeSpinBoxSlot);
    connect(ui->KPSizeSpinBox, &QSpinBox::valueChanged, this, &Widget::SizeSpinBoxSlot);
    connect(ui->StartSizeSpinBox, &QSpinBox::valueChanged, this, &Widget::SizeSpinBoxSlot);
    connect(ui->KPWidthSpinBox, &QSpinBox::valueChanged, this, &Widget::SizeSpinBoxSlot);
    connect(ui->StartWidthSpinBox, &QSpinBox::valueChanged, this, &Widget::SizeSpinBoxSlot);
    connect(ui->LineWidthSpinBox, &QSpinBox::valueChanged, this, &Widget::SizeSpinBoxSlot);

    connect(ui->CursorSizeComboBox, &QComboBox::currentTextChanged, this,  &Widget::setCursorSlot);
    connect(ui->CursorColorComboBox, &QComboBox::currentTextChanged, this, &Widget::setCursorSlot);
    connect(ui->addNewMapButton, &QToolButton::clicked, this, &Widget::addNewMapButtonSlot);

    StyleHelper::setToolButtonStyleDark(ui->addNewMapButton, StyleHelper::MapIconsType::Add,false);
    connect(mapScene, &MapScene::startRulerModeSignal, this, &Widget::startRulerModeSlot);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event)
{
    QString iniFile = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    qDebug() << iniFile;
    QSettings settings(iniFile+"/settings.ini",QSettings::IniFormat);
    settings.setValue("window/max",isMaximized());
    settings.setValue("window/height", height());
    settings.setValue("window/width",width());
}

void Widget::changCurrentToolSlot()
{
    //using CType    = MapApl::CursorType;
    using MapIcons = StyleHelper::MapIconsType;
    QToolButton* btn = qobject_cast<QToolButton*>(sender());
    if(currentTool== btn)
        return;
    //endPathButton->hide();
    //endRulerButton->hide();
    StyleHelper::MapIconsType type;
    if(currentTool != nullptr){
        type = static_cast<StyleHelper::MapIconsType>(currentTool->property("MapIconsType").toUInt());
        StyleHelper::setToolButtonStyleDark(currentTool, type,false);
    }
    type = static_cast<StyleHelper::MapIconsType>(btn->property("MapIconsType").toUInt());
    StyleHelper::setToolButtonStyleDark(btn, type,true);
    if(type == MapIcons::Ruler){
        endPathButton->move(10, ui->topPanelWidget->height() + 10);
    }
    currentTool = btn;
    switch (type) {
    case MapIcons::Move:
        qDebug() << "lskjdf";

        toolMode = MapApl::ToolType::Move;

        ui->mapView->setToolCursor(Cursors::ToolCursor::Move);

        ui->mapView->setCursorType();
        ui->mapView->setDragMode(QGraphicsView::ScrollHandDrag);
        mapScene->setCurrentToolType(MapScene::ToolType::Move);
        break;
    case MapIcons::Path:

        toolMode = MapApl::ToolType::Path;

        ui->mapView->setToolCursor(Cursors::ToolCursor::Point);

        ui->mapView->setDragMode(QGraphicsView::NoDrag);
        ui->mapView->setCursorType();
        mapScene->setCurrentToolType(MapScene::ToolType::Path);
        break;
    case MapIcons::Ruler:

        toolMode = MapApl::ToolType::Ruler;

        ui->mapView->setToolCursor(Cursors::ToolCursor::Ruler);

        ui->mapView->setDragMode(QGraphicsView::NoDrag);
        ui->mapView->setCursorType();
        mapScene->setCurrentToolType(MapScene::ToolType::Ruler);

        endPathButton->move(10, ui->topPanelWidget->height() + 50);
        endRulerButton->show();
        break;

    default:


        ui->mapView->setToolCursor(Cursors::ToolCursor::Default);

        ui->mapView->setDragMode(QGraphicsView::NoDrag);
        ui->mapView->setCursorType();
        mapScene->setCurrentToolType(MapScene::ToolType::Default);
    }


}

void Widget::scaleSceneSlot()
{
    if(sender()->objectName()=="plusButton"){
        if(mapSizeValue >= 200){
            return;
        }
        mapSizeValue+=10;
    }else{

        if(mapSizeValue <= 0){
            return;
        }
        mapSizeValue-=10;
    }

    qreal scale = qPow(qreal(2), (mapSizeValue-100) / qreal(100));

    QTransform matrix;
    matrix.scale(scale, scale);
    matrix.rotate(0);

    ui->mapView->setTransform(matrix);
}

void Widget::ColorButtonSlot()
{
    QColor oldColor(sender()->property("ColorName").toString());
    QColor newColor = QColorDialog::getColor(oldColor, this, "Выбор цвета");
    if(!newColor.isValid()) return;
    QString name = sender()->objectName();
    if(name == "KPNumColorButton"){
        mapScene->setKPNumColor(newColor);
        ui->KPNumColorButton->setProperty("ColorName", newColor.name());
        ui->KPNumColorButton->setStyleSheet("background:" + newColor.name());
        ui->KPNumColorButton->setStyleSheet(StyleHelper::getColorButtonStyle(newColor.name()));
    }else if(name == "KPColorButton"){
        mapScene->setKPColor(newColor);
        ui->KPColorButton->setProperty("ColorName", newColor.name());
        ui->KPColorButton->setStyleSheet("background:" + newColor.name());
        ui->KPColorButton->setStyleSheet(StyleHelper::getColorButtonStyle(newColor.name()));
    }else if(name == "StartColorButton"){
        mapScene->setStartColor(newColor);
        ui->StartColorButton->setProperty("ColorName", newColor.name());
        ui->StartColorButton->setStyleSheet("background:" + newColor.name());
        ui->StartColorButton->setStyleSheet(StyleHelper::getColorButtonStyle(newColor.name()));
    }else if(name == "LineColorButton"){
        mapScene->setLineColor(newColor);
        ui->LineColorButton->setProperty("ColorName", newColor.name());
        ui->LineColorButton->setStyleSheet("background:" + newColor.name());
        ui->LineColorButton->setStyleSheet(StyleHelper::getColorButtonStyle(newColor.name()));
    }
}

void Widget::settingsComboBoxSlot(int index)
{
    QList<QWidget*> widgets = ui->tab->findChildren<QWidget*>(Qt::FindDirectChildrenOnly);
    for(QWidget* w: widgets){
        if(index == 1){
            w->setEnabled(true);
        }else if(index == 0){
            QString name = w->objectName();
            if(name == "SettingsComboBox" || name.indexOf("Cursor") != -1){
                continue;
            }
            w->setEnabled(false);
        }
    }
}

void Widget::addStartPointSlot()
{
    endPathButton->show();
    //endPathButtonHide = false;
}

void Widget::endPathSlot()
{
    if(mapScene->getPointCount() == 1){
        mapScene->deletePoints();
    }else{
        mapScene->setFinishPoint();
    }
}

void Widget::backButtonSlot()
{
    endPathButton->hide();
    endPathButtonHide = true;
    ui->stackedWidget->setCurrentWidget(ui->mapsListPage);
}

void Widget::endButtonPointSlot()
{
    endPathButton->hide();
    endPathButtonHide = true;
    if(toolMode == MapApl::ToolType::Path){
        if(mapScene->getPointCount() == 1){
            mapScene->deletePoints();
            return;
        }
        mapScene->setFinishPointFlag(true);
        mapScene->setFinishPoint();
    }else if(toolMode == MapApl::ToolType::Ruler){
        mapScene->finishRulerMode();
    }
}

void Widget::openMapSlot()
{
    ui->stackedWidget->setCurrentWidget(ui->mapPage);
}

void Widget::SizeSpinBoxSlot(int value)
{
    qDebug() << "SpinBox";
    QString name = sender()->objectName();
    if(name == "KPNumSpinBox"){
        ui->KPNumSpinBox->setValue(value);
        mapScene->setKPNumSize(value);
    }else if(name == "KPSizeSpinBox"){
        ui->KPSizeSpinBox->setValue(value);
        mapScene->setKPSize(value);
    }else if(name == "StartSizeSpinBox"){
        ui->StartSizeSpinBox->setValue(value);
        mapScene->setStartSize(value);
    }else if(name == "KPWidthSpinBox"){
        ui->KPWidthSpinBox->setValue(value);
        mapScene->setKPWidth(value);
    }else if(name == "StartWidthSpinBox"){
        ui->StartWidthSpinBox->setValue(value);
        mapScene->setStartWidth(value);
    }else if(name == "LineWidthSpinBox"){
        ui->LineWidthSpinBox->setValue(value);
        mapScene->setLineWidth(value);
    }
}

void Widget::setCursorSlot(const QString& str)
{
    if(sender()->objectName() == "CursorSizeComboBox"){
        qDebug() << "Size";
        if(str == "Нормальный"){
            ui->mapView->setCursorSize(Cursors::Size::Normal);
        }else if(str == "Крупный"){
            ui->mapView->setCursorSize(Cursors::Size::Big);
        }
    }else if(sender()->objectName() == "CursorColorComboBox"){
        qDebug() << "Color";
        if(str == "Черно-белый"){
            ui->mapView->setCursorColor(Cursors::Color::WhiteAndBlack);
        }else if(str == "Красно-белый"){
            ui->mapView->setCursorColor(Cursors::Color::RedAndWhite);
        }else if(str == "Красно-черный"){
            ui->mapView->setCursorColor(Cursors::Color::RedAndBlack);
        }
    }
}



void Widget::startRulerModeSlot()
{
    endPathButton->show();
    endPathButtonHide = false;
}

void Widget::addNewMapButtonSlot()
{
    MapSettings dlg(this);
    if(dlg.exec()==QDialog::Accepted){

    }
}

void Widget::endButtonRulerSlot()
{
    endRulerButton->hide();
    if(!endPathButtonHide){
        endButtonPointSlot();
    }
    mapScene->endRuler();
    ui->moveButton->click();
}

void Widget::settingsInit()
{
    //Номера КП
    ui->KPNumSpinBox->setValue(30);
    mapScene->setKPNumSize(30);

    ui->KPNumStyleComboBox->setCurrentIndex(0);
    mapScene->setKPNumStyle(0);

    ui->KPNumColorButton->setProperty("ColorName", "#ff0000");
    ui->KPNumColorButton->setStyleSheet(StyleHelper::getColorButtonStyle("#ff0000"));
    mapScene->setKPNumColor(Qt::red);
    //КП
    ui->KPSizeSpinBox->setValue(40);
    mapScene->setKPSize(40);

    ui->KPWidthSpinBox->setValue(3);
    mapScene->setKPWidth(3);

    ui->KPColorButton->setProperty("ColorName", "#ff0000");
    ui->KPColorButton->setStyleSheet(StyleHelper::getColorButtonStyle("#ff0000"));
    mapScene->setKPColor(Qt::red);
    //Старт/Финиш
    ui->StartSizeSpinBox->setValue(40);
    mapScene->setStartSize(40);

    ui->StartWidthSpinBox->setValue(3);
    mapScene->setStartWidth(3);

    ui->StartColorButton->setProperty("ColorName", "#ff0000");
    ui->StartColorButton->setStyleSheet(StyleHelper::getColorButtonStyle("#ff0000"));
    mapScene->setStartColor(Qt::red);
    //Линии
    ui->LineWidthSpinBox->setValue(3);
    mapScene->setLineWidth(3);

    ui->LineColorButton->setProperty("ColorName", "#ff0000");
    ui->LineColorButton->setStyleSheet(StyleHelper::getColorButtonStyle("#ff0000"));
    mapScene->setLineColor(Qt::red);

}

void Widget::setRulerMode()
{
    mapScene->setCurrentToolType(MapScene::ToolType::Ruler);
}



