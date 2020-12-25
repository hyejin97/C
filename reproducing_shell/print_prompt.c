#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PATH	100
#define LF	10

int print_prompt(){
	char *buffer = (char *)malloc(MAX_PATH);
	char *value = (char *)getcwd(buffer, MAX_PATH);
	if (value != 0) fprintf(stdout, "jsh:%s>", buffer);
	free (buffer);

	return 0;
}

int main(){
	char c;
	while((c = getchar()) != EOF)
		if(c == LF)	print_prompt();
	return 0;
}
