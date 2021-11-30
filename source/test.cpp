#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#define FRAMES_MEMORY_WIDTH 30
#define REF_STRING_MAX_LEN  30
#define MAX_PHYSICAL_FRAMES 8

void startMenu(int physicalFramesNum);

static void simulateFIFO(int physicalFramesNum);
static void simulateOPT(int  physicalFramesNum);
static void simulateLRU(int  physicalFramesNum);
static void simulateLFU(int  physicalFramesNum);
static void printFrames(int  physicalFramesNum);
static void DisplayCurrentReferenceString();
static void GenerateReferenceString();
static void printReferenceString();
static void initVictimsAndFaults();
static void ReadReferenceString();
static void initPhysicalFrames();
static void initRefString();
static void printVictims();
static void printFaults();

static int faults[FRAMES_MEMORY_WIDTH];
static int victims[FRAMES_MEMORY_WIDTH];
static int reference_string[REF_STRING_MAX_LEN];
static int physicalFrames[MAX_PHYSICAL_FRAMES * FRAMES_MEMORY_WIDTH];


void startMenu(int physicalFramesNum){
    char option = -1;
    char nop;

    initRefString();
    srand((int)time(0));

    while (option != '0'){
        printf(" Choose option:\n\n");
        printf(" 1 - Read reference string\n");
        printf(" 2 - Generate reference string\n");
        printf(" 3 - Display current reference string\n");
        printf(" 4 - Simulate FIFO\n");
        printf(" 5 - Simulate OPT\n");
        printf(" 6 - Simulate LRU\n");
        printf(" 7 - Simulate LFU\n\n");
        printf(" 0 - Exit simulation\n\n");
        
        nop = scanf("%c", &option);
        fseek(stdin, 0, SEEK_END);

        while ((option < '0') | (option > '7')){
            printf("'%c' is invalid option, try again!\n\n", option);
            nop = scanf("%c", &option);
            fseek(stdin, 0, SEEK_END);
        }
        system("cls");

        switch (option){
            case '1': ReadReferenceString(); break;
            case '2': GenerateReferenceString(); break;
            case '3': DisplayCurrentReferenceString(); break;
            case '4': simulateFIFO(physicalFramesNum); break;
            case '5': simulateOPT(physicalFramesNum); break;
            case '6': simulateLRU(physicalFramesNum); break;
            case '7': simulateLFU(physicalFramesNum); break;
            case '0': printf("Exiting program!\n\n");
        }
    }
}

static void ReadReferenceString(){
    char page = 0;
    int  i    = 0;
    char nop;

    initRefString();
    printf("Enter reference string page by page('s' or 'S' to save string):\n\n");

    while ((page != 's') & (page != 'S')){
        printf("\nPage number (0-9): ");
        page = getchar();
        fseek(stdin, 0, SEEK_END);

        if (((page < '0') | (page > '9')) & ((page != 's') & (page != 'S'))){
            printf(" %c  - REJECTED!", page);
        } else if ((page != 's') & (page != 'S')){
            reference_string[i] = (int)page - 48;
            i++;
            printf(" %c - ACCEPTED!", page);
        }

        if (i == REF_STRING_MAX_LEN){
            printf("\n\nReference string max length reached!\n");
            break;
        }
    }

    if (i > 0){
        printf("\nReference string saved!\n\n");
    }else{
        printf("\nReference string not saved!\n");
    }

    printf(" Press any key to continue ...");
    nop = getchar();
    fseek(stdin, 0, SEEK_END);
    system("cls");
}


static void GenerateReferenceString(){
    int length =-1;
    int i      = 0;
    char nop;

    initRefString();
    printf("Enter length for random reference string (30 or less):\n");

    while ((length < 0) | (length > REF_STRING_MAX_LEN)){
        nop = scanf("%d", &length);

        if ((length < 0) | (length > REF_STRING_MAX_LEN)){
            printf(" Not valid lenght. Try again!\n");
        }
        fseek(stdin, 0, SEEK_END);
    }

    if (length == 0){
        printf("\nReference string not saved!\n\n");
    }else{
        for (i = 0; i < length; i++){
            reference_string[i] = rand() % 10;
        }

        reference_string[length] = -1;
        printf("\nReference string saved!\n\n");
    }

    printf(" Press any key to continue ...");
    nop = getchar();
    fseek(stdin, 0, SEEK_END);
    system("cls");
}

