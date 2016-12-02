// main.c : 주함수와 스레드 관리 + 터미널 입력 받기
#include "header.h"

#define N_THREADS 2
static pthread_t thid[N_THREADS];

// externs
char *strAnswerCorrect = "nnnnn";
bool isDrawer = false;
bool bGameOver = false;

extern int main(int argc, char* argv[]) {
	int i;
	char tmp_strAnswer[100];

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
	/*
	// 그리는(writer) 측에게 제시어 입력 받음
	if (isDrawer) {
		printf("tell me what your answer is: ");
		tcflush(0, TCIFLUSH);
		scanf("%s", tmp_strAnswer);
		getchar();
		strAnswerCorrect = tmp_strAnswer;
		printf("the answer is %s\n", strAnswerCorrect);
		tcflush(0, TCIFLUSH);		
	//	SndAnswerCorrect(strAnswerCorrect);
	}
	*/
	// 본편 실행
	printf("creating threads... \n");
	if (isDrawer) {
		IpcInitClear();
		InitDisplay();
		pthread_create(&thid[0], NULL, Thread1, NULL);  // user_input
		pthread_create(&thid[1], NULL, Thread2Writer, NULL);  // ipc
	} else /* if (!isDrawer) */ {
		IpcInit();
		InitDisplay();
		pthread_create(&thid[0], NULL, Thread1, NULL);  // user_input
		pthread_create(&thid[1], NULL, Thread2Reader, NULL);  // ipc
	}	
	printf("hello there \n");

	for (;;);
	/*
	// 맞히는(reader) 측에게 제시어 입력 받음
	if (!isDrawer) {
		for (;;) {
			printf("\ntell me what you think the answer is: \n");
			tcflush(0, TCIFLUSH);
			scanf("%s", tmp_strAnswer); // 입력 받아서
			if (strcmp(strAnswerCorrect, tmp_strAnswer) == 0) { // 맞혔다면
				for (i = 0;i < 5;++i) {
					printf("%s, you got that right\n", tmp_strAnswer);
				}
				SndGameOver();
				break;
			}
			else { // 틀렸다면
				printf("your answer is %s, which is incorrect. lol\n", tmp_strAnswer);
			}
			tcflush(0, TCIFLUSH);
		}
	} else { // 그리는(writer) 측의 경우 정답을 맞힐 때까지 실행
		for (;!bGameOver;sleep(3));
		for (i = 0;i < 5;++i) {
			printf("%s, your mate got that one right\n", strAnswerCorrect);
		}
	}
	*/
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



