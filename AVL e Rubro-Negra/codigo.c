#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct no{
	int chave, bal;
	struct no *esq, *dir, *pai;
	char cor;
} No;

// Algoritmos AVL
int ABB_altura(No *pt);
int eh_AVL(No *pt);
void rotacao_esq(No **pt, int* h, int* rotacoes);
void rotacao_dir(No **pt, int* h, int* rotacoes);
void rotacao_esq_remover(No** pt, int* h, int* rotacoes);
void rotacao_dir_remover(No** pt, int* h, int* rotacoes);
void balancear(No** pt, char r, int *h, int* rotacoes);
void iniciar_No(No** pt, int x);
void AVL_print(No* pt);
void AVL_inserir(int x, No** pt, int *h, int* rotacoes, int* nos);
void AVL_remover(int x, No** pt, int *h, int* rotacoes, int* nos);
int AVL_buscar(int x, No* pt);
int qtd_nos_AVL(No* pt);
void trocar(No** pt, No** ptu);

// Algoritmos RN
No* criar_no(int valor, No* externo);
No* criar_externo();
void inserir_RN(No* z, No** ptraiz, No* externo, int* rotacoes, int* nos);
void rota_RN(No* z, No** ptraiz, No* externo, int* rotacoes);
void mover_pai(No* u, No* v, No** ptraiz, No* externo);
void remover_RN(No* z, No** ptraiz, No* externo, int* rotacoes, int* nos);
void rotacao_remover_RN(No* x, No** ptraiz, No* externo, int* rotacoes);
No* sucessor(No* z, No* externo);
No* RN_buscar(No* pt, int chave, No* externo);
void rotacao_esq_RN(No* z, No** ptraiz, No* externo, int* rotacoes);
void rotacao_dir_RN(No* z, No** ptraiz, No* externo, int* rotacoes);
void RN_print(No* pt, No* externo);
int eh_RN(No* ptraiz, No* externo);
int verificar_RN(No* pt, No* externo);
int qtd_nos_RN(No* pt, No* externo);
int existe(No* ptraiz, int chave, No* externo);

// Funcoes de teste
void testar_RN(No* ptraiz, int* valores);
void testar_AVL(No* ptraiz, int* valores);
void tempo_busca_AVL(No* ptraiz, int* valores);
void tempo_busca_RN(No* ptraiz, int* valores, No* externo);

int main(){
    srand(time(NULL));
    int* valores = (int*)malloc(20000 * sizeof(int));

    No** arvoresAVL = (No**)malloc(1000 * sizeof(No*));
    No** arvoresRN = (No**)malloc(1000 * sizeof(No*));
    for(int i = 0; i < 1000; i++){
        printf("arvores numero %d:\n\n", i + 1);
        for(int i = 0; i < 20000; i++){
            valores[i] = (rand() % 100001) + 1;
        }
        testar_AVL(arvoresAVL[i], valores);
        printf("\n");
        testar_RN(arvoresRN[i], valores);
        printf("\n\n");
    }
}

// Funções de teste e tempo de busca

void testar_AVL(No* ptraiz, int* valores){
    ptraiz = NULL;
    int nos = 0, h = 0, rotacoes = 0, i = 0;
    while(nos < 10000){
        AVL_inserir(valores[i], &ptraiz, &h, &rotacoes, &nos);
        i++;
    }

    int n = qtd_nos_AVL(ptraiz);
    printf("%d nos na arvore AVL\n", n);
    if(eh_AVL(ptraiz))printf("eh AVL, rotacoes = %d\n", rotacoes);
    else printf("nao eh AVL\n");

    while(nos > 9000){
        i = rand() % 20000;
        AVL_remover(valores[i], &ptraiz, &h, &rotacoes, &nos);
        i++;
    }

    printf("%d nos na arvore, %d nos removidos\n", nos, n - nos);
    if(eh_AVL(ptraiz))printf("ainda eh AVL, rotacoes = %d", rotacoes);
    else printf("nao eh mais AVL");

    tempo_busca_AVL(ptraiz, valores);
}

