/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef __COLOR_H___
#define __COLOR_H___

#include <QColor>
#include <QHash>

// Contain color related utilities such as color look up by name or
// color defines
class ColorByName
{
public:
  static QColor get(const QString &color);

private:
  static QHash<QString, QColor> colors;
  static void initColorMap();
};

#endif
