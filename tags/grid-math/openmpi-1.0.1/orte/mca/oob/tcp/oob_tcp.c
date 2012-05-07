/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ompi_config.h"

#include "include/orte_types.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <fcntl.h>
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#include "include/ompi_socket_errno.h"
#include "opal/util/output.h"
#include "opal/util/if.h"
#include "orte/class/orte_proc_table.h"
#include "mca/oob/tcp/oob_tcp.h"
#include "mca/errmgr/errmgr.h"
#include "mca/ns/ns.h"
#include "mca/gpr/gpr.h"

#define IMPORTANT_WINDOWS_COMMENT() \
            /* In windows, many of the socket functions return an EWOULDBLOCK instead of \
               things like EAGAIN, EINPROGRESS, etc. It has been verified that this will \
               not conflict with other error codes that are returned by these functions \
               under UNIX/Linux environments */

/*
 * Data structure for accepting connections.
 */

struct mca_oob_tcp_event_t {
    opal_list_item_t item;
    opal_event_t event;
};
typedef struct mca_oob_tcp_event_t mca_oob_tcp_event_t;

static void mca_oob_tcp_event_construct(mca_oob_tcp_event_t* event)
{
    OPAL_THREAD_LOCK(&mca_oob_tcp_component.tcp_lock);
    opal_list_append(&mca_oob_tcp_component.tcp_events, &event->item);
    OPAL_THREAD_UNLOCK(&mca_oob_tcp_component.tcp_lock);
}

static void mca_oob_tcp_event_destruct(mca_oob_tcp_event_t* event)
{
    OPAL_THREAD_LOCK(&mca_oob_tcp_component.tcp_lock);
    opal_list_remove_item(&mca_oob_tcp_component.tcp_events, &event->item);
    OPAL_THREAD_UNLOCK(&mca_oob_tcp_component.tcp_lock);
}

OBJ_CLASS_INSTANCE(
    mca_oob_tcp_event_t,
    opal_list_item_t,
    mca_oob_tcp_event_construct,
    mca_oob_tcp_event_destruct);

/*
 * Local utility functions
 */

static int  mca_oob_tcp_create_listen(void);
static void mca_oob_tcp_recv_handler(int sd, short flags, void* user);
static void mca_oob_tcp_accept(void);


struct mca_oob_tcp_subscription_t {
    opal_list_item_t item;
    orte_jobid_t jobid;
    orte_gpr_subscription_id_t subid;
};
typedef struct mca_oob_tcp_subscription_t mca_oob_tcp_subscription_t;

OBJ_CLASS_INSTANCE(
    mca_oob_tcp_subscription_t,
    opal_list_item_t,
    NULL,
    NULL);



/*
 * Struct of function pointers and all that to let us be initialized
 */
mca_oob_tcp_component_t mca_oob_tcp_component = {
  {
    {
        MCA_OOB_BASE_VERSION_1_0_0,
        "tcp", /* MCA module name */
        1,  /* MCA component major version */
        0,  /* MCA component minor version */
        0,  /* MCA component release version */
        mca_oob_tcp_component_open,  /* component open */
        mca_oob_tcp_component_close /* component close */
    },
    {
        false /* checkpoint / restart */
    },
    mca_oob_tcp_component_init
  }
};

static mca_oob_t mca_oob_tcp = {
    mca_oob_tcp_get_addr,
    mca_oob_tcp_set_addr,
    mca_oob_tcp_ping,
    mca_oob_tcp_send,
    mca_oob_tcp_recv,
    mca_oob_tcp_send_nb,
    mca_oob_tcp_recv_nb,
    mca_oob_tcp_recv_cancel,
    mca_oob_tcp_init,
    mca_oob_tcp_fini,
    mca_oob_xcast
};


/*
 * Utility function to register/lookup module parameters.
 */

static inline int mca_oob_tcp_param_register_int(
    const char* param_name,
    int default_value)
{
    int id = mca_base_param_register_int("oob","tcp",param_name,NULL,default_value);
    int param_value = default_value;
    mca_base_param_lookup_int(id,&param_value);
    return param_value;
}


static inline char* mca_oob_tcp_param_register_str(
    const char* param_name,
    const char* default_value)
{
    int id = mca_base_param_register_string("oob","tcp",param_name,NULL,default_value);
    char* param_value = NULL;
    mca_base_param_lookup_string(id,&param_value);
    return param_value;
}


/*
 * Initialize global variables used w/in this module.
 */
