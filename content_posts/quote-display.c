#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/* ============================= ANSI COLOR CODES ============================= */

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define ITALIC "\033[3m"
#define QUOTE_COLOR "\033[38;5;116m"
#define AUTHOR_COLOR "\033[38;5;220m"

typedef struct {
    const char *text;
    const char *author;
} Quote;

static Quote quote[] = {
    {"The quiter it becoms around me,\n The more solitude I'm exposed to,\n It's the more I can actually hear the unnoticed mysteries, the false hopes, the lies I am told and the ones\n I keep telling myself.\n The more I hear the inner me,\n In pieces.", "~Lawliet"}
};

#define QUOTE_COUNT (int)(sizeof(quote) / sizeof(quote[0]))

void typewrite(const char *text, int delay_us)
{
    while (*text) {
        putchar(*text++);
        fflush(stdout);
        usleep(delay_us);
    }
}

int main(int argc, char*argv[])
{
    srand((unsigned)time(NULL));
    int idx = (argc > 1) ? atoi(argv[1]) % QUOTE_COUNT:rand() & QUOTE_COUNT;

    printf("\033[2J\033[H");
    printf("\n\n\n");

printf(BOLD QUOTE_COLOR);
typewrite(quote[idx].text, 45000);
printf(RESET);

printf("\n\n");

printf(ITALIC AUTHOR_COLOR);
typewrite(quote[idx].author, 40000);
printf(RESET);

printf("\n\n\n");
fflush(stdout);

usleep(3000000); // hold for 3 seconds
return 0;
}