#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//강조할 부분 글씨 색 다르게하기
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//영어, 필리핀어 문장 구조체
typedef struct TranslationNode {
    char* english;      // 영어 문장 
    char* filipino;     // 필리핀어 문장
    struct TranslationNode* next;  // 다음 노드를 가리키는 포인터
} TranslationNode;

//연결 리스트 시작
TranslationNode* head = NULL;

//tgl.txt 파일에 데이터를 연결리스트에 저장
void loadTranslationsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char english[1000];
    char filipino[1000]; 

    while (fscanf(file, "%999[^\t]\t%999[^\n]\n", english, filipino) == 2) {
        //동적할당으로 저장
        char* english_copy = strdup(english);
        char* filipino_copy = strdup(filipino);
        if (english_copy == NULL || filipino_copy == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        //노드 생성
        TranslationNode* new_node = (TranslationNode*)malloc(sizeof(TranslationNode));
        if (new_node == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        new_node->english = english_copy;
        new_node->filipino = filipino_copy;
        new_node->next = NULL;

        //노드 추가하는 부분
        if (head == NULL) {
            head = new_node;
        } else {
            TranslationNode* current = head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_node;
        }
    }

    fclose(file);
}

//영어문장과 매치된 필리핀문장 출력함수
void translate(const char* english_sentence) {
    TranslationNode* current = head;
    while (current != NULL) {
        if (strcmp(current->english, english_sentence) == 0) {
            printf(ANSI_COLOR_YELLOW "번역 결과: %s\n" ANSI_COLOR_RESET, current->filipino);
            return;
        }
        current = current->next;
    }

    printf(ANSI_COLOR_RED "해당하는 번역을 찾을 수 없습니다.\n" ANSI_COLOR_RESET);
}

//메모리 해제
void freeTranslationList() {
    TranslationNode* current = head;
    while (current != NULL) {
        TranslationNode* temp = current;
        current = current->next;
        free(temp->english);
        free(temp->filipino);
        free(temp);
    }
    head = NULL;
}

//랜덤으로 10개의 필리핀 문장을 보여주는 함수
void showRandomFilipinoSentences() {
    if (head == NULL) {
        printf(ANSI_COLOR_RED "저장된 번역 데이터가 없습니다.\n" ANSI_COLOR_RESET);
        return;
    }

    srand(time(NULL));

    printf("\n" ANSI_COLOR_GREEN "랜덤으로 선택된 필리핀어 문장 10개:\n" ANSI_COLOR_RESET);
    TranslationNode* current = head;
    int count = 0;
    while (current != NULL && count < 10) {
        int random_index = rand() % 2;//랜덤
        if (random_index == 0) {
            printf("%s\n", current->filipino);
            ++count;
        }
        current = current->next;
    }

    printf("\n영어 번역을 보려면 '" ANSI_COLOR_YELLOW "open" ANSI_COLOR_RESET "'을 입력하세요. 다른 기능을 선택하려면 아무 키나 입력하세요.\n");
}

//선택된 필리핀문장을 영어로 번역
void showRandomEnglishTranslations() {
    if (head == NULL) {
        printf(ANSI_COLOR_RED "저장된 번역 데이터가 없습니다.\n" ANSI_COLOR_RESET);
        return;
    }

    srand(time(NULL));

    printf("\n" ANSI_COLOR_GREEN "랜덤으로 선택된 필리핀어 문장 10개의 영어 번역:\n" ANSI_COLOR_RESET);
    TranslationNode* current = head;
    int count = 0;
    while (current != NULL && count < 10) {
        int random_index = rand() % 2;
        if (random_index == 0) {
            printf("%s -> %s\n", current->filipino, current->english);
            ++count;
        }
        current = current->next;
    }

    printf("\n기능 선택으로 돌아갑니다.\n");
}

int main() {
    int number;
    loadTranslationsFromFile("tgl.txt");

    while (1) {
        printf("\n" ANSI_COLOR_YELLOW "===== 필리핀어 번역기 =====\n" ANSI_COLOR_RESET);
        printf("(1: 번역기, 2: 랜덤 10개의 문장, 3: 프로그램 종료)\n");
        printf("수행 할 기능을 선택하세요: ");
        scanf("%d", &number);
        getchar();

        if (number == 1) {
            printf("영어 문장을 입력하세요 (입력을 중지하려면 'stop'을 입력하세요):\n");

            char input[100];
            while (1) {
                printf("> ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0';

                if (strcmp(input, "stop") == 0) {
                    printf(ANSI_COLOR_YELLOW "번역 프로그램을 종료합니다.\n" ANSI_COLOR_RESET);
                    break;
                }

                translate(input);
            }

            //메모리 해제
            freeTranslationList();
            break;

        } else if (number == 2) {
            showRandomFilipinoSentences();

            char command[10];
            fgets(command, sizeof(command), stdin);
            command[strcspn(command, "\n")] = '\0';

            if (strcmp(command, "open") == 0) {
                showRandomEnglishTranslations();
            } else {
                printf(ANSI_COLOR_YELLOW "기능 선택으로 돌아갑니다.\n" ANSI_COLOR_RESET);
            }
        } else if (number == 3) {
            printf(ANSI_COLOR_YELLOW "번역기 프로그램을 종료합니다.\n" ANSI_COLOR_RESET);
            break;
        } else {
            printf(ANSI_COLOR_RED "잘못된 입력입니다. 다시 입력해주세요.\n" ANSI_COLOR_RESET);
        }
    }

    return 0;
}
