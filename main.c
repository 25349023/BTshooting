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

#define GAME_TERMINATE (-1)

// ALLEGRO Variables
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_BITMAP *cursorImg = NULL;
ALLEGRO_BITMAP *airplaneImgs[20];
ALLEGRO_BITMAP *enemyImgs[20];
ALLEGRO_BITMAP *bulletImgs[20];
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_KEYBOARD_STATE keyState;
ALLEGRO_TIMER *bulletUpdateTimer = NULL;
ALLEGRO_TIMER *shootingDefaultBulletTimer = NULL;
ALLEGRO_TIMER *playerMovingTimer = NULL;
ALLEGRO_TIMER *enemyMovingTimer = NULL;
ALLEGRO_TIMER *collisionDetectTimer = NULL;
ALLEGRO_TIMER *generateEnemyBulletTimer = NULL;
ALLEGRO_SAMPLE *song = NULL;
ALLEGRO_FONT *bigFont = NULL;
ALLEGRO_FONT *smallFont = NULL;
ALLEGRO_MOUSE_CURSOR *cursor = NULL;

ALLEGRO_COLOR white;

//Custom Definition
const char *title = "Final Project 107062303";
const float FPS = 60;
const int WIDTH = 400;
const int HEIGHT = 600;

Bullet *player_bullet_list = NULL;
Bullet *enemy_bullet_list = NULL;


Character player;
Character boss;
Character *enemy_list = NULL;

int window = 1;
int draw_count = 0;
int level;
int enemy_count[5] = {5, 6, 7, 10, 15};

bool enter_game_window = false;
bool appear = true; //true: appear, false: disappear
bool next = false; //true: trigger
bool dir = true; //true: left, false: right

void show_err_msg(int msg);
void game_init();
void game_begin();
int process_event();
int game_run();
void game_destroy();

bool within(float x, float y, float x1, float y1, float x2, float y2);
bool collide_with(Circle a, Circle b);

void load_images();
void draw_menu();
void draw_about();
void draw_game_scene();

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
    for (int i = 0; i < 2; i++){
        sprintf(filename, "bullet%d.png", i);
        bulletImgs[i] = al_load_bitmap(filename);
    }
    for (int i = 0; i < 2; i++){
        sprintf(filename, "airplane%d.png", i);
        airplaneImgs[i] = al_load_bitmap(filename);
    }
    cursorImg = al_load_bitmap("cursor2.png");
    enemyImgs[0] = al_load_bitmap("enemy0.png");
}

void game_begin() {
    // Load sound
    song = al_load_sample("hello.wav");
    if (!song){
        printf("Audio clip sample not loaded!\n");
        show_err_msg(-6);
    }
    // Loop the song until the display closes
    // al_play_sample(song, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    // load font
    bigFont = al_load_ttf_font("pirulen.ttf", 25, 0);
    smallFont = al_load_ttf_font("pirulen.ttf", 15, 0);

    // load images
    load_images();
    draw_menu();
    cursor = al_create_mouse_cursor(cursorImg, 0, 0);
    if (!cursor){
        show_err_msg(5);
    }
    al_set_mouse_cursor(display, cursor);
}


