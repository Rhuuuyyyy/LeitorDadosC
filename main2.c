#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <json-c/json.h>

#define TOTAL_ALIMENTOS_A_SORTEAR 100
#define MIN_CATEGORIAS_A_USAR 10
#define MIN_ALIMENTOS_POR_CATEGORIA 5
#define MAX_ALIMENTOS_POR_CATEGORIA 20
#define ARQUIVO_SAIDA "alimentos_selecionados.csv" // Esse será o arquivo que o código irá gerar assim que o sorteio dos dados acontecer.

typedef struct { // Isso cria uma estrutura de dados pré-definida para utilizarmos durante o código.
    int numero;
    char* descricao;
    double umidade;
    int energia_kcal;
    double proteina;
    double carboidrato;
    char* categoria;
} Alimento; // A estrutura se chama "Alimento", para que possamos utilizá-la de maneira mais prática, ao invés de sempre repetir toda a estrutura.

// Enum para representar as categorias, conforme requisito do trabalho
typedef enum { // O enum serve para atribuir números inteiros aos atributos que adicionamos, ou seja: CEREAIS = 0, VERDURAS = 1, FRUTAS = 2, etc... Dessa forma, será mais fácil para o processador sortear e comparar.
    CEREAIS, VERDURAS, FRUTAS, GORDURAS, PESCADOS, CARNES, LEITE,
    BEBIDAS, OVOS, ACUCARADOS, MISCELANEAS, INDUSTRIALIZADOS,
    PREPARADOS, LEGUMINOSAS, NOZES, CATEGORIA_INVALIDA
} TipoCategoria;


                    // AQUI FICAM OS PROTÓTIPOS DAS FUNÇÕES... //
/* Explicação: 
O compilador lê nosso código de cima para baixo, então, se no main() ele ler uma função que não existe, o compilador retornará um erro. 
Os protótipos das funções servem para evitar esse erro. 
Então acontece o seguinte:
Antes de iniciar o main(), o compilador lê a lista dos protótipos apenas com os nomes e tipos de dados das funções, e guarda na memória que essas funções
existem, e provavelmente vão ser esclarecidas no final do código.
Sendo assim, o main() poderá ler funções que aparecem no final do código... */

// Funções da Fase 1
Alimento** ler_alimentos_do_json(const char* nome_arquivo, int* total_alimentos); // O "Alimento**" retorna um ponteiro de um vetor que guarda os ponteiros de cada um dos alimentos..
Alimento** sortear_100_alimentos(Alimento** todos_alimentos, int total_alimentos, int* total_sorteados); // O "Alimento**" retorna um ponteiro de um vetor que guarda os ponteiros dos alimentos sorteados..
void escrever_alimentos_em_csv(Alimento** alimentos_selecionados, int total); // Essa função vai servir apenas para escrever as informações no arquivo csv...

// Funções da Fase 2
Alimento** ler_alimentos_do_csv(const char* nome_arquivo, int* total_alimentos); // Irá retornar os ponteiros de uma nova ficha dos 100 alimentos que estarão presentes no arquivo csv...
void exibir_menu(); // Função que vai exibir os prints das opções do menu para o usuário...
void processar_opcao(char opcao, Alimento** alimentos, int total); // Função que vai guardar a opção do usuário, e chamar a função de sua respectiva escolha...
void listar_categorias_unicas(Alimento** alimentos, int total); // Função para imprimir na tela todas as categorias sem repetições...

// Função utilitária
void liberar_memoria_alimentos(Alimento** alimentos, int total); // Essa função vai liberar a memória utilizada...


                    // AQUI É O INÍCIO DA FUNÇÃO PRINCIPAL... //
