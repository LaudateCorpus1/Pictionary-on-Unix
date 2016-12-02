// main.c : 주함수와 스레드 관리
#include "header.h"

#define N_THREADS 2
static pthread_t thid[N_THREADS];

// externs
char strAnswerCorrect[100] = "nn45nn3c8n";
bool isDrawer = false;
bool bGameOver = false;

extern int main(int argc, char* argv[]) {

	// 옵션 지정하지 않았을 때 에러 출력하고 종료
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

	// writer와 reader 인자 구분
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

	// 본편 실행
	printf("creating threads... \n");
	if (isDrawer) {
		IpcInitClear(); // ipc 초기화
		InitDisplay(); // x window 초기화
		TypoSetInputAnswer(); // 사용자에게 제시어(맞혀야 할 단어) 입력 받음
		pthread_create(&thid[0], NULL, Thread1, NULL);  // 스레드0: x window 입력 받음
		pthread_create(&thid[1], NULL, Thread2Writer, NULL);  // 스레드1: ipc 메시지 들음
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

	// 종료
	printf("end of process (0) \n");
	return 0;
}



