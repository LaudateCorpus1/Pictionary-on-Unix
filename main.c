#include "header.h"

#define N_THREADS 2
static pthread_t thid[N_THREADS];

extern int main() {  // (int argc, char* argv[])
	int i;

	printf("hello there \n");
	pthread_create(&thid[0], NULL, Thread1, NULL);  // user_input
	pthread_create(&thid[1], NULL, Thread2, NULL);  // ipc

	for (i = 0; i < N_THREADS; ++i) {
		if (pthread_join(thid[i], NULL) != 0) {
			printf("end of process (-1) \n");
			return -1;
		}
	}

	printf("end of process (0) \n");
	return 0;
}



