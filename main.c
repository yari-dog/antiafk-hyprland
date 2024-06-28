#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int MIN = 100;
int MAX = 500;

int PIPE_MSG_SIZE = 100;

// have to use the window class and not the name, as for some reason xdotool
// search returns the wrong id with name?
char *WINDOW_CLASS = "ffxiv_dx11.exe";
char *KEY = "Control,w"; // period key
char *UNDO = "Control,w";

// run a command and return the output as a string
char *command(char *command, int size) {
  FILE *fp = popen(command, "r");
  char *returnPtr = malloc(size);
  fgets(returnPtr, size, fp);
  pclose(fp);
  return returnPtr;
}


// jq likes to return strings with quotes, this function removes them
void stripChars(char *s, char c) {
  int writer = 0;
  int reader = 0;
  while (s[reader]) {
    if (s[reader] != c) {
      s[writer++] = s[reader];
    }
    reader++;
  }
  s[writer] = 0;
}

// get a property of the active window through hyprland
char *getActiveWindowProp(char *buf, int size, char *prop) {
  char intermediary[100];
  sprintf(intermediary, "hyprctl activewindow -j | jq .%s", prop);
  char *returnPtr = command(intermediary, size);
  stripChars(returnPtr, '"');
  returnPtr[strcspn(returnPtr, "\n\r")] = 0;
  return returnPtr;
}

int isXIVWindowActive() {
  char *class = getActiveWindowProp(class, 1024, "class");
  // char *class = command("hyprctl activewindow -j | jq .class", 1024);
  // stripChars(class, '"');            // remove quotes
  // class[strcspn(class, "\n\r")] = 0; // remove newline

  int isActive;
  if (class != 0) {
    if (strcmp(class, WINDOW_CLASS) == 0) {
      isActive = 1;
    } else {
      isActive = 0;
    }
  } else {
    printf("something went fucky class is 0");
    exit(1);
  }

  free(class);
  return isActive;
}

void sendKey(char *key) {
  char intermediary[100];
  sprintf(intermediary, "hyprctl dispatch sendshortcut %s,\"class:^(%s)$\"",
          key, WINDOW_CLASS);
  char *returnPtr = command(intermediary, 1);
  free(returnPtr);
  return;
}

// double tap is needed because the first run just activates the window, the
// second run is what actually works for some reasons
void doubleTap() {
  sendKey(KEY);
  sendKey(UNDO); // have a backspace key just incase i'm typing
  return;
}

long currentWindowPID() {
  char *stringPID = getActiveWindowProp(stringPID, 1024, "pid");
  int PID = atoi(stringPID);
  free(stringPID);
  return PID;
}

void deAFK() {
  if (isXIVWindowActive() == 0) {
    long PID = currentWindowPID();
    doubleTap();
    char intermediary[100];
    sprintf(intermediary, "hyprctl dispatch focuswindow pid:%li", PID);
    free(command(intermediary, 0));
  }
  return;
}

int main(void) {
  time_t t;
  srand((unsigned)time(&t));
  while (1) {
    deAFK();
    int random =
        MIN +
        rand() %
            (MAX - MIN); // generate random number between 100 & 500 seconds
    sleep(random);
  };
  return 0;
}