void testar_RN(No* ptraiz, int* valores){
    No* externo = criar_externo();
    ptraiz = externo;
    int nos = 0, rotacoes = 0, i = 0;
    
    while(nos < 10000 && i < 20000){
        No* z = criar_no(valores[i], externo);
        inserir_RN(z, &ptraiz, externo, &rotacoes, &nos);
        i++;
    }

    int n = qtd_nos_RN(ptraiz, externo);
    printf("%d nos na arvore RN\n", n);
    if(eh_RN(ptraiz, externo))printf("eh RN, rotacoes = %d\n", rotacoes);
    else printf("nao eh RN\n");

    while (nos > 9000) {
        i = rand() % 20000;
        if(existe(ptraiz, valores[i], externo)){
            No* z = RN_buscar(ptraiz, valores[i], externo);
            remover_RN(z, &ptraiz, externo, &rotacoes, &nos);
        }
        i++;
    }

    printf("%d nos na arvore, %d nos removidos\n", nos, n - nos);
    if(eh_RN(ptraiz, externo))printf("ainda eh RN, rotacoes = %d", rotacoes);
    else printf("nao eh mais RN");

    tempo_busca_RN(ptraiz, valores, externo);
}

void tempo_busca_AVL(No* ptraiz, int* valores){
    clock_t inicio, fim;
    double tempo;
    int aux;
    
    inicio = clock();
    for(int i = 0; i < 10000; i++){
        aux = AVL_buscar(valores[i], ptraiz);
    }
    fim = clock();

    if(aux > 0) printf(" "); // uso mínimo de aux pra o compilador n travar
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("\ntempo de busca AVL: %.5f ms\n", tempo * 1000);
}

void tempo_busca_RN(No* ptraiz, int* valores, No* externo){
    clock_t inicio, fim;
    double tempo;
    No* aux;
    
    inicio = clock();
    for(int i = 0; i < 10000; i++){
        aux = RN_buscar(ptraiz, valores[i], externo);
    }
    fim = clock();

    if(aux != NULL) printf(" "); // uso mínimo de aux pra o compilador n travar
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("\ntempo de busca RN: %.5f ms", tempo * 1000);
}

// ----------------------RUBRO-NEGRA----------------------

int existe(No* ptraiz, int chave, No* externo){
    if(RN_buscar(ptraiz, chave, externo) != externo) return 1;
    else return 0;
}

No* criar_externo(){
    No* externo = (No*)malloc(sizeof(No));
    
    externo->chave = -1;
    externo->cor = 'N';
    externo->dir = externo->esq = externo->pai = externo;
    
    return externo;
}

No* criar_no(int valor, No* externo){
    No* novo = (No*)malloc(sizeof(No));
    
    novo->chave = valor;
    novo->esq = novo->dir = novo->pai = externo;
    novo->cor = 'R';
    
    return novo;
}

void RN_print(No* pt, No* externo) {
    if (pt != externo) {
        RN_print(pt->dir, externo);
        
        printf("%d(%c) ", pt->chave, pt->cor);
        
        RN_print(pt->esq, externo);
    }
}

void inserir_RN(No* z, No** ptraiz, No* externo, int* rotacoes, int* nos){
    No* y = externo;
    No* pt = *ptraiz;
    
    while(pt != externo){
        y = pt;
        if(z->chave == pt->chave){
            y = NULL; pt = externo;
        }else if(z->chave < pt->chave) pt = pt->esq;
        else pt = pt->dir;
    }
    
    if(y != NULL){
        z->pai = y;
        if(y == externo) *ptraiz = z;
        else if(z->chave < y->chave) y->esq = z;
        else y->dir = z;
        z->esq = z->dir = externo;
        z->cor = 'R';
        (*nos)++;
        rota_RN(z, ptraiz, externo, rotacoes);
    }
}

