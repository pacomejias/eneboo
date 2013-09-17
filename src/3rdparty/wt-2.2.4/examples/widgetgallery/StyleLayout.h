// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba
 *
 * See the LICENSE file for terms of use.
 */

#ifndef STYLE_LAYOUT_H_
#define STYLE_LAYOUT_H_

#include "ControlsWidget.h"

class StyleLayout : public ControlsWidget
{
public:
  StyleLayout(EventDisplayer *ed);

  void populateSubMenu(Wt::WMenu *menu);

private:
  WWidget *css();
  WWidget *wBoxLayout();
  WWidget *wGridLayout();
  WWidget *wBorderLayout();
};

#endif // STYLE_LAYOUT_H_
