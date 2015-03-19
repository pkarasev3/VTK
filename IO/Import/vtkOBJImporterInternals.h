#include <string>
#include "vtkOBJImporter.h"
#include "vtkPolyDataAlgorithm.h"
#include <memory>
#include <vector>
#include <map>
#include "vtkActor.h"

const int OBJ_FILENAME_LENGTH = 500;
const int MATERIAL_NAME_SIZE  = 255;

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


void obj_set_material_defaults(obj_material* mtl);

std::vector<obj_material*> obj_parse_mtl_file(std::string filename,int& result_code);

struct ImportedPolydataWithMaterial;

class vtkOBJPolydataProcessor : public vtkPolyDataAlgorithm
{
public:
  static vtkOBJPolydataProcessor *New();
  vtkTypeMacro(vtkOBJPolydataProcessor,vtkPolyDataAlgorithm);
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


  virtual vtkPolyData* GetOutput(int idx);

  //////////////////////

  obj_material*  GetMaterial(int k);

  std::string GetTextureFilename( int idx ); // return string by index

  double VertexScale; // scale vertices by this during import

  std::map<std::string,obj_material*>  mtlName_to_mtlData;

  // our internal parsing/storage
  std::vector<ImportedPolydataWithMaterial*> poly_list;

  // what gets returned to client code via GetOutput()
  std::vector<vtkSmartPointer<vtkPolyData> >  outVector_of_vtkPolyData;

  std::vector<std::string> outVector_of_textureFilnames;

  std::vector<vtkSmartPointer<vtkActor> >  actor_list;
  /////////////////////

protected:
  vtkOBJPolydataProcessor();
  ~vtkOBJPolydataProcessor();
  int RequestData(vtkInformation *,
                  vtkInformationVector **, vtkInformationVector *) /*override*/;

  std::string FileName;     // filename (.obj) being read
  std::string MTLfilename;  // associated .mtl to *.obj, typically it is *.obj.mtl

private:
  vtkOBJPolydataProcessor(const vtkOBJPolydataProcessor&);  // Not implemented.
  void operator=(const vtkOBJPolydataProcessor&);  // Not implemented.
};

class vtkRenderWindow;
class vtkRenderer;
void  bindTexturedPolydataToRenderWindow( vtkRenderWindow* renderWindow,
                                          vtkRenderer* renderer,
                                          vtkOBJPolydataProcessor* reader );
