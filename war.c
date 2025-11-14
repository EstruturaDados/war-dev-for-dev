// ============================================================================
//         PROJETO WAR ESTRUTURADO - NÍVEL AVENTUREIRO
// ============================================================================
//
// OBJETIVOS (Nível Aventureiro):
// - Substituir o vetor estático por alocação dinâmica com calloc.
// - Criar uma função para simular ataques entre dois territórios.
// - Utilizar números aleatórios para representar dados de batalha.
// - Usar ponteiros para manipular os territórios.
// - Foco em: Alocação dinâmica, ponteiros, lógica de jogo.
//
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h> 

// --- Constantes Globais ---
#define QTD_TERRITORIOS 5
#define TAM_STRING 50

// --- Estrutura de Dados ---
typedef struct
{
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
void limparBuffer();
void simularAtaque(Territorio* atacante, Territorio* defensor);
void exibirMapa(Territorio* mapa);

// --- Função Principal (main) ---
int main() {
    // 1. Configuração Inicial (Setup):
    setlocale(LC_ALL, "Portuguese");
    // Inicializa a semente para geração de números aleatórios
    srand(time(NULL));
    // Aloca a memória para o mapa dinamicamente com calloc
    Territorio* mapa = (Territorio*) calloc(QTD_TERRITORIOS, sizeof(Territorio));

    // Verifica se a alocação foi bem-sucedida
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para o mapa!\n");
        return 1; // Encerra o programa se não houver memória
    }

    printf("--- CADASTRO DOS %d TERRITORIOS ---\n", QTD_TERRITORIOS);
    // Preenche os territórios com seus dados iniciais
    for (int i = 0; i < QTD_TERRITORIOS; i++)
    {
 
        printf("Digite o nome do Territorio %d: ", i + 1);
        fgets(mapa[i].nome, TAM_STRING, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0; // Remove o \n

        printf("Digite a cor do Territorio %d: ", i + 1);
        fgets(mapa[i].cor, TAM_STRING, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = 0; // Remove o \n

        printf("Digite o numero de Tropas do Territorio %d: ", i + 1);
        scanf("%d", &mapa[i].tropas);
        limparBuffer();
        printf("\n");
    }

    // 2. Laço Principal do Jogo (Fase de Ataque):
    int op = -1;
    do {
        printf("\n--- RELATORIO ATUAL DO MAPA ---\n");
        exibirMapa(mapa); // Mostra o mapa atualizado

        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha uma acao:\n");
        printf("1. Atacar\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer();

        if (op == 1) {
            int idxAtacante, idxDefensor;
            printf("Escolha o territorio ATACANTE (1 a %d): ", QTD_TERRITORIOS);
            scanf("%d", &idxAtacante);
            printf("Escolha o territorio DEFENSOR (1 a %d): ", QTD_TERRITORIOS);
            scanf("%d", &idxDefensor);
            limparBuffer();

            // Validando índices (convertendo de 1-based para 0-based)
            if (idxAtacante < 1 || idxAtacante > QTD_TERRITORIOS ||
                idxDefensor < 1 || idxDefensor > QTD_TERRITORIOS) {
                printf("Indices invalidos!\n");
                continue;
            }
            if (idxAtacante == idxDefensor) {
                printf("Um territorio nao pode atacar a si mesmo!\n");
                continue;
            }

            // Chama a simulação de ataque passando os endereços (ponteiros)
            // dos territórios específicos 
            simularAtaque(&mapa[idxAtacante - 1], &mapa[idxDefensor - 1]);
        }

    } while (op != 0);


    // 3. Limpeza:
    // Libera a memória alocada dinamicamente
    printf("Liberando memoria e encerrando o jogo.\n");
    free(mapa);

    return 0;
}

// --- Implementação das Funções ---

// Função utilitária para limpar o buffer de entrada do teclado (stdin)
void limparBuffer() {
    int entrada;
    while ((entrada = getchar()) != '\n' && entrada != EOF);
}

// Exibe o estado atual de todos os territórios
void exibirMapa(Territorio* mapa) {
    for (int i = 0; i < QTD_TERRITORIOS; i++)
    {
        printf("%d. Territorio: %s | Cor: %s | Tropas: %d\n",
            i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula a lógica de batalha entre dois territórios 
void simularAtaque(Territorio* atacante, Territorio* defensor) {
    // Validação de ataque [cite: 684]
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("ATAQUE INVALIDO: Voce nao pode atacar um territorio seu!\n");
        return;
    }
    if (atacante->tropas <= 1) {
        printf("ATAQUE INVALIDO: Voce precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }

    // Simulação dos dados (1 a 6)
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("\n--- BATALHA: %s vs %s ---\n", atacante->nome, defensor->nome);
    printf("Dado ATACANTE (%s): %d\n", atacante->cor, dadoAtaque);
    printf("Dado DEFENSOR (%s): %d\n", defensor->cor, dadoDefesa);

    // Lógica de batalha (Empates favorecem o atacante)
    if (dadoAtaque >= dadoDefesa) {
        printf("VITORIA DO ATACANTE! O defensor perdeu 1 tropa.\n");
        defensor->tropas--;

        // Verifica se o território foi conquistado
        if (defensor->tropas <= 0) {
            printf("CONQUISTA! %s dominou %s!\n", atacante->nome, defensor->nome);
            // Muda a cor (dono) do território defensor
            strcpy(defensor->cor, atacante->cor);
            // Move 1 tropa para o novo território (regra do WAR)
            defensor->tropas = 1;
            atacante->tropas--;
        }
    }
    else {
        printf("VITORIA DO DEFENSOR! O atacante perdeu 1 tropa.\n");
        atacante->tropas--; 
    }
}