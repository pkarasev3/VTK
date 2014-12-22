#include "mtl_parser.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <map>

using namespace std;
#define OBJ_LINE_SIZE 500   // TODO: get rid of this


namespace Retarded {

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

//static map<string,string>   mtlName_to_imgFile;

}

using namespace Retarded;

//string   getImageFilename( boost::shared_ptr ) {
//  if( 0 == mtlName_to_imgFile.count(key) ) {
//    cout << "Warning, requested string key " << key << " was not found." << endl;
//    return string("");
//  }  else {
//    return mtlName_to_imgFile[key];
//  }
//}

void obj_set_material_defaults(boost::shared_ptr<obj_material> mtl)
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

vector<boost::shared_ptr<obj_material> > obj_parse_mtl_file(const std::string& Filename,
                                                                           int& result_code)
{
  // Maybe this should return a map instead, e.g. mapping string keys to ptrToMaterialStruct values
  // problem is that we don't know the material name at creation-time of this thing ... d'oh

  vector<boost::shared_ptr<obj_material> >  listOfMaterials;
  result_code    = 0;
  const char* filename = Filename.c_str();


  int line_number = 0;
  char *current_token;
  char current_line[OBJ_LINE_SIZE];
  char material_open = 0;
  boost::shared_ptr<obj_material> current_mtl; //= NULL;
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
      current_mtl = boost::shared_ptr<obj_material> (new obj_material);
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
