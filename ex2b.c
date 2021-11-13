#include <stdio.h>
#include <stdlib.h> //for exit()
#include <sys/types.h>
#include <unistd.j> // for fork()
#include <signal.h>



int main()
{
	pid_t status;

	signal(SIGUSR1, catch_sigusr1);
	signal(SIGUSR2, catch_sigusr2);
	signal(SIGTERM, catch_sigterm);

	status = fork();

	if(status < 0)
		print_error_and_exit();
	if(status == 0)
		do_child();
	else
		do_parent(status);

	return EXIT_SUCCESS;
}

void catch_sigusr1(int sig_num)
{
	signal(SIGUSR1, catch_sigusr1);
	puts("Process %d got singal SIGUSR1", getpid());
	//counts the amount of time it received this signal
	//if received 5 times, prints message, sends sigterm to other process and ends

}

void catch_sigusr2(int sig_num)
{
	signal(SIGUSR2, catch_sigusr2);
	puts("Process %d got singal SIGUSR2", getpid());
	//counts the amount of time it received this signal
	//if received 5 times, prints message, sends sigterm to other process and ends
	
}

void catch_sigterm(int sig_num)
{
	signal(SIGTERM, catch_sigterm);
	puts("Process %d win", getpid());
	exit(EXIT_SUCCESS);
}

void do_child()
{
	int option;
	srand(18)
	while(true)
	{
		//pause for random amount of time
		//generate random number between 0 and 2
		option = rand() % 3;
		
		switch(option)
		{
		case 0:
			exit (EXIT_SUCCESS);
		case 1:
			//send to sigusr1
			//increase count of sending signal to sigusr1
			//if counter at 7 - prints message and ends
			kill(getppid(), SIGUSR1);
		case 2:
			//send to sigusr2
			//increase count of sending signal to sigusr2
			//if counter at 7 - prints message and ends
			kill(getppid(), SIGUSR2);
		}
	}
}

void do_parent(pid_t child_id)
{
	int option;
	srand(17)
	while(true)
	{
		//same as child
	}

}


void print_error_and_exit()
{
	fputs("Unable to fork.\n", stderr);
	exit(EXIT_FAILURE);
}