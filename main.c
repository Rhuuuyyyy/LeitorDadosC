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
    srand(time(NULL)); // Função para que cada vez que o programa for executado, time(NULL) fornecerá um valor diferente, resultando em uma sequência de números pseudoaleatórios distinta a cada execução.

    // ===================================================================================
    // SORTEAR 100 ALIMENTOS E GERAR O ARQUIVO CSV
    // ===================================================================================

    printf("Gerando o arquivo de dados...\n");
    int total_alimentos_lidos = 0;
    Alimento** todos_alimentos = ler_alimentos_do_json("dados.json", &total_alimentos_lidos); 
    /* O "Alimento**" retorna um ponteiro de um vetor que guarda os ponteiros de cada um dos alimentos, e o "todos_alimentos" é o nome da variável que vai guardar esse ponteiro...
    o "dados.json" é o arquivo que a função vai ler, e o "&total_alimentos_lidos" é a variável que será guardada o resultado da consulta... */
    if (todos_alimentos == NULL) {
        fprintf(stderr, "Erro ao ler os dados do arquivo JSON. Encerrando.\n");
        return 1;
    } // O return 1 serve para indicar que houve um erro na execução do programa...
    
    int total_alimentos_sorteados = 0;
    Alimento** alimentos_sorteados = sortear_100_alimentos(todos_alimentos, total_alimentos_lidos, &total_alimentos_sorteados);
    /* O "Alimento**" retorna um ponteiro de um vetor que guarda os ponteiros dos alimentos sorteados, e o "alimentos_sorteados" é o nome da variável que vai guardar esse ponteiro... */   
    
    if (alimentos_sorteados != NULL) { // Se o total de alimentos sorteados for diferente de NULL, ou seja, se o sorteio foi realizado com sucesso, então:
        escrever_alimentos_em_csv(alimentos_sorteados, total_alimentos_sorteados); // Chama a função que escreve os alimentos sorteados no arquivo csv...
        printf("Arquivo '%s' gerado com %d alimentos.\n", ARQUIVO_SAIDA, total_alimentos_sorteados);
    } else {
        printf("Nao foi possivel sortear os alimentos. Encerrando.\n");
        liberar_memoria_alimentos(todos_alimentos, total_alimentos_lidos);
        return 1;
    }
    liberar_memoria_alimentos(todos_alimentos, total_alimentos_lidos);
    free(alimentos_sorteados);

    // ===================================================================================
    // LER O ARQUIVO CSV GERADO E INTERAGIR COM O USUÁRIO
    // ===================================================================================
    printf("\nLendo e analisando os dados...\n");
    int total_alimentos_csv = 0;
    Alimento** alimentos_para_analise = ler_alimentos_do_csv(ARQUIVO_SAIDA, &total_alimentos_csv); 
    /* O "Alimento**" retorna um ponteiro de um vetor que guarda os ponteiros de cada um dos alimentos, e o "alimentos_para_analise" é o nome da variável que vai guardar esse ponteiro... */

    if(alimentos_para_analise == NULL) {
        fprintf(stderr, "Erro ao ler o arquivo CSV. Encerrando...\n");
        return 1;
    }
    printf("Sucesso! %d alimentos foram carregados do CSV para analise.\n");

    // ===================================================================================
    // MOSTRAR MENU DE INTERAÇÃO 
    // ===================================================================================

    char opcao;
    do {
        exibir_menu();
        scanf(" %c", &opcao); // Espaço antes de %c para consumir quebras de linha pendentes
        processar_opcao(opcao, alimentos_para_analise, total_alimentos_csv);
    } while (opcao != 'j' && opcao != 'J'); // Pois o J é a opção de sair do programa...

    // Libera a memória usado para armazenar os alimentos
    liberar_memoria_alimentos(alimentos_para_analise, total_alimentos_csv);
    
    printf("\nPrograma finalizado...\n");
    return 0;
}

