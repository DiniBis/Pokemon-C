#include "header.h"

int main() {
    srand(time(NULL));
    // Création d'objets
    struct Item items[3] = {
        {100, "Potion", 0},
        {300, "Super Potion", 0},
        {700, "Rare Candy", 0},
    };

    // Création de capacités
    struct Move moves[5] = {
        {3, 0, 0, 0, "Scratch"},
        {0, 0, 1, 0, "Grawl"},
        {2, 0, 0, 0, "Pound"},
        {0, 0, 0, 1, "Foliege"},
        {0, 1, 0, 0, "Shell"},
    };

    // Création de pokémons
    struct Pokemon pokes[3] = {
        {10, 10, 1, 1, 1, 2, 1, 0, 1, "Samlaleche", {moves[0], moves[1]}},
        {9, 9, 1, 1, 3, 2, 2, 0, 1, "FDP de CR7", {moves[2], moves[3]}},
        {11, 11, 1, 2, 2, 1, 2, 0, 1, "Karamazov", {moves[2], moves[4]}}
    };

    //Début de partie -> Charger ?
    int load;
    printf("Charger une sauvegarde ?\n0 - Non\n");
    scanf("%d",&load);
    struct Player player;
    if (load==0){
        //Création de player
        player.money=200;
        Intro(&player);
        //Choix du strater
        int num_starter=displayChoix();
        player.pokemons[0]=pokes[num_starter-1];
        printf("%s\n",player.pokemons[0].name);
    } else {
        charger(&player,items);
    }

    int num_pokemon;
    for (int i = 0; i<MAX_POKEMONS; i++){
        if (strlen(player.pokemons[i].name)>3){
            num_pokemon+=1;
        }
    }
    player.pokemon_actif=num_pokemon-1;

    printf("Ce jeu sauvegarde automatiquement toutes les 10s, ne pas fermer la fenêtre\n");
    //Paramètres de mon thread dans une structure
    struct ThreadParameters params;
    params.player=&player;
    params.items=items;
    HANDLE hThread = CreateThread(NULL, 0, sauvegarde, &params, 0, NULL);

    while (1==1){
        params.player=&player;
        params.items=items;
        system("cls");
        int choixDestination;
        displayDestination();
        scanf("%d",&choixDestination);
        switch (choixDestination){
            case 1:
                system("cls");
                wild(&player,&pokes, &items);
                break;
            case 2:
                system("cls");
                printf("1 - Acheter\n2 - Vendre\n");
                int choixBoutique;
                scanf("%d",&choixBoutique);
                if (choixBoutique==1){
                    displayAcheter(&items,&player);
                    int choixAcheter;
                    scanf("%d",&choixAcheter);
                    acheter(&items,&player,choixAcheter);
                }
                else if (choixBoutique==2){
                    displayVendre(&items,&player);
                    int choixVendre;
                    scanf("%d",&choixVendre);
                    vendre(&items,&player,choixVendre);
                }
                break;
            case 3:
                system("cls");
                displayCentrePokemon(&player, num_pokemon);
                int choixCentre;
                scanf("%d", &choixCentre);
                if (choixCentre==1){
                    for (int i=0; i<num_pokemon;i++){player.pokemons[i].HP=player.pokemons[i].HPmax;}
                }
                break;
            case 4:
                system("cls");
                CloseHandle(hThread);
                printf("Vous pouvez quitter");
                free(player.pokemons);
                return 0;
        }
    }
    return 0;
}