static void DisplayCurrentReferenceString(){
    int i = 0;
    char nop;

    printf("###################### REFERENCE STRING: ########################\n\n");

    if (reference_string[0] == -1){
        printf(" \t\tNo reference string available!\n\n");
    }else{
        printf(" [ ");
        while ((reference_string[i] != -1) & (i < FRAMES_MEMORY_WIDTH))
        {
            printf("%d ", reference_string[i]);
            i++;
        }
        printf("]\n\n");
    }
    printf("#################################################################\n\n");

    printf("\nPress any key to continue ...");
    nop = getchar();
    fseek(stdin, 0, SEEK_END);
    system("cls");
}

static void simulateFIFO(int physicalFramesNum){
    if (reference_string[0] != -1) {
        int tmpFrames[MAX_PHYSICAL_FRAMES];
        int FirstPosition = 0;
        int cntFaults     = 0;
        int i             = 0;

        initPhysicalFrames();
        initVictimsAndFaults();

        for (int j = 0; j < MAX_PHYSICAL_FRAMES; j++){
            tmpFrames[j] = -1;
        }

        system("cls");
        printf("\n\t##########  Simulating FIFO  ##########\n\n");

        printReferenceString();
        printFrames(physicalFramesNum);
        printVictims();
        printFaults();

        printf("\nPress ENTER to start!");
        char nop = getchar();
        fseek(stdin, 0, SEEK_END);

        while (reference_string[i] != -1 & (i < FRAMES_MEMORY_WIDTH)){
            int foundEmpty  = 0;
            int victimFrame =-1;
            int pagePresent = 0;

            for (int j = 0; j < physicalFramesNum; j++){
                if (tmpFrames[j] == reference_string[i]){
                    pagePresent = 1;
                    break;
                }
            }

            if (pagePresent == 0){
                for (int j = 0; j < physicalFramesNum; j++){
                    if ((tmpFrames[j] == -1) & (foundEmpty == 0)){
                        tmpFrames[j] = reference_string[i];
                        foundEmpty   = 1;
						cntFaults++;
					}
                }
            }

            if ((foundEmpty == 0) & (pagePresent == 0)){
                victimFrame = tmpFrames[FirstPosition];
                tmpFrames[FirstPosition] = reference_string[i];

                FirstPosition = (FirstPosition + 1) % physicalFramesNum;
            }

            if (victimFrame != -1){
                cntFaults++;
            }

            for (int j = 0; j < physicalFramesNum; j++){
                physicalFrames[j * FRAMES_MEMORY_WIDTH + i] = tmpFrames[j];
            }

            victims[i] = victimFrame;
            faults [i] = cntFaults;

            system("cls");
            printf("\n\t##########  Simulating FIFO  ##########\n\n");

            printReferenceString();
            printFrames(physicalFramesNum);
            printVictims();
            printFaults();

            i++;

            if ((reference_string[i] == -1) | (i == FRAMES_MEMORY_WIDTH)){
                break;
            }
            printf("\nPress ENTER for next step ...");
            nop = getchar();
            fseek(stdin, 0, SEEK_END);
        }
        printf("\nEnd of reference string!\n");
    }else{
        printf("\nNo reference string available!\n\n");
    }

    printf("\nPress any key to continue ...");
    char nop = getchar();
    fseek(stdin, 0, SEEK_END);
    system("cls");
}

