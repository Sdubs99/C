#define _CRT_SECURE_NO_WARNINGS    // Visual Studio에서 strcpy 경고(오류)를 막기 위한 설정
#include <stdio.h>
#include <string.h>    // memset, memcpy, strcpy 함수가 선언된 헤더 파일
#include <stddef.h>    // offsetof 매크로가 정의된 헤더 파일

#define ITEM_COUNT 3     // 인벤토리 칸 수
#define TYPE_WEAPON 1    // 아이템 종류: 무기
#define TYPE_POTION 2    // 아이템 종류: 포션

// ───────────────── Unit 51. 구조체 멤버 정렬 ─────────────────
struct NormalInfo {      // 정렬을 따로 지정하지 않은 구조체
    char grade;          // 1바이트
    int price;           // 4바이트
};

#pragma pack(push, 1)    // 지금부터 1바이트 단위로 정렬
struct PackedInfo {      // 1바이트 정렬을 적용한 구조체
    char grade;          // 1바이트
    int price;           // 4바이트
};
#pragma pack(pop)        // 정렬 설정을 원래대로 되돌림

// ───────────────── Unit 54. 공용체 ─────────────────
union Data {             // 모든 멤버가 같은 메모리 공간을 함께 사용
    char c;              // 1바이트
    short s;             // 2바이트
    int i;               // 4바이트
};

// ───────────────── Unit 55. 구조체와 공용체 활용 ─────────────────
struct Item {
    char name[20];       // 아이템 이름
    int type;            // 아이템 종류 (TYPE_WEAPON 또는 TYPE_POTION)
    union {              // 익명 공용체: 이름이 없어서 멤버에 바로 접근 가능
        int attack;      // 무기일 때 사용: 공격력
        int heal;        // 포션일 때 사용: 회복량
    };
};

// 인벤토리(구조체 배열)를 출력하는 함수
void printInventory(struct Item items[], int count)
{
    int i;    // 반복문에 사용할 변수

    for (i = 0; i < count; i++)
    {
        if (items[i].type == TYPE_WEAPON)
            printf("  [%d] %s (무기) 공격력: %d\n", i, items[i].name, items[i].attack);
        else if (items[i].type == TYPE_POTION)
            printf("  [%d] %s (포션) 회복량: %d\n", i, items[i].name, items[i].heal);
        else
            printf("  [%d] (빈 칸)\n", i);
    }
}

int main()
{
    // ── 1. 구조체 멤버 정렬 (Unit 51) ──
    printf("=== 1. 구조체 멤버 정렬 ===\n");
    printf("NormalInfo 크기: %d바이트\n", (int)sizeof(struct NormalInfo));
    printf("PackedInfo 크기: %d바이트\n", (int)sizeof(struct PackedInfo));
    printf("NormalInfo에서 price의 위치: %d\n", (int)offsetof(struct NormalInfo, price));
    printf("PackedInfo에서 price의 위치: %d\n", (int)offsetof(struct PackedInfo, price));

    // ── 2. 구조체 배열 선언 + memset으로 초기화 (Unit 53, 52) ──
    printf("\n=== 2. 구조체 배열을 memset으로 0 초기화 ===\n");
    struct Item inventory[ITEM_COUNT];          // 구조체 배열 선언
    memset(inventory, 0, sizeof(inventory));    // 배열 전체를 0으로 채움
    printInventory(inventory, ITEM_COUNT);

    // ── 3. 구조체 배열의 요소에 값 넣기 (Unit 53, 55) ──
    printf("\n=== 3. 아이템 넣기 ===\n");
    strcpy(inventory[0].name, "철검");
    inventory[0].type = TYPE_WEAPON;
    inventory[0].attack = 50;                   // 무기이므로 attack 사용

    strcpy(inventory[1].name, "빨간 포션");
    inventory[1].type = TYPE_POTION;
    inventory[1].heal = 30;                     // 포션이므로 heal 사용

    strcpy(inventory[2].name, "나무 활");
    inventory[2].type = TYPE_WEAPON;
    inventory[2].attack = 35;

    printInventory(inventory, ITEM_COUNT);

    // ── 4. memcpy로 구조체 배열 복사 (Unit 52) ──
    printf("\n=== 4. memcpy로 인벤토리 백업 ===\n");
    struct Item backup[ITEM_COUNT];
    memcpy(backup, inventory, sizeof(inventory));   // inventory 내용을 backup에 그대로 복사

    inventory[0].attack = 999;                      // 원본만 수정 (철검 강화!)

    printf("원본 인벤토리:\n");
    printInventory(inventory, ITEM_COUNT);
    printf("백업 인벤토리:\n");
    printInventory(backup, ITEM_COUNT);

    // ── 5. 공용체 (Unit 54) ──
    printf("\n=== 5. 공용체 ===\n");
    union Data d;
    printf("Data 크기: %d바이트\n", (int)sizeof(d));   // 가장 큰 멤버(int)의 크기

    d.i = 65;                                   // int 멤버에 65를 저장
    printf("d.i = %d\n", d.i);
    printf("d.s = %d\n", d.s);                  // 같은 공간을 쓰므로 65
    printf("d.c = %c\n", d.c);                  // 65는 문자 'A'의 ASCII 코드

    printf("철검의 attack: %d, heal: %d\n", inventory[0].attack, inventory[0].heal);

    return 0;
}
