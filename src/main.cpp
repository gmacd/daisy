#include "entry/entry.h"
#include "runner.h"

using namespace s7;

int _main_(int /*_argc*/, char** /*_argv*/)
{
	Runner runner;
	runner.Init();
	runner.Run();
	runner.Shutdown();

	return 0;
}
