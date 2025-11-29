/* transfer_client.c */
#include "transfer.h"
#include <stdio.h>
#include <stdlib.h>

void
file_transfer_prog_1(char *host, char *filename)
{
    CLIENT *clnt;
    int  *result_1;
    file_data  upload_file_1_arg;
    FILE *fp;

    // 1. Kết nối đến Server RPC
    clnt = clnt_create(host, FILE_TRANSFER_PROG, V1, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    // 2. Đọc file từ ổ cứng
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Khong tim thay file: %s\n", filename);
        exit(1);
    }

    // Lấy kích thước file
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Cấp phát bộ nhớ cho dữ liệu gửi đi
    upload_file_1_arg.name = filename;
    upload_file_1_arg.bytes_sent = filesize;
    upload_file_1_arg.data.data_val = (char *)malloc(filesize);
    upload_file_1_arg.data.data_len = filesize;

    // Đọc toàn bộ file vào bộ nhớ
    fread(upload_file_1_arg.data.data_val, 1, filesize, fp);
    fclose(fp);

    printf("[CLIENT] Dang gui file '%s' (%ld bytes) qua RPC...\n", filename, filesize);

    // 3. GỌI HÀM TỪ XA (Remote Procedure Call)
    result_1 = upload_file_1(&upload_file_1_arg, clnt);
    if (result_1 == (int *) NULL) {
        clnt_perror(clnt, "Call failed");
    } else {
        if (*result_1 == 1) printf("[CLIENT] Thanh cong! Server da nhan file.\n");
        else printf("[CLIENT] That bai! Server bao loi.\n");
    }

    // Dọn dẹp
    free(upload_file_1_arg.data.data_val);
    clnt_destroy(clnt);
}

int
main(int argc, char *argv[])
{
    char *host;
    char *filename;

    if (argc < 3) {
        printf("usage: %s server_host filename\n", argv[0]);
        exit(1);
    }
    host = argv[1];
    filename = argv[2];
    file_transfer_prog_1(host, filename);
    exit(0);
}