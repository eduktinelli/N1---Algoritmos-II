// João Eduardo De Oliveira Nascimento
// Eduardo Kaua Tinelli
// 27 de setembro de 2026

/*
Uso de matriz bidimensional (5x5) para armazenar o cen�rio.
La�os de repeti��o para imprimir o labirinto a cada rodada.
Condicionais para validar os movimentos e impedir atravessar paredes.
Leitura de teclado com scanf para capturar as dire��es.
Verifica��o de vit�ria quando o jogador chega na sa�da.

Regras:
P - posi��o atual do jogador.
0 - caminho livre.
1 - parede (n�o pode atravessar).
S - sa�da (objetivo do jogo).
2 - buraco escondido (armadilha), aparece como (O) depois que o jogador cai nele.

Pontuacao:
+10 pontos por movimento correto.
-5 pontos ao bater na parede.
-30 pontos ao cair em um buraco (e volta para o inicio).
+100 pontos de bonus ao encontrar a saida.

*/


#include <stdio.h>   // Necess�rio para printf e scanf
#include <stdlib.h>  // Necess�rio para system (limpar tela em alguns sistemas)
#include <windows.h> // Necessario para Beep e Sleep (sons do Windows)
#include <ctype.h>   // Necess�rio para toupper (converter tecla para mai�scula)

#define N 12 // Tamanho da matriz do labirinto

// Cores (codigos ANSI) usadas para colorir o jogo no terminal
#define COR_RESET    "\033[0m"  // Volta para a cor normal
#define COR_VERMELHO "\033[31m"
#define COR_VERDE    "\033[32m"
#define COR_AMARELO  "\033[33m"
#define COR_AZUL     "\033[34m"
#define COR_CIANO    "\033[36m"

    int labirinto[N][N] =
    {
        {4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4},
        {3, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 3},
        {3, 0, 1, 2, 1, 0, 1, 1, 1, 0, 1, 3},
        {3, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 3},
        {3, 1, 1, 0, 1, 1, 0, 2, 1, 0, 1, 3},
        {3, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 3},
        {3, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 3},
        {3, 0, 1, 1, 1, 0, 0, 1, 2, 0, 1, 3},
        {3, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 3},
        {3, 0, 1, 0, 1, 1, 1, -1, 1, 1, 0, 3},
        {3, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 3},
        {4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4} // -1 indica sa�da (ser� exibida como 'S')
    };                                        // 2 indica buraco escondido

// Beep(frequencia em Hz, duracao em ms) toca um bipe pelo Windows

// Som de vitoria: melodia subindo (do, mi, sol, do agudo)
void som_vitoria()
{
    Beep(523, 150);
    Beep(659, 150);
    Beep(784, 150);
    Beep(1047, 400);
}

// Som de passo: bipe curto
void emitir_som()
{
    Beep(800, 60);
}

// Som de parede: dois bipes graves
void emitir_som_parede()
{
    int i;
    for (i = 0; i < 2; i++)
    {
        Beep(200, 150);
        Sleep(80); // Pausa de 80 ms entre os bipes
    }
}

// Som de armadilha: tons descendo, como se o jogador estivesse caindo
void som_armadilha()
{
    int freq;
    for (freq = 1200; freq >= 300; freq -= 150)
    {
        Beep(freq, 70);
    }
}

void mostrar_labirinto(int x, int y)
{
    int i, j;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                if (i == x && j == y)
                {
                    printf(COR_VERDE "  P  " COR_RESET); // Mostra jogador
                }
                else if (labirinto[i][j] == 1)
                {
                    printf(COR_AZUL " ||| " COR_RESET); // Mostra parede
                }
                else if (labirinto[i][j] == -1)
                {
                    printf(COR_AMARELO "| -*|" COR_RESET); // Mostra sa�da
                }
                else if (labirinto[i][j] == 6)
                {
                    printf(COR_VERMELHO " (O) " COR_RESET); // Mostra buraco que o jogador ja descobriu
                }
                else if(labirinto[i][j] == 3)
                {
                    printf(COR_CIANO "  |  " COR_RESET);
                }
                else if(labirinto[i][j] == 4)
                {
                    printf(COR_CIANO "  *  " COR_RESET);
                }else if(labirinto[i][j] == 5)
                {
                    printf(COR_CIANO "_____" COR_RESET);
                }
                else
                {
                    printf("     "); // Mostra caminho livre (o buraco escondido tambem aparece assim)
                }
            }
            printf("\n");

        }
}

// Mostra a linha e a coluna do jogador, os passos e a pontuacao
void mostrar_painel(int x, int y, int contador, int pontos)
{
    printf(COR_CIANO "Linha: %d | Coluna: %d" COR_RESET, x, y);
    printf("  ||  Passos: %d  ||  Pontos: %d\n", contador, pontos);
}

// Retorna 1 se o jogador pode ir para a posicao e 0 se for parede ou fora dos limites
int validar_movimento(int x, int y)
{
    if (x < 1 || x >= N-1 || y < 1 || y >= N-1)
    {
        return 0; // Fora dos limites
    }
    if (labirinto[x][y] == 1 || labirinto[x][y] == 3 || labirinto[x][y] == 4 || labirinto[x][y] == 5)
    {
        return 0; // Parede ou borda
    }
    return 1; // Caminho livre, buraco ou saida
}

int main()
{
    // Matriz que representa o labirinto
    // 0 = caminho, 1 = parede, S = sa�da (marcada por -1 aqui)
    int contador = 0;
    int pontos = 0;   // Pontuacao do jogador

    int x = 1, y = 1; // Posi��o inicial do jogador (linha, coluna)
    int novoX, novoY; // Nova posicao calculada a cada movimento
    char comando;     // Vari�vel para armazenar o comando do jogador
    int jogando = 1;  // Controle do loop principal do jogo

    // Loop principal do jogo
    while (jogando)
    {
        // Exibe o labirinto
        printf("Jogo do Labirinto 10x10\n");
        printf("Use W (cima), S (baixo), A (esquerda), D (direita)\n");
        printf("Objetivo: chegar na saida (| *|)\n");
        printf(COR_VERMELHO "Cuidado: existem buracos escondidos no caminho!\n\n" COR_RESET);

        mostrar_labirinto(x,y);
        mostrar_painel(x, y, contador, pontos);

        // Verifica se chegou � sa�da
        if (labirinto[x][y] == -1)
        {
            pontos = pontos + 100; // Bonus por encontrar a saida
            printf(COR_VERDE "\nParabens! Voce encontrou a saida!\n" COR_RESET);
            printf("\nVocê precisou de %d passos para chegar ao final\n", contador);
            printf("Pontuacao final: %d pontos (com bonus de 100 pela saida)\n", pontos);
            som_vitoria();
            break; // Sai do jogo
        }

        // Solicita movimento do jogador
        printf("\nDigite seu movimento (W/A/S/D): ");
        scanf(" %c", &comando);
        comando = toupper(comando); // Converte para mai�scula para facilitar

        // Calcula nova posi��o do jogador
        novoX = x;
        novoY = y;

        if (comando == 'W')
        {
            novoX--; // Move para cima
            emitir_som();

        }
        else if (comando == 'S')
        {
            novoX++; // Move para baixo
            emitir_som();
        }
        else if (comando == 'A')
        {
            novoY--; // Move para esquerda
            emitir_som();
        }
        else if (comando == 'D')
        {
            novoY++; // Move para direita
            emitir_som();
        }
        else
        {
            printf(COR_VERMELHO "Comando invalido!\n" COR_RESET);
            emitir_som_parede();
            continue; // Volta para o inicio do loop sem contar passo
        }

        // Verifica se nova posi��o � v�lida (dentro da matriz e n�o � parede)
        if (validar_movimento(novoX, novoY))
        {
            x = novoX;
            y = novoY; // Atualiza posi��o do jogador
            contador++;
            pontos = pontos + 10; // Ganha pontos por movimento correto

            // Verifica se caiu em um buraco (escondido ou ja descoberto)
            if (labirinto[x][y] == 2 || labirinto[x][y] == 6)
            {
                printf(COR_VERMELHO "Voce caiu em um buraco! Perdeu 30 pontos e voltou para o inicio!\n" COR_RESET);
                som_armadilha();
                labirinto[x][y] = 6; // O buraco passa a aparecer no mapa
                pontos = pontos - 30;
                x = 1;
                y = 1; // Volta para a posicao inicial
            }
        }
        else
        {
            printf(COR_VERMELHO "Movimento invalido! Parede ou fora dos limites! (-5 pontos)\n" COR_RESET);
            emitir_som_parede();
            pontos = pontos - 5; // Penalidade por bater na parede
        }
    }

    return 0; // Fim do jogo
}
