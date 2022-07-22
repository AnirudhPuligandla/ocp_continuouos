#include <QCoreApplication>
#include <QTimer>

#include "continuousOCP.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // test function
    ContinuousOCP ocpObj;
    ocpObj.testOCP();

    QTimer::singleShot(0,[](){
             //do stuff
             QCoreApplication::exit(0);
        });
    return a.exec();
}
