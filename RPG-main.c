#include "RPG.h"
#include <time.h>
#include <string.h>
void mainPtr(void);
void stream(void);
void restopt(void);
player newPlayer;

void restopt(void){
	cls;
	char* saveitemheal[4] = {"저장하기", "인벤토리", "건강상태", "선택지로 돌아간다."};
	int  choice = sellect(saveitemheal);
	if (choice == 0) {
        SetColor(11);
        printf("\n지금까지의 모험들을 저장합니다.(아직)\n\n");
		pause;
    } 
    else if (choice == 1) {
		SetColor(11);
        printf("\n아직\n\n");
		pause;
    } 
    else if (choice == 2) {
        SetColor(11);
        printf("\n체력 : 현재[%.1f] / 최대[%.1f] || 스트레스 : [%d] || 상태 : [%s]\n",newPlayer.nowhp,newPlayer.maxhp,newPlayer.stress,newPlayer.status);
		pause;
    }
	else if(choice == 3){
		SetColor(11);
		printf("\n선택지로 돌아갑니다.\n\n");
		pause;
	}
    SetColor(15);
	stream();
}

int sellect(char* situ[]){
	int POS = 0;
    CursorView(0);
    system("COLOR 0F");

	while (1) {

        	switch (POS) {
            	case 0: 
                	SetColor(11); gotoxy(10, 5); printf("> %s", situ[0]); // 선택된 항목은 앞에 '>' 기호를 붙여서 가시성 업!
                	SetColor(15); gotoxy(10, 7); printf("  %s", situ[1]);
                	gotoxy(10, 9); printf("  %s", situ[2]);
					SetColor(15); gotoxy(10, 11); printf("  %s", situ[3]);
                	break;
            	case 1:
                	SetColor(15); gotoxy(10, 5); printf("  %s", situ[0]);
                	SetColor(11); gotoxy(10, 7); printf("> %s", situ[1]);
                	SetColor(15); gotoxy(10, 9); printf("  %s", situ[2]);
					SetColor(15); gotoxy(10, 11); printf("  %s", situ[3]);
                	break;
            	case 2:
                	SetColor(15); gotoxy(10, 5); printf("  %s", situ[0]);
                    gotoxy(10, 7); printf("  %s", situ[1]);
                	SetColor(11); gotoxy(10, 9); printf("> %s", situ[2]);
					SetColor(15); gotoxy(10, 11); printf("  %s", situ[3]);
                	break;
				case 3:
					SetColor(15); gotoxy(10, 5); printf("  %s", situ[0]);
                    gotoxy(10, 7); printf("  %s", situ[1]);
                	SetColor(15); gotoxy(10, 9); printf("  %s", situ[2]);
					SetColor(11); gotoxy(10, 11); printf("> %s", situ[3]);
                	SetColor(15);
                	break;
            	default: break; 
        	}
        	Sleep(100);
        
        	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_UP)) {
        	    if (POS == 0) POS = 3;
        	    else POS -= 1;
        	}
        	else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_DOWN)) {
        	    if (POS == 3) POS = 0;
        	    else POS += 1;
        	}
        	else if (GetAsyncKeyState(VK_RETURN)) { // 엔터를 눌렀을 때
        	    break;
        	}
    }
    
    cls; 
    return POS;

}

