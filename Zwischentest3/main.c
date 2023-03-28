#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum node_type{
    number_type = 1, operator_type, parenthesis_type
};

struct node {
    char operator;
    int number;
    enum node_type type;
    struct node* next;
};

bool parFound, closedParFound, pointFound, dashFound;

struct node* inputToNode(const char *input);
struct node* addLast(struct node* head, struct node *newNode);
struct node *findLastParenthesisOpen(struct node *head);
struct node *findFirstPointOperator(struct node *startNode);
struct node *findFirstDashOperator(struct node *startNode);
struct node *findPrevious(struct node *head, struct node *node);

struct node* createParenthesisNode(char operator){
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    if(newNode == NULL){                       // Ueberpruefen ob malloc erfolgreich
        printf("Memory not available!\n");
        return NULL;
    } else {
        newNode->operator = operator;
        newNode->type = parenthesis_type;
    }
    return newNode;
};

struct node* createOperatorNode(char operator){
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    if(newNode == NULL){                       // Ueberpruefen ob malloc erfolgreich
        printf("Memory not available!\n");
        return NULL;
    } else {
        newNode->operator = operator;
        newNode->type = operator_type;
    }
    return newNode;
};

struct node* createNumberNode(int number){
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    if(newNode == NULL){                       // Ueberpruefen ob malloc erfolgreich
        printf("Memory not available!\n");
        return NULL;
    } else {
        newNode->number = number;
        newNode->type = number_type;
    }
    return newNode;
};

struct node *inputToNode(const char *input) {
    int number = atoi(input);
    if (number != 0) {
        return createNumberNode(number);
    }
    if (input[0] == '0') {
        return createNumberNode(0);
    }
    if (strcmp(input, "(") == 0 ||
        strcmp(input, ")") == 0) {
        return createParenthesisNode(input[0]);
    }

    if (strcmp(input, "+") == 0 ||
        strcmp(input, "-") == 0 ||
        strcmp(input, "*") == 0 ||
        strcmp(input, "/") == 0) {
        return createOperatorNode(input[0]);
    }
    return NULL;
}

void printList(struct node* head){
    struct node* printerBoy = head;
    while (printerBoy != NULL){
        if(printerBoy->type == parenthesis_type || printerBoy->type == operator_type) {
            printf("%c", printerBoy->operator);
        } else if (printerBoy->type == number_type){
            printf("%d", printerBoy->number);
        }
        printerBoy = printerBoy->next;
    }
}

struct node* addLast(struct node* head, struct node *newNode){

    if(head == NULL){                       // Wenn liste leer ist (head zeigt auf nichts), dann setze die neue Node als head (also Anfang der Liste)
        head = newNode;
    } else {                                // sonst, erstelle hilfspointer, lass ihn auf anfang der liste zeigen
        struct node* temp = head;
        while (temp->next != NULL) {        // hilfspointer so weit verschieben, bis er auf null zeigt (ABER SELBER NOCH NICHT NULL GEWORDEN!!)
            temp = temp->next;
        }
        temp->next = newNode;               // hilfspointer soll jetzt auf die neue Node zeigen, somit ist sie in letzter Stelle.
    }
    newNode->next = NULL;
    return head;                            // return head weil er am anfang ist und damit eh die ganze liste zurueckgibt (wegen pointer, alle zeigen auf dem naechsten)
}

struct node *findLastParenthesisOpen(struct node *head){
    parFound = 0;
    struct node* temp = head;
    struct node* parenthisisBro = head;
    while (temp->next != NULL) {
        if(temp->type == parenthesis_type && temp->operator == '('){
            parenthisisBro = temp;
            parFound = 1;
        }
        temp = temp->next;
    }
    return parenthisisBro;
}

struct node *findFirstParenthesisClosed(struct node *startNode){
    closedParFound = 0;
    struct node* temp = startNode;
    struct node* parDead = startNode;
    while (temp->next != NULL && temp->operator != ')') {
        temp = temp->next;
    } if(temp != NULL && temp->operator == ')'){
        parDead = temp;
        closedParFound = 1;
    }
    return parDead;
}

