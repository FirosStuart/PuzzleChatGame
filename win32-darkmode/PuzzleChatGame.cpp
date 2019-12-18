//  �w�b�_�t�@�C��
#include "win32-darkmode.h"
#include <Windows.h>
#include <WinSock.h>
#include <string>
#include <stdio.h>


// �g�p���C�u����
#pragma comment(lib,"wsock32.lib")


HINSTANCE g_hInst;
HWND g_hWndListView;

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
#define IDB_CLEAR       1012            // [�N���A]�{�^��


#define IDF_HOSTNAME    2000            // �z�X�g�����̓G�f�B�b�g�{�b�N�X
#define IDF_QUESTION    2001            // ����\���p�G�f�B�b�g�{�b�N�X
#define IDF_RULE        2002            // ��������p�G�f�B�b�g�{�b�N�X
#define IDF_SENDMSG     2003            // ���M���b�Z�[�W���͗p�G�f�B�b�g�{�b�N�X
#define IDF_RECVMSG     2004            // ��M���b�Z�[�W�\���p�G�f�B�b�g�{�b�N�X
#define IDF_SCORE_PL1   2005            // �����̓��_�p�G�f�B�b�g�{�b�N�X
#define IDF_SCORE_PL2   2006            // ����̓��_�p�G�f�B�b�g�{�b�N�X
#define IDF_HELP        2007            // �w���v���b�Z�[�W�p�G�f�B�b�g�{�b�N�X

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


static HWND hWndHost;                       // �z�X�g�����͗p�G�f�B�b�g�{�b�N�X
static HWND hWndQuestion;                   // ����\���p�G�f�B�b�g�{�b�N�X
static HWND hWndRule;                       // ��������p�G�f�B�b�g�{�b�N�X
static HWND hWndSendMSG;                    // ���M���b�Z�[�W���͗p�G�f�B�b�g�{�b�N�X
static HWND hWndRecvMSG;                    // ��M���b�Z�[�W�\���p�G�f�B�b�g�{�b�N�X
static HWND hWndScore_pl1;                  // �����̓��_�p�G�f�B�b�g�{�b�N�X
static HWND hWndScore_pl2;                  // ����̓��_�p�G�f�B�b�g�{�b�N�X
static HWND hWndHelp;                       // �w���v���b�Z�[�W�p�G�f�B�b�g�{�b�N�X
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
static HWND hWndClear;                       // �z�X�g�����͗p�G�f�B�b�g�{�b�N�X

static int DrawableFlag = 0;	// ���� 1:�\/0:�s�� ��Ԋm�F�t���O

int FlagPlayer;					//���݂�PL���e�ł��邩�q�ł��邩���m�F����t���O
int Use_PL = 0;					//PL�i���o�[���m�F����
int score_PL1 = 0;				//PL1�̃X�R�A
int score_PL2 = 0;				//PL2�̃X�R�A
int turn = 0;					//���
int dice_num;					//�_�C�X�̌���
int rule_num;					//���[���̔ԍ�

const RECT d = { 10, 200, 450, 700 };               // �`��̈�(�������x���W, �������y���W, �E������x���W, �E������y���W)

int n;                                              // �J�E���^
int flag[MAX_ARRAY];                                // �y���_�E���t���O
POINT pos[MAX_ARRAY];                               // ���W���i�[
HPEN colors[MAX_ARRAY];								// �g�p����F

char rule[3][10] = { "�G�ƕ���","�����̂�","�G�̂�" };//���[���̓��e
int card[10] = { 0,1,2,3,4,5,6,7,8,9 };				//�J�[�h����
char card_text[10][6][30] =							//����̏����ꂽ�J�[�h
{
	{"�L","�l","��","�F","��","�n"},
	{"�e���r","����@","�G�A�R��","�①��","�d�q�����W","������"},
	{"�N���X�}�X","�o�����^�C��","������","�n���E�B��","�čՂ�","���["},
	{"�T�b�J�[�{�[��","�싅�{�[��","�o���[�{�[��","�o�X�P�{�[��","�싅�{�[��","�e�j�X�{�[��"},
	{"�L���r���A�e���_���g","�i�[�X","��Ј�","�w��","�x�@��","�o�[�e���_�["},
	{"�s�U","�n���o�[�K�[","�R�[��","�|�b�v�R�[��","�t���C�h�`�L��","�t���C�h�|�e�g"},
	{"�P�[�L", "�p�t�F", "�v����", "�A�C�X�N���[��", "�h�[�i�c", "�N���[�v"},
	{"���","�X�J�[�g","���{��","�C��","�R�[�g","�X�[�c"},
	{"�����Y","�����Y","������P","���������R","�Y�����Y","�߂̉��Ԃ�"},
	{"�A�����J","���V�A","�h�C�c","����","���{","�C�M���X"}

};

//  �v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);//�E�B���h�E�֐�
LRESULT CALLBACK OnPaint(HWND, UINT, WPARAM, LPARAM);	//�`��֐�
LRESULT CALLBACK ClearPaint(HWND);//�`�惊�Z�b�g�֐�
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL checkMousePos(int x, int y);						//�}�E�X�̈ʒu���L�����p�X�̒����ǂ������肷��
void setData(int flag, int x, int y, HPEN color);       //�`���������
void FXY(int f, int x, int y);							//�`��f�[�^�𑗐M����֐�
void ChatReset(HWND chatbox);							//�`���b�g�{�b�N�X�����Z�b�g����֐�

int randAtoC();											//1�`3�̃����_���Ȑ�����Ԃ�
void rand0toi(int ary[], int size);						//i�̔z��̏��Ԃ������_���ɓ���ւ���
int dice();												//1�`6�̃����_���Ȑ�����Ԃ�

void game_start(HWND hWnd);								//�Q�[���J�n���Ɏ��s����֐�
int change(HWND, int a, int b);							//��Ԃ���シ��Ƃ��Ɏ��s����֐�
int score_judge(HWND hWnd);								//����������s���֐�

BOOL SockInit(HWND hWnd);                               // �\�P�b�g������
BOOL SockAccept(HWND hWnd);                             // �\�P�b�g�ڑ��҂�
BOOL SockConnect(HWND hWnd, LPCSTR host);               // �\�P�b�g�ڑ�


void enable_master();			//�e�̏������	
void enable_player();			//�q�̏������
void enable_end();				//�Q�[���I�����̏��
void enable_correct();			//�����������̎q�̏��
void enable_pause();			//�����{�^������������̐e�̏��
void enable_pointout_master();	//�w�E�����ꂽ��̐e�̏��
void enable_pointout_player();	//�w�E��������̎q�̏��
void enable_wait();				//�ڑ��҂��̎��̏��
void enable_standby();			//�ڑ�����O�̏��


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	constexpr COLORREF darkBkColor = 0x383838;
	constexpr COLORREF darkTextColor = 0xFFFFFF;
	static HBRUSH hbrBkgnd = nullptr;
	switch (message)
	{
	case WM_INITDIALOG:
	{
		if (g_darkModeSupported)
		{
			SetWindowTheme(GetDlgItem(hDlg, IDOK), L"Explorer", nullptr);
			SendMessageW(hDlg, WM_THEMECHANGED, 0, 0);
		}
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSTATIC:
	{
		if (g_darkModeSupported && g_darkModeEnabled)
		{
			HDC hdc = reinterpret_cast<HDC>(wParam);
			SetTextColor(hdc, darkTextColor);
			SetBkColor(hdc, darkBkColor);
			if (!hbrBkgnd)
				hbrBkgnd = CreateSolidBrush(darkBkColor);
			return reinterpret_cast<INT_PTR>(hbrBkgnd);
		}
	}
	break;
	case WM_DESTROY:
		if (hbrBkgnd)
		{
			DeleteObject(hbrBkgnd);
			hbrBkgnd = nullptr;
		}
		break;
	case WM_SETTINGCHANGE:
	{
		if (g_darkModeSupported && IsColorSchemeChangeMessage(lParam))
			SendMessageW(hDlg, WM_THEMECHANGED, 0, 0);
	}
	break;
	case WM_THEMECHANGED:
	{
		if (g_darkModeSupported)
		{
			_AllowDarkModeForWindow(hDlg, g_darkModeEnabled);
			RefreshTitleBarThemeColor(hDlg);

			HWND hButton = GetDlgItem(hDlg, IDOK);
			_AllowDarkModeForWindow(hButton, g_darkModeEnabled);
			SendMessageW(hButton, WM_THEMECHANGED, 0, 0);

			UpdateWindow(hDlg);
		}
	}
	break;
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wP, LPARAM lP)
{
	static BOOL mouseFlg = FALSE;       //�O��̏�� TRUE:�`�悵���AFALSE:�`�悵�Ă��Ȃ�
	SOCKADDR_IN cl_sin;					//�C���^�[�l�b�g�p�\�P�b�g�A�h���X
	int flag2, x2, y2;					//�󂯎�������W�A�t���O���i�[����
	int len = sizeof(cl_sin);			//�C���^�[�l�b�g�p�\�P�b�g�A�h���X�̒���
	char host[100];						//�z�X�g��
	char buf[100];						//��M���e���ꎞ�I�Ɋi�[����o�b�t�@
	int score_Master = 0;				//�e�̃X�R�A	
	int score_Player = 0;				//�q�̃X�R�A
	static const COLORREF newSysColor[] = { RGB(255,255,255)};
	static const int setSysColor[] = { COLOR_ACTIVECAPTION };
	static COLORREF defSysColor[1];
	switch (uMsg)
	{
	case WM_CREATE: // �E�B���h�E���������ꂽ
	{
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
			460, 250, 200, 18, hWnd, NULL, NULL, NULL);
		CreateWindow(TEXT("static"), TEXT("Receive Message"), WS_CHILD | WS_VISIBLE,
			460, 420, 200, 18, hWnd, NULL, NULL, NULL);
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
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 800, 560, 90, 25,
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
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 660, 30, 70, 25,
			hWnd, (HMENU)IDB_CHANGE, NULL, NULL);
		// [�N���A]�{�^��
		hWndClear = CreateWindow("button", "�N���A",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 660, 70, 70, 25,
			hWnd, (HMENU)IDB_CLEAR, NULL, NULL);
		// ����p�G�f�B�b�g�{�b�N�X
		hWndQuestion = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_READONLY, 10, 150, 200, 25,
			hWnd, (HMENU)IDF_QUESTION, NULL, NULL);
		// ��������p�G�f�B�b�g�{�b�N�X
		hWndRule = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_READONLY, 460, 200, 300, 50,
			hWnd, (HMENU)IDF_RULE, NULL, NULL);
		// ���M���b�Z�[�W���͗p�G�f�B�b�g�{�b�N�X
		hWndSendMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_DISABLED, 460, 270, 300, 150,
			hWnd, (HMENU)IDF_SENDMSG, NULL, NULL);
		// ��M���b�Z�[�W�\���p�G�f�B�b�g�{�b�N�X
		hWndRecvMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 460, 440, 300, 150,
			hWnd, (HMENU)IDF_RECVMSG, NULL, NULL);
		// �w���v���b�Z�[�W�p�G�f�B�b�g�{�b�N�X
		hWndHelp = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 460, 600, 300, 100,
			hWnd, (HMENU)IDF_HELP, NULL, NULL);
		// �����̓��_�p�G�f�B�b�g�{�b�N�X
		hWndScore_pl1 = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_READONLY, 720, 100, 25, 25,
			hWnd, (HMENU)IDF_SCORE_PL1, NULL, NULL);
		// ����̓��_�p�G�f�B�b�g�{�b�N�X
		hWndScore_pl2 = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_READONLY, 845, 100, 25, 25,
			hWnd, (HMENU)IDF_SCORE_PL2, NULL, NULL);

		SockInit(hWnd);         // �\�P�b�g������
		hPenBlack = (HPEN)CreatePen(PS_SOLID, 3, RGB(0, 0, 0));		//���y���쐬
		hPenRed = (HPEN)CreatePen(PS_SOLID, 3, RGB(255, 0, 0));		//�ԃy���쐬
		//defSysColor[0] = GetSysColor(setSysColor[0]);
		SetSysColors(1, setSysColor, newSysColor);

		if (g_darkModeSupported)
		{
			_AllowDarkModeForWindow(hWnd, true);
			RefreshTitleBarThemeColor(hWnd);
		}

		return 0L;
	}

	case WM_COMMAND: // �{�^���������ꂽ�ꍇ
	{
		
		// Parse the menu selections:
		switch (LOWORD(wP))
		{
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		// System menu

		case IDB_ACCEPT:			// [�ڑ��҂�]�{�^���������ꍇ(�T�[�o�[)
			if (SockAccept(hWnd)) { // �ڑ��҂��v��
				return 0L;			// �ڑ��҂����s

			}
			enable_wait();			//�ڑ��҂��̏�Ԃɂ���
			return 0L;

		case IDB_CONNECT:					// [�ڑ�]�{�^���������ꍇ(�N���C�A���g)
			GetWindowText(hWndHost, host, sizeof(host));
			if (SockConnect(hWnd, host)) {  // �ڑ��v��
				return 0L;

			}
			enable_wait();					//�ڑ��҂��̏�Ԃɂ���
			return 0L;

		case IDB_REJECT:					// [�ؒf]�{�^���������ꍇ

			if (sock != INVALID_SOCKET) {   // �������N���C�A���g���Ȃ�
				closesocket(sock);			// �\�P�b�g�����
				sock = INVALID_SOCKET;

			}
			if (sv_sock != INVALID_SOCKET) { // �������T�[�o���Ȃ�
				closesocket(sv_sock);		 // �T�[�o�p�\�P�b�g�����
				sv_sock = INVALID_SOCKET;

			}
			phe = NULL;
			enable_standby();
			return 0L;

		case IDB_REJECTORDER:						// [�ؒf�v��]�{�^���������ꍇ
			strcpy_s(buf, "REJECT");				// �ؒf�̓��e���o�b�t�@�ɕۑ�
			send(sock, buf, strlen(buf) + 1, 0);	// REJECT�Ƒ��M
			return 0L;

		case IDB_SEND:											// [���M]�{�^���������ꍇ
			char sender[100];

			GetWindowText(hWndSendMSG, buf, sizeof(buf) - 1);   //���M���b�Z�[�W���͗��̓��e���擾
			sprintf_s(sender, "%s%s", "MESS", buf);				//���M�̓��e�Ƃ��̑O��MESS��t�������̂��o�b�t�@�ɕۑ�
																// ���M����
			if (send(sock, sender, strlen(sender) + 1, 0) == SOCKET_ERROR) {
				// ���M�Ɏ��s������G���[��\��
				MessageBox(hWnd, TEXT("sending failed"), TEXT("Error"),
					MB_OK | MB_ICONEXCLAMATION);

			}
			ChatReset(hWndSendMSG);								//�`���b�g�{�b�N�X�̃N���A
			return 0L;

		case IDB_GIVEUP:									// [�M�u�A�b�v]�{�^���������ꍇ
			strcpy_s(buf, "GIVEUP");						// �M�u�A�b�v�̓��e���o�b�t�@�ɕۑ�
			send(sock, buf, strlen(buf) + 1, 0);			// GIVEUP�Ƒ��M
			score_Master = 1;								// �e�Ǝq�̓_�����P�_���t�^
			score_Player = 1;
			MessageBox(hWnd, "�M�u�A�b�v���܂����B",
				"Information", MB_OK | MB_ICONINFORMATION);	//�M�u�A�b�v���܂����ƃ��b�Z�[�W�{�b�N�X�ɕ\������
			change(hWnd, score_Master, score_Player);		// ���
			return 0L;

		case IDB_CORRECT:								// [����]�������ꍇ
			strcpy_s(buf, "CORRECT");					// �����̓��e���o�b�t�@�ɕۑ�
														//�hCORRECT�h�Ɓh�����F�h�A���݂̂���𑗐M����
			sprintf_s(sender, "%s%s%s", "CORRECT", "����:", card_text[card[turn - 1]][dice_num]);
			send(sock, sender, strlen(sender) + 1, 0);	//���M����
			enable_pause();
			SetWindowText(hWndHelp, "����̔��f��҂��Ă��܂��B");
			return 0L;

		case IDB_CLEAR:							// [�N���A]���������ꍇ
			strcpy_s(buf, "CLEAR");				// �N���A�̓��e���o�b�t�@�ɕۑ�
			send(sock, buf, strlen(buf) + 1, 0);//���M����
			ClearPaint(hWnd);		//��ʂ̃N���A�����s
			InvalidateRect(hWnd, &d, TRUE);		//�\���I�ɍĕ`�揈�������s
			return 0L;

		case IDB_INCORRECT:						// [�s����]���������ꍇ
			strcpy_s(buf, "INCORRECT");			// �s�����̓��e���o�b�t�@�ɕۑ�
			send(sock, buf, strlen(buf) + 1, 0);// INCORRECT�Ƒ��M
			return 0L;

		case IDB_POINTOUT:						// [�w�E]���������ꍇ
			strcpy_s(buf, "POINTOUT");			// �w�E�̓��e���o�b�t�@�ɕۑ�
			send(sock, buf, strlen(buf) + 1, 0);// POINTOUT�Ƒ��M	
			enable_pointout_player();
			SetWindowText(hWndHelp, "����̔��f��҂��Ă��܂��B");
			return 0L;

		case IDB_CONSENT:						// [����]���������ꍇ
			strcpy_s(buf, "CONSENT");           // �����̓��e���o�b�t�@�ɕۑ�
			send(sock, buf, strlen(buf) + 1, 0);// CONSENT�Ƒ��M
			score_Master = 0;                   // �e��0�_�A�q��1�_��t�^
			score_Player = 1;
			// ���
			change(hWnd, score_Master, score_Player);
			return 0L;

		case IDB_DENIAL:						// [�۔F]���������ꍇ
			strcpy_s(buf, "DENIAL");			// �۔F�̓��e���o�b�t�@�ɕۑ�
			send(sock, buf, strlen(buf) + 1, 0);// DENIAL�Ƒ��M

			if (rule_num == 0) {				//A�𐳉�
				score_Master = 1;				//�e��1�_�A�q��2�_��t�^
				score_Player = 2;

			}
			else if (rule_num == 1) {			//B�𐳉�
				score_Master = 1;				//�e��1�_�A�q��3�_��t�^
				score_Player = 3;

			}
			else if (rule_num == 2) {			//C�𐳉�
				score_Master = 1;				//�e��1�_�A�q��5�_��t�^
				score_Player = 5;

			}									// ���
			change(hWnd, score_Master, score_Player);
			return 0L;

		case IDB_CHANGE:						// [���]���������ꍇ
			strcpy_s(buf, "CHANGE");            // ���̓��e���o�b�t�@�ɕۑ�
			send(sock, buf, strlen(buf) + 1, 0);// CHANGE�Ƒ��M

			if (rule_num == 0) {				//A�𐳉�
				score_Master = 1;				//�e��1�_�A�q��2�_��t�^
				score_Player = 2;

			}
			else if (rule_num == 1) {			//B�𐳉�
				score_Master = 1;				//�e��1�_�A�q��3�_��t�^
				score_Player = 3;

			}
			else if (rule_num == 2) {			//C�𐳉�
				score_Master = 1;				//�e��1�_�A�q��5�_��t�^
				score_Player = 5;

			}									// ���
			change(hWnd, score_Master, score_Player);
			return 0L;
		default:
			return DefWindowProcW(hWnd, uMsg, wP, lP);
		} /* end of switch (LOWORD(wP)) */
		return 0L;
	}
	break;
	case WM_SIZE:
	{
		int clientWidth = GET_X_LPARAM(lP), clientHeight = GET_Y_LPARAM(lP);
		HDWP hDWP = BeginDeferWindowPos(1);
		if (hDWP != nullptr)
		{
			DeferWindowPos(hDWP, g_hWndListView, 0, 0, 0, clientWidth, clientHeight, SWP_NOZORDER);
			EndDeferWindowPos(hDWP);
		}
	}
	break;
	case WM_SETTINGCHANGE:
	{
		if (IsColorSchemeChangeMessage(lP))
		{
			g_darkModeEnabled = _ShouldAppsUseDarkMode() && !IsHighContrast();

			RefreshTitleBarThemeColor(hWnd);
			SendMessageW(g_hWndListView, WM_THEMECHANGED, 0, 0);
		}
	}
	break;
	case WM_LBUTTONDOWN:    // �}�E�X���{�^���������ꂽ
															// �`��̈�̒��ŕ`��\�t���O���P�ł���Ȃ��
		if ((checkMousePos(LOWORD(lP), HIWORD(lP)) == TRUE) && (DrawableFlag == 1)) {
			setData(0, LOWORD(lP), HIWORD(lP), hPenBlack);  // ���̎n�_�Ƃ��č��W���L�^
			FXY(0, LOWORD(lP), HIWORD(lP));                 // ���̎n�_�Ƃ��č��W���L�^
			mouseFlg = TRUE;								//mouseFlag��TRUE�Ƃ���
			n++;											//�J�E���^�𑝉�����
			InvalidateRect(hWnd, &d, FALSE);				//�\���I�ɍĕ`�悷��

		}
		return 0L;

	case WM_MOUSEMOVE:  // �}�E�X�|�C���^���ړ�����

		if (wP == MK_LBUTTON) {  // ���{�^����������Ă���
																	// �`��̈�̒��Ȃ�
			if ((checkMousePos(LOWORD(lP), HIWORD(lP)) == TRUE) && (DrawableFlag == 1)) {

				if (mouseFlg) {										// �O��`�悵�Ă���Ȃ�
					setData(1, LOWORD(lP), HIWORD(lP), hPenBlack);   // ���̓r���Ƃ��č��W���L�^
					FXY(1, LOWORD(lP), HIWORD(lP));					// ���̓r���Ƃ��č��W�𑗐M

				}
				else {												// �O��`�悵�Ă��Ȃ��Ȃ�
					setData(0, LOWORD(lP), HIWORD(lP), hPenBlack);  // ���̎n�_�Ƃ��č��W���L�^
					FXY(0, LOWORD(lP), HIWORD(lP));					// ���̎n�_�Ƃ��č��W�𑗐M

				}
				n++;												//�J�E���^�𑝉�
				mouseFlg = TRUE;									//mouseFlag��TRUE�Ƃ���
				InvalidateRect(hWnd, &d, FALSE);					//�\���I�ɍĕ`�������

			}
			else {													// �`��̈�̊O�Ȃ�
				mouseFlg = FALSE;									//mouseFlag��FALSE�Ƃ���

			}

		}
		return 0L;

	case WM_SOCKET:          // �񓯊��������b�Z�[�W

		if (WSAGETSELECTERROR(lP) != 0) { return 0L; }
		switch (WSAGETSELECTEVENT(lP)) {
		case FD_ACCEPT:						// �ڑ��҂������ʒm
		{									//�ڑ��v���̊m�F
			sock = accept(sv_sock, (LPSOCKADDR)&cl_sin, &len);

			if (sock == INVALID_SOCKET) {	//�N���C�A���g���̃\�P�b�g��INVALID_SOCKET�������Ă����Ȃ��
											//�G���[���b�Z�[�W��\������
				MessageBox(hWnd, "Accepting connection failed",
					"Error", MB_OK | MB_ICONEXCLAMATION);
				closesocket(sv_sock);		//�T�[�o�p�̃\�P�b�g�����
				sv_sock = INVALID_SOCKET;	//�T�[�o�p�̃\�P�b�g��INVALID_SOCKET�Ƃ���
				enable_standby();			//�A�v���P�[�V�����J�n���̏�Ԃɂ���
				return 0L;

			}
#ifndef NO_DNS
			// �z�X�g���擾
			phe = gethostbyaddr((char*)&cl_sin.sin_addr, 4, AF_INET);
			if (phe) { SetWindowText(hWndHost, phe->h_name); }
#endif  NO_DNS
			// �񓯊����[�h (��M���ؒf�j
			if (WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE) == SOCKET_ERROR) {
				// �ڑ��Ɏ��s�����珉����Ԃɖ߂�
				//�G���[���b�Z�[�W��\������
				MessageBox(hWnd, "WSAAsyncSelect() failed",
					"Error", MB_OK | MB_ICONEXCLAMATION);
				enable_standby();			//�A�v���P�[�V�����J�n���̏�Ԃɂ���
				return 0L;

			}

			Use_PL = 1;						//PL�P�Ƃ���
			FlagPlayer = 1;					//�e�Ƃ��ăQ�[�����J�n
			game_start(hWnd);				//�e��0�_�q��0�_�Ƃ��ăQ�[�����J�n����
			return 0L;

		}/* end of case FD_ACCEPT: */

		case FD_CONNECT:					// �ڑ������ʒm
											// �񓯊����[�h (��M���ؒf)
			if (WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE) == SOCKET_ERROR) {
				// �ڑ��Ɏ��s�����珉����Ԃɖ߂�
				//�G���[���b�Z�[�W��\������
				MessageBox(hWnd, "WSAAsyncSelect() failed",
					"Error", MB_OK | MB_ICONEXCLAMATION);
				enable_standby();			//�A�v���P�[�V�����J�n���̏�Ԃɂ���
				return 0L;
			}

			Use_PL = 0;						//PL�Q�Ƃ���
			game_start(hWnd);				//�ʐM�e0�_�A�q0�_�Ƃ��ăQ�[�����J�n����
			return 0L;

		case FD_READ:										//���b�Z�[�W��M
			char b[100];									//buf�̑���ɗp����

			if (recv(sock, buf, sizeof(buf) - 1, 0) != SOCKET_ERROR) {
				// ��M�ł����Ȃ�			
				if (strncmp(buf, "MESS", 4) == 0) {			//�`���b�g�{�b�N�X�̓��e��
															//����ꂽ�ꍇ
					strncpy_s(b, buf + 4, sizeof(buf) - 4);	//4�����ڈȍ~�̓��e��ϐ�b�Ɋi�[
					SetWindowText(hWndRecvMSG, b);			//b�̓��e�𑗐M
					return 0L;

				}
				else if (strncmp(buf, "RULE", 4) == 0) {	//���[���������Ă����ꍇ
					strncpy_s(b, buf + 4, sizeof(buf) - 4);	//4�����ڈȍ~�̓��e��ϐ�b�Ɋi�[
					rule_num = atoi(b);						//b����int�^�ɕς������̂�rule_num�Ɋi�[
					SetWindowText(hWndRule, rule[rule_num]);//rule_num�ɑΉ��������[����񎦂���
					return 0L;

				}
				else if (strcmp(buf, "REJECT") == 0) {		//REJECT�������Ă����ꍇ
														   //�ؒf�v���������ƃ��b�Z�[�W�{�b�N�X�ɕ\������
					MessageBox(hWnd, "�ؒf�v�������܂����B",
						"Information", MB_OK | MB_ICONINFORMATION);
					return 0L;

				}
				else if (strncmp(buf, "CORRECT", 7) == 0) {//�����������ꍇ
					strncpy_s(b, buf + 7, sizeof(buf) - 7);	//7�����ڈȍ~�̓��e��ϐ�b�Ɋi�[
					SetWindowText(hWndRecvMSG, b);			//���̓��e����M�{�b�N�X�ɓ\��
					MessageBox(hWnd, "�����ł��I",			//���b�Z�[�W�{�b�N�X�ɐ����ł��邱�Ƃ�\������
						"Information", MB_OK | MB_ICONINFORMATION);
					enable_correct();						//�������̏�Ԃɂ���
					SetWindowText(hWndHelp, "���肪�ᔽ�s�ׂ����Ă����Ɗ�������w�E�������Ă��������B\r\n���Ȃ��ꍇ�͌��������Ă��������B");
					return 0L;

				}
				else if (strcmp(buf, "POINTOUT") == 0) {	//�w�E�������ꍇ
					MessageBox(hWnd, "�w�E�_������܂��I",	//���b�Z�[�W�{�b�N�X�Ɏw�E������|��\��
						"Information", MB_OK | MB_ICONINFORMATION);
					enable_pointout_master();				//�w�E���������̏�Ԃɂ���
					SetWindowText(hWndHelp, "�w�E��F�߂�ꍇ�͏��F�������Ă��������B\r\n�F�߂Ȃ��ꍇ�͔۔F�������Ă��������B");
					return 0L;

				}
				else if (strcmp(buf, "CLEAR") == 0) {		//�`��{�b�N�X���N���A���邱�Ƃ����߂�ꂽ�ꍇ
					ClearPaint(hWnd);			//�`��{�b�N�X���N���A����
					InvalidateRect(hWnd, &d, TRUE);			//�\���I�ɍĕ`�悷��
					return 0L;

				}
				else if (strcmp(buf, "CHANGE") == 0) {		//��シ��ꍇ
					if (rule_num == 0) {					//A�𐳉�
						score_Master = 1;					//�e��1�_�A�q��2�_��t�^
						score_Player = 2;

					}
					else if (rule_num == 1) {				//B�𐳉�
						score_Master = 1;					//�e��1�_�A�q��3�_��t�^
						score_Player = 3;

					}
					else if (rule_num == 2) {				//C�𐳉�
						score_Master = 1;					//�e��1�_�A�q��5�_��t�^
						score_Player = 5;

					}										//��シ��

					change(hWnd, score_Master, score_Player);
					return 0L;

				}
				else if (strcmp(buf, "DENIAL") == 0) {		//�w�E��ے肵���ꍇ

					if (rule_num == 0) {					//A�𐳉�
						score_Master = 1;					//�e��1�_�A�q��2�_��t�^
						score_Player = 2;

					}
					else if (rule_num == 1) {				//B�𐳉�
						score_Master = 1;					//�e��1�_�A�q��3�_��t�^
						score_Player = 3;

					}
					else if (rule_num == 2) {				//C�𐳉�
						score_Master = 1;					//�e��1�_�A�q��5�_��t�^
						score_Player = 5;

					}

					change(hWnd, score_Master, score_Player);//��シ��
					return 0L;

				}
				else if (strcmp(buf, "CONSENT") == 0) {	//�w�E�����F
					score_Master = 0;						//�e��0�_�A�q��1�_��t�^
					score_Player = 1;
					change(hWnd, score_Master, score_Player);//��シ��
					return 0L;

				}
				else if (strcmp(buf, "GIVEUP") == 0) {		//�M�u�A�b�v
					score_Master = 1;						//�e��1�_�A�q��1�_��t�^
					score_Player = 1;
					//���肪�M�u�A�b�v�����|�����b�Z�[�W�{�b�N�X�ɕ\��
					MessageBox(hWnd, "���肪�M�u�A�b�v���܂���",
						"Information", MB_OK | MB_ICONINFORMATION);
					change(hWnd, score_Master, score_Player);//��シ��
					return 0L;

				}
				else if (strcmp(buf, "POINTOUT") == 0) {	//�w�E�������ꍇ
					MessageBox(hWnd, "�w�E�_������܂��I",	//�w�E�������|�����b�Z�[�W�{�b�N�X�ɕ\��
						"Information", MB_OK | MB_ICONINFORMATION);
					return 0L;

				}
				else if (strcmp(buf, "INCORRECT") == 0) {	//�s�����ł���Ȃ��
					strncpy_s(buf, "�s�����ł�", 10);		//�s�����ł���|����M�`���b�g�{�b�N�X�ɕ\��
					SetWindowText(hWndRecvMSG, buf);
					return 0L;

				}
				else {										//����ȊO�Ȃ��
					sscanf_s(buf, "%1d%03d%03d", &flag2, &x2, &y2);
					//���W���ʂɊl������
					setData(flag2, x2, y2, hPenRed);		//�ԃy���ŕ`��ł���悤�Ƀf�[�^��ۑ�����
					n++;									//�J�E���^�𑝉�����
					InvalidateRect(hWnd, &d, FALSE);		//�\���I�ɍĕ`�悷��
					return 0L;

				}

			}

			return 0L;

		case FD_CLOSE:										// �ؒf���ꂽ�ꍇ
			MessageBox(hWnd, "�ؒf����܂����B",			//�ؒf���ꂽ�|�����b�Z�[�W�{�b�N�X�ɕ\��
				"Information", MB_OK | MB_ICONINFORMATION);
			SendMessage(hWnd, WM_COMMAND, IDB_REJECT, 0); // �ؒf�������s
			return 0L;

		}/* end of switch (WSAGETSELECTEVENT(lP)) */
		return 0L;

	case WM_DESTROY:				// �E�B���h�E���j�����ꂽ
		DeleteObject(hPenBlack);    // ���y���폜
		DeleteObject(hPenRed);      // �ԃy���폜
		SetSysColors(1, setSysColor, defSysColor);
		closesocket(sock);          // �\�P�b�g�����
		PostQuitMessage(0);         // �I������
		return 0L;

	case WM_PAINT:      // �ĕ`��
		return OnPaint(hWnd, uMsg, wP, lP);  // �`��֐��̌Ăяo��
		return 0L;

	default:
		return DefWindowProcW(hWnd, uMsg, wP, lP);
	}
	return 0;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	InitDarkMode();

	if (!g_darkModeSupported)
	{
		TaskDialog(nullptr, hInstance, L"Error", nullptr, L"Darkmode is not supported.", TDCBF_OK_BUTTON, TD_ERROR_ICON, nullptr);
	}

	WNDCLASSEXW wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_WIN32DARKMODE));
	wc.hCursor = LoadCursorW(nullptr, NULL);
	wc.hbrBackground = CreateSolidBrush(RGB(25, 25, 25));
	wc.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32DARKMODE);
	wc.lpszClassName = L"PuzzleChatGame";
	wc.hIconSm = LoadIconW(wc.hInstance, MAKEINTRESOURCEW(IDI_WIN32DARKMODE));

	RegisterClassExW(&wc);

	g_hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(
		L"PuzzleChatGame",
		L"PuzzleChatGame", 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_W,
		WINDOW_H,
		nullptr, 
		nullptr, 
		hInstance, 
		nullptr);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	// Main message loop:
	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return (int)msg.wParam;
}
LRESULT CALLBACK OnPaint(HWND hWnd, UINT uMsg, WPARAM wP, LPARAM lP)
{
	HDC hdc;							//�f�o�C�X�R���e�L�X�g
	PAINTSTRUCT ps;						//�y�C���g�X�g���N�g
	


	hdc = BeginPaint(hWnd, &ps);		//�`��̊J�n
										// �`��̈�̏�����
	SetDCBrushColor(hdc, RGB(255, 255, 241));
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	Rectangle(hdc, 10, 200, 450, 700);
	MoveToEx(hdc, d.left, d.top, NULL);
	LineTo(hdc, d.right, d.top);		// �㉡��
	LineTo(hdc, d.right, d.bottom);		// �E�c��
	LineTo(hdc, d.left, d.bottom);		// ������
	LineTo(hdc, d.left, d.top);			// ���c��


	for (int i = 0; i < n; i++) {		// ����`��
		SelectObject(hdc, colors[i]);
		if (flag[i] == 0) {				// �J�n�_�Ȃ�A�n�_���ړ�
			MoveToEx(hdc, pos[i].x, pos[i].y, NULL);

		}
		else {							// �r���̓_�Ȃ��������
			LineTo(hdc, pos[i].x, pos[i].y);

		}
	}

	EndPaint(hWnd, &ps);				//�`��I��

	return 0L;

}