char* totalAction[15] = {
    "주변을 살펴봐야겠다.",
    "어떻게 할지 모르겠다, 무섭다.",
    "우선 물건들을 모아야겠다.",
    "사람이 있는지 큰소리로 찾아본다.",
    "마실 수 있는 물이 있는지 찾는다.",
    "소리를 죽이고 기어서 이동한다.",
    "몸을 숨길 만한 은신처를 찾는다.",
    "심호흡을 하며 마음을 진정시킨다.",
    "더 높은 곳으로 올라가 지형을 본다.",
    "쓸만한 임시 무기를 제작한다.",
    "불을 피울 만한 재료를 구한다.",
    "경계를 서며 주변 소리에 집중한다.",
    "잠시 눈을 붙이고 체력을 회복한다.",
    "무작정 뛰기 시작한다.",
    "바닥에 떨어진 무언가를 먹는다."
}; int totalCount = sizeof(totalAction) / sizeof(totalAction[0]);
char* totalRes[15] = {
	"주변에 아무도 없는거같다.",
    "내가 할수있는게 없다",
    "을 주웠다.",
    "사람소리가 아닌게 들린다.",
    "근처건물까지는 물을 못마실거같다",
    "인기척이 느껴지지않아서 그냥 소리내며 움직인다.",
    "근처건물이나 열린공간으로 가야할거같다.",
    "아직상황이 이해되지않지만 마음을 가다듬는다.",
    "높은건물이 보이니 그곳으로 가야할거같다",
    "만들었지만 공격이 들어갈거같지않아 버렸다.",
    "비가많이와서인지 재료가 다 눅눅하다, 버려야지.",
    "동물소리도 안들리고 흔한 자동차소리도 안들린다.",
    "무슨일이 생길지모르니 쉬는건 나중에하자.",
    "근처에 인기척도 없고 건물만 있다.",
    "쓰고 이상한맛이나서 뱉었다, 조심하자."
};

void stream(void){
	cls;
		printf("나는 ...");
		int pickedIndexes[4]; // 뽑힌 인덱스 저장할 배열

    	for (int i = 0; i < 4; i++) {
        	while (1) {
            	int r = rand() % totalCount; // 0 ~ (전체개수-1) 사이에서 하나 뽑기
            
            	// 중복 검사
            	int duplicate = 0;
            	for (int j = 0; j < i; j++) {
                	if (pickedIndexes[j] == r) {
                    	duplicate = 1; // 이미 뽑은 거면 다시!
                    	break;
                	}
            	}
            	if (!duplicate) {
                	pickedIndexes[i] = r; // 중복 아니면 확정
                	break;
            	}
        	}
    	}

	char* nowAction[4];
    nowAction[0] = totalAction[pickedIndexes[0]];
    nowAction[1] = totalAction[pickedIndexes[1]];
    nowAction[2] = totalAction[pickedIndexes[2]];
	nowAction[3] = "기타(저장, 인벤토리, 건강)";
    int choice = sellect(nowAction);
    int mypick = pickedIndexes[choice];

	if(choice == 3) restopt();

    cls;
    printf("\n나는  %s \n", totalAction[mypick]);
    printf("결국 %s \n\n", totalRes[mypick]);
    
	static int turnCount = 0;
    static int bonusChance = 0;

    turnCount++;
    // 기본 확률은 20%로 시작하지만, 흐른 시간(turnCount)과 수색 실패 보너스가 계속 더해짐
    int finalChance = 20 + (turnCount * 2) + bonusChance; 
    
    // 확률이 100%를 넘지 않도록 안전장치 설정
    if (finalChance > 90) finalChance = 60; 

    int monsterChance = rand() % 100; // 0 ~ 99 무작위 수 생성
	int possible;
    
    // 디버그용으로 현재 인게임 위험도를 살짝 표시해주면 플레이어가 더 스릴을 느낍니다.
    printf("⏳ 생존 시간: %d턴 | ⚠️주변 위험도: %d%%\n\n", turnCount, finalChance);
    Sleep(500);

    if (monsterChance < finalChance) { // 가변 확률 적용!
        SetColor(12); // 빨간색 위험 신호
        printf("[위험] 부스럭거리는 소리와 함께 무언가 나타났다!\n");
        printf("좆됐다! 모를 괴생명체가 앞을 가로막았다!\n");
        SetColor(15);
		Sleep(500);
        pause;
		cls;
        // 몬스터를 마주쳤으므로 쌓여있던 추가 확률 보너스는 초기화
        bonusChance = 0;

        // 전투 선택지 호출 (만들어둔 sellect 활용)
        char* battleAction[4] = {"싸운다!", "방어 태세를 취한다.", "최대한 멀리 도망친다!", "심호흡하고 약점을 찾는다."};
        int battleChoice = sellect(battleAction);

        cls;
        if (battleChoice == 0) {
            SetColor(13);
			possible =  monsterChance + finalChance;
			if(possible > 75) {
				possible = 0;
				printf("씨발! 오히려 더맞았다. 아프다.(체력 -35.6, 스트레스 +15, 타박상/약한 출혈)");
				newPlayer.nowhp -= 35.6; newPlayer.stress += 15; strcpy(newPlayer.status, "타박상,약한 출혈");
			}
			else {
				printf("\n격렬하게 싸워 승리했다! (체력 -10, 스트레스 +5)\n\n");
				newPlayer.nowhp -=10; newPlayer.stress +=5;
			}
        } 
        else if (battleChoice == 1) {
			SetColor(13);
			possible =  monsterChance + finalChance;
			if(possible > 80) {
				possible = 0;
				printf("못막았다. 존나아프다 ...(체력 -52.3,스트레스 +20, 출혈)");
				newPlayer.nowhp -= 52.3; newPlayer.stress += 20; strcpy(newPlayer.status, "출혈");
			}
            else {
				printf("\n공격을 잘 막아내어 위기를 넘겼다. 몬스터가 숲으로 사라진다.(체력 -30, 스트레스 +15)\n\n");
				newPlayer.nowhp -=30; newPlayer.stress +=15;
			}	
		} 
        else if (battleChoice == 2) {
            SetColor(13);
            printf("\n뒤도 안 돌아보고 째낀다! 다행히 따돌린 것 같다. (스트레스 +15)\n\n");
			newPlayer.stress +=15;
        }
		else if(battleChoice == 3){
			SetColor(13);
			printf("\n천천히 약점을 찾았다. 때리고 튀자. (체력 + 5)\n\n");
			newPlayer.nowhp += 5;
		}
        SetColor(15);
    	} else {
        // 몬스터를 안 만났다면 다음 번엔 더 잘 만나도록 보너스 확률을 8%씩 누적 적립!
        bonusChance += 5;
        printf("아무것도 없지만 불안한기분이든다.\n\n");
    	}

    pause;

	stream();
}



