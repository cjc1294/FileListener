#include <sys/inotify.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_LENGTH	(1024 * (sizeof(struct inotify_event) + 16))

char * stringEvent(struct inotify_event *event){
	char *stringEvent = malloc(strlen(event->name) + 19);
	if (event->mask & IN_ISDIR){
		strcat(stringEvent, "Directory ");
		strcat(stringEvent, event->name);
		if (event->mask & IN_CREATE){
			strcat(stringEvent, " created");
		}else if (event->mask & IN_MODIFY){
			strcat(stringEvent, " modified");
		}else if (event->mask & IN_DELETE){
			strcat(stringEvent, " deleted");
		}
	}else{	
		strcat(stringEvent, "File ");
		strcat(stringEvent, event->name);
		if (event->mask & IN_CREATE){
			strcat(stringEvent, " created");
		}else if (event->mask & IN_MODIFY){
			strcat(stringEvent, " modified");
		}else if (event->mask & IN_DELETE){
			strcat(stringEvent, " deleted");
		}
	}
	return stringEvent;
}

int main(int argc, const char* argv[]){
	if(argc == 1){
		printf("Usage: listener \"target directory\"\n");
		return 0;
	}

	int listener;
	int length;
	
	while(1==1){
		char buffer[BUFFER_LENGTH];
		listener = inotify_init();
		inotify_add_watch(listener, argv[1], IN_MODIFY | IN_CREATE | IN_DELETE);

		length = read(listener, buffer, BUFFER_LENGTH);	
		int i = 0;
		while(i < length){
			struct inotify_event *event = (struct inotify_event * )&buffer[0];

			char * textEvent = stringEvent(event);
			printf("%s\n", textEvent);
			free(textEvent);

			i += sizeof(struct inotify_event) + event->len;
		}
	}

	return 0;
}

