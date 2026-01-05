#ifndef INOTIFY_HANDLER_H
#define INOTIFY_HANDLER_H

#include <sys/inotify.h>

int init_inotify(const char *watch_dir);

/**
 * 파일의 마지막 메시지 읽기
 * filepath : 읽을 파일 경로
 * 마지막 메시지 문자열 return
 */
char* read_last_message(const char *filepath);

/**
 * username에서 .txt 확장자 제거
 * username : 수정할 문자
 */
void strip_txt_extension(char *username);

#endif
