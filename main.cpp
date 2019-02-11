/*  Code for the user interface for Lab 4 for CS 211 Fall 2015
 *
 *  Author: Pat Troy
 *  Date: 10/6/2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef enum {FALSE = 0, TRUE, NO = 0, YES} boolean;

/* forward definition of functions */
void clearToEoln();


 struct list{
    char name[30];
    int groupsize;
    int status;
     struct list* next;
};

int Debug ;

//If the person called ahead, the status is 1, if they didn't, the status is 0

/* Read in until the first Non-White-Space character is Read */
/* The white space characters are:
 *      space, tab \t, newline \n, vertical tab \v,
 *      form feed \f, and carriage return \r
 */
int getNextNWSChar ()
{
    int ch;
    
    ch = getc(stdin);
    if (ch == EOF)
        return ch;
    while (isspace (ch))
    {
        ch = getc(stdin);
        if (ch == EOF)
            return ch;
    }
    return ch;
}

/* read in the next Positive Integer or error:    */
/* This is based on the Mathematical definition of a Postive Integer */
/*    zero is not counted as a positive number */
int getPosInt ()
{
    int value = 0;
    
    /* clear white space characters */
    int ch;
    ch = getc(stdin);
    while (!isdigit(ch))
    {
        if ('\n' == ch)  /* error \n ==> no integer given */
            return 0;
        if (!isspace(ch)) /* error non white space ==> integer not given next */
        {
            clearToEoln();
            return 0;
        }
        ch = getc(stdin);
    }
    
    value = ch - '0';
    ch = getc(stdin);
    while (isdigit(ch))
    {
        value = value * 10 + ch - '0';
        ch = getc(stdin);
    }
    
    ungetc (ch, stdin);  /* put the last read character back in input stream */
    
    /* Integer value of 0 is an error in this program */
    if (0 == value)
        clearToEoln();
    
    return value;
}

/* read in the name until the end of the input */
char *getName()
{
    /* skip over the white space characters */
    int ch;
    ch = getc(stdin);
    while (isspace(ch))
    {
        if ('\n' == ch)  /* error \n ==> no integer given */
            return NULL;
        ch = getc(stdin);
    }
    
    char *word;
    int size;
    size = 10;
    word = (char *) malloc (sizeof(char) * size);
    
    // read in character-by-character until the newline is encountered
    int count = 0;
    
    while (ch != '\n')
    {
        if (count+1 >= size)
        {
            // to grow an array to make it "dynamically sized" using malloc
            char* temp;
            int i;
            size = size * 2;
            temp = (char *) malloc (sizeof(char) * size);
            
            // printf ("Growing array from size %d to size %d\n", count, size);
            // copy the characters to the new array
            for (i = 0 ; i < count ; i++)
                temp[i] = word[i];
            
            free (word);
            word = temp;
        }
        
        word[count] = ch;
        count++;
        word[count] = '\0';
        
        // read next character
        ch = getc(stdin);
    }
    
    if (count > 30)
    {
        count = 30;
        word[count] = '\0';
    }
    
    /* clear ending white space characters */
    while (isspace (word[count-1]))
    {
        count--;
        word[count] = '\0';
    }
    
    return word;
}

/* Clear input until next End of Line Character - \n */
void clearToEoln()
{
    int ch;
    
    do {
        ch = getc(stdin);
    }
    while ((ch != '\n') && (ch != EOF));
}

/* Print out a list of the commands for this program */
void printCommands()
{
    printf ("The commands for this program are:\n\n");
    printf ("q - to quit the program\n");
    printf ("? - to list the accepted commands\n");
    printf ("a <size> <name> - to add a group to the wait list\n");
    printf ("c <size> <name> - to add a call-ahead group to the wait list\n");
    printf ("w <name> - to specify a call-ahead group is now waiting in the restaurant\n");
    printf ("r <table-size> - to retrieve the first waiting group that can fit at the available table size\n");
    printf ("l <name> - list how many groups are ahead of the named group\n");
    printf ("d - display the wait list information\n");
    
    /* clear input to End of Line */
    clearToEoln();
}

typedef struct list snode;
snode *newnode, *temp, *ptr;



void addTo(snode** hd, int val, char *Name, boolean status)
{
    int stat = status;
    snode* ptr = (snode*) malloc (sizeof(snode));
    ptr->groupsize = val;
    ptr->status = stat;
    strncpy(ptr->name, Name, 30);
    ptr->next = *hd;       /* note that hd must be "de-referenced" when used */
    *hd = ptr;             /*   the unary * operator is the "de-reference" operator */
}

int doesNameExist(snode *hd, char* Name){
    
    snode *ptr = hd;
    
    while(ptr != NULL){
        if(strncmp(ptr->name, Name, 30)== 0){
            return TRUE;
        }
        else{
            ptr=ptr->next;
        }
    }
    return FALSE;
}

