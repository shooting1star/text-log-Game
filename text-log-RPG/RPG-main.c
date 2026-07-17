#include "RPG.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define BAG_SIZE 25

void mainPtr(void);
void stream(void);
void restopt(void);
void meetmon(void);
player newPlayer;

int turnCount;
int bonusChance;
int finalChance;
int monsterChance;
int bag_pos=0;

char* items[25] = {
	"나뭇가지","나뭇잎","동전","사과","음식물","괴물의 살점","괴물의 피","벌레","개미",
	"짱돌","커터칼","식칼","오렌지","귤","삼각김밥","바나나우유","초코에몽","에너지드링크",
	"반쯤먹은 생수","술","반창고","자전거 헬멧","배달조끼","곰팡이 핀 치킨","상한 음식물"
};
char* effects[5]={"출혈","만취","배부름","허기","구토"};

void SaveGame(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("저장 파일 생성 실패!\n");
        return;
    }

    // 아이템을 쉼표(,)로 하나의 문자열로 묶기
    char inven[1024] = "";
    for (int i = 0; i < BAG_SIZE; i++) {
        if (newPlayer.bag[i] != NULL) {
            strcat(inven, newPlayer.bag[i]);
            strcat(inven, ",");
        }
    }
    
    // 가방이 비어있으면 EMPTY 기록, 아니면 마지막 쉼표 제거
    int len = strlen(inven);
    if (len > 0) inven[len - 1] = '\0';
    else strcpy(inven, "EMPTY");

    // 상태(status)에 띄어쓰기가 포함될 수 있으므로 구분자를 공백 대신 '|'로 사용
    fprintf(fp, "%.1f|%.1f|%d|%s|%d|%d|%s\n", 
        newPlayer.nowhp, newPlayer.maxhp, newPlayer.stress, 
        newPlayer.status, turnCount, bonusChance, inven);

    fclose(fp);
    printf("현재 상태가 안전하게 저장되었습니다. (%s)\n", filename);
}
int LoadGame(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) return 0; // 파일이 없으면 0 반환

    char buffer[2048];
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    // 줄바꿈 문자 제거
    buffer[strcspn(buffer, "\n")] = 0;

    // '|' 기준으로 문자열을 잘라서 구조체에 덮어씌우기
    char* token = strtok(buffer, "|");
    if (token == NULL) return 0;
    newPlayer.nowhp = atof(token);

    token = strtok(NULL, "|");
    if (token == NULL) return 0;
    newPlayer.maxhp = atof(token);

    token = strtok(NULL, "|");
    if (token == NULL) return 0;
    newPlayer.stress = atoi(token);

    token = strtok(NULL, "|");
    if (token == NULL) return 0;
    strcpy(newPlayer.status, token);

    token = strtok(NULL, "|");
    if (token == NULL) return 0;
    turnCount = atoi(token);

    token = strtok(NULL, "|");
    if (token == NULL) return 0;
    bonusChance = atoi(token);

    token = strtok(NULL, "|"); // 아이템 목록 텍스트
    
    // 가방 초기화
    for(int i = 0; i < BAG_SIZE; i++) newPlayer.bag[i] = NULL;
    bag_pos = 0;

    if (token != NULL && strcmp(token, "EMPTY") != 0) {
        char* item_token = strtok(token, ",");
        while (item_token != NULL && bag_pos < BAG_SIZE) {
            // items 배열에서 일치하는 원본 문자열 주소를 찾아 가방에 넣음
            for (int i = 0; i < 25; i++) {
                if (strcmp(item_token, items[i]) == 0) {
                    newPlayer.bag[bag_pos++] = items[i];
                    break;
                }
            }
            item_token = strtok(NULL, ",");
        }
    }
    return 1; // 로드 성공
}

void sort_bag() {
    for (int i = 0; i < BAG_SIZE - 1; i++) {
        if (newPlayer.bag[i] == NULL) {
            for (int j = i + 1; j < BAG_SIZE; j++) {
                if (newPlayer.bag[j] != NULL) {
                    newPlayer.bag[i] = newPlayer.bag[j];
                    newPlayer.bag[j] = NULL;
                    break;
                }
            }
        }
    }
}
int check_food(int pos){
	char* isfood[] = {
		"사과", "오렌지", "귤","삼각김밥","바나나우유","초코에몽","에너지드링크","반쯤먹은 생수","술"
	};
    int list_size = sizeof(isfood) / sizeof(isfood[0]);

    for (int i = 0; i < list_size; i++) {
        if (strcmp(newPlayer.bag[pos], isfood[i]) == 0) {
            return 1; // 일치하는 음식을 찾음 (참)
        }
    }
    return 0; // 음식을 찾지 못함 (거짓)
}
void useornot(int bag_pos){
if (newPlayer.bag[bag_pos] == NULL) {
        printf("가방의 해당 슬롯이 비어있습니다.\n");
        pause;
        return;
    }

    char* opt[3] = {"사용한다", "그만둔다", "버린다"};
    int choice = sellect(opt, get_size(opt)); 

    if (choice == 0) { // 1. 사용한다
        if (check_food(bag_pos) == 1) { 
            printf("%s을/를 먹으니까 기분이 좋아졌다! (체력 +25 , 스트레스 -10)\n", newPlayer.bag[bag_pos]); 
            newPlayer.nowhp += 25; newPlayer.stress -= 10;
            newPlayer.bag[bag_pos] = NULL; // 사용 완료 후 삭제
            sort_bag();
			pause;
        } 
        else {
            printf("사용해봤자 쓸데가 없어보인다.\n"); // 기획 의도에 맞는 자연스러운 거절 대사
            pause;
        }
    }
    else if (choice == 1) { 
        printf("쓸려다가 그만 뒀다.\n"); 
        pause;
    }
    else {
        printf("%s을/를 버렸다.\n", newPlayer.bag[bag_pos]); 
        newPlayer.bag[bag_pos] = NULL;
		sort_bag();
        pause; 
    }
    return;
}

