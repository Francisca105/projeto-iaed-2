/*
    Ficheiro: project1.c
    Autora: Francisca Vicente de Almeida (ist1105901)
    Descricao: Ficheiro que contem todas as antigas funcoes do primeiro 
               projeto, tudo o que engloba desde criar paragens,
               carreiras e ligacoes, ler inputs, etc.
*/

#include "project2.h"


/*      FUNCOES DE ADICIONAR      */


/* 
    Funcao que adiciona uma paragem a lista de paragens dentro da
    estrutura global.
*/
void adicionar_paragem(Global **global, char nome[MAX_INPUT], Reais latitude,
                       Reais longitude) {

    /* Criar uma nova paragem */
    Paragem *nova_paragem = 
    (Paragem *) testar_sistema(global, sizeof(Paragem));

    /* Coloca os dados dentro da paragem */
    nova_paragem->nome = 
    (char *) testar_sistema(global, sizeof(char) * (strlen(nome)+1));
    strcpy(nova_paragem->nome, nome);

    nova_paragem->latitude = latitude;
    nova_paragem->longitude = longitude;
    nova_paragem->proxima = NULL;
    nova_paragem->numero_carreiras = 0;
    nova_paragem->carreira = NULL;

    /* Verificar se a lista de paragens esta vazia */
    if((*global)->paragem == NULL) {
        (*global)->paragem = (*global)->ultima_paragem = nova_paragem;
    } else {
        /* 
            Caso a lista de paragens nao esteja vazia, adicionar a nova
            paragem ao fim da lista 
        */
        (*global)->ultima_paragem->proxima = nova_paragem;
        (*global)->ultima_paragem = nova_paragem;
    }
}

/* Funcao que adiciona uma carreira a lista de carreiras */
void adicionar_carreira(Global **global, char nome[MAX_INPUT]) {
    /* Cria uma carreira nova */
    Carreira *nova = 
    (Carreira *) testar_sistema(global, sizeof(Carreira));

    /* Coloca os dados dentro da paragem */
    nova->nome = 
    (char *) testar_sistema(global, sizeof(char) * (strlen(nome)+1));
    strcpy(nova->nome, nome);

    nova->primeira_ligacao = nova->ultima_ligacao = NULL;
    nova->numero_ligacoes = 0;
    nova->proxima = NULL;

    /* Verificar se a lista de carreiras esta vazia */
    if((*global)->carreira == NULL) {
        (*global)->carreira = (*global)->ultima_carreira = nova;
    } else {
        /* 
            Caso a lista de carreiras nao esteja vazia, adicionar a nova
            carreira no fim da lista 
        */
        (*global)->ultima_carreira->proxima = nova;
        (*global)->ultima_carreira = nova;
    }
}

/* Funcao que adiciona uma ligacao a uma carreira */
void adicionar_ligacao(Global **global, Carreira ** carreira, 
                       Paragem ** paragem1, Paragem ** paragem2,
                       Reais custo, Reais tempo) {
    /* Cria uma nova ligação */
    Ligacao *nova = (Ligacao *) testar_sistema(global, sizeof(Ligacao));
    
    /* Coloca os dados dentro da ligacao */
    nova->paragem_origem = 
    (char *) testar_sistema(global,sizeof(char)*(strlen((*paragem1)->nome)+1));
    strcpy(nova->paragem_origem, (*paragem1)->nome);

    nova->paragem_destino = 
    (char *) testar_sistema(global,sizeof(char)*(strlen((*paragem2)->nome)+1));
    strcpy(nova->paragem_destino, (*paragem2)->nome);

    nova->custo = custo;
    nova->tempo = tempo;
    nova->proxima = NULL;
    nova->anterior = NULL;

    /* Verificar se a carreira ja tem ligacoes */
    if((*carreira)->primeira_ligacao == NULL) {
        adicionar_origem_destino_carreira(global, carreira, nova, paragem1,
                                          paragem2);
    } else if(strcmp((*carreira)->ultima_ligacao->paragem_destino, 
                   (*paragem1)->nome) == 0) {
        /* 
            Se a primeira paragem do comando 'l' for a ultima da
            carreira, ou seja l atual_paragem_final nova_final,
            entao a paragem eh adicionada no final do percurso
        */
        adicionar_ligacao_carreira(global, carreira, &nova, paragem1, 
                                   paragem2, FALSO);
    } else{
        /* Caso contrario eh adicionada no comeco da carreira */
        adicionar_ligacao_carreira(global, carreira, &nova, paragem1, 
                                   paragem2, VERDADEIRO);
    }
}

