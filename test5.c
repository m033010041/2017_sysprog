#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    for (int i = 0; i < 3; i++) {
        fork();
	printf("-");
    }
    wait(NULL); wait(NULL); wait(NULL);
    return 0;
}


