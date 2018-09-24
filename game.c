//REILLY JENSEN ARENA GAME 8-15-18
//**************************************************************INCLUDES**************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

//**************************************************************DEFINITIONS**********************************************************

//PLAYER COEFFICIENTS These are used to balance the game properly 
// Heal function is too complicated to use simple definitions
#define PLAYER_HEAL_PER_FIGHT_COEFF .08 //Percent of max health you heal after each fight per heal skill
#define PLAYER_FIRE_COEFF 6 //This is multiplied by fire skill for base fire damage 
#define PLAYER_SWORD_COEFF 8 //This times sword skill gives base output damage 
#define PLAYER_BLOCK_COEFF .08 //Amount of damage percent removed per skill of block 
#define PLAYER_DODGE_COEFF .065 //Chance to dodge per skill level 
#define PLAYER_POWER_COEFF .03 //Percent increase in damage per level 
#define PLAYER_ARMOR_COEFF .05 //How much damage percent gets lowered by armor skill

//PLAYER MODIFIERS

//#define PLAYER_HEAL_MOD 5 Player heal function has too many coefficients 
#define PLAYER_FIRE_MOD 60 //Base modifier of fire damage 
#define PLAYER_SWORD_MOD 20 //Variance of extra sword damage 
#define PLAYER_BLOCK_MOD 5 //Variance of extra blockage 
#define PLAYER_POWER_MOD 3 //Variance of extra damage 

//PLAYER PERFECTS 
#define PERFECT_FIRE_MOD 400//Increase in probability range 
#define PERFECT_SWORD_MOD 120 //Raw damage increase 
#define PERFECT_ARMOR_MOD 15//chance to activate-1
#define PERFECT_POWER_MOD 15//chance to activate-1
#define PERFECT_UNLOCK_LEVEL 5 //The level at which perfect attacks become available -1
#define PERFECT_POWER_MULTIPLIER 2//how much does it multiply damage by??
#define PLAYER_MAX_HEALTH_PER_PERFECT_HEAL 10
//Perfect heal, dodge, and block have no probability elements

//MISC PLAYER

#define PLAYER_HP_PER_LEVEL 20 
#define MAX_NAME_SIZE 21
#define PLAYER_STAT_MAX 10

//NPC COEFFICIENTS

#define NPC_HEAL_COEFF .08 //Multiplied by heal skill to heal after each round 
#define NPC_FIRE_COEFF 8 //Multiplied by fire skill for base damage 
#define NPC_SWORD_COEFF 11 //Multiplied by sword skill for base damage 
#define NPC_ARMOR_COEFF .05 //Percent damage not taken per armor skill 
#define NPC_BLOCK_COEFF .07 //Percent damage blocked by block skill 
#define NPC_DODGE_COEFF .04 //Percent chance per dodge skill to dodge 
#define NPC_POWER_COEFF .025 //Percent increase in damage per power skill 

//NPC MODIFIERS

#define NPC_HEAL_MOD 4 //Most amount of extra possible heal from probability 
#define NPC_FIRE_MOD 6 //This times fire skill makes max modifier possible 
#define NPC_SWORD_MOD 35 //Sword max random modifier 
#define NPC_BLOCK_MOD 5 //Possible extra block points 

//MISC NPC 
#define MAX_NPC_NAME_SIZE 21

//**************************************************************GLOBAL VARIABLES AND STRUCTS**************************************************

struct Player_Stats {
  char name[MAX_NAME_SIZE];
  int block;
  int dodge;
  int sword;
  int fire;
  int heal;
  int armor;
  int power;
  float maxhealth;
  float health;
  int perf_block;
  int perf_dodge;
  int perf_sword;
  int perf_heal;
  int perf_armor;
  int perf_fire;
  int perf_power;
};
struct NPC {
  int block;
  int dodge;
  int sword;
  int fire;
  int armor;
  int heal;
  int power;
  float health;
  float maxhealth;
  char name[MAX_NPC_NAME_SIZE];
};

//Globals for stats
int total_fire_attacks=0;
float total_damage_by_fire=0;
int total_sword_attacks=0;
float total_damage_by_sword=0;
int total_heals=0;
int total_dodges=0;
int total_blocks=0;
float total_damage_blocked=0;
float total_damage_negated_by_armor=0;
float total_damage_boosted_by_power=0;
float total_damage_taken=0;
float total_damage_given=0;
float strongest_ever_attack=0;
float strongest_ever_attack_survived=0;
float most_health=100;
int total_perfect_fires=0;
int total_perfect_swords=0;
int total_perfect_heals=0;
int total_perfect_dodges=0;
int total_perfect_blocks=0;
int total_perfect_armors=0;
int total_perfect_powers=0;
int total_enemies_slain=0;
int final_bosses_killed=0;
int perfect_state=0;
int type_speed_setting = 15000;

char journal[25000][72];
int journal_status[25000];
int journal_entry=0;

//**************************************************************FUNCTION FOREWARD DECLARATIONS*********************************************************

void reset_npc_stats(struct NPC* NPC);
void reset_player_stats(struct Player_Stats* p);
void gift_player_stats(struct Player_Stats* p);
void tutorial(void);
void endscreen(struct Player_Stats* p);
void npc_taunts(void);
void npc_death_sounds(void);
void npc_victory_sounds(void);
void check_player_health(struct Player_Stats* p);
int check_npc_health(struct NPC* p);
void skill_upgrade(int i,struct Player_Stats* p);
float player_block(float damage,struct Player_Stats* p);
float player_armor(float damage,struct Player_Stats* p);
int player_dodge(struct Player_Stats* p);
float player_fire(struct Player_Stats* p);
float player_sword(struct Player_Stats* p);
void player_heal(struct Player_Stats* p);
void player_defense(float damage,struct Player_Stats* p);
float npc_block(float damage,struct NPC* p);
int npc_dodge(struct NPC* p);
float npc_armor(float damage,struct NPC* p);
void npc_defense(float damage,struct NPC* p);
float player_attack(struct Player_Stats* p);
float npc_sword(struct NPC* p);
float npc_fire(struct NPC* p);
void battle(struct NPC* p,struct Player_Stats* q);
float npc_attack(struct NPC* p);
void  battle_screen (int state,struct NPC* p, struct Player_Stats* q);
char getcha(void);
void printstring(char* s);
void print_log();
void intro(char* s);
void name_input(struct Player_Stats* p);
void go_on(void);
float player_power(float damage,struct Player_Stats *p);
float npc_power(float damage, struct NPC* p);
void print_battle_screen_main (struct NPC* p, struct Player_Stats* q);
int battle_buttons(int button_types);
void submit_log(char input[],int status, int print_or_not);

