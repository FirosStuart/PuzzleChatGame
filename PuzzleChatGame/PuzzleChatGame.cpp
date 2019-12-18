
//  ヘッダファイル
#include <Windows.h>
#include <WinSock.h>
#include <string>
#include <stdio.h>

// 使用ライブラリ
#pragma comment(lib,"wsock32.lib")

//  定数定義
#define WM_SOCKET       (WM_USER+1)     // ソケット用メッセージ
#define PORT            10000           // 通信ポート番号

#define IDB_CONNECT     1000            // [接続]ボタン
#define IDB_ACCEPT      1001            // [接続待ち]ボタン
#define IDB_REJECT      1002            // [切断]ボタン
#define IDB_REJECTORDER 1003            // [切断要請]ボタン
#define IDB_GIVEUP      1004			// [ギブアップ]ボタン
#define IDB_CORRECT		1005			// [正解]ボタン
#define	IDB_INCORRECT   1006			// [不正解]ボタン
#define IDB_POINTOUT    1007			// [指摘]ボタン
#define	IDB_CONSENT     1008			// [承諾]ボタン
#define IDB_DENIAL      1009			// [否認]ボタン
#define IDB_CHANGE      1010			// [交代]ボタン
#define IDB_SEND        1011            // [送信]ボタン
#define IDB_CLEAR       1012            // [クリア]ボタン

#define IDF_HOSTNAME    2000            // ホスト名入力エディットボックス
#define IDF_QUESTION    2001            // お題表示用エディットボックス
#define IDF_RULE        2002            // 制約条件用エディットボックス
#define IDF_SENDMSG     2003            // 送信メッセージ入力用エディットボックス
#define IDF_RECVMSG     2004            // 受信メッセージ表示用エディットボックス
#define IDF_SCORE_PL1   2005            // 自分の得点用エディットボックス
#define IDF_SCORE_PL2   2006            // 相手の得点用エディットボックス
#define IDF_HELP	2007		// ヘルプボックス用エディットボックス
#define WINDOW_W		910				// ウィンドウの幅
#define WINDOW_H		800             // ウィンドウの高さ
#define MAX_ARRAY		10000			// 配列の最大要素数
#define MAX_MESSEGE		30				// メッセージの最大要素数

//グローバル変数
LPCTSTR lpClassName = "Oekaki";         // ウィンドウクラス名
LPCTSTR lpWindowName = "Oekaki";        // タイトルバーにつく名前

SOCKET sock = INVALID_SOCKET;           // ソケット
SOCKET sv_sock = INVALID_SOCKET;        // サーバ用ソケット
HOSTENT* phe;							// HOSTENT構造体

HPEN hPenBlack;							// 黒ペン
HPEN hPenRed;							// 赤ペン

static HWND hWndHost;                       // ホスト名入力用エディットボックス
static HWND hWndQuestion;                   // お題表示用エディットボックス
static HWND hWndRule;                       // 制約条件用エディットボックス
static HWND hWndSendMSG;                    // 送信メッセージ入力用エディットボックス
static HWND hWndRecvMSG;                    // 受信メッセージ表示用エディットボックス
static HWND hWndScore_pl1;                  // 自分の得点用エディットボックス
static HWND hWndScore_pl2;                  // 相手の得点用エディットボックス
static HWND hWndHelp;			    // ヘルプボックス用エディットボックス
static HWND hWndConnect, hWndAccept;        // [接続]ボタンと[接続待ち]ボタン
static HWND hWndReject;                     // [切断]ボタン
static HWND hWndRejectOrder;                // [切断要請]ボタン
static HWND hWndGiveup;						// [ギブアップ]ボタン
static HWND hWndCorrect;					// [正解]ボタン
static HWND hWndIncorrect;					// [不正解]ボタン
static HWND hWndPointout;					// [指摘]ボタン
static HWND hWndConsent;					// [承諾]ボタン
static HWND hWndDenial;						// [否認]ボタン
static HWND hWndChange;						// [交代]ボタン
static HWND hWndSend;                       // [送信]ボタン
static HWND hWndClear;                       // ホスト名入力用エディットボックス

static int DrawableFlag = 0;				// 入力 1:可能/0:不可 状態確認フラグ
int FlagPlayer;				// 入力 1:可能/0:不可 状態確認フラグ
int Use_PL = 0;
int score_PL1 = 0;									//PL1のスコア
int score_PL2 = 0;									//PL2のスコア
int turn = 0;											//手番
int dice_num;
int rule_num;										//ルールの番号

const RECT d = { 10, 200, 450, 700 };               // 描画領域(左上隅のx座標, 左上隅のy座標, 右下隅のx座標, 右下隅のy座標)

int n;                                              // カウンタ
int flag[MAX_ARRAY];                                // ペンダウンフラグ
POINT pos[MAX_ARRAY];                               // 座標を格納
HPEN colors[MAX_ARRAY];								// 使用する色

