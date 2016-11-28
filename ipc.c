// ipc.c : 프로세스간 통신
#include "header.h"

void IpcLoop() {
	for (;;) {
	}
}

void *Thread2() {
	IpcLoop();
	pthread_exit(NULL);
}