void newGame(){
    cls;
    system("mode con: cols=106 lines=15");
    printf("========================================================================================================\n\n\n");
    gotoxy(39,3); printf("눈을 떴다! 나는........");
    Sleep(1000);
 
	char* stsitu[4] = {"도로한가운데 있다.", "숲속에서 눈을떴다.","어디인지 모르는 건물 안에있다.", "대피소에 있다."};
    int choice = sellect(stsitu);
    if(choice == 0) { newPlayer.maxhp = 180.0; newPlayer.nowhp = 180.0; newPlayer.stress = 15; strcpy(newPlayer.status, "도시사람");}
    if(choice == 1) {newPlayer.maxhp = 240.0; newPlayer.nowhp = 240.0; newPlayer.stress = 25; strcpy(newPlayer.status, "자연인");}
    if(choice == 2) {newPlayer.maxhp = 200.0; newPlayer.nowhp = 200.0; newPlayer.stress = 30; strcpy(newPlayer.status, "실내?");}
	if(choice == 3) {newPlayer.maxhp = 190.0; newPlayer.nowhp = 190.0; newPlayer.stress = 50; strcpy(newPlayer.status, "실내(대피)");}
    cls;

	puts("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	puts("■                                                                                                      ■");
	puts("■                                                                                                      ■");
	puts("■                                                                                                      ■");
	puts("■                                                                                                      ■");
	puts("■                                                                                                      ■");
	puts("■                                                                                                      ■");
	puts("■                                                                                                      ■");
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

	gotoxy(40,2); printf("당신의 선택 결과:\n");
	gotoxy(20, 5); printf("  [초기 상태] : %s\n", newPlayer.status);
	gotoxy(40, 5); printf("  [최대 체력] : %.1f\n", newPlayer.maxhp);
    gotoxy(60,5);  printf("  [스 트 레 스] : %d\n", newPlayer.stress);

	gotoxy(35,10); printf("아무키나 누르고 모험시작!");
    pause;

	stream();
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

