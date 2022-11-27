#include "TestingCameraApp.h"
#include <iostream>
#include <stdexcept>

int main() {
	try {
		TestingCameraApp app;
		app.run();
	} catch (const std::exception& exception) {
		std::cout << exception.what() << "\n";
	}
}