// ===================================================================================
// FUNÇÃO PARA EXIBIR O MENU DE OPÇÕES DO USUÁRIO
// ===================================================================================
void exibir_menu() {
    // Esta função tem um trabalho simples: apenas imprimir o menu de opções na tela
    // para que o usuário saiba quais letras digitar para cada ação.





        DEVE SER COMPLETO AQUI...









}

// ===================================================================================
// FUNÇÃO PARA PROCESSAR A OPÇÃO ESCOLHIDA PELO USUÁRIO
// ===================================================================================
void processar_opcao(char opcao, Alimento** alimentos, int total) {
    switch (opcao) {
        case 'a': case 'A':
            // Se o usuário digitou 'a' ou 'A', chama a função que lista as categorias.
            listar_categorias_unicas(alimentos, total);
            break;
        case 'b': case 'B':
        case 'c': case 'C':
        case 'd': case 'D':
        case 'e': case 'E':                  DEVE SER COMPLETO AQUI...
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
            strcpy(lista_unicas[total_unicas], alimentos[i]->categoria); // ...anota o nome da categoria na "folha de papel".
            total_unicas++; // E aumenta o contador de categorias anotadas.
        }
    }

    // No final, imprime a lista final de categorias que foi anotada.
    printf("\n--- Categorias Disponiveis para Analise ---\n");
    for (int i = 0; i < total_unicas; i++) {
        printf("- %s\n", lista_unicas[i]);
    }
}

// ===================================================================================
// FUNÇÃO PARA LER OS DADOS DO ARQUIVO .CSV
// ===================================================================================
Alimento** ler_alimentos_do_csv(const char* nome_arquivo, int* total_alimentos) {
    // Esta função é uma especialista em ler o arquivo .csv (que é um tipo de texto simples, tipo planilha).
    FILE* arquivo = fopen(nome_arquivo, "r"); // Tenta abrir o arquivo.
    if (arquivo == NULL) { // Se não conseguir (ex: arquivo não existe), avisa o erro e para.
        perror("Nao foi possivel abrir o arquivo CSV");
        return NULL;
    }
    
    // Prepara uma nova "agenda" para guardar os 100 alimentos que serão lidos.
    Alimento** alimentos = (Alimento**)malloc(TOTAL_ALIMENTOS_A_SORTEAR * sizeof(Alimento*));
    if (alimentos == NULL) { // Se não houver memória, para.
        fclose(arquivo);
        return NULL;
    }

    char linha[1024]; // Um espaço temporário para guardar cada linha lida do arquivo.
    int contador = 0; // Contador de quantos alimentos já foram lidos.

    fgets(linha, sizeof(linha), arquivo); // Lê e descarta a primeira linha, que é o cabeçalho ("Numero;Descricao;...").

    // Enquanto conseguir ler uma linha do arquivo e ainda não tivermos lido 100 alimentos...
    while (fgets(linha, sizeof(linha), arquivo) && contador < TOTAL_ALIMENTOS_A_SORTEAR) {
        alimentos[contador] = (Alimento*)malloc(sizeof(Alimento)); // Cria uma nova "ficha de alimento" em branco na memória.
        
        char descricao_temp[256]; // Espaços temporários para guardar os textos.
        char categoria_temp[100];

        // "sscanf" é como um "preenchedor de formulário". Ele olha a 'linha' de texto e,
        // com base no formato que passamos, extrai as informações e as coloca nas variáveis.
        sscanf(linha, "%d;\"%[^\"]\";%lf;%d;%lf;%lf;\"%[^\"]\"",
            &alimentos[contador]->numero, descricao_temp, &alimentos[contador]->umidade,
            &alimentos[contador]->energia_kcal, &alimentos[contador]->proteina,
            &alimentos[contador]->carboidrato, categoria_temp);

        // Copia os textos dos espaços temporários para a "ficha de alimento" final.
        alimentos[contador]->descricao = strdup(descricao_temp);
        alimentos[contador]->categoria = strdup(categoria_temp);

        contador++; // Aumenta o contador de alimentos lidos.
    }

    fclose(arquivo); // Fecha o arquivo.
    *total_alimentos = contador; // Informa à 'main' quantos alimentos foram lidos.
    return alimentos; // Retorna a "agenda" com os 100 alimentos.
}