int mca_oob_tcp_component_open(void)
{
#ifdef WIN32
    WSADATA win_sock_data;
    if (WSAStartup(MAKEWORD(2,2), &win_sock_data) != 0) {
        opal_output (0, "mca_oob_tcp_component_init: failed to initialise windows sockets: error %d\n", WSAGetLastError());
        return OMPI_ERROR;
    }
#endif

    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_subscriptions, opal_list_t);
    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_peer_list,     opal_list_t);
    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_peers,         opal_hash_table_t);
    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_peer_names,    opal_hash_table_t);
    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_peer_free,     opal_free_list_t);
    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_msgs,          opal_free_list_t);
    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_lock,          opal_mutex_t);
    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_events,        opal_list_t);
    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_msg_post,      opal_list_t);
    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_msg_recv,      opal_list_t);
    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_msg_completed, opal_list_t);
    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_match_lock,    opal_mutex_t);
    OBJ_CONSTRUCT(&mca_oob_tcp_component.tcp_match_cond,    opal_condition_t);

    /* register oob module parameters */
    mca_oob_tcp_component.tcp_peer_limit =
        mca_oob_tcp_param_register_int("peer_limit", -1);
    mca_oob_tcp_component.tcp_peer_retries =
        mca_oob_tcp_param_register_int("peer_retries", 60);
    mca_oob_tcp_component.tcp_debug =
        mca_oob_tcp_param_register_int("debug", 0);
    mca_oob_tcp_component.tcp_include =
        mca_oob_tcp_param_register_str("include", NULL);
    mca_oob_tcp_component.tcp_exclude =
        mca_oob_tcp_param_register_str("exclude", NULL);
    mca_oob_tcp_component.tcp_sndbuf =
        mca_oob_tcp_param_register_int("sndbuf", 128*1024);
    mca_oob_tcp_component.tcp_rcvbuf =
        mca_oob_tcp_param_register_int("rcvbuf", 128*1024);

    /* initialize state */
    mca_oob_tcp_component.tcp_listen_sd = -1;
    mca_oob_tcp_component.tcp_match_count = 0;
    return OMPI_SUCCESS;
}


/*
 * Cleanup of global variables used by this module.
 */

int mca_oob_tcp_component_close(void)
{
#ifdef WIN32
    WSACleanup();
#endif

    /* cleanup resources */
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_peer_list);
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_peers);
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_peer_names);
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_peer_free);
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_events);
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_subscriptions);
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_msgs);
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_lock);
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_msg_post);
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_msg_recv);
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_msg_completed);
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_match_lock);
    OBJ_DESTRUCT(&mca_oob_tcp_component.tcp_match_cond);
    return OMPI_SUCCESS;
}


/*
 *  Called by mca_oob_tcp_recv_handler() when the TCP listen
 *  socket has pending connection requests. Accept incoming
 *  requests and queue for completion of the connection handshake.
*/

static void mca_oob_tcp_accept(void)
{
    while(true) {
        ompi_socklen_t addrlen = sizeof(struct sockaddr_in);
        struct sockaddr_in addr;
        mca_oob_tcp_event_t* event;
        int sd;

        sd = accept(mca_oob_tcp_component.tcp_listen_sd, (struct sockaddr*)&addr, &addrlen);
        if(sd < 0) {
            IMPORTANT_WINDOWS_COMMENT();
            if(ompi_socket_errno == EINTR)
                continue;
            if(ompi_socket_errno != EAGAIN || ompi_socket_errno != EWOULDBLOCK)
                opal_output(0, "mca_oob_tcp_accept: accept() failed with errno %d.", ompi_socket_errno);
            return;
        }

        /* setup socket options */
        mca_oob_tcp_set_socket_options(sd);

        /* log the accept */
        if(mca_oob_tcp_component.tcp_debug) {
            opal_output(0, "[%lu,%lu,%lu] mca_oob_tcp_accept: %s:%d\n",
                ORTE_NAME_ARGS(orte_process_info.my_name),
                inet_ntoa(addr.sin_addr),
                addr.sin_port);
        }

        /* wait for receipt of peers process identifier to complete this connection */
        event = OBJ_NEW(mca_oob_tcp_event_t);
        opal_event_set(&event->event, sd, OPAL_EV_READ, mca_oob_tcp_recv_handler, event);
        opal_event_add(&event->event, 0);
    }
}

/*
 * Create a listen socket and bind to all interfaces
 */

