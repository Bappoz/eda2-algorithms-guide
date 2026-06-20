#include <stdio.h>
#include <stdlib.h>
#include "arvore_vp.h"

typedef enum ColorType {
	Red, Black
} ColorType;

typedef struct RedBlackNode {
	ElementType Element;
	ColorType Color;
	RedBlackTree Left;
	RedBlackTree Right;
} Node;

// Técnica nó sentinela
static Position NullNode = NULL; // --> Necessita de inicialização

RedBlackTree Initialize(void) {
	RedBlackTree T;

	if(NullNode == NULL) {
		NullNode = malloc(sizeof(struct RedBlackNode));
		if(NullNode == NULL) return NULL;
		NullNode->Color = Black;
		NullNode->Left = NullNode->Right = NullNode;
		NullNode->Element = 12345;
	}

	T = malloc(sizeof(struct RedBlackNode));
	if(T == NULL) return NULL;
	T->Element = -10000;
	T->Left = T->Right = NullNode;
	T->Color = Black;
	return T;
}

// Print Redblacktree functions
void Output(ElementType Element){
	printf("%d\n", Element);
}

static void DoPrint(RedBlackTree T) {
	if (T != NullNode) {
		DoPrint(T->Left);
		Output(T->Element);
		DoPrint(T->Right);
	}
}

void PrintTree(RedBlackTree T) {
	DoPrint(T->Right);
}

static RedBlackTree MakeEmptyRec(RedBlackTree T) {
	if(T != NullNode) {
		MakeEmptyRec(T->Left);
		MakeEmptyRec(T->Right);
		free(T);
	}
	return NullNode;
}

RedBlackTree MakeEmpty(RedBlackTree T){
	T->Right = MakeEmptyRec(T->Right);
	return T;
}

Position Find(ElementType X, RedBlackTree T) {
	if(T == NullNode) return NullNode;
	if(X < T->Element) {
		return Find(X, T->Left);
	} else if (X > T->Element) {
		return Find(X, T->Right);
	} 
	return T;
}

Position FindMin(RedBlackTree T) {
	T = T->Right;
	while (T->Left != NullNode) {
		T = T->Left;
	}
	return T;
}

Position FindMax(RedBlackTree T) {
	T = T->Right;
	while (T->Right != NullNode) {
		T = T->Right;
	}
	return T;
}

static Position SingleRotateWithLeft()


