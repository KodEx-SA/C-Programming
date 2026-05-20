/*
 * Snake Game in C using ncurses
 * Linux / Linux Mint XFCE 22
 *
 * Compile: gcc -o snake snake.c -lncurses
 * Run:     ./snake
 *
 * Controls: W/A/S/D or Arrow Keys | Q to quit | P to pause
 */

#include <ncurses.h> // for terminal graphics
#include <stdlib.h> // for rand() and srand()
#include <time.h> // for time() and nanosleep()
#include <string.h> // for memset() and memmove()

/* ======================================== Game constants ======================================== */
#define MAX_SNAKE_LEN 256
#define INITIAL_SPEED 150 /* ms per frame */
#define SPEED_STEP 5      /* ms faster per food eaten */
#define MIN_SPEED 50      /* fastest possible */

/* Board boundaries (drawn inside a border) */
#define BOARD_TOP 2
#define BOARD_LEFT 2
#define BOARD_ROWS 22
#define BOARD_COLS 50

/* ======================================== Data types ======================================== */
typedef struct // Represents a point on the board (y, x)
{
    int y, x;
} Point;

typedef enum // Possible movement directions for the snake
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct // Represents the snake in the game
{
    Point body[MAX_SNAKE_LEN];
    int len;
    Direction dir;
} Snake;

typedef struct // Represents the overall game state
{
    Snake snake;
    Point food;
    int score;
    int speed; /* delay in ms */
    int running;
    int paused;
} GameState;

/* ======================================== Prototypes ======================================== */
void init_ncurses(void); // Initializes ncurses and sets up the terminal
void draw_border(void);
void draw_status(const GameState *g); // Draws the score, speed, and controls at the top of the screen
void draw_snake(const Snake *s);
void draw_food(Point food);
void place_food(GameState *g);
int check_collision(const GameState *g, Point next);
Point next_head(const Snake *s);
void handle_input(GameState *g, int ch); // Handles user input to change direction, pause, or quit
void update(GameState *g);
void game_loop(void);
void show_start_screen(void);
void show_game_over(int score);

/* ======================================== ncurses init ======================================== */
void init_ncurses(void) // Sets up ncurses mode, colors, and input settings
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); /* non-blocking getch */
    curs_set(0);           /* hide cursor */
    srand((unsigned)time(NULL));

    if (has_colors())
    {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);   /* snake body */
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);  /* snake head */
        init_pair(3, COLOR_RED, COLOR_BLACK);     /* food       */
        init_pair(4, COLOR_CYAN, COLOR_BLACK);    /* border     */
        init_pair(5, COLOR_WHITE, COLOR_BLACK);   /* status     */
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK); /* title      */
    }
}

/* ======================================== Drawing helpers ======================================== */
void draw_border(void)
{
    attron(COLOR_PAIR(4) | A_BOLD);
    /* Top & bottom edges */
    for (int c = BOARD_LEFT; c < BOARD_LEFT + BOARD_COLS + 1; c++) // +1 to include the rightmost column
    {
        mvaddch(BOARD_TOP - 1, c, ACS_HLINE);
        mvaddch(BOARD_TOP + BOARD_ROWS, c, ACS_HLINE);
    }
    /* Left & right edges */
    for (int r = BOARD_TOP - 1; r <= BOARD_TOP + BOARD_ROWS; r++)
    {
        mvaddch(r, BOARD_LEFT - 1, ACS_VLINE);
        mvaddch(r, BOARD_LEFT + BOARD_COLS + 1, ACS_VLINE);
    }
    /* Corners */
    mvaddch(BOARD_TOP - 1, BOARD_LEFT - 1, ACS_ULCORNER);
    mvaddch(BOARD_TOP - 1, BOARD_LEFT + BOARD_COLS + 1, ACS_URCORNER);
    mvaddch(BOARD_TOP + BOARD_ROWS, BOARD_LEFT - 1, ACS_LLCORNER);
    mvaddch(BOARD_TOP + BOARD_ROWS, BOARD_LEFT + BOARD_COLS + 1, ACS_LRCORNER);
    attroff(COLOR_PAIR(4) | A_BOLD);
}