//Foreward declarations of skill upgrade functions 
void print_divide(void);
void print_pressed(void);
void print_unpressed(void);
void print_unpressed2(void);
void print_locked(void);
void print_locked2(void);
void print_stat_upgrade_intro(int x);
void print_stat_upgrade_dodge(int x, int button_state);
void print_stat_upgrade_block(int x, int button_state);
void print_stat_upgrade_armor(int x, int button_state);
void print_stat_upgrade_fire(int x, int button_state);
void print_stat_upgrade_sword(int x, int button_state);
void print_stat_upgrade_power(int x, int button_state);
void print_stat_upgrade_heal(int x, int button_state);
void print_stat_upgrade_health(double x, int button_state);
void print_reset_button(int button_state);
void print_done_button(int button_state);
void print_stat_upgrade(int x,int y, struct Player_Stats* p, bool reset_button_state);
void print_like_typing(char *s, int speed);

//**************************************************************RESET FUNCTIONS**************************************************************

void reset_npc_stats(struct NPC* NPC){//Used between fights to make sure there is no spill over
  NPC->block=0;
  NPC->dodge=0;
  NPC->sword=0;
  NPC->power=0;
  NPC->heal=0;
  NPC->fire=0;
  NPC->armor=0;
  NPC->health=100;
  NPC->maxhealth=100;
}
void reset_player_stats(struct Player_Stats* p){//When the game ends the player stats are wiped
  p->block=0;
  p->dodge=0;
  p->sword=0;
  p->fire=0;
  p->heal=0;
  p->armor=0;
  p->power=0;
  p->perf_block=0;
  p->perf_power = 0;
  p->perf_dodge=0;
  p->perf_fire=0;
  p->perf_sword=0;
  p->perf_heal=0;
  p->perf_armor=0;
  p->health=100;
  p->maxhealth=100;
}
void gift_player_stats(struct Player_Stats* p){
  p->block=10;
  p->dodge=10;
  p->sword=10;
  p->fire=10;
  p->heal=10;
  p->power=10;
  p->armor=10;
  p->maxhealth=100;
  p->health=100;
  p->perf_block=1;
  p->perf_dodge=1;
  p->perf_sword=1;
  p->perf_heal=1;
  p->perf_armor=1;
  p->perf_fire=10;
  p->perf_power=1;
}

//*********************************************************UTILITY FUNCTIONS******************************************************************
void clear_log(){
    for(int i=0;i<21;i++){
        printf("\033[%d;%dH%s", 2+i, 39, "                                                                         "); fflush(stdout);
    }
}

void print_log(void){
    clear_log();

    journal_entry--;
  for(int i=0;i<9;i++){
    if(journal_entry-9+i>-1){
      printf("\033[%d;%dH%s", 2+2*i, 40,journal[journal_entry-9+i]); fflush(stdout);
    }}
    printf("\033[%d;%dH", 20, 40); fflush(stdout);
    print_like_typing(journal[journal_entry], type_speed_setting);
    journal_entry++;
}
void print_journal(void){
  /*
  //Fill log
  for(int k=0;k<25000;k++){
    for(int j=0;j<71;j++){
      journal[k][j]=j;
    }
    journal[k][71]='\0';
    }*/
  
  //print log
  for(int i=0;i<25000;i++){
    if(journal_status[i]==0){continue;}
    printf("Entry %d : %s\n",i,journal[i]);}

}
void name_input(struct Player_Stats* p){
    int i = 0;
    char temp = 0;
    char string[MAX_NAME_SIZE];
    while(temp != 10){
        system("clear");
        printf("Please enter your name: "); fflush(stdout);
        for (int j = 0; j < i; j++){
            printf("%c", string[j]); fflush(stdout);
        }
        temp = getcha();
        if (temp == 10){
            if (i == 0){
                temp = 0;
            }
        }
        else if (temp == 8){
            if (i != 0){
                string[i-1] = 0;
                i--;
            }
        }
        else if (i != 20){
            if (temp > 31 && temp < 127){
                string[i] = temp;
                i++;
            }
        }
    }
    while(i < MAX_NAME_SIZE){
        string[i] = ' ';
        i++;
    }
    string[MAX_NAME_SIZE - 1] = '\0';
    for (int i = 0; i < MAX_NAME_SIZE; i++){
        p->name[i] = string[i];
    }
    return;
}

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

void go_on(void){//Asks user to press any key to continue
    int temp = 0;
    while(temp == 0) temp = system("read -t 1 -n 10000 discard");
    printf("\033[22;40H                       PRESS ANY KEY TO CONTINUE"); fflush(stdout);
    getcha();
}

void submit_log(char input[], int status,int print_or_not){
    for (int i = 0; i < 71; i++){
        journal[journal_entry][i] = input[i];
    }
    journal_status[journal_entry]=status;//1 is save to log ,0 is no save to log 
    journal[journal_entry][71] = '\0';
    journal_entry++;
    if(print_or_not==0){return;}
    print_log();
}

//**************************************************************PRINT FUNCTIONS**************************************************************

void print_like_typing(char* s, int speed){ //For printing text like its being typed
    srand(time(0));
    int i = 0;
    while (s[i] != '\0'){
        printf("%c", s[i]);
        fflush(stdout);
        usleep(speed);
        i++;
    }
}

void intro(char* s){
    system("clear");
    print_like_typing(s, type_speed_setting);
    printf("\n");
    go_on();
}

void tutorial(void)
{
  intro("Greetings new arena combatant! Your job is to somehow survive through all the bad guys in this arena!\n\nAll combat is 1 on 1\n\nCombat is turn based. During your turn you can either attack or heal.\nAttacking is done with either sword or fire.\nThe sword does more base damage but has a lower random modifier, the fire attack is the opposite!\n\nWhen defending you can either dodge or block.\nDodging has a percent chance of succeeding, if it works you avoid ALL damage!\nBlocking gaurantees you reduce some amount of damage but you will never fully eliminate damage with a block!\nNo matter what you do when you defend, your armor skill will absorb some of the damage.\n\nOver time you will aquire points to spend on upgrading your character's skills.\n10 points is mastery of a skill, at 10 you get a special ability!\nGood Luck!\n");
}

void endscreen (struct Player_Stats* p){//Prints the ending of the game SHOULD MAKE A SCREEN FOR THIS 
  printf("You lost the game! Game over man! game over!\n");
  printf("Your final stats were....\n");
  //print_stats(p);
  reset_player_stats(p);
  exit(0);
}

void  battle_screen (int state,struct NPC* p, struct Player_Stats* q){//does all battle screen functions at once
    print_battle_screen_main(p, q);
    print_log();
    battle_buttons(3);
   return;
 }
 
//**************************************************************CHECK FUNCTIONS************************************************************

void check_player_health(struct Player_Stats* p){//If player is dead game will end
  if(p->health<0){endscreen(p);}
  return;
}

