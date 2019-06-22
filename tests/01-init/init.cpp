#include "io/Window.hpp"
#include "gfx/gfx.hpp"

int main(int argc, char* argv[])
{
	ari::gfx::gfxSetup setup;
	SetupGfx(setup);
	while(ari::io::Run())
	{
		ari::gfx::Present();
	}
	return 0;
}