char rule[3][10] = {"絵と文字","文字のみ","絵のみ"};//ルールの内容
int card[10] = { 0,1,2,3,4,5,6,7,8,9 };				//カード順番
char card_text[10][6][30] =							//お題の書かれたカード
{
	{"猫","鼠","犬","熊","狐","馬"},
	{"テレビ","洗濯機","エアコン","冷蔵庫","電子レンジ","自動車"},
	{"クリスマス","バレンタイン","お正月","ハロウィン","夏祭り","七夕"},
	{"サッカーボール","野球ボール","バレーボール","バスケボール","卓球ボール","テニスボール"},
	{"キャビンアテンダント","ナース","会社員","学生","警察官","バーテンダー"},
	{"ピザ","ハンバーガー","コーラ","ポップコーン","フライドチキン","フライドポテト"},
	{"ケーキ", "パフェ", "プリン", "アイスクリーム", "ドーナツ", "クレープ"},
	{"手袋","スカート","リボン","靴下","コート","スーツ"},
	{"桃太郎","金太郎","かぐや姫","かちかち山","浦島太郎","鶴の恩返し"},
	{"アメリカ","ロシア","ドイツ","中国","日本","イギリス"}
};
//  プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);// ウィンドウ関数
LRESULT CALLBACK OnPaint(HWND, UINT, WPARAM, LPARAM);	// 描画関数
LRESULT CALLBACK ClearPaint(HWND, UINT, WPARAM, LPARAM);	// 描画関数

BOOL checkMousePos(int x, int y);						// マウスの位置がキャンパスの中かどうか判定する
void setData(int flag, int x, int y, HPEN color);       // 描画情報を入れる
void FXY(int f, int x, int y);
void ChatReset(HWND chatbox);

int randAtoC();
void rand0toi(int ary[], int size);
int dice();

void game_start(HWND hWnd);
int change(HWND, int a, int b);
int score_judge(HWND hWnd);

BOOL SockInit(HWND hWnd);                               // ソケット初期化
BOOL SockAccept(HWND hWnd);                             // ソケット接続待ち
BOOL SockConnect(HWND hWnd, LPCSTR host);               // ソケット接続

void enable_master();
void enable_player();
void enable_end();
void enable_correct();
void enable_pause();
void enable_pointout_master();
void enable_pointout_player();
void enable_wait();
void enable_standby();


//  WinMain関数 (Windowsプログラム起動時に呼ばれる関数)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	MSG  msg;                                           // メッセージ
	WNDCLASSEX wc;                                      // ウィンドウクラス
	HWND hWnd;                                          // ウィンドウハンドル

	//ウィンドウクラス定義
	wc.hInstance = hInstance;							// インスタンス
	wc.lpszClassName = lpClassName;                     // クラス名
	wc.lpfnWndProc = WindowProc;						// ウィンドウ関数名
	wc.style = 0;										// クラススタイル
	wc.cbSize = sizeof(WNDCLASSEX);						// 構造体サイズ
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// アイコンハンドル
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);			// スモールアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// マウスポインタ
	wc.lpszMenuName = NULL;                             // メニュー(なし)
	wc.cbClsExtra = 0;									// クラス拡張情報
	wc.cbWndExtra = 0;									// ウィンドウ拡張情報
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;			// ウィンドウの背景色
	if (!RegisterClassEx(&wc)) return 0;                // ウィンドウクラス登録

	// ウィンドウ生成
	hWnd = CreateWindow(
		lpClassName,                                // ウィンドウクラス名
		lpWindowName,                               // ウィンドウ名
		WS_DLGFRAME | WS_VISIBLE | WS_SYSMENU,      // ウィンドウ属性
		CW_USEDEFAULT,                              // ウィンドウ表示位置(X)
		CW_USEDEFAULT,                              // ウィンドウ表示位置(Y)
		WINDOW_W,                                   // ウィンドウサイズ(X)
		WINDOW_H,                                   // ウィンドウサイズ(Y)
		HWND_DESKTOP,                               // 親ウィンドウハンドル
		NULL,
		hInstance,                                  // インスタンスハンドル
		NULL
	);

	// ウィンドウ表示
	ShowWindow(hWnd, nCmdShow);                         // ウィンドウ表示モード
	UpdateWindow(hWnd);                                 // ウインドウ更新

	// メッセージループ
	while (GetMessage(&msg, NULL, 0, 0)) {              // メッセージを取得
		TranslateMessage(&msg);
		DispatchMessage(&msg);                          // メッセージ送る
	}
	return (int)msg.wParam;                             // プログラム終了
}


