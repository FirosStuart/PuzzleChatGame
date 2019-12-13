
//  �w�b�_�t�@�C��
#include <Windows.h>
#include <WinSock.h>
#include <string>
#include <stdio.h>

// �g�p���C�u����
#pragma comment(lib,"wsock32.lib")

//  �萔��`
#define WM_SOCKET       (WM_USER+1)     // �\�P�b�g�p���b�Z�[�W
#define PORT            10000           // �ʐM�|�[�g�ԍ�

#define IDB_CONNECT     1000            // [�ڑ�]�{�^��
#define IDB_ACCEPT      1001            // [�ڑ��҂�]�{�^��
#define IDB_REJECT      1002            // [�ؒf]�{�^��
#define IDB_REJECTORDER 1003            // [�ؒf�v��]�{�^��
#define IDB_GIVEUP      1004			// [�M�u�A�b�v]�{�^��
#define IDB_CORRECT		1005			// [����]�{�^��
#define	IDB_INCORRECT   1006			// [�s����]�{�^��
#define IDB_POINTOUT    1007			// [�w�E]�{�^��
#define	IDB_CONSENT     1008			// [����]�{�^��
#define IDB_DENIAL      1009			// [�۔F]�{�^��
#define IDB_CHANGE      1010			// [���]�{�^��
#define IDB_SEND        1011            // [���M]�{�^��

#define IDF_HOSTNAME    2000            // �z�X�g�����̓G�f�B�b�g�{�b�N�X
#define IDF_QUESTION    2001            // ����\���p�G�f�B�b�g�{�b�N�X
#define IDF_RULE        2002            // ��������p�G�f�B�b�g�{�b�N�X
#define IDF_SENDMSG     2003            // ���M���b�Z�[�W���͗p�G�f�B�b�g�{�b�N�X
#define IDF_RECVMSG     2004            // ��M���b�Z�[�W�\���p�G�f�B�b�g�{�b�N�X
#define IDF_SCORE_PL1   2005            // �����̓��_�p�G�f�B�b�g�{�b�N�X
#define IDF_SCORE_PL2   2006            // ����̓��_�p�G�f�B�b�g�{�b�N�X
#define WINDOW_W		910				// �E�B���h�E�̕�
#define WINDOW_H		800             // �E�B���h�E�̍���
#define MAX_ARRAY		10000			// �z��̍ő�v�f��
#define MAX_MESSEGE		30				// ���b�Z�[�W�̍ő�v�f��

//�O���[�o���ϐ�
LPCTSTR lpClassName = "Oekaki";         // �E�B���h�E�N���X��
LPCTSTR lpWindowName = "Oekaki";        // �^�C�g���o�[�ɂ����O

SOCKET sock = INVALID_SOCKET;           // �\�P�b�g
SOCKET sv_sock = INVALID_SOCKET;        // �T�[�o�p�\�P�b�g
HOSTENT* phe;							// HOSTENT�\����

HPEN hPenBlack;							// ���y��
HPEN hPenRed;							// �ԃy��
HPEN hPenWhite;							// ���y��

static HWND hWndHost;                       // �z�X�g�����͗p�G�f�B�b�g�{�b�N�X
static HWND hWndQuestion;                   // ����\���p�G�f�B�b�g�{�b�N�X
static HWND hWndRule;                       // ��������p�G�f�B�b�g�{�b�N�X
static HWND hWndSendMSG;                    // ���M���b�Z�[�W���͗p�G�f�B�b�g�{�b�N�X
static HWND hWndRecvMSG;                    // ��M���b�Z�[�W�\���p�G�f�B�b�g�{�b�N�X
static HWND hWndScore_pl1;                  // �����̓��_�p�G�f�B�b�g�{�b�N�X
static HWND hWndScore_pl2;                  // ����̓��_�p�G�f�B�b�g�{�b�N�X
static HWND hWndConnect, hWndAccept;        // [�ڑ�]�{�^����[�ڑ��҂�]�{�^��
static HWND hWndReject;                     // [�ؒf]�{�^��
static HWND hWndRejectOrder;                // [�ؒf�v��]�{�^��
static HWND hWndGiveup;						// [�M�u�A�b�v]�{�^��
static HWND hWndCorrect;					// [����]�{�^��
static HWND hWndIncorrect;					// [�s����]�{�^��
static HWND hWndPointout;					// [�w�E]�{�^��
static HWND hWndConsent;					// [����]�{�^��
static HWND hWndDenial;						// [�۔F]�{�^��
static HWND hWndChange;						// [���]�{�^��
static HWND hWndSend;                       // [���M]�{�^��
static int DrowableFlag = 0;				// ���� 1:�\/0:�s�� ��Ԋm�F�t���O
int FlagPlayer;				// ���� 1:�\/0:�s�� ��Ԋm�F�t���O

const RECT d = { 10, 200, 450, 700 };               // �`��̈�(�������x���W, �������y���W, �E������x���W, �E������y���W)

