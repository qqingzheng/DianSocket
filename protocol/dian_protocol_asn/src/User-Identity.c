/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DianProtocol"
 * 	found in "dian.asn"
 */

#include "User-Identity.h"

/*
 * This type is implemented using NativeInteger,
 * so here we adjust the DEF accordingly.
 */
static const ber_tlv_tag_t asn_DEF_User_Identity_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (2 << 2))
};
asn_TYPE_descriptor_t asn_DEF_User_Identity = {
	"User-Identity",
	"User-Identity",
	&asn_OP_NativeInteger,
	asn_DEF_User_Identity_tags_1,
	sizeof(asn_DEF_User_Identity_tags_1)
		/sizeof(asn_DEF_User_Identity_tags_1[0]), /* 1 */
	asn_DEF_User_Identity_tags_1,	/* Same as above */
	sizeof(asn_DEF_User_Identity_tags_1)
		/sizeof(asn_DEF_User_Identity_tags_1[0]), /* 1 */
	{ 0, 0, NativeInteger_constraint },
	0, 0,	/* No members */
	0	/* No specifics */
};
