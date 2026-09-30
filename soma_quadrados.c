/* ----------------------------------------------------------------------
 * TODO 1: Preparacao do ambiente
 *
 * - Instalar o MPI, se ainda nao estiver instalado:
 *     Ubuntu/Debian ou WSL: sudo apt install openmpi-bin libopenmpi-dev
 *     macOS:                brew install open-mpi
 * - Conferir a instalacao: mpicc --version  e  mpirun --version
 * - Manter este arquivo (soma_quadrados.c) na pasta 02_mpi/atividade_03.
 * - Compilar:  mpicc -o soma_quadrados soma_quadrados.c
 * - Executar:  mpirun -np 4 ./soma_quadrados
 *   Se o mpirun reclamar de falta de slots (poucos nucleos ou VM), usar:
 *   mpirun --oversubscribe -np 4 ./soma_quadrados
 * - Definir N = 40 com #define (abaixo), para facilitar mudancas.
 * ---------------------------------------------------------------------- */


    /* ------------------------------------------------------------------
     * TODO 2: Estrutura basica do programa MPI
     *
     * - MPI_Init inicia o ambiente MPI; deve ser a primeira chamada MPI.
     * - MPI_Comm_rank guarda em rank o identificador deste processo
     *   (0 a size-1). O rank 0 e o root.
     * - MPI_Comm_size guarda em size o total de processos.
     * - Validar N % size: se nao for divisivel, o root imprime o erro e
     *   TODOS os processos chamam MPI_Finalize antes de sair, para nao
     *   travar o programa.
     * - Calcular local_n = N / size (elementos por processo).
     * - MPI_Finalize no fim do programa; toda chamada MPI fica entre
     *   MPI_Init e MPI_Finalize.
     * ------------------------------------------------------------------ */

    /* ------------------------------------------------------------------
     * TODO 3: Preparar os dados
     *
     * - Declarar um ponteiro para o vetor global (int *global = NULL).
     *   Ele so sera alocado de verdade no root (rank 0); nos demais
     *   processos continua NULL, e o MPI_Scatter aceita isso.
     * - No root: alocar N inteiros (malloc) e preencher com 1, 2, ..., N
     *   (global[i] = i + 1).
     * - Em TODOS os processos (inclusive o root): alocar o vetor local
     *   com local_n inteiros. E nele que cada processo vai receber a
     *   sua fatia.
     * - Lembrar de verificar se o malloc retornou NULL.
     * ------------------------------------------------------------------ */

    /* ------------------------------------------------------------------
     * TODO 4: Distribuir o vetor com MPI_Scatter
     *
     * - Chamar MPI_Scatter em todos os processos (e uma chamada
     *   coletiva: se so o root chamar, o programa trava). Parametros:
     *     1. vetor de envio (global)        -> so o root usa
     *     2. quantidade enviada POR processo (local_n)
     *     3. tipo de envio (MPI_INT)
     *     4. vetor de recebimento (local)
     *     5. quantidade recebida (local_n)
     *     6. tipo de recebimento (MPI_INT)
     *     7. root (0)
     *     8. comunicador (MPI_COMM_WORLD)
     * - Depois do Scatter, cada processo imprime o seu vetor local:
     *   "Processo X recebeu: a b c ...".
     * - Dica: como varios processos imprimem ao mesmo tempo, a ordem das
     *   linhas na tela pode ficar embaralhada. Isso e normal. Se quiser
     *   a saida ordenada, pode usar MPI_Barrier com um laco sobre os
     *   ranks, mas nao e obrigatorio no enunciado.
     * ------------------------------------------------------------------ */

    /* ------------------------------------------------------------------
     * TODO 5: Soma local dos quadrados e MPI_Reduce
     *
     * - Cada processo percorre o seu vetor local e acumula o quadrado de
     *   cada elemento em uma variavel local (long long local_sum = 0).
     *   Converter para long long antes de multiplicar, para evitar
     *   overflow em casos maiores.
     * - Imprimir a soma local de cada processo:
     *   "Processo X: soma local dos quadrados = ...".
     * - Declarar long long global_sum = 0 (so tem significado no root).
     * - Chamar MPI_Reduce em todos os processos. Parametros:
     *     1. endereco do valor local (&local_sum)
     *     2. endereco onde o root recebe o total (&global_sum)
     *     3. quantidade de elementos (1)
     *     4. tipo (MPI_LONG_LONG)
     *     5. operacao (MPI_SUM)
     *     6. root (0)
     *     7. comunicador (MPI_COMM_WORLD)
     * - Atencao: o tipo passado ao MPI precisa corresponder ao tipo da
     *   variavel em C. Se a variavel e long long, use MPI_LONG_LONG.
     * ------------------------------------------------------------------ */

    /* ------------------------------------------------------------------
     * TODO 6: Validacao (somente no root, rank == 0)
     *
     * - Calcular a soma sequencial com um laco de 1 ate N acumulando i*i.
     * - Calcular tambem pela formula N*(N+1)*(2*N+1)/6 (opcional, mas o
     *   enunciado sugere como forma de validar). Para N = 40 o valor
     *   esperado e 22140.
     * - Imprimir o resultado paralelo (global_sum), o sequencial e,
     *   se quiser, o da formula.
     * - Comparar global_sum com o sequencial e imprimir se os valores
     *   conferem ou nao.
     * - Liberar a memoria: free(local) em todos os processos e
     *   free(global) apenas no root (free(NULL) tambem e seguro).
     * - O free deve ser feito ANTES do MPI_Finalize.
     * ------------------------------------------------------------------ */