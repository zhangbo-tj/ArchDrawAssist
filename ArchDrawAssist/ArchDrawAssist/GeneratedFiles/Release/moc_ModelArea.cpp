/****************************************************************************
** Meta object code from reading C++ file 'ModelArea.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ModelArea.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModelArea.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ModelArea_t {
    QByteArrayData data[6];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModelArea_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModelArea_t qt_meta_stringdata_ModelArea = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ModelArea"
QT_MOC_LITERAL(1, 10, 11), // "UpdateTimer"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 15), // "AddCuttingPlane"
QT_MOC_LITERAL(4, 39, 14), // "ResetTrackball"
QT_MOC_LITERAL(5, 54, 18) // "DeleteCuttingPlane"

    },
    "ModelArea\0UpdateTimer\0\0AddCuttingPlane\0"
    "ResetTrackball\0DeleteCuttingPlane"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModelArea[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ModelArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ModelArea *_t = static_cast<ModelArea *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UpdateTimer(); break;
        case 1: _t->AddCuttingPlane(); break;
        case 2: _t->ResetTrackball(); break;
        case 3: _t->DeleteCuttingPlane(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ModelArea::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_ModelArea.data,
      qt_meta_data_ModelArea,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ModelArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModelArea::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ModelArea.stringdata0))
        return static_cast<void*>(const_cast< ModelArea*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int ModelArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
