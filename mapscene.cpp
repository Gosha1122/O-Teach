#include "mapscene.h"
#include <QDebug>
#include "mapcontrolpoint.h"
#include "poliline.h"
#include <QMenu>
#include "maplinekp.h"
#include "maptext.h"

MapScene::MapScene(QObject *parent)
    : QGraphicsScene{parent}
{

}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(currentToolType==ToolType::Path){
            MapControlPoint* point = new MapControlPoint;
            point->setObjectName("Point");
            if(!startPointFlag && !finishPointFlag){
                qDebug() << "Start";
                startPointFlag = true;

                point->setShape(MapControlPoint::Start);
                point->setSettings(KPNumColor, KPColor, KPNumSize, KPSize, KPWidth, KPNumStyle, StartSize,
                                   StartWidth, StartColor, LineWidth, LineColor);
                point->setPos(event->scenePos());


                lastItem = point;
                flagLastItemStart = true;
                oldPoint = event->scenePos();
                /*
                poliline = new PoliLine;
                QPainterPath path;
                path.moveTo(event->scenePos());
                poliline->setPath(path);
                addItem(poliline);
                */
                emit addStartPointSignal();
                pointCount ++;
            }else if(startPointFlag && !finishPointFlag){
                point->setSettings(KPNumColor, KPColor, KPNumSize, KPSize, KPWidth, KPNumStyle, StartSize,
                                   StartWidth, StartColor, LineWidth, LineColor);
                point->setShape(MapControlPoint::KP);
                point->setPos(event->scenePos());
                point->setPrevPoint(lastItem);

                qDebug() << "Kp";
                pointCount ++;
                /*
                QPainterPath path = poliline->path();
                path.lineTo(event->scenePos());
                poliline->setPath(path);
                */
                MapLineKP* line = new MapLineKP;
                line->setObjectName("Line");
                line->setRKP(KPSize / 2);
                if(flagLastItemStart){
                    flagLastItemStart = false;
                    line->setFlagStartPoint(MapLineKP::StartPoint::beginStartPoint);
                    line->setRStart(StartSize / 2);
                    qDebug() << StartSize;
                    lastItem->update();
                }
                line->setKPLine(oldPoint.x(), oldPoint.y(), event->scenePos().x(), event->scenePos().y());
                QPen pen;
                pen.setColor(LineColor);
                pen.setWidth(LineWidth);
                line->setPen(pen);
                addItem(line);
                oldPoint = event->scenePos();


                lastItem->setFinishLine(line);
                point->setStartLine(line);

                lastItem = point;

                QGraphicsSimpleTextItem* textItem = new QGraphicsSimpleTextItem(point);
                textItem->setPos(-40, -50);
                QFont font = textItem->font();
                font.setPixelSize(KPNumSize);
                textItem->setFont(font);
                textItem->setBrush(QBrush(KPNumColor));

            }else{
                qDebug() << "??";
            }
            point->setParentScene(this);
            addItem(point);
            connect(point, &MapControlPoint::removeMapPoint,
                    this, &MapScene::removeMapPointSlot);
            connect(point, &MapControlPoint::moveMapPoint,
                    this, &MapScene::moveMapPoitSlot);

            connect(point, &MapControlPoint::movePointSignal, this, &MapScene::movePointSlot);
            point->setNumberKP(pointCount - 1);

        }else if(currentToolType == ToolType::Ruler){
            if(poliline != nullptr){
                QPainterPath new_path = poliline->path();
                new_path.lineTo(event->scenePos());
                poliline->setPath(new_path);
                poliline->setEndPoint(event->scenePos());
            }
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

void MapScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // if(event->button()==Qt::LeftButton){
    //     if(currentToolType==ToolType::Path){
    //         if(startPoint && !finishPoint){
    //             qDebug() << "Finish";
    //             finishPoint = true;
    //             MapControlPoint* point = new MapControlPoint;
    //             point->setSettings(KPNumColor, KPColor, KPNumSize, KPSize, KPWidth, KPNumStyle, StartSize,
    //                                StartWidth, StartColor, LineWidth, LineColor);
    //             point->setShape(MapControlPoint::Finish);
    //             point->setPos(event->scenePos());
    //             addItem(point);
    //         }
    //     }
    // }
    QGraphicsScene::mouseDoubleClickEvent(event);
}

void MapScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsScene::contextMenuEvent(event);
    if(itemContextMenuFlag){
        itemContextMenuFlag = false;
        return;
    }
    QMenu menu;
    QAction* clearAction = menu.addAction("Очистить");
    QAction* zoomAction = menu.addAction("Увеличить");
    QAction* unzoomAction = menu.addAction("Уменьшить");
    if(pointCount == 0){
        clearAction->setEnabled(false);
    }
    QAction *selectedAction = menu.exec(event->screenPos());
    if (selectedAction == clearAction ) {
        deletePoints();
    } else if (selectedAction == zoomAction ) {
        emit zoomSignal();
    }else if (selectedAction == unzoomAction ) {
        emit unzoomSignal();
    }

}

void MapScene::endRuler()
{
    for(int i = 0; i < polilineVec.count(); i++){
        polilineVec[i]->hide();
    }
    QList<QGraphicsItem*> lst = this->items();
    for(int i = 0; i < lst.size(); i++){
        if(lst[i] == mapItem){
            continue;
        }
        MapControlPoint* mapPoint = qgraphicsitem_cast<MapControlPoint*>(lst[i]);
        if(mapPoint->objectName() == "Line"){
            mapPoint->show();
        }
    }
}

PoliLine *MapScene::getPoliline() const
{
    return poliline;
}

bool MapScene::getFinishPointFlag() const
{
    return finishPointFlag;
}

void MapScene::setFinishPointFlag(bool newFinishPointFlag)
{
    finishPointFlag = newFinishPointFlag;
}

void MapScene::startRulerMode(MapControlPoint *mp)
{


    for(int i = 0; i < polilineVec.count(); i++){
        polilineVec[i]->show();
    }

    poliline = new PoliLine;
    QPainterPath new_path;
    new_path.moveTo(mp->scenePos());
    poliline->setPath(new_path);
    poliline->setStartPoint(mp);
    addItem(poliline);
    poliline->setEndPoint(mp->scenePos());
    poliline->setPen(QPen(LineColor, LineWidth));

    polilineVec.push_back(poliline);

    QList<QGraphicsItem*> lst = this->items();
    for(int i = 0; i < lst.count(); i++){
        if(lst[i] == mapItem){
            continue;
        }
        MapControlPoint* item = qgraphicsitem_cast<MapControlPoint*>(lst[i]);
        if(item == mp){
            continue;
        }else if(item->scenePos() == mp->getFinishLine()->getFinishPoint()){
            poliline->setFinishPoint(qgraphicsitem_cast<MapControlPoint*>(item));
            continue;
        }
        if(item->objectName() == "Line"){
            MapLineKP* line = qgraphicsitem_cast<MapLineKP*>(lst[i]);
            line->hide();
        }else if(item->objectName() == "Point"){
            QColor color = item->getColorPoint();
            color.setAlpha(100);
            item->setColorPoint(color);
            item->setColorAlphaFlag(false);
            item->update();
        }else if(item->objectName() == "MapText"){
            MapText* text = qgraphicsitem_cast<MapText*>(lst[i]);
            QColor color = text->pen().color();
            color.setAlpha(100);
            QPen pen = text->pen();
            pen.setColor(color);
            text->setPen(pen);
            text->setOriginColorFlag(false);
            item->update();
        }
    }

    qDebug() << "StartRulerMode";

    emit startRulerModeSignal();
}

