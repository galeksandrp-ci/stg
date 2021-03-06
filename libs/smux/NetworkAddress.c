/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "RFC1155-SMI"
 * 	found in "RFC1155-SMI.asn1"
 * 	`asn1c -S/mnt/data/software/asn1c/skeletons -fcompound-names -fwide-types`
 */

#include "NetworkAddress.h"

static asn_TYPE_member_t asn_MBR_NetworkAddress_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct NetworkAddress, choice.internet),
		(ASN_TAG_CLASS_APPLICATION | (0 << 2)),
		0,
		&asn_DEF_IpAddress,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"internet"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_NetworkAddress_tag2el_1[] = {
    { (ASN_TAG_CLASS_APPLICATION | (0 << 2)), 0, 0, 0 } /* internet */
};
static asn_CHOICE_specifics_t asn_SPC_NetworkAddress_specs_1 = {
	sizeof(struct NetworkAddress),
	offsetof(struct NetworkAddress, _asn_ctx),
	offsetof(struct NetworkAddress, present),
	sizeof(((struct NetworkAddress *)0)->present),
	asn_MAP_NetworkAddress_tag2el_1,
	1,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_NetworkAddress = {
	"NetworkAddress",
	"NetworkAddress",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	0,	/* No PER visible constraints */
	asn_MBR_NetworkAddress_1,
	1,	/* Elements count */
	&asn_SPC_NetworkAddress_specs_1	/* Additional specs */
};

