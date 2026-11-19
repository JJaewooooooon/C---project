#include "func.h"

/* 전역 구조체 배열 및 매출 통계 배열 정의 */
Product inventory[MAX_ITEMS];
int item_count = 0;
int category_sales[MAX_CATEGORY] = { 0, 0, 0, 0 };
const char* category_names[MAX_CATEGORY] = { "음료", "스낵", "생활용품", "신선식품" };

/* 표준 입력 버퍼 비우기 */
void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/* 품번(ID)으로 배열 인덱스 검색 */
int find_item_index_by_id(int id) {
    for (int i = 0; i < item_count; i++) {
        if (inventory[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* 초기 샘플 재고 데이터 5종 세팅 */
void init_sample_data(void) {
    Product samples[5] = {
        { 1001, "제주삼다수500ml", 0,  450, 1000, 25, 10, 12 },
        { 1002, "아메리카노캔",     0,  700, 1800,  4, 10, 20 },
        { 2001, "포카칩오리지널",   1,  900, 1700, 15,  8,  9 },
        { 3001, "물티슈100매",      2,  800, 2000,  3,  5,  7 },
        { 4001, "친환경우유1L",     3, 1800, 3200, 12,  6, 15 }
    };

    item_count = 5;
    for (int i = 0; i < item_count; i++) {
        inventory[i] = samples[i];
        int cat = inventory[i].category;
        category_sales[cat] += inventory[i].sales_count * inventory[i].sell_price;
    }

    printf("========================================================\n");
    printf(" [시스템 알림] 초기 샘플 데이터 %d종 세팅이 완료되었습니다.\n", item_count);
    printf("========================================================\n");
}

/* 사용자 메뉴 UI 출력 */
void print_menu(void) {
    printf("\n+------------------------------------------------------+\n");
    printf("|      스마트 매장 재고 및 매출 관리 시스템 (POS)      |\n");
    printf("+------------------------------------------------------+\n");
    printf("|  1. 상품 등록 및 입고      (add_item)                |\n");
    printf("|  2. 상품 판매 및 출고      (sell_item)               |\n");
    printf("|  3. 전체 재고 현황 조회    (print_all)               |\n");
    printf("|  4. 상품 키워드 탐색       (search_item)             |\n");
    printf("|  5. 안전재고 경보/자동발주 (check_low_stock)         |\n");
    printf("|  6. 인기순위 버블 정렬     (sort_by_sales)           |\n");
    printf("|  7. 매출 및 순이익 리포트  (show_sales_report)       |\n");
    printf("|  0. 프로그램 종료                                    |\n");
    printf("+------------------------------------------------------+\n");
    printf(" 메뉴 선택 (1~7, 종료:0) >> ");
}

/* 1. 상품 신규 등록 및 입고 */
void add_item(void) {
    int id, index, add_qty;

    printf("\n[1. 상품 등록 및 입고]\n");
    printf("입고할 상품 품번(ID, 예: 1001) 입력: ");
    if (scanf("%d", &id) != 1 || id <= 0) {
        printf("[오류] 올바른 품번(양의 정수)을 입력하세요.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    index = find_item_index_by_id(id);

    if (index != -1) {
        printf(" -> 기존 상품 확인: [%s] (현재 재고: %d개)\n",
            inventory[index].name, inventory[index].stock);
        printf("추가 입고할 수량 입력: ");
        if (scanf("%d", &add_qty) != 1 || add_qty <= 0) {
            printf("[오류] 입고 수량은 1개 이상이어야 합니다.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();

        inventory[index].stock += add_qty;
        printf("[입고 완료] '%s' 재고가 %d개로 증가했습니다.\n",
            inventory[index].name, inventory[index].stock);
        return;
    }

    if (item_count >= MAX_ITEMS) {
        printf("[경고] 최대 저장 가능한 상품 수(%d개)를 초과했습니다.\n", MAX_ITEMS);
        return;
    }

    Product new_item;
    new_item.id = id;
    new_item.sales_count = 0;

    printf("신규 상품명 입력 (공백 없이 입력): ");
    scanf("%49s", new_item.name);
    clear_input_buffer();

    printf("카테고리 선택 (0:음료, 1:스낵, 2:생활용품, 3:신선식품): ");
    if (scanf("%d", &new_item.category) != 1 || new_item.category < 0 || new_item.category >= MAX_CATEGORY) {
        printf("[오류] 카테고리는 0~3 사이의 번호만 입력 가능합니다.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    printf("입고 원가(원) 입력: ");
    if (scanf("%d", &new_item.cost_price) != 1 || new_item.cost_price < 0) {
        printf("[오류] 올바른 원가를 입력하세요.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    printf("판매 단가(원) 입력: ");
    if (scanf("%d", &new_item.sell_price) != 1 || new_item.sell_price < new_item.cost_price) {
        printf("[오류] 판매 단가는 원가(%d원) 이상이어야 합니다.\n", new_item.cost_price);
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    printf("초기 입고 수량 입력: ");
    if (scanf("%d", &new_item.stock) != 1 || new_item.stock < 0) {
        printf("[오류] 올바른 초기 재고 수량을 입력하세요.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    printf("안전 재고 기준 수량 입력: ");
    if (scanf("%d", &new_item.safe_stock) != 1 || new_item.safe_stock < 0) {
        printf("[오류] 올바른 안전 재고 수량을 입력하세요.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    inventory[item_count] = new_item;
    item_count++;

    printf("[등록 완료] 신규 상품 '%s'(품번: %d) 등록이 완료되었습니다.\n",
        new_item.name, new_item.id);
}

/* 2. 상품 판매 및 출고 */
void sell_item(void) {
    int id, index, sell_qty;

    printf("\n[2. 상품 판매 및 출고 (POS)]\n");
    printf("판매할 상품 품번(ID) 입력: ");
    if (scanf("%d", &id) != 1) {
        printf("[오류] 숫자로 된 품번을 입력하세요.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    index = find_item_index_by_id(id);
    if (index == -1) {
        printf("[오류] 해당 품번(%d)의 상품이 존재하지 않습니다.\n", id);
        return;
    }

    printf(" -> 선택 상품: [%s] | 판매가: %d원 | 현재 재고: %d개\n",
        inventory[index].name, inventory[index].sell_price, inventory[index].stock);

    printf("판매(출고) 수량 입력: ");
    if (scanf("%d", &sell_qty) != 1 || sell_qty <= 0) {
        printf("[오류] 판매 수량은 1개 이상이어야 합니다.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (inventory[index].stock < sell_qty) {
        printf("[판매 불가] 재고가 부족합니다! (요청: %d개, 현재고: %d개)\n",
            sell_qty, inventory[index].stock);
        return;
    }

    inventory[index].stock -= sell_qty;
    inventory[index].sales_count += sell_qty;

    int total_price = sell_qty * inventory[index].sell_price;
    int cat = inventory[index].category;
    category_sales[cat] += total_price;

    printf("[판매 완료] '%s' %d개 출고 (결제 금액: %d원 / 남은 재고: %d개)\n",
        inventory[index].name, sell_qty, total_price, inventory[index].stock);

    if (inventory[index].stock < inventory[index].safe_stock) {
        printf(" * [안전재고 경보] '%s' 현재고(%d개)가 안전재고(%d개) 미만입니다!\n",
            inventory[index].name, inventory[index].stock, inventory[index].safe_stock);
    }
}

/* 3. 전체 재고 현황 조회 */
void print_all(void) {
    printf("\n[3. 전체 재고 현황 조회 (총 %d종)]\n", item_count);
    printf("=========================================================================================\n");
    printf("%-6s | %-18s | %-8s | %8s | %8s | %6s | %8s | %6s | %-6s\n",
        "품번", "상품명", "카테고리", "원가", "판매가", "현재고", "안전재고", "판매량", "상태");
    printf("-----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < item_count; i++) {
        const char* status = (inventory[i].stock < inventory[i].safe_stock) ? "부족!" : "정상";
        printf("%-6d | %-18s | %-8s | %7d원 | %7d원 | %5d개 | %7d개 | %5d개 | %-6s\n",
            inventory[i].id,
            inventory[i].name,
            category_names[inventory[i].category],
            inventory[i].cost_price,
            inventory[i].sell_price,
            inventory[i].stock,
            inventory[i].safe_stock,
            inventory[i].sales_count,
            status);
    }
    printf("=========================================================================================\n");
}

/* 4. 상품 키워드 탐색 */
void search_item(void) {
    char keyword[NAME_LEN];
    int found_count = 0;

    printf("\n[4. 상품 키워드 탐색]\n");
    printf("검색할 상품명 키워드 입력: ");
    scanf("%49s", keyword);
    clear_input_buffer();

    printf("\n-----------------------------------------------------------------------------------------\n");
    printf("%-6s | %-18s | %-8s | %8s | %8s | %6s | %6s\n",
        "품번", "상품명", "카테고리", "원가", "판매가", "현재고", "판매량");
    printf("-----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < item_count; i++) {
        if (strstr(inventory[i].name, keyword) != NULL) {
            printf("%-6d | %-18s | %-8s | %7d원 | %7d원 | %5d개 | %5d개\n",
                inventory[i].id,
                inventory[i].name,
                category_names[inventory[i].category],
                inventory[i].cost_price,
                inventory[i].sell_price,
                inventory[i].stock,
                inventory[i].sales_count);
            found_count++;
        }
    }
    printf("-----------------------------------------------------------------------------------------\n");

    if (found_count == 0) {
        printf(" -> 키워드 '%s'와(과) 일치하는 상품을 찾지 못했습니다.\n", keyword);
    }
    else {
        printf(" -> 총 %d건의 상품이 검색되었습니다.\n", found_count);
    }
}

/* 5. 안전재고 경보 및 자동발주 */
void check_low_stock(void) {
    int low_count = 0;
    char answer;

    printf("\n[5. 안전재고 경보 및 자동발주 시스템]\n");
    printf("-------------------------------------------------------------------------\n");
    for (int i = 0; i < item_count; i++) {
        if (inventory[i].stock < inventory[i].safe_stock) {
            int shortage = inventory[i].safe_stock - inventory[i].stock;
            printf(" [경보] 품번:%d | %-16s | 현재고:%3d개 | 안전재고:%3d개 (부족분:%3d개)\n",
                inventory[i].id, inventory[i].name,
                inventory[i].stock, inventory[i].safe_stock, shortage);
            low_count++;
        }
    }
    printf("-------------------------------------------------------------------------\n");

    if (low_count == 0) {
        printf(" -> 현재 안전재고 미만인 상품이 없습니다.\n");
        return;
    }

    printf(" -> 총 %d종의 상품이 안전재고 미만입니다.\n", low_count);
    printf(" -> 부족 상품을 안전재고의 2배수 수량으로 자동 발주하시겠습니까? (Y/N): ");
    scanf(" %c", &answer);
    clear_input_buffer();

    if (answer == 'Y' || answer == 'y') {
        printf("\n[자동 발주 처리 결과]\n");
        for (int i = 0; i < item_count; i++) {
            if (inventory[i].stock < inventory[i].safe_stock) {
                int target_stock = inventory[i].safe_stock * 2;
                int order_qty = target_stock - inventory[i].stock;
                inventory[i].stock += order_qty;
                printf(" * [%s] %d개 자동 발주 및 입고 완료 -> 현재 재고: %d개\n",
                    inventory[i].name, order_qty, inventory[i].stock);
            }
        }
    }
    else {
        printf(" -> 자동 발주를 취소했습니다.\n");
    }
}

/* 6. 인기순위 버블 정렬 조회 */
void sort_by_sales(void) {
    Product sorted[MAX_ITEMS];

    for (int i = 0; i < item_count; i++) {
        sorted[i] = inventory[i];
    }

    for (int i = 0; i < item_count - 1; i++) {
        for (int j = 0; j < item_count - 1 - i; j++) {
            if (sorted[j].sales_count < sorted[j + 1].sales_count) {
                Product temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    printf("\n[6. 상품 인기순위 조회 (판매량 기준 내림차순 - 버블 정렬)]\n");
    printf("=========================================================================\n");
    printf("%-4s | %-6s | %-18s | %-8s | %8s | %10s\n",
        "순위", "품번", "상품명", "카테고리", "누적판매", "누적매출액");
    printf("-------------------------------------------------------------------------\n");

    for (int i = 0; i < item_count; i++) {
        int item_revenue = sorted[i].sales_count * sorted[i].sell_price;
        printf("%2d위 | %-6d | %-18s | %-8s | %6d개 | %9d원\n",
            i + 1,
            sorted[i].id,
            sorted[i].name,
            category_names[sorted[i].category],
            sorted[i].sales_count,
            item_revenue);
    }
    printf("=========================================================================\n");
}

/* 7. 총매출, 원가, 순이익률 및 카테고리 통계 리포트 */
void show_sales_report(void) {
    long long total_revenue = 0;
    long long total_cost = 0;
    long long net_profit = 0;
    double profit_margin = 0.0;

    for (int i = 0; i < item_count; i++) {
        total_revenue += (long long)inventory[i].sales_count * inventory[i].sell_price;
        total_cost += (long long)inventory[i].sales_count * inventory[i].cost_price;
    }

    net_profit = total_revenue - total_cost;

    if (total_revenue > 0) {
        profit_margin = ((double)net_profit / (double)total_revenue) * 100.0;
    }

    printf("\n[7. 스마트 매장 매출 및 순이익 정산 리포트]\n");
    printf("========================================================\n");
    printf(" [A] 전체 재무 정산 요약\n");
    printf("--------------------------------------------------------\n");
    printf("  * 총 매출액 (Gross Sales) : %12lld 원\n", total_revenue);
    printf("  * 총 매출원가 (COGS)      : %12lld 원\n", total_cost);
    printf("  * 순이익 (Net Profit)     : %12lld 원\n", net_profit);
    printf("  * 순이익률 (Profit Margin): %11.2f %%\n", profit_margin);
    printf("========================================================\n");
    printf(" [B] 카테고리 배열(category_sales[4]) 판매 통계 분석\n");
    printf("--------------------------------------------------------\n");

    int best_cat = 0;
    for (int c = 0; c < MAX_CATEGORY; c++) {
        double share = (total_revenue > 0)
            ? ((double)category_sales[c] / (double)total_revenue) * 100.0
            : 0.0;
        printf("  * [%d] %-8s : %10d 원 (비중: %5.1f%%)\n",
            c, category_names[c], category_sales[c], share);

        if (category_sales[c] > category_sales[best_cat]) {
            best_cat = c;
        }
    }
    printf("--------------------------------------------------------\n");
    if (total_revenue > 0) {
        printf("  -> 최다 매출 효자 카테고리: [%s] (%d원)\n",
            category_names[best_cat], category_sales[best_cat]);
    }
    printf("========================================================\n");
}