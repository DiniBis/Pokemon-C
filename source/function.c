#include "header.h"

// Structure des capacités
struct Move {
	unsigned short damage;
	unsigned short evasion_boost;
	unsigned short attack_boost;
	unsigned short defense_boost;
	char name[20];
};

struct Item {
	unsigned short price; //(prix_vente étant égal à la moitié)
	char name[20];
	unsigned short quantity;
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
	struct Move moves[2]; //Fait un tableau de structure
};

// Structure du joueur
struct Player {
    char name[12];
	unsigned short money;
	struct Pokemon pokemons[MAX_POKEMONS];
	struct Item items[MAX_ITEMS];
	int pokemon_actif;
};

struct ThreadParameters {
    struct Player *player;
    struct Item *items;
};

DWORD WINAPI sauvegarde(LPVOID lpParameter){
	Sleep(10*1000); //Toutes les 10 secondes
	//Pour avoir 2 paramètres dans mon thread, je les mets dans une structure de pointeurs
    struct ThreadParameters *params=(struct ThreadParameters*)lpParameter;
    struct Player *player=params->player;
    struct Item *items=params->items;

    FILE *file = fopen("sauvegarde.txt", "w");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sauvegarde.\n");
        return 1;
    }
    //Sauvegarde des informations du joueur
    fprintf(file, "%s %hu\n", player->name, player->money);
    //Sauvegarde des Pokémons
    for (int i = 0; i < MAX_POKEMONS; i++) {
        if (strlen(player->pokemons[i].name)>3) {
            fprintf(file, "%s %hu %hd %hd %hd %hd %hd %u %hd\n",
                    player->pokemons[i].name,
                    player->pokemons[i].HPmax,
                    player->pokemons[i].HP,
                    player->pokemons[i].Attack,
                    player->pokemons[i].Defense,
                    player->pokemons[i].Evasion,
                    player->pokemons[i].Accuracy,
                    player->pokemons[i].Speed,
                    player->pokemons[i].Experience,
                    player->pokemons[i].Level);
        }
    }
    //Sauvegarde des objets
    for (int i = 0; i < MAX_ITEMS; i++) {
        fprintf(file, "%hu\n", player->items[i].quantity);
    }
    fclose(file);
}

void charger(struct Player *player, struct Item *items) {
    FILE *file = fopen("sauvegarde.txt", "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sauvegarde.\n");
        return;
    }
    // Charger les informations du joueur
    fscanf(file, "%s %hu\n", player->name, &player->money);
    // Charger les Pokémons
    for (int i = 0; i < MAX_POKEMONS; i++) {
        if (fscanf(file, "%s %hu %hd %hd %hd %hd %hd %u %hd\n",
                   player->pokemons[i].name,
                   &player->pokemons[i].HPmax,
                   &player->pokemons[i].HP,
                   &player->pokemons[i].Attack,
                   &player->pokemons[i].Defense,
                   &player->pokemons[i].Evasion,
                   &player->pokemons[i].Accuracy,
                   &player->pokemons[i].Speed,
                   &player->pokemons[i].Experience,
                   &player->pokemons[i].Level) == EOF) {
            break;
        }
    }
    // Charger les objets
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (fscanf(file, "%hu\n", items[i].quantity) == EOF) {
            break;
        }
    }
    fclose(file);
}


void Intro(struct Player *joueur) {
    printf("Bonjour jeune dresseur, quel est ton nom ?\n");
    scanf("%s", joueur->name);
};

int displayChoix(){
    int num;
    printf("Choisis ton starter:\n");
    printf("1 - Samlaleche\n2 - FDP de CR7\n3 - Karamazov\n");
    scanf("%d", &num);
    return num;
}

