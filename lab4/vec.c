#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
int main(void){
    int nthreads,tid;
    int sum,n;
    unsigned int seed = omp_get_thread_num() + time(NULL);
    printf("N :");
    scanf("%d",&n);

    int * vectora=(int*)malloc(n*sizeof(int));
    int * vectorb=(int*)malloc(n*sizeof(int));
    int * vectorc=(int*)malloc(n*sizeof(int));
    int i;
    #pragma omp parallel for schedule(guided,4)
    for(int i=0;i<n;i++)
    {
        vectora[i] = rand_r(&seed);
        vectorb[i] = rand_r(&seed)

    }
    #pragma omp parallel for schedule(static,4)  private(i)
    for(i=0;i<n;i++)
    {
        vectorc[i]=vectorb[i]+vectora[i];
    }

    for(i=0;i<n;i++)
    {
        printf("%d\t",vectorc[i]);
    }
    free(vectorc);
    free(vectora);
    free(vectorb);




}