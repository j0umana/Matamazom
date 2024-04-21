#include "products.h"


struct Product_t{

    unsigned int id;
    char *name;

    MatamazomAmountType amountType;
    MtmProductData customData;

    MtmCopyData copyData;
    MtmFreeData freeData;

    MtmGetProductPrice prodPrice;
    double income;


};

Product createProduct(unsigned int id,
                      const char *name,
                      MatamazomAmountType amountType,
                      MtmProductData customData,
                      MtmCopyData copyData,
                      MtmFreeData freeData,
                      MtmGetProductPrice prodPrice) {

    Product product = malloc(sizeof(*product));

    if(!product)
        return NULL;

    product->name=malloc(strlen(name)+1);

    product->copyData=copyData;
    product->freeData=freeData;

    product->id=id;
    strcpy(product->name,name);
    product->amountType=amountType;

    product->customData=copyData(customData);
    product->prodPrice=prodPrice;
    product->income=0;

    return product;
}



int compareProducts(Product product1, Product product2){

    int result=product1->id-product2->id;
    if(result>0) {
        return 1;
    }

    if(result<0) {
        return -1;
    }

    return 0;
}



Product copyProduct(Product product){

    Product copyProduct=malloc(sizeof(*copyProduct));
    if(!copyProduct)
        return NULL;

    copyProduct->name=malloc(strlen(product->name)+1);
    if(!copyProduct->name) {
        free(copyProduct);
        return NULL;
    }

    copyProduct->id=product->id;

    copyProduct->amountType=product->amountType;
    copyProduct->customData=product->copyData(product->customData);
    strcpy(copyProduct->name,product->name);
    copyProduct->prodPrice=product->prodPrice;
    copyProduct->income = product->income;

    copyProduct->copyData=product->copyData;
    copyProduct->freeData=product->freeData;

    return copyProduct;
}



void freeProduct(Product product){

    if(!product)
        return;

    product->freeData(product->customData);
    free(product->name);
    free(product);
}



void updateIncome(Product product, double income){
    if(!product){
        return;
    }

    product->income+=income;
}



MatamazomAmountType getAmountType (Product product){
    return (product->amountType);
}



double getIncome (Product product){
    return (product->income);
}



char* get_name(Product product){
    if(!product || !product->name)
        return NULL;

    return product->name;
}



double get_price_per_amount(Product product , double amount) {
    if (product == NULL)
        return 0;

    return product->prodPrice(product->customData,amount);
}



unsigned int get_id(Product product){
    if(!product){
        return 0;
    }
    return product->id;
}



Product get_iterator(AmountSet set, unsigned int id){
    if(set==NULL)
        return  NULL;

    Product current =  asGetFirst(set);
    while (current){
        if(current->id==id){
            return current;
        }
        current=asGetNext(set);
    }
    return NULL;
}



MtmProductData get_custom_data(Product product){
    if(!product)
        return NULL;

    return product->customData;
}



Product getProduct(AmountSet SetOfProducts,unsigned int id){

    Product currentProduct=asGetFirst(SetOfProducts);

    while(currentProduct){

        if(currentProduct->id==id){
            return currentProduct;
        }

        currentProduct=asGetNext(SetOfProducts);
    }
    return NULL;
}





