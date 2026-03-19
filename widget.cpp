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
#include <QDateTime>
#include <QTextStream>

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

    logger = new Logger;
    logger->setLogPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs/log_" + QDateTime::currentDateTime().toString("yyyy_mm_dd_hh_mm_ss") + ".log");
    logger->message("Start work", Fatal::Message::Debug);


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
    mapScene->setLogger(logger);

    QPixmap pix(":/resourses/maps/map5.jpg");
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
    connect(mapScene, &MapScene::addStartPointSignal, this, [this](){this->endPathMode = false;});
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
    getSaveMapInit();
    /*
    MapIconButton* iconBtn = new MapIconButton;
    iconBtn->setStyleSheet(StyleHelper::getMapIconButtonStyle());
    iconBtn->setFixedSize(320,280);
    iconBtn->setData("Открытый кубок Пермского края", "", ":/resourses/maps/map1.png",":/resourses/maps/map1-s.png",1000,20);
    grid->addWidget(iconBtn, 0, 0);
    connect(iconBtn, &MapIconButton::openMap, this, &Widget::openMapSlot);
    ++countMaps;

    MapIconButton* iconBtn2 = new MapIconButton;
    iconBtn2->setStyleSheet(StyleHelper::getMapIconButtonStyle());
    iconBtn2->setFixedSize(320,280);
    iconBtn2->setData("A-Zel Series", "", ":/resourses/maps/map2.jpg",":/resourses/maps/map2-s.png",1000,20);
    grid->addWidget(iconBtn2, 0, 1);
    connect(iconBtn2, &MapIconButton::openMap, this, &Widget::openMapSlot);
    ++countMaps;

    MapIconButton* iconBtn3 = new MapIconButton;
    iconBtn3->setStyleSheet(StyleHelper::getMapIconButtonStyle());
    iconBtn3->setFixedSize(320,280);
    iconBtn3->setData("Покровский остров", "", ":/resourses/maps/map3.jpg",":/resourses/maps/map3-s.png",1000,20);
    grid->addWidget(iconBtn3, 0, 2);
    connect(iconBtn3, &MapIconButton::openMap, this, &Widget::openMapSlot);
    ++countMaps;

    MapIconButton* iconBtn4 = new MapIconButton;
    iconBtn4->setStyleSheet(StyleHelper::getMapIconButtonStyle());
    iconBtn4->setFixedSize(320,280);
    iconBtn4->setData("Легенда Адыгеи", "", ":/resourses/maps/map5.jpg",":/resourses/maps/map4-s.png",1000,20);
    grid->addWidget(iconBtn4, 0, 3);
    connect(iconBtn4, &MapIconButton::openMap, this, &Widget::openMapSlot);
    ++countMaps;
    */





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

    ui->TreeStatisticWidget->setStyleSheet(StyleHelper::getMapStatisticStyle());
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
    saveMaps();

    logger->message("Close program", Fatal::Message::Debug);
    logger->closeFile();
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

    if(!endRulerMode && type != MapIcons::Ruler){
        endRulerButton->hide();
        endRulerMode = true;
        if(!endPathButtonHide){
            endButtonPointSlot();
        }
        mapScene->endRuler();
    }

    switch (type) {
    case MapIcons::Move:
        logger->message("Start select move mode", Fatal::Message::Debug);

        toolMode = MapApl::ToolType::Move;

        ui->mapView->setToolCursor(Cursors::ToolCursor::Move);

        ui->mapView->setCursorType();
        ui->mapView->setDragMode(QGraphicsView::ScrollHandDrag);
        mapScene->setCurrentToolType(MapScene::ToolType::Move);

        logger->message("Finish select move mode", Fatal::Message::Debug);
        break;
    case MapIcons::Path:

        logger->message("Start select path mode", Fatal::Message::Debug);

        toolMode = MapApl::ToolType::Path;

        ui->mapView->setToolCursor(Cursors::ToolCursor::Point);

        ui->mapView->setDragMode(QGraphicsView::NoDrag);
        ui->mapView->setCursorType();
        mapScene->setCurrentToolType(MapScene::ToolType::Path);

        logger->message("Finish select move mode", Fatal::Message::Debug);
        break;
    case MapIcons::Ruler:

        logger->message("Start select ruler mode", Fatal::Message::Debug);

        toolMode = MapApl::ToolType::Ruler;

        ui->mapView->setToolCursor(Cursors::ToolCursor::Ruler);

        ui->mapView->setDragMode(QGraphicsView::NoDrag);
        ui->mapView->setCursorType();
        mapScene->setCurrentToolType(MapScene::ToolType::Ruler);

        endPathButton->move(10, ui->topPanelWidget->height() + 50);
        endRulerButton->show();
        mapScene->startRulerModeStart();

        endRulerMode = false;

        logger->message("Finish select ruler mode", Fatal::Message::Debug);
        break;

    default:
        logger->message("Start select default mode", Fatal::Message::Debug);

        ui->mapView->setToolCursor(Cursors::ToolCursor::Default);

        ui->mapView->setDragMode(QGraphicsView::NoDrag);
        ui->mapView->setCursorType();
        mapScene->setCurrentToolType(MapScene::ToolType::Default);

        logger->message("Finish select default mode", Fatal::Message::Debug);
    }



}

