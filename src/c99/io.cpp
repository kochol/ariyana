
#include "io.h"
#include "io/FileSystem.hpp"
#include "io/Window.hpp"

void UpdateIo()
{
    ari::io::Update();
}

bool Run()
{
    return ari::io::Run();
}
