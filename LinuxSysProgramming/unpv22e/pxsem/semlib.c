/*
 * semlib.c
 *
 *  Created on: Mar 21, 2017
 *      Author: bob
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <string.h>

static const char * sem_name = "csim_semaphore";

sem_t * csim_sem_open() {
	return sem_open(sem_name, O_CREAT | O_RDWR,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, 1);
}

int csim_sem_getvalue() {
	int sval = 0;
	int res = 0;

	sem_t * sem_id = csim_sem_open();
	res = sem_getvalue(sem_id, &sval);
	if (res < 0) {
		printf("Get semaphore value failed\n");
		return -1;
	} else {
		printf("Get semaphore value succeeded\n");
		return sval;
	}
}

int csim_sem_wait() {
	int res = 0;

	sem_t * sem_id = csim_sem_open();
	res = sem_wait(sem_id);

	return res;
}

int csim_sem_trywait() {
	int res = 0;

	sem_t * sem_id = csim_sem_open();
	res = sem_trywait(sem_id);

	return res;
}

int csim_sem_timedwait(int secs) {
	struct timespec ts;
	int res = 0;

	memset(&ts, 0, sizeof(struct timespec));
	ts.tv_nsec = secs;

	sem_t * sem_id = csim_sem_open();

	res = sem_timedwait(sem_id, &ts);

	return res;
}

int csim_sem_post() {
	int res = 0;
	sem_t * sem_id = csim_sem_open();

	res = sem_post(sem_id);

	return res;
}

int csim_sem_close() {
	int res = 0;
	sem_t * sem_id = csim_sem_open();
	res = sem_close(sem_id);

	return res;
}

int csim_sem_unlink() {
	int res = 0;

	res = sem_unlink(sem_name);

	return res;
}

int
main(int argc, char **argv)
{
	//printf("sem value: %d\n", csim_sem_getvalue());
	csim_sem_wait();
}
