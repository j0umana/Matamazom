#ifndef MATAMAZOM_PROJECT_PRODUCTS_H
#define MATAMAZOM_PROJECT_PRODUCTS_H

#include "amount_set.h"
#include "matamazom.h"
#include "set.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *type that represents the products in the warehouse.
 */
typedef struct Product_t* Product;

/**
 * compareProducts is a function that compares between products' id numbers
 * to determine which product has lower id value or wether the products are
 * the same.
 * @param product1
 * @param product2
 * @return positive integer if product1's id > product2's id,
 *         negative integer if product2's id > product1's id,
 *         0 if product1 and product2 are the same.
 */
int compareProducts(Product product1, Product product2);

/**
 * copyProduct is a function used to copy a given product.
 * @param product - the product we want to copy.
 * @return the copy of the given product, NULL in case of failure.
 */
Product copyProduct(Product product);


/**
 * freeProduct: freeing a given product.
 * @param product - the product we want to free.
 */
void freeProduct(Product product);


/**
 * createProduct: a function that creates a new product.
 * @param id
 * @param name
 * @param amountType
 * @param customData
 * @param copyData
 * @param freeData
 * @param prodPrice
 * @return the Product we created which has the the parameters we received as
 * parameters, NULL in case of allocation failure.
 */
Product createProduct(unsigned int id,
                      const char *name,
                      MatamazomAmountType amountType,
                      MtmProductData customData,
                      MtmCopyData copyData,
                      MtmFreeData freeData,
                      MtmGetProductPrice prodPrice);
/**
 * updateIncome: a function that updates the income of the products when we ship
 * some amount of them.
 * @param product - the product that we will be updating its income.
 * @param income - the income we will add to the existing income.
 */
void updateIncome(Product product, double income);

/**
 * a function that returns the amount type of a given product.
 * @param product - the product we want to get its amount type.
 * @return: MATAMAZOM_INTEGER_AMOUNT - if the amount type is integer.
            MATAMAZOM_HALF_INTEGER_AMOUNT - if the amount type is half-integer.
            MATAMAZOM_ANY_AMOUNT - if all the amount types are valid.
 */
MatamazomAmountType getAmountType (Product product);

/**
 * get_id function is a function that returns the id of a given product.
 * @param product
 * @return the id of the product.
 */
unsigned int get_id(Product product);

/**
 * a function that returns the price of a certain amount of the product.
 * @param product
 * @param amount - the amount we want to know its the price
 * @return the price per the given amount
 */
double get_price_per_amount(Product product,double amount);

/**
 * get_name: a function that returns the name of the product.
 * @param product
 * @return a string which is the name of the product.
 */
char* get_name(Product product);

/**
 * get_iterator: a function that returns that puts the iterator of the amount
 *               the function on the product that holds the specific id we
 *               receive in the function.
 * @param set - the set of products.
 * @param id - the id of the products that we want the iterator to point at.
 * @return - a pointer to the place of the product in the set.
 *           or NULL in case of failure.
 */
Product get_iterator(AmountSet set, unsigned int id);

/**
 * a function that returns the income of a given product.
 * @param product
 * @return - the income of the product.
 */
double getIncome (Product product);

/**
 * get_custom_data - a function that returns the customData
 *                  field of the given product.
 * @param product
 * @return - the customData field of the product.
 */
MtmProductData get_custom_data(Product product);

/**
 * getProduct: a function that returns a pointer to the product that holds
 *             the received id.
 * @param SetOfProducts - the set that has the product in it.
 * @param id - the id of the wanter product.
 * @return - NULL in case of failure.
 *           else, a pointer to the product in the set.
 */
Product getProduct(AmountSet SetOfProducts,unsigned int id);



#endif //MATAMAZOM_PROJECT_PRODUCTS_H
