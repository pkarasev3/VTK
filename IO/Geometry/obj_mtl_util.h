#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>
#include <string>
#include <math.h>

#define OBJ_FILENAME_LENGTH 500
#define MATERIAL_NAME_SIZE 255

using std::vector;
using boost::shared_ptr;

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

//std::string   getImageFilename( const std::string& key );

void obj_set_material_defaults(shared_ptr<obj_material> mtl);

vector<shared_ptr<obj_material> > obj_parse_mtl_file(
                 const std::string& filename,int& result_code);
