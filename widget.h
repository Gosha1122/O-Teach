#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mapscene.h"
#include "defines.h"
#include <QCloseEvent>


QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
class QToolButton;
class QPushButton;
class QGridLayout;
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void changCurrentToolSlot();
    void scaleSceneSlot();
    void ColorButtonSlot();
    void settingsComboBoxSlot(int index);
    void addStartPointSlot();
    void endPathSlot();
    void backButtonSlot();
    void endButtonPointSlot();
    void openMapSlot();
    void SizeSpinBoxSlot(int value);
    void setCursorSlot(const QString& str);
    void startRulerModeSlot();
    void addNewMapButtonSlot();
    void endButtonRulerSlot();

private:
    Ui::Widget*  ui;
    QToolButton* currentTool;
    MapScene*    mapScene;
    int          mapSizeValue;

    QPushButton* endPathButton;
    QPushButton* endRulerButton;

    QGridLayout* grid;

    MapApl::ToolType toolMode = MapApl::ToolType::Move;

    bool endPathButtonHide = true;


    void settingsInit();
    void setRulerMode();

};
#endif // WIDGET_H