int check_npc_health(struct NPC* p){//If the NPC is dead it will return a 0, 1 if alive
  if(p->health<0){
    p->health=0;
    printf("| The NPC is dead!                                                           |\n");//Eliminate printfs
  //printf("|----------------------------------------------------------------------------|\n");
    return 0;}
  return 1;
}

//**************************************************************SKILL UPGRADE FUNCTIONS********************************************************

//     DO NOT CHANGE ORDER!!!!  (I foreward declared them anyway) 
void print_divide(){
    printf("0=======================================0\n");
}
void print_pressed(){
    printf("||\033[46m ");
}
void print_pressed2(){
    printf(" \033[49m||\n");
}
void print_unpressed(){
    printf("|| ");
}
void print_unpressed2(){
    printf(" ||\n");
}
void print_locked(){
    printf("||\033[37m ");
}
void print_locked2(){
    printf(" \033[30m||\n");
}
typedef void (*stat_button)(void);
stat_button const stat_buttons[] = {
    print_unpressed,
    print_pressed,
    print_locked,
};
typedef void (*stat_button2)(void);
stat_button2 const stat_buttons2[] = {
    print_unpressed2,
    print_pressed2,
    print_locked2,
};
void print_stat_upgrade_intro(int x){
    print_unpressed(); printf("Skill Upgrades Left : %d          ", x);
    if (x < 100) printf(" ");
    if (x < 10) printf(" ");
    print_unpressed2();
}
void print_stat_upgrade_dodge(int x, int button_state){
    stat_button s_button = stat_buttons[button_state];
    stat_button2 s_button2 = stat_buttons2[button_state];
    s_button(); printf("+1 Dodge (d)                       "); s_button2();
    s_button(); printf("Defense              %5d/10      ", x); s_button2();
    s_button(); printf("Luck based evasion of taken damage "); s_button2();
}
void print_stat_upgrade_block(int x, int button_state){
    stat_button s_button = stat_buttons[button_state];
    stat_button2 s_button2 = stat_buttons2[button_state];
    s_button(); printf("+1 Block (b)                       "); s_button2();
    s_button(); printf("Defense              %5d/10      ", x); s_button2();
    s_button(); printf("Gaurantee incoming damage reduction"); s_button2();
}
void print_stat_upgrade_armor(int x, int button_state){
    stat_button s_button = stat_buttons[button_state];
    stat_button2 s_button2 = stat_buttons2[button_state];
    s_button(); printf("+1 Armor (a)                       "); s_button2();
    s_button(); printf("Defense              %5d/10      ", x); s_button2();
    s_button(); printf("Constantly lowers received damage  "); s_button2();
}
void print_stat_upgrade_fire(int x, int button_state){
    stat_button s_button = stat_buttons[button_state];
    stat_button2 s_button2 = stat_buttons2[button_state];
    s_button(); printf("+1 Fire (f)                        "); s_button2();
    s_button(); printf("Offense              %5d/10      ", x); s_button2();
    s_button(); printf("Chance to incur high damage        "); s_button2();
}
void print_stat_upgrade_sword(int x, int button_state){
    stat_button s_button = stat_buttons[button_state];
    stat_button2 s_button2 = stat_buttons2[button_state];
    s_button(); printf("+1 Sword (s)                       "); s_button2();
    s_button(); printf("Offense              %5d/10      ", x); s_button2();
    s_button(); printf("Consistent damage output           "); s_button2();
}
void print_stat_upgrade_power(int x, int button_state){
    stat_button s_button = stat_buttons[button_state];
    stat_button2 s_button2 = stat_buttons2[button_state];
    s_button(); printf("+1 Power (p)                       "); s_button2();
    s_button(); printf("Offense              %5d/10      ", x); s_button2();
    s_button(); printf("Always increases given damage      "); s_button2();
}
void print_stat_upgrade_heal(int x, int button_state){
    stat_button s_button = stat_buttons[button_state];
    stat_button2 s_button2 = stat_buttons2[button_state];
    s_button(); printf("+1 Healing (h)                     "); s_button2();
    s_button(); printf("Utility              %5d/10      ", x); s_button2();
    s_button(); printf("Heal instead of attacking          "); s_button2();
}
void print_stat_upgrade_health(double x, int button_state){//Why is this a double and not a float? 
    stat_button s_button = stat_buttons[button_state];
    stat_button2 s_button2 = stat_buttons2[button_state];
    s_button(); printf("+%d Max HP (m)                   ",PLAYER_HP_PER_LEVEL);
    if (PLAYER_HP_PER_LEVEL < 1000) printf(" ");
    if (PLAYER_HP_PER_LEVEL < 100) printf(" ");
    if (PLAYER_HP_PER_LEVEL < 10) printf(" ");
    s_button2();
    s_button(); printf("Utility              %8.2f      ", x); s_button2();
    s_button(); printf("Makes it harder to die             "); s_button2();
}
void print_reset_button(int button_state){
    stat_button s_button = stat_buttons[button_state];
    stat_button2 s_button2 = stat_buttons2[button_state];
    s_button(); printf("              RESET                "); s_button2();
}
void print_done_button(int button_state){
    stat_button s_button = stat_buttons[button_state];
    stat_button2 s_button2 = stat_buttons2[button_state];
    s_button(); printf("              DONE                 "); s_button2();
}
void print_stat_upgrade(int x, int y, struct Player_Stats* p, bool reset_button_state){
    //x: upgrade points, y: which button is selected
    printf("\e[8;41;41t"); fflush(stdout);
    system("clear");
    int button_state = 0;
    print_divide();
    print_stat_upgrade_intro(x);
    print_divide();
    button_state = 0;
    if (y == 0) button_state = 1;
    if (p->dodge == 10 || x == 0) button_state = 2;
    print_stat_upgrade_dodge(p->dodge, button_state);
    print_divide();
    button_state = 0;
    if (y == 1) button_state = 1;
    if (p->block == 10 || x == 0) button_state = 2;
    print_stat_upgrade_block(p->block, button_state);
    print_divide();
    button_state = 0;
    if (y == 2) button_state = 1;
    if (p->armor == 10 || x == 0) button_state = 2;
    print_stat_upgrade_armor(p->armor, button_state);
    print_divide();
    button_state = 0;
    if (y == 3) button_state = 1;
    if (p->fire == 10 || x == 0) button_state = 2;
    print_stat_upgrade_fire(p->fire, button_state);
    print_divide();
    button_state = 0;
    if (y == 4) button_state = 1;
    if (p->sword == 10 || x == 0) button_state = 2;
    print_stat_upgrade_sword(p->sword, button_state);
    print_divide();
    button_state = 0;
    if (y == 5) button_state = 1;
    if (p->power == 10 || x == 0) button_state = 2;
    print_stat_upgrade_power(p->power, button_state);
    print_divide();
    button_state = 0;
    if (y == 6) button_state = 1;
    if (p->heal == 10 || x == 0) button_state = 2;
    print_stat_upgrade_heal(p->heal, button_state);
    print_divide();
    button_state = 0;
    if (y == 7) button_state = 1;
    if (x == 0) button_state = 2;
    print_stat_upgrade_health(p->maxhealth, button_state);
    print_divide();
    button_state = 0;
    if (y == 8) button_state = 1;
    if (reset_button_state) button_state = 2;
    print_reset_button(button_state);
    print_divide();
    button_state = 2;
    if (x == 0) button_state = 0;
    if (y == 9) button_state = 1;
    print_done_button(button_state);
    print_divide();
}

