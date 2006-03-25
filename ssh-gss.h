/* $OpenBSD: ssh-gss.h,v 1.6 2006/03/25 22:22:43 djm Exp $ */
/*
 * Copyright (c) 2001-2003 Simon Wilkinson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR `AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SSH_GSS_H
#define _SSH_GSS_H

#ifdef GSSAPI

#include "buffer.h"

#include <gssapi.h>

/* draft-ietf-secsh-gsskeyex-06 */
#define SSH2_MSG_USERAUTH_GSSAPI_RESPONSE		60
#define SSH2_MSG_USERAUTH_GSSAPI_TOKEN			61
#define SSH2_MSG_USERAUTH_GSSAPI_EXCHANGE_COMPLETE	63
#define SSH2_MSG_USERAUTH_GSSAPI_ERROR			64
#define SSH2_MSG_USERAUTH_GSSAPI_ERRTOK			65
#define SSH2_MSG_USERAUTH_GSSAPI_MIC			66

#define SSH_GSS_OIDTYPE 0x06

typedef struct {
	char *filename;
	char *envvar;
	char *envval;
	void *data;
} ssh_gssapi_ccache;

typedef struct {
	gss_buffer_desc displayname;
	gss_buffer_desc exportedname;
	gss_cred_id_t creds;
	struct ssh_gssapi_mech_struct *mech;
	ssh_gssapi_ccache store;
} ssh_gssapi_client;

typedef struct ssh_gssapi_mech_struct {
	char *enc_name;
	char *name;
	gss_OID_desc oid;
	int (*dochild) (ssh_gssapi_client *);
	int (*userok) (ssh_gssapi_client *, char *);
	int (*localname) (ssh_gssapi_client *, char **);
	void (*storecreds) (ssh_gssapi_client *);
} ssh_gssapi_mech;

typedef struct {
	OM_uint32	major; /* both */
	OM_uint32	minor; /* both */
	gss_ctx_id_t	context; /* both */
	gss_name_t	name; /* both */
	gss_OID		oid; /* client */
	gss_cred_id_t	creds; /* server */
	gss_name_t	client; /* server */
	gss_cred_id_t	client_creds; /* server */
} Gssctxt;

extern ssh_gssapi_mech *supported_mechs[];

int  ssh_gssapi_check_oid(Gssctxt *, void *, size_t);
void ssh_gssapi_set_oid_data(Gssctxt *, void *, size_t);
void ssh_gssapi_set_oid(Gssctxt *, gss_OID);
void ssh_gssapi_supported_oids(gss_OID_set *);
ssh_gssapi_mech *ssh_gssapi_get_ctype(Gssctxt *);

OM_uint32 ssh_gssapi_import_name(Gssctxt *, const char *);
OM_uint32 ssh_gssapi_acquire_cred(Gssctxt *);
OM_uint32 ssh_gssapi_init_ctx(Gssctxt *, int,
    gss_buffer_desc *, gss_buffer_desc *, OM_uint32 *);
OM_uint32 ssh_gssapi_accept_ctx(Gssctxt *,
    gss_buffer_desc *, gss_buffer_desc *, OM_uint32 *);
OM_uint32 ssh_gssapi_getclient(Gssctxt *, ssh_gssapi_client *);
void ssh_gssapi_error(Gssctxt *);
char *ssh_gssapi_last_error(Gssctxt *, OM_uint32 *, OM_uint32 *);
void ssh_gssapi_build_ctx(Gssctxt **);
void ssh_gssapi_delete_ctx(Gssctxt **);
OM_uint32 ssh_gssapi_sign(Gssctxt *, gss_buffer_t, gss_buffer_t);
OM_uint32 ssh_gssapi_server_ctx(Gssctxt **, gss_OID);
void ssh_gssapi_buildmic(Buffer *, const char *, const char *, const char *);

/* In the server */
int ssh_gssapi_userok(char *name);
OM_uint32 ssh_gssapi_checkmic(Gssctxt *, gss_buffer_t, gss_buffer_t);
void ssh_gssapi_do_child(char ***, u_int *);
void ssh_gssapi_cleanup_creds(void);
void ssh_gssapi_storecreds(void);

#endif /* GSSAPI */

#endif /* _SSH_GSS_H */
