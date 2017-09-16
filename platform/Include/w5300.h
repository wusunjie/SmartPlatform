#ifndef _SMART_PLATFORM_W5300_H
#define _SMART_PLATFORM_W5300_H


#define W5300_MAP_ADDRESS	0x68000000

#define COMMON_REG_BASE     W5300_MAP_ADDRESS
#define SOCKET_REG_BASE     W5300_MAP_ADDRESS+0x0200

#define SOCKET_REG_SIZE    0x40

/** Maxmium number of socket */
#define	MAX_SOCK_NUM		8

/* Mode Register address */
#define MR              (W5300_MAP_ADDRESS)
/*
#define MR0             MR
#define MR1             (MR + 1)
*/

/* MODE register values */
#define MR_FIFOSWAP        0x100 // FIFO SWAP
#define MR_WR_TIMING(X)    ((X & 0x07) << 11)
#define MR_SAMPLING(X)     ((X & 0x03) << 9)
#define MR_RST             0x80 // reset
#define MR_MEMTEST         0x20 // memory test
#define MR_PB              0x10 // ping block
#define MR_PPPOE           0x08 // enable pppoe
#define MR_LB              0x04 // little or big endian selector in indirect mode
#define MR_IND             0x01 // enable indirect mode

/* Direct accessable register if W5300 bus mode is indirect(MR_IND is set in MR register) */ 
#define IDM_AR          (W5300_MAP_ADDRESS + 0x02)
/*
#define IDM_AR0         IDM_AR
#define IDM_AR1         (IDM_AR + 1)
*/

#define IDM_DR          (W5300_MAP_ADDRESS + 0x04)
/*
#define IDM_DR0         IDM_DR
#define IDM_DR1         (IDM_DR + 1)
*/

/* Interrupt Register */
#define IR              (COMMON_REG_BASE + 0x02)
/*
#define IR0             IR
#define IR1             (IR + 1);
*/

/* Interrupt mask register */
#define IMR             (COMMON_REG_BASE + 0x04)
/*
#define IMR0            IMR
#define IMR1            (IMR + 1)
*/

/* Interrupt De-asserted Time register */
#define ICFGR           (COMMON_REG_BASE + 0x06)
/*
#define ICFGR0          ICFGR
#define ICFGR1          (ICFGR0 + 1)
*/

/* Source MAC Register address */
#define SHAR            (COMMON_REG_BASE + 0x08)
/*
#define SHAR0           SHAR
#define SHAR1           (SHAR + 1)
#define SHAR2           (SHAR + 2)
#define SHAR3           (SHAR + 3)
#define SHAR4           (SHAR + 4)
#define SHAR5           (SHAR + 5)
*/

/* Gateway IP Register address */
#define GAR             (COMMON_REG_BASE + 0x10)
/*
#define GAR0            GAR
#define GAR1            (GAR + 1)
#define GAR2            (GAR + 2)
#define GAR3            (GAR + 3)
*/

/* Subnet mask Register address */
#define SUBR            (COMMON_REG_BASE + 0x14)
/*
#define SUBR0           SUBR
#define SUBR1           (SUBR + 1)
#define SUBR2           (SUBR + 2)
#define SUBR3           (SUBR + 3)
*/

/* Source IP Register address */
#define SIPR            (COMMON_REG_BASE + 0x18)
/*
#define SIPR0           SIPR
#define SIPR1           (SIPR + 1)
#define SIPR2           (SIPR + 2)
#define SIPR3           (SIPR + 3)
*/

/* Timeout register address
1 is 100us */
#define RTR             (COMMON_REG_BASE + 0x1C)
/*
#define RTR0            RTR
#define RTR1            (RTR + 1)
*/

/* Retry count reigster */
#define RCR             (COMMON_REG_BASE + 0x1E)
/*
#define RCR0            RCR
#define RCR1            (RCR + 1)
*/

/* Transmit memory size reigster */
#define TMS01R          (COMMON_REG_BASE + 0x20)
#define TMS23R          (TMS01R + 2)
#define TMS45R          (TMS01R + 4)
#define TMS67R          (TMS01R + 6)
/*
#define TMSR0           TMS01R
#define TMSR1           (TMSR0 + 1)
#define TMSR2           (TMSR0 + 2)
#define TMSR3           (TMSR0 + 3)
#define TMSR4           (TMSR0 + 4)
#define TMSR5           (TMSR0 + 5)
#define TMSR6           (TMSR0 + 6)
#define TMSR7           (TMSR0 + 7)
*/