void skill_upgrade(int i,struct Player_Stats* p)
{
    struct Player_Stats q = *p;
    int temp_i = i;
    int button_state = 0;
    //button_state: which button is selected
    if (p->dodge == 10){ button_state = 1;
        if (p->block == 10){ button_state = 2;
            if (p->armor == 10){ button_state = 3;
                if (p->fire == 10){ button_state = 4;
                    if (p->sword == 10){ button_state = 5;
                        if (p->power == 10){ button_state = 6;
                            if (p->heal == 10){ button_state = 7;
    }   }   }   }   }   }   }
    bool doneflag = 1;
    char input = 0;
    while(doneflag){
      restart:
        ; //can't declare variable after : (bullshit)
      bool reset_button_state = 0;
      if (temp_i == i) reset_button_state = 1;
      print_stat_upgrade(i, button_state, p, reset_button_state);
      input = getcha();
      if (input == 91){
          input = getcha();
          if (input == 'A'){ //up
              button_state--;
              if (button_state < 0){
                  button_state = 9;
              }
          }
          else if (input == 'B'){ //down
              button_state++;
              if (button_state > 9){
                  button_state = 0;
              }
          }
          else if (input == 'C'){
              goto restart; //do nothing
          }
          else if (input == 'D'){
              goto restart; //do nothing
          }
          else if (input == 10){
              //they pressed esc then enter
              switch(button_state){
                  case 0:
                      p->dodge++;
                      i--;
                      break;
                  
                  case 1:
                      p->block++;
                      i--;
                      break;
              
                  case 2:
                      p->armor++;
                      i--;
                      break;
          
                  case 3:
                      p->fire++;
                      i--;
                      break;
        
                  case 4:
                      p->sword++;
                      i--;
                      break;
    
                  case 5:
                      p->power++;
                      i--;
                      break;

                  case 6:
                      p->heal++;
                      i--;
                      break;

                  case 7:
                      p->maxhealth+=PLAYER_HP_PER_LEVEL;
                      i--;
                      break;

                  case 8:
                      i = temp_i;
                      *p = q;
                      break;

                  case 9:
                      doneflag = 0;
                      break;

                  default:
                      goto restart;//this should never happen!
                      break;

              }
              if (i == 0){
                  button_state = 8;
              }
          }
          else {
              //they pressed the esc key?
              goto restart; //??
          }
          
          //check grayed out button after down arrow (DO NOT CHANGE ORDER!)
          if (button_state == 0 && i == 0) button_state = 8;
          if (button_state == 7 && i == 0) button_state = 9;
          if (button_state == 8 && input == 'B' && temp_i == i) button_state++;
          if (i > 0 && button_state == 9 && input == 'B') button_state = 0;
          if (p->dodge == 10 && button_state == 0 && input == 'B') button_state++;
          if (p->block == 10 && button_state == 1 && input == 'B') button_state++;
          if (p->armor == 10 && button_state == 2 && input == 'B') button_state++;
          if (p->fire == 10 && button_state == 3 && input == 'B') button_state++;
          if (p->sword == 10 && button_state == 4 && input == 'B') button_state++;
          if (p->power == 10 && button_state == 5 && input == 'B') button_state++;
          if (p->heal == 10 && button_state == 6 && input == 'B') button_state++;
          
          //check grayed out button after up arrow (DO NOT CHANGE ORDER!)
          if (p->heal == 10 && button_state == 6 && input == 'A') button_state--;
          if (p->power == 10 && button_state == 5 && input == 'A') button_state--;
          if (p->sword == 10 && button_state == 4 && input == 'A') button_state--;
          if (p->fire == 10 && button_state == 3 && input == 'A') button_state--;
          if (p->armor == 10 && button_state == 2 && input == 'A') button_state--;
          if (p->block == 10 && button_state == 1 && input == 'A') button_state--;
          if (p->dodge == 10 && button_state == 0 && input == 'A') button_state = 9;
          if (i > 0 && button_state == 9 && input == 'A') button_state--;
          if (button_state == 8 && input == 'A' && temp_i == i) button_state--;
      }
      else {
          switch(input){
              case 'b':
                  if (p->block < 10 && i > 0){
                      p->block++;
                      i--;
                      if (i == 0){
                          button_state = 8;
                      }
                  }
                  break;
              case 'd':
                  if (p->dodge < 10 && i > 0){
                      p->dodge++;
                      i--;
                      if (i == 0){
                          button_state = 8;
                      }
                  }
                  break;
              case 's':
                  if (p->sword < 10 && i > 0){
                      p->sword++;
                      i--;
                      if (i == 0){
                          button_state = 8;
                      }
                  }
                  break;
              case 'f':
                  if (p->fire < 10 && i > 0){
                      p->fire++;
                      i--;
                      if (i == 0){
                          button_state = 8;
                      }
                  }
                  break;
              case 'h':
                  if (p->heal < 10 && i > 0){
                      p->heal++;
                      i--;
                      if (i == 0){
                          button_state = 8;
                      }
                  }
                  break;
              case 'p':
                  if (p->power < 10 && i > 0){
                      p->power++;
                      i--;
                      if (i == 0){
                          button_state = 8;
                      }
                  }
                  break;
              case 'm':
                  if (i > 0){
                      p->maxhealth+=PLAYER_HP_PER_LEVEL;
                      i--;
                      if (i == 0){
                          button_state = 8;
                      }
                  }
                  break;
              case 'a':
                  if (p->armor < 10 && i > 0){
                      p->armor++;
                      i--;
                      if (i == 0){
                          button_state = 8;
                      }
                  }
                  break;
              case 10:
                  switch(button_state){
                      case 0:
                          if (i > 0){
                              p->dodge++;
                              i--;
                          }
                          break;
                      
                      case 1:
                          if (i > 0){
                              p->block++;
                              i--;
                          }
                          break;
                  
                      case 2:
                          if (i > 0){
                              p->armor++;
                              i--;
                          }
                          break;
          
                      case 3:
                          if (i > 0){
                              p->fire++;
                              i--;
                          }
                          break;
        
                      case 4:
                          if (i > 0){
                              p->sword++;
                              i--;
                          }
                          break;
    
                      case 5:
                          if (i > 0){
                              p->power++;
                              i--;
                          }
                          break;

                      case 6:
                          if (i > 0){
                              p->heal++;
                              i--;
                          }
                          break;

                      case 7:
                          if (i > 0){
                              p->maxhealth+=PLAYER_HP_PER_LEVEL;
                              i--;
                          }
                          break;

                      case 8:
                          i = temp_i;
                          *p = q;
                          break;

                      case 9:
                          doneflag = 0;
                          break;

                      default:
                          goto restart;//this should never happen!
                          break;

                  }
                  if (i == 0){
                      button_state = 8;
                  }
                  break;
              default:
                  goto restart;
                  break;
          }
          if (button_state == 8 && temp_i == i) button_state++;
          if (i > 0 && button_state == 9) button_state = 0;
          if (p->dodge == 10 && button_state == 0) button_state++;
          if (p->block == 10 && button_state == 1) button_state++;
          if (p->armor == 10 && button_state == 2) button_state++;
          if (p->fire == 10 && button_state == 3) button_state++;
          if (p->sword == 10 && button_state == 4) button_state++;
          if (p->power == 10 && button_state == 5) button_state++;
          if (p->heal == 10 && button_state == 6) button_state++;
      }
    }
}

