#include "Onyx.h"
#include "Onyx/Core/EntryPoint.h"

#include "ApplicationLayer.h"

namespace Onyx
{

	class OnyxApp : public Application
	{
	public:
		OnyxApp(ApplicationCommandLineArgs args) : Application("Onyx App", args)
		{
			
		}

		~OnyxApp() 
		{}

	private:

	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		OnyxApp* app = new OnyxApp(args);
		app->PushLayer(new ApplicationLayer());
		return app;
	}
}

