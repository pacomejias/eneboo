/***************************************************************************
WQRadioButton_p.h
-------------------
begin                : 13/02/2010
copyright            : (C) 2003-2010 by InfoSiAL S.L.
email                : mail@infosial.com
***************************************************************************/
/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 ***************************************************************************/
/***************************************************************************
 Este  programa es software libre. Puede redistribuirlo y/o modificarlo
 bajo  los  términos  de  la  Licencia  Pública General de GNU   en  su
 versión 2, publicada  por  la  Free  Software Foundation.
 ***************************************************************************/

#ifndef WQRADIOBUTTON_P_H_
#define WQRADIOBUTTON_P_H_

#include "AQUi/WQWidget_p.h"

class WQRadioButtonPrivate : public WQWidgetPrivate
{
  AQ_DECLARE_PUBLIC( WQRadioButton )

public :

  WQRadioButtonPrivate();
  ~WQRadioButtonPrivate();

  void init();

  WRadioButton * ww_;
  QRadioButton * qBut_;
};

#endif /* WQRADIOBUTTON_P_H_ */
