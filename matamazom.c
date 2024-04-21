#include "set.h"
#include "products.h"
#include "orders.h"
#include "matamazom.h"
#include "matamazom_print.h"
#include <math.h>

#define ERROR_RANGE 0.001
#define COMPLETE_PIECE 1
#define HALF_COMPLETE 0.5



struct Matamazom_t{
    AmountSet set_of_products;
    Set set_of_orders;
    int max_id;

};




static bool checkIfAmountIsValid(MatamazomAmountType amountType, double amount){

    double error=fabs(amount)-abs((int)amount);

    if(amountType==MATAMAZOM_INTEGER_AMOUNT){

        if( error<=ERROR_RANGE ||
            (COMPLETE_PIECE-ERROR_RANGE<=error && error<COMPLETE_PIECE)) {
            return true; }

        return false; }

    if(amountType==MATAMAZOM_HALF_INTEGER_AMOUNT ){
        if( error<=ERROR_RANGE ||
            (COMPLETE_PIECE-ERROR_RANGE<=error && error<COMPLETE_PIECE)){
            return true; }

        if( error <= HALF_COMPLETE+ERROR_RANGE &&
            error >= HALF_COMPLETE-ERROR_RANGE ){
            return true; }

        return false; }

    return true;
}



MatamazomResult mtmNewProduct(Matamazom matamazom, const unsigned int id,
                                const char *name,const double amount,
                                const MatamazomAmountType amountType,
                                const MtmProductData customData,
                                MtmCopyData copyData, MtmFreeData freeData,
                                MtmGetProductPrice prodPrice){

    if(matamazom==NULL || matamazom->set_of_products==NULL || name==NULL ||
       customData==NULL || copyData==NULL || freeData==NULL || prodPrice==NULL){
        return MATAMAZOM_NULL_ARGUMENT;}


    if( (*name<'0') || (*name>'9'&& *name<'A') ||
    (*name>'Z' && *name<'a') || (*name>'z') ){
        return MATAMAZOM_INVALID_NAME; }

    if(amount<0 || !checkIfAmountIsValid(amountType, amount)){
        return MATAMAZOM_INVALID_AMOUNT; }

    Product new_product = createProduct(id, name, amountType, customData,
                                        copyData, freeData, prodPrice);

    if(!new_product){
        return MATAMAZOM_OUT_OF_MEMORY; }

    AmountSetResult result= asRegister(matamazom->set_of_products,new_product);

    if(result==AS_ITEM_ALREADY_EXISTS){
        freeProduct(new_product);
        return  MATAMAZOM_PRODUCT_ALREADY_EXIST;}

    if (result==AS_OUT_OF_MEMORY){
        freeProduct(new_product);
        return MATAMAZOM_OUT_OF_MEMORY;}

    Product current_product = get_iterator(matamazom->set_of_products,id);
    asChangeAmount(matamazom->set_of_products,current_product, amount);
    freeProduct(new_product);
    return MATAMAZOM_SUCCESS;
}



MatamazomResult mtmChangeProductAmount(Matamazom matamazom,
                                        const unsigned int id,
                                        const double amount){

    if(matamazom==NULL || matamazom->set_of_orders==NULL){
        return MATAMAZOM_NULL_ARGUMENT;}

    Product current_product=get_iterator(matamazom->set_of_products,id);

    if(!current_product){
        return MATAMAZOM_PRODUCT_NOT_EXIST;}

    if(!checkIfAmountIsValid(getAmountType(current_product), amount)){
        return MATAMAZOM_INVALID_AMOUNT; }

    double current_amount= 0;
    asGetAmount(matamazom->set_of_products,current_product, &current_amount);

    double new_amount=current_amount + amount;
    if (new_amount<0){
        return MATAMAZOM_INSUFFICIENT_AMOUNT; }

    asChangeAmount(matamazom->set_of_products,current_product,amount);
    return MATAMAZOM_SUCCESS;
}



static MatamazomResult RemoveProductFromOrders(Set set_of_orders,
                                                Product ProductToRemove){
    order CurrentOrder=setGetFirst(set_of_orders);

    while(CurrentOrder){
        if(asContains(getProductListOfOrder(CurrentOrder), ProductToRemove)){
            asDelete(getProductListOfOrder(CurrentOrder), ProductToRemove);}

        CurrentOrder=setGetNext(set_of_orders);
    }
    return MATAMAZOM_SUCCESS;
}



MatamazomResult mtmClearProduct(Matamazom matamazom, const unsigned int id){

    if(matamazom==NULL || matamazom->set_of_orders==NULL ||
        matamazom->set_of_products==NULL){
        return  MATAMAZOM_NULL_ARGUMENT; }

    Product current_product=get_iterator(matamazom->set_of_products,id);

    if(current_product==NULL){
        return MATAMAZOM_PRODUCT_NOT_EXIST; }


    RemoveProductFromOrders(matamazom->set_of_orders,current_product);
    asDelete(matamazom->set_of_products,current_product);

    return MATAMAZOM_SUCCESS;
}



