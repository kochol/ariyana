#include "io/Window.hpp"

int main(int argc, char* argv[])
{
	ari::io::Window window;
	auto winh = ari::io::CreateAriWindow(window, "01-init");
	while(ari::io::Run())
	{
		
	}
	return 0;
}
