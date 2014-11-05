<<<<<<< HEAD
/*
 * customer.h
 *
 *  Created on: Oct 20, 2014
 *      Author: ajm5774
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

typedef struct Customer {
	int id;
	struct Customer* behind;
	long timeStart;
	long timeWaiting;
	long timeWithTeller;
	long timeEnd;
} Customer;

void StartCustomerThread();

#endif /* CUSTOMER_H_ */
=======
/*
 * customer.h
 *
 *  Created on: Oct 20, 2014
 *      Author: ajm5774
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

typedef struct Customer {
	int id;
	struct Customer* behind;
	long timeStart;
	long timeWaiting;
	long timeWithTeller;
	long timeEnd;
} Customer;

void StartCustomerThread();

#endif /* CUSTOMER_H_ */
>>>>>>> 7a45798c5ed9dba6cef3f6cc29f864e3972417fd
