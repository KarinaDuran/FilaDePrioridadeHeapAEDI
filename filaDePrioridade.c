#include "filaDePrioridade.h"

PFILA criarFila(int max)
{
    PFILA res = (PFILA) malloc(sizeof(FILADEPRIORIDADE));
    res->maxElementos = max;
    res->arranjo = (PONT*) malloc(sizeof(PONT)*max);
    res->heap = (PONT*) malloc(sizeof(PONT)*max);
    int i;
    for (i=0; i<max; i++)
    {
        res->arranjo[i] = NULL;
        res->heap[i] = NULL;
    }
    res->elementosNoHeap = 0;
    return res;
}

void exibirLog(PFILA f)
{
    printf("Log [elementos: %i]\n", f->elementosNoHeap);
    PONT atual;
    int i;
    for (i=0; i<f->elementosNoHeap; i++)
    {
        atual = f->heap[i];
        printf("[%i;%f;%i] ", atual->id, atual->prioridade, atual->posicao);
    }
    printf("\n\n");
}

//Funcao para retornar o tamanho da fila
int tamanho(PFILA f)
{
    return f->elementosNoHeap;
}

//Funcao auxiliar para reduzir a prioridade, recebe como parametro
// a fila e a posicao que se quer reduzir
void reduzirPrioridadeAux(PFILA f, int posicao)
{
    //Se o tamanho for menor ou igual que 1, acaba a funcao
    if(f->elementosNoHeap <=1)
    {
        return;
    }
    //Calcula a posicao dos "filhos" da posicao atual
    int i = posicao;
    //Esquerda
    int j = 2*i+1;
    //Direita
    int k = 2*i+2;
    int maior;
    //Enquanto o elemento na esquerda nao for nulo
    while (f->heap[j]!=NULL)
    {
        //Verifica qual elemento eh maior:o da esquerda ou da direita
        if( f->heap[k]==NULL || f->heap[j]->prioridade > f->heap[k]->prioridade ) maior = j;
        else maior = k;
        //Se o elemento atual tiver prioridade maior que a maior prioridade dos filhos, acaba a funcao
        if(f->heap[i]->prioridade> f->heap[maior]->prioridade)break;
        //Troca as posicoes dos elementos
        f->heap[maior]->posicao=i;
        PONT aux = f->heap[maior];
        f->heap[i]->posicao = maior;
        f->heap[maior]=f->heap[i];

        f->heap[i]= aux;
        //Proximos filhos
        j = 2*maior+1;
        k = 2*maior+2;
        i=maior;
    }
    }

//Funcao auxiliar para aumentar a prioridade
//Recebe como parametro o ponteiro para o elemento que se quer mudar
//a prioridade
void aumentarPrioridadeAux2(PFILA f, PONT atual)
{
    if (tamanho(f)<=1)
    {
        return;
    }
    else
    {
        //Pega o "pai" do elemento atual
        int i = atual->posicao;
        int j = (i-1)/2;
        //Enquanto a prioridade do pai for menor que a prioridade do atual
        while (f->heap[i]->prioridade > f->heap[j]->prioridade)
        {
            //Inverte as posicoes
            PONT aux = f->heap[j];
            f->heap[j] = f->heap[i];
            f->heap[i] = aux;
            aux->posicao = atual->posicao;
            atual->posicao = j;
            //Proximo "pai"
            i = j;
            j = (j-1)/2;
        }
    }
}

//Funcao que insere um elemento na fila, inserindo-o no arranjo e no heap maximo
//Retorna true se for possivel fazer a insercao, senao, retorna false.
//recebe como parametro a prioridade e o id.
bool inserirElemento(PFILA f, int id, float prioridade)
{
    bool res = false;
    if ( id<0 || id >= f->maxElementos|| f->arranjo[id]!=NULL)
    {
        return res;
    }
    else
    {
        PONT novo = (PONT) malloc (sizeof(ELEMENTO));
        //Atribuicoes das caracteristicas do elemento novo
        novo->id = id;
        novo->prioridade = prioridade;
        novo->posicao = f->elementosNoHeap;
        //insercao nos vetores heap e arranjo
        f->heap[f->elementosNoHeap] = novo;
        f->arranjo[id] = novo;
        //acrescenta em um o numero de elementos
        f->elementosNoHeap = f->elementosNoHeap+1;
        //Chama a funcao para acertar a posicao no heap maximo.
        aumentarPrioridadeAux2(f, novo);


        res = true;
    }
    return res;
}

//Funcao que aumenta a prioridade de um elemento e retorna false somente
//se o id nao existir na fila ou
bool aumentarPrioridade(PFILA f, int id, float novaPrioridade)
{
    bool res = false;
    if(f->arranjo[id] == NULL || f->arranjo[id]->prioridade >= novaPrioridade)
    {
        return res;

    }
    else
    {
        f->arranjo[id]->prioridade = novaPrioridade;
        aumentarPrioridadeAux2(f, f->arranjo[id]);
        res = true;
    }

    return res;
}

//Funcao para reduzir a prioridade de um elemento
//Retorna false caso a nova prioridade seja maior que a prioridade anterior
//ou se um elemento com o devido id nao existir
bool reduzirPrioridade(PFILA f, int id, float novaPrioridade)
{
    bool res = false;
    if(f->arranjo[id]==NULL || f->arranjo[id]->prioridade <= novaPrioridade)
    {
        return res;
    }
    else
    {
        f->arranjo[id]->prioridade = novaPrioridade;
        reduzirPrioridadeAux( f, f->arranjo[id]->posicao);
        res = true;
    }
    return res;
}

//Funcao para remover um elemento, retorna NULL caso a fila esteja vazia
//Se nao, retorna o elemento contido na primeira posicao do vetor f->heap
//e posiciona o ultimo elemento no lugar. Entao, chama a funcao reduzirPrioridadeAux
//Para reposicioná-lo no lugar certo
PONT removerElemento(PFILA f)
{
    PONT res = NULL;
    if (f->elementosNoHeap == 0)return res;
       //pega o valor em 0
       res = f->heap[0];
       f->arranjo[res->id]=NULL;

        int x = (f->elementosNoHeap)-1;

        f->elementosNoHeap = x;
        //Salva o ultimo no primeiro
        f->heap[0] = f->heap[x];
        f->heap[0]->posicao = 0;
        //Exclui o ultimo
        f->heap[x] = NULL;
        //Chama o metodo para reordenar
         reduzirPrioridadeAux(f, 0);

         return res;

    }


//Funcao para consultar a prioridade de um elemento
//Recebe o id e o caminho para a resposta (prioridade do elemento).
bool consultarPrioridade(PFILA f, int id, float* resposta)
{
    bool res = false;
    if (tamanho(f)==0 || id < 0 || id > f->maxElementos ||f->arranjo[id]==NULL)
    {
        return res;
    }
    else
    {
        *resposta = f->arranjo[id]->prioridade;
        res =true;
        return res;
    }

}


