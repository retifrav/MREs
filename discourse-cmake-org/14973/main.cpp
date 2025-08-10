#include <QCoreApplication>
#include <QDebug>

#include "R3DSDK.h"

using namespace R3DSDK;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Set up code that uses the Qt event loop here.
    // Call a.quit() or a.exit() to quit the application.
    // A not very useful example would be including
    // #include <QTimer>
    // near the top of the file and calling
    // QTimer::singleShot(5000, &a, &QCoreApplication::quit);
    // which quits the application after 5 seconds.

    // If you do not need a running Qt event loop, remove the call
    // to a.exec() or	// Initialize the R3DSDK prior to using any R3DSDK objects.
    InitializeStatus status = InitializeSdk(".", OPTION_RED_NONE);

//    if ( status != ISInitializeOK)
//    {
//        printf("Failed to initialize SDK: %d\n", status);
//        return 1;
//    }

//    printf("SDK VERSION: %s\n",  GetSdkVersion());

    // Finalize the R3DSDK after destroying all R3DSDK objects
//    FinalizeSdk();

    qDebug() << "Qt - Link to R3DSDK.";
    return a.exec();
}
