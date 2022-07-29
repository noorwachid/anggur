#include "App.h"
#include <iostream>
#include <stdexcept>

int main() 
{
	try 
	{
		App app;
		app.Run();
	} 
	catch (const std::exception& exception) 
	{
		std::cout << exception.what() << "\n";
	}
}