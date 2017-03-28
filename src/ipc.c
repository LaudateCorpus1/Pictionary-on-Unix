// ipc1 : �׸��� ����
// ipc.c : ���μ����� ���
#include "header.h"

#define KEY				(key_t)600201
#define PERMISSION		IPC_CREAT | 0660
#define MSGTYPE_PATH		1
#define MSGTYPE_ANSWER		2
#define MSGTYPE_GAMEOVER	3
static int qid;

// �ְ� ���� ������: �׸� ��
struct path {
	long	data_type;
	int		index;
	int		x;
	int		y;
	int		color;
	int		width;
};

// �ְ� ���� ������: ����
struct answer {
	long	data_type;
	int n;
	char	str[100];
};

#define YO 100 // ����: struct gameover

// reader�� ������ ������ ������ �������� �˸��� �޽���
struct gameover {
	long data_type;
	int yo;
};

// ����
static void fatal(char *err) {
	perror(err);
	exit(0);
}

// ť�� ����
static void ClearQueue() {
	struct path buf1;
	struct answer buf2;
	struct gameover buf3;
	for (;msgrcv(qid, &buf1, sizeof(struct path) - sizeof(long), MSGTYPE_PATH, IPC_NOWAIT) == 0;);
	for (;msgrcv(qid, &buf2, sizeof(struct answer) - sizeof(long), MSGTYPE_ANSWER, IPC_NOWAIT) == 0;);
	for (;msgrcv(qid, &buf3, sizeof(struct gameover) - sizeof(long), MSGTYPE_GAMEOVER, IPC_NOWAIT) == 0;);
}

// includes msgctl() msgget()
extern void IpcInit() {
	msgctl(qid, IPC_RMID, NULL);
	if (-1 == (qid = msgget(KEY, PERMISSION))) {
		fatal("failed to init queue");
	}
}

// includes msgctl() msgget() // ClearQueue()
extern void IpcInitClear() {
	msgctl(qid, IPC_RMID, NULL);
	if (-1 == (qid = msgget(KEY, PERMISSION))) {
		fatal("failed to init queue");
	}
	ClearQueue();
}

// ���� ������
extern void SndGameOver() {
	struct gameover buf;
	buf.data_type = MSGTYPE_GAMEOVER;
	buf.yo = YO;
	if (-1 == msgsnd(qid, &buf, sizeof(struct gameover) - sizeof(long), 0)) {
		fatal("SndGameOver msgsnd() failed");
	}
}

// ���� �����ϱ�
extern void SndAnswerCorrect(char *strAnswer) {
	struct answer buf;
	//
	buf.data_type = MSGTYPE_ANSWER;
	sprintf(buf.str, "%s", strAnswer); // strcpy(buf.str, strAnswer);
	printf("%s \n", buf.str);
	if (-1 == msgsnd(qid, &buf, sizeof(struct answer) - sizeof(long), 0)) {
		fatal("SndAnswerCorrect msgsnd() failed");
	}
}

// �׸� ���� �� ������ �޽��� ����
extern void SndPath(int _index, int _x, int _y, int _color, int _width) {
	struct path buf;
	// copy path
	buf.data_type = MSGTYPE_PATH;
	buf.index = _index;
	buf.x = _x;
	buf.y = _y;
	buf.color = _color;
	buf.width = _width;
	if (-1 == msgsnd(qid, &buf, sizeof(struct path) - sizeof(long), 0)) {
		fatal("SndPath msgsnd() failed");
	}
}

// ���� �׸� ������(����ü �޽���; �� �ϳ�)�� Ǯ� ����
static void RcvPath(struct path rcvpath) {
	if (indexPath >= MAX_INDEX_PATH) { return; } // ������ ������ ������ �߰��� �� ����
	// ������ġ ����
	path[rcvpath.index].x = rcvpath.x;
	path[rcvpath.index].y = rcvpath.y;
	// ������ġ�� �Բ� �ٸ� (�߰�) ���� ����
	pathColor[rcvpath.index] = rcvpath.color;
	pathWidth[rcvpath.index] = rcvpath.width;
	// CLEAR �Է� ���� ���
	if (rcvpath.index == -1) {
		Clear(); // �����
		indexPath = 0;
	}
	// ��������
	if (indexPath <= rcvpath.index) {
		indexPath = rcvpath.index + 1;
	}
}

// ������ ���� �׸��� ��(writer)�� �޽����� ��� ��
// ("Loop"�� ����(�޽����� �߻�)�� �ƴ� ���(�а�) �ϱ� ���� ��... Ÿ�̸� �̺�Ʈ�� �ƴ� �̻�)
// (�޽����� ������ �޽����� �߻��� �̺�Ʈ���� �����ϴ� ���� �Ǵ�)
static void IpcLoopWriter() {
	struct gameover buf;

	// gameover �޽����� üũ�ϴٰ� �޴� ���� ���� ������
	for (;;) {
		if (-1 == msgrcv(qid, &buf, sizeof(struct path) - sizeof(long), MSGTYPE_GAMEOVER, 0)) {
			fatal("failed to msgrcv()");
		}
		if (buf.yo == YO) {
			bGameOver = true;
		}
	}
} // func

// ������ ������ ��(reader)�� �޽����� ��� ��
// ("Loop"�� ����(�޽����� �߻�)�� �ƴ� ���(�а�) �ϱ� ���� ��... Ÿ�̸� �̺�Ʈ�� �ƴ� �̻�)
// (�޽����� ������ �޽����� �߻��� �̺�Ʈ���� �����ϴ� ���� �Ǵ�)
static void IpcLoopReader() {
	struct path buf;

	// ���� ���þ �о���δ�
	struct answer buf_answer;
	if (-1 == msgrcv(qid, &buf_answer, sizeof(buf_answer) - sizeof(long), MSGTYPE_ANSWER, 0)) {
		fatal("failed to msgrcv()");
	}
	strcpy(strAnswerCorrect, buf_answer.str);
	printf("\n your mate came up with a word. guess what! \n\n");
	// printf("debug: ans: %s \n", strAnswerCorrect);

	// �׸� �����͸� �д´�
	for (;;) {
		if (-1 == msgrcv(qid, &buf, sizeof(struct path) - sizeof(long), MSGTYPE_PATH, 0)) {
			fatal("failed to msgrcv()");
		}
		RcvPath(buf);
		RepaintPath();
	}
} // func

// �� ��ƾ�� writer(�׸��� ��)���� ȣ���Ѵ�
extern void *Thread2Writer() {
	IpcLoopWriter();
	pthread_exit(NULL);
}

// �� ��ƾ�� reader(�д� ��)���� ȣ���Ѵ�
extern void *Thread2Reader() {
	IpcLoopReader();
	pthread_exit(NULL);
}


// ���α׷��� writer(�׸��� ��)�� ���� ������ �� reader(�д� ��)�� �����ؾ� �Ѵ�

