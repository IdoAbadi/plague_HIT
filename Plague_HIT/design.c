#include <stdio.h>
#include "design.h"
#include <windows.h>

const char* RED = "\033[1;31m";
const char* YELLOW = "\033[1;33m";
const char* ORANGE = "\033[38;5;208m";
const char* BLUE = "\033[1;34m";
const char* GREEN = "\033[1;32m";
const char* CYAN = "\033[1;36m";
const char* PURPLE = "\033[0;35m";
const char* PINK = "\033[1;35m";
const char* RESET = "\033[0m";

void print_colored(const char* text, const char* color_code) {
    printf("%s%s\033[0m", color_code, text);
}

void print_welcome_message() {
    print_colored("===============================================================================================================\n", YELLOW);
    print_colored("                                    Welcome to Plague H.I.T!\n", BLUE);
    print_colored("===============================================================================================================\n", YELLOW);
    print_colored("                    A simulation of survival, strategy, and the unstoppable spread of disease.\n", BLUE);
    print_colored("===============================================================================================================\n", YELLOW);
    print_colored("                              Brace yourself... the outbreak begins!\n", BLUE);
    print_colored("===============================================================================================================\n", YELLOW);
    Sleep(3000);
}

void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}