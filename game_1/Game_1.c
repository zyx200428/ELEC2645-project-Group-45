#include "Game_1.h"
#include "InputHandler.h"
#include "Menu.h"
#include "LCD.h"
#include "Buzzer.h"
#include "Joystick.h"
#include "stm32l4xx_hal.h"
#include <stdio.h>

// Shared globals from template main.c
extern ST7789V2_cfg_t cfg0;
extern Buzzer_cfg_t buzzer_cfg;
extern Joystick_cfg_t joystick_cfg;
extern Joystick_t joystick_data;

#define SNAKE_W               16
#define SNAKE_H               14
#define MAX_SNAKE_LEN         64
#define OBSTACLE_COUNT        8

#define FRAME_TIME_MS         30
#define START_MOVE_DELAY_MS   220
#define MIN_MOVE_DELAY_MS     90

typedef struct {
    int x;
    int y;
} SnakePoint;

typedef enum {
    SNAKE_STATE_START = 0,
    SNAKE_STATE_RUNNING,
    SNAKE_STATE_PAUSED,
    SNAKE_STATE_GAME_OVER
} SnakeState;

typedef enum {
    DIR_UP = 0,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} SnakeDir;

static SnakePoint snake[MAX_SNAKE_LEN];
static int snake_len = 3;
static SnakePoint food;
static SnakePoint obstacles[OBSTACLE_COUNT];

static SnakeDir snake_dir = DIR_RIGHT;
static SnakeDir next_dir = DIR_RIGHT;
static SnakeState game_state = SNAKE_STATE_START;

static uint32_t last_move_ms = 0;
static uint32_t move_delay_ms = START_MOVE_DELAY_MS;

static int score = 0;
static uint32_t rng_state = 0x12345678;

static uint32_t snake_rand(void)
{
    rng_state = rng_state * 1664525u + 1013904223u;
    return rng_state;
}

static void snake_beep_short(void)
{
    buzzer_tone(&buzzer_cfg, 1200, 35);
    HAL_Delay(35);
    buzzer_off(&buzzer_cfg);
}

static void snake_beep_game_over(void)
{
    for (int i = 0; i < 3; i++) {
        buzzer_tone(&buzzer_cfg, 450, 40);
        HAL_Delay(120);
        buzzer_off(&buzzer_cfg);
        HAL_Delay(60);
    }
}

static int point_equals(SnakePoint a, SnakePoint b)
{
    return (a.x == b.x) && (a.y == b.y);
}

static int snake_contains(int x, int y)
{
    for (int i = 0; i < snake_len; i++) {
        if (snake[i].x == x && snake[i].y == y) {
            return 1;
        }
    }
    return 0;
}

static int obstacle_contains(int x, int y)
{
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        if (obstacles[i].x == x && obstacles[i].y == y) {
            return 1;
        }
    }
    return 0;
}

static void spawn_food(void)
{
    while (1) {
        SnakePoint p;
        p.x = (int)(snake_rand() % SNAKE_W);
        p.y = (int)(snake_rand() % SNAKE_H);

        if (!snake_contains(p.x, p.y) && !obstacle_contains(p.x, p.y)) {
            food = p;
            return;
        }
    }
}

static void init_obstacles(void)
{
    obstacles[0] = (SnakePoint){5, 3};
    obstacles[1] = (SnakePoint){5, 4};
    obstacles[2] = (SnakePoint){5, 5};

    obstacles[3] = (SnakePoint){10, 7};
    obstacles[4] = (SnakePoint){11, 7};
    obstacles[5] = (SnakePoint){12, 7};

    obstacles[6] = (SnakePoint){8, 11};
    obstacles[7] = (SnakePoint){9, 11};
}

static void snake_reset(void)
{
    rng_state ^= HAL_GetTick();

    snake_len = 3;
    snake[0] = (SnakePoint){4, 6};
    snake[1] = (SnakePoint){3, 6};
    snake[2] = (SnakePoint){2, 6};

    snake_dir = DIR_RIGHT;
    next_dir = DIR_RIGHT;
    game_state = SNAKE_STATE_START;

    score = 0;
    move_delay_ms = START_MOVE_DELAY_MS;
    last_move_ms = HAL_GetTick();

    init_obstacles();
    spawn_food();
}

static void update_direction_from_joystick(void)
{
    Joystick_Read(&joystick_cfg, &joystick_data);
    UserInput input = Joystick_GetInput(&joystick_data);

    switch (input.direction) {
        case N:
        case NE:
        case NW:
            if (snake_dir != DIR_DOWN) {
                next_dir = DIR_UP;
            }
            break;

        case S:
        case SE:
        case SW:
            if (snake_dir != DIR_UP) {
                next_dir = DIR_DOWN;
            }
            break;

        case W:
            if (snake_dir != DIR_RIGHT) {
                next_dir = DIR_LEFT;
            }
            break;

        case E:
            if (snake_dir != DIR_LEFT) {
                next_dir = DIR_RIGHT;
            }
            break;

        default:
            break;
    }
}