int main() {
    srand(time(NULL));

    printf("--- INICIANDO FASE 1: Geracao do arquivo de dados ---\n");
    int total_alimentos_lidos = 0;
    Alimento** todos_alimentos = ler_alimentos_do_json("dados.json", &total_alimentos_lidos);
    if (todos_alimentos == NULL) return 1;
    
    int total_alimentos_sorteados = 0;
    Alimento** alimentos_sorteados = sortear_100_alimentos(todos_alimentos, total_alimentos_lidos, &total_alimentos_sorteados);
    
    if (alimentos_sorteados != NULL) {
        escrever_alimentos_em_csv(alimentos_sorteados, total_alimentos_sorteados);
        printf("Arquivo '%s' gerado com %d alimentos.\n", ARQUIVO_SAIDA, total_alimentos_sorteados);
    } else {
        printf("Nao foi possivel sortear os alimentos. Encerrando.\n");
        liberar_memoria_alimentos(todos_alimentos, total_alimentos_lidos);
        return 1;
    }
    liberar_memoria_alimentos(todos_alimentos, total_alimentos_lidos);
    free(alimentos_sorteados);
    printf("--- FIM DA FASE 1 ---\n");

    // ===================================================================================
    // FASE 2: LER O ARQUIVO CSV GERADO E INTERAGIR COM O USUÁRIO
    // ===================================================================================
    printf("\n--- INICIANDO FASE 2: Leitura e analise dos dados ---\n");
    int total_alimentos_csv = 0;
    Alimento** alimentos_para_analise = ler_alimentos_do_csv(ARQUIVO_SAIDA, &total_alimentos_csv);

    if(alimentos_para_analise == NULL) {
        fprintf(stderr, "Erro ao ler o arquivo CSV. Encerrando.\n");
        return 1;
    }
    printf("Sucesso! %d alimentos foram carregados do CSV para analise.\n", total_alimentos_csv);

    // (NOVO) Loop do menu de interação
    char opcao;
    do {
        exibir_menu();
        scanf(" %c", &opcao); // Espaço antes de %c para consumir quebras de linha pendentes
        processar_opcao(opcao, alimentos_para_analise, total_alimentos_csv);
    } while (opcao != 'j' && opcao != 'J');

    // Libera a memória usada na Fase 2
    liberar_memoria_alimentos(alimentos_para_analise, total_alimentos_csv);
    
    printf("\nPrograma finalizado.\n");
    return 0;
}

/**
 * @brief (NOVO) Exibe o menu de opções para o usuário.
 */
void exibir_menu() {





--- AQUI DEVE TER O PRINT DO MENU DE OPÇÕES DO USUÁRIO ---







}

/**
 * @brief (NOVO) Chama a função correspondente à opção do usuário.
 */
void processar_opcao(char opcao, Alimento** alimentos, int total) {
    switch (opcao) {
        case 'a': case 'A':
        case 'b': case 'B':
        case 'c': case 'C':
        case 'd': case 'D':
        case 'e': case 'E':
        case 'f': case 'F':
        case 'g': case 'G':
        case 'h': case 'H':
        case 'i': case 'I':
        case 'j': case 'J':
    }
}











--- AQUI DEVEM SER COLOCADAS AS FUNÇÕES QUE IRÃO SER CHAMADAS DE ACORDO COM A OPÇÃO QUE O USUÁRIO ESCOLHER













// --- Funções da Fase 1 e de Leitura/Liberação de Memória (sem alterações) ---
Alimento** ler_alimentos_do_csv(const char* nome_arquivo, int* total_alimentos) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        perror("Nao foi possivel abrir o arquivo CSV");
        return NULL;
    }
    Alimento** alimentos = (Alimento**)malloc(TOTAL_ALIMENTOS_A_SORTEAR * sizeof(Alimento*));
    if (alimentos == NULL) {
        fclose(arquivo);
        return NULL;
    }
    char linha[1024];
    int contador = 0;
    fgets(linha, sizeof(linha), arquivo);
    while (fgets(linha, sizeof(linha), arquivo) && contador < TOTAL_ALIMENTOS_A_SORTEAR) {
        alimentos[contador] = (Alimento*)malloc(sizeof(Alimento));
        char descricao_temp[256];
        char categoria_temp[100];
        sscanf(linha, "%d;\"%[^\"]\";%lf;%d;%lf;%lf;\"%[^\"]\"",
            &alimentos[contador]->numero, descricao_temp, &alimentos[contador]->umidade,
            &alimentos[contador]->energia_kcal, &alimentos[contador]->proteina,
            &alimentos[contador]->carboidrato, categoria_temp);
        alimentos[contador]->descricao = strdup(descricao_temp);
        alimentos[contador]->categoria = strdup(categoria_temp);
        contador++;
    }
    fclose(arquivo);
    *total_alimentos = contador;
    return alimentos;
}

typedef struct { char* nome; Alimento** alimentos; int quantidade; } GrupoCategoria;

