#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // Para getch() no Windows
#include <time.h>

#define L 10       // N mero de linhas na tela
#define C 20       // N mero de colunas na tela
#define Colum 22    // Numero da tela do jogo
#define MAX_AMMO 15 // M ximo de proj teis

char game[L][Colum];

// Estrutura para representar um pato
typedef struct
{
    int x, y;
    int alive; // 1 se o pato estiver vivo, 0 se estiver morto
} Duck;

// Estrutura para representar o rifle
typedef struct
{
    int x;
    int ammo;
    int score;
} Rifle;

void initGame(Duck** ducks, Rifle* rifle)
{
    // Aloque mem ria para os patos
    *ducks = (Duck*)malloc(3 * sizeof(Duck));

    // Inicialize os patos na linha superior, em colunas aleat rias
    for (int i = 0; i < 3; i++)
    {
        (*ducks)[i].x = rand() % C;
        (*ducks)[i].y = 0;
        (*ducks)[i].alive = 1;
    }

    // Inicialize o rifle na linha inferior
    rifle->x = C / 2;
    rifle->ammo = MAX_AMMO;
    rifle->score = 0;
}

void updateDucks(Duck* ducks)
{
    for (int i = 0; i < 3; i++)
    {
        if (ducks[i].alive)
        {
            // Atualize a posi  o do pato
            if (ducks[i].y % 2 == 0)
            {
                ducks[i].x += 1;
            }
            else
            {
                ducks[i].x -= 1;
            }

            if (ducks[i].x == 20 || ducks[i].x == 0)
            {
                ducks[i].y += 1;
            }
        }
    }
}


// Fun  o para atualizar a posi  o do rifle
void updateRifle(Rifle* rifle, Duck* ducks, char input)
{
    switch (input) {
    case 44: // Tecla de seta esquerda
        if (rifle->x > 0) {
            rifle->x -= 1;
        }
        break;
    case 46: // Tecla de seta direita
        if (rifle->x < 20) {
            rifle->x += 1;
        }
        break;
    case 32:// Tecla espaco
        // Dispara o tiro
        rifle->ammo -= 1;
        for (int i = 0; i < 3; i++) {
            if (ducks[i].alive == 1 && rifle->x == ducks[i].x) {
                ducks[i].alive = 0;
                rifle->score += 1;
            }
        }

        break;
    }
}

// Fun  o para liberar a mem ria alocada para os patos
void cleanupDucks(Duck* ducks)
{
    free(ducks);
}

void fillGame() {
    for (int l = 0; l < L; l++) {
        for (int c = 0; c < Colum; c++) {
            game[l][c] = '-';
            if (c == 0 || c == Colum - 1) {
                game[l][c] = '|';
            }

        }
    }
}
    
void print() {
    for (int l = 0; l < L; l++) {//eixo Y
        for (int c = 0; c < Colum; c++) {//eixo X
            printf("%c",game[l][c]);
        }
        printf("\n");
    }
}

void funcPrint(Duck* ducks, Rifle* rifle) {
    fillGame();
    for (int i = 0; i < 3; i++) {
        if (ducks[i].alive == 1) {
            game[ducks[i].y][ducks[i].x] = 'P';
        }
    }
    game[L - 1][rifle->x] = 'I';

    printf("Municao:%d Score:%d\n",rifle->ammo, rifle->score );
    print();
}

bool verificGame(Duck* ducks, Rifle* rifle) {
    bool result;
    for (int i = 0; i < 3; i++) {
        if (ducks[i].alive == 1 && ducks[i].y < L - 1) {
            result = false;
            break;
        }
        else {
            result = true;
        }
    }
    if (rifle->ammo == 0) {
        result = true;
    }

    return result;
}

int main()
{
    Duck* ducks = NULL;
    Rifle rifle;

    initGame(&ducks, &rifle);

    clock_t lastDuckUpdateTime = clock();
    const int updateInterval = 2000;

    while (1)
    {
        system("cls");
        funcPrint(ducks, &rifle);

        // Atualize a posicao dos patos
        updateDucks(ducks);

        /*clock_t currentTime = clock();
        if ((currentTime - lastDuckUpdateTime) >= updateInterval) {
            updateDucks(ducks);
            lastDuckUpdateTime = currentTime;
        }*/

        // Atualize a posicao do rifle
        char input = _getch();
        updateRifle(&rifle,ducks, input);

        // Exiba o estado atual do jogo
        funcPrint(ducks, &rifle);

        // Verifique as condicoes do jogo
        bool gameCondicions = verificGame(ducks, &rifle);
        if (gameCondicions) {
            break;
        }


    }
    // Libere a memoria alocada para os patos
    cleanupDucks(ducks);
    return 0;
}


