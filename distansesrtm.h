#ifndef DISTANSESRTM_H
#define DISTANSESRTM_H
#include <QObject>
#include <QPoint>
#include <QImage>
#include <QPixmap>

#define NONEHEIGHT -32768
#define MAXHEIGHT  8850

class DistanseSrtm : public QObject
{
    Q_OBJECT
public:
    explicit DistanseSrtm(QObject *parent = nullptr);

    void initMatrix(QString path);
    int getH(int x, int y, qreal x_0, qreal y_0, qreal scaled);

    int calculateDistance(int x1, int y1, int x2, int y2, qreal scaled, qreal x_0, qreal y_0);

    QVector<QVector<int> > *getHeights() const;

    QImage *getImage() const;

signals:

private:
    QVector<QVector<int>>* heights;
    int n = 0;
    int max = -1e6;
    int min = 1e6;
    QPoint max_p;
    QPoint min_p;
    QImage* image;

    int x_0 = 0;
    int y_0 = 0;

};

#endif // DISTANSESRTM_H
