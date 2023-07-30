#include <stdio.h>

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
    struct RecordType* records; 
    int num_records; 
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x % hashSz;
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
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    for (int i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].records != NULL && pHashArray[i].num_records > 0)
        {
            printf("Index %d -> ", i);
            for (int j = 0; j < pHashArray[i].num_records; ++j)
            {
                printf("%d %c %d", pHashArray[i].records[j].id, pHashArray[i].records[j].name, pHashArray[i].records[j].order);
                if (j < pHashArray[i].num_records - 1)
                {
                    printf(" -> ");
                }
            }
            printf("\n");
        }
    }
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
  int hashSz = 10;

   struct HashType* pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
    if (pHashArray == NULL)
    {
        printf("Cannot allocate memory for the hash table\n");
        exit(-1);
    }

    // Initialize each bucket in the hash table
    for (int i = 0; i < hashSz; i++)
    {
        pHashArray[i].records = NULL; 
        pHashArray[i].num_records = 0;
    }

    // Insert records into the hash table based on their hash values
    for (int i = 0; i < recordSz; i++)
    {
        int hashed_index = hash(pRecords[i].id, hashSz);
        struct HashType* bucket = &pHashArray[hashed_index];

        // Allocate memory for the new record in the bucket
        bucket->records = (struct RecordType*)realloc(bucket->records, sizeof(struct RecordType) * (bucket->num_records + 1));

        if (bucket->records == NULL)
        {
            printf("Cannot allocate memory for records in the bucket\n");
            exit(-1);
        }

        // Add the record to the bucket
        bucket->records[bucket->num_records] = pRecords[i];
        bucket->num_records++;
    }

    displayRecordsInHash(pHashArray, hashSz);

    //Free Memory Used for no memory leaks
    for (int i = 0; i < hashSz; i++)
    {
        free(pHashArray[i].records);
    }
  
    free(pHashArray);

  
    free(pRecords);

    return 0;
}