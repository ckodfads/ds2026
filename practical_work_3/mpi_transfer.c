#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAG_FILENAME 0
#define TAG_CONTENT 1
#define BUFFER_SIZE 1024

void run_server() {
    char filename[256];
    char buffer[BUFFER_SIZE];
    MPI_Status status;
    int received_bytes;

    printf("[SERVER-Rank0] Dang cho file...\n");

    MPI_Recv(filename, 256, MPI_CHAR, 1, TAG_FILENAME, MPI_COMM_WORLD, &status);
    printf("[SERVER-Rank0] Dang nhan file: %s\n", filename);

    char save_name[300];
    sprintf(save_name, "received_%s", filename);
    
    FILE *fp = fopen(save_name, "wb");
    if (!fp) {
        perror("File open error");
        return;
    }

    while (1) {
        MPI_Recv(buffer, BUFFER_SIZE, MPI_CHAR, 1, TAG_CONTENT, MPI_COMM_WORLD, &status);
        
        MPI_Get_count(&status, MPI_CHAR, &received_bytes);

        if (received_bytes == 0) {
            break;
        }

        fwrite(buffer, 1, received_bytes, fp);
    }

    fclose(fp);
    printf("[SERVER-Rank0] Da luu file thanh cong: %s\n", save_name);
}

void run_client(char *filename) {
    char buffer[BUFFER_SIZE];
    FILE *fp = fopen(filename, "rb");
    
    if (!fp) {
        printf("[CLIENT-Rank1] Loi: Khong tim thay file '%s'\n", filename);
        MPI_Send("", 0, MPI_CHAR, 0, TAG_FILENAME, MPI_COMM_WORLD); 
        return;
    }

    printf("[CLIENT-Rank1] Dang gui file '%s'...\n", filename);

    MPI_Send(filename, strlen(filename) + 1, MPI_CHAR, 0, TAG_FILENAME, MPI_COMM_WORLD);

    int bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        MPI_Send(buffer, bytes_read, MPI_CHAR, 0, TAG_CONTENT, MPI_COMM_WORLD);
    }

    MPI_Send(buffer, 0, MPI_CHAR, 0, TAG_CONTENT, MPI_COMM_WORLD);

    fclose(fp);
    printf("[CLIENT-Rank1] Gui file hoan tat!\n");
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank == 0) {
        run_server();
    } else if (world_rank == 1) {
        if (argc < 2) {
            printf("Usage: mpirun -n 2 ./mpi_transfer <filename>\n");
        } else {
            run_client(argv[1]);
        }
    }

    MPI_Finalize();
    return 0;
}