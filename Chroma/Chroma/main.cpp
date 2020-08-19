#include <core/Application.h>

#ifndef EDITOR

int main()
{
	// Init Application
	Chroma::Application* ChromaApp = Chroma::CreateApplication();

	// Run 
	ChromaApp->Run();

	// Cleanup
	delete ChromaApp;

	return 0;
}
#endif