/* Funcao que define a primeira ligacao da carreira */
void adicionar_origem_destino_carreira(Global **global, 
Carreira ** carreira, Ligacao * ligacao, Paragem ** paragem1,
                                       Paragem ** paragem2) {
    /* Acrescenta a carreira na primeira paragem */
    adicionar_carreira_paragem(global, paragem1, (*carreira)->nome);
    /* Se a paragem for diferente acrescenta a carreira na segunda paragem */
    if(strcmp((*paragem1)->nome, (*paragem2)->nome) != 0)
        adicionar_carreira_paragem(global, paragem2, (*carreira)->nome);

    (*carreira)->primeira_ligacao = (*carreira)->ultima_ligacao = ligacao;
    (*carreira)->numero_ligacoes++;

}

/* Acrescenta uma ligacao na carreira conforme seja a nova origem ou nao */
void adicionar_ligacao_carreira(Global **global, Carreira ** carreira, 
Ligacao ** ligacao, Paragem ** paragem1, Paragem ** paragem2, int origem) {
    /* No caso da paragem ser colocada no comeco do percurso */
    if(origem) {
        /* Se ainda nao tem a carreira na paragem, adiciona-a */
        if(!verificar_paragem_ligacao(*carreira, (*paragem1)->nome))
            adicionar_carreira_paragem(global, paragem1, (*carreira)->nome);

        (*carreira)->primeira_ligacao->anterior = *ligacao;
        (*ligacao)->proxima = (*carreira)->primeira_ligacao;
        (*carreira)->primeira_ligacao = *ligacao;
        (*ligacao)->anterior = NULL;

    /* Caso contrario */
    } else {
        /* Se a paragem nao tem a carreira ainda, adiciona-a*/
        if(!verificar_paragem_ligacao(*carreira, (*paragem2)->nome))
            adicionar_carreira_paragem(global, paragem2, (*carreira)->nome);

        (*carreira)->ultima_ligacao->proxima = *ligacao;
        (*ligacao)->anterior = (*carreira)->ultima_ligacao;
        (*carreira)->ultima_ligacao = *ligacao;	
        (*ligacao)->proxima = NULL;

    }
    
    (*carreira)->numero_ligacoes++;
}

/* Funcao que adiciona uma carreira na lista de paragens */
void adicionar_carreira_paragem(Global **global, Paragem **paragem, 
                                char *nome) {
    /* Cria uma nova carreira para colocar dentro da lista de carreiras */
    Lista_Carreiras *nova =
    (Lista_Carreiras *) testar_sistema(global, sizeof(Lista_Carreiras));

    /* Coloca o nome */
    nova->nome = 
    (char *) testar_sistema(global, sizeof(char) * (strlen(nome)+1));
    strcpy(nova->nome, nome);

    /* Aumenta o numero de carreiras da paragem */
    ((*paragem)->numero_carreiras)++;

    /* 
        Se ainda nao tem carreira ou se o nome vem primeiro do que
        o proximo (ordenado de forma alfabetica por ordem crescente)
    */
    if((*paragem)->carreira == NULL || 
        strcmp((*paragem)->carreira->nome, nome) > 0) {
        
        nova->anterior = NULL;
        nova->proxima = (*paragem)->carreira;
        (*paragem)->carreira = nova;
    
    } else {
        Lista_Carreiras *procurar = (*paragem)->carreira;
        /* Insere a carreira de forma ordenada */
        while(procurar->proxima != NULL && 
              strcmp(procurar->proxima->nome, nome) < 0) {
            procurar = procurar->proxima;
        }
        nova->proxima = procurar->proxima;
        procurar->proxima = nova;
    }
}


/*      FUNCOES DE BUSCA      */


/* Funcao que procura uma paragem na lista de paragens */
void procurar_paragem(Global **global, char *nome, Paragem **paragem) {
    Paragem *procurar = (*global)->paragem;
    while (procurar != NULL) {
        if (strcmp(procurar->nome, nome) == 0) {
            *paragem = procurar;
            return;
        }
        procurar = procurar->proxima;
    }
    *paragem = NULL;
}

/* Funcao que procura uma carreira na lista de carreiras */
void procurar_carreira(Global **global, char *nome, Carreira **carreira) {
    Carreira *procurar = (*global)->carreira;
    while (procurar != NULL) {
        if (strcmp(procurar->nome, nome) == 0) {
            *carreira = procurar;
            return;
        }
        procurar = procurar->proxima;
    }
    *carreira = NULL;
}


/*      FUNCOES DE VERIFICACAO      */


