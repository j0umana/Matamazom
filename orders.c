#include "orders.h"
#include "set.h"

struct order_t{

    unsigned int order_id;
    AmountSet productList;

};



order createOrder(unsigned int id){

    order newOrder=malloc(sizeof(*newOrder));
    if(!newOrder)
        return NULL;


    newOrder->productList= asCreate((CopyASElement) copyProduct,
                                    (FreeASElement) freeProduct,
                                    (CompareASElements) compareProducts);

    if(!newOrder->productList){
        free(newOrder);
        return NULL;
    }

    newOrder->order_id=id;

    return newOrder;
}



void freeOrder(order order){

    if(!order)
        return;

    asDestroy(order->productList);
    free(order);
}



order findOrderById(Set set, unsigned int orderId){

    order setIterator=setGetFirst(set);

    while(setIterator){

        if(setIterator->order_id==orderId) {
            return setIterator;
        }

        setIterator=setGetNext(set);
    }

    return NULL;
}



AmountSet getProductListOfOrder(order order){
    if(!order)
        return NULL;

    return order->productList;
}



order copyOrder(order orderToCopy){

    order newOrder=malloc(sizeof(*newOrder));
    if(!newOrder)
        return NULL;

    newOrder->order_id=orderToCopy->order_id;
    newOrder->productList=asCopy(orderToCopy->productList);

    if(!newOrder->productList){
        free(newOrder);
        return NULL;
    }

    return newOrder;
}



int compareOrders(order order1, order order2){

    return (order1->order_id)-(order2->order_id);
}





