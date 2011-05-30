/*
 * Generated by asn1c-0.9.21 (http://lionet.info/asn1c)
 * From ASN.1 module "RFC1213-MIB"
 * 	found in "RFC1213-MIB.asn1"
 * 	`asn1c -fskeletons-copy`
 */

#ifndef	_IfEntry_H_
#define	_IfEntry_H_


#include <asn_application.h>

/* Including external dependencies */
#include <INTEGER.h>
#include "DisplayString.h"
#include "Gauge.h"
#include "PhysAddress.h"
#include "TimeTicks.h"
#include "Counter.h"
#include <OBJECT_IDENTIFIER.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* IfEntry */
typedef struct IfEntry {
	INTEGER_t	 ifIndex;
	DisplayString_t	 ifDescr;
	INTEGER_t	 ifType;
	INTEGER_t	 ifMtu;
	Gauge_t	 ifSpeed;
	PhysAddress_t	 ifPhysAddress;
	INTEGER_t	 ifAdminStatus;
	INTEGER_t	 ifOperStatus;
	TimeTicks_t	 ifLastChange;
	Counter_t	 ifInOctets;
	Counter_t	 ifInUcastPkts;
	Counter_t	 ifInNUcastPkts;
	Counter_t	 ifInDiscards;
	Counter_t	 ifInErrors;
	Counter_t	 ifInUnknownProtos;
	Counter_t	 ifOutOctets;
	Counter_t	 ifOutUcastPkts;
	Counter_t	 ifOutNUcastPkts;
	Counter_t	 ifOutDiscards;
	Counter_t	 ifOutErrors;
	Gauge_t	 ifOutQLen;
	OBJECT_IDENTIFIER_t	 ifSpecific;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} IfEntry_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IfEntry;

#ifdef __cplusplus
}
#endif

#endif	/* _IfEntry_H_ */
