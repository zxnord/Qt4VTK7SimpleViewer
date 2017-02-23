//------------------------------------------------------------------------------
// Copyright 2017 Edson Contreras

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//------------------------------------------------------------------------------

#include "Geometry.h"

#include "GeometryPart.h"

#include <vtkAlgorithmOutput.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkPassThrough.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>

#include <limits>

//------------------------------------------------------------------------------

void fillDatasetMap(vtkDataSetAttributes* att, QMap<QString, double*>& dataMap)
{
  if( !att )
  {
    return;
  }

  for( vtkIdType i = 0; i < att->GetNumberOfArrays(); ++i )
  {
    vtkDataArray* arr = att->GetArray(i);

    if( !dataMap.contains(arr->GetName()) )
    {
      dataMap[arr->GetName()] = new double [2] {
        std::numeric_limits<double>::max(),
        -std::numeric_limits<double>::max()};
    }

    dataMap[arr->GetName()][0] = qMin(
      dataMap[arr->GetName()][0],
      arr->GetRange()[0]);

    dataMap[arr->GetName()][1] = qMax(
      dataMap[arr->GetName()][1],
      arr->GetRange()[1]);
  }
}

//------------------------------------------------------------------------------

Geometry::Geometry(const QString& name, QObject *parent)
  :
  QObject(parent),
  m_name(name)
{
}

//------------------------------------------------------------------------------

Geometry::~Geometry()
{
  qDeleteAll(m_pointDatasetsInfo);
  qDeleteAll(m_cellDatasetsInfo);
}

//------------------------------------------------------------------------------

void Geometry::addPart(std::unique_ptr<GeometryPart>& part)
{
  if( !part )
  {
    return;
  }

  vtkPointData* pData = part->getGeometryData()->GetPointData();
  vtkCellData*  cData = part->getGeometryData()->GetCellData();

  fillDatasetMap(pData, m_pointDatasetsInfo);
  fillDatasetMap(cData, m_cellDatasetsInfo);

  m_geomParts.push_back( std::move(part) );
}

//------------------------------------------------------------------------------

const std::vector<std::unique_ptr<GeometryPart>>& Geometry::getParts() const
{
//  std::vector<std::unique_ptr<GeometryPart>&> parts;

//  for( std::unique_ptr<GeometryPart>& sPart : m_geomParts )
//  {
//    parts.push_back( sPart );
//  }

//  return parts;

  return m_geomParts;
}

//------------------------------------------------------------------------------

const QMap<QString, double*>& Geometry::getPointDatasetsInfo() const
{
  return m_pointDatasetsInfo;
}

//------------------------------------------------------------------------------

const QMap<QString, double*>& Geometry::getCellDatasetsInfo() const
{
  return m_cellDatasetsInfo;
}

//------------------------------------------------------------------------------

const QString& Geometry::getName() const
{
  return m_name;
}

//------------------------------------------------------------------------------

