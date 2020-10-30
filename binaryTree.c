/*******************************************************
*  Purpose :  Read the data file MobileSubscription.txt
*             which contains mbile phone subscriiptions
*             in different countries. And then output
*             the analysed data into a file.
*
*  By   : Pierre Chow
*  Date : 01/07/2014
*******************************************************/
#include <stdio.h>
#include <string.h>

#define NAME_SIZE (50)
#define YEARS (10)
#define DIRECTORY_SIZE (512)
#define ASCENDING (0)
#define DESCENDING (1)

enum Year {y2000 = 0, y2001, y2002, y2003, y2004, y2005, y2006, y2007, y2008, y2009};

struct mobileSubscription {
  char countryName[NAME_SIZE];              /* name of the country */
  unsigned long int figureOfYear[YEARS];    /* Array of year containing subscription figure */
  struct mobileSubscription* left;          /* left node */
  struct mobileSubscription* right;         /* right node */
  struct mobileSubscription* next;          /* Linked list node. For section 4 report*/
};

struct mobileSubscription* addMobileData(struct mobileSubscription* list, char *countryName, unsigned long int *figureOfYear){
  struct mobileSubscription* newNode;
  struct mobileSubscription* tempNode;
  int i;

  /* allocate memory block and assign values */
  newNode = (struct mobileSubscription*)malloc(sizeof(struct mobileSubscription));
  strcpy(newNode->countryName, countryName);
  for(i = 0; i < YEARS; i++){
    newNode->figureOfYear[i] = figureOfYear[i];
  }
  newNode->left = NULL;
  newNode->right = NULL;
  newNode->next = NULL;

  /* check if the list is empty, return the newnode if it is NULL */
  if(list == NULL)
    return newNode;

  /* Search the correct location for insertion */
  tempNode = list;

  while(1){    /* Compare the names, if tempNode->countryName is smaller, go left */
    if(strcmp(tempNode->countryName, countryName) > 0){
      if(tempNode->left == NULL){    /* add here if left is NULL */
        tempNode->left = newNode;
        return list;
      }
      else{
        tempNode = tempNode->left;    /* go to next left if left is not NULL */
        continue;
      }
    }

    /* Compare the names, if tempNode->countryName is greater, go right */
    if(strcmp(tempNode->countryName, countryName) < 0){
      if(tempNode->right == NULL){
        tempNode->right = newNode;   /* add here if right is NULL */
        return list;
      }
      else{
        tempNode = tempNode->right;   /* go to next left if right is not NULL */
        continue;
      }
    }
    else{
      /* Error occurs */
      printf("Error. Same country name \"%s\". THIS LINE of data won't accepted.\nPress Enter to continue.\n", countryName);
      free(newNode);
      fflush(stdin);
      getchar();
      return list;
    }
  }
}  

struct mobileSubscription* readFile(struct mobileSubscription* list, char *fileName){
  FILE *fp;
  char countryName[NAME_SIZE];
  unsigned long int figureOfYear[YEARS];

  fp = fopen(fileName, "r");
  if (fp == NULL){ /* File open failure, programme exit */
    printf("Failed to open the %s file.\n", fileName);
    return 1;
  }else{
    printf("File %s opened.\n", fileName);
  }

  printf("Reading Data...\n");

  /* Remove the first line of the data file */
  fscanf(fp, "%*[^\n]\n");

  /* Read data and add to the list by calling addMobileData function */
  while(fscanf(fp, "%[^,],%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu\n",
        countryName, &figureOfYear[y2000], &figureOfYear[y2001], &figureOfYear[y2002], &figureOfYear[y2003], &figureOfYear[y2004],
        &figureOfYear[y2005], &figureOfYear[y2006], &figureOfYear[y2007], &figureOfYear[y2008], &figureOfYear[y2009]) != EOF){
    list = addMobileData(list, countryName, figureOfYear);
  }

  printf("Data input completed.\n");
  fclose(fp);
  return list;
}

void creatDirectory(char *fileName){
  int len;  /* For creating directory, store the length of the directory name */
  char directory[DIRECTORY_SIZE];
  char *ptr = fileName;

  /* create all the directories specified in fileName variable */
  while((ptr = strchr( ptr, '/' )) != NULL){
  /* copy directory paths to directory array layer by layer by using while loop */
    len =  strlen(fileName) - strlen(++ptr);  /* ++ptr to include the '/' character */
    strncpy(directory, fileName, len);
    directory[len] = '\0';                    /* End the string array */
    mkdir(directory, 0777);                   /* Create the directory */
  }
}

void analysisSection_1(struct mobileSubscription* list, FILE* fp){
  if(list == NULL)
    return;

  analysisSection_1(list->left, fp);
  /* If the figure in 2009 is greater than zero, write into the report */
  if(list->figureOfYear[y2009] > 0){
    fprintf(fp, "%s,%lu\n", list->countryName, list->figureOfYear[y2009]);
  }
  analysisSection_1(list->right, fp);
}

void analysisSection_2(struct mobileSubscription* list, FILE* fp){
  if(list == NULL)
    return;

  analysisSection_2(list->left, fp);
  /* Make sure the total figure in the decade is zero */
  if(list->figureOfYear[y2000] + list->figureOfYear[y2001] + list->figureOfYear[y2002] + list->figureOfYear[y2003] + list->figureOfYear[y2004] + list->figureOfYear[y2005] + list->figureOfYear[y2006] + list->figureOfYear[y2007] + list->figureOfYear[y2008] + list->figureOfYear[y2009] == 0){
    fprintf(fp, "%s\n", list->countryName);
  }
  analysisSection_2(list->right, fp);
}

