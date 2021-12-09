#pragma once

typedef unsigned char      uint8_t;		// BYTE
typedef unsigned short     uint16_t;		// WORD
typedef unsigned int       uint32_t;		// DWORD

typedef struct tagPCAPFILEHDR {			// 24 �����
	uint32_t magic;
	uint16_t version_major;
	uint16_t version_minor;
	uint32_t thiszone;
	uint32_t sigfigs;
	uint32_t snap_len;
	uint32_t linktype;
} PCAPFILEHDR;


typedef struct tagtimeval {
	uint32_t sec;
	uint32_t usec;
} tVal;

typedef struct tagPCAPPKTHDR {
	tVal ts;
	uint32_t caplen;
	uint32_t len;
} PCAPPKTHDR;

typedef struct tagIPHDR {
	uint8_t ihl : 4,
		version : 4;
	uint8_t tos;
	uint16_t tot_len;
	uint16_t id;
	uint16_t frag_off;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t check;
	struct in_addr saddr;
	struct in_addr daddr;
} IPHDR, *PIPHDR;

typedef struct tagICMPHDR {
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
	union {
		struct {
			uint16_t id;
			uint16_t sequence;
		} echo;
		uint32_t gateway;
		struct {
			uint16_t reserved;
			uint16_t mtu;
		} frag;
	} un;
} ICMPHDR, *PICMPHDR;

typedef struct tagTCPHDR {
	uint16_t source;
	uint16_t dest;
	uint32_t seq;
	uint32_t ack_seq;
	uint8_t res;
	uint8_t flags;
	uint16_t window;
	uint16_t check;
	uint16_t urg_ptr;
} TCPHDR, *PTCPHDR;

typedef struct tagEVENT {
	IPHDR ip;
	union {
		ICMPHDR icmp;
		TCPHDR tcp;
		//     UDPHDR udp
	};
	uint8_t rezerved[8000];
} EVENT, *PEVENT;

