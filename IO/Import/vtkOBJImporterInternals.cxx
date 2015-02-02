
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include "vtkOBJImporter.h"
#include "vtkOBJImporterInternals.h"
#include "vtkJPEGReader.h"
#include "vtkPNGReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#define OBJ_LINE_SIZE 500   // TODO: get rid of this

#define SP(X) vtkSmartPointer< X >

namespace
{

char strequal(const char *s1, const char *s2)
{
  if(strcmp(s1, s2) == 0)
    return 1;
  return 0;
}

char contains(const char *haystack, const char *needle)
{
  if(strstr(haystack, needle) == NULL)
    return 0;
  return 1;
}

}

//string   getImageFilename( std::shared_ptr ) {
//  if( 0 == mtlName_to_imgFile.count(key) ) {
//    cout << "Warning, requested string key " << key << " was not found." << endl;
//    return string("");
//  }  else {
//    return mtlName_to_imgFile[key];
//  }
//}

void obj_set_material_defaults(obj_material* mtl)
{
  mtl->amb[0] = 0.2;
  mtl->amb[1] = 0.2;
  mtl->amb[2] = 0.2;
  mtl->diff[0] = 0.8;
  mtl->diff[1] = 0.8;
  mtl->diff[2] = 0.8;
  mtl->spec[0] = 1.0;
  mtl->spec[1] = 1.0;
  mtl->spec[2] = 1.0;
  mtl->reflect = 0.0;
  mtl->trans = 1;
  mtl->glossy = 98;
  mtl->shiny = 0;
  mtl->refract_index = 1;
  mtl->texture_filename[0] = '\0';

  cout << "created a default obj_material, texture filename is "
       << std::string(mtl->texture_filename) << endl;
}

std::vector<obj_material*> obj_parse_mtl_file(std::string Filename,int& result_code)
{
  // Maybe this should return a map instead, e.g. mapping string keys to ptrToMaterialStruct values
  // problem is that we don't know the material name at creation-time of this thing ... d'oh

  std::vector<obj_material*>  listOfMaterials;
  result_code    = 0;
  const char* filename = Filename.c_str();


  int line_number = 0;
  char *current_token;
  char current_line[OBJ_LINE_SIZE];
  char material_open = 0;
  obj_material* current_mtl = NULL;
  FILE *mtl_file_stream;

  // open scene
  mtl_file_stream = fopen( filename, "r");
  if(mtl_file_stream == 0)
  {
    fprintf(stderr, "Error reading file: %s\n", filename);
    result_code = -1;
    return listOfMaterials;
  }

  //list_make(listOfMaterials, 10, 1);

  while( fgets(current_line, OBJ_LINE_SIZE, mtl_file_stream) )
  {
    current_token = strtok( current_line, " \t\n\r");
    line_number++;

    //skip comments
    if( current_token == NULL || strequal(current_token, "//") || strequal(current_token, "#"))
      continue;


    //start material
    else if( strequal(current_token, "newmtl"))
    {
      material_open = 1;
      current_mtl = (new obj_material);
      listOfMaterials.push_back(current_mtl);
      obj_set_material_defaults(current_mtl);

      // get the name
      strncpy(current_mtl->name, strtok(NULL, " \t\n"), MATERIAL_NAME_SIZE);
    }

    //ambient
    else if( strequal(current_token, "Ka") && material_open)
    {
      // THIS line is hit:
      cout << "got amb token: " << current_token << ", line is: " << current_line << endl;

      // But this is ... right? no?
      current_mtl->amb[0] = atof( strtok(NULL, " \t"));
      current_mtl->amb[1] = atof( strtok(NULL, " \t"));
      current_mtl->amb[2] = atof( strtok(NULL, " \t"));
      cout<<"current_mtl->amb[0]: " << current_mtl->amb[0] << endl; // seems good here. doesn't get read in vtk right...
    }

    //diff
    else if( strequal(current_token, "Kd") && material_open)
    {
      current_mtl->diff[0] = atof( strtok(NULL, " \t"));
      current_mtl->diff[1] = atof( strtok(NULL, " \t"));
      current_mtl->diff[2] = atof( strtok(NULL, " \t"));
    }

    //specular
    else if( strequal(current_token, "Ks") && material_open)
    {
      current_mtl->spec[0] = atof( strtok(NULL, " \t"));
      current_mtl->spec[1] = atof( strtok(NULL, " \t"));
      current_mtl->spec[2] = atof( strtok(NULL, " \t"));
    }
    //shiny
    else if( strequal(current_token, "Ns") && material_open)
    {
      current_mtl->shiny = atof( strtok(NULL, " \t"));
    }
    //transparent
    else if( strequal(current_token, "d") && material_open)
    {
      current_mtl->trans = atof( strtok(NULL, " \t"));
    }
    //reflection
    else if( strequal(current_token, "r") && material_open)
    {
      current_mtl->reflect = atof( strtok(NULL, " \t"));
    }
    //glossy
    else if( strequal(current_token, "sharpness") && material_open)
    {
      current_mtl->glossy = atof( strtok(NULL, " \t"));
    }
    //refract index
    else if( strequal(current_token, "Ni") && material_open)
    {
      current_mtl->refract_index = atof( strtok(NULL, " \t"));
    }
    // illumination type
    else if( strequal(current_token, "illum") && material_open)
    {
    }
    // texture map
    else if( strequal(current_token, "map_Kd") && material_open)
    {   /** (pk note: huh, why was this map_Ka initially?) */
      strncpy(current_mtl->texture_filename, strtok(NULL, " \t\n"), OBJ_FILENAME_LENGTH);
      cout << current_mtl->name << " texture requests map_Kd filename: "
           << current_mtl->texture_filename << endl;
    }
    else
    {
      fprintf(stderr, "Unknown command '%s' in material file %s at line %i:\n\t%s\n",
          current_token, filename, line_number, current_line);
      // just skip it, unsupported feature or comment in file ?
    }
  }

  fclose(mtl_file_stream);

  return listOfMaterials;
}


