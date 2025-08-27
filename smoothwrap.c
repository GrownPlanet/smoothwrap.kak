#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char *data;
    int len;
} String_t;

typedef struct {
    int *data;
    int len;
    int capacity;
} IntVec_t;

IntVec_t IntVec_new(int capacity) {
    int *data = (int*)malloc(sizeof(int) * capacity);
    if (data == NULL) {
        fprintf(stderr, "malloc failed!");
        exit(1);
    }

    return (IntVec_t) {
        .data = data,
        .capacity = capacity,
        .len = 0,
    };
}

void IntVec_push(IntVec_t *vec, int element) {
    if (vec->len + 1 >= vec->capacity) {
        vec->capacity *= 2;
        vec->data = (int*)realloc(vec->data, sizeof(int) * vec->capacity);
        if (vec->data == NULL) {
            fprintf(stderr, "realloc failed!");
            exit(1);
        }
    }
    vec->data[vec->len] = element;
    vec->len += 1;
}

// check if a certain character can break for a newline
bool is_brakable_char(char ch) {
    switch (ch) {
        case ' ': return true;
        default: return false;
    }
}

// calculate the distance to the next space or end of word
int distance_to_boundry(String_t line, int x, int wrap_width) {
    if (is_brakable_char(line.data[x])) return 1;

    for (int i = x; i < line.len; i++) {
        if (is_brakable_char(line.data[i])) {
            int dist = i - x;
            return dist > wrap_width ? wrap_width : dist;
        }
    }

    int dist = line.len - x;
    return dist > wrap_width ? wrap_width : dist;
}

// calculate the positions where the line wraps
IntVec_t get_breakpoints(String_t line, int wrap_width) {
    IntVec_t breakpoints = IntVec_new(2);
    IntVec_push(&breakpoints, 0);

    int position = 0;
    int visual_column = 0;

    while (position < line.len) {
        int dist = distance_to_boundry(line, position, wrap_width);
        position += dist;
        visual_column += dist;
        if (visual_column > wrap_width) {
            IntVec_push(&breakpoints, position - dist);
            visual_column = dist;
        }
    }

    IntVec_push(&breakpoints, line.len);

    return breakpoints;
}

// turn the colmn into an x, y position
void column_to_position(IntVec_t breakpoints, int column, int *x, int *y) {
    for (int i = 0; i < breakpoints.len - 1; i++) {
        if (breakpoints.data[i] <= column && column < breakpoints.data[i + 1]) {
            *x = column - breakpoints.data[i] + 1;
            *y = i + 1;
            return;
        }
    }
}

int position_to_column(IntVec_t breakpoints, int x, int y) {
    int mapped_x = x;
    if (x >= breakpoints.data[y] - breakpoints.data[y - 1]) {
        mapped_x = breakpoints.data[y] - breakpoints.data[y - 1];
    }
    return breakpoints.data[y - 1] + mapped_x;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "expected 4 arguments, got %d");
        return 1;
    }

    char* motion = argv[1];
    int column = atoi(argv[2]) - 1;
    String_t line = {
        .data = argv[3],
        .len = strlen(argv[3])
    };
    int wrap_width = atoi(argv[4]);

    IntVec_t breakpoints = get_breakpoints(line, wrap_width);

    int x, y;
    column_to_position(breakpoints, column, &x, &y);

    if (strcmp(motion, "down") == 0) {
        printf("gh");
        int pos = position_to_column(breakpoints, x, y + 1);
        if (y + 1 != breakpoints.len) {
            printf("%dl", pos - 1);
        } else {
            printf(": set-option global go_to_next_line true\n");
            printf(": set-option global visual_column %d\n", x);
            fprintf(stderr, "set: %d\n", x);
            printf("j");
        }
    }

    else if (strcmp(motion, "up") == 0) {
        printf("gh");
        int pos = position_to_column(breakpoints, x, y - 1);
        if (y > 1) {
            if (pos > 1) printf("%dl", pos - 1);
        } else {
            printf(": set-option global go_to_previous_line true\n");
            printf(": set-option global visual_column %d\n", x);
            printf("k");
        }
    }

    else if (strcmp(motion, "next") == 0) {
        int pos = position_to_column(breakpoints, column + 1, 1);
        printf("gh");
        if (pos != 1) printf("%dl", pos - 1);
    }

    else if (strcmp(motion, "previous") == 0) {
        int pos = position_to_column(breakpoints, x, breakpoints.len - 1);
        printf("gh");
        fprintf(stderr, "%d\n", pos);
        if (pos != 0) printf("%dl", pos - 1);
    }
}