void rota_RN(No* z, No** ptraiz, No* externo, int* rotacoes){
    while(z->pai->cor == 'R'){
        if(z->pai == z->pai->pai->esq){
            No* y = z->pai->pai->dir;
            if(y->cor == 'R'){
                z->pai->cor = y->cor = 'N';
                z->pai->pai->cor = 'R';
                z = z->pai->pai;
            }else{
                if(z == z->pai->dir){
                    z = z->pai;
                    rotacao_esq_RN(z, ptraiz, externo, rotacoes);
                }
                z->pai->cor = 'N';
                z->pai->pai->cor = 'R';
                rotacao_dir_RN(z->pai->pai, ptraiz, externo, rotacoes);
            }
        }else{
            No* y = z->pai->pai->esq;
            if(y->cor == 'R'){
                z->pai->cor = y->cor = 'N';
                z->pai->pai->cor = 'R';
                z = z->pai->pai;
            }else{
                if(z == z->pai->esq){
                    z = z->pai;
                    rotacao_dir_RN(z, ptraiz, externo, rotacoes);
                }
                z->pai->cor = 'N';
                z->pai->pai->cor = 'R';
                rotacao_esq_RN(z->pai->pai, ptraiz, externo, rotacoes);
            }
        }
    }
    (*ptraiz)->cor = 'N';
}

void rotacao_esq_RN(No* z, No** ptraiz, No* externo, int* rotacoes){
    (*rotacoes)++;
    No* v = z->dir;
    z->dir = v->esq;
    
    if(v->esq != externo) v->esq->pai = z;
    v->pai = z->pai;
    if(z->pai == externo) *ptraiz = v;
    else if(z == z->pai->esq) z->pai->esq = v;
    else z->pai->dir = v;
    
    v->esq = z;
    z->pai = v;
}

void rotacao_dir_RN(No* z, No** ptraiz, No* externo, int* rotacoes){
    (*rotacoes)++;
    No* v = z->esq;
    z->esq = v->dir;
    
    if(v->dir != externo) v->dir->pai = z;
    v->pai = z->pai;
    if(z->pai == externo) *ptraiz = v;
    else if(z == z->pai->dir) z->pai->dir = v;
    else z->pai->esq = v;
    
    v->dir = z;
    z->pai = v;
}

void mover_pai(No* u, No* v, No** ptraiz, No* externo){
    if(u->pai == externo){
        *ptraiz = v;
    }else if(u == u->pai->esq){
        u->pai->esq = v;
    }else{
        u->pai->dir = v;
    }
    v->pai = u->pai;
}

No* sucessor(No* z, No* externo){
    while(z->esq != externo){
        z = z->esq;
    }
    No* w = z;
    return w;
}

No* RN_buscar(No* pt, int chave, No* externo){
    if(pt == externo || pt->chave == chave) return pt;
    else if(pt->chave < chave) return RN_buscar(pt->dir, chave, externo);
    else return RN_buscar(pt->esq, chave, externo);
}

void remover_RN(No* z, No** ptraiz, No* externo, int* rotacoes, int* nos){
    No* y = z; char cor_og = y->cor; No* x;
    if(y->esq == externo){
        x = z->dir;
        mover_pai(z, z->dir, ptraiz, externo);
    }else if(y->dir == externo){
        x = z->esq;
        mover_pai(z, z->esq, ptraiz, externo);
    }else{
        y = sucessor(z->dir, externo), cor_og = y->cor;
        x = y->dir;
        if(y->pai != z){
            mover_pai(y, x, ptraiz, externo);
            y->dir = z->dir;
            y->dir->pai = y;
        }else x->pai = y;
        mover_pai(z, y, ptraiz, externo);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }
    
    if(cor_og == 'N') rotacao_remover_RN(x, ptraiz, externo, rotacoes);
    free(z);
    (*nos)--;
}

void rotacao_remover_RN(No* x, No** ptraiz, No* externo, int* rotacoes){
    while(x != *ptraiz && x->cor != 'R'){
        if(x == x->pai->esq){
            No* w = x->pai->dir;
            if(w->cor == 'R'){
                x->pai->cor = 'R'; w->cor = 'N';
                rotacao_esq_RN(x->pai, ptraiz, externo, rotacoes);
                w = x->pai->dir;
            }
            if(w->dir->cor == 'N' && w->esq->cor == 'N'){
                w->cor = 'R';
                x = x->pai;
            }else{
                if(w->esq->cor == 'R'){
                    w->esq->cor = 'N'; w->cor = 'R';
                    rotacao_dir_RN(w, ptraiz, externo, rotacoes);
                    w = x->pai->dir;
                }
                w->cor = x->pai->cor;
                x->pai->cor = w->dir->cor = 'N';
                rotacao_esq_RN(x->pai, ptraiz, externo, rotacoes);
                x = *ptraiz;
            }
        }
        else{
            No* w = x->pai->esq;
            if(w->cor == 'R'){
                x->pai->cor = 'R'; w->cor = 'N';
                rotacao_dir_RN(x->pai, ptraiz, externo, rotacoes);
                w = x->pai->esq;
            }
            if(w->esq->cor == 'N' && w->dir->cor == 'N'){
                w->cor = 'R';
                x = x->pai;
            }else{
                if(w->dir->cor == 'R'){
                    w->dir->cor = 'N'; w->cor = 'R';
                    rotacao_esq_RN(w, ptraiz, externo, rotacoes);
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = w->esq->cor = 'N';
                rotacao_dir_RN(x->pai, ptraiz, externo, rotacoes);
                x = *ptraiz;
            }
        }
    }
    x->cor = 'N';
}

int eh_RN(No* pt, No* externo){
    return verificar_RN(pt, externo);
}

int verificar_RN(No* pt, No* externo){
    if(pt == externo) return 1;
    
    int alt_esq = verificar_RN(pt->esq, externo);
    int alt_dir = verificar_RN(pt->dir, externo);
    
    if(alt_esq == 0 || alt_dir == 0 || alt_esq != alt_dir) return 0;
    
    if(pt->cor == 'N') return alt_esq + 1;
    else return alt_esq;
}

int qtd_nos_RN(No* pt, No* externo){
    if(pt == externo) return 0;
    else{
        int ptesq = qtd_nos_RN(pt->esq, externo);
        int ptdir = qtd_nos_RN(pt->dir, externo);
        return 1 + ptesq + ptdir;
    }
}

// ----------------------AVL----------------------

void AVL_print(No* pt){
    if(pt->dir != NULL) AVL_print(pt->dir);
    printf("%d ", pt->chave);
    if(pt->esq != NULL) AVL_print(pt->esq);
}

int ABB_altura(No *pt){
    if(pt == NULL) return -1;
    
    int abb_esq = ABB_altura(pt->esq);
    int abb_dir = ABB_altura(pt->dir);
    
    if(abb_esq > abb_dir) return abb_esq + 1;
    else return abb_dir + 1;
}

int eh_AVL(No *pt){
    if(pt == NULL) return 1;
    
    int alt_e = ABB_altura(pt->esq);
    int alt_d = ABB_altura(pt->dir);
    int bal = alt_d - alt_e;

    if(bal > 1 || bal < -1) return 0;

    if(eh_AVL(pt->esq) && eh_AVL(pt->dir)) return 1;
    return 0;
}

int AVL_buscar(int x, No* pt){
    if(pt == NULL) return 0;
    else if(pt->chave == x) return 1;
    else if(x > pt->chave) return AVL_buscar(x, pt->dir);
    else return AVL_buscar(x, pt->esq);
}

void rotacao_dir(No **pt, int *h, int* rotacoes){
    No *ptu = (*pt)->esq;

    if(ptu->bal == -1){
        (*pt)->esq = ptu->dir;
        ptu->dir = *pt;
        *pt = ptu;
        (*pt)->dir->bal = 0;
        (*rotacoes)++;
    }else{
        No *ptv = ptu->dir;
        ptu->dir = ptv->esq;
        ptv->esq = ptu;
        (*pt)->esq = ptv->dir;
        ptv->dir = *pt;

        if(ptv->bal == 1){
            (*pt)->bal = 0;
            ptu->bal = -1;
        }
        else if(ptv->bal == 0){
            (*pt)->bal = 0;
            ptu->bal = 0;
        }
        else if(ptv->bal == -1){
            (*pt)->bal = 1;
            ptu->bal = 0;
        }
        *pt = ptv;
        *rotacoes += 2;
    }
    (*pt)->bal = 0;
    *h = 0;
}

void rotacao_esq(No **pt, int *h, int* rotacoes){
    No *ptu = (*pt)->dir;
    if(ptu->bal == 1){
        (*pt)->dir = ptu->esq;
        ptu->esq = *pt;
        *pt = ptu;
        (*pt)->esq->bal = 0;
        (*rotacoes)++;
    }else{
        No *ptv = ptu->esq;
        ptu->esq = ptv->dir;
        ptv->dir = ptu;
        (*pt)->dir = ptv->esq;
        ptv->esq = *pt;
        
        if(ptv->bal == 1){
            (*pt)->bal = -1;
            ptu->bal = 0;
        }
        else if(ptv->bal == 0){
            (*pt)->bal = 0;
            ptu->bal = 0;
        }
        else if(ptv->bal == -1){
            (*pt)->bal = 0;
            ptu->bal = 1;
        }

        *pt = ptv;
        *rotacoes += 2;
    }
    (*pt)->bal = 0;
    *h = 0;
}

void iniciar_No(No** pt, int x){
    No* novo = (No*)malloc(sizeof(No));
    
    novo->chave = x;
    novo->esq = novo->dir = NULL;
    novo->bal = 0;
    
    *pt = novo;
}

void AVL_inserir(int x, No** pt, int* h, int* rotacoes, int* nos){
    if(*pt == NULL){
        iniciar_No(pt, x);
        *h = 1;
        (*nos)++;
    }else{
        if(x == (*pt)->chave) *h = 0;
        else if(x < (*pt)->chave){
            AVL_inserir(x, &(*pt)->esq, h, rotacoes, nos);
            if(*h == 1){
                if((*pt)->bal == 1){
                    (*pt)->bal = 0;
                    *h = 0;
                }
                else if((*pt)->bal == 0){
                    (*pt)->bal = -1;
                }
                else if((*pt)->bal == -1){
                    rotacao_dir(pt, h, rotacoes);
                    *h = 0;
                }
            }
        }else{
            AVL_inserir(x, &(*pt)->dir, h, rotacoes, nos);
            if(*h == 1){
                if((*pt)->bal == -1){
                    (*pt)->bal = 0;
                    *h = 0;
                }
                else if((*pt)->bal == 0){
                    (*pt)->bal = 1;
                }
                else if((*pt)->bal == 1){
                    rotacao_esq(pt, h, rotacoes);
                    *h = 0;
                }
            }
        }
    }
}

void rotacao_dir_remover(No** pt, int* h, int* rotacoes){
    No* ptu = (*pt)->esq;
    if(ptu->bal <= 0){
        (*pt)->esq = ptu->dir;
        ptu->dir = *pt;
        *pt = ptu;

        if(ptu->bal == -1){
            ptu->bal = 0;
            (*pt)->dir->bal = 0;
            *h = 1;
        }
        else{
            ptu->bal = 1;
            (*pt)->dir->bal = -1;
            *h = 0;
        }
        (*rotacoes)++;
    }else{
        No *ptv = ptu->dir;

        ptu->dir = ptv->esq;
        ptv->esq = ptu;
        (*pt)->esq = ptv->dir;
        ptv->dir = *pt;
        *pt = ptv;

        if(ptv->bal == 1){
            ptu->bal = -1;
            (*pt)->dir->bal = 0;
        }
        else if(ptv->bal == 0){
            ptu->bal = 0;
            (*pt)->dir->bal = 0;
        }
        else if(ptv->bal == -1){
            ptu->bal = 0;
            (*pt)->dir->bal = 1;
        }

        (*pt)->bal = 0;
        *h = 1;
        *rotacoes += 2;
    }
}

void rotacao_esq_remover(No** pt, int* h, int* rotacoes){
    No *ptu = (*pt)->dir;

    if (ptu->bal >= 0) {
        (*pt)->dir = ptu->esq;
        ptu->esq = *pt;
        *pt = ptu;

        if (ptu->bal == 1) {
            ptu->bal = 0;
            (*pt)->esq->bal = 0;
            *h = 1;
        } else {
            ptu->bal = -1;
            (*pt)->esq->bal = 1;
            *h = 0;
        }
        (*rotacoes)++;
    } else {
        No *ptv = ptu->esq;
        ptu->esq = ptv->dir;
        ptv->dir = ptu;
        (*pt)->dir = ptv->esq;
        ptv->esq = *pt;
        *pt = ptv;

        if(ptv->bal == 1){
            ptu->bal = 0;
            (*pt)->esq->bal = -1;
        }
        else if(ptv->bal == 0){
            ptu->bal = 0;
            (*pt)->esq->bal = 0;
        }
        else if(ptv->bal == -1){
            ptu->bal = 1;
            (*pt)->esq->bal = 0;
        }
        (*pt)->bal = 0;
        *h = 1;
        *rotacoes += 2;
    }
}

void balancear(No** pt, char r, int *h, int* rotacoes){
    if(*h == 1){
        if(r == 'D'){
            switch((*pt)->bal){
                case 1: (*pt)->bal = 0; break;
                case 0: (*pt)->bal = -1; *h = 0; break;
                case -1: rotacao_dir_remover(pt, h, rotacoes); break;
            }
        }else{
            switch((*pt)->bal){
                case -1: (*pt)->bal = 0; break;
                case 0: (*pt)->bal = 1; *h = 0; break;
                case 1: rotacao_esq_remover(pt, h, rotacoes); break;
            }
        }
    }
}

void trocar(No **pt, No **ptu){
    No *aux = *ptu;
    *ptu = *pt;
    *pt = aux;

    No *aux_esq = (*ptu)->esq;
    No *aux_dir = (*ptu)->dir;
    
    int aux_bal = (*ptu)->bal;

    (*ptu)->esq = (*pt)->esq;
    (*ptu)->dir = (*pt)->dir;
    (*ptu)->bal = (*pt)->bal;

    (*pt)->esq = aux_esq;
    (*pt)->dir = aux_dir;
    (*pt)->bal = aux_bal;
}

void AVL_remover(int x, No** pt, int *h, int* rotacoes, int* nos){
    if(*pt == NULL){
        *h = 0;
    }else{
        if(x < (*pt)->chave){
            AVL_remover(x, &(*pt)->esq, h, rotacoes, nos);
            balancear(pt, 'E', h, rotacoes);
        }else if(x > (*pt)->chave){
            AVL_remover(x, &(*pt)->dir, h, rotacoes, nos);
            balancear(pt, 'D', h, rotacoes);
        }else{
            No *aux = *pt;
            
            if((*pt)->esq == NULL){
                *pt = (*pt)->dir;
                *h = 1;
                
                free(aux);
                (*nos)--;
            }else{
                if((*pt)->dir == NULL){
                    *pt = (*pt)->esq;
                    *h = 1;

                    free(aux);
                    (*nos)--;
                }else{
                    No *s = (*pt)->dir;

                    if(s->esq == NULL){
                        s->esq = (*pt)->esq;
                        s->bal = (*pt)->bal;
                        *pt = s;
                        *h = 1;

                        free(aux);
                        (*nos)--;
                    }else{
                        No* paiS = *pt;

                        while(s->esq != NULL){
                            paiS = s;
                            s = s->esq;
                        }

                        trocar(pt, &paiS->esq);
                        AVL_remover(s->chave, &(*pt)->dir, h, rotacoes, nos);
                    }
                    balancear(pt, 'D', h, rotacoes);
                }
            }
        }
    }
}

int qtd_nos_AVL(No* pt){
    if(pt == NULL) return 0;
    else{
        int ptesq = qtd_nos_AVL(pt->esq);
        int ptdir = qtd_nos_AVL(pt->dir);
        return ptesq + ptdir + 1;
    }
}