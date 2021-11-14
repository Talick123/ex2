/*
	- parent creates child process
	- child runs in infinite loop
		- in each loop: child magril number
		- when he randomizes 100 thousand prime numbers, he stops
		- numbers are not saved or presented
	- if the child process finishes in less than 1 second, the parent prints 1
	- if the child process finishes in more than 1 but less than 2, parent prints 2
	- if the child process finishes in more than 2, the parent prints 0
*/


//------------ inclide section ----------------

#include <stdio.h>
#include <stdlib.h> //for exit()
#include <sys/types.h>
#include <sys/wait.h> //for wait()
#include <unistd.h> // for fork()
#include <stdbool.h>
#include <signal.h>

//--------------- const section --------------

const int NUM_OF_PRIMES = 100000;

//----------- prototype section --------------

void catch_alarm(int sig_num);
void catch_sigusr1(int sig_num);
bool prime_num(int num);
void do_child();
void do_parent(pid_t status);
void print_error_and_exit();

//----------- global variables ---------------

short time_elapsed;

//-------------- main section ----------------
int main()
{
	pid_t status;

	// signal handlers declaration
	signal(SIGALRM, catch_alarm);
	signal(SIGUSR1, catch_sigusr1);

	time_elapsed = 1; //reset to 1
	status = fork(); //create proccess

	if(status < 0) // handle error in fork
		print_error_and_exit();
	if (status == 0) //child
		do_child();
	else //parent
		do_parent(status);

	return EXIT_SUCCESS;
}

//--------------------------------------------
//signal handler
void catch_alarm(int sig_num)
{
	signal(SIGALRM, catch_alarm);
 	time_elapsed = (time_elapsed == 1) ? 2 :  0;
}

//--------------------------------------------
//signal handler
void catch_sigusr1(int sig_num)
{
	signal(SIGUSR1, catch_sigusr1);
	printf("%d\n", time_elapsed);
	wait(NULL);
	exit(EXIT_SUCCESS);
}

//--------------------------------------------
//get initger - return if prime or not
bool prime_num(int num)
{
	int i;
	for(i = 2; i*i < num; i++)
	{
		if(num % i == 0)
			return false;
	}
	return true;
}

//--------------------------------------------
//child process: find 'NUM_OF_PRIMES' primes
void do_child()
{
	int num, primes_counter = 0;
	srand(17);

	while(1)
	{
		num = rand(); //randomize num
		if(prime_num(num))
			primes_counter++;

		if (primes_counter == NUM_OF_PRIMES)
		{
			kill(getppid(), SIGUSR1);
			exit(EXIT_SUCCESS);
		}
	}
}

//--------------------------------------------
// parent process: get child pid and check if child finished find NUM_OF_PRIMES
// prints 0 if less then 1 second, 2 if less then 2 second if more so 0
void do_parent(pid_t status)
{
	int i;
	for(i = 0; i < 2; i++)
	{
		alarm(1);
		pause(); // wait to sigalrm
		alarm(0);
	}
	kill(status, SIGKILL);
	printf("%d\n", time_elapsed);
}

//--------------------------------------------
void print_error_and_exit()
{
	fputs("Unable to fork.\n", stderr);
	exit(EXIT_FAILURE);
}
