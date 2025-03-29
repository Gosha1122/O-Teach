#include "mapview.h"
#include <QDebug>
MapView::MapView(QWidget *parent):QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
}

void MapView::enterEvent(QEnterEvent *event)
{
    qDebug() << "enter";
    setCursorType();

}

void MapView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    setCursorType();
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    setCursorType();
}

void MapView::setToolCursor(Cursors::ToolCursor newToolCursor)
{
    toolCursor = newToolCursor;
}

QString MapView::getToolCursorType()
{
    switch(toolCursor){
        case Cursors::ToolCursor::Move:  return QString("move") ;
        case Cursors::ToolCursor::Point: return QString("point");
        case Cursors::ToolCursor::Ruler: return QString("ruler");
        default: return QString("arrow");
    }
}

void MapView::setCursorColor(Cursors::Color newCursorColor)
{
    cursorColor = newCursorColor;
}

void MapView::setCursorSize(Cursors::Size newCursorSize)
{
    cursorSize = newCursorSize;
}

void MapView::setCursorType()
{
    qDebug() << toolCursor;
    if(getToolCursorType() != "arrow"){
        if(cursorSize == Cursors::Size::Normal){
            if(cursorColor == Cursors::Color::WhiteAndBlack){
                viewport()->setCursor(QCursor(QPixmap(":/resourses/cursors/bw_" + getToolCursorType() + "_cursor.png")));

            }else if(cursorColor == Cursors::Color::RedAndWhite){
                viewport()->setCursor(QCursor(QPixmap(":/resourses/cursors/rw_" + getToolCursorType() + "_cursor.png")));

            }else if(cursorColor == Cursors::Color::RedAndBlack){
                viewport()->setCursor(QCursor(QPixmap(":/resourses/cursors/rb_" + getToolCursorType() + "_cursor.png")));

            }
        }else if(cursorSize == Cursors::Size::Big){
            if(cursorColor == Cursors::Color::WhiteAndBlack){
                viewport()->setCursor(QCursor(QPixmap(":/resourses/cursors/bw_large_" + getToolCursorType() + "_cursor.png")));

            }else if(cursorColor == Cursors::Color::RedAndWhite){
                viewport()->setCursor(QCursor(QPixmap(":/resourses/cursors/rw_large_" + getToolCursorType() + "_cursor.png")));

            }else if(cursorColor == Cursors::Color::RedAndBlack){
                viewport()->setCursor(QCursor(QPixmap(":/resourses/cursors/rb_large_" + getToolCursorType() + "_cursor.png")));

            }
        }
    }else{
        viewport()->setCursor(Qt::ArrowCursor);
    }

    /*
    using CType = MapApl::CursorType;
    cursorType = newCursorType;
    switch (newCursorType) {
    case CType::OpenHand:
        viewport()->setCursor(Qt::OpenHandCursor);

        break;
    case CType::PathCross:
        viewport()->setCursor(QCursor(QPixmap(":/resourses/cursors/cross-cursor-red-2.png")));
        break;
    case CType::Default:
        break;
    }
    */
}
