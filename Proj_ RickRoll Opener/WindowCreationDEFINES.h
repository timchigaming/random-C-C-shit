#include <windows.h>
#include <shellapi.h>
#include <thread>
#include <mutex> // ��������� ������������ ���� ��� ��������

// ��������� �������
static LRESULT CALLBACK Main(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
static WNDCLASS NewWindowClass(HBRUSH BGcolor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
static void AddMenus(HWND hWnd);
static void AddWidgets(HWND hWnd);
static void OpenRickRoll(DWORD mode);
static void StopRickRoll();
static void ToggleRickRoll(DWORD mode);

// ������� ��� ������� ��������� �������� ����
#define processName     (L"rickRollOpener")
#define windowStyles    (WS_OVERLAPPEDWINDOW | WS_VISIBLE)

// ������� ��� ������� �� ����
#define menu_File           101
#define submenu_PeterAlert  102
#define submenu_Exit        103

// ������� ��� ������� �� ������
#define btn_Easy            1

// ������ ���������
#define EasyMode            2000
#define MediumMode          1000
#define HardMode            500
#define PIZDECMode          250

// ���� ���������� ��� ��������� �����
// ��������� ���� ��� ��������������� ���������� ������
static std::atomic<bool> weDontWantToStop(true); // �������� bool �� atomic
static std::thread* rickRockOpener = nullptr; // �������������� nullptr
static std::mutex weDontWantToStopMutex;
static bool isRickRolling = false;