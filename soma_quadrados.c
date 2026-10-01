#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 40 // tamanho do vetor, pedido no enunciado

int main(int argc, char *argv[]) {
    int rank, tamanho; // meu id e quantos processos existem
    int *vetor = NULL; // vetor completo, só o root usa de verdade
    int *sub_vetor = NULL; // pedaço que cada processo recebe
    int elementos_por_processo; // quantos numeros cada um vai pegar
    int soma_local = 0; // soma dos quadrados que EU calculei
    int soma_global = 0; // soma de todo mundo junto (só vale no root)

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // descobre meu rank
    MPI_Comm_size(MPI_COMM_WORLD, &tamanho); // descobre quantos processos tem

    if (N % tamanho != 0) {
        if (rank == 0) {
            printf("Erro: N (%d) deve ser divisivel pelo numero de processos (%d).\n", N, tamanho);
        }
        MPI_Finalize();
        return 1; // sem divisao exata o Scatter nao fecha certo
    }

    elementos_por_processo = N / tamanho; // ex: 40/4 = 10 cada

    if (rank == 0) {
        vetor = (int *) malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            vetor[i] = i + 1; // começa em 1, não em 0
        }
    }

    sub_vetor = (int *) malloc(elementos_por_processo * sizeof(int)); // todo mundo precisa alocar isso, não só o root

    MPI_Scatter(vetor, elementos_por_processo, MPI_INT,
                sub_vetor, elementos_por_processo, MPI_INT,
                0, MPI_COMM_WORLD); // reparte o vetor entre os processos

    printf("Processo %d recebeu: ", rank);
    for (int i = 0; i < elementos_por_processo; i++) {
        printf("%d ", sub_vetor[i]);
    }
    printf("\n");

    for (int i = 0; i < elementos_por_processo; i++) {
        soma_local += sub_vetor[i] * sub_vetor[i]; // soma dos quadrados do meu pedaço
    }

    printf("Processo %d: soma local dos quadrados = %d\n", rank, soma_local);

    MPI_Reduce(&soma_local, &soma_global, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); // junta as somas de todos no root

    if (rank == 0) {
        int soma_sequencial = N * (N + 1) * (2 * N + 1) / 6; // formula fechada pra conferir

        printf("\n");
        printf("Processo 0: soma paralela dos quadrados = %d\n", soma_global);
        printf("Processo 0: soma sequencial esperada    = %d\n", soma_sequencial);

        if (soma_global == soma_sequencial) {
            printf("\nOs valores conferem!\n");
        } else {
            printf("\nOs valores NAO conferem!\n");
        }
    }

    free(sub_vetor);
    if (rank == 0) {
        free(vetor); // só quem alocou libera
    }

    MPI_Finalize();
    return 0;
}