//  ヘッダファイル


#include "Resource.h"
#include "framework.h"
#include "PuzzleChatGame.h"

//グローバル変数

LPCWSTR lpClassName = L" ";         // ウィンドウクラス名
LPCWSTR lpWindowName = L" ";        // タイトルバーにつく名前

SOCKET sock = INVALID_SOCKET;           // ソケット
SOCKET sv_sock = INVALID_SOCKET;        // サーバ用ソケット

HOSTENT* phe;							// HOSTENT構造体

HPEN hPenBlack;							// 黒ペン
HPEN hPenRed;							// 赤ペン


static HWND  hWndVS;			// VS文字
static HWND hWndWelcome;//WelcomeWindows
static HWND hWndHostName;//HostnameWindows
static HFONT hFont;//フォント指定Windows
static HWND hWndStart;//クライアントが開始するボタン

static HWND hWndHost;                       // ホスト名入力用エディットボックス
static HWND hWndQuestion;                   // お題表示用エディットボックス
static HWND hWndRule;                       // 制約条件用エディットボックス
static HWND hWndSendMSG;                    // 送信メッセージ入力用エディットボックス
static HWND hWndRecvMSG;                    // 受信メッセージ表示用エディットボックス
static HWND hWndScore_pl1;                  // 自分の得点用エディットボックス
static HWND hWndScore_pl2;                  // 相手の得点用エディットボックス
static HWND hWndHelp;                       // ヘルプメッセージ用エディットボックス
static HWND hWndConnect, hWndAccept;        // [接続]ボタンと[接続待ち]ボタン
static HWND hWndReject;                     // [切断]ボタン
static HWND hWndBack;						// [戻る]ボタン
static HWND hWndGiveup;						// [パス]ボタン
static HWND hWndCorrect;					// [正解]ボタン
static HWND hWndIncorrect;					// [不正解]ボタン
static HWND hWndPointout;					// [指摘]ボタン
static HWND hWndConsent;					// [承諾]ボタン
static HWND hWndDenial;						// [否認]ボタン
static HWND hWndChange;						// [交代]ボタン
static HWND hWndSend;                       // [送信]ボタン
static HWND hWndClear;                       

static int DrawableFlag = 0;	// 入力 1:可能/0:不可 状態確認フラグ

int PlayerRuleNum = -1;
int FlagPlayer;					//現在のPLが親であるか子であるかを確認するフラグ
int Use_PL = 0;					//PLナンバーを確認する
int score_PL1 = 0;				//PL1のスコア
int score_PL2 = 0;				//PL2のスコア
int turn = 0;					//手番
int dice_num;					//ダイスの結果
int rule_num=-1;				//ルールの番号

const RECT d = { 160, 110, 540, 440 };               // 描画領域(左上隅のx座標, 左上隅のy座標, 右下隅のx座標, 右下隅のy座標)
const RECT line = { 150, 110, 550, 450 };               // 描画領域(左上隅のx座標, 左上隅のy座標, 右下隅のx座標, 右下隅のy座標)
const RECT window_size = { 0, 0, 700, 600 };               // 描画領域(左上隅のx座標, 左上隅のy座標, 右下隅のx座標, 右下隅のy座標)

int n;                                              // カウンタ
int flag[MAX_ARRAY];                                // ペンダウンフラグ
POINT pos[MAX_ARRAY];                               // 座標を格納
HPEN colors[MAX_ARRAY];								// 使用する色

char rule[3][10] = { "絵と文字","文字のみ","絵のみ" };//ルールの内容
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
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);//ウィンドウ関数
LRESULT CALLBACK OnPaint(HWND, UINT, WPARAM, LPARAM);	//描画関数
LRESULT CALLBACK ClearPaint(HWND);//描画リセット関数

BOOL checkMousePos(int x, int y);						//マウスの位置がキャンパスの中かどうか判定する
void setData(int flag, int x, int y, HPEN color);       //描画情報を入れる
void FXY(int f, int x, int y);							//描画データを送信する関数
void ChatReset(HWND chatbox);							//チャットボックスをリセットする関数

int randAtoC();											//1～3のランダムな数字を返す
void rand0toi(int ary[], int size);						//i個の配列の順番をランダムに入れ替える
int dice();												//1～6のランダムな数字を返す

void game_start(HWND hWnd);								//ゲーム開始時に実行する関数
int change(HWND, int a, int b);							//手番を交代するときに実行する関数
int score_judge(HWND hWnd);								//勝利判定を行う関数

BOOL SockInit(HWND hWnd);                               // ソケット初期化
BOOL SockAccept(HWND hWnd);                             // ソケット接続待ち
BOOL SockConnect(HWND hWnd, LPCSTR host);               // ソケット接続
//void ColorSet(HWND hWnd, int flag); //描画区域に色指定で区別する

void enable_master();			//親の初期状態	
void enable_player();			//子の初期状態
void enable_correct();			//正解した時の子の状態
void enable_pause();			//正解ボタンを押した後の親の状態
void enable_pointout_master();	//指摘をされた後の親の状態
void enable_pointout_player();	//指摘をした後の子の状態

HINSTANCE hInst;                                // current instance


