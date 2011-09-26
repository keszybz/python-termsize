#include <windows.h>
#include <conio.h>
#include <stdio.h>

/*
BOOL WINAPI GetConsoleScreenBufferInfo(
  __in   HANDLE hConsoleOutput,
  __out  PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo
);


typedef struct _CONSOLE_SCREEN_BUFFER_INFO {
  COORD dwSize;
  COORD dwCursorPosition;
  WORD wAttributes;
  SMALL_RECT srWindow;
  COORD dwMaximumWindowSize;
} CONSOLE_SCREEN_BUFFER_INFO;

*/

int main(){

	CONSOLE_SCREEN_BUFFER_INFO csbi; 
	SMALL_RECT srctWindow;
	HANDLE hstdout;
	hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!GetConsoleScreenBufferInfo(hstdout,&csbi))
		return 1;

	int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	printf("columns=%d rows=%d\n", columns, rows);

	return 0;
}
