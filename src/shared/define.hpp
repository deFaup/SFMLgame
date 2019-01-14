#ifndef DEFINE_H_
#define DEFINE_H_

#define MAX_NB_ATTACK 		5
#define MAX_NB_CHARACTER	5
#define MAX_NB_PLAYER		5


#define MAX_LIFE_POINT		2000	/** Defini le nombre max de point de vie autorisé */
#define MAX_ATTACK_POINT	2000	/** Defini le nombre max de point d'attaque autorisé */
#define MAX_MOVE_POINT		2000	/** Defini le nombre max de point de mouvement autorisé */

#define MAX_ATTACK_COST		100
#define MAX_ATTACK_DAMAGE	100

//speed at wich we move sprtite / characters
#define SPEED 8

// AI name
#define AI_1 "2409975411082179859"
#define AI_2 "9330385949384014003"
#define AI_3 "1601501520215445689"
#define AI_4 "802400488603333171"
#define AI_5 "1275483555166853449"

// define for the different test scenarios
#define REAL 0
#define RANDOM_AI 1
#define HEURISTIC_AI 2
#define DEEP_AI 3
#define ROLLBACK 4
#define THREAD 5
#define PLAY 6

#define JSON_FILENAME "replay.json"
#endif	/* DEFINE_H_ */

