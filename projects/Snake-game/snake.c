/*
 * Snake Game in C — Linux (ncurses)
 * Compile: gcc -o snake snake.c -lncurses
 * Run:     ./snake
 * Controls: W/A/S/D or Arrow Keys | P to pause | Q to quit
 */

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

/* ── Constants ── */
#define ROWS       20
#define COLS       40
#define MAX_LEN    (ROWS * COLS)
#define INIT_SPEED 150   /* ms per frame */
#define SPEED_STEP 5     /* ms faster per food eaten */
#define MIN_SPEED  40    /* speed cap */

/* Board offset — leaves room for the status bar at the top */
#define ORIGIN_Y 2
#define ORIGIN_X 2

/* ── Types ── */
typedef struct { int y, x; } Point;
typedef enum   { UP, DOWN, LEFT, RIGHT } Dir;

/* ── Globals ── */
Point snake[MAX_LEN];
int   snake_len = 3;
Dir   direction = RIGHT;
Point food;
int   score   = 0;
int   speed   = INIT_SPEED;
int   running = 1;
int   paused  = 0;

/* ───────────────────────── Setup ───────────────────────── */

void setup_ncurses(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);    /* enable arrow keys */
    nodelay(stdscr, TRUE);   /* non-blocking getch */
    curs_set(0);             /* hide cursor */
    srand((unsigned)time(NULL));

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN,  COLOR_BLACK); /* body   */
        init_pair(2, COLOR_YELLOW, COLOR_BLACK); /* head   */
        init_pair(3, COLOR_RED,    COLOR_BLACK); /* food   */
        init_pair(4, COLOR_CYAN,   COLOR_BLACK); /* border */
        init_pair(5, COLOR_WHITE,  COLOR_BLACK); /* status */
    }
}

/* ───────────────────────── Food ───────────────────────── */

void place_food(void) {
    int i, ok;
    do {
        ok = 1;
        food.y = ORIGIN_Y + rand() % ROWS;
        food.x = ORIGIN_X + rand() % COLS;
        for (i = 0; i < snake_len; i++) {
            if (snake[i].y == food.y && snake[i].x == food.x) {
                ok = 0;
                break;
            }
        }
    } while (!ok);
}

/* ───────────────────────── Drawing ───────────────────────── */

void draw_border(void) {
    int r, c;
    attron(COLOR_PAIR(4) | A_BOLD);
    for (c = ORIGIN_X - 1; c <= ORIGIN_X + COLS; c++) {
        mvaddch(ORIGIN_Y - 1,    c, ACS_HLINE);
        mvaddch(ORIGIN_Y + ROWS, c, ACS_HLINE);
    }
    for (r = ORIGIN_Y - 1; r <= ORIGIN_Y + ROWS; r++) {
        mvaddch(r, ORIGIN_X - 1,    ACS_VLINE);
        mvaddch(r, ORIGIN_X + COLS, ACS_VLINE);
    }
    mvaddch(ORIGIN_Y - 1,    ORIGIN_X - 1,    ACS_ULCORNER);
    mvaddch(ORIGIN_Y - 1,    ORIGIN_X + COLS, ACS_URCORNER);
    mvaddch(ORIGIN_Y + ROWS, ORIGIN_X - 1,    ACS_LLCORNER);
    mvaddch(ORIGIN_Y + ROWS, ORIGIN_X + COLS, ACS_LRCORNER);
    attroff(COLOR_PAIR(4) | A_BOLD);
}

void draw_status(void) {
    attron(COLOR_PAIR(5));
    mvprintw(0, ORIGIN_X, "SNAKE  |  Score: %d  |  [WASD / Arrows] Move  [P] Pause  [Q] Quit", score);
    attroff(COLOR_PAIR(5));

    if (paused) {
        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(ORIGIN_Y + ROWS / 2, ORIGIN_X + COLS / 2 - 3, " PAUSED ");
        attroff(COLOR_PAIR(2) | A_BOLD);
    }
}

