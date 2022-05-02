#include "Engine.h"

int main(int argc, char* argv[])
{
    Enix::Engine engine{};
    const int status = engine.run();
    return status;
}