/* Funcao que qual das paragens eh uma das extremidades da carreira */
int verificar_extremidade(Carreira *carreira, char *paragem1, char *paragem2) {
    /* Verificar se a paragem2 eh a primeira paragem da carreira */
    if(strcmp(carreira->primeira_ligacao->paragem_origem, paragem2) == 0) {
        return PARAGEM_ORIGEM_ENCONTRADA;
    }

    /* Verificar se a paragem1 eh a ultima paragem da carreira */
    if(strcmp(carreira->ultima_ligacao->paragem_destino, paragem1) == 0) {
        return PARAGEM_DESTINO_ENCONTRADA;
    }

    /* Caso nenhuma das extremidades seja igual, retorna 0 */
    return FALSO;
}

/* Funcao que verifica se uma paragem ja estava numa ligacao da carreira */
int verificar_paragem_ligacao(Carreira *carreira, char *paragem) {
    Ligacao *ligacao = carreira->primeira_ligacao;

    /* Percorrer todas as ligacoes da carreira */
    while(ligacao != NULL) {
        /* Verificar se a paragem ja esta na ligacao */
        if(strcmp(ligacao->paragem_origem, paragem) == 0 ||
           strcmp(ligacao->paragem_destino, paragem) == 0) {
            return VERDADEIRO;
        }
        ligacao = ligacao->proxima;
    }

    /* Caso a paragem nao esteja na ligacao, retorna 0 */
    return FALSO;
}

/* 
    Funcao que verifica se uma carreira ja esta numa paragem, indo a procura
    nas carreiras da paragem
*/
int verificar_carreira_paragem(Paragem *paragem, char *carreira) {
    Lista_Carreiras *procurar = paragem->carreira;
    while(procurar != NULL) {
        if(strcmp(procurar->nome, carreira) == 0) {
            return VERDADEIRO;
        }
        procurar = procurar->proxima;
    }
    return FALSO;
}


/*      FUNCOES QUE AVALIAM VALORES     */


/* Funcao para ver a duracao total de uma carreira */
Reais duracao_total(Carreira *carreira) {
    Ligacao *ligacao = carreira->primeira_ligacao;
    Reais duracao = 0;

    while(ligacao != NULL) {
        duracao += ligacao->tempo;
        ligacao = ligacao->proxima;
    }
    return duracao;
}

/* Funcao para ver a custo total de uma carreira */
Reais custo_total(Carreira *carreira) {
    Ligacao *ligacao = carreira->primeira_ligacao;
    Reais custo = 0;

    while(ligacao != NULL) {
        custo += ligacao->custo;
        ligacao = ligacao->proxima;
    }
    return custo;
}


/*      FUNCOES DE IMPRIMIR      */


/* Funcao que imprime quando uma carreira nao existe*/
void carreira_nao_existe(char *nome) {
    printf("%s: no such line.\n", nome);
}

/* Funcao que imprime quando uma paragem nao existe*/
void paragem_nao_existe(char *nome) {
    printf("%s: no such stop.\n", nome);
}

/* 
    Funcao que permite mostrar a localizacao de uma paragem e o
    numero de carreiras 
*/
void mostrar_paragem_localizacao(Paragem *paragem) {
    printf("%16.12f %16.12f\n", paragem->latitude,
           paragem->longitude);
}

/* 
    Funcao que permite imprimir as informacoes da carreira, usada na 
    ListaCarreiras (modificada do primeiro projeto do professor).
*/
void mostrar_carreira(Carreira *carreira) {
    printf("%s ", carreira->nome);
    if(carreira->numero_ligacoes > 0) {
        printf("%s %s %d %.2f %.2f\n", 
                carreira->primeira_ligacao->paragem_origem,
                carreira->ultima_ligacao->paragem_destino,
                carreira->numero_ligacoes + 1,
                custo_total(carreira),
                duracao_total(carreira));
    }
    else
        printf("%d %.2f %.2f\n", 0, 0.0, 0.0);
}

/* 
    Funcao que permite listar todas as carreiras (modificada do primeiro
    projeto do professor).
*/
void listaCarreiras(Global *global) {
    Carreira *carreira = global->carreira;
    while(carreira != NULL) {
        mostrar_carreira(carreira);
        carreira = carreira->proxima;
    }
}

/* Funcao que permite mostrar uma paragem */
void mostrar_paragem(Paragem *paragem) {
    printf("%s: %16.12f %16.12f %d\n", paragem->nome, 
    paragem->latitude, paragem->longitude, 
    paragem->numero_carreiras);
}

