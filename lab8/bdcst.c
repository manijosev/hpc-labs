#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char **argv) {

    int rank,ierror,size,tag=0,val;
    ierror = MPI_Init(&argc,&argv);
    ierror = MPI_Comm_size(MPI_COMM_WORLD,&size);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if(rank == 0){
        printf("val : ");
        scanf("%d",&val);
    }
    MPI_Bcast(&val,1,MPI_INT,0,MPI_COMM_WORLD);

    printf("Process %d, val %d \n",rank,val);


    ierror=MPI_Finalize();
    return ierror;
}