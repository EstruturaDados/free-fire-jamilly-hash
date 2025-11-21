#include <stdio.h> // Necessário para funções de entrada e saída (printf, scanf)
#include <string.h> // Necessário para funções de manipulação de strings (strcmp)
#include <stdlib.h> // Biblioteca padrão (não usada diretamente, mas recomendada)



// Definição da struct Item
typedef struct {
    char nome[30];       // Nome do item (ex: "Pistola", "Bandagem")
    char tipo[20];       // Tipo do item (ex: "arma", "munição", "cura")
    int quantidade;      // Quantidade do item
    int prioridade;      // Prioridade (1 = baixa, 5 = alta)
} Item;

// Enum com critérios possíveis de ordenação
typedef enum {
    ORDENAR_POR_NOME = 1,
    ORDENAR_POR_TIPO,
    ORDENAR_POR_PRIORIDADE
} CriterioOrdenacao;

// Variáveis globais
Item mochila[10];         // Vetor com os itens
int numItens = 0;         // Quantidade atual na mochila
int comparacoesSequencial = 0;
int comparacoesBinaria = 0;

int ordenadaPorNome = 0;  // libera busca binária apenas quando for 1

// Função para simular limpeza de tela
void limparTela() {
    for (int i = 0; i < 40; i++) printf("\n");
}

// Menu principal
void exibirMenu() {
    printf("\n MOCHILA DO JOGADOR \n");
    printf("Itens armazenados: %d/10\n", numItens);
    
    if (ordenadaPorNome)
        printf("Status: Ordenada por nome (busca binária disponível)\n");
    else
        printf("Status: Não ordenada (busca binária indisponível)\n");

   
    printf("1 - Inserir item\n");
    printf("2 - Remover item\n");
    printf("3 - Listar itens\n");
    printf("4 - Buscar item (sequencial)\n");
    printf("5 - Buscar item (binária)\n");
    printf("6 - Menu de ordenação\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

// Inserir item no vetor
void inserirItem() {
    if (numItens >= 10) {
        printf("A mochila está cheia!\n");
        return;
    }

   Item novo;

    printf("\nNome do item: ");
    scanf("%s", novo.nome);
    
    printf("Tipo do item: ");
    scanf("%s", novo.tipo);

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);

    do {
        printf("Prioridade (1 a 5): ");
        scanf("%d", &novo.prioridade);
    } while (novo.prioridade < 1 || novo.prioridade > 5);

    mochila[numItens] = novo;
    numItens++;
   

// Qualquer inserção invalida a ordenação por nome
    ordenadaPorNome = 0;

    printf("Item inserido com sucesso!\n");
}

// Remover item por nome
void removerItem() {
    char nome[30];
    printf("\nNome do item a remover: ");
    scanf("%s", nome);

    int index = -1;

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nItem não encontrado!\n");
        return;
    }


     // Desloca itens para remover o buraco
    for (int i = index; i < numItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    numItens--;

    printf("\nItem removido com sucesso!\n");
}

// Listagem dos itens no inventário
void listarItens() {
    if (numItens == 0) {
        printf("A mochila está vazia.\n");
        return;
    }

    printf("\n ITENS NA MOCHILA \n");
    for (int i = 0; i < numItens; i++) {
        printf("%d) Nome: %s\n", i + 1, mochila[i].nome);
        printf("    Tipo: %s\n", mochila[i].tipo);
        printf("    Quantidade: %d\n", mochila[i].quantidade);
        printf("    Prioridade: %d\n", mochila[i].prioridade);
        printf("----------------------------------------\n");
    }
}

// Função Insertion Sort para diferentes critérios
void insertionSort(CriterioOrdenacao criterio) {
    comparacoesSequencial = 0;

    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;

        while (j >= 0) {
            comparacoesSequencial++;

            int mover = 0;

            if (criterio == ORDENAR_POR_NOME)
                mover = strcmp(chave.nome, mochila[j].nome) < 0;

            else if (criterio == ORDENAR_POR_TIPO)
                mover = strcmp(chave.tipo, mochila[j].tipo) < 0;

            else if (criterio == ORDENAR_POR_PRIORIDADE)
                mover = chave.prioridade > mochila[j].prioridade;

            if (!mover) break;

            mochila[j + 1] = mochila[j];
            j--;
        }

        mochila[j + 1] = chave;
    }

    ordenadaPorNome = (criterio == ORDENAR_POR_NOME);

    printf("\nItens ordenados com sucesso!");
    printf("\nComparações feitas: %d\n", comparacoesSequencial);
}

    
// Menu para escolher o tipo de ordenação
void menuDeOrdenacao() {
    int opc;
    printf("\n=== MENU DE ORDENACAO ===\n");
    printf("1 - Ordenar por nome\n");
    printf("2 - Ordenar por tipo\n");
    printf("3 - Ordenar por prioridade\n");
    printf("Escolha: ");
    scanf("%d", &opc);

    if (opc < 1 || opc > 3) {
        printf("Opção inválida!\n");
        return;
    }

    insertionSort((CriterioOrdenacao)opc);
}

// Busca sequencial por nome
void buscarSequencial() {
    char nome[30];
    printf("Nome do item para buscar: ");
    scanf("%s", nome);

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            printf("Item encontrado!\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            printf("Prioridade: %d\n", mochila[i].prioridade);
            return;
        }
    }

    printf("Item não encontrado.\n");
     printf("Comparações: %d\n", comparacoesSequencial);
}

// Busca binária por nome (requer ordenação)
void buscaBinariaPorNome() {
    if (!ordenadaPorNome) {
        printf("A mochila não está ordenada por nome.\n");
        printf("Use a opção de ordenação primeiro.\n");
        return;
    }

    char nome[30];
    printf("Nome do item para buscar: ");
    scanf("%s", nome);

    int inicio = 0, fim = numItens - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        comparacoesBinaria++;

        int cmp = strcmp(nome, mochila[meio].nome);

        if (cmp == 0) {
            printf("Item encontrado!\n");
            printf("Nome: %s\n", mochila[meio].nome);
            printf("Tipo: %s\n", mochila[meio].tipo);
            printf("Quantidade: %d\n", mochila[meio].quantidade);
            printf("Prioridade: %d\n", mochila[meio].prioridade);
            return;
        }

        if (cmp > 0)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }

    printf("Item não encontrado.\n");
     printf("Comparações: %d\n", comparacoesBinaria);
}

// Função PRINCIPAL
int main() {
    int opcao;

    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: inserirItem(); break;
            case 2: removerItem(); break;
            case 3: listarItens(); break;
            case 4: buscarSequencial(); break;
            case 5: buscaBinariaPorNome(); break;
            case 6: menuDeOrdenacao(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}