/* Funcao que permite mostrar todas as paragens */
void listaParagens(Global *global) {
    Paragem *paragem = global->paragem;
    while(paragem != NULL) {
        mostrar_paragem(paragem);
        paragem = paragem->proxima;
    }
}

/* Funcao que permite mostrar as paragens de uma carreira */
void mostrar_paragens_carreira(Carreira *carreira, int inverso) {
    Ligacao *ligacao = carreira->primeira_ligacao;
    int primeira = VERDADEIRO;

    /* 
        Se for por ordem normal, vai buscar a primeira ligacao,
        caso contrario, vai buscar a ultima
    */
    if(inverso)
        ligacao = carreira->ultima_ligacao;

    /* Enquanto existir uma ligacao definida, vai imprimi-la */
    while(ligacao != NULL) {
        /* 
            Se for a primeira, imprime so a origem/destino conforme
            seja inverso/nao
        */
        if(primeira) {
            if(inverso)
                printf("%s", ligacao->paragem_destino);
            else
                printf("%s", ligacao->paragem_origem);
            primeira = FALSO;
        }

        /* Imprime o resto das ligacoes e atualiza o valor da proxima */
        if(inverso) {
            printf(", %s", ligacao->paragem_origem);
            ligacao = ligacao->anterior;
        }
        else {
            printf(", %s", ligacao->paragem_destino);
            ligacao = ligacao->proxima;
        }

    }

    if(carreira->numero_ligacoes > 0)
        printf("\n");
}


/*      FUNCOES DE INPUT      */


/* Funcao do primeiro projeto do professor */
int leEspacos() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') 
        return 0;
    ungetc(c, stdin);
    return 1;
}

/* Funcao do primeiro projeto do professor */
void leNome(char s[]) {
    int i = 0, c;
    s[0] = getchar();
    if (s[0] != '"') {
        i = 1;
        while ((c = getchar()) != ' ' && c != '\t' && c != '\n')
            s[i++] = c;
        ungetc(c, stdin);
    }
    else {
        while((c = getchar()) != '"')
            s[i++] = c;
    }
    s[i++] = '\0';
}

/* Funcao modificada primeiro projeto do professor */
void leAteFinalLinha() {
    char c;
    while ((c = getchar()) != '\n') {
        if (c == EOF)
            break;
    }
}

/* Funcao que le os inputs necessarios para o comando 'l' */
void leLigacao(char nome[MAX_INPUT], char origem[MAX_INPUT], 
               char destino[MAX_INPUT], Reais *tempo, Reais *custo) {
    leEspacos();
    leNome(nome);
    leEspacos();
    leNome(origem);
    leEspacos();
    leNome(destino);
    leEspacos();
    scanf("%lf", custo);
    leEspacos();
    scanf("%lf", tempo);
    leEspacos();
}

/* Funcao do primeiro projeto do professor */
int verificaInversoOk(char s[]) {
    char inv[] = "inverso";
    int size = strlen(s), i;

    if (size < 3 || size > 7)
        return FALSO;
    for (i = 0; i < size; i++)
        if (inv[i] != s[i])
            return FALSO;
    return VERDADEIRO;
}

/*Funcao que verifica se o custo e a duracao nao sao negativos*/
int verifica_custo_duracao(Reais custo, Reais tempo) {
    if(tempo < 0 || custo < 0) {
        printf("negative cost or duration.\n");
        return FALSO;
    }
    return VERDADEIRO;
}

/* Funcao que verifica os argumentos do comando 'l' */
int verifica_ligacao(Global **global, char *nome, char *origem, 
char *destino, Reais *tempo, Reais *custo, Carreira **carreira,
Paragem **paragem_origem, Paragem **paragem_destino) {
    /* Porucra os elementos */
    procurar_carreira(global, nome, carreira);
    procurar_paragem(global, origem, paragem_origem);
    procurar_paragem(global, destino, paragem_destino);
    
    if(*carreira == NULL) {
        carreira_nao_existe(nome);
        return FALSO;
    }

    if(*paragem_origem == NULL || *paragem_destino == NULL) {
        paragem_nao_existe(*paragem_origem == NULL ? origem : destino);
        return FALSO;
    }

    if((*carreira)->numero_ligacoes > 0 && 
    strcmp((*carreira)->primeira_ligacao->paragem_origem, destino) && 
    strcmp((*carreira)->ultima_ligacao->paragem_destino, origem)) {
        printf("link cannot be associated with bus line.\n");
        return FALSO;
    }

    if(!verifica_custo_duracao(*custo, *tempo))
        return FALSO;

    return VERDADEIRO;
}