int n;                                              // �J�E���^
int flag[MAX_ARRAY];                                // �y���_�E���t���O
POINT pos[MAX_ARRAY];                               // ���W���i�[
HPEN colors[MAX_ARRAY];								// �g�p����F

int turn = 0;											//���
int score_Master = 0;								//�}�X�^�[�̃X�R�A	
int score_Player = 0;								//�v���C���[�̃X�R�A
int score_PL1 = 0;									//PL1�̃X�R�A
int score_PL2 = 0;									//PL2�̃X�R�A
char rule[3][10] = { "�G�̂�","�G�ƕ���","�����̂�" };
int card[6] = { 0,1,2,3,4 };
int rule_num = 0;
char card_text[5][6][30] =
{
	{"�L","�l","��","�F","��","�n"},
	{"�e���r","����@","�G�A�R��","�①��","�d�q�����W","������"},
	{"�N���X�}�X","�o�����^�C��","������","�n���E�B��","�čՂ�","���["},
	{"�T�b�J�[","�싅","�o���[","�o�X�P","�싅","�e�j�X"},
	{"�L���r���A�e���_���g","�i�[�X","��Ј�","�w��","�x�@��","�o�[�e���_�["},
};
//  �v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);// �E�B���h�E�֐�
LRESULT CALLBACK OnPaint(HWND, UINT, WPARAM, LPARAM);	// �`��֐�

void change(int a, int b);
void enable(BOOL Host, BOOL Connect, BOOL Accept, BOOL Reject, BOOL RejectOrder, int Drow,
	BOOL Giveup, BOOL Correct, BOOL InCorrect, BOOL Consent, BOOL Pointout, BOOL Denial, BOOL Send, BOOL Change);
BOOL checkMousePos(int x, int y);						// �}�E�X�̈ʒu���L�����p�X�̒����ǂ������肷��
BOOL SockInit(HWND hWnd);                               // �\�P�b�g������
BOOL SockAccept(HWND hWnd);                             // �\�P�b�g�ڑ��҂�
BOOL SockConnect(HWND hWnd, LPCSTR host);               // �\�P�b�g�ڑ�
void setData(int flag, int x, int y, HPEN color);       // �`���������
void FXY(int f, int x, int y);
void ChatReset(HWND chatbox);
int randAtoC();
void rand0toi(int ary[], int size);
int dice();
int ctoi(char a);

//  WinMain�֐� (Windows�v���O�����N�����ɌĂ΂��֐�)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;                                          // �E�B���h�E�n���h��
	MSG  msg;                                           // ���b�Z�[�W
	WNDCLASSEX wc;                                      // �E�B���h�E�N���X

	//�E�B���h�E�N���X��`
	wc.hInstance = hInstance;							// �C���X�^���X
	wc.lpszClassName = lpClassName;                     // �N���X��
	wc.lpfnWndProc = WindowProc;						// �E�B���h�E�֐���
	wc.style = 0;										// �N���X�X�^�C��
	wc.cbSize = sizeof(WNDCLASSEX);						// �\���̃T�C�Y
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// �A�C�R���n���h��
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);			// �X���[���A�C�R��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// �}�E�X�|�C���^
	wc.lpszMenuName = NULL;                             // ���j���[(�Ȃ�)
	wc.cbClsExtra = 0;									// �N���X�g�����
	wc.cbWndExtra = 0;									// �E�B���h�E�g�����
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;			// �E�B���h�E�̔w�i�F
	if (!RegisterClassEx(&wc)) return 0;                // �E�B���h�E�N���X�o�^

	// �E�B���h�E����
	hWnd = CreateWindow(
		lpClassName,                                // �E�B���h�E�N���X��
		lpWindowName,                               // �E�B���h�E��
		WS_DLGFRAME | WS_VISIBLE | WS_SYSMENU,      // �E�B���h�E����
		CW_USEDEFAULT,                              // �E�B���h�E�\���ʒu(X)
		CW_USEDEFAULT,                              // �E�B���h�E�\���ʒu(Y)
		WINDOW_W,                                   // �E�B���h�E�T�C�Y(X)
		WINDOW_H,                                   // �E�B���h�E�T�C�Y(Y)
		HWND_DESKTOP,                               // �e�E�B���h�E�n���h��
		NULL,
		hInstance,                                  // �C���X�^���X�n���h��
		NULL
	);

	// �E�B���h�E�\��
	ShowWindow(hWnd, nCmdShow);                         // �E�B���h�E�\�����[�h
	UpdateWindow(hWnd);                                 // �E�C���h�E�X�V

	// ���b�Z�[�W���[�v
	while (GetMessage(&msg, NULL, 0, 0)) {              // ���b�Z�[�W���擾
		TranslateMessage(&msg);
		DispatchMessage(&msg);                          // ���b�Z�[�W����
	}
	return (int)msg.wParam;                             // �v���O�����I��
}


