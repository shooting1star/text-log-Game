#include "RPG.h"

void mainPtr(void);

int sellect(){
    char* situ[3] = {"도로한가운데 있다.", "숲속에서 눈을떴다.","어디인지 모르는 건물 안에있다."};
    
    int POS = 0;
	CursorView(0);
	system("COLOR 0F");
	while (1) {
		
		switch (POS) {
			case 0: 
				SetColor(11);
				gotoxy(20, 4); printf("%s\n",situ[0]);
				SetColor(15); 
				gotoxy(42, 4); printf("%s\n",situ[1]);
				gotoxy(64, 4); printf("%s\n",situ[2]);
                break;
			case 1:
				gotoxy(20, 4); printf("%s\n",situ[0]);
				SetColor(11);
				gotoxy(42, 4); printf("%s\n",situ[1]);
				SetColor(15);
				gotoxy(64, 4); printf("%s\n",situ[2]);
				break;
			case 2:
				gotoxy(20, 4); printf("%s\n",situ[0]);
				gotoxy(42, 4); printf("%s\n",situ[1]);
				SetColor(11);
				gotoxy(64, 4); printf("%s\n",situ[2]);
				SetColor(15);
                break;
			default: break; 
		}
		Sleep(100);
        
        if (GetAsyncKeyState(VK_LEFT))
			if (POS == 0) POS = 2;
			else POS -= 1;
		else if (GetAsyncKeyState(VK_RIGHT))
			if (POS == 2) POS = 0;
			else POS += 1;
		else if (GetAsyncKeyState(VK_RETURN))//엔터를 눌렀을 때
			break;
	}
    return POS+1;
}

void newGame(){
    cls;
    system("mode con: cols=106 lines=15");
    printf("=============================================\n");
    printf("눈을 떴다! 나는........\n");
    Sleep(1000);
    player newPlayer;
    int choice = sellect();
    if(choice == 1) { newPlayer.maxhp = 180.0; newPlayer.stress = 15; strcpy(newPlayer.status, "도시사람");}
    if(choice == 2) {newPlayer.maxhp = 240.0; newPlayer.stress = 25; strcpy(newPlayer.status, "자연인");}
    if(choice == 3) {newPlayer.maxhp = 200.0; newPlayer.stress = 30; strcpy(newPlayer.status, "실내?");}

    cls;
    printf("\n  모험이 시작되었습니다! 당신의 선택 결과:\n");
    printf("  -----------------------------------------------\n");
    printf("  [초기 상태] : %s\n", newPlayer.status);
    printf("  [최대 체력] : %.1f\n", newPlayer.maxhp);
    printf("  [스 트 레 스] : %d\n", newPlayer.stress);
    printf("  -----------------------------------------------\n\n");
    pause;
}

void mainPtr(void) {
	system("mode con: cols=106 lines=9");   //참고로 cols 2단위가 특수문자 또는 한글 1글자다.
    SetConsoleTitleW(L"RPG");
	puts("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	puts("■                                                                                                      ■");
	puts("■                                                                                                      ■");
	puts("■                                                                                                      ■");
	puts("■                                                                                                      ■");
	puts("■                                                                                                      ■");
	puts("■                                                                                                      ■");
	puts("■                                                                                                      ■");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	gotoxy(20, 4); printf("<새로  하기>");
	gotoxy(42, 4); printf("<이어  하기>");
	gotoxy(64, 4); printf("<게임  종료>");
	return;
}

int main(void) {

    system("chcp 65001 > nul");

	int POS = 3;	//0 - 새로 하기, 1 - 이어 하기, 2 - 랭킹 보기, 3 - 게임 종료
	CursorView(0);
	system("COLOR 0F");
	mainPtr();
	while (1) {
		if (GetAsyncKeyState(VK_LEFT))
			if (POS == 0) POS = 2;
			else POS -= 1;
		else if (GetAsyncKeyState(VK_RIGHT))
			if (POS == 2) POS = 0;
			else POS += 1;
		else if (GetAsyncKeyState(VK_RETURN))//엔터를 눌렀을 때
			break;
		switch (POS) {
			case 0: 
				SetColor(11);
				gotoxy(20, 4); printf("<새로  하기>");
				SetColor(15); 
				gotoxy(42, 4); printf("<이어  하기>");
				gotoxy(64, 4); printf("<게임  종료>"); 
				break;
			case 1:
				gotoxy(20, 4); printf("<새로  하기>");
				SetColor(11);
				gotoxy(42, 4); printf("<이어  하기>");
				SetColor(15);
				gotoxy(64, 4); printf("<게임  종료>");
				break;
			case 2:
				gotoxy(20, 4); printf("<새로  하기>");
				gotoxy(42, 4); printf("<이어  하기>");
				SetColor(11);
				gotoxy(64, 4); printf("<게임  종료>");
				SetColor(15);
				break;
			default: break;
		}
		Sleep(100);
	}
	cls;
	if (POS == 0) newGame();
	else if (POS == 1) puts("이어 하기를 눌렀습니다.");
	else if (POS == 2) puts("게임을 종료합니다 .");

	return 0;
}