static int snake_check_collision(SnakePoint p)
{
    if (p.x < 0 || p.x >= SNAKE_W || p.y < 0 || p.y >= SNAKE_H) {
        return 1;
    }

    if (obstacle_contains(p.x, p.y)) {
        return 1;
    }

    for (int i = 0; i < snake_len; i++) {
        if (point_equals(p, snake[i])) {
            return 1;
        }
    }

    return 0;
}

static void snake_step(void)
{
    SnakePoint new_head = snake[0];
    snake_dir = next_dir;

    switch (snake_dir) {
        case DIR_UP:    new_head.y--; break;
        case DIR_DOWN:  new_head.y++; break;
        case DIR_LEFT:  new_head.x--; break;
        case DIR_RIGHT: new_head.x++; break;
        default: break;
    }

    if (snake_check_collision(new_head)) {
        game_state = SNAKE_STATE_GAME_OVER;
        snake_beep_game_over();
        return;
    }

    for (int i = snake_len; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = new_head;

    if (point_equals(new_head, food)) {
        if (snake_len < MAX_SNAKE_LEN - 1) {
            snake_len++;
        }
        score += 1;
        snake_beep_short();
        spawn_food();

        if (move_delay_ms > MIN_MOVE_DELAY_MS) {
            move_delay_ms -= 8;
        }
    }
}

static void snake_update_logic(void)
{
    update_direction_from_joystick();

    if (game_state != SNAKE_STATE_RUNNING) {
        return;
    }

    uint32_t now = HAL_GetTick();
    if ((now - last_move_ms) >= move_delay_ms) {
        last_move_ms = now;
        snake_step();
    }
}

static void build_row_string(int row, char *out)
{
    int idx = 0;
    out[idx++] = '#';

    for (int col = 0; col < SNAKE_W; col++) {
        char c = ' ';

        if (food.x == col && food.y == row) {
            c = '*';
        }

        for (int i = 0; i < OBSTACLE_COUNT; i++) {
            if (obstacles[i].x == col && obstacles[i].y == row) {
                c = 'X';
            }
        }

        for (int i = snake_len - 1; i >= 0; i--) {
            if (snake[i].x == col && snake[i].y == row) {
                c = (i == 0) ? 'O' : 'o';
            }
        }

        out[idx++] = c;
    }

    out[idx++] = '#';
    out[idx] = '\0';
}

static void draw_status_text(void)
{
    char line[32];

    sprintf(line, "Score: %d", score);
    LCD_printString(line, 10, 10, 1, 2);

    switch (game_state) {
        case SNAKE_STATE_START:
            LCD_printString("BT2: Start", 10, 28, 1, 1);
            LCD_printString("BT3: Menu", 120, 28, 1, 1);
            break;

        case SNAKE_STATE_RUNNING:
            LCD_printString("BT2: Pause", 10, 28, 1, 1);
            LCD_printString("BT3: Menu", 120, 28, 1, 1);
            break;

        case SNAKE_STATE_PAUSED:
            LCD_printString("PAUSED", 95, 28, 1, 1);
            LCD_printString("BT2: Resume", 10, 42, 1, 1);
            LCD_printString("BT3: Menu", 120, 42, 1, 1);
            break;

        case SNAKE_STATE_GAME_OVER:
            LCD_printString("GAME OVER", 80, 28, 1, 2);
            LCD_printString("BT2: Restart", 10, 50, 1, 1);
            LCD_printString("BT3: Menu", 120, 50, 1, 1);
            break;
    }
}

static void snake_render(void)
{
    LCD_Fill_Buffer(0);

    LCD_printString("SNAKE", 85, 0, 1, 2);
    draw_status_text();

    LCD_printString("==================", 20, 65, 1, 1);

    char row_buf[SNAKE_W + 3];
    int y0 = 80;

    for (int row = 0; row < SNAKE_H; row++) {
        build_row_string(row, row_buf);
        LCD_printString(row_buf, 20, y0 + row * 10, 1, 1);
    }

    LCD_printString("O=head o=body *=food X=wall", 10, 225, 1, 1);

    LCD_Refresh(&cfg0);
}

MenuState Game1_Run(void)
{
    snake_reset();

    while (1) {
        uint32_t frame_start = HAL_GetTick();

        Input_Read();

        if (current_input.btn3_pressed) {
            return MENU_STATE_HOME;
        }

        if (current_input.btn2_pressed) {
            if (game_state == SNAKE_STATE_START) {
                game_state = SNAKE_STATE_RUNNING;
            } else if (game_state == SNAKE_STATE_RUNNING) {
                game_state = SNAKE_STATE_PAUSED;
            } else if (game_state == SNAKE_STATE_PAUSED) {
                game_state = SNAKE_STATE_RUNNING;
            } else if (game_state == SNAKE_STATE_GAME_OVER) {
                snake_reset();
                game_state = SNAKE_STATE_RUNNING;
            }
        }

        snake_update_logic();
        snake_render();

        uint32_t frame_time = HAL_GetTick() - frame_start;
        if (frame_time < FRAME_TIME_MS) {
            HAL_Delay(FRAME_TIME_MS - frame_time);
        }
    }
}
