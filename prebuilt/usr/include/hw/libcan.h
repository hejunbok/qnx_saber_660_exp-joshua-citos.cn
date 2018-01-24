/*
 * $QNXLicenseC:
 * Copyright 2014, QNX Software Systems.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#ifndef __LIBCAN_H_INCLUDED
#define __LIBCAN_H_INCLUDED

#include <stdint.h>
#include <inttypes.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/can_dcmd.h>

/* Event Flag Definitions */
#define CANDEV_EVENT_QUEUED     0x00000001

/* CAN Message Linked List element */
typedef struct _canmsg {
    CAN_MSG         cmsg;  /* CAN message */
    struct _canmsg *next;  /* Pointer to next message */
    struct _canmsg *prev;  /* Pointer to previous message */
} canmsg_t;

/* Message List control */
typedef struct _canmsg_list {
    size_t                len;        /* Length of each msg in bytes */
    unsigned              nmsg;       /* Total number of messages in list */
    volatile unsigned     msgcnt;     /* Number of messages in use */
    canmsg_t             *msgs;       /* Array of messages defining the message list */
    canmsg_t             *head;       /* Head of list to add new messages */
    canmsg_t             *tail;       /* Tail of list to consume messages */
    intrspin_t            lock;       /* List locking mechanism */
} canmsg_list_t;

/* CAN mailbox type */
typedef enum
{
    CANDEV_TYPE_RX,
    CANDEV_TYPE_TX
} CANDEV_TYPE;


/* Blocked Client Wait Structure */
typedef struct client_wait_entry
{
    int                         rcvid;    /* Blocked client's rcvid */
    struct client_wait_entry   *next;    /* Pointer to next blocked client */
} CLIENTWAIT;

/* Blocked Client Wait Queue */
typedef struct client_waitq_entry
{
    CLIENTWAIT     *elems;
    CLIENTWAIT     *head;    /* Head of client wait queue */
    CLIENTWAIT     *tail;    /* Tail of client wait queue */
    int             cnt;     /* Number of clients waiting */
    intrspin_t      lock;    /* List locking mechanism */
} CLIENTWAITQ;

/* CAN Init Device Structure */
typedef struct can_dev_init_entry
{
    CANDEV_TYPE         devtype;        /* CAN device type */
    int                 can_unit;       /* CAN unit number */
    int                 dev_unit;       /* Device unit number */
    uint32_t            msgq_size;      /* Number of messages */
    uint32_t            waitq_size;     /* Length of CAN message data */
    CANDEV_MODE         mode;           /* CAN driver mode - I/O or raw frames */
} CANDEV_INIT;

/* Generic CAN Device Structure */
typedef struct can_dev_entry
{
    iofunc_attr_t       attr;
    iofunc_mount_t      mount;
    CANDEV_TYPE         devtype;            /* CAN device type */
    int                 can_unit;           /* CAN unit number */
    int                 dev_unit;           /* Device unit number */
    CANDEV_MODE         mode;               /* CAN driver mode - I/O or raw frames */
    canmsg_list_t      *free_queue;         /* Free element queue */
    canmsg_list_t      *msg_queue;          /* Active tx or rx queue */
    volatile uint32_t   cflags;             /* CAN device flags */
    struct sigevent     event;              /* Device event */
    CLIENTWAITQ        *wait_free_queue;    /* Client wait queue free elems */
    CLIENTWAITQ        *wait_client_queue;  /* Blocked Clients queue */
} CANDEV;

/* CAN Device Driver Implemented Functions */
typedef struct _can_drvr_funcs_t
{
    void    (*transmit)     (CANDEV *cdev);
    int     (*devctl)       (CANDEV *cdev, int dcmd, DCMD_DATA *data);
    void    (*transmit_raw) (CANDEV *cdev, CAN_MSG msg);
    void    (*receive_raw)  (CANDEV *cdev, CAN_MSG msg, uint32_t is_blocking);
} can_drvr_funcs_t;

/* Resource Manager Info Structure */
typedef struct resmgr_info_entry
{
    dispatch_t                  *dpp;
    dispatch_context_t          *ctp;
    int                          coid;
    resmgr_attr_t                resmgr_attr;
    resmgr_connect_funcs_t       connect_funcs;
    resmgr_io_funcs_t            io_funcs;
    can_drvr_funcs_t            *drvr_funcs;
} RESMGR_INFO;

/* Global resource manager info variable */
extern RESMGR_INFO rinfo;

/* Library functions available to driver */
void can_resmgr_init(can_drvr_funcs_t *drvr_funcs);
void can_resmgr_start(void);
void can_resmgr_create_device(CANDEV *cdev);
void can_resmgr_init_device(CANDEV *cdev, CANDEV_INIT *cinit);
struct sigevent *can_client_check(CANDEV *cdev);

/* Message list function prototypes */
canmsg_list_t *msg_list_create(unsigned nmsg);
void msg_list_destroy(canmsg_list_t *ml);

/* Queueing function for two queue mode (free queue + data queue) */
canmsg_t *canmsg_dequeue_element(canmsg_list_t *ml);
void      canmsg_queue_element(canmsg_list_t *ml, canmsg_t *queue_elem);

/* Mini-driver support functions */
canmsg_t *msg_enqueue_mdriver_next(canmsg_list_t *ml);
void      msg_enqueue_mdriver(canmsg_list_t *ml);

#endif


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/6.6.0/trunk/lib/can/public/hw/libcan.h $ $Rev: 781202 $")
#endif
