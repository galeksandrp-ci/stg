/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "RFC1157-SNMP"
 * 	found in "RFC1157-SNMP.asn1"
 * 	`asn1c -S/mnt/data/software/asn1c/skeletons -fcompound-names -fwide-types`
 */

#include "VarBind.h"

static asn_TYPE_member_t asn_MBR_VarBind_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct VarBind, name),
		(ASN_TAG_CLASS_UNIVERSAL | (6 << 2)),
		0,
		&asn_DEF_ObjectName,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"name"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct VarBind, value),
		-1 /* Ambiguous tag (CHOICE?) */,
		0,
		&asn_DEF_ObjectSyntax,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"value"
		},
};
static const ber_tlv_tag_t asn_DEF_VarBind_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_VarBind_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 1, 0, 0 }, /* number */
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 1, 0, 0 }, /* string */
    { (ASN_TAG_CLASS_UNIVERSAL | (5 << 2)), 1, 0, 0 }, /* empty */
    { (ASN_TAG_CLASS_UNIVERSAL | (6 << 2)), 0, 0, 1 }, /* name */
    { (ASN_TAG_CLASS_UNIVERSAL | (6 << 2)), 1, -1, 0 }, /* object */
    { (ASN_TAG_CLASS_APPLICATION | (0 << 2)), 1, 0, 0 }, /* internet */
    { (ASN_TAG_CLASS_APPLICATION | (1 << 2)), 1, 0, 0 }, /* counter */
    { (ASN_TAG_CLASS_APPLICATION | (2 << 2)), 1, 0, 0 }, /* gauge */
    { (ASN_TAG_CLASS_APPLICATION | (3 << 2)), 1, 0, 0 }, /* ticks */
    { (ASN_TAG_CLASS_APPLICATION | (4 << 2)), 1, 0, 0 } /* arbitrary */
};
static asn_SEQUENCE_specifics_t asn_SPC_VarBind_specs_1 = {
	sizeof(struct VarBind),
	offsetof(struct VarBind, _asn_ctx),
	asn_MAP_VarBind_tag2el_1,
	10,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_VarBind = {
	"VarBind",
	"VarBind",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_VarBind_tags_1,
	sizeof(asn_DEF_VarBind_tags_1)
		/sizeof(asn_DEF_VarBind_tags_1[0]), /* 1 */
	asn_DEF_VarBind_tags_1,	/* Same as above */
	sizeof(asn_DEF_VarBind_tags_1)
		/sizeof(asn_DEF_VarBind_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_VarBind_1,
	2,	/* Elements count */
	&asn_SPC_VarBind_specs_1	/* Additional specs */
};