//WindowSDK 1909 version
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance); // ウィンドウクラス登録
BOOL                InitInstance(HINSTANCE, int);// インスタンス初期化
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);//  ウィンドウ関数(イベント処理を記述)
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);//ABOUT関数
//  WinMain関数 (Windowsプログラム起動時に呼ばれる関数)
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyRegisterClass(hInstance);// ウィンドウクラス登録

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))// インスタンス初期化
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PUZZLECHATGAME));//アクセルロード

	MSG msg;     // メッセージ

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))// メッセージを取得
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); // メッセージ送る
		}
	}

	return (int)msg.wParam;  // プログラム終了
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;// クラススタイル
	wcex.lpfnWndProc = WndProc;// ウィンドウ関数
	wcex.cbClsExtra = 0;// クラス拡張情報
	wcex.cbWndExtra = 0;// ウィンドウ拡張情報
	wcex.hInstance = hInstance;// インスタンス
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PUZZLECHATGAME));// アイコンハンドル
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);// マウスポインタ
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);// ウィンドウの背景色
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PUZZLECHATGAME);   // メニュー
	wcex.lpszClassName = lpClassName;   // クラス名
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));// スモールアイコン

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)// インスタンス初期化
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(
		lpClassName, // ウィンドウクラス名
		lpWindowName,      // ウィンドウ名
		WS_OVERLAPPEDWINDOW,// ウィンドウ属性
		100, // ウィンドウ表示位置(X)
		100,  // ウィンドウ表示位置(Y)
		WINDOW_W, // ウィンドウサイズ(X)
		WINDOW_H,    // ウィンドウサイズ(Y)
		nullptr,  // 親ウィンドウハンドル
		nullptr,
		hInstance,   // インスタンスハンドル
		nullptr
	);	// ウィンドウ生成


	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);// ウィンドウ表示モード
	UpdateWindow(hWnd);   // ウインドウ更新

	return TRUE;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG://ダイアログ初期化
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));//ダイアログ生成
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//  ウィンドウ関数(イベント処理を記述)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wP, LPARAM lP)
{
	static BOOL mouseFlg = FALSE;       //前回の状態 TRUE:描画した、FALSE:描画していない
	SOCKADDR_IN cl_sin;					//インターネット用ソケットアドレス
	int flag2, x2, y2;					//受け取った座標、フラグを格納する
	int len = sizeof(cl_sin);			//インターネット用ソケットアドレスの長さ
	char host[100];						//ホスト名
	char buf[100];						//受信内容を一時的に格納するバッファ
	int score_Master = 0;				//親のスコア	
	int score_Player = 0;				//子のスコア



	switch (uMsg) {

	case WM_CREATE:			// ウィンドウが生成された
	// 文字列表示
		rule_num = -1;
		// [接続]ボタン
		hWndConnect = CreateWindow("button", "参加する",
			WS_CHILD | WS_VISIBLE, 125, 325, 150, 150,
			hWnd, (HMENU)IDB_CONNECT, NULL, NULL);
		// [接続待ち]ボタン
		hWndAccept = CreateWindow("button", "参加者を待つ",
			WS_CHILD | WS_VISIBLE, 425, 325, 150, 150,
			hWnd, (HMENU)IDB_ACCEPT, NULL, NULL);
		//WELCOME!と表示
		hWndWelcome = CreateWindow(TEXT("static"), TEXT("WELCOME!"), WS_CHILD | WS_VISIBLE,
			200, 80, 300, 60, hWnd, NULL, NULL, NULL);
			hFont = CreateFont(60, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
		SendMessage(hWndWelcome, WM_SETFONT, WPARAM(hFont), TRUE);

		SockInit(hWnd);         // ソケット初期化
		hPenBlack = (HPEN)CreatePen(PS_SOLID, 3, RGB(0, 0, 0));		//黒ペン作成
		hPenRed = (HPEN)CreatePen(PS_SOLID, 3, RGB(255, 0, 0));		//赤ペン作成
		
		return 0L;

	case WM_COMMAND:				// ボタンが押された場合

		switch (LOWORD(wP)) {
		
		case IDB_ACCEPT:			// [接続待ち]ボタン押した場合(サーバー)
			//作成したウィンドウを破棄する
			DestroyWindow(hWndConnect);
			DestroyWindow(hWndAccept);
			DestroyWindow(hWndWelcome);

			// [戻る]ボタン
			hWndBack = CreateWindow("button", "BACK",
				WS_CHILD | WS_VISIBLE | WS_DISABLED, 550, 10, 124, 30,
				hWnd, (HMENU)IDB_BACK, NULL, NULL);
			//ウィンドウWelcomeにWAITING...と表示する
			hWndWelcome = CreateWindow(TEXT("static"), TEXT("WAITING..."), WS_CHILD | WS_VISIBLE,
				200, 200, 300, 60, hWnd, NULL, NULL, NULL);
			hFont = CreateFont(60, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
			SendMessage(hWndWelcome, WM_SETFONT, WPARAM(hFont), TRUE);
			EnableWindow(hWndBack, TRUE);	    // [切断]    
			if (SockAccept(hWnd)) { // 接続待ち要求
				return 0L;			// 接続待ち失敗

			}

			return 0L;

		case IDB_CONNECT:					// [接続]ボタン押した場合(クライアント)
			DestroyWindow(hWndConnect);
			DestroyWindow(hWndAccept);
			DestroyWindow(hWndWelcome);

			hWndWelcome = CreateWindow(TEXT("static"), TEXT("CONNECTING..."), WS_CHILD | WS_VISIBLE,
				140, 100, 400, 60, hWnd, NULL, NULL, NULL);
			hFont = CreateFont(60, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
			SendMessage(hWndWelcome, WM_SETFONT, WPARAM(hFont), TRUE);

			hWndHostName = CreateWindow(TEXT("static"), TEXT("Input IPAddress"), WS_CHILD | WS_VISIBLE,
				200, 200, 150, 30, hWnd, NULL, NULL, NULL);
			// ホスト名入力用エディットボックス
			hWndHost = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
				WS_CHILD | WS_VISIBLE, 350, 200, 150, 30,
				hWnd, (HMENU)IDF_HOSTNAME, NULL, NULL);
			hWndStart = CreateWindow("button", "START",
				WS_CHILD | WS_VISIBLE, 200, 300, 300, 140,
				hWnd, (HMENU)IDB_START, NULL, NULL);
			SendMessage(hWndStart, WM_SETFONT, WPARAM(hFont), TRUE);

			// [切断]ボタン
			hWndBack = CreateWindow("button", "BACK",
				WS_CHILD | WS_VISIBLE | WS_DISABLED, 550, 10, 124, 30,
				hWnd, (HMENU)IDB_BACK, NULL, NULL);
			EnableWindow(hWndBack, TRUE);	    // [切断]    
			return 0L;

		case IDB_START:
			EnableWindow(hWndReject, FALSE);	    // [切断]   
			GetWindowText(hWndHost, host, sizeof(host));
			if (SockConnect(hWnd, host)==FALSE) {	// 接続要求
				//ダブルクリック時のエラーを回避
				EnableWindow(hWndStart, FALSE);	    // [接続]  
				return 0L;
			}
			return 0L;

		case IDB_REJECT:					// [切断]ボタン押した場合

			if (sock != INVALID_SOCKET) {   // 自分がクライアント側なら
				closesocket(sock);			// ソケットを閉じる
				sock = INVALID_SOCKET;

			}
			if (sv_sock != INVALID_SOCKET) { // 自分がサーバ側なら
				closesocket(sv_sock);		 // サーバ用ソケットを閉じる
				sv_sock = INVALID_SOCKET;

			}
			phe = NULL;
			//以下のウィンドウを削除し初期状態の戻す
			DestroyWindow(hWndQuestion);                   // お題表示用エディットボックス
			DestroyWindow(hWndRule);                       // 制約条件用エディットボックス
			DestroyWindow(hWndSendMSG);                    // 送信メッセージ入力用エディットボックス
			DestroyWindow(hWndRecvMSG);                    // 受信メッセージ表示用エディットボックス
			DestroyWindow(hWndScore_pl1);                  // 自分の得点用エディットボックス
			DestroyWindow(hWndScore_pl2);                  // 相手の得点用エディットボックス
			DestroyWindow(hWndHelp);                       // ヘルプメッセージ用エディットボックス
			DestroyWindow(hWndReject);                     // [切断]ボタン
			DestroyWindow(hWndGiveup);						// [パス]ボタン
			DestroyWindow(hWndCorrect);					// [正解]ボタン
			DestroyWindow(hWndIncorrect);					// [不正解]ボタン
			DestroyWindow(hWndPointout);					// [指摘]ボタン
			DestroyWindow(hWndConsent);					// [承諾]ボタン
			DestroyWindow(hWndDenial);						// [否認]ボタン
			DestroyWindow(hWndChange);						// [交代]ボタン
			DestroyWindow(hWndSend);                       // [送信]ボタン
			DestroyWindow(hWndClear);
			DestroyWindow(hWndVS);
			
			ClearPaint(hWnd);			//描画ボックスをクリアする

			rule_num = -1;
			// [接続]ボタン
			hWndConnect = CreateWindow("button", "参加する",
				WS_CHILD | WS_VISIBLE, 125, 325, 150, 150,
				hWnd, (HMENU)IDB_CONNECT, NULL, NULL);
			// [接続待ち]ボタン
			hWndAccept = CreateWindow("button", "参加者を待つ",
				WS_CHILD | WS_VISIBLE, 425, 325, 150, 150,
				hWnd, (HMENU)IDB_ACCEPT, NULL, NULL);
			//WELCOME!と表示
			hWndWelcome = CreateWindow(TEXT("static"), TEXT("WELCOME!"), WS_CHILD | WS_VISIBLE,
				200, 100, 300, 60, hWnd, NULL, NULL, NULL);
			hFont = CreateFont(60, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
			SendMessage(hWndWelcome, WM_SETFONT, WPARAM(hFont), TRUE);

			SockInit(hWnd);         // ソケット初期化
			UpdateWindow(hWnd);                                 // ウインドウ更新
			InvalidateRect(hWnd, &window_size, TRUE);			//能動的に再描画する

			return 0L;

		case IDB_BACK:						// [戻る]ボタン押した場合

			if (sock != INVALID_SOCKET) {   // 自分がクライアント側なら
				closesocket(sock);			// ソケットを閉じる
				sock = INVALID_SOCKET;

			}
			if (sv_sock != INVALID_SOCKET) { // 自分がサーバ側なら
				closesocket(sv_sock);		 // サーバ用ソケットを閉じる
				sv_sock = INVALID_SOCKET;

			}
			DestroyWindow(hWndHostName);
			DestroyWindow(hWndWelcome);
			DestroyWindow(hWndHost);
			DestroyWindow(hWndStart);
			DestroyWindow(hWndBack);
			SendMessage(hWnd, WM_CREATE, 0, 0); // 切断処理発行
			return 0L;

		case IDB_SEND:											// [送信]ボタン押した場合
			char sender[100];

			GetWindowText(hWndSendMSG, buf, sizeof(buf) - 1);   //送信メッセージ入力欄の内容を取得
			sprintf_s(sender, "%s%s", "MESS", buf);				//送信の内容とその前にMESSを付けたものをバッファに保存
																// 送信処理
			if (send(sock, sender, strlen(sender) + 1, 0) == SOCKET_ERROR) {
				// 送信に失敗したらエラーを表示
				MessageBox(hWnd, TEXT("sending failed"), TEXT("Error"),
					MB_OK | MB_ICONEXCLAMATION);

			}
			ChatReset(hWndSendMSG);								//チャットボックスのクリア
			return 0L;

		case IDB_GIVEUP:									// [パス]ボタン押した場合
			strcpy_s(buf, "GIVEUP");						// パスの内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);			// GIVEUPと送信
			score_Master = 1;								// 親と子の点数を１点ずつ付与
			score_Player = 1;
			MessageBox(hWnd, "パスしました。",
				"Information", MB_OK | MB_ICONINFORMATION);	//パスしましたとメッセージボックスに表示する
			change(hWnd, score_Master, score_Player);		// 交代
			InvalidateRect(hWnd, &line, TRUE);			//能動的に再描画する
			return 0L;

		case IDB_CORRECT:								// [正解]押した場合
			strcpy_s(buf, "CORRECT");					// 正解の内容をバッファに保存
														//”CORRECT”と”答え：”、現在のお題を送信する
			sprintf_s(sender, "%s%s%s", "CORRECT", "答え:", card_text[card[turn - 1]][dice_num]);
			send(sock, sender, strlen(sender) + 1, 0);	//送信処理
			enable_pause();
			SetWindowText(hWndHelp, "相手の判断を待っています。");
			return 0L;

		case IDB_CLEAR:							// [クリア]を押した場合
			strcpy_s(buf, "CLEAR");				// クリアの内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);//送信処理
			ClearPaint(hWnd);		//画面のクリアを実行
			InvalidateRect(hWnd, &line, TRUE);		//能動的に再描画処理を実行
			return 0L;

		case IDB_INCORRECT:						// [不正解]を押した場合
			strcpy_s(buf, "INCORRECT");			// 不正解の内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);// INCORRECTと送信
			return 0L;

		case IDB_POINTOUT:						// [指摘]を押した場合
			strcpy_s(buf, "POINTOUT");			// 指摘の内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);// POINTOUTと送信	
			enable_pointout_player();
			SetWindowText(hWndHelp, "相手の判断を待っています。");
			return 0L;

		case IDB_CONSENT:						// [承諾]を押した場合
			strcpy_s(buf, "CONSENT");           // 承諾の内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);// CONSENTと送信
			score_Master = 0;                   // 親に0点、子に1点を付与
			score_Player = 1;
			// 交代
			change(hWnd, score_Master, score_Player);
			return 0L;

		case IDB_DENIAL:						// [否認]を押した場合
			strcpy_s(buf, "DENIAL");			// 否認の内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);// DENIALと送信

			if (rule_num == 0) {				//Aを正解
				score_Master = 1;				//親に1点、子に2点を付与
				score_Player = 2;

			}
			else if (rule_num == 1) {			//Bを正解
				score_Master = 1;				//親に1点、子に3点を付与
				score_Player = 3;

			}
			else if (rule_num == 2) {			//Cを正解
				score_Master = 1;				//親に1点、子に5点を付与
				score_Player = 5;

			}									// 交代
			change(hWnd, score_Master, score_Player);
			return 0L;

		case IDB_CHANGE:						// [交代]を押した場合
			strcpy_s(buf, "CHANGE");            // 交代の内容をバッファに保存
			send(sock, buf, strlen(buf) + 1, 0);// CHANGEと送信

			if (rule_num == 0) {				//Aを正解
				score_Master = 1;				//親に1点、子に2点を付与
				score_Player = 2;

			}
			else if (rule_num == 1) {			//Bを正解
				score_Master = 1;				//親に1点、子に3点を付与
				score_Player = 3;

			}
			else if (rule_num == 2) {			//Cを正解
				score_Master = 1;				//親に1点、子に5点を付与
				score_Player = 5;

			}									// 交代
			change(hWnd, score_Master, score_Player);
			return 0L;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wP, lP);
		} /* end of switch (LOWORD(wP)) */
		return 0L;

	case WM_LBUTTONDOWN:    // マウス左ボタンが押された
															// 描画領域の中で描画可能フラグが１であるならば
		if ((checkMousePos(LOWORD(lP), HIWORD(lP)) == TRUE) && (DrawableFlag == 1)) {
			setData(0, LOWORD(lP), HIWORD(lP), hPenBlack);  // 線の始点として座標を記録
			FXY(0, LOWORD(lP), HIWORD(lP));                 // 線の始点として座標を記録
			mouseFlg = TRUE;								//mouseFlagをTRUEとする
			n++;											//カウンタを増加する
			InvalidateRect(hWnd, &line, FALSE);				//能動的に再描画する

		}
		return 0L;

	case WM_MOUSEMOVE:  // マウスポインタが移動した

		if (wP == MK_LBUTTON) {  // 左ボタンが押されている
																	// 描画領域の中なら
			if ((checkMousePos(LOWORD(lP), HIWORD(lP)) == TRUE) && (DrawableFlag == 1)) {

				if (mouseFlg) {										// 前回描画しているなら
					setData(1, LOWORD(lP), HIWORD(lP), hPenBlack);   // 線の途中として座標を記録
					FXY(1, LOWORD(lP), HIWORD(lP));					// 線の途中として座標を送信

				}
				else {												// 前回描画していないなら
					setData(0, LOWORD(lP), HIWORD(lP), hPenBlack);  // 線の始点として座標を記録
					FXY(0, LOWORD(lP), HIWORD(lP));					// 線の始点として座標を送信

				}
				n++;												//カウンタを増加
				mouseFlg = TRUE;									//mouseFlagをTRUEとする
				InvalidateRect(hWnd, &line, FALSE);					//能動的に再描画をする

			}
			else {													// 描画領域の外なら
				mouseFlg = FALSE;									//mouseFlagをFALSEとする

			}

		}
		return 0L;

	case WM_SOCKET:          // 非同期処理メッセージ

		if (WSAGETSELECTERROR(lP) != 0) { return 0L; }
		switch (WSAGETSELECTEVENT(lP)) {
		case FD_ACCEPT:						// 接続待ち完了通知
		{									//接続要求の確認
			sock = accept(sv_sock, (LPSOCKADDR)&cl_sin, &len);

			if (sock == INVALID_SOCKET) {	//クライアント側のソケットがINVALID_SOCKETを示していたならば
											//エラーメッセージを表示する
				MessageBox(hWnd, "Accepting connection failed",
					"Error", MB_OK | MB_ICONEXCLAMATION);
				closesocket(sv_sock);		//サーバ用のソケットを閉じる
				sv_sock = INVALID_SOCKET;	//サーバ用のソケットをINVALID_SOCKETとする
				return 0L;

			}
#ifndef NO_DNS
			// ホスト名取得
			phe = gethostbyaddr((char*)&cl_sin.sin_addr, 4, AF_INET);
			if (phe) { SetWindowText(hWndHost, phe->h_name); }
#endif  NO_DNS
			// 非同期モード (受信＆切断）
			if (WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE) == SOCKET_ERROR) {
				// 接続に失敗したら初期状態に戻す
				//エラーメッセージを表示する
				MessageBox(hWnd, "WSAAsyncSelect() failed",
					"Error", MB_OK | MB_ICONEXCLAMATION);
				return 0L;

			}
			DestroyWindow(hWndWelcome);
			DestroyWindow(hWndBack);
			Use_PL = 1;						//PL１とする
			FlagPlayer = 1;					//親としてゲームを開始
			game_start(hWnd);				//親を0点子を0点としてゲームを開始する
			return 0L;

		}/* end of case FD_ACCEPT: */

		case FD_CONNECT:					// 接続完了通知
											// 非同期モード (受信＆切断)
			if (WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE) == SOCKET_ERROR) {
				// 接続に失敗したら初期状態に戻す
				//エラーメッセージを表示する
				MessageBox(hWnd, "WSAAsyncSelect() failed",
					"Error", MB_OK | MB_ICONEXCLAMATION);
				return 0L;
			}

			Use_PL = 0;						//PL２とする
			game_start(hWnd);				//通信親0点、子0点としてゲームを開始する
			return 0L;

		case FD_READ:										//メッセージ受信
			char b[100];									//bufの代わりに用いる

			if (recv(sock, buf, sizeof(buf) - 1, 0) != SOCKET_ERROR) {
				// 受信できたなら			
				if (strncmp(buf, "MESS", 4) == 0) {			//チャットボックスの内容が
															//送られた場合
					strncpy_s(b, buf + 4, sizeof(buf) - 4);	//4文字目以降の内容を変数bに格納
					SetWindowText(hWndRecvMSG, b);			//bの内容を送信
					return 0L;

				}
				else if (strncmp(buf, "RULE", 4) == 0) {	//ルールが送られてきた場合
					strncpy_s(b, buf + 4, sizeof(buf) - 4);	//4文字目以降の内容を変数bに格納
					rule_num = atoi(b);						//bからint型に変えたものをrule_numに格納
					SetWindowText(hWndRule, rule[rule_num]);//rule_numに対応したルールを提示する

					PlayerRuleNum = rule_num;
					DestroyWindow(hWndSend);
					DestroyWindow(hWndSendMSG);	//送信チャットボックスをリセットする
					DestroyWindow(hWndRecvMSG);			//受信チャットボックスをリセットする
					if (PlayerRuleNum == 0) {				//ルール１ならば文字と絵が使えるため
						// 受信メッセージ表示用エディットボックス
						
						hWndRecvMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
							WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 190, 455, 150, 70,
							hWnd, (HMENU)IDF_RECVMSG, NULL, NULL);
						// 送信メッセージ入力用エディットボックス
						hWndSendMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
							WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_DISABLED, 345, 455, 150, 70,
							hWnd, (HMENU)IDF_SENDMSG, NULL, NULL);
						// [送信]ボタン
						hWndSend = CreateWindow("button", "送信",
							WS_CHILD | WS_VISIBLE | WS_DISABLED, 505, 485, 50, 30,
							hWnd, (HMENU)IDB_SEND, NULL, NULL);
						
					}
					else if (PlayerRuleNum == 1) {

						// 受信メッセージ表示用エディットボックス
						hWndRecvMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
							WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 200, 150, 300, 150,
							hWnd, (HMENU)IDF_RECVMSG, NULL, NULL);
						// 送信メッセージ入力用エディットボックス
						hWndSendMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
							WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_DISABLED, 200, 310, 300, 150,
							hWnd, (HMENU)IDF_SENDMSG, NULL, NULL);
						// [送信]ボタン
						hWndSend = CreateWindow("button", "送信",
							WS_CHILD | WS_VISIBLE | WS_DISABLED, 325, 470, 50, 30,
							hWnd, (HMENU)IDB_SEND, NULL, NULL);
						
					}
					else if (PlayerRuleNum == 2) {
						//enable_color();
						// 送信メッセージ入力用エディットボックス
						hWndSendMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
							WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_DISABLED, 190, 455, 300, 70,
							hWnd, (HMENU)IDF_SENDMSG, NULL, NULL);
						// [送信]ボタン
						hWndSend = CreateWindow("button", "送信",
							WS_CHILD | WS_VISIBLE | WS_DISABLED, 500, 485, 50, 30,
							hWnd, (HMENU)IDB_SEND, NULL, NULL);
					}
					enable_player();			//子の初期状態にする
					InvalidateRect(hWnd, &line, TRUE);			//能動的に再描画する
					return 0L;

				}
				else if (strcmp(buf, "REJECT") == 0) {		//REJECTが送られてきた場合
														   //切断要請が来たとメッセージボックスに表示する
					MessageBox(hWnd, "切断要請がきました。",
						"Information", MB_OK | MB_ICONINFORMATION);
					return 0L;

				}
				else if (strncmp(buf, "CORRECT", 7) == 0) {//正解だった場合
					strncpy_s(b, buf + 7, sizeof(buf) - 7);	//7文字目以降の内容を変数bに格納
					SetWindowText(hWndQuestion, b);			//問題の内容を受信ボックスに貼る
					MessageBox(hWnd, "正解です！",			//メッセージボックスに正解であることを表示する
						"Information", MB_OK | MB_ICONINFORMATION);
					enable_correct();						//正解時の状態にする
					SetWindowText(hWndHelp, "相手が違反行為をしていたと感じたら指摘を押してください。\r\n問題ない場合は交代を押してください。");
					return 0L;

				}
				else if (strcmp(buf, "POINTOUT") == 0) {	//指摘が来た場合
					MessageBox(hWnd, "指摘点があります！",	//メッセージボックスに指摘がある旨を表示
						"Information", MB_OK | MB_ICONINFORMATION);
					enable_pointout_master();				//指摘が来た時の状態にする
					SetWindowText(hWndHelp, "指摘を認める場合は承認を押してください。\r\n認めない場合は否認を押してください。");
					return 0L;

				}
				else if (strcmp(buf, "CLEAR") == 0) {		//描画ボックスをクリアすることを求められた場合
					ClearPaint(hWnd);			//描画ボックスをクリアする
					InvalidateRect(hWnd, &line, TRUE);			//能動的に再描画する
					return 0L;

				}
				else if (strcmp(buf, "CHANGE") == 0) {		//交代する場合
					if (rule_num == 0) {					//Aを正解
						score_Master = 1;					//親に1点、子に2点を付与
						score_Player = 2;

					}
					else if (rule_num == 1) {				//Bを正解
						score_Master = 1;					//親に1点、子に3点を付与
						score_Player = 3;

					}
					else if (rule_num == 2) {				//Cを正解
						score_Master = 1;					//親に1点、子に5点を付与
						score_Player = 5;

					}										//交代する

					change(hWnd, score_Master, score_Player);
					return 0L;

				}
				else if (strcmp(buf, "DENIAL") == 0) {		//指摘を否定した場合

					if (rule_num == 0) {					//Aを正解
						score_Master = 1;					//親に1点、子に2点を付与
						score_Player = 2;

					}
					else if (rule_num == 1) {				//Bを正解
						score_Master = 1;					//親に1点、子に3点を付与
						score_Player = 3;

					}
					else if (rule_num == 2) {				//Cを正解
						score_Master = 1;					//親に1点、子に5点を付与
						score_Player = 5;

					}

					change(hWnd, score_Master, score_Player);//交代する
					return 0L;

				}
				else if (strcmp(buf, "CONSENT") == 0) {	//指摘を承認
					score_Master = 0;						//親に0点、子に1点を付与
					score_Player = 1;
					change(hWnd, score_Master, score_Player);//交代する
					return 0L;

				}
				else if (strcmp(buf, "GIVEUP") == 0) {		//パス
					score_Master = 1;						//親に1点、子に1点を付与
					score_Player = 1;
					//相手がパスした旨をメッセージボックスに表示
					change(hWnd, score_Master, score_Player);//交代する
					MessageBox(hWnd, "相手がパスしました",
						"Information", MB_OK | MB_ICONINFORMATION);
					return 0L;

				}
				else if (strcmp(buf, "POINTOUT") == 0) {	//指摘が来た場合
					MessageBox(hWnd, "指摘点があります！",	//指摘が来た旨をメッセージボックスに表示
						"Information", MB_OK | MB_ICONINFORMATION);
					return 0L;

				}
				else if (strcmp(buf, "INCORRECT") == 0) {	//不正解であるならば
					strncpy_s(buf, "不正解です", 10);		//不正解である旨を受信チャットボックスに表示
					SetWindowText(hWndQuestion, buf);
					return 0L;

				}
				else {										//それ以外ならば
					sscanf_s(buf, "%1d%03d%03d", &flag2, &x2, &y2);
					//座標を個別に獲得する
					setData(flag2, x2, y2, hPenRed);		//赤ペンで描画できるようにデータを保存する
					n++;									//カウンタを増加する
					InvalidateRect(hWnd, &line, FALSE);		//能動的に再描画する
					return 0L;

				}

			}

			return 0L;

		case FD_CLOSE:										// 切断された場合
			MessageBox(hWnd, "切断されました。",			//切断された旨をメッセージボックスに表示
				"Information", MB_OK | MB_ICONINFORMATION);
			SendMessage(hWnd, WM_COMMAND, IDB_REJECT, 0); // 切断処理発行

			return 0L;

		}/* end of switch (WSAGETSELECTEVENT(lP)) */
		return 0L;

	case WM_DESTROY:				// ウィンドウが破棄された
		DeleteObject(hPenBlack);    // 黒ペン削除
		DeleteObject(hPenRed);      // 赤ペン削除

		closesocket(sock);          // ソケットを閉じる
		PostQuitMessage(0);         // 終了処理
		return 0L;

	case WM_PAINT:      // 再描画
		return OnPaint(hWnd, uMsg, wP, lP);  // 描画関数の呼び出し
		return 0L;

	default:
		return DefWindowProc(hWnd, uMsg, wP, lP);  // 標準メッセージ処理

	}/* end of switch (uMsg) */

}