static int mca_oob_tcp_create_listen(void)
{
    int flags;
    struct sockaddr_in inaddr;
    ompi_socklen_t addrlen;

    /* create a listen socket for incoming connections */
    mca_oob_tcp_component.tcp_listen_sd = socket(AF_INET, SOCK_STREAM, 0);
    if(mca_oob_tcp_component.tcp_listen_sd < 0) {
        opal_output(0,"mca_oob_tcp_component_init: socket() failed with errno=%d", ompi_socket_errno);
        return OMPI_ERROR;
    }

    /* setup socket options */
    mca_oob_tcp_set_socket_options(mca_oob_tcp_component.tcp_listen_sd);

    /* bind address */
    memset(&inaddr, 0, sizeof(inaddr));
    inaddr.sin_family = AF_INET;
    inaddr.sin_addr.s_addr = INADDR_ANY;
    inaddr.sin_port = 0;

    if(bind(mca_oob_tcp_component.tcp_listen_sd, (struct sockaddr*)&inaddr, sizeof(inaddr)) < 0) {
        opal_output(0,"mca_oob_tcp_create_listen: bind() failed with errno=%d", ompi_socket_errno);
        return OMPI_ERROR;
    }

    /* resolve system assigned port */
    addrlen = sizeof(struct sockaddr_in);
    if(getsockname(mca_oob_tcp_component.tcp_listen_sd, (struct sockaddr*)&inaddr, &addrlen) < 0) {
        opal_output(0, "mca_oob_tcp_create_listen: getsockname() failed with errno=%d", ompi_socket_errno);
        return OMPI_ERROR;
    }
    mca_oob_tcp_component.tcp_listen_port = inaddr.sin_port;

    /* setup listen backlog to maximum allowed by kernel */
    if(listen(mca_oob_tcp_component.tcp_listen_sd, SOMAXCONN) < 0) {
        opal_output(0, "mca_oob_tcp_component_init: listen() failed with errno=%d", ompi_socket_errno);
        return OMPI_ERROR;
    }

    /* set socket up to be non-blocking, otherwise accept could block */
    if((flags = fcntl(mca_oob_tcp_component.tcp_listen_sd, F_GETFL, 0)) < 0) {
        opal_output(0, "mca_oob_tcp_component_init: fcntl(F_GETFL) failed with errno=%d", ompi_socket_errno);
        return OMPI_ERROR;
    } else {
        flags |= O_NONBLOCK;
        if(fcntl(mca_oob_tcp_component.tcp_listen_sd, F_SETFL, flags) < 0) {
            opal_output(0, "mca_oob_tcp_component_init: fcntl(F_SETFL) failed with errno=%d", ompi_socket_errno);
            return OMPI_ERROR;
        }
    }

    /* register listen port */
    opal_event_set(
        &mca_oob_tcp_component.tcp_recv_event,
        mca_oob_tcp_component.tcp_listen_sd,
        OPAL_EV_READ|OPAL_EV_PERSIST,
        mca_oob_tcp_recv_handler,
        0);
    opal_event_add(&mca_oob_tcp_component.tcp_recv_event, 0);
    return OMPI_SUCCESS;
}


/*
 * Handle probe
 */
static void mca_oob_tcp_recv_probe(int sd, mca_oob_tcp_hdr_t* hdr)
{
    unsigned char* ptr = (unsigned char*)hdr;
    size_t cnt = 0;

    hdr->msg_type = MCA_OOB_TCP_PROBE;
    hdr->msg_dst = hdr->msg_src;
    hdr->msg_src = *orte_process_info.my_name;
    MCA_OOB_TCP_HDR_HTON(hdr);

    while(cnt < sizeof(mca_oob_tcp_hdr_t)) {
        int retval = send(sd, (char *)ptr+cnt, sizeof(mca_oob_tcp_hdr_t)-cnt, 0);
        if(retval < 0) {
            IMPORTANT_WINDOWS_COMMENT();
            if(ompi_socket_errno != EINTR && ompi_socket_errno != EAGAIN && ompi_socket_errno != EWOULDBLOCK) {
                opal_output(0, "[%lu,%lu,%lu]-[%lu,%lu,%lu] mca_oob_tcp_peer_recv_probe: send() failed with errno=%d\n",
                    ORTE_NAME_ARGS(orte_process_info.my_name),
                    ORTE_NAME_ARGS(&(hdr->msg_src)),
                    ompi_socket_errno);
                close(sd);
                return;
            }
            continue;
        }
        cnt += retval;
    }
    close(sd);
}

/*
 * Handle connection request
 */
static void mca_oob_tcp_recv_connect(int sd, mca_oob_tcp_hdr_t* hdr)
{
    mca_oob_tcp_peer_t* peer;
    int flags;
    int cmpval;

    /* now set socket up to be non-blocking */
    if((flags = fcntl(sd, F_GETFL, 0)) < 0) {
        opal_output(0, "[%lu,%lu,%lu] mca_oob_tcp_recv_handler: fcntl(F_GETFL) failed with errno=%d",
                ORTE_NAME_ARGS(orte_process_info.my_name), ompi_socket_errno);
    } else {
        flags |= O_NONBLOCK;
        if(fcntl(sd, F_SETFL, flags) < 0) {
            opal_output(0, "[%lu,%lu,%lu] mca_oob_tcp_recv_handler: fcntl(F_SETFL) failed with errno=%d",
                ORTE_NAME_ARGS(orte_process_info.my_name), ompi_socket_errno);
        }
    }

    /* check for wildcard name - if this is true - we allocate a name from the name server
     * and return to the peer
     */
    cmpval = orte_ns.compare(ORTE_NS_CMP_ALL, &hdr->msg_src, MCA_OOB_NAME_ANY);
    if (cmpval == 0) {
        if (ORTE_SUCCESS != orte_ns.create_jobid(&hdr->msg_src.jobid)) {
           return;
        }
        if (ORTE_SUCCESS != orte_ns.reserve_range(hdr->msg_src.jobid, 1, &hdr->msg_src.vpid)) {
           return;
        }
        if (ORTE_SUCCESS != orte_ns.assign_cellid_to_process(&hdr->msg_src)) {
           return;
        }
    }

    /* lookup the corresponding process */
    peer = mca_oob_tcp_peer_lookup(&hdr->msg_src);
    if(NULL == peer) {
        opal_output(0, "[%lu,%lu,%lu] mca_oob_tcp_recv_handler: unable to locate peer",
                ORTE_NAME_ARGS(orte_process_info.my_name));
        close(sd);
        return;
    }
    /* is the peer instance willing to accept this connection */
    if(mca_oob_tcp_peer_accept(peer, sd) == false) {
        if(mca_oob_tcp_component.tcp_debug > 0) {
            opal_output(0, "[%lu,%lu,%lu]-[%lu,%lu,%lu] mca_oob_tcp_recv_handler: "
                    "rejected connection from [%lu,%lu,%lu] connection state %d",
                    ORTE_NAME_ARGS(orte_process_info.my_name),
                    ORTE_NAME_ARGS(&(peer->peer_name)),
                    ORTE_NAME_ARGS(&(hdr->msg_src)),
                    peer->peer_state);
        }
        close(sd);
        return;
    }
}

