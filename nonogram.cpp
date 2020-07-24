#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <tuple>
#include <sstream>
#include <conio.h>
#include <Windows.h>

inline void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

enum class alignment {
	vertical, horizental
};

const std::string tstlvl(
R"(000000000
001000100
011101110
111111111
111111111
011111110
001111100
000111000
000010000
000000000)"
);
int mapH, mapW;
int cursorX = 0, cursorY = 0;
std::vector<std::string> lvl;
std::vector<std::vector<uint16_t>> downNums;
std::vector<std::vector<uint16_t>> rightNums;
std::vector<std::vector<bool>> checked;
std::vector<std::vector<bool>> falseChecked;

std::vector<uint16_t> calcrows(std::vector<std::string>& vc, alignment al, int i);
auto render()->void;
auto init()->void;
auto keycheck(bool& gameplay)->void;
auto gameLogic()->bool;
auto showCleared()->void;

auto main(int argc, char** argv)->int {
	bool gameplay = 1;
	if (argc == 1) {
		init();
		while (gameplay) {
			render();
			keycheck(gameplay);
		}
		showCleared();
	}
	return 0;
}

auto init()->void {
	std::istringstream istm(tstlvl);
	std::string buff;
	while (istm >> buff) {
		mapW = buff.length();
		lvl.push_back(buff);
		checked.push_back(std::vector<bool>());
		falseChecked.push_back(std::vector<bool>());
		for (auto& bl : buff) {
			checked.back().push_back(0);
			falseChecked.back().push_back(0);
		}
	}
	mapH = buff.size();

	for (int i = 0; i < mapH; i++) {
		rightNums.push_back(calcrows(lvl, alignment::horizental, i));
	}
	for (int i = 0; i < mapW; i++) {
		downNums.push_back(calcrows(lvl, alignment::vertical, i));
	}
}

auto render()->void {
	system("cls");
	std::cout << "\n";
	for (int i = 0; i < mapH; i++) {
		for (int j = 0; j < mapW; j++) {
			if (checked[i][j]) {
				printf("■ ");
			}
			else if (falseChecked[i][j]) {
				printf("※ ");
			}
			else {
				printf("□ ");
			}
		}
		for (auto& lst : rightNums[i]) {
			printf("%3d", lst);
		}
		puts("\n");
	}
	int mxsz = 0;
	for (auto& lst : downNums) {
		if (lst.size() > mxsz) {
			mxsz = lst.size();
		}
	}
	for (int i = 0; i < mxsz; i++) {
		for (int j = 0; j < downNums.size(); j++) {
			if (downNums.at(j).size() <= i) {
				printf("   ");
			}
			else {
				printf("%-3d", (int)downNums.at(j).at(i));
			}
		}
		puts("\n");
	}
	gotoxy(3 * cursorX, 2 * cursorY);
	std::cout << "▼";
	gotoxy(0, 0);
}

std::vector<uint16_t> calcrows(std::vector<std::string>& vc, alignment al, int i) {
	std::vector<uint16_t> ret;
	std::string valid;

	if (al == alignment::vertical) {
		for (size_t idx = 0; idx < mapH; idx++) {
			valid.push_back(vc[idx][i]);
		}
	}
	else if (al == alignment::horizental) {
		valid = vc[i];
	}

	int icnt = 0;
	for (auto c : valid){
		if (c == '0') {
			if (icnt > 0) {
				ret.push_back(icnt);
				icnt = 0;
			}
		}
		else if (c == '1') {
			++icnt;
		}
	}
	if (icnt) {
		ret.push_back(icnt);
	}

	return ret;
}

auto keycheck(bool& gameplay)->void {
	switch (getch()) {
	case 'w':
		--cursorY;
		break;
	case 'a':
		--cursorX;
		break;
	case 's':
		++cursorY;
		break;
	case 'd':
		++cursorX;
		break;
	case 'j':
		falseChecked[cursorY][cursorX] = !falseChecked[cursorY][cursorX];
		break;
	case 'k':
		checked.at(cursorY).at(cursorX) = !checked.at(cursorY).at(cursorX);
		gameplay = gameLogic();
		break;
	default:
		break;
	}
	
	if (cursorX < 0) {
		cursorX = 0;
		return;
	}
	else if (cursorX == mapW) {
		cursorX = mapW - 1;
	}

	if (cursorY < 0) {
		cursorY = 0;
		return;
	}
	else if (cursorY == mapH) {
		cursorY = mapH - 1;
	}

	return;
}

auto gameLogic()->bool {

	bool ret = true;

	for (int i = 0; i < checked.size(); i++) {
		for (int j = 0; j < checked[i].size(); j++) {
			ret &= (checked[i][j] == (lvl[i][j] - '0'));
		}
	}

	return !ret;
}

auto showCleared()->void {
	system("cls");
	std::cout << "You Beat It!" << std::endl;
	std::cout << "Congratulation!" << std::endl << std::endl;

	for (size_t i = 0; i < mapH; i++) {
		for (size_t j = 0; j < mapW; j++)
			(checked[i][j] ? (std::cout << "■") : (std::cout << "□"));
		std::cout << std::endl;
	}
	return;
}