static void simulateOPT(int physicalFramesNum){
    char nop;

    if (reference_string[0] != -1){
        int tmpFrames [MAX_PHYSICAL_FRAMES];
        int nextCallIn[MAX_PHYSICAL_FRAMES];
        int cntFaults = 0;
        int i         = 0;

        initPhysicalFrames();
        initVictimsAndFaults();

        for (int j = 0; j < MAX_PHYSICAL_FRAMES; j++){
            tmpFrames [j] = -1;
            nextCallIn[j] = -1;
        }

        system("cls");
        printf("\n\t##########  Simulating OPT  ##########\n\n");

        printReferenceString();
        printFrames(physicalFramesNum);
        printVictims();
        printFaults();

        printf("\nPress ENTER to start!");
        nop = getchar();
        fseek(stdin, 0, SEEK_END);
        system("cls");

        while (reference_string[i] != -1){
            int foundEmpty  = 0;
            int victimFrame = -1;
            int pagePresent = 0;
            int OPT         = 0;

            for (int j = 0; j < physicalFramesNum; j++){
                if (tmpFrames[j] == reference_string[i]){
                    pagePresent = 1;
                    break;
                }
            }

            if (pagePresent == 0){
                for (int j = 0; j < physicalFramesNum; j++){
                    if ((tmpFrames[j] == -1) & (foundEmpty == 0)){
                        tmpFrames[j] = reference_string[i];
                        foundEmpty   = 1;
                        cntFaults++;
                    }
                }
            }

            for (int j = 0; j < physicalFramesNum; j++){
                int nextIn = 0;
                int x = i;
                nextCallIn[j] = -2;

                while (reference_string[x] != -1){
                    nextIn++;
                    x++;
                    if (reference_string[x] == tmpFrames[j]){
                        nextCallIn[j] = nextIn;
                        break;
                    }
                }
            }

            for (int j = 1; j < physicalFramesNum; j++){
                if(nextCallIn[OPT] == -2) break;
                if ((nextCallIn[OPT] < nextCallIn[j]) & (nextCallIn[j] > -1)){
                    OPT = j;
                }
                if (nextCallIn[j] == -2){
                    OPT = j;
                    break;
                }
                printf("%d\n", OPT);
            }

            if ((foundEmpty == 0) & (pagePresent == 0)){
                victimFrame    = tmpFrames[OPT];
                tmpFrames[OPT] = reference_string[i];
            }

            if (victimFrame != -1){
                cntFaults++;
            }

            for (int j = 0; j < physicalFramesNum; j++){
                physicalFrames[j * FRAMES_MEMORY_WIDTH + i] = tmpFrames[j];
            }

            victims[i] = victimFrame;
            faults[i]  = cntFaults;

            system("cls");
            printf("\n\t##########  Simulating OPT  ##########\n\n");

            printReferenceString();
            printFrames(physicalFramesNum);
            printVictims();
            printFaults();

            printf("nextCallIn: ");
            for(int k=0;k<physicalFramesNum;k++) printf("%2d->%2d|", k, nextCallIn[k]);

            i++;

            if ((reference_string[i] == -1) | (i == FRAMES_MEMORY_WIDTH)){
                break;
            }
            printf("\nPress ENTER for next step ...");
            nop = getchar();
            fseek(stdin, 0, SEEK_END);
        }
        printf("\nEnd of reference string!\n");
    }
    else{
        printf("\nNo reference string available!\n\n");
    }

    printf("\nPress any key to continue ...");
    nop = getchar();
    fseek(stdin, 0, SEEK_END);
    system("cls");
}

