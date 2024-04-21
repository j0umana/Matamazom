#include <stdio.h>
#include "amount_set.h"
#include <stdlib.h>

#define EMPTY 0

typedef struct node {
    ASElement Element;
    double Amount;
    struct node *Next;
} *Node;

struct AmountSet_t{
    Node head;
    CopyASElement CopyElement;
    FreeASElement FreeElement;
    CompareASElements CompareElements;
    Node  Iterator;
};



AmountSet asCreate(CopyASElement copyElement, FreeASElement freeElement,
                   CompareASElements compareElements){

    if(copyElement==NULL || freeElement==NULL || compareElements==NULL)
        return NULL;

    AmountSet set=malloc(sizeof(*set));
    if(!set)
        return NULL;

    set -> head=NULL;
    set -> Iterator=NULL;
    set -> CopyElement=copyElement;
    set -> FreeElement=freeElement;
    set -> CompareElements=compareElements;
    return set;
}



static Node createNewNode(AmountSet set, ASElement Element) {

    if (set == NULL || Element == NULL)
        return NULL;

    Node new_node = malloc(sizeof(*new_node));

    if (!new_node)
        return NULL;

    new_node->Element = set->CopyElement(Element);
    new_node->Amount = EMPTY;
    new_node->Next = NULL;
    return new_node;

}



AmountSetResult asRegister(AmountSet set, ASElement element) {

    if (!set || !element)
        return AS_NULL_ARGUMENT;

    if (asContains(set, element) == true)
        return AS_ITEM_ALREADY_EXISTS;

    Node newNode= createNewNode(set, element);

    if(!newNode)
        return AS_OUT_OF_MEMORY;

    newNode->Amount=EMPTY;

    if(!set->head){
        set->head=newNode;
        newNode->Next=NULL;
        return AS_SUCCESS;
    }

    Node previousNode=set->head;
    set->Iterator=set->head;

    while(set->Iterator){
        if(set->CompareElements(set->Iterator->Element,newNode->Element)>0){
            if(set->Iterator==set->head){
                set->head=newNode;
                newNode->Next=previousNode;
                return AS_SUCCESS;}

            else {
                previousNode->Next=newNode;
                newNode->Next=set->Iterator;
                return AS_SUCCESS;}
        }
        previousNode=set->Iterator;
        set->Iterator=set->Iterator->Next;
    }

    previousNode->Next=newNode;
    newNode->Next=NULL;
    return AS_SUCCESS;
}



static Node findLastNode(AmountSet set){
    if(!set->head)
        return NULL;

    asGetFirst(set);
    while(set->Iterator->Next) {
        set->Iterator = set->Iterator->Next;
    }
    return set->Iterator;
}



AmountSet asCopy(AmountSet set){

    if(set==NULL)
        return NULL;

    AmountSet newSet=asCreate(set->CopyElement,
                              set->FreeElement,set->CompareElements);
    if(newSet==NULL)
        return NULL;

    if(set->head==NULL)
        return newSet;

    asGetFirst(set);

    Node newNode= createNewNode(set, set->Iterator->Element);
    if(newNode==NULL){
        asDestroy(newSet);
        return NULL;
    }

    newNode->Amount=set->head->Amount;
    newSet->head=newNode;
    newSet->Iterator=newSet->head;

    set->Iterator=set->Iterator->Next;

    while (set->Iterator != NULL){

        newSet->Iterator=newSet->Iterator->Next;

        AmountSetResult Result=asRegister(newSet,set->Iterator->Element);
        if(Result!=AS_SUCCESS) {
            asDestroy(newSet);
            return NULL;
        }
        newSet->Iterator= findLastNode(newSet);
        newSet->Iterator->Amount=set->Iterator->Amount;
        set->Iterator=set->Iterator->Next;
    }

    return newSet;
}



int asGetSize(AmountSet set){

    if(set==NULL)
        return -1;

    Node OriginalIterator=set->Iterator;

    int size=EMPTY;

    AS_FOREACH(ASElement, iterator, set){
        size++;
    }

    set->Iterator=OriginalIterator;

    return size;
}



bool asContains(AmountSet set, ASElement element){
    if(set==NULL || element==NULL )
        return false;

    if(asGetSize(set) == 0)
        return false;

    Node tmp = set->head;

    while (tmp!=NULL){

        if(set->CompareElements(tmp->Element,element)==0){
            return true;
        }
        tmp=tmp->Next;
    }

    return false;
}





ASElement asGetNext(AmountSet set){

    if(set==NULL || set->head==NULL)
        return NULL;

    if(set->Iterator->Next==NULL)
        return NULL;

    set->Iterator = set->Iterator->Next;
    return set->Iterator->Element;
}






ASElement asGetFirst(AmountSet set){

    if(set == NULL)
        return NULL;

    if(set->head == NULL){
        set->Iterator=NULL;
        return NULL;
    }

    set->Iterator=set->head;
    return set->head->Element;
}



AmountSetResult asGetAmount(AmountSet set, ASElement element, double *outAmount) {

    if (set == NULL || element == NULL || outAmount == NULL)
        return AS_NULL_ARGUMENT;


    if (asContains(set, element) == false)
        return AS_ITEM_DOES_NOT_EXIST;


    Node tmp = set->head;

    while (tmp != NULL) {

        if (set->CompareElements(tmp->Element, element) == 0) {
            *outAmount = tmp->Amount;
            return AS_SUCCESS;
        }
        tmp = tmp->Next;
    }

    return AS_ITEM_DOES_NOT_EXIST;
}



AmountSetResult asChangeAmount(AmountSet set,
                               ASElement element, const double amount){

    if(set == NULL ||element == NULL)
        return AS_NULL_ARGUMENT;

    if(asContains(set,element)==false)
        return AS_ITEM_DOES_NOT_EXIST;

    Node temp=set->head;
    while(temp){
        if(set->CompareElements(temp->Element,element)==0){
            double tempAmount=temp->Amount+amount;

            if(tempAmount<0){
                return AS_INSUFFICIENT_AMOUNT;
            }

            temp->Amount+=amount;
            return AS_SUCCESS;
        }
        temp=temp->Next;
    }

    return AS_ITEM_DOES_NOT_EXIST;

}



AmountSetResult asDelete(AmountSet set, ASElement element){
    if( set==NULL || element==NULL )
        return AS_NULL_ARGUMENT;

    if(asContains(set,element)==false || !set->head){
        return AS_ITEM_DOES_NOT_EXIST;
    }

    set->Iterator=set->head;
    Node prev=set->head;

    if(set->CompareElements(set->head->Element,element)==0){
        set->Iterator=set->Iterator->Next;
        set->head=set->Iterator;
        set->FreeElement(prev->Element);
        free(prev);
        return AS_SUCCESS;
    }

    asGetNext(set);

    while(set->CompareElements(set->Iterator->Element,element)!=0){
        prev=set->Iterator;
        asGetNext(set);
    }

    if(set->Iterator!=NULL && prev!=NULL){
        prev->Next=set->Iterator->Next;
        set->FreeElement(set->Iterator->Element);
        free(set->Iterator);
        return AS_SUCCESS;
    }

    return AS_ITEM_DOES_NOT_EXIST;

}



void asDestroy(AmountSet set){

    if(set==NULL)
        return;

    asClear(set);
    free(set);

}



AmountSetResult asClear(AmountSet set){
    if(set==NULL)
        return AS_NULL_ARGUMENT;

    asGetFirst(set);

    while(set->Iterator){
        asDelete(set,set->Iterator->Element);
    }

    return AS_SUCCESS;
}

