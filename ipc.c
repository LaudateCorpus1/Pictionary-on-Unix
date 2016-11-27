#include "header.h"


void IpcLoop() {
	for (;;) {
	}
}

void *Thread2() {
	IpcLoop();
	pthread_exit(NULL);
}



