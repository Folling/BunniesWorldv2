#include <iostream>
#include <conio.h>
#include "Colony.h"
#include <stdlib.h>

int main() {
	
	
//	Colony main;
//	for(int i = 0; i < 25; i++) {
//		main.iterate();
//	}
//	main.saveColony("BunnySave_1.txt");
//	
	
	Colony main = Colony(emptyColony);

	main.loadColony("BunnySave_1.txt");
	main.printList();
	main.iterations.at(main.itCount).printIt();

	_getch();
}
