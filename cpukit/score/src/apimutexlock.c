/**
 * @file
 *
 * @brief Acquires the specified API mutex.
 *
 * @ingroup ScoreAPIMutex
 */

/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/apimutex.h>
#include <rtems/score/coremuteximpl.h>
#include <rtems/score/threadimpl.h>

void _API_Mutex_Lock( API_Mutex_Control *the_mutex )
{
  Thread_Life_state previous_thread_life_state;
  ISR_lock_Context  lock_context;

  previous_thread_life_state =
    _Thread_Set_life_protection( THREAD_LIFE_PROTECTED );

  _ISR_lock_ISR_disable( &lock_context );

  _CORE_mutex_Seize(
    &the_mutex->Mutex,
    _Thread_Executing,
    true,
    0,
    &lock_context
  );

  if ( the_mutex->Mutex.nest_count == 1 ) {
    the_mutex->previous_thread_life_state = previous_thread_life_state;
  }
}
