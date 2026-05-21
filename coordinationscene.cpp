#include "coordinationscene.h"
#include "coordinationpoint.h"

CoordinationScene::CoordinationScene(QObject *parent)
    : QGraphicsScene{parent}
{}

void CoordinationScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}

void CoordinationScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        CoordinationPoint* point = new CoordinationPoint;
        point->setPos(event->scenePos());
        point->initCoordination(longetudeEdit->text().toDouble(), latitudeEdit->text().toDouble());
        point->setParent(this);

        connect(point, &CoordinationPoint::pressCoordPoint, this, &CoordinationScene::pointPressed);
        addItem(point);
        activePoint = point;
    }

    QGraphicsScene::mouseDoubleClickEvent(event);
}

void CoordinationScene::pointPressed(CoordinationPoint *point, bool is_newActive)
{
    qDebug() << "Point pressed scene";
    if(is_newActive){
        activePoint = point;
    }else{
        activePoint = nullptr;
    }
}

void CoordinationScene::setLongetudeEdit(QLineEdit *newLongetudeEdit)
{
    longetudeEdit = newLongetudeEdit;
}

QVector<CoordPoint>* CoordinationScene::getPoints()
{
    QVector<CoordPoint>* points = new QVector<CoordPoint>;
    QList<QGraphicsItem*> lst = this->items();
    for(auto& item: lst){
        CoordinationPoint* p = qgraphicsitem_cast<CoordinationPoint*>(item);
        if(p->objectName() == "CoordinationPoint"){
            points->push_back({p->getX_0(), p->getY_0(), static_cast<int>(p->scenePos().x()), static_cast<int>(p->scenePos().y())});
        }
    }
    return points;
}

void CoordinationScene::applyButtonPressed()
{
    if(activePoint != nullptr){
        activePoint->initCoordination(longetudeEdit->text().toDouble(), latitudeEdit->text().toDouble());
    }
}

void CoordinationScene::setLatitudeEdit(QLineEdit *newLatitudeEdit)
{
    latitudeEdit = newLatitudeEdit;
}