//  �E�B���h�E�֐�(�C�x���g�������L�q)
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wP, LPARAM lP)
{
	static BOOL mouseFlg = FALSE;       // �O��̏�� TRUE:�`�悵���AFALSE:�`�悵�Ă��Ȃ�
	SOCKADDR_IN cl_sin;
	int flag2, x2, y2;
	int len = sizeof(cl_sin);
	char host[100];
	char buf[100];						// ��M���e���ꎞ�I�Ɋi�[����o�b�t�@


	switch (uMsg) {
	case WM_CREATE:			// �E�B���h�E���������ꂽ
	{// ������\��
		CreateWindow("static", "Host Name",
			WS_CHILD | WS_VISIBLE, 10, 10, 100, 18,
			hWnd, NULL, NULL, NULL);
		CreateWindow("static", "Question",
			WS_CHILD | WS_VISIBLE, 10, 130, 100, 18,
			hWnd, NULL, NULL, NULL);
		CreateWindow("static", "Rule",
			WS_CHILD | WS_VISIBLE, 460, 180, 100, 18,
			hWnd, NULL, NULL, NULL);
		CreateWindow("static", "player1",
			WS_CHILD | WS_VISIBLE, 660, 100, 100, 25,
			hWnd, NULL, NULL, NULL);
		CreateWindow("static", "vs player2",
			WS_CHILD | WS_VISIBLE, 760, 100, 100, 25,
			hWnd, NULL, NULL, NULL);
		CreateWindow(TEXT("static"), TEXT("Send Message"), WS_CHILD | WS_VISIBLE,
			460, 300, 200, 18, hWnd, NULL, NULL, NULL);
		CreateWindow(TEXT("static"), TEXT("Receive Message"), WS_CHILD | WS_VISIBLE,
			460, 470, 200, 18, hWnd, NULL, NULL, NULL);
		// �z�X�g�����͗p�G�f�B�b�g�{�b�N�X
		hWndHost = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE, 10, 30, 200, 25,
			hWnd, (HMENU)IDF_HOSTNAME, NULL, NULL);
		// [�ڑ�]�{�^��
		hWndConnect = CreateWindow("button", "�ڑ�",
			WS_CHILD | WS_VISIBLE, 220, 30, 50, 25,
			hWnd, (HMENU)IDB_CONNECT, NULL, NULL);
		// [�ڑ��҂�]�{�^��
		hWndAccept = CreateWindow("button", "�ڑ��҂�",
			WS_CHILD | WS_VISIBLE, 275, 30, 90, 25,
			hWnd, (HMENU)IDB_ACCEPT, NULL, NULL);
		// [�ؒf�v��]�{�^��
		hWndRejectOrder = CreateWindow("button", "�ؒf�v��",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 275, 70, 90, 25,
			hWnd, (HMENU)IDB_REJECTORDER, NULL, NULL);
		// [�ؒf]�{�^��
		hWndReject = CreateWindow("button", "�ؒf",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 220, 70, 50, 25,
			hWnd, (HMENU)IDB_REJECT, NULL, NULL);
		// [���M]�{�^��
		hWndSend = CreateWindow("button", "���M",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 810, 610, 90, 25,
			hWnd, (HMENU)IDB_SEND, NULL, NULL);
		// [�M�u�A�b�v]�{�^��
		hWndGiveup = CreateWindow("button", "�M�u�A�b�v",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 480, 30, 100, 25,
			hWnd, (HMENU)IDB_GIVEUP, NULL, NULL);
		// [����]�{�^��
		hWndCorrect = CreateWindow("button", "����",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 400, 30, 70, 25,
			hWnd, (HMENU)IDB_CORRECT, NULL, NULL);
		// [�s����]�{�^��
		hWndIncorrect = CreateWindow("button", "�s����",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 400, 70, 70, 25,
			hWnd, (HMENU)IDB_INCORRECT, NULL, NULL);
		// [�w�E]�{�^��
		hWndPointout = CreateWindow("button", "�w�E",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 480, 70, 100, 25,
			hWnd, (HMENU)IDB_POINTOUT, NULL, NULL);
		// [����]�{�^��
		hWndConsent = CreateWindow("button", "����",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 590, 30, 50, 25,
			hWnd, (HMENU)IDB_CONSENT, NULL, NULL);
		// [�۔F]�{�^��
		hWndDenial = CreateWindow("button", "�۔F",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 590, 70, 50, 25,
			hWnd, (HMENU)IDB_DENIAL, NULL, NULL);
		// [���]�{�^��
		hWndChange = CreateWindow("button", "���",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 660, 30, 50, 25,
			hWnd, (HMENU)IDB_CHANGE, NULL, NULL);
		// ����p�G�f�B�b�g�{�b�N�X
		hWndQuestion = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_READONLY, 10, 150, 200, 25,
			hWnd, (HMENU)IDF_QUESTION, NULL, NULL);
		// ��������p�G�f�B�b�g�{�b�N�X
		hWndRule = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_READONLY, 460, 200, 300, 100,
			hWnd, (HMENU)IDF_RULE, NULL, NULL);
		// ���M���b�Z�[�W���͗p�G�f�B�b�g�{�b�N�X
		hWndSendMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_DISABLED, 460, 320, 300, 150,
			hWnd, (HMENU)IDF_SENDMSG, NULL, NULL);
		// ��M���b�Z�[�W�\���p�G�f�B�b�g�{�b�N�X
		hWndRecvMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 460, 490, 300, 150,
			hWnd, (HMENU)IDF_RECVMSG, NULL, NULL);
		// �����̓��_�p�G�f�B�b�g�{�b�N�X
		hWndScore_pl1 = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_READONLY, 720, 100, 25, 25,
			hWnd, (HMENU)IDF_SCORE_PL1, NULL, NULL);
		// ����̓��_�p�G�f�B�b�g�{�b�N�X
		hWndScore_pl2 = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_READONLY, 835, 100, 25, 25,
			hWnd, (HMENU)IDF_SCORE_PL2, NULL, NULL);

		SockInit(hWnd);         // �\�P�b�g������

		rand0toi(card, 5);

		hPenBlack = (HPEN)CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		hPenRed = (HPEN)CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		hPenWhite = (HPEN)CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
		return 0L;
	}
	case WM_COMMAND:		// �{�^���������ꂽ
	{
		switch (LOWORD(wP)) {
		case IDB_ACCEPT:    // [�ڑ��҂�]�{�^������(�T�[�o�[)
			if (SockAccept(hWnd)) {  // �ڑ��҂��v��
				return 0L;      // �ڑ��҂����s
			}
			enable(FALSE, FALSE, FALSE, TRUE, FALSE, 0,
				FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);
			return 0L;

		case IDB_CONNECT:   // [�ڑ�]�{�^������(�N���C�A���g)

			GetWindowText(hWndHost, host, sizeof(host));

			if (SockConnect(hWnd, host)) {   // �ڑ��v��
				return 0L;
			}
			enable(FALSE, FALSE, FALSE, TRUE, FALSE, 0,
				FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);;
			return 0L;

		case IDB_REJECT:    // [�ؒf]�{�^������
			if (sock != INVALID_SOCKET) {    // �������N���C�A���g���Ȃ�
				// �\�P�b�g�����
				closesocket(sock);
				sock = INVALID_SOCKET;
			}
			if (sv_sock != INVALID_SOCKET) { // �������T�[�o���Ȃ�
				// �T�[�o�p�\�P�b�g�����
				closesocket(sv_sock);
				sv_sock = INVALID_SOCKET;
			}
			phe = NULL;
			enable(TRUE, TRUE, TRUE, FALSE, FALSE, 0,
				FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);
			return 0L;

		case IDB_REJECTORDER:      // [�ؒf�v��]�{�^������
			strcpy_s(buf, "REJECT");
			send(sock, buf, strlen(buf) + 1, 0);
			return 0L;

		case IDB_SEND:      // [���M]�{�^������
			char sender[100];
			GetWindowText(hWndSendMSG, buf, sizeof(buf) - 1);     // ���M���b�Z�[�W���͗��̓��e���擾
			sprintf_s(sender, "%s%s", "MESS", buf);
			if (send(sock, sender, strlen(sender) + 1, 0) == SOCKET_ERROR) {    // ���M����
						// ���M�Ɏ��s������G���[��\��
				MessageBox(hWnd, TEXT("sending failed"), TEXT("Error"),
					MB_OK | MB_ICONEXCLAMATION);
			}

			ChatReset(hWndSendMSG);
			return 0L;

		case IDB_GIVEUP:	// [�M�u�A�b�v]�{�^������
			strcpy_s(buf, "GIVEUP");
			send(sock, buf, strlen(buf) + 1, 0);
			score_Master = 1;
			score_Player = 1;
			change(score_Master, score_Player);//���
			return 0L;

		case IDB_CORRECT:   // [����]����
			strcpy_s(buf, "CORRECT");
			send(sock, buf, strlen(buf) + 1, 0);
			return 0L;

		case IDB_INCORRECT: // [�s����]����
			strcpy_s(buf, "INCORRECT");
			send(sock, buf, strlen(buf) + 1, 0);
			return 0L;

		case IDB_POINTOUT:  // [�w�E]����
			strcpy_s(buf, "POINTOUT");
			send(sock, buf, strlen(buf) + 1, 0);
			return 0L;

		case IDB_CONSENT:   // [����]����
			strcpy_s(buf, "CONSENT");
			send(sock, buf, strlen(buf) + 1, 0);
			score_Master = 0;
			score_Player = 1;
			change(score_Master, score_Player);//���
			return 0L;

		case IDB_DENIAL:    // [�۔F]����
			strcpy_s(buf, "DENIAL");
			send(sock, buf, strlen(buf) + 1, 0);
			if (rule_num == 0) {//A�𐳉�
				score_Master = 1;
				score_Player = 2;
			}
			else if (rule_num == 1) {//B�𐳉�
				score_Master = 1;
				score_Player = 3;
			}
			else if (rule_num == 2) {//C�𐳉�
				score_Master = 1;
				score_Player = 5;
			}
			change(score_Master, score_Player);//���
			return 0L;

		case IDB_CHANGE:    // [���]����
			strcpy_s(buf, "CHANGE");
			send(sock, buf, strlen(buf) + 1, 0);
			if (rule_num == 0) {//A�𐳉�
				score_Master = 1;
				score_Player = 2;
			}
			else if (rule_num == 1) {//B�𐳉�
				score_Master = 1;
				score_Player = 3;
			}
			else if (rule_num == 2) {//C�𐳉�
				score_Master = 1;
				score_Player = 5;
			}
			change(score_Master, score_Player);//���
			return 0L;

		} /* end of switch (LOWORD(wP)) */

		return 0L;
	}
	case WM_LBUTTONDOWN:    // �}�E�X���{�^���������ꂽ
	{
		if (checkMousePos(LOWORD(lP), HIWORD(lP)) == TRUE) {   // �`��̈�̒��Ȃ�
			setData(0, LOWORD(lP), HIWORD(lP), hPenBlack);       // ���̎n�_�Ƃ��č��W���L�^
			FXY(0, LOWORD(lP), HIWORD(lP));       // ���̎n�_�Ƃ��č��W���L�^
			mouseFlg = FALSE;
			if (DrowableFlag == 1) {
				n++;
				mouseFlg = TRUE;
			}
			InvalidateRect(hWnd, &d, FALSE);


		}
		return 0L;
	}
	case WM_RBUTTONDOWN:    // �}�E�X�E�{�^���������ꂽ
	{
		if (checkMousePos(LOWORD(lP), HIWORD(lP)) == TRUE) {   // �`��̈�̒��Ȃ�
			setData(0, LOWORD(lP), HIWORD(lP), hPenWhite);       // ���̎n�_�Ƃ��č��W���L�^
			FXY(0, LOWORD(lP), HIWORD(lP));       // ���̎n�_�Ƃ��č��W���L�^
			mouseFlg = FALSE;
			if (DrowableFlag == 1) {
				n++;
				mouseFlg = TRUE;
			}
			InvalidateRect(hWnd, &d, FALSE);


		}
		return 0L;
	}
	case WM_MOUSEMOVE:  // �}�E�X�|�C���^���ړ�����
	{
		if (wP == MK_LBUTTON) {  // ���{�^����������Ă���
			if (checkMousePos(LOWORD(lP), HIWORD(lP)) == TRUE) {		// �`��̈�̒��Ȃ�
				if (mouseFlg) {										// �O��`�悵�Ă���Ȃ�
					setData(1, LOWORD(lP), HIWORD(lP), hPenBlack);   // ���̓r���Ƃ��č��W���L�^
					FXY(1, LOWORD(lP), HIWORD(lP));					// ���̓r���Ƃ��č��W�𑗐M
				}
				else {												// �O��`�悵�Ă��Ȃ��Ȃ�
					setData(0, LOWORD(lP), HIWORD(lP), hPenBlack);   // ���̎n�_�Ƃ��č��W���L�^
					FXY(0, LOWORD(lP), HIWORD(lP));					// ���̎n�_�Ƃ��č��W�𑗐M
				}
				mouseFlg = FALSE;
				if (DrowableFlag == 1) {
					n++;
					mouseFlg = TRUE;
				}InvalidateRect(hWnd, &d, FALSE);
			}
			else {                     // �`��̈�̊O�Ȃ�
				mouseFlg = FALSE;
			}
		}
		else if (wP == MK_RBUTTON) {  // ���{�^����������Ă���
			if (checkMousePos(LOWORD(lP), HIWORD(lP)) == TRUE) {		// �`��̈�̒��Ȃ�
				if (mouseFlg) {										// �O��`�悵�Ă���Ȃ�
					setData(1, LOWORD(lP), HIWORD(lP), hPenWhite);   // ���̓r���Ƃ��č��W���L�^
					FXY(1, LOWORD(lP), HIWORD(lP));					// ���̓r���Ƃ��č��W�𑗐M
				}
				else {												// �O��`�悵�Ă��Ȃ��Ȃ�
					setData(0, LOWORD(lP), HIWORD(lP), hPenWhite);   // ���̎n�_�Ƃ��č��W���L�^
					FXY(0, LOWORD(lP), HIWORD(lP));					// ���̎n�_�Ƃ��č��W�𑗐M
				}
				mouseFlg = FALSE;
				if (DrowableFlag == 1) {
					n++;
					mouseFlg = TRUE;
				}InvalidateRect(hWnd, &d, FALSE);
			}
			else {                     // �`��̈�̊O�Ȃ�
				mouseFlg = FALSE;
			}
		}

		return 0L;
	}
	case WM_SOCKET:          // �񓯊��������b�Z�[�W
	{
		if (WSAGETSELECTERROR(lP) != 0) { return 0L; }

		switch (WSAGETSELECTEVENT(lP)) {
		case FD_ACCEPT:     // �ڑ��҂������ʒm
		{

			sock = accept(sv_sock, (LPSOCKADDR)&cl_sin, &len);

			if (sock == INVALID_SOCKET) {
				MessageBox(hWnd, "Accepting connection failed",
					"Error", MB_OK | MB_ICONEXCLAMATION);
				closesocket(sv_sock);
				sv_sock = INVALID_SOCKET;
				enable(TRUE, TRUE, TRUE, FALSE, FALSE, 0,
					FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);
				return 0L;
			}

#ifndef NO_DNS
			// �z�X�g���擾
			phe = gethostbyaddr((char*)&cl_sin.sin_addr, 4, AF_INET);
			if (phe) { SetWindowText(hWndHost, phe->h_name); }
#endif  NO_DNS

			// �񓯊����[�h (��M���ؒf�j
			if (WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE)
				== SOCKET_ERROR) {
				// �ڑ��Ɏ��s�����珉����Ԃɖ߂�
				MessageBox(hWnd, "WSAAsyncSelect() failed",
					"Error", MB_OK | MB_ICONEXCLAMATION);
				enable(TRUE, TRUE, TRUE, FALSE, FALSE, 0,
					FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);
				return 0L;
			}
			enable(FALSE, FALSE, FALSE, TRUE, TRUE, 0,
				FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE);
			FlagPlayer = 1;
			change(0, 0);
			DrowableFlag = 1;
			return 0L;
		}/* end of case FD_ACCEPT: */

		case FD_CONNECT:    // �ڑ������ʒm
			// �񓯊����[�h (��M���ؒf)
			if (WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE)
				== SOCKET_ERROR) {
				// �ڑ��Ɏ��s�����珉����Ԃɖ߂�

				MessageBox(hWnd, "WSAAsyncSelect() failed",
					"Error", MB_OK | MB_ICONEXCLAMATION);
				enable(TRUE, TRUE, TRUE, FALSE, FALSE, 0,
					FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);
				return 0L;
			}
			enable(FALSE, FALSE, FALSE, TRUE, TRUE, 0,
				TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE);
			change(0, 0);
			DrowableFlag = 1;
			return 0L;

		case FD_READ:									//���b�Z�[�W��M
			char b[100];

			if (recv(sock, buf, sizeof(buf) - 1, 0) != SOCKET_ERROR) { // ��M�ł����Ȃ�			
				if (strncmp(buf, "MESS", 4) == 0) {

					strncpy_s(b, buf + 4, sizeof(buf) - 4);

					SetWindowText(hWndRecvMSG, b);

				}
				else if (strncmp(buf, "RULE", 4) == 0) {
					strncpy_s(b, buf + 4, sizeof(buf) - 4);
					rule_num = atoi(b);
					SetWindowText(hWndRule, rule[rule_num]);
				}
				else if (strcmp(buf, "REJECT") == 0) {
					MessageBox(hWnd, "�ؒf�v�������܂����B",
						"Information", MB_OK | MB_ICONINFORMATION);
				}
				else if (strcmp(buf, "CORRECT") == 0) {
					enable(FALSE, FALSE, FALSE, TRUE, TRUE, 0,
						FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE);

				}
				else if (strcmp(buf, "POINTOUT") == 0) {
					enable(FALSE, FALSE, FALSE, TRUE, TRUE, 0,
						FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, FALSE);

				}
				else if (strcmp(buf, "CHANGE") == 0 || strcmp(buf, "DENIAL") == 0) {//�w�E��F�߂Ȃ����A
					if (rule_num == 0) {//A�𐳉�
						score_Master = 1;
						score_Player = 2;
					}
					else if (rule_num == 1) {//B�𐳉�
						score_Master = 1;
						score_Player = 3;
					}
					else if (rule_num == 2) {//C�𐳉�
						score_Master = 1;
						score_Player = 5;
					}
					change(score_Master, score_Player);//���
				}
				else if (strcmp(buf, "CONSENT") == 0) {//�w�E�����F
					score_Master = 0;
					score_Player = 1;
					change(score_Master, score_Player);//���
				}
				else if (strcmp(buf, "GIVEUP") == 0) {//�M�u�A�b�v
					score_Master = 1;
					score_Player = 1;
					change(score_Master, score_Player);//���
				}
				else if (strcmp(buf, "POINTOUT") == 0) {//�w�E������
					MessageBox(hWnd, "�w�E�_������܂��B",
						"Information", MB_OK | MB_ICONINFORMATION);
				}
				else {
					sscanf_s(buf, "%1d%03d%03d", &flag2, &x2, &y2);
					setData(flag2, x2, y2, hPenRed);
					mouseFlg = FALSE;
					if (DrowableFlag == 1) {
						n++;
						mouseFlg = TRUE;
					}InvalidateRect(hWnd, &d, FALSE);
				}
			}
			return 0L;

		case FD_CLOSE:      // �ؒf���ꂽ
			MessageBox(hWnd, "�ؒf����܂����B",
				"Information", MB_OK | MB_ICONINFORMATION);
			SendMessage(hWnd, WM_COMMAND, IDB_REJECT, 0); // �ؒf�������s
			DrowableFlag = 0;
			return 0L;

		}/* end of switch (WSAGETSELECTEVENT(lP)) */

		return 0L;
	}
	case WM_DESTROY:    // �E�B���h�E���j�����ꂽ
	{
		DeleteObject(hPenBlack);    // ���y���폜
		DeleteObject(hPenRed);    // �ԃy���폜
		DeleteObject(hPenWhite);    // �ԃy���폜
		closesocket(sock);
		PostQuitMessage(0);
		return 0L;
	}
	case WM_PAINT:      // �ĕ`��
	{
		return OnPaint(hWnd, uMsg, wP, lP);
		return 0L;
	}
	default:
	{
		return DefWindowProc(hWnd, uMsg, wP, lP);  // �W�����b�Z�[�W����
	}
	}/* end of switch (uMsg) */
}

