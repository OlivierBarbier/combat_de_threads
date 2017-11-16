#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

pid_t fils0, fils1;
int points;
struct sigaction sa;

void randsleep()
{	
	sranddev();
	int r = rand();
	double ratio = (double)r / (double) RAND_MAX;
	struct timespec tosleep;
	tosleep.tv_sec = 0;
	tosleep.tv_nsec = 300000000 + ratio*700000000;
	struct timespec remain;
	while (nanosleep(&tosleep, &remain) != 0) {
		tosleep = remain;
	}
}

void hit (int sig)
{
	(void) sig;

	points--;

	printf ("Attaque reçu par %d; Pv restants %d\n", getpid(), points);

	if (points == 0)
	{
		printf ("Plus de PV pour %d; terminaison du processus\n", getpid());
		exit (1);
	}
}


void attaque (pid_t adversaire)
{
	int ret;

	sa.sa_handler = hit;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);

	if ((ret = kill(adversaire, SIGUSR1)) == -1)
	{
		exit (0);
	}

	randsleep();
}

void defense()
{
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);	
	
	randsleep();
}

void combat (pid_t adversaire)
{
	while (1)
	{
		defense();
		attaque (adversaire);
	}
}
int main (void)
{
	int stat_loc;
	int descr[2];
	pid_t wpid;

	points = 3;

	pipe(descr);

	if ((fils0 = fork()) > 0)
	{
		if ((fils1 = fork()) > 0)
		{
			printf(
				"(P) Processus %u, P=%u, F1=%u, F2=%u\n", 
				getpid(), 
				getppid(), 
				fils0, 
				fils1
			);

			wpid = waitpid(-1, &stat_loc, 0);
			printf("P Processus fils pid %u est terminé avec valeur %d\n", wpid, WEXITSTATUS(stat_loc));
			
			wpid = waitpid(-1, &stat_loc, 0);
			printf("P Processus fils pid %u est terminé avec valeur %d\n", wpid, WEXITSTATUS(stat_loc));
		}
		else
		{
			close(descr[0]);
			/* fils0 ok par héritage */ 
			fils1 = getpid();
			write(descr[1], &fils1, sizeof(fils1));
			printf(
				"(F1) Processus %u, P=%u, F1=%u, F2=%u\n", 
				getpid(), getppid(), fils0, fils1
			);
			combat (fils0);
		}
	}
	else
	{
		close(descr[1]);
		fils0 = getpid();
		read(descr[0], &fils1, sizeof(fils1));
		printf(
			"(F0) Processus %u, P=%u, F1=%u, F2=%u\n", 
			getpid(), getppid(), fils0, fils1
		);
		combat (fils1);
	}
	return 0;
}
