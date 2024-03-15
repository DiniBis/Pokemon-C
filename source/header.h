#ifndef POKEMON_FUNCTIONS_H
#define POKEMON_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

#define MAX_POKEMONS 6
#define MAX_ITEMS 3

// Structure des capacités
struct Move {
    unsigned short damage;
    unsigned short evasion_boost;
    unsigned short attack_boost;
    unsigned short defense_boost;
    char name[20];
};

// Structure représentant un pokémon
struct Pokemon {
    unsigned short HPmax;
    short HP;
    unsigned short Attack;
    unsigned short Defense;
    unsigned short Evasion;
    unsigned short Accuracy;
    unsigned short Speed;
    unsigned int Experience;
    unsigned short Level;
    char name[20];
    struct Move moves[2]; // Tableau de structures de capacités
};

// Structure des objets
struct Item {
    unsigned short price;
    char name[20];
    unsigned short quantity;
};

// Structure du joueur
struct Player {
    char name[12];
    unsigned short money;
    struct Pokemon pokemons[MAX_POKEMONS];
    struct Item items[MAX_ITEMS];
    int pokemon_actif;
};

// Fonctions de sauvegarde et de chargement
DWORD WINAPI sauvegarde(LPVOID lpParameter);
void charger(struct Player *player, struct Item *items);

// Fonctions d'affichage
void displayDestination();
void displayWild(struct Player *player, struct Pokemon *ennemy);
void displaySwitch(struct Player *player);
void displayAcheter(struct Item *items, struct Player *player);
void displayVendre(struct Item *items, struct Player *player);
void displayCentrePokemon(struct Player *player, int num_pokemon);

// Fonctions de jeu
void Intro(struct Player *joueur);
int displayChoix();
void capacite(struct Player *player, struct Pokemon *ennemy, int lanceur);
void levelup(struct Player *player, struct Pokemon *ennemy, int side);
void useItem(struct Item *items, struct Player *player, struct Pokemon *ennemy);
void victoire(struct Player *player, struct Pokemon *ennemy);
int capture(struct Player *player, struct Pokemon *ennemy);
void wild(struct Player *player, struct Pokemon *pokes, struct Item *items);

#endif // POKEMON_FUNCTIONS_H