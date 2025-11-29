# FileChat
File-based Chat Server using **inotify** (Linux System Programming Study)

---

## inotify C API

### 1. inotify_init()
```C
#include <sys/inotify.h>

int inotify_init (void);
int inotify_init1 (int flags); // inotify with flags
```
The inotify_init() system call initializes inotify and returns a file descriptor representing the initialized instance.

The **flags** parameter is usually **0**, but may be a bitwise OR of the following flags:

- **IN_CLOEXEC**

Sets close-on-exec on the new file descriptor.

- **IN_NONBLOCK**

Sets O_NONBLOCK on the new file descriptor.

### 2. inotify_add_watch()
```C
#include <sys/inotify.h>

int inotify_add_watch (int fd,
                       const char *path,
                       uint32_t mask);
```

### 3. inotify_rm_watch()
```C
#include <inotify.h>

int inotify_rm_watch (int fd, uint32_t wd);
```
