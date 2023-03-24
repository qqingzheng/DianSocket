/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DianProtocol"
 * 	found in "dian.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_DianConnectionRequest_H_
#define	_DianConnectionRequest_H_


#include <asn_application.h>

/* Including external dependencies */
#include "User-Identity.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DianConnectionRequest */
typedef struct DianConnectionRequest {
	User_Identity_t	 userIdentity;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DianConnectionRequest_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DianConnectionRequest;

#ifdef __cplusplus
}
#endif

#endif	/* _DianConnectionRequest_H_ */
#include <asn_internal.h>