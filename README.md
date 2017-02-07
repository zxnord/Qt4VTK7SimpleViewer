# Qt4VTK7SimpleViewer
Simple example to track VTK, Mesa and Qt4 memory leaks

The main purpose of this example is to have a test environment
for checking memory leaks asociated to Mesa-12+ and/or VTK-7.x and/or Qt-4.x

This code has been tested under the following environments:

CentOS 6.6:
- VTK-7.1 (debug/leaks C++11)
- Qt-4.7.2 (C++98)
- Mesa-12.0.0 (llvm-3.6)
- Source code (C++11)
- GCC-5.2.0

ArchLinux:
- VTK-7.1 (debug/leaks C++11)
- Qt-4.8.7 (repository)
- Mesa-13.0.2-DRI (repository)
- Source code (C++11)
- GCC-6.2.1

The test case is to generate a new plot, then delete it, then create it again
and you will be able to notice that memory is always increasing even though
the actors, renderer and render widget were removed (using vtkDebugLeaks::PrintCurrentLeaks())

Valgrind log is:

TBA

If you have VTK built in a non-standard path, before running qmake, please set the following environment variables:

VTK_7_INCLUDE_PATH=/your/remote/location/include/vtk-7.1

VTK_7_LIBRARY_PATH=/your/remote/location/lib

Any other usage that you might give to this software is welcomed and I hope 
having feedback from you
