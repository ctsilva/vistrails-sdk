/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef ASPECTPRESERVINGLABEL_HPP
#define ASPECTPRESERVINGLABEL_HPP

#include <QtGui>
#include <QLabel>
#include <QWidget>
#include <iostream>

class QAspectPreservingLabel : public QLabel
{
    Q_OBJECT

public:
  QAspectPreservingLabel(QWidget *parent = 0) : QLabel(parent), ratio(1.0) { 
    QSizePolicy policy;
    policy.setHeightForWidth(true);
    this->setSizePolicy(policy);
  }
  void setAspectRatio(double ratio) { this->ratio = ratio; }
  virtual int heightForWidth(int w) const { 
    int h = (int) (w / this->ratio);
    return h; 
  }

private:
  double ratio;
};

#endif
