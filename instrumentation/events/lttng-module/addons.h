#undef TRACE_SYSTEM
#define TRACE_SYSTEM addons

#if !defined(LTTNG_ADDONS_H_) || defined(TRACE_HEADER_MULTI_READ)
#define LTTNG_ADDONS_H_

#include "../../../probes/lttng-tracepoint-event.h"
#include <linux/tcp.h>
#include <net/sock.h>
#include <linux/blkdev.h>
#include <linux/virtio.h>
#include <linux/vhost.h>
LTTNG_TRACEPOINT_EVENT_CLASS(inet_sock_local_template,
	TP_PROTO(struct sock *sk, struct tcphdr *tcph),
	TP_ARGS(sk, tcph),
	TP_FIELDS(
		ctf_integer_hex(struct sock *, sk, sk)
		ctf_integer_network_hex(uint32_t, seq, tcph->seq)
		ctf_integer_network_hex(uint32_t, ack_seq, tcph->ack_seq)
		ctf_integer_network_hex(uint16_t, check, tcph->check)
		ctf_integer_network_hex(uint16_t, window, tcph->window)
		ctf_integer_network_hex(uint16_t, flags, *(((uint16_t *)tcph) + 6))
	)
)

LTTNG_TRACEPOINT_EVENT_INSTANCE(inet_sock_local_template, inet_sock_local_in,
	TP_PROTO(struct sock *sk, struct tcphdr *tcph),
	TP_ARGS(sk, tcph))

LTTNG_TRACEPOINT_EVENT_INSTANCE(inet_sock_local_template, inet_sock_local_out,
	TP_PROTO(struct sock *sk, struct tcphdr *tcph),
	TP_ARGS(sk, tcph))

LTTNG_TRACEPOINT_EVENT(sched_ttwu,
	TP_PROTO(int tid),
	TP_ARGS(tid),
	TP_FIELDS(
		ctf_integer(int, tid, tid)
	)
)

LTTNG_TRACEPOINT_EVENT(sched_mc,
  TP_PROTO(int tid),
  TP_ARGS(tid),
  TP_FIELDS(
            ctf_integer(int, tid, tid)
  )
)


LTTNG_TRACEPOINT_EVENT_CLASS(vmsync_template,
	TP_PROTO(unsigned int cnt, unsigned long vm_uid),
	TP_ARGS(cnt, vm_uid),
	TP_FIELDS(
		ctf_integer(unsigned int, cnt, cnt)
		ctf_integer(unsigned long, vm_uid, vm_uid)
	)
)

LTTNG_TRACEPOINT_EVENT_INSTANCE(vmsync_template, vmsync_gh_guest,
	TP_PROTO(unsigned int cnt, unsigned long vm_uid),
	TP_ARGS(cnt, vm_uid))

LTTNG_TRACEPOINT_EVENT_INSTANCE(vmsync_template, vmsync_gh_host,
	TP_PROTO(unsigned int cnt, unsigned long vm_uid),
	TP_ARGS(cnt, vm_uid))

LTTNG_TRACEPOINT_EVENT_INSTANCE(vmsync_template, vmsync_hg_guest,
	TP_PROTO(unsigned int cnt, unsigned long vm_uid),
	TP_ARGS(cnt, vm_uid))

LTTNG_TRACEPOINT_EVENT_INSTANCE(vmsync_template, vmsync_hg_host,
	TP_PROTO(unsigned int cnt, unsigned long vm_uid),
	TP_ARGS(cnt, vm_uid))

LTTNG_TRACEPOINT_EVENT(addons_elv_merge_requests,
    TP_PROTO(struct request_queue *q, struct request *rq, struct request *next),
    TP_ARGS(q, rq, next),
    TP_FIELDS(
        ctf_integer_hex(struct request_queue *, queue, q)
        ctf_integer_hex(struct request *, curr, rq)
        ctf_integer_hex(struct request *, next, next)
    )
)


LTTNG_TRACEPOINT_EVENT(addons_vcpu_enter_guest,
    TP_PROTO(unsigned int vcpuID, ulong cr3tmp, unsigned long sptmp, unsigned long iptmp, unsigned int accessed),
    TP_ARGS(vcpuID,cr3tmp, sptmp, iptmp, accessed),
    TP_FIELDS(
	ctf_integer(unsigned int , vcpuID, vcpuID)
  	ctf_integer(ulong , cr3tmp, cr3tmp)
	ctf_integer(unsigned long , sptmp, sptmp)
	ctf_integer(unsigned long , iptmp, iptmp)
  	ctf_integer(unsigned int , accessed, accessed)
    )
)

LTTNG_TRACEPOINT_EVENT(addons_emulator_pio_in_out,
	TP_PROTO(int size, unsigned int count),
	TP_ARGS(size,count),
	TP_FIELDS(
		ctf_integer(int , size, size)
		ctf_integer(unsigned int , count, count)
	)
)

LTTNG_TRACEPOINT_EVENT(addons_handle_tx_kick,
	TP_PROTO(unsigned  done_seq),
	TP_ARGS(done_seq),
	TP_FIELDS(
		ctf_integer(unsigned , done_seq, done_seq)
	)
)


LTTNG_TRACEPOINT_EVENT(addons_handle_rx_kick,
	TP_PROTO(unsigned  done_seq),
	TP_ARGS(done_seq),
	TP_FIELDS(
		ctf_integer(unsigned , done_seq, done_seq)
	)
)

#endif /* LTTNG_ADDONS_H_ */

/* This part must be outside protection */
#include "../../../probes/define_trace.h"
