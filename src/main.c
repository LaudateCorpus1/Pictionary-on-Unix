// main.c : ���Լ��� ������ ����
#include "header.h"

#define N_THREADS 2
static pthread_t thid[N_THREADS];

// externs
char strAnswerCorrect[100] = "nn45nn3c8n";
bool isDrawer = false;
bool bGameOver = false;

extern int main(int argc, char* argv[]) {

	// �ɼ� �������� �ʾ��� �� ���� ����ϰ� ����
	if (argc == 1) {
		printf(
			"not enough argument. \n"
			"usage: \n"
			"[%s -w] for writer \n"
			"[%s -r] for reader \n", 
			argv[0], argv[0]
		);
		return 1;
	}

	// writer�� reader ���� ����
	if (strcmp(argv[1],"-w") == 0) {
		isDrawer = true;
		printf("starting as a writer. \n");
	} else if (strcmp(argv[1], "-r") == 0) {
		isDrawer = false;
		printf("starting as a reader. \n");
	} else {
		printf("invalid argument \n");
		return 1;
	}

	// ���� ����
	printf("creating threads... \n");
	if (isDrawer) {
		IpcInitClear(); // ipc �ʱ�ȭ
		InitDisplay(); // x window �ʱ�ȭ
		TypoSetInputAnswer(); // ����ڿ��� ���þ�(������ �� �ܾ�) �Է� ����
		pthread_create(&thid[0], NULL, Thread1, NULL);  // ������0: x window �Է� ����
		pthread_create(&thid[1], NULL, Thread2Writer, NULL);  // ������1: ipc �޽��� ����
		printf("hello there \n");
		TypoInputLoopWriter();
	} else /* if (!isDrawer) */ {
		IpcInit();
		InitDisplay();
		pthread_create(&thid[0], NULL, Thread1, NULL);  // x window
		pthread_create(&thid[1], NULL, Thread2Reader, NULL);  // ipc
		printf("hello there \n");
		TypoInputLoopReader();
	}

	/*
	for (i = 0; i < N_THREADS; ++i) {
		if (pthread_join(thid[i], NULL) != 0) {
			printf("end of process (-1) \n");
			return -1;
		}
	}
	*/

	// ����
	printf("end of process (0) \n");
	return 0;
}



