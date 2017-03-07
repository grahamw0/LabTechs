/* monsters.c - monster abilities and vulnerabilities.
 *
 * D Provine and a cast of dozens, 5 Feb 2013
 *
 * Edited by Will Graham <grahamw0@students.rowan.edu>
 */

#include <stdio.h>

/* MONSTER ATTACKS */
#define  FIRE    0x01
#define  BITE    0x02
#define  CLAW    0x04
#define  ACID    0x08
#define  FROST   0x10
#define  POISON  0x20

/* MONSTER TYPES */
#define WOLF    ( BITE )
#define COUGAR  ( BITE | CLAW )
#define TIGER   ( BITE | CLAW )
#define COBRA   ( BITE | POISON )
#define DRAGON  ( FIRE | BITE | CLAW )
#define YETI    ( BITE | CLAW | FROST )
#define ALIEN   ( BITE | ACID )

/* DEFENSES */
#define  NONE        ( 0x00 )
#define  INSULATION  ( FIRE | FROST )
#define  ARMOR       ( BITE | CLAW )
#define  AMULET      ( POISON | ACID )


int main()
{
    // prototype for the "report_danger" function.
    void report_danger(int, int);
    int  equipment;
    int  monster;

    printf("Testing report_danger() function.\n");

    printf("Test  1: ");
    equipment = NONE;
    monster   = WOLF;
    report_danger(equipment, monster);

    printf("Test  2: ");
    equipment = ARMOR;
    monster   = WOLF;
    report_danger(equipment, monster);

    printf("Test  3: ");
    equipment = ARMOR;
    monster   = DRAGON;
    report_danger(equipment, monster);

    printf("Test  4: ");
    equipment = INSULATION;
    monster   = ALIEN;
    report_danger(equipment, monster);

    printf("Test  5: ");
    equipment = ARMOR;
    monster   = TIGER;
    report_danger(equipment, monster);

    printf("Test  6: ");
    equipment = ARMOR | AMULET | INSULATION;
    monster   = DRAGON;
    report_danger(equipment, monster);

    printf("Test  7: ");
    equipment = NONE;
    monster   = DRAGON;
    report_danger(equipment, monster);

    printf("Test  8: ");
    equipment = INSULATION;
    monster   = DRAGON;
    report_danger(equipment, monster);

    printf("Test  9: ");
    equipment = INSULATION;
    monster   = COBRA;
    report_danger(equipment, monster);

    printf("Test 10: ");
    equipment = AMULET;
    monster   = ALIEN;
    report_danger(equipment, monster);

    return 0;
}



/* void function - does not return anything
 *
 * report_danger takes two arguments:
 *   1) What defenses the user has
 *   2) What kind of monster it is (which attacks it has)
 *
 * report_danger should print out what things the
 * user has to worry about.
 */

void report_danger(int defenses, int monster)
{
    // DECLARE SOME VARIABLES, IF NEEDED
    int defBit;
    int monsBit;


    printf("Beware of: ");


    // I WROTE THIS PART
    for(int i = 0; i <= 5; i++) {
        defBit = (defenses >> i) & 1;
        monsBit = (monster >> i) & 1;

        if (defBit == 0 && monsBit == 1) {
            switch(i) {
            case 0:
                printf("fire ");
                break;
            case 1:
                printf("bite ");
                break;
            case 2:
                printf("claw  ");
                break;
            case 3:
                printf("acid  ");
                break;
            case 4:
                printf("frost ");
                break;
            case 5:
                printf("poison ");
                break;
            }
        }
    }

    printf("\n");
    return;
}
