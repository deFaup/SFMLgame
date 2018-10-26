#include <iostream>
#include <string>	//string
#include <cstring>	//strcmp

#include "define.hpp"

// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>

void testSFML() {
    sf::Texture texture;
	sf::Image image;
}
// Fin test SFML

/*************************************************************/
/*************************************************************/

#include "state.h"
#include "render.h"

using namespace std;
using namespace state;
using namespace render;

/*************************************************************/
/*--------- Unit tests for the package state ----------------*/
/*************************************************************/

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
	Characters perso("test class Characters");
	
	cout << "\n\nVerification des ajouts d'attaques possibles\n" << endl;

	void * random(0);
	for (int i = 0; i< 7; i++)
	{		
		cout << "\tAjout d'une attaque a la liste du personnage" << endl;
		perso.addAttack(10,10,random);
		cout << "\tNombre d'attaque enregistre = " << perso.get_number_of_attacks() << endl << endl;
	}

	cout << endl << "Fin du test de Characters" << endl << endl;
}

/* Test de la classe Player */
void testPlayer()
{
	cout << "\n\nTest de la classe Player" << endl;
	Player joueur;
	
	cout << "\nChoix du nombre maximum de personnages par joueur. Ici 4 (au +5)" << endl;
	Player::set_characters_range(4);

	cout << "Verification\n" << endl;
	for (int i = 0; i< 7; i++)
	{	
		{
			shared_ptr<Characters> perso = make_shared<Characters>();
			cout << "\tAjout d'un personnage a la liste du du joueur" << endl;
			joueur.add_character(perso);
			cout << "\tNombre de joueur enregistre = " << joueur.get_number_of_characters() << endl << endl;
		}
	}
	cout << "\nNous avons au total " << joueur.get_number_of_characters() << " personnages pour notre joueur\n" << endl;

	cout << "\nFin du test de Player" << endl << endl;
}

/* Test de la classe GameState */
void testGameState(){
	
	cout << "\n\nTest de la classe GameState" << endl;
	GameState etat;

	cout << "\nCreation d'une carte de jeu 800x600." << endl;
	etat.new_map(800,600);	

	cout << "\n\nVerification du nombre de joueurs max: 5\n" << endl;

	string name_player("");
	for (int i = 0; i< 7; i++)
	{
		cout << "\tAjout d'un joueur au jeu" << endl;
		name_player = "player " + to_string(i+1);
		etat.new_player(name_player);
		cout << "\tNombre de joueur enregistre = " << etat.get_number_of_player() << endl;
	}
	
	cout << "\nNous avons donc " << etat.get_number_of_player() <<" joueurs dans l'etat de jeu" << endl;
	cout << "Ajoutons un personnages a player 1." << endl;
	/* En utilisant new_character */
	etat.get_player(0)->new_character("john");

	cout << "\nAjoutons un personnages a player 2.\n" << endl;
	/* En utilisant add_character */
	shared_ptr<Characters> player2_char1 = make_shared<Characters>("player2_char1");
	etat.get_player(1)->add_character(player2_char1);

	cout << "\nFin du test de la classe GameState" << endl;

}

/*************************************************************/
/*--------- Unit tests for the package render ---------------*/
/*************************************************************/

void rendering()
{
	sf::RenderWindow renderWindow(sf::VideoMode(500, 500), "test affichage");

	sf::Event event;

  	sf::Image image;
  	image.create(800, 600, sf::Color::Black);

	sf::Image ciel, herbe, sol;
	ciel.loadFromFile("res/textureciel.png");
	herbe.loadFromFile("res/textureherbe.png");
	sol.loadFromFile("res/texturesol.png");

	GameState etat;
	etat.new_map(800,600);

	sf::Image person;
	person.loadFromFile("res/DBZ_gokusheet2.gif");
	person.createMaskFromColor(person.getPixel(1,1));

	sf::Texture textureperso;
  	textureperso.loadFromImage(person);

	sf::Sprite spriteperso(textureperso,sf::IntRect(134,192,80,80));

	const std::vector<std::vector<int>>& mask = etat.map.get_mask();
	//x_max is width, y_max is height
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

	/*
	GameState etats;
	etats.new_player("jack");
	etats.new_player("john");
	etats.players[0]->new_character("worm1");
	etats.players[0]->new_character("worm2");
	etats.players[1]->new_character("worm3");
	etats.players[1]->new_character("worm4");
	//etats.characters[0]->position.setPosition(10,10);
	//etats.characters[1]->position.setPosition(100,100);
	//etats.characters[2]->position.setPosition(50,100);
	//etats.characters[3]->position.setPosition(400,100);

	Grid grid = Grid(&etats);
	Character character = Character(&etats);
	Information info = Information(&etats);

	GridTileSet cart;
	cart.sky_file = "res/textureciel.png";
	cart.ground_file = "res/texturesol.png";
	cart.grass_file = "res/textureherbe.png";

	grid.tileset = cart;

	shared_ptr<CharacterTileSet> chara = make_shared<CharacterTileSet>();
	chara->tileset_file = "res/DBZ_gokusheet2.gif";
	std::vector<sf::IntRect> vect;
	vect.push_back(sf::IntRect(134,192,80,80));
	chara->CharacterTile = vect;
	character.tilesets.push_back(chara);
	character.tilesets.push_back(chara);
	character.tilesets.push_back(chara);
	character.tilesets.push_back(chara);

	GameRender rendu;
	rendu.grid = grid;
	rendu.character = character;
	rendu.info = info;
	
	rendu.rend();
	*/
}

void render_state()
{
	GameState etat;
	etat.new_map(800, 600);
	etat.new_player("Joueur 1");
	etat.get_player(0)->new_character("goku");

	//etat.new_player("Joueur 2");
	//etat.get_player(0)->new_character("vegeta");
	Scene scene(etat, etat.get_map());
	scene.draw();

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
			testPlayer();
			testGameState();
		}
		
		if (strcmp(argv[1], "render")==0)
		{
			testSFML();
			//rendering();
			render_state();
		}
	}
	return 0;
}