//**************************************************************PLAYER DEFENSE FUNCTIONS**************************************************************


float player_block(float damage,struct Player_Stats* p)
{
  char journal_block[72];
  snprintf(journal_block,72,"You decided to block!");
  submit_log(journal_block,1,1);
  char input;
  int savedamage;
  if(p->block>PERFECT_UNLOCK_LEVEL && p->perf_block !=0){
    snprintf(journal_block,72,"Do you want to use a perfect block?");
    submit_log(journal_block,0,1);
    input=battle_buttons(2);
    if(input==0){
      damage=0;
      snprintf(journal_block,72,"You performed a perfect block and received zero damage!");
      submit_log(journal_block,1,1);
      p->perf_block--;
      return damage;}
    else{
    }
  }
  savedamage=damage;
  time_t t;
  int extra_block=0;
  extra_block=( rand () % PLAYER_BLOCK_MOD);
  if(p->block==0){damage=damage*.95-extra_block;
    if(damage<0){damage=0;}
    snprintf(journal_block,72,"Your normal block stopped %f damage",savedamage-damage);
    submit_log(journal_block,1,1);
    return damage;}
  damage=damage*(1-(p->block*PLAYER_BLOCK_COEFF))-extra_block;
  if(damage<0){damage=0;}
  snprintf(journal_block,72,"Your normal block stopped %f damage",savedamage-damage);
  submit_log(journal_block,1,1);
  return damage;
}
float player_armor(float damage,struct Player_Stats* p){
  char journal_armor[72];
  time_t t;
  int threshold=0;
  int random_number=0;
  random_number=( rand () % 100); 
  if(p->armor>PERFECT_UNLOCK_LEVEL && p->perf_armor!=0 && damage!=0){
    snprintf(journal_armor,72,"You have a %d chance to take zero damage with perfect armor!",PERFECT_ARMOR_MOD);
    submit_log(journal_armor,0,1);
    if(random_number<16){
      snprintf(journal_armor,72,"You used a perfect armor!");
      submit_log(journal_armor,1,1);
      p->perf_armor--;
      return 0;
    }}
  float savedamage=damage;
  damage=damage*(1-(p->armor*PLAYER_ARMOR_COEFF));
  snprintf(journal_armor,72,"Your armor stopped %f damage!",savedamage-damage);
  submit_log(journal_armor,1,1);
  return damage;
}
int player_dodge(struct Player_Stats* p){
  char journal_dodge[72];
  char input=0;
  if(p->dodge>PERFECT_UNLOCK_LEVEL && p->perf_dodge!=0){
    snprintf(journal_dodge,72,"Do you want to use a perfect dodge?");
    submit_log(journal_dodge,0,1);
    input=battle_buttons(2);
    if(input==0){
      snprintf(journal_dodge,72,"You performed a perfect dodge!");
      submit_log(journal_dodge,1,1);
      p->perf_dodge--;
      return 1;
    }
    else{
      snprintf(journal_dodge,72,"You did not perform a perfect dodge!");
      submit_log(journal_dodge,1,1);
    }
  }
  time_t t;
  int threshold=0;
  int random_number=0;
  random_number=( rand () % 100);
  threshold=100*(p->dodge*PLAYER_DODGE_COEFF);
  snprintf(journal_dodge,72,"Your dodge skills give you a %d to dodge",threshold);
  submit_log(journal_dodge,1,1);
  if(random_number<threshold){
    snprintf(journal_dodge,72,"You dodged and avoided all damage!");
    submit_log(journal_dodge,1,1);
    return 1;}
  snprintf(journal_dodge,72,"You did not dodge.");
  submit_log(journal_dodge,1,1);
  return 0;
}
float player_heal_after_battle(struct Player_Stats* p){
   float savehealth;
   char journal_heal[72];
   savehealth=p->health;
   if(p->heal==0){
     p->health=1.05*p->health;
     if(p->health>p->maxhealth){p->health=p->maxhealth;}
     snprintf(journal_heal,72,"After the battle you only healed %f because you have 0 healing skill",p->health-savehealth);
     submit_log(journal_heal,1,1);
     return p->health-savehealth;
   }
   p->health=p->health+p->maxhealth*(PLAYER_HEAL_PER_FIGHT_COEFF*p->heal);
   if(p->health>p->maxhealth){p->health=p->maxhealth;}
   snprintf(journal_heal,72,"After the battle you healed %f becuase of your healing skills!",p->health-savehealth);
   submit_log(journal_heal,1,1);
   return p->health-savehealth;
 }

//**************************************************************PLAYER ATTACK FUNCTIONS**************************************************************

