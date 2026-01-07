CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -pthread
SRCDIR = src
OBJDIR = obj
INCDIR = include

# 소스 파일들
LIB_SRCS = $(SRCDIR)/inotify_handler.c \
           $(SRCDIR)/socket_handler.c \
           $(SRCDIR)/message.c

# 오브젝트 파일들
LIB_OBJS = $(OBJDIR)/inotify_handler.o \
           $(OBJDIR)/socket_handler.o \
           $(OBJDIR)/message.o

# 기본 타겟
all: server client

# 서버 빌드
server: server.c $(LIB_OBJS)
	@echo "Building server..."
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Server build complete!"

# 클라이언트 빌드
client: client.c $(LIB_OBJS)
	@echo "Building client..."
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Client build complete!"

# 오브젝트 파일 생성 규칙
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

# 정리
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJDIR) server client
	rm -rf /tmp/chat
	@echo "Clean complete!"

# 디버그 빌드 (gdb용)
debug: CFLAGS += -g -DDEBUG
debug: clean all

# 재빌드
rebuild: clean all

# 도움말
help:
	@echo "Available targets:"
	@echo "  make          - Build server and client"
	@echo "  make server   - Build server only"
	@echo "  make client   - Build client only"
	@echo "  make clean    - Remove build files and chat directory"
	@echo "  make debug    - Build with debug symbols"
	@echo "  make rebuild  - Clean and rebuild"

.PHONY: all clean debug rebuild help
