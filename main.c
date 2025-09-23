#include <stdio.h>      // Para funções de entrada e saída, como printf e fopen
#include <stdlib.h>     // Para alocação de memória (malloc), sorteio (rand, srand)
#include <string.h>     // Para manipulação de strings, como strcmp e strcpy
#include <time.h>       // Para funções relacionadas ao tempo
#include <stdbool.h>    // Para usar tipos booleanos
#include <json-c/json.h> // Biblioteca para ler JSON

#define QUANTIDADE_A_SORTEAR 10 // Indica ao programa quantas categorias devem ser sorteadas

/**
 * @brief Lê um arquivo JSON, extrai categorias únicas e sorteia uma quantidade definida delas.
 * @return char** Um array alocado dinamicamente contendo as strings das categorias sorteadas.
 * Retorna NULL em caso de erro. O chamador é responsável por liberar a memória.
 */
char** sortear() {
    FILE *arquivo = NULL;
    bool abrir = false; // Variável para controlar se a leitura foi bem-sucedida
    int quantidade = 0; // Contador de tentativas de leitura

    // // // // // // // // //  Função para tentar abrir o arquivo // // // // // // // // //
    do {
        printf("Abrindo o arquivo...\n");
        arquivo = fopen("dados.json", "r");
        if (arquivo != NULL) { // Verifica se o arquivo foi aberto com sucesso
            abrir = true;
            printf("Arquivo aberto com sucesso...\n");
        } else {
            quantidade++;
        }
    } while (quantidade < 5 && !abrir); // Tenta no máximo 5 vezes ou até conseguir abrir

    // Se a abertura do arquivo falhou, retorna NULL para sinalizar o erro.
    if (!abrir) {
        printf("Não foi possível ler o arquivo após %d tentativas...\n", quantidade);
        return NULL;
    }

    // // // // // // // // Função para alocação de memória e leitura do conteúdo do arquivo // // // // // // // // //
    printf("Lendo o arquivo dados.json...\n");
    fseek(arquivo, 0, SEEK_END); // Move o cursor para o final do arquivo para podermos medir seu tamanho
    long tamanho = ftell(arquivo); // Pega a posição atual do cursor (que é o tamanho total) e guarda na variável 'tamanho'
    fseek(arquivo, 0, SEEK_SET); // Volta o cursor para o início do arquivo, preparando para a leitura do conteúdo
    char *buffer_json = malloc(tamanho + 1); // Aloca na memória um espaço (buffer) grande o suficiente para todo o arquivo | O "+1" é para o caractere final '\0'
    fread(buffer_json, 1, tamanho, arquivo); // Lê 'tamanho' bytes do 'arquivo' e copia para o 'buffer_json' que acabamos de alocar
    fclose(arquivo); // Fecha o arquivo aqui, pois seu conteúdo já foi copiado para a memória
    buffer_json[tamanho] = '\0'; // Adiciona o caractere terminador nulo '\0' no final do buffer, transformando-o em uma string C válida
    printf("Arquivo lido com sucesso...\n");

    // // // // // // // // Função para interpretar o conteúdo JSON // // // // // // // // //
    printf("Analisando o conteudo do JSON...\n");
    json_object *json_completo = json_tokener_parse(buffer_json);

    // // // // // // // // Função para encontrar todas as categorias únicas do arquivo de dados // // // // // // // // //
    printf("Analisando os dados do arquivo dados.json...\n");
    char lista_categorias_unicas[100][100];
    int total_categorias_unicas = 0;

    size_t total_itens = json_object_array_length(json_completo); // Variável para pegar o número da quantidade de itens

    for (int i = 0; i < total_itens; i++) {
        json_object *item = json_object_array_get_idx(json_completo, i); // Laço para cada item do JSON
        json_object *valor_categoria;

        json_object_object_get_ex(item, "Categoria", &valor_categoria); // Função para guardar o nome da categoria do item que está sendo lido
        const char *nome_categoria = json_object_get_string(valor_categoria);

        bool ja_existe = false;

        for (int j = 0; j < total_categorias_unicas; j++) {
            if (strcmp(lista_categorias_unicas[j], nome_categoria) == 0) { // Função para analisar se a categoria já está listada ou não
                ja_existe = true;
                break;
            }
        }
        
        if (!ja_existe) {
            strcpy(lista_categorias_unicas[total_categorias_unicas], nome_categoria); // Se a categoria não existe, adiciona ela à lista de categorias únicas,
            total_categorias_unicas++;
        }
    }

    printf("Foram encontradas %d categorias unicas.\n\n", total_categorias_unicas); // Função que mostra quantas categorias tem no total

    if (total_categorias_unicas < QUANTIDADE_A_SORTEAR) {
        printf("Nao ha categorias suficientes para sortear: %d...\n", QUANTIDADE_A_SORTEAR); // Função para analisar se o total de categorias é menor que o suficiente
        // Libera a memória já alocada antes de sair com erro
        json_object_put(json_completo);
        free(buffer_json);
        return NULL;
    }

    printf("Sorteando %d Categorias...\n", QUANTIDADE_A_SORTEAR); // O %d é utilizado para mostrar números inteiros

    srand(time(NULL)); // Função para que cada vez que o programa for executado, time(NULL) fornecerá um valor diferente, resultando em uma sequência de números pseudoaleatórios distinta a cada execução.

    // Aloca memória dinamicamente (na HEAP) para o array que será retornado.
    // Esta memória não será destruída quando a função terminar.
    char **categorias_sorteadas = malloc(QUANTIDADE_A_SORTEAR * sizeof(char*));
    if (categorias_sorteadas == NULL) {
        json_object_put(json_completo); // Função que serve para retornar um erro crítico caso tenha falhas na alocação de memórias
        free(buffer_json);
        return NULL;
    }

    int indices_ja_sorteados[QUANTIDADE_A_SORTEAR]; // Variável que vai definir se o índice sorteado já não foi sorteado antes

    for (int i = 0; i < QUANTIDADE_A_SORTEAR; i++) { // Laço para iniciar a análise das categorias para o sorteio
        int indice_sorteado; // Variável para indicar qual está sendo analisado
        bool eh_novo; // Variável para definir se foi um índice já sorteado ou não

        do { // Loop que vai garantir que o índice sorteado seja novo
            eh_novo = true;
            indice_sorteado = rand() % total_categorias_unicas; // Função para sortear um índice entre 0 e total_categorias_unicas

            for (int j = 0; j < i; j++) {
                if (indices_ja_sorteados[j] == indice_sorteado) {
                    eh_novo = false; // Verifica se este índice já foi sorteado antes
                    break; // Se já foi sorteado antes, a repetição quebra
                }
            }
        } while (!eh_novo); // Continua sorteando até encontrar um índice novo

        indices_ja_sorteados[i] = indice_sorteado; // Variável para guardar os índices que já foram sorteados
        
        categorias_sorteadas[i] = strdup(lista_categorias_unicas[indice_sorteado]); /* Copia o conteúo do categorias_sorteadas
         para uma nova variável que vai permanecer na memória mesmo após a função terminar, por meio da memória dinâmica (HEAP)*/
    }

    json_object_put(json_completo); // Libera a memória que foi usada apenas DENTRO desta função
    free(buffer_json);

    return categorias_sorteadas; // Retorna o ponteiro para o array alocado dinamicamente.
}

