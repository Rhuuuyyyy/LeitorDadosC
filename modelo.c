
#include <stdio.h>    // Para funções de entrada e saída, como printf e fopen
#include <stdlib.h>   // Para alocação de memória (malloc), sorteio (rand, srand)
#include <string.h>   // Para manipulação de strings, como strcmp e strcpy
#include <time.h>     // Para usar o tempo atual como "semente" do sorteio (time)
#include <stdbool.h>  // Para usar o tipo de dado 'bool' (true/false)
#include <json-c/json.h> // A biblioteca que nos ajuda a ler o JSON

#define QUANTIDADE_A_SORTEAR 10

int main() {
    // --- PASSO 1: Ler o arquivo JSON ---
    printf("Lendo o arquivo dados.json...\n");

    FILE *arquivo = fopen("dados.json", "r");
    if (arquivo == NULL) {
        perror("Nao foi possivel abrir o arquivo 'dados.json'. Verifique se ele esta na mesma pasta.");
        return 1;
    }

    // Lê o arquivo inteiro para a memória
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);
    char *buffer_json = malloc(tamanho + 1);
    fread(buffer_json, 1, tamanho, arquivo);
    fclose(arquivo);
    buffer_json[tamanho] = '\0';

    // --- PASSO 2: Interpretar o conteúdo JSON ---
    printf("Analisando o conteudo do JSON...\n");
    json_object *json_completo = json_tokener_parse(buffer_json);
    
    // --- PASSO 3: Encontrar todas as categorias únicas ---
    char lista_categorias_unicas[100][100]; // Matriz para guardar até 100 nomes de categoria de até 99 letras
    int total_categorias_unicas = 0;

    // Pega o número de itens no array JSON
    size_t total_de_itens = json_object_array_length(json_completo);

    // Loop por cada item do JSON
    for (int i = 0; i < total_de_itens; i++) {
        json_object *item = json_object_array_get_idx(json_completo, i);
        json_object *valor_categoria;
        
        // Pega o valor da chave "Categoria"
        json_object_object_get_ex(item, "Categoria", &valor_categoria);
        const char *nome_categoria = json_object_get_string(valor_categoria);

        // Verifica se essa categoria já está na nossa lista de únicas
        bool ja_existe = false;
        for (int j = 0; j < total_categorias_unicas; j++) {
            if (strcmp(lista_categorias_unicas[j], nome_categoria) == 0) {
                ja_existe = true;
                break;
            }
        }

        // Se não existe, adiciona na lista
        if (!ja_existe) {
            strcpy(lista_categorias_unicas[total_categorias_unicas], nome_categoria);
            total_categorias_unicas++;
        }
    }

    printf("Foram encontradas %d categorias unicas.\n\n", total_categorias_unicas);

    // Verifica se temos categorias suficientes para o sorteio
    if (total_categorias_unicas < QUANTIDADE_A_SORTEAR) {
        printf("ERRO: Nao ha categorias suficientes para sortear %d.\n", QUANTIDADE_A_SORTEAR);
        return 1;
    }

    // --- PASSO 4: Sortear 10 categorias sem repetir ---
    printf("--- Sorteando %d Categorias ---\n", QUANTIDADE_A_SORTEAR);
    
    // Prepara o gerador de números aleatórios
    srand(time(NULL)); 
    
    char *categorias_sorteadas[QUANTIDADE_A_SORTEAR];
    int indices_ja_sorteados[QUANTIDADE_A_SORTEAR];

    for (int i = 0; i < QUANTIDADE_A_SORTEAR; i++) {
        int indice_sorteado;
        bool eh_novo;

        // Loop para garantir que o índice sorteado seja novo
        do {
            eh_novo = true;
            // Sorteia um índice entre 0 e (total_categorias_unicas - 1)
            indice_sorteado = rand() % total_categorias_unicas;

            // Verifica se este índice já foi sorteado antes
            for (int j = 0; j < i; j++) {
                if (indices_ja_sorteados[j] == indice_sorteado) {
                    eh_novo = false; // Opa, já sorteamos esse. Tente de novo.
                    break;
                }
            }
        } while (!eh_novo); // Continua sorteando até encontrar um índice novo

        // Guarda o índice para não repeti-lo e a categoria correspondente
        indices_ja_sorteados[i] = indice_sorteado;
        categorias_sorteadas[i] = lista_categorias_unicas[indice_sorteado];
    }

    // --- PASSO 5: Mostrar o resultado ---
    for (int i = 0; i < QUANTIDADE_A_SORTEAR; i++) {
        printf("%d: %s\n", i + 1, categorias_sorteadas[i]);
    }
    
    // --- PASSO FINAL: Liberar a memória utilizada ---
    json_object_put(json_completo);
    free(buffer_json);

    return 0; // Sucesso!
}