Matamazom matamazomCreate(){

    Matamazom newMatamazom=malloc(sizeof(*newMatamazom));
    if(!newMatamazom) {
        return NULL; }

    newMatamazom->set_of_orders= setCreate((copySetElements) copyOrder,
                                           (freeSetElements) freeOrder,
                                           (compareSetElements) compareOrders);

    if(!newMatamazom->set_of_orders){
        free(newMatamazom);
        return NULL; }

    newMatamazom->set_of_products= asCreate((CopyASElement) (copyProduct),
                                            (FreeASElement) (freeProduct),
                                            (CompareASElements) (compareProducts));

    if(!newMatamazom->set_of_products){
        matamazomDestroy(newMatamazom);
        return NULL; }

    newMatamazom->max_id=0;

    return newMatamazom;
}



void matamazomDestroy(Matamazom matamazom){
    if(matamazom==NULL){
        return; }

    setDestroy(matamazom->set_of_orders);
    asDestroy(matamazom->set_of_products);

    free(matamazom);
}



static bool CheckIfIsOrderValid(Matamazom matamazom, order order){
    AmountSet productList= getProductListOfOrder(order);
    Product Iterator=asGetFirst(productList);

    while(Iterator){
        double amountInOrder=0,amountInProduct=0;
        asGetAmount(productList,Iterator,&amountInOrder);
        asGetAmount(matamazom->set_of_products,Iterator,&amountInProduct);


        if(amountInOrder>amountInProduct) {
            return false; }
        Iterator=asGetNext(productList);
    }

    return true;
}


MatamazomResult mtmShipOrder(Matamazom matamazom, const unsigned int orderId) {

    if(!matamazom){
        return MATAMAZOM_NULL_ARGUMENT; }

    order orderToShip= findOrderById(matamazom->set_of_orders, orderId);
    if(!orderToShip){
        return MATAMAZOM_ORDER_NOT_EXIST; }

    if(!CheckIfIsOrderValid(matamazom,orderToShip)){
        return MATAMAZOM_INSUFFICIENT_AMOUNT; }

    Product Iterator=asGetFirst(getProductListOfOrder(orderToShip));

    while(Iterator){

        double amount = 0;
        asGetAmount(getProductListOfOrder(orderToShip), Iterator, &amount);
        updateIncome(getProduct(matamazom->set_of_products, get_id(Iterator)),
                     get_price_per_amount(Iterator, amount));
        asChangeAmount(matamazom->set_of_products,Iterator,-amount);
        Iterator=asGetNext(getProductListOfOrder(orderToShip));
    }

    mtmCancelOrder(matamazom,orderId);

    return MATAMAZOM_SUCCESS;
}



unsigned int mtmCreateNewOrder(Matamazom matamazom){

    if (matamazom==NULL || matamazom->set_of_orders==NULL)
        return 0;

    matamazom->max_id++;

    order newOrder = createOrder(matamazom->max_id);
    if (newOrder==NULL){
        return 0;
    }
    if(setAdd(matamazom->set_of_orders,newOrder)!=SET_SUCCESS){
        freeOrder(newOrder);
        return 0;
    }

    freeOrder(newOrder);

    return matamazom->max_id;

}



MatamazomResult mtmChangeProductAmountInOrder(Matamazom matamazom,
                                                const unsigned int orderId,
                                                const unsigned int productId,
                                                const double amount){

    if(matamazom==NULL || matamazom->set_of_orders==NULL
        || matamazom->set_of_products==NULL)
        return MATAMAZOM_NULL_ARGUMENT;

    order current_order = findOrderById(matamazom->set_of_orders, orderId);

    if(current_order==NULL)
        return MATAMAZOM_ORDER_NOT_EXIST;

    Product current_product=get_iterator(matamazom->set_of_products,productId);

    if(!current_product)
        return MATAMAZOM_PRODUCT_NOT_EXIST;


    if(!checkIfAmountIsValid(getAmountType(current_product), amount))
        return MATAMAZOM_INVALID_AMOUNT;

    double product_amount = 0;

    if(asGetAmount(getProductListOfOrder(current_order),
                   current_product, &product_amount) == AS_ITEM_DOES_NOT_EXIST){

        if(amount>0) {
            asRegister(getProductListOfOrder(current_order), current_product);
            asChangeAmount(getProductListOfOrder(current_order),
                           current_product, amount);

            return MATAMAZOM_SUCCESS; }

        else {
            return MATAMAZOM_SUCCESS; }
    }

    if(product_amount+amount<=0){
        asDelete(getProductListOfOrder(current_order), current_product);
        return MATAMAZOM_SUCCESS; }

    asChangeAmount(getProductListOfOrder(current_order), current_product, amount);

    return MATAMAZOM_SUCCESS;
}



