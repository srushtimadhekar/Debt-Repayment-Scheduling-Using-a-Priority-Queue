#include <stdio.h>
#include <stdlib.h>

// Structure for Debt
typedef struct {
    char description[50]; // Debt description (max 50 characters)
    float interestRate;    // Interest rate of the debt
    float amountDue;       // Amount due for the debt
    int debtId;            // Unique debt ID
} Debt;

// Structure for PriorityQueue
typedef struct {
    Debt* queue;    // Array to hold debts
    int front, rear;  // Indices for the front and rear of the queue
    int capacity;   // Max capacity of the queue
} PriorityQueue;

// Function to initialize the queue
void initializeQueue(PriorityQueue* pq, int capacity) {
    pq->capacity = capacity;
    pq->queue = (Debt*)malloc(sizeof(Debt) * capacity); // Dynamically allocate memory
    pq->front = pq->rear = -1;  // Initialize indices
}

// Function to check if the queue is empty
int isQueueEmpty(PriorityQueue* pq) {
    return pq->front == -1;
}

// Function to check if the queue is full
int isQueueFull(PriorityQueue* pq) {
    return pq->rear == pq->capacity - 1;
}

// Function to swap two debts in the queue (for maintaining heap property)
void swapDebts(Debt* a, Debt* b) {
    Debt temp = *a;
    *a = *b;
    *b = temp;
}

// Function to maintain the heap property by bubbling up the debt
void heapifyQueue(PriorityQueue* pq, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // Compare the current debt with its left child
    if (left <= pq->rear && (pq->queue[left].interestRate > pq->queue[largest].interestRate ||
        (pq->queue[left].interestRate == pq->queue[largest].interestRate && pq->queue[left].amountDue > pq->queue[largest].amountDue))) {
        largest = left;
    }

    // Compare the current debt with its right child
    if (right <= pq->rear && (pq->queue[right].interestRate > pq->queue[largest].interestRate ||
        (pq->queue[right].interestRate == pq->queue[largest].interestRate && pq->queue[right].amountDue > pq->queue[largest].amountDue))) {
        largest = right;
    }

    // If largest is not the current index, swap and recursively heapify
    if (largest != index) {
        swapDebts(&pq->queue[index], &pq->queue[largest]);
        heapifyQueue(pq, largest);
    }
}

// Function to add a debt to the priority queue
void enqueueDebt(PriorityQueue* pq, char* description, float interestRate, float amountDue, int debtId) {
    if (isQueueFull(pq)) {
        printf("Queue is full! Cannot add more debts.\n");
        return;
    }

    Debt newDebt;
    int i = 0;
    // Manually copy the description into the newDebt structure
    while (description[i] != '\0' && i < 49) {
        newDebt.description[i] = description[i];
        i++;
    }
    newDebt.description[i] = '\0';  // Ensure null-termination

    newDebt.interestRate = interestRate;
    newDebt.amountDue = amountDue;
    newDebt.debtId = debtId;

    // Insert the new debt at the rear of the queue
    pq->queue[++pq->rear] = newDebt;

    // Reorganize the queue to maintain heap property
    i = pq->rear;
    while (i > 0 && (pq->queue[(i - 1) / 2].interestRate < pq->queue[i].interestRate ||
        (pq->queue[(i - 1) / 2].interestRate == pq->queue[i].interestRate && pq->queue[(i - 1) / 2].amountDue < pq->queue[i].amountDue))) {
        swapDebts(&pq->queue[i], &pq->queue[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

    printf("Debt '%s' (Interest Rate: %.2f%%, Amount Due: %.2f) added to queue.\n", description, interestRate, amountDue);
}

// Function to remove and repay the highest priority debt (highest interest rate, then highest amount due)
void dequeueDebt(PriorityQueue* pq) {
    if (isQueueEmpty(pq)) {
        printf("Queue is empty! No debts to repay.\n");
        return;
    }

    Debt frontDebt = pq->queue[0];
    printf("Repaying debt ID %d: '%s' with Interest Rate: %.2f%% and Amount Due: %.2f\n", frontDebt.debtId, frontDebt.description, frontDebt.interestRate, frontDebt.amountDue);

    // Replace the root with the last element and reheapify
    pq->queue[0] = pq->queue[pq->rear--];
    heapifyQueue(pq, 0);
}

// Function to display the front debt without removing it from the queue
void displayFrontDebt(PriorityQueue* pq) {
    if (isQueueEmpty(pq)) {
        printf("Queue is empty! No debts to display.\n");
        return;
    }

    printf("Front debt: '%s' (Interest Rate: %.2f%%, Amount Due: %.2f)\n", pq->queue[0].description, pq->queue[0].interestRate, pq->queue[0].amountDue);
}

// Main function to drive the program
int main() {
    PriorityQueue pq;
    int capacity;

    // Ask the user for the capacity of the queue
    printf("Enter the capacity of the priority queue: ");
    while (scanf("%d", &capacity) != 1 || capacity <= 0) {
        printf("Invalid input. Please enter a positive number for the capacity: ");
        while(getchar() != '\n'); // Clear the input buffer
    }

    // Initialize the queue
    initializeQueue(&pq, capacity);

    int choice;
    while (1) {
        printf("\n--- Debt Repayment Scheduling ---\n");
        printf("1. Add Debt\n");
        printf("2. Repay Debt\n");
        printf("3. Display Front Debt\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        while (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
            printf("Invalid choice. Please enter a number between 1 and 4: ");
            while(getchar() != '\n'); // Clear the input buffer
        }

        if (choice == 1) {
            char description[50];
            float interestRate, amountDue;
            int debtId;

            printf("Enter debt description: ");
            scanf(" %[^\n]", description);  // Read full description with spaces

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
                while(getchar() != '\n'); // Clear the input buffer
            }

            // Add debt to the queue
            enqueueDebt(&pq, description, interestRate, amountDue, debtId);
        } 
        else if (choice == 2) {
            dequeueDebt(&pq);
        } 
        else if (choice == 3) {
            displayFrontDebt(&pq);
        } 
        else {
            break;
        }
    }

    // Free the allocated memory for the queue
    if (pq.queue != NULL) {
        free(pq.queue);
        pq.queue = NULL; // Ensure pointer is null after freeing
    }

    return 0;
}
