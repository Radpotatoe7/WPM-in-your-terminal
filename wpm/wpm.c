#include <stdio.h>
#include <string.h>

#include <stdlib.h> //for rand
#include <time.h>//For time related stuff (not timer)

#include <unistd.h>//unix lib standard (and for alarm())
#include <termios.h>//yes its not ncurses kill me i know

#include <poll.h> //For poll timer
#define TEST_DURATION 60

//colours used
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m" 
#define COLOR_RED     "\033[31m"

#define maxlen 4096//Can anyone write more than this ??

static struct termios orig_termios;//Storing the users orignal terminal settings (DO NOT REMOVE)

int sourceText(int diff, char *dest, size_t max_len){ //WORKING YESSSS
    const char *filepath = NULL;
    switch (diff){
        case 1://easy
            filepath = "texts/easy.txt";break;
        case 2://medium
            filepath = "texts/medium.txt";break;
        case 3://hard
            filepath = "texts/hard.txt";break;}
    //We need to make the wpm exec where the texts folder is otherwise it stops working , need a fix for that.
    FILE *f = fopen(filepath, "r");
    if (!f){//file reading failure handling
        perror("Error opening text file");
        return 0;}

    //Trying to randomize the pick
    char buffer[4096];
    int line_count = 0;

    //getting line count of selected text file , for randomness
    while (fgets(buffer, sizeof(buffer), f)){
        if (strlen(buffer) > 1) line_count++;  // Skip empty newlines
    }
    if (line_count == 0){//empty file prank
        fclose(f);
        return 0;}

    //Picking a random line index
    int target_line = rand() % line_count;

    //Rewind back to the beginning of the file and finding target
    rewind(f);
    int i = 0; // Stores current line index
    while (fgets(dest, max_len, f)) {
        if (strlen(dest) <= 1) continue; // Skip blank lines
        if (i == target_line) {
            // Strip the trailing newline character (\r or \n)
            dest[strcspn(dest, "\r\n")] = '\0';
            break;}
        i++;
    }
    fclose(f);//good cleaning ethicates :) (remove if you dont love your ram)
    return 1;
}


//MAKING TERMINAL WRITE FUNCTIONS
// Restore terminal settings using the struct we saved ealier on exit
void disable_raw_mode(void) { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);} 
// Enable raw character-by-character input
void enable_raw_mode(void) {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {//IMPORTANT DO NOT REMOVE
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }//if we dont get the settings to restore , we dont go

    atexit(disable_raw_mode);//VERY IMPORTANT DO NOT REMOVE (cant believe a program can be this dangerous)

    struct termios raw = orig_termios;
    // Disabling canon mode (line buffering) and automatic echo so that the wpm can be like a wpm test
    raw.c_lflag &= ~(ICANON | ECHO);//changing the c_lflag attributes of ICANON and ECHO

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

//TIMER FUNCTIONS
void print_timer(int seconds_left) {
    // \033[s   : Save current cursor position
    // \033[H   : Move cursor to home (top-left) or specific line
    // \033[u   : Restore cursor position
    printf("\033[s\033[1;1H\033[33mTime Remaining: %02ds\033[0m\033[u", seconds_left);
    fflush(stdout);
}

//RUNNING FUNCTION
void run_test(const char *target_text) {
    enable_raw_mode();

    // Clear screen and leave the top line for the timer
    printf("\033[2J\033[2;1H");
    printf("=============================================== Target Text ===============================================\n%s\n", target_text);
    printf("========================================= Start typing below (Ctrl+C to quit) =========================================\n");

    int text_len = strlen(target_text);
    int current_idx = 0;
    int correct_chars = 0;
    int total_keystrokes = 0;
    int is_correct[4096] = {0};

    time_t start_time = time(NULL);
    int last_printed_remaining = -1;

    // Setup poll for stdin (file descriptor 0)
    struct pollfd pfd;
    pfd.fd = STDIN_FILENO;
    pfd.events = POLLIN;

    while (current_idx < text_len) {
        int elapsed = (int)difftime(time(NULL), start_time);
        int remaining = TEST_DURATION - elapsed;

        if (remaining <= 0) {
            print_timer(0);
            printf("\n\nTime's up! (60s elapsed)\n");
            break;
        }

        // Only redraw the timer text when the second actually ticks down
        if (remaining != last_printed_remaining) {
            print_timer(remaining);
            last_printed_remaining = remaining;
        }

        // Wait up to 100 milliseconds for a keypress
        int ret = poll(&pfd, 1, 100);

        if (ret < 0) {
            perror("poll");
            break;
        }

        // Timeout expired: no key pressed during the 100ms interval, loop around to update timer
        if (ret == 0) {
            continue;
        }

        // A key is ready to be read
        char ch;
        if (read(STDIN_FILENO, &ch, 1) <= 0) {
            continue;
        }

        // Ctrl+C handling
        if (ch == 3) {
            printf("\nOk bye bye.\n");
            break;
        }

        // Handle Backspace (127 or 8)
        if (ch == 127 || ch == 8) {
            if (current_idx > 0) {
                current_idx--;
                if (is_correct[current_idx]) {
                    correct_chars--;
                    is_correct[current_idx] = 0;
                }
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }

        // Skip non-printable characters
        if (ch < 32 || ch > 126) {
            continue;
        }

        total_keystrokes++;

        // Space blocking check
        if (target_text[current_idx] == ' ') {
            if (ch == ' ') {
                printf(" ");
                correct_chars++;
                is_correct[current_idx] = 1;
                current_idx++;
            } else {
                printf("%s%c%s\b \b", COLOR_RED, ch, COLOR_RESET);
            }
        } else {//Colouring book
            if (ch == target_text[current_idx]) {
                printf("%s%c%s", COLOR_GREEN, ch, COLOR_RESET);
                correct_chars++;
                is_correct[current_idx] = 1;
            } else {
                printf("%s%c%s", COLOR_RED, ch, COLOR_RESET);
                is_correct[current_idx] = 0;
            }
            current_idx++;
        }

        fflush(stdout);
    }

    printf("\n\nCharacters matched: %d / %d\n", correct_chars, total_keystrokes);
}

//MAIN PART
int main(){
    srand(time(NULL)); //for making the rng actually work. unix epoch time cool concept
    int choice = 0;
    char text[maxlen];
    printf("Choose Difficulty (1: Easy, 2: Medium, 3: Hard): ");
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input. Please enter a number.\n");
        return 1;
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF); //For flushing the remaining \n from scanf

    if (!sourceText(choice, text, maxlen)) {
        fprintf(stderr, "Failed to load passage. Exiting.\n");
        return 1;
    }

    run_test(text);
    return 0; //We have won.
}