// Estrutura auxiliar usada apenas pela função de sorteio.
typedef struct { char* nome; Alimento** alimentos; int quantidade; } GrupoCategoria;

// ===================================================================================
// FUNÇÃO PARA SORTEAR OS 100 ALIMENTOS
// ===================================================================================
Alimento** sortear_100_alimentos(Alimento** todos_alimentos, int total_alimentos, int* total_sorteados) {
    // --- PASSO 1: Agrupar todos os alimentos por categoria ---
    // Primeiro, descobre todas as categorias únicas e depois cria grupos,
    // onde cada grupo tem o nome da categoria e uma lista de todos os alimentos que pertencem a ela.
    char lista_categorias_unicas[100][100]; int total_categorias_unicas = 0;
    for (int i = 0; i < total_alimentos; i++) { bool ja_existe = false; for (int j = 0; j < total_categorias_unicas; j++) { if (strcmp(lista_categorias_unicas[j], todos_alimentos[i]->categoria) == 0) { ja_existe = true; break; } } if (!ja_existe) { strcpy(lista_categorias_unicas[total_categorias_unicas++], todos_alimentos[i]->categoria); } }
    GrupoCategoria* grupos = (GrupoCategoria*)malloc(total_categorias_unicas * sizeof(GrupoCategoria));
    for (int i = 0; i < total_categorias_unicas; i++) { grupos[i].nome = strdup(lista_categorias_unicas[i]); grupos[i].alimentos = (Alimento**)malloc(total_alimentos * sizeof(Alimento*)); grupos[i].quantidade = 0; }
    for (int i = 0; i < total_alimentos; i++) { for (int j = 0; j < total_categorias_unicas; j++) { if (strcmp(todos_alimentos[i]->categoria, grupos[j].nome) == 0) { grupos[j].alimentos[grupos[j].quantidade++] = todos_alimentos[i]; break; } } }
    
    // --- PASSO 2: Filtrar categorias que são grandes o suficiente para o sorteio (mínimo de 5 alimentos).
    GrupoCategoria** categorias_elegiveis = (GrupoCategoria**)malloc(total_categorias_unicas * sizeof(GrupoCategoria*));
    int total_elegiveis = 0;
    for (int i = 0; i < total_categorias_unicas; i++) { if (grupos[i].quantidade >= MIN_ALIMENTOS_POR_CATEGORIA) { categorias_elegiveis[total_elegiveis++] = &grupos[i]; } }
    if (total_elegiveis < MIN_CATEGORIAS_A_USAR) { fprintf(stderr, "Erro: Nao ha categorias elegiveis suficientes (%d) para o sorteio. Minimo necessario: %d\n", total_elegiveis, MIN_CATEGORIAS_A_USAR); return NULL; }
    
    // --- PASSO 3: Embaralhar a lista de categorias elegíveis para sortear 10 delas.
    for (int i = 0; i < total_elegiveis - 1; i++) { int j = i + rand() % (total_elegiveis - i); GrupoCategoria* temp = categorias_elegiveis[j]; categorias_elegiveis[j] = categorias_elegiveis[i]; categorias_elegiveis[i] = temp; }
    
    // --- PASSO 4: Preparar a lista final e sortear os 5 alimentos obrigatórios de cada uma das 10 primeiras categorias da lista embaralhada.
    Alimento** alimentos_sorteados = (Alimento**)malloc(TOTAL_ALIMENTOS_A_SORTEAR * sizeof(Alimento*));
    int contador_sorteados = 0; int contagem_por_categoria[MIN_CATEGORIAS_A_USAR] = {0};
    for (int i = 0; i < MIN_CATEGORIAS_A_USAR; i++) {
        GrupoCategoria* categoria_atual = categorias_elegiveis[i];
        for (int k = 0; k < categoria_atual->quantidade - 1; k++) { int l = k + rand() % (categoria_atual->quantidade - k); Alimento* temp = categoria_atual->alimentos[l]; categoria_atual->alimentos[l] = categoria_atual->alimentos[k]; categoria_atual->alimentos[k] = temp; }
        for (int k = 0; k < MIN_ALIMENTOS_POR_CATEGORIA; k++) { alimentos_sorteados[contador_sorteados++] = categoria_atual->alimentos[k]; }
        contagem_por_categoria[i] = MIN_ALIMENTOS_POR_CATEGORIA;
    }
    
    // --- PASSO 5: Sortear os 50 alimentos restantes, escolhendo aleatoriamente uma das 10 categorias e pegando um alimento dela,
    // respeitando o limite máximo de 20 por categoria.
    while (contador_sorteados < TOTAL_ALIMENTOS_A_SORTEAR) {
        int indice_categoria_sorteada = rand() % MIN_CATEGORIAS_A_USAR;
        if (contagem_por_categoria[indice_categoria_sorteada] < MAX_ALIMENTOS_POR_CATEGORIA) {
            GrupoCategoria* cat_sorteada = categorias_elegiveis[indice_categoria_sorteada];
            int proximo_alimento_idx = contagem_por_categoria[indice_categoria_sorteada];
            if (proximo_alimento_idx < cat_sorteada->quantidade) { alimentos_sorteados[contador_sorteados++] = cat_sorteada->alimentos[proximo_alimento_idx]; contagem_por_categoria[indice_categoria_sorteada]++; }
        }
    }
    
    *total_sorteados = contador_sorteados; // Informa a 'main' o total sorteado.
    for (int i = 0; i < total_categorias_unicas; i++) { free(grupos[i].nome); free(grupos[i].alimentos); }
    free(grupos); free(categorias_elegiveis); // Limpa a memória auxiliar usada no sorteio.
    return alimentos_sorteados; // Retorna a lista final com os 100 alimentos.
}