void displayDestination(){
    printf("+--------------------------------+\n");
    printf("| Ou voulez-vous aller ?         |\n");
    printf("|%-26s|\n","\t1 - Route Nationale");
    printf("|%-26s|\n","\t2 - Magasin");
    printf("|%-26s|\n","\t3 - Centre Pokemon");
    printf("|%-26s|\n","\t4 - Quitter le Jeu");
    printf("+--------------------------------+\n");
}
void capacite(struct Player *player, struct Pokemon *ennemy, int lanceur){
    //En paramètre lanceur pour savoir de qui l'attaque provient et sa cible)
    int actif=player->pokemon_actif;
    int choixMove;
    if (lanceur==1){ //Si le lanceur est le joueur
        //Affichage des tes attaques -> Selection
        for (int i=0; i<2; i+=1) {
            printf("%d - %s\n",i+1,player->pokemons[actif].moves[i].name);
        }
        scanf("%d",&choixMove);
        //dommage = Launcher_Attack x Damage / Target_Defense
        ennemy->HP-=player->pokemons[actif].moves[choixMove-1].damage*player->pokemons[actif].Attack/ennemy->Defense;//Retirer à l'adversaire les dégâts inflifés
//        player->pokemons[actif].Evasion+=player->pokemons[actif].moves[choixMove].evasion_boost;//Boost d'evasion
//        player->pokemons[actif].Attack+=player->pokemons[actif].moves[choixMove].attack_boost;//Boost d'attaque
//        player->pokemons[actif].Defense+=player->pokemons[actif].moves[choixMove].defense_boost;//Boost de défense
        //Retrait des boost de stat pour l'instant, il y a déjà assez de paramètres dans la structure Pokemon, pas besoin de doubler sa taille
        fflush(stdin);
    } else if (lanceur==2){ //Si le lanceur est le CPU
        //Selection d'une capacité aléatoire
        int ennemyMove=rand()%2;
        player->pokemons[actif].HP-=ennemy->moves[ennemyMove].damage*ennemy->Attack/player->pokemons[actif].Defense;
        //Pour les boost ce serait pareil, seul la cible est différente, ex:
        //ennemy->Evasion+=ennemy->moves[ennemyMove].evasion_boost
    }
}

void levelup (struct Player *player, struct Pokemon *ennemy, int side) {
    if (side==0) {
        player->pokemons[player->pokemon_actif].Accuracy += 1 + 0.2*player->pokemons[player->pokemon_actif].Level;
        player->pokemons[player->pokemon_actif].Attack += 1 + 0.2*player->pokemons[player->pokemon_actif].Level;
        player->pokemons[player->pokemon_actif].Defense += 1 + 0.2*player->pokemons[player->pokemon_actif].Level;
        player->pokemons[player->pokemon_actif].Evasion += 1 + 0.2*player->pokemons[player->pokemon_actif].Level;
        player->pokemons[player->pokemon_actif].Speed += 1 + 0.2*player->pokemons[player->pokemon_actif].Level;
        player->pokemons[player->pokemon_actif].HPmax += 1 + 0.5*player->pokemons[player->pokemon_actif].Level;
        player->pokemons[player->pokemon_actif].Level += 1;
    }
    else {
        ennemy->Accuracy += 1 + 0.2*ennemy->Level;
        ennemy->Attack += 1 + 0.2*ennemy->Level;
        ennemy->Defense += 1 + 0.2*ennemy->Level;
        ennemy->Evasion += 1 + 0.2*ennemy->Level;
        ennemy->Speed += 1 + 0.2*ennemy->Level;
        ennemy->HPmax += 1 + 0.5*ennemy->Level;
        ennemy->HP=ennemy->HPmax;
        ennemy->Level += 1;
    }
}

void useItem (struct Item *items, struct Player *player, struct Pokemon *ennemy){
    printf("+-------------------------------------+\n");
    printf("|%-20s (+5HP)  Bag : %d |\n"," 1 - Potion",items[0].quantity);
    printf("|%-20s (+10HP) Bag : %d |\n"," 2 - Super Potion",items[1].quantity);
    printf("|%-20s (+1LVL) Bag : %d |\n"," 3 - Rare Candy",items[2].quantity);
    printf("+-------------------------------------+\n");
    int choixObjet;
    scanf("%d",&choixObjet);
    switch (choixObjet){
        case 1:
            if (items[0].quantity>0){
                player->pokemons[player->pokemon_actif].HP+=5;
                if (player->pokemons[player->pokemon_actif].HP>player->pokemons[player->pokemon_actif].HPmax){
                    player->pokemons[player->pokemon_actif].HP=player->pokemons[player->pokemon_actif].HPmax;
                }
                items[0].quantity-=1;
            }
            break;
        case 2:
            if (items[1].quantity>0){
                player->pokemons[player->pokemon_actif].HP+=10;
                if (player->pokemons[player->pokemon_actif].HP>player->pokemons[player->pokemon_actif].HPmax){
                    player->pokemons[player->pokemon_actif].HP=player->pokemons[player->pokemon_actif].HPmax;
                }
                items[1].quantity-=1;
            }
        case 3:
            if (items[2].quantity>0){
                levelup(player, &ennemy, 0);
                items[2].quantity-=1;
            }
    }
}

void victoire (struct Player *player, struct Pokemon *ennemy) {
    int argent = rand()%400;
    int expSup = 0;
    player->money += argent+101;
    player->pokemons[player->pokemon_actif].Experience += (ennemy->Level-1)*1000 + 500;
    if (((player->pokemons[player->pokemon_actif].Level-1)*1000 + 500) < player->pokemons[player->pokemon_actif].Experience) {
        expSup = player->pokemons[player->pokemon_actif].Experience - ((player->pokemons[player->pokemon_actif].Level-1)*1000 + 500);
        levelup(player, &ennemy, 0);
        player->pokemons[player->pokemon_actif].Experience = expSup;
    }
}