static void simulateLRU(int const physicalFramesNum){
    char nop;

    if (reference_string[0] != -1){
        int tmpFrames[FRAMES_MEMORY_WIDTH];
        time_t Times[MAX_PHYSICAL_FRAMES];
        int cntFaults = 0;
        int i         = 0;

        initPhysicalFrames();
        initVictimsAndFaults();

        for (int j = 0; j < MAX_PHYSICAL_FRAMES; j++){
            tmpFrames[j] = -1;
            Times[j]     = (time_t)0;
        }

        system("cls");
        printf("\n\t##########  Simulating LRU  ##########\n\n");

        printReferenceString();
        printFrames(physicalFramesNum);
        printVictims();
        printFaults();

        printf("\nPress ENTER to start!");
        nop = getchar();
        fseek(stdin, 0, SEEK_END);
        system("cls");

        while (reference_string[i] != -1){
            int foundEmpty  = 0;
            int victimFrame = -1;
            int pagePresent = 0;
            int LRU         = 0;

            for (int j = 0; j < physicalFramesNum; j++){
                if (tmpFrames[j] == reference_string[i]){
                    pagePresent = 1;
                    time(&Times[j]);
                    break;
                }
            }

            if (pagePresent == 0){
                for (int j = 0; j < physicalFramesNum; j++){
                    if ((tmpFrames[j] == -1) & (foundEmpty == 0)){
                        tmpFrames[j] = reference_string[i];
                        foundEmpty   = 1;
                        time(&Times[j]);
                        cntFaults++;
                    }
                }
            }

            if ((foundEmpty == 0) & (pagePresent == 0)){
                for (int j = 1; j < physicalFramesNum; j++){
                    if (Times[LRU] > Times[j]){
                        LRU = j;
                    }
                }
                victimFrame    = tmpFrames[LRU];
                tmpFrames[LRU] = reference_string[i];
                time(&Times[LRU]);
            }
            LRU = 0;

            for (int j = 0; j < physicalFramesNum; j++){
                if (tmpFrames[j] != -1){
                    if (Times[LRU] > Times[j]){
                        LRU = j;
                    }
                }
            }

            if (victimFrame != -1){
                cntFaults++;
            }

            for (int j = 0; j < physicalFramesNum; j++){
                physicalFrames[j * FRAMES_MEMORY_WIDTH + i] = tmpFrames[j];
            }

            victims[i] = victimFrame;
            faults[i]  = cntFaults;

            system("cls");
            printf("\n\t##########  Simulating LRU  ##########\n\n");

            printReferenceString();
            printFrames(physicalFramesNum);
            printVictims();
            printFaults();
            for(int k=0; k<physicalFramesNum; k++) printf("%d ", Times[k]);
            i++;
            Sleep(100);

            if ((reference_string[i] == -1) | (i == FRAMES_MEMORY_WIDTH)){
                break;
            }

            printf("\nPress ENTER for next step ...");
            nop = getchar();
            fseek(stdin, 0, SEEK_END);
        }
        printf("\nEnd of reference string!\n");
    }else{
        printf("\nNo reference string available!\n\n");
    }

    printf("\nPress any key to continue ...");
    nop = getchar();
    fseek(stdin, 0, SEEK_END);
    system("cls");
}

static void simulateLFU(int physicalFramesNum){
    char nop;

    if (reference_string[0] != -1){
        int tmpFrames[MAX_PHYSICAL_FRAMES];
        int usageFreq[10] = {0};
        int cntFaults = 0;
        int i         = 0;

        initPhysicalFrames();
        initVictimsAndFaults();

        for (int s = 0; s < MAX_PHYSICAL_FRAMES; s++) tmpFrames[s] = -1;

        system("cls");
        printf("\n\t##########  Simulating LFU  ##########\n\n");

        printReferenceString();
        printFrames(physicalFramesNum);
        printVictims();
        printFaults();

        printf("\nPress ENTER to start!");
        nop = getchar();
        fseek(stdin, 0, SEEK_END);
        system("cls");

        while (reference_string[i] != -1){
            int foundEmpty  = 0;
            int victimFrame = -1;
            int pagePresent = 0;

            usageFreq[reference_string[i]]++;

            for (int j = 0; j < physicalFramesNum; j++){
                if (tmpFrames[j] == reference_string[i]){
                    pagePresent = 1;
                    break;
                }
            }

            if (pagePresent == 0){
                for (int j = 0; j < physicalFramesNum; j++){
                    if ((tmpFrames[j] == -1) & (foundEmpty == 0)){
                        tmpFrames[j] = reference_string[i];
                        foundEmpty   = 1;
                        cntFaults++;
                    }
                }
            }

            if ((foundEmpty == 0) & (pagePresent == 0)){
                int LFU = tmpFrames[0];
                int position = 0;

                for (int j = 0; j < physicalFramesNum; j++){
                    if (usageFreq[tmpFrames[j]] < usageFreq[LFU]){
                        LFU = tmpFrames[j];
                        position = j;
                    }
                }

                printf("\nposition: %d\n", position);
                printf("\ntmpFrames[position]: %d\n", tmpFrames[position]);
                printf("victimFrame: %d\n", victimFrame);
                victimFrame         = tmpFrames[position];
                tmpFrames[position] = reference_string[i];
            }

            if (victimFrame != -1) cntFaults++;

            for (int j = 0; j < physicalFramesNum; j++){
                physicalFrames[j * FRAMES_MEMORY_WIDTH + i] = tmpFrames[j];
            }
            victims[i] = victimFrame;
            faults[i]  = cntFaults;
            
            system("cls");
            printf("\n\t##########  Simulating LFU  ##########\n\n");

            printReferenceString();
            printFrames(physicalFramesNum);
            printVictims();
            printFaults();


            printf("foundEmpty: %d, pagePresent: %d\n",foundEmpty, pagePresent);
            for(int k=0;k<10;k++) printf("%2d->%2d|", k, usageFreq[k]);

            printf("\n");

            i++;

            if ((reference_string[i] == -1) | (i == FRAMES_MEMORY_WIDTH)){
                break;
            }

            printf("\nPress ENTER for next step ...");
            nop = getchar();
            fseek(stdin, 0, SEEK_END);
        }
        printf("\nEnd of reference string!\n");
    }
    else{
        printf("\nNo reference string available!\n\n");
    }

    printf("\nPress any key to continue ...");
    nop = getchar();
    fseek(stdin, 0, SEEK_END);
    system("cls");
}

