/***************************************************************************
    qgssymbollayerregistry.cpp
    ---------------------
    begin                : November 2009
    copyright            : (C) 2009 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgssymbollayerregistry.h"

#include "qgsarrowsymbollayer.h"
#include "qgsellipsesymbollayerv2.h"
#include "qgsmarkersymbollayerv2.h"
#include "qgslinesymbollayerv2.h"
#include "qgsfillsymbollayerv2.h"
#include "qgsvectorfieldsymbollayer.h"
#include "qgsgeometrygeneratorsymbollayerv2.h"

QgsSymbolLayerRegistry::QgsSymbolLayerRegistry()
{
  // init registry with known symbol layers
  addSymbolLayerType( new QgsSymbolLayerMetadata( "SimpleLine", QObject::tr( "Simple line" ), QgsSymbol::Line,
                      QgsSimpleLineSymbolLayerV2::create, QgsSimpleLineSymbolLayerV2::createFromSld ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "MarkerLine", QObject::tr( "Marker line" ), QgsSymbol::Line,
                      QgsMarkerLineSymbolLayerV2::create, QgsMarkerLineSymbolLayerV2::createFromSld ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "ArrowLine", QObject::tr( "Arrow" ), QgsSymbol::Line, QgsArrowSymbolLayer::create ) );

  addSymbolLayerType( new QgsSymbolLayerMetadata( "SimpleMarker", QObject::tr( "Simple marker" ), QgsSymbol::Marker,
                      QgsSimpleMarkerSymbolLayerV2::create, QgsSimpleMarkerSymbolLayerV2::createFromSld ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "FilledMarker", QObject::tr( "Filled marker" ), QgsSymbol::Marker,
                      QgsFilledMarkerSymbolLayer::create ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "SvgMarker", QObject::tr( "SVG marker" ), QgsSymbol::Marker,
                      QgsSvgMarkerSymbolLayerV2::create, QgsSvgMarkerSymbolLayerV2::createFromSld ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "FontMarker", QObject::tr( "Font marker" ), QgsSymbol::Marker,
                      QgsFontMarkerSymbolLayerV2::create, QgsFontMarkerSymbolLayerV2::createFromSld ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "EllipseMarker", QObject::tr( "Ellipse marker" ), QgsSymbol::Marker,
                      QgsEllipseSymbolLayerV2::create, QgsEllipseSymbolLayerV2::createFromSld ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "VectorField", QObject::tr( "Vector field marker" ), QgsSymbol::Marker,
                      QgsVectorFieldSymbolLayer::create ) );

  addSymbolLayerType( new QgsSymbolLayerMetadata( "SimpleFill", QObject::tr( "Simple fill" ), QgsSymbol::Fill,
                      QgsSimpleFillSymbolLayerV2::create, QgsSimpleFillSymbolLayerV2::createFromSld ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "GradientFill", QObject::tr( "Gradient fill" ), QgsSymbol::Fill,
                      QgsGradientFillSymbolLayerV2::create ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "ShapeburstFill", QObject::tr( "Shapeburst fill" ), QgsSymbol::Fill,
                      QgsShapeburstFillSymbolLayerV2::create ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "RasterFill", QObject::tr( "Raster image fill" ), QgsSymbol::Fill,
                      QgsRasterFillSymbolLayer::create ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "SVGFill", QObject::tr( "SVG fill" ), QgsSymbol::Fill,
                      QgsSVGFillSymbolLayer::create, QgsSVGFillSymbolLayer::createFromSld ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "CentroidFill", QObject::tr( "Centroid fill" ), QgsSymbol::Fill,
                      QgsCentroidFillSymbolLayerV2::create, QgsCentroidFillSymbolLayerV2::createFromSld ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "LinePatternFill", QObject::tr( "Line pattern fill" ), QgsSymbol::Fill,
                      QgsLinePatternFillSymbolLayer::create, QgsLinePatternFillSymbolLayer::createFromSld ) );
  addSymbolLayerType( new QgsSymbolLayerMetadata( "PointPatternFill", QObject::tr( "Point pattern fill" ), QgsSymbol::Fill,
                      QgsPointPatternFillSymbolLayer::create, QgsPointPatternFillSymbolLayer::createFromSld ) );

  addSymbolLayerType( new QgsSymbolLayerMetadata( "GeometryGenerator", QObject::tr( "Geometry generator" ), QgsSymbol::Hybrid,
                      QgsGeometryGeneratorSymbolLayerV2::create ) );
}

QgsSymbolLayerRegistry::~QgsSymbolLayerRegistry()
{
  qDeleteAll( mMetadata );
}

bool QgsSymbolLayerRegistry::addSymbolLayerType( QgsSymbolLayerAbstractMetadata* metadata )
{
  if ( !metadata || mMetadata.contains( metadata->name() ) )
    return false;

  mMetadata[metadata->name()] = metadata;
  return true;
}


QgsSymbolLayerAbstractMetadata* QgsSymbolLayerRegistry::symbolLayerMetadata( const QString& name ) const
{
  return mMetadata.value( name );
}

QgsSymbolLayerRegistry* QgsSymbolLayerRegistry::instance()
{
  static QgsSymbolLayerRegistry mInstance;
  return &mInstance;
}

QgsSymbolLayer* QgsSymbolLayerRegistry::defaultSymbolLayer( QgsSymbol::SymbolType type )
{
  switch ( type )
  {
    case QgsSymbol::Marker:
      return QgsSimpleMarkerSymbolLayerV2::create();

    case QgsSymbol::Line:
      return QgsSimpleLineSymbolLayerV2::create();

    case QgsSymbol::Fill:
      return QgsSimpleFillSymbolLayerV2::create();

    case QgsSymbol::Hybrid:
      return nullptr;
  }

  return nullptr;
}


QgsSymbolLayer* QgsSymbolLayerRegistry::createSymbolLayer( const QString& name, const QgsStringMap& properties ) const
{
  if ( !mMetadata.contains( name ) )
    return nullptr;

  return mMetadata[name]->createSymbolLayer( properties );
}

QgsSymbolLayer* QgsSymbolLayerRegistry::createSymbolLayerFromSld( const QString& name, QDomElement& element ) const
{
  if ( !mMetadata.contains( name ) )
    return nullptr;

  return mMetadata[name]->createSymbolLayerFromSld( element );
}

QStringList QgsSymbolLayerRegistry::symbolLayersForType( QgsSymbol::SymbolType type )
{
  QStringList lst;
  QMap<QString, QgsSymbolLayerAbstractMetadata*>::ConstIterator it = mMetadata.constBegin();
  for ( ; it != mMetadata.constEnd(); ++it )
  {
    if ( it.value()->type() == type || it.value()->type() == QgsSymbol::Hybrid )
      lst.append( it.key() );
  }
  return lst;
}