float player_fire(struct Player_Stats* p){
  char input;
  char journal_fire[72];
  int perfect_modifier=0;
  snprintf(journal_fire,72,"You decide to use a fire attack!");
  submit_log(journal_fire,1,1);
   if(p->fire>PERFECT_UNLOCK_LEVEL && p->perf_fire !=0){
    snprintf(journal_fire, 72, "Do you want to augment your attack with a perfect fire?");
    submit_log(journal_fire,0,1);
       input = battle_buttons(2); //2 means yes or no
       //returns 0: yes, 1: no
    if(input==0){
      perfect_state=1;
      snprintf(journal_fire,72,"Your perfect fire attack has a larger random contribution!");
      submit_log(journal_fire,1,1);
      p->perf_fire--;
      perfect_modifier=PERFECT_FIRE_MOD;
      }
    else{
      snprintf(journal_fire,72,"You did not use a perfect fire attack!");
      submit_log(journal_fire,1,1);
    }
  }
  int random_modifier=0;
  float output_damage=0;
  time_t t;
  random_modifier=( rand () % (PLAYER_FIRE_MOD+perfect_modifier));
  output_damage=p->fire*PLAYER_FIRE_COEFF+random_modifier;
  snprintf(journal_fire, 72, "Fire power is  %-8.2f",output_damage);
  submit_log(journal_fire,1,1);
  return output_damage;
}
float player_sword(struct Player_Stats* p){
  char journal_sword[72];
  char input;
  int perfect_modifier=0;
  snprintf(journal_sword,72,"You decided to use a sword attack!");
  submit_log(journal_sword,1,1);
   if(p->sword>PERFECT_UNLOCK_LEVEL && p->perf_sword !=0){
    snprintf(journal_sword, 72, "Do you want to use a perfect sword attack?"); 
    submit_log(journal_sword,0,1);
    input = battle_buttons(2); //2 means yes or no
       //returns 0: yes, 1: no
    if(input==0){
      perfect_state=1;
      snprintf(journal_sword, 72, "Your perfect sword attack will have higher damage!");
      submit_log(journal_sword,1,1);
      p->perf_sword--;
      perfect_modifier=1;
      }
    else{
      snprintf(journal_sword, 72, "You did not perform a perfect sword attack!");
      submit_log(journal_sword,1,1);
    }
  }
  int random_modifier=0;
  float output_damage=0;
  time_t t;
  random_modifier=( rand () % PLAYER_SWORD_MOD);
  output_damage=p->sword*PLAYER_SWORD_COEFF;
  if(perfect_modifier==1){output_damage=output_damage+PERFECT_SWORD_MOD;}
  snprintf(journal_sword,72,"Your sword is doing %f damage!",output_damage+random_modifier);
  submit_log(journal_sword,1,1);
  output_damage=output_damage+random_modifier;
  return output_damage+random_modifier;
}
void player_heal(struct Player_Stats* p){
  char journal_heal[72];
  snprintf(journal_heal,72,"You decided to heal instead of attacking.");
  submit_log(journal_heal,1,1);
  float healthprior=p->health;
  if(p->heal==0){
    p->health=p->health+.1*p->maxhealth;
    if(p->health>p->maxhealth){p->health=p->maxhealth;}
    snprintf(journal_heal,72,"You will perform a normal heal with no skill bonuses.");
    submit_log(journal_heal,0,1);
    snprintf(journal_heal,72,"You healed %f points.",p->health-healthprior);
    submit_log(journal_heal,0,1);
    return;}
  time_t t;
  int random_modifier=0;
  int maxrand=.05*p->maxhealth*p->heal;
  random_modifier=( rand () % (maxrand));
  char input;
   if(p->heal>PERFECT_UNLOCK_LEVEL && p->perf_heal !=0){
    snprintf(journal_heal,72,"Will you do a perfect heal? This will also boost your max HP by %d.",PLAYER_MAX_HEALTH_PER_PERFECT_HEAL);
    submit_log(journal_heal,0,1);
    if(p->perf_heal==1){snprintf(journal_heal,72,"You currently have %d perfect heal available.",p->perf_heal);}
    else{snprintf(journal_heal,72,"You currently have %d perfect heals available.",p->perf_heal);}
    submit_log(journal_heal,0,1);
    input = battle_buttons(2);
    if(input==0){
      snprintf(journal_heal,72,"You performed a perfect heal!");
      submit_log(journal_heal,1,1);
      p->perf_heal--;
      p->maxhealth=p->maxhealth+PLAYER_MAX_HEALTH_PER_PERFECT_HEAL;
      p->health=p->maxhealth;
      snprintf(journal_heal,72,"You healed a %f amount of points!",p->health-healthprior);
      submit_log(journal_heal,1,1);
      return;
      }
    else{
    }
   }
   snprintf(journal_heal,72,"You perform a normal heal!");
   submit_log(journal_heal,1,1);
   p->health=p->health+(.35*(p->maxhealth*(p->heal*.08))+random_modifier)+p->maxhealth*.1;
   if(p->health>p->maxhealth){p->health=p->maxhealth;}
   snprintf(journal_heal,72,"You healed %f points!",p->health-healthprior);
   submit_log(journal_heal,1,1);
   return;
}

float player_power(float damage,struct Player_Stats *p){
  int random_number=0;
  float savedamage=damage;
  char journal_power[72];
  random_number=( rand () % 100);
  if(p->power>PERFECT_UNLOCK_LEVEL && p->perf_power>0){
    if(random_number<PERFECT_POWER_MOD){
      damage=PERFECT_POWER_MULTIPLIER*damage;
      return damage;}}
  damage=damage+damage*(p->power*PLAYER_POWER_COEFF);
  printf("Your power increased your damage by %f!\n",damage-savedamage);
  snprintf(journal_power,72,"Your power increased your damage by %f!",damage-savedamage);
  submit_log(journal_power,1,1);
  return damage;

}
//**************************************************************PLAYER GENERAL DEFENSE FUNCTION**************************************************************
void player_defense(float damage,struct Player_Stats* p){
  char journal_defense[72];
  float nextdamage;
  float finaldamage;
  int dodge_success=0;
  char input = battle_buttons(1);
 restart_defense :
  switch(input){
      case 1 :
        nextdamage=player_block(damage,p);
        finaldamage=player_armor(nextdamage,p);
        p->health=p->health-finaldamage;
	snprintf(journal_defense,72,"Your final received damage was %f",finaldamage);
	submit_log(journal_defense,1,1);
        check_player_health(p);
        break;
      case 0 :
        dodge_success=player_dodge(p);
        if(dodge_success==1){/*printf("You survived the attack with no damage!\n");*/ return;}
        finaldamage=player_armor(damage,p);
        p->health=p->health-finaldamage;
        check_player_health(p);
        break;
      case 3 :
          print_journal();
          break;
  default :
    goto restart_defense;
  }
  return;
}
//**************************************************************NPC DEFENSE FUNCTIONS**************************************************************
float npc_block(float damage,struct NPC* p){
  char journal_block[72];
  int extra_block;
  float savedamage;
  savedamage=damage;
  time_t t;  
  extra_block=( rand () % NPC_BLOCK_MOD);
  snprintf(journal_block,72,"The enemy is going to block");
  submit_log(journal_block,1,1);
  if(p->block==0){damage=damage*.95-extra_block;
    if(damage<0){damage=0;}
    snprintf(journal_block,72,"The enemy blocked %f damage.",savedamage-damage);
    submit_log(journal_block,1,1);
    return damage;}
  damage=damage*(1-(p->block*NPC_BLOCK_COEFF))-extra_block;
  if(damage<0){damage=0;}
  snprintf(journal_block,72,"The enemy block stopped %f damage.",savedamage-damage);
  submit_log(journal_block,1,1);
  return damage;
}
int npc_dodge(struct NPC* p){
  char journal_dodge[72];
  if(perfect_state==0){
    time_t t;
    int threshold=0;
    int random_number=0;
    random_number=( rand () % 100);
    threshold=100*(p->dodge*NPC_DODGE_COEFF);
    snprintf(journal_dodge,72,"Your enemy has a %d chance of dodging.",threshold);
    submit_log(journal_dodge,1,1);
    if(random_number<threshold){
      snprintf(journal_dodge,72,"They dodged!");
      submit_log(journal_dodge,1,1);
      return 1;}
    printf("They did not dodge!\n");
    snprintf(journal_dodge,72,"They did not dodge.");
    submit_log(journal_dodge,1,1);
    return 0;}

  printf("The enemy tried to stupidly dodge your perfect attack!\n");
  perfect_state=0;
  return 0;
}
float npc_armor(float damage,struct NPC* p){
float savedamage=damage;
  damage=damage*(1-(p->armor*NPC_ARMOR_COEFF));
  char journal_armor[72];
  snprintf(journal_armor,72,"Their armor stopped %f damage.",savedamage-damage);
  submit_log(journal_armor,1,1);
  return damage;
}

