#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    char **lines;
    int count;
    int capacity;
} Buffer;

static void *xmalloc(size_t n) {
    void *p = malloc(n);
    if (!p) {
        fprintf(stderr, "Memory allocation failed (malloc %zu bytes)\n", n);
        exit(EXIT_FAILURE);
    }
    return p;
}

static void *xrealloc(void *ptr, size_t n) {
    void *p = realloc(ptr, n);
    if (!p) {
        fprintf(stderr, "Memory allocation failed (realloc %zu bytes)\n", n);
        exit(EXIT_FAILURE);
    }
    return p;
}

static char *xstrdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = xmalloc(n);
    memcpy(p, s, n);
    return p;
}

/* read a full line from FILE*, return malloc'd exact-sized string (no trailing newline) */
static char *readLineFile(FILE *in) {
    size_t cap = 128, len = 0;
    char *buf = xmalloc(cap);
    int c;
    while ((c = fgetc(in)) != EOF) {
        if (c == '\r') continue;
        if (c == '\n') break;
        if (len + 1 >= cap) {
            cap *= 2;
            buf = xrealloc(buf, cap);
        }
        buf[len++] = (char)c;
    }
    if (len == 0 && c == EOF) { free(buf); return NULL; }
    buf[len] = '\0';
    buf = xrealloc(buf, len + 1);
    return buf;
}

/* ensures pointer-array has at least minCapacity */
static void ensureCapacity(Buffer *b, int minCapacity) {
    if (b->capacity >= minCapacity) return;
    int newCap = (b->capacity > 0) ? b->capacity : 1;
    while (newCap < minCapacity) newCap *= 2;
    b->lines = (char**) xrealloc(b->lines, sizeof(char*) * newCap);
    b->capacity = newCap;
}

void insertLine(Buffer *b, int index, const char *text) {
    if (index < 0 || index > b->count) {
        fprintf(stderr, "insertLine: index %d out of bounds (0..%d)\n", index, b->count);
        return;
    }
    ensureCapacity(b, b->count + 1);
    if (index < b->count) {
        memmove(&b->lines[index + 1], &b->lines[index], sizeof(char*) * (b->count - index));
    }
    b->lines[index] = xstrdup(text);
    b->count++;
}

void deleteLine(Buffer *b, int index) {
    if (index < 0 || index >= b->count) {
        fprintf(stderr, "deleteLine: index %d out of bounds (0..%d)\n", index, b->count - 1);
        return;
    }
    free(b->lines[index]);
    if (index < b->count - 1) {
        memmove(&b->lines[index], &b->lines[index + 1], sizeof(char*) * (b->count - index - 1));
    }
    b->count--;
}

void printAllLines(Buffer *b) {
    for (int i = 0; i < b->count; ++i) {
        printf("%d: %s\n", i, b->lines[i]);
    }
}

void shrinkToFit(Buffer *b) {
    if (b->count == 0) {
        free(b->lines);
        b->lines = (char**) xmalloc(sizeof(char*) * 1);
        b->capacity = 1;
        return;
    }
    if (b->capacity == b->count) return;
    b->lines = (char**) xrealloc(b->lines, sizeof(char*) * b->count);
    b->capacity = b->count;
}

int saveToFile(Buffer *b, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "saveToFile: cannot open '%s' for writing: %s\n", filename, strerror(errno));
        return -1;
    }
    for (int i = 0; i < b->count; ++i) {
        if (fprintf(f, "%s\n", b->lines[i]) < 0) { fclose(f); return -1; }
    }
    fclose(f);
    return 0;
}

int loadFromFile(Buffer *b, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "loadFromFile: cannot open '%s' for reading: %s\n", filename, strerror(errno));
        return -1;
    }
    /* free existing lines */
    for (int i = 0; i < b->count; ++i) free(b->lines[i]);
    b->count = 0;

    char *line;
    while ((line = readLineFile(f)) != NULL) {
        ensureCapacity(b, b->count + 1);
        b->lines[b->count++] = line; /* line already malloc'd exact size */
    }
    fclose(f);
    return 0;
}

void freeAll(Buffer *b) {
    for (int i = 0; i < b->count; ++i) free(b->lines[i]);
    free(b->lines);
    b->lines = NULL;
    b->count = 0;
    b->capacity = 0;
}

/* Main: robust command parsing using fgets to avoid scanf mix issues.
   Commands (one per line):
     INSERT <index> <text...>
     DELETE <index>
     PRINT
     SHRINK
     SAVE <filename>
     LOAD <filename>
     CLEAR
     EXIT
*/
int main(void) {
    Buffer buf;
    buf.count = 0;
    buf.capacity = 4;
    buf.lines = (char**) xmalloc(sizeof(char*) * buf.capacity);

    char input[8192];
    while (fgets(input, sizeof(input), stdin) != NULL) {
        /* strip trailing newline */
        size_t L = strlen(input);
        if (L > 0 && input[L-1] == '\n') input[L-1] = '\0';

        /* tokenize command */
        char *cmd = strtok(input, " ");
        if (!cmd) continue;

        if (strcmp(cmd, "INSERT") == 0) {
            char *idxs = strtok(NULL, " ");
            if (!idxs) { fprintf(stderr, "INSERT requires index and text\n"); continue; }
            int idx = atoi(idxs);
            char *text = strtok(NULL, ""); /* rest of line, may be NULL => empty string */
            if (!text) text = "";
            insertLine(&buf, idx, text);
        }
        else if (strcmp(cmd, "DELETE") == 0) {
            char *idxs = strtok(NULL, " ");
            if (!idxs) { fprintf(stderr, "DELETE requires index\n"); continue; }
            int idx = atoi(idxs);
            deleteLine(&buf, idx);
        }
        else if (strcmp(cmd, "PRINT") == 0) {
            printAllLines(&buf);
        }
        else if (strcmp(cmd, "SHRINK") == 0) {
            shrinkToFit(&buf);
        }
        else if (strcmp(cmd, "SAVE") == 0) {
            char *fname = strtok(NULL, " ");
            if (!fname) { fprintf(stderr, "SAVE requires filename\n"); continue; }
            if (saveToFile(&buf, fname) == 0) printf("Saved %d lines to %s\n", buf.count, fname);
        }
        else if (strcmp(cmd, "LOAD") == 0) {
            char *fname = strtok(NULL, " ");
            if (!fname) { fprintf(stderr, "LOAD requires filename\n"); continue; }
            if (loadFromFile(&buf, fname) == 0) printf("Loaded %d lines from %s\n", buf.count, fname);
        }
        else if (strcmp(cmd, "CLEAR") == 0) {
            for (int i = 0; i < buf.count; ++i) free(buf.lines[i]);
            buf.count = 0;
            printf("Buffer cleared\n");
        }
        else if (strcmp(cmd, "EXIT") == 0) {
            break;
        }
        else {
            fprintf(stderr, "Unknown command: %s\n", cmd);
        }
    }

    freeAll(&buf);
    return 0;
}
