#include "Anggur/System/Window.h"

namespace Anggur
{
	class Application 
	{
	public:
		static void Initialize(const Vector2& windowSize);

		static void Terminate();

		static bool IsOpen();

		static void BeginFrame();

		static void EndFrame();
	
		static void Run();
	};
}