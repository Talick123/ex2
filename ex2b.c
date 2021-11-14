

//------------ inclide section ----------------

#include <stdio.h>
#include <stdlib.h> //for exit()
#include <sys/types.h>
#include <unistd.h> // for fork()
#include <signal.h>
#include<sys/wait.h> //for wait()

//----------- prototype section --------------

void catch_sigusr(int sig_num);
void catch_sigterm(int sig_num);
void do_child();
void do_parent(pid_t child_id);
void process_action(pid_t process_to_kill, int n_seed);
void print_error_and_exit();

//----------- global variables ---------------

int s_usr1_counter = 0, s_usr2_counter = 0;

//------------- enum section -----------------

enum SIGNALS { S_SIGUSR1 = 10, S_SIGUSR2 = 12 };

//------------- main section -----------------
int main()
{
	pid_t status;

	// signal handlers declaration
	signal(SIGUSR1, catch_sigusr);
	signal(SIGUSR2, catch_sigusr);
	signal(SIGTERM, catch_sigterm);

	status = fork(); //create child process

	if(status < 0) //handle error in fork
		print_error_and_exit();
	if(status == 0) //child
		do_child();
	else //parent
		do_parent(status);

	return EXIT_SUCCESS;
}

//-------------------------------------
//signal handler
void catch_sigusr(int sig_num)
{
	signal(SIGUSR1, catch_sigusr);
	signal(SIGUSR2, catch_sigusr);

	printf("Process %d got signal %s\n", getpid(), sig_num == S_SIGUSR1 ? "SIGUSR1" : "SIGUSR2");

	if(sig_num == S_SIGUSR1)
		s_usr1_counter ++;

	else if(sig_num == S_SIGUSR2)
		s_usr2_counter ++;
}

//-------------------------------------
//signal handler
void catch_sigterm(int sig_num)
{
	signal(SIGTERM, catch_sigterm);

	printf("Process %d win\n", getpid());
	exit(EXIT_SUCCESS);
}

//-------------------------------------
void do_child()
{
	process_action(getppid(), 17); //send parent pid and num to srand
}

//-------------------------------------
void do_parent(pid_t child_id)
{
	process_action(child_id, 18); //send child pid and num to srand
}

//-------------------------------------
void process_action(pid_t process_to_kill, int n_seed)
{
	int option, s_usr1, s_usr2;
	s_usr1 = s_usr2 = 0;

	while(1)
	{
		srand(n_seed);

		sleep((rand() % 3)); //sleep for random amount of time

		option = rand() % 3; //generate random number between 0 and 2

		switch(option)
		{
		case 0:
			exit (EXIT_SUCCESS);
		case 1:
			kill(process_to_kill, SIGUSR1);
			s_usr1 ++ ;
		case 2:
			kill(process_to_kill, SIGUSR2);
			s_usr2 ++ ;
		}

		if(s_usr1 == 7 || s_usr2 == 7)
		{
			puts("you probably ended");
			exit(EXIT_SUCCESS);
		}

		if( s_usr1_counter == 5 || s_usr2_counter == 5)
		{
			printf("Process %d surrender\n", getpid());
			kill(process_to_kill, SIGTERM);
			wait(NULL);
			exit(EXIT_SUCCESS);
		}
	}
}

//-------------------------------------
void print_error_and_exit()
{
	fputs("Unable to fork.\n", stderr);
	exit(EXIT_FAILURE);
}