/*
 * Event callback when there is data available on the registered
 * socket to recv.
 */

static void mca_oob_tcp_recv_handler(int sd, short flags, void* user)
{
    mca_oob_tcp_hdr_t hdr;
    mca_oob_tcp_event_t* event = (mca_oob_tcp_event_t *)user;
    int rc;

    /* accept new connections on the listen socket */
    if(mca_oob_tcp_component.tcp_listen_sd == sd) {
        mca_oob_tcp_accept();
        return;
    }
    OBJ_RELEASE(event);

    /* recv the process identifier */
    while((rc = recv(sd, (char *)&hdr, sizeof(hdr), 0)) != sizeof(hdr)) {
        if(rc >= 0) {
            if(mca_oob_tcp_component.tcp_debug > 1) {
                opal_output(0, "[%lu,%lu,%lu] mca_oob_tcp_recv_handler: peer closed connection",
                    ORTE_NAME_ARGS(orte_process_info.my_name));
            }
            close(sd);
            return;
        }
        if(ompi_socket_errno != EINTR) {
            opal_output(0, "[%lu,%lu,%lu] mca_oob_tcp_recv_handler: recv() failed with errno=%d\n",
                ORTE_NAME_ARGS(orte_process_info.my_name), ompi_socket_errno);
            close(sd);
            return;
        }
    }
    MCA_OOB_TCP_HDR_NTOH(&hdr);

    /* dispatch based on message type */
    switch(hdr.msg_type) {
        case MCA_OOB_TCP_PROBE:
            mca_oob_tcp_recv_probe(sd, &hdr);
            break;
        case MCA_OOB_TCP_CONNECT:
            mca_oob_tcp_recv_connect(sd, &hdr);
            break;
        default:
            opal_output(0, "[%lu,%lu,%lu] mca_oob_tcp_recv_handler: invalid message type: %d\n",
                ORTE_NAME_ARGS(orte_process_info.my_name), hdr.msg_type);
            close(sd);
            break;
    }
}


/*
 * Component initialization - create a module.
 * (1) initialize static resources
 * (2) create listen socket
 */
mca_oob_t* mca_oob_tcp_component_init(int* priority)
{
    *priority = 1;

    /* are there any interfaces? */
    if(opal_ifcount() == 0)
        return NULL;

    /* initialize data structures */
    opal_hash_table_init(&mca_oob_tcp_component.tcp_peers, 128);
    opal_hash_table_init(&mca_oob_tcp_component.tcp_peer_names, 128);

    opal_free_list_init(&mca_oob_tcp_component.tcp_peer_free,
        sizeof(mca_oob_tcp_peer_t),
        OBJ_CLASS(mca_oob_tcp_peer_t),
        8,  /* initial number */
        mca_oob_tcp_component.tcp_peer_limit, /* maximum number */
        8);  /* increment to grow by */

    opal_free_list_init(&mca_oob_tcp_component.tcp_msgs,
        sizeof(mca_oob_tcp_msg_t),
        OBJ_CLASS(mca_oob_tcp_msg_t),
        8,  /* initial number */
       -1,  /* maximum number */
        8);  /* increment to grow by */

    /* intialize event library */
    memset(&mca_oob_tcp_component.tcp_recv_event, 0, sizeof(opal_event_t));
    memset(&mca_oob_tcp_component.tcp_send_event, 0, sizeof(opal_event_t));

    /* create a listen socket */
    if(mca_oob_tcp_create_listen() != OMPI_SUCCESS) {
        opal_output(0, "mca_oob_tcp_init: unable to create listen socket\n");
        return NULL;
    }
    return &mca_oob_tcp;
}


/*
 * Callback from registry on change to subscribed segments.
 */

