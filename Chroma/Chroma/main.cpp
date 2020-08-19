#include <core/Application.h>

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