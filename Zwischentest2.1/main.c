#include <stdio.h>

typedef enum  { passenger = 80, sleeper = 83, diner = 68} type;

typedef struct carriage{
    type type1;
    int capacity;
} carriage;

typedef struct train{
    carriage carriages[10];
    int length;
} train;

void printCarriage(carriage carriage1){
    printf("[%c:%03d]", carriage1.type1, carriage1.capacity);
}

void printTrain(train * train1){
    printf("\nTrain: ");
    if(train1->length == 0){
        printf("No Carriages!");
    } else {
        for (int i = 0; i < train1->length; ++i) {
            printf("%d:", i);
            printCarriage(train1->carriages[i]);
            if (i != (train1->length - 1)) {
                printf("-");
            }
        }
        printf(" Length: %d", train1->length);
    }
}

char getMenu(){
    char action;
    printf("\nChoose Action: print train (p), new carriage (n), print stats (s) or exit (x): ");
    scanf(" %c", &action);
    while ((action!='p') && (action!='n') && (action!='s') && (action!='x')){
        printf("\nInput invalid! Try again: ");
        scanf(" %c", &action);
    }
    return action;
}

carriage getCarriage(){
    carriage carriage1;
    char type;
    int capacity;
    printf("\nChoose type of carriage: passenger (p), sleeper (s) or diner (d): ");
    scanf(" %c", &type);
    while ((type!='p') && (type!='s') && (type!='d')){
        printf("\nInput invalid! Try again: ");
        scanf(" %c", &type);
    }
    switch (type) {
        case 'p': carriage1.type1 = 80;
            break;
        case 's': carriage1.type1 = 83;
            break;
        case 'd': carriage1.type1 = 68;
    }

    printf("\nChoose capacity (20 - 130): ");
    scanf(" %d", &capacity);
    while ((capacity>130) || (capacity<20)){
        printf("\nInput invalid! Try again: ");
        scanf(" %d", &capacity);
    }
    carriage1.capacity = capacity;

    return carriage1;
}

int getPosition(int length){
    int position;
    printf("\nChoose position for the new carriage (0-%d): ", length);
    scanf(" %d", &position);
    while (position<0 || position>length){
        printf("\nInput invalid! Try again: ");
        scanf(" %d", &position);
    }
    return position;
}

int insertCarriage(train *train1, int position,  carriage carriage1){
    if(train1->length >= 10){
        return -1;
    }
    if (position < 0 || position > train1->length) {
        return -2;
    }
    if ( position != 0 &&
            (((carriage1.type1 == 68) && (train1->carriages[position].type1 != 80)) ||
                   ((carriage1.type1 == 68) && (train1->carriages[position - 1].type1 != 80)) ||
                   ((carriage1.type1 == 83) && (train1->carriages[position].type1 == 68)) ||
                   ((carriage1.type1 == 83) && (train1->carriages[position - 1].type1 == 68)))) {
        return -3;
    }
    if (position == 0 && carriage1.type1 == 68){
        return -3;
    }
    (train1->length) = (train1->length) + 1;
    for (int i = (train1->length) - 2; i >= position; --i) {
        train1->carriages[i + 1] = train1->carriages[i];
    }
    train1->carriages[position] = carriage1;
    return 0;
}


int sumCapacity(train * train1, type type1){
    int sum=0;
    for (int i = 0; i < train1->length; ++i) {
        if(train1->carriages[i].type1 == type1){
            sum += train1->carriages[i].capacity;
        }
    }
    return sum;
}

void printTrainStats(train *train1){
    printTrain(train1);
    int pCapacity, sCapacity, dCapacity;
    pCapacity = sumCapacity(train1, 80);
    sCapacity = sumCapacity(train1, 83);
    dCapacity = sumCapacity(train1, 68);

    printf("\nCapacities:");
    printf("\n  Passenger: %d", pCapacity);
    printf("\n  Sleeper: %d", sCapacity);
    printf("\n  Diner: %d", dCapacity);
    if(dCapacity>60){
        printf(" - invalid");
    }
}

int main() {
    train train1;
    train1.length=0;
    carriage carriage1;
    char auswahl = getMenu();
    while (auswahl != 'x') {
        switch (auswahl) {
            case 'p':
                printTrain(&train1);
                break;
            case 'n':
                carriage1 = getCarriage();
                int position = getPosition(train1.length);
                int insertResult = insertCarriage(&train1, position, carriage1);
                switch (insertResult) {
                    case -1:
                        printf("\nError: Train too long!");
                        break;
                    case -2:
                        printf("\nError: Position not possible!");
                        break;
                    case -3:
                        printf("\nError: Diner only possible between two passenger carriages!");
                        break;
                }
                break;
            case 's':
                printTrainStats(&train1);
                break;
        }
        auswahl = getMenu();
    }
}

/* //Zum Ausprobieren Fkt print train:
    train train1;
    train1.length = 3;
    carriage carriage1 = {80, 333};
    carriage carriage2 = {83, 444};
    carriage carriage3 = {68, 222};
    train1.carriages[0] = carriage1;
    train1.carriages[1] = carriage2;
    train1.carriages[2] = carriage3;

    printTrain(&train1);
    return 0;
     */

/* //Zum Ausprobieren Fkt get menu:
printf("%c", getMenu());
*/
/* //Zum Ausprobieren Fkt get carriage:
carriage carriage1 = getCarriage();
printCarriage(carriage1);
 */

/* //Zum Ausprobieren Fkt get position:
train train1;
train1.length = 9;
printf("%d",getPosition(train1.length));
 */

/* //Zum Ausprobieren Fkt insert carriage:
train train1;
train1.length = 4;
carriage carriage1 = {83, 222};
carriage carriage2 = {80, 333};
carriage carriage3 = {68, 444};
carriage carriage4 = {80, 555};
train1.carriages[0] = carriage1;
train1.carriages[1] = carriage2;
train1.carriages[2] = carriage3;
train1.carriages[3] = carriage4;
carriage carriage5 = getCarriage();
int position = getPosition(train1.length);
printf("%d", insertCarriage(&train1,position,carriage5));
printTrain(&train1);
 */

/* //Zum Ausprobieren Fkt sum capacity:
train train1;
train1.length = 4;
carriage carriage1 = {83, 222};
carriage carriage2 = {80, 333};
carriage carriage3 = {68, 444};
carriage carriage4 = {80, 555};
train1.carriages[0] = carriage1;
train1.carriages[1] = carriage2;
train1.carriages[2] = carriage3;
train1.carriages[3] = carriage4;
printf("%d", sumCapacity(&train1,68));
 */

/* //Zum Ausprobieren Fkt print Train statistics:
train train1;
train1.length = 4;
carriage carriage1 = {83, 222};
carriage carriage2 = {80, 333};
carriage carriage3 = {68, 444};
carriage carriage4 = {80, 555};
train1.carriages[0] = carriage1;
train1.carriages[1] = carriage2;
train1.carriages[2] = carriage3;
train1.carriages[3] = carriage4;
printTrainStats(&train1);
 */
