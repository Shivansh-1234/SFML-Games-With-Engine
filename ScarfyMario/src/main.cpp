#include "SFML/Graphics.hpp"

#include "GameEngine.h"

int main()
{
	srand(static_cast<unsigned>(time(NULL)));

	sa::GameEngine g("../resources/assets.txt");

	g.run();

	return EXIT_SUCCESS;
}