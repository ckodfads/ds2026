#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LEN 4096

void mapper(char *filename, char *longest_local) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("File open error");
        strcpy(longest_local, "");
        return;
    }

    char line[MAX_PATH_LEN];
    int max_len = 0;
    strcpy(longest_local, "");

    // Đọc từng dòng
    while (fgets(line, sizeof(line), fp)) {
        // Xóa ký tự xuống dòng ở cuối
        line[strcspn(line, "\n")] = 0;
        
        int current_len = strlen(line);
        if (current_len > max_len) {
            max_len = current_len;
            strcpy(longest_local, line);
        }
    }
    fclose(fp);
    printf("[MAPPER] File '%s' -> Max len: %d\n", filename, max_len);
}

// --- REDUCER FUNCTION ---
// Nhiệm vụ: Tìm chuỗi dài nhất trong danh sách các kết quả Mapper
void reducer(char longest_candidates[][MAX_PATH_LEN], int count, char *final_result) {
    int max_len = 0;
    strcpy(final_result, "");

    for (int i = 0; i < count; i++) {
        int len = strlen(longest_candidates[i]);
        if (len > max_len) {
            max_len = len;
            strcpy(final_result, longest_candidates[i]);
        }
    }
    printf("[REDUCER] Processing %d candidates -> Global Max len: %d\n", count, max_len);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }

    int num_files = argc - 1;
    char candidates[num_files][MAX_PATH_LEN]; // Mảng lưu kết quả từ các Mapper

    // --- PHASE 1: MAP ---
    // Giả lập chạy song song: Gọi Mapper cho từng file input
    printf("--- MAP PHASE ---\n");
    for (int i = 0; i < num_files; i++) {
        mapper(argv[i + 1], candidates[i]);
    }

    // --- PHASE 2: REDUCE ---
    // Gom kết quả lại để tìm ra cái dài nhất tổng thể
    printf("\n--- REDUCE PHASE ---\n");
    char result[MAX_PATH_LEN];
    reducer(candidates, num_files, result);

    // --- OUTPUT ---
    printf("\n=== FINAL RESULT ===\n");
    printf("Longest Path: %s\n", result);
    printf("Length: %ld\n", strlen(result));

    return 0;
}