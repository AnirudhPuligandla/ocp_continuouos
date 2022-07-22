#include "continuousOCP.h"

void ContinuousOCP::testOCP()
{
    // parameters for ellipsoid
    double ell_a = 2.0, ell_b=2.0, ell_c=1.0;
    // Set of control points
    std::vector<std::vector<double>> controlPoints;
    // control points on left side
    for(float i = -4; i <= 4; i=i+0.5)
    {
        for(float j = 3; j <= 4; j++)
        {
            controlPoints.push_back(std::vector<double>({i,j,0,1}));
        }
    }
    // control points on front side
    for(float i = 3; i <= 4; i=i+0.5)
    {
        for(float j = -4; j <= 2; j++)
            controlPoints.push_back(std::vector<double>({i,j,0,1}));
    }
    pyFolder = "/media/anirudh/Data/Documents/PhD/optimal camera placement/python/fst-pso-master/fstpso/";
    // write the parameters to a file
    QString contFName = pyFolder + "cont_points.dat";
    QFile contFile(contFName);
    contFile.open(QIODevice::WriteOnly);
    QDataStream contOut(&contFile);
    for(int i = 0; i < controlPoints.size(); i++)
    {
        for(int j = 0; j < controlPoints[i].size(); j++)
            contOut << (double)controlPoints[i][j];
    }
    contOut.~QDataStream();
    contFile.close();
    // Ellipsoid parameters
    float a=2.0f, b=2.0f, c=1.0f, e_cent_x=0.0f, e_cent_y=0.0f, e_cent_z=0.0f;
    int nCam = 2;
    // Call the python program for optimization
    // fst-pso takes seven arguments + 1 for name of the program
    // {a,b,c} ellipsoid parameters, number of cameras, center of ellipsoid
    QStringList arguments{pyFolder+"ocp.py",
                QString::number(a), QString::number(b), QString::number(c),
                QString::number(nCam),
                QString::number(e_cent_x), QString::number(e_cent_y), QString::number(e_cent_z)};
    QProcess *p = new QProcess();
    p->setProcessChannelMode(QProcess::ForwardedChannels);
    p->start("python", arguments);
    if(p->waitForFinished())
        qDebug() << "optimization with fst-pso complete!";
    // read output from the file
    std::vector<double> optResult;
    QString resFileName = pyFolder + "out_result.dat";
    QFile resFile(resFileName);
    resFile.open(QIODevice::ReadOnly);
    QDataStream resIn(&resFile);
    double resVal;
    while (!resFile.atEnd()) {
        resIn >> resVal;
        optResult.push_back(resVal);
    }

    // Create a visual model and write to file
    SimModels simModel;
    simModel.createAndSaveEnv(std::vector<double>({a,b,c}), nCam, controlPoints, optResult);
    qDebug() << "Debug holder!";
}
