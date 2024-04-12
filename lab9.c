#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType {
  int id;
  char name;
  int order;
};

// Fill out this structure
struct HashType {
  struct RecordType *lists;
  struct HashType *next;
};

// Compute the hash function
int hash(int x) { return x % 23; }

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData) {
  FILE *inFile = fopen(inputFileName, "r");
  int dataSz = 0;
  int i, n;
  char c;
  struct RecordType *pRecord;
  *ppData = NULL;

  if (inFile) {
    fscanf(inFile, "%d\n", &dataSz);
    *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
    // Implement parse data block
    if (*ppData == NULL) {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i = 0; i < dataSz; ++i) {
      pRecord = *ppData + i;
      fscanf(inFile, "%d ", &n);
      pRecord->id = n;
      fscanf(inFile, "%c ", &c);
      pRecord->name = c;
      fscanf(inFile, "%d ", &n);
      pRecord->order = n;
    }

    fclose(inFile);
  }

  return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz) {
  int i;
  printf("\nRecords:\n");
  for (i = 0; i < dataSz; ++i) {
    printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
  }
  printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz,
                          struct RecordType *pRecords) {
  int i;
  struct HashType *currentNode;
  for (i = 0; i < hashSz; ++i) {
    // if index is occupied with any records, print all
    int index = hash(pRecords[i].id);
    currentNode = &pHashArray[index];

    while (currentNode != NULL) {
      currentNode = currentNode->next;
    }
    currentNode->lists = (struct RecordType *)malloc(sizeof(struct RecordType));
    currentNode->next = (struct HashType *)malloc(sizeof(struct HashType));

    currentNode->lists[index].id = pRecords[i].id;
    currentNode->lists[index].name = pRecords[i].name;
    currentNode->lists[index].order = pRecords[i].order;
    currentNode->next = NULL;


    for (i = 0; i < hashSz; ++i) {
      currentNode = &pHashArray[i];
      printf("Index %d -> ", i);
      while (currentNode->next != NULL) {
        printf("%d %c %d -> ", currentNode->next->lists->id,
               currentNode->next->lists->name, currentNode->next->lists->order);
        currentNode = currentNode->next;
      }
      printf("\n");
    }
  }
}

int main(void) {
  struct RecordType *pRecords;
  int recordSz = 0;

  recordSz = parseData("input_lab_9.txt", &pRecords);
  printRecords(pRecords, recordSz);
  // Your hash implementation

  struct HashType *pHashArray =
      (struct HashType *)malloc(recordSz * sizeof(struct HashType));
  pHashArray->lists = (struct RecordType *)malloc(sizeof(struct RecordType)*recordSz);
  for (int i=0; i<recordSz; i++) {
    pHashArray->lists[i].id = 0;
    pHashArray->lists[i].name = '\0';
    pHashArray->lists[i].order = 0;
    pHashArray->next = NULL;
  }

  displayRecordsInHash(pHashArray, recordSz, pRecords);

  free(pRecords);
  free(pHashArray);
}