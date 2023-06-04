#include <stdio.h>

int main() {
    int sum = 0;
    int num;

    printf("숫자를 입력하세요 (0을 입력하면 종료): ");
    fflush(stdout);

    while (1) {
        scanf("%d", &num);

        if (num == 0) {
            break;
        }

        sum += num;
        printf("현재 합계: %d\n", sum);
        printf("숫자를 입력하세요 (0을 입력하면 종료): ");
        fflush(stdout);
    }

    printf("프로그램을 종료합니다.\n");

    return 0;
}
