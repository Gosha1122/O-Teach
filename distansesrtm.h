#ifndef DISTANSESRTM_H
#define DISTANSESRTM_H
#include <QObject>

#define NONEHEIGHT -32768

class DistanseSrtm : public QObject
{
    Q_OBJECT
public:
    explicit DistanseSrtm(QObject *parent = nullptr);

    void initMatrix(QString path);
    int getHeight(int x, int y);

    QVector<QVector<int> > *getHeights() const;

signals:

private:
    QVector<QVector<int>>* heights;
    int n = 0;
    int max = -1e6;
    int min = 1e6;

};

#endif // DISTANSESRTM_H
