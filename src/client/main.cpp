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
#include "engine.h"
#include "ai.h"
#include <thread>

using namespace std;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;

/* Test de la classe Characters */

/* Test de la classe GameState */
void testGameState() {

	cout << "\n\nTest de la classe GameState" << endl;
	GameState etat;

	cout << "\nCreation d'une carte de jeu 800x600." << endl;
	etat.new_map(800, 600);

	cout << "\n\nVerification du nombre de joueurs max: 5\n" << endl;

	string name_player("");
	for (int i = 0; i< 7; i++)
	{
		cout << "\tAjout d'un joueur au jeu" << endl;
		name_player = "player " + to_string(i + 1);
		etat.new_player(name_player);
		cout << "\tNombre de joueur enregistre = " << etat.get_number_of_player() << endl;
	}

	cout << "\nNous avons donc " << etat.get_number_of_player() << " joueurs dans l'etat de jeu" << endl;
	cout << "Ajoutons un personnages a player 1." << endl;
	/* En utilisant new_character */
	etat.get_player(0)->new_character(vegeta);

	//cout << "\nAjoutons un personnages a player 2.\n" << endl;
	/* En utilisant add_character */
	//shared_ptr<Characters> player2_char1 = make_shared<Characters>(default_value);
	//etat.get_player(1)->add_character(player2_char1);

	cout << "\nFin du test de la classe GameState" << endl;

}

/*************************************************************/
/*--------- Unit tests for the package render ---------------*/
/*************************************************************/

/*void render_state()
{
	GameState etat;
	etat.new_map(3000, 2000);

	etat.new_player("Joueur 1");
	cout << "nombre de joueurs: " << etat.get_number_of_player() << endl;

	etat.new_character(0, goku);
	etat.get_player(0)->get_character(0)->position.setPosition(200, 200);
	cout << "nombre de personnages du joueur 0: " << etat.get_player(0)->get_number_of_characters() << endl;


	etat.new_player("Joueur 2");
	cout << "\nnombre de joueurs: " << etat.get_number_of_player() << endl;

	etat.new_character(1, vegeta);
	cout << "nombre de personnages du joueur 1: " << etat.get_player(1)->get_number_of_characters() << endl;
	etat.get_player(1)->get_character(0)->position.setPosition(400, 400);

	cout << "\nnombre de personnages: " << etat.characters.size() << endl;

	Scene scene(etat, etat.get_map());	//etat.get_map(); is not the problem scene is
	scene.draw();
}
*/

void enginet(int ai_type)
{// problème à résoudre: une fois que l'on a touché le sol si l'on va tout à gauche de l'écran 
//	puis que l'on repart à droite ALORS seg fault
	sf::RenderWindow renderWindow(sf::VideoMode(1102,869), "menu test");

	sf::Texture pic; 
	pic.loadFromFile("res/menu.png");
	sf::Sprite menu_sprite(pic);

	bool Return_key_pressed(false);
	while (!Return_key_pressed)
	{
		sf::Event event;
		while (renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Return)
					Return_key_pressed = true;
			}
		}
		renderWindow.draw(menu_sprite);
		renderWindow.display();
		renderWindow.clear();
	}

	/* le thread render/main aura une méthode class whatever pour afficher un menu dans renderwindow.
	Dans le menu on choisit le nombre de joueurs et de personnages + autres paramètres si besoin.
	Ensuite tous ces paramètres sont transmis au gameEngine qui va ensuite modifier le state.
	Seul le moteur peut modifier l'état pour que cela marche en mode réseau.
	La méthode menu sera appelé par l'engine au premier call de check state ID
	*/

	GameState etat;
	GameEngine engine(&etat); std::thread thread_engine;
	Controller controller(renderWindow, engine, etat);

	DeepAI *ai_deep(0); HyAI *ai_heuristic(0); RandomAI *ai_random(0); // Classe AI et système d'héritage à faire
	if (ai_type == 1) ai_random = new RandomAI(engine);
	else if (ai_type == 2) ai_heuristic = new HyAI(engine);
	else if (ai_type == 3) ai_deep = new DeepAI(engine);
	cout << "main: IA created" << endl;

	shared_ptr<Scene> scene = make_shared<Scene>(renderWindow, etat);
	cout << "main: scene created\n" << endl;

	/* Linking the observer to each observable */
	//in Characters::stats & position + Player + Map + GameState
	etat.registerObserver(scene);
	etat.map.registerObserver(scene);
	cout << "main: observers listed\n" << endl;

	engine.init_game(ai_type); //create the team when id is "not started", id="team selected"
	scene->background.new_background_layer();
	scene->characters.new_character_layer();

	thread_engine = thread(&engine::GameEngine::workLoop, &engine);
	while (renderWindow.isOpen())
	{
		renderWindow.display();

		// Process events
		sf::Event event;
		while (renderWindow.pollEvent(event))
			controller.handle_sfEvents(event);
		
		if (ai_random) ai_random->play();
		if (ai_heuristic) ai_heuristic->play();
		if (ai_deep) ai_deep->play();

		renderWindow.clear();
		while (engine.updating) {}
		scene->draw();

		Statistics& stats = etat.current_player->get_current_character()->stats;

		sf::String info;

		info += "Current Player: " + etat.current_player->name + "\n\n";
		info += "Current Character Statistics:\nlife point : ";
		info += std::to_string(stats.get_life_point());
		info += "\nmove point : ";
		info += std::to_string(stats.get_move_point());
		info += "\nattack point : ";
		info += std::to_string(stats.get_attack_point());


		sf::Text text;

		// choix de la police à utiliser
		
		sf:: Font font;
		font.loadFromFile("res/arial.ttf");

		text.setFont(font); // font est un sf::Font

		// choix de la chaîne de caractères à afficher
		text.setString(info);
	
		// choix de la taille des caractères
		text.setCharacterSize(50); // exprimée en pixels, pas en points !

		// choix de la couleur du texte
		//text.setFillColor(sf::Color::Black);

		// choix du style du texte
		text.setStyle(sf::Text::Bold /*| sf::Text::Underlined*/);

		// puis, dans la boucle de dessin, entre window.clear() et window.display()
		renderWindow.draw(text);

		//renderWindow.display();
		
	}
	
	thread_engine.join();
	cout << "engine thread closed\n";
}

int main(int argc, char* argv[])
{

	if (argc == 2)
	{
		if (strcmp(argv[1], "hello") == 0)
			cout << "Bienvenue chez antoine et gregoire !" << endl;

		if (strcmp(argv[1], "state") == 0)
		{
			//testStatistics();
			//testCharacters();
			//testPlayer();
			testGameState();
		}

		if (strcmp(argv[1], "render") == 0)
		{
			testSFML();
			//rendering();
			//render_state();
		}

		if (strcmp(argv[1], "engine") == 0)
			enginet(0);
	
		if (strcmp(argv[1], "random_ai") == 0)
			enginet(1);

		if (strcmp(argv[1], "heuristic_ai") == 0)
			enginet(2);		
		
		if (strcmp(argv[1], "deep_ai") == 0)
			enginet(3);

		if (strcmp(argv[1], "test") == 0)
		{
			#include "unit_test.hpp"
			result_statistics();
			result_position();
		}
	}
	return 0;
}



