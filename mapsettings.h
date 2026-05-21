#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "mapcropborderitem.h"
#include "linerpoint.h"
#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>

class CoordinationScene;
namespace Ui {
class MapSettings;
}

class QPixmap;

class MapSettings : public QDialog
{
    Q_OBJECT

public:
    explicit MapSettings(QWidget *parent = nullptr);
    ~MapSettings();



    QString getTitle() const;
    void setTitle(const QString &newTitle);

    QString getDescription() const;
    void setDescription(const QString &newDescription);

    QString getOriginPath() const;
    void setOriginPath(const QString &newOriginPath);

    QString getPrevPath() const;
    void setPrevPath(const QString &newPrevPath);

    int getSz() const;

    int getPixSize() const;

    qreal getX_0() const;


    qreal getY_0() const;

    QString getSrtmPath() const;

private slots:
    void on_addButton_clicked();
    void on_cancellButton_clicked();
    void on_selectButton_clicked();
    void on_pushButton_3_clicked();

    void updatePointPositonSlot();

    void on_pushButton_9_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_selectButtonHeights_clicked();

private:
    Ui::MapSettings *ui;
    QPixmap* originImg = nullptr;
    QPixmap* prevImg = nullptr;

    QGraphicsScene* cropScene;
    MapCropBorderItem* cropItem;
    QGraphicsPixmapItem* pixMapItem;

    LinerPoint* point_1;
    LinerPoint* point_2;
    QGraphicsLineItem* pointsLine;
    QGraphicsSimpleTextItem* pointText;

    qreal metrOnePixel = 5.0;
    QString title;
    QString description;
    QString originPath;
    QString prevPath;
    int sz;
    int pixSize;

    int xPrev = 0;
    int yPrev = 0;

    void initPage_4();
    CoordinationScene* pointsScene;

    qreal x_0;
    qreal y_0;
    QString srtmPath = "";
};

#endif // MAPSETTINGS_H
