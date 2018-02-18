
//
//  main.c
//  CSC 173 Project 2.2
//
//  Created by Carolina I. Brager, Kenji Mulhall, Geraldine Marin-Zamora
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FAILED NULL
#define TRUE 1
#define FALSE 0
#define ENDM '$'

typedef int BOOLEAN;

typedef struct NODE *TREE;
struct NODE{
    char label;
    TREE leftMostChild, rightSibling;
    int indent;
};

struct Element {
    char value;
    struct Element *next;
};

int indent = 0;
int nodeIndent = 0;
void begin(char* input);
void createStack();
TREE func(char input, int prod, TREE tree);

//Functions for stack
void push(char x, struct Element **pL);
void printList(struct Element * L);
char peek(struct Element **pL);
void pop(struct Element **pL);
BOOLEAN isEmpty(struct Element **pL);

// Functions for table parser
BOOLEAN isTerminal(char x);
BOOLEAN isSynCat(char x);
BOOLEAN match(char c);
TREE* parse(TREE root, char x);
BOOLEAN matchingLabel(char label, char c);

//Functions for calc
char* result = "";
void calc(struct NODE *node);

// Functions for downward-recursive parser
TREE D();
TREE Ntail();
TREE N();
TREE S();
TREE E();
TREE Etail();
TREE T();
TREE Ttail();
TREE makeNode0(char x);
TREE makeNode1(char x, TREE t);
TREE makeNode2(char x, TREE t1, TREE t2);
TREE makeNode3(char x, TREE t1, TREE t2, TREE t3);
void inOrderPrint(struct NODE *node);
/*void preOrderPrint(struct NODE *node);*/
void preOrderPrint(struct NODE *node, int index);

TREE parseTree;
char *next;

int main( int argc, const char* argv[] ){
    FILE *file;
    file = fopen(/*Use your path here*/"/Users/kenjimulhall/Downloads/Test.txt" , "r");
    char buff[225];
    char* current = fgets(buff, 255, (FILE*)file);
    
    int lineNum = 1;
    while(strcmp(current, "-1") != 0){
        next = current;
        parseTree = S();
        printf("Expression %d\n", lineNum);
        if( parseTree==FAILED)
            printf("FAILED\n");
        
        preOrderPrint(parseTree, 0);
        printf(" \n");
        current = fgets(buff, 255, (FILE*)file);
        lineNum++;
        
        calc(parseTree);
    }
    
    
    
    begin("1+2$");
    
    
    return 0;
}

void preOrderPrint(struct NODE *node, int index){
    
    if(node == NULL)
        return;
    if(node->label == 't' || node->label == 'n' || node->label == 'e' || node->label == '~'){
        if(node->leftMostChild != NULL && node->leftMostChild->label != '~')
            index = index -1;
    }
    else{
        printf("\n");
        for(int i = 0; i < index; i++){
            printf("    ");
        }
        printf("%c", node->label);}
    preOrderPrint(node->leftMostChild, index + 1);
    preOrderPrint(node->rightSibling, index);
}

TREE makeNode0(char x){
    TREE root;
    
    root = (TREE) malloc(sizeof(struct NODE));
    root->label = x;
    root-> leftMostChild = NULL;
    root-> rightSibling = NULL;
    //printf("Root Label: %c\n",root->label);
    return root;
}

TREE makeNode1(char x, TREE t){
    TREE root;
    root = makeNode0(x);
    root->leftMostChild = t;
    return root;
}

TREE makeNode2(char x, TREE t1, TREE t2){
    
    TREE root;
    root = makeNode1(x, t1);
    t1->rightSibling = t2;
    return root;
}

TREE makeNode3(char x, TREE t1, TREE t2, TREE t3){
    
    TREE root;
    root = makeNode1(x, t1);
    t1->rightSibling = t2;
    t2->rightSibling = t3;
    return root;
}

