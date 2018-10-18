#include <iostream>
#include <string>	//string
#include <cstring>	//strcmp

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

/* Test de la classe statistiques */
void testStatistics()
{
	// Setters
	cout << "\n\nTest de la classe Statistics" << endl;
	Statistics stats;
	int vie, attaque, deplacement;
	cout << "Entrer nombre de point de vie (valeur entre 0 et 2000)" << endl;
	cin >> vie;
	cout << "Entrer nombre de point d'attaque (valeur entre 0 et 2000)" << endl;
	cin >> attaque;
	cout << "Entrer nombre de point de deplacement (valeur entre 0 et 2000)" << endl;
	cin >> deplacement;
	stats.Set_statistiques(vie, attaque, deplacement);
	cout << "\n" << "vie = " << stats.Get_life_points() << "\n" << "attaque = " << stats.Get_attack_points() << "\n" << "deplacement = " << stats.Get_move_points();
	cout << endl << "Fin du test de Statistics" << endl << endl;
}

/* Test de la classe Characters */
void testCharacters(){

	cout << "\n\nTest de la classe Characters (5 attaques maximum)" << endl;
	Characters perso;
	
	cout << "\n\nVerification des ajouts d'attaques possibles\n" << endl;

	void * random;
	for (int i = 0; i< 10; i++)
	{		
		cout << "\tAjout d'une attaque a la liste du personnage" << endl;
		perso.addAttack(10,10,random);
		cout << "\tNombre d'attaque enregistre = " << perso.get_number_of_attacks() << endl << endl;
	}

	cout << endl << "Fin du test de Characters" << endl << endl;
}

/* Test de la classe Player */
void testPlayer(){

	cout << "\n\nTest de la classe Player" << endl;
	Player joueur;
	Characters perso;
	
	cout << "\n\nVerification du nombre de personnages par joueur (5 max)\n" << endl;
	for (int i = 0; i< 7; i++)
	{	
		cout << "\tAjout d'un personnage a la liste du du joueur" << endl;
		joueur.add_character(perso);
		cout << "\tNombre d'attaque enregistre = " << joueur.get_number_of_characters() << endl << endl;
		
	}
	cout << "\nFin du test de Player" << endl << endl;
}

/* Test de la classe GameState */
void testGameState(){
	
	cout << "\n\nTest de la classe GameState" << endl;
	GameState etat;

	cout << "\n\nCreation d'une carte de jeu 50x50\n" << endl;
	etat.new_map();	

	cout << "\n\nVerification du nombre de joueurs max (5)\n" << endl;
	for (int i = 0; i< 7; i++)
	{
		cout << "Ajout d'un joueur au jeu" << endl;
		etat.new_player("player");
		cout << "Nombre de joueur enregistre = " << etat.get_number_of_player() << endl;
	}
	

}

int main(int argc,char* argv[]) 
{
	cout << "Bienvenue chez antoine et gregoire !" << endl;
	if(argc == 2)
	{
		if(strcmp(argv[1], "state") == 0 )
		{

			testStatistics();
			testCharacters();
			testGameState();
			//testPlayer();

		}
	}
	return 0;
}



