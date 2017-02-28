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
#include "GeometryPartMoleculeRepresentation.h"

#include "GeometryPartMolecule.h"

#include <vtkActor.h>
#include <vtkMolecule.h>
#include <vtkMoleculeMapper.h>

//------------------------------------------------------------------------------

GeometryPartMoleculeRepresentation::GeometryPartMoleculeRepresentation(
  const std::unique_ptr<GeometryPart>& part)
  :
  GeometryPartRepresentation(part)
{
  m_datasetActor = vtkSmartPointer<vtkActor>();
  m_datasetLinesActor = vtkSmartPointer<vtkActor>();

  updateMoleculeActor();
  m_modified = true;
}

//------------------------------------------------------------------------------

void GeometryPartMoleculeRepresentation::updateMoleculeActor()
{
  m_solidActor->VisibilityOff();

  vtkSmartPointer<vtkMoleculeMapper> molMapper =
    vtkSmartPointer<vtkMoleculeMapper>::New();

  molMapper->SetInputData( static_cast<GeometryPartMolecule*>(
    m_geomPart.get().get())->getMoleculeData() );
//  molMapper->UseFastSettings();
  molMapper->UseBallAndStickSettings();
//  molMapper->UseVDWSpheresSettings();
//  molMapper->UseLiquoriceStickSettings();
  molMapper->UseMultiCylindersForBondsOn();
  molMapper->SetAtomicRadiusScaleFactor(0.1);
  molMapper->SetBondRadius(0.04);

  m_solidActor->SetMapper(molMapper);
  m_solidActor->VisibilityOn();
}

//------------------------------------------------------------------------------

void GeometryPartMoleculeRepresentation::customEvent(QEvent* ev)
{
  if( (ev->type() == UpdateActorsEvent) && m_modified )
  {
    m_modified = false;
    updateMoleculeActor();
  }

  QObject::customEvent(ev);
}

//------------------------------------------------------------------------------
