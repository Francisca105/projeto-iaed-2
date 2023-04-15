/*
    Ficheiro: project2.c
    Autora: Francisca Vicente de Almeida (ist1105901)
    Descricao: Ficheiro que contem todas as novas funcoes necessarias
               para o segundo projeto, tudo o que engloba desde eleminar
               paragens, carreiras e ligacoes, ate a main.
*/

#include "project2.h"


/*      FUNCOES DE INPUT      */


/* Funcao que le o input do comando que apaga as carreiras */
void leRemoverCarreira(Global **global, Carreira **carreira, char nome[]) {
    leEspacos();
    leNome(nome);
    leEspacos();

    procurar_carreira(global, nome, carreira);
}

/* Funcao que le o input do comando que apaga as paragens */
void leRemoverParagem(Global **global, Paragem **paragem, char nome[]) {
    leEspacos();
    leNome(nome);
    leEspacos();

    procurar_paragem(global, nome, paragem);
}


/*      FUNCOES DE REMOVER      */


/* Funcao que remove uma carreira da lista de carreiras de uma paragem */
void remover_carreira_paragem(Paragem **paragem, char *nome) {
    Lista_Carreiras *procurar = (*paragem)->carreira;
    Lista_Carreiras *anterior = NULL;

    while(procurar != NULL) {
        if(strcmp(procurar->nome, nome) == 0) {
            if(anterior == NULL) {
                (*paragem)->carreira = procurar->proxima;
            } else {
                anterior->proxima = procurar->proxima;
            }

            apagar_lista_carreira(&procurar);

            ((*paragem)->numero_carreiras)--;
            return;
        }
        anterior = procurar;
        procurar = procurar->proxima;
    }
}

/* Funcao que ve todas as carreiras que tem uma paragem e se for caso, tira-a */
void remover_carreira_paragens(Global **global, Carreira *carreira) {
    Paragem *procurar = (*global)->paragem;
    while(procurar != NULL) {
        if(verificar_carreira_paragem(procurar, carreira->nome)) {
            remover_carreira_paragem(&procurar, carreira->nome);
        }

        procurar = procurar->proxima;
    }
}

/* 
    Funcao que remove uma ligacao das pontas (inicio/fim) do percurso de uma
    carreira 
*/
void remover_ligacao_extremos(Carreira **carreira, Ligacao *ligacao,
                              int origem) {
    if(origem) {
        if(ligacao->proxima != NULL)
            (ligacao->proxima)->anterior = NULL;
        (*carreira)->primeira_ligacao = ligacao->proxima;
        apagar_ligacao(&ligacao);
    } else {
        if(ligacao->anterior != NULL)
            (ligacao->anterior)->proxima = NULL;
        (*carreira)->ultima_ligacao = ligacao->anterior;
        apagar_ligacao(&ligacao);
    }
    ((*carreira)->numero_ligacoes--);
}

/* Funcao que remove uma ligacao do meio do percurso de uma carreira */
void remover_ligacao_meio(Carreira **carreira, Ligacao **ligacao,
                          Ligacao **proxima) {
    
    if((*proxima)->proxima != NULL)
        ((*proxima)->proxima)->anterior = *ligacao;
    else
        (*carreira)->ultima_ligacao = *ligacao;

    (*ligacao)->paragem_destino = realloc((*ligacao)->paragem_destino,
            sizeof(char)*(strlen((*proxima)->paragem_destino)+1));

    (*ligacao)->paragem_destino = (*proxima)->paragem_destino;

    (*ligacao)->tempo += (*proxima)->tempo;
    (*ligacao)->custo += (*proxima)->custo;

    (*ligacao)->proxima = (*proxima)->proxima;

    ((*carreira)->numero_ligacoes--);
}

/* Funcao que remove uma paragem de uma carreira */
void remover_paragem_carreira(Carreira **carreira, char *nome) {
    Ligacao *procurar = (*carreira)->primeira_ligacao;
    Ligacao *anterior, *proxima;

    while(procurar != NULL) {
        anterior = procurar->anterior;
        proxima = procurar->proxima;

        if((anterior == NULL && strcmp(procurar->paragem_origem, nome) == 0) ||
           (procurar->proxima == NULL && 
           strcmp(procurar->paragem_destino, nome) == 0)){
                remover_ligacao_extremos(carreira, procurar, anterior == NULL
                ? VERDADEIRO : FALSO);
        } else if(strcmp(procurar->paragem_destino, nome) == 0) {
            remover_ligacao_meio(carreira, &procurar, &(procurar->proxima));
        }
        
        procurar = proxima;
    }

    if(verificar_paragem_ligacao(*carreira, nome))
        remover_paragem_carreira(carreira, nome);
}

/* Funcao que ve todas as paragens que tem uma carreira e se for caso, tira-a */
void remover_paragem_carreiras(Global **global, Paragem *paragem) {
    Carreira *procurar = (*global)->carreira;

    while(procurar != NULL) {
        if(verificar_paragem_ligacao(procurar, paragem->nome)) {
            remover_paragem_carreira(&procurar, paragem->nome);
        }
        procurar = procurar->proxima;
    }
}


/*      PROCESSAR COMANDOS      */