Alimento** sortear_100_alimentos(Alimento** todos_alimentos, int total_alimentos, int* total_sorteados) {
    char lista_categorias_unicas[100][100]; int total_categorias_unicas = 0;
    for (int i = 0; i < total_alimentos; i++) {
        bool ja_existe = false;
        for (int j = 0; j < total_categorias_unicas; j++) { if (strcmp(lista_categorias_unicas[j], todos_alimentos[i]->categoria) == 0) { ja_existe = true; break; } }
        if (!ja_existe) { strcpy(lista_categorias_unicas[total_categorias_unicas++], todos_alimentos[i]->categoria); }
    }
    GrupoCategoria* grupos = (GrupoCategoria*)malloc(total_categorias_unicas * sizeof(GrupoCategoria));
    for (int i = 0; i < total_categorias_unicas; i++) {
        grupos[i].nome = strdup(lista_categorias_unicas[i]);
        grupos[i].alimentos = (Alimento**)malloc(total_alimentos * sizeof(Alimento*));
        grupos[i].quantidade = 0;
    }
    for (int i = 0; i < total_alimentos; i++) {
        for (int j = 0; j < total_categorias_unicas; j++) { if (strcmp(todos_alimentos[i]->categoria, grupos[j].nome) == 0) { grupos[j].alimentos[grupos[j].quantidade++] = todos_alimentos[i]; break; } }
    }
    GrupoCategoria** categorias_elegiveis = (GrupoCategoria**)malloc(total_categorias_unicas * sizeof(GrupoCategoria*));
    int total_elegiveis = 0;
    for (int i = 0; i < total_categorias_unicas; i++) { if (grupos[i].quantidade >= MIN_ALIMENTOS_POR_CATEGORIA) { categorias_elegiveis[total_elegiveis++] = &grupos[i]; } }
    if (total_elegiveis < MIN_CATEGORIAS_A_USAR) { fprintf(stderr, "Erro: Nao ha categorias elegiveis suficientes (%d) para o sorteio. Minimo necessario: %d\n", total_elegiveis, MIN_CATEGORIAS_A_USAR); return NULL; }
    for (int i = 0; i < total_elegiveis - 1; i++) { int j = i + rand() % (total_elegiveis - i); GrupoCategoria* temp = categorias_elegiveis[j]; categorias_elegiveis[j] = categorias_elegiveis[i]; categorias_elegiveis[i] = temp; }
    Alimento** alimentos_sorteados = (Alimento**)malloc(TOTAL_ALIMENTOS_A_SORTEAR * sizeof(Alimento*));
    int contador_sorteados = 0; int contagem_por_categoria[MIN_CATEGORIAS_A_USAR] = {0};
    for (int i = 0; i < MIN_CATEGORIAS_A_USAR; i++) {
        GrupoCategoria* categoria_atual = categorias_elegiveis[i];
        for (int k = 0; k < categoria_atual->quantidade - 1; k++) { int l = k + rand() % (categoria_atual->quantidade - k); Alimento* temp = categoria_atual->alimentos[l]; categoria_atual->alimentos[l] = categoria_atual->alimentos[k]; categoria_atual->alimentos[k] = temp; }
        for (int k = 0; k < MIN_ALIMENTOS_POR_CATEGORIA; k++) { alimentos_sorteados[contador_sorteados++] = categoria_atual->alimentos[k]; }
        contagem_por_categoria[i] = MIN_ALIMENTOS_POR_CATEGORIA;
    }
    while (contador_sorteados < TOTAL_ALIMENTOS_A_SORTEAR) {
        int indice_categoria_sorteada = rand() % MIN_CATEGORIAS_A_USAR;
        if (contagem_por_categoria[indice_categoria_sorteada] < MAX_ALIMENTOS_POR_CATEGORIA) {
            GrupoCategoria* cat_sorteada = categorias_elegiveis[indice_categoria_sorteada];
            int proximo_alimento_idx = contagem_por_categoria[indice_categoria_sorteada];
            if (proximo_alimento_idx < cat_sorteada->quantidade) { alimentos_sorteados[contador_sorteados++] = cat_sorteada->alimentos[proximo_alimento_idx]; contagem_por_categoria[indice_categoria_sorteada]++; }
        }
    }
    *total_sorteados = contador_sorteados;
    for (int i = 0; i < total_categorias_unicas; i++) { free(grupos[i].nome); free(grupos[i].alimentos); }
    free(grupos); free(categorias_elegiveis);
    return alimentos_sorteados;
}

