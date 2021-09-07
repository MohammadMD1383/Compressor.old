#pragma once

#include <Windows.h>

#define BLACK          0x00

#define F_BLUE         0x01
#define F_GREEN        0x02
#define F_AQUA         0x03
#define F_RED          0x04
#define F_PURPLE       0x05
#define F_YELLOW       0x06
#define F_WHITE        0x07
#define F_GRAY         0x08
#define F_LIGHT_BLUE   0x09
#define F_LIGHT_GREEN  0x0A
#define F_LIGHT_AQUA   0x0B
#define F_LIGHT_RED    0x0C
#define F_LIGHT_PURPLE 0x0D
#define F_LIGHT_YELLOW 0x0E
#define F_BRIGHT_WHITE 0x0F

#define B_BLUE         0x10
#define B_GREEN        0x20
#define B_AQUA         0x30
#define B_RED          0x40
#define B_PURPLE       0x50
#define B_YELLOW       0x60
#define B_WHITE        0x70
#define B_GRAY         0x80
#define B_LIGHT_BLUE   0x90
#define B_LIGHT_GREEN  0xA0
#define B_LIGHT_AQUA   0xB0
#define B_LIGHT_RED    0xC0
#define B_LIGHT_PURPLE 0xD0
#define B_LIGHT_YELLOW 0xE0
#define B_BRIGHT_WHITE 0xF0

#define CONSOLE_HANDLE GetStdHandle(STD_OUTPUT_HANDLE)

namespace utility::console {
	void setColor(const unsigned __int16 color) { SetConsoleTextAttribute(CONSOLE_HANDLE, color); }
	
	COORD getCursorPos() {
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(CONSOLE_HANDLE, &info);
		return info.dwCursorPosition;
	}
	
	void setCursorPos(__int16 col = -1, __int16 row = -1) {
		COORD coord = getCursorPos();
		if (col >= 0) coord.X = col;
		if (row >= 0) coord.Y = row;
		SetConsoleCursorPosition(CONSOLE_HANDLE, coord);
	}
	
	void setCursorVisibility(const bool visible) {
		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(CONSOLE_HANDLE, &info);
		info.bVisible = visible;
		SetConsoleCursorInfo(CONSOLE_HANDLE, &info);
	}
	
	int getRowCount() {
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(CONSOLE_HANDLE, &info);
		return info.srWindow.Bottom - info.srWindow.Top;
	}
	
	int getColCount() {
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(CONSOLE_HANDLE, &info);
		return info.srWindow.Right - info.srWindow.Left;
	}
	
	class colorize {
	private:
		const __int8 color;
		const char   *text;
	
	public:
		colorize(const __int8 color, const char *text) : color(color), text(text) {}
		
		friend std::ostream &operator<<(std::ostream &os, const colorize &c) {
			CONSOLE_SCREEN_BUFFER_INFO info;
			GetConsoleScreenBufferInfo(CONSOLE_HANDLE, &info);
			
			setColor(c.color);
			std::cout << c.text;
			
			setColor(info.wAttributes);
			
			return os;
		}
	};
}