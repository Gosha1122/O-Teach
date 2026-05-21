#include "mapsettings.h"
#include "ui_mapsettings.h"
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>
#include <QPixmap>
#include <QStandardPaths>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QCursor>
#include "stylehelper.h"
#include <QScreen>
#include <QIcon>
#include <QProcess>
#include "coordinationpoint.h"
#include "coordinationscene.h"

MapSettings::MapSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MapSettings)
{
    ui->setupUi(this);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cropScene = new QGraphicsScene;
    cropItem = new MapCropBorderItem;
    ui->graphicsView->setSceneRect(0,0,540, 310);
    ui->graphicsView->setScene(cropScene);
    cropScene->addItem(cropItem);
    setFixedSize(577,428);
    ui->stackedWidget->setCurrentWidget(ui->page_1);

    point_1 = new LinerPoint;
    point_2 = new LinerPoint;
    pointsLine = new QGraphicsLineItem;
    pointText = new QGraphicsSimpleTextItem(point_1);
    pointText->setText("10");
    pointText->setBrush(QBrush(Qt::red));
    QFont font = pointText->font();
    font.setPixelSize(14);
    pointText->setFont(font);
    pointText->setPos(-25,-30);
    cropScene->addItem(pointsLine);
    cropScene->addItem(point_1);
    cropScene->addItem(point_2);
    connect(point_1, &LinerPoint::updatePositionSignal, this, &MapSettings::updatePointPositonSlot);
    connect(point_2, &LinerPoint::updatePositionSignal, this, &MapSettings::updatePointPositonSlot);

    point_1->hide();
    point_2->hide();
    pointsLine->hide();

    this->setStyleSheet(StyleHelper::getMapSettingsStyle());
    ui->metrsSpinBox->setStyleSheet(StyleHelper::getSpinBoxStyle());
}

MapSettings::~MapSettings()
{
    delete ui;
    delete originImg;
    delete prevImg;
}

void MapSettings::on_addButton_clicked()
{

    QString error;
    if(ui->nameEdit->text().trimmed().isEmpty()){
        error+= "Поле Название карты не может быть пустым\n";
    }
    if(ui->sizeEdit->text().trimmed().isEmpty()){
        error+= "Поле Масштаб не может быть пустым\n";
    }
    if(ui->pathEdit->text().trimmed().isEmpty()){
        error+= "Не выбрана карта\n";
    }else if(!QFile::exists(ui->pathEdit->text().trimmed())){
        error+= "Не существует изображения карты\n";
    }
    if(!error.isEmpty()){
        QMessageBox msbox(this);
        msbox.setWindowTitle("Ошибка");
        msbox.setText(error);
        msbox.setStyleSheet(StyleHelper::getMessageBoxStyle());
        msbox.setIcon(QMessageBox::Warning);
        msbox.exec();
        return;
    }
    ui->stepImageLabel->setPixmap(QPixmap(":/resourses/icons/step_2.jpg"));
    if(originImg->load(originPath)){
        pixMapItem = new QGraphicsPixmapItem(QPixmap(originPath));
        pixMapItem->setFlags(QGraphicsItem::ItemIsMovable);
        cropScene->addItem(pixMapItem);
        pixMapItem->setZValue(cropItem->zValue()-1);
        ui->graphicsView->setCursor(Qt::SizeAllCursor);
        srtmPath = ui->pathHeightsEdit->text();

    }
    ui->stackedWidget->setCurrentWidget(ui->page_2);


}


void MapSettings::on_cancellButton_clicked()
{
    this->reject();
}


void MapSettings::on_selectButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Выбор изображения", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), "Images (*.png *.jpg *.jpeg *.webr *.webp *.tiff *.bmp *.svg)");
    if(path.isEmpty()){
        return;
    }
    ui->pathEdit->setText(path);
    originPath = path;

    QProcess process;
    QStringList args;
    QFileInfo info(originPath);
    args << originPath << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +"/maps/map" + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss") + ".jpg";
    process.start("magick.exe", args);
    if(!process.waitForStarted() || !process.waitForFinished()){
        qDebug() << "Error start";
        return;
    }
    originPath = args[1];

    originImg = new QPixmap(originPath);

    qDebug() << originPath;
}


