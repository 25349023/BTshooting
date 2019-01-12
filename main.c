#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "general.h"
#include "bullet.h"
#include "character.h"
#include "fireworks.h"
#include "levelSetting.h"
#include "skill.h"

#define GAME_TERMINATE (-1)

// ALLEGRO Variables
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_BITMAP *cursorImg = NULL;
ALLEGRO_BITMAP *airplaneImgs[20];
ALLEGRO_BITMAP *airLargeImgs[20];
ALLEGRO_BITMAP *airGrayImgs[20];
ALLEGRO_BITMAP *enemyImgs[20];
ALLEGRO_BITMAP *bulletImgs[20];
ALLEGRO_BITMAP *messageImg = NULL;
ALLEGRO_BITMAP *optionsImg = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_TIMER *bulletUpdateTimer = NULL;
ALLEGRO_TIMER *drawingTimer = NULL;
ALLEGRO_TIMER *playerShootingTimer = NULL;
ALLEGRO_TIMER *enemyShootingTimer = NULL;
ALLEGRO_TIMER *playerMovingTimer = NULL;
ALLEGRO_TIMER *enemyMovingTimer = NULL;
ALLEGRO_TIMER *collisionDetectTimer = NULL;
ALLEGRO_TIMER *generateDroppingBulletTimer = NULL;
ALLEGRO_TIMER *generateEnemyTimer = NULL;
ALLEGRO_TIMER *bossComingOutTimer = NULL;
ALLEGRO_TIMER *bossMovingTimer = NULL;
ALLEGRO_TIMER *bossShootingTimer = NULL;
ALLEGRO_TIMER *warningTimer = NULL;
ALLEGRO_TIMER *hintTimer = NULL;

ALLEGRO_TIMER *skillQTimer = NULL;
ALLEGRO_TIMER *skillWTimer = NULL;
ALLEGRO_TIMER *defendTimer = NULL;

ALLEGRO_TIMER *fireworksTickTimer = NULL;
ALLEGRO_TIMER *generateFirework1Timer = NULL;
ALLEGRO_TIMER *happyTimer = NULL;
ALLEGRO_TIMER *the2019Timer = NULL;

ALLEGRO_SAMPLE *menuMusic = NULL;
ALLEGRO_SAMPLE *gameMusic = NULL;
ALLEGRO_SAMPLE *letoff, *letoff2;
ALLEGRO_SAMPLE_INSTANCE *letoffInstance[2];
ALLEGRO_FONT *bigFont = NULL;
ALLEGRO_FONT *midFont = NULL;
ALLEGRO_FONT *smallFont = NULL;
ALLEGRO_FONT *warningFont = NULL;
ALLEGRO_FONT *hintFont = NULL;
ALLEGRO_MOUSE_CURSOR *cursor = NULL;

ALLEGRO_COLOR white;

//Custom Definition
const char *title = "Final Project 107062303";
const double FPS = 60;
const int WIDTH = 400;
const int HEIGHT = 600;

enum windows {
    none, menu, main_game, about, special, stage_choosing,
    message_box, options, airplane_choosing
};

Bullet *player_bullet_list = NULL;
Bullet *enemy_bullet_list = NULL;
Bullet *fireworks_bullet_list = NULL;

Character player;
Character boss;
Character *enemy_list = NULL;

PlayerSetting playerPrefabs[10];
EnemySetting enemyPrefabs[10];

LevelSetting settings[6];  // 0 for tutorial

int window = menu;
int draw_count = 0;
int level = 1;
int score;
int killed_enemies = 0;
int chosenPlayer = 0;
int message_count = 0;
int reading = 0;
int airplanePage = 0;

char hint[48] = "hello world";
char messageTitle[50][24];
char messageContent[50][64];

bool enter_game_window = false;
bool game_pause = false;
bool draw_boss = false;
bool pass = false;
bool unlock_airplane[10] = {true, [4]=true, [5]=true};
bool hintOut[4] = {false};
bool has_read[50] = {false};
bool musicOn = false;
bool soundOn = true;


void show_err_msg(int msg);
void game_init();
void game_begin();
int process_event();
int game_run();
void game_destroy();
void load_images();

bool within(float x, float y, float x1, float y1, float x2, float y2);
bool collide_with(Circle a, Circle b);
void process_bullets(Bullet **list);
void draw_bullets(Bullet *list);

void draw_menu();
void draw_about();
void draw_game_scene();
void draw_stage_choosing();
void draw_message_box();
void draw_options();
void draw_character_choosing();

void show_warning();
void show_hint();
void send_message(char *title, char *content);

void reset_level();
void next_level();


int main(int argc, char *argv[]) {
    int msg = 0;

    srand((unsigned int) time(NULL));

    game_init();
    game_begin();

    while (msg != GAME_TERMINATE){
        msg = game_run();
        if (msg == GAME_TERMINATE){
            printf("Game Over\n");
        }
    }

    game_destroy();
    return 0;
}

void show_err_msg(int msg) {
    fprintf(stderr, "unexpected msg: %d\n", msg);
    game_destroy();
    exit(9);
}

void game_init() {
    if (!al_init()){
        show_err_msg(-1);
    }
    if (!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        show_err_msg(-2);
    }
    if (!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        show_err_msg(-3);
    }
    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to reserve samples!\n");
        show_err_msg(-4);
    }
    // Create display
    display = al_create_display(WIDTH, HEIGHT);
    event_queue = al_create_event_queue();
    if (display == NULL || event_queue == NULL){
        show_err_msg(-5);
    }
    // Initialize Allegro settings
    al_set_window_position(display, 0, 0);
    al_set_window_title(display, title);
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_audio();
    al_install_mouse();
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    // initialize variables
    white = al_map_rgb(255, 255, 255);
}

void load_images() {
    char filename[20];
    for (int i = 0; i < 10; i++){
        sprintf(filename, "bullet%d.png", i);
        bulletImgs[i] = al_load_bitmap(filename);
    }
    for (int i = 0; i < 6; i++){
        sprintf(filename, "airplane%d.png", i);
        airplaneImgs[i] = al_load_bitmap(filename);
    }
    for (int i = 0; i < 6; i++){
        sprintf(filename, "airLarge%d.png", i);
        airLargeImgs[i] = al_load_bitmap(filename);
    }
    for (int i = 0; i < 6; i++){
        sprintf(filename, "airGray%d.png", i);
        airGrayImgs[i] = al_load_bitmap(filename);
    }
    for (int i = 0; i < 2; i++){
        sprintf(filename, "enemy%d.png", i);
        enemyImgs[i] = al_load_bitmap(filename);
    }
    cursorImg = al_load_bitmap("cursor2.png");
    messageImg = al_load_bitmap("message.png");
    optionsImg = al_load_bitmap("settings.png");
    enemyImgs[10] = al_load_bitmap("boss0.png");
}