void mca_oob_tcp_registry_callback(
    orte_gpr_notify_data_t* data,
    void* cbdata)
{
    size_t i, j, k;
    orte_gpr_value_t **values, *value;
    orte_gpr_keyval_t *keyval;
    orte_buffer_t buffer;
    mca_oob_tcp_addr_t* addr, *existing;
    mca_oob_tcp_peer_t* peer;

    if(mca_oob_tcp_component.tcp_debug > 1) {
        opal_output(0, "[%lu,%lu,%lu] mca_oob_tcp_registry_callback\n",
            ORTE_NAME_ARGS(orte_process_info.my_name));
    }

    /* process the callback */
    OPAL_THREAD_LOCK(&mca_oob_tcp_component.tcp_lock);
    values = (orte_gpr_value_t**)(data->values)->addr;
    for(i = 0, k=0; k < data->cnt &&
                    i < (data->values)->size; i++) {
        if (NULL != values[i]) {
            k++;
            value = values[i];
            for(j = 0; j < value->cnt; j++) {

                /* check to make sure this is the requested key */
                keyval = value->keyvals[j];
                if(strcmp(keyval->key,"oob-tcp") != 0)
                    continue;

                /* transfer ownership of registry object to buffer and unpack */
                OBJ_CONSTRUCT(&buffer, orte_buffer_t);
                if(orte_dps.load(&buffer,
                                 keyval->value.byteobject.bytes,
                                 keyval->value.byteobject.size) != ORTE_SUCCESS) {
                    /* TSW - throw ERROR */
                    continue;
                }
                keyval->type = ORTE_NULL;
                keyval->value.byteobject.bytes = NULL;
                keyval->value.byteobject.size = 0;
                addr = mca_oob_tcp_addr_unpack(&buffer);
                OBJ_DESTRUCT(&buffer);
                if(NULL == addr) {
                    opal_output(0, "[%lu,%lu,%lu] mca_oob_tcp_registry_callback: unable to unpack peer address\n",
                        ORTE_NAME_ARGS(orte_process_info.my_name));
                    continue;
                }

                if(mca_oob_tcp_component.tcp_debug > 1) {
                    opal_output(0, "[%lu,%lu,%lu] mca_oob_tcp_registry_callback: received peer [%lu,%lu,%lu]\n",
                        ORTE_NAME_ARGS(orte_process_info.my_name),
                        ORTE_NAME_ARGS(&(addr->addr_name)));
                }

                /* check for existing cache entry */
                existing = (mca_oob_tcp_addr_t *)orte_hash_table_get_proc(
                    &mca_oob_tcp_component.tcp_peer_names, &addr->addr_name);
                if(NULL != existing) {
                    /* TSW - need to update existing entry */
                    OBJ_RELEASE(addr);
                    continue;
                }

                /* insert into cache and notify peer */
                orte_hash_table_set_proc(&mca_oob_tcp_component.tcp_peer_names, &addr->addr_name, addr);
                peer = (mca_oob_tcp_peer_t *)orte_hash_table_get_proc(
                    &mca_oob_tcp_component.tcp_peers, &addr->addr_name);
                if(NULL != peer)
                    mca_oob_tcp_peer_resolved(peer, addr);
            }
        }
    }
    OPAL_THREAD_UNLOCK(&mca_oob_tcp_component.tcp_lock);
}

/*
 * Attempt to resolve peer name.
 */

