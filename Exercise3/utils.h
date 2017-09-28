#ifndef _UTILS_H_
#define _UTILS_H_

#define FNE "File does not exist!"
#define MESSAGE_SIZE 2048

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

struct params {
    int socket;
};
#define clear() printf("\033[H\033[J")
#define del_console_line() printf("\033[H\033[K");
#define move_console_cursor_up(lines) printf("\033[%dA", lines);

#endif