void draw_snake(void) {
    int i;

    attron(COLOR_PAIR(2) | A_BOLD);
    mvaddch(snake[0].y, snake[0].x, '@');   /* head */
    attroff(COLOR_PAIR(2) | A_BOLD);

    attron(COLOR_PAIR(1));
    for (i = 1; i < snake_len; i++) {
        mvaddch(snake[i].y, snake[i].x, 'o'); /* body */
    }
    attroff(COLOR_PAIR(1));
}

void draw_food(void) {
    attron(COLOR_PAIR(3) | A_BOLD);
    mvaddch(food.y, food.x, '*');
    attroff(COLOR_PAIR(3) | A_BOLD);
}

/* ───────────────────────── Input ───────────────────────── */

void handle_input(int ch) {
    switch (ch) {
        case 'w': case 'W': case KEY_UP:    if (direction != DOWN)  direction = UP;    break;
        case 's': case 'S': case KEY_DOWN:  if (direction != UP)    direction = DOWN;  break;
        case 'a': case 'A': case KEY_LEFT:  if (direction != RIGHT) direction = LEFT;  break;
        case 'd': case 'D': case KEY_RIGHT: if (direction != LEFT)  direction = RIGHT; break;
        case 'p': case 'P': paused  = !paused; break;
        case 'q': case 'Q': running = 0;        break;
    }
}

/* ───────────────────────── Update ───────────────────────── */

void update(void) {
    if (paused) return;

    int i;

    /* Calculate next head position */
    Point next = snake[0];
    if (direction == UP)    next.y--;
    if (direction == DOWN)  next.y++;
    if (direction == LEFT)  next.x--;
    if (direction == RIGHT) next.x++;

    /* Wall collision */
    if (next.y < ORIGIN_Y || next.y >= ORIGIN_Y + ROWS ||
        next.x < ORIGIN_X || next.x >= ORIGIN_X + COLS) {
        running = 0;
        return;
    }

    /* Self collision */
    for (i = 0; i < snake_len - 1; i++) {
        if (snake[i].y == next.y && snake[i].x == next.x) {
            running = 0;
            return;
        }
    }

    int ate = (next.y == food.y && next.x == food.x);

    /* Shift body segments back, place new head */
    for (i = snake_len - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = next;

    if (ate) {
        snake_len++;
        score += 10;
        if (speed > MIN_SPEED) speed -= SPEED_STEP;
        place_food();
    }
}

/* ───────────────────────── Game over ───────────────────────── */

void game_over_screen(void) {
    clear();

    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(LINES / 2 - 1, COLS / 2 - 5, " GAME OVER ");
    attroff(COLOR_PAIR(3) | A_BOLD);

    attron(COLOR_PAIR(5));
    mvprintw(LINES / 2 + 1, COLS / 2 - 7, "Final Score: %d", score);
    mvprintw(LINES / 2 + 3, COLS / 2 - 11, "Press any key to exit...");
    attroff(COLOR_PAIR(5));

    refresh();
    nodelay(stdscr, FALSE); /* block until key press */
    getch();
}

/* ───────────────────────── Main ───────────────────────── */

int main(void) {
    int i;
    struct timespec ts = {.tv_sec = 0};

    setup_ncurses();

    /* Initialize snake in the center, facing right */
    snake[0].y = ORIGIN_Y + ROWS / 2;
    snake[0].x = ORIGIN_X + COLS / 2;
    for (i = 1; i < snake_len; i++) {
        snake[i].y = snake[0].y;
        snake[i].x = snake[0].x - i;
    }

    place_food();

    while (running) {
        int ch = getch();
        if (ch != ERR) handle_input(ch);

        update();

        clear();
        draw_border();
        draw_status();
        draw_food();
        draw_snake();
        refresh();

        ts.tv_nsec = speed * 1000000L;
        nanosleep(&ts, NULL);
    }

    game_over_screen();
    endwin();
    return 0;
}