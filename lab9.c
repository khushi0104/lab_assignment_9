#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType record;
	struct HashType* next;
	struct HashType* pHead;
	int count;
};

// Compute the hash function
int hash(int x)
{
	int hash = 0; 
	int mod;
	while (x != 0) {
		mod = x % 10; 
		hash += mod;
		x = x / 10;
	}

	return hash;
}

// Insert a record into hash table
void insertRecord(struct HashType* pHashArray, int hashSz, struct RecordType record)
{
    int index = hash(record.id);
    struct HashType* pHash = &pHashArray[index];
    while (pHash->next != NULL)
    {
        pHash = pHash->next;
    }

    struct HashType* pNewHash = (struct HashType*)malloc(sizeof(struct HashType));
    pNewHash->record = record;
    pNewHash->next = NULL;
    pHash->next = pNewHash;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
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
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	struct HashType *pHash;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		if (pHashArray[i].next == NULL)
        {
            continue;
        }

        pHash = &pHashArray[i];
        printf("Index %d -> ", i);

        while (pHash->next != NULL)
        {
            pHash = pHash->next;
            printf("%d,%c,%d -> ", pHash->record.id, pHash->record.name, pHash->record.order);
        }

        printf("\n");
    	
	}
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    
    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);
    
    // size of the hash table - next prime number after double the number of records
    int hashSz = 67;
    
    // Allocate memory for the hash table and initialize it
    struct HashType *pHashArray = (struct HashType*) malloc(sizeof(struct HashType) * hashSz);
    if (pHashArray == NULL)
    {
        printf("Cannot allocate memory\n");
        exit(-1);
    }
    for (int i = 0; i < hashSz; i++)
    {
        pHashArray[i].pHead = NULL;
		pHashArray[i].next = NULL;
        pHashArray[i].count = 0;
    }
    
    // Insert the records into the hash table
    for (int i = 0; i < recordSz; i++)
    {
        int index = hash(pRecords[i].id);
        insertRecord(&pHashArray[index], hashSz, pRecords[i]);
    }
    
    // Display the records in the hash table
    displayRecordsInHash(pHashArray, hashSz);
    
    // Free memory
    free(pRecords);
    free(pHashArray);
    
    return 0;
}
