#include <stdio.h> // Necessário para funções de entrada e saída (printf, scanf)
#include <string.h> // Necessário para funções de manipulação de strings (strcmp)
#include <stdlib.h> // Biblioteca padrão (não usada diretamente, mas recomendada)
#include <time.h> 

#define MAX_COMPONENTES 20
#define MAX_NOME 30
#define MAX_TIPO 20


// Definição da struct Item
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade; // 1..10
} Componente;

//  Vetor global e controle
Componente componentes[MAX_COMPONENTES];
int totalComponentes = 0;

// Estado de ordenação atual (para controlar busca binária) 
typedef enum { NAO_ORDENADO=0, ORDENADO_NOME, ORDENADO_TIPO, ORDENADO_PRIORIDADE } EstadoOrdenacao;
EstadoOrdenacao estado = NAO_ORDENADO;

// Contadores de comparações por algoritmo
long compsBubble = 0;
long compsInsertion = 0;
long compsSelection = 0;
long compsBuscaBinaria = 0;
long compsBuscaSequencial = 0;

//  Leitura segura com fgets
void lerString(const char *prompt, char *dest, int tamanho) {
    printf("%s", prompt);
    if (fgets(dest, tamanho, stdin) == NULL) {
        dest[0] = '\0';
        return;
    }

// remove '\n' final, se existir
size_t len = strlen(dest);
    if (len > 0 && dest[len-1] == '\n') dest[len-1] = '\0';

// se string vazia, pede novamente
if (dest[0] == '\0') {
        printf("Entrada vazia. Tente novamente.\n");
        printf("%s", prompt);
        if (fgets(dest, tamanho, stdin) == NULL) dest[0] = '\0';
        len = strlen(dest);
        if (len > 0 && dest[len-1] == '\n') dest[len-1] = '\0';
    }
}

int lerInt(const char *prompt, int min, int max) {
    char buf[64];
    int v;
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
        if (sscanf(buf, "%d", &v) == 1) {
            if (v >= min && v <= max) return v;
            printf("Valor fora do intervalo [%d..%d].\n", min, max);
        } else {
            printf("Entrada inválida. Digite um número.\n");
        }
    }
}
   

