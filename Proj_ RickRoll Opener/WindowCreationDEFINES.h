#include <windows.h>
#include <shellapi.h>
#include <thread>
#include <mutex> // Добавляем заголовочный файл для мьютекса

// Прототипы функций
static LRESULT CALLBACK Main(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
static WNDCLASS NewWindowClass(HBRUSH BGcolor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
static void AddMenus(HWND hWnd);
static void AddWidgets(HWND hWnd);
static void OpenRickRoll(DWORD mode);
static void StopRickRoll();
static void ToggleRickRoll(DWORD mode);

// Дефайны для удобной настройки создания окна
#define processName     (L"rickRollOpener")
#define windowStyles    (WS_OVERLAPPEDWINDOW | WS_VISIBLE)

// Дефайны для нажатий на меню
#define menu_File           101
#define submenu_PeterAlert  102
#define submenu_EditURL        103
#define submenu_Exit        104

// Дефайны для нажатий на КНОПКИ
#define btn_Easy            1
// Зарезервировано
#define btn_EditUrl			4

// Режимы сложности
#define EasyMode            2000
#define MediumMode          1000
#define HardMode            500
#define PIZDECMode          250

// Глоб переменная для остановки цикла
// Добавляем флаг для принудительного завершения потока
static std::atomic<bool> weDontWantToStop(true); // Заменяем bool на atomic
static std::thread* rickRockOpener = nullptr; // Инициализируем nullptr
static std::mutex weDontWantToStopMutex;
static bool isRickRolling = false;
// Эдит бокс, который мы будем добавлять когда прожали на нужное для этого подменя.
HWND editBox = NULL;
// Кнопка под эдитбоксом. Заносим их в переменные чтоб потом удалить.
HWND editBox_ConfirmBtn = NULL;
WCHAR editBox_Text[256] = L"https://youtu.be/sEwMKGMvzf4";