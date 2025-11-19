#include <stdio.h> // Necessário para funções de entrada e saída (printf, scanf)
#include <string.h> // Necessário para funções de manipulação de strings (strcmp)
#include <stdlib.h> // Biblioteca padrão (não usada diretamente, mas recomendada)
#include <stdbool.h>


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
int comparacoes = 0;      // Número de comparações (análise)
int ordenadaPorNome = 0;  // Indica se está ordenada alfabeticamente

// Função para simular limpeza de tela
void limparTela() {
    for (int i = 0; i < 40; i++)
        printf("\n");
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

    printf("Nome: ");
    scanf("%s", novo.nome);

    printf("Tipo: ");
    scanf("%s", novo.tipo);

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);

    printf("Prioridade (1 a 5): ");
    scanf("%d", &novo.prioridade);

    mochila[numItens] = novo;
    numItens++;

// Qualquer inserção invalida a ordenação por nome
    ordenadaPorNome = 0;

    printf("Item inserido com sucesso!\n");
}

// Remover item por nome
void removerItem() {
    char nome[30];
    printf("Digite o nome do item a remover: ");
    scanf("%s", nome);

    int i, encontrado = -1;

    // Busca linear pelo item
    for (i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Item não encontrado!\n");
        return;
    }

    // Desloca elementos para fechar o buraco
    for (i = encontrado; i < numItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    numItens--;
    printf("Item removido com sucesso!\n");
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
    comparacoes = 0;  // reinicia o contador

    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;

        // Critério dinâmico
        while (j >= 0) {

            comparacoes++;

            int comp = 0;

            if (criterio == ORDENAR_POR_NOME)
                comp = strcmp(chave.nome, mochila[j].nome) < 0;

            else if (criterio == ORDENAR_POR_TIPO)
                comp = strcmp(chave.tipo, mochila[j].tipo) < 0;

            else if (criterio == ORDENAR_POR_PRIORIDADE)
                comp = chave.prioridade > mochila[j].prioridade;

            if (!comp) break;

            mochila[j + 1] = mochila[j];
            j--;
        }

        mochila[j + 1] = chave;
    }

    // Se for ordenado por nome, liberar busca binária
    ordenadaPorNome = (criterio == ORDENAR_POR_NOME);

    printf("Itens ordenados com sucesso!\n");
    printf("Comparações realizadas: %d\n", comparacoes);
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

        comparacoes++;

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





// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