/* Receive memory size reigster */
#define RMS01R          (COMMON_REG_BASE + 0x28)
#define RMS23R          (RMS01R + 2)
#define RMS45R          (RMS01R + 4)
#define RMS67R          (RMS01R + 6)
/*
#define RMSR0           RMS01R
#define RMSR1           (RMSR0 + 1)
#define RMSR2           (RMSR0 + 2)
#define RMSR3           (RMSR0 + 3)
#define RMSR4           (RMSR0 + 4)
#define RMSR5           (RMSR0 + 5)
#define RMSR6           (RMSR0 + 6)
#define RMSR7           (RMSR0 + 7)
*/


/** Memory Type Register
'1' - TX memory
'0' - RX memory
**/
#define MTYPER          (COMMON_REG_BASE + 0x30)
/*
#define MTYPER0         MTYPER
#define MTYPER1         (MTYPER + 1)
*/

/* Authentication type register address in PPPoE mode */
#define PATR            (COMMON_REG_BASE + 0x32)
/*
#define PATR0           PATR
#define PATR1           (PATR + 1)
*/

#define PPPALGOR      (COMMON_REG_BASE + 0x34)
/*
#define PPPALGOR0     PPPALGOR
#define PPPALGOR1     (PPPALGOR + 1)
*/

#define PTIMER          (COMMON_REG_BASE + 0x36)
/*
#define PTIMER0         PTIMER
#define PTIMER1         (PTIMER + 1)
*/

#define PMAGICR         (COMMON_REG_BASE + 0x38)
/*
#define PMAGICR0        PMAGICR
#define PMAGICR1        (PMAGICR + 1)
*/

#define PSTATER       (COMMON_REG_BASE + 0x3A)
/*
#define PSTATER0      PSTATER
#define PSTATER1      (PSTATER + 1)
*/

#define PSIDR           (COMMON_REG_BASE + 0x3C)
/*
#define PSIDR0          PSIDR
#define PSIDR1          (PSIDR + 1)
*/

#define PDHAR           (COMMON_REG_BASE + 0x40)
/*
#define PDHAR0          PDHAR
#define PDHAR1          (PDHAR + 1)
#define PDHAR2          (PDHAR + 2)
#define PDHAR3          (PDHAR + 3)
#define PDHAR4          (PDHAR + 4)
#define PDHAR5          (PDHAR + 5)
*/

/* Unreachable IP register address in UDP mode */
#define UIPR            (COMMON_REG_BASE + 0x48)
/*
#define UIPR0           UIPR
#define UIPR1           (UIPR + 1)
#define UIPR2           (UIPR + 2)
#define UIPR3           (UIPR + 3)
*/

/* Unreachable Port register address in UDP mode */
#define UPORTR          (COMMON_REG_BASE + 0x4C)
/*
#define UPORTR0         UPORTR
#define UPORTR1         (UPORT + 1)
*/

/* Fragment Register */
#define FMTUR           (COMMON_REG_BASE + 0x4E)
/*
#define FMTUR0          FMTUR
#define FMTUR1          (FMTUR + 1)
*/

/* Retransmitted Count Register of SOCKET n */
#define Sn_RTCR(n)      (COMMON_REG_BASE + 0x50 + n)
/*
#define Sn_RTCR0(n)     Sn_RTCR(n)
#define Sn_RTCR1(n)     (Sn_RTCR(n)+1)
*/

/* Buffer Ready Config Register of BRDYn PIN (0 <= n <= 3) */
#define Pn_BRDYR(n)     (COMMON_REG_BASE + 0x60 + n*4)
/*
#define Pn_BRDYR0(n)    Pn_BRDYR(n)
#define Pn_BRDYR1(n)    (Pn_BRDYR(n) + 1)
*/

/* Buffer Depth Config Register of BRYNn PIN (0 <= n <= 3) */
#define Pn_BDPTHR(n)    (COMMON_REG_BASE + 0x60 + n*4 + 2)
/*
#define Pn_BDPTHR0(n)   Pn_BDPTHR(n)
#define Pn_BDPTHR1(n)   (Pn_BDPTHR(n) + 1)
*/

