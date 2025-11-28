// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char filename[100];

    // Nhập tên file muốn gửi
    printf("Nhap ten file muon gui: ");
    scanf("%s", filename);

    // Mở file trước để kiểm tra có tồn tại không
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("Khong tim thay file!");
        return -1;
    }

    // 1. Tạo socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Chuyển IP (localhost)
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 2. Kết nối
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // --- BẮT ĐẦU GIAO THỨC GỬI FILE ---

    // Bước A: Gửi tên file trước
    send(sock, filename, strlen(filename), 0);
    printf("[CLIENT] Da gui ten file. Doi Server xac nhan...\n");

    // Bước B: Chờ Server phản hồi "OK"
    read(sock, buffer, BUFFER_SIZE);
    if (strcmp(buffer, "OK") != 0) {
        printf("Server tu choi hoac loi!\n");
        fclose(fp);
        close(sock);
        return -1;
    }

    // Bước C: Gửi nội dung file
    printf("[CLIENT] Bat dau gui du lieu...\n");
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        send(sock, buffer, bytes_read, 0);
    }

    printf("[CLIENT] Gui file thanh cong!\n");

    fclose(fp);
    close(sock);
    return 0;
}