int process_event() {
    // Request the event
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    if (event.timer.source == bulletUpdateTimer){
        for (Bullet *current = player_bullet_list, *previous = player_bullet_list; current != NULL;){
            if (current != player_bullet_list && previous->next != current){
                previous = previous->next;
            }
            // TODO: Add more case into switch
            switch (current->mode){
                case straight_forward:
                    current->pos.y -= 17;
                    al_draw_bitmap(current->bitmap, current->pos.x - current->size.x / 2, current->pos.y,
                                   current->flip);
                    break;
                case right_front:
                    current->pos.x += 12;
                    current->pos.y -= 12;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) (ALLEGRO_PI / 4), current->flip);
                    break;
                case left_front:
                    current->pos.x -= 12;
                    current->pos.y -= 12;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) (-ALLEGRO_PI / 4), current->flip);
                    break;
                case right:
                    current->pos.x += 17;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) (ALLEGRO_PI / 2), current->flip);
                    break;
                case left:
                    current->pos.x -= 17;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) (-ALLEGRO_PI / 2), current->flip);
                    break;
                case down:
                    current->pos.y += 17;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) ALLEGRO_PI, current->flip);
                    break;
                case rf_26:
                    current->pos.x += 6;
                    current->pos.y -= 16;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) (ALLEGRO_PI / 180 * 22.5), current->flip);
                    break;
                case lf_26:
                    current->pos.x -= 6;
                    current->pos.y -= 16;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) (-ALLEGRO_PI / 180 * 22.5), current->flip);
                    break;
                case stopped:
                    destroy_bullet(current, &previous, &player_bullet_list);
                    current = previous;
                    break;
                default:
                    printf("unknown fly mode\n");
                    show_err_msg(1);
                    break;
            }
            if (current == NULL){
                if (player_bullet_list == NULL){
                    break;
                }
                printf("NULL\n");
                current = player_bullet_list;
                previous = player_bullet_list;
                continue;
            }
            al_draw_circle(current->pos.x + current->hit_area.center.x,
                           current->pos.y + current->hit_area.center.y,
                           current->hit_area.radius, al_map_rgb(255, 220, 188), 0);

            // set the outside bullet stopped
            if (current->pos.y < -40 || current->pos.y > HEIGHT + 40){
                // printf("one bullet stop: y = %f\n", current->pos.y);
                current->mode = stopped;
            }
            else if (current->pos.x < -40 || current->pos.x > WIDTH + 40){
                // printf("one bullet stop: x = %f\n", current->pos.x);
                current->mode = stopped;
            }
            current = current->next;
        }
        for (Bullet *current = enemy_bullet_list, *previous = enemy_bullet_list; current != NULL;){
            if (current != enemy_bullet_list && previous->next != current){
                previous = previous->next;
            }
            // TODO: Add more case into switch
            switch (current->mode){
                case straight_forward:
                    current->pos.y -= 17;
                    al_draw_bitmap(current->bitmap, current->pos.x - current->size.x / 2, current->pos.y,
                                   current->flip);
                    break;
                case right_front:
                    current->pos.x += 12;
                    current->pos.y -= 12;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) (ALLEGRO_PI / 4), current->flip);
                    break;
                case left_front:
                    current->pos.x -= 12;
                    current->pos.y -= 12;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) (-ALLEGRO_PI / 4), current->flip);
                    break;
                case right:
                    current->pos.x += 17;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) (ALLEGRO_PI / 2), current->flip);
                    break;
                case left:
                    current->pos.x -= 17;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) (-ALLEGRO_PI / 2), current->flip);
                    break;
                case down:
                    current->pos.y += 17;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) ALLEGRO_PI, current->flip);
                    break;
                case rf_26:
                    current->pos.x += 6;
                    current->pos.y -= 16;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) (ALLEGRO_PI / 180 * 22.5), current->flip);
                    break;
                case lf_26:
                    current->pos.x -= 6;
                    current->pos.y -= 16;
                    al_draw_rotated_bitmap(current->bitmap, current->size.x / 2, 0, current->pos.x, current->pos.y,
                                           (float) (-ALLEGRO_PI / 180 * 22.5), current->flip);
                    break;
                case stopped:
                    destroy_bullet(current, &previous, &enemy_bullet_list);
                    current = previous;
                    break;
                default:
                    printf("unknown fly mode\n");
                    show_err_msg(1);
                    break;
            }
            if (current == NULL){
                if (enemy_bullet_list == NULL){
                    break;
                }
                printf("NULL\n");
                current = enemy_bullet_list;
                previous = enemy_bullet_list;
                continue;
            }
            /**al_draw_circle(current->pos.x + current->hit_area.center.x,
                           current->pos.y + current->hit_area.center.y,
                           current->hit_area.radius, white, 0);
            */
            // set the outside bullet stopped
            if (current->pos.y < -40 || current->pos.y > HEIGHT + 40){
                // printf("one bullet stop: y = %lf\n", current->pos.y);
                current->mode = stopped;
            }
            else if (current->pos.x < -40 || current->pos.x > WIDTH + 40){
                // printf("one bullet stop: x = %lf\n", current->pos.x);
                current->mode = stopped;
            }
            current = current->next;
        }
        draw_count++;
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
    else if (event.timer.source == shootingDefaultBulletTimer){
        Bullet *bt = make_bullet(player.default_bullet, player.bullet_mode,
                                 (Vector2) {player.pos.x + player.firing_point.x,
                                            player.pos.y + player.firing_point.y});
        register_bullet(bt, &player_bullet_list);
        /**bt = make_bullet(player.default_bullet, lf_26,
                         (Vector2) {player.pos.x + player.firing_point.x,
                                    player.pos.y + player.firing_point.y});
        register_bullet(bt, &player_bullet_list);
        bt = make_bullet(player.default_bullet, rf_26,
                         (Vector2) {player.pos.x + player.firing_point.x,
                                    player.pos.y + player.firing_point.y});
        register_bullet(bt, &player_bullet_list);
        */
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
                    printf("Collide\n");
                    pla_curr->mode = stopped;
                    ene_curr->mode = stopped;
                }
            }
        }
        for (Bullet *pla_curr = player_bullet_list; pla_curr != NULL; pla_curr = pla_curr->next){
            for (Character *enemy = enemy_list; enemy != NULL; enemy = enemy->next){
                if (collide_with((Circle) {(Vector2) {pla_curr->pos.x + pla_curr->hit_area.center.x,
                                                      pla_curr->pos.y + pla_curr->hit_area.center.y},
                                           pla_curr->hit_area.radius},
                                 (Circle) {(Vector2) {enemy->pos.x, enemy->pos.y}, enemy->body.radius})){
                    pla_curr->mode = stopped;
                    enemy->health -= 4;
                }
            }
        }
    }
    else if (event.timer.source == generateEnemyBulletTimer){
        Bullet *bt = make_bullet(bulletImgs[1], down, (Vector2) {-1, 0});
        register_bullet(bt, &enemy_bullet_list);
    }
    else if (event.timer.source == enemyMovingTimer){
        for (Character *current = enemy_list, *prev = enemy_list; current != NULL;){
            if (current != enemy_list && prev->next != current){
                prev = prev->next;
            }
            // TODO: make enemy fly
            current->pos.x += current->speed.x;
            current->pos.y += current->speed.y;
            al_draw_rotated_bitmap(current->image, current->body.center.x, current->body.center.y,
                                   current->pos.x, current->pos.y,
                                   (float) (ALLEGRO_PI / 180 * current->dire_angle), 0);
            al_draw_circle(current->pos.x, current->pos.y, current->body.radius, white, 0);

            // TODO: change the enemy's direction if necessary

            if (current->health <= 0 || current->pos.y < -100 || current->pos.y > HEIGHT + 100){
                printf("one enemy stopped: %f\n", current->pos.y);
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
        draw_count++;
    }


    // Keyboard
    if (event.type == ALLEGRO_EVENT_KEY_UP){
        if (window == 1 && event.keyboard.keycode == ALLEGRO_KEY_ENTER){
            enter_game_window = true;
        }
        if (window == 2){
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
        if (window == 2){
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
                default:
                    break;
            }
        }
    }
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1){
        printf("press ");
        if (window == 1){
            if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 150, 225, WIDTH / 2 + 150, 285)){
                enter_game_window = true;
                printf("START");
            }

            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 120, 340, WIDTH / 2 + 120, 380)){
                window = 3;
                draw_about();
                printf("ABOUT");
            }
            else if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 120, 410, WIDTH / 2 + 120, 450)){
                printf("EXIT\n");
                return GAME_TERMINATE;
            }
        }
        else if (window == 2){
            if (within(event.mouse.x, event.mouse.y, 1, HEIGHT - 55, 100, HEIGHT)){
                printf("SPEED UP");
                al_set_timer_speed(playerMovingTimer, 1.0 / 60.0);
            }
            else if (within(event.mouse.x, event.mouse.y, 101, HEIGHT - 55, 200, HEIGHT)){
                printf("SLOW DOWN");
                al_set_timer_speed(playerMovingTimer, 1.0 / 30.0);
            }
            else if (within(event.mouse.x, event.mouse.y, 201, HEIGHT - 55, 300, HEIGHT)){
                printf("FIRE3");
                Bullet *bt_tmp = make_bullet(bulletImgs[0], down, (Vector2) {-1, 0});
                register_bullet(bt_tmp, &enemy_bullet_list);
            }
            else if (within(event.mouse.x, event.mouse.y, 301, HEIGHT - 55, 400, HEIGHT)){
                printf("Enemy");
                Character *ene = create_enemy(enemyImgs[0], 10, (Vector2) {-1, 0}, 5, 180,
                                              bulletImgs[1], (float) 1 / 2, down);
                register_enemy(ene);
            }
        }
        else if (window == 3){
            if (within(event.mouse.x, event.mouse.y, WIDTH / 2 - 120, 440, WIDTH / 2 + 120, 480)){
                window = 1;
                draw_menu();
                printf("BACK");
            }
        }
        printf("\n");

    }
        // Shutdown our program
    else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
        printf("byebye");
        return GAME_TERMINATE;
    }

    return 0;
}

