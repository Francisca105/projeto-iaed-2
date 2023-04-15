/*
    Ficheiro: project2.h
    Autora: Francisca Vicente de Almeida (ist1105901)
    Descricao: Ficheiro que contem as definicoes de todas as funcoes
               e estruturas de dados do segundo projeto de IAED
*/

#ifndef MAIN_H
#define MAIN_H

/* Bibliotecas necessarias a importar */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define VERDADEIRO 1
#define FALSO 0

#define PARAGEM_ORIGEM_ENCONTRADA 1
#define PARAGEM_DESTINO_ENCONTRADA 2

/* Numero maximo de caracteres para o input */
#define MAX_INPUT 65535

/* Numero maximo de argumentos para os comandos */
#define MAX_ARGUMENTOS 5

/* Tipo usado para os numeros decimais */
typedef double Reais;

/* Estrutura para a lista de carreiras dentro das paragens */
typedef struct Lista_Carreiras {
    char *nome;
    struct Lista_Carreiras *proxima;
    struct Lista_Carreiras *anterior;
} Lista_Carreiras;

/* Estrutura para as paragens */
typedef struct Paragem {
    char *nome;
    Reais latitude, longitude;
    Lista_Carreiras *carreira;
    int numero_carreiras;
    struct Paragem *proxima;
} Paragem;

/* Estrutura para a ligação */
typedef struct Ligacao {
    char *paragem_origem;
    char *paragem_destino;
    Reais custo;
    Reais tempo;
    struct Ligacao *proxima;
    struct Ligacao *anterior;
} Ligacao;

/* Estrutura para as carreiras */
typedef struct Carreira {
    char *nome;
    Ligacao *primeira_ligacao;
    Ligacao *ultima_ligacao;
    int numero_ligacoes;
    struct Carreira *proxima;
} Carreira;

/* Onde estao guardadas todas as estruturas */
typedef struct {
    Paragem *paragem;
    Paragem *ultima_paragem;
    Carreira *carreira;
    Carreira *ultima_carreira;
} Global;

/* FUNCOES */

/*      Funcoes de adicionar      */
void adicionar_paragem(Global **global, char nome[MAX_INPUT], Reais latitude,
                       Reais longitude);
void adicionar_carreira(Global **global, char nome[MAX_INPUT]);
void adicionar_ligacao(Global **global, Carreira ** carreira, 
                       Paragem ** paragem1, Paragem ** paragem2,
                       Reais custo, Reais tempo);
void adicionar_origem_destino_carreira(Global **global, 
Carreira ** carreira, Ligacao * ligacao, Paragem ** paragem1,
Paragem ** paragem2);
void adicionar_ligacao_carreira(Global **global, Carreira ** carreira, 
Ligacao ** ligacao, Paragem ** paragem1, Paragem ** paragem2, int origem);
void adicionar_carreira_paragem(Global **global, Paragem **paragem, 
                                char *nome);

/*      Funcoes de remover      */
void remover_carreira_paragem(Paragem **paragem, char *nome);
void remover_carreira_paragens(Global **global, Carreira *carreira);
void remover_ligacao_extremos(Carreira **carreira, Ligacao *ligacao,
                              int origem);
void remover_ligacao_meio(Carreira **carreira, Ligacao **ligacao,
                          Ligacao **proxima);

/*      Funcoes de busca      */
void procurar_paragem(Global **global, char *nome, Paragem **paragem);
void procurar_carreira(Global **global, char *nome, Carreira **carreira);

/*      Funcoes de verificacao      */
int verificar_extremidade(Carreira *carreira, char *paragem1, char *paragem2);
int verificar_paragem_ligacao(Carreira *carreira, char *paragem);
int verificar_carreira_paragem(Paragem *paragem, char *carreira);

/*      Funcoes que avaliam valores     */
Reais duracao_total(Carreira *carreira);
Reais custo_total(Carreira *carreira);

/*      Funcoes de imprimir      */
void carreira_nao_existe(char *nome);
void paragem_nao_existe(char *nome);
void mostrar_paragem_localizacao(Paragem *paragem);
void mostrar_carreira(Carreira *carreira);
void listaCarreiras(Global *global);
void mostrar_paragem(Paragem *paragem);
void listaParagens(Global *global);
void mostrar_paragens_carreira(Carreira *carreira, int inverso);

/*      Funcoes de processar      */
void processar_comandos(Global **global);
void processar_carreiras(Global **global);
void processar_paragens(Global **global);
void processar_ligacao(Global **global);
void processar_interligacao(Global *global);
void processar_remover_carreira(Global **global);
void processar_remover_paragem(Global **global);
void processar_apagar(Global **global);

/*      Funcoes de input      */
int leEspacos();
void leNome(char s[]);
void leAteFinalLinha();
void leLigacao(char nome[MAX_INPUT], char origem[MAX_INPUT], 
               char destino[MAX_INPUT], Reais *tempo, Reais *custo);
int verificaInversoOk(char s[]);
int verifica_custo_duracao(Reais custo, Reais tempo);
int verifica_ligacao(Global **global, char *nome, char *origem, 
char *destino, Reais *tempo, Reais *custo, Carreira **carreira,
Paragem **paragem_origem, Paragem **paragem_destino);
void leRemoverCarreira(Global **global, Carreira **carreira, char *nome);

/*      Funcoes do sistema      */
void iniciar_sistema(Global **global);
void apagar_lista_carreira(Lista_Carreiras **lista);
void apagar_paragem(Paragem **paragem);
void apagar_paragens(Paragem **paragem);
void apagar_ligacao(Ligacao **ligacao);
void apagar_carreira(Carreira **carreira);
void apagar_carreiras(Carreira **carreira);
void apagar_sistema(Global **global);
void *testar_sistema(Global **global, size_t tamanho);

#endif