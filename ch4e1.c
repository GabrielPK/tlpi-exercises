// Exercise 4-1

#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char* argv[]) {
    int outputFd, openFlags;
    mode_t filePerms;
    char buffer[BUF_SIZE];
    ssize_t numRead;
    
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    
    // Check for correct arguments
    if (argc < 2 || argc > 3 || (argc == 3 && strcmp(argv[1], "-a") != 0)) {
        usageErr("%s [-a] dest-file\n", argv[0]);
    }

    // Open the file
    if (argc == 2 || (argc == 3 && strcmp(argv[1], "-a") == 0)) {
        openFlags = O_WRONLY | O_CREAT;
        openFlags |= (argc == 2) ? O_TRUNC : O_APPEND;
        outputFd = open(argv[argc - 1], openFlags, filePerms);
        if (outputFd == -1)
            errExit("opening file %s", argv[argc - 1]);
    }

    while ((numRead = read(STDIN_FILENO, buffer, BUF_SIZE - 1)) > 0) {
        if (numRead == -1)
            errExit("read");
        buffer[numRead] = '\0';
        printf("%s\n", buffer);

        if (write(outputFd, buffer, numRead) != numRead)
            fatal("write() returned error or partial write occurred");
    }

    if (numRead == -1)
        errExit("read");

    if (close(outputFd) == -1)
        errExit("close output");
    
    exit(EXIT_SUCCESS);
}