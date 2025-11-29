#include "transfer.h"
#include <stdio.h>

int *
upload_file_1_svc(file_data *argp, struct svc_req *rqstp)
{
    static int result;

    printf("[SERVER] Nhan duoc file: %s (%d bytes)\n", argp->name, argp->bytes_sent);
    char filename[300];
    sprintf(filename, "server_%s", argp->name);


    FILE *fp = fopen(filename, "wb"); 
    if (fp == NULL) {
        perror("Loi mo file");
        result = 0; 
        return &result;
    }

    // Ghi dữ liệu từ argp->data.val vào file
    fwrite(argp->data.data_val, 1, argp->bytes_sent, fp);
    fclose(fp);

    printf("[SERVER] Da luu file thanh cong!\n");
    result = 1; // Thanh cong

    return &result;
}