/*      PROCESSAR COMANDOS      */


/* Funcao modificada do meu primeiro projeto */
void processar_comandos(Global **global) {
    int continuar = VERDADEIRO;

    while (continuar) {
        char command = getchar();
        switch (command) {
            case 'q':
                apagar_sistema(global);
                continuar = FALSO;
                break;
            case 'c':
                processar_carreiras(global);
                break;
            case 'p':
                processar_paragens(global);
                break;
            case 'l':
                processar_ligacao(global);
                break;
            case 'i':
                processar_interligacao(*global);
                break;
            /* Novos comandos */
            case 'r':
                processar_remover_carreira(global);
                break;
            case 'e':
                processar_remover_paragem(global);
                break;
            case 'a':
                processar_apagar(global);
                break;
        }
    }
}

/* Funcao que permite processar o comando 'p' */
void processar_paragens(Global **global) {
    char nome[MAX_INPUT], latitude[MAX_INPUT], longitude[MAX_INPUT];
    int leu_espaco = leEspacos();
    Paragem *paragem;
    /* Se nao tem argumentos, imprime as paragens */
    if(!leu_espaco) {
        listaParagens(*global);
        return;
    }

    leNome(nome);
    procurar_paragem(global, nome, &paragem);
    leu_espaco = leEspacos();

    /* 
        Se foi colocado o nome da paragem, verifica se existe,
        se existir sao impressos os dados da paragem, caso
        contrario, imprime uma mensagem de erro.
    */
    if(!leu_espaco) {
        if(paragem == NULL) {
            paragem_nao_existe(nome);
            return;
        }
        mostrar_paragem_localizacao(paragem);
        return;
    }

    leNome(latitude);
    leEspacos();
    leNome(longitude);
    leEspacos();
    /* Se a paragem nao existe, cria uma, caso contrario imprime o erro. */
    if(paragem == NULL)
        adicionar_paragem(global, nome, atof(latitude), atof(longitude));
    else
        printf("%s: stop already exists.\n", nome);
}

/* Funcao que permite processar o comando 'c' */
void processar_carreiras(Global **global) {
    char nome[MAX_INPUT], inverso[MAX_INPUT];
    int leu_espaco = leEspacos();
    Carreira *carreira;

    /* Se nao tem argumentos, imprime as carreiras */
    if(!leu_espaco) {
        listaCarreiras(*global);
        return;
    }

    leNome(nome);
    procurar_carreira(global, nome, &carreira);

    /* Se a carreira nao existe, adiciona-a */
    if(carreira == NULL) {
        adicionar_carreira(global, nome);
        return;
    }

    /*
        Se nao foram colocados mais argumentos, imprime as paragens
        de forma normal
    */
    leu_espaco = leEspacos();
    if(!leu_espaco) {
        mostrar_paragens_carreira(carreira, FALSO);
        return;
    }

    leNome(inverso);
    leEspacos();
    /* 
        Se foi colocado o argumento, corretamente, do inverso,
        imprime as paragens pela ordem inversa
    */
    if(verificaInversoOk(inverso)) {
        mostrar_paragens_carreira(carreira, VERDADEIRO);   
        return;
    }

    printf("incorrect sort option.\n");
}

/* Funcao que permite processar o comando 'l' */
void processar_ligacao(Global **global) {
    char nome[MAX_INPUT], origem[MAX_INPUT], destino[MAX_INPUT];
    Reais tempo, custo;
    Carreira *carreira;
    Paragem *paragem_origem, *paragem_destino;
    
    /* 
        Le os inputs necessarios para a ligacao, caso estes estejam colocados
        corretamente, a ligacao eh adicionada
    */
    leLigacao(nome, origem, destino, &tempo, &custo);
    if(!verifica_ligacao(global, nome, origem, destino, &tempo, 
    &custo, &carreira, &paragem_origem, &paragem_destino))
        return;

    adicionar_ligacao(global, &carreira, &paragem_origem, 
                      &paragem_destino, custo, tempo);
}

/* Funcao que permite processar o comando 'i' */
void processar_interligacao(Global *global) {
    Paragem *atual = global->paragem;
    Lista_Carreiras *aux;
    
    leEspacos();
    /* Imprime as intersecoes das paragens */
    while(atual != NULL) {
        if(atual->numero_carreiras > 1) {
            printf("%s %d:", atual->nome, atual->numero_carreiras);

            aux = atual->carreira;
            while(aux != NULL) {
                printf(" %s", aux->nome);
                aux = aux->proxima;
            }
            
            printf("\n");
        }
        atual = atual->proxima;
    }
}