//  ウィンドウ関数(イベント処理を記述)
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wP, LPARAM lP)
{
	static BOOL mouseFlg = FALSE;       // 前回の状態 TRUE:描画した、FALSE:描画していない
	SOCKADDR_IN cl_sin;
	int flag2, x2, y2;
	int len = sizeof(cl_sin);
	char host[100];
	char buf[100];						// 受信内容を一時的に格納するバッファ
	int score_Master = 0;								//親のスコア	
	int score_Player = 0;								//子のスコア

	switch (uMsg) {
	case WM_CREATE:			// ウィンドウが生成された
	{// 文字列表示
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
		// ホスト名入力用エディットボックス
		hWndHost = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE, 10, 30, 200, 25,
			hWnd, (HMENU)IDF_HOSTNAME, NULL, NULL);
		// [接続]ボタン
		hWndConnect = CreateWindow("button", "接続",
			WS_CHILD | WS_VISIBLE, 220, 30, 50, 25,
			hWnd, (HMENU)IDB_CONNECT, NULL, NULL);
		// [接続待ち]ボタン
		hWndAccept = CreateWindow("button", "接続待ち",
			WS_CHILD | WS_VISIBLE, 275, 30, 90, 25,
			hWnd, (HMENU)IDB_ACCEPT, NULL, NULL);
		// [切断要請]ボタン
		hWndRejectOrder = CreateWindow("button", "切断要請",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 275, 70, 90, 25,
			hWnd, (HMENU)IDB_REJECTORDER, NULL, NULL);
		// [切断]ボタン
		hWndReject = CreateWindow("button", "切断",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 220, 70, 50, 25,
			hWnd, (HMENU)IDB_REJECT, NULL, NULL);
		// [送信]ボタン
		hWndSend = CreateWindow("button", "送信",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 800, 560, 90, 25,
			hWnd, (HMENU)IDB_SEND, NULL, NULL);
		// [ギブアップ]ボタン
		hWndGiveup = CreateWindow("button", "ギブアップ",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 480, 30, 100, 25,
			hWnd, (HMENU)IDB_GIVEUP, NULL, NULL);
		// [正解]ボタン
		hWndCorrect = CreateWindow("button", "正解",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 400, 30, 70, 25,
			hWnd, (HMENU)IDB_CORRECT, NULL, NULL);
		// [不正解]ボタン
		hWndIncorrect = CreateWindow("button", "不正解",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 400, 70, 70, 25,
			hWnd, (HMENU)IDB_INCORRECT, NULL, NULL);
		// [指摘]ボタン
		hWndPointout = CreateWindow("button", "指摘",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 480, 70, 100, 25,
			hWnd, (HMENU)IDB_POINTOUT, NULL, NULL);
		// [承諾]ボタン
		hWndConsent = CreateWindow("button", "承諾",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 590, 30, 50, 25,
			hWnd, (HMENU)IDB_CONSENT, NULL, NULL);
		// [否認]ボタン
		hWndDenial = CreateWindow("button", "否認",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 590, 70, 50, 25,
			hWnd, (HMENU)IDB_DENIAL, NULL, NULL);
		// [交代]ボタン
		hWndChange = CreateWindow("button", "交代",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 660, 30, 70, 25,
			hWnd, (HMENU)IDB_CHANGE, NULL, NULL);
		// [クリア]ボタン
		hWndClear = CreateWindow("button", "クリア",
			WS_CHILD | WS_VISIBLE | WS_DISABLED, 660, 70, 70, 25,
			hWnd, (HMENU)IDB_CLEAR, NULL, NULL);
		// お題用エディットボックス
		hWndQuestion = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_READONLY, 10, 150, 200, 25,
			hWnd, (HMENU)IDF_QUESTION, NULL, NULL);
		// 制約条件用エディットボックス
		hWndRule = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_READONLY, 460, 200, 300, 50,
			hWnd, (HMENU)IDF_RULE, NULL, NULL);
		// 送信メッセージ入力用エディットボックス
		hWndSendMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_DISABLED, 460, 270, 300, 150,
			hWnd, (HMENU)IDF_SENDMSG, NULL, NULL);
		// 受信メッセージ表示用エディットボックス
		hWndRecvMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 460, 440, 300, 150,
			hWnd, (HMENU)IDF_RECVMSG, NULL, NULL);
		// ヘルプボックス用エディットボックス
		hWndHelp = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 460, 600, 300, 100,
			hWnd, (HMENU)IDF_HELP, NULL, NULL);
		// 自分の得点用エディットボックス
		hWndScore_pl1 = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_READONLY, 720, 25, 50, 25,
			hWnd, (HMENU)IDF_SCORE_PL1, NULL, NULL);
		// 相手の得点用エディットボックス
		hWndScore_pl2 = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
			WS_CHILD | WS_VISIBLE | ES_READONLY, 835, 25, 50, 25,
			hWnd, (HMENU)IDF_SCORE_PL2, NULL, NULL);

		SockInit(hWnd);         // ソケット初期化
		hPenBlack	= (HPEN)CreatePen(PS_SOLID, 3, RGB(0, 0, 0));		//黒ペン作成
		hPenRed		= (HPEN)CreatePen(PS_SOLID, 3, RGB(255, 0, 0));		//赤ペン作成
		return 0L;
	}
	case WM_COMMAND:		// ボタンが押された
	{
		switch (LOWORD(wP)) {
		case IDB_ACCEPT:    // [接続待ち]ボタン押下(サーバー)
			if (SockAccept(hWnd)) {  // 接続待ち要求
				return 0L;      // 接続待ち失敗
			}
			enable_wait();
			return 0L;

		case IDB_CONNECT:   // [接続]ボタン押下(クライアント)

			GetWindowText(hWndHost, host, sizeof(host));

			if (SockConnect(hWnd, host)) {   // 接続要求
				return 0L;
			}
			enable_wait();
			return 0L;

		case IDB_REJECT:    // [切断]ボタン押下
			if (sock != INVALID_SOCKET) {    // 自分がクライアント側なら
				// ソケットを閉じる
				closesocket(sock);
				sock = INVALID_SOCKET;
			}
			if (sv_sock != INVALID_SOCKET) { // 自分がサーバ側なら
				// サーバ用ソケットを閉じる
				closesocket(sv_sock);
				sv_sock = INVALID_SOCKET;
			}
			phe = NULL;
			enable_standby();
			return 0L;

		case IDB_REJECTORDER:      // [切断要請]ボタン押下
			strcpy_s(buf, "REJECT");				// 切断の内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);	// REJECTと送信
			return 0L;

		case IDB_SEND:      // [送信]ボタン押下
			char sender[100];
			GetWindowText(hWndSendMSG, buf, sizeof(buf) - 1);     // 送信メッセージ入力欄の内容を取得
			sprintf_s(sender,"%s%s","MESS",buf);	//送信の内容をバッファに保存
			if (send(sock, sender, strlen(sender) + 1, 0) == SOCKET_ERROR) {    // 送信処理
						// 送信に失敗したらエラーを表示
				MessageBox(hWnd, TEXT("sending failed"), TEXT("Error"),
					MB_OK | MB_ICONEXCLAMATION);
			}
			
			ChatReset(hWndSendMSG);//チャットボックスのクリア
			return 0L;

		case IDB_GIVEUP:	// [ギブアップ]ボタン押下
			strcpy_s(buf, "GIVEUP");			// ギブアップの内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);// GIVEUPと送信
			score_Master = 1;					// 親と子の点数を１点ずつ付与
			score_Player = 1;
			MessageBox(hWnd, "ギブアップしました。",
				"Information", MB_OK | MB_ICONINFORMATION);
			change(hWnd,score_Master, score_Player);	// 交代
			return 0L;

		case IDB_CORRECT:   // [正解]押下
			strcpy_s(buf, "CORRECT");			// 正解の内容をバッファに保存
			sprintf_s(sender, "%s%s%s", "CORRECT","答え:", card_text[card[turn-1]][dice_num]);
			send(sock, sender, strlen(sender) + 1, 0);
			enable_pause();
			SetWindowText(hWndHelp, "相手の判断を待っています。");
			return 0L;	

		case IDB_CLEAR:   // [クリア]押下
			strcpy_s(buf, "CLEAR");			// クリアの内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);
			ClearPaint(hWnd, uMsg, wP, lP);
			InvalidateRect(hWnd, &d, TRUE);
			return 0L;

		case IDB_INCORRECT: // [不正解]押下
			strcpy_s(buf, "INCORRECT");			// 不正解の内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);// INCORRECTと送信
			return 0L;

		case IDB_POINTOUT:  // [指摘]押下
			strcpy_s(buf, "POINTOUT");			// 指摘の内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);// POINTOUTと送信	
			enable_pointout_player();
			SetWindowText(hWndHelp, "相手の判断を待っています。");
			return 0L;

		case IDB_CONSENT:   // [承諾]押下
			strcpy_s(buf, "CONSENT");           // 承諾の内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);// CONSENTと送信
			score_Master = 0;                   // 親に0点、子に1点を付与
			score_Player = 1;
			change(hWnd, score_Master, score_Player);	// 交代
			return 0L;

		case IDB_DENIAL:    // [否認]押下
			strcpy_s(buf, "DENIAL");			// 否認の内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);// DENIALと送信
			if (rule_num == 0) {		//Aを正解
				score_Master = 1;		//親に1点、子に2点を付与
				score_Player = 2;
			}
			else if (rule_num == 1) {	//Bを正解
				score_Master = 1;		//親に1点、子に3点を付与
				score_Player = 3;
			}
			else if (rule_num == 2) {	//Cを正解
				score_Master = 1;		//親に1点、子に5点を付与
				score_Player = 5;
			}
			change(hWnd,score_Master, score_Player);	// 交代
			return 0L;

		case IDB_CHANGE:    // [交代]押下
			strcpy_s(buf, "CHANGE");             // 交代の内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0); // CHANGEと送信
			if (rule_num == 0) {		//Aを正解
				score_Master = 1;		//親に1点、子に2点を付与
				score_Player = 2;
			}
			else if (rule_num == 1) {	//Bを正解
				score_Master = 1;		//親に1点、子に3点を付与
				score_Player = 3;
			}
			else if (rule_num == 2) {	//Cを正解
				score_Master = 1;		//親に1点、子に5点を付与
				score_Player = 5;
			}
			change(hWnd, score_Master, score_Player);  // 交代
			return 0L;

		} /* end of switch (LOWORD(wP)) */

		return 0L;
	}
	case WM_LBUTTONDOWN:    // マウス左ボタンが押された
	{
		if ((checkMousePos(LOWORD(lP), HIWORD(lP)) == TRUE )&& (DrawableFlag==1)) {  // 描画領域の中なら
			setData(0, LOWORD(lP), HIWORD(lP), hPenBlack);    // 線の始点として座標を記録
			FXY(0, LOWORD(lP), HIWORD(lP));                   // 線の始点として座標を記録
			mouseFlg = TRUE;
			n++;
			InvalidateRect(hWnd, &d, FALSE);
		}
		return 0L;
	}
	
	case WM_MOUSEMOVE:  // マウスポインタが移動した
	{
		if (wP == MK_LBUTTON) {  // 左ボタンが押されている
			if ((checkMousePos(LOWORD(lP), HIWORD(lP)) == TRUE )&&( DrawableFlag == 1)) {		// 描画領域の中なら
				if (mouseFlg) {										// 前回描画しているなら
					setData(1, LOWORD(lP), HIWORD(lP), hPenBlack);   // 線の途中として座標を記録
					FXY(1, LOWORD(lP), HIWORD(lP));					// 線の途中として座標を送信
				}
				else {												// 前回描画していないなら
					setData(0, LOWORD(lP), HIWORD(lP), hPenBlack);   // 線の始点として座標を記録
					FXY(0, LOWORD(lP), HIWORD(lP));					// 線の始点として座標を送信
				}
				
				n++;
				mouseFlg = TRUE;
				InvalidateRect(hWnd, &d, FALSE);
			}
			else {                     // 描画領域の外なら
				mouseFlg = FALSE;
			}
		}
		

		return 0L;
	}
	case WM_SOCKET:          // 非同期処理メッセージ
	{
		if (WSAGETSELECTERROR(lP) != 0) { return 0L; }

		switch (WSAGETSELECTEVENT(lP)) {
		case FD_ACCEPT:     // 接続待ち完了通知
		{

			sock = accept(sv_sock, (LPSOCKADDR)&cl_sin, &len);

			if (sock == INVALID_SOCKET) {
				MessageBox(hWnd, "Accepting connection failed",
					"Error", MB_OK | MB_ICONEXCLAMATION);
				closesocket(sv_sock);
				sv_sock = INVALID_SOCKET;
				enable_standby();
				return 0L;
			}

#ifndef NO_DNS
			// ホスト名取得
			phe = gethostbyaddr((char*)&cl_sin.sin_addr, 4, AF_INET);
			if (phe) { SetWindowText(hWndHost, phe->h_name);}
#endif  NO_DNS

			// 非同期モード (受信＆切断）
			if (WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE)
				== SOCKET_ERROR) {
				// 接続に失敗したら初期状態に戻す
				MessageBox(hWnd, "WSAAsyncSelect() failed",
					"Error", MB_OK | MB_ICONEXCLAMATION);
				enable_standby();
				return 0L;
			}
			Use_PL = 1;
			FlagPlayer = 1;	//親としてゲームを開始
			game_start(hWnd);	//親を0点子を0点としてゲームを開始する
			return 0L;
		}/* end of case FD_ACCEPT: */

		case FD_CONNECT:    // 接続完了通知
			// 非同期モード (受信＆切断)
			if (WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE)
				== SOCKET_ERROR) {
				// 接続に失敗したら初期状態に戻す

				MessageBox(hWnd, "WSAAsyncSelect() failed",
					"Error", MB_OK | MB_ICONEXCLAMATION);
				enable_standby();
				return 0L;
			}

			Use_PL = 0;
			game_start(hWnd);		//通信親0点、子0点としてゲームを開始する
			return 0L;

		case FD_READ:									//メッセージ受信
			char b[100];

			if (recv(sock, buf, sizeof(buf) - 1, 0) != SOCKET_ERROR) { // 受信できたなら			
				if (strncmp(buf, "MESS",4) == 0) {		//チャットボックスの内容が
														//送られた場合
					strncpy_s(b,buf+4,sizeof(buf)-4);	//4文字目以降の内容を変数bに格納
					SetWindowText(hWndRecvMSG,b);		//bの内容を送信
					return 0L;

				}else if (strncmp(buf, "RULE",4) == 0) {	//ルールが送られてきた場合
					strncpy_s(b, buf + 4, sizeof(buf) - 4);	//4文字目以降の内容を変数bに格納
					rule_num = atoi(b);						//bからint型に変えたものをrule_numに格納
					SetWindowText(hWndRule, rule[rule_num]);//rule_numに対応したルールを提示する
					return 0L;
				}
				else if (strcmp(buf, "REJECT") == 0) {		//REJECTが送られてきた場合
															//切断要請が来たとメッセージボックスに表示する
					MessageBox(hWnd, "切断要請がきました。",
						"Information", MB_OK | MB_ICONINFORMATION);
					return 0L;
				}
				if (strncmp(buf, "CORRECT", 7) == 0) {		//正解だった場合
															//送られた場合
					strncpy_s(b, buf + 7, sizeof(buf) - 7);	//7文字目以降の内容を変数bに格納
					SetWindowText(hWndRecvMSG, b);			//問題の内容を受信ボックスに貼る
					MessageBox(hWnd, "正解です！",
						"Information", MB_OK | MB_ICONINFORMATION);
					enable_correct();
					SetWindowText(hWndHelp, "相手が違反行為をしていたと感じたら指摘を押してください。\r\n問題ない場合は交代を押してください。");
					return 0L;
				}
				else if (strcmp(buf, "POINTOUT") == 0) {
					MessageBox(hWnd, "指摘点があります！",
						"Information", MB_OK | MB_ICONINFORMATION);
					enable_pointout_master();
					SetWindowText(hWndHelp, "指摘を認める場合は承認を押してください。\r\n認めない場合は否認を押してください。");
					return 0L;

				}
				else if (strcmp(buf, "CLEAR") == 0) {
					ClearPaint(hWnd, uMsg, wP, lP);
					InvalidateRect(hWnd, &d, TRUE);
					return 0L;
				}
				else if (strcmp(buf, "CHANGE") == 0) {//指摘を認めないか、
					if (rule_num == 0) {		//Aを正解
						score_Master = 1;		//親に1点、子に2点を付与
						score_Player = 2;
					}
					else if (rule_num == 1) {	//Bを正解
						score_Master = 1;		//親に1点、子に3点を付与
						score_Player = 3;
					}
					else if (rule_num == 2) {	//Cを正解
						score_Master = 1;		//親に1点、子に5点を付与
						score_Player = 5;
					}
					change(hWnd, score_Master, score_Player);		//交代
					return 0L;
				}
				else if (strcmp(buf, "DENIAL") == 0) {
					if (rule_num == 0) {		//Aを正解
						score_Master = 1;		//親に1点、子に2点を付与
						score_Player = 2;
					}
					else if (rule_num == 1) {	//Bを正解
						score_Master = 1;		//親に1点、子に3点を付与
						score_Player = 3;
					}
					else if (rule_num == 2) {	//Cを正解
						score_Master = 1;		//親に1点、子に5点を付与
						score_Player = 5;
					}
					change(hWnd, score_Master, score_Player);		//交代
					return 0L;
				}
				else if (strcmp(buf, "CONSENT") == 0) {		//指摘を承認
					score_Master = 0;						//親に0点、子に1点を付与
					score_Player = 1;
					change(hWnd, score_Master, score_Player);		//交代
					return 0L;
				}
				else if (strcmp(buf, "GIVEUP") == 0) {		//ギブアップ
					score_Master = 1;						//親に1点、子に1点を付与
					score_Player = 1;
					MessageBox(hWnd, "相手がギブアップしました",
						"Information", MB_OK | MB_ICONINFORMATION);
					change(hWnd, score_Master, score_Player);		//交代
					return 0L;
				}
				else if (strcmp(buf, "POINTOUT") == 0) {	//指摘が来た
					MessageBox(hWnd, "指摘点があります！",
						"Information", MB_OK | MB_ICONINFORMATION);
					return 0L;
				}
				else if (strcmp(buf, "INCORRECT") == 0) {	//不正解であるならば
					strncpy_s(buf,"不正解です",10);
					SetWindowText(hWndRecvMSG, buf);
					return 0L;
				}
				else{										//それ以外ならば
					sscanf_s(buf, "%1d%03d%03d", &flag2, &x2, &y2);										
					setData(flag2, x2, y2, hPenRed);
					n++;
					InvalidateRect(hWnd, &d, FALSE);
					return 0L;
				}
			}
			return 0L;

		case FD_CLOSE:      // 切断された
			MessageBox(hWnd, "切断されました。",
				"Information", MB_OK | MB_ICONINFORMATION);
			SendMessage(hWnd, WM_COMMAND, IDB_REJECT, 0); // 切断処理発行
			return 0L;

		}/* end of switch (WSAGETSELECTEVENT(lP)) */

		return 0L;
	}
	case WM_DESTROY:    // ウィンドウが破棄された
	{
		DeleteObject(hPenBlack);    // 黒ペン削除
		DeleteObject(hPenRed);      // 赤ペン削除
		closesocket(sock);          // ソケットを閉じる
		PostQuitMessage(0);         // 終了処理
		return 0L;
	}
	case WM_PAINT:      // 再描画
	{
		return OnPaint(hWnd, uMsg, wP, lP);  // 描画関数の呼び出し
		return 0L;
	}
	default:
	{
		return DefWindowProc(hWnd, uMsg, wP, lP);  // 標準メッセージ処理
	}
	}/* end of switch (uMsg) */
}