struct node *findFirstPointOperator(struct node *startNode){
    pointFound = 0;
    struct node* temp = startNode;
    struct node* pointBoy = startNode;
    while (temp->next != NULL && temp->operator != '*' && temp->operator != '/') {
        temp = temp->next;
    }
    if(temp != NULL && (temp->operator == '*' || temp->operator == '/')){           // weil temp in der while loop immer um eine position weiter verschoben wird. wenn keine + oder - vorkommen, dann wird temp zu NULL, weil es bis ende verschoben wird, in diesem Fall wollen wir pointBoy NICHT gleich temp setzen!!!!
        pointBoy = temp;
        pointFound = 1;
    }
    return pointBoy;
}

struct node *findFirstDashOperator(struct node *startNode){
    dashFound = 0;
    struct node* temp = startNode;
    struct node* dashSmash = startNode;
    while (temp->next != NULL && temp->operator != '+' && temp->operator != '-') {
        temp = temp->next;
    }
    if(temp != NULL && (temp->operator == '+' || temp->operator == '-')){
        dashSmash = temp;
        dashFound = 1;
    }
    return dashSmash;
}

struct node *findPrevious(struct node *head, struct node *node){
    struct node* previous = head;
    while (previous != NULL && previous->next != node){
        previous = previous->next;
    }
    return previous;
}

int calculate(int a, int b, char operator){
    switch (operator) {
        case '*': return (a)*(b);
        case '/':
            if(b==0) {
                b = 1;
            }
                return (a)/(b);
        case '+': return (a)+(b);
        case '-': return (a)-(b);
        default: return 0;
    }
}

struct node* createResultNode(struct node *head, struct node *operation){
    if(operation->type == operator_type){
        struct node* vorDemOperator = findPrevious(head,operation);
        struct node* nachDemOperator = operation->next;
        struct node* resultNode = createNumberNode(calculate(vorDemOperator->number,nachDemOperator->number,operation->operator));
        return resultNode;
    } else {
        return head;
    }
}

struct node* klammerLoesen(struct node *head){
    struct node *klammerAuf = findLastParenthesisOpen(head);
    if(parFound){
        struct node *klammerZu = findFirstParenthesisClosed(klammerAuf);
        if(closedParFound){
            struct node* zahlInKlammer = findLastParenthesisOpen(head)->next;
            if(zahlInKlammer->next == klammerZu){
                if(klammerAuf == head && klammerZu->next == NULL){
                    head = zahlInKlammer;
                    zahlInKlammer->next = NULL;
                    free(klammerAuf);
                    free(klammerZu);
                } else if (klammerAuf == head){
                    struct node* nachDerKlammer = klammerZu->next;
                    free(klammerAuf);
                    free(klammerZu);
                    head = zahlInKlammer;
                    zahlInKlammer->next = nachDerKlammer;
                } else if(klammerZu->next == NULL){
                    struct node* vorDerKlammer = findPrevious(head,klammerAuf);
                    vorDerKlammer->next = zahlInKlammer;
                    zahlInKlammer->next = NULL;
                    free(klammerAuf);
                    free(klammerZu);
                } else {
                    struct node* nachDerKlammer = klammerZu->next;
                    struct node* vorDerKlammer = findPrevious(head,klammerAuf);
                    vorDerKlammer->next = zahlInKlammer;
                    zahlInKlammer->next = nachDerKlammer;
                    free(klammerAuf);
                    free(klammerZu);
                }
            }
        }
    }
    return head;
}

struct node* replaceOperationWithResult(struct node *head, struct node *operation){
    struct node *resultNode = createResultNode(head,operation);
    struct node* vorDemOperator = findPrevious(head,operation);
    struct node* nachDemOperator = operation->next;
    if(nachDemOperator->next == NULL && vorDemOperator == head){
        head = resultNode;
        resultNode->next = NULL;
        free(nachDemOperator);
        free(operation);
        free(vorDemOperator);
    } else if(nachDemOperator->next == NULL){
        findPrevious(head,vorDemOperator)->next = resultNode;
        resultNode->next = NULL;
        free(nachDemOperator);
        free(operation);
        free(vorDemOperator);
    } else if (vorDemOperator == head){
        head = resultNode;
        resultNode->next = nachDemOperator->next;
        free(nachDemOperator);
        free(operation);
        free(vorDemOperator);
    } else {
        findPrevious(head,vorDemOperator)->next = resultNode;
        resultNode->next = nachDemOperator->next;
        free(nachDemOperator);
        free(operation);
        free(vorDemOperator);
    }
    printf("\nResulting term: ");
    printList(head);

