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
void testPlayer(/*Gamestate etat&*/){

	cout << "\n\nTest de la classe Player" << endl;
	Player joueur;
	
	cout << "\n\nVerification du nombre de personnages par joueur (5 max)\n" << endl;
	for (int i = 0; i< 7; i++)
	{	
		{
			unique_ptr<Characters> perso = make_unique<Characters>();
			cout << "\tAjout d'un personnage a la liste du du joueur" << endl;
			joueur.add_character(perso);
			cout << "\tNombre d'attaque enregistre = " << joueur.get_number_of_characters() << endl << endl;
		}
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
		cout << "\tAjout d'un joueur au jeu" << endl;
		etat.new_player("player");
		cout << "\tNombre de joueur enregistre = " << etat.get_number_of_player() << endl;
	}
	
	cout << "\n\nNous avons donc 5 joueurs dans l'etat de jeu\n" << endl;
	cout << "\nAjoutons des joueurs à nos personnages.\n" << endl;

}


void render()
{
	sf::RenderWindow renderWindow(sf::VideoMode(500, 500), "test affichage");

	sf::Event event;

  	sf::Image image;
  	image.create(500, 500, sf::Color::Black);

	sf::Image ciel;
	ciel.loadFromFile("textureciel.png");

	sf::Image herbe;
	herbe.loadFromFile("textureherbe.png");

	sf::Image sol;
	sol.loadFromFile("texturesol.png");

	GameState etat;
	etat.new_map();

	sf::Image person;
	person.loadFromFile("DBZ_gokusheet2.gif");
	person.createMaskFromColor(person.getPixel(1,1));

	sf::Texture textureperso;
  	textureperso.loadFromImage(person);

	sf::Sprite spriteperso(textureperso,sf::IntRect(134,192,80,80));

	std::vector<std::vector<int>>& mask = etat.map.get_mask();

	for (int y = 0; y < 500; y++){
		for (int x = 0; x < 500; x++){
			if (mask[y][x] == 1){
				image.setPixel(x, y, sol.getPixel(x,y));
			}
			else if(mask[y][x] == 2){
				image.setPixel(x, y, herbe.getPixel(x,y));
			}
			else {
				image.setPixel(x, y, ciel.getPixel(x,y));
			}
		}
	}
	sf::Texture texture;
  	texture.loadFromImage(image);
  	sf::Sprite sprite(texture);

	while (renderWindow.isOpen()){
		while (renderWindow.pollEvent(event)){
			if (event.type == sf::Event::EventType::Closed)
			renderWindow.close();
		}

		renderWindow.clear();
		renderWindow.draw(sprite);
		renderWindow.draw(spriteperso);
		renderWindow.display();
	}	
}

int main(int argc,char* argv[]) 
{	

	if(argc == 2)
	{
		if(strcmp(argv[1], "hello") == 0 )
		{
			cout << "Bienvenue chez antoine et gregoire !" << endl;
		}

		if(strcmp(argv[1], "state") == 0 )
		{

			testStatistics();
			testCharacters();
			testGameState();
			testPlayer();

		}
		
		if (strcmp(argv[1], "render")==0)
		{
			render();
		}
	}
	return 0;
}



