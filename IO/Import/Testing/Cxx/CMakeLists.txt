file(DOWNLOAD https://www.dropbox.com/s/510ttbkyk5453uj/MiG-35.obj
              ${CMAKE_BINARY_DIR}/bin/MiG-35.obj      TIMEOUT  20)
file(DOWNLOAD https://www.dropbox.com/s/wonunhzotw9sghr/MiG-35.obj.mtl
              ${CMAKE_BINARY_DIR}/bin/MiG-35.obj.mtl  TIMEOUT  20)

#TODO: verify hash sums: file(MD5  file  mdf5var)

vtk_add_test_cxx(${vtk-module}CxxTests tests
  TestOBJImporter.cxx,NO_VALID,NO_DATA,NO_OUTPUT
  TestVRMLNormals.cxx
  TestVRMLImporter.cxx,NO_VALID
  )

vtk_test_cxx_executable(${vtk-module}CxxTests tests)

# TODO: this also should succeed.
#add_test(vtkIOImportCxxTests TestOBJImporter MiG-35.obj MiG-35.obj.mtl)
#
