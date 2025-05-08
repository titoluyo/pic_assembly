/* 
 * File:   semaforo.h
 * Author: tito.luyo
 *
 * Created on May 1, 2025, 12:11 PM
 */

#ifndef SEMAFORO_H
#define	SEMAFORO_H

#ifdef	__cplusplus
extern "C" {
#endif

void SemaforoInit (void);
void SemaforoLoop (void);
void InterruptorInit(void);
void InterruptorLoop(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SEMAFORO_H */

