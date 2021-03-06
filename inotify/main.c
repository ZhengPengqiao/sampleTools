
/*****************************************************************************
 *  Copyright          :  All Rights Reserved.
 *
 *  Date               :  2013-03-01 13:11:22
 *  Author/Corporation :  Dengzhaoqun
 *  Email              :  dengzhaoqun@163.com
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
 
#define EVENT_NUM 12
 
char *event_str[EVENT_NUM] = 
{
	(char *)"IN_ACCESS",
	(char *)"IN_MODIFY",
	(char *)"IN_ATTRIB",
	(char *)"IN_CLOSE_WRITE",
	(char *)"IN_CLOSE_NOWRITE",
	(char *)"IN_OPEN",
	(char *)"IN_MOVED_FROM",
	(char *)"IN_MOVED_TO",
	(char *)"IN_CREATE",
	(char *)"IN_DELETE",
	(char *)"IN_DELETE_SELF",
	(char *)"IN_MOVE_SELF"
};
 
int main(int argc, char *argv[])
{
	int fd;
	int wd;
	int len;
	int nread;
	char buf[BUFSIZ];
	struct inotify_event *event;
	int i;
	
	if(argc < 2)
	{
		fprintf(stderr, "%s path\n", argv[0]);
		return -1;
	}
	
	fd = inotify_init();
	if( fd < 0 )
	{
		fprintf(stderr, "inotify_init failed\n");
		return -1;
	}
	
	wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
	if(wd < 0)
	{
		fprintf(stderr, "inotify_add_watch %s failed\n", argv[1]);
		return -1;
	}
	
	buf[sizeof(buf) - 1] = 0;
	while( (len = read(fd, buf, sizeof(buf) - 1)) > 0 )
	{
		nread = 0;
		while( len > 0 )
		{
			event = (struct inotify_event *)&buf[nread];
			for(i=0; i<EVENT_NUM; i++)
			{
				if((event->mask >> i) & 1)
				{
					if(event->len > 0)
						fprintf(stdout, "%s --- %s\n", event->name, event_str[i]);
					else
						fprintf(stdout, "%s --- %s\n", " ", event_str[i]);
				}
			}
			nread = nread + sizeof(struct inotify_event) + event->len;
			len = len - sizeof(struct inotify_event) - event->len;
		}
	}
	
	return 0;
}