// // // // // // // // //  Função Principal: Orquestra o sorteio e gerencia a memória // // // // // // // // // 
int main() {
    // Chama a função e armazena o ponteiro para o array de resultados
    char **resultado_sorteio = sortear();

    // [NOVO] É CRUCIAL verificar se o ponteiro é NULL. Se for, significa que ocorreu um erro.
    if (resultado_sorteio != NULL) {
        printf("\n--- Categorias Sorteadas (impresso pela funcao main) ---\n");
        for (int i = 0; i < QUANTIDADE_A_SORTEAR; i++) {
            printf("%d: %s\n", i + 1, resultado_sorteio[i]);
        }
        
        // ------------------------ IMPORTANTE ! Função para liberar a memória utilizada ------------------------
        // [NOVO] Como a memória foi alocada por sortear(), 'main' é agora responsável por liberá-la.
        printf("\nLiberando memoria alocada...\n");
        for (int i = 0; i < QUANTIDADE_A_SORTEAR; i++) {
            free(resultado_sorteio[i]); // Primeiro, libera cada string individual que foi criada com strdup()
        }
        free(resultado_sorteio); // Depois, libera o array de ponteiros que foi criado com malloc()
        printf("Memoria liberada com sucesso.\n");

    } else {
        printf("\nOcorreu um erro durante o processo de sorteio...\n");
        return 1; // Termina o programa com um código de erro
    }
    
    return 0; 
}