/* W5300 ID register */
#define IDR             (COMMON_REG_BASE + 0xFE)
/*
#define IDR1            (IDR + 1)
*/

/********************************* socket register ****************************/
/* Mode register of SOCKET n */
#define Sn_MR(n)        (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x00)
/*
#define Sn_MR0(n)       Sn_MR(n)
#define Sn_MR1(n)       (Sn_MR(n)+1)
*/

/* Command register of SOCKET n */
#define Sn_CR(n)        (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x02)
/*
#define Sn_CR0(n)       Sn_CR(n)
#define Sn_CR1(n)       (Sn_CR(n) + 1)
*/

/* Interrupt mask register of SOCKET n */
#define Sn_IMR(n)       (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x04)
/*
#define Sn_IMR0(n)      Sn_IMR(n)
#define Sn_IMR1(n)      (Sn_IMR(n)+1)
*/

/* Interrupt register of SOCKET n */
#define Sn_IR(n)        (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x06)
/*
#define Sn_IR0(n)       Sn_IR(n)
#define Sn_IR1(n)       (Sn_IR(n)+1)
*/

/* socket status register */
#define Sn_SSR(n)       (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x08)
/*
#define Sn_SSR0(n)      Sn_SSR(n)
#define Sn_SSR1(n)      (Sn_SSR(n)+1)
*/

/* source port register */
#define Sn_PORTR(n)     (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x0A)
/*
#define Sn_PORTR0(n)    Sn_PORTR(n)
#define Sn_PORTR1(n)    (Sn_PORTR(n)+1)
*/

/* Peer MAC register address */
#define Sn_DHAR(n)      (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x0C)
/*
#define Sn_DHAR0(n)     Sn_DHAR(n)
#define Sn_DHAR1(n)     (Sn_DHAR(n)+1)
#define Sn_DHAR2(n)     (Sn_DHAR(n)+2)
#define Sn_DHAR3(n)     (Sn_DHAR(n)+3)
#define Sn_DHAR4(n)     (Sn_DHAR(n)+4)
#define Sn_DHAR5(n)     (Sn_DHAR(n)+5)
*/

/* Destination port register of SOCKET n */
#define Sn_DPORTR(n)    (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x12)
/*
#define Sn_DPORTR0(n)   Sn_DPORTR(n)
#define Sn_DPORTR1(n)   (Sn_DPORTR(n)+1)
*/

/* Destination IP register of SOCKET n */
#define Sn_DIPR(n)      (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x14)
/*
#define Sn_DIPR0(n)     Sn_DIPR(n)
#define Sn_DIPR1(n)     (Sn_DIPR(n)+1)
#define Sn_DIPR2(n)     (Sn_DIPR(n)+2)
#define Sn_DIPR3(n)     (Sn_DIPR(cnnh)+3)
*/

/* Maximum Segment Size register of SOCKET n */
#define Sn_MSSR(n)      (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x18)
/*
#define Sn_MSSR0(n)     Sn_MSSR(n)
#define Sn_MSSR1(n)     (Sn_MSSR(n)+1)
*/

/* Protocol of IP Header field register of SOCKET n */
#define Sn_PROTOR(n)		(SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x1A)
/*
#define Sn_KPALVTR(n)   Sn_PROTOR(n)
#define Sn_PROTOR1(n)   (Sn_PROTOR(n)+1)
*/

/* IP Type of Service(TOS) Register of SOCKET n */
#define Sn_TOSR(n)      (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x1C)
/*
#define Sn_TOSR0(n)     Sn_TOSR(n)
#define Sn_TOSR1(n)     (Sn_TOSR(n)+1)
*/

/* IP Time to live(TTL) Register of SOCKET n */
#define Sn_TTLR(n)      (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x1E)
/*
#define Sn_TTLR0(n)     Sn_TTLR(n)
#define Sn_TTLR1(n)     (Sn_TTLR(n)+1)
*/

/* Transmit Size Register of SOCKET n (Byte count) */
#define Sn_TX_WRSR(n)		(SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x20)
/*
#define Sn_TX_WRSR0(n)		Sn_TX_WRSR(n)
#define Sn_TX_WRSR1(n)		(Sn_TX_WRSR(n) + 1)
#define Sn_TX_WRSR2(n)		(Sn_TX_WRSR(n) + 2)
#define Sn_TX_WRSR3(n)		(Sn_TX_WRSR(n) + 3)
*/

