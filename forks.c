#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

// Variável que armazena o tempo de início do processo
time_t starttime;
time_t nascF1;
time_t nascF2;
time_t nascN1;
time_t nascN2;

// Essa função printa a data.
void timestamp(){
	time_t ltime;
	ltime = time(NULL);
	printf("%s\n", asctime( localtime(&ltime)));
}

// Essa função diz o tempo de execução do processo. 
void execTime(){
	//printf("Data de nascimento: %d \n", ntime.tm_sec);

	time_t ltime;
	ltime = time(NULL);
	//printf("Data de morte:\n");
	timestamp();
	double x = difftime(ltime, starttime);
	printf("Anos desde o início: %.0f\n\n", x);
}

// Retorna o tempo de execução.
double getExecTime(){
	time_t ltime;
	ltime = time(NULL);
	return difftime(ltime, starttime);
}

int main(){
	starttime = time(NULL); // Armazenando o tempo de início
	printf("Pai Nasceu! Data de Nascimento:\n");
	timestamp(); // Printa o tempo de início

	pid_t pid[4] = {-1,-1,-1,-1}; // vetor que vai guardar os pids.
	int status; // Status para utilizar os Wait(NULL)

	sleep(14); // Esperar 14 segundos pra o filho nascer
	printf("PAI %d: Criando o primeiro Filho\n", getpid());
	printf("Filho 1 Nasceu! Data de Nascimento:\n");
	nascF1 = time(NULL);
	timestamp();
	pid[0] = fork();

	if(pid[0] < 0){
		printf("Erro ao criar processo #0\n");
		return 1;
	}

	// Essas funções de execuções pode tirar depois, eu só deixei pra poder saber o PID dos processos.
	if(pid[0] == 0){
		printf("Esta é a execução do filho 1(PID=%d), cujo pai tem PID=%d\n\n", getpid(), getppid());
	}

	// Pai cria filho 2
	if(pid[0] > 0){
		sleep(2);
		printf("PAI %d: Criando o segundo filho\n", getpid());
		printf("Filho 2 Nasceu! Data de Nascimento:\n");
		nascF2 = time(NULL);
		timestamp();
		pid[1] = fork();
		
		if(pid[1] < 0){
			printf("Erro ao criar processo #1\n");
			return 1;
		}
	}

	if(pid[1] == 0){
		printf("Esta é a execução do filho 2(PID=%d), cujo pai tem PID=%d\n\n", getpid(), getppid() );
	}

	// Filho 1 vai ter o Neto 1
	if(pid[0] == 0){
		sleep(12);
		printf("FILHO1 %d: Criando o primeiro neto\n", getpid());
		printf("Neto 1 Nasceu! Data de Nascimento:\n");
		nascN1 = time(NULL);
		timestamp();
		pid[2] = fork();

		if(pid[2] < 0){
			printf("Erro ao criar processo #2\n");
			return 1;
		}
	}

	if(pid[2] == 0){
		printf("Esta é a execução do Neto 1 (PID=%d), cujo pai tem PID=%d\n\n", getpid(), getppid() );
	}


	// Filho 2 vai ter o neto 2
	if(pid[1] == 0){
		sleep(14);
		printf("FILHO2 %d: Criando o segundo neto\n", getpid());
		printf("Neto 2 Nasceu! Data de Nascimento:\n");
		nascN2 = time(NULL);
		timestamp();
		pid[3] = fork();

		if(pid[3] < 0){
			printf("Erro ao criar processo #3\n");
			return 1;
		}
	}

	if(pid[3] == 0){
		printf("Esta é a execução do Neto 2 (PID=%d), cujo pai tem PID=%d\n\n", getpid(), getppid() );
	}

	// Após ter os filhos, os processos esperam pela morte.
	if(pid[3] == 0){
		sleep(18);
		printf("Processo Neto 2(%d) morre \n", getpid());
		execTime();
		exit(0);
	}

	if(pid[1] == 0){
		sleep(16);
		exit(0);
	}

	// Aqui o Neto 1 espera 12 segundos e morre.
	if(pid[2] == 0){
		sleep(12);
		exit(0);
	}
	
	// Depois do Neto 1 morrer, Filho 1 para de esperar, anuncia a morte do Neto 1 e espera sua morte.
	if(pid[0] == 0){
		status = wait(NULL);
		printf("Processo Neto 1(%d) morre \n", status);
		execTime();
		sleep(6);
		exit(0);
	}else{
		// Aqui o PAI espera os filhos morrerem
		status = wait(NULL);
		printf("Processo Filho 1(%d) morre \n", status);
		execTime();
	}
	// Anuncia a morte final.

	status = wait(NULL);
	printf("Processo Filho 2(%d) morre \n", status);
	execTime();
	

	sleep(14);
	printf("Processo PAI(%d) morre \n", getpid());
	execTime();

	return 0;

}