#include "MainWindow.hpp"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    try
    {
        return app.exec();
    }
    catch (std::exception &e)
    {
        QMessageBox::critical(nullptr, "An error occurred !", e.what());

        return 1;
    }
}