float npc_heal(struct NPC* p){
  char journal_heal[72];
  float heal_amount=0;
  int random_number=0;
  random_number=( rand () % NPC_HEAL_MOD);
  if(p->heal==0){
      heal_amount=p->maxhealth*NPC_HEAL_COEFF;
      heal_amount+=random_number;
      if (p->health+heal_amount > p->maxhealth) heal_amount = p->maxhealth - p->health;
    p->health=p->health+heal_amount;
    return heal_amount;}
  heal_amount=p->maxhealth*NPC_HEAL_COEFF*p->heal+random_number;
    if (heal_amount+p->health > p->maxhealth) heal_amount = p->maxhealth - p->health;
  p->health=p->health+heal_amount;
  snprintf(journal_heal,72,"The NPC healed %f.",heal_amount);
  submit_log(journal_heal,1,1);
  return heal_amount;
}

//**************************************************************NPC GENERAL DEFENSE FUNCTION**************************************************************

void npc_defense(float damage,struct NPC* p){
  float next_damage;
  float final_damage;
  int imba;
  int dodge_state;
  int random;
  random=( rand () % 100);
  imba=p->block-p->dodge;
  if(random+(imba*5)<50){//then dodge
    dodge_state=npc_dodge(p);
    if(dodge_state==1){/*printf("NPC defended and lost zero health!\n");*/ return;}
    if(dodge_state==0){final_damage=npc_armor(damage,p);}
  }
  else {
    next_damage=npc_block(damage,p);
    final_damage=npc_armor(next_damage,p);
  }
  p->health=p->health-final_damage;
  check_npc_health(p);
  return;

}

//**************************************************************PLAYER GENERAL ATTACK FUNCTION**************************************************************

float player_attack(struct Player_Stats* p){
  float damage=0;
  char input = battle_buttons(0);
    switch(input){
        case 1 :
            damage=player_fire(p);
            break;
        case 0 :
            damage=player_sword(p);
            break;
        case 2 :
            player_heal(p);
            break;
        case 3 :
            print_journal();
            break;
        default :
            //this should never happen!
            break;
  }

    float final_damage;
    final_damage=player_power(damage,p);
  return final_damage;
}

//**************************************************************NPC GENERAL ATTACK FUNCTION*******************************************************

float npc_attack(struct NPC* p){
  char journal_attack[72];
  snprintf(journal_attack,72,"The enemy is deciding whether to use fire or sword.");
  submit_log(journal_attack,0,1);
  float damage=0;
  int imba=0;
  int random=0;
  random=( rand () % 100);
  imba=p->fire-p->sword;
  if(random+(imba*5)<50){//then sword
    snprintf(journal_attack,72,"The enemy is going to use a sword.");
    submit_log(journal_attack,0,1);
    damage=npc_sword(p);}
  else {
    snprintf(journal_attack,72,"The enemy is going to use a fire attack.");
    submit_log(journal_attack,0,1);
    damage=npc_fire(p);}
  float final_damage;
  final_damage=npc_power(damage,p);
  return final_damage;
}


//**************************************************************NPC ATTACK FUNCTIONS**************************************************************

float npc_sword(struct NPC* p){
  char journal_sword[72];
  int random_modifier=0;
  float output_damage=0;
  random_modifier=( rand () % NPC_SWORD_MOD);
  output_damage=p->sword*NPC_SWORD_COEFF+random_modifier;
  snprintf(journal_sword,72,"Their sword did %f damage.",output_damage);
  submit_log(journal_sword,1,1);
  //npc_power(output_damage,p);
  return output_damage;
}
float npc_fire(struct NPC* p){
  char journal_fire[72];
  int random_modifier=0;
  float output_damage=0;
  int maxrand=p->fire*NPC_FIRE_MOD+2;
  random_modifier=( rand () % (maxrand));
  output_damage=p->fire*NPC_FIRE_COEFF+random_modifier;
  snprintf(journal_fire,72,"The enemy output %f fire damage.",output_damage);
  submit_log(journal_fire,1,1);
  //npc_power(output_damage,p);
  return output_damage;
}

float npc_power(float damage, struct NPC* p){
  char journal_power[72];
  float savedamage=damage;
  damage=damage*(1-(p->power*NPC_POWER_COEFF));
  snprintf(journal_power,72,"Their power increased their damage by %f!",savedamage-damage);
  submit_log(journal_power,1,1);
  return damage;
}

//**************************************************************BATTLE FUNCTION************************************************************

void battle(struct NPC* p,struct Player_Stats* q){
  char journal_battle[72];
  int npc_alive=1;
  float damage;
  float next_damage;
  while(npc_alive){
     battle_screen(0,p,q);
      battle_buttons(3);
    damage=player_attack(q);
    if(damage==0){
      snprintf(journal_battle,72,"Since you healed the enemy does not need to defend.");
      submit_log(journal_battle,1,1);
      goto npc_attack;}
    npc_defense(damage,p);
    npc_alive=check_npc_health(p);
    if(npc_alive==0){break;}
  npc_attack :
    npc_heal(p);
    snprintf(journal_battle,72,"It is the enemy attack turn.");
    submit_log(journal_battle,1,1);
    damage=npc_attack(p);
    snprintf(journal_battle,72,"It is your turn to defend.");
    submit_log(journal_battle,1,1);
    player_defense(damage,q);
  }}

