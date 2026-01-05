#include "inotify_handler.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>

int init_inotify(const char *watch_dir) {
    // initialize inotify instance
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("inotify_init");
        return -1;
    }
    
    // add a new watch
    int wd = inotify_add_watch(inotify_fd, watch_dir,
                               IN_CREATE | IN_DELETE | IN_MODIFY);
    if (wd == -1) {
        perror("inotify_add_watch");
        close(inotify_fd);
        return -1;
    }
    
    return inotify_fd;
}

char* read_last_message(const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        return NULL;
    }
    
    // check file size
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    
    if (size <= 0) {
        fclose(fp);
        return NULL;
    }
    
    // read entire file
    fseek(fp, 0, SEEK_SET);
    char *content = malloc(size + 1);
    if (!content) {
        fclose(fp);
        return NULL;
    }
    
    fread(content, 1, size, fp);
    content[size] = '\0';
    fclose(fp);
    
    // search last line
    char *last_line = strrchr(content, '\n');
    
    if (last_line && *(last_line + 1) != '\0') {
        // 마지막 개행 뒤에 내용이 있음
        last_line++;
    } else if (!last_line) {
        // 개행이 없음 (한 줄짜리 파일)
        last_line = content;
    } else {
        // 마지막이 개행으로 끝남 -> 그 이전 줄 찾기
        *last_line = '\0';
        last_line = strrchr(content, '\n');
        if (last_line) {
            last_line++;
        } else {
            last_line = content;
        }
    }
    
    // copy result
    char *result = NULL;
    if (strlen(last_line) > 0) {
        result = strdup(last_line);
    }
    
    free(content);
    return result;
}

void strip_txt_extension(char *username) {
    char *dot = strrchr(username, '.');
    if (dot && strcmp(dot, ".txt") == 0) {
        *dot = '\0';  // remove the .txt extension by replacing it with null
    }
}
