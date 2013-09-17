/***************************************************************************
AQAccessControlFactory.cpp
-------------------
begin                : 09/08/2007
copyright            : (C) 2003-2007 by InfoSiAL S.L.
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

#include <AQCore/AQTableMD.h>

#include "AQAccessControlFactory.h"
#include "../corelib/acl/AQAccessControlTable.h"

AQAccessControl * AQAccessControlFactory::create( const QString & type )
{
  if ( type == "table" )
    return new AQAccessControlTable;

  //  if ( type == "mainwindow" )
  //    return new AQAccessControlMainWindow();
  //  else if ( type == "form" )
  //    return new AQAccessControlForm();
  //  else if ( type == "table" )
  //    return new AQAccessControlTable();

  return 0;
}

QString AQAccessControlFactory::type( QObject * obj )
{
  if ( qobject_cast<AQTableMD *>( obj ) )
    return "table";

  //  if ( obj->isA( "QMainWindow" ) )
  //    return "mainwindow";
  //  else if ( obj->isA( "AQTableMetaData" ) )
  //    return "table";
  //  else if ( obj->inherits( "AQFormDB" ) )
  //    return "form";

  return QString();
}
