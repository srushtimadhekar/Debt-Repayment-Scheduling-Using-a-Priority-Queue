#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for Debt
typedef struct {
    char description[50];  // Debt description (max 50 characters)
    float interestRate;    // Interest rate of the debt
    float amountDue;       // Amount due for the debt
    int debtId;            // Unique debt ID
} Debt;

// Structure for Priority Queue
typedef struct {
    Debt* queue;    // Array to hold debts (the heap)
    int size;       // Current number of debts in the queue
    int capacity;   // Max capacity of the queue
} PriorityQueue;

// Function to initialize the priority queue
void initializeQueue(PriorityQueue* pq, int capacity) {
    pq->capacity = capacity;
    pq->queue = (Debt*)malloc(sizeof(Debt) * capacity);
    pq->size = 0;
}

// Function to check if the queue is empty
int isQueueEmpty(PriorityQueue* pq) {
    return pq->size == 0;
}

// Function to check if the queue is full
int isQueueFull(PriorityQueue* pq) {
    return pq->size == pq->capacity;
}

// Function to swap two debts in the queue
void swapDebts(Debt* a, Debt* b) {
    Debt temp = *a;
    *a = *b;
    *b = temp;
}

// Function to maintain the max heap property by bubbling up
void bubblingUp(PriorityQueue* pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;

        Debt* currentDebt = &pq->queue[index];
        Debt* parentDebt = &pq->queue[parent];

        if (parentDebt->interestRate < currentDebt->interestRate ||
            (parentDebt->interestRate == currentDebt->interestRate && parentDebt->amountDue < currentDebt->amountDue)) {
            swapDebts(parentDebt, currentDebt);
            index = parent;
        } else {
            break;
        }
    }
}

// Function to restore the heap property by moving the element down
void restoreHeapProperty(PriorityQueue* pq, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int largest = index;

    Debt* currentDebt = &pq->queue[index];
    Debt* leftDebt = (left < pq->size) ? &pq->queue[left] : NULL;
    Debt* rightDebt = (right < pq->size) ? &pq->queue[right] : NULL;

    if (leftDebt && (leftDebt->interestRate > currentDebt->interestRate ||
        (leftDebt->interestRate == currentDebt->interestRate && leftDebt->amountDue > currentDebt->amountDue))) {
        largest = left;
    }

    if (rightDebt && (rightDebt->interestRate > (largest == left ? leftDebt->interestRate : currentDebt->interestRate) ||
        (rightDebt->interestRate == (largest == left ? leftDebt->interestRate : currentDebt->interestRate) && 
        rightDebt->amountDue > (largest == left ? leftDebt->amountDue : currentDebt->amountDue)))) {
        largest = right;
    }

    if (largest != index) {
        swapDebts(&pq->queue[index], &pq->queue[largest]);
        restoreHeapProperty(pq, largest);
    }
}

// Function to add a debt to the priority queue
void enqueueDebt(PriorityQueue* pq, char* description, float interestRate, float amountDue, int debtId) {
    if (isQueueFull(pq)) {
        printf("Queue is full! Cannot add more debts.\n");
        return;
    }

    Debt newDebt;
    strncpy(newDebt.description, description, 49);
    newDebt.description[49] = '\0';  // Ensure null termination
    newDebt.interestRate = interestRate;
    newDebt.amountDue = amountDue;
    newDebt.debtId = debtId;

    pq->queue[pq->size] = newDebt;
    pq->size++;

    bubblingUp(pq, pq->size - 1);
    printf("Debt '%s' added to the queue with Interest Rate: %.2f%% and Amount Due: %.2f.\n", description, interestRate, amountDue);
}

