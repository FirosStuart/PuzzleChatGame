//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by PuzzleChatGame.rc
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
#define IDB_START  114

#define IDB_BLACK 514
#define IDB_RED    1919
#define IDB_ORANGE 810
#define IDB_GREEN 411
#define IDB_YELLOW 415
#define IDB_BLUE 9191



#define IDF_HOSTNAME    2000            // ホスト名入力エディットボックス
#define IDF_QUESTION    2001            // お題表示用エディットボックス
#define IDF_RULE        2002            // 制約条件用エディットボックス
#define IDF_SENDMSG     2003            // 送信メッセージ入力用エディットボックス
#define IDF_RECVMSG     2004            // 受信メッセージ表示用エディットボックス
#define IDF_SCORE_PL1   2005            // 自分の得点用エディットボックス
#define IDF_SCORE_PL2   2006            // 相手の得点用エディットボックス
#define IDF_HELP        2007            // ヘルプメッセージ用エディットボックス

#define WINDOW_W		700				// ウィンドウの幅
#define WINDOW_H		600             // ウィンドウの高さ
#define MAX_ARRAY		10000			// 配列の最大要素数

#define MAX_MESSEGE		30				// メッセージの最大要素数




#define IDS_APP_TITLE			103

#define IDR_MAINFRAME			128
#define IDD_PUZZLECHATGAME_DIALOG	102
#define IDD_ABOUTBOX			103
#define IDM_ABOUT				104
#define IDM_EXIT				105
#define IDI_PUZZLECHATGAME			107
#define IDI_SMALL				108
#define IDC_PUZZLECHATGAME			109
#define IDC_MYICON				2
#ifndef IDC_STATIC
#define IDC_STATIC				-1
#endif
// Next default values for new objects
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NO_MFC					130
#define _APS_NEXT_RESOURCE_VALUE	129
#define _APS_NEXT_COMMAND_VALUE		32771
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		110
#endif
#endif
