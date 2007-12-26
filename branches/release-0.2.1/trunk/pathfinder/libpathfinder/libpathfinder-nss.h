/*
 * libpathfinder-nss.h
 *
 * Copyright (C) 2007 Carillon Information Security Inc.
 *
 * This library is covered by the LGPL v2.1 or later, please read LICENSE for details.
 */
#ifndef __LIBPATHFINDER_NSS_H
#define __LIBPATHFINDER_NSS_H
#include <nss.h>
#include <prio.h>
#include <secitem.h>
#include <ssl.h>

extern "C" {
SECStatus nss_verify_cb(void *arg, PRFileDesc *socket, PRBool checksig, 
                        PRBool isServer);
}
#endif // __LIBPATHFINDER_NSS_H
