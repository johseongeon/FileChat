# FileChat
File-based Chat Server using **inotify** (Linux System Programming Study)

---

## inotify C API

### 1. inotify_init()

The inotify_init() system call initializes inotify and returns a file descriptor representing the initialized instance.
```C
#include <sys/inotify.h>

int inotify_init (void);
int inotify_init1 (int flags); // inotify with flags
```

The **flags** parameter is usually **0**, but may be a bitwise OR of the following flags:

- **IN_CLOEXEC**

Sets close-on-exec on the new file descriptor.

- **IN_NONBLOCK**

Sets O_NONBLOCK on the new file descriptor.

**Usage**
```C
int fd;

fd = inotify_init1 (0); // inotify_init returns fd or -1(error)
if (fd == −1) {
        perror ("inotify_init1");
        exit (EXIT_FAILURE);
}
```

---

### 2. inotify_add_watch()

The system call **inotify_add_watch()** adds a watch for the event or events described by **mask** on the file or directory **path** to the inotify instance represented by **fd**
```C
#include <sys/inotify.h>

int inotify_add_watch (int fd,
                       const char *path,
                       uint32_t mask);
```

On success, the call returns a new watch descriptor. On failure, inotify_add_watch() returns −1 and sets errno

#### Watch mask
The watch mask is a binary OR of one or more inotify events, which <inotify.h> defines:

<img width="500" height="560" alt="image" src="https://github.com/user-attachments/assets/2d233c6e-5da1-4685-80b3-7999670314f1" />

**Usage**
```C
int wd;

wd = inotify_add_watch (fd, "/etc", IN_ACCESS | IN_MODIFY);
if (wd == −1) {
        perror ("inotify_add_watch");
        exit (EXIT_FAILURE);
}
```

---

### 3. inotify_rm_watch()

The system call inotify_rm_watch() removes a watch from an inotify instance

```C
#include <inotify.h>

int inotify_rm_watch (int fd, uint32_t wd);
```

**Usage**
```C
int ret;

ret = inotify_rm_watch (fd, wd); //returns 0 for success
if (ret)
        perror ("inotify_rm_watch");
```