TREE D(){
    if(*next == '0'){
        next++;
        /*printf("Next: %c\n", *next);*/
        return makeNode1('D', makeNode0('0'));}
    else if(*next == '1'){
        next++;
        /*printf("Next: %c\n", *next);*/
        return makeNode1('D', makeNode0('1'));}
    else if(*next == '2'){
        next++;
        /*printf("Next: %c\n", *next);*/
        return makeNode1('D', makeNode0('2'));}
    else if(*next == '3'){
        next++;
        /*printf("Next: %c\n", *next);*/
        return makeNode1('D', makeNode0('3'));}
    else if(*next == '4'){
        next++;
        /*printf("Next: %c\n", *next);*/
        return makeNode1('D', makeNode0('4'));}
    else if(*next == '5'){
        next++;
        /*printf("Next: %c\n", *next);*/
        return makeNode1('D', makeNode0('5'));}
    else if(*next == '6'){
        next++;
        /*printf("Next: %c\n", *next);*/
        return makeNode1('D', makeNode0('6'));}
    else if(*next == '7'){
        next++;
        /*printf("Next: %c\n", *next);*/
        return makeNode1('D', makeNode0('7'));}
    else if(*next == '8'){
        next++;
        /*printf("Next: %c\n", *next);*/
        return makeNode1('D', makeNode0('8'));}
    else if(*next == '9'){
        next++;
        /*printf("Next: %c\n", *next);*/
        return makeNode1('D', makeNode0('9'));}
    else
        return FAILED;
}

TREE N(){
    TREE first, second;
    first = D();
    if(first != FAILED){
        second = Ntail();
        if(second == FAILED)
            return FAILED;
        else
            return makeNode2('N', first, second);
    }
    else
        return FAILED;
}


TREE Ntail(){
    TREE first;
    first = N();
    if(first != FAILED){
        return makeNode1('N', first);
    }
    else
        return makeNode0('~');
}

TREE F(){
    TREE first;
    if(*next == '('){
        next++;
        /*printf("Next: %c\n", *next);*/
        first = E();
        if(first != FAILED && *next == ')'){
            next++;
            /*printf("Next: %c\n", *next);*/
            return makeNode3('F', makeNode0('('), first, makeNode0(')'));
        }
        else
            return FAILED;
    }
    else{
        first = N();
        if(first == FAILED)
            return FAILED;
        else
            return makeNode1('F', first);
    }
    
}

TREE T(){
    TREE first, second;
    first = F();
    if(first != FAILED){
        second = Ttail();
        if(second == FAILED)
            return FAILED;
        else
            return makeNode2('T', first, second);
    }
    else
        return FAILED;
}

TREE Ttail(){
    TREE first;
    if(*next == '*'){ /*follow production 1*/
        next++;
        /*printf("Next: %c\n", *next);*/
        first = T();
        if(first == FAILED)
            return FAILED;
        else
            return makeNode2('t', makeNode0('*'), first);
    }
    else if(*next == '/'){ /*follow production 1*/
        next++;
        /*printf("Next: %c\n", *next);*/
        first = T();
        if(first == FAILED)
            return FAILED;
        else
            return makeNode2('t', makeNode0('/'), first);
    }
    else{ /*follow production 3*/
        return makeNode1('t', makeNode0('~'));
    }
}

TREE E(){
    TREE first, second;
    first = T();
    if(first != FAILED){
        second = Etail();
        if(second == FAILED)
            return FAILED;
        else
            return makeNode2('E', first, second);
    }
    else
        return FAILED;
}

TREE Etail(){
    TREE first;
    if(*next == '+'){ /*follow production 1*/
        next++;
        /*printf("Next: %c\n", *next);*/
        first = E();
        if(first == FAILED)
            return FAILED;
        else
            return makeNode2('e', makeNode0('+'), first);
    }
    else if(*next == '-'){ /*follow production 1*/
        next++;
        /*printf("Next: %c\n", *next);*/
        first = E();
        if(first == FAILED)
            return FAILED;
        else
            return makeNode2('e', makeNode0('-'), first);
    }
    else //Otherwise it's null
        return makeNode1('e', makeNode0('~'));
}

TREE S(){
    TREE first;
    first = E();
    if(first != FAILED && *next == ENDM)
        return makeNode2('S', first, makeNode0(ENDM));
    else
        return FAILED;
    
}

BOOLEAN match(char c){
    if(c==*next){
        return TRUE;
    }
    else{
        return FALSE;
    }
}


/*
 *
 * Code for the stack starts here.
 *
 */


void push(char x, struct Element **pL)
{
    if(*pL == NULL)
    {
        (*pL) = malloc(sizeof(*pL));
        (*pL)->value = x;
        (*pL)->next = NULL;
    }
    else
    {
        struct Element *temp = malloc(sizeof(*pL));
        temp->value = x;
        temp->next = *pL;
        (*pL) = temp;
    }
}

