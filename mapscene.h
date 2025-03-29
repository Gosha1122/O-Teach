#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
class MapControlPoint;
class PoliLine;

class MapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum class ToolType{
        Default,
        Path,
        Ruler,
        Start,
        Move
    };

    explicit MapScene(QObject *parent = nullptr);
    ToolType getCurrentToolType() const;
    void setCurrentToolType(ToolType newCurrentToolType);

    void setKPNumSize(int newKPNumSize);

    void setKPNumStyle(int newKPNumStyle);

    void setKPNumColor(const QColor &newKPNumColor);

    void setKPSize(int newKPSize);

    void setKPWidth(int newKPWidth);

    void setKPColor(const QColor &newKPColor);

    void setStartSize(int newStartSize);

    void setStartWidth(int newStartWidth);

    void setStartColor(const QColor &newStartColor);

    void setLineWidth(int newLineWidth);

    void setLineColor(const QColor &newLineColor);
    int getPointCount() const;
    void deletePoints();
    QGraphicsItem *getMapItem() const;
    void setMapItem(QGraphicsItem *newMapItem);
    void setFinishPoint();

    bool itemContextMenuFlag = false;
    void setFinishPointFlag(bool newFinishPointFlag);

    void startRulerMode(MapControlPoint* mp);
    void finishRulerMode();

    bool getFinishPointFlag() const;

    PoliLine *getPoliline() const;

    void endRuler();

private slots:
    void removeMapPointSlot(MapControlPoint* point);
    void moveMapPoitSlot(QPointF oldPos, QPointF newPos);
    void movePointSlot(MapControlPoint* mp);

signals:
    void addStartPointSignal();
    void zoomSignal();
    void unzoomSignal();
    void removeAllMapPointsSignal();
    void startRulerModeSignal();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    ToolType currentToolType;
    bool startPointFlag = false;
    bool finishPointFlag = false;

    //Settings
    //Номера КП
    int    KPNumSize;
    int    KPNumStyle;
    QColor KPNumColor;
    //КП
    int    KPSize;
    int    KPWidth;
    QColor KPColor;
    //Старт/Финиш
    int    StartSize;
    int    StartWidth;
    QColor StartColor;
    //Линии
    int    LineWidth;
    QColor LineColor;

    int pointCount = 0;
    QGraphicsItem* mapItem;
    MapControlPoint * lastItem;
    PoliLine* poliline = nullptr;
    QVector<PoliLine*> polilineVec;

    QPointF oldPoint;

    bool flagLastItemStart = false;


};

#endif // MAPSCENE_H
