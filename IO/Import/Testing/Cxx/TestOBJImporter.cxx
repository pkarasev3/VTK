#include "vtkOBJImporter.h"
#include "vtkNew.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

int main(int argc,char* argv[])
{
    vtkNew<vtkOBJImporter> importer;
    std::string filenameOBJ;
    std::string filenameMTL;
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
        importer->SetFileName(filenameOBJ.data());
        importer->SetFileNameMTL(filenameMTL.data());
    }

    // Now create the RenderWindow, Renderer and Interactor
    vtkNew<vtkRenderer> ren1;
    vtkNew<vtkRenderWindow> renWin;
    renWin->AddRenderer(ren1.GetPointer());
    vtkNew<vtkRenderWindowInteractor> iren;

    iren->SetRenderWindow(renWin.GetPointer());

    importer->SetRenderWindow(renWin.GetPointer());

    iren->Start();

    return 0;
}
