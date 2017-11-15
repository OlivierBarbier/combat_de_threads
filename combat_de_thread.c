#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  c = PTHREAD_COND_INITIALIZER;

struct global {
    int flag;
    int gameover;
} g;

struct data {
    int indice;
    int point;
    int tirage;
} d[2];

int tirage() { 
    int r;
    r = rand()%2;
    return r;
}

void * handler (void * arg)
{
    struct data *dat = (struct data *) arg;

    do {
        pthread_mutex_lock(&m);
        while (g.flag == dat->indice)
        {
            pthread_cond_wait (&c, &m);
        }
        pthread_mutex_unlock(&m);

        dat->tirage++;

        while (tirage()==tirage() && g.gameover == 0)
        {
            dat->point++;
            /*printf("%u: %d %d %d\n", (unsigned) pthread_self(), dat->indice, dat->tirage, dat->point);*/
            if (dat->point == 10) {
                g.gameover = 1;
                break;
            }
        }

        pthread_mutex_lock(&m);
        g.flag = dat->indice;
        pthread_cond_signal (&c);
        pthread_mutex_unlock(&m);

        if (g.gameover) {
            pthread_exit(NULL);
        }
    }
    while (1);
}


int main (void)
{
    pthread_t t1, t2;

    sranddev();

    d[0].indice = 0;
    d[0].point = 0;
    d[0].tirage = 0;
    pthread_create(&t1, NULL, handler, &d[0]);

    d[1].indice = 1;
    d[1].point = 0;
    d[1].tirage = 0;
    pthread_create(&t2, NULL, handler, &d[1]);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Thread #%d Nb tirages : %d Points : %d\n", d[0].indice, d[0].tirage, d[0].point);
    printf("Thread #%d Nb tirages : %d Points : %d\n", d[1].indice, d[1].tirage, d[1].point);
}
