/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DianProtocol"
 * 	found in "dian.asn"
 */

#ifndef	_DianSendMsg_H_
#define	_DianSendMsg_H_


#include <asn_application.h>

/* Including external dependencies */
#include "User-Identity.h"
#include "Message.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DianSendMsg */
typedef struct DianSendMsg {
	User_Identity_t	 userIdentity;
	Message_t	 msg;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DianSendMsg_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DianSendMsg;
extern asn_SEQUENCE_specifics_t asn_SPC_DianSendMsg_specs_1;
extern asn_TYPE_member_t asn_MBR_DianSendMsg_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _DianSendMsg_H_ */
#include <asn_internal.h>