    // klammerLoesen(head);

    return head;
}

int findPositionOfNode(struct node* head, struct node* node){
    struct node* temp = head;
    int positionOfNode = -1;
    for (int i = 0; temp != NULL; ++i) {
        if(temp == node){
            positionOfNode = i;
        }
        temp = temp->next;
    }
    return positionOfNode;
}

struct node *klammernVerarbeiten(struct node* head){

    struct node* lastParOpen = findLastParenthesisOpen(head);
    struct node* firstParClosed = findFirstParenthesisClosed(lastParOpen);
    struct node* firstPointOp = findFirstPointOperator(lastParOpen);
    struct node* firstDashOp = findFirstDashOperator(lastParOpen);

    if (parFound){
        int posParClosed = findPositionOfNode(head, firstParClosed);
        if (pointFound){
            int posFirstPointOp = findPositionOfNode(head, firstPointOp);
            if(posParClosed > posFirstPointOp && posFirstPointOp > 0){
                head = replaceOperationWithResult(head,findFirstPointOperator(lastParOpen));
            }
        }
        findFirstDashOperator(lastParOpen);
        if (dashFound) {
            int posFirstDashOp = findPositionOfNode(head, firstDashOp);
            if (posParClosed > posFirstDashOp && posFirstDashOp>0) {
                head = replaceOperationWithResult(head, findFirstDashOperator(lastParOpen));
            }
        }
    }
    head = klammerLoesen(head);
    return head;
}

int main() {
    char input[2];
    printf("\nInput: ");
    scanf("%s", input);

    struct node* head = NULL;

    while (input[0] != '='){
        struct node* newNode = inputToNode(input);
        head = addLast(head, newNode);
        printf("\nTerm: ");
        printList(head);

        printf("\nInput: ");
        scanf("%s", input);
    }

    findLastParenthesisOpen(head);
    while (parFound){
        head = klammernVerarbeiten(head);
        findLastParenthesisOpen(head);
    }
    findFirstPointOperator(head);
    while (pointFound){
        head = replaceOperationWithResult(head, findFirstPointOperator(head));
        findFirstPointOperator(head);
    }
    findFirstDashOperator(head);
    while (dashFound){
        head = replaceOperationWithResult(head, findFirstDashOperator(head));
        findFirstDashOperator(head);
    }

    free(head);
    return 0;
}


// F U N K T I O N E N     T E S T E N :

/*
 * && findPrevious(head,findFirstPointOperator(findLastParenthesisOpen(head))) !=
                                 findFirstParenthesisClosed(head)
 *
 * && findPrevious(head,findFirstDashOperator(findLastParenthesisOpen(head))) !=
                        findFirstParenthesisClosed(head)
 */

/* Testing that calculate function works as intended:
int resultProbe = calculate(findPrevious(head,findFirstDashOperator(head))->number,findFirstDashOperator(head)->next->number,
                            findFirstDashOperator(head)->operator);
printf("Test result: %d", resultProbe);
 */

/* Testing createResultNode:
 * struct node* result = createResultNode(head, findFirstDashOperator(head));
    printf("Test result: %d", result->number);
 */


/*
    findFirstDashOperator(head);
    if(dashFound){
        printf("Dash was found :) \n");
    } else {
        printf("Dash not found :( \n");
    }

    findFirstParenthesisClosed(head);
    if(closedParFound){
        printf("Closed Par was found :) \n");
    } else {
        printf("Closed Par not found :( \n");
    }

    findLastParenthesisOpen(head);
    if(parFound){
        printf("Par was found :) \n");
    } else {
        printf("Par not found :( \n");
    }

*/


/*
 *     int posOfPoint = findPositionOfNode(head, findFirstPointOperator(findLastParenthesisOpen(head)));
    int posOfKlammerzu = findPositionOfNode(head,findFirstParenthesisClosed(head));
    int posOfKlammerAuf = findPositionOfNode(head,findLastParenthesisOpen(head));
    int posOfPlus = findPositionOfNode(head,findFirstDashOperator(findLastParenthesisOpen(head)));

    printf("\nFirst Point starting from last ( : %d \n", posOfPoint);
    printf(") at position : %d \n", posOfKlammerzu);
    printf("( at position : %d \n", posOfKlammerAuf);
    printf("First plus starting from last ( : %d \n", posOfPlus);
 *
 */