int game_run() {
    int error = 0;
    // First window(Menu)
    if (window == 1){
        if (!al_is_event_queue_empty(event_queue)){
            error = process_event();
            if (enter_game_window){
                window = 2;
                // Setting Character
                // background = al_load_bitmap("stage.jpg");
                player.pos.x = 200;
                player.pos.y = HEIGHT - 100;
                set_character(airplaneImgs[0], 100, bulletImgs[0], (float) 1 / 4, straight_forward);

                // Initialize Timer
                playerMovingTimer = al_create_timer(1.0 / 30.0);
                enemyMovingTimer = al_create_timer(1.0 / 30.0);
                bulletUpdateTimer = al_create_timer(1.0 / 30.0);
                shootingDefaultBulletTimer = al_create_timer(player.shooting_rate);
                collisionDetectTimer = al_create_timer(1.0 / 60.0);
                generateEnemyBulletTimer = al_create_timer(1.0 / 5.0);
                al_register_event_source(event_queue, al_get_timer_event_source(playerMovingTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(enemyMovingTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(bulletUpdateTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(shootingDefaultBulletTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(collisionDetectTimer));
                al_register_event_source(event_queue, al_get_timer_event_source(generateEnemyBulletTimer));
                al_start_timer(playerMovingTimer);
                al_start_timer(enemyMovingTimer);
                al_start_timer(bulletUpdateTimer);
                al_start_timer(shootingDefaultBulletTimer);
                al_start_timer(collisionDetectTimer);
                al_start_timer(generateEnemyBulletTimer);
            }
        }
    }
        // Second window(Main Game)
    else if (window == 2){
        if (draw_count == 0){
            draw_game_scene();
        }
        if (draw_count >= 2){
            draw_count = 0;
            al_flip_display();
        }

        // Listening for new event
        if (!al_is_event_queue_empty(event_queue)){
            error = process_event();
        }
    }
    else if (window == 3){
        // Listening for new event
        if (!al_is_event_queue_empty(event_queue)){
            error = process_event();
        }
    }
    return error;
}

void game_destroy() {
    // Make sure you destroy all things
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
    al_destroy_timer(shootingDefaultBulletTimer);
    al_destroy_timer(collisionDetectTimer);
    al_destroy_sample(song);
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
        printf("%lf\n", a.radius + b.radius);
        printf("%lf\n", sqrtf(powf(b.center.x - a.center.x, 2) + powf(b.center.y - a.center.y, 2)));
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

    al_flip_display();
}

void draw_about() {
    al_clear_to_color(al_map_rgb(70, 70, 10));
    al_draw_text(smallFont, white, WIDTH / 2, 450, ALLEGRO_ALIGN_CENTER, "BACK");
    al_draw_rectangle(WIDTH / 2 - 120, 440, WIDTH / 2 + 120, 480, white, 0);
    al_flip_display();
}

void draw_game_scene() {
    al_clear_to_color(al_map_rgb(50, 50, 80));
    /*
    al_draw_text(smallFont, white, 50, HEIGHT - 50 + 15, ALLEGRO_ALIGN_CENTER, "Speed UP");
    al_draw_rectangle(1, HEIGHT - 55, 100, HEIGHT, white, 0);
    al_draw_text(smallFont, white, 150, HEIGHT - 50 + 15, ALLEGRO_ALIGN_CENTER, "Slow");
    al_draw_rectangle(100, HEIGHT - 55, 200, HEIGHT, white, 0);
    */
    al_draw_text(smallFont, white, 250, HEIGHT - 50 + 15, ALLEGRO_ALIGN_CENTER, "FIRE3");
    al_draw_rectangle(200, HEIGHT - 55, 300, HEIGHT, white, 0);
    al_draw_text(smallFont, white, 350, HEIGHT - 50 + 15, ALLEGRO_ALIGN_CENTER, "FIRE4");
    al_draw_rectangle(300, HEIGHT - 55, 400, HEIGHT, white, 0);

    al_draw_bitmap(player.image, player.pos.x, player.pos.y, 0);
}


