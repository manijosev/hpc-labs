#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char **argv) {

    int rank,ierror,size,tag=0,val;
    ierror = MPI_Init(&argc,&argv);
    ierror = MPI_Comm_size(MPI_COMM_WORLD,&size);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    char local_char,*all_chars;

    val = rand() % 26 + 97;
    local_char = (char)val;
    if(rank==0){
        all_chars = (char *)malloc(size * sizeof(char));
    }
    ierror=MPI_Gather(&local_char,1,MPI_CHAR,all_chars,1,MPI_CHAR,0,MPI_COMM_WORLD);
    if(rank==0){
            printf("Gathered characters: ");
            for (int i = 0; i < size; i++) {
                printf("%c ", all_chars[i]);
            }
            printf("\n");

            // Free the allocated memory for the characters
            free(all_chars);
    }



    ierror=MPI_Finalize();
    return ierror;
}