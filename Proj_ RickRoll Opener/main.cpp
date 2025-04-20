
#include "WindowCreationDEFINES.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	WNDCLASS MainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(NULL, IDI_QUESTION),
		processName, Main);

	if (!RegisterClass(&MainClass)) { return 1; }

	CreateWindow(processName, L"Открыватор рикроллов by TimChi", windowStyles, 200, 200, 640, 360, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


LRESULT CALLBACK Main(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			AddMenus(hWnd);
			AddWidgets(hWnd);
			break;
		}

		case WM_COMMAND:
		{
			switch (wp)
			{ // Нажатия на меню
				case submenu_PeterAlert:
				{
					MessageBox(hWnd, L"Peter Alert", L"Peter Alert", (MB_APPLMODAL | MB_ICONMASK));
					break;
				}
				case submenu_Exit:
				{
					weDontWantToStop = false; // Atomic, не нужен мьютекс
					if (rickRockOpener && rickRockOpener->joinable())
					{
						rickRockOpener->join();
						delete rickRockOpener;
						rickRockOpener = nullptr;
					}
					PostQuitMessage(0);
					break;
				}
				// Нажатия на кнопки
				case btn_Easy:
				{
					ToggleRickRoll(EasyMode);
					break;
				}
				default:
					break;
			}
			break;
		}

		case WM_DESTROY:
		{
			weDontWantToStop = false;
			if (rickRockOpener && rickRockOpener->joinable())
			{
				rickRockOpener->join();
				delete rickRockOpener;
				rickRockOpener = nullptr;
			}
			PostQuitMessage(0);
			break;
		}

		default:
			return DefWindowProc(hWnd, msg, wp, lp);
	}
}

WNDCLASS NewWindowClass(HBRUSH BGcolor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure)
{
	WNDCLASS NWC = { NULL };
	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGcolor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}

// Функции добавления в окно Меню, Виджетов, и другой шняги

static void AddMenus(HWND hWnd)
{
	HMENU RootMenu_File = CreateMenu();
	HMENU SubMenu = CreateMenu();

	AppendMenu(RootMenu_File, MF_POPUP, (UINT_PTR)SubMenu, L"Приложение");
		AppendMenu(SubMenu, MF_STRING, submenu_PeterAlert, L"Peter Alert!");
		AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(SubMenu, MF_STRING, submenu_Exit, L"Закрыть");

	SetMenu(hWnd, RootMenu_File);
}

static void AddWidgets(HWND hWnd)
{
	CreateWindowA("static", "Открыватор РикРоллов v2 by TimChi", WS_VISIBLE | WS_CHILD | ES_CENTER, 170, 5, 300, 20, hWnd, NULL, NULL, NULL);
	CreateWindowA("button", "Легко", WS_VISIBLE | WS_CHILD, 270, 180, 100, 50, hWnd, (HMENU)btn_Easy, NULL, NULL);
}

// Функция для открытия RickRoll
static void OpenRickRoll(DWORD mode)
{
	while (weDontWantToStop.load())
	{ // Используем atomic вместо мьютекса для проверки
		ShellExecute(NULL, NULL, L"https://youtu.be/sEwMKGMvzf4", NULL, NULL, SW_SHOWNORMAL);
		std::this_thread::sleep_for(std::chrono::milliseconds(mode));
	}
}
// Функция для остановки RickRoll
static void StopRickRoll()
{
	std::lock_guard<std::mutex> lock(weDontWantToStopMutex);
	if (isRickRolling)
	{
		weDontWantToStop = false;
		if (rickRockOpener->joinable())
		{
			rickRockOpener->join(); // Ждем завершения потока
		}
		delete rickRockOpener;
		isRickRolling = false;
	}
}

// Обновленная функция ToggleRickRoll
static void ToggleRickRoll(DWORD mode)
{
	std::lock_guard<std::mutex> lock(weDontWantToStopMutex);
	if (!isRickRolling)
	{
		weDontWantToStop = true;
		if (rickRockOpener)
		{
			rickRockOpener->join(); // Если старый поток есть — завершаем его
			delete rickRockOpener;
		}
		rickRockOpener = new std::thread(OpenRickRoll, mode);
		isRickRolling = true;
	}
	else {
		weDontWantToStop = false;
		if (rickRockOpener && rickRockOpener->joinable())
		{
			rickRockOpener->join(); // Ждём завершения
			delete rickRockOpener;
			rickRockOpener = nullptr;
		}
		isRickRolling = false;
	}
}