/* Transmit free memory size register of SOCKET n (Byte count) */
#define Sn_TX_FSR(n)       (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x0024)
/*
#define Sn_TX_FSR0(n)      Sn_TX_FSR(n)
#define Sn_TX_FSR1(n)      (Sn_TX_FSR(n) + 1)
#define Sn_TX_FSR2(n)      (Sn_TX_FSR(n) + 2)
#define Sn_TX_FSR3(n)      (Sn_TX_FSR(n) + 3)
*/

/* Received data size register of SOCKET n (Byte count) */
#define Sn_RX_RSR(n)       (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x0028)
/*
#define Sn_RX_RSR0(n)      Sn_RX_RSR(n)
#define Sn_RX_RSR1(n)      (Sn_RX_RSR(n) + 1)
#define Sn_RX_RSR2(n)      (Sn_RX_RSR(n) + 2)
#define Sn_RX_RSR3(n)      (Sn_RX_RSR(n) + 3)
*/

#define Sn_FRAGR(n)        (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x002C)
/*
#define Sn_FRAGR0(n)       Sn_FRAGR(n)
#define Sn_FRAGR1(n)       (Sn_FRAGR(n) + 1)
*/

/* FIFO register of SOCKET n, For data transmittion */
#define Sn_TX_FIFOR(n)     (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x2E)
/*
#define Sn_TX_FIFOR0(n)    Sn_TX_FIFO(n)
#define Sn_TX_FIFOR1(n)    (Sn_TX_FIFO(n) + 1)
*/

/* FIFO register of SOCKET n, For data reception */
#define Sn_RX_FIFOR(n)     (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x30)
/*
#define Sn_RX_FIFOR0(n)    Sn_RX_FIFO(n)
#define Sn_RX_FIFOR1(n)    (Sn_RX_FIFO(n) + 1)
*/

/* TX Memory Base Address Register of SOCKET n */
#define Sn_TX_SADR(n)      (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x32)
/*
#define Sn_TX_SADR0(n)     Sn_TX_SADR(n)
#define Sn_TX_SADR1(n)     (Sn_TX_SADR(n) + 1)
*/

/* RX Memory Base Address Register of SOCKET n */
#define Sn_RX_SADR(n)      (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x34)
/*
#define Sn_RX_SADR0(n)     Sn_RX_SADR(n)
#define Sn_RX_SADR1(n)     (Sn_RX_SADR(n) + 1)
*/

/* TX memory read pointer register of SOCKET n */
#define Sn_TX_RD(n)        (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x36)
/*
#define Sn_TX_RD0(n)       (Sn_TX_RD(n) + 1)
#define Sn_TX_RD1(n)       (Sn_TX_RD(n) + 1)
*/

/* TX memory write pointer register of SOCKET n */
#define Sn_TX_WR(n)        (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x38)
/*
#define Sn_TX_WR0(n)       Sn_TX_WR(n)
#define Sn_TX_WR1(n)       (Sn_TX_WR(n) + 1)
*/

/* TX memory ack pointer register of SOCKET n */
#define Sn_TX_ACK(n)       (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x3A)
/*
#define Sn_TX_ACK0(n)      (Sn_TX_ACK(n) + 1)
#define Sn_TX_ACK1(n)      (Sn_TX_ACK(n) + 1)
*/

/* RX memory read pointer register of SOCKET n */
#define Sn_RX_RD(n)		   (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x3C)
/*
#define Sn_RX_RD0(n)		Sn_RX_RD(n)
#define Sn_RX_RD1(n)	   (Sn_RX_RD(n) + 1)
*/

/* RX memory write pointer of SOCKET n */
#define Sn_RX_WR(n)        (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x3E)
/*
#define Sn_RX_WR0(n)        Sn_RX_WR(n)
#define Sn_RX_WR1(n)       (Sn_RX_WR(n) + 1)
*/

/* IR register values */
#define IR_CONFLICT			0x80 // check ip confict
#define IR_UNREACH			0x40 // get the destination unreachable message in UDP sending
#define IR_PPPoE			0x20 // get the PPPoE close message
#define IR_FMTU				0x10
#define IR_SOCK(ch)			(0x01 << ch) // check socket interrupt

