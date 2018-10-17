#include "define.hpp"
#include <iostream>
#include "Map.h"
#include <SFML/Window.hpp> //getDesktopMode

#include <ctime>	// time
#include <cstdlib>	// rand srand

using namespace std;
using namespace state;

void Map::set_screen_dimensions (){

	width = sf::VideoMode::getDesktopMode().width;
	height = sf::VideoMode::getDesktopMode().height;
	cout << width << ", " << height << endl;
}

void Map::get_screen_dimensions(int* width, int* height){

	*width = this->width;
	*height =this->height;
	cout << width << ", " << height << endl;
}

void Map::create_mask ()
{
	fill_mask_with_x(mask,1);
	affiche(mask);

	/* Intializes random number generator */
	srand(time(NULL));

	/* Starting with the first column and random row */
	int x(0), y(0);
	y = int((30 * height) / 100) + rand() % int(50 * height / 100);
	printf("y = %d\n", y);
	mask[x][y]++;
	puts_zero(mask, x, y);

	while (x != width - 1) {
		/* new x and new y*/
		x++;
		new_line_index(&y, height);
		/* update the matrix */
		mask[x][y]++;
		puts_zero(mask, x, y);

	}
}

void Map::fill_mask_with_x(auto& mask, const int x){
    
    for (int i = 0; i < height; i++)
    {
        //On ajoute une ligne de 4 cases
        mask.push_back(vector<int>(width,x));   
    } 	//*4
}

void Map::affiche(const auto& mask){
    
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            cout << "mask[" <<i<<"]"<<"["<< j<<"] = "<< mask[i][j] << endl;
        }
    }
}

void Map::puts_zero(auto& mask, const int x, const int y) {
	/* if mask(xxx xxx) = 2 then
	we put all values in the col xxx from the beginning to xxx to 0 (reduce 1)
	*/
	for (int i = 0; i < y; i++) {
		mask[x][i]--;
	}
}

int int_power(int number, int power) {
	int temp = 1;
	for (int i = 0; i < power; i++) {
		temp = temp*number;
	}
	return temp;
}

int* Map::new_line_index(int* y, const int& height) {
	/* new index based on the preivous one */
	/* be careful no to make x negative or superior to height -1 */
	int sign = 1;
	if (*y <= 0.3 * height) //30% of free space in the top for the sky
		sign = +1;
	else if (*y >= 0.8 * height) // 20% of bottom space for the ground
		sign = -1;
	else
		sign = int_power(-1, rand() % 2); // Either -1 or 1
	
	*y = *y + sign* (rand() %3);
	return y;
}