//  描画関数
LRESULT CALLBACK OnPaint(HWND hWnd, UINT uMsg, WPARAM wP, LPARAM lP)
{
	HDC hdc;							//デバイスコンテキスト
	PAINTSTRUCT ps;						//ペイントストラクト

	hdc = BeginPaint(hWnd, &ps);		//描画の開始

	MoveToEx(hdc, d.left, d.top, NULL);
	if (rule_num == 0 || rule_num == 2) {
		LineTo(hdc, d.right, d.top);    // 上横線
		LineTo(hdc, d.right, d.bottom); // 右縦線
		LineTo(hdc, d.left, d.bottom);  // 下横線
		LineTo(hdc, d.left, d.top);		// 左縦線
		UpdateWindow(hWnd);                                 // ウインドウ更新

	}
	for (int i = 0; i < n; i++) {		// 線を描画
		SelectObject(hdc, colors[i]);
		if (flag[i] == 0) {				// 開始点なら、始点を移動
			MoveToEx(hdc, pos[i].x, pos[i].y, NULL);

		}
		else {							// 途中の点なら線を引く
			LineTo(hdc, pos[i].x, pos[i].y);

		}
	}

	EndPaint(hWnd, &ps);				//描画終了
	return 0L;

}

LRESULT CALLBACK ClearPaint(HWND hWnd)
{
	HDC hdc;						//デバイスコンテキスト
	PAINTSTRUCT ps;					//ペイントストラクト

	hdc = BeginPaint(hWnd, &ps);	//描画開始
	// 描画領域の初期化
	MoveToEx(hdc, d.left, d.top, NULL);
	if (rule_num == 0||rule_num==2) {		
		LineTo(hdc, d.right, d.top);    // 上横線
		LineTo(hdc, d.right, d.bottom); // 右縦線
		LineTo(hdc, d.left, d.bottom);  // 下横線
		LineTo(hdc, d.left, d.top);		// 左縦線

	}
	
	for (int i = 0; i < n; i++) {   // 全ての情報をリセットする
		flag[i] = 0;
		pos[i].x = 0;
		pos[i].y = 0;
		colors[i] = NULL;

	}

	n = 0;							//カウンタをリセットする
	EndPaint(hWnd, &ps);			//描画終了
	return 0L;

}

