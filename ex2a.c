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
#include<sys/wait.h> //for wait()
#include <unistd.h> // for fork()
#include <stdbool.h>
#include <signal.h>

//--------------- const section --------------
const int NUM_OF_LOOPS = 100000;

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
void catch_alarm(int sig_num)
{
	signal(SIGALRM, catch_alarm); //dont need this in linux im pretty sure but ok
 	time_elapsed = (time_elapsed == 1) ? 2 :  0;
	// if(time_elapsed == 1) //does this change the global value for the child too??
	// 	time_elapsed = 2;
	// else
	// 	time_elapsed = 0;
}

//--------------------------------------------
void catch_sigusr1(int sig_num)
{
	signal(SIGUSR1, catch_sigusr1);
	printf("%d\n", time_elapsed);
	wait(NULL); //does parent need to wait for child to end? will child be ok? babysitter? the answer is yes
	exit(EXIT_SUCCESS);
}

//--------------------------------------------
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
void do_child()
{
	int num, //random num
	 		num_of_primes = 0; //primes counter
	srand(18);

	while(1)
	{
		num = rand();
		if(prime_num(num))
			num_of_primes++;

		if (num_of_primes == NUM_OF_LOOPS)
		{
			kill(getppid(), SIGUSR1);
			exit(EXIT_SUCCESS);
		}
	}
}

//--------------------------------------------
void do_parent(pid_t status)
{
	int i;
	for(i = 0; i < 2; i++)
	{
		alarm(1);
		pause();
		alarm(0); //???
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

//============================================================================
/*
int main()
{
	pid_t status;
	int num_of_primes = 0;
	int i;
	signal(SIGALRM, catch_alarm);
	srand(17);
	screensaver = 1;

	status = fork();

	if(status < 0)
	{
		fputs("error in fork", stderr);
		exit(EXIT_FAILURE);
	}

	if (status == 0)
	{

		while(num_of_primes < NUM_OF_LOOPS)
		{
			num = rand();
			if(prime_num(num))
				num_of_primes++;
		}

		exit(EXIT_SUCCESS);
	}

	if(status > 0)
	{
		for(i = 0; i < 2; i++)
		{
			alarm(1);
			pause();
		}
		kill(status, SIGKILL);
		puts("%d", screensaver);
	}

	return EXIT_SUCCESS;
}

void catch_alarm(int sig_num)
{
	(screensaver == 1) ? screensaver = 2 : screensaver = 0;
}


//============================================================================

int main()
{

	pid_t status;
	int num_of_primes = 0;
	signal(SIGALRM, catch_alarm);
	srand(17);

	status = fork();

	if(status < 0)
	{
		fputs("error in fork", stderr);
		exit(EXIT_FAILURE);
	}

	if (status == 0)
	{

		while(num_of_primes < NUM_OF_LOOPS)
		{
			num = rand();
			if(prime_num(num))
				num_of_primes++;
		}
		exit(EXIT_SUCCESS);
	}

	if(status > 0)
	{
		alarm(2);
		pause();
		kill(status, SIGKILL);
	}

	return EXIT_SUCCESS;
}

void catch_alarm(int sig_num)
{
	puts("%d", 0);
}
*/
