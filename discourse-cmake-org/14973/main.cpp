#include <QCoreApplication>
#include <QDebug>

#include "R3DSDK.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Initialize the R3DSDK prior to using any R3DSDK objects
    auto status = R3DSDK::InitializeSdk(".", OPTION_RED_NONE);

    if ( status != R3DSDK::ISInitializeOK)
    {
        printf("Failed to initialize SDK: %d\n", status);
        return 1;
    }

    printf("SDK VERSION: %s\n", R3DSDK::GetSdkVersion());

    // Finalize the R3DSDK after destroying all R3DSDK objects
    R3DSDK::FinalizeSdk();

    qDebug() << "Qt - Link to R3DSDK.";
    return a.exec();
}