MatamazomResult mtmCancelOrder(Matamazom matamazom, const unsigned int orderId){

    if(!matamazom || !matamazom->set_of_orders)
        return MATAMAZOM_NULL_ARGUMENT;


    order current_order = findOrderById(matamazom->set_of_orders, orderId);

    if(!current_order)
        return MATAMAZOM_ORDER_NOT_EXIST;


    setRemove(matamazom->set_of_orders,current_order);

    return MATAMAZOM_SUCCESS;
}



MatamazomResult mtmPrintOrder(Matamazom matamazom,
                              const unsigned int orderId, FILE *output){

    if(!matamazom || !matamazom->set_of_orders || !output )
        return MATAMAZOM_NULL_ARGUMENT;

    order current_order = findOrderById(matamazom->set_of_orders, orderId);

    if(!current_order)
        return MATAMAZOM_ORDER_NOT_EXIST;

    mtmPrintOrderHeading(orderId,output);

    AmountSet ProductList= getProductListOfOrder(current_order);
    Product iterator=asGetFirst(ProductList);
    double amount=0;
    double price=0;
    double total_price=0;

    while (iterator){

        asGetAmount(getProductListOfOrder(current_order), iterator, &amount);
        price=get_price_per_amount(iterator,amount);
        mtmPrintProductDetails(get_name(iterator),get_id(iterator),
                               amount,price,output);
        total_price=total_price+(price);
        iterator=asGetNext(ProductList);
    }

    mtmPrintOrderSummary(total_price,output);

    return MATAMAZOM_SUCCESS;
}

MatamazomResult mtmPrintInventory(Matamazom matamazom, FILE *output){

    if(!matamazom || !matamazom->set_of_products || !output )
        return MATAMAZOM_NULL_ARGUMENT;

    fprintf(output,"Inventory Status:\n");
    Product iterator=asGetFirst(matamazom->set_of_products);
    double amount=0;

    while (iterator){

        asGetAmount(matamazom->set_of_products,iterator,&amount);

        mtmPrintProductDetails(get_name(iterator), get_id(iterator), amount,
                        get_price_per_amount(iterator,1), output);

        iterator=asGetNext(matamazom->set_of_products);
    }

    return MATAMAZOM_SUCCESS;
}



MatamazomResult mtmPrintBestSelling(Matamazom matamazom, FILE *output){
    if(!matamazom || !output || !matamazom->set_of_products)
        return MATAMAZOM_NULL_ARGUMENT;

    Product Iterator=asGetFirst(matamazom->set_of_products);
    Product BestSeller=Iterator;

    double maximum=getIncome(Iterator);
    while(Iterator){

        if(getIncome(Iterator)>maximum){
            if(getIncome(Iterator)-maximum<=ERROR_RANGE){
                Iterator=asGetNext(matamazom->set_of_products);
                continue;
            }
            maximum=getIncome(Iterator);
            BestSeller=Iterator;
        }

        Iterator=asGetNext(matamazom->set_of_products);
    }

    fprintf(output,"Best Selling Product:\n");
    if(fabs(maximum)==0){
        fprintf(output,"none\n");
        return MATAMAZOM_SUCCESS;
    }

    double amount = 0;
    asGetAmount(matamazom->set_of_products,BestSeller,&amount);

    mtmPrintIncomeLine(get_name(BestSeller),
                       get_id(BestSeller),
                       getIncome(BestSeller),
                       output);

    return MATAMAZOM_SUCCESS;
}

MatamazomResult mtmPrintFiltered(Matamazom matamazom,
                                 MtmFilterProduct customFilter, FILE *output){

    if(!matamazom || !matamazom->set_of_products || !matamazom->set_of_orders
        || !output || !customFilter)
        return MATAMAZOM_NULL_ARGUMENT;

    Product Iterator=asGetFirst(matamazom->set_of_products);

    while(Iterator) {

        double amount = 0;
        asGetAmount(matamazom->set_of_products,Iterator,&amount);

        if(customFilter(get_id(Iterator),
                        get_name(Iterator),
                        amount,
                        get_custom_data(Iterator))){

            mtmPrintProductDetails(get_name(Iterator),
                                   get_id(Iterator),
                                   amount,
                             get_price_per_amount(Iterator,1),
                                   output);
        }

        Iterator=asGetNext(matamazom->set_of_products);
    }
    return MATAMAZOM_SUCCESS;
}


