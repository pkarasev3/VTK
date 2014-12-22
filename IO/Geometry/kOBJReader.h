/*=========================================================================

  Program:   Visualization Toolkit
  Module:    kOBJReader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME kOBJReader - read Wavefront .obj files
// .SECTION Description
// kOBJReader is a source object that reads Wavefront .obj
// files. The output of this source object is polygonal data.
// .SECTION See Also
// vtkOBJImporter (wtf doesn't exist...)

#ifndef __kOBJReader_h
#define __kOBJReader_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkSmartPointer.h"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>
#include <cstdio>
#include <boost/filesystem.hpp>
#include "vtkActor.h"
#include "mtl_parser.h"
#define SP(X) vtkSmartPointer< X >


using std::string;
using std::vector;

class VTK_IO_EXPORT kOBJReader : public vtkPolyDataAlgorithm
{
public:
  static kOBJReader *New();
  vtkTypeMacro(kOBJReader,vtkPolyDataAlgorithm);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Specify file name of Wavefront .obj file.
  void SetFileName(const char* arg)
  { // by default assume prefix.obj => prefix.obj.mtl
    FileName    = std::string(arg);
    MTLfilename = FileName + ".mtl";
  }
  void SetMTLfileName( const char* arg ) {
    MTLfilename = std::string(arg);
  }

  vtkSetMacro(VertexScale,double);
  vtkGetMacro(VertexScale,double);

  struct RawPolyData_mit_Material;

  virtual vtkPolyData* GetOutput(int idx) {
    if( idx < (int)outVector_of_vtkPolyData.size() )
      return outVector_of_vtkPolyData[idx];
    else
      return NULL;
  }

  boost::shared_ptr<obj_material>  GetMaterial(int k);

  std::string GetTextureFilename( int idx ); // return string by index

protected:
  kOBJReader();
  ~kOBJReader();
  int RequestData(vtkInformation *,
                  vtkInformationVector **, vtkInformationVector *);
  std::string FileName;     // filename (.obj) being read
  std::string MTLfilename;  // associated .mtl to *.obj, typically it is *.obj.mtl
  double VertexScale; // scale vertices by this during import

  std::map<std::string,boost::shared_ptr<obj_material> >  mtlName_to_mtlData;

  // our internal parsing/storage
  std::vector<boost::shared_ptr<RawPolyData_mit_Material> > poly_list;

  // what gets returned to client code via GetOutput()
  std::vector<vtkSmartPointer<vtkPolyData> >  outVector_of_vtkPolyData;

public:
  std::vector<std::string> outVector_of_textureFilnames;

  std::vector<vtkSmartPointer<vtkActor> >  actor_list;

private:
  kOBJReader(const kOBJReader&);  // Not implemented.
  void operator=(const kOBJReader&);  // Not implemented.
};



#include <vtkJPEGReader.h>
#include <vtkTexture.h>
#include <vtkPNGReader.h>
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"

void  bindTexturedPolydataToRenderWindow( vtkRenderWindow* renderWindow,
                                          vtkRenderer* renderer,
                                          kOBJReader* reader );


#endif
