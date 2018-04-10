#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

int get_file_length(char filename [])
{
    FILE * m_file;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int length = 0;
    m_file = fopen(filename, "r");
    if (m_file == NULL)
        printf("File not found...");

    while ((read = getline(&line, &len, m_file)) != -1) {
        length = length + 1;
    }

    fclose(m_file);
    if (line)
        free(line);
    return length;
}