void restopt(void){
	cls;
	char* saveitemheal[4] = {"저장하기", "인벤토리", "건강상태", "선택지로 돌아간다."};
	int  choice = sellect(saveitemheal,get_size(saveitemheal));
	if (choice == 0) {
		SetColor(11);
        printf("\n지금까지의 모험들을 저장합니다.\n");
        SaveGame("log.data"); // 추가된 저장 함수 호출!
		pause;
    } 
    else if (choice == 1) {
		SetColor(11);
        
        // 1. 실제 아이템들만 담을 임시 배열과 개수 카운터 생성
        char* active_bag[BAG_SIZE]; 
        int active_count = 0;
        
        // 2. 가방을 돌면서 실제 아이템(NULL이 아닌 것)만 임시 배열에 채워 넣기
        for (int i = 0; i < BAG_SIZE; i++) {
            if (newPlayer.bag[i] != NULL) {
                active_bag[active_count] = newPlayer.bag[i];
                active_count++;
            }
        }
        
        // 3. 예외 처리: 만약 가방이 텅 비어 있다면 메시지 출력 후 탈출
        if (active_count == 0) {
            printf("\n가방이 텅 비어 있습니다!\n\n");
            pause;
        } 
        else {
            printf("내가 가진 아이템들은 ..\n");
            
            // 4. 실제 존재하는 아이템 개수(active_count)만큼만 선택지 창에 넘겨줌!
            int selected_idx = sellect(active_bag, active_count);
            
            // 5. useornot() 함수에 '실제 가방 배열'의 인덱스를 전달해야 하므로
            // 우리가 이전에 sort_bag()을 해서 앞에서부터 채워놨기 때문에 
            // selected_idx가 곧 newPlayer.bag의 실제 인덱스(bag_pos)와 완벽히 일치합니다!
            useornot(selected_idx);
        }
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
}

int sellect(char* situ[],int size){
	int POS = 0;
    	CursorView(0);
    	system("COLOR 0F");

	    while (1) {
	        // 선택지 출력 (size만큼 루프)
	        for (int i = 0; i < size; i++) {
	            gotoxy(10, 5 + (i * 2)); // 2칸 간격으로 배치
	            if (i == POS) {
	                SetColor(11);
	                printf("> %s", situ[i]);
	            } else {
	                SetColor(15);
	                printf("  %s", situ[i]);
	            }
	        }

	        Sleep(100);

	        if (GetAsyncKeyState(VK_UP)) {
	            if (POS == 0) POS = size - 1;
    	        else POS -= 1;
    	    }
        	else if (GetAsyncKeyState(VK_DOWN)) {
        	    if (POS == size - 1) POS = 0;
            	else POS += 1;
        	}
        	else if (GetAsyncKeyState(VK_RETURN)) {
            	break;
        	}
    	}
    
    cls; // 선택 후 화면 초기화
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
void meetmon(void){
	SetColor(12); // 빨간색 위험 신호
    printf("[위험] 부스럭거리는 소리와 함께 무언가 나타났다!\n");
    printf("좆됐다! 모를 괴생명체가 앞을 가로막았다!\n");
    SetColor(15);
	Sleep(500);
    pause;
	cls;
	bonusChance = 0;
	int pick_item = rand()%25;
	SetColor(13);
    char* battleAction[4] = {"싸운다!", "방어 태세를 취한다.", "최대한 멀리 도망친다!", "심호흡하고 약점을 찾는다."};
    int battleChoice = sellect(battleAction,get_size(battleAction));
	int possible =  monsterChance + finalChance;
    cls;
    if (battleChoice == 0) {
        SetColor(13);
		if(possible > 75) {
			possible = 0;
			printf("씨발! 오히려 더맞았다. 아프다.(체력 -35.6, 스트레스 +15, 타박상/약한 출혈)");
			newPlayer.nowhp -= 35.6; newPlayer.stress += 15; strcpy(newPlayer.status, "타박상,약한 출혈");
			pause;
		}
		else {
			possible = 0;
			printf("\n격렬하게 싸워 승리했다! (체력 -10, 스트레스 +5)\n\n");
			newPlayer.nowhp -=10; newPlayer.stress +=5;
			if(bag_pos > 24 ) {printf("아이템이 다차서 못주워가겠다 ㄲㅂ");}
			else {
				int inserted = 0;
				for(int i = 0; i < BAG_SIZE; i++) {
    				if(newPlayer.bag[i] == NULL) {
        				newPlayer.bag[i] = items[pick_item];
        				printf("아이템 : %s을/를 주웠다!", items[pick_item]);
        				inserted = 1;
        				break;
    				}
				}
				if(!inserted) {printf("아이템이 다차서 못주워가겠다 ㄲㅂ\n");}
			}
			pause;
		}
	}

    else if (battleChoice == 1) {
		SetColor(13);
		if(possible > 80) {
			possible = 0;
			printf("못막았다. 존나아프다 ...(체력 -52.3,스트레스 +20, 출혈)");
			newPlayer.nowhp -= 52.3; newPlayer.stress += 20; strcpy(newPlayer.status, "출혈");
			pause;
		}
        else {
			possible =0;
			printf("\n공격을 잘 막아내어 위기를 넘겼다. 몬스터가 숲으로 사라진다.(체력 -30, 스트레스 +15)\n\n");
			newPlayer.nowhp -=30; newPlayer.stress +=15;
			pause;
		}	
	}

    else if (battleChoice == 2) {
        SetColor(13);
        printf("\n뒤도 안 돌아보고 째낀다! 다행히 따돌린 것 같다. (스트레스 +15)\n\n");
		newPlayer.stress +=15;
		pause;
    }

	else if(battleChoice == 3){
		SetColor(13);
		printf("\n천천히 약점을 찾았다. 때리고 튀자. (체력 + 5)\n\n");
		newPlayer.nowhp += 5;
		pause;
	}

    SetColor(15);
	printf("\n전투종료. 몬스터가 다 도망간거같다 다시 움직이자\n\n");
	pause;
}
void stream(void){
	if(newPlayer.stress >= 89) {printf("스트레스 존나받는다....... 힘이빠진다 ( 체력 -2.5)"); newPlayer.nowhp -=2.5;  newPlayer.stress -= 1.9; pause;}
	if(newPlayer.nowhp <= 0) {printf("몸이 너무 힘들다.. 눈이 감긴다. "); SetColor(13); gotoxy(20 ,6); puts("사망하였습니다, 게임을 종료합니다.(저장 X)"); pause; exit(0);}
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
    int choice = sellect(nowAction,get_size(nowAction));
    int mypick = pickedIndexes[choice];

	if(choice == 3) restopt();

    cls;
    printf("\n나는  %s \n", totalAction[mypick]);
    printf("결국 %s \n\n", totalRes[mypick]);
    
	finalChance =0 ;

    turnCount++;
    // 기본 확률은 20%로 시작하지만, 흐른 시간(turnCount)과 수색 실패 보너스가 계속 더해짐
    finalChance = 20+ bonusChance * ((rand()%5)+1); 
    
    // 확률이 100%를 넘지 않도록 안전장치 설정
    if (finalChance > 90) finalChance = 60; 
	monsterChance = rand() % 100; // 0 ~ 99 무작위 수 생성
    
    // 디버그용으로 현재 인게임 위험도를 살짝 표시해주면 플레이어가 더 스릴을 느낍니다.
    printf("⏳ 생존 시간: %d턴 | ⚠️ 주변 위험도: %d%%\n\n", turnCount, finalChance);
    Sleep(500);

    if (monsterChance < finalChance) 
		meetmon(); // 가변 확률 적용!
	else {
        // 몬스터를 안 만났다면 다음 번엔 더 잘 만나도록 보너스 확률을 8%씩 누적 적립!
    bonusChance += 5;
    printf("아무것도 없지만 불안한기분이든다.\n\n");
    }

    pause;
}

void newGame(){
    cls;
    system("mode con: cols=106 lines=15");
    printf("========================================================================================================\n\n\n");
    gotoxy(39,3); printf("눈을 떴다! 나는........");
    Sleep(1000);
 
	char* stsitu[4] = {"도로한가운데 있다.", "숲속에서 눈을떴다.","어디인지 모르는 건물 안에있다.", "대피소에 있다."};
    int choice = sellect(stsitu,get_size(stsitu));
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
	turnCount = 0;
    bonusChance = 0;
	bag_pos = 0;
	gotoxy(35,10); printf("아무키나 누르고 모험시작!");
    pause;

	while(1) { stream();}
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
	else if (POS == 1) {
		if (LoadGame("log.data")) { // 게임 내부에서 이어하기를 눌렀을 때
            system("mode con: cols=106 lines=15");
			SetColor(11);
            printf("\n[log.data] 성공적으로 불러왔습니다!\n");
            pause;
            while(1) { stream(); }
        } else {
            printf("\n저장된 파일(log.data)이 없습니다. 새로 시작합니다.\n");
            pause;
            newGame();
        }
	}
	else if (POS == 2) puts("게임을 종료합니다 .");

	return 0;
}

