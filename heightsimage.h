#ifndef HEIGHTSIMAGE_H
#define HEIGHTSIMAGE_H

#include <QObject>
#include <QGraphicsPixmapItem>

class HeightsImage : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit HeightsImage(QObject *parent = nullptr);
    ~HeightsImage();


signals:
};

#endif // HEIGHTSIMAGE_H