//  描画情報を格納
void setData(int f, int x, int y, HPEN color)
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

//１～３の値をランダムに返す
int randAtoC() {
	srand(time(0));
	int A2C[] = { 0,1,2 };
	return A2C[rand() % 3];

}

//引数としてsize個の配列をランダムな形にする
void rand0toi(int ary[], int size)
{

	for (int i = 0; i < size; i++) {
		int j = rand() % size;
		int t = ary[i];
		ary[i] = ary[j];
		ary[j] = t;

	}

}

//1～6の値をランダムに返す
int dice() {
	srand(time(0));
	int DICE[] = { 0,1,2,3,4,5 };
	return DICE[rand() % 6];

}

//チャットボックスの内容を消去
void ChatReset(HWND chatbox) {
	SetWindowText(chatbox, TEXT(""));
	SetFocus(chatbox);

}

//点数を各プレイヤーに与える
void set_score(int x, int y) {

	char buf[100];

	score_PL1 += x;						//親の点数aをPL1に付与
	score_PL2 += y;						//子の点数bをPL2に付与
	sprintf_s(buf, "%d", score_PL1);    //PL1の点数をバッファに保存
	SetWindowText(hWndScore_pl1, buf);  //PL1の点数を表示
	sprintf_s(buf, "%d", score_PL2);	//PL2の点数をバッファに保存
	SetWindowText(hWndScore_pl2, buf);	//PL2の点数を表示

}

