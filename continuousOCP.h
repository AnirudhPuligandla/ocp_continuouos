#ifndef CONTINUOUSOCP_H
#define CONTINUOUSOCP_H

#include <vector>

#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QProcess>
#include <QStringList>

#include "SimModels.h"

class ContinuousOCP
{
public:
    // A test function to run python script for fst_pso and visualize the optimization results
    void testOCP();
private:
    // python folder
    QString pyFolder;
};

#endif // CONTINUOUSOCP_H
