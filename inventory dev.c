#define _CRT_SECURE_NO_WARNINGS    
#include <stdio.h>
#include <string.h>    
#include <stddef.h>    

#define ITEM_COUNT 3     
#define TYPE_WEAPON 1    
#define TYPE_POTION 2    

struct NormalInfo {     
    char grade;         
    int price;          
};

#pragma pack(push, 1)   
struct PackedInfo {      
    char grade;          
    int price;           
};
#pragma pack(pop)       

union Data {             
    char c;              
    short s;             
    int i;              
};

struct Item {
    char name[20];      
    int type;            
    union {              
        int attack;      
        int heal;        
    };
};

void printInventory(struct Item items[], int count)
{
    int i;    

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
    printf("=== 1. 구조체 멤버 정렬 ===\n");
    printf("NormalInfo 크기: %d바이트\n", (int)sizeof(struct NormalInfo));
    printf("PackedInfo 크기: %d바이트\n", (int)sizeof(struct PackedInfo));
    printf("NormalInfo에서 price의 위치: %d\n", (int)offsetof(struct NormalInfo, price));
    printf("PackedInfo에서 price의 위치: %d\n", (int)offsetof(struct PackedInfo, price));

    printf("\n=== 2. 구조체 배열을 memset으로 0 초기화 ===\n");
    struct Item inventory[ITEM_COUNT];          // 구조체 배열 선언
    memset(inventory, 0, sizeof(inventory));    // 배열 전체를 0으로 채움
    printInventory(inventory, ITEM_COUNT);

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

    printf("\n=== 4. memcpy로 인벤토리 백업 ===\n");
    struct Item backup[ITEM_COUNT];
    memcpy(backup, inventory, sizeof(inventory));   // inventory 내용을 backup에 그대로 복사

    inventory[0].attack = 999;                      // 원본만 수정 (철검 강화!)

    printf("원본 인벤토리:\n");
    printInventory(inventory, ITEM_COUNT);
    printf("백업 인벤토리:\n");
    printInventory(backup, ITEM_COUNT);

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