// ===================================================================================
// FUNÇÃO PARA ESCREVER OS DADOS EM UM ARQUIVO .CSV
// ===================================================================================
void escrever_alimentos_em_csv(Alimento** alimentos_selecionados, int total) {
    // Esta função pega a lista dos 100 alimentos sorteados e a escreve em um arquivo de texto.
    FILE* arquivo = fopen(ARQUIVO_SAIDA, "w"); // Abre um arquivo em branco para escrita.
    if (arquivo == NULL) { perror("Nao foi possivel criar o arquivo de saida"); return; }
    
    // Escreve a primeira linha, o cabeçalho, no arquivo.
    fprintf(arquivo, "Numero;Descricao;Umidade;Energia_kcal;Proteina;Carboidrato;Categoria\n");
    
    // Percorre a lista dos 100 alimentos...
    for (int i = 0; i < total; i++) {
        if (alimentos_selecionados[i] == NULL) continue;
        // ...e para cada um, escreve seus dados em uma nova linha, separando com ';'.
        fprintf(arquivo, "%d;\"%s\";%.1f;%d;%.1f;%.1f;\"%s\"\n", alimentos_selecionados[i]->numero, alimentos_selecionados[i]->descricao, alimentos_selecionados[i]->umidade, alimentos_selecionados[i]->energia_kcal, alimentos_selecionados[i]->proteina, alimentos_selecionados[i]->carboidrato, alimentos_selecionados[i]->categoria);
    }
    fclose(arquivo); // Fecha o arquivo, salvando as alterações.
}