//ゲーム開始時に実行
void game_start(HWND hWnd) {
	DestroyWindow(hWndHostName);
	DestroyWindow(hWndWelcome);
	DestroyWindow(hWndHost);
	DestroyWindow(hWndStart);
	DestroyWindow(hWndBack);
	turn = 0;				//手番を0にする

	// 制約条件用エディットボックス
	hWndRule = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
		WS_CHILD | WS_VISIBLE | ES_READONLY, 35, 10, 100, 30,
		hWnd, (HMENU)IDF_RULE, NULL, NULL);
	hFont = CreateFont(23, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
	SendMessage(hWndRule, WM_SETFONT, WPARAM(hFont), TRUE);
	// お題用エディットボックス
	hWndQuestion = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
		WS_CHILD | WS_VISIBLE | ES_READONLY, 10, 45, 150, 30,
		hWnd, (HMENU)IDF_QUESTION, NULL, NULL);
	hFont = CreateFont(23, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
	SendMessage(hWndQuestion, WM_SETFONT, WPARAM(hFont), TRUE);
	// 自分の得点用エディットボックス
	hWndScore_pl1 = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
		WS_CHILD | WS_VISIBLE | ES_READONLY, 255, 25, 40, 50,
		hWnd, (HMENU)IDF_SCORE_PL1, NULL, NULL);
	hFont = CreateFont(50, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
	SendMessage(hWndScore_pl1, WM_SETFONT, WPARAM(hFont), TRUE);
	//VS
	hWndVS = CreateWindow("static", "VS", WS_CHILD | WS_VISIBLE, 318, 25, 64, 50, hWnd, NULL, NULL, NULL);
	hFont = CreateFont(50, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
	SendMessage(hWndVS, WM_SETFONT, WPARAM(hFont), TRUE);
	// 相手の得点用エディットボックス
	hWndScore_pl2 = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
		WS_CHILD | WS_VISIBLE | ES_READONLY, 405, 25, 40, 50,
		hWnd, (HMENU)IDF_SCORE_PL2, NULL, NULL);
	hFont = CreateFont(50, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
	SendMessage(hWndScore_pl2, WM_SETFONT, WPARAM(hFont), TRUE);

	
	// [切断]ボタン
	hWndReject = CreateWindow("button", "切断",
		WS_CHILD | WS_VISIBLE | WS_DISABLED, 550, 10, 124, 30,
		hWnd, (HMENU)IDB_REJECT, NULL, NULL);
	// [交代]ボタン
	hWndChange = CreateWindow("button", "交代",
		WS_CHILD | WS_VISIBLE | WS_DISABLED, 550, 44, 60, 35,
		hWnd, (HMENU)IDB_CHANGE, NULL, NULL);
	// [パス]ボタン
	hWndGiveup = CreateWindow("button", "パス",
		WS_CHILD | WS_VISIBLE | WS_DISABLED, 620, 44, 60, 35,
		hWnd, (HMENU)IDB_GIVEUP, NULL, NULL);

	// [正解]ボタン
	hWndCorrect = CreateWindow("button", "正解",
		WS_CHILD | WS_VISIBLE | WS_DISABLED, 17, 156, 66, 41,
		hWnd, (HMENU)IDB_CORRECT, NULL, NULL);
	// [不正解]ボタン
	hWndIncorrect = CreateWindow("button", "不正解",
		WS_CHILD | WS_VISIBLE | WS_DISABLED, 17, 222, 66, 41,
		hWnd, (HMENU)IDB_INCORRECT, NULL, NULL);
	// [指摘]ボタン
	hWndPointout = CreateWindow("button", "指摘",
		WS_CHILD | WS_VISIBLE | WS_DISABLED, 600, 156, 66, 41,
		hWnd, (HMENU)IDB_POINTOUT, NULL, NULL);
	// [承諾]ボタン
	hWndConsent = CreateWindow("button", "承諾",
		WS_CHILD | WS_VISIBLE | WS_DISABLED, 600, 222, 66, 41,
		hWnd, (HMENU)IDB_CONSENT, NULL, NULL);
	// [否認]ボタン
	hWndDenial = CreateWindow("button", "否認",
		WS_CHILD | WS_VISIBLE | WS_DISABLED, 600, 288, 66, 41,
		hWnd, (HMENU)IDB_DENIAL, NULL, NULL);
	// [クリア]ボタン
	hWndClear = CreateWindow("button", "クリア",
		WS_CHILD | WS_VISIBLE | WS_DISABLED, 600, 354, 66, 41,
		hWnd, (HMENU)IDB_CLEAR, NULL, NULL);
	// ヘルプメッセージ用エディットボックス
	hWndHelp = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "",
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 20, 288, 130, 200,
		hWnd, (HMENU)IDF_HELP, NULL, NULL);


	rand0toi(card, 10);		//カード順番を設定する
	change(hWnd, 0, 0);		//互いに0点として手番交代の手順を実行する

}



