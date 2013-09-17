/***************************************************************************
AQTranslator.cpp
-------------------
begin                : 07/08/2007
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

#include <QFile>

#include <AQlrelease/AQTranslations.h>
#include <AQCore/AQCore.h>

#include "AQTranslator.h"
#include "AQCore.h"
#include "cache/AQDiskCache.h"

AQTranslator::AQTranslator( QObject * parent ) :
    QTranslator( parent )
{}

bool AQTranslator::loadTsContent( const QString & key )
{
  QString tsFile = AQ_DISKCACHE_FILEPATH( key );
  QString qmFile = tsFile + ".qm";
  AQTranslations trans;

  if ( !QFile::exists( qmFile ) ) {
    if ( tsFile.isEmpty() )
      return false;
    trans.lrelease( tsFile, qmFile );
    aqDebug( "lrelease :  " + tsFile + " -> " + qmFile );
  }

  aqDebug( "QTranslator::load : " + qmFile );
  return QTranslator::load( qmFile );
}
