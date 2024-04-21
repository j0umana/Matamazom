#ifndef MATAMAZOM_PROJECT_ORDERS_H
#define MATAMAZOM_PROJECT_ORDERS_H
#include "amount_set.h"
#include "products.h"
#include "matamazom.h"
#include "set.h"
#include <stdlib.h>

/**
 * type that represents the orders' list in the matamazom.
 */
typedef struct order_t *order;

/**
 * createOrder: a function that creates a new order.
 * @param id - the id assigned to the new order.
 * @return - a pointer to the order we created.
 */
order createOrder(unsigned int id);

/**
 * findOrderById: a function that finds the order which holds the given id.
 * @param set - the set we search in.
 * @param orderId - the id of the order we want to go.
 * @return - a pointer to the order in case of success.
 *           NULL, otherwise.
 */
order findOrderById(Set set, unsigned int orderId);

/**
 * getProductListOfOrder: a function that returns the product list of the given
 *                        order.
 * @param order
 * @return: the product list of the order in case of success.
 *          NULL, otherwise.
 */
AmountSet getProductListOfOrder(order order);

/**
 * freeOrder: a function that frees all the space allocated for the specific
 *            order.
 * @param order
 */
void freeOrder(order order);

/**
 * copyOrder: a function that copies the given order.
 * @param orderToCopy - the order we want to  be copied.
 * @return a pointer to the new copy in case of success.
 *         NULL, otherwise.
 */
order copyOrder(order orderToCopy);

/**
 * compareOrders: a function that compares the functions by their id.
 * @param order1
 * @param order2
 * @return positive integer if order1's id is greater than order2's.
 *         negative integer if order2's id is greater than order1's.
 *         0 if order1 is the same as order2.
 */
int compareOrders(order order1, order order2);


#endif //MATAMAZOM_PROJECT_ORDERS_H
