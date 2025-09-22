#include <stdio.h>    // Para funções de entrada e saída, como printf e fopen
#include <stdlib.h>   // Para alocação de memória (malloc), sorteio (rand, srand)
#include <string.h>   // Para manipulação de strings, como strcmp e strcpy
#include <time.h>     // Para funções relacionadas ao tempo
#include <stdbool.h>  // Para usar tipos booleanos
#include <json-c/json.h> // Biblioteca para ler JSON

int main() {
    bool abrir = false; // Variável para controlar se a leitura foi bem-sucedida
    int quantidade = 0; // Contador de tentativas de leitura

    // // // // // // // // //  Função para tentar abrir o arquivo // // // // // // // // // 
    do { 
        printf("Abrindo o arquivo...\n");
        FILE *arquivo = fopen("dados.json", "r"); // Tenta abrir o arquivo | O "r" indica que é para leitura
        quantidade++;
        if (arquivo != NULL) { // Verifica se o arquivo foi aberto com sucesso
            abrir = true;
            printf("Arquivo aberto com sucesso...\n");
            fclose(arquivo);
        } 
    } while (quantidade < 5 && !abrir); // Tenta no máximo 5 vezes ou até conseguir abrir
    if (!abrir) {
        printf("Não foi possível ler o arquivo após %d tentativas.\n", quantidade);
    }
    
        // // // // // // // // Função para alocação de memória e leitura do conteúdo do arquivo // // // // // // // // // 
    if (abrir) {
        printf("Lendo o arquivo dados.json...\n");
        fseek(arquivo, 0, SEEK_END); // Move o cursor para o final do arquivo para podermos medir seu tamanho
        long tamanho = ftell(arquivo); // Pega a posição atual do cursor (que é o tamanho total) e guarda na variável 'tamanho'
        fseek(arquivo, 0, SEEK_SET); // Volta o cursor para o início do arquivo, preparando para a leitura do conteúdo
        char *buffer_json = malloc(tamanho + 1); // Aloca na memória um espaço (buffer) grande o suficiente para todo o arquivo | O "+1" é para o caractere final '\0'
        fread(buffer_json, 1, tamanho, arquivo); // Lê 'tamanho' bytes do 'arquivo' e copia para o 'buffer_json' que acabamos de alocar
        fclose(arquivo); // Fecha o arquivo, pois seu conteúdo já foi copiado para a memória e não precisamos mais dele aberto
        buffer_json[tamanho] = '\0'; // Adiciona o caractere terminador nulo '\0' no final do buffer, transformando-o em uma string C válida
        printf("Arquivo lido com sucesso...\n");
        
        // // // // // // // // Função para interpretar o conteúdo JSON // // // // // // // // //
        printf("Analisando o conteudo do JSON...\n");
        json_object *json_completo = json_tokener_parse(buffer_json);

}