//  描画関数
LRESULT CALLBACK OnPaint(HWND hWnd, UINT uMsg, WPARAM wP, LPARAM lP)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hWnd, &ps);

	// 描画領域の初期化
	MoveToEx(hdc, d.left, d.top, NULL);
	LineTo(hdc, d.right, d.top);    // 上横線
	LineTo(hdc, d.right, d.bottom); // 右縦線
	LineTo(hdc, d.left, d.bottom);  // 下横線
	LineTo(hdc, d.left, d.top); // 左縦線
	
	for (int i = 0; i < n; i++) {    // 線を描画
		SelectObject(hdc, colors[i]);
		if (flag[i] == 0) {      // 開始点なら、始点を移動
			MoveToEx(hdc, pos[i].x, pos[i].y, NULL);
		}
		else {             // 途中の点なら線を引く
			LineTo(hdc, pos[i].x, pos[i].y);
		}
	}

	EndPaint(hWnd, &ps);

	return 0L;
}

LRESULT CALLBACK ClearPaint(HWND hWnd, UINT uMsg, WPARAM wP, LPARAM lP)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hWnd, &ps);

	// 描画領域の初期化
	MoveToEx(hdc, d.left, d.top, NULL);
	LineTo(hdc, d.right, d.top);    // 上横線
	LineTo(hdc, d.right, d.bottom); // 右縦線
	LineTo(hdc, d.left, d.bottom);  // 下横線
	LineTo(hdc, d.left, d.top); // 左縦線

	for (int i = 0; i < n; i++) {    // 線を描画
		flag[i] = 0;
		pos[i].x = 0;
		pos[i].y = 0;
		colors[i] = NULL;
	}
	
	n = 0;

	EndPaint(hWnd, &ps);

	return 0L;
}

