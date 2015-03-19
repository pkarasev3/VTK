
vtk_add_test_cxx(${vtk-module}CxxTests tests
  TestOBJImporter.cxx,NO_VALID,NO_DATA,NO_OUTPUT
  TestVRMLNormals.cxx
  TestVRMLImporter.cxx,NO_VALID
  )
vtk_test_cxx_executable(${vtk-module}Cxx