void draw_status(const GameState *g)
{
    attron(COLOR_PAIR(5));
    mvprintw(0, BOARD_LEFT, "SNAKE  |  Score: %-4d  |  Speed: %-3d  |  [W/A/S/D] Move  [P] Pause  [Q] Quit",
             g->score, (INITIAL_SPEED - g->speed) / SPEED_STEP + 1);
    attroff(COLOR_PAIR(5));

    if (g->paused)
    {
        attron(COLOR_PAIR(6) | A_BOLD | A_BLINK);
        mvprintw(BOARD_TOP + BOARD_ROWS / 2, BOARD_LEFT + BOARD_COLS / 2 - 4, " PAUSED ");
        attroff(COLOR_PAIR(6) | A_BOLD | A_BLINK);
    }
}

void draw_snake(const Snake *s)
{
    /* Head */
    attron(COLOR_PAIR(2) | A_BOLD);
    mvaddch(s->body[0].y, s->body[0].x, '@');
    attroff(COLOR_PAIR(2) | A_BOLD);
    /* Body */
    attron(COLOR_PAIR(1));
    for (int i = 1; i < s->len; i++)
    {
        mvaddch(s->body[i].y, s->body[i].x, 'o');
    }
    attroff(COLOR_PAIR(1));
}

void draw_food(Point food)
{
    attron(COLOR_PAIR(3) | A_BOLD);
    mvaddch(food.y, food.x, '*');
    attroff(COLOR_PAIR(3) | A_BOLD);
}

/* ======================================== Food placement ======================================== */
void place_food(GameState *g)
{
    Point p;
    int ok;
    do
    {
        ok = 1;
        p.y = BOARD_TOP + rand() % BOARD_ROWS;
        p.x = BOARD_LEFT + rand() % (BOARD_COLS + 1);
        for (int i = 0; i < g->snake.len; i++)
        {
            if (g->snake.body[i].y == p.y && g->snake.body[i].x == p.x)
            {
                ok = 0;
                break;
            }
        }
    } while (!ok);
    g->food = p;
}

/* ======================================== Movement ======================================== */
Point next_head(const Snake *s)
{
    Point h = s->body[0];
    switch (s->dir)
    {
    case UP:
        h.y--;
        break;
    case DOWN:
        h.y++;
        break;
    case LEFT:
        h.x--;
        break;
    case RIGHT:
        h.x++;
        break;
    }
    return h;
}

int check_collision(const GameState *g, Point next)
{
    /* Wall */
    if (next.y < BOARD_TOP || next.y >= BOARD_TOP + BOARD_ROWS)
        return 1;
    if (next.x < BOARD_LEFT || next.x > BOARD_LEFT + BOARD_COLS)
        return 1;
    /* Self */
    for (int i = 0; i < g->snake.len - 1; i++)
    {
        if (g->snake.body[i].y == next.y && g->snake.body[i].x == next.x)
            return 1;
    }
    return 0;
}

/* ======================================== Input ======================================== */
void handle_input(GameState *g, int ch)
{
    Direction d = g->snake.dir;
    switch (ch)
    {
    case 'w':
    case 'W':
    case KEY_UP:
        if (d != DOWN)
            g->snake.dir = UP;
        break;
    case 's':
    case 'S':
    case KEY_DOWN:
        if (d != UP)
            g->snake.dir = DOWN;
        break;
    case 'a':
    case 'A':
    case KEY_LEFT:
        if (d != RIGHT)
            g->snake.dir = LEFT;
        break;
    case 'd':
    case 'D':
    case KEY_RIGHT:
        if (d != LEFT)
            g->snake.dir = RIGHT;
        break;
    case 'p':
    case 'P':
        g->paused = !g->paused;
        break;
    case 'q':
    case 'Q':
        g->running = 0;
        break;
    default:
        break;
    }
}