//  描画情報を格納
void setData(int f, int x, int y,HPEN color)
{
	flag[n] = f;
	pos[n].x = x;
	pos[n].y = y;
	colors[n] = color;
}

//  マウスの位置がキャンパスの中かどうか判定する
BOOL checkMousePos(int x, int y)
{
	if (x >= d.left && x <= d.right
		&& y >= d.top && y <= d.bottom) {
		return TRUE;
	}
	return FALSE;
}

// データを送信する
void FXY(int f3, int x3, int y3) {
	char buf[9];                  // 送信内容を一時的に格納するバッファ

	sprintf_s(buf, "%1d%03d%03d", f3, x3, y3);
	send(sock, buf, strlen(buf) + 1, 0);
}

int randAtoC() {
	int A2C[] = {0,1,2};
	return A2C[rand()%3];
}

void rand0toi(int ary[],int size) {
	for (int i = 0; i < size;i++) {
		int j = rand() % size;
		int t = ary[i];
		ary[i] = ary[j];
		ary[j] = t;
	}
}

int dice() {
	int DICE[] = {0,1,2,3,4,5};
	return DICE[rand() % 6];
}

//チャットボックスの内容を消去
void ChatReset(HWND chatbox){
	SetWindowText(chatbox, TEXT(""));
	SetFocus(chatbox);
}

