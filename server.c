#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

int main(int argc, char** argv){

 	if (mkdir("/tmp/chat", 0755) == -1 && errno != EEXIST) {
                perror("mkdir /tmp/chat");
                exit(EXIT_FAILURE);
        }
        
        if (mkdir("/tmp/chat/clients", 0755) == -1 && errno != EEXIST) {
                perror("mkdir /tmp/chat/clients");
                exit(EXIT_FAILURE);
        }
        
        int fd = inotify_init();
        if (fd == -1){
                perror("inotify_init");
                exit(EXIT_FAILURE);
        }
        printf("inotify initialized\n");
        
        int wd = inotify_add_watch(fd, "/tmp/chat/clients",
                           IN_CREATE | IN_DELETE | IN_MODIFY);
        if (wd == -1){
                perror("inotify_add_watch");
                exit(EXIT_FAILURE);
        }
        printf("watching /tmp/chat/clients\n");
        
	return 0;
}
