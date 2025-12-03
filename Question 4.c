#include <stdio.h>
#include <string.h>

struct Book {
    int id;
    int pop;
    int lastUsed;   // smaller = older
};

int main() {
    int capacity, Q;
    scanf("%d %d", &capacity, &Q);

    struct Book shelf[1000];   // fixed-size array
    int count = 0;
    int timer = 0;

    while (Q--) {

        char op[10];
        scanf("%s", op);

        if (strcmp(op, "ADD") == 0) {

            int x, y;
            scanf("%d %d", &x, &y);

            // Check if book already exists
            int found = -1;
            for (int i = 0; i < count; i++) {
                if (shelf[i].id == x) {
                    found = i;
                    break;
                }
            }

            if (found != -1) {
                // Update existing book
                shelf[found].pop = y;
                shelf[found].lastUsed = timer++;
            }
            else {
                // Shelf full ? remove least recently used
                if (count == capacity) {
                    int LRU = 0;
                    for (int i = 1; i < count; i++) {
                        if (shelf[i].lastUsed < shelf[LRU].lastUsed) {
                            LRU = i;
                        }
                    }
                    // Remove LRU by shifting
                    for (int i = LRU; i < count - 1; i++) {
                        shelf[i] = shelf[i + 1];
                    }
                    count--;
                }

                // Add new book
                shelf[count].id = x;
                shelf[count].pop = y;
                shelf[count].lastUsed = timer++;
                count++;
            }
        }

        else if (strcmp(op, "ACCESS") == 0) {

            int x;
            scanf("%d", &x);

            int found = -1;
            for (int i = 0; i < count; i++) {
                if (shelf[i].id == x) {
                    found = i;
                    break;
                }
            }

            if (found == -1) {
                printf("-1\n");
            } else {
                printf("%d\n", shelf[found].pop);
                shelf[found].lastUsed = timer++;
            }
        }
    }

    return 0;
}
