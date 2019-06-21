#include "io/Window.hpp"

int main(int argc, char* argv[])
{
	auto winh = ari::io::CreateAriWindow(640, 480, "01-init");
	while(ari::io::Run())
	{
		
	}
	return 0;
}