LRESULT CALLBACK ClearPaint(HWND hWnd)
{
	HDC hdc;						//�f�o�C�X�R���e�L�X�g
	PAINTSTRUCT ps;					//�y�C���g�X�g���N�g

	hdc = BeginPaint(hWnd, &ps);	//�`��J�n
	// �`��̈�̏�����
	MoveToEx(hdc, d.left, d.top, NULL);
	LineTo(hdc, d.right, d.top);    // �㉡��
	LineTo(hdc, d.right, d.bottom); // �E�c��
	LineTo(hdc, d.left, d.bottom);  // ������
	LineTo(hdc, d.left, d.top);		// ���c��



	for (int i = 0; i < n; i++) {   // �S�Ă̏������Z�b�g����
		flag[i] = 0;
		pos[i].x = 0;
		pos[i].y = 0;
		colors[i] = NULL;

	}

	n = 0;							//�J�E���^�����Z�b�g����
	EndPaint(hWnd, &ps);			//�`��I��
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

// �f�[�^�𑗐M����
void FXY(int f3, int x3, int y3) {
	char buf[9];                  // ���M���e���ꎞ�I�Ɋi�[����o�b�t�@
	sprintf_s(buf, "%1d%03d%03d", f3, x3, y3);
	send(sock, buf, strlen(buf) + 1, 0);

}

//�P�`�R�̒l�������_���ɕԂ�
int randAtoC() {
	int A2C[] = { 0,1,2 };
	return A2C[rand() % 3];

}

//�����Ƃ���size�̔z��������_���Ȍ`�ɂ���
void rand0toi(int ary[], int size)
{
	for (int i = 0; i < size; i++) {
		int j = rand() % size;
		int t = ary[i];
		ary[i] = ary[j];
		ary[j] = t;

	}

}

//1�`6�̒l�������_���ɕԂ�
int dice() {
	int DICE[] = { 0,1,2,3,4,5 };
	return DICE[rand() % 6];

}

//�`���b�g�{�b�N�X�̓��e������
void ChatReset(HWND chatbox) {
	SetWindowText(chatbox, TEXT(""));
	SetFocus(chatbox);

}

//�_�����e�v���C���[�ɗ^����
void set_score(int x, int y) {

	char buf[100];

	score_PL1 += x;						//�e�̓_��a��PL1�ɕt�^
	score_PL2 += y;						//�q�̓_��b��PL2�ɕt�^
	sprintf_s(buf, "%d", score_PL1);    //PL1�̓_�����o�b�t�@�ɕۑ�
	SetWindowText(hWndScore_pl1, buf);  //PL1�̓_����\��
	sprintf_s(buf, "%d", score_PL2);	//PL2�̓_�����o�b�t�@�ɕۑ�
	SetWindowText(hWndScore_pl2, buf);	//PL2�̓_����\��

}

//�Q�[���J�n���Ɏ��s
void game_start(HWND hWnd) {

	turn = 0;				//��Ԃ�0�ɂ���
	rand0toi(card, 10);		//�J�[�h���Ԃ�ݒ肷��
	change(hWnd, 0, 0);		//�݂���0�_�Ƃ��Ď�Ԍ��̎菇�����s����

}



//��Ԍ��

int change(HWND hWnd, int a, int b) {

	char buf[100];

	ChatReset(hWndSendMSG);			//���M�`���b�g�{�b�N�X�����Z�b�g����
	ChatReset(hWndRecvMSG);			//��M�`���b�g�{�b�N�X�����Z�b�g����
	ChatReset(hWndQuestion);		//��������Z�b�g����
	ChatReset(hWndHelp);                    //�w���v���b�Z�[�W�{�b�N�X�����Z�b�g����
	ClearPaint(hWnd);				//�`��{�b�N�X���N���A����
	InvalidateRect(hWnd, &d, TRUE);	//�\���I�ɍĕ`�悷��

	if (FlagPlayer % 2 == 1) {      //�������e�̏ꍇ
		if (Use_PL == 1) {			//����PL1�Ȃ��	
			set_score(b, a);		//b��PL1�́Aa��PL2�̓��_�ɉ�����

		}
		else if (Use_PL == 0) {	//����PL2�Ȃ��
			set_score(a, b);		//a��PL1�́Ab��PL2�̓��_�ɉ�����

		}
		if (turn == 3) {			//���ꂪ�w��^�[�������}�����Ȃ��
			score_judge(hWnd);		//����������s���I������
			return 0;

		}

		rule_num = randAtoC();		//���[���ԍ������߂�
		dice_num = dice();			//�P�`�U�̐��������߂�

		enable_master();			//�e�̏�����Ԃɂ���
									//�hRULE�h�ƃ��[���ԍ�buf�Ɋi�[����
		sprintf_s(buf, "%s%d", "RULE", rule_num);
		//buf�𑗐M����
		send(sock, buf, strlen(buf) + 1, 0);
		//���̌�A���[���ԍ��ɑΉ��������[���A
		//�_�C�X�ɑΉ������J�[�h�ɏ����ꂽ�����\������
		SetWindowText(hWndRule, rule[rule_num]);
		SetWindowText(hWndQuestion, card_text[card[turn]][dice_num]);

		if (rule_num == 0) {				//���[���P�Ȃ�Ε����ƊG���g���邽��
			EnableWindow(hWndSendMSG, TRUE);//���M�p�`���b�g�{�b�N�X�A
			EnableWindow(hWndSend, TRUE);	//[���M]�{�^���A
			EnableWindow(hWndClear, TRUE);	//[�N���A]�{�^����L���ɂ���
			DrawableFlag = 1;				//�`��\�t���O���P�ɂ���
			SetWindowText(hWndHelp, "�G�ƕ����ł����\�����Ă��������B\r\n�G�����������ꍇ�̓N���A�������Ă��������B");

		}
		else if (rule_num == 1) {
			EnableWindow(hWndSendMSG, TRUE);//���M�p�`���b�g�{�b�N�X�A
			EnableWindow(hWndSend, TRUE);	//[���M]�{�^����L���ɂ���
			SetWindowText(hWndHelp, "�����ł����\�����Ă��������B");

		}
		else if (rule_num == 2) {
			EnableWindow(hWndClear, TRUE);	// [�N���A]�{�^����L���ɂ��A
			DrawableFlag = 1;				//�`��\�t���O���P�ɂ���
			SetWindowText(hWndHelp, "�G�ł����\�����Ă��������B\r\n�G�����������ꍇ�̓N���A�������Ă��������B");
		}

	}
	else if (FlagPlayer % 2 == 0) {//�q�Ȃ��

		if (Use_PL == 1) {			//����PL1�Ȃ��
			set_score(a, b);		//a��PL1�́Ab��PL2�̓��_�ɉ�����

		}
		else if (Use_PL == 0) {	//����PL2�Ȃ��
			set_score(b, a);		//b��PL1�́Aa��PL2�̓��_�ɉ�����

		}

		if (turn == 3) {			//���ꂪ�w��^�[�������}�����Ȃ��
			score_judge(hWnd);		//����������s���I������
			return 0;

		}

		enable_player();			//�q�̏�����Ԃɂ���
		SetWindowText(hWndHelp, "���肪����\�����Ă��邩�����Ă��������B\r\n�킩��Ȃ��ꍇ�̓M�u�A�b�v�������Ă��������B");

	}

	FlagPlayer++;		//�������I������Ȃ�Ύ��̐e�A�q�����߂�
	turn++;				//�����Ď�Ԃ𑝂₵�I������
	return 0;

}

//����������s��
int score_judge(HWND hWnd) {

	if (Use_PL == 1) {						//������PL1�Ȃ��

		if (score_PL1 > score_PL2) {		//PL1�̓_����������Ώ���
			MessageBox(hWnd, "YOU WIN!!\n�e�̏����ł��I",
				"Information", MB_OK | MB_ICONINFORMATION);

		}
		else if (score_PL1 < score_PL2) {	//PL1�̓_�����Ⴏ��Ε���
			MessageBox(hWnd, "YOU LOSE!!\n�q�̏����ł��I",
				"Information", MB_OK | MB_ICONINFORMATION);

		}
		else if (score_PL1 == score_PL2) {	//PL1�̓_����PL2�Ɠ����Ȃ�Έ�������
										   //�Ƃ����|�����b�Z�[�W�{�b�N�X�ɕ\��
			MessageBox(hWnd, "���������ł��I",
				"Information", MB_OK | MB_ICONINFORMATION);
		}

	}

	else if (Use_PL == 0) {					//������PL2�Ȃ��

		if (score_PL1 > score_PL2) {		//PL2�̓_�����Ⴏ��Ε���
			MessageBox(hWnd, "YOU LOSE!!\n�e�̏����ł��I",
				"Information", MB_OK | MB_ICONINFORMATION);

		}
		else if (score_PL1 < score_PL2) {	//PL2�̓_����������Ώ���
			MessageBox(hWnd, "YOU WIN!!\n�q�̏����ł��I",
				"Information", MB_OK | MB_ICONINFORMATION);

		}
		else if (score_PL1 == score_PL2) {	//PL1�̓_����PL2�Ɠ����Ȃ�Έ�������
										   //�Ƃ����|�����b�Z�[�W�{�b�N�X�ɕ\��
			MessageBox(hWnd, "���������ł��I",
				"Information", MB_OK | MB_ICONINFORMATION);

		}

	}

	FlagPlayer = 0;		//�e�Ǝq���ǂ����0�Ƃ�
	turn = 0;			//��Ԃ����Z�b�g
	enable_end();		//�I����ԂƂȂ��ďI���
	return 0;

}

//  �\�P�b�g����������
BOOL SockInit(HWND hWnd)
{
	WSADATA wsa;		//WSAStartup��������������Ԃ��i�[����
	int ret;			//WSAStartup�̕Ԃ�l���i�[����
	char ret_buf[80];	//ret�̃G���[���e���i�[���A�����񉻂���

						//ret������������
	ret = WSAStartup(MAKEWORD(1, 1), &wsa);

	if (ret != 0) {		//����ret��0�o�Ȃ����
						//�G���[���e��\�����A�v���O�������I������
		wsprintf(ret_buf, "%d is the err", ret);
		MessageBox(hWnd, ret_buf, "Error", MB_OK | MB_ICONSTOP);
		exit(-1);
	}
	return FALSE;		//����ɍs��ꂽ�ꍇFALSE��Ԃ�
}

//  �\�P�b�g�ڑ� (�N���C�A���g��)
BOOL SockConnect(HWND hWnd, LPCSTR host)
{
	SOCKADDR_IN cl_sin;				// SOCKADDR_IN�\����

									// �\�P�b�g���J��
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET) {   // �\�P�b�g�쐬���s�����ꍇ
									//�G���[���e��\�����ATRUE��Ԃ�
		MessageBox(hWnd, "Socket() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	memset(&cl_sin, 0x00, sizeof(cl_sin));	// �\���̏�����
	cl_sin.sin_family = AF_INET;			// �C���^�[�l�b�g
	cl_sin.sin_port = htons(PORT);			// �|�[�g�ԍ��w��

	phe = gethostbyname(host);				// �A�h���X�擾

	if (phe == NULL) {						//phe��NULL�Ȃ�΃G���[���e��\�����ATRUE��Ԃ�
		MessageBox(hWnd, "gethostbyname() failed.",
			"Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;

	}
	//cl_sin��sin_addr��phe��h_addr��h_length���i�[����
	memcpy(&cl_sin.sin_addr, phe->h_addr, phe->h_length);

	// �񓯊����[�h (�ڑ�)
											//WSAAsyncSelect�̕Ԃ�l��SOCKET_ERROR�Ȃ��
	if (WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_CONNECT) == SOCKET_ERROR) {
		closesocket(sock);					//�\�P�b�g���
		sock = INVALID_SOCKET;				//�\�P�b�g��INVALID_SOCKET�Ƃ�
											//�G���[���e��\�����ATRUE��Ԃ�
		MessageBox(hWnd, "WSAAsyncSelect() failed",
			"Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;

	}

	// �ڑ�����
											//connect�����s���A�Ԃ�l��SOCKET_ERROR�ł���
	if (connect(sock, (LPSOCKADDR)&cl_sin, sizeof(cl_sin)) == SOCKET_ERROR) {
		//WSAGetLastError��WSAEWOULDLOCK�łȂ����
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			closesocket(sock);				//�\�P�b�g���
			sock = INVALID_SOCKET;			//�\�P�b�g��INVALID_SOCKET�Ƃ�
											//�G���[���e��\�����ATRUE��Ԃ�
			MessageBox(hWnd, "connect() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
			return TRUE;

		}

	}

	return FALSE;		//�S�ẴG���[������ʉ߂����Ȃ��FALSE��Ԃ�

}

//  �ڑ��҂� (�T�[�o��)
BOOL SockAccept(HWND hWnd)
{
	SOCKADDR_IN sv_sin;						// SOCKADDR_IN�\����

	// �T�[�o�p�\�P�b�g
	sv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sv_sock == INVALID_SOCKET) {		// �\�P�b�g�쐬���s�����ꍇ
											//�G���[���e��\�����ATRUE��Ԃ�
		MessageBox(hWnd, "Socket() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;

	}

	memset(&sv_sin, 0x00, sizeof(sv_sin));      // �\���̏�����
	sv_sin.sin_family = AF_INET;				// �C���^�[�l�b�g
	sv_sin.sin_port = htons(PORT);				// �|�[�g�ԍ��w��
	sv_sin.sin_addr.s_addr = htonl(INADDR_ANY); // �A�h���X�w��

												//bind�̕Ԃ�l��SOCKET_ERROR�Ȃ��
	if (bind(sv_sock, (LPSOCKADDR)&sv_sin, sizeof(sv_sin)) == SOCKET_ERROR) {
		closesocket(sv_sock);					//�\�P�b�g���
		sv_sock = INVALID_SOCKET;				//�\�P�b�g��INVALID_SOCKET�Ƃ�
												//�G���[���e��\�����ATRUE��Ԃ�
		MessageBox(hWnd, "bind() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;

	}

	if (listen(sv_sock, 5) == SOCKET_ERROR) {	//����listen�̕Ԃ�l��SOCKET_ERROR�Ȃ��
												// �ڑ��҂����s�Ƃ��āA
		closesocket(sv_sock);					//�\�P�b�g���
		sv_sock = INVALID_SOCKET;				//�\�P�b�g��INVALID_SOCKET�Ƃ�
												//�G���[���e��\�����ATRUE��Ԃ�
		MessageBox(hWnd, "listen() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;

	}

	// �񓯊��������[�h (�ڑ��҂�)
												//WSAAsyncSelect�̕Ԃ�l��SOCKET_ERROR�Ȃ��
	if (WSAAsyncSelect(sv_sock, hWnd, WM_SOCKET, FD_ACCEPT) == SOCKET_ERROR) {
		closesocket(sv_sock);					//�\�P�b�g���
		sv_sock = INVALID_SOCKET;				//�\�P�b�g��INVALID_SOCKET�Ƃ�
												//�G���[���e��\�����ATRUE��Ԃ�
		MessageBox(hWnd, "WSAAsyncSelect() failed",
			"Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;

	}

	return FALSE;		//�S�ẴG���[������ʉ߂����Ȃ��FALSE��Ԃ�

}


void enable_wait() {
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [�ڑ�]    
	EnableWindow(hWndAccept, FALSE);	// [�ڑ��҂�]
	EnableWindow(hWndReject, TRUE);	    // [�ؒf]    
	EnableWindow(hWndRejectOrder, FALSE);	// [�ؒf�v��]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [�M�u�A�b�v]
	EnableWindow(hWndCorrect, FALSE);	// [�����{�^��]    
	EnableWindow(hWndIncorrect, FALSE);	// [�s�����{�^��]
	EnableWindow(hWndConsent, FALSE);	// [����]    
	EnableWindow(hWndPointout, FALSE);	// [�w�E]    
	EnableWindow(hWndDenial, FALSE);	// [�۔F]   
	EnableWindow(hWndSend, FALSE);	// [���M]  
	EnableWindow(hWndChange, FALSE);	// [���]
	EnableWindow(hWndClear, FALSE);	// [�N���A]
}

void enable_standby() {
	EnableWindow(hWndHost, TRUE);	    // [HostName]
	EnableWindow(hWndConnect, TRUE);	    // [�ڑ�]    
	EnableWindow(hWndAccept, TRUE);	    // [�ڑ��҂�]
	EnableWindow(hWndReject, FALSE);	// [�ؒf]    
	EnableWindow(hWndRejectOrder, FALSE);	// [�ؒf�v��]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [�M�u�A�b�v]
	EnableWindow(hWndCorrect, FALSE);	// [�����{�^��]    
	EnableWindow(hWndIncorrect, FALSE);	// [�s�����{�^��]
	EnableWindow(hWndConsent, FALSE);	// [����]    
	EnableWindow(hWndPointout, FALSE);	// [�w�E]    
	EnableWindow(hWndDenial, FALSE);	// [�۔F]   
	EnableWindow(hWndSend, FALSE);	// [���M]  
	EnableWindow(hWndChange, FALSE);	// [���]
	EnableWindow(hWndClear, FALSE);	// [�N���A]
}

void enable_master() {
	EnableWindow(hWndSendMSG, FALSE);	// [���M�p�`���b�g�{�b�N�X]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [�ڑ�]    
	EnableWindow(hWndAccept, FALSE);	// [�ڑ��҂�]
	EnableWindow(hWndReject, TRUE);	    // [�ؒf]    
	EnableWindow(hWndRejectOrder, TRUE);	    // [�ؒf�v��]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [�M�u�A�b�v]
	EnableWindow(hWndCorrect, TRUE);	    // [�����{�^��]    
	EnableWindow(hWndIncorrect, TRUE);	    // [�s�����{�^��]
	EnableWindow(hWndConsent, FALSE);	// [����]    
	EnableWindow(hWndPointout, FALSE);	// [�w�E]    
	EnableWindow(hWndDenial, FALSE);	// [�۔F]   
	EnableWindow(hWndSend, FALSE);	// [���M]  
	EnableWindow(hWndChange, FALSE);	// [���]
	EnableWindow(hWndClear, FALSE);	// [�N���A]
}

void enable_player() {
	EnableWindow(hWndSendMSG, TRUE);	    // [���M�p�`���b�g�{�b�N�X]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [�ڑ�]    
	EnableWindow(hWndAccept, FALSE);	// [�ڑ��҂�]
	EnableWindow(hWndReject, TRUE);	    // [�ؒf]    
	EnableWindow(hWndRejectOrder, TRUE);	    // [�ؒf�v��]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, TRUE);	    // [�M�u�A�b�v]
	EnableWindow(hWndCorrect, FALSE);	// [�����{�^��]    
	EnableWindow(hWndIncorrect, FALSE);	// [�s�����{�^��]
	EnableWindow(hWndConsent, FALSE);	// [����]    
	EnableWindow(hWndPointout, FALSE);	// [�w�E]    
	EnableWindow(hWndDenial, FALSE);	// [�۔F]   
	EnableWindow(hWndSend, TRUE);	    // [���M]  
	EnableWindow(hWndChange, FALSE);	// [���]
	EnableWindow(hWndClear, FALSE);	// [�N���A]
}

void enable_end() {
	EnableWindow(hWndSendMSG, FALSE);	// [���M�p�`���b�g�{�b�N�X]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [�ڑ�]    
	EnableWindow(hWndAccept, FALSE);	// [�ڑ��҂�]
	EnableWindow(hWndReject, TRUE);	    // [�ؒf]    
	EnableWindow(hWndRejectOrder, TRUE);	    // [�ؒf�v��]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [�M�u�A�b�v]
	EnableWindow(hWndCorrect, FALSE);	// [�����{�^��]    
	EnableWindow(hWndIncorrect, FALSE);	// [�s�����{�^��]
	EnableWindow(hWndConsent, FALSE);	// [����]    
	EnableWindow(hWndPointout, FALSE);	// [�w�E]    
	EnableWindow(hWndDenial, FALSE);	// [�۔F]   
	EnableWindow(hWndSend, FALSE);	// [���M]  
	EnableWindow(hWndChange, FALSE);	// [���]
	EnableWindow(hWndClear, FALSE);	// [�N���A]
}

void enable_correct() {
	EnableWindow(hWndSendMSG, FALSE);	// [���M�p�`���b�g�{�b�N�X]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [�ڑ�]    
	EnableWindow(hWndAccept, FALSE);	// [�ڑ��҂�]
	EnableWindow(hWndReject, TRUE);	    // [�ؒf]    
	EnableWindow(hWndRejectOrder, TRUE);	    // [�ؒf�v��]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [�M�u�A�b�v]
	EnableWindow(hWndCorrect, FALSE);	// [�����{�^��]    
	EnableWindow(hWndIncorrect, FALSE);	// [�s�����{�^��]
	EnableWindow(hWndConsent, FALSE);	// [����]    
	EnableWindow(hWndPointout, TRUE);	    // [�w�E]    
	EnableWindow(hWndDenial, FALSE);	// [�۔F]   
	EnableWindow(hWndSend, FALSE);	// [���M]  
	EnableWindow(hWndChange, TRUE);	    // [���]
	EnableWindow(hWndClear, FALSE);	// [�N���A]
}

void enable_pause() {
	EnableWindow(hWndSendMSG, FALSE);	// [���M�p�`���b�g�{�b�N�X]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [�ڑ�]    
	EnableWindow(hWndAccept, FALSE);	// [�ڑ��҂�]
	EnableWindow(hWndReject, TRUE);	    // [�ؒf]    
	EnableWindow(hWndRejectOrder, TRUE);     // [�ؒf�v��]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [�M�u�A�b�v]
	EnableWindow(hWndCorrect, FALSE);	// [�����{�^��]    
	EnableWindow(hWndIncorrect, FALSE);	// [�s�����{�^��]
	EnableWindow(hWndConsent, FALSE);	// [����]    
	EnableWindow(hWndPointout, FALSE);	// [�w�E]    
	EnableWindow(hWndDenial, FALSE);	// [�۔F]   
	EnableWindow(hWndSend, TRUE);	    // [���M]  
	EnableWindow(hWndChange, FALSE);	// [���]
	EnableWindow(hWndClear, FALSE);    // [�N���A]
}

void enable_pointout_master() {
	EnableWindow(hWndSendMSG, TRUE);	    // [���M�p�`���b�g�{�b�N�X]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [�ڑ�]    
	EnableWindow(hWndAccept, FALSE);	// [�ڑ��҂�]
	EnableWindow(hWndReject, TRUE);	    // [�ؒf]    
	EnableWindow(hWndRejectOrder, TRUE);	    // [�ؒf�v��]
	DrawableFlag = 1;
	EnableWindow(hWndGiveup, FALSE);	// [�M�u�A�b�v]
	EnableWindow(hWndCorrect, FALSE);	// [�����{�^��]    
	EnableWindow(hWndIncorrect, FALSE);	// [�s�����{�^��]
	EnableWindow(hWndConsent, TRUE);	    // [����]    
	EnableWindow(hWndPointout, FALSE);	// [�w�E]    
	EnableWindow(hWndDenial, TRUE);	    // [�۔F]   
	EnableWindow(hWndSend, TRUE);	    // [���M]  
	EnableWindow(hWndChange, FALSE);	// [���]
	EnableWindow(hWndClear, FALSE);	// [�N���A]
}

void enable_pointout_player() {
	EnableWindow(hWndSendMSG, TRUE);	    // [���M�p�`���b�g�{�b�N�X]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [�ڑ�]    
	EnableWindow(hWndAccept, FALSE);	// [�ڑ��҂�]
	EnableWindow(hWndReject, TRUE);	    // [�ؒf]    
	EnableWindow(hWndRejectOrder, TRUE);     // [�ؒf�v��]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [�M�u�A�b�v]
	EnableWindow(hWndCorrect, FALSE);	// [�����{�^��]    
	EnableWindow(hWndIncorrect, FALSE);	// [�s�����{�^��]
	EnableWindow(hWndConsent, FALSE);	// [����]    
	EnableWindow(hWndPointout, FALSE);	// [�w�E]    
	EnableWindow(hWndDenial, FALSE);	// [�۔F]   
	EnableWindow(hWndSend, TRUE);	    // [���M]  
	EnableWindow(hWndChange, FALSE);	// [���]
	EnableWindow(hWndClear, FALSE);	// [�N���A]
}