// ===================================================================================
// FUNÇÃO PARA LER OS DADOS DO ARQUIVO .JSON ORIGINAL
// ===================================================================================
Alimento** ler_alimentos_do_json(const char* nome_arquivo, int* total_alimentos) {
    // Esta função é uma especialista em ler o arquivo .json, que tem uma estrutura mais complexa.
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) { perror("Nao foi possivel abrir o arquivo dados.json"); return NULL; }
    
    // Lê o conteúdo inteiro do arquivo para a memória de uma só vez.
    fseek(arquivo, 0, SEEK_END); long tamanho = ftell(arquivo); fseek(arquivo, 0, SEEK_SET);
    char* buffer_json = (char*)malloc(tamanho + 1);
    if (buffer_json == NULL) { fprintf(stderr, "Falha ao alocar memoria para o buffer do JSON.\n"); fclose(arquivo); return NULL; }
    fread(buffer_json, 1, tamanho, arquivo); fclose(arquivo); buffer_json[tamanho] = '\0';
    
    // Usa a biblioteca 'json-c' para interpretar o texto do arquivo e transformá-lo em objetos que o C entende.
    json_object* json_completo = json_tokener_parse(buffer_json);
    if (json_completo == NULL) { fprintf(stderr, "Erro ao interpretar o conteudo JSON.\n"); free(buffer_json); return NULL; }
    
    size_t n_alimentos = json_object_array_length(json_completo); *total_alimentos = n_alimentos;
    Alimento** array_alimentos = (Alimento**)malloc(n_alimentos * sizeof(Alimento*));
    if (array_alimentos == NULL) { fprintf(stderr, "Falha ao alocar memoria para o array de alimentos.\n"); free(buffer_json); json_object_put(json_completo); return NULL; }
    
    // Percorre cada objeto de alimento no JSON...
    for (int i = 0; i < n_alimentos; i++) {
        json_object* item = json_object_array_get_idx(json_completo, i);
        array_alimentos[i] = (Alimento*)malloc(sizeof(Alimento)); // Cria uma "ficha de alimento" na memória.
        
        // Pega cada dado do objeto JSON (Numero, Descricao, etc.)...
        json_object *j_numero, *j_descricao, *j_umidade, *j_energia, *j_proteina, *j_carboidrato, *j_categoria;
        json_object_object_get_ex(item, "Numero", &j_numero); json_object_object_get_ex(item, "Descricao", &j_descricao); json_object_object_get_ex(item, "Umidade", &j_umidade); json_object_object_get_ex(item, "Energia_kcal", &j_energia); json_object_object_get_ex(item, "Proteina", &j_proteina); json_object_object_get_ex(item, "Carboidrato", &j_carboidrato); json_object_object_get_ex(item, "Categoria", &j_categoria);
        
        // ...e preenche a "ficha de alimento" com esses dados.
        array_alimentos[i]->numero = json_object_get_int(j_numero); array_alimentos[i]->descricao = strdup(json_object_get_string(j_descricao)); array_alimentos[i]->umidade = json_object_get_double(j_umidade); array_alimentos[i]->energia_kcal = json_object_get_int(j_energia); array_alimentos[i]->proteina = json_object_get_double(j_proteina); array_alimentos[i]->carboidrato = json_object_get_double(j_carboidrato); array_alimentos[i]->categoria = strdup(json_object_get_string(j_categoria));
    }
    
    json_object_put(json_completo); free(buffer_json); // Libera a memória usada pela leitura do JSON.
    return array_alimentos; // Retorna a lista completa com os 597 alimentos.
}

// ===================================================================================
// FUNÇÃO PARA LIBERAR A MEMÓRIA (A "EQUIPE DE LIMPEZA")
// ===================================================================================
void liberar_memoria_alimentos(Alimento** alimentos, int total) {
    // Esta função é a "equipe de limpeza". Ela é chamada no final para garantir que toda a memória
    // que pegamos "emprestada" do computador seja devolvida.
    if (alimentos == NULL) return; // Se a lista já estiver vazia, não faz nada.

    // Percorre a lista de alimentos...
    for (int i = 0; i < total; i++) {
        if (alimentos[i] != NULL) {
            // Para cada alimento, devolve a memória usada para guardar a descrição e a categoria.
            free(alimentos[i]->descricao);
            free(alimentos[i]->categoria);
            // Depois, devolve a memória da "ficha de alimento" em si.
            free(alimentos[i]);
        }
    }
    // No final, devolve a memória usada para a "agenda de endereços".
    free(alimentos);
}

