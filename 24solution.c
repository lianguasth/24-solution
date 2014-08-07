#include <stdio.h>
#include <stdlib.h> 

typedef struct{
	int number;
	char operator;
}Value;

typedef struct Node{
	int isNumber;
	Value value;
	struct Node* left;
	struct Node* right;
}Node;

typedef Node *Ptr;
typedef Ptr Root;

Root topRoot;
char operator[4];

void permuteAndCal(int *data, int start, int end);
void calRes(int *data, int n);
void freeTree(Root r);
void printTree(Ptr node);

int main(){
	int data[4];
	operator[0] = '+';
	operator[1] = '-';
	operator[2] = '*';
	operator[3] = '/';
	printf("please input for number:\n");
	scanf("%d %d %d %d", data, data+1, data+2, data+3);
	//make the permutation and calculate
	permuteAndCal(data, 0, 3);
	//calRes(data, 4);
	return 0;
}

void swamp(int *data, int i, int j){
	int temp = data[i];
	data[i] = data[j];
	data[j] = temp;
}

void printData(int *data){
	int i;
	for(i = 0; i < 4; i++){
		printf("%d\t", data[i]);
	}
	printf("\n");
}

void permuteAndCal(int *data, int start, int end){
	if(start == end){
		//printData(data);
		calRes(data, 4);
	}else{
		int i;
		for(i = start; i <= end; i++){
			swamp(data, start, i);
			//printf("%d\t%d\n", i, end);
			permuteAndCal(data, start+1, end);
			swamp(data, start, i);
		}
	}
}

float calTree(Root r){
	if(r->isNumber == 0){
		switch(r->value.operator){
			case '+':
				return calTree(r->left) + calTree(r->right);
			case '-':
				return calTree(r->left) - calTree(r->right);
			case '*':
				return calTree(r->left) * calTree(r->right);
			case '/':
				return calTree(r->left) / calTree(r->right);
			default:
				return -1;
		}
	}else{
		return (float)r->value.number;
	}
}

void assignValue(Ptr p, int *data){
	Ptr tmp;
	//printf("assign: p->value: %d\n", p->value.number);
	if(p->left == NULL){
		tmp = (Ptr)malloc(sizeof(Node));
		tmp->isNumber = 1;
		tmp->value.number = data[p->value.number];
		tmp->left = NULL;
		tmp->right = NULL;
		p->left = tmp;
	}else{
		assignValue(p->left, data);
	}
	if(p->right == NULL){
		tmp = (Ptr)malloc(sizeof(Node));
		tmp->isNumber = 1;
		tmp->value.number = data[p->value.number+1];
		tmp->left = NULL;
		tmp->right = NULL;
		p->right = tmp;
	}else{
		assignValue(p->right, data);
	}
}


void makeTree(int *index, int *data, int n){
	int i, ind;
	Ptr p,tmp;
	if(n >= 2){
		for(i = 0; i < n-1; i++){
			//make node
			p = (Ptr)malloc(sizeof(Node));
			p->isNumber = 0;
			ind = p->value.number = index[i];
			p->left = NULL;
			p->right = NULL;
			if(topRoot == NULL){
				topRoot = p;
				continue;
			}
			tmp = topRoot;
			while(1){
				//printf("p value %d, tmp value %d\n", p->value.number, tmp->value.number);
				if(p->value.number < tmp->value.number){
					if(tmp->left == NULL){
						tmp->left = p;
						break;
					}else{
						tmp = tmp->left;
					}
				}else{
					if(tmp->right == NULL){
						tmp->right = p;
						break;
					}else{
						tmp = tmp->right;
					}
				}
			}
		}
		assignValue(topRoot, data);
	}else{
		printf("less than one!\n");
	}
}

void assignOperator(char *set, Root root){
	if(root->isNumber == 0){
		root->value.operator = set[root->value.number];
		assignOperator(set, root->left);
		assignOperator(set, root->right);
	}
}

//permute the operation and then make a tree
void permuteAndTree(int *index, int start, int end, int* data){
	int j,i,p;
	if(start == end){
	/*
		for(j = 0; j <= end; j++){
			printf("%d\t ", index[j]);
		}
		printf("\n");
	*/
		makeTree(index, data, 4);
		char set[4];
		for(i = 0; i < 4; i++){
			set[0] = operator[i];
			for(j = 0; j < 4; j++){
				set[1] = operator[j];
				for(p = 0; p < 4; p++){
					set[2] = operator[p];
					assignOperator(set, topRoot);
					float res = calTree(topRoot);
					if(!(res < 24.0 || res > 24.0)){
						printTree(topRoot);
						printf("\n");
					}
				}
			}
		}
		freeTree(topRoot);
		topRoot = NULL;
	}else{
		for(i = start; i <= end; i++){
			swamp(index, start, i);
			//printf("%d\t%d\n", i, end);
			permuteAndTree(index, start+1, end, data);
			swamp(index, start, i);
		}
	}
}
//the root of the temporary tree
//using recursion to solve, failed
/*
void calRes(int *data, int leftmost, int rightmost, Root root, int r){
	int i,j;
	if(rightmost == leftmost){
		root->isNumber = 1;
		root->value.number = data[leftmost];
		//print the tree AT LAST
		if(rightmost == 3){
			printTree(topRoot);
			printf("\n");
		}
		return;
	}
	for(i = leftmost+1; i <= rightmost; i++){
		//make a root
		Ptr pr = (Node *)malloc(sizeof(Node));
		Ptr pl = (Node *)malloc(sizeof(Node));
		if(pr == NULL || pl == NULL){
			printf("out of space\n");
		}
		root->left = pl;
		root->right = pr;
		root->isNumber = 0;
		//operator
		for(j = 0; j < 1; j++){
			root->value.operator = operator[j];
			printf("cal Left: leftmost %d\t rightmost: %d\t i is: %d\n", leftmost, rightmost, i);
			calRes(data, leftmost, i-1, pl);
			printf("cal right: leftmost %d\t rightmost: %d\t i is: %d\n", leftmost, rightmost, i);
			calRes(data, i, rightmost, pr);
		}
		freeTree(pl);
		freeTree(pr);
	}
	return;
}
*/

void calRes(int *data, int n){
	int indexOfOperator[n-1];
	int i;
	for(i = 0; i < n-1; i++){
		indexOfOperator[i] = i;
	}
	permuteAndTree(indexOfOperator, 0, n-2, data);
}

void freeTree(Root r){
	if(r == NULL){
		printf("r is NULL!\n");
		return;
	}
	if(r->isNumber == 1){
		free(r);
		return;
	}else{
		freeTree(r->left);
		freeTree(r->right);
		free(r);
		return;
	}
}

void printTree(Ptr node){
	if(node->isNumber == 1){
		printf("\t%d", node->value.number);
	}else if(node->isNumber == 0){
		printf("(");
		printTree(node->left);
		printf("\t%c", node->value.operator);
		printTree(node->right);
		printf(")");
	}
}
