#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

typedef struct Desktop{
	UINT16 resolutionX, resolutionY;
	HWND hwnd;
	HDC hdc;
	LPRECT rect;
} Desktop;

void MakePIZDEC();
void HideAll();
Desktop* InitDesktop();

int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	if(MessageBox(NULL, "Сосал??", "Колись!", MB_YESNO | MB_ICONQUESTION) == IDNO) {
		if(MessageBox(NULL, "Точно?", NULL, MB_YESNO) == IDYES) {
			MakePIZDEC();
		}
		MessageBox(NULL, "Вот так мы постепенно и учимся всякой хуйне.", "блеблебле", MB_OK);

		char dstPath[MAX_PATH] = {};
		GetCurrentDirectory(MAX_PATH, dstPath);
	
		strcat(dstPath, "\\");
		strcat(dstPath, "CachedImage_1920_1080_POS4.jpg");

		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)dstPath, SPIF_UPDATEINIFILE);
		return 1;
	}

	return 0;
}

void MakePIZDEC() {
	Desktop* desktop = InitDesktop();

	// Меняем обои дабы было смешно.
	char srcPath[MAX_PATH];
	DWORD len = ExpandEnvironmentStrings(
		TEXT("%AppData%\\Microsoft\\Windows\\Themes\\CachedFiles\\CachedImage_1920_1080_POS4.jpg"),
		srcPath, MAX_PATH
	);
	CopyFile(srcPath, "CachedImage_1920_1080_POS4.jpg", FALSE);

	char dstPath[MAX_PATH] = {};
    GetCurrentDirectory(MAX_PATH, dstPath);

    strcat(dstPath, "\\");
    strcat(dstPath, "ZRYA.png");

	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)dstPath, SPIF_UPDATEINIFILE);
	HideAll();

	// Сделаем красивый шрифт.
	#define fontH 80
	#define fontW 30
	HFONT fancyText = CreateFontW(
		fontH, fontW, 0, 0, FW_MEDIUM,
		FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		FF_DECORATIVE, (LPCWSTR)"Comic Sans"
	);
	
	SetTextColor(desktop->hdc, RGB(255, 255, 255));
	SetBkColor(desktop->hdc, RGB(0, 0, 0));
	SelectObject(desktop->hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(desktop->hdc, RGB(0,0,0));
	
	clock_t now = clock();
	char elapsed = 0;
	char cooldown = 10;

	while(elapsed < cooldown) {
		elapsed = (clock() - now)/CLOCKS_PER_SEC;

		SetCursorPos(0, desktop->resolutionY);

		SelectObject(desktop->hdc, GetStockObject(DC_BRUSH));
		Rectangle(desktop->hdc, 0, 0, desktop->resolutionX, desktop->resolutionY);

		char countdown[2] = {};
		sprintf(countdown, "%d", cooldown - elapsed);
		SelectObject(desktop->hdc, fancyText);
		TextOut(desktop->hdc,
			desktop->resolutionX / 2 - fontW/2,
			desktop->resolutionY / 2 - fontH/2,
			countdown,
			2
		);
	}

	ReleaseDC(desktop->hwnd, desktop->hdc);
	SetCursorPos(desktop->resolutionX / 2 + 10, desktop->resolutionY / 2 + 46); // Курсор будет четко на кнопке OK
	free(desktop->rect); desktop->rect = 0;
	free(desktop); desktop = 0;
	return;
}

Desktop* InitDesktop() {
	Desktop* desktop = malloc(sizeof(Desktop));
	
	desktop->hwnd = GetDesktopWindow();

	desktop->rect = malloc(sizeof(*desktop->rect));
	GetWindowRect(desktop->hwnd, desktop->rect);
	GetClientRect(desktop->hwnd, desktop->rect);
	
	desktop->hdc = GetWindowDC(desktop->hwnd);
	desktop->resolutionX = desktop->rect->right;
	desktop->resolutionY = desktop->rect->bottom;
}

void HideAll()
{
    keybd_event(VK_LWIN, 0, 0, 0);
    keybd_event('M', 0, 0, 0);

    keybd_event('M', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
}