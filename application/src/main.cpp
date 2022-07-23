#include "CoreSystem.h"
#include "TestEntity.h"

int main() {
	CoreSystem coreSystem;
	coreSystem.setRootEntity<TestEntity>();
	coreSystem.run();
}