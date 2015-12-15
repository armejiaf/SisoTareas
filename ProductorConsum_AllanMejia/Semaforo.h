#ifndef SEMAFORO_H_
#define SEMAFORO_H_

struct Semaforo {
	int S;
};

void set(int s,struct Semaforo *sem){
	sem->S = s;
}

void wait(struct Semaforo *sem){
	while (sem->S <= 0);
	sem->S--;
}

void signal(struct Semaforo *sem){
    sem->S++;
}

#endif