//**************************************************************PRINT BATTLE SCREEN**************************************************************

void print_battle_screen_main (struct NPC* p, struct Player_Stats* q){
    printf("\e[8;28;149t");
    fflush(stdout); system("clear");
    printf("0====================================0=========================================================================0====================================0\n");
    printf("||                                   |                                                                         |                                   ||\n");
    printf("||   %s           ",q->name);
    printf("|                                                                         |     %s          ", p->name);
    printf("||\n");
    printf("||                                   |                                                                         |                                   ||\n");
    printf("||   HP : %8.2f/%-8.2f          |                                                                         |     HP : %8.2f/%-8.2f        ||\n", q->health, q->maxhealth, p->health, p->maxhealth);
    printf("||                                   |                                                                         |                                   ||\n");
    printf("||            Skills      Perfects   |                                                                         |                 Skills            ||\n");
    printf("||                                   |                                                                         |                                   ||\n");
    printf("||   Dodge    %3d/%-3d      %3d       |                                                                         |     Dodge      %4d               ||\n", q->dodge, PLAYER_STAT_MAX, q->perf_dodge, p->dodge);
    printf("||                                   |                                                                         |                                   ||\n");
    printf("||   Block    %3d/%-3d      %3d       |                                                                         |     Block      %4d               ||\n", q->block, PLAYER_STAT_MAX, q->perf_block, p->block);
    printf("||                                   |                                                                         |                                   ||\n");
    printf("||   Armor    %3d/%-3d      %3d       |                                                                         |     Armor      %4d               ||\n", q->armor, PLAYER_STAT_MAX, q->perf_armor, p->armor);
    printf("||                                   |                                                                         |                                   ||\n");
    printf("||   Fire     %3d/%-3d      %3d       |                                                                         |     Fire       %4d               ||\n", q->fire, PLAYER_STAT_MAX, q->perf_fire, p->fire);
    printf("||                                   |                                                                         |                                   ||\n");
    printf("||   Sword    %3d/%-3d      %3d       |                                                                         |     Sword      %4d               ||\n", q->sword, PLAYER_STAT_MAX, q->perf_sword, p->sword);
    printf("||                                   |                                                                         |                                   ||\n");
    printf("||   Power    %3d/%-3d      %3d       |                                                                         |     Power      %4d               ||\n", q->power, PLAYER_STAT_MAX, q->perf_power, p->power);
    printf("||                                   |                                                                         |                                   ||\n");
    printf("||   Healing  %3d/%-3d      %3d       |                                                                         |     Healing    %4d               ||\n", q->heal, PLAYER_STAT_MAX, q->perf_heal, p->heal);
    printf("||                                   |                                                                         |                                   ||\n");
    printf("0====================================0====================================0====================================0====================================0\n");
    return;
}
void print_battle_screen_buttons(int button_state, int selected){
    //state: 0 = attacking, 1 = defending, 2 = yes/no 3: BLANK
    //selected: which button is currently selected
    char *button_strings[4][4] = {
        {"SWORD (s)  ", "  FIRE (f)  ", "  HEAL (h)  ", "JOURNAL (j)"},
        {"DODGE (d)  ", " BLOCK (b)  ", "            ", "JOURNAL (j)"},
        {" YES (y)   ", "   NO (n)   ", "            ", "JOURNAL (j)"},
        {"           ", "            ", "            ", "           "}
    };
    printf("\033[24;1H");
    for (int j = 0; j < 3; j++){
        printf("||");
        for (int i = 0; i < 4; i++){
            if (selected == i) printf("\033[46m");
            if (j == 1){
                printf("            %s            ", button_strings[button_state][i]);
            }
            else{
                printf("                                   ");
                if (i == 1 || i == 2) printf(" ");
            }
            if (selected == i) printf("\033[49m");
            printf("|");
        }
        printf("|\n");
    }
    printf("0====================================0====================================0====================================0====================================0\n");
}

int battle_buttons(int button_types){
    //which state the buttons are in
    int button_state = 0;
    if (button_types == 3){
        print_battle_screen_buttons(3, 4);
        return 0;
    }
    else {
        //which button is selected
        char input = 0;
        while(1){
        restart:
            print_battle_screen_buttons(button_types, button_state);
            input = getcha();
            if (input == 91){
                input = getcha();
                if (input == 'A'){ //up
                    goto restart; //do nothing
                }
                else if (input == 'B'){ //down
                    goto restart; //do nothing
                }
                else if (input == 'C'){
                    button_state++;
                    if (button_state == 4) button_state = 0;
                }
                else if (input == 'D'){
                    button_state--;
                    if (button_state == -1) button_state = 3;
                }
                else if (input == 10){
                    return button_state;
                }
                else {
                    //they pressed the esc key?
                    goto restart; //??
                }
                if (button_state == 2 && input == 'C' && button_types != 0) button_state = 3;
                if (button_state == 2 && input == 'D' && button_types != 0) button_state = 1;
            }
            else {
                switch(button_types){
                    case 0:
                        switch(input){
                            case 's':
                                return 0;
                            case 'f':
                                return 1;
                            case 'h':
                                return 2;
                            case 'j':
                                return 3;
                            case 10:
                                return button_state;
                            default:
                                goto restart;
                        }
                    case 1:
                        switch(input){
                            case 'd':
                                return 0;
                            case 'b':
                                return 1;
                            case 'j':
                                return 3;
                            case 10:
                                return button_state;
                            default:
                                goto restart;
                        }
                    case 2:
                        switch(input){
                            case 'y':
                                return 0;
                            case 'n':
                                return 1;
                            case 'j':
                                return 3;
                            case 10:
                                return button_state;
                            default:
                                goto restart;
                        }
                    default:
                        goto restart;
                }
            }
        }
    }
}
//**************************************************************MAIN FUNCTION**************************************************************


int  main(){
    printf("\e[8;28;149t");
    printf("\e[?25l");
    fflush(stdout); system("clear");
    srand(time(0));
    //tutorial();
    struct Player_Stats q;
    reset_player_stats(&q);
    //gift_player_stats(&q);
    name_input(&q);
    struct NPC noob;//Enemy one!
    char npc_name[] = "this is 20 char str!";
    for (int i = 0; i < 21; i++){
        noob.name[i]=npc_name[i];
    }
    noob.armor=0;
    noob.fire=1;
    noob.sword=1;
    noob.block=2;
    noob.dodge=0;
    noob.health=1000;
    noob.power = 2;
    noob.maxhealth=1000;
    noob.heal = 0;
    battle_screen(0, &noob, &q);
    skill_upgrade(15, &q);
    battle(&noob,&q);
return 0;
}
