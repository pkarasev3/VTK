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
#include <memory>
#include <vector>
#include <map>
#include <cstdio>
#include "vtkActor.h"

#include "vtkIOImportModule.h" // For export macro
#include "vtkImporter.h"

class vtkPolyData;

// class  vtk3DSImporter : public vtkImporter

#include <string>
#include <math.h>

#define OBJ_FILENAME_LENGTH 500
#define MATERIAL_NAME_SIZE 255

using std::vector;
using std::shared_ptr;

struct obj_material
{
  char name[MATERIAL_NAME_SIZE];
  char texture_filename[OBJ_FILENAME_LENGTH];
  double amb[3];
  double diff[3];
  double spec[3];
  double reflect;
  double refract;
  double trans;
  double shiny;
  double glossy;
  double refract_index;
  double get_amb_coeff() {
    return sqrt( amb[0]*amb[0]+amb[1]*amb[1]+amb[2]*amb[2] );
  }
  double get_diff_coeff() {
    return sqrt( diff[0]*diff[0]+diff[1]*diff[1]+diff[2]*diff[2] );
  }
  double get_spec_coeff() {
    return sqrt( spec[0]*spec[0]+spec[1]*spec[1]+spec[2]*spec[2] );
  }
};

void obj_set_material_defaults(shared_ptr<obj_material> mtl);

vector<shared_ptr<obj_material> > obj_parse_mtl_file(std::string filename,int& result_code);


class VTKIOIMPORT_EXPORT kOBJReader : public vtkPolyDataAlgorithm
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

  std::shared_ptr<obj_material>  GetMaterial(int k);

  std::string GetTextureFilename( int idx ); // return string by index

protected:
  kOBJReader();
  ~kOBJReader();
  int RequestData(vtkInformation *,
                  vtkInformationVector **, vtkInformationVector *) override;
  std::string FileName;     // filename (.obj) being read
  std::string MTLfilename;  // associated .mtl to *.obj, typically it is *.obj.mtl
  double VertexScale; // scale vertices by this during import

  std::map<std::string,std::shared_ptr<obj_material> >  mtlName_to_mtlData;

  // our internal parsing/storage
  std::vector<std::shared_ptr<RawPolyData_mit_Material> > poly_list;

  // what gets returned to client code via GetOutput()
  std::vector<vtkSmartPointer<vtkPolyData> >  outVector_of_vtkPolyData;

public:
  std::vector<std::string> outVector_of_textureFilnames;

  std::vector<vtkSmartPointer<vtkActor> >  actor_list;

private:
  kOBJReader(const kOBJReader&);  // Not implemented.
  void operator=(const kOBJReader&);  // Not implemented.
};



class vtkRenderWindow;
class vtkRenderer;

void  bindTexturedPolydataToRenderWindow( vtkRenderWindow* renderWindow,
                                          vtkRenderer* renderer,
                                          kOBJReader* reader );


#endif