void set_score(int x,int y) {
	char buf[100];

	score_PL1 += x;                    //親の点数aをPL1に付与
	score_PL2 += y;                    //子の点数bをPL2に付与
	sprintf_s(buf, "%d", score_PL1);     //PL1の点数をバッファに保存
	SetWindowText(hWndScore_pl1, buf);  //PL1の点数を表示
	sprintf_s(buf, "%d", score_PL2);   //PL2の点数をバッファに保存
	SetWindowText(hWndScore_pl2, buf); //PL2の点数を表示
}

void game_start(HWND hWnd) {

	turn = 0;

	rand0toi(card, 10);					//カード順番を設定する

	change(hWnd,0,0);

}

//手番交代
int change(HWND hWnd, int a, int b) {

	char buf[100];

	ChatReset(hWndSendMSG);
	ChatReset(hWndRecvMSG);
	ChatReset(hWndQuestion);
	ChatReset(hWndHelp);

	if (FlagPlayer % 2 == 1) {              //自分が親の場合
		if (Use_PL == 1) {
			set_score(b, a);
		}
		else if (Use_PL == 0) {
			set_score(a, b);
		}
		if (turn == 3) {
			score_judge(hWnd);
			return 0;
		}

		rule_num = randAtoC();
		dice_num = dice();

		enable_master();
		
		sprintf_s(buf, "%s%d", "RULE", rule_num);
		send(sock, buf, strlen(buf) + 1, 0);
		SetWindowText(hWndRule, rule[rule_num]);
		SetWindowText(hWndQuestion, card_text[card[turn]][dice_num]);
		if (rule_num == 0) {
			EnableWindow(hWndSendMSG, TRUE);
			EnableWindow(hWndSend, TRUE);
			EnableWindow(hWndClear, TRUE);	// [クリア]
			DrawableFlag = 1;
			SetWindowText(hWndHelp, "絵と文字でお題を表現してください。\r\n絵を消したい場合はクリアを押してください。");
		}
		else if (rule_num == 1) {
			EnableWindow(hWndSendMSG, TRUE);
			EnableWindow(hWndSend, TRUE);
			SetWindowText(hWndHelp, "文字でお題を表現してください。");

		}
		else if (rule_num == 2) {
			EnableWindow(hWndClear, TRUE);	// [クリア]
			DrawableFlag = 1;
			SetWindowText(hWndHelp, "絵でお題を表現してください。\r\n絵を消したい場合はクリアを押してください。");
		}

	}
	else if (FlagPlayer % 2 == 0) {
		
		if (Use_PL == 1) {
			set_score(a, b);
		}
		else if (Use_PL == 0) {
			set_score(b, a);
		}
		if (turn == 3) {
			score_judge(hWnd);
			return 0;
		}
		enable_player();
		SetWindowText(hWndHelp, "相手が何を表現しているか答えてください。\r\nわからない場合はギブアップを押してください。");

	}

	FlagPlayer++;
	turn++;
	return 0;
}