void game_begin() {
    // Load sound
    menuMusic = al_load_sample("Long_Stream.wav");
    gameMusic = al_load_sample("game.wav");
    letoff = al_load_sample("fireworks.wav");
    letoff2 = al_load_sample("fireworks2.wav");
    al_reserve_samples(5);
    for (int i = 0; i < 2; i++){
        letoffInstance[i] = al_create_sample_instance(letoff);
        al_set_sample_instance_playmode(letoffInstance[i], ALLEGRO_PLAYMODE_ONCE);
        al_set_sample_instance_gain(letoffInstance[i], 1.0);
        al_attach_sample_instance_to_mixer(letoffInstance[i], al_get_default_mixer());
    }
    if (!menuMusic || !letoff){
        printf("Audio clip sample not loaded!\n");
        show_err_msg(-6);
    }
    // Loop the menuMusic until the display closes
    al_play_sample(menuMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    // load font
    bigFont = al_load_ttf_font("pirulen.ttf", 25, 0);
    midFont = al_load_ttf_font("pirulen.ttf", 20, 0);
    smallFont = al_load_ttf_font("pirulen.ttf", 15, 0);
    warningFont = al_load_font("bauhs93.ttf", 36, 0);
    hintFont = al_load_font("geometria.otf", 24, 0);

    // load images
    load_images();
    draw_menu();
    cursor = al_create_mouse_cursor(cursorImg, 0, 0);
    if (!cursor){
        show_err_msg(5);
    }
    al_set_mouse_cursor(display, cursor);

    // TODO: set playerPrefabs
    set_airplane(&playerPrefabs[0], airplaneImgs[0], 100, 5, bulletImgs[0], 1.0 / 7.0, 2,
                 (enum flyMode[]) {up}, 1,
                 (Skill[4]) {launch_big_fire, launch_big_arrow}, (int[]) {5, 3, 5, 5});
    set_airplane(&playerPrefabs[1], airplaneImgs[1], 120, 6, bulletImgs[8], 1.0 / 6.0, 2,
                 (enum flyMode[]) {up, right_back, left_back}, 3,
                 (Skill[4]) {launch_big_arrow, shoot_triangle}, (int[]) {3, 1, 5, 5});
    set_airplane(&playerPrefabs[2], airplaneImgs[2], 60, 8, bulletImgs[5], 1.0 / 5.0, 2,
                 (enum flyMode[]) {up, right_back, left_back, rf_f, lf_f}, 5,
                 (Skill[4]) {defend}, (int[]) {10, 5, 5, 5});
    set_airplane(&playerPrefabs[3], airplaneImgs[3], 90, 4, bulletImgs[6], 1.0 / 6.0, 1.5,
                 (enum flyMode[]) {down, right_back, left_back, rb_b, lb_b}, 5,
                 (Skill[4]) {launch_big_arrow}, (int[]) {5, 5, 5, 5});
    set_airplane(&playerPrefabs[4], airplaneImgs[4], 80, 5, bulletImgs[9], 1.0 / 9.0, 2.2,
                 (enum flyMode[]) {right_front, left_front, right_back, left_back}, 4,
                 (Skill[4]) {big_blow, thin_circle}, (int[]) {6, 3, 5, 5});
    set_airplane(&playerPrefabs[5], airplaneImgs[5], 90, 7, bulletImgs[8], 1.0 / 6.0, 2,
                 (enum flyMode[]) {up, down, right, left}, 4,
                 (Skill[4]) {big_blow, horizontal_blow}, (int[]) {4, 5, 5, 5});

    // TODO: set enemyPrefabs
    set_enemy(&enemyPrefabs[0], enemyImgs[0], bulletImgs[3], 12, 8, 30, (Vector2) {-1, 0}, 5, 180, 1.8,
              (enum flyMode[]) {down}, 1);
    set_enemy(&enemyPrefabs[1], enemyImgs[1], bulletImgs[7], 25, 10, 30, (Vector2) {-1, 0}, 4, 180, 1.8,
              (enum flyMode[]) {down, right, left}, 3);

    // TODO: set level settings
    set_level(&settings[0], 0, 15, 3, 5, enemyImgs[10], bulletImgs[1], 1.0 / 3.0, 1,
              (enum flyMode[]) {down}, 1, 1, (int[]) {150}, bulletImgs[4], 1.0 / 3.0, 1);
    set_level(&settings[1], 1, 120, 5, 12, enemyImgs[10], bulletImgs[1], 1.0 / 3.0, 1,
              (enum flyMode[]) {down, rb_b, lb_b}, 3, 1, (int[]) {90}, bulletImgs[4], 1.0 / 3.0, 1);
    set_level(&settings[2], 2, 500, 6, 15, enemyImgs[10], bulletImgs[2], 1.0 / 2.0, 1,
              (enum flyMode[]) {down, rb_b, lb_b, right_back, left_back}, 5, 1, (int[]) {75},
              bulletImgs[4], 1.0 / 4.0, 1);
    set_level(&settings[3], 3, 888, 7, 20, enemyImgs[10], bulletImgs[3], 1.0 / 2.0, 1,
              (enum flyMode[]) {down, rb_b, lb_b, right, left}, 5, 2, (int[]) {90, 150},
              bulletImgs[4], 1.0 / 4.0, 0.8);
    settings[1].enemy_prefab[0] = &enemyPrefabs[0];
    settings[2].enemy_prefab[0] = &enemyPrefabs[0];
    settings[3].enemy_prefab[0] = &enemyPrefabs[0];
    settings[3].enemy_prefab[1] = &enemyPrefabs[1];


    send_message("HOW TO PLAY", "Use arrow keys to move\nthe player.");
}


int process_event() {
    // Request the event
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    static int sparkle_times[4] = {0};
    static bool warning_appear = false, hint_appear = false;
    if (event.timer.source == bulletUpdateTimer){
        process_bullets(&player_bullet_list);
        process_bullets(&enemy_bullet_list);
        if (window == special){
            for (Bullet *current = fireworks_bullet_list, *previous = fireworks_bullet_list; current != NULL;){
                if (current != fireworks_bullet_list && previous->next != current){
                    previous = previous->next;
                }
                float mul = current->speed_multiplier;
                switch (current->mode){
                    case up:
                        if (!current->pause){
                            current->pos.y -= 8 * mul;
                        }
                        al_draw_bitmap(current->bitmap, current->pos.x - current->size.x / 2, current->pos.y,
                                       current->flip);
                        break;
                    case right_front:
                        if (!current->pause){
                            current->pos.x += 5.7 * mul;
                            current->pos.y -= 5.7 * mul;
                        }
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (ALLEGRO_PI / 4), current->flip);
                        break;
                    case left_front:
                        if (!current->pause){
                            current->pos.x -= 5.7 * mul;
                            current->pos.y -= 5.7 * mul;
                        }
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (-ALLEGRO_PI / 4), current->flip);
                        break;
                    case right:
                        if (!current->pause){
                            current->pos.x += 8 * mul;
                        }
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (ALLEGRO_PI / 2), current->flip);
                        break;
                    case left:
                        if (!current->pause){
                            current->pos.x -= 8 * mul;
                        }
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (-ALLEGRO_PI / 2), current->flip);
                        break;
                    case right_back:
                        if (!current->pause){
                            current->pos.x += 5.7 * mul;
                            current->pos.y += 5.7 * mul;
                        }
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (ALLEGRO_PI * 3 / 4), current->flip);
                        break;
                    case left_back:
                        if (!current->pause){
                            current->pos.x -= 5.7 * mul;
                            current->pos.y += 5.7 * mul;
                        }
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (-ALLEGRO_PI * 3 / 4), current->flip);
                        break;
                    case down:
                        if (!current->pause){
                            current->pos.y += 8 * mul;
                        }
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) ALLEGRO_PI, current->flip);
                        break;
                    case rf_f:
                        current->pos.x += 3;
                        current->pos.y -= 7.4;
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (ALLEGRO_PI / 180 * 22.5), current->flip);
                        break;
                    case lf_f:
                        current->pos.x -= 3;
                        current->pos.y -= 7.4;
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (-ALLEGRO_PI / 180 * 22.5), current->flip);
                        break;
                    case rf_r:
                        current->pos.x += 7.4;
                        current->pos.y -= 3;
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (ALLEGRO_PI / 180 * 67.5), current->flip);
                        break;
                    case lf_l:
                        current->pos.x -= 7.4;
                        current->pos.y -= 3;
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (-ALLEGRO_PI / 180 * 67.5), current->flip);
                        break;
                    case rb_r:
                        current->pos.x += 7.4;
                        current->pos.y += 3;
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (ALLEGRO_PI / 180 * 112.5), current->flip);
                        break;
                    case lb_l:
                        current->pos.x -= 7.4;
                        current->pos.y += 3;
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (-ALLEGRO_PI / 180 * 112.5), current->flip);
                        break;
                    case rb_b:
                        current->pos.x += 3;
                        current->pos.y += 7.4;
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (ALLEGRO_PI / 180 * 157.5), current->flip);
                        break;
                    case lb_b:
                        current->pos.x -= 3;
                        current->pos.y += 7.4;
                        al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                               (float) (-ALLEGRO_PI / 180 * 157.5), current->flip);
                        break;
                    case stopped:
                        destroy_bullet(current, &previous, &fireworks_bullet_list);
                        current = previous;
                        break;
                    default:
                        printf("unknown fly mode\n");
                        show_err_msg(1);
                        break;
                }
                if (current == NULL){
                    if (fireworks_bullet_list == NULL){
                        break;
                    }
                    current = fireworks_bullet_list;
                    previous = fireworks_bullet_list;
                    continue;
                }
                // set the outside bullet stopped
                if (current->pos.y < -40 || current->pos.y > HEIGHT + 40){
                    current->mode = stopped;
                }
                else if (current->pos.x < -40 || current->pos.x > WIDTH + 40){
                    current->mode = stopped;
                }
                current = current->next;
            }
            draw_count++;
        }
    }
    else if (event.timer.source == drawingTimer){
        draw_bullets(player_bullet_list);
        draw_bullets(enemy_bullet_list);
        for (Character *current = enemy_list; current != NULL; current = current->next){
            al_draw_rotated_bitmap(current->image, current->body.center.x, current->body.center.y,
                                   current->pos.x, current->pos.y,
                                   (float) (ALLEGRO_PI / 180 * current->dire_angle), 0);
//            al_draw_circle(current->pos.x, current->pos.y, current->body.radius, white, 0);
        }
        if (player.health > 0){
            al_draw_bitmap(player.image, player.pos.x, player.pos.y, 0);
//            al_draw_circle(player.pos.x + player.body.center.x, player.pos.y + player.body.center.y,
//                           player.body.radius, white, 0);
            if (player.show_shield){
                al_draw_circle(player.pos.x + player.body.center.x, player.pos.y + player.body.center.y,
                               player.body.radius + 20, al_map_rgba(0, 119, 179, 50), 8);
            }
        }
        if (draw_boss){
            al_draw_rotated_bitmap(boss.image, boss.body.center.x, boss.body.center.y, boss.pos.x, boss.pos.y,
                                   (float) ALLEGRO_PI, 0);
            al_draw_circle(boss.pos.x, boss.pos.y, boss.body.radius, white, 0);
            if (boss.health >= 0){
                al_draw_filled_rectangle(boss.pos.x - 20, boss.pos.y - 60,
                                         boss.pos.x - 20 + 40 * ((float) boss.health / settings[level].boss_hp),
                                         boss.pos.y - 50, al_map_rgb(255, 0, 0));
            }
            al_draw_rectangle(boss.pos.x - 20, boss.pos.y - 60, boss.pos.x + 20, boss.pos.y - 50, white, 0);
        }

        if (warning_appear){
            show_warning();
        }
        if (hint_appear){
            show_hint();
        }
        al_flip_display();
    }
    else if (event.timer.source == playerMovingTimer){
        player.pos.x += player.speed.x;
        player.pos.y += player.speed.y;

        if (player.pos.x < -(player.size.x / 3)){
            player.pos.x = -(player.size.x / 3);
        }
        else if (player.pos.x > WIDTH - player.size.x / 3 * 2){
            player.pos.x = WIDTH - player.size.x / 3 * 2;
        }
        if (player.pos.y < 0){
            player.pos.y = 0;
        }
        else if (player.pos.y > HEIGHT - player.size.y / 3 * 2){
            player.pos.y = HEIGHT - player.size.y / 3 * 2;
        }
    }
    else if (event.timer.source == playerShootingTimer){
        for (int i = 0; player.bullet_mode[i]; i++){
            Bullet *bt = make_bullet(player.default_bullet, player.bullet_mode[i],
                                     (Vector2) {player.pos.x + player.firing_point.x,
                                                player.pos.y + player.firing_point.y},
                                     player.damage, player.bullet_speed);
            register_bullet(bt, &player_bullet_list);
        }
    }
    else if (event.timer.source == collisionDetectTimer){
        for (Bullet *pla_curr = player_bullet_list; pla_curr != NULL; pla_curr = pla_curr->next){
            for (Bullet *ene_curr = enemy_bullet_list; ene_curr != NULL; ene_curr = ene_curr->next){
                if (collide_with((Circle) {(Vector2) {pla_curr->pos.x + pla_curr->hit_area.center.x,
                                                      pla_curr->pos.y + pla_curr->hit_area.center.y},
                                           pla_curr->hit_area.radius},
                                 (Circle) {(Vector2) {ene_curr->pos.x + ene_curr->hit_area.center.x,
                                                      ene_curr->pos.y + ene_curr->hit_area.center.y},
                                           ene_curr->hit_area.radius})){
                    pla_curr->mode = stopped;
                    ene_curr->mode = stopped;
                    if (player.health > 0){
                        score += rand() % 31 + 30;
                    }
                }
            }

            for (Character *enemy = enemy_list; enemy != NULL; enemy = enemy->next){
                if (collide_with((Circle) {(Vector2) {pla_curr->pos.x + pla_curr->hit_area.center.x,
                                                      pla_curr->pos.y + pla_curr->hit_area.center.y},
                                           pla_curr->hit_area.radius},
                                 (Circle) {(Vector2) {enemy->pos.x, enemy->pos.y}, enemy->body.radius})){
                    if (pla_curr->mode != stopped){
                        pla_curr->mode = stopped;
                        enemy->health -= pla_curr->damage;
                        if (player.health > 0){
                            score += rand() % 300 + 300;
                            if (enemy->health <= 0){
                                killed_enemies++;
                                score += rand() % 600 + 1500;
                                if (killed_enemies == settings[level].enemy_count && !draw_boss){
                                    printf("BOSS\n");
                                    al_start_timer(bossComingOutTimer);
                                    al_start_timer(warningTimer);
                                    draw_boss = true;
                                }
                            }
                        }
                    }
                }
            }

            if (collide_with((Circle) {(Vector2) {pla_curr->pos.x + pla_curr->hit_area.center.x,
                                                  pla_curr->pos.y + pla_curr->hit_area.center.y},
                                       pla_curr->hit_area.radius},
                             (Circle) {(Vector2) {boss.pos.x, boss.pos.y}, boss.body.radius}) &&
                al_get_timer_started(bossMovingTimer)){
                boss.health -= pla_curr->damage;
                pla_curr->mode = stopped;
                if (player.health > 0){
                    score += rand() % 300 + 400;
                    if (boss.health <= 0){
                        score += rand() % 2001 + 4000;
                        pass = true;
                        draw_boss = false;
                        al_stop_timer(bossMovingTimer);
                        al_stop_timer(bossShootingTimer);
                        printf("\nLEVEL CLEAR\n");
                        al_stop_timer(generateEnemyTimer);
                        al_stop_timer(enemyShootingTimer);
                        al_stop_timer(generateDroppingBulletTimer);
                        switch (level){
                            case 1:
                                if (!unlock_airplane[1]){
                                    send_message("NEW PLANE", "You got a new airplane!");
                                    unlock_airplane[1] = true;
                                }
                                break;
                            case 2:
                                if (!unlock_airplane[3]){
                                    send_message("NEW PLANE", "You got a new airplane!");
                                    unlock_airplane[3] = true;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
        for (Bullet *ene_curr = enemy_bullet_list; ene_curr != NULL && player.health > 0; ene_curr = ene_curr->next){
            if (collide_with((Circle) {(Vector2) {ene_curr->pos.x + ene_curr->hit_area.center.x,
                                                  ene_curr->pos.y + ene_curr->hit_area.center.y},
                                       ene_curr->hit_area.radius},
                             (Circle) {(Vector2) {player.pos.x + player.body.center.x,
                                                  player.pos.y + player.body.center.y}, player.body.radius})){
                if (ene_curr->mode == stopped || pass){
                    continue;
                }
                if (player.invincible){
                    ene_curr->mode = stopped;
                    continue;
                }
                player.health -= ene_curr->damage;
                ene_curr->mode = stopped;
                printf("HURT\n");
                if (player.health <= 0 && !unlock_airplane[2]){
                    send_message("NEW PLANE", "You got a new airplane!");
                    unlock_airplane[2] = true;
                    printf("DEAD\n");
                    player.damage = 0;
                }
            }
        }
    }
    else if (event.timer.source == generateDroppingBulletTimer){
        Bullet *bt = make_bullet(settings[level].dropping_bullet, down, (Vector2) {-201, 0},
                                 5, settings[level].dropping_bullet_speed);
        register_bullet(bt, &enemy_bullet_list);
    }
    else if (event.timer.source == enemyMovingTimer){
        for (Character *current = enemy_list, *prev = enemy_list; current != NULL;){
            if (current != enemy_list && prev->next != current){
                prev = prev->next;
            }
            if (!game_pause){
                current->pos.x += current->speed.x;
                current->pos.y += current->speed.y;
            }

            // TODO: change the enemy's direction if necessary

            /** if (current->health == 7){
                current->dire_angle = 22.5;
                current->speed.x = current->e_speed * sinf((float) (ALLEGRO_PI / 180 * current->dire_angle));
                current->speed.y = current->e_speed * -cosf((float) (ALLEGRO_PI / 180 * current->dire_angle));
                enum flyMode f[3] = {rf_f, up, right_front};
                for (int i = 0; i < 3; i++){
                    current->bullet_mode[i] = f[i];
                }
                for (int i = 3; i < 20; i++){
                    current->bullet_mode[i] = stopped;
                }
            }*/

            if (current->health <= 0 || current->pos.y < -100 || current->pos.y > HEIGHT + 100 ||
                current->pos.x < -50 || current->pos.x > WIDTH + 50){
                // printf("one enemy stopped: %f\n", current->pos.y);
                destroy_enemy(current, &prev);
                current = prev;
                if (current == NULL){
                    current = enemy_list;
                    prev = enemy_list;
                    continue;
                }
            }

            current = current->next;
        }
    }
    else if (event.timer.source == enemyShootingTimer){
        for (Character *enemy = enemy_list; enemy != NULL; enemy = enemy->next){
            enemy->shoot_interval++;
            if (enemy->shoot_interval == enemy->shoot_CD){
                enemy->shoot_interval = 0;
                for (int i = 0; enemy->bullet_mode[i]; i++){
                    Bullet *bt = make_bullet(enemy->default_bullet, enemy->bullet_mode[i], enemy->pos,
                                             enemy->damage, enemy->bullet_speed);
                    register_bullet(bt, &enemy_bullet_list);

                }
            }
        }
    }
    else if (event.timer.source == generateEnemyTimer && !game_pause){
        static int tick = 0;
        tick++;
        bool flag = true;
        for (int i = 0; i < settings[level].kind_of_enemy; i++){
            if (tick % settings[level].generate_enemy_cd[i] == 0 && tick != 0){
                Character *ene = create_enemy(settings[level].enemy_prefab[i]);
                register_enemy(ene);
            }
            else if (tick % settings[level].generate_enemy_cd[i] != 0){
                flag = false;
            }
        }
        if (flag){
            tick = 0;
        }
    }
    else if (event.timer.source == bossComingOutTimer){
        if (!game_pause){
            boss.pos.x += boss.speed.x;
            boss.pos.y += boss.speed.y;
        }

        if (boss.pos.y >= 90){
            boss.e_speed = 2;
            boss.speed = (Vector2) {boss.e_speed * (rand() % 2 ? 1 : -1), (level >= 3 ? 0.3f : 0)};
            al_stop_timer(bossComingOutTimer);
            al_start_timer(bossMovingTimer);
            al_start_timer(bossShootingTimer);
        }
    }
    else if (event.timer.source == bossMovingTimer){
        if (!game_pause){
            boss.pos.x += boss.speed.x;
            boss.pos.y += boss.speed.y;
        }

        if (boss.pos.x < boss.size.x / 6 || boss.pos.x > WIDTH - boss.size.x / 6){
            boss.speed.x *= -1;
        }
        if (boss.pos.y < 20 || boss.pos.y > 120){
            boss.speed.y *= -1;
        }

        draw_count++;
    }
    else if (event.timer.source == bossShootingTimer){
        for (int i = 0; boss.bullet_mode[i]; i++){
            Bullet *bt = make_bullet(boss.default_bullet, boss.bullet_mode[i], boss.pos,
                                     boss.damage, boss.bullet_speed);
            register_bullet(bt, &enemy_bullet_list);
        }
        if (level == 1 && boss.health < 0.8 * settings[level].boss_hp && !hintOut[Q]){
            hintOut[Q] = true;
            player.can[Q] = true;
            player.show[Q] = true;
            strncpy(hint, "Press 'Q' to use skill.", 47);
            send_message("USING SKILL", "From now on, you can\npress Q to use a skill.");
            al_start_timer(hintTimer);
        }
        else if (level == 2 && boss.health < 0.8 * settings[level].boss_hp && !hintOut[W]){
            hintOut[W] = true;
            player.can[W] = true;
            player.show[W] = true;
            strncpy(hint, "Press 'W' to use skill.", 47);
            send_message("SKILL W", "You can press W to use a skill.");
            al_start_timer(hintTimer);
        }
        if (level >= 1 && boss.skills[1] && boss.health < 0.5 * settings[level].boss_hp){
            launch_big_fire(boss.pos, down, &enemy_bullet_list, 5, 1);
            printf("launch 1");
            boss.skills[1] = false;
        }
        if (level >= 2 && boss.skills[2] && boss.health < 0.3 * settings[level].boss_hp){
            launch_big_fire(boss.pos, down, &enemy_bullet_list, 5, 1);
            printf("launch 2");
            boss.skills[2] = false;
        }
        if (level >= 3 && boss.skills[3] && boss.health < 0.8 * settings[level].boss_hp){
            launch_big_fire(boss.pos, down, &enemy_bullet_list, 5, 1);
            printf("launch 3");
            boss.skills[3] = false;
        }
        if (level >= 4 && boss.skills[4] && boss.health < 0.7 * settings[level].boss_hp){
            launch_big_fire(boss.pos, down, &enemy_bullet_list, 5, 1);
            printf("launch 4");
            boss.skills[4] = false;
        }
        if (level >= 5 && boss.skills[5] && boss.health < 0.2 * settings[level].boss_hp){
            launch_big_fire(boss.pos, down, &enemy_bullet_list, 5, 1);
            printf("launch 5");
            boss.skills[5] = false;
        }
    }
    else if (event.timer.source == warningTimer){
        static int times = 0;
        warning_appear = !warning_appear;
        times++;
        if (times == 6){
            times = 0;
            al_stop_timer(warningTimer);
        }
    }
    else if (event.timer.source == hintTimer){
        static int times = 0;
        hint_appear = !hint_appear;
        times++;
        if (times == 6){
            times = 0;
            al_stop_timer(hintTimer);
        }
    }
    else if (event.timer.source == defendTimer){
        static int times = 0;
        al_set_timer_speed(defendTimer, 0.5);
        times++;
        player.show_shield = !player.show_shield;
        if (times == 5){
            player.invincible = false;
            times = 0;
            al_stop_timer(defendTimer);
            al_set_timer_speed(defendTimer, 3.0);
        }
    }
    else if (event.timer.source == skillQTimer){
        sparkle_times[Q]++;
        if (sparkle_times[Q] == 1){
            player.can[Q] = true;
            player.show[Q] = true;
            al_set_timer_speed(skillQTimer, 0.5);
        }
        else if (sparkle_times[Q] == 6){
            sparkle_times[Q] = 0;
            al_set_timer_speed(skillQTimer, player.skill_CD[Q]);
            al_stop_timer(skillQTimer);
        }
        else {
            player.show[Q] = !player.show[Q];
        }
    }
    else if (event.timer.source == skillWTimer){
        sparkle_times[W]++;
        if (sparkle_times[W] == 1){
            player.can[W] = true;
            player.show[W] = true;
            al_set_timer_speed(skillWTimer, 0.5);
        }
        else if (sparkle_times[W] == 6){
            sparkle_times[W] = 0;
            al_set_timer_speed(skillWTimer, player.skill_CD[W]);
            al_stop_timer(skillWTimer);
        }
        else {
            player.show[W] = !player.show[W];
        }

    }


    static bool normal_fireworks = true;

    if (event.type == ALLEGRO_EVENT_KEY_UP){
        if (window == stage_choosing && event.keyboard.keycode == ALLEGRO_KEY_ENTER){
            enter_game_window = true;
        }
        if (window == main_game){
            switch (event.keyboard.keycode){
                // Control
                case ALLEGRO_KEY_UP:
                    player.speed.y -= -10;
                    break;
                case ALLEGRO_KEY_DOWN:
                    player.speed.y -= 10;
                    break;
                case ALLEGRO_KEY_LEFT:
                    player.speed.x -= -10;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    player.speed.x -= 10;
                    break;
                default:
                    break;
            }
        }
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN){
        if (window == main_game){
            switch (event.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    player.speed.y += -10;
                    break;
                case ALLEGRO_KEY_DOWN:
                    player.speed.y += 10;
                    break;
                case ALLEGRO_KEY_LEFT:
                    player.speed.x += -10;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    player.speed.x += 10;
                    break;
                case ALLEGRO_KEY_Q:
                    if (player.can[Q] && !game_pause){
                        player.skill[Q]((Vector2) {player.pos.x + player.firing_point.x,
                                                   player.pos.y + player.firing_point.y},
                                        up, &player_bullet_list, player.damage, player.bullet_speed);
                        player.can[Q] = false;
                        player.show[Q] = false;
                        sparkle_times[Q] = 0;
                        al_stop_timer(skillQTimer);
                        al_set_timer_speed(skillQTimer, player.skill_CD[Q]);
                        al_start_timer(skillQTimer);
                    }
                    break;
                case ALLEGRO_KEY_W:
                    if (player.can[W] && !game_pause){

                        player.skill[W]((Vector2) {player.pos.x + player.firing_point.x,
                                                   player.pos.y + player.firing_point.y},
                                        up, &player_bullet_list, player.damage, player.bullet_speed);
                        player.can[W] = false;
                        player.show[W] = false;
                        sparkle_times[W] = 0;
                        al_stop_timer(skillWTimer);
                        al_set_timer_speed(skillWTimer, player.skill_CD[W]);
                        al_start_timer(skillWTimer);
                    }
                    break;
                default:
                    break;
            }
        }
        if (window == special){
            switch (event.keyboard.keycode){
                case ALLEGRO_KEY_F:
                    printf("F\n");
                    draw_H((Vector2) {48, 200}, false, 3);
                    draw_A((Vector2) {138, 200}, false, 3);
                    draw_P2((Vector2) {238, 200}, false, 3);
                    draw_Y((Vector2) {348, 200}, false, 3);
                    al_start_timer(happyTimer);
                    al_start_timer(the2019Timer);
                    normal_fireworks = false;
                    al_stop_timer(generateFirework1Timer);
                    break;
                default:
                    break;
            }
        }
    }

    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1){
        printf("press ");
        if (window == menu){
            if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 150, 225, WIDTH / 2 + 150, 285)){
                draw_stage_choosing();
                window = stage_choosing;
                printf("START");
            }

            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 120, 340, WIDTH / 2 + 120, 380)){
                window = about;
                draw_about();
                printf("ABOUT");
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 120, 410, WIDTH / 2 + 120, 450)){
                printf("EXIT\n");
                return GAME_TERMINATE;
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH - 60, HEIGHT - 48, WIDTH - 8, HEIGHT - 4)){
                printf("MESSAGE");
                if (drawingTimer != NULL){
                    al_stop_timer(drawingTimer);
                }
                draw_message_box();
                window = message_box;
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH - 118, HEIGHT - 50, WIDTH - 72.5f, HEIGHT - 5)){
                printf("OPTIONS");
                if (drawingTimer != NULL){
                    al_stop_timer(drawingTimer);
                }
                draw_options();
                window = options;
            }
        }
        else if (window == main_game){
            if (!game_pause && !pass && player.health > 0){
                if (!game_pause && within(event.mouse.x, event.mouse.y, 1, HEIGHT - 55, 100, HEIGHT)){
                    al_start_timer(warningTimer);
                    printf("Fire 1");
                }
                else if (!game_pause && within(event.mouse.x, event.mouse.y, 101, HEIGHT - 55, 200, HEIGHT)){
                    printf("Fire 2");
                }
                else if (!game_pause && within(event.mouse.x, event.mouse.y, 201, HEIGHT - 55, 300, HEIGHT)){
                    printf("FIRE3");
                }
                else if (!game_pause && within(event.mouse.x, event.mouse.y, 301, HEIGHT - 55, 400, HEIGHT)){
                    printf("Fire4");
                }
            }
            if (player.health > 0 && !pass && within(event.mouse.x, event.mouse.y, 10, 10, 31, 30)){
                printf("Pause / Resume");
                if (!game_pause){
                    al_stop_timer(playerShootingTimer);
                    al_stop_timer(enemyShootingTimer);
                    al_stop_timer(playerMovingTimer);
                    al_stop_timer(collisionDetectTimer);
                    al_stop_timer(generateDroppingBulletTimer);
                    al_stop_timer(bossShootingTimer);
                }
                else {
                    al_start_timer(playerShootingTimer);
                    al_start_timer(enemyShootingTimer);
                    al_start_timer(playerMovingTimer);
                    al_start_timer(collisionDetectTimer);
                    al_start_timer(generateDroppingBulletTimer);
                    if (al_get_timer_started(bossMovingTimer)){
                        al_start_timer(bossShootingTimer);
                    }
                }
                game_pause = !game_pause;
            }
            else if (player.health <= 0 &&
                     within(event.mouse.x, event.mouse.y, WIDTH / 2 - 90, HEIGHT / 2 + 60,
                            WIDTH / 2 + 150, HEIGHT / 2 + 100)){
                printf("RESTART\n");
                reset_level();
            }
            else if (pass && within(event.mouse.x, event.mouse.y, WIDTH / 2 - 90,
                                    HEIGHT / 2 + 60, WIDTH / 2 + 150, HEIGHT / 2 + 100)){
                printf("NEXT\n");
                next_level();
            }
            else if ((player.health <= 0 || pass || game_pause) && within(event.mouse.x, event.mouse.y,
                                                                          WIDTH - 60, HEIGHT - 48, WIDTH - 8,
                                                                          HEIGHT - 4)){
                printf("MESSAGE");
                al_stop_timer(drawingTimer);
                draw_message_box();
                window = message_box;
            }
            else if ((player.health <= 0 || pass || game_pause) && within(event.mouse.x, event.mouse.y,
                                                                          WIDTH - 118, HEIGHT - 50, WIDTH - 72.5f,
                                                                          HEIGHT - 5)){
                printf("OPTIONS");
                al_stop_timer(drawingTimer);
                draw_options();
                window = options;
            }
        }
        else if (window == about){
            if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 120, 440, WIDTH / 2 + 120, 480)){
                window = menu;
                draw_menu();
                printf("BACK");
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 120, 360, WIDTH / 2 + 120, 400)){
                window = special;
                al_clear_to_color(al_map_rgb(10, 10, 20));
                al_flip_display();
                printf("SPECIAL");
                bulletUpdateTimer = al_create_timer(1.0 / 30.0);
                fireworksTickTimer = al_create_timer(1.0 / 30.0);
                generateFirework1Timer = al_create_timer(1.0 / 3.0);
                happyTimer = al_create_timer(1.0 / 30.0);
                the2019Timer = al_create_timer(1.0 / 30.0);
                al_register_event_source(event_queue, al_get_timer_event_source(bulletUpdateTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(fireworksTickTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(generateFirework1Timer));
                al_register_event_source(event_queue, al_get_timer_event_source(happyTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(the2019Timer));
                al_start_timer(bulletUpdateTimer);
                al_start_timer(fireworksTickTimer);
                al_start_timer(generateFirework1Timer);
            }
        }
        else if (window == stage_choosing){
            if (within(event.mouse.x, event.mouse.y, 60, 90, 190, 180)){
                level = 1;
                printf("1");
                enter_game_window = true;
            }
            else if (within(event.mouse.x, event.mouse.y, 210, 90, 340, 180)){
                level = 2;
                hintOut[Q] = true;
                enter_game_window = true;
                printf("2");
            }
            else if (within(event.mouse.x, event.mouse.y, 60, 200, 190, 290)){
                level = 3;
                hintOut[Q] = true;
                hintOut[W] = true;
                enter_game_window = true;
                printf("3");
            }
            else if (within(event.mouse.x, event.mouse.y, 210, 200, 340, 290)){
                printf("4");
            }
            else if (within(event.mouse.x, event.mouse.y, 60, 310, 340, 400)){
                printf("5");
            }
            else if (within(event.mouse.x, event.mouse.y, 60, 420, 340, 510)){
                printf("ENDLESS");
            }
        }
        else if (window == message_box){
            if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 120, 520, WIDTH / 2 + 120, 560)){
                if (reading){
                    printf("BACK");
                    reading = 0;
                    draw_message_box();
                }
                else {
                    printf("BACK");
                    if (bulletUpdateTimer == NULL || !al_get_timer_started(bulletUpdateTimer)){
                        window = menu;
                        draw_menu();
                    }
                    else {
                        window = main_game;
                        al_start_timer(drawingTimer);
                    }
                }
            }
            else {
                for (int i = 1; i <= message_count; i++){
                    if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 150, 90 * i - 30,
                               WIDTH / 2 + 150, 90 * i + 30)){
                        printf("Message %d", i);
                        reading = i;
                        draw_message_box();
                    }
                }
            }
        }
        else if (window == options){
            if (within(event.mouse.x, event.mouse.y, 205, 175, 235, 205)){
                printf("Music ON");
                musicOn = true;
                draw_options();
            }
            else if (within(event.mouse.x, event.mouse.y, 325, 175, 355, 205)){
                printf("Music OFF");
                musicOn = false;
                draw_options();
            }
            else if (within(event.mouse.x, event.mouse.y, 205, 255, 235, 285)){
                printf("Sound ON");
                soundOn = true;
                draw_options();
            }
            else if (within(event.mouse.x, event.mouse.y, 325, 255, 355, 285)){
                printf("Sound OFF");
                soundOn = false;
                draw_options();
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 150, 340, WIDTH / 2 + 150, 380)){
                printf("Choose Character");
                draw_character_choosing();
                window = airplane_choosing;
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 120, 480, WIDTH / 2 + 120, 520)){
                printf("BACK");
                if (bulletUpdateTimer == NULL || !al_get_timer_started(bulletUpdateTimer)){
                    window = menu;
                    draw_menu();
                }
                else {
                    window = main_game;
                    al_start_timer(drawingTimer);
                }
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 120, 420, WIDTH / 2 + 120, 460)){
                printf("MENU");
                if (drawingTimer == NULL){
                    window = menu;
                    draw_menu();
                }
                else {
                    window = menu;
                    printf("\nRETURN");
                    reset_level();
                    al_stop_timer(playerMovingTimer);
                    al_stop_timer(enemyMovingTimer);
                    al_stop_timer(bulletUpdateTimer);
                    al_stop_timer(playerShootingTimer);
                    al_stop_timer(enemyShootingTimer);
                    al_stop_timer(collisionDetectTimer);
                    al_stop_timer(generateDroppingBulletTimer);
                    al_stop_timer(generateEnemyTimer);
                    al_stop_timer(bossComingOutTimer);
                    al_stop_timer(bossMovingTimer);
                    al_stop_timer(bossShootingTimer);
                    enter_game_window = false;
                    game_pause = false;
                    al_stop_samples();
                    al_play_sample(menuMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                    draw_menu();
                }
            }
        }
        else if (window == airplane_choosing){
            if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 120, 510, WIDTH / 2 + 120, 550)){
                printf("BACK");
                window = options;
                draw_options();
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 150, 60, WIDTH / 2 + 150, 170)){
                printf("Choose 1");
                if (unlock_airplane[airplanePage * 3]){
                    chosenPlayer = airplanePage * 3;
                    draw_character_choosing();
                }
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 150, 210, WIDTH / 2 + 150, 320)){
                printf("Choose 2");
                if (unlock_airplane[airplanePage * 3 + 1]){
                    chosenPlayer = airplanePage * 3 + 1;
                    draw_character_choosing();
                }
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 150, 360, WIDTH / 2 + 150, 470)){
                printf("Choose 3");
                if (unlock_airplane[airplanePage * 3 + 2]){
                    chosenPlayer = airplanePage * 3 + 2;
                    draw_character_choosing();
                }
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 + 170, HEIGHT / 2,
                            WIDTH / 2 + 190, HEIGHT / 2 + 30)){
                printf("NEXT PG");
                if (airplanePage != 2){
                    airplanePage++;
                }
                draw_character_choosing();
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 190, HEIGHT / 2,
                            WIDTH / 2 - 170, HEIGHT / 2 + 30)){
                printf("PREV PG");
                if (airplanePage != 0){
                    airplanePage--;
                }
                draw_character_choosing();
            }
        }
        printf("\n");
    }

    if (window == special){
        static bool finish = false;
        if (event.timer.source == fireworksTickTimer){
            static Vector2 tmp;
            static int size;
            static int tick = 0, tick_saved = -100;

            if (finish && fireworks_bullet_list == NULL){
                al_draw_text(smallFont, white, WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "REPLAY?");
                al_draw_rectangle(WIDTH / 2 - 120, HEIGHT / 2 - 10, WIDTH / 2 + 120, HEIGHT / 2 + 30, white, 0);
                al_draw_text(smallFont, white, WIDTH / 2 + 60, HEIGHT / 2 + 55, ALLEGRO_ALIGN_CENTER, "BACK");
                al_draw_rectangle(WIDTH / 2, HEIGHT / 2 + 50, WIDTH / 2 + 120, HEIGHT / 2 + 80, white, 0);
                al_flip_display();
            }
            for (Bullet *curr = fireworks_bullet_list; curr != NULL; curr = curr->next){
                curr->time--;
                if (curr->time == 0){
                    curr->mode = stopped;
                }
                if (curr->time == 249){
                    curr->pause = false;
                }
                if (curr->time == 15 && curr->pause){
                    curr->pause = false;
                }
            }
            if (!finish){
                tick++;
            }
            else {
                tick = 0;
                tick_saved = -100;
            }
            if (normal_fireworks){
                if (tick % 65 == 0 && tick != 0){
                    tmp = (Vector2) {rand() % 280 + 60, rand() % 250 + 350};
                    tick_saved = 0;
                    tick = 0;
                    size = rand() % 5 + 16;
                    Bullet *bt = make_firework_bullet(bulletImgs[3], up, tmp, 15, 2, NULL);
                    register_bullet(bt, &fireworks_bullet_list);
                    tmp.y -= 240;
                }
                if (tick == tick_saved + 15){
                    al_play_sample(letoff, 0.8, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    draw_fireworks1(size, tmp);
                }
                if (tick == tick_saved + 20){
                    draw_fireworks1(size, tmp);
                }
                if (tick == tick_saved + 25){
                    draw_fireworks2(size, tmp);
                }
            }
        }
        else if (event.timer.source == generateFirework1Timer){
            al_play_sample(letoff, (float) (1.0 / (rand() % 4 + 2)), 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            draw_fireworks1(rand() % 4 + 16, (Vector2) {rand() % 240 + 80, rand() % 400 + 200});
            al_set_timer_speed(generateFirework1Timer, 1.0 / (rand() % 3 + 1));
        }
        else if (event.timer.source == happyTimer){
            al_play_sample(letoff, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            al_play_sample(letoff2, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            draw_H((Vector2) {48, 200}, true, 1);
            draw_A((Vector2) {138, 200}, true, 2);
            draw_P2((Vector2) {238, 200}, true, 2);
            draw_Y((Vector2) {348, 200}, true, 1);
            draw_H((Vector2) {48, 200}, true, 3);
            draw_A((Vector2) {138, 200}, true, 3);
            draw_P2((Vector2) {238, 200}, true, 3);
            draw_Y((Vector2) {348, 200}, true, 3);
            al_stop_timer(happyTimer);
        }
        else if (event.timer.source == the2019Timer){
            static int tick = 0;
            tick++;
            if (tick == 20){
                draw_2((Vector2) {55, 450}, true, 3);
            }
            else if (tick == 30){
                draw_0((Vector2) {155, 450}, true, 3);
            }
            else if (tick == 40){
                draw_1((Vector2) {255, 450}, true, 3);
            }
            else if (tick == 50){
                draw_9((Vector2) {340, 450}, true, 3);
            }
            if (tick == 105){
                al_play_sample_instance(letoffInstance[0]);
            }
            else if (tick == 115){
                al_play_sample_instance(letoffInstance[1]);
            }
            else if (tick == 125){
                al_play_sample_instance(letoffInstance[0]);
            }
            else if (tick == 135){
                al_play_sample_instance(letoffInstance[1]);
            }
            else if (tick == 150){
                al_play_sample(letoff, 1.2, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                al_play_sample(letoff2, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                tick = 0;
                finish = true;
                al_stop_timer(the2019Timer);
            }
        }

        if (finish && event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            if (within(event.mouse.x, event.mouse.y,
                       WIDTH / 2 - 120, HEIGHT / 2 - 10, WIDTH / 2 + 120, HEIGHT / 2 + 30)){
                finish = false;
                normal_fireworks = true;
                al_clear_to_color(al_map_rgb(10, 10, 20));
                al_flip_display();
                al_start_timer(generateFirework1Timer);
            }
            if (within(event.mouse.x, event.mouse.y,
                       WIDTH / 2, HEIGHT / 2 + 50, WIDTH / 2 + 120, HEIGHT / 2 + 80)){
                window = about;
                al_stop_timer(bulletUpdateTimer);
                al_stop_timer(fireworksTickTimer);
                finish = false;
                normal_fireworks = true;
                draw_about();
                al_flip_display();
            }
        }
    }

    // Shutdown our program
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
        printf("byebye");
        return GAME_TERMINATE;
    }

    return 0;
}

int game_run() {
    int error = 0;
    if (window == menu){
        if (!al_is_event_queue_empty(event_queue)){
            error = process_event();
        }
    }
    else if (window == main_game){
        draw_game_scene();

        if (!al_is_event_queue_empty(event_queue)){
            error = process_event();
        }
    }
    else if (window == about){
        if (!al_is_event_queue_empty(event_queue)){
            error = process_event();
        }
    }
    else if (window == special){
        if (draw_count == 0){
            al_clear_to_color(al_map_rgb(10, 10, 20));
            al_draw_rectangle(0, 0, 0, 0, al_map_rgb(10, 10, 20), 0);
        }
        if (draw_count >= 1){
            draw_count = 0;
            al_flip_display();
        }

        if (!al_is_event_queue_empty(event_queue)){
            error = process_event();
        }
    }
    else if (window == stage_choosing){
        if (!al_is_event_queue_empty(event_queue)){
            error = process_event();
            if (enter_game_window){
                window = 2;
                // Setting Character
                player.pos.x = 200;
                player.pos.y = HEIGHT - 100;
                set_player(&playerPrefabs[chosenPlayer]);

                set_boss(settings[level].boss_img, settings[level].boss_hp, settings[level].boss_damage,
                         settings[level].boss_bullet, settings[level].boss_rate, settings[level].boss_bt_speed,
                         settings[level].boss_modes, settings[level].count_of_modes);
                for (int i = 1; i <= 5; i++){
                    if (level >= i){
                        boss.skills[i] = true;
                    }
                    else {
                        boss.skills[i] = false;
                    }
                }

                // Initialize Timer
                playerMovingTimer = al_create_timer(1.0 / 30.0);
                enemyMovingTimer = al_create_timer(1.0 / 30.0);
                bulletUpdateTimer = al_create_timer(1.0 / 30.0);
                drawingTimer = al_create_timer(1.0 / 60.0);
                playerShootingTimer = al_create_timer(player.shooting_rate);
                enemyShootingTimer = al_create_timer(1.0 / 60.0);
                collisionDetectTimer = al_create_timer(1.0 / 60.0);
                generateDroppingBulletTimer = al_create_timer(settings[level].dropping_rate);
                generateEnemyTimer = al_create_timer(1.0 / 30.0);
                bossComingOutTimer = al_create_timer(1.0 / 30.0);
                bossMovingTimer = al_create_timer(1.0 / 30.0);
                bossShootingTimer = al_create_timer(boss.shooting_rate);
                warningTimer = al_create_timer(0.5);
                hintTimer = al_create_timer(0.8);
                skillQTimer = al_create_timer(player.skill_CD[Q]);
                skillWTimer = al_create_timer(player.skill_CD[W]);
                defendTimer = al_create_timer(3.0);
                al_register_event_source(event_queue, al_get_timer_event_source(playerMovingTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(enemyMovingTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(bulletUpdateTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(drawingTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(playerShootingTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(enemyShootingTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(collisionDetectTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(generateDroppingBulletTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(generateEnemyTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(bossComingOutTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(bossMovingTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(bossShootingTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(warningTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(hintTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(skillQTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(skillWTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(defendTimer));
                al_start_timer(playerMovingTimer);
                al_start_timer(enemyMovingTimer);
                al_start_timer(bulletUpdateTimer);
                al_start_timer(drawingTimer);
                al_start_timer(playerShootingTimer);
                al_start_timer(enemyShootingTimer);
                al_start_timer(collisionDetectTimer);
                al_start_timer(generateDroppingBulletTimer);
                al_start_timer(generateEnemyTimer);

                al_stop_samples();
                al_play_sample(gameMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
            }
        }
    }
    else if (window == message_box){
        if (!al_is_event_queue_empty(event_queue)){
            error = process_event();
        }
    }
    else if (window == options){
        if (!al_is_event_queue_empty(event_queue)){
            error = process_event();
        }
    }
    else if (window == airplane_choosing){
        if (!al_is_event_queue_empty(event_queue)){
            error = process_event();
        }
    }
    return error;
}

void game_destroy() {
    // TODO: Make sure you destroy all things
    al_destroy_event_queue(event_queue);
    al_destroy_font(bigFont);
    al_destroy_font(smallFont);
    al_destroy_bitmap(cursorImg);
    for (int i = 0; i < 2; i++){
        al_destroy_bitmap(bulletImgs[i]);
    }
    for (int i = 0; i < 1; i++){
        al_destroy_bitmap(airplaneImgs[i]);
    }
    al_destroy_timer(bulletUpdateTimer);
    al_destroy_timer(playerMovingTimer);
    al_destroy_timer(playerShootingTimer);
    al_destroy_timer(collisionDetectTimer);
    al_destroy_sample(menuMusic);
    al_destroy_mouse_cursor(cursor);
    al_destroy_display(display);
}


bool within(float x, float y, float x1, float y1, float x2, float y2) {
    if (y >= y1 && y <= y2 && x >= x1 && x <= x2){
        return true;
    }
    else {
        return false;
    }
}

bool collide_with(Circle a, Circle b) {
    if (sqrtf(powf(b.center.x - a.center.x, 2) + powf(b.center.y - a.center.y, 2)) <= a.radius + b.radius){
        return true;
    }
    return false;
}

void draw_menu() {
    al_clear_to_color(al_map_rgb(70, 70, 80));
    // draw button
    al_draw_text(bigFont, al_map_rgb(255, 255, 200), WIDTH / 2, HEIGHT / 2 - 60, ALLEGRO_ALIGN_CENTRE, "S T A R T");
    al_draw_rectangle(WIDTH / 2 - 150, 225, WIDTH / 2 + 150, 285, al_map_rgb(255, 255, 200), 0);
    al_draw_text(smallFont, white, WIDTH / 2, HEIGHT / 2 + 50, ALLEGRO_ALIGN_CENTRE, "ABOUT");
    al_draw_rectangle(WIDTH / 2 - 120, 340, WIDTH / 2 + 120, 380, white, 0);
    al_draw_text(smallFont, al_map_rgb(255, 100, 100), WIDTH / 2, HEIGHT / 2 + 120, ALLEGRO_ALIGN_CENTRE, "EXIT");
    al_draw_rectangle(WIDTH / 2 - 120, 410, WIDTH / 2 + 120, 450, al_map_rgb(255, 100, 100), 0);
    // al_draw_rectangle(WIDTH - 118, HEIGHT - 50, WIDTH - 72.5f, HEIGHT - 5, al_map_rgb(255, 100, 100), 0);
    al_draw_bitmap(messageImg, WIDTH - 64, HEIGHT - 56, 0);
    al_draw_bitmap(optionsImg, WIDTH - 128, HEIGHT - 60, 0);

    al_flip_display();
}

void draw_about() {
    al_clear_to_color(al_map_rgb(60, 60, 30));
    al_draw_text(smallFont, white, WIDTH / 2, 450, ALLEGRO_ALIGN_CENTER, "BACK");
    al_draw_rectangle(WIDTH / 2 - 120, 440, WIDTH / 2 + 120, 480, white, 0);
    al_draw_text(smallFont, white, WIDTH / 2, 370, ALLEGRO_ALIGN_CENTER, "SPECIAL");
    al_draw_rectangle(WIDTH / 2 - 120, 360, WIDTH / 2 + 120, 400, white, 0);
    al_flip_display();
}

void draw_game_scene() {
    al_clear_to_color(al_map_rgb(50, 50, 80));

    char score_text[10];
    sprintf(score_text, "%08d", score);
    al_draw_text(bigFont, white, WIDTH - 5, 5, ALLEGRO_ALIGN_RIGHT, score_text);
    if (player.health > 0){
        double ratio = (double) player.health / playerPrefabs[chosenPlayer].hp;
        ALLEGRO_COLOR hp_bar = al_map_rgb((unsigned char) (150 + 50 * (1 - ratio)),
                                          (unsigned char) (50 + 150 * ratio), 0);
        al_draw_filled_rectangle(10, HEIGHT - 40, (float) (10 + 150 * ratio), HEIGHT - 10, hp_bar);
        al_draw_rectangle(10, HEIGHT - 40, 160, HEIGHT - 10, white, 0);
        if (player.show[Q]){
            al_draw_text(bigFont, white, 180, HEIGHT - 44, ALLEGRO_ALIGN_LEFT, "Q");
        }
        if (player.show[W]){
            al_draw_text(bigFont, white, 210, HEIGHT - 44, ALLEGRO_ALIGN_LEFT, "W");
        }
        if (pass){
            al_draw_rectangle(WIDTH / 2 - 150, HEIGHT / 2 - 20, WIDTH / 2 + 150, HEIGHT / 2 + 50, white, 0);
            al_draw_text(bigFont, white, WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "LEVEL CLEAR");
            al_draw_rectangle(WIDTH / 2 - 90, HEIGHT / 2 + 60, WIDTH / 2 + 150, HEIGHT / 2 + 100, white, 0);
            al_draw_text(smallFont, white, WIDTH / 2 + 30, HEIGHT / 2 + 70, ALLEGRO_ALIGN_CENTER, "NEXT");
            al_draw_bitmap(messageImg, WIDTH - 64, HEIGHT - 56, 0);
            al_draw_bitmap(optionsImg, WIDTH - 128, HEIGHT - 60, 0);
        }
    }
    else {
        al_draw_rectangle(WIDTH / 2 - 150, HEIGHT / 2 - 20, WIDTH / 2 + 150, HEIGHT / 2 + 50, white, 0);
        al_draw_text(bigFont, white, WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "GAME OVER");
        al_draw_rectangle(WIDTH / 2 - 90, HEIGHT / 2 + 60, WIDTH / 2 + 150, HEIGHT / 2 + 100, white, 0);
        al_draw_text(smallFont, white, WIDTH / 2 + 30, HEIGHT / 2 + 70, ALLEGRO_ALIGN_CENTER, "RESTART");
        al_draw_bitmap(messageImg, WIDTH - 64, HEIGHT - 56, 0);
        al_draw_bitmap(optionsImg, WIDTH - 128, HEIGHT - 60, 0);
    }

    if (!game_pause){
        al_draw_filled_rectangle(10, 10, 18, 30, white);
        al_draw_filled_rectangle(23, 10, 31, 30, white);
    }
    else {
        al_draw_filled_triangle(10, 10, 10, 30, 31, 20, white);
        al_draw_bitmap(messageImg, WIDTH - 64, HEIGHT - 56, 0);
        al_draw_bitmap(optionsImg, WIDTH - 128, HEIGHT - 60, 0);
        //al_draw_rectangle(WIDTH - 60, HEIGHT - 48, WIDTH - 8, HEIGHT - 4, white, 0);
    }

}

void draw_stage_choosing() {
    ALLEGRO_FONT *fnt = al_load_font("pirulen.ttf", 40, 0);
    al_clear_to_color(al_map_rgb(126, 46, 41));
    al_draw_rectangle(60, 90, 190, 180, white, 0);
    al_draw_text(fnt, white, 125, 110, ALLEGRO_ALIGN_CENTER, "1");
    al_draw_rectangle(210, 90, 340, 180, white, 0);
    al_draw_text(fnt, white, 275, 110, ALLEGRO_ALIGN_CENTER, "2");
    al_draw_rectangle(60, 200, 190, 290, white, 0);
    al_draw_text(fnt, white, 125, 220, ALLEGRO_ALIGN_CENTER, "3");
    al_draw_rectangle(210, 200, 340, 290, white, 0);
    al_draw_text(fnt, white, 275, 220, ALLEGRO_ALIGN_CENTER, "4");
    al_draw_rectangle(60, 310, 340, 400, white, 0);
    al_draw_text(fnt, white, 200, 330, ALLEGRO_ALIGN_CENTER, "5");
    al_draw_rectangle(60, 420, 340, 510, white, 0);
    al_draw_text(bigFont, white, 200, 450, ALLEGRO_ALIGN_CENTER, "ENDLESS");

    al_flip_display();
    al_destroy_font(fnt);
}

void draw_message_box() {
    static int page = 0;

    al_clear_to_color(al_map_rgb(31, 64, 96));

    al_draw_text(smallFont, white, WIDTH / 2, 530, ALLEGRO_ALIGN_CENTER, "BACK");
    al_draw_rectangle(WIDTH / 2 - 120, 520, WIDTH / 2 + 120, 560, white, 0);

    // TODO: finish message system
    if (message_count == 0){
        al_draw_rectangle(WIDTH / 2 - 160, HEIGHT / 2 - 20, WIDTH / 2 + 160, HEIGHT / 2 + 30, white, 0);
        al_draw_text(hintFont, white, WIDTH / 2, HEIGHT / 2 - 10, ALLEGRO_ALIGN_CENTER, "There's no message yet");
    }
    else if (reading == 0){
        for (int i = 1; i <= message_count; i++){
            ALLEGRO_COLOR color = (has_read[page * 5 + i - 1] ? white : al_map_rgb(255, 230, 0));
            al_draw_rectangle(WIDTH / 2 - 150, 90 * i - 30, WIDTH / 2 + 150, 90 * i + 30, white, 0);
            al_draw_text(midFont, color, WIDTH / 2, 90 * i - 12, ALLEGRO_ALIGN_CENTER,
                         messageTitle[page * 5 + i - 1]);
        }
    }
    else {
        al_draw_rectangle(WIDTH / 2 - 160, 60, WIDTH / 2 + 160, 480, white, 0);
        has_read[page * 5 + reading - 1] = true;
        char tmp_content[30];
        strcpy(tmp_content, messageContent[page * 5 + reading - 1]);
        char *line = strtok(tmp_content, "\n");
        int i = 0;
        do {
            al_draw_text(hintFont, white, WIDTH / 2, 75 + 30 * i, ALLEGRO_ALIGN_CENTER, line);
            i++;
        } while ((line = strtok(NULL, "\n")) != NULL);
    }


    al_flip_display();
}

void draw_options() {
    ALLEGRO_COLOR on, off;
    on = al_map_rgb(155, 195, 34);
    off = al_map_rgb(234, 85, 51);

    al_clear_to_color(al_map_rgb(43, 59, 59));
    al_draw_text(midFont, white, 30, 178, ALLEGRO_ALIGN_LEFT, "MUSIC");
    al_draw_text(midFont, white, 155, 178, ALLEGRO_ALIGN_LEFT, "ON");
    al_draw_text(midFont, white, 265, 178, ALLEGRO_ALIGN_LEFT, "OFF");
    al_draw_text(midFont, white, 25, 258, ALLEGRO_ALIGN_LEFT, "SOUND");
    al_draw_text(midFont, white, 155, 258, ALLEGRO_ALIGN_LEFT, "ON");
    al_draw_text(midFont, white, 265, 258, ALLEGRO_ALIGN_LEFT, "OFF");
    al_draw_rectangle(205, 175, 235, 205, white, 3);
    al_draw_rectangle(325, 175, 355, 205, white, 3);
    al_draw_rectangle(205, 255, 235, 285, white, 3);
    al_draw_rectangle(325, 255, 355, 285, white, 3);
    if (musicOn){
        al_draw_filled_rectangle(210, 180, 229, 199, on);
    }
    else {
        al_draw_filled_rectangle(330, 180, 349, 199, off);
    }
    if (soundOn){
        al_draw_filled_rectangle(210, 260, 229, 279, on);
    }
    else {
        al_draw_filled_rectangle(330, 260, 349, 279, off);
    }

    al_draw_text(smallFont, white, WIDTH / 2, 350, ALLEGRO_ALIGN_CENTER, "CHOOSE CHARACTER");
    al_draw_rectangle(WIDTH / 2 - 150, 340, WIDTH / 2 + 150, 380, white, 0);
    al_draw_text(smallFont, white, WIDTH / 2, 430, ALLEGRO_ALIGN_CENTER, "MENU");
    al_draw_rectangle(WIDTH / 2 - 120, 420, WIDTH / 2 + 120, 460, white, 0);
    al_draw_text(smallFont, white, WIDTH / 2, 490, ALLEGRO_ALIGN_CENTER, "BACK");
    al_draw_rectangle(WIDTH / 2 - 120, 480, WIDTH / 2 + 120, 520, white, 0);

    al_flip_display();
}

void draw_character_choosing() {
    change_player(&playerPrefabs[chosenPlayer]);
    if (playerShootingTimer){
        al_set_timer_speed(playerShootingTimer, player.shooting_rate);
    }
    printf("page %d\n", airplanePage);

    ALLEGRO_COLOR bg = al_map_rgb(69, 43, 33);

    al_clear_to_color(al_map_rgb(69, 43, 33));
    al_draw_text(smallFont, white, WIDTH / 2, 520, ALLEGRO_ALIGN_CENTER, "BACK");
    al_draw_rectangle(WIDTH / 2 - 120, 510, WIDTH / 2 + 120, 550, white, 0);

    // TODO: design UI, make player can Choose Character
    for (int i = 0; i < 3 && airplanePage * 3 + i < 6; i++){
        int index = airplanePage * 3 + i;
        ALLEGRO_COLOR fore;
        fore = (index == chosenPlayer ? bg : white);
        if (index == chosenPlayer){
            al_draw_filled_rectangle(WIDTH / 2 - 150, 60 + 150 * i, WIDTH / 2 + 150, 170 + 150 * i,
                                     al_map_rgb(255, 240, 230));
        }
        al_draw_textf(smallFont, fore, WIDTH / 2 - 120, 75 + 150 * i, ALLEGRO_ALIGN_LEFT,
                      "HP %d", playerPrefabs[index].hp);
        al_draw_textf(smallFont, fore, WIDTH / 2 - 120, 95 + 150 * i, ALLEGRO_ALIGN_LEFT,
                      "ATK %d", playerPrefabs[index].damage);
        al_draw_textf(smallFont, fore, WIDTH / 2 - 120, 115 + 150 * i, ALLEGRO_ALIGN_LEFT,
                      "DIR %d", playerPrefabs[index].count_of_mode);
        al_draw_textf(smallFont, fore, WIDTH / 2 - 120, 135 + 150 * i, ALLEGRO_ALIGN_LEFT,
                      "SHT %ld", lround(1 / playerPrefabs[index].shooting_rate));
        float angle = (index != 4 ? (float) (-ALLEGRO_PI / 5) : 0);
        al_draw_rotated_bitmap((unlock_airplane[index] ? airLargeImgs[index] : airGrayImgs[index]),
                               al_get_bitmap_width(airLargeImgs[index]) / 2,
                               al_get_bitmap_height(airLargeImgs[index]) / 2, WIDTH / 2 + 80,
                               105 + 150 * i + (index == 5 ? 20 : 0), angle, 0);
        al_draw_rectangle(WIDTH / 2 - 150, 60 + 150 * i, WIDTH / 2 + 150, 170 + 150 * i, white, 0);
    }

    al_draw_filled_triangle(WIDTH / 2 + 170, HEIGHT / 2, WIDTH / 2 + 170, HEIGHT / 2 + 30, WIDTH / 2 + 190,
                            HEIGHT / 2 + 15, white);
    al_draw_filled_triangle(WIDTH / 2 - 170, HEIGHT / 2, WIDTH / 2 - 170, HEIGHT / 2 + 30, WIDTH / 2 - 190,
                            HEIGHT / 2 + 15, white);


    al_flip_display();
}

void process_bullets(Bullet **list) {
    for (Bullet *current = *list, *previous = *list; current != NULL;){
        if (current != *list && previous->next != current){
            previous = previous->next;
        }
        float mul = current->speed_multiplier;
        switch (current->mode){
            case up:
                if (!game_pause){
                    current->pos.y -= 8 * mul;
                }
                break;
            case right_front:
                if (!game_pause){
                    current->pos.x += 5.7 * mul;
                    current->pos.y -= 5.7 * mul;
                }
                break;
            case left_front:
                if (!game_pause){
                    current->pos.x -= 5.7 * mul;
                    current->pos.y -= 5.7 * mul;
                }
                break;
            case right:
                if (!game_pause){
                    current->pos.x += 8 * mul;
                }
                break;
            case left:
                if (!game_pause){
                    current->pos.x -= 8 * mul;
                }
                break;
            case right_back:
                if (!game_pause){
                    current->pos.x += 5.7 * mul;
                    current->pos.y += 5.7 * mul;
                }
                break;
            case left_back:
                if (!game_pause){
                    current->pos.x -= 5.7 * mul;
                    current->pos.y += 5.7 * mul;
                }
                break;
            case down:
                if (!game_pause){
                    current->pos.y += 8 * mul;
                }
                break;
            case rf_f:
                if (!game_pause){
                    current->pos.x += 3 * mul;
                    current->pos.y -= 7.4 * mul;
                }
                break;
            case lf_f:
                if (!game_pause){
                    current->pos.x -= 3 * mul;
                    current->pos.y -= 7.4 * mul;
                }
                break;
            case rf_r:
                if (!game_pause){
                    current->pos.x += 7.4 * mul;
                    current->pos.y -= 3 * mul;
                }
                break;
            case lf_l:
                if (!game_pause){
                    current->pos.x -= 7.4 * mul;
                    current->pos.y -= 3 * mul;
                }
                break;
            case rb_r:
                if (!game_pause){
                    current->pos.x += 7.4 * mul;
                    current->pos.y += 3 * mul;
                }
                break;
            case lb_l:
                if (!game_pause){
                    current->pos.x -= 7.4 * mul;
                    current->pos.y += 3 * mul;
                }
                break;
            case rb_b:
                if (!game_pause){
                    current->pos.x += 3 * mul;
                    current->pos.y += 7.4 * mul;
                }
                break;
            case lb_b:
                if (!game_pause){
                    current->pos.x -= 3 * mul;
                    current->pos.y += 7.4 * mul;
                }
                break;
            case stopped:
                destroy_bullet(current, &previous, list);
                current = previous;
                break;
            default:
                printf("unknown fly mode\n");
                show_err_msg(1);
                break;
        }
        if (current == NULL){
            if (*list == NULL){
                break;
            }
            current = *list;
            previous = *list;
            continue;
        }

        // set the outside bullet stopped
        if (current->pos.y < -40 || current->pos.y > HEIGHT + 40){
            current->mode = stopped;
        }
        else if (current->pos.x < -40 || current->pos.x > WIDTH + 40){
            current->mode = stopped;
        }
        current = current->next;
    }
}

void draw_bullets(Bullet *list) {
    for (Bullet *current = list; current != NULL; current = current->next){
        switch (current->mode){
            case up:
                al_draw_bitmap(current->bitmap, current->pos.x - current->size.x / 2, current->pos.y,
                               current->flip);
                break;
            case right_front:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (ALLEGRO_PI / 4), current->flip);
                break;
            case left_front:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (-ALLEGRO_PI / 4), current->flip);
                break;
            case right:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (ALLEGRO_PI / 2), current->flip);
                break;
            case left:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (-ALLEGRO_PI / 2), current->flip);
                break;
            case right_back:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (ALLEGRO_PI * 3 / 4), current->flip);
                break;
            case left_back:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (-ALLEGRO_PI * 3 / 4), current->flip);
                break;
            case down:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) ALLEGRO_PI, current->flip);
                break;
            case rf_f:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (ALLEGRO_PI / 180 * 22.5), current->flip);
                break;
            case lf_f:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (-ALLEGRO_PI / 180 * 22.5), current->flip);
                break;
            case rf_r:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (ALLEGRO_PI / 180 * 67.5), current->flip);
                break;
            case lf_l:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (-ALLEGRO_PI / 180 * 67.5), current->flip);
                break;
            case rb_r:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (ALLEGRO_PI / 180 * 112.5), current->flip);
                break;
            case lb_l:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (-ALLEGRO_PI / 180 * 112.5), current->flip);
                break;
            case rb_b:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (ALLEGRO_PI / 180 * 157.5), current->flip);
                break;
            case lb_b:
                al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                       (float) (-ALLEGRO_PI / 180 * 157.5), current->flip);
                break;
            case stopped:
                break;
            default:
                printf("unknown fly mode\n");
                show_err_msg(1);
                break;
        }
//        al_draw_circle(current->pos.x + current->hit_area.center.x,
//                       current->pos.y + current->hit_area.center.y,
//                       current->hit_area.radius, al_map_rgb(255, 220, 188), 0);
    }
}

void reset_level() {
    al_stop_timer(playerMovingTimer);
    al_stop_timer(enemyMovingTimer);
    al_stop_timer(bulletUpdateTimer);
    al_stop_timer(playerShootingTimer);
    al_stop_timer(enemyShootingTimer);
    al_stop_timer(collisionDetectTimer);
    al_stop_timer(generateDroppingBulletTimer);
    al_stop_timer(generateEnemyTimer);
    al_stop_timer(bossComingOutTimer);
    al_stop_timer(bossMovingTimer);
    al_stop_timer(bossShootingTimer);
    player.pos.x = 200;
    player.pos.y = HEIGHT - 100;
    set_player(&playerPrefabs[chosenPlayer]);
    free_bullet_list(&player_bullet_list);
    free_bullet_list(&enemy_bullet_list);
    for (Character *ct = enemy_list; ct != NULL;){
        Character *temp = ct;
        ct = ct->next;
        free(temp);
    }
    enemy_list = NULL;
    score = 0;
    killed_enemies = 0;
    pass = false;
    draw_boss = false;
    set_boss(settings[level].boss_img, settings[level].boss_hp, settings[level].boss_damage, bulletImgs[4],
             1.0 / 3.0, 1, settings[level].boss_modes, settings[level].count_of_modes);
    for (int i = 1; i <= 5; i++){
        if (level >= i){
            boss.skills[i] = true;
        }
        else {
            boss.skills[i] = false;
        }
    }
    al_start_timer(playerMovingTimer);
    al_start_timer(enemyMovingTimer);
    al_start_timer(bulletUpdateTimer);
    al_start_timer(playerShootingTimer);
    al_start_timer(enemyShootingTimer);
    al_start_timer(collisionDetectTimer);
    al_start_timer(generateDroppingBulletTimer);
    al_start_timer(generateEnemyTimer);
}

void show_warning() {
    al_draw_triangle(WIDTH / 2, HEIGHT / 2 - 32, WIDTH / 2 - 27.7f, HEIGHT / 2 + 16,
                     WIDTH / 2 + 27.7f, HEIGHT / 2 + 16, al_map_rgb(230, 40, 0), 3);
    al_draw_text(warningFont, al_map_rgb(230, 40, 0), WIDTH / 2, HEIGHT / 2 - 24, ALLEGRO_ALIGN_CENTER, "!");
}

void show_hint() {
    al_draw_text(hintFont, al_map_rgb(255, 255, 230), WIDTH / 2, HEIGHT / 2 - 12, ALLEGRO_ALIGN_CENTER, hint);
    al_draw_rectangle(WIDTH / 2 - 150, HEIGHT / 2 - 25, WIDTH / 2 + 150, HEIGHT / 2 + 30, al_map_rgb(255, 255, 230), 1);
}

void next_level() {
    free_bullet_list(&player_bullet_list);
    free_bullet_list(&enemy_bullet_list);
    for (Character *ct = enemy_list; ct != NULL;){
        Character *temp = ct;
        ct = ct->next;
        free(temp);
    }

    level++;
    player.pos.x = 200;
    player.pos.y = HEIGHT - 100;
    set_player(&playerPrefabs[chosenPlayer]);
    set_boss(settings[level].boss_img, settings[level].boss_hp, settings[level].boss_damage, bulletImgs[4],
             1.0 / 3.0, 1, settings[level].boss_modes, settings[level].count_of_modes);
    for (int i = 1; i <= 5; i++){
        if (level >= i){
            boss.skills[i] = true;
        }
        else {
            boss.skills[i] = false;
        }
    }

    al_set_timer_speed(generateDroppingBulletTimer, settings[level].dropping_rate);
    al_start_timer(generateDroppingBulletTimer);
    al_start_timer(generateEnemyTimer);
    al_start_timer(enemyShootingTimer);

    // TODO: reset anything about boss
    killed_enemies = 0;

    pass = 0;
}

void send_message(char *title, char *content) {
    strcpy(messageTitle[message_count], title);
    strcpy(messageContent[message_count], content);
    message_count++;
}
