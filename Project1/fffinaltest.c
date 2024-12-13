#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    int korean;
    int english;
    int math;
    int total;
    char name[16];
    struct node* p_next;
} NODE;

// 성적 입력 함수
void InputGrade(NODE** pp_head, NODE** pp_tail, unsigned short* ap_count) {
    NODE* p_node = (NODE*)malloc(sizeof(NODE)), * p_temp_node, * p_prev_node;

    printf("%d 번째 학생 이름: ", *ap_count + 1);
    scanf("%s", p_node->name);

    printf("국어 점수: ");
    scanf("%d", &p_node->korean);

    printf("영어 점수: ");
    scanf("%d", &p_node->english);

    printf("수학 점수: ");
    scanf("%d", &p_node->math);

    p_node->total = p_node->korean + p_node->english + p_node->math;

    p_temp_node = *pp_head;
    p_prev_node = NULL;

    while (p_temp_node != NULL && p_node->total < p_temp_node->total) {
        p_prev_node = p_temp_node;
        p_temp_node = p_temp_node->p_next;
    }

    if (*pp_head == NULL) {
        *pp_head = p_node;
        *pp_tail = *pp_head;
        (*pp_tail)->p_next = NULL;

    }
    else if (p_temp_node == *pp_head) {
        p_node->p_next = *pp_head;
        *pp_head = p_node;
    }
    else if (p_temp_node == NULL) {
        (*pp_tail)->p_next = p_node;
        *pp_tail = (*pp_tail)->p_next;
        (*pp_tail)->p_next = NULL;

    }
    else {
        p_node->p_next = p_prev_node->p_next;
        p_prev_node->p_next = p_node;
    }
    (*ap_count)++;
}

// 성적 출력 함수
void PrintGrade(NODE** pp_head) {
    printf("----------------------------------\n");
    printf("이름   국어 영어 수학 총점 평균 등수\n");
    printf("----------------------------------\n");

    NODE* p_node = *pp_head;
    char rank = 1;

    while (p_node != NULL) {
        printf("%-6s %3d %3d %3d %4d %5d %d등\n",
            p_node->name, p_node->korean, p_node->english,
            p_node->math, p_node->total, p_node->total / 3, rank);
        p_node = p_node->p_next;
        rank++;
    }
}

// 모든 노드를 삭제하는 함수
void DeleteAllNode(NODE* p_head) {
    NODE* p_node;
    while (p_head != NULL) {
        p_node = p_head;
        p_head = p_head->p_next;
        free(p_node);
    }
}

// .dat 파일에 저장하는 함수
void SaveToFile(NODE* p_head, unsigned short count) {
    FILE* file = fopen("students.dat", "wb");
    if (file == NULL) {
        printf("파일 저장 실패.\n");
        return;
    }

    fwrite(&count, sizeof(unsigned short), 1, file);

    NODE* p_node = p_head;
    while (p_node != NULL) {
        fwrite(p_node, sizeof(NODE), 1, file);
        p_node = p_node->p_next;
    }

    fclose(file);
    printf("데이터가 파일에 저장되었습니다.\n");
}

// .dat 파일에서 데이터를 읽는 함수
void ReadFromFile(NODE** pp_head, NODE** pp_tail, unsigned short* ap_count) {
    FILE* file = fopen("students.dat", "rb");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return;
    }

    fread(ap_count, sizeof(unsigned short), 1, file);

    for (int i = 0; i < *ap_count; i++) {
        NODE* p_node = (NODE*)malloc(sizeof(NODE));
        fread(p_node, sizeof(NODE), 1, file);

        p_node->p_next = NULL;

        if (*pp_head == NULL) {
            *pp_head = p_node;
            *pp_tail = *pp_head;
        }
        else {
            (*pp_tail)->p_next = p_node;
            *pp_tail = p_node;
        }
    }

    fclose(file);
    printf("파일에서 데이터를 성공적으로 읽었습니다.\n");
}

// 메인 함수
int main(void) {
    NODE* p_head = NULL, * p_tail = NULL;
    unsigned short count = 0;
    short menu;

    while (1) {
        printf("[MENU]\n");
        printf(" 1. .dat 파일에서 데이터 읽기\n");
        printf(" 2. 추가 학생 성적 입력\n");
        printf(" 3. .dat 파일 저장\n");
        printf(" 4. 성적 확인\n");
        printf(" 5. 종료\n");
        printf("-------------------------\n");
        printf("선택 (1 ~ 5): ");
        scanf("%hd", &menu);

        if (menu == 1) {
            ReadFromFile(&p_head, &p_tail, &count);
        }
        else if (menu == 2) {
            InputGrade(&p_head, &p_tail, &count);
        }
        else if (menu == 3) {
            SaveToFile(p_head, count);
        }
        else if (menu == 4) {
            PrintGrade(&p_head);
        }
        else if (menu == 5) {
            break;
        }
        else {
            printf("잘못된 입력입니다. 다시 시도하세요.\n");
        }
    }

    DeleteAllNode(p_head);
    return 0;
}