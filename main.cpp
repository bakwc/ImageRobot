#include <iostream>
#include "application.h"

void myMsgHandler(QtMsgType messageType, const char *msg) {
    if (messageType == QtDebugMsg) {
        std::cout << msg << "\n";
    }
}

int main(int argc, char *argv[])
{
    qInstallMsgHandler(myMsgHandler);
    TApplication app(argc, argv);
    app.Init();
    app.Start();
    return app.exec();
}
