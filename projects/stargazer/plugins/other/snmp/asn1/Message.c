/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "RFC1157-SNMP"
 * 	found in "RFC1157-SNMP.asn1"
 * 	`asn1c -fskeletons-copy`
 */

#include <asn_internal.h>

#include "Message.h"

static asn_TYPE_member_t asn_MBR_Message_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct Message, version),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"version"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Message, community),
		(ASN_TAG_CLASS_UNIVERSAL | (4 << 2)),
		0,
		&asn_DEF_OCTET_STRING,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"community"
		},
	{ ATF_OPEN_TYPE | ATF_NOFLAGS, 0, offsetof(struct Message, data),
		-1 /* Ambiguous tag (ANY?) */,
		0,
		&asn_DEF_ANY,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"data"
		},
};
static ber_tlv_tag_t asn_DEF_Message_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_Message_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 0, 0, 0 }, /* version at 18 */
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 1, 0, 0 } /* community at 22 */
};
static asn_SEQUENCE_specifics_t asn_SPC_Message_specs_1 = {
	sizeof(struct Message),
	offsetof(struct Message, _asn_ctx),
	asn_MAP_Message_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_Message = {
	"Message",
	"Message",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_Message_tags_1,
	sizeof(asn_DEF_Message_tags_1)
		/sizeof(asn_DEF_Message_tags_1[0]), /* 1 */
	asn_DEF_Message_tags_1,	/* Same as above */
	sizeof(asn_DEF_Message_tags_1)
		/sizeof(asn_DEF_Message_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_Message_1,
	3,	/* Elements count */
	&asn_SPC_Message_specs_1	/* Additional specs */
};