//手番交代

int change(HWND hWnd, int a, int b) {

	char buf[100];

	DestroyWindow(hWndSend);
	DestroyWindow(hWndSendMSG);	//送信チャットボックスをリセットする
	DestroyWindow(hWndRecvMSG);			//受信チャットボックスをリセットする
	ChatReset(hWndQuestion);		//お題をリセットする
	ChatReset(hWndHelp);                    //ヘルプメッセージボックスをリセットする
	ClearPaint(hWnd);				//描画ボックスをクリアする
	InvalidateRect(hWnd, &window_size, TRUE);	//能動的に再描画する



	if (FlagPlayer % 2 == 1) {      //自分が親の場合
		if (Use_PL == 1) {			//もしPL1ならば	
			set_score(b, a);		//bをPL1の、aをPL2の得点に加える

		}
		else if (Use_PL == 0) {	//もしPL2ならば
			set_score(a, b);		//aをPL1の、bをPL2の得点に加える

		}
		if (turn == 3) {			//これが指定ターン数を迎えたならば
			score_judge(hWnd);		//勝利判定を行い終了する
			return 0;

		}
		rule_num = randAtoC();		//ルール番号を決める
		dice_num = dice();			//１～６の数字を決める
		sprintf_s(buf, "%s%d", "RULE", rule_num); //”RULE”とルール番号bufに格納する
		//bufを送信する
		send(sock, buf, strlen(buf) + 1, 0);
		//その後、ルール番号に対応したルール、
		//ダイスに対応したカードに書かれたお題を表示する
		SetWindowText(hWndRule, rule[rule_num]);
		SetWindowText(hWndQuestion, card_text[card[turn]][dice_num]);
		enable_master();			//親の初期状態にする
		if (rule_num == 0) {				//ルール１ならば文字と絵が使えるため

			DrawableFlag = 1;				//描画可能フラグを１にする
			SetWindowText(hWndHelp, "絵と文字でお題を表現してください。\r\n絵を消したい場合はクリアを押してください。");
			//enable_color();
			// 受信メッセージ表示用エディットボックス
			hWndRecvMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 190, 455, 150, 70,
				hWnd, (HMENU)IDF_RECVMSG, NULL, NULL);
			// 送信メッセージ入力用エディットボックス
			hWndSendMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_DISABLED, 345, 455, 150, 70,
				hWnd, (HMENU)IDF_SENDMSG, NULL, NULL);
			// [送信]ボタン
			hWndSend = CreateWindow("button", "送信",
				WS_CHILD | WS_VISIBLE | WS_DISABLED, 505, 485, 50, 30,
				hWnd, (HMENU)IDB_SEND, NULL, NULL);
			EnableWindow(hWndSendMSG, TRUE);//送信用チャットボックス、
			EnableWindow(hWndSend, TRUE);	//[送信]ボタン、
			EnableWindow(hWndClear, TRUE);	//[クリア]ボタンを有効にする
			//ColorSet(hWnd, 1);
		}
		else if (rule_num == 1) {

			SetWindowText(hWndHelp, "文字でお題を表現してください。");

			// 受信メッセージ表示用エディットボックス
			hWndRecvMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 200, 150, 300, 150,
				hWnd, (HMENU)IDF_RECVMSG, NULL, NULL);
			// 送信メッセージ入力用エディットボックス
			hWndSendMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_DISABLED, 200, 310, 300, 150,
				hWnd, (HMENU)IDF_SENDMSG, NULL, NULL);
			// [送信]ボタン
			hWndSend = CreateWindow("button", "送信",
				WS_CHILD | WS_VISIBLE | WS_DISABLED, 325, 470, 50 ,30,
				hWnd, (HMENU)IDB_SEND, NULL, NULL);
			EnableWindow(hWndSendMSG, TRUE);//送信用チャットボックス、
			EnableWindow(hWndSend, TRUE);	//[送信]ボタンを有効にする
			//ColorSet(hWnd, 0);
		}
		else if (rule_num == 2) {
			//enable_color();
			DrawableFlag = 1;				//描画可能フラグを１にする
			SetWindowText(hWndHelp, "絵でお題を表現してください。\r\n絵を消したい場合はクリアを押してください。");
			// 受信メッセージ表示用エディットボックス
			hWndRecvMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 190, 455, 300, 70,
				hWnd, (HMENU)IDF_RECVMSG, NULL, NULL);
			EnableWindow(hWndClear, TRUE);	// [クリア]ボタンを有効にし、
			//ColorSet(hWnd, 1);
		}
		
	}
	else if (FlagPlayer % 2 == 0) {//子ならば

		if (Use_PL == 1) {			//もしPL1ならば
			set_score(a, b);		//aをPL1の、bをPL2の得点に加える

		}
		else if (Use_PL == 0) {	//もしPL2ならば
			set_score(b, a);		//bをPL1の、aをPL2の得点に加える

		}

		if (turn == 3) {			//これが指定ターン数を迎えたならば
			score_judge(hWnd);		//勝利判定を行い終了する
			return 0;

		}
		if (PlayerRuleNum == 0) {				//ルール１ならば文字と絵が使えるため
// 受信メッセージ表示用エディットボックス
			//enable_color();
			hWndRecvMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 190, 455, 150, 70,
				hWnd, (HMENU)IDF_RECVMSG, NULL, NULL);
			// 送信メッセージ入力用エディットボックス
			hWndSendMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_DISABLED, 345, 455, 150, 70,
				hWnd, (HMENU)IDF_SENDMSG, NULL, NULL);
			// [送信]ボタン
			hWndSend = CreateWindow("button", "送信",
				WS_CHILD | WS_VISIBLE | WS_DISABLED, 505, 485, 50, 30,
				hWnd, (HMENU)IDB_SEND, NULL, NULL);
			
		}
		else if (PlayerRuleNum == 1) {

			// 受信メッセージ表示用エディットボックス
			hWndRecvMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY, 200, 150, 300, 150,
				hWnd, (HMENU)IDF_RECVMSG, NULL, NULL);
			// 送信メッセージ入力用エディットボックス
			hWndSendMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_DISABLED, 200, 310, 300, 150,
				hWnd, (HMENU)IDF_SENDMSG, NULL, NULL);
			// [送信]ボタン
			hWndSend = CreateWindow("button", "送信",
				WS_CHILD | WS_VISIBLE | WS_DISABLED, 325, 470, 50, 30,
				hWnd, (HMENU)IDB_SEND, NULL, NULL);
		}
		else if (PlayerRuleNum == 2) {
			//enable_color();
			// 送信メッセージ入力用エディットボックス
			hWndSendMSG = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT(""),
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_DISABLED, 190, 455, 300, 70,
				hWnd, (HMENU)IDF_SENDMSG, NULL, NULL);
			// [送信]ボタン
			hWndSend = CreateWindow("button", "送信",
				WS_CHILD | WS_VISIBLE | WS_DISABLED, 500, 485, 50, 30,
				hWnd, (HMENU)IDB_SEND, NULL, NULL);
		}
		enable_player();			//子の初期状態にする
		SetWindowText(hWndHelp, "相手が何を表現しているか答えてください。\r\nわからない場合はパスを押してください。");

	}

	FlagPlayer++;		//処理が終わったならば次の親、子を決める
	turn++;				//そして手番を増やし終了する
	return 0;

}

