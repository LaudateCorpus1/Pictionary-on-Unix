// 키보드 입력 받기 (본 프로그램은 이것을 메인 스레드에서 실행한다)
#include "header.h"

// 그리는(writer) 측에게 제시어 입력 받음
// 주의: ipc 초기화된 이후에 호출되어야 한다
extern void TypoSetInputAnswer() {
	printf("tell me what your answer is: ");
	tcflush(0, TCIFLUSH);
	scanf("%s", strAnswerCorrect);
	getchar();
	printf("the answer is %s\n", strAnswerCorrect);
	tcflush(0, TCIFLUSH);
	SndAnswerCorrect(strAnswerCorrect);
}

// 그리는(writer) 측의 경우 정답을 맞힐 때까지 실행
// 주의: ipc 초기화된 이후에 호출되어야 한다
extern void TypoInputLoopWriter() {
	int i;
	for (;!bGameOver;sleep(3));
	for (i = 0;i < 5;++i) {
		printf("%s, your mate got that one right\n", strAnswerCorrect);
	}
}

// 맞히는(reader) 측에게 제시어 입력 받음
// 주의: ipc 초기화된 이후에 호출되어야 한다
extern void TypoInputLoopReader() {
	int i;
	char tmp_strAnswer[100];
	for (;;tcflush(0, TCIFLUSH)) {
		printf("\ntell me what you think the answer is: \n");
		tcflush(0, TCIFLUSH);
		scanf("%s", tmp_strAnswer); // 입력 받아서
		if (strcmp(strAnswerCorrect, tmp_strAnswer) == 0) { // 맞혔다면
			for (i = 0;i < 5;++i) {
				printf("%s, you got that right\n", tmp_strAnswer);
			}
			SndGameOver();
			return;
		} else { // 틀렸다면
			printf("your answer is %s, which is incorrect. lol\n", tmp_strAnswer);
		}
	}
}

