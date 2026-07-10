#pragma once

/* ▼▼▼▼▼ 헤더 선언 ▼▼▼▼▼ */
#include <stdio.h>
#include <windows.h>
#include <stralign.h>
#include <stdlib.h>


/* ▼▼▼▼▼ define 선언 ▼▼▼▼▼ */
#define pause system("pause > nul")
#define cls system("cls")

/* 플레이어 체력  */
typedef struct{
    double maxhp;
    double nowhp;
    char status[100];
    int stress;
}player;

/* ▼▼▼▼▼ 함수 선언 ▼▼▼▼▼ */
void SetColor(int color);
void CursorView(char show);
void gotoxy(int x, int y);
int sellect(char* situ[]);