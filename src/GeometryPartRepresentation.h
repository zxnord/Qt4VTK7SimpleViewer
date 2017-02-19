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

#ifndef GEOMETRYPARTREPRESENTATION_H
#define GEOMETRYPARTREPRESENTATION_H

#include <QColor>
#include <QEvent>
#include <QList>
#include <QObject>
#include <QPair>
#include <QString>

#include <vtkSmartPointer.h>
#include <vtkWeakPointer.h>

#include <memory>

class vtkActor;
class vtkRenderer;

class GeometryPart;

class GeometryPartRepresentation : public QObject
{
  Q_OBJECT
public:
  static const QEvent::Type UpdateActorsEvent = static_cast<QEvent::Type>(2000);

  Q_PROPERTY(int nofBands READ getNofBands WRITE setNofBands)
  Q_PROPERTY(QColor solidColor READ getSolidColor WRITE setSolidColor)
  Q_PROPERTY(QColor contoursColor READ getContoursColor WRITE setContoursColor)
  Q_PROPERTY(bool showSolid READ isShowSolid WRITE setShowSolid)
  Q_PROPERTY(bool showDataset READ isShowDataset WRITE setShowDataset)
  Q_PROPERTY(bool showDatasetLines READ isShowDatasetLines WRITE setShowDatasetLines)

  explicit GeometryPartRepresentation(
    std::weak_ptr<GeometryPart> geomPart,
    QObject* parent = 0 );

  GeometryPartRepresentation(GeometryPartRepresentation&&);

  GeometryPartRepresentation& operator =(GeometryPartRepresentation&&);

  virtual ~GeometryPartRepresentation();

  vtkSmartPointer<vtkActor>& getSolidActor();
  vtkSmartPointer<vtkActor>& getDatasetActor();
  vtkSmartPointer<vtkActor>& getDatasetLinesActor();

  const QPair<QString, double*>& getDatasetInfo() const;
  int getNofBands() const;
  const QColor& getSolidColor() const;
  const QColor& getContoursColor() const;
  const QString& getPartName() const;

  bool isModified() const;
  bool isShowSolid() const;
  bool isShowDataset() const;
  bool isShowDatasetLines() const;

  void setDatasetInfo(const QPair<QString, double*>& info);
  void setNofBands(int nofBands);
  void setSolidColor(const QColor& color);
  void setContoursColor(const QColor& color);
  void setShowSolid(bool);
  void setShowDataset(bool);
  void setShowDatasetLines(bool);


signals:

public slots:

protected slots:

protected:
  virtual void customEvent(QEvent *);
  void updateSolidPartActor();
  void updateDatasetPartActor();

  std::weak_ptr<GeometryPart> m_geomPart;

  QPair<QString, double*> m_datasetInfo;
  QColor  m_solidColor;
  QColor  m_contoursColor;

  int    m_nofBands;
  bool   m_oldVisibility[3];

  bool   m_showSolid;
  bool   m_showDataset;
  bool   m_showDatasetLines;
  bool   m_modified;

  vtkSmartPointer<vtkActor> m_solidActor;
  vtkSmartPointer<vtkActor> m_datasetActor;
  vtkSmartPointer<vtkActor> m_datasetLinesActor;
};

#endif // GEOMETRYPARTREPRESENTATION_H
