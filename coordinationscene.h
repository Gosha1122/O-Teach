#ifndef COORDINATIONSCENE_H
#define COORDINATIONSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>

class CoordinationPoint;

struct CoordPoint{
    qreal x_0;
    qreal y_0;
    int x;
    int y;
};

class CoordinationScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CoordinationScene(QObject *parent = nullptr);

    void setLatitudeEdit(QLineEdit *newLatitudeEdit);

    void setLongetudeEdit(QLineEdit *newLongetudeEdit);

    QVector<CoordPoint>* getPoints();

    bool touchPoint = false;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

public slots:
    void pointPressed(CoordinationPoint* point, bool is_newActive);
    void applyButtonPressed();

private:
    QLineEdit* latitudeEdit;
    QLineEdit* longetudeEdit;



    CoordinationPoint* activePoint;
};

#endif // COORDINATIONSCENE_H
