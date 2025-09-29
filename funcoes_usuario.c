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

// ===================================================================================
// FUNÇÃO PARA EXIBIR O MENU DE OPÇÕES DO USUÁRIO
// ===================================================================================
void exibir_menu() {
    



    AQUI DEVE SER FEITO OS PRINTs DAS OPÇÕES PARA O USUÁRIO





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

