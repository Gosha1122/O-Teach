/****************************************************************************
** Meta object code from reading C++ file 'mapscene.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mapscene.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mapscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMapSceneENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMapSceneENDCLASS = QtMocHelpers::stringData(
    "MapScene",
    "addStartPointSignal",
    "",
    "zoomSignal",
    "unzoomSignal",
    "removeAllMapPointsSignal",
    "startRulerModeSignal",
    "removeMapPointSlot",
    "MapControlPoint*",
    "point",
    "moveMapPoitSlot",
    "oldPos",
    "newPos",
    "movePointSlot",
    "mp"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMapSceneENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x06,    1 /* Public */,
       3,    0,   63,    2, 0x06,    2 /* Public */,
       4,    0,   64,    2, 0x06,    3 /* Public */,
       5,    0,   65,    2, 0x06,    4 /* Public */,
       6,    0,   66,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    1,   67,    2, 0x08,    6 /* Private */,
      10,    2,   70,    2, 0x08,    8 /* Private */,
      13,    1,   75,    2, 0x08,   11 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QPointF, QMetaType::QPointF,   11,   12,
    QMetaType::Void, 0x80000000 | 8,   14,

       0        // eod
};

Q_CONSTINIT const QMetaObject MapScene::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsScene::staticMetaObject>(),
    qt_meta_stringdata_CLASSMapSceneENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMapSceneENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMapSceneENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MapScene, std::true_type>,
        // method 'addStartPointSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'zoomSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'unzoomSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeAllMapPointsSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startRulerModeSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeMapPointSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<MapControlPoint *, std::false_type>,
        // method 'moveMapPoitSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPointF, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPointF, std::false_type>,
        // method 'movePointSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<MapControlPoint *, std::false_type>
    >,
    nullptr
} };

void MapScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MapScene *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->addStartPointSignal(); break;
        case 1: _t->zoomSignal(); break;
        case 2: _t->unzoomSignal(); break;
        case 3: _t->removeAllMapPointsSignal(); break;
        case 4: _t->startRulerModeSignal(); break;
        case 5: _t->removeMapPointSlot((*reinterpret_cast< std::add_pointer_t<MapControlPoint*>>(_a[1]))); break;
        case 6: _t->moveMapPoitSlot((*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[2]))); break;
        case 7: _t->movePointSlot((*reinterpret_cast< std::add_pointer_t<MapControlPoint*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MapScene::*)();
            if (_t _q_method = &MapScene::addStartPointSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MapScene::*)();
            if (_t _q_method = &MapScene::zoomSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MapScene::*)();
            if (_t _q_method = &MapScene::unzoomSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MapScene::*)();
            if (_t _q_method = &MapScene::removeAllMapPointsSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MapScene::*)();
            if (_t _q_method = &MapScene::startRulerModeSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *MapScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapScene::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMapSceneENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsScene::qt_metacast(_clname);
}

int MapScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void MapScene::addStartPointSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MapScene::zoomSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MapScene::unzoomSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MapScene::removeAllMapPointsSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void MapScene::startRulerModeSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
