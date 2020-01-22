#ifndef SETTINGS_H
#define SETTINGS_H


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

#define FRAME_TIME 16 // 33 -> 30 fps, 16 -> 60 fps
#define USE_HARDWARE_ACCELERATION 0
#define BENCHMARKING 0

#define IMAGE_FOLDER "images/" // Where the game images are saved. Don't remove the slash!
#define SCORES_FOLDER "scores/" // Where the top scores are stored.
#define SAVES_FOLDER "saves/" // Where the saved games are stored.
#define BEST_SCORES_FILENAME "best_scores" // Where the best results are stored.

#define ANIMATION_BLINKS_NUMBER 3
#define INPUT_DELAY (400. / FRAME_TIME) // 0.4 second
#define ANIMATION_DURATION (1000. / FRAME_TIME) // 1 second
#define BONUS_MIN_DELAY (12000. / FRAME_TIME) // 12 seconds
#define BONUS_RANGE_DELAY (5000. / FRAME_TIME) // 5 seconds

#define NB_MAX_SCORES 10
#define MAX_NAME_LENGTH 25

#define SHIP_JUMP (0.15 * FRAME_TIME)
#define SHIP_SIZE 60
#define BONUS_SIZE 40
#define ENEMY_SIZE 40

#define NB_ENEMY_ROWS 3
#define NB_ENNEMY_COLS 8

#define SHIP_YPOS (WINDOW_HEIGHT - SHIP_SIZE - 55)
#define BONUS_YPOS (0.1 * WINDOW_WIDTH)
#define STARTING_BONUS_SPEED (0.05 * FRAME_TIME)

#define ENEMY_JUMP 15
#define ENEMY_ATTACK_PROBABILITY 0.005
#define STARTING_ENEMY_SPEED (1000. / FRAME_TIME) // The lower the value, the quicker they move.

#define SHOT_LENGTH 10
#define SHOT_WIDTH 3
#define STARTING_SHOT_SPEED (0.1 * FRAME_TIME)

#define BONUS_REWARD 100
#define KILL_REWARD 10
#define ENEMY_SHOT_REWARD 1
#define PLAYER_HP 3
#define DIFFICULTY_STEP 0.05
#define SCORE_TO_GAIN_LIFE 1000


#endif