/* Sn_MR values */
#define Sn_MR_ALIGN        0x100 // 2 Byte Alignment Data Trnascation(No use TCP Header)
#define Sn_MR_MULTI        0x80 // support multicating
//#define Sn_MR_MF         0x40
#define Sn_MR_ND           0x20 // No Delayed Ack(TCP) flag
//#define Sn_MR_ZC         0x10


#define Sn_MR_CLOSE        0x00 // unused socket
#define Sn_MR_TCP          0x01 // TCP
#define Sn_MR_UDP          0x02 // UDP
#define Sn_MR_IPRAW        0x03 // IP LAYER RAW SOCK
#define Sn_MR_MACRAW       0x04 // MAC LAYER RAW SOCK
#define Sn_MR_PPPOE        0x05 // PPPoE


/* Sn_CR values */
#define Sn_CR_OPEN         0x01 // initialize or open socket
#define Sn_CR_LISTEN       0x02 // wait connection request in tcp mode(Server mode)
#define Sn_CR_CONNECT      0x04 // send connection request in tcp mode(Client mode)
#define Sn_CR_DISCON       0x08 // send closing reqeuset in tcp mode
#define Sn_CR_CLOSE        0x10 // close socket
#define Sn_CR_SEND         0x20 // updata txbuf pointer, send data
#define Sn_CR_SEND_MAC     0x21 // send data with MAC address, so without ARP process
#define Sn_CR_SEND_KEEP    0x22 // send keep alive message
#define Sn_CR_RECV         0x40 // update rxbuf pointer, recv data

#define Sn_CR_PCON      0x23 //
#define Sn_CR_PDISCON   0x24 //
#define Sn_CR_PCR       0x25 //
#define Sn_CR_PCN       0x26 //
#define Sn_CR_PCJ       0x27 //

/* Sn_IR values */
#define Sn_IR_PRECV     0x80 //
#define Sn_IR_PFAIL     0x40 //
#define Sn_IR_PNEXT     0x20 //

#define Sn_IR_SEND_OK      0x10 // complete sending
#define Sn_IR_TIMEOUT      0x08 // assert timeout
#define Sn_IR_RECV         0x04 // receiving data
#define Sn_IR_DISCON       0x02 // closed socket
#define Sn_IR_CON          0x01 // established connection

/* Sn_SSR values */
#define SOCK_CLOSED        0x00 // closed
#define SOCK_INIT          0x13 // init state
#define SOCK_LISTEN        0x14 // listen state
#define SOCK_SYNSENT       0x15 // connection state
#define SOCK_SYNRECV       0x16 // connection state
#define SOCK_ESTABLISHED   0x17 // success to connect
#define SOCK_FIN_WAIT      0x18 // closing state
#define SOCK_CLOSING       0x1A // closing state
#define SOCK_TIME_WAIT     0x1B // closing state
#define SOCK_CLOSE_WAIT    0x1C // closing state
#define SOCK_LAST_ACK      0x1D // closing state
#define SOCK_UDP           0x22 // udp socket
#define SOCK_IPRAW         0x32 // ip raw mode socket
#define SOCK_MACRAW        0x42 // mac raw mode socket
#define SOCK_PPPOE         0x5F // pppoe socket

/* IP PROTOCOL */
#define IPPROTO_IP         0   /* Dummy for IP */
#define IPPROTO_ICMP       1   /* Control message protocol */
#define IPPROTO_IGMP       2   /* Internet group management protocol */
#define IPPROTO_GGP        3   /* Gateway^2 (deprecated) */
#define IPPROTO_TCP        6   /* TCP */
#define IPPROTO_PUP        12  /* PUP */
#define IPPROTO_UDP        17  /* UDP */
#define IPPROTO_IDP        22  /* XNS idp */
#define IPPROTO_ND         77  /* UNOFFICIAL net disk protocol */
#define IPPROTO_RAW        255 /* Raw IP packet */


/* Buffer Ready Pin Config Value*/
#define Pn_BRDYR_EN        (1<<7)
#define Pn_BRDYR_TX        (1<<6)
#define Pn_BRDYR_HIGH      (1<<5)
#define Pn_BRDYR_CH(n)     (n & 0x07)

#endif