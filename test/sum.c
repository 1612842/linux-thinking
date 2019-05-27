#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    ssize_t read;
    char *line = NULL;
    size_t len = 0;
    double sum = 0.0;

    while (read = getline(&line, &len, stdin) != -1) {
        sum += atof(line);
    }

    printf("Sum of number in file is: %.0f (unit)\n", sum);
    return 0;
}
