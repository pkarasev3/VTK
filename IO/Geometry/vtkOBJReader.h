/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtvtkOBJImporter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtvtkOBJImporter - read Wavefront .obj files
// .SECTION Description
// vtvtkOBJImporter is a source object that reads Wavefront .obj
// files. The output of this source object is polygonal data.
// .SECTION See Also
// vtkOBJImporter

#ifndef __vtvtkOBJImporter_h
#define __vtvtkOBJImporter_h

#include "vtkIOGeometryModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

class VTKIOGEOMETRY_EXPORT vtvtkOBJImporter : public vtkPolyDataAlgorithm
{
public:
  static vtvtkOBJImporter *New();
  vtkTypeMacro(vtvtkOBJImporter,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Specify file name of Wavefront .obj file.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

protected:
  vtvtkOBJImporter();
  ~vtvtkOBJImporter();

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  char *FileName;
private:
  vtvtkOBJImporter(const vtvtkOBJImporter&);  // Not implemented.
  void operator=(const vtvtkOBJImporter&);  // Not implemented.
};

#endif