void escrever_alimentos_em_csv(Alimento** alimentos_selecionados, int total) {
    FILE* arquivo = fopen(ARQUIVO_SAIDA, "w");
    if (arquivo == NULL) { perror("Nao foi possivel criar o arquivo de saida"); return; }
    fprintf(arquivo, "Numero;Descricao;Umidade;Energia_kcal;Proteina;Carboidrato;Categoria\n");
    for (int i = 0; i < total; i++) {
        if (alimentos_selecionados[i] == NULL) continue;
        fprintf(arquivo, "%d;\"%s\";%.1f;%d;%.1f;%.1f;\"%s\"\n", alimentos_selecionados[i]->numero, alimentos_selecionados[i]->descricao, alimentos_selecionados[i]->umidade, alimentos_selecionados[i]->energia_kcal, alimentos_selecionados[i]->proteina, alimentos_selecionados[i]->carboidrato, alimentos_selecionados[i]->categoria);
    }
    fclose(arquivo);
}

Alimento** ler_alimentos_do_json(const char* nome_arquivo, int* total_alimentos) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) { perror("Nao foi possivel abrir o arquivo dados.json"); return NULL; }
    fseek(arquivo, 0, SEEK_END); long tamanho = ftell(arquivo); fseek(arquivo, 0, SEEK_SET);
    char* buffer_json = (char*)malloc(tamanho + 1);
    if (buffer_json == NULL) { fprintf(stderr, "Falha ao alocar memoria para o buffer do JSON.\n"); fclose(arquivo); return NULL; }
    fread(buffer_json, 1, tamanho, arquivo); fclose(arquivo); buffer_json[tamanho] = '\0';
    json_object* json_completo = json_tokener_parse(buffer_json);
    if (json_completo == NULL) { fprintf(stderr, "Erro ao interpretar o conteudo JSON.\n"); free(buffer_json); return NULL; }
    size_t n_alimentos = json_object_array_length(json_completo); *total_alimentos = n_alimentos;
    Alimento** array_alimentos = (Alimento**)malloc(n_alimentos * sizeof(Alimento*));
    if (array_alimentos == NULL) { fprintf(stderr, "Falha ao alocar memoria para o array de alimentos.\n"); free(buffer_json); json_object_put(json_completo); return NULL; }
    for (int i = 0; i < n_alimentos; i++) {
        json_object* item = json_object_array_get_idx(json_completo, i);
        array_alimentos[i] = (Alimento*)malloc(sizeof(Alimento));
        json_object *j_numero, *j_descricao, *j_umidade, *j_energia, *j_proteina, *j_carboidrato, *j_categoria;
        json_object_object_get_ex(item, "Numero", &j_numero); json_object_object_get_ex(item, "Descricao", &j_descricao); json_object_object_get_ex(item, "Umidade", &j_umidade); json_object_object_get_ex(item, "Energia_kcal", &j_energia); json_object_object_get_ex(item, "Proteina", &j_proteina); json_object_object_get_ex(item, "Carboidrato", &j_carboidrato); json_object_object_get_ex(item, "Categoria", &j_categoria);
        array_alimentos[i]->numero = json_object_get_int(j_numero); array_alimentos[i]->descricao = strdup(json_object_get_string(j_descricao)); array_alimentos[i]->umidade = json_object_get_double(j_umidade); array_alimentos[i]->energia_kcal = json_object_get_int(j_energia); array_alimentos[i]->proteina = json_object_get_double(j_proteina); array_alimentos[i]->carboidrato = json_object_get_double(j_carboidrato); array_alimentos[i]->categoria = strdup(json_object_get_string(j_categoria));
    }
    json_object_put(json_completo); free(buffer_json);
    return array_alimentos;
}

void liberar_memoria_alimentos(Alimento** alimentos, int total) {
    if (alimentos == NULL) return;
    for (int i = 0; i < total; i++) {
        if (alimentos[i] != NULL) {
            free(alimentos[i]->descricao);
            free(alimentos[i]->categoria);
            free(alimentos[i]);
        }
    }
    free(alimentos);
}