/* Funcao que permite processar o comando 'r' */
void processar_remover_carreira(Global **global) {
    char nome[MAX_INPUT];
    Carreira *carreira, *aux = (*global)->carreira;

    leRemoverCarreira(global, &carreira, nome);

    if(carreira == NULL) {
        carreira_nao_existe(nome);
        return;
    }

    remover_carreira_paragens(global, carreira);

    if(aux == carreira) {
        (*global)->carreira = carreira->proxima;
    } else {
        while(aux->proxima && strcmp(aux->proxima->nome, nome) != 0)
            aux = aux->proxima;

        if(carreira == (*global)->ultima_carreira) {
            aux->proxima = NULL;
            (*global)->ultima_carreira = aux;
        }
        else
            aux->proxima = carreira->proxima;
    }

    apagar_carreira(&carreira);
}

/* Funcao que permite processar o comando 'e' */
void processar_remover_paragem(Global ** global) {
    char nome[MAX_INPUT];
    Paragem *paragem, *aux = (*global)->paragem;
    
    leRemoverParagem(global, &paragem, nome);

    procurar_paragem(global, nome, &paragem);
    
    if(paragem == NULL) {
        paragem_nao_existe(nome);
        return;
    }

    remover_paragem_carreiras(global, paragem);

    if(aux == paragem) {
        (*global)->paragem = paragem->proxima;
    } else {
        while(aux->proxima && strcmp(aux->proxima->nome, nome) != 0)
            aux = aux->proxima;

        if(paragem->proxima == NULL) {
            aux->proxima = NULL;
            (*global)->ultima_paragem = aux;
        }
        else {
            aux->proxima = paragem->proxima;
        }
    }

    apagar_paragem(&paragem);
}

/* Funcao que permite processar o comando 'a' */
void processar_apagar(Global **global) {
    apagar_sistema(global);
    iniciar_sistema(global);
}


/*      SISTEMA      */


/* Funcao que inicia o sistema */
void iniciar_sistema(Global **global) {
    *global = malloc(sizeof(Global));
    (*global)->paragem = (*global)->ultima_paragem = NULL;
    (*global)->carreira = (*global)->ultima_carreira = NULL;
}

/* Funcao que apaga uma unica carreira da lista carreiras */
void apagar_lista_carreira(Lista_Carreiras **lista) {
    free((*lista)->nome);
    free(*lista);
    *lista = NULL;
}

/* Funcao que apaga todas as carreiras guardadas na lista de carreiras */
void apagar_lista_carreiras(Lista_Carreiras **lista) {
    Lista_Carreiras *aux;
    while(*lista != NULL) {
        aux = (*lista)->proxima;
        apagar_lista_carreira(lista);
        *lista = aux;
    }
}

/* Funcao que apaga uma unica ligacao */
void apagar_ligacao(Ligacao **ligacao) {
    free((*ligacao)->paragem_origem);
    free((*ligacao)->paragem_destino);
    free(*ligacao);
    *ligacao = NULL;
}

/* Funcao que apaga todas as ligacoes guardadas */
void apagar_ligacoes(Ligacao **ligacao) {
    Ligacao *aux;
    while(*ligacao != NULL) {
        aux = (*ligacao)->proxima;
        apagar_ligacao(ligacao);
        *ligacao = aux;
    }
}

/* Funcao que apaga uma unica carreira */
void apagar_carreira(Carreira **carreira) {
    free((*carreira)->nome);
    free(*carreira);
    *carreira = NULL;
}

/* Funcao que apaga todas as carreiras guardadas na estrutura global */
void apagar_carreiras(Carreira **carreira) {
    Carreira *aux;
    while(*carreira != NULL) {
        aux = (*carreira)->proxima;
        apagar_ligacoes(&((*carreira)->primeira_ligacao));
        apagar_carreira(carreira);
        *carreira = aux;
    }
}

/* Funcao que apaga uma unica paragem */
void apagar_paragem(Paragem **paragem) {
    apagar_lista_carreiras(&((*paragem)->carreira));
    free((*paragem)->nome);
    free(*paragem);
    *paragem = NULL;
}

/* Funcao que apaga todas as paragens guardadas na estrutura global */
void apagar_paragens(Paragem **paragem) {
    Paragem *aux = *paragem;
    while(*paragem != NULL) {
        aux = (*paragem)->proxima;
        apagar_paragem(paragem);
        *paragem = aux;
    }
}

/* 
    Funcao que apaga todos os elementos da estrutura Global
    (definida na main) 
*/
void apagar_sistema(Global **global) {
    /* Liberta a memoria utilizada pelas paragens */
    apagar_paragens(&((*global)->paragem));
    /* Liberta a memoria utilizada pelas carreiras */
    apagar_carreiras(&((*global)->carreira));
    /* Liberta a memoria utilizada pela estrutura global */
    free(*global);
}

/* 
    Funcao que testa se e possivel adicionar mais elementos
    na memoria (e depois no sistema) 
*/
void* testar_sistema(Global **global, size_t size) {
    void *ptr = malloc(size);
    if(ptr != NULL)
        return ptr;

    printf("No memory.\n");
    apagar_sistema(global);
    exit(0);

    return NULL;
}

/*      MAIN      */

int main() {
    Global *global;
    iniciar_sistema(&global);

    processar_comandos(&global);
    return 0;
}