#include "func.h"

int main(void) {
    int choice = -1;

    /* 1. 프로그램 시작 시 초기 샘플 상품 데이터 세팅 */
    init_sample_data();

    /* 2. 메인 컨트롤러 루프 (while 루프 & switch 분기) */
    while (1) {
        print_menu(); // 사용자 메뉴 UI 출력

        /* 메뉴 번호 입력 및 정수 입력 유효성 검증 */
        if (scanf("%d", &choice) != 1) {
            printf("\n[오류] 숫자만 입력해 주세요.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer(); // 개행 문자 제거

        /* 종료 조건 확인 */
        if (choice == 0) {
            printf("\n========================================================\n");
            printf("  스마트 매장 재고 및 매출 관리 시스템을 종료합니다.\n");
            printf("========================================================\n");
            break;
        }

        /* 사용자 선택에 따른 핵심 비즈니스 로직 함수 호출 */
        switch (choice) {
        case 1:
            add_item();             // 상품 등록 및 입고
            break;
        case 2:
            sell_item();            // 상품 판매 및 출고
            break;
        case 3:
            print_all();            // 전체 재고 현황 조회
            break;
        case 4:
            search_item();          // 상품 키워드 탐색
            break;
        case 5:
            check_low_stock();      // 안전재고 경보 및 자동발주
            break;
        case 6:
            sort_by_sales();        // 인기순위 버블 정렬 조회
            break;
        case 7:
            show_sales_report();    // 매출 및 순이익 정산 리포트
            break;
        default:
            printf("\n[경고] 잘못된 메뉴 번호입니다. 0부터 7 사이의 번호를 선택하세요.\n");
            break;
        }
    }

    return 0;
}