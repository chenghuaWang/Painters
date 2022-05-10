/*!
 * @author  chaneghua.wang
 * @brief   A PCH(pre compiled head) for all file that
 * need Qt-std or GNU-std libs. Some basic macro also
 * defined here.
 *
 * @details This file is pre compiled to static lib. Although
 * Qt don't use snake_case naming methods. All variables in this
 * project is named in snake_case. For variable in class, I name
 * it m_xxx. For static function or variables, naming it s_xxxx.
 * For static in class, naming it m_s_xxx.
 *
 * @note
 * For convience, I have already defined a ROOT macro in cmake.
 * And you can use this ROOT to generate all absolute path you
 * want, to link asset or other stuff.
 */

#ifndef CORE_BASE_H
#define CORE_BASE_H

#if _MSC_VER > 1000
#pragma once
#endif //! _MSC_VER

///< inlcude the basic std from qt.
#include <QString>
#include <QList>
#include <QVector>
#include <QMap>
#include <QVariant>
#include <QDebug>

///< include Qt platform component.
#include <QApplication>
#include <QWidget>
#include <QColor>
#include <QFont>
#include <QPoint>
#include <QPen>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QMouseEvent>

///< inlcude the GNU-std from there.
#include <cinttypes>
#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>
#include <memory>
#include <utility>

///< inlude the file from Core/include.


// Macro defined

///< Use macro trait to get ROOT value from Cmake.in
#define P_XSTR(s) P_STR(s)
#define P_STR(s) #s

///< Memory usage. Shared ptr basing.
#define CREATE_REF(x)   std::make_shared<x>
#define REF(x)          std::shared_ptr<x>
#define CAST_REF(x)     std::static_cast<x>

///< min max
#define P_MIN(x, y)     (x) > (y) ? (y) : (x)
#define P_MAX(x, y)     (x) > (y) ? (x) : (y)

#endif // CORE_BASE_H
