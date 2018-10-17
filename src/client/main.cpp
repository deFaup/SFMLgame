#include <iostream>
#include <string>

#include "define.hpp"

// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>

void testSFML() {
    sf::Texture texture;
}

// Fin test SFML

#include "state.h"

using namespace std;
using namespace state;

int main(int argc,char* argv[]) 
{

	cout << "Bienvenue chez antoine et gregoire !" << endl;
	if(argc == 2 /*&& argv[1] == "test"*/){
		cout << "Test de la classe Statistiques" << endl;
		Statistics stats;
		unsigned int vie,attaque,deplacement;
		cout << "Entrer nombre de point de vie (valeur entre 0 et 2000)" << endl;
		cin >> vie;
		cout << "Entrer nombre de point d'attaque (valeur entre 0 et 2000)" << endl;
		cin >> attaque;
		cout << "Entrer nombre de point de deplacement (valeur entre 0 et 2000)" << endl;
		cin >> deplacement;
		stats.Set_statistiques(vie,attaque,deplacement);
		cout << "\n" << "vie = "<< stats.Get_life_points() << "\n" << "attaque = " << stats.Get_attack_points() << "\n" << "deplacement = " << stats.Get_move_points() << endl;
	}
	return 0;
}
