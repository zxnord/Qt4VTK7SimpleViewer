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

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QList>
#include <QMap>
#include <QObject>

#include <vtkSmartPointer.h>

#include <memory>

class vtkAlgorithmOutput;
class vtkDoubleArray;
class vtkPolyData;
class vtkPassThrough;

class GeometryPart;

class Geometry : public QObject
{
  Q_OBJECT
public:
  explicit Geometry(const QString& name, QObject* parent = 0);
  ~Geometry();

  void addPart(std::unique_ptr<GeometryPart> part);
  QList<std::weak_ptr<GeometryPart>> getParts() const;

  QMap<QString, double*> getPointDatasetsInfo() const;
  QMap<QString, double*> getCellDatasetsInfo() const;

  const QString& getName() const;

protected:
  const QString                        m_name;
  QMap<QString, double*>               m_pointDatasetsInfo;
  QMap<QString, double*>               m_cellDatasetsInfo;
  QList<std::shared_ptr<GeometryPart>> m_geomParts;
};

#endif // GEOMETRY_H