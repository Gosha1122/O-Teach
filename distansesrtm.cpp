#include "distansesrtm.h"

#include <QFile>
#include <QDebug>

DistanseSrtm::DistanseSrtm(QObject *parent)
    : QObject{parent}
{}

void DistanseSrtm::initMatrix(QString path)
{
    QFile file(path);

    if(file.open(QIODevice::ReadOnly)){
        qDebug() << "Open\n";
        QByteArray data = file.readAll();
        file.close();
        int count = data.size();
        if(count > 1201 * 1201 * 2 + 1){
            n = 3601;
            heights = new QVector<QVector<int>>(3601, QVector<int>(3601, 0));
            for(int r = 0; r < 3601; r++){
                for(int c = 0; c < 3601; c++){
                    int t = (data.at((r * 3602 + c)  * 2) << 8) | (data.at((r * 3602 + c) * 2 + 1));
                    (*heights)[r][c] = t;
                    if(t > max){
                        max = t;
                    }
                    if(t < min && t != NONEHEIGHT){
                        min = t;
                    }
                }
            }
        }else{
            n = 1201;
            heights = new QVector<QVector<int>>(1201, QVector<int>(1201, 0));
            for(int r = 0; r < 1201; r++){
                for(int c = 0; c < 1201; c++){
                    int t = (data.at((r * 1202 + c)  * 2) << 8) | (data.at((r * 1202 + c) * 2 + 1));
                    (*heights)[r][c] = t;
                    if(t > max){
                        max = t;
                    }
                    if(t < min && t != NONEHEIGHT){
                        min = t;
                    }
                }
            }
        }

    }else{
        qDebug() << "Error";
    }
}

int DistanseSrtm::getHeight(int x, int y)
{
    return heights->at(x).at(y);
}

QVector<QVector<int> > *DistanseSrtm::getHeights() const
{
    return heights;
}
