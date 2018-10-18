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

		// test de la classe statistiques

		cout << "Test de la classe Statistiques" << endl;
		Statistics stats;
		int vie,attaque,deplacement;
		cout << "Entrer nombre de point de vie (valeur entre 0 et 2000)" << endl;
		cin >> vie;
		cout << "Entrer nombre de point d'attaque (valeur entre 0 et 2000)" << endl;
		cin >> attaque;
		cout << "Entrer nombre de point de deplacement (valeur entre 0 et 2000)" << endl;
		cin >> deplacement;
		stats.Set_statistiques(vie,attaque,deplacement);
		cout << "\n" << "vie = "<< stats.Get_life_points() << "\n" << "attaque = " << stats.Get_attack_points() << "\n" << "deplacement = " << stats.Get_move_points() << endl;

		// test de la classe Characters

		cout << "\nTest de la classe personnage (5 attaque maximum)" << endl;
		Characters perso;
		cout << "Ajout d'une attaque à la liste du personnage" << endl;
		void * random;
		perso.addAttack(10,10,random);
		cout << "Nombre d'attaque enregistré = " << perso.get_number_of_attacks() << endl;
		cout << "Ajout d'une attaque à la liste du personnage" << endl;
		perso.addAttack(10,10,random);
		cout << "Nombre d'attaque enregistré = " << perso.get_number_of_attacks() << endl;
		cout << "Ajout d'une attaque à la liste du personnage" << endl;
		perso.addAttack(10,10,random);
		cout << "Nombre d'attaque enregistré = " << perso.get_number_of_attacks() << endl;
		cout << "Ajout d'une attaque à la liste du personnage" << endl;
		perso.addAttack(10,10,random);
		cout << "Nombre d'attaque enregistré = " << perso.get_number_of_attacks() << endl;
		cout << "Ajout d'une attaque à la liste du personnage" << endl;
		perso.addAttack(10,10,random);
		cout << "Nombre d'attaque enregistré = " << perso.get_number_of_attacks() << endl;
		cout << "Ajout d'une attaque à la liste du personnage" << endl;
		perso.addAttack(10,10,random);
		cout << "Nombre d'attaque enregistré = " << perso.get_number_of_attacks() << endl;
		cout << "Ajout d'une attaque à la liste du personnage" << endl;
		perso.addAttack(10,10,random);
		cout << "Nombre d'attaque enregistré = " << perso.get_number_of_attacks() << endl;
		cout << "Ajout d'une attaque à la liste du personnage" << endl;
		perso.addAttack(10,10,random);
		cout << "Nombre d'attaque enregistré = " << perso.get_number_of_attacks() << endl;

		// test de la classe Player

		cout << "\nTest de la classe joueur (5 personnages maximum)" << endl;
		Player joueur;
		cout << "Ajout d'un personnage à la liste du du joueur" << endl;
		joueur.add_character(perso);
		cout << "Nombre d'attaque enregistré = " << joueur.get_number_of_characters() << endl;
		cout << "Ajout d'un personnage à la liste du du joueur" << endl;
		joueur.add_character(perso);
		cout << "Nombre d'attaque enregistré = " << joueur.get_number_of_characters() << endl;
		cout << "Ajout d'un personnage à la liste du du joueur" << endl;
		joueur.add_character(perso);
		cout << "Nombre d'attaque enregistré = " << joueur.get_number_of_characters() << endl;
		cout << "Ajout d'un personnage à la liste du du joueur" << endl;
		joueur.add_character(perso);
		cout << "Nombre d'attaque enregistré = " << joueur.get_number_of_characters() << endl;
		cout << "Ajout d'un personnage à la liste du du joueur" << endl;
		joueur.add_character(perso);
		cout << "Nombre d'attaque enregistré = " << joueur.get_number_of_characters() << endl;
		cout << "Ajout d'un personnage à la liste du du joueur" << endl;
		joueur.add_character(perso);
		cout << "Nombre d'attaque enregistré = " << joueur.get_number_of_characters() << endl;
		cout << "Ajout d'un personnage à la liste du du joueur" << endl;
		joueur.add_character(perso);
		cout << "Nombre d'attaque enregistré = " << joueur.get_number_of_characters() << endl;
		cout << "Ajout d'un personnage à la liste du du joueur" << endl;
		joueur.add_character(perso);
		cout << "Nombre d'attaque enregistré = " << joueur.get_number_of_characters() << endl;
		cout << "Ajout d'un personnage à la liste du du joueur" << endl;
		joueur.add_character(perso);
		cout << "Nombre d'attaque enregistré = " << joueur.get_number_of_characters() << endl;

		// test de la classe GameState

		cout << "\nTest de la classe etat de jeu (5 joueur maximum)" << endl;
		GameState etat;
		cout << "Ajout d'un joueur au jeu" << endl;
		etat.new_player("player");
		//cout << "Nombre de joueur enregistré = " << etat.get_number_of_player() << endl;
		cout << "Ajout d'un joueur au jeu" << endl;
		etat.new_player("player");
		//cout << "Nombre de joueur enregistré = " << etat.get_number_of_player() << endl;
		cout << "Ajout d'un joueur au jeu" << endl;
		etat.new_player("player");
		cout << "Nombre de joueur enregistré = " << etat.get_number_of_player() << endl;
		cout << "Ajout d'un joueur au jeu" << endl;
		etat.new_player("player");
		cout << "Nombre de joueur enregistré = " << etat.get_number_of_player() << endl;
		cout << "Ajout d'un joueur au jeu" << endl;
		etat.new_player("player");
		cout << "Nombre de joueur enregistré = " << etat.get_number_of_player() << endl;
		cout << "Ajout d'un joueur au jeu" << endl;
		etat.new_player("player");
		cout << "Nombre de joueur enregistré = " << etat.get_number_of_player() << endl;
		cout << "Ajout d'un joueur au jeu" << endl;
		etat.new_player("player");
		cout << "Nombre de joueur enregistré = " << etat.get_number_of_player() << endl;
		cout << "Ajout d'un joueur au jeu" << endl;
		etat.new_player("player");
		cout << "Nombre de joueur enregistré = " << etat.get_number_of_player() << endl;
		cout << "Ajout d'un joueur au jeu" << endl;
		etat.new_player("player");
		cout << "Nombre de joueur enregistré = " << etat.get_number_of_player() << endl;
		
	}
	return 0;
}



