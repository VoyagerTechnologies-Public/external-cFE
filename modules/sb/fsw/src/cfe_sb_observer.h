/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *
 * Optional, platform-selected observation points for Software Bus message
 * delivery. The default implementation is compiled away. A platform that
 * needs deterministic external transaction accounting may supply an observer
 * source through CFE_SB_OBSERVER_SOURCE.
 */

#ifndef CFE_SB_OBSERVER_H
#define CFE_SB_OBSERVER_H

#include "cfe_sb_api_typedefs.h"

#include <stdbool.h>
#include <stdint.h>

typedef uintptr_t CFE_SB_ObserverToken_t;

#define CFE_SB_OBSERVER_INVALID_TOKEN ((CFE_SB_ObserverToken_t)0)

#ifdef CFE_SB_OBSERVER_ENABLED

/**
 * Reserve observation state for one destination immediately before SB writes
 * the buffer pointer to that destination's queue. The returned token is passed
 * to CFE_SB_Observer_EndDelivery() exactly once, including on queue failure.
 * Implementations must not retain or modify the message buffer.
 */
CFE_SB_ObserverToken_t CFE_SB_Observer_BeginDelivery(CFE_SB_MsgId_t         RoutingMsgId,
                                                      CFE_SB_PipeId_t        PipeId,
                                                      const CFE_SB_Buffer_t *Buffer);

/** Confirm or cancel the reservation made for one destination queue write. */
void CFE_SB_Observer_EndDelivery(CFE_SB_ObserverToken_t Token, bool Delivered);

/** Notify the observer after every public transmit operation has concluded. */
void CFE_SB_Observer_EndTransmit(void);

/**
 * Notify the observer immediately before receiving from a validated pipe.
 * Polling distinguishes durable input queues, whose contents are intentionally
 * consumed by a later scheduled activation, from pipes that wake a task.
 */
void CFE_SB_Observer_BeginReceive(CFE_SB_PipeId_t PipeId, bool Polling);

/** Notify the observer after a buffer was successfully received from a pipe. */
void CFE_SB_Observer_MessageReceived(CFE_SB_MsgId_t         RoutingMsgId,
                                     CFE_SB_PipeId_t        PipeId,
                                     const CFE_SB_Buffer_t *Buffer);

#endif

#endif /* CFE_SB_OBSERVER_H */