int score_judge(HWND hWnd){
	if (Use_PL == 1) {
		if (score_PL1 > score_PL2) {
			MessageBox(hWnd, "YOU WIN!!\n親の勝ちです！",
				"Information", MB_OK | MB_ICONINFORMATION);
		}
		else if (score_PL1 < score_PL2) {
			MessageBox(hWnd, "YOU LOSE!!\n子の勝ちです！",
				"Information", MB_OK | MB_ICONINFORMATION);
		}
		else if (score_PL1 == score_PL2) {
			MessageBox(hWnd, "引き分けです！",
				"Information", MB_OK | MB_ICONINFORMATION);
		}

	}
	else if (Use_PL == 0) {
		if (score_PL1 > score_PL2) {
			MessageBox(hWnd, "YOU LOSE!!\n親の勝ちです！",
				"Information", MB_OK | MB_ICONINFORMATION);
		}
		else if (score_PL1 < score_PL2) {
			MessageBox(hWnd, "YOU WIN!!\n子の勝ちです！",
				"Information", MB_OK | MB_ICONINFORMATION);
		}
		else if (score_PL1 == score_PL2) {
			MessageBox(hWnd, "引き分けです！",
				"Information", MB_OK | MB_ICONINFORMATION);
		}

	}
	FlagPlayer = 0;
	turn = 0;
	enable_end();
	return 0;
}


//  ソケット初期化処理
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