void printList(struct Element * L)
{
    printf("%c\n",L->value);
    if (L->next == NULL)
        return;
    printList(L->next);
}


BOOLEAN isEmpty(struct Element **pL)
{
    if(*pL == NULL)
        return TRUE;
    else
        return FALSE;
}

char peek(struct Element **pL)
{
    if(!isEmpty(pL))
    {
        return (*pL)->value;
    }
    else
    {
        return '\0';
    }
}

void pop(struct Element **pL)
{
    if(!isEmpty(pL))
    {
        struct Element *temp = (*pL);
        (*pL) = (*pL)->next;
        free(temp);
    }
    else
    {
        printf("Error: Stack is Empty");
        return;
    }
}



/*
 *
 *  Code for the table starts here.
 *
 */

//#include "parse.h"

struct Element *stack;

/*TREE parse{
 }*/

struct Element *stack;

void begin(char* input) {
    
    //TABLE IS FOR WHEN SYNCAT ON STACK
    int table[252][252];
    //S-> E$ so ( or digit
    table[(int)('S')][(int)('(')] = 1;
    table[(int)('S')][(int)('0')] = 1;
    table[(int)('S')][(int)('1')] = 1;
    table[(int)('S')][(int)('2')] = 1;
    table[(int)('S')][(int)('3')] = 1;
    table[(int)('S')][(int)('4')] = 1;
    table[(int)('S')][(int)('5')] = 1;
    table[(int)('S')][(int)('6')] = 1;
    table[(int)('S')][(int)('7')] = 1;
    table[(int)('S')][(int)('8')] = 1;
    table[(int)('S')][(int)('9')] = 1;
    
    //E-> T so ( or digit
    table[(int)('E')][(int)('(')] = 2;
    table[(int)('E')][(int)('0')] = 2;
    table[(int)('E')][(int)('1')] = 2;
    table[(int)('E')][(int)('2')] = 2;
    table[(int)('E')][(int)('3')] = 2;
    table[(int)('E')][(int)('4')] = 2;
    table[(int)('E')][(int)('5')] = 2;
    table[(int)('E')][(int)('6')] = 2;
    table[(int)('E')][(int)('7')] = 2;
    table[(int)('E')][(int)('8')] = 2;
    table[(int)('E')][(int)('9')] = 2;
    
    //e-> + or - CONSUME INPUT
    table[(int)('e')][(int)('+')] = 3;
    table[(int)('e')][(int)('-')] = 4;
    
    //e -> epsilon, just pop()
    table[(int)('e')][(int)('(')] = 5;
    table[(int)('e')][(int)(')')] = 5;
    table[(int)('e')][(int)('*')] = 5;
    table[(int)('e')][(int)('/')] = 5;
    table[(int)('e')][(int)('0')] = 5;
    table[(int)('e')][(int)('1')] = 5;
    table[(int)('e')][(int)('2')] = 5;
    table[(int)('e')][(int)('3')] = 5;
    table[(int)('e')][(int)('4')] = 5;
    table[(int)('e')][(int)('5')] = 5;
    table[(int)('e')][(int)('6')] = 5;
    table[(int)('e')][(int)('7')] = 5;
    table[(int)('e')][(int)('8')] = 5;
    table[(int)('e')][(int)('9')] = 5;
    
    //T -> Ft so ( or digit next
    table[(int)('T')][(int)('(')] = 6;
    table[(int)('T')][(int)('0')] = 6;
    table[(int)('T')][(int)('1')] = 6;
    table[(int)('T')][(int)('2')] = 6;
    table[(int)('T')][(int)('3')] = 6;
    table[(int)('T')][(int)('4')] = 6;
    table[(int)('T')][(int)('5')] = 6;
    table[(int)('T')][(int)('6')] = 6;
    table[(int)('T')][(int)('7')] = 6;
    table[(int)('T')][(int)('8')] = 6;
    table[(int)('T')][(int)('9')] = 6;
    
    //t-> * or / CONSUME INPUT
    table[(int)('t')][(int)('*')] = 7;
    table[(int)('t')][(int)('/')] = 8;
    
    //t -> epsilon, just pop()
    table[(int)('t')][(int)('(')] = 9;
    table[(int)('t')][(int)('0')] = 9;
    table[(int)('t')][(int)('1')] = 9;
    table[(int)('t')][(int)('2')] = 9;
    table[(int)('t')][(int)('3')] = 9;
    table[(int)('t')][(int)('4')] = 9;
    table[(int)('t')][(int)('5')] = 9;
    table[(int)('t')][(int)('6')] = 9;
    table[(int)('t')][(int)('7')] = 9;
    table[(int)('t')][(int)('8')] = 9;
    table[(int)('t')][(int)('9')] = 9;
    table[(int)('t')][(int)(')')] = 9;
    table[(int)('t')][(int)('+')] = 9;
    table[(int)('t')][(int)('-')] = 9;
    
    //F-> ( CONSUME INPUT
    table[(int)('F')][(int)('(')] = 10;
    
    //F-> N (digit next)
    table[(int)('F')][(int)('0')] = 11;
    table[(int)('F')][(int)('1')] = 11;
    table[(int)('F')][(int)('2')] = 11;
    table[(int)('F')][(int)('3')] = 11;
    table[(int)('F')][(int)('4')] = 11;
    table[(int)('F')][(int)('5')] = 11;
    table[(int)('F')][(int)('6')] = 11;
    table[(int)('F')][(int)('7')] = 11;
    table[(int)('F')][(int)('8')] = 11;
    table[(int)('F')][(int)('9')] = 11;
    
    //N -> D or n
    table[(int)('N')][(int)('0')] = 12;
    table[(int)('N')][(int)('1')] = 12;
    table[(int)('N')][(int)('2')] = 12;
    table[(int)('N')][(int)('3')] = 12;
    table[(int)('N')][(int)('4')] = 12;
    table[(int)('N')][(int)('5')] = 12;
    table[(int)('N')][(int)('6')] = 12;
    table[(int)('N')][(int)('7')] = 12;
    table[(int)('N')][(int)('8')] = 12;
    table[(int)('N')][(int)('9')] = 12;
    
    //n -> N
    table[(int)('n')][(int)('0')] = 13;
    table[(int)('n')][(int)('1')] = 13;
    table[(int)('n')][(int)('2')] = 13;
    table[(int)('n')][(int)('3')] = 13;
    table[(int)('n')][(int)('4')] = 13;
    table[(int)('n')][(int)('5')] = 13;
    table[(int)('n')][(int)('6')] = 13;
    table[(int)('n')][(int)('7')] = 13;
    table[(int)('n')][(int)('8')] = 13;
    table[(int)('n')][(int)('9')] = 13;
    
    //n -> epsilon (not N)
    table[(int)('n')][(int)('(')] = 14;
    table[(int)('n')][(int)(')')] = 14;
    table[(int)('n')][(int)('+')] = 14;
    table[(int)('n')][(int)('-')] = 14;
    /*
     table[(int)('n')][(int)('0')] = 14;
     table[(int)('n')][(int)('1')] = 14;
     table[(int)('n')][(int)('2')] = 14;
     table[(int)('n')][(int)('3')] = 14;
     table[(int)('n')][(int)('4')] = 14;
     table[(int)('n')][(int)('5')] = 14;
     table[(int)('n')][(int)('6')] = 14;
     table[(int)('n')][(int)('7')] = 14;
     table[(int)('n')][(int)('8')] = 14;
     table[(int)('n')][(int)('9')] = 14;
     */
    
    //D -> any 0-9 CONSUME INPUT
    table[(int)('D')][(int)('0')] = 15;
    table[(int)('D')][(int)('1')] = 15;
    table[(int)('D')][(int)('2')] = 15;
    table[(int)('D')][(int)('3')] = 15;
    table[(int)('D')][(int)('4')] = 15;
    table[(int)('D')][(int)('5')] = 15;
    table[(int)('D')][(int)('6')] = 15;
    table[(int)('D')][(int)('7')] = 15;
    table[(int)('D')][(int)('8')] = 15;
    table[(int)('D')][(int)('9')] = 15;
    
    
    /*
     int table[252][252];
     table[(int)('S')][(int)('(')] = 1;
     table[(int)('S')][(int)('N')] = 1;
     table[(int)('E')][(int)('(')] = 2;
     table[(int)('E')][(int)('N')] = 2;
     table[(int)('e')][(int)(')')] = 5;
     table[(int)('e')][(int)('+')] = 3;
     table[(int)('e')][(int)('-')] = 4;
     table[(int)('e')][(int)('~')] = 5;
     table[(int)('T')][(int)('(')] = 6;
     table[(int)('T')][(int)('N')] = 6;
     table[(int)('t')][(int)(')')] = 9;
     table[(int)('t')][(int)('+')] = 9;
     table[(int)('t')][(int)('-')] = 9;
     table[(int)('t')][(int)('*')] = 7;
     table[(int)('t')][(int)('/')] = 8;
     table[(int)('t')][(int)('~')] = 9;
     table[(int)('F')][(int)('(')] = 10;
     table[(int)('F')][(int)('N')] = 11;
     */
    
    TREE root;
    push('S', &stack);
    root = makeNode0('S');
    pop(&stack);
    push('$', &stack);
    push('E', &stack);
    root->leftMostChild = makeNode0('E');
    root->leftMostChild->rightSibling = makeNode0('$');
    TREE *ptr = &root; //tree pointer to where to insert new node
    char x = peek(&stack);
    while(x != '$' && (isSynCat(x) || isTerminal(x)))
    {
        //printf("Input: %c\n");
        *ptr = func(*input, table[(int) x][(int) *input], root);
        if(ptr == NULL)
        {
            break;
        }
        else
        {
            
        }
        x = peek(&stack);
        if(isTerminal(x) && (x == *input))
        {
            input++;
            pop(&stack);
        }
        x = peek(&stack);
        printf("Char: %c\n", x);
        printf("Current Input: %c\n", *input);
        printList(stack);
        preOrderPrint(root,0);
    }
    if(*input == '$')
    {
        printf("Failed.");
        printf("Input Left: %c\n", *input);
    }
    preOrderPrint(root, 0);
    printf("\n");
}

