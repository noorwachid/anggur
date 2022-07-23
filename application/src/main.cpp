#include "CoreSystem.h"
#include "TestEntity.h"

int main() {
	CoreSystem coreSystem;
	coreSystem.setWindow(std::make_shared<Window>(Vector2(600, 600), "An Empty Window"));
	coreSystem.setRootEntity(std::make_shared<TestEntity>());
	coreSystem.run();
}