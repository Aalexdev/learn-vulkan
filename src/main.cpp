#include "App.hpp"

// std
#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <unordered_map>
#include <array>
#include <cmath>
#include <chrono>

int main(int argc, char const *argv[]){
	App app{};

	try{
		app.run();
	} catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