int mca_oob_tcp_resolve(mca_oob_tcp_peer_t* peer)
{
    mca_oob_tcp_addr_t* addr;
    mca_oob_tcp_subscription_t* subscription;
    char *segment, *sub_name, *trig_name;
    char *key="oob-tcp";
    orte_gpr_subscription_id_t sub_id;
    opal_list_item_t* item;
    int rc;

    /* if the address is already cached - simply return it */
    OPAL_THREAD_LOCK(&mca_oob_tcp_component.tcp_lock);
    addr = (mca_oob_tcp_addr_t *)orte_hash_table_get_proc(&mca_oob_tcp_component.tcp_peer_names,
         &peer->peer_name);
    if(NULL != addr) {
         OPAL_THREAD_UNLOCK(&mca_oob_tcp_component.tcp_lock);
         mca_oob_tcp_peer_resolved(peer, addr);
         return OMPI_SUCCESS;
    }

    /* check to see if we have subscribed to this registry segment */
    for( item =  opal_list_get_first(&mca_oob_tcp_component.tcp_subscriptions);
         item != opal_list_get_end(&mca_oob_tcp_component.tcp_subscriptions);
         item =  opal_list_get_next(item)) {
        subscription = (mca_oob_tcp_subscription_t*)item;
        if(subscription->jobid == peer->peer_name.jobid) {
            OPAL_THREAD_UNLOCK(&mca_oob_tcp_component.tcp_lock);
            return OMPI_SUCCESS;
        }
    }

    if (ORTE_SUCCESS != (rc = orte_schema.get_std_subscription_name(&sub_name,
                                OMPI_OOB_SUBSCRIPTION, peer->peer_name.jobid))) {
        ORTE_ERROR_LOG(rc);
        return rc;
    }

    /* attach to the stage-1 standard trigger */
    if (ORTE_SUCCESS != (rc = orte_schema.get_std_trigger_name(&trig_name,
                                    ORTE_STG1_TRIGGER, peer->peer_name.jobid))) {
        ORTE_ERROR_LOG(rc);
        free(sub_name);
        return rc;
    }

    /* define the segment */
    if (ORTE_SUCCESS != (rc = orte_schema.get_job_segment_name(&segment,
                                peer->peer_name.jobid))) {
        ORTE_ERROR_LOG(rc);
        free(sub_name);
        free(trig_name);
        return rc;
    }

    /* If we do not release the mutex before the subscrition we will deadlock
     * as the suscription involve oob_tcp_send who involve again a lookup
     * call. Before unlocking the mutex (just to be protected by it) we can
     * create the subscription and add it to the list.
     */
    subscription = OBJ_NEW(mca_oob_tcp_subscription_t);
    subscription->jobid = peer->peer_name.jobid;
    opal_list_append(&mca_oob_tcp_component.tcp_subscriptions, &subscription->item);

    OPAL_THREAD_UNLOCK(&mca_oob_tcp_component.tcp_lock);
    if (ORTE_SUCCESS != (rc = orte_gpr.subscribe_1(&sub_id, NULL, NULL,
                                         ORTE_GPR_NOTIFY_ADD_ENTRY |
                                         ORTE_GPR_NOTIFY_VALUE_CHG |
                                         ORTE_GPR_NOTIFY_PRE_EXISTING,
                                         ORTE_GPR_KEYS_OR | ORTE_GPR_TOKENS_OR,
                                         segment,
                                         NULL,  /* look at all containers on this segment */
                                         key,
                                         mca_oob_tcp_registry_callback, NULL))) {
        ORTE_ERROR_LOG(rc);
        free(sub_name);
        free(trig_name);
        free(segment);
        /* Subscription registration failed, we should activate the cleaning logic:
         * remove the subscription from the list (protected by the mutex).
         */
        OPAL_THREAD_LOCK(&mca_oob_tcp_component.tcp_lock);
        opal_list_remove_item( &mca_oob_tcp_component.tcp_subscriptions,
                               &subscription->item );
        OPAL_THREAD_UNLOCK(&mca_oob_tcp_component.tcp_lock);
        return rc;
    }

    /* the id of each subscription is recorded
     * here so we can (if desired) cancel that subscription later
     */
    subscription->subid = sub_id;
    /* done with these, so release any memory */
    free(trig_name);
    free(sub_name);
    free(segment);

    return rc;
}


/*
 * Setup contact information in the registry.
 */
