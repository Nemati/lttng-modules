/*
 * addons/lttng-virtio-kick.c
 *
 * FIXME: Random experiment
 *
 * Copyright (C) 2015 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; only
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kprobes.h>
#include <net/sock.h>
#include <linux/spinlock.h>
#include <linux/vhost.h>
#include "../wrapper/tracepoint.h"
#include "../lttng-abi.h"
#include <linux/kvm_host.h>
#include "vhost.h"
#include "../instrumentation/events/lttng-module/x86.h"
#include "../instrumentation/events/lttng-module/kvm_cache_regs.h"
#define LTTNG_INSTRUMENTATION
#include "../instrumentation/events/lttng-module/addons.h"

DEFINE_TRACE(addons_handle_tx_kick);



static void
lttng_handle_tx_kick_probe(struct vhost_work *work)
{
   unsigned  done_seq = work->done_seq;
   trace_addons_handle_tx_kick(done_seq);
   jprobe_return();
}

static struct jprobe handle_tx_kick_jprobe = {
		.entry = lttng_handle_tx_kick_probe,
		.kp = {
			.symbol_name = "handle_tx_kick",
		},
};

static int __init lttng_addons_handle_tx_kick_init(void)
{
	int ret;

//	(void) wrapper_lttng_fixup_sig(THIS_MODULE);
	ret = register_jprobe(&handle_tx_kick_jprobe);
	if (ret < 0) {
		printk("register_jprobe failed, returned %d\n", ret);
		goto out;
	}

	printk("handle_tx_kick loaded\n");
out:
	return ret;
}
module_init(lttng_addons_handle_tx_kick_init);

static void __exit lttng_addons_handle_tx_kick_exit(void)
{
	unregister_jprobe(&handle_tx_kick_jprobe);
	printk("handle_tx_kick removed\n");
}
module_exit(lttng_addons_handle_tx_kick_exit);

MODULE_LICENSE("GPL and additional rights");
MODULE_AUTHOR("Hani Nemati <hani.nemati@gmail.com>");
MODULE_DESCRIPTION("LTTng handle_tx_kick event");

