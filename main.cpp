#include "application.h"

int main(int argc, char *argv[])
{
    TApplication app(argc, argv);
    app.Init();
    app.Start();
    return app.exec();
}
