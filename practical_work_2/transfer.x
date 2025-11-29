/* transfer.x */
struct file_data {
    string name<256>;
    opaque data<>;
    int bytes_sent;
};

program FILE_TRANSFER_PROG {
    version V1 {
        int UPLOAD_FILE(file_data) = 1;
    } = 1;
} = 0x31230000;