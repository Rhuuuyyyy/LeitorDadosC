#include <stdio.h>
#include <string.h>
#include "funcoes_usuario.h" 

/*
EXPLICAÇÃO do motivo de não incluirmos o funcoes_usuario.c no main.c:
A razão pela qual main.c não inclui funcoes_usuario.c está fundamentada no processo de compilação e 
ligação (linking) da linguagem C. A tentativa de incluir um arquivo .c em outro resultaria em erros
 de "definição múltipla de símbolos" durante a fase de ligação.

O processo pode ser dividido em duas etapas principais:

Compilação Separada (Compilation):
O compilador (GCC) processa cada arquivo .c de forma independente para gerar um "arquivo objeto" (.o).

gcc -c main.c -o main.o: O compilador lê main.c. Quando encontra a diretiva #include "funcoes_usuario.h", 
ele textualmente insere o conteúdo do cabeçalho. Isso permite que main.c conheça os protótipos das
 funções (ex: exibir_menu()) e as definições de tipos (ex: struct Alimento). Ele então gera main.o, 
 um arquivo que contém o código de máquina para main e referências a símbolos externos que ele ainda
  não conhece (como o código de exibir_menu).

gcc -c funcoes_usuario.c -o funcoes_usuario.o: O compilador realiza o mesmo processo para
 funcoes_usuario.c, gerando funcoes_usuario.o. Este arquivo objeto contém o
  código de máquina real (as definições) para as funções exibir_menu, processar_opcao, etc.

Ligação (Linking):
Após a criação de todos os arquivos objeto, o "linker" é invocado.

gcc main.o funcoes_usuario.o -o programa: O linker combina os arquivos objeto em um único 
arquivo executável. Sua principal tarefa é resolver as referências a símbolos. Quando ele 
vê que main.o faz uma chamada a exibir_menu, ele procura a definição desse símbolo nos outros 
arquivos objeto fornecidos. Ele a encontra em funcoes_usuario.o e "conecta" a chamada à 
sua definição, resolvendo a referência.

O Problema da Inclusão Direta de .c

Se main.c contivesse #include "funcoes_usuario.c", a diretiva do pré-processador copiaria 
todo o texto de funcoes_usuario.c para dentro de main.c antes da compilação. O resultado seria:

O compilador geraria main.o, que conteria as definições de main, ler_alimentos_do_json, 
exibir_menu, processar_opcao, etc.

O compilador também geraria funcoes_usuario.o, que conteria as definições de exibir_menu, 
processar_opcao, etc.

Na fase de ligação, ao tentar combinar main.o e funcoes_usuario.o, o linker encontraria duas 
definições para a mesma função (exibir_menu, por exemplo), uma em cada arquivo objeto. Isso 
viola a "One Definition Rule" (Regra de Uma Definição) do C e causa um erro fatal de "símbolo definido multiplamente".

Conclusão:

Arquivos .h (cabeçalhos) são utilizados para declarações: informam ao compilador "o que" 
existe (protótipos de funções, definições de tipos).

Arquivos .c (fonte) são utilizados para definições: informam ao compilador "como" as funções 
funcionam (a implementação do código).

A inclusão de cabeçalhos permite a compilação separada, enquanto a ligação posterior resolve 
as referências entre os módulos de código compilados.
*/

// --- Funções Auxiliares para o Quick Sort ---

// Função para trocar dois ponteiros de Alimento de lugar
void trocar_ponteiros(Alimento** a, Alimento** b) {
    Alimento* temp = *a;
    *a = *b;
    *b = temp;
}

// Funções de comparação para o Quick Sort
int comparar_por_descricao(const void* a, const void* b) {
    Alimento* alim_a = *(Alimento**)a;
    Alimento* alim_b = *(Alimento**)b;
    return strcmp(alim_a->descricao, alim_b->descricao);
}

int comparar_por_energia(const void* a, const void* b) {
    Alimento* alim_a = *(Alimento**)a;
    Alimento* alim_b = *(Alimento**)b;
    // Retorna > 0 se b for maior, 0 se igual, < 0 se a for maior (para ordem decrescente)
    return alim_b->energia_kcal - alim_a->energia_kcal;
}

// Partição do Quick Sort: rearranja o array
int particao(Alimento** arr, int baixo, int alto, int (*comparar)(const void*, const void*)) {
    Alimento* pivo = arr[alto];
    int i = (baixo - 1);

    for (int j = baixo; j <= alto - 1; j++) {
        if (comparar(&arr[j], &pivo) < 0) {
            i++;
            trocar_ponteiros(&arr[i], &arr[j]);
        }
    }
    trocar_ponteiros(&arr[i + 1], &arr[alto]);
    return (i + 1);
}

