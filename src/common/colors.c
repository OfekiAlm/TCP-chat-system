#include "colors.h"

#define RED_CODE "\033[0;31m"
#define YELLOW_CODE "\033[0;33m"
#define CYAN_CODE "\033[0;36m"
#define PURPLE_CODE "\033[0;35m"
#define GREEN_CODE "\033[0;32m"
#define WHITE_CODE "\033[0;37m"

char* getColorCode(int color){
    switch (color)
    {
    case RED:
        return RED_CODE;
        break;
    case YELLOW:
        return YELLOW_CODE;
        break;
    case CYAN:
        return CYAN_CODE;
        break;
    case PURPLE:
        return PURPLE_CODE;
        break;
    case GREEN:
        return GREEN_CODE;
        break;
    case WHITE:
        return WHITE_CODE;
        break;
    default:
        return "\033[0;37m";
        break;
    }
}