/****************************************************************************
** Meta object code from reading C++ file 'analysis.h'
**
** Created: Tue Sep 17 11:01:12 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "analysis.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'analysis.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Analysis[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   10,    9,    9, 0x08,
      46,    9,    9,    9, 0x08,
      72,   66,    9,    9, 0x08,
     112,  106,    9,    9, 0x08,
     137,   66,    9,    9, 0x08,
     169,    9,    9,    9, 0x08,
     189,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Analysis[] = {
    "Analysis\0\0mouseUsage\0onMouseUsageChanged(int)\0"
    "onViewPortChanged()\0event\0"
    "onMouseMovePlotArea(QMouseEvent*)\0"
    "value\0onHScrollBarChanged(int)\0"
    "onMouseWheelChart(QWheelEvent*)\0"
    "onLineEditChanged()\0onLineEditMAChanged()\0"
};

void Analysis::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Analysis *_t = static_cast<Analysis *>(_o);
        switch (_id) {
        case 0: _t->onMouseUsageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onViewPortChanged(); break;
        case 2: _t->onMouseMovePlotArea((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->onHScrollBarChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onMouseWheelChart((*reinterpret_cast< QWheelEvent*(*)>(_a[1]))); break;
        case 5: _t->onLineEditChanged(); break;
        case 6: _t->onLineEditMAChanged(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Analysis::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Analysis::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Analysis,
      qt_meta_data_Analysis, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Analysis::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Analysis::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Analysis::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Analysis))
        return static_cast<void*>(const_cast< Analysis*>(this));
    return QDialog::qt_metacast(_clname);
}

int Analysis::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