// Mostrar componentes (formatado)
    void mostrarComponentes(Componente arr[], int n) {
    if (n == 0) {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }
    printf("\n---- COMPONENTES (total %d) ----\n", n);
    for (int i = 0; i < n; ++i) {
        printf("[%2d] Nome: %-28s | Tipo: %-12s | Prioridade: %2d\n",
               i+1, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
    printf("---------------------------------\n");
}

// Inserir/Remover/Listar/Busca Sequencial
void inserirComponente() {
    if (totalComponentes >= MAX_COMPONENTES) {
        printf("\nA lista está cheia (%d componentes).\n", MAX_COMPONENTES);
        return;
    }
    Componente c;
    lerString("\nNome do componente: ", c.nome, MAX_NOME);
    lerString("Tipo do componente: ", c.tipo, MAX_TIPO);
    c.prioridade = lerInt("Prioridade (1-10): ", 1, 10);

    componentes[totalComponentes++] = c;
    estado = NAO_ORDENADO;
    printf("Componente \"%s\" adicionado.\n", c.nome);
}

void removerComponente() {
    if (totalComponentes == 0) {
        printf("\nNenhum componente para remover.\n");
        return;
    }
    char nome[MAX_NOME];
    lerString("\nNome do componente a remover: ", nome, MAX_NOME);
    int idx = -1;
    for (int i = 0; i < totalComponentes; ++i) {
        if (strcmp(componentes[i].nome, nome) == 0) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("Componente \"%s\" não encontrado.\n", nome);
        return;
    }
    for (int j = idx; j < totalComponentes - 1; ++j)
        componentes[j] = componentes[j+1];
    totalComponentes--;
    estado = NAO_ORDENADO;
    printf("Componente \"%s\" removido.\n", nome);
}

void buscaSequencial(const char *chave) {
    compsBuscaSequencial = 0;
    for (int i = 0; i < totalComponentes; ++i) {
        compsBuscaSequencial++;
        if (strcmp(componentes[i].nome, chave) == 0) {
            printf("\nEncontrado (sequencial) na posição %d:\n", i+1);
            printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                   componentes[i].nome, componentes[i].tipo, componentes[i].prioridade);
            printf("Comparações: %ld\n", compsBuscaSequencial);
            return;
        }
    }
    printf("\nNão encontrado (sequencial). Comparações: %ld\n", compsBuscaSequencial);
}

//  Algoritmos de ordenação pedidos (cada um em sua função)
// Assinaturas conforme requisitos:
// - selectionSortPrioridade(Componente[], int)
// - insertionSortTipo(Componente[], int)
// - bubbleSortPorNome(Componente[], int)
// As implementações atualizam contadores globais.

// Bubble Sort por nome (string)
void bubbleSortPorNome(Componente arr[], int n) {
    compsBubble = 0;
    for (int i = 0; i < n - 1; ++i) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; ++j) {
            compsBubble++;
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                Componente tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
    estado = ORDENADO_NOME;
}

// Insertion Sort por tipo (string)
void insertionSortTipo(Componente arr[], int n) {
    compsInsertion = 0;
    for (int i = 1; i < n; ++i) {
        Componente chave = arr[i];
        int j = i - 1;
        while (j >= 0) {
            compsInsertion++;
            if (strcmp(chave.tipo, arr[j].tipo) < 0) {
                arr[j+1] = arr[j];
                j--;
            } else break;
        }
        arr[j+1] = chave;
    }
    estado = ORDENADO_TIPO;
}

    
// Selection Sort por prioridade (int) - ordena: prioridade maior primeiro
void selectionSortPrioridade(Componente arr[], int n) {
    compsSelection = 0;
    for (int i = 0; i < n - 1; ++i) {
        int idxMax = i;
        for (int j = i + 1; j < n; ++j) {
            compsSelection++;
            if (arr[j].prioridade > arr[idxMax].prioridade) {
                idxMax = j;
            }
        }
        if (idxMax != i) {
            Componente tmp = arr[i];
            arr[i] = arr[idxMax];
            arr[idxMax] = tmp;
        }
    }
    estado = ORDENADO_PRIORIDADE;
}

// Busca binária por nome (aplicável apenas se ordenado por nome)
// assinatura requerida: buscaBinariaPorNome(Componente[], int, char[])
// retorna índice >=0 se encontrado, -1 se não encontrado, -2 se estado inválido
int buscaBinariaPorNome(Componente arr[], int n, char chave[]) {
    compsBuscaBinaria = 0;
    if (estado != ORDENADO_NOME) return -2; // não ordenado por nome

    int l = 0, r = n - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        compsBuscaBinaria++;
        int cmp = strcmp(chave, arr[m].nome);
        if (cmp == 0) return m;
        if (cmp > 0) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

   
// medirTempo: recebe ponteiro para função de ordenação com assinatura void func(Componente[], int) e mede tempo de execução em segundos; também permite passar o array e n. Retorna o tempo em segundos.
double medirTempo(void (*algoritmo)(Componente[], int), Componente arr[], int n) {
    // Faz cópia do vetor para não alterar o original ao medir (protótipo educacional)
    Componente copia[MAX_COMPONENTES];
    for (int i = 0; i < n; ++i) copia[i] = arr[i];

    clock_t inicio = clock();
    algoritmo(copia, n);
    clock_t fim = clock();

    return (double)(fim - inicio) / CLOCKS_PER_SEC;
}


// Menu e integração
void ordenarMenu() {
    if (totalComponentes == 0) {
        printf("\nNenhum componente para ordenar.\n");
        return;
    }

    printf("\nEscolha algoritmo de ordenação:\n");
    printf("1 - Bubble Sort (por NOME)\n");
    printf("2 - Insertion Sort (por TIPO)\n");
    printf("3 - Selection Sort (por PRIORIDADE)\n");
    int op = lerInt("Opção: ", 1, 3);

    double tempo = 0.0;
    if (op == 1) {
        // usamos medirTempo para obter tempo e executamos de fato na estrutura global
        tempo = medirTempo(bubbleSortPorNome, componentes, totalComponentes);
        // Executa de verdade para modificar o array global
        bubbleSortPorNome(componentes, totalComponentes);
        printf("\nOrdenado por NOME (Bubble Sort).\n");
        printf("Comparações: %ld | Tempo: %.6f s\n", compsBubble, tempo);
    } else if (op == 2) {
        tempo = medirTempo(insertionSortTipo, componentes, totalComponentes);
        insertionSortTipo(componentes, totalComponentes);
        printf("\nOrdenado por TIPO (Insertion Sort).\n");
        printf("Comparações: %ld | Tempo: %.6f s\n", compsInsertion, tempo);
    } else {
        tempo = medirTempo(selectionSortPrioridade, componentes, totalComponentes);
        selectionSortPrioridade(componentes, totalComponentes);
        printf("\nOrdenado por PRIORIDADE (Selection Sort).\n");
        printf("Comparações: %ld | Tempo: %.6f s\n", compsSelection, tempo);
    }

    // Mostrar componentes após ordenação
    mostrarComponentes(componentes, totalComponentes);
}

// Função para buscar componente-chave com busca binária (apenas por nome)
void buscarChaveMenu() {
    if (totalComponentes == 0) {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }
    if (estado != ORDENADO_NOME) {
        printf("\nBusca binária disponível APENAS quando estiver ordenado por NOME.\n");
        return;
    }
    char chave[MAX_NOME];
    lerString("\nDigite o NOME do componente-chave: ", chave, MAX_NOME);
    int idx = buscaBinariaPorNome(componentes, totalComponentes, chave);
    if (idx == -2) {
        printf("Estado inconsistente (não está ordenado por nome).\n");
        return;
    }
    if (idx >= 0) {
        printf("\nComponente-CHAVE encontrado (posição %d):\n", idx+1);
        printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
               componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
        printf("Comparações na busca binária: %ld\n", compsBuscaBinaria);
    } else {
        printf("\nComponente-chave NÃO encontrado. Comparações: %ld\n", compsBuscaBinaria);
    }
}

// Relatório comparativo educacional (executa cada algoritmo em cópia e mostra tempo/comps) 
void relatorioComparativo() {
    if (totalComponentes == 0) {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }

    Componente backup[MAX_COMPONENTES];
    for (int i = 0; i < totalComponentes; ++i) backup[i] = componentes[i];

    // Bubble
    double tBubble = medirTempo(bubbleSortPorNome, backup, totalComponentes);
    long cb = compsBubble;

    // Insertion
    for (int i = 0; i < totalComponentes; ++i) backup[i] = componentes[i];
    double tInsertion = medirTempo(insertionSortTipo, backup, totalComponentes);
    long ci = compsInsertion;

    // Selection
    for (int i = 0; i < totalComponentes; ++i) backup[i] = componentes[i];
    double tSelection = medirTempo(selectionSortPrioridade, backup, totalComponentes);
    long cs = compsSelection;

    printf("\n=== Relatório Comparativo ===\n");
    printf("Bubble Sort (nome)    : Comparações=%ld | Tempo=%.6f s\n", cb, tBubble);
    printf("Insertion Sort (tipo) : Comparações=%ld | Tempo=%.6f s\n", ci, tInsertion);
    printf("Selection Sort (prio) : Comparações=%ld | Tempo=%.6f s\n", cs, tSelection);
    printf("\n");

    // restaurar estado original (manter ordenação antiga)
    estado = NAO_ORDENADO;
}

// Menu principal (integra tudo)
  
void menuPrincipal() {
    while (1) {
        printf("\n===== MONTAGEM DA TORRE (NÍVEL MESTRE) =====\n");
        printf("Componentes cadastrados: %d/%d\n", totalComponentes, MAX_COMPONENTES);
        printf("Estado: ");
        if (estado == NAO_ORDENADO) printf("NÃO ORDENADO\n");
        else if (estado == ORDENADO_NOME) printf("ORDENADO POR NOME\n");
        else if (estado == ORDENADO_TIPO) printf("ORDENADO POR TIPO\n");
        else if (estado == ORDENADO_PRIORIDADE) printf("ORDENADO POR PRIORIDADE\n");

        printf("\n1 - Cadastrar componente\n");
        printf("2 - Remover componente (por nome)\n");
        printf("3 - Listar componentes\n");
        printf("4 - Ordenar componentes (escolher algoritmo)\n");
        printf("5 - Buscar componente-chave (busca binária por NOME)\n");
        printf("6 - Buscar componente (sequencial)\n");
        printf("7 - Relatório comparativo (execução em cópia)\n");
        printf("0 - Sair\n");

        int opc = lerInt("\nEscolha: ", 0, 7);

        switch (opc) {
            case 1: inserirComponente(); break;
            case 2: removerComponente(); break;
            case 3: mostrarComponentes(componentes, totalComponentes); break;
            case 4: ordenarMenu(); break;
            case 5: buscarChaveMenu(); break;
            case 6: {
                char chave[MAX_NOME];
                lerString("\nNome para busca sequencial: ", chave, MAX_NOME);
                buscaSequencial(chave);
                break;
            }
            case 7: relatorioComparativo(); break;
            case 0:
                printf("\nEncerrando. Boa sorte na fuga!\n");
                return;
            default:
                printf("Opção inválida.\n");
        }
    }
}

// Main
int main() {
    printf("=== Sistema de Montagem da Torre - Atualizado (Nível Mestre) ===\n");
    printf("Instruções: cadastre até %d componentes, escolha algoritmo de ordenação\n", MAX_COMPONENTES);
    printf("e use busca binária apenas quando estiver ordenado por NOME.\n\n");
    menuPrincipal();
    return 0;
}


