#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int MIN = 100;
int MAX = 500;

// have to use the window class and not the name, as for some reason xdotool search returns the wrong id with name?
char* WINDOW_CLASS = "ffxiv_dx11.exe";
char* KEY = "code:60"; // period key

// this xdotool shit is so fucky.
// void sendKey(char *key, int *windowId) {
// 	char command[100];
// 	sprintf(command, "xdotool key --window %d %s", *windowId, key);
// 	printf("Sending key: %s\n", command);
// 	system(command);
// }
//
// int getWindowId() {
// 	char command[100];
// 	sprintf(command, "xdotool search --class \"%s\" | head -n 1", WINDOW_CLASS);
// 	FILE *fp = popen(command, "r");
// 	if (fp == NULL) {
// 		printf("Failed to run command\n" );
// 		exit(1);
// 	}
// 	char output[100];
// 	fgets(output, sizeof(output)-1, fp);
// 	pclose(fp);
// 	return atoi(output);
// }

int main(void) {
	time_t t;
	int windowId = getWindowId();
	printf("Window ID: %d\n", windowId);
	srand((unsigned) time(&t));
	while(1) {
		sendKey("space", &windowId);
		return 0;
		int random = MIN + rand() % (MAX-MIN); // generate random number between 100 & 500 seconds
		sleep(random);
	};
	return 0;
}