// Implementação manual do Quick Sort, conforme requisito do trabalho
void quick_sort(Alimento** arr, int baixo, int alto, int (*comparar)(const void*, const void*)) {
    if (baixo < alto) {
        int pi = particao(arr, baixo, alto, comparar);
        quick_sort(arr, baixo, pi - 1, comparar);
        quick_sort(arr, pi + 1, alto, comparar);
    }
}

// ===================================================================================
// FUNÇÃO PARA EXIBIR O MENU DE OPÇÕES DO USUÁRIO
// ===================================================================================
void exibir_menu() {




    AQUI DEVE SER FEITO OS PRINTS DO MENU PARA O USUÁRIO...




}

// ===================================================================================
// FUNÇÃO PARA PROCESSAR A OPÇÃO ESCOLHIDA PELO USUÁRIO
// ===================================================================================
void processar_opcao(char opcao, Alimento** alimentos, int total) {

    switch (opcao) {
        case 'a': case 'A':
            listar_categorias_unicas(alimentos, total);
            break;

        case 'b': case 'B':
            listar_alimentos_por_categoria_alfa(alimentos, total);
            break;

        case 'c': case 'C':
            listar_alimentos_por_categoria_energia(alimentos, total);
            break;
            
        case 'd': case 'D':
        case 'e': case 'E':
        case 'f': case 'F':
        case 'g': case 'G':
        case 'h': case 'H':
        case 'i': case 'I':
            // Para as outras opções, por enquanto, apenas exibimos uma mensagem.
            printf("\nOpcao ainda nao implementada.\n");
            break;
        case 'j': case 'J':
            // Se o usuário digitou 'j' ou 'J', apenas mostra uma mensagem de despedida.
            // O programa vai terminar por causa da condição no loop 'do-while' da main.
            printf("\nEncerrando...\n");
            break;
        default:
            // Se o usuário digitar qualquer outra letra, avisa que a opção é inválida.
            printf("\nOpcao invalida! Por favor, tente novamente.\n");
            break;
    }
}

// ===================================================================================
// FUNÇÃO PARA LISTAR AS CATEGORIAS (OPÇÃO 'a')
// ===================================================================================
void listar_categorias_unicas(Alimento** alimentos, int total) {
    // O objetivo desta função é olhar os 100 alimentos e dizer quais são as categorias presentes, sem repetir nenhuma.
    char lista_unicas[50][100]; // Cria uma matriz para guardar até 50 categorias, cada uma com até 100 caracteres.
    int total_unicas = 0;       // Um contador para saber quantas categorias já anotou.

    // Percorre a lista de todos os 100 alimentos, um por um.
    for (int i = 0; i < total; i++) {
        bool ja_existe = false; // Começa assumindo que a categoria do alimento atual é nova.
        
        // Olha a matriz para ver se a categoria atual já foi anotada antes.
        for (int j = 0; j < total_unicas; j++) {
            if (strcmp(lista_unicas[j], alimentos[i]->categoria) == 0) { // O strcmp compara duas strings. Se forem iguais, retorna 0. 
                ja_existe = true; // Se encontrou, marca que ela já existe.
                break;            // E para de procurar.
            }
        }
        
        // Se, depois de procurar, viu que a categoria realmente era nova...
        if (!ja_existe) {
            strcpy(lista_unicas[total_unicas], alimentos[i]->categoria); // ...copia o nome da categoria no array de categorias únicas...
            total_unicas++; // E aumenta o contador de categorias anotadas...
        }
    }

    // No final, imprime a lista final de categorias que foi anotada.
    printf("\nEssas sao as categorias disponiveis para a analise:\n");
    for (int i = 0; i < total_unicas; i++) {
        printf("- %s\n", lista_unicas[i]); // Laço para dar print em cada um dos elementos da lista de categorias únicas...
    }
}

// ===================================================================================
// FUNÇÃO PARA LISTAR AS CATEGORIAS (OPÇÃO 'b')
// ===================================================================================