void MapSettings::on_pushButton_3_clicked()
{
    ui->stepImageLabel->setPixmap(QPixmap(":/resourses/icons/step_3.jpg"));
    ui->descriptionLabel->setText("Переместите точки, чтобы указать растояние");
    QPixmap pix;
    xPrev = (pixMapItem->x()<0)? abs(pixMapItem->x())+120: 120-pixMapItem->x();
    yPrev = (pixMapItem->y()<0)? abs(pixMapItem->y())+50: 50-pixMapItem->y();
    pix = originImg->copy(xPrev, yPrev, 300,200);
    prevImg = new QPixmap(pix);
    ui->graphicsView_2->setScene(cropScene);
    cropItem->hide();
    ui->graphicsView_2->setSceneRect(0,0,540, 310);

    point_1->show();
    point_2->show();
    pointsLine->show();
    point_1->setPos(100,100);
    point_2->setPos(150,100);
    point_1->setFlags(QGraphicsItem::ItemIsMovable);
    point_2->setFlags(QGraphicsItem::ItemIsMovable);
    pointsLine->setLine(point_1->x(), point_1->y(),point_2->x(),point_2->y());
    ui->metrsSpinBox->setValue(metrOnePixel* pointsLine->line().length());
    pointText->setText(QString::number(round(pointsLine->line().length() * metrOnePixel))+" м.");
    ui->graphicsView_2->setCursor(Qt::SizeAllCursor);
    point_1->setCursor(Qt::ArrowCursor);
    point_2->setCursor(Qt::ArrowCursor);
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

void MapSettings::updatePointPositonSlot()
{
    pointsLine->setLine(point_1->x(), point_1->y(),point_2->x(),point_2->y());
    pointText->setText(QString::number(round(pointsLine->line().length() * metrOnePixel))+" м.");
}


void MapSettings::on_pushButton_9_clicked()
{
    metrOnePixel = ui->metrsSpinBox->value()/pointsLine->line().length();
    qDebug() << "lenght = " << pointsLine->line().length();
    qDebug() << "lenght/value=" << pointsLine->line().length() << "/"<< ui->metrsSpinBox->value();
    qDebug() << "lenght = " << metrOnePixel;
    pointText->setText(QString::number(ui->metrsSpinBox->value())+" м.");
}


void MapSettings::on_pushButton_6_clicked()
{

    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +"/maps";
    QDir dir(dirPath);
    if(!dir.exists()){
        if(!dir.mkpath(dirPath)){
            qDebug() << "Error create dir" << dirPath;
            return;
        }
    }
    qDebug() << dirPath;
    QString filename = "map" + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");

    qDebug() << originPath << prevPath;

    QProcess process;
    QStringList args;
    QFileInfo info(originPath);
    // args << originPath << dirPath + "/" + filename + ".jpg";
    // process.start("magick.exe", args);
    // if(!process.waitForStarted() || !process.waitForFinished()){
    //     qDebug() << "Error start";
    //     return;
    // }
    // originPath = args[1];

    info.setFile(prevPath);
    args.clear();
    args << originPath << "-crop" << "300x200+" + QString::number(xPrev) + "+" + QString::number(yPrev) << "+repage" << dirPath + "/" + filename + "-s.png";
    process.start("magick.exe", args);
    if(!process.waitForStarted() || !process.waitForFinished()){
        qDebug() << "Error start";
        return;
    }
    prevPath = args[4];

    title = ui->nameEdit->text();
    sz = ui->sizeEdit->text().toInt();
    if(!ui->lineEdit->text().trimmed().isEmpty()){
        pixSize = ui->lineEdit->text().trimmed().toInt();
    }
    ui->stackedWidget->setCurrentWidget(ui->page_4);
    ui->stepImageLabel->setPixmap(QPixmap(":/resourses/icons/step_4.jpg"));
    initPage_4();

}

int MapSettings::getPixSize() const
{
    return pixSize;
}

int MapSettings::getSz() const
{
    return sz;
}

QString MapSettings::getPrevPath() const
{
    return prevPath;
}

void MapSettings::setPrevPath(const QString &newPrevPath)
{
    prevPath = newPrevPath;
}

QString MapSettings::getOriginPath() const
{
    return originPath;
}

void MapSettings::setOriginPath(const QString &newOriginPath)
{
    originPath = newOriginPath;
}

QString MapSettings::getDescription() const
{
    return description;
}

void MapSettings::setDescription(const QString &newDescription)
{
    description = newDescription;
}

QString MapSettings::getTitle() const
{
    return title;
}

void MapSettings::setTitle(const QString &newTitle)
{
    title = newTitle;
}

void MapSettings::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}


