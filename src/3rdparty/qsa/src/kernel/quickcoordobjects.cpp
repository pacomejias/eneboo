/****************************************************************************
**
** Copyright (C) 1992-2006 Trolltech AS. All rights reserved.
**
** This file is part of the QSA of the Qt Toolkit.
**
** For QSA Commercial License Holders (non-open source):
** 
** Licensees holding a valid Qt Script for Applications (QSA) License Agreement
** may use this file in accordance with the rights, responsibilities and
** obligations contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of the Licensing Agreement are
** not clear to you.
** 
** Further information about QSA licensing is available at:
** http://www.trolltech.com/products/qsa/licensing.html or by contacting
** info@trolltech.com.
** 
** 
** For Open Source Edition:  
** 
** This file may be used under the terms of the GNU General Public License
** version 2 as published by the Free Software Foundation and appearing in the
** file LICENSE.GPL included in the packaging of this file.  Please review the
** following information to ensure GNU General Public Licensing requirements
** will be met:  http://www.trolltech.com/products/qt/opensource.html 
** 
** If you are unsure which license is appropriate for your use, please review
** the following information:
** http://www.trolltech.com/products/qsa/licensing.html or contact the 
** sales department at sales@trolltech.com.

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "quickcoordobjects.h"
#include "qstypes.h"
#include "qsfunction.h"
#include "qsenv.h"
#include "qslexer.h"
#include <qpoint.h>
#include <qsize.h>
#include <qrect.h>
#include <assert.h>
#include <quickinterpreter.h>

class PointShared : public QSShared
{
public:
    PointShared( const QPoint &p ) : point( p )
    { }
    QPoint point;
};

QSPointClass::QSPointClass( QSClass *b, QuickInterpreter *i )
        : QSSharedClass( b ), QuickEnvClass( i )
{
    addMember( REG_USTR( QLatin1String( "x" ) ), QSMember( QSMember::Custom, 0, AttributePublic ) );
    addMember( REG_USTR( QLatin1String( "y" ) ), QSMember( QSMember::Custom, 1, AttributePublic ) );
}

QPoint *QSPointClass::point( const QSObject *obj ) const
{
    assert( obj->objectType() == this );
    return &( static_cast<PointShared *>( obj->shVal() ) ->point );
}

QSObject QSPointClass::fetchValue( const QSObject *obj,
                                   const QSMember &mem ) const
{
    if ( mem.type() == QSMember::Custom ) {
        switch ( mem.idx ) {
        case 0:
            return createNumber( point( obj ) ->x() );
        case 1:
            return createNumber( point( obj ) ->y() );
        default:
            qFatal( "QSPointClass::fetchValue: unhandled case" );
            return createUndefined();
        }
    }
    return QSClass::fetchValue( obj, mem );
}

void QSPointClass::write( QSObject *objPtr, const QSMember &mem,
                          const QSObject &val ) const
{
    if ( mem.type() == QSMember::Custom ) {
        switch ( mem.idx ) {
        case 0:
            point( objPtr ) ->setX( val.toInteger() );
            break;
        case 1:
            point( objPtr ) ->setY( val.toInteger() );
            break;
        default:
            qDebug( "QSPointClass::write() Unhandled case" );
        }
    } else {
        QSClass::write( objPtr, mem, val );
    }
}

QString QSPointClass::toString( const QSObject *obj ) const
{
    QPoint * p = point( obj );
    return QString( QLatin1String( "(%1, %2)" ) ).arg( p->x() ).arg( p->y() );
}

QVariant QSPointClass::toVariant( const QSObject *obj, QVariant::Type type ) const
{
    if ( type == QVariant::PointF )
        return QPointF( *point( obj ) );
    else
        return *point( obj );
}


QSObject QSPointClass::construct( const QSList &args ) const
{
    if ( args.size() == 1 ) {
        if ( args[ 0 ].objectType() == this ) {
            QSObject pt = args.at( 0 );
            return construct( QPoint( *point( &pt ) ) );
        }
    } else if ( args.size() == 2 ) {
        return construct( QPoint( args[ 0 ].toInteger(), args[ 1 ].toInteger() ) );
    }

    return construct( QPoint() );
}

QSObject QSPointClass::construct( const QPoint &p ) const
{
    return QSObject( this, new PointShared( p ) );
}

QSEqualsResult QSPointClass::isEqual( const QSObject &a, const QSObject &b ) const
{
    if ( !b.isA( this ) )
        return EqualsNotEqual;
    return ( QSEqualsResult ) ( *point( &a ) == *point( &b ) );
}

//////////////////////////////////////////////////////////////////////////

class SizeShared : public QSShared
{
public:
    SizeShared( const QSize &s ) : size( s )
    { }
    QSize size;
};

static QSObject transpose( QSEnv *env )
{
    QSObject t = env->thisValue();
    static_cast<const QSSizeClass *>( t.objectType() ) ->size( &t ) ->transpose();
    return env->createUndefined();
}

QSSizeClass::QSSizeClass( QSClass *b, QuickInterpreter *i )
        : QSSharedClass( b ), QuickEnvClass( i )
{
    addMember( REG_USTR( QLatin1String( "width" ) ), QSMember( QSMember::Custom, 0, AttributePublic ) );
    addMember( REG_USTR( QLatin1String( "height" ) ), QSMember( QSMember::Custom, 1, AttributePublic ) );
    addMember( REG_USTR( QLatin1String( "transpose" ) ), QSMember( &transpose ) );
}

QSize *QSSizeClass::size( const QSObject *obj ) const
{
    assert( obj->objectType() == this );
    return &( static_cast<SizeShared *>( obj->shVal() ) ->size );
}

QSObject QSSizeClass::fetchValue( const QSObject *objPtr,
                                  const QSMember &mem ) const
{
    if ( mem.type() == QSMember::Custom ) {
        switch ( mem.idx ) {
        case 0:
            return createNumber( size( objPtr ) ->width() );
        case 1:
            return createNumber( size( objPtr ) ->height() );
        default:
            qFatal( "QSSizeClass::fetchValue, unhandled case" );
            return createUndefined();
        }
    }
    return QSClass::fetchValue( objPtr, mem );
}

void QSSizeClass::write( QSObject *objPtr, const QSMember &mem,
                         const QSObject &val ) const
{
    if ( mem.type() == QSMember::Custom ) {
        switch ( mem.idx ) {
        case 0:
            size( objPtr ) ->setWidth( val.toInteger() );
            break;
        case 1:
            size( objPtr ) ->setHeight( val.toInteger() );
            break;
        default:
            qFatal( "QSSizeClass::write, unhandled case" );
            break;
        }
    } else {
        QSClass::write( objPtr, mem, val );
    }
}

// QSObject QSSizeClass::invoke( QSObject * objPtr, const QSMember &mem,
//     const QSList &args ) const
// {
//     if( mem.type()==QSMember::Custom ) {
//  if( mem.idx==2 ) {
//      size( objPtr )->transpose();
//  }
//  return QSUndefined();
//     } else {
//  return QSClass::invoke( objPtr, mem, args );
//     }
// }

QString QSSizeClass::toString( const QSObject *obj ) const
{
    QSize * s = size( obj );
    return QString( QLatin1String( "(%1, %2)" ) ).arg( s->width() ).arg( s->height() );
}

QVariant QSSizeClass::toVariant( const QSObject *obj, QVariant::Type type ) const
{

    if ( type == QVariant::SizeF )
        return QSizeF( *size( obj ) );
    else
        return *size( obj );
}

QSObject QSSizeClass::construct( const QSList &args ) const
{
    if ( args.size() == 1 ) {
        if ( args[ 0 ].objectType() == this ) {
            QSObject sz = args.at( 0 );
            return construct( QSize( *size( &sz ) ) );
        }
    } else if ( args.size() == 2 ) {
        return construct( QSize( args[ 0 ].toInteger(),
                                 args[ 1 ].toInteger() ) );
    }

    return construct( QSize() );
}

QSObject QSSizeClass::construct( const QSize &s ) const
{
    return QSObject( this, new SizeShared( s ) );
}

//////////////////////////////////////////////////////////////////////////

class RectShared : public QSShared
{
public:
    RectShared( const QRect &r ) : rect( r )
    { }
    QRect rect;
};

QSRectClass::QSRectClass( QSClass * b, QuickInterpreter *i )
        : QSSharedClass( b ), QuickEnvClass( i )
{
    addMember( REG_USTR( QLatin1String( "x" ) ), QSMember( QSMember::Custom, 0, AttributePublic ) );
    addMember( REG_USTR( QLatin1String( "left" ) ), QSMember( QSMember::Custom, 1, AttributePublic ) );
    addMember( REG_USTR( QLatin1String( "y" ) ), QSMember( QSMember::Custom, 2, AttributePublic ) );
    addMember( REG_USTR( QLatin1String( "top" ) ), QSMember( QSMember::Custom, 3, AttributePublic ) );
    addMember( REG_USTR( QLatin1String( "width" ) ), QSMember( QSMember::Custom, 4, AttributePublic ) );
    addMember( REG_USTR( QLatin1String( "height" ) ), QSMember( QSMember::Custom, 5, AttributePublic ) );
    addMember( REG_USTR( QLatin1String( "right" ) ), QSMember( QSMember::Custom, 6, AttributePublic ) );
    addMember( REG_USTR( QLatin1String( "bottom" ) ), QSMember( QSMember::Custom, 7, AttributePublic ) );
    addMember( REG_USTR( QLatin1String( "center" ) ), QSMember( QSMember::Custom, 8, AttributePublic ) );
    addMember( REG_USTR( QLatin1String( "isNull" ) ), QSMember( &isNull ) );
    addMember( REG_USTR( QLatin1String( "isEmpty" ) ), QSMember( &isEmpty ) );
    addMember( REG_USTR( QLatin1String( "contains" ) ), QSMember( &contains ) );
    addMember( REG_USTR( QLatin1String( "intersection" ) ), QSMember( &intersection ) );
    addMember( REG_USTR( QLatin1String( "union" ) ), QSMember( &union_ ) );
    addMember( REG_USTR( QLatin1String( "intersects" ) ), QSMember( &intersects ) );
    addMember( REG_USTR( QLatin1String( "normalize" ) ), QSMember( &normalize ) );
    addMember( REG_USTR( QLatin1String( "moveLeft" ) ), QSMember( &moveLeft ) );
    addMember( REG_USTR( QLatin1String( "moveRight" ) ), QSMember( &moveRight ) );
    addMember( REG_USTR( QLatin1String( "moveTop" ) ), QSMember( &moveTop ) );
    addMember( REG_USTR( QLatin1String( "moveBottom" ) ), QSMember( &moveBottom ) );
    addMember( REG_USTR( QLatin1String( "moveBy" ) ), QSMember( &moveBy ) );
}


QSObject QSRectClass::fetchValue( const QSObject *o,
                                  const QSMember &mem ) const
{
    if ( mem.type() == QSMember::Custom ) {
        switch ( mem.idx ) {
        case 0:
        case 1:
            return createNumber( rect( o ) ->x() );
        case 2:
        case 3:
            return createNumber( rect( o ) ->y() );
        case 4:
            return createNumber( rect( o ) ->width() );
        case 5:
            return createNumber( rect( o ) ->height() );
        case 6:
            return createNumber( rect( o ) ->right() );
        case 7:
            return createNumber( rect( o ) ->bottom() );
        case 8:
            return pointClass() ->construct( rect( o ) ->center() );
        default:
            qFatal( "QSRectClass::fetchValue: unhandled case" );
            return createUndefined();
        }
    }
    return QSClass::fetchValue( o, mem );
}


void QSRectClass::write( QSObject *o, const QSMember &mem,
                         const QSObject &val ) const
{
    if ( mem.type() == QSMember::Custom ) {
        int i = val.toInteger();
        switch ( mem.idx ) {
        case 0:
        case 1:
            rect( o ) ->setX( i );
            break;
        case 2:
        case 3:
            rect( o ) ->setY( i );
            break;
        case 4:
            rect( o ) ->setWidth( i );
            break;
        case 5:
            rect( o ) ->setHeight( i );
            break;
        case 6:
            rect( o ) ->setRight( i );
            break;
        case 7:
            rect( o ) ->setBottom( i );
            break;
        case 8:
            break;
        default:
            qFatal( "QSRectClass::write: unhandled case" );
        }
    } else {
        QSClass::write( o, mem, val );
    }
}


QRect *QSRectClass::rect( const QSObject *obj ) const
{
    assert( obj->objectType() == this );
    return &( static_cast<RectShared *>( obj->shVal() ) ->rect );
}

QRect *QSRectClass::rect( QSEnv *e )
{
    return & ( static_cast<RectShared *>( e->thisValue().shVal() ) ->rect );
}

QString QSRectClass::toString( const QSObject *obj ) const
{
    QRect * r = rect( obj );
    return QString( QLatin1String( "(%1, %2, %3, %4)" ) ).arg( r->x() ).arg( r->y() )
           .arg( r->width() ).arg( r->height() );
}

QVariant QSRectClass::toVariant( const QSObject *obj, QVariant::Type type ) const
{
    if ( type == QVariant::RectF )
        return QRectF( *rect( obj ) );
    else
        return *rect( obj );
}

QSObject QSRectClass::construct( const QSList &args ) const
{
    if ( args.size() == 1 ) {
        if ( args[ 0 ].objectType() == this ) {
            QSObject rt = args.at( 0 );
            return construct( QRect( *rect( &rt ) ) );
        }
    } else if ( args.size() == 4 ) {
        return construct( QRect( args[ 0 ].toInteger(),
                                 args[ 1 ].toInteger(),
                                 args[ 2 ].toInteger(),
                                 args[ 3 ].toInteger() ) );
    }

    return construct( QRect() );
}

QSObject QSRectClass::construct( const QRect &r ) const
{
    return QSObject( this, new RectShared( r ) );
}

QSEqualsResult QSRectClass::isEqual( const QSObject &a, const QSObject &b ) const
{
    if ( !b.isA( this ) )
        return EqualsNotEqual;
    return ( QSEqualsResult ) ( *rect( &a ) == *rect( &b ) );
}

QSObject QSRectClass::isNull( QSEnv *env )
{
    QRect * r = rect( env );
    return env->createBoolean( r->isNull() );
}

QSObject QSRectClass::isEmpty( QSEnv *env )
{
    QRect * r = rect( env );
    return env->createBoolean( r->isEmpty() );
}

QSObject QSRectClass::contains( QSEnv *env )
{
    QRect * r = rect( env );

    if ( env->numArgs() < 1 || env->numArgs() > 2 ) {
        env->throwError( QString( QLatin1String( "Rect.contains() called with %1 arguments. 1 or 2 arguments expected." ) ).
                         arg( env->numArgs() ) );
        return env->createUndefined();
    }

    if ( env->numArgs() == 1 ) {
        QSObject a0 = env->arg( 0 );
        if ( !a0.isA( "Point" ) ) {
            env->throwError( QString( QLatin1String( "Rect.contains() called with an argument of type %1. "
                                      "Type Point is expeced" ) ).
                             arg( a0.typeName() ) );
            return env->createUndefined();
        }

        return env->createBoolean( r->contains( *( static_cast<const QSPointClass *>( a0.objectType() ) ->point( &a0 ) ) ) );
    } else {
        if ( !env->arg( 0 ).isNumber() || !env->arg( 1 ).isNumber() ) {
            env->throwError( QString( QLatin1String( "Rect.contains() called with arguments of type %1 and %2. "
                                      "Type Number and Number is expeced" ) ).
                             arg( env->arg( 0 ).typeName() ).arg( env->arg( 1 ).typeName() ) );
            return env->createUndefined();
        }
        return env->createBoolean( r->contains( env->arg( 0 ).toInteger(),
                                                env->arg( 1 ).toInteger() ) );
    }

    return env->createUndefined();
}

QSObject QSRectClass::intersection( QSEnv *env )
{
    QSObject t = env->thisValue();
    const QSRectClass *cl = static_cast<const QSRectClass *>( t.objectType() );
    QRect *r = cl->rect( &t );

    if ( env->numArgs() != 1 ) {
        env->throwError( QString( QLatin1String( "Rect.intersection() called with %1 arguments. 1 argument expected." ) ).
                         arg( env->numArgs() ) );
        return env->createUndefined();
    }

    QSObject a0 = env->arg( 0 );
    if ( !a0.isA( cl ) ) {
        env->throwError( QString( QLatin1String( "Rect.intersection() called with an argument of type %1. "
                                  "Type Rect is expeced" ) ).
                         arg( a0.typeName() ) );
        return env->createUndefined();
    }

    return cl->construct( r->intersect( *cl->rect( &a0 ) ) );
}

QSObject QSRectClass::union_( QSEnv *env )
{
    QSObject t = env->thisValue();
    const QSRectClass *cl = static_cast<const QSRectClass *>( t.objectType() );
    QRect *r = cl->rect( &t );

    if ( env->numArgs() != 1 ) {
        env->throwError( QString( QLatin1String( "Rect.union() called with %1 arguments. 1 argument expected." ) ).
                         arg( env->numArgs() ) );
        return env->createUndefined();
    }

    QSObject a0 = env->arg( 0 );
    if ( !a0.isA( cl ) ) {
        env->throwError( QString( QLatin1String( "Rect.union() called with an argument of type %1. "
                                  "Type Rect is expeced" ) ).
                         arg( a0.typeName() ) );
        return env->createUndefined();
    }

    return cl->construct( r->unite( *cl->rect( &a0 ) ) );
}

QSObject QSRectClass::intersects( QSEnv *env )
{
    QSObject t = env->thisValue();
    const QSRectClass *cl = static_cast<const QSRectClass *>( t.objectType() );
    QRect *r = cl->rect( &t );

    if ( env->numArgs() != 1 ) {
        env->throwError( QString( QLatin1String( "Rect.intersects() called with %1 arguments. 1 argument expected." ) ).
                         arg( env->numArgs() ) );
        return env->createUndefined();
    }

    QSObject a0 = env->arg( 0 );
    if ( !a0.isA( cl ) ) {
        env->throwError( QString( QLatin1String( "Rect.intersects() called with an argument of type %1. "
                                  "Type Rect is expeced" ) ).
                         arg( a0.typeName() ) );
        return env->createUndefined();
    }

    return env->createBoolean( r->intersects( *cl->rect( &a0 ) ) );
}

QSObject QSRectClass::normalize( QSEnv *env )
{
    QSObject t = env->thisValue();
    const QSRectClass *cl = static_cast<const QSRectClass *>( t.objectType() );
    QRect *r = cl->rect( &t );
    return cl->construct( r->normalized() );
}

void QSRectClass::moveLeft( QSEnv *env )
{
    QRect * r = rect( env );

    if ( env->numArgs() != 1 ) {
        env->throwError( QString( QLatin1String( "Rect.moveLeft() called with %1 arguments. 1 argument expected." ) ).
                         arg( env->numArgs() ) );
        return ;
    }

    if ( !env->arg( 0 ).isNumber() ) {
        env->throwError( QString( QLatin1String( "Rect.moveLeft() called with an argument of type %1. "
                                  "Type Number is expeced" ) ).
                         arg( env->arg( 0 ).typeName() ) );
        return ;
    }

    r->moveLeft( env->arg( 0 ).toInteger() );
}

void QSRectClass::moveRight( QSEnv *env )
{
    QRect * r = rect( env );

    if ( env->numArgs() != 1 ) {
        env->throwError( QString( QLatin1String( "Rect.moveRight() called with %1 arguments. 1 argument expected." ) ).
                         arg( env->numArgs() ) );
        return ;
    }

    if ( !env->arg( 0 ).isNumber() ) {
        env->throwError( QString( QLatin1String( "Rect.moveRight() called with an argument of type %1. "
                                  "Type Number is expeced" ) ).
                         arg( env->arg( 0 ).typeName() ) );
        return ;
    }

    r->moveRight( env->arg( 0 ).toInteger() );
}

void QSRectClass::moveTop( QSEnv *env )
{
    QRect * r = rect( env );

    if ( env->numArgs() != 1 ) {
        env->throwError( QString( QLatin1String( "Rect.moveTop() called with %1 arguments. 1 argument expected." ) ).
                         arg( env->numArgs() ) );
        return ;
    }

    if ( !env->arg( 0 ).isNumber() ) {
        env->throwError( QString( QLatin1String( "Rect.moveTop() called with an argument of type %1. "
                                  "Type Number is expeced" ) ).
                         arg( env->arg( 0 ).typeName() ) );
        return ;
    }

    r->moveTop( env->arg( 0 ).toInteger() );
}

void QSRectClass::moveBottom( QSEnv *env )
{
    QRect * r = rect( env );

    if ( env->numArgs() != 1 ) {
        env->throwError( QString( QLatin1String( "Rect.moveBottom() called with %1 arguments. 1 argument expected." ) ).
                         arg( env->numArgs() ) );
        return ;
    }

    if ( !env->arg( 0 ).isNumber() ) {
        env->throwError( QString( QLatin1String( "Rect.moveBottom() called with an argument of type %1. "
                                  "Type Number is expeced" ) ).
                         arg( env->arg( 0 ).typeName() ) );
        return ;
    }

    r->moveBottom( env->arg( 0 ).toInteger() );
}

void QSRectClass::moveBy( QSEnv *env )
{
    QRect * r = rect( env );

    if ( env->numArgs() != 2 ) {
        env->throwError( QString( QLatin1String( "Rect.moveBy() called with %1 arguments. 2 arguments expected." ) ).
                         arg( env->numArgs() ) );
        return ;
    }

    if ( !env->arg( 0 ).isNumber() || !env->arg( 1 ).isNumber() ) {
        env->throwError( QString( QLatin1String( "Rect.moveBy() called with arguments of type %1 and %2. "
                                  "Type Number and Number are expeced" ) ).
                         arg( env->arg( 0 ).typeName() ).arg( env->arg( 1 ).typeName() ) );
        return ;
    }

    r->moveTo( r->left() + env->arg( 0 ).toInteger(), r->top() + env->arg( 1 ).toInteger() );
}