//  �\�P�b�g����������
BOOL SockInit(HWND hWnd)
{
	WSADATA wsa;
	int ret;
	char ret_buf[80];

	ret = WSAStartup(MAKEWORD(1, 1), &wsa);

	if (ret != 0) {
		wsprintf(ret_buf, "%d is the err", ret);
		MessageBox(hWnd, ret_buf, "Error", MB_OK | MB_ICONSTOP);
		exit(-1);
	}
	return FALSE;
}

//  �\�P�b�g�ڑ� (�N���C�A���g��)
BOOL SockConnect(HWND hWnd, LPCSTR host)
{
	SOCKADDR_IN cl_sin; // SOCKADDR_IN�\����

	// �\�P�b�g���J��
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {        // �\�P�b�g�쐬���s
		MessageBox(hWnd, "Socket() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	memset(&cl_sin, 0x00, sizeof(cl_sin)); // �\���̏�����
	cl_sin.sin_family = AF_INET;           // �C���^�[�l�b�g
	cl_sin.sin_port = htons(PORT);       // �|�[�g�ԍ��w��

	phe = gethostbyname(host); // �A�h���X�擾

	if (phe == NULL) {
		MessageBox(hWnd, "gethostbyname() failed.",
			"Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}
	memcpy(&cl_sin.sin_addr, phe->h_addr, phe->h_length);

	// �񓯊����[�h (�ڑ�)
	if (WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_CONNECT) == SOCKET_ERROR) {
		closesocket(sock);
		sock = INVALID_SOCKET;
		MessageBox(hWnd, "WSAAsyncSelect() failed",
			"Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	// �ڑ�����
	if (connect(sock, (LPSOCKADDR)&cl_sin, sizeof(cl_sin)) == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			MessageBox(hWnd, "connect() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
			return TRUE;
		}

	}
	return FALSE;
}

//  �ڑ��҂� (�T�[�o��)
BOOL SockAccept(HWND hWnd)
{
	SOCKADDR_IN sv_sin;         // SOCKADDR_IN�\����

	// �T�[�o�p�\�P�b�g
	sv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sv_sock == INVALID_SOCKET) { // �\�P�b�g�쐬���s
		MessageBox(hWnd, "Socket() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	memset(&sv_sin, 0x00, sizeof(sv_sin));      // �\���̏�����
	sv_sin.sin_family = AF_INET;           // �C���^�[�l�b�g
	sv_sin.sin_port = htons(PORT);       // �|�[�g�ԍ��w��
	sv_sin.sin_addr.s_addr = htonl(INADDR_ANY); // �A�h���X�w��

	if (bind(sv_sock, (LPSOCKADDR)&sv_sin, sizeof(sv_sin)) == SOCKET_ERROR) {
		closesocket(sv_sock);
		sv_sock = INVALID_SOCKET;
		MessageBox(hWnd, "bind() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	if (listen(sv_sock, 5) == SOCKET_ERROR) {
		// �ڑ��҂����s
		closesocket(sv_sock);
		sv_sock = INVALID_SOCKET;
		MessageBox(hWnd, "listen() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	// �񓯊��������[�h (�ڑ��҂�)
	if (WSAAsyncSelect(sv_sock, hWnd, WM_SOCKET, FD_ACCEPT) == SOCKET_ERROR) {
		closesocket(sv_sock);
		sv_sock = INVALID_SOCKET;
		MessageBox(hWnd, "WSAAsyncSelect() failed",
			"Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}
	return FALSE;
}


//  �`��֐�
LRESULT CALLBACK OnPaint(HWND hWnd, UINT uMsg, WPARAM wP, LPARAM lP)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hWnd, &ps);
	// �`��̈�̏�����
	MoveToEx(hdc, d.left, d.top, NULL);
	LineTo(hdc, d.right, d.top);    // �㉡��
	LineTo(hdc, d.right, d.bottom); // �E�c��
	LineTo(hdc, d.left, d.bottom);  // ������
	LineTo(hdc, d.left, d.top); // ���c��

	for (int i = 0; i < n; i++) {    // ����`��
		SelectObject(hdc, colors[i]);
		if (flag[i] == 0) {      // �J�n�_�Ȃ�A�n�_���ړ�
			MoveToEx(hdc, pos[i].x, pos[i].y, NULL);
		}
		else {             // �r���̓_�Ȃ��������
			LineTo(hdc, pos[i].x, pos[i].y);
		}
	}

	EndPaint(hWnd, &ps);

	return 0L;
}

//  �`������i�[
void setData(int f, int x, int y, HPEN color)
{
	flag[n] = f;
	pos[n].x = x;
	pos[n].y = y;
	colors[n] = color;
}

//  �}�E�X�̈ʒu���L�����p�X�̒����ǂ������肷��
BOOL checkMousePos(int x, int y)
{
	if (x >= d.left && x <= d.right
		&& y >= d.top && y <= d.bottom) {
		return TRUE;
	}
	return FALSE;
}

// �{�^���y�ѕ`��\���̐ݒ�
void enable(BOOL Host, BOOL Connect, BOOL Accept, BOOL Reject, BOOL RejectOrder, int Drow,
	BOOL Giveup, BOOL Correct, BOOL InCorrect, BOOL Consent, BOOL Pointout, BOOL Denial, BOOL Send, BOOL Change) {
	EnableWindow(hWndHost, Host);				// [HostName]
	EnableWindow(hWndConnect, Connect);			// [�ڑ�]    
	EnableWindow(hWndAccept, Accept);			// [�ڑ��҂�]
	EnableWindow(hWndReject, Reject);			// [�ؒf]    
	EnableWindow(hWndRejectOrder, RejectOrder);	// [�ؒf�v��]
	DrowableFlag = Drow;
	EnableWindow(hWndGiveup, Giveup);			// [�M�u�A�b�v]
	EnableWindow(hWndCorrect, Correct);			// [�����{�^��]    
	EnableWindow(hWndIncorrect, InCorrect);		// [�s�����{�^��]
	EnableWindow(hWndConsent, Consent);			// [����]    
	EnableWindow(hWndPointout, Pointout);			// [�w�E]    
	EnableWindow(hWndDenial, Denial);			// [�۔F]   
	EnableWindow(hWndSend, Send);				// [���M]  
	EnableWindow(hWndChange, Change);			// [���]
}

int ctoi(char a) {
	return a;
}

// �f�[�^�𑗐M����
void FXY(int f3, int x3, int y3) {
	char buf[9];                  // ���M���e���ꎞ�I�Ɋi�[����o�b�t�@

	sprintf_s(buf, "%1d%03d%03d", f3, x3, y3);
	send(sock, buf, strlen(buf) + 1, 0);
}

int randAtoC() {
	int A2C[] = { 0,1,2 };
	return A2C[rand() % 3];
}

void rand0toi(int ary[], int size) {
	for (int i = 0; i < size; i++) {
		int j = rand() % size;
		int t = ary[i];
		ary[i] = ary[j];
		ary[j] = t;
	}
}

int dice() {
	int DICE[] = { 0,1,2,3,4,5 };
	return DICE[rand() % 6];
}

//�`���b�g�{�b�N�X�̓��e������
void ChatReset(HWND chatbox) {
	SetWindowText(chatbox, TEXT(""));
	SetFocus(chatbox);
}

//��Ԍ��
void change(int a, int b) {

	char sender[100];
	char buf[100];

	EnableWindow(hWndSendMSG, TRUE);
	EnableWindow(hWndSend, TRUE);
	rule_num = randAtoC();
	ChatReset(hWndSendMSG);
	ChatReset(hWndRecvMSG);
	ChatReset(hWndQuestion);

	if (FlagPlayer % 2 == 1) {
		score_PL1 += a;
		score_PL2 += b;
		sprintf_s(buf, "%d", score_PL1);
		SetWindowText(hWndScore_pl1, buf);
		sprintf_s(buf, "%d", score_PL2);
		SetWindowText(hWndScore_pl2, buf);

		enable(FALSE, FALSE, FALSE, TRUE, TRUE, 1,
			FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE);
		rule_num = randAtoC();
		sprintf_s(sender, "%s%d", "RULE", rule_num);
		send(sock, sender, strlen(sender) + 1, 0);
		SetWindowText(hWndRule, rule[rule_num]);
		SetWindowText(hWndQuestion, card_text[card[turn]][dice()]);

	}
	else {
		score_PL1 += b;
		score_PL2 += a;
		sprintf_s(buf, "%d", score_PL1);
		SetWindowText(hWndScore_pl1, buf);
		sprintf_s(buf, "%d", score_PL2);
		SetWindowText(hWndScore_pl2, buf);

		enable(FALSE, FALSE, FALSE, TRUE, TRUE, 1,
			TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE);

	}
	FlagPlayer++;
	turn++;
}

//�`��̈�̓��e�����Z�b�g
void PictReset() {
	int i;
	for (i = 0; i > MAX_ARRAY; i++) {
		flag[i] = 0;
		pos[i].x = 0;
		pos[i].y = 0;
	}
}

//�����S��
void Eraser() {

}