void Widget::scaleSceneSlot()
{
    logger->message("Start scale(button) scene", Fatal::Message::Debug);

    if(sender()->objectName()=="plusButton"){
        if(mapSizeValue >= 200){
            return;
        }
        mapSizeValue+=10;
    }else{

        if(mapSizeValue <= -200){
            return;
        }
        mapSizeValue-=10;
    }

    qreal scale = qPow(qreal(2), (mapSizeValue-100) / qreal(100));

    QTransform matrix;
    matrix.scale(scale, scale);
    matrix.rotate(0);

    ui->mapView->setTransform(matrix);

    ui->zoomValueLabel->setText(QString::number(static_cast<int>(scale * 100)) + "%");

    logger->message("Finish scale(button) scene", Fatal::Message::Debug);
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
    endRulerButton->click();
    endPathButton->hide();
    endPathButtonHide = true;
    ui->stackedWidget->setCurrentWidget(ui->mapsListPage);
}

void Widget::endButtonPointSlot()
{
    endPathButton->hide();
    endPathButtonHide = true;
    endPathMode = true;
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

void Widget:: openMapSlot()
{
    ui->stackedWidget->setCurrentWidget(ui->mapPage);
    MapIconButton* btn = qobject_cast<MapIconButton*> (sender());
    QString originPath = btn->getOrigingPath();
    QPixmap pix(originPath);
    QGraphicsPixmapItem* imageItem = new QGraphicsPixmapItem(pix);

    ui->mapView->setSceneRect(0,0,pix.width(), pix.height());
    QGraphicsItem* item = mapScene->getMapItem();
    mapScene->removeItem(item);
    mapScene->setMapItem(imageItem);
    mapScene->addItem(imageItem);
    imageItem->setZValue(-1);
    mapSizeValue = 100;
    QTransform matrix;
    matrix.scale(1, 1);
    matrix.rotate(0);
    ui->mapView->setTransform(matrix);
    ui->zoomValueLabel->setText("100%");

    ui->mapNameLabel->setText(btn->getTitle()+" 1:"+QString::number(btn->getSz()));

    if(!endPathMode) endPathButton->show();
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
        MapIconButton* iconBtn = new MapIconButton;
        iconBtn->setStyleSheet(StyleHelper::getMapIconButtonStyle());
        iconBtn->setFixedSize(320,280);
        QString originPath = dlg.getOriginPath();
        QString prevPath = dlg.getPrevPath();
        iconBtn->setData(dlg.getTitle(), dlg.getDescription(), originPath, prevPath, dlg.getSz(), dlg.getPixSize());
        qDebug() <<  "count=" << grid->count();
        iconBtn->setIndex(countMaps);
        maps.push_back(iconBtn);
        grid->addWidget(iconBtn, (countMaps - countMaps % 5) / 5, countMaps % 5);
        connect(iconBtn, &MapIconButton::openMap, this, &Widget::openMapSlot);
        ++countMaps;
        iconBtn->setLogger(logger);
    }
}

void Widget::deleteMapButtonSlot(int index)
{
    maps.remove(index);
}

void Widget::endButtonRulerSlot()
{
    endRulerButton->hide();
    endRulerMode = true;
    if(!endPathButtonHide){
        endButtonPointSlot();
    }
    mapScene->endRuler();
    ui->moveButton->click();

}

void Widget::settingsInit()
{
    QColor allColor;
    allColor.setNamedColor(QString("#8e2cff"));
    //Номера КП
    ui->KPNumSpinBox->setValue(60);
    mapScene->setKPNumSize(60);

    ui->KPNumStyleComboBox->setCurrentIndex(0);
    mapScene->setKPNumStyle(0);

    ui->KPNumColorButton->setProperty("ColorName", "#8e2cff");
    ui->KPNumColorButton->setStyleSheet(StyleHelper::getColorButtonStyle("#8e2cff"));
    mapScene->setKPNumColor(allColor);
    //КП
    ui->KPSizeSpinBox->setValue(180);
    mapScene->setKPSize(180);

    ui->KPWidthSpinBox->setValue(10);
    mapScene->setKPWidth(10);

    ui->KPColorButton->setProperty("ColorName", "#8e2cff");
    ui->KPColorButton->setStyleSheet(StyleHelper::getColorButtonStyle("#8e2cff"));

    mapScene->setKPColor(allColor);
    //Старт/Финиш
    ui->StartSizeSpinBox->setValue(180);
    mapScene->setStartSize(180);

    ui->StartWidthSpinBox->setValue(10);
    mapScene->setStartWidth(10);

    ui->StartColorButton->setProperty("ColorName", "#8e2cff");
    ui->StartColorButton->setStyleSheet(StyleHelper::getColorButtonStyle("#8e2cff"));
    mapScene->setStartColor(allColor);
    //Линии
    ui->LineWidthSpinBox->setValue(10);
    mapScene->setLineWidth(10);

    ui->LineColorButton->setProperty("ColorName", "#8e2cff");
    ui->LineColorButton->setStyleSheet(StyleHelper::getColorButtonStyle("#8e2cff"));
    mapScene->setLineColor(allColor);

}

void Widget::getSaveMapInit()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString path = dataPath + "/savemap.xml";
    QFile saveMapFile(path);
    if(!saveMapFile.open(QIODevice::ReadOnly |QIODevice::Text)){
        qDebug() << "Error open saveMap file";
        return;
    }
    QXmlStreamReader reader;
    reader.setDevice(&saveMapFile);
    reader.readNext();
    MapIconButton* map = new MapIconButton;

    while(!reader.atEnd()){
        if(reader.isStartElement()){
            QString t = reader.name().toString();
            if(t == "map"){
                if(map->isNormal()){
                    grid->addWidget(map, (countMaps - countMaps % 5) / 5, countMaps % 5);
                    connect(map, &MapIconButton::openMap, this, &Widget::openMapSlot);
                    connect(map, &MapIconButton::deleteMap, this, &Widget::deleteMapButtonSlot);
                    map->setIndex(countMaps);
                    ++countMaps;
                    maps.push_back(map);
                    map->setLogger(logger);
                }else{
                    delete map;
                }
                map = new MapIconButton;
                map->setStyleSheet(StyleHelper::getMapIconButtonStyle());
                map->setFixedSize(320,280);
            }else if(t == "title"){
                map->setTitle(reader.readElementText());
            }else if(t == "origin"){
                map->setOrigingPath(reader.readElementText());
            }else if(t == "small"){
                map->setPrevPath(reader.readElementText());
            }else if(t == "size"){
                map->setSz(reader.readElementText().toInt());
            }else if(t == "scale"){
                map->setPixSize(reader.readElementText().toInt());
            }else if(t == "description"){
                map->setDescription(reader.readElementText());
            }
        }
        reader.readNext();
    }
    if(map->isNormal()){
        grid->addWidget(map, (countMaps - countMaps % 5) / 5, countMaps % 5);
        connect(map, &MapIconButton::openMap, this, &Widget::openMapSlot);
        connect(map, &MapIconButton::deleteMap, this, &Widget::deleteMapButtonSlot);
        map->setIndex(countMaps);
        ++countMaps;
        maps.push_back(map);
        map->setLogger(logger);
    }else{
        delete map;
    }
    saveMapFile.close();
    /*
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
    */
}

void Widget::saveMaps()
{
    /*
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
    */
    qDebug() << "saveMaps";
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString path = dataPath + "/savemap.xml";
    QFile saveMapFile(path);
    if(!saveMapFile.open(QIODevice::WriteOnly |QIODevice::Text)){
        qDebug() << "Error open saveMap file";
        return;
    }
    QXmlStreamWriter output(&saveMapFile);
    output.setAutoFormatting(true);
    output.writeStartDocument();
    output.writeStartElement("maps");
    for(MapIconButton* map: maps){
        output.writeStartElement("map");
        output.writeTextElement("title", map->getTitle());
        output.writeTextElement("origin", map->getOrigingPath());
        output.writeTextElement("small", map->getPrevPath());
        output.writeTextElement("size", QString::number(map->getSz()));
        output.writeTextElement("scale", QString::number(map->getPixSize()));
        output.writeTextElement("description", map->getDescription());
        output.writeEndElement();
    }
    output.writeEndElement();
    output.writeEndDocument();
    saveMapFile.close();

}

void Widget::setRulerMode()
{
    mapScene->setCurrentToolType(MapScene::ToolType::Ruler);
}

// void Widget::writeLogFile(QString msg, Fatal::Message type)
// {

// }



