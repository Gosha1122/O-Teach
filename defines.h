#ifndef DEFINES_H
#define DEFINES_H
namespace MapApl
{

enum CursorType{
    Default,        //Курсор по умолчанию (на всякий случай)
    PathCross,      //Инструмент Дистанция, ставим КП
    OpenHand        //Инструмент Сдвиг (перемещение)
};
enum ToolType{
    Path,           //Дистанция
    Ruler,          //Измерить
    Start,          //Пройти
    Move            //Сдвинуть
};
}

namespace Cursors{

enum Size{
    Normal,
    Big
};

enum Color{
    WhiteAndBlack,
    RedAndWhite,
    RedAndBlack
};
enum ToolCursor{
    Move,
    Point,
    Ruler,
    Default
};
}
#endif // DEFINES_H
