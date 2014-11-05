<<<<<<< HEAD
/*
 * metrics.h
 *
 *  Created on: Oct 31, 2014
 *      Author: ajm5774
 */

#ifndef METRICS_H_
#define METRICS_H_

#include "TellerInterface.h"
#include "queue.h"

/*
 * Metrics include:
 * 1. The total number of customers serviced during the day.
 * 2. The average time each customer spends waiting in the queue.
 * 3. The average time each sutomer spends with the teller.
 * 4. The average time tellers wait for customers.
 * 5. The maximum customer wait time in the queue.
 * 6. The maximum wait time for tellers waiting for customers.
 * 7. The maximum transaction time for the tellers.
 * 8. The maximum depth of the queue.
 */

int getNumServiced();

long getAverageCustomerWaitTime();

long getAverageServiceTime();

long getAverageTellerWaitTime();

long getMaximumCustomerWaitTime();

long getMaximumTellerWaitTime();

long getMaximumServiceTime();

int getMaximumLineLength();

#endif /* METRICS_H_ */
=======
/*
 * metrics.h
 *
 *  Created on: Oct 31, 2014
 *      Author: ajm5774
 */

#ifndef METRICS_H_
#define METRICS_H_

#include "TellerInterface.h"
#include "queue.h"

/*
 * Metrics include:
 * 1. The total number of customers serviced during the day.
 * 2. The average time each customer spends waiting in the queue.
 * 3. The average time each sutomer spends with the teller.
 * 4. The average time tellers wait for customers.
 * 5. The maximum customer wait time in the queue.
 * 6. The maximum wait time for tellers waiting for customers.
 * 7. The maximum transaction time for the tellers.
 * 8. The maximum depth of the queue.
 */

int getNumServiced();

long getAverageCustomerWaitTime();

long getAverageServiceTime();

long getAverageTellerWaitTime();

long getMaximumCustomerWaitTime();

long getMaximumTellerWaitTime();

long getMaximumServiceTime();

int getMaximumLineLength();

#endif /* METRICS_H_ */
>>>>>>> 7a45798c5ed9dba6cef3f6cc29f864e3972417fd
