#-------------------------------------------------------------------------------
#
# Project created by QtCreator 2017-01-09T12:30:07
#
# Copyright 2017 Edson Contreras
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

#-------------------------------------------------------------------------------

QT       += core gui opengl

TARGET = QtVTKViewer
TEMPLATE = app

#Note: Even though the program has the c++11 flag forced,
#      it is not necessary to build Qt4 with c++11 support
QMAKE_CXXFLAGS += -std=c++11


SOURCES += \
  ./src/main.cpp\
  ./src/MainWindow.cpp \
  ./src/PlotHD.cpp \
  ./src/Geometry.cpp \
  ./src/MyVTKApplication.cpp \
  ./src/AboutDialog.cpp \
  ./src/GeometryPartRepresentation.cpp \
  ./src/GeometryPart.cpp \
  ./src/GeometryFactory.cpp \
    src/GeometryPropertiesDialog.cpp \
    src/GeometrySettings.cpp

HEADERS  += \
  ./src/MainWindow.h \
  ./src/PlotHD.h \
  ./src/Geometry.h \
  ./src/MyVTKApplication.h \
  ./src/AboutDialog.h \
  ./src/GeometryPartRepresentation.h \
  ./src/GeometryPart.h \
  ./src/GeometryFactory.h \
    src/GeometryPropertiesDialog.h \
    src/GeometrySettings.h

FORMS    += \
  ./src/ui/MainWindow.ui \
  ./src/ui/AboutDialog.ui \
    src/ui/GeometryPropertiesDialog.ui

INCLUDEPATH += \
  $$(VTK_7_INCLUDE_PATH)

LIBS += \
  -L$$(VTK_7_LIBRARY_PATH) \
  -lvtkCommonColor-7.1 \
  -lvtkCommonCore-7.1 \
  -lvtkCommonComputationalGeometry-7.1 \
  -lvtkCommonDataModel-7.1 \
  -lvtkCommonExecutionModel-7.1 \
  -lvtkCommonMath-7.1 \
  -lvtkCommonMisc-7.1 \
  -lvtkCommonSystem-7.1 \
  -lvtkCommonTransforms-7.1 \
  -lvtkDICOMParser-7.1 \
  -lvtkFiltersCore-7.1 \
  -lvtkFiltersGeneral-7.1 \
  -lvtkFiltersGeometry-7.1 \
  -lvtkFiltersModeling-7.1 \
  -lvtkFiltersExtraction-7.1 \
  -lvtkFiltersSources-7.1 \
  -lvtkFiltersStatistics-7.1 \
  -lvtkGUISupportQt-7.1 \
  -lvtkGUISupportQtOpenGL-7.1 \
  -lvtkIOImage-7.1 \
  -lvtkImagingCore-7.1 \
  -lvtkImagingFourier-7.1 \
  -lvtkInteractionStyle-7.1 \
  -lvtkRenderingCore-7.1 \
  -lvtkRenderingOpenGL2-7.1 \
  -lvtkRenderingFreeType-7.1 \
  -lvtkalglib-7.1 \
  -lvtkfreetype-7.1 \
  -lvtkglew-7.1 \
  -lvtkjpeg-7.1 \
  -lvtkmetaio-7.1 \
  -lvtkpng-7.1 \
  -lvtksys-7.1 \
  -lvtktiff-7.1 \
  -lvtkzlib-7.1