static void initPhysicalFrames(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < FRAMES_MEMORY_WIDTH; j++){
            physicalFrames[i * FRAMES_MEMORY_WIDTH + j] = -1;
        }
    }
}

static void printReferenceString(){
    int i = 0;

    printf(" ----------------------------------------------------------------------------------------------------------");
    printf("\n|Reference_string|");

    for (int i = 0; i < FRAMES_MEMORY_WIDTH; i++){
        if (reference_string[i] != -1){
            printf("%2d|", reference_string[i]);
        }else{
            printf("  |");
        }
    }
    printf("\n");
}

static void printFrames(int physicalFramesNum){
    int i = 0;
    int j = 0;

    printf(" ----------------------------------------------------------------------------------------------------------");

    for (i = 0; i < physicalFramesNum; i++){
        printf("\n|Physical_Frame_%d|", i);

        for (j = 0; j < FRAMES_MEMORY_WIDTH; j++){
            if (physicalFrames[i * FRAMES_MEMORY_WIDTH + j] != -1){
                printf("%2d|", physicalFrames[i * FRAMES_MEMORY_WIDTH + j]);
            }else{
                printf("  |");
            }
        }
    }
    printf("\n ----------------------------------------------------------------------------------------------------------\n");
}

static void initRefString(){
    for (int i = 0; i < FRAMES_MEMORY_WIDTH; i++){
        reference_string[i] = -1;
    }
}

static void initVictimsAndFaults(){
    for (int i = 0; i < FRAMES_MEMORY_WIDTH; i++){
        victims[i] = -1;
        faults[i]  = -1;
    }
}

static void printFaults(){
    printf("|    Faults      |");
    for (int i = 0; i < FRAMES_MEMORY_WIDTH; i++){
        if (faults[i] != -1){
            printf("%2d|", faults[i]);
        }else{
            printf("  |");
        }
    }
    printf("\n ----------------------------------------------------------------------------------------------------------\n");
}

static void printVictims(){
    printf("| Victim_frames  |");
    for (int i = 0; i < FRAMES_MEMORY_WIDTH; i++){
        if (victims[i] != -1){
            printf("%2d|", victims[i]);
        }else{
            printf("  |");
        }
    }
    printf("\n ----------------------------------------------------------------------------------------------------------\n");
}

int main(int argv, char* argc[]){
    int physicalFramesNum;

    if (argv < 2){
        printf("Please check command line arguments! Please input physical Frames num:\n");
        scanf("%d", &physicalFramesNum);
    }else{
    	physicalFramesNum = atoi(argc[1]);
	}
	
    if (physicalFramesNum > 8) physicalFramesNum = 8;
    if (physicalFramesNum < 1) physicalFramesNum = 1;
    startMenu(physicalFramesNum);
    return 0;
}
