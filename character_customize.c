#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

char getcha(void) {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}
char *eight = "        ";
char eyes[7] = {'0', '-', '@', '$', 'O', '^', ' '};
char *mouth[6] = {" \\__/ ", " WWWW ", "~~~~~~", "(VVVV)", " <==> ", "      "};
char *head[5][7] = {
    {"   /^^^^^^\\", "  /        \\", "  |  ", "  |", "  | ", " |", "   \\______/"},
    {" ____________", "|            |", "|    ", "    |", "|   ", "   |", "|____________|"},
    {"   ________", "  /        \\", " /   ", "   \\", " \\  ", "  /", "  \\________/"},
    {"   ||||||||", "  /        \\", "  \\  ", "  /", "  / ", " \\", "  \\________/"},
    {"   __    __", "  /  \\__/  \\", "  \\  ", "  /", "  / ", " \\", "  \\/\\____/\\/"}
};
void print_char(int x, int y, int z){
    printf("%s%s\n%s%s\n%s%s%c  %c%s\n%s%s%s%s\n%s%s\n      ______|    |______\n     /                  \\\n\n\n",eight, head[x][0],  eight, head[x][1],  eight, head[x][2], eyes[y], eyes[y], head[x][3],  eight, head[x][4], mouth[z], head[x][5],  eight, head[x][6]);
}
int main(){
    printf("\e[8;28;149t");
    printf("\e[?25l");
    fflush(stdout); system("clear");
    bool doneflag = 1;
    char input = 0;
    int c_y = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    while(doneflag){
        fflush(stdout); system("clear");
        if (c_y == 0) printf("-");
        printf("HEAD\n");
        if (c_y == 1) printf("-");
        printf("EYES\n");
        if (c_y == 2) printf("-");
        printf("HEAD\n\n\n");
        print_char(x, y, z);
        input = getcha();
        if (input == 91){
            input = getcha();
            if (input == 'A'){ //up
                c_y--;
                if (c_y == -1) c_y = 2;
            }
            else if (input == 'B'){ //down
                c_y++;
                if (c_y == 3) c_y = 0;
            }
            else if (input == 'D'){
                if (c_y == 0){
                    x--;
                    if (x == -1) x = 4;
                }
                if (c_y == 1){
                    y--;
                    if (y == -1) y = 6;
                }
                if (c_y == 2){
                    z--;
                    if (z == -1) z = 5;
                }
            }
            else if (input == 'C'){
                if (c_y == 0){
                    x++;
                    if (x == 5) x = 0;
                }
                if (c_y == 1){
                    y++;
                    if (y == 7) y = 0;
                }
                if (c_y == 2){
                    z++;
                    if (z == 6) z = 0;
                }
            }
            else if (input == 10){
                doneflag = 0;
            }
            else {

            }
        }
        else {
            if (input == 10){
                doneflag = 0;
            }
        }
    }
}