int updateStatus(snode *hd, char* Name, boolean status){
    
    snode *ptr = hd;
    int stat = status;
    while(ptr != NULL){
        if(strncmp(ptr->name, Name, 30)== 0){
            
            if (ptr->status == stat){
                printf("The group is marked as being in the restuarant");
                return FALSE;
            }
            else {
                ptr->status = stat;
                return 1;
            }
            
            
        }
        else{
            ptr=ptr->next;
        }
    }
    return 0;
}

snode *header, *ptr1;

void retrieveAndRemove (snode** hd, int size, boolean nostatus)
{
    
    
    snode *currP, *prevP;
    
    /* For 1st node, indicate there is no previous. */
    prevP = NULL;
    
    /*
     * Visit each node, maintaining a pointer to
     * the previous node we just visited.
     */
    for (currP = *hd;
         currP != NULL;
         prevP = currP, currP = currP->next) {
        
        if (currP->groupsize <= size) {  /* Found it. */
            if (prevP == NULL) {
                /* Fix beginning pointer. */
                *hd = currP->next;
            } else {
                /*
                 * Fix previous node's next to
                 * skip over the removed node.
                 */
                if (currP->status == TRUE){
                
                    prevP->next = currP->next;}
                else {
                    retrieveAndRemove (hd, size,  nostatus);
                }
            }
            
            /* Deallocate the node. */
            
            free(currP);
            
            /* Done searching. */
            return;
        }
    }
}
    
    


void deleteNode(snode **hd, int size)
{
    ptr = *hd;
    
    
    
    if(ptr == NULL)
    {
        printf("\nEmpty Linked List. Deletion not possible.\n");
    }
    else
    {
       
        
        
        while((ptr->next != NULL) && (ptr->groupsize != size))
        {
            ptr1 = ptr;
            ptr = ptr->next;
        }
        if(ptr->groupsize == size)
        {
            ptr1->next = temp;
            temp = ptr->next;
            free(ptr);
            printf("\nNode with data %d deleted.\n", size);
        }
        else
        {
            printf("\nValue %d not found. Deletion not possible.\n", size);
        }
    }
}

void displayGroupSizeAhead ( snode* hd, boolean status, char *Name ){
    
    while (hd != NULL)
    {
        if(strncmp(hd->name, Name, 30) != 0){
            
            
            //printf (" %s \n", hd->name);
            hd=hd->next;
            
            
        }
        else {
            
            
            while (hd != NULL){
                
                
                printf (" %d \n", hd->groupsize);
                
                hd=hd->next;
                
                if (hd == NULL) break;
                
                
                
            }
            
            
        }
        
        //hd = hd->next;
    }
    printf ("\n");
    
}

void countGroupsAhead(snode* hd, boolean status, char *Name)
{
    
    while (hd != NULL)
    {
        if(strncmp(hd->name, Name, 30) == 0){
            
            
            //printf (" %s \n", hd->name);
            
            while (hd != NULL){
                
                
                printf (" %s \n", hd->name);
                
                
                if (hd == NULL) break;
                
                hd=hd->next;
                
            }
            
        }
        else {
            hd=hd->next;
            
            
        }
        
        //hd = hd->next;
    }
    printf ("\n");
}

void displayListInformation (snode* hd, boolean status)
{
    // ***For normal printing
    //int count = -1;
    
//    while (hd != NULL)
//    {
//        printf (" Group: ");
//        printf (" %d %s ", hd->groupsize, hd->name);
//        if(hd->status == TRUE){
//            printf("Waiting in restuarant \n");
//        }
//        else {
//            printf("Called in\n");
//        }
//        hd = hd->next;
//        
//    }
//    
//    printf ("\n");
    
    //****print in reverse
        if (hd == NULL)
            return;
    
        // print the list after head node
        displayListInformation(hd->next, status);
    
        // After everything else is printed, print head
        printf ("%d %s ", hd->groupsize, hd->name);
                if(hd->status == TRUE){
                    printf("Waiting in restuarant \n");
                }
                else {
                    printf("Called in\n");
                }

}



