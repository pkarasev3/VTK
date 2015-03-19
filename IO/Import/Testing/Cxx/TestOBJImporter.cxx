
#include "vtkVRMLImporter.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkOBJImporter.h"
#include "vtkTestUtilities.h"
#include "vtkNew.h"

int TestOBJImporter( int argc, char * argv [] )
{

    std::string filenameOBJ;
    std::string filenameMTL;
    vtkNew<vtkOBJImporter> importer;
    if(argc==0)
    {
        filenameOBJ = "MiG-35.obj";
        filenameMTL = "MiG-35.obj.mtl";
        std::cerr<<"got zero args; using default test files: "
                <<filenameOBJ<<","<<filenameMTL;
    }
    else
    {
        if( argc != 3 )
        {
            std::cerr << "invalid args; expected: "<< argv[0]
                      << " File.obj "<<" File.obj.mtl " << std::endl;
            return 1;
        }
        else
        {
            // Ok, set the filenames to load geometry + material info.
            filenameOBJ = std::string(argv[1]);
            filenameMTL = std::string(argv[2]);
        }
    }
    importer->SetFileName(filenameOBJ.data());
    importer->SetFileNameMTL(filenameMTL.data());

    importer->Update();

    //iren->Start();

    return 0;
}
