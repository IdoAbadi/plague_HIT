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

void PrintColored(const char* text, const char* color_code) {
    printf("%s%s\033[0m", color_code, text);
}

void PrintWelcomeMessage() {
    PrintColored("===============================================================================================================\n", YELLOW);
    PrintColored("                                          Welcome to Plague H.I.T!\n", BLUE);
    PrintColored("===============================================================================================================\n", YELLOW);
    PrintColored("                    A simulation of survival, strategy, and the unstoppable spread of disease.\n", BLUE);
    PrintColored("===============================================================================================================\n", YELLOW);
    PrintColored("                              Brace yourself... the outbreak begins!\n", BLUE);
    PrintColored("===============================================================================================================\n\n", YELLOW);
    Sleep(3000);
}

void ClearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}