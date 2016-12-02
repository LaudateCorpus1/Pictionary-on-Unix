// ipc1 : 그리는 입장
// ipc.c : 프로세스간 통신
#include "header.h"

#define KEY				(key_t)600201
#define PERMISSION		IPC_CREAT | 0660
#define MSGTYPE_PATH		1
#define MSGTYPE_ANSWER		2
#define MSGTYPE_GAMEOVER	3
static int qid;

// 주고 받을 데이터: 그린 점
struct path {
	long	data_type;
	int		index;
	int		x;
	int		y;
	int		color;
	int		width;
};

// 오류
static void fatal(char *err) {
	perror(err);
	exit(0);
}

// includes msgget()
extern void IpcInit() {
	if (-1 == (qid = msgget(KEY, PERMISSION))) {
		fatal("failed to init queue");
	}
}

// includes msgctl() msgget()
extern void IpcInitClear() {
	msgctl(qid, IPC_RMID, 0);
	if (-1 == (qid = msgget(KEY, PERMISSION))) {
		fatal("failed to init queue");
	}
}

// 그린 지점 한 단위를 메시지 전송
extern void SndPath(int _index, int _x, int _y, int _color, int _width) {
	struct path buf;
	// copy path
	printf("sndpath\n");
	buf.data_type = MSGTYPE_PATH;
	buf.index = _index;
	buf.x = _x;
	buf.y = _y;
	buf.color = _color;
	buf.width = _width;
	if (-1 == msgsnd(qid, &buf, sizeof(struct path) - sizeof(long), 0)) {
		fatal("SndPath msgsnd() failed");
	}
	printf("%d \n", _index);
}

// 받은 그린 데이터(구조체 메시지; 점 하나)를 풀어서 저장
static void RcvPath(struct path rcvpath) {
	if (indexPath >= MAX_INDEX_PATH) { return; } // 여분의 공간이 없으면 추가할 수 없음
	// 현재위치 저장
	path[rcvpath.index].x = rcvpath.x;
	path[rcvpath.index].y = rcvpath.y;
	// 현재위치와 함께 다른 (추가) 정보 저장
	pathColor[rcvpath.index] = rcvpath.color;
	pathWidth[rcvpath.index] = rcvpath.width;
	// 다음으로
	if (indexPath <= rcvpath.index) {
		indexPath = rcvpath.index + 1;
	}
}

// 정답을 맞히는 측(reader)이 메시지를 듣게 함
// ("Loop"은 쓰기(메시지의 발생)가 아닌 듣게(읽게) 하기 위한 것... 타이머 이벤트가 아닌 이상)
// (메시지의 전송은 메시지가 발생한 이벤트에서 수행하는 것이 옳다)
static void IpcLoopReader() {
	struct path buf;
	
	// 그린 데이터를 읽는다
	for (;;) {
		if (-1 == msgrcv(qid, &buf, sizeof(struct path) - sizeof(long), MSGTYPE_PATH, 0)) {
			fatal("failed to msgrcv()");
		}
		RcvPath(buf);
		RepaintPath();
	}
} // func

// 이 루틴을 writer(그리는 측)만이 호출한다
extern void *Thread2Writer() {
//	IpcLoopWriter();
	pthread_exit(NULL);
}

// 이 루틴을 reader(읽는 측)만이 호출한다
extern void *Thread2Reader() {
	IpcLoopReader();
	pthread_exit(NULL);
}


// 프로그램은 writer(그리는 측)를 먼저 실행한 뒤 reader(읽는 측)를 실행해야 한다