/* ======================================== Update game state ======================================== */
void update(GameState *g)
{
    if (g->paused)
        return;

    Point next = next_head(&g->snake);

    if (check_collision(g, next))
    {
        g->running = 0;
        return;
    }

    int ate = (next.y == g->food.y && next.x == g->food.x);

    /* Shift body */
    memmove(&g->snake.body[1], &g->snake.body[0],
            sizeof(Point) * (size_t)(g->snake.len - (ate ? 0 : 1)));
    g->snake.body[0] = next;
    if (ate)
    {
        if (g->snake.len < MAX_SNAKE_LEN)
            g->snake.len++;
        g->score += 10;
        if (g->speed > MIN_SPEED)
            g->speed -= SPEED_STEP;
        place_food(g);
    }
}

/* ======================================== Screens ======================================== */
void show_start_screen(void)
{
    clear();
    int cy = LINES / 2;
    int cx = COLS / 2;

    attron(COLOR_PAIR(6) | A_BOLD);
    mvprintw(cy - 4, cx - 10, "  ___  _  _   __   _  _  ____ ");
    mvprintw(cy - 3, cx - 10, " / __)( \\/ ) / _\\ ( )/ )(  __)");
    mvprintw(cy - 2, cx - 10, " \\__ \\ )  / /    \\ )  (  ) _) ");
    mvprintw(cy - 1, cx - 10, " (___/(__/  \\_/\\_/(_)\\_)(____) ");
    attroff(COLOR_PAIR(6) | A_BOLD);

    attron(COLOR_PAIR(5));
    mvprintw(cy + 1, cx - 14, "  Controls: W/A/S/D or Arrow Keys  |  P = Pause  |  Q = Quit");
    mvprintw(cy + 3, cx - 10, "       Press any key to start ...");
    attroff(COLOR_PAIR(5));

    refresh();
    nodelay(stdscr, FALSE); /* wait for key */
    getch();
    nodelay(stdscr, TRUE);
}

void show_game_over(int score)
{
    int cy = LINES / 2;
    int cx = COLS / 2;

    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(cy - 2, cx - 5, " GAME OVER ");
    attroff(COLOR_PAIR(3) | A_BOLD);

    attron(COLOR_PAIR(5));
    mvprintw(cy, cx - 8, "  Final Score: %-4d", score);
    mvprintw(cy + 2, cx - 12, "  Press R to restart or Q to quit");
    attroff(COLOR_PAIR(5));

    refresh();
    nodelay(stdscr, FALSE);
    int ch;
    while ((ch = getch()) != 'q' && ch != 'Q' && ch != 'r' && ch != 'R')
        ;
    nodelay(stdscr, TRUE);

    if (ch == 'r' || ch == 'R')
        game_loop();
}

/* ======================================== Main game loop ======================================== */
void game_loop(void)
{
    GameState g;
    memset(&g, 0, sizeof(g));

    /* Init snake in the middle */
    g.snake.len = 4;
    g.snake.dir = RIGHT;
    g.snake.body[0] = (Point){BOARD_TOP + BOARD_ROWS / 2, BOARD_LEFT + BOARD_COLS / 2};
    for (int i = 1; i < g.snake.len; i++)
    {
        g.snake.body[i] = (Point){g.snake.body[0].y, g.snake.body[0].x - i};
    }

    g.speed = INITIAL_SPEED;
    g.running = 1;
    place_food(&g);

    struct timespec ts = {.tv_sec = 0};

    while (g.running)
    {
        int ch = getch();
        if (ch != ERR)
            handle_input(&g, ch);

        update(&g);

        /* Render */
        clear();
        draw_border();
        draw_status(&g);
        draw_snake(&g.snake);
        draw_food(g.food);
        refresh();

        /* Frame delay */
        ts.tv_nsec = g.speed * 1000000L;
        nanosleep(&ts, NULL);
    }

    show_game_over(g.score);
}

/* ======================================== Entry point ======================================== */
int main(void)
{
    init_ncurses();
    show_start_screen();
    game_loop();
    endwin();
    return 0;
}