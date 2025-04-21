#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
int main(void){
    int nthreads,tid;
    int sum,n,i;
    printf("N :");
    scanf("%d",&n);
    unsigned int seed = omp_get_thread_num() + time(NULL);

    int * vectora=(int*)malloc(n*sizeof(int));
    int * vectorb=(int*)malloc(n*sizeof(int));
    int * vectorc=(int*)malloc(n*sizeof(int));
    #pragma omp parallel for shared(vectora,vectorb),private(i)
    for(i=0;i<n;i++)
    {
        vectora[i] = rand_r(&seed);
        vectorb[i] = rand_r(&seed); // normal rand() is thread unsafe
    }
    #pragma omp parallel for shared(vectora,vectorb),private(i)
    for(i=0;i<n;i++)
    {
        vectorc[i]=vectorb[i]+vectora[i];
    }

    for(i=0;i<n;i++)
    {
        printf("%d\t",vectorc);
    }
    free(vectorc);
    free(vectora);
    free(vectorb);




}