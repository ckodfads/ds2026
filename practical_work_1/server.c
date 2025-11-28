// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 1. Tạo socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Cấu hình địa chỉ
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 2. Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] Dang cho ket noi o cong %d...\n", PORT);

    // 4. Accept
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] Client da ket noi!\n");

    // --- BẮT ĐẦU GIAO THỨC NHẬN FILE ---
    
    // Bước A: Nhận tên file
    recv(new_socket, buffer, BUFFER_SIZE, 0);
    printf("[SERVER] Dang nhan file: %s\n", buffer);
    
    // Mở file để ghi (wb = write binary)
    // Lưu file với tên nhận được, thêm prefix "received_" để tránh ghi đè file gốc nếu chạy chung thư mục
    char filename[BUFFER_SIZE + 10];
    sprintf(filename, "received_%s", buffer);
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("File open error");
        exit(1);
    }

    // Bước B: Gửi xác nhận (ACK) cho Client biết đã sẵn sàng
    send(new_socket, "OK", 2, 0);

    // Bước C: Nhận nội dung file
    int bytes_received;
    memset(buffer, 0, BUFFER_SIZE); // Xóa sạch buffer
    while ((bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, fp);
        memset(buffer, 0, BUFFER_SIZE);
    }

    printf("[SERVER] Da nhan file xong. Luu tai: %s\n", filename);

    fclose(fp);
    close(new_socket);
    close(server_fd);
    return 0;
}