int mca_oob_tcp_init(void)
{
    orte_jobid_t jobid;
    orte_buffer_t *buffer;
    orte_gpr_subscription_id_t sub_id;
    char *sub_name, *segment, *trig_name, **tokens;
    char *keys[] = {"oob-tcp", ORTE_PROC_RML_IP_ADDRESS_KEY};
    orte_data_type_t types[2];
    orte_gpr_value_union_t values[2];
    mca_oob_tcp_subscription_t *subscription;
    int rc;
    opal_list_item_t* item;
    char *tmp, *tmp2, *tmp3;
    size_t i, num_tokens;

    /* random delay to stagger connections back to seed */
#if defined(WIN32)
    sleep((orte_process_info.my_name->vpid % orte_process_info.num_procs % 1000) * 1000);
#else
    usleep((orte_process_info.my_name->vpid % orte_process_info.num_procs % 1000) * 1000);
#endif

    /* get my jobid */
    if (ORTE_SUCCESS != (rc = orte_ns.get_jobid(&jobid,
                                                orte_process_info.my_name))) {
        ORTE_ERROR_LOG(rc);
        return rc;
    }

    /* iterate through the open connections and send an ident message to all peers -
     * note that we initially come up w/out knowing our process name - and are assigned
     * a temporary name by our peer. once we have determined our real name - we send it
     * to the peer.
    */
    OPAL_THREAD_LOCK(&mca_oob_tcp_component.tcp_lock);
    for(item =  opal_list_get_first(&mca_oob_tcp_component.tcp_peer_list);
        item != opal_list_get_end(&mca_oob_tcp_component.tcp_peer_list);
        item =  opal_list_get_next(item)) {
        mca_oob_tcp_peer_t* peer = (mca_oob_tcp_peer_t*)item;
        mca_oob_tcp_peer_send_ident(peer);
    }

    /* register subscribe callback to receive notification when all processes have registered */
    subscription = OBJ_NEW(mca_oob_tcp_subscription_t);
    subscription->jobid = jobid;
    opal_list_append(&mca_oob_tcp_component.tcp_subscriptions, &subscription->item);
    OPAL_THREAD_UNLOCK(&mca_oob_tcp_component.tcp_lock);

    if(mca_oob_tcp_component.tcp_debug > 2) {
        opal_output(0, "[%lu,%lu,%lu] mca_oob_tcp_init: calling orte_gpr.subscribe\n",
            ORTE_NAME_ARGS(orte_process_info.my_name));
    }

    if (ORTE_SUCCESS != (rc = orte_schema.get_std_subscription_name(&sub_name,
                                OMPI_OOB_SUBSCRIPTION, jobid))) {
        ORTE_ERROR_LOG(rc);
        return rc;
    }

    /* attach to the stage-1 standard trigger */
    if (ORTE_SUCCESS != (rc = orte_schema.get_std_trigger_name(&trig_name,
                                    ORTE_STG1_TRIGGER, jobid))) {
        ORTE_ERROR_LOG(rc);
        free(sub_name);
        return rc;
    }

    /* define the segment */
    if (ORTE_SUCCESS != (rc = orte_schema.get_job_segment_name(&segment, jobid))) {
        ORTE_ERROR_LOG(rc);
        free(sub_name);
        free(trig_name);
        return rc;
    }

    if (ORTE_SUCCESS != (rc = orte_gpr.subscribe_1(&sub_id, trig_name, sub_name,
                                         ORTE_GPR_NOTIFY_ADD_ENTRY |
                                         ORTE_GPR_NOTIFY_VALUE_CHG |
                                         ORTE_GPR_NOTIFY_STARTS_AFTER_TRIG,
                                         ORTE_GPR_KEYS_OR | ORTE_GPR_TOKENS_OR,
                                         segment,
                                         NULL,  /* look at all containers on this segment */
                                         keys[0],
                                         mca_oob_tcp_registry_callback, NULL))) {
        ORTE_ERROR_LOG(rc);
        free(sub_name);
        free(trig_name);
        free(segment);
        return rc;
    }
    /* the id of each subscription is recorded
     * here so we can (if desired) cancel that subscription later
     */
    subscription->subid = sub_id;
    /* done with these, so release any memory */
    free(trig_name);
    free(sub_name);


    /* now setup to put our contact info on registry */
    buffer = OBJ_NEW(orte_buffer_t);
    if(buffer == NULL) {
        ORTE_ERROR_LOG(ORTE_ERR_OUT_OF_RESOURCE);
        return ORTE_ERR_OUT_OF_RESOURCE;
    }
    if (ORTE_SUCCESS != (rc = mca_oob_tcp_addr_pack(buffer))) {
        ORTE_ERROR_LOG(rc);
        OBJ_RELEASE(buffer);
        return rc;
    }

    /* extract payload for storage */
    types[0] = ORTE_BYTE_OBJECT;
    if (ORTE_SUCCESS != (rc = orte_dps.unload(buffer, (void**)&(values[0].byteobject.bytes),
                                 &(values[0].byteobject.size)))) {
        ORTE_ERROR_LOG(rc);
        free(segment);
        OBJ_RELEASE(buffer);
        return rc;
    }
    OBJ_RELEASE(buffer);

    /* setup the IP address for storage */
    tmp = mca_oob.oob_get_addr();
    tmp2 = strrchr(tmp, '/') + 1;
    tmp3 = strrchr(tmp, ':');
    if(NULL == tmp2 || NULL == tmp3) {
        opal_output(0, "[%lu,%lu,%lu] mca_oob_tcp_init: invalid address \'%s\' "
                    "returned for selected oob interfaces.\n",
                    ORTE_NAME_ARGS(orte_process_info.my_name), tmp);
        ORTE_ERROR_LOG(ORTE_ERROR);
        free(segment);
        free(tmp);
        free(values[0].byteobject.bytes);
        return ORTE_ERROR;
    }
    *tmp3 = '\0';
    types[1] = ORTE_STRING;
    values[1].strptr = strdup(tmp2);
    free(tmp);

    /* get the process tokens */
    if (ORTE_SUCCESS != (rc = orte_schema.get_proc_tokens(&tokens, &num_tokens,
                                    orte_process_info.my_name))) {
        ORTE_ERROR_LOG(rc);
        free(segment);
        free(values[0].byteobject.bytes);
        free(values[1].strptr);
        return rc;
    }

    /* put our contact info in registry */
    if (ORTE_SUCCESS != (rc = orte_gpr.put_N(ORTE_GPR_OVERWRITE | ORTE_GPR_TOKENS_XAND,
                                        segment, tokens, 2, keys, types, values))) {
        ORTE_ERROR_LOG(rc);
    }

    free(segment);
    for(i=0; i < num_tokens; i++) {
        free(tokens[i]);
        tokens[i] = NULL;
    }
    if (NULL != tokens) free(tokens);
    free(values[0].byteobject.bytes);
    free(values[1].strptr);

    return rc;
}

/*
 * Module cleanup.
 */
int mca_oob_tcp_fini(void)
{
    opal_list_item_t *item;
    OPAL_THREAD_LOCK(&mca_oob_tcp_component.tcp_lock);
    opal_event_disable(); /* disable event processing */

    /* close listen socket */
    if (mca_oob_tcp_component.tcp_listen_sd >= 0) {
        opal_event_del(&mca_oob_tcp_component.tcp_recv_event);
        close(mca_oob_tcp_component.tcp_listen_sd);
        mca_oob_tcp_component.tcp_listen_sd = -1;
    }

    /* cleanup all peers */
    for(item = opal_list_remove_first(&mca_oob_tcp_component.tcp_peer_list);
        item != NULL;
        item = opal_list_remove_first(&mca_oob_tcp_component.tcp_peer_list)) {
        mca_oob_tcp_peer_t* peer = (mca_oob_tcp_peer_t*)item;
        MCA_OOB_TCP_PEER_RETURN(peer);
    }

    /* delete any pending events */
    for(item =  opal_list_remove_first(&mca_oob_tcp_component.tcp_events);
        item != NULL;
        item =  opal_list_remove_first(&mca_oob_tcp_component.tcp_events)) {
        mca_oob_tcp_event_t* event = (mca_oob_tcp_event_t*)item;
        opal_event_del(&event->event);
        OBJ_RELEASE(event);
    }

    opal_event_enable();
    OPAL_THREAD_UNLOCK(&mca_oob_tcp_component.tcp_lock);
    return OMPI_SUCCESS;
}


