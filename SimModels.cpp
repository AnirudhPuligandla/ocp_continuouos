#include "SimModels.h"

void SimModels::createAndSaveEnv(std::vector<double> ellipseParams, int nCam,
                                 std::vector<std::vector<double>> &controlp,
                                 std::vector<double> optSolution)
{
    // Colors for different polyData
    auto colors = vtkSmartPointer<vtkNamedColors>::New();
    std::string ellipseColor = "OrangeRed";
    vtkColor3d pointColor = colors->GetColor3d("MediumSpringGreen");
    vtkColor3d camAxColor1 = colors->GetColor3d("lime");
    vtkColor3d camAxColor2 = colors->GetColor3d("orange");
    vtkColor3d camAxColor3 = colors->GetColor3d("Raspberry");
    //----------------------------- Ellipsoid -----------------------------------------------------------
    vtkSmartPointer<vtkParametricEllipsoid> ellipsoid = vtkSmartPointer<vtkParametricEllipsoid>::New();
    // set ellipsoid parameters
    ellipsoid->SetXRadius(ellipseParams[0]);
    ellipsoid->SetYRadius(ellipseParams[1]);
    ellipsoid->SetZRadius(ellipseParams[2]);
    ellipsoid->SetMaximumV(M_PI/2);
    // set ellipsoid center
    vtkSmartPointer<vtkParametricFunctionSource> ellipsoidSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
    ellipsoidSource->SetParametricFunction(ellipsoid);
    ellipsoidSource->SetUResolution(50);
    ellipsoidSource->SetVResolution(50);
    ellipsoidSource->SetWResolution(50);
    ellipsoidSource->Update();
    vtkSmartPointer<vtkPolyData> scene = vtkSmartPointer<vtkPolyData>::New();
    scene->ShallowCopy(ellipsoidSource->GetOutput());
    // ellipsoid color
//    int numCells = scene->GetNumberOfCells();
//    vtkSmartPointer<vtkUnsignedCharArray> colorEllipse = vtkSmartPointer<vtkUnsignedCharArray>::New();
//    colorEllipse->SetNumberOfComponents(3);
//    for(int i = 0; i < numCells; i++)
//        colorEllipse->InsertNextTuple(colors->GetColor3d(ellipseColor).GetData());
//    scene->GetCellData()->SetScalars(colorEllipse);
//    scene->GetCellData()->Update();
    // -----------------------------------Create another polydata with control points-----------------------------------
    auto pointSet = vtkSmartPointer<vtkPoints>::New();
    int numControl = controlp.size();
    pointSet->SetNumberOfPoints(numControl);
    for(vtkIdType i = 0; i < numControl; i++)
    {
        double point[3];
        point[0] = controlp[i][0];
        point[1] = controlp[i][1];
        point[2] = controlp[i][2];
        pointSet->SetPoint(i,point);
    }
    auto sceneControl = vtkSmartPointer<vtkPolyData>::New();
    sceneControl->SetPoints(pointSet);
    // pass through glyph3d filter to render points as small spheres and save into new polydata
    auto glyphFilter = vtkSmartPointer<vtkGlyph3D>::New();
    auto sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(0.08);
    //sphereSource->SetThetaResolution(21);
    //sphereSource->SetPhiResolution(21);
    glyphFilter->SetSourceConnection(sphereSource->GetOutputPort());
    glyphFilter->SetInputData(sceneControl);
    glyphFilter->Update();
    auto scenePoints = vtkSmartPointer<vtkPolyData>::New();
    scenePoints->ShallowCopy(glyphFilter->GetOutput());
    // --------------------------------divide solution values as per cameras-------------------
    int valsPerCam = optSolution.size()/nCam;
    std::vector<std::vector<double>> optSolVecs(nCam);
    for(int i = 0; i < nCam; i++)
    {
        int counter = 0;
        while(counter < valsPerCam)
        {
            optSolVecs[i].push_back(optSolution[i*valsPerCam + counter]);
            counter++;
        }
    }
    // -------------------------------------Display camera axes----------------------------------------------------------
    std::vector<std::vector<vtkSmartPointer<vtkPolyData>>> arrowDatVec(nCam);
    std::vector<vtkSmartPointer<vtkPolyData>> linesDatVec(nCam);
    for(int camCount = 0; camCount < nCam; camCount++)
    {
        std::vector<double> optSol = optSolVecs[camCount];
        double x = ellipseParams[0] * cos(optSol[0]) * sin(optSol[1]);
        double y = ellipseParams[1] * sin(optSol[0]) * sin(optSol[1]);
        double z = ellipseParams[2] * cos(optSol[1]);
        std::vector<double> camPos({x,y,z});
        std::vector<std::vector<double>> camDirs(3);
        // camera direction vector from the optimized solution
        std::vector<double> camDir = normVec(std::vector<double>({optSol[2],optSol[3],optSol[4]}));
        camDirs[2] = camDir;
        // camera right vector as a cross product of direction and world up vectors
        std::vector<double> worldUp({0.0,0.0,1.0});
        std::vector<double> camRight = normVec(crossVec(camDir,worldUp));
        camDirs[0] = camRight;
        std::vector<double> camUp = normVec(crossVec(camRight,camDir));
        camDirs[1] = camUp;
        std::vector<vtkSmartPointer<vtkPolyData>> arrowDat(3);
        for(int i = 0 ; i < 3; i++)
        {
            // Create an arrow.
            vtkNew<vtkArrowSource> arrowSource;
            arrowSource->SetShaftRadius(.01);
            arrowSource->SetShaftResolution(20);
            arrowSource->SetTipLength(.1);
            arrowSource->SetTipRadius(.05);
            arrowSource->SetTipResolution(20);
            //vtkMath::Normalize(camDirs[i].data());
            auto rng = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
            rng->SetSeed(8775070);
            std::vector<double> arbitrary(3);
            for(int j = 0; j < 3; j++)
            {
                rng->Next();
                arbitrary[i] = rng->GetRangeValue(-10, 10);
            }
            std::vector<double> zTemp(3), yTemp(3);
            vtkMath::Cross(camDirs[i].data(), arbitrary.data(), zTemp.data());
            vtkMath::Normalize(zTemp.data());
            vtkMath::Cross(zTemp.data(), camDirs[i].data(), yTemp.data());
            vtkNew<vtkMatrix4x4> matrix;
            // Create the direction cosine matrix
            matrix->Identity();
            for (auto j = 0; j < 3; j++)
            {
                matrix->SetElement(j, 0, camDirs[i][j]);
                matrix->SetElement(j, 1, yTemp[j]);
                matrix->SetElement(j, 2, zTemp[j]);
            }
            // Apply the transforms
            auto transform = vtkSmartPointer<vtkTransform>::New();
            transform->Translate(camPos.data());
            transform->Concatenate(matrix);
            double length = vtkMath::Norm(camDirs[i].data());
            transform->Scale(length, length, length);
            // Transform the polydata
            auto transformPD = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
            transformPD->SetTransform(transform);
            transformPD->SetInputConnection(arrowSource->GetOutputPort());
            transformPD->Update();
            arrowDat[i] = transformPD->GetOutput();
        }
        arrowDatVec[camCount] = arrowDat;
        // --------------------Draw camera FoV----------------------------------
        intrinsics intParams;
        double hFov = 2 * atan(intParams.h/(2*intParams.f_x));
        double vFov = 2 * atan(intParams.v/(2*intParams.f_y));
        double halfH = intParams.depth * tan(hFov/2);
        double halfV = intParams.depth * tan(vFov/2);
        double fovCent[3] = {x+(intParams.depth * camDir[0]), y+(intParams.depth * camDir[1]), z+(intParams.depth * camDir[2])};
        double fovTL[3] = {fovCent[0]+(camUp[0]*halfV)+(-camRight[0]*halfH), fovCent[1]+(camUp[1]*halfV)+(-camRight[1]*halfH), fovCent[2]+(camUp[2]*halfV)+(-camRight[2]*halfH)};
        double fovBL[3] = {fovTL[0]+(-camUp[0]*2*halfV), fovTL[1]+(-camUp[1]*2*halfV), fovCent[2]+(-camUp[2]*2*halfV)};
        double fovBR[3] = {fovBL[0]+(camRight[0]*2*halfH), fovCent[1]+(camRight[1]*2*halfH), fovCent[2]+(camRight[2]*2*halfH)};
        double fovTR[3] = {fovBR[0]+(camUp[0]*2*halfV), fovBR[1]+(camUp[1]*2*halfV), fovBR[2]+(camUp[2]*2*halfV)};
        // Insert the 5 points defining fov
        vtkSmartPointer<vtkPoints> fovPoints = vtkSmartPointer<vtkPoints>::New();
        fovPoints->InsertNextPoint(x,y,z);
        fovPoints->InsertNextPoint(fovTL);
        fovPoints->InsertNextPoint(fovBL);
        fovPoints->InsertNextPoint(fovBR);
        fovPoints->InsertNextPoint(fovTR);
        vtkSmartPointer<vtkPolyData> linesDat = vtkSmartPointer<vtkPolyData>::New();
        linesDat->SetPoints(fovPoints);
        std::vector<vtkSmartPointer<vtkLine>> lineVec(8);
        for(int i = 0; i < 8; i++)
            lineVec[i] = vtkSmartPointer<vtkLine>::New();
        // Draw 8 lines defining camera fov
        lineVec[0]->GetPointIds()->SetId(0,0);      // origin to TL
        lineVec[0]->GetPointIds()->SetId(1,1);
        lineVec[1]->GetPointIds()->SetId(0,0);      // origin to BL
        lineVec[1]->GetPointIds()->SetId(1,2);
        lineVec[2]->GetPointIds()->SetId(0,0);      // origin to BR
        lineVec[2]->GetPointIds()->SetId(1,3);
        lineVec[3]->GetPointIds()->SetId(0,0);      // origin to TR
        lineVec[3]->GetPointIds()->SetId(1,4);
        lineVec[4]->GetPointIds()->SetId(0,1);      // TL to BL
        lineVec[4]->GetPointIds()->SetId(1,2);
        lineVec[5]->GetPointIds()->SetId(0,2);      // BL to BR
        lineVec[5]->GetPointIds()->SetId(1,3);
        lineVec[6]->GetPointIds()->SetId(0,3);      // BR to TR
        lineVec[6]->GetPointIds()->SetId(1,4);
        lineVec[7]->GetPointIds()->SetId(0,4);      // TR to TL
        lineVec[7]->GetPointIds()->SetId(1,1);
        // create cell array and set it to polydata
        auto lineCell = vtkSmartPointer<vtkCellArray>::New();
        for(int i = 0; i < 8; i++)
            lineCell->InsertNextCell(lineVec[i]);
        linesDat->SetLines(lineCell);
        // Draw lines as tubes
        vtkSmartPointer<vtkTubeFilter> tubeFilt = vtkSmartPointer<vtkTubeFilter>::New();
        tubeFilt->SetInputData(linesDat);
        tubeFilt->Update();
        // add poly data into array
        linesDatVec[camCount] = tubeFilt->GetOutput();
    }
    //-------------------------------- Combine all polyData-----------------------------------------------------------------
    vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();
    appendFilter->AddInputData(scene);
    appendFilter->AddInputData(scenePoints);
    for(int i = 0; i < nCam; i++)
    {
        appendFilter->AddInputData(linesDatVec[i]);
        for(int j = 0; j < 3; j++)
            appendFilter->AddInputData(arrowDatVec[i][j]);
    }
    //appendFilter->Update();
    auto cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
    cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
    cleanFilter->Update();
    // --------------------------------write to file-------------------------------------------------------------------------
    vtkSmartPointer<vtkPLYWriter> outWrite = vtkSmartPointer<vtkPLYWriter>::New();
    std::string fileName = folderPath+"ellipse_test.ply";
    outWrite->SetFileName(fileName.c_str());
    //outWrite->SetInputData(scenePoints);
    outWrite->SetInputConnection(cleanFilter->GetOutputPort());
    outWrite->Write();
}

template<typename T> std::vector<T> SimModels::normVec(std::vector<T> inVec)
{
    std::vector<T> outVec(inVec.size());
    // compute magnitude of the vector
    double mag = sqrt(pow(inVec[0],2)+pow(inVec[1],2)+pow(inVec[2],2));
    outVec[0] = inVec[0]/mag;
    outVec[1] = inVec[1]/mag;
    outVec[2] = inVec[2]/mag;
    return outVec;
}

template<typename T> std::vector<T> SimModels::crossVec(std::vector<T> inVec1, std::vector<T> inVec2)
{
    std::vector<double> outVec(inVec1.size());
    outVec[0] = inVec1[1]*inVec2[2]-inVec1[2]*inVec2[1];
    outVec[1] = inVec1[2]*inVec2[0]-inVec1[0]*inVec2[2];
    outVec[2] = inVec1[0]*inVec2[1]-inVec1[1]*inVec2[0];
    return outVec;
}
