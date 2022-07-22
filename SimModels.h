#ifndef SIMMODELS_H
#define SIMMODELS_H

/* Creating VTK environments or loading polygon data will be done in this class
 */
#include <vtkSmartPointer.h>
#include <vtkParametricEllipsoid.h>
#include <vtkParametricFunctionSource.h>
#include <vtkPLYWriter.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkAppendPolyData.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkGlyph3D.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkArrowSource.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkLine.h>
#include <vtkNamedColors.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPoints.h>
#include <vtkTubeFilter.h>

#include <vector>
#include <string>
#include <math.h>

#if VTK_VERSION_NUMBER >= 89000000000ULL
#define VTK890 1
#endif

class SimModels
{
public:
    // Function to create a simple environment with ellipsoid an few points using vtk
    void createAndSaveEnv(std::vector<double> ellipseParams,
                          int nCam,
                          std::vector<std::vector<double> > &controlP,
                          std::vector<double> optSolution);
private:
    std::string folderPath = "/media/anirudh/Data/Documents/PhD/Qt_projects/ocp_continuous/resources/";
    // Function to normalize vectors
    template<typename T> std::vector<T> normVec(std::vector<T> inVec);
    // function to return cross product of two vectors
    template<typename T> std::vector<T> crossVec(std::vector<T> inVec1, std::vector<T> inVec2);
    // struct for intrinsic parameters
    struct intrinsics{
        // for now define it for a pinhole camera
        float f_x = 1.0f, f_y = 1.0f, c_x = 0.0f, c_y = 0.0f;
        int depth = 2;
        // image dimensions
        int h = 2, v = 2;
        // formula for calculating
    };
};

#endif // SIMMODELS_H