void analysisSection_3(struct mobileSubscription* list, FILE* fp){
  if(list == NULL)
    return;

  analysisSection_3(list->left, fp);
  /* Make sure from 2005 to 2009, each year's figure is greater than 50000000 */
  if(list->figureOfYear[y2005] >= 50000000 && list->figureOfYear[y2006] >= 50000000 && list->figureOfYear[y2007] >= 50000000 && list->figureOfYear[y2008] >= 50000000 && list->figureOfYear[y2009] >= 50000000){
    fprintf(fp, "%s\n", list->countryName);
  }
  analysisSection_3(list->right, fp);
}

void createLinkedList(struct mobileSubscription* list, struct mobileSubscription** linkedListNode, unsigned int rankingSize, int year){
  struct mobileSubscription* preNode;
  struct mobileSubscription* tempNode;
  unsigned int i;

  if(list == NULL)
    return;

  createLinkedList(list->left, linkedListNode, rankingSize, year);
  preNode = NULL;
  tempNode = *linkedListNode;
  i = 0;

  if(*linkedListNode == NULL){   /* If the linkedListNode is NULL, make list be the heading of the node */
     *linkedListNode = list;
  }
  else if(list->figureOfYear[year] > tempNode->figureOfYear[year]){   /* If list's figure is bigger than linkedListNode's figure, make list be the heading of the node */
    list->next = tempNode;
    *linkedListNode = list;
  }
  else{
    /* Find the proper position for inserting the list. It will not compare more than the rankingSize required because it is waste of time */
    while(tempNode != NULL && list->figureOfYear[year] <= tempNode->figureOfYear[year] && i < rankingSize){
      preNode = tempNode;
      tempNode = tempNode->next;
      i++;
    }
    if(i < rankingSize){         /* Only those within the rankingSize will be inserted */
      list->next = tempNode;     /* Insert the list between preNode and tempNode */
      preNode->next = list;
    }
  }
  createLinkedList(list->right, linkedListNode, rankingSize, year);
}

void outputLinkedList(struct mobileSubscription* linkedListNode, FILE* fp, unsigned int rankingSize, int year, int ascendingOrDescending){
  if(linkedListNode == NULL || rankingSize == 0)
    return;

  if(ascendingOrDescending == ASCENDING)
    fprintf(fp, "%s,%lu\n", linkedListNode->countryName, linkedListNode->figureOfYear[year]);

  outputLinkedList(linkedListNode->next, fp, rankingSize - 1, year, ascendingOrDescending); /* Call itself to make a recursion printing. rankingSize minus 1 each time */

  if(ascendingOrDescending == DESCENDING)
    fprintf(fp, "%s,%lu\n", linkedListNode->countryName, linkedListNode->figureOfYear[year]);
}

void analysisSection_4(struct mobileSubscription* list, FILE* fp){
  unsigned int rankingSize = 3;             /* Setup the number of ranking, you can change to other number if you wish */
  int year = y2009;                         /* Setup the year you want to be sorted or printed, you can change to other year if you wish. please refer to the enum at the top */
  int ascendingOrDescending = DESCENDING;   /* Setup ascending or descending, you can change it if you wish */

  struct mobileSubscription* linkedListNode = NULL;      /* Structure pointer for the linked list */

  createLinkedList(list, &linkedListNode, rankingSize, year); /* Call a function to creat a linked list sorted by the year you specified */
  outputLinkedList(linkedListNode, fp, rankingSize, year, ascendingOrDescending);  /* Call a function to output the ranking */
}

int writeReport(struct mobileSubscription* list, char *fileName){
  FILE *fp;        /* File pointer for output report */

  creatDirectory(fileName);     /* Create the directory by calling the creatDirectory function */

  fp = fopen(fileName, "w");
  if (fp == NULL){ /* File open failure, programme exit */
    printf("Failed to creat the %s file.\n", fileName);
    return 1;
  }else{
    printf("File %s created.\n", fileName);
  }

  /* Section 1 Analysis */
  fprintf(fp, "Section 1\n");
  analysisSection_1(list, fp);
  fprintf(fp, "\n");

  /* Section 2 Analysis */
  fprintf(fp, "Section 2\n");
  analysisSection_2(list, fp);
  fprintf(fp, "\n");

  /* Section 3 Analysis */
  fprintf(fp, "Section 3\n");
  analysisSection_3(list, fp);
  fprintf(fp, "\n");

  /* Section 4 Analysis */
  fprintf(fp, "Section 4\n");
  analysisSection_4(list, fp);
  
  printf("Report done.\n");
  fclose(fp);
  return 0;
}

int main(){

char inputFileName[] = "MobileSubscription.txt";   /* Initial input file name */
char outputFileName[] = "folder/results.txt";      /* Initial output file name */

struct mobileSubscription* list = NULL;

list = readFile(list, inputFileName);    /* Read data from MobileSubscription.txt */
writeReport(list, outputFileName);       /* Analyzes and outputs to the file results.txt */

printf("Programm done.\n");
fflush(stdin);
getchar();

return 0;
}