//勝利判定を行う
int score_judge(HWND hWnd) {

	if (Use_PL == 1) {						//自分がPL1ならば

		if (score_PL1 > score_PL2) {		//PL1の点数が多ければ勝ち
			MessageBox(hWnd, "YOU WIN!!\n親の勝ちです！",
				"Information", MB_OK | MB_ICONINFORMATION);

		}
		else if (score_PL1 < score_PL2) {	//PL1の点数が低ければ負け
			MessageBox(hWnd, "YOU LOSE!!\n子の勝ちです！",
				"Information", MB_OK | MB_ICONINFORMATION);

		}
		else if (score_PL1 == score_PL2) {	//PL1の点数がPL2と同じならば引き分け
										   //という旨をメッセージボックスに表示
			MessageBox(hWnd, "引き分けです！",
				"Information", MB_OK | MB_ICONINFORMATION);
		}

	}

	else if (Use_PL == 0) {					//自分がPL2ならば

		if (score_PL1 > score_PL2) {		//PL2の点数が低ければ負け
			MessageBox(hWnd, "YOU LOSE!!\n親の勝ちです！",
				"Information", MB_OK | MB_ICONINFORMATION);

		}
		else if (score_PL1 < score_PL2) {	//PL2の点数が多ければ勝ち
			MessageBox(hWnd, "YOU WIN!!\n子の勝ちです！",
				"Information", MB_OK | MB_ICONINFORMATION);

		}
		else if (score_PL1 == score_PL2) {	//PL1の点数がPL2と同じならば引き分け
										   //という旨をメッセージボックスに表示
			MessageBox(hWnd, "引き分けです！",
				"Information", MB_OK | MB_ICONINFORMATION);

		}

	}

	FlagPlayer = 0;		//親と子をどちらも0とし
	turn = 0;			//手番もリセット
	return 0;

}

