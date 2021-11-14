#include <stdio.h>
#include <stdlib.h> //for exit()
#include <sys/types.h>
#include <unistd.j> // for fork()
#include <signal.h>

const int NUM_OF_LOOPS = 100000;
short time_elapsed;

int main()
{
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
	pid_t status;

	signal(SIGALRM, catch_alarm);
	signal(SIGUSR1, catch_sigusr1);

	time_elapsed = 1;
	status = fork();

	if(status < 0)
		print_error_and_exit();
	if (status == 0)
		do_child();
	else 
		do_parent();

	return EXIT_SUCCESS;
}

void catch_alarm(int sig_num)
{
	signal(SIGALRM, catch_alarm); //dont need this in linux im pretty sure but ok
	if(time_elapsed == 1) //does this change the global value for the child too??
		time_elapsed = 2;
	else
		time_elapsed = 0;
}

void catch_sigusr1(int sig_num)
{
	signal(SIGUSR1, catch_sigusr1);
	puts("%d\n", time_elapsed);
	wait(); //does parent need to wait for child to end? will child be ok? babysitter? the answer is yes
	exit(EXIT_SUCCESS);
}

bool prime_num(int num)
{
	int i;
	for(i = 2, i*i < num; i++)
		if(num % i == 0)
			return false;

	return true;
}

void do_child()
{
	int num, num_of_primes = 0;
	srand(17);

	while(num_of_primes < NUM_OF_LOOPS)
	{
		num = rand();
		if(prime_num(num))
			num_of_primes++;
	}
	kill(getppid(), SIGUSR1);
	exit(EXIT_SUCCESS);
}

void do_parent()
{
	for(i = 0; i < 2; i++)
	{
		alarm(1);
		pause();
		//alarm(0)???
	}
	kill(status, SIGKILL);
	puts("%d", time_elapsed);
}

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