void  bindTexturedPolydataToRenderWindow( vtkRenderWindow* renderWindow,
                                          vtkRenderer* renderer,
                                          vtkOBJPolydataProcessor* reader )
{ // TODO: move this function to library, call from simulator client!
    if( NULL == (renderWindow) ) { cout << "renderWindow is null, you fail" << endl; return; }
    if( NULL == (renderer) ) { cout << "renderer is null, you fail" << endl; return; }
    if( NULL == (reader) ) { cout << "vtkOBJPolydataProcessor is null, you fail" << endl; return; }

    reader->actor_list.clear();
    reader->actor_list.reserve( reader->GetNumberOfOutputPorts() );

    for( int port_idx=0; port_idx < reader->GetNumberOfOutputPorts(); port_idx++)
    {
        vtkPolyData* objPoly = reader->GetOutput(port_idx);
        cout << "grabbed objPoly " << objPoly << ", port index " << port_idx << endl;
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(objPoly);

        { /** debuggish */
            int numPolys  = objPoly->GetNumberOfPolys();
            int numPoints = objPoly->GetNumberOfPoints();
            printf("numPolys = %08d, numPoints = %08d ...\n",numPolys,numPoints);
        }

        // For each named material, load and bind the texture, add it to the renderer
        SP(vtkTexture) vtk_texture = SP(vtkTexture)::New();

        std::string textureFilename = reader->GetTextureFilename(port_idx);

        std::string file_extension;//  = boost::filesystem::extension( textureFilename );
        cout << "attempting load texture named " << textureFilename
             << "    whose extensions seems to be " << file_extension << endl;

        SP(vtkJPEGReader) tex_jpg_Loader = SP(vtkJPEGReader)::New();
        SP(vtkPNGReader)  tex_png_Loader = SP(vtkPNGReader)::New();
        bool bIsReadableJPEG = tex_jpg_Loader->CanReadFile( textureFilename.c_str() );
        bool bIsReadablePNG  = tex_png_Loader->CanReadFile( textureFilename.c_str() );

        // TODO: crap, what if there is no texture image? seems required now
        if( bIsReadableJPEG ) {
            tex_jpg_Loader->SetFileName( textureFilename.c_str() );
            tex_jpg_Loader->Update();
            vtk_texture->AddInputConnection( tex_jpg_Loader->GetOutputPort() );
        } else if( bIsReadablePNG ) {
            tex_png_Loader->SetFileName( textureFilename.c_str() );
            tex_png_Loader->Update();
            vtk_texture->AddInputConnection( tex_png_Loader->GetOutputPort() );
        } else {
            cout << "Bad, unhandled or nonexistant texture image type! "
                 << "Offender:   " << textureFilename << endl; exit(1);
        }
        // vtk_texture->InterpolateOn();     // Better?? (cant see obvious benefit)
        vtk_texture->InterpolateOff(); // Faster?? (yes clearly faster for largish texture)
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->SetTexture(vtk_texture);
        SP(vtkProperty) properties = SP(vtkProperty)::New();

        obj_material* raw_mtl_data = reader->GetMaterial(port_idx);
        properties->SetDiffuseColor(raw_mtl_data->diff);
        properties->SetSpecularColor(raw_mtl_data->spec);
        properties->SetAmbientColor(raw_mtl_data->amb);
        properties->SetOpacity(raw_mtl_data->trans);
        properties->SetInterpolationToPhong();
        properties->SetLighting(true);
        properties->SetSpecular( raw_mtl_data->get_spec_coeff() );
        properties->SetAmbient( raw_mtl_data->get_amb_coeff() );
        properties->SetDiffuse( raw_mtl_data->get_diff_coeff() );
        cout << ".. done set up material definition, properties->Print says: " << endl;
        properties->Print(std::cout);
        actor->SetProperty(properties);
        renderer->AddActor(actor);
        //properties->ShadingOn(); // use ShadingOn() if loading vtkMaterial from xml
        // available in mtl parser are:
        //    double amb[3];
        //    double diff[3];
        //    double spec[3];
        //    double reflect;
        //    double refract;
        //    double trans;
        //    double shiny;
        //    double glossy;
        //    double refract_index;

        reader->actor_list.push_back(actor); // keep a handle on actors to animate later
    }
    /** post-condition of this function: the renderer has had a bunch of actors added to it */
}
