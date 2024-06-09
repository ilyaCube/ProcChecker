#include "AppLauncher.hpp"

int main(int argc, char** argv)
{
    App::AppLauncher launcher(argc, argv);
    return launcher.start();
}