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
#include <getopt.h>
#include <stdlib.h>
#include <time.h>

extern char *optarg;
extern int opterr;

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
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		return -1;
	}
	ts.tv_sec += secs;

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
	int c, res;
	struct option opts[] = {
		{"unlink", required_argument, NULL, 'u'},
		{"close", required_argument, NULL, 'c'},
		{"get", required_argument, NULL, 'g'},
		{"post", required_argument, NULL, 'p'},
		{"wait", required_argument, NULL, 'w'},
		{"trywait", required_argument, NULL, 't'},
		{"timedwait", required_argument, NULL, 'd'},
		{0, 0, 0, 0}
	};

	opterr = 0;

	while ((c=getopt_long(argc, argv, "gucpwtd:", opts, NULL)) != -1)
	{
		switch (c)
		{
		case 'u':
			csim_sem_unlink();
			break;

		case 'c':
			csim_sem_close();
			break;

		case 'w':
			csim_sem_wait();
			break;

		case 'p':
			csim_sem_post();
			break;

		case 'g':
			printf("sem value: %d\n", csim_sem_getvalue());
			break;

		case 't':
			res = csim_sem_trywait();
			printf("sem_trywait(): %d\n", res);
			break;

		case 'd':
			printf("%d\n", atoi(optarg));
			res = csim_sem_timedwait(atoi(optarg));
			printf("sem_timedwait(): %d\n", res);
			break;

		case '?':
			printf("Usage: \n"
			"-p post: post semaphore\n"
			"-g get: get value\n"
			"-u unlink: unlink semaphore\n"
			"-c close: close semaphore\n"
			"-w wait: wait semaphore\n"
			"-t trywait: trywait semaphore\n"
			"-d timedwait: timedwait semaphore\n"
			);
			break;

		default:
			break;
		}
	}

	return 0;
}
