#include <pch.hpp>

#include <Modules/Core/AppRunner.hpp>

int main(int argc, char** argv)
{
    AppInitializer::initialize();

    AppRunner app;
    app.run();

    return EXIT_SUCCESS;
}