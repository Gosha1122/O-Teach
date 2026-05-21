#include "distansesrtm.h"

#include <QFile>
#include <QDebug>
#include <QtMath>
#include <QFileInfo>

DistanseSrtm::DistanseSrtm(QObject *parent)
    : QObject{parent}
{}

void DistanseSrtm::initMatrix(QString path)
{
    QFile file(path);

    QFileInfo info(path);
    QString str = info.fileName();
    y_0 = (str[0] == 'N'? 1: -1) * (str.sliced(1, 2).toInt() - 1);
    x_0 = (str[3] == 'E'? 1: -1) * str.sliced(4, 3).toInt();

    if(file.open(QIODevice::ReadOnly)){
        qDebug() << "Open\n";
        QByteArray data = file.readAll();
        file.close();
        int count = data.size();
        if(count > 1201 * 1201 * 2 + 1){
            n = 3600;
        } else {
            n = 1200;
        }

        image = new QImage((new QPixmap(n, n))->toImage());
        heights = new QVector<QVector<int>>(n, QVector<int>(n, NONEHEIGHT));
        for(int r = 0; r < n; r++){
            for(int c = 0; c < n; c++){
                int t = (data.at((r * (n+1) + c)  * 2) << 8) | (data.at((r * (n+1) + c) * 2 + 1));
                if (t == (*heights)[r][c]) {
                    if (c > 0) {
                        t = (*heights)[r][c-1];
                    } else if (r > 0) {
                        t = (*heights)[r-1][c];
                    } else {
                        t = 0;
                    }
                }
                (*heights)[r][c] = t;
                if(t > max){
                    max = t;
                    max_p = {r, c};
                }
                if(t < min){
                    min = t;
                    min_p = {r, c};
                }
            }
        }

        for(int r = 0; r < n; r++){
            for(int c = 0; c < n; c++){
                int color = 255 * ((*heights)[r][c] - min) / (max - min);
                image->setPixelColor(r, c, QColor(color, color, color, 255));
            }
        }
    }else{
        qDebug() << "Error";
    }
}

int DistanseSrtm::getH(int x, int y, qreal x_0, qreal y_0, qreal scaled)
{
    int stepx = ((x_0 - this->x_0) * (111300 * qCos(y_0)) + x * scaled) / (n == 1200? 90: 30);
    int stepy = ((y_0 - this->y_0) * (111300 * qCos(y_0)) + y * scaled) / (n == 1200? 90: 30);
    return heights->at((stepy < 0 || stepy >= n? 0: stepy)).at((stepx < 0 || stepx >= n? 0: stepx));
}

int DistanseSrtm::calculateDistance(int x1, int y1, int x2, int y2, qreal scaled, qreal x_0, qreal y_0)
{
    int steps = qSqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) * scaled / (n == 1200? 90: 30);
    if(steps <= 0) steps = 1;
    int distance = 0;
    qreal dx = (x1 - x2) / steps;
    qreal dy = (y1 - y2) / steps;
    for(int i = 1; i < steps; i++){
        int n_x1 = static_cast<int>(x1 + dx * i - dx);
        int n_y1 = static_cast<int>(y1 + dy * i - dy);
        int n_x2 = static_cast<int>(x1 + dx * i);
        int n_y2 = static_cast<int>(y1 + dy * i);
        int h1 = getH(n_x1, n_y1, x_0, y_0, scaled);
        int h2 = getH(n_x2, n_y2, x_0, y_0, scaled);
        distance += qSqrt(scaled * scaled * (qPow(n_x2 - n_x1, 2) + qPow(n_y2 - n_y1, 2)) + qPow(h1 - h2, 2));
    }
    return distance;
}

QVector<QVector<int> > *DistanseSrtm::getHeights() const
{
    return heights;
}