void listar_alimentos_por_categoria_alfa(Alimento** alimentos, int total) {
    char categoria_escolhida[100];

    // Passo 1: Mostrar as categorias disponível e pedir para o usuário escolher uma.
    printf("\n--- Lista dos Alimentos em Ordem Alfabetica ---\n");
    listar_categorias_unicas(alimentos, total); // Reutilizamos a função da 'a' para mostrar as opções

    printf("\n> Digite o nome (exatamente igual) de uma das categorias acima: ");
    
    scanf(" %[^\n]", categoria_escolhida);

    // Passo 2: Filtrar os alimentos que pertencem à categoria escolhida.
    // E criar um vetor que guardara os ponteiros para os alimentos originais.(Assim como na 'a')
    Alimento* alimentos_filtrados[100]; 
    int total_filtrados = 0;

    for (int i = 0; i < total; i++) {
        // Compara se a categoria do alimento atual é igual a categoria escolhida.
        if (strcmp(alimentos[i]->categoria, categoria_escolhida) == 0) {
            alimentos_filtrados[total_filtrados] = alimentos[i]; // Adiciona o ponteiro ao nosso vetor.
            total_filtrados++;
        }
    }

    // Se nenhum alimento for encontrado, informa o usuário e encerra a função.
    if (total_filtrados == 0) {
        printf("\nNenhum alimento encontrado para a categoria \"%s\". Verifique se o nome foi digitado corretamente.\n", categoria_escolhida);
        return;
    }
    
    // A ideia dessa parte é usar o Quick Sort, um algoritmo eficiente que escolhe
    // um elemento como "pivô" e particiona a lista, colocando elementos menores
    // de um lado e maiores do outro, e então repete o processo recursivamente.
    quick_sort(alimentos_filtrados, 0, total_filtrados - 1, comparar_por_descricao);

    // Passo 4: Exibi os resultados já ordenados.
    printf("\n--- Alimentos na categoria '%s' ---\n", categoria_escolhida);
    for (int i = 0; i < total_filtrados; i++) {
        printf("  - %s (No %d)\n", alimentos_filtrados[i]->descricao, alimentos_filtrados[i]->numero);
    }
}

// ===================================================================================
// FUNÇÃO PARA LISTAR AS CATEGORIAS (OPÇÃO 'c')
// ===================================================================================

void listar_alimentos_por_categoria_energia(Alimento** alimentos, int total) {
    char categoria_escolhida[100];

    // Passo 1: Interação com o usuário (Mesma estrutura da B)
    printf("\n--- Listar Alimentos por Categoria (Ordem Decrescente de Energia) ---\n");
    listar_categorias_unicas(alimentos, total);
    // Função da 'a' para nós ajudar
    printf("\n> Digite o nome exato de uma das categorias acima: ");
    
    scanf(" %[^\n]", categoria_escolhida);

    // Passo 2: Filtragem dos alimentos (Mesma estrutura da B)
    Alimento* alimentos_filtrados[100];
    int total_filtrados = 0;

    for (int i = 0; i < total; i++) {
        if (strcmp(alimentos[i]->categoria, categoria_escolhida) == 0) {
            alimentos_filtrados[total_filtrados++] = alimentos[i];
        }
    }

    if (total_filtrados == 0) {
        printf("\nNenhum alimento encontrado para a categoria \"%s\". Verifique se o nome foi digitado corretamente.\n", categoria_escolhida);
        return;
    }

    // Passo 3: Ordenação por energia
    // Usaremos Quick Sort novamente, mas agora o critério de comparação é a energia, em ordem decrescente.
    quick_sort(alimentos_filtrados, 0, total_filtrados - 1, comparar_por_energia);


    // Passo 4: Exibi os resultados
    printf("\n--- Alimentos na categoria '%s' (Ordem Decrescente de Energia) ---\n", categoria_escolhida);
    for (int i = 0; i < total_filtrados; i++) {
        printf("  - %s | Energia: %d Kcal\n",
               alimentos_filtrados[i]->descricao,
               alimentos_filtrados[i]->energia_kcal);
    }
}

// ===================================================================================
// FUNÇÃO PARA LISTAR AS CATEGORIAS (OPÇÃO 'd')
// ===================================================================================

// ===================================================================================
// FUNÇÃO PARA LISTAR AS CATEGORIAS (OPÇÃO 'e')
// ===================================================================================

// ===================================================================================
// FUNÇÃO PARA LISTAR AS CATEGORIAS (OPÇÃO 'f')
// ===================================================================================

// ===================================================================================
// FUNÇÃO PARA LISTAR AS CATEGORIAS (OPÇÃO 'g')
// ===================================================================================

// ===================================================================================
// FUNÇÃO PARA LISTAR AS CATEGORIAS (OPÇÃO 'h')
// ===================================================================================

// ===================================================================================
// FUNÇÃO PARA LISTAR AS CATEGORIAS (OPÇÃO 'i')
// ===================================================================================