void MapSettings::on_pushButton_7_clicked()
{
    QVector<CoordPoint>* points = pointsScene->getPoints();
    int n = points->size();
    qreal sum_x_longetude = 0;
    qreal sum_x2_longetude = 0;
    qreal sum_xy_longetude = 0;
    qreal sum_y_longetude = 0;
    qreal sum_x_latitude = 0;
    qreal sum_x2_latitude = 0;
    qreal sum_xy_latitude = 0;
    qreal sum_y_latitude = 0;
    for(int i = 0; i < n; i++){
        CoordPoint p = points->at(i);
        sum_x_longetude += p.x;
        sum_y_longetude += p.x_0;
        sum_x2_longetude += p.x * p.x;
        sum_xy_longetude += p.x * p.x_0;

        sum_x_latitude  += p.y;
        sum_y_latitude  += p.y_0;
        sum_x2_latitude += p.y * p.y;
        sum_xy_latitude += p.y * p.y_0;
    }
    qreal x_longetude = sum_x_longetude / n;
    qreal x2_longetude = sum_x2_longetude / n;
    qreal y_longetude = sum_y_longetude / n;
    qreal xy_longetude = sum_xy_longetude / n;

    qreal x_latitude = sum_x_latitude / n;
    qreal x2_latitude = sum_x2_latitude / n;
    qreal y_latitude = sum_y_latitude / n;
    qreal xy_latitude = sum_xy_latitude / n;

    y_0 = (x2_latitude * y_latitude - x_latitude * xy_latitude) / (x2_latitude - x_latitude * x_latitude);
    x_0 = (x2_longetude * y_longetude - x_longetude * xy_longetude) / (x2_longetude - x_longetude * x_longetude);
    this->accept();
}


void MapSettings::on_pushButton_10_clicked()
{
    this->reject();
}

void MapSettings::initPage_4()
{
    pointsScene = new CoordinationScene;
    pointsScene->setLatitudeEdit(ui->latitudeEdit);
    pointsScene->setLongetudeEdit(ui->longitudeEdit);
    pointsScene->addPixmap(QPixmap(originPath));
    ui->graphicsView_3->setScene(pointsScene);
    ui->graphicsView_3->setSceneRect(0, 0, QPixmap(originPath).rect().size().width(), QPixmap(originPath).rect().size().height());
    //connect(ui->pushButton_11, &QPushButton::clicked, pointsScene, &CoordinationScene::applyButtonPressed);
}

QString MapSettings::getSrtmPath() const
{
    return srtmPath;
}

qreal MapSettings::getY_0() const
{
    return y_0;
}

qreal MapSettings::getX_0() const
{
    return x_0;
}

void MapSettings::on_pushButton_11_clicked()
{
    pointsScene->applyButtonPressed();
}


void MapSettings::on_selectButtonHeights_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Выбор карты высот", QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), "Srtm(*.hgt)");
    if(path.isEmpty()){
        return;
    }
    ui->pathHeightsEdit->setText(path);
    srtmPath = path;
}

