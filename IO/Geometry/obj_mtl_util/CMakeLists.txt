set(MTL_PARSER_SRC  obj_mtl_util.cxx obj_mtl_util.h)
add_library( obj_mtl_util   ${MTL_PARSER_SRC} )
#target_link_libraries( mtl_parser ${Boost_LIBRARIES} )