//  ソケット初期化処理
BOOL SockInit(HWND hWnd)
{
	WSADATA wsa;		//WSAStartupが初期化した状態を格納する
	int ret;			//WSAStartupの返り値を格納する
	char ret_buf[80];	//retのエラー内容を格納し、文字列化する

						//retを初期化する
	ret = WSAStartup(MAKEWORD(1, 1), &wsa);

	if (ret != 0) {		//もしretが0出なければ
						//エラー内容を表示し、プログラムを終了する
		wsprintf(ret_buf, "%d is the err", ret);
		MessageBox(hWnd, ret_buf, "Error", MB_OK | MB_ICONSTOP);
		exit(-1);
	}
	return FALSE;		//正常に行われた場合FALSEを返す
}

//  ソケット接続 (クライアント側)
BOOL SockConnect(HWND hWnd, LPCSTR host)
{
	SOCKADDR_IN cl_sin;				// SOCKADDR_IN構造体

									// ソケットを開く
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET) {   // ソケット作成失敗した場合
									//エラー内容を表示し、TRUEを返す
		MessageBox(hWnd, "Socket() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	memset(&cl_sin, 0x00, sizeof(cl_sin));	// 構造体初期化
	cl_sin.sin_family = AF_INET;			// インターネット
	cl_sin.sin_port = htons(PORT);			// ポート番号指定

	phe = gethostbyname(host);				// アドレス取得

	if (phe == NULL) {						//pheがNULLならばエラー内容を表示し、TRUEを返す
		MessageBox(hWnd, "gethostbyname() failed.",
			"Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;

	}
	//cl_sinのsin_addrにpheのh_addrをh_length分格納する
	memcpy(&cl_sin.sin_addr, phe->h_addr, phe->h_length);

	// 非同期モード (接続)
											//WSAAsyncSelectの返り値がSOCKET_ERRORならば
	if (WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_CONNECT) == SOCKET_ERROR) {
		closesocket(sock);					//ソケットを閉じ
		sock = INVALID_SOCKET;				//ソケットをINVALID_SOCKETとし
											//エラー内容を表示し、TRUEを返す
		MessageBox(hWnd, "WSAAsyncSelect() failed",
			"Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;

	}

	// 接続処理
											//connectを実行し、返り値がSOCKET_ERRORであり
	if (connect(sock, (LPSOCKADDR)&cl_sin, sizeof(cl_sin)) == SOCKET_ERROR) {
		//WSAGetLastErrorがWSAEWOULDLOCKでなければ
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			closesocket(sock);				//ソケットを閉じ
			sock = INVALID_SOCKET;			//ソケットをINVALID_SOCKETとし
											//エラー内容を表示し、TRUEを返す
			MessageBox(hWnd, "connect() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
			return TRUE;

		}

	}

	return FALSE;		//全てのエラー処理を通過したならばFALSEを返す

}

//  接続待ち (サーバ側)
BOOL SockAccept(HWND hWnd)
{
	SOCKADDR_IN sv_sin;						// SOCKADDR_IN構造体

	// サーバ用ソケット
	sv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sv_sock == INVALID_SOCKET) {		// ソケット作成失敗した場合
											//エラー内容を表示し、TRUEを返す
		MessageBox(hWnd, "Socket() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;

	}

	memset(&sv_sin, 0x00, sizeof(sv_sin));      // 構造体初期化
	sv_sin.sin_family = AF_INET;				// インターネット
	sv_sin.sin_port = htons(PORT);				// ポート番号指定
	sv_sin.sin_addr.s_addr = htonl(INADDR_ANY); // アドレス指定

												//bindの返り値がSOCKET_ERRORならば
	if (bind(sv_sock, (LPSOCKADDR)&sv_sin, sizeof(sv_sin)) == SOCKET_ERROR) {
		closesocket(sv_sock);					//ソケットを閉じ
		sv_sock = INVALID_SOCKET;				//ソケットをINVALID_SOCKETとし
												//エラー内容を表示し、TRUEを返す
		MessageBox(hWnd, "bind() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;

	}

	if (listen(sv_sock, 5) == SOCKET_ERROR) {	//もしlistenの返り値がSOCKET_ERRORならば
												// 接続待ち失敗として、
		closesocket(sv_sock);					//ソケットを閉じ
		sv_sock = INVALID_SOCKET;				//ソケットをINVALID_SOCKETとし
												//エラー内容を表示し、TRUEを返す
		MessageBox(hWnd, "listen() failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;

	}

	// 非同期処理モード (接続待ち)
												//WSAAsyncSelectの返り値がSOCKET_ERRORならば
	if (WSAAsyncSelect(sv_sock, hWnd, WM_SOCKET, FD_ACCEPT) == SOCKET_ERROR) {
		closesocket(sv_sock);					//ソケットを閉じ
		sv_sock = INVALID_SOCKET;				//ソケットをINVALID_SOCKETとし
												//エラー内容を表示し、TRUEを返す
		MessageBox(hWnd, "WSAAsyncSelect() failed",
			"Error", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;

	}

	return FALSE;		//全てのエラー処理を通過したならばFALSEを返す

}




void enable_master() {
	EnableWindow(hWndSendMSG, FALSE);	// [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	    // [切断]    
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [パス]
	EnableWindow(hWndCorrect, TRUE);	    // [正解ボタン]    
	EnableWindow(hWndIncorrect, TRUE);	    // [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, FALSE);	// [送信]  
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}

void enable_player() {
	EnableWindow(hWndSendMSG, TRUE);	    // [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	    // [切断]    
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, TRUE);	    // [パス]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, TRUE);	    // [送信]  
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}


void enable_correct() {
	EnableWindow(hWndSendMSG, FALSE);	// [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	    // [切断]    
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [パス]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, TRUE);	    // [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, FALSE);	// [送信]  
	EnableWindow(hWndChange, TRUE);	    // [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}

void enable_pause() {
	EnableWindow(hWndSendMSG, FALSE);	// [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	    // [切断]    
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [パス]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, TRUE);	    // [送信]  
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);    // [クリア]
}

void enable_pointout_master() {
	EnableWindow(hWndSendMSG, TRUE);	    // [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	    // [切断]    
	DrawableFlag = 1;
	EnableWindow(hWndGiveup, FALSE);	// [パス]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, TRUE);	    // [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, TRUE);	    // [否認]   
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}

void enable_pointout_player() {
	EnableWindow(hWndSendMSG, TRUE);	    // [送信用チャットボックス]
	EnableWindow(hWndHost, FALSE);	// [HostName]
	EnableWindow(hWndConnect, FALSE);	// [接続]    
	EnableWindow(hWndAccept, FALSE);	// [接続待ち]
	EnableWindow(hWndReject, TRUE);	    // [切断]    
	DrawableFlag = 0;
	EnableWindow(hWndGiveup, FALSE);	// [パス]
	EnableWindow(hWndCorrect, FALSE);	// [正解ボタン]    
	EnableWindow(hWndIncorrect, FALSE);	// [不正解ボタン]
	EnableWindow(hWndConsent, FALSE);	// [承諾]    
	EnableWindow(hWndPointout, FALSE);	// [指摘]    
	EnableWindow(hWndDenial, FALSE);	// [否認]   
	EnableWindow(hWndSend, TRUE);	    // [送信]  
	EnableWindow(hWndChange, FALSE);	// [交代]
	EnableWindow(hWndClear, FALSE);	// [クリア]
}
