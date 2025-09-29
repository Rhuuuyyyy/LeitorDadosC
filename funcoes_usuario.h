#ifndef FUNCOES_USUARIO_H // Isso cria um "guarda cabeçalho" para que o compilador não tenha que ler esse arquivo mais de uma vez...
#define FUNCOES_USUARIO_H

#include <stdbool.h>

typedef struct { // Isso cria uma estrutura de dados pré-definida para utilizarmos durante o código.
    int numero;
    char* descricao;
    double umidade;
    int energia_kcal;
    double proteina;
    double carboidrato;
    char* categoria;
} Alimento; // A estrutura se chama "Alimento", para que possamos utilizá-la de maneira mais prática, ao invés de sempre repetir toda a estrutura.

typedef enum { // O enum serve para atribuir números inteiros aos atributos que adicionamos, ou seja: CEREAIS = 0, VERDURAS = 1, FRUTAS = 2, etc... Dessa forma, será mais fácil para o processador sortear e comparar.
    CEREAIS, VERDURAS, FRUTAS, GORDURAS, PESCADOS, CARNES, LEITE,
    BEBIDAS, OVOS, ACUCARADOS, MISCELANEAS, INDUSTRIALIZADOS,
    PREPARADOS, LEGUMINOSAS, NOZES, CATEGORIA_INVALIDA
} TipoCategoria;

// Protótipos das Funções do Usuário 

void exibir_menu(); // Função que vai exibir os prints das opções do menu para o usuário...
void processar_opcao(char opcao, Alimento** alimentos, int total); // Função que vai guardar a opção do usuário, e chamar a função de sua respectiva escolha...
void listar_categorias_unicas(Alimento** alimentos, int total); // Função para imprimir na tela todas as categorias sem repetições...

#endif // FUNCOES_USUARIO_H

