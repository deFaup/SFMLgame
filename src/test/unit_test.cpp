// Unitary tests for state
#include "unit_test.hpp"

using namespace std;
/*************************************************************/
/*--------- Unit tests for the package state ----------------*/
/*************************************************************/

/********************************/
//			Statistics			//
/********************************/

// MAX_LIFE_POINT		2000
// MAX_ATTACK_POINT		2000
// MAX_MOVE_POINT		2000

int result_statistics()
{
	Statistics stat_test_enum(4000, -100, 1.2); // should set stats to 2000, 0 and 1
	Statistics stat_test_id(goku);

	cout << "test get_life with 4000, expected 2000: " << test_get_life_point(stat_test_enum, 2000) << endl;
	cout << "test get_life with goku, expected 100: " << test_get_life_point(stat_test_id, 100) << endl;
	
	cout << "test_get_attack_point with -100, expected 0: " << test_get_attack_point(stat_test_enum, 0) << endl;
	cout << "test_get_attack_point with goku, expected 10: " << test_get_attack_point(stat_test_id, 10) << endl;
	
	cout << "test_get_move_point with 1.2, expected 1: " << test_get_move_point(stat_test_enum, 1) << endl;
	cout << "test_get_move_point with goku, expected 100: " << test_get_move_point(stat_test_id, 100) << endl;

	cout << "test_set_statistics on goku with 10,10,10: " << test_set_statistics(stat_test_id, 10,10,10) << endl;
	cout << "test_reset_all_but_life on goku expected 100/10/100: " << test_reset_all_but_life(stat_test_id, goku) << endl;

	return 0;
}

bool test_get_life_point(Statistics m_stat, unsigned int life)
{
	unsigned int m_life = m_stat.get_life_point(); //get life
	m_life = -1;	// modify the value that we got
	m_life -= 1;	// modify the value that we got
	return (m_stat.get_life_point() == life); // check if the attribute has been changed
}

bool test_get_attack_point(Statistics m_stat, unsigned int attack)
{
	unsigned int m_attack = m_stat.get_attack_point(); //get life
	m_attack = -1;	// modify the value that we got
	m_attack -= 1;	// modify the value that we got
	return (m_stat.get_attack_point() == attack); // check if the attribute has been changed
}

bool test_get_move_point(Statistics m_stat, unsigned int move)
{
	unsigned int m_move = m_stat.get_move_point(); //get life
	m_move = -1;	// modify the value that we got
	m_move -= 1;	// modify the value that we got
	return (m_stat.get_move_point() == move); // check if the attribute has been changed
}

bool test_set_statistics(Statistics given_stats, unsigned life, unsigned attack, unsigned move)
{
	given_stats.set_statistics(Statistics(life, attack, move));
	return (given_stats.get_life_point() == life && given_stats.get_attack_point() == attack && given_stats.get_move_point() == move);
}

bool test_reset_all_but_life(Statistics given_stats, CharactersID id)
{
	given_stats.reset_all_but_life(id);
	// if after the reset the stats are equals to these stats then it's ok
	unsigned int attack_point, move_point;
	switch (id)
	{
	case goku:
		//life_point = 100;
		attack_point = 10;
		move_point = 100;
		break;

	case vegeta:
		//life_point = 100;
		attack_point = 10;
		move_point = 100;
		break;

	case miyo:
		//life_point = 100;
		attack_point = 10;
		move_point = 100;
		break;
	}

	return (given_stats.get_attack_point() == attack_point && given_stats.get_move_point() == move_point);
}

/********************************/
//			Position			//
/********************************/

int result_position()
{
	cout << "\n\n";
	Position m_pos; // x(0), y(0)
	cout << "test_getPositionX/Y with 0,0; expected 0/0: ";
	cout << (test_getPositionX(m_pos, 0) && test_getPositionY(m_pos, 0)) << endl;
	
	cout << "test_setPosition with 4000,10; expected 4000 / 10: " << test_setPosition(m_pos, 4000,10) << endl;

	cout << "test_getPositionX/Y with 4000, 10; expected 4000 / 10: ";
	cout << (test_getPositionX(m_pos, 4000) && test_getPositionY(m_pos, 10)) << endl;

	cout << "test_increaseX/Y with 13,11; expected 4013,21" << endl;
	cout << (test_increaseX(m_pos, 13) && test_increaseY(m_pos, 11)) << endl;
	
	return 0;
}

bool test_getPositionX(Position pos, const unsigned int x)
{
	unsigned int temp = pos.getPositionX();
	temp += 12; // to check if we modify the value in its object or if it is independant
	return pos.getPositionX() == x;	
}

bool test_getPositionY(Position pos, const unsigned int y)
{	return pos.getPositionY() == y;	}

bool test_setPosition(Position& pos, const unsigned int x, const unsigned int y)
{
	pos.setPosition(x, y);
	return (pos.getPositionX() == x && pos.getPositionY() == y);
}

bool test_increaseX(Position& pos, int increment)
{
	unsigned manual_increase = pos.getPositionX() + increment;
	pos.increaseX(increment);
	return pos.getPositionX() == manual_increase;
}

bool test_increaseY(Position& pos, int increment)
{
	unsigned manual_increase = pos.getPositionY() + increment;
	pos.increaseY(increment);
	return pos.getPositionY() == manual_increase;
}

/********************************/
//			Characters			//	// ANTOINE 
/********************************/
/*void testCharacters(){

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
}*/
int result_characters()
{
	cout << "\n\n";

	return 0;
}
/********************************/
//			Player				//	 
/********************************/

/* Test de la classe Player */
/*void testPlayer()
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
}*/

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