void displaySwitch(struct Player *player){
    printf("+---------------------------------------+\n");
    for (int i = 0; i < MAX_POKEMONS; i++) {
        if (strlen(player->pokemons[i].name)>3){
            printf("%s\t",player->pokemons[i].name);
            printf("%d/%d ",player->pokemons[i].HP,player->pokemons[i].HPmax);
            float sur10=(float)player->pokemons[i].HP/player->pokemons[i].HPmax*10;
            printf("[");
            for (int i=0; i<=sur10;i++){printf("+");}
            for (int i=0;i<10-sur10;i++){printf("-");}
            printf("]\n");
        }
    }
    printf("+---------------------------------------+\n");
}

int capture(struct Player *player, struct Pokemon *ennemy){
    srand(time(NULL));
    int taille_equipe=0;
    //trouver la taille de l'équipe
    for (int i = 0; i < MAX_POKEMONS; i++) {
        if (strlen(player->pokemons[i].name)>3) {
            taille_equipe+=1;
        }
    }
    float chancesCapture = ((float)(ennemy->HPmax - ennemy->HP) / ennemy->HPmax) * 100;
    int captured = rand()%101;

    if ((taille_equipe<6) && (chancesCapture > captured)){
        player->pokemons[taille_equipe]=*ennemy;
        return 1;
    } else {
        printf("Capture echoue\n");
        return 0;
    }
}

void wild(struct Player *player, struct Pokemon *pokes, struct Item *items){
    //selection d'un pokemon aléatoire comme ennemi
    srand(time(NULL));
    int random_ennemy=rand()%3;
    //initialiser sa structure
    struct Pokemon ennemy = pokes[random_ennemy];
    printf("%s sauvage apparait !!!\n",ennemy.name);
    //calcul de ses stats par rapport au niveau du joueur
    for (int i=1; i<player->pokemons[player->pokemon_actif].Level; ++i){
        levelup(player, &ennemy, 1);
    }

    int choix;
    int CombatPossible=1; //De base le combat est possible, si changement arrête le while
    int CombatGagne=0; //Si le Pokemon adverse est KO, donner récompense après le while
    while (CombatPossible==1){
        fflush(stdin);
        displayWild(player, &ennemy);
        scanf("%d",&choix);
        switch(choix){
            //Capacité
            case 1:
                //Si le pokemon du joueur a une plus grande vitesse...
                if (player->pokemons[player->pokemon_actif].Speed > ennemy.Speed){
                    capacite(player, &ennemy, 1);//Le joueur utilise une capacité
                    capacite(player, &ennemy, 2);//Puis l'adversaire
                }
                //Sinon
                else if (player->pokemons[player->pokemon_actif].Speed < ennemy.Speed){
                    capacite(player, &ennemy, 2);//L'adversaire utilise une capacité
                    capacite(player, &ennemy, 1);//Puis le joueur
                }
                else {
                    int tie = rand()%2;
                    if (tie==0) {
                        capacite(player, &ennemy, 1);//Le joueur utilise une capacité
                        capacite(player, &ennemy, 2);//Puis l'adversaire
                    }
                    else {
                        capacite(player, &ennemy, 2);//L'adversaire utilise une capacité
                        capacite(player, &ennemy, 1);//Puis le joueur
                    }
                }
                //Si le Pokemon adverse a des HP inférieurs ou égaux à 0
                if (ennemy.HP<=0){CombatPossible=0; CombatGagne=1;};
                if (player->pokemons[player->pokemon_actif].HP<=0){CombatPossible=0;}
                break;
            case 2:
                //switch
                system("cls");
                printf("Choisis un pokemon :\n");
                displaySwitch(player);
                int choixSwitch;
                scanf("%d",&choixSwitch);
                player->pokemon_actif=choixSwitch-1;
                capacite(player, &ennemy, 2);//L'adversaire utilise une capacité
                break;
            case 3:
                useItem(items, player, &ennemy);
                capacite(player, &ennemy, 2);//L'adversaire utilise une capacité
                break;
            case 4:
                //capture
                if (capture(player, &ennemy)==1){
                    CombatPossible=0;
                } else {
                    capacite(player, &ennemy, 2);//L'adversaire utilise une capacité
                }
                break;
            case 5:
                if (player->pokemons[player->pokemon_actif].Speed > ennemy.Speed) {
                    CombatPossible=0;
                }
                else {
                    float chanceFuite = (float)player->pokemons[player->pokemon_actif].Speed / (ennemy.Speed + player->pokemons[player->pokemon_actif].Speed) * 100;
                    int fuite = rand()%101;
                    if (chanceFuite > fuite) {
                        CombatPossible=0;
                    } else {
                        printf("Fuite echoue\n");
                        capacite(player, &ennemy, 2);//L'adversaire utilise une capacité
                    }
                }
                break;
        };
        if (player->pokemons[player->pokemon_actif].HP<=0){CombatPossible=0;}
    }
    if (CombatGagne==1){
        victoire(player, &ennemy); //Gain exp + argent
    }
}