// Function to remove and repay the highest priority debt
void dequeueDebt(PriorityQueue* pq) {
    if (isQueueEmpty(pq)) {
        printf("Queue is empty! No debts to repay.\n");
        return;
    }

    Debt* frontDebt = &pq->queue[0];
    printf("Repaying Debt ID %d: '%s' with Interest Rate: %.2f%% and Amount Due: %.2f\n", frontDebt->debtId, frontDebt->description, frontDebt->interestRate, frontDebt->amountDue);

    pq->queue[0] = pq->queue[pq->size - 1];
    pq->size--;
    restoreHeapProperty(pq, 0);
}

// Function to display the front debt without removing it from the queue
void displayFrontDebt(PriorityQueue* pq) {
    if (isQueueEmpty(pq)) {
        printf("Queue is empty! No debts to display.\n");
        return;
    }

    Debt* frontDebt = &pq->queue[0];
    printf("Front Debt: '%s' (Interest Rate: %.2f%%, Amount Due: %.2f)\n", frontDebt->description, frontDebt->interestRate, frontDebt->amountDue);
}

// Function to assist customers with debt information or inquiries
void customerService(PriorityQueue* pq) {
    int debtId;
    printf("Enter Debt ID to inquire about: ");
    while (scanf("%d", &debtId) != 1) {
        printf("Invalid input. Please enter a valid debt ID: ");
        while(getchar() != '\n');
    }

    for (int i = 0; i < pq->size; i++) {
        Debt* currentDebt = &pq->queue[i];
        if (currentDebt->debtId == debtId) {
            printf("Debt ID %d found: '%s' with Interest Rate: %.2f%% and Amount Due: %.2f\n", debtId, currentDebt->description, currentDebt->interestRate, currentDebt->amountDue);
            return;
        }
    }

    printf("Debt ID %d not found in the system.\n", debtId);
}

// Main function
int main() {
    PriorityQueue pq;
    int capacity;

    // Greeting and Instructions
    printf("Welcome to the Debt Repayment Scheduling System!\n");
    printf("This system helps you manage your debts by prioritizing repayment based on the highest interest rate and amount due.\n");
    printf("You can add debts, repay the highest priority debt, inquire about debts, or view the current highest priority debt.\n");
    printf("Please enter the capacity of the priority queue (maximum number of debts you can add): ");
    while (scanf("%d", &capacity) != 1 || capacity <= 0) {
        printf("Invalid input. Please enter a positive number for the capacity: ");
        while(getchar() != '\n');
    }

    initializeQueue(&pq, capacity);

    int choice;
    while (1) {
        printf("\n--- Debt Repayment Scheduling ---\n");
        printf("1. Add Debt\n");
        printf("2. Repay Debt\n");
        printf("3. Display Front Debt\n");
        printf("4. Customer Service (Inquire about Debt)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        while (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
            printf("Invalid choice. Please enter a number between 1 and 5: ");
            while(getchar() != '\n');
        }

        if (choice == 1) {
            char description[50];
            float interestRate, amountDue;
            int debtId;

            printf("Enter debt description: ");
            scanf(" %[^\n]", description);

            printf("Enter interest rate: ");
            while (scanf("%f", &interestRate) != 1 || interestRate < 0 || interestRate > 100) {
                printf("Invalid interest rate. Please enter a number between 0 and 100: ");
                while(getchar() != '\n');
            }

            printf("Enter amount due: ");
            while (scanf("%f", &amountDue) != 1 || amountDue <= 0) {
                printf("Invalid amount. Please enter a positive value greater than zero: ");
                while(getchar() != '\n');
            }

            printf("Enter debt ID: ");
            while (scanf("%d", &debtId) != 1) {
                printf("Invalid input. Please enter a valid debt ID: ");
                while(getchar() != '\n');
            }

            enqueueDebt(&pq, description, interestRate, amountDue, debtId);
        } else if (choice == 2) {
            dequeueDebt(&pq);
        } else if (choice == 3) {
            displayFrontDebt(&pq);
        } else if (choice == 4) {
            customerService(&pq);
        } else {
            break;
        }
    }

    if (pq.queue != NULL) {
        free(pq.queue);
        pq.queue = NULL;
    }

    return 0;
}