//  ソケット接続 (クライアント側)
BOOL SockConnect(HWND hWnd, LPCSTR host)
{
	SOCKADDR_IN cl_sin; // SOCKADDR_IN構造体

	// ソケットを開く
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {        // ソケット作成失敗
		MessageBox(hWnd, "Socket() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	memset(&cl_sin, 0x00, sizeof(cl_sin)); // 構造体初期化
	cl_sin.sin_family = AF_INET;           // インターネット
	cl_sin.sin_port = htons(PORT);       // ポート番号指定

	phe = gethostbyname(host); // アドレス取得

	if (phe == NULL) {
		MessageBox(hWnd, "gethostbyname() failed.",
			"Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}
	memcpy(&cl_sin.sin_addr, phe->h_addr, phe->h_length);

	// 非同期モード (接続)
	if (WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_CONNECT) == SOCKET_ERROR) {
		closesocket(sock);
		sock = INVALID_SOCKET;
		MessageBox(hWnd, "WSAAsyncSelect() failed",
			"Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	// 接続処理
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

//  接続待ち (サーバ側)
BOOL SockAccept(HWND hWnd)
{
	SOCKADDR_IN sv_sin;         // SOCKADDR_IN構造体

	// サーバ用ソケット
	sv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sv_sock == INVALID_SOCKET) { // ソケット作成失敗
		MessageBox(hWnd, "Socket() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	memset(&sv_sin, 0x00, sizeof(sv_sin));      // 構造体初期化
	sv_sin.sin_family = AF_INET;           // インターネット
	sv_sin.sin_port = htons(PORT);       // ポート番号指定
	sv_sin.sin_addr.s_addr = htonl(INADDR_ANY); // アドレス指定

	if (bind(sv_sock, (LPSOCKADDR)&sv_sin, sizeof(sv_sin)) == SOCKET_ERROR) {
		closesocket(sv_sock);
		sv_sock = INVALID_SOCKET;
		MessageBox(hWnd, "bind() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	if (listen(sv_sock, 5) == SOCKET_ERROR) {
		// 接続待ち失敗
		closesocket(sv_sock);
		sv_sock = INVALID_SOCKET;
		MessageBox(hWnd, "listen() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	// 非同期処理モード (接続待ち)
	if (WSAAsyncSelect(sv_sock, hWnd, WM_SOCKET, FD_ACCEPT) == SOCKET_ERROR) {
		closesocket(sv_sock);
		sv_sock = INVALID_SOCKET;
		MessageBox(hWnd, "WSAAsyncSelect() failed",
			"Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}
	return FALSE;
}

void enable_wait() {
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	// [切断]    
	EnableWindow(hWndRejectOrder, FALSE);	// [切断要請]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [ギブアップ]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, FALSE);	// [送信]  
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}

void enable_standby() {
	EnableWindow(hWndHost, TRUE);	// [HostName]
	EnableWindow(hWndConnect, TRUE);	// [接続]    
	EnableWindow(hWndAccept, TRUE);	// [接続待ち]
	EnableWindow(hWndReject, FALSE);	// [切断]    
	EnableWindow(hWndRejectOrder, FALSE);	// [切断要請]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [ギブアップ]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, FALSE);	// [送信]  
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}

void enable_master() {
	EnableWindow(hWndSendMSG, FALSE);	// [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	// [切断]    
	EnableWindow(hWndRejectOrder, TRUE);	// [切断要請]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [ギブアップ]
	EnableWindow(hWndCorrect, TRUE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, TRUE);	// [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, FALSE);	// [送信]  
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}

void enable_player() {
	EnableWindow(hWndSendMSG, TRUE);	// [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	// [切断]    
	EnableWindow(hWndRejectOrder, TRUE);	// [切断要請]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, TRUE);	// [ギブアップ]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, TRUE);	// [送信]  
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}

void enable_end() {
	EnableWindow(hWndSendMSG, FALSE);	// [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	// [切断]    
	EnableWindow(hWndRejectOrder, TRUE);	// [切断要請]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [ギブアップ]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, FALSE);	// [送信]  
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}

void enable_correct() {
	EnableWindow(hWndSendMSG, FALSE);	// [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	// [切断]    
	EnableWindow(hWndRejectOrder, TRUE);	// [切断要請]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [ギブアップ]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, TRUE);	// [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, FALSE);	// [送信]  
	EnableWindow(hWndChange, TRUE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}

void enable_pause() {
	EnableWindow(hWndSendMSG, FALSE);	// [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	// [切断]    
	EnableWindow(hWndRejectOrder, TRUE);	// [切断要請]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [ギブアップ]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, TRUE);	// [送信]  
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}

void enable_pointout_master() {
	EnableWindow(hWndSendMSG, TRUE);	// [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	// [切断]    
	EnableWindow(hWndRejectOrder, TRUE);	// [切断要請]
	DrawableFlag = 1;
	EnableWindow(hWndGiveup, FALSE);	// [ギブアップ]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, TRUE);	// [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, TRUE);	// [否認]   
	EnableWindow(hWndSend, TRUE);	// [送信]  
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}

void enable_pointout_player() {
	EnableWindow(hWndSendMSG, TRUE);	// [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	// [切断]    
	EnableWindow(hWndRejectOrder, TRUE);	// [切断要請]
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [ギブアップ]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, TRUE);	// [送信]  
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}