void displayWild(struct Player *player, struct Pokemon *ennemy){
    int actif=player->pokemon_actif;
    printf("Combat :\n");
    printf("+-----------------------+\n");
    printf("| %s (%s)\t|\n",player->pokemons[actif].name,player->name);
    printf("| HP : %hd/%hu\t",player->pokemons[actif].HP,player->pokemons[actif].HPmax);
    printf("Lvl : %hd |\n",player->pokemons[actif].Level);
    printf("| Atk : %hu\t",player->pokemons[actif].Attack);
    printf("Def : %hd |\n",player->pokemons[actif].Defense);
    printf("| Acc : %hu\t",player->pokemons[actif].Accuracy);
    printf("Eva : %hd |\n",player->pokemons[actif].Evasion);
    printf("+-----------------------+\n");
    printf("| %s (ennemy)\t|\n",ennemy->name);
    printf("| HP : %hd/%hu\t",ennemy->HP,ennemy->HPmax);
    printf("Lvl : %hd |\n",ennemy->Level);
    printf("| Atk : %hu\t",ennemy->Attack);
    printf("Def : %hd |\n",ennemy->Defense);
    printf("| Acc : %hu\t",ennemy->Accuracy);
    printf("Eva : %hd |\n",ennemy->Evasion);
    printf("+-----------------------+\n\n");
    printf("+-------------------------------+\n");
    printf("| Choisis une option\t\t|\n");
    printf("|\t1 - Capacite\t\t|\n");
    printf("|\t2 - Changer de Pokemon\t|\n");
    printf("|\t3 - Utiliser un objet\t|\n");
    printf("|\t4 - Capture\t\t|\n");
    printf("|\t5 - Fuir\t\t|\n");
    printf("+-------------------------------+\n");
}



void displayAcheter(struct Item *items, struct Player *player){
    printf("Acheter :\n");
    printf("+---------------------------------------+\n");
    printf("|%-20s (100 spc) Bag : %d |\n"," 1 - Potion",items[0].quantity);
    printf("|%-20s (300 spc) Bag : %d |\n"," 2 - Super Potion",items[1].quantity);
    printf("|%-20s (700 spc) Bag : %d |\n"," 3 - Rare Candy",items[2].quantity);
    printf("+---------------------------------------+\n");
    printf("spc en poche: %hu\n",player->money);
}

void acheter(struct Item *items, struct Player *player, int num){
    if(player->money >= items[num-1].price){
        player->money -= items[num-1].price;
        items[num-1].quantity += 1;
    }
}

void displayVendre(struct Item *items, struct Player *player){
    printf("Vendre :\n");
    printf("+---------------------------------------+\n");
    printf("|%-20s (50 spc)  Bag : %d |\n"," 1 - Potion",items[0].quantity);
    printf("|%-20s (150 spc) Bag : %d |\n"," 2 - Super Potion",items[1].quantity);
    printf("|%-20s (350 spc) Bag : %d |\n"," 3 - Rare Candy",items[2].quantity);
    printf("+---------------------------------------+\n");
    printf("spc en poche: %hu\n",player->money);
}

void vendre(struct Item *items, struct Player *player, int num)
{
    if(items[num-1].quantity > 0){
        player->money += (items[num-1].price)/2;
        items[num-1].quantity -= 1;
    }
}

void displayCentrePokemon(struct Player *player, int num_pokemon){
    printf("Voulez-vous soigner vos Pokemons ?\n");
    printf("+---------------------------------------+\n");
    for (int i = 0; i < MAX_POKEMONS; i++) {
        if (strlen(player->pokemons[i].name)>3) {
            if (player->pokemons[i].name!=NULL){
                printf("%s\t",player->pokemons[i].name);
                printf("%d/%d ",player->pokemons[i].HP,player->pokemons[i].HPmax);
                float sur10=(float)player->pokemons[i].HP/player->pokemons[i].HPmax*10;
                printf("[");
                for (int i=0; i<=sur10;i++){printf("+");}
                for (int i=0;i<10-sur10;i++){printf("-");}
                printf("]\n");
            }
        }
    }
    printf("+---------------------------------------+\n");
    printf("1 - Oui\n2 - Non\n");
}