#ifndef __CDIO_H__
#define	__CDIO_H__

#define RETRY_COUNT 3

int readData();

void safeRead(readFunc);

#endif