/*
* Compare two process names for equality.
*
* @param  n1  Process name 1.
* @param  n2  Process name 2.
* @return     (-1 for n1<n2 0 for equality, 1 for n1>n2)
*
* Note that the definition of < or > is somewhat arbitrary -
* just needs to be consistently applied to maintain an ordering
* when process names are used as indices.
*/


int mca_oob_tcp_process_name_compare(const orte_process_name_t* n1, const orte_process_name_t* n2)
{
    return orte_ns.compare(ORTE_NS_CMP_ALL, n1, n2);
}


/*
* Return local process address as a URI string.
*/

char* mca_oob_tcp_get_addr(void)
{
    int i;
    char *contact_info = (char *)malloc((opal_ifcount()+1) * 32);
    char *ptr = contact_info;
    *ptr = 0;

    for(i=opal_ifbegin(); i>0; i=opal_ifnext(i)) {
        struct sockaddr_in addr;
        char name[32];
        opal_ifindextoname(i, name, sizeof(name));
        if (mca_oob_tcp_component.tcp_include != NULL &&
            strstr(mca_oob_tcp_component.tcp_include,name) == NULL)
            continue;
        if (mca_oob_tcp_component.tcp_exclude != NULL &&
            strstr(mca_oob_tcp_component.tcp_exclude,name) != NULL)
            continue;
        opal_ifindextoaddr(i, (struct sockaddr*)&addr, sizeof(addr));
        if(opal_ifcount() > 1 && addr.sin_addr.s_addr == inet_addr("127.0.0.1"))
            continue;
        if(ptr != contact_info) {
            ptr += sprintf(ptr, ";");
        }
        ptr += sprintf(ptr, "tcp://%s:%d", inet_ntoa(addr.sin_addr),
                    ntohs(mca_oob_tcp_component.tcp_listen_port));
    }
    return contact_info;
}

/*
* Parse a URI string into an IP address and port number.
*/

int mca_oob_tcp_parse_uri(const char* uri, struct sockaddr_in* inaddr)
{
    char* tmp = strdup(uri);
    char* ptr = tmp + 6;
    char* addr = ptr;
    char* port;
    if(strncmp(tmp, "tcp://", 6) != 0) {
        free(tmp);
        return OMPI_ERR_BAD_PARAM;
    }

    ptr = strchr(addr, ':');
    if(NULL == ptr) {
        free(tmp);
        return OMPI_ERR_BAD_PARAM;
    }

    *ptr = '\0';
    ptr++;
    port = ptr;

    memset(inaddr, 0, sizeof(inaddr));
    inaddr->sin_family = AF_INET;
    inaddr->sin_addr.s_addr = inet_addr(addr);
    if(inaddr->sin_addr.s_addr == INADDR_ANY) {
        free(tmp);
        return OMPI_ERR_BAD_PARAM;
    }
    inaddr->sin_port = htons(atoi(port));
    free(tmp);
    return OMPI_SUCCESS;
}


/*
 * Setup address in the cache. Note that this could be called multiple
 * times if a given destination exports multiple addresses.
 */

int mca_oob_tcp_set_addr(const orte_process_name_t* name, const char* uri)
{
    struct sockaddr_in inaddr;
    mca_oob_tcp_addr_t* addr;
    mca_oob_tcp_peer_t* peer;
    int rc;
    if((rc = mca_oob_tcp_parse_uri(uri,&inaddr)) != OMPI_SUCCESS)
        return rc;

    OPAL_THREAD_LOCK(&mca_oob_tcp_component.tcp_lock);
    addr = (mca_oob_tcp_addr_t*)orte_hash_table_get_proc(&mca_oob_tcp_component.tcp_peer_names, name);
    if(NULL == addr) {
        addr = OBJ_NEW(mca_oob_tcp_addr_t);
        addr->addr_name = *name;
        orte_hash_table_set_proc(&mca_oob_tcp_component.tcp_peer_names, &addr->addr_name, addr);
    }
    rc = mca_oob_tcp_addr_insert(addr, &inaddr);
    peer = (mca_oob_tcp_peer_t *)orte_hash_table_get_proc(
        &mca_oob_tcp_component.tcp_peers, &addr->addr_name);
    if(NULL != peer) {
        mca_oob_tcp_peer_resolved(peer, addr);
    }
    OPAL_THREAD_UNLOCK(&mca_oob_tcp_component.tcp_lock);
    return rc;
}
