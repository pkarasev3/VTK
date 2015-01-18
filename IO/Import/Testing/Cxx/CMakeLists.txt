vtk_add_test_cxx(${vtk-module}CxxTests tests
  TestVRMLNormals.cxx
  TestVRMLImporter.cxx,NO_VALID
  )
vtk_test_cxx_executable(${vtk-module}CxxTests tests)

add_executable(TestOBJImporter  TestOBJImporter.cxx)
target_link_libraries( TestOBJImporter
 ${vtk-module}
 vtkRendering${VTK_RENDERING_BACKEND}
 vtkInteractionStyle )
