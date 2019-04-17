#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Other */
#define WIDTH 1280 
#define HEIGHT 720
#define SPEED_CHAR 150
#define DELTA_TIME 0.0167
#define WINDOW_TITLE "Royal Retribution"
#define SCORE_FILENAME "data/scores.txt"
#define SPRITE_PADDING_AMOUNT_X 28
#define SPRITE_PADDING_AMOUNT_Y 20
#define NUM_STATES              9  /* INCLUDE ALL STATES EXCEPT LOADING */
#define LOADTIME_FILENAME "data/loading_times"
#define PAN_DELAY_SECONDS     3

/* Maps */
#define TILES_IMG "assets/blocks.png"
#define TILES_TXT "maps/level_legend.txt"
#define TILES_ADD "maps/level_additions_legend.txt"
#define FOLLOW_RADIUS 100
// Intro
#define INTRO "maps/intro.txt"
#define INTRO_ADD "maps/intro_additions.txt"
// Tutorial
#define TUTORIAL "maps/tutorial.txt"
#define TUTORIAL_ADD "maps/tutorial_additions.txt"
// level 1
#define LEVEL_1 "maps/level_1.txt"
#define LEVEL_1_E "maps/level_1_enemies.txt"
#define LEVEL_1_ADD "maps/level_1_additions.txt"
// level 2
#define LEVEL_2 "maps/level_2.txt"
#define LEVEL_2_E "maps/level_2_enemies.txt"
#define LEVEL_2_ADD "maps/level_2_additions.txt"
// level 3
#define LEVEL_3 "maps/level_3.txt"
#define LEVEL_3_E "maps/level_3_enemies.txt"
#define LEVEL_3_ADD "maps/level_3_additions.txt"
// level 4
#define LEVEL_4 "maps/level_4.txt"
#define LEVEL_4_E "maps/level_4_enemies.txt"
#define LEVEL_4_ADD "maps/level_4_additions.txt"
//boss level
#define FINAL_LEVEL "maps/final_level.txt"

/* Fonts */
#define FONT_ARCADE "assets/arcade.ttf"
#define FONT_ROBOTO "assets/Roboto.ttf"
#define FONT_FILENAME "assets/digit.ttf"

/* Audio */
#define MAX_MUSIC_VOLUME 60
#define KILL_FILENAME "assets/sword.wav"
#define INTRO_FILENAME "assets/intro.xm"
#define THEME1_FILENAME "assets/theme1.ogg"
#define BOSS_THEME_FILENAME "assets/BossFight.ogg"
#define COLLISION_FILENAME "assets/collision.wav"

/* Sprites */
#define NUM_ROWS 4
#define NUM_COLS 4
#define HEART "assets/Heart.png"
#define SWORD "assets/characters/Sword_anim.png"
#define ANI_FILENAME "assets/characters/Animations.png"
#define SQUIRE_FILENAME "assets/characters/Squire_sheet.png"
#define BIG_HEAD "assets/characters/alien_big_head.png"
#define BIG_BODY "assets/characters/alien_big_body.png"
#define BIG_RF "assets/characters/alien_big_Rfist.png"
#define BIG_LF "assets/characters/alien_big_Lfist.png"
#define BIG_BEAM "assets/characters/beam.png"

/* Title */
#define BG_FILENAME "assets/space.png"
#define SHIP_FILENAME "assets/ship.png"
#define CASTLE_FILENAME "assets/castle.png"
#define PLANET_FILENAME "assets/planet.png"

/* Torch Light */
#define BLACK_PIXEL "assets/black.png"
#define LIGHTS_FILENAME "assets/lights.png"
#define CAMERA_LIGHT "assets/light_camera.png"
#define THRONE_FILENAME "assets/throne.png"

/* Level Elements */
#define KEY "assets/key.png"
#define COIN "assets/coin.png"
#define FOOD "assets/food.png"
#define MILK "assets/milk.png"
#define STAIRS_FILENAME "assets/stairs.png"

/* Bars */
#define BAR_BOX "assets/bar_outline.png"
#define STA_BAR "assets/stamina_bar.png"
#define EXP_BAR "assets/experience_bar.png"

/* LAYER */
#define ELE 0
#define PPL 1
#define ADD 2
#define TOP 3

#endif
