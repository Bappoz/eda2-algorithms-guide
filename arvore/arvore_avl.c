#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// Queremos manter a altura da árvore O(log(n)), para isso, devemos balanceá-la
// Uma árvore é considerada balanceda caso:
	// | h(NodeLeft) - h(NodeRight) | <= 1  

typedef struct Node {
	int key;
	int height;
	struct Node* left;
	struct Node* right;
} Node;

int height(Node *node) {
	return node == NULL ? -1 : node->height;
}

void updateHeight(Node *node) {
	node->height = 1 + fmax(height(node->left), height(node->right));
}

// O valor negativo ou positivo na substração é relevante para determinarmos se está 
// pendendo para a direita ou para a esquerda
int getBalanceFactor(Node *node){
	if(node != NULL) return height(node->left) - height(node->right); 
	return 0;
}

bool isLeftPending(Node *node) {
	if(node == NULL) return false;
	return getBalanceFactor(node) > 0;
}

bool isRightPending(Node *node) {
	if(node == NULL) return false;
	return getBalanceFactor(node) < 0;
}

bool isBalanced(Node *node) {
	if(node == NULL) return false;
	return getBalanceFactor(node) <= 1 && getBalanceFactor(node) >= -1;
}

Node* createNode(int key) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->key = key;
	node->left = node->right = NULL;
	node->height = 0;
	return node;
}

// Right Rotation (RR)
Node* rightRotation(Node *node) {
	Node *leftSon = node->left;
	Node *rightGrandchild = leftSon->right;

	leftSon->right = node;
	node->left = rightGrandchild;

	updateHeight(node);
	updateHeight(leftSon);
	return leftSon;
}

// Left Rotation (LL)
Node* leftRotation(Node *node) {
	Node *rightSon = node->right;
	Node *leftGrandchild = rightSon->left;

	rightSon->left = node;
	node->right = leftGrandchild;

	updateHeight(node);
	updateHeight(rightSon);
	return rightSon;
}

Node *rebalance(Node *node) {
	// BalanceFactor of the ancestor
	int balance = getBalanceFactor(node);

	// Pendendo para a esquerda
	if(balance > 1) {
		if(getBalanceFactor(node->left) < 0) {
			node->left = leftRotation(node->left);
		}
		return rightRotation(node);
	}

	// Pendendo para a direita
	if(balance < -1) {
		if(getBalanceFactor(node->right) > 0) {
			node->right = rightRotation(node->right);
		}
		return leftRotation(node);
	}
	return node;
}

Node* insert(Node *node, int new_key) {
	if(node == NULL) return createNode(new_key);

	if(new_key < node->key) node->left = insert(node->left, new_key);
	else if(new_key > node->key) node->right = insert(node->right, new_key);
	else return node;

	updateHeight(node);
	return rebalance(node);
}

Node* findMin(Node *node) {
	while(node->left != NULL) node = node->left;
	return node;
}

Node* delete(Node *node, int key) {
	if(node == NULL) return NULL;

	if(key < node->key) {
		node->left = delete(node->left, key);
	} else if (key > node->key) {
		node->right = delete(node->right, key);
	} else {
		// Encontrou o Nó
		if(node->left == NULL || node->right == NULL) {
			Node *child = node->left ? node->left : node->right;
			if(child == NULL) {
				free(node);
				return NULL;
			} else {
				Node *tmp = child;
				free(node);
				return tmp;
			}
		} else {
			// Sucessor é o menor da direita
			Node *sucessor = findMin(node->right);
			node->key = sucessor->key;
		 	node->right = delete(node->right, sucessor->key);
		}
	}
	updateHeight(node);
	return rebalance(node);
}

Node* search(Node *node, int key) {
	if(node == NULL || node->key == key) return node;
	if(key < node->key) search(node->left, key);
	return search(node->right, key);
}

// Function to perform preorder traversal of AVL tree
void inOrder(struct Node* root)
{
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->key);
        inOrder(root->right);
    }
}

void freeTree(Node *root) {
	if(root != NULL) {
		freeTree(root->left);
		freeTree(root->right);
		free(root);
	}
}

int main() {
    Node *root = NULL;
    int values[] = {1, 2, 4, 5, 6, 3};
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++) {
        root = insert(root, values[i]);
    }

    printf("Inorder: ");
    inOrder(root);
    printf("\n");

    root = delete(root, 4);
    printf("Apos remover 4: ");
    inOrder(root);
    printf("\n");

    freeTree(root);
    return 0;
}