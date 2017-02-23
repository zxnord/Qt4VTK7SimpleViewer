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
#ifndef GEOMETRYSETTINGS_H
#define GEOMETRYSETTINGS_H

#include <QEvent>
#include <QObject>
#include <QVariant>

#include <vtkSmartPointer.h>

#include <functional>
#include <memory>
#include <vector>

class Geometry;
class GeometryPartRepresentation;

class vtkRenderer;

class GeometrySettings : public QObject
{
  Q_OBJECT

public:
  static const QEvent::Type GeometryUpdated = QEvent::Type(5001);
  static const QEvent::Type GeometryDeleted = QEvent::Type(5002);

  explicit GeometrySettings(
    const std::unique_ptr<Geometry>& geom,
    QObject* parent = 0);
  GeometrySettings(GeometrySettings&&);
  virtual ~GeometrySettings();

  void addGeometryActorsToRenderer(vtkSmartPointer<vtkRenderer>);

  void registerToUpdate(QObject*);
  void registerToDelete(QObject*);

  const QString& getGeometryName() const;
  QStringList    getGeometryPartsNames() const;

  bool getGeometryPartSolidColor(const QString&, QColor&);
  bool getGeometryPartDatasetLinesColor(const QString&, QColor&);

  bool getGeometryPartShowSolid(const QString&, bool&);
  bool getGeometryPartShowDataset(const QString&, bool&);
  bool getGeometryPartShowDatasetLines(const QString&, bool&);

  bool getGeometryPartNofDatasetLines(const QString&, int&);

  bool setGeometryPartSolidColor(const QString&, const QColor&);
  bool setGeometryPartDatasetLinesColor(const QString&, const QColor&);

  bool setGeometryPartShowSolid(const QString&, bool);
  bool setGeometryPartShowDataset(const QString&, bool);
  bool setGeometryPartShowDatasetLines(const QString&, bool);

  bool setGeometryPartNofDatasetLines(const QString&, int);
signals:

public slots:

protected:
  virtual void customEvent(QEvent*);
  void createPartRepresentations();
  std::unique_ptr<GeometryPartRepresentation>& getGeometryPartByName(
    const QString&);

  std::vector<QObject*> m_toUpdate;
  std::vector<QObject*> m_toDelete;

  std::reference_wrapper<const std::unique_ptr<Geometry>> m_geom;
  std::vector<std::unique_ptr<GeometryPartRepresentation>> m_geometryParts;
};

#endif // GEOMETRYSETTINGS_H
