/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "RFC1213-MIB"
 * 	found in "RFC1213-MIB.asn1"
 * 	`asn1c -fskeletons-copy`
 */

#ifndef	_IpAddrEntry_H_
#define	_IpAddrEntry_H_


#include <asn_application.h>

/* Including external dependencies */
#include "IpAddress.h"
#include <INTEGER.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* IpAddrEntry */
typedef struct IpAddrEntry {
	IpAddress_t	 ipAdEntAddr;
	INTEGER_t	 ipAdEntIfIndex;
	IpAddress_t	 ipAdEntNetMask;
	INTEGER_t	 ipAdEntBcastAddr;
	long	 ipAdEntReasmMaxSize;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} IpAddrEntry_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IpAddrEntry;

#ifdef __cplusplus
}
#endif

#endif	/* _IpAddrEntry_H_ */
