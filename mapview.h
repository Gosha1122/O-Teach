#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QEnterEvent>
#include "defines.h"
class MapView : public QGraphicsView
{
    Q_OBJECT
public:

    explicit MapView(QWidget* parent = nullptr);


    void setCursorType();

    void setCursorSize(Cursors::Size newCursorSize);

    void setCursorColor(Cursors::Color newCursorColor);


    void setToolCursor(Cursors::ToolCursor newToolCursor);

protected:
    void enterEvent(QEnterEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
private:
    MapApl::CursorType cursorType;

    Cursors::Size cursorSize = Cursors::Size::Normal;
    Cursors::Color cursorColor = Cursors::Color::WhiteAndBlack;
    Cursors::ToolCursor toolCursor = Cursors::ToolCursor::Default;

    QString getToolCursorType();
};

#endif // MAPVIEW_H