TREE* parse(TREE root, char x)
{
    TREE* result = NULL;
    if(matchingLabel(root->label, x) && root->leftMostChild == NULL)
    {
        result = &root;
        return result;
    }
    else if(root->leftMostChild != NULL)
    {
        result = parse(root->leftMostChild, x);
    }
    else if(!result && root->rightSibling != NULL)
    {
        result = parse(root->rightSibling, x);
    }
    return result;
}

/*label = label of node, c = children*/
BOOLEAN matchingLabel(char label, char c){
    if(label == 'D')
    {
        if(c == '0' || c == '1' || c == '2' || c == '3' ||
           c == '4' || c == '5' || c == '6' || c == '7' ||
           c == '8' || c == '9')
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else if(label == 'N' && c == 'D')
    {
        return TRUE;
    }
    else if(label == 'F' && (c == 'N' || c == '('))
    {
        return TRUE;
    }
    else if(label == 'T' && (c == 'F' || c == 't'))
    {
        return TRUE;
    }
    else if(label == 't' && (c == '*' || c == '/'))
    {
        return TRUE;
    }
    else if(label == 'E' && c == 'T')
    {
        return TRUE;
    }
    else if(label == 'e' && (c == '+' || c == '-'))
    {
        return TRUE;
    }
    else if(label == 'S' && (c == 'E' || c == '~'))
    {
        return TRUE;
    }
    return FALSE;
}

TREE func(char input, int prod, TREE tree)
{
    TREE temp = NULL;
    char peeked = stack->value;
    TREE* ptr;
    TREE* ptr2 = &tree;
    if(prod == 1) //S-> E$ so ( or digit
    {
        pop(&stack);
        push('$', &stack);
        push('E', &stack);
        
        ptr = parse(*ptr2, 'E');
        temp = makeNode2((*ptr)->label, makeNode0('E'), makeNode0('$'));
        
        (*ptr) = temp;
        //what do we do with tree passed in?
    }
    else if(prod == 2) //E-> T so ( or digit
    {
        pop(&stack);
        push('e', &stack);
        push('T', &stack);
        
        ptr = parse(*ptr2, 'T');
        temp = makeNode2((*ptr)->label, makeNode0('T'), makeNode0('e'));
        
        (*ptr) = temp;
    }
    else if(prod == 3) //e-> + CONSUME INPUT
    {
        pop(&stack);
        push('e', &stack);
        push('T', &stack);
        push('+', &stack);
        
        ptr = parse(*ptr2, '+');
        temp = makeNode3((*ptr)->label, makeNode0('+'), makeNode0('T'), makeNode0('e'));
        
        (*ptr) = temp;
    }
    else if(prod == 4) //e-> - CONSUME INPUT
    {
        pop(&stack);
        push('e', &stack);
        push('T', &stack);
        push('-', &stack);
        
        ptr = parse(*ptr2, '-');
        temp = makeNode3((*ptr)->label, makeNode0('-'), makeNode0('T'), makeNode0('e'));
        
        (*ptr) = temp;
    }
    else if(prod == 5) //e -> epsilon, just pop()
    {
        pop(&stack);
        temp = makeNode0(peeked);
    }
    else if(prod == 6) //T -> Ft so ( or digit next
    {
        pop(&stack);
        push('t', &stack);
        push('F', &stack);
        
        parse(*ptr2, 'F');
        temp = makeNode2((*ptr)->label, makeNode0('F'), makeNode0('t'));
        
        (*ptr) = temp;
    }
    else if(prod == 7) //t-> * CONSUME INPUT
    {
        pop(&stack);
        push('t', &stack);
        push('F', &stack);
        push('*', &stack);
        
        ptr = parse(*ptr2, '*');
        temp = makeNode3((*ptr)->label, makeNode0('*'), makeNode0('F'), makeNode0('t'));
        
        (*ptr) = temp;
    }
    else if(prod == 8) //t-> / CONSUME INPUT
    {
        pop(&stack);
        push('t', &stack);
        push('F', &stack);
        push('/', &stack);
        
        ptr = parse(*ptr2, '/');
        temp = makeNode3((*ptr)->label, makeNode0('/'), makeNode0('F'), makeNode0('t'));
        
        (*ptr) = temp;
    }
    else if(prod == 9) //t -> epsilon, just pop()
    {
        pop(&stack);
        temp = makeNode0(peeked);
    }
    else if(prod == 10) //F-> ( CONSUME INPUT
    {
        pop(&stack);
        push(')', &stack);
        push('E', &stack);
        push('(', &stack);
        
        ptr = parse(*ptr2, '(');
        temp = makeNode3((*ptr)->label, makeNode0('('), makeNode0('E'), makeNode0(')'));
        
        (*ptr) = temp;
    }
    else if(prod == 11) //F-> N (digit next)
    {
        pop(&stack);
        push('N', &stack);
        
        ptr = parse(*ptr2, 'N');
        temp = makeNode1((*ptr)->label, makeNode0('N'));
        
        (*ptr) = temp;
    }
    else if(prod == 12) //N -> D or n
    {
        pop(&stack);
        push('n', &stack);
        push('D', &stack);
        
        ptr = parse(*ptr2, 'D');
        temp = makeNode2((*ptr)->label, makeNode0('D'), makeNode0('n'));
        
        (*ptr) = temp;
    }
    else if(prod == 13) //n -> N
    {
        pop(&stack);
        push('N', &stack);
        
        ptr = parse(*ptr2, 'N');
        temp = makeNode1((*ptr)->label, makeNode0('N'));
        
        (*ptr) = temp;
    }
    else if(prod == 14) //n -> epsilon (not N)
    {
        pop(&stack);
        temp = makeNode0(peeked);
    }
    else if(prod == 15) //D -> any 0-9 CONSUME INPUT
    {
        if (input == '0' || input == '1' || input == '2' || input == '3' ||
            input == '4' || input == '5' || input == '6' || input == '7' ||
            input == '8' || input == '9')
        {
            pop(&stack);
            push(input, &stack);
            temp = makeNode1((*ptr)->label, makeNode0(input));
            ptr = parse(*ptr2, input);
            (*ptr) = temp;
        }
        else
        {
            printf("fail\n");
        }
    }
    return temp;
}

void calc(struct NODE *node)
{
    if(node == NULL){
        
        return;
    }
    else{
        if(node->leftMostChild == NULL){
            result = result + node->label;
        }
        else{
        }
    }
    calc(node->leftMostChild);
    calc(node->rightSibling);
}

BOOLEAN isTerminal(char x) {
    if(x == '0' || x == '1' || x == '2' || x == '3' || x == '4' || x == '5' ||
       x == '6' || x == '7' || x == '8' || x == '9' || x == '+' || x == '-' ||
       x == '*' || x == '/' || x == '(' || x == ')' || x == '$')
        return TRUE;
    else
        return FALSE;
}

BOOLEAN isSynCat(char x) {
    if(x == 'S' || x == 'E' || x == 'T' || x == 'F' || x == 'D' ||  x == 'N' ||
       x == 'e' || x == 't' || x == 'n')
        return TRUE;
    else
        return FALSE;
}
