#ifndef STYLEHELPER_H
#define STYLEHELPER_H
#include <QString>
class QToolButton;

class StyleHelper
{
public:
    enum MapIconsType{
        Back,
        Path,
        Ruler,
        Start,
        Move,
        Add
    };
    StyleHelper();

    //Widget
    void static setFonts();
    QString static getMapStyleDark();
    QString static getMapListStyleDark();
    void static setToolButtonStyleDark(QToolButton* btn, MapIconsType type, bool active=false);
    QString static getEndPathButtonStyle();
    QString static getMapIconButtonStyle();
    QString static getSpinBoxStyle();
    QString static getColorButtonStyle(QString color);
    QString static getMessageBoxStyle();

    //MapSettings
    QString static getMapSettingsStyle();
private:
    QString static darkBgTop;
    QString static darkBgRight;

};

#endif // STYLEHELPER_H
