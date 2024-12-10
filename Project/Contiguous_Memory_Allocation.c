#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef long long int lli;

lli is_valid_number (char* s) {
    lli result = 0;
    int i = 0;
    while (s[i] != '\0') {
        if (!('0' <= s[i] && s[i] <= '9')) return -1;
        result = result * 10 + s[i] - '0';

        i++;
    }
    return result;
}

const int MAX = 500;

int main (int argc, char* argv[]) {
    lli SIZE_OF_MEMORY;

    int nBlock;
    char nameBlocks[MAX][20];
    int sizeBlocks[MAX];

    for (int i = 0; i < MAX; i++) strcpy(nameBlocks[i], "Unused");

    if (argc != 2 || (SIZE_OF_MEMORY = is_valid_number(argv[1])) == -1) {
        printf("Please following the format: ./allocation <size of memory>\n\n");
        return 1;
    }

    nBlock = 1;
    strcpy(nameBlocks[0], "Unused");
    sizeBlocks[0] = SIZE_OF_MEMORY;

    while (true) {
        char str[200];
        // Read line and get tokens
        printf("allocator>");
        scanf("%[^\n]", str);
        getchar();

        // tách lệnh của người dùng thành các token, tách dựa trên dấu cách
        char *token = strtok(str, " ");
        char *listToken[6];
        int numberToken = 0;

        while (token != NULL) {
            listToken[numberToken++] = token;
            token = strtok(NULL, " ");
        }

        if (numberToken == 0) continue; // read a empty line

        if (strcmp(listToken[0], "RQ") == 0) { // Request command: RQ name size type
            int position = -1;
            lli ma = 0, mi = SIZE_OF_MEMORY + 1;

            lli sizeRequest = is_valid_number(listToken[2]);
            for (int i = 0; i < nBlock; i++) if (strcmp(nameBlocks[i], "Unused") == 0 && sizeBlocks[i] >= sizeRequest) {
                if (strcmp(listToken[3], "F") == 0) {
                    position = i;
                    break;
                }
                else if (strcmp(listToken[3], "W") == 0 && ma < sizeBlocks[i]) {
                    ma = sizeBlocks[i];
                    position = i;
                }
                else if (strcmp(listToken[3], "B") == 0 && mi > sizeBlocks[i]) {
                    mi = sizeBlocks[i];
                    position = i;
		}
                else {
                    position = -2;
                    break;
                }
            }

            if (position == -2 || sizeRequest == -1)
                printf("Wrong format: RQ <name> <size request> <type: F, W or B>\n");
            else if (position == -1)
                printf("Request failed, could not be allocation\n");
            else {
                if (sizeBlocks[position] == sizeRequest) strcpy(nameBlocks[position], listToken[1]);
                else {
                    for (int i = nBlock - 1; i > position;  i--) {
                        strcpy(nameBlocks[i + 1], nameBlocks[i]);
                        sizeBlocks[i + 1] = sizeBlocks[i];
                    };
                    nBlock++;

                    strcpy(nameBlocks[position + 1], "Unused");
                    sizeBlocks[position + 1] = sizeBlocks[position] - sizeRequest;

                    strcpy(nameBlocks[position], listToken[1]);
                    sizeBlocks[position] = sizeRequest;
                }
            }
        }
        else if (strcmp(listToken[0], "RL") == 0) { // Release command: RL name
            /*
            Three case need to merge:
            Unused Unused Unused
            Unused Unused Used
            Used   Unused Unused
            */
            int position = -1;
            for (int i = 0; i < nBlock; i++) if (strcmp(nameBlocks[i], listToken[1]) == 0) {
                position = i;
                break;
            }

            if (position == -1) {
                printf("Not found a process with name = \"%s\"\n", listToken[1]);
                continue;
            }

            strcpy(nameBlocks[position], "Unused");

            bool unusedLeft  = (position - 1 >= 0) && (strcmp(nameBlocks[position - 1], "Unused") == 0);
            bool unusedRight = (position + 1 < nBlock) && (strcmp(nameBlocks[position + 1], "Unused") == 0);

            if (unusedLeft && unusedRight) {
                sizeBlocks[position - 1] += sizeBlocks[position] + sizeBlocks[position + 1];
                nBlock -= 2;
                for (int i = position; i < nBlock; i++) {
                    strcpy(nameBlocks[i], nameBlocks[i + 2]);
                    sizeBlocks[i] = sizeBlocks[i + 2];
                }
            }
            else if (unusedLeft) {
                sizeBlocks[position - 1] += sizeBlocks[position];
                nBlock -= 1;
                for (int i = position; i < nBlock; i++) {
                    strcpy(nameBlocks[i], nameBlocks[i + 1]);
                    sizeBlocks[i] = sizeBlocks[i + 1];
                }
            }
            else if (unusedRight) {
                sizeBlocks[position] += sizeBlocks[position + 1];
                nBlock -= 1;
                for (int i = position + 1; i < nBlock; i++) {
                    strcpy(nameBlocks[i], nameBlocks[i + 1]);
		    sizeBlocks[i] = sizeBlocks[i + 1];
                }
            }
        }
        else if (strcmp(listToken[0], "C") == 0){ // Compact command: C
            lli sumEmpty = 0;
            int position = 0;

            for (int i = 0; i < nBlock; i++) {
                if (strcmp(nameBlocks[i], "Unused")) {
                    strcpy(nameBlocks[position], nameBlocks[i]);
                    sizeBlocks[position++] = sizeBlocks[i];
                }
                else sumEmpty += sizeBlocks[i];
            }
            if (sumEmpty > 0) {
                strcpy(nameBlocks[position], "Unused");
                sizeBlocks[position++] = sumEmpty;

                nBlock = position;
            }
        }
        else if (strcmp(listToken[0], "STAT") == 0) {  // Status command: STAT
            lli cur = 0;  
            for (int i = 0; i < nBlock; i++) {
                printf("Addresses [%lld:%lld] ", cur, cur + sizeBlocks[i] - 1);
                if (strcmp(nameBlocks[i], "Unused") == 0)  
                    printf("Unused\n");
                else
                    printf("Process %s\n", nameBlocks[i]);  

                cur += sizeBlocks[i];  
            }
        }

        else if (strcmp(listToken[0], "X") == 0) { // Exit command: X
            printf("Bye\n");
            exit(0);
        }
        else {
            printf("Command %s is not supported, please only use commands such as: RQ (request), RL (release), C (compact), STAT (status report), and X (exit).\n", listToken[0]);
        }
    }
}