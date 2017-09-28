#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/fcntl.h>
 
 
/* Set buffer size at least the default maxim size of the queue
  * found in/proc/sys/kernel/msgmax */
#define BUF_SIZE 	(1<<13)
#define TEXT		"test message"
#define NAME		"/test_queue"
 
char buf[BUF_SIZE];
 
int main(int argc, char **argv)
{
	unsigned int prio = 10;
	int rc;
	mqd_t m;
 
	m = mq_open(NAME, (argc>1 ? O_CREAT : 0) | O_RDWR, 0666, NULL);
	// DIE(m == (mqd_t)-1, "mq_open");
 
	if (argc > 1) {
		/* server sending message */
 
		rc = mq_send(m, TEXT, strlen(TEXT), prio);
		// DIE(rc == -1, "mq_send");
 
		rc = mq_close(m);
		// DIE(rc == -1, "mq_close");
 
	} else {
		/* client receiving message */
 
		rc = mq_receive(m, buf, BUF_SIZE, &prio);
		// DIE(rc == -1, "mq_receive");
 
		printf("received: %s\n", buf);
 
		rc = mq_close(m);
		// DIE(rc == -1, "mq_close");
 
		rc = mq_unlink(NAME);
		// DIE(rc == -1, "mq_unlink");
	}
 
	return 0;
}