void doAdd (snode **hd, boolean status)
{
    /* get group size from input */
    
    int size = getPosInt();
    if (size < 1)
    {
        printf ("Error: Add command requires an integer value of at least 1\n");
        printf ("Add command is of form: a <size> <name>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        printf ("         <name> is the name of the group making the reservation\n");
        return;
    }
    
    /* get group name from input */
    char *name = getName();
    if (NULL == name)
    {
        
        
        
        
        printf ("Error: Add command requires a name to be given\n");
        printf ("Add command is of form: a <size> <name>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        printf ("         <name> is the name of the group making the reservation\n");
        return;
    }
    //printf("%zu\n ", strlen(name));
    //valsize =strlen(name);
    
    
    printf ("Adding group \"%s\" of size %d\n", name, size);
    if (     doesNameExist(*hd, name) == TRUE){
        fprintf(stderr, "%s", "Name already exists!\n");
        
    }
    else {
        //fprintf(stderr, "%s", "Name doesn't exit in the list.\n");
        addTo( hd, size, name, status);
    }
    
    if (Debug == TRUE){
        printf("Debug activated \n");
    }
    if (Debug == TRUE){
        fprintf(stderr, "%s", "Current List: \n");
        displayListInformation(*hd, status);
    }
    
    
    // add code to perform this operation here

}


void doCallAhead (snode **hd, boolean status)
{
    /* get group size from input */
    int size = getPosInt();
    if (size < 1)
    {
        printf ("Error: Call-ahead command requires an integer value of at least 1\n");
        printf ("Call-ahead command is of form: c <size> <name>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        printf ("         <name> is the name of the group making the reservation\n");
        return;
    }
    
    /* get group name from input */
    char *name = getName();
    if (NULL == name)
    {
        printf ("Error: Call-ahead command requires a name to be given\n");
        printf ("Call-ahead command is of form: c <size> <name>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        printf ("         <name> is the name of the group making the reservation\n");
        return;
    }
    
    printf ("Call-ahead group \"%s\" of size %d\n", name, size);
    
    // add code to perform this operation here
    
    if (     doesNameExist(*hd, name) == TRUE){
        fprintf(stderr, "%s", "Name already exists!\n");
        
    }
    else {
        fprintf(stderr, "%s", "Name doesn't exit in the list.\n");
        addTo( hd, size, name, status);
    }
    
    if (Debug == TRUE){
        fprintf(stderr, "%s", "Current List: \n");
        displayListInformation(*hd, status);
    }
    
}



void doWaiting (snode* hd, boolean status)
{
    /* get group name from input */
    char *name = getName();
    if (NULL == name)
    {
        printf ("Error: Waiting command requires a name to be given\n");
        printf ("Waiting command is of form: w <name>\n");
        printf ("  where: <name> is the name of the group that is now waiting\n");
        return;
    }
    
    printf ("Waiting group \"%s\" is now in the restaurant\n", name);
    
    // add code to perform this operation here
    updateStatus(hd, name, status);
    if(Debug == TRUE){
        fprintf(stderr, "%s", "Current List: \n");
        displayListInformation(hd, status);
    }
    

}


void doRetrieve (snode **hd, boolean nostatus)
{
    /* get table size from input */
    int size = getPosInt();
    if (size < 1)
    {
        printf ("Error: Retrieve command requires an integer value of at least 1\n");
        printf ("Retrieve command is of form: r <size>\n");
        printf ("  where: <size> is the size of the group making the reservation\n");
        return;
    }
    clearToEoln();
    printf ("Retrieve (and remove) the first group that can fit at a tabel of size %d\n", size);
    
    // add code to perform this operation here
  retrieveAndRemove(hd, size, nostatus);
    if(Debug == TRUE){
        fprintf(stderr, "%s", "Current List: \n");
        retrieveAndRemove(hd, size, nostatus);}
    
}


void doList (snode *hd, boolean status)
{
    /* get group name from input */
    char *name = getName();
    if (NULL == name)
    {
        printf ("Error: List command requires a name to be given\n");
        printf ("List command is of form: l <name>\n");
        printf ("  where: <name> is the name of the group to inquire about\n");
        return;
    }
    
    printf ("Group \"%s\" is behind the following groups\n", name);
    
    // add code to perform this operation here
    countGroupsAhead (hd, status, name);
    displayGroupSizeAhead (hd, status, name);
    if(Debug == TRUE){
        fprintf(stderr, "%s", "Current List: \n");
        displayListInformation(hd, status);}
    

}



void doDisplay (snode *hd, boolean status)
{
    
    clearToEoln();
    
    // add code to perform this operation here
    displayListInformation(hd, status);
    
}





int main (int argc, char **argv)
{
    snode *head = NULL, *last = NULL;
    char *input;
    int ch;
    boolean status = TRUE;
    boolean nostatus = FALSE;
    int i;
    
    for (i = 0; i < argc; i++)
    {
        
        if ('-' == argv[i][0]){
            if ('d' == argv[i][1]){
                /*
                 Activates the degug mode
                 */
                Debug=TRUE;
                
            }
            
        }
        
        else {Debug=FALSE;}
        
    }
    
    printf ("Starting Restaurant Wait List Program\n\n");
    
    printf ("Enter command: ");
    
    while ((ch = getNextNWSChar ()) != EOF)
    {
        /* check for the various commands */
        if ('q' == ch)
        {
            printf ("Quitting Program\n");
            return (0);
        }
        else if ('?' == ch)
        {
            printCommands();
        }
        else if('a' == ch)
        {
            doAdd(&head, status);
            
        } 
        else if('c' == ch)
        {
            doCallAhead(&head, nostatus);
        } 
        else if('w' == ch)
        {
            doWaiting(head, status);
        } 
        else if('r' == ch)
        {
            doRetrieve(&head, nostatus);
        } 
        else if('l' == ch)
        {
            doList(head, status);
        } 
        else if('d' == ch)
        {
            doDisplay(head, status);
           
        } 
        else
        {
            printf ("%c - in not a valid command\n", ch);
            printf ("For a list of valid commands, type ?\n");
            clearToEoln();
        }
        
        printf ("\nEnter command: ");
    }
    
    printf ("Quiting Program - EOF reached\n");
    return 1;
}