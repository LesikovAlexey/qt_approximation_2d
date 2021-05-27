/****************************************************************************
** Meta object code from reading C++ file 'scene3D.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "scene3D.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scene3D.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Scene3D[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      23,    8,    8,    8, 0x0a,
      36,    8,    8,    8, 0x0a,
      51,    8,    8,    8, 0x0a,
      66,    8,    8,    8, 0x0a,
      82,    8,    8,    8, 0x0a,
     100,    8,    8,    8, 0x0a,
     118,    8,    8,    8, 0x0a,
     134,    8,    8,    8, 0x0a,
     150,    8,    8,    8, 0x0a,
     161,    8,    8,    8, 0x0a,
     172,    8,    8,    8, 0x0a,
     185,    8,    8,    8, 0x0a,
     199,    8,    8,    8, 0x0a,
     213,    8,    8,    8, 0x0a,
     228,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Scene3D[] = {
    "Scene3D\0\0change_func()\0add_n_appr()\0"
    "add_n_appr_x()\0add_n_appr_y()\0"
    "reduce_n_appr()\0reduce_n_appr_x()\0"
    "reduce_n_appr_y()\0show_method_1()\0"
    "show_method_2()\0show_err()\0scale_up()\0"
    "scale_down()\0add_disturb()\0sub_disturb()\0"
    "add_fineness()\0reduce_fineness()\0"
};

void Scene3D::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Scene3D *_t = static_cast<Scene3D *>(_o);
        switch (_id) {
        case 0: _t->change_func(); break;
        case 1: _t->add_n_appr(); break;
        case 2: _t->add_n_appr_x(); break;
        case 3: _t->add_n_appr_y(); break;
        case 4: _t->reduce_n_appr(); break;
        case 5: _t->reduce_n_appr_x(); break;
        case 6: _t->reduce_n_appr_y(); break;
        case 7: _t->show_method_1(); break;
        case 8: _t->show_method_2(); break;
        case 9: _t->show_err(); break;
        case 10: _t->scale_up(); break;
        case 11: _t->scale_down(); break;
        case 12: _t->add_disturb(); break;
        case 13: _t->sub_disturb(); break;
        case 14: _t->add_fineness(); break;
        case 15: _t->reduce_fineness(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Scene3D::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Scene3D::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_Scene3D,
      qt_meta_data_Scene3D, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Scene3D::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Scene3D::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Scene3D::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Scene3D))
        return static_cast<void*>(const_cast< Scene3D*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int Scene3D::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
static const uint qt_meta_data_Window[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_Window[] = {
    "Window\0"
};

void Window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Window::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Window::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Window,
      qt_meta_data_Window, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Window::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Window::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Window))
        return static_cast<void*>(const_cast< Window*>(this));
    return QWidget::qt_metacast(_clname);
}

int Window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
