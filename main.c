#include <stdio.h>
#include<stdlib.h>
#include <string.h>
int totalTime = 0;

struct element {	//We are defining the struct here.
	int number;
	int frequency;
	char word[20];
	struct element *right;	//Since we want a tree structure we define the right and left elements inside the struct.
	struct element *left;
};

typedef struct element Element;


Element *add(int number, int frequency, char *word) {	//In this function we create new elements.
	Element *node;										
	node = malloc(sizeof(Element));						//We allocate a part of the memory to hold the element.
	node->number = number;								//We assign the values that we got from the file one by one.
	node->frequency = frequency;
	strcpy(node->word, word);
	node->left = NULL;
	node->right = NULL;
	
	return node;
}

Element *insert(Element *root, Element *node) {			//In this function we insert the elements alphabetically in the tree.
	if(root == NULL) {									//If root is NULL that means tree is empty. So we assigned the root.
		return node;
	}
	if(stricmp(root->word, node->word) > 0) {			//With these if statements we compare the words with the elements.
		root->left = insert(root->left, node);			//After comparison we put the elements in the correct place in the tree.
	}
	else if (stricmp(root->word, node->word) < 0) {
		root->right = insert(root->right, node);
	}
	return root;										//After the operations we send the real root of the tree back to main function.
}

Element *frequencyInsert(Element *arr, Element *freqRoot, int count, int a) {	//In this function we insert the elements sorted by frequencies.
	if(a < count) {								//We do the inserting operation until the current node counter is smaller than original count of the array.
		freqRoot = &arr[a];						//We assign the current element to the root.
		freqRoot->left = frequencyInsert(arr, freqRoot->left, count, 2 * a + 1);	//We traverse the tree and make the level order insertion in the tree.
		freqRoot->right = frequencyInsert(arr,freqRoot->right, count, 2 * a + 2);
	}
	return freqRoot;		//We return the unchanged root to the main function.
}

void orderLNR(Element *root) { 		//We print the elements in this function
    if (root != NULL) { 			//We check if the root is empty or not.Since we are using recursion root = NULL shows that we are at the last element.
        orderLNR(root->left); 		//First we traverse to leftmost child.
        printf("%s  ", root->word); 	//We print from leftmost child to root.
        orderLNR(root->right); 			//We traverse to rightmost child then we print it to the root.
    } 
} 

void sort(Element *arr, int lineCount) { 	//We sort the elements in the array by frequency using bubble sort.
   int i, j; 
   for (i = 0; i < lineCount - 1; i++) {
       for (j = 0; j < lineCount - i - 1; j++) {
           if (arr[j].frequency < arr[j+1].frequency) {
           		Element temp;			//We assign the temporary element. We are using it because we dont want to lose our element in swap operation.
           		temp = arr[j];
           		arr[j] = arr[j+1];
           		arr[j+1] = temp;
		   }      
	   } 
   }     
}

int accessTimeCalculator(Element *root,int depth) {		//We calculate the total access time.
	if(root != NULL) {									
		accessTimeCalculator(root->left, depth + 1);	//As in the print function we traverse the tree by LNR order.	
		totalTime += (depth + 1) * (root->frequency);			//We calculate total Time using this formula.
		accessTimeCalculator(root->right, depth + 1);
	}
	return totalTime;
}

int main(int argc, char *argv[]) {
	FILE *fptr = fopen("input.txt", "r");	//We open the file and assign the pointer.
	char str[40];
	Element *root = NULL;					//At first there is nothing in the tree so we assign it to NULL
	int lineCount = 0;
	
	while(fgets(str, 40, fptr) != NULL)		//We read the file until the end of the file.
		lineCount++;		//We count the lines to hold elements in the array.
	fclose(fptr);	//We close the file and oper it again to reset the position of the pointer.
	fptr = fopen("input.txt", "r");
	Element elementArray[lineCount];	//We create the array to hold frequency sorted elements.
	
	while(fgets(str, 40, fptr) != NULL) {	//We read the input file line by line.
		int number;
		int frequency;
		char word[20];
		char *numberToken;
		char *freqToken;
		char *wordToken;
		
		numberToken = strtok(str, ",");		//In the next three lines we tokenize the line by using strtok method.
		wordToken = strtok(NULL, ",");
		freqToken = strtok(NULL, ",");
		
		number = atoi(numberToken);		//Since tokenizer returns strings, we convert them to integers by using atoi method.
		strcpy(word, wordToken);		//We are using strcpy method to assign strings.
		frequency = atoi(freqToken);
		
		Element *node = add(number,frequency,word);		//We create the node with the correct variables we got from the file.
		elementArray[number - 1] = *add(number,frequency,word);	//Also we add them to the frequency array.
		root = insert(root, node);	//At the end of the loop we insert the elements to BST.
	}
	sort(elementArray, lineCount); //We sort the elements by their frequencies.
	Element *freqRoot = &elementArray[0];	//We assign the first element of the frequency sorted array to freqRoot.
	frequencyInsert(elementArray, freqRoot, lineCount, 0);	//Since we have all the elements sorted in the array we call the function and insert the tree.
	printf("Frequency sorted tree (LNR):\n\n");			//We print the trees and access times.
	orderLNR(freqRoot);
	printf("\n\nTotal access time: %d\n", accessTimeCalculator(freqRoot, 0));
	printf("\n\nAlphabetically sorted BST (LNR):\n\n");
	totalTime = 0;			//We reset the access time otherwise it will stack up.
	orderLNR(root);
	printf("\n\nTotal access time: %d", accessTimeCalculator(root, 0));
	return 0;
}