void MapScene::finishRulerMode()
{
    QPainterPath path = poliline->path();
    qDebug() << poliline->getFinishPoint()->scenePos() << " " << poliline->getFinishPoint()->getStartLine()->line().p2();
    QPointF lastPoint = poliline->getEndPoint();
    qDebug() << lastPoint;
    qDebug() << poliline->getStartPoint()->scenePos();
    if(qPow(qAbs(lastPoint.x() - poliline->getFinishPoint()->scenePos().x()), 2) +
        qPow(qAbs(lastPoint.y() - poliline->getFinishPoint()->scenePos().y()), 2) <=
        KPSize * KPSize  / 4){
        qDebug() << "FinishRuler";
        path.setElementPositionAt(path.elementCount() - 1, poliline->getFinishPoint()->scenePos().x(), poliline->getFinishPoint()->scenePos().y());
        poliline->setPath(path);
    }else{
        polilineVec.pop_back();
        delete poliline;
    }
    qDebug() << poliline->calculateDistance();
    poliline = nullptr;
    qDebug() << polilineVec.count();
    QList<QGraphicsItem*> lst = this->items();
    for(int i = 0; i < lst.count(); i++){
        if(lst[i] == mapItem){
            continue;
        }
        MapControlPoint* mapPoint = qgraphicsitem_cast<MapControlPoint*>(lst[i]);
        if(mapPoint->objectName() == "Line"){
            MapLineKP* line = qgraphicsitem_cast<MapLineKP*>(lst[i]);
            line->hide();
        }else if(mapPoint->objectName() == "Point"){
            mapPoint->setColorAlphaFlag(true);
            mapPoint->update();
        }else if(mapPoint->objectName() == "MapText"){
            MapText* text = qgraphicsitem_cast<MapText*>(lst[i]);
            text->setOriginColorFlag(true);
            text->update();
        }
    }
}

QGraphicsItem *MapScene::getMapItem() const
{
    return mapItem;
}

void MapScene::setMapItem(QGraphicsItem *newMapItem)
{
    mapItem = newMapItem;
}

void MapScene::setFinishPoint()
{
    lastItem->setShape(MapControlPoint::Finish);
    if(flagLastItemStart){
        lastItem->getStartLine()->setFlagStartPoint(MapLineKP::StartPoint::beginAndEndStartPoint);
    }else{
        lastItem->getStartLine()->setFlagStartPoint(MapLineKP::StartPoint::endStartPoint);
    }
    lastItem->getStartLine()->setRStart(StartSize / 2);
    lastItem->getStartLine()->updateKP();
    qDebug() << "count: "<< lastItem->childItems().count();
    delete lastItem->childItems()[0];
    lastItem->update();
}

void MapScene::removeMapPointSlot(MapControlPoint *point)
{
    QList<QGraphicsItem *> lst = this->items();
    /*
    for(int i=lst.size()-1;i>=0;i--){
        if(lst[i]==mapItem)
            continue;
        MapControlPoint* mp = qgraphicsitem_cast<MapControlPoint*>(lst[i]);
        if(mp->getPrevPoint()==point){
            mp->setPrevPoint(point->getPrevPoint());
        }
    }
    if(point==lastItem){
        lastItem= point->getPrevPoint();
    }
    removeItem(point);
    */
    QPointF beginPoint;
    QPointF endPoint;
    if(point->getStartLine() != nullptr){
        beginPoint = point->getStartLine()->getStartPoint();
    }else{
        endPoint = point->getFinishLine()->getFinishPoint();
        for(int i = 0; i < lst.size(); i++){
            if(lst[i] != mapItem){
                MapControlPoint* mp = qgraphicsitem_cast<MapControlPoint*>(lst[i]);
                if(mp->scenePos() == endPoint){
                    mp->setShape(MapControlPoint::Shape::Start);
                    mp->setStartLine(nullptr);
                    mp->update();
                    delete mp->childItems()[0];
                }
                if(mp != point){
                    if(!mp->childItems().isEmpty()){
                        mp->setNumberKP(qgraphicsitem_cast<QGraphicsSimpleTextItem*>(mp->childItems()[0])->text().toInt() - 1);
                    }
                }
            }
        }
        removeItem(point->getFinishLine());
        removeItem(point);
        return;
    }
    if(point->getFinishLine() != nullptr){
        endPoint = point->getFinishLine()->getFinishPoint();
    }else{
        beginPoint = point->getStartLine()->getStartPoint();
        for(int i = 0; i < lst.size(); i++){
            if(lst[i] != mapItem){
                MapControlPoint* mp = qgraphicsitem_cast<MapControlPoint*>(lst[i]);
                if(mp->scenePos() == beginPoint){
                    mp->setShape(MapControlPoint::Shape::Finish);
                    mp->setFinishLine(nullptr);
                    mp->update();
                    break;
                }
            }
        }
        removeItem(point->getStartLine());
        removeItem(point);
        return;
    }
    MapLineKP* line = new MapLineKP;
    line->setRKP(KPSize / 2);
    line->setKPLine(beginPoint.x(), beginPoint.y(), endPoint.x(), endPoint.y());
    QPen pen;
    pen.setColor(LineColor);
    pen.setWidth(LineWidth);
    line->setPen(pen);
    addItem(line);

    int countP = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(point->childItems()[0])->text().toInt();

    for(int i = 0; i < lst.size(); i++){
        if(lst[i] == mapItem)
            continue;
        MapControlPoint* mp = qgraphicsitem_cast<MapControlPoint*>(lst[i]);
        if(mp->scenePos() == beginPoint){
            mp->setFinishLine(line);
        }
        if(mp->scenePos() == endPoint){
            mp->setStartLine(line);
        }
        if(!mp->childItems().isEmpty()){
            int countMp = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(mp->childItems()[0])->text().toInt();
            if(countMp > countP){
                mp->setNumberKP(countMp - 1);
            }
        }
    }
    removeItem(point->getStartLine());
    removeItem(point->getFinishLine());
    removeItem(point);
}

void MapScene::moveMapPoitSlot(QPointF oldPos, QPointF newPos)
{
    QPainterPath path = poliline->path();
    QPainterPath newPath;
    for(int i=0;i<path.elementCount();i++){
        if(path.elementAt(i).x == oldPos.x() && path.elementAt(i).y == oldPos.y()){
            path.setElementPositionAt(i,newPos.x(), newPos.y());
            break;
        }
    }
    poliline->setPath(path);
}

void MapScene::movePointSlot(MapControlPoint *mp)
{
    QList<QGraphicsItem *> lst = this->items();
    for(int i = 0; i < lst.size(); i++){
        if(lst[i] != mapItem){
            MapControlPoint* mp_1 = qgraphicsitem_cast<MapControlPoint*>(lst[i]);
            if(mp_1->scenePos() == mp->getStartLine()->getStartPoint()){
                mp_1->update();
            }else if(mp_1->scenePos() == mp->getFinishLine()->getFinishPoint()){
                mp_1->update();
            }
        }
    }
}

int MapScene::getPointCount() const
{
    return pointCount;
}

void MapScene::deletePoints()
{
    QList<QGraphicsItem *> lst = this->items();
    for(int i=lst.size()-1;i>=0;i--){
        if(lst[i]==mapItem)
            continue;
        this->removeItem(lst[i]);
    }
    pointCount = 0;
    startPointFlag = false;
    finishPointFlag = false;
    emit removeAllMapPointsSignal();
}

void MapScene::setLineColor(const QColor &newLineColor)
{
    LineColor = newLineColor;
}

void MapScene::setLineWidth(int newLineWidth)
{
    LineWidth = newLineWidth;
}

void MapScene::setStartColor(const QColor &newStartColor)
{
    StartColor = newStartColor;
}

void MapScene::setStartWidth(int newStartWidth)
{
    StartWidth = newStartWidth;
}

void MapScene::setStartSize(int newStartSize)
{
    StartSize = newStartSize;
}

void MapScene::setKPColor(const QColor &newKPColor)
{
    KPColor = newKPColor;
}

void MapScene::setKPWidth(int newKPWidth)
{
    KPWidth = newKPWidth;
}

void MapScene::setKPSize(int newKPSize)
{
    KPSize = newKPSize;
}

void MapScene::setKPNumColor(const QColor &newKPNumColor)
{
    KPNumColor = newKPNumColor;
}

void MapScene::setKPNumStyle(int newKPNumStyle)
{
    KPNumStyle = newKPNumStyle;
}

void MapScene::setKPNumSize(int newKPNumSize)
{
    KPNumSize = newKPNumSize;
}

MapScene::ToolType MapScene::getCurrentToolType() const
{
    return currentToolType;
}

void MapScene::setCurrentToolType(ToolType newCurrentToolType)
{
    currentToolType = newCurrentToolType;
}
