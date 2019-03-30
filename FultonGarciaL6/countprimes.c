//Author: Cesar Reyes PID:4035960
//I certify this is my own work and noone elses.
//Program description: This program uses MPI to find how many prime numbers are from 0 to N , and prints the numbers , the ID of the 
//process that found the prime , and the total number of primes found by all the processes. 
//This program uses the circuit.c structure to handle the processes.

#include "mpi.h"
#include <stdio.h>
#include <unistd.h>



#define N 10000 //Upper boundary. (0 - N)

//Function to check primes, parameters id for process ID and n for number to be checked.
int check_prime (int id, int n) {
  	
	if(n == 0 || n == 1){ //0 and 1 are not primes
		return 0;
	}
	int i;
	for(i = 2; i <= n/2;i++){ //Go from 2 through N/2, if there is a number divisible by i, N is not a prime
		if(n%i == 0){
		return 0;
		}
	}

    printf ("%d) %d\n", id, n); //Print the process id along with n, which is a prime number at this point.
    fflush (stdout); 
    return 1;
}

int main (int argc, char *argv[]) 
{
  int count;            /* Solutions found by this proc */
  double elapsed_time;  /* Time to find, count solutions */
  int global_count;     /* Total number of solutions */
  int i;
  int id;               /* Process rank */
  int p;                /* Number of processes */
  char hostname[1024];

  MPI_Init (&argc, &argv);

  MPI_Comm_rank (MPI_COMM_WORLD, &id);
  MPI_Comm_size (MPI_COMM_WORLD, &p);

  hostname[1023] = '\0';
  gethostname(hostname, 1023);
  printf("MPI rank %d on host %s\n", id, hostname);

  /* Start timer */
  MPI_Barrier (MPI_COMM_WORLD);
  elapsed_time = - MPI_Wtime();

  count = 0;
  for (i = id; i < N; i += p)
    count += check_prime (id, i);

  MPI_Reduce (&count, &global_count, 1, MPI_INT, MPI_SUM, 0,
	      MPI_COMM_WORLD); 

  /* Stop timer */
  elapsed_time += MPI_Wtime();

  if (!id) {
    printf ("Execution time %8.6f\n", elapsed_time);
    fflush (stdout);
  }
  MPI_Finalize();
  if (!id) printf ("There are %d different solutions\n",
		   global_count);
  return 0;
}

