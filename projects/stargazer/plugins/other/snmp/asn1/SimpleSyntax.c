/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "RFC1155-SMI"
 * 	found in "RFC1155-SMI.asn1"
 * 	`asn1c -fskeletons-copy`
 */

#include <asn_internal.h>

#include "SimpleSyntax.h"

static asn_TYPE_member_t asn_MBR_SimpleSyntax_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SimpleSyntax, choice.number),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_INTEGER,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"number"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SimpleSyntax, choice.string),
		(ASN_TAG_CLASS_UNIVERSAL | (4 << 2)),
		0,
		&asn_DEF_OCTET_STRING,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"string"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SimpleSyntax, choice.object),
		(ASN_TAG_CLASS_UNIVERSAL | (6 << 2)),
		0,
		&asn_DEF_OBJECT_IDENTIFIER,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"object"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SimpleSyntax, choice.empty),
		(ASN_TAG_CLASS_UNIVERSAL | (5 << 2)),
		0,
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"empty"
		},
};
static asn_TYPE_tag2member_t asn_MAP_SimpleSyntax_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 0, 0, 0 }, /* number at 73 */
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 1, 0, 0 }, /* string at 76 */
    { (ASN_TAG_CLASS_UNIVERSAL | (5 << 2)), 3, 0, 0 }, /* empty at 82 */
    { (ASN_TAG_CLASS_UNIVERSAL | (6 << 2)), 2, 0, 0 } /* object at 79 */
};
static asn_CHOICE_specifics_t asn_SPC_SimpleSyntax_specs_1 = {
	sizeof(struct SimpleSyntax),
	offsetof(struct SimpleSyntax, _asn_ctx),
	offsetof(struct SimpleSyntax, present),
	sizeof(((struct SimpleSyntax *)0)->present),
	asn_MAP_SimpleSyntax_tag2el_1,
	4,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_SimpleSyntax = {
	"SimpleSyntax",
	"SimpleSyntax",
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
	asn_MBR_SimpleSyntax_1,
	4,	/* Elements count */
	&asn_SPC_SimpleSyntax_specs_1	/* Additional specs */
};

