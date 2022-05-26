#pragma once 

namespace Anggur {
	class Scene {
	public:
		virtual void initialize();
		virtual void update();
		virtual void terminate();
	};
}
