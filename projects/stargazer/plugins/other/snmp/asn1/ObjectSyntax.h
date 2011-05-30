/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "RFC1155-SMI"
 * 	found in "RFC1155-SMI.asn1"
 * 	`asn1c -fskeletons-copy`
 */

#ifndef	_ObjectSyntax_H_
#define	_ObjectSyntax_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SimpleSyntax.h"
#include "ApplicationSyntax.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ObjectSyntax_PR {
	ObjectSyntax_PR_NOTHING,	/* No components present */
	ObjectSyntax_PR_simple,
	ObjectSyntax_PR_application_wide
} ObjectSyntax_PR;

/* ObjectSyntax */
typedef struct ObjectSyntax {
	ObjectSyntax_PR present;
	union ObjectSyntax_u {
		SimpleSyntax_t	 simple;
		ApplicationSyntax_t	 application_wide;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ObjectSyntax_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ObjectSyntax;

#ifdef __cplusplus
}
#endif

#endif	/* _ObjectSyntax_H_ */
