/*
 * addons/lttng-memcheck.c
 *
 * Try to wake up event
 *
 * Copyright (C) 2013 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
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

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/kprobes.h>
#include <linux/spinlock.h>

#include <linux/mm.h>
#include <linux/utsname.h>
#include <asm/pgtable.h>
#include <linux/string.h>


#include "../wrapper/tracepoint.h"
#include "../lttng-abi.h"
#define LTTNG_INSTRUMENTATION
#include "../instrumentation/events/lttng-module/addons.h"

DEFINE_TRACE(sched_mc);

#define SCHED_SWITCH_TP "sched_switch"


bool page_by_address(struct task_struct *task, const unsigned long addr)
{
    pgd_t *pgd;
    pte_t *ptep, pte;
    pud_t *pud;
    pmd_t *pmd;
    bool accessed = false;
    struct mm_struct *mm = task->mm;
    pgd = pgd_offset(mm, addr);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
        goto out;
    pud = pud_offset(pgd, addr);
    if (pud_none(*pud) || pud_bad(*pud))
        goto out;
    pmd = pmd_offset(pud, addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
        goto out;
    ptep = pte_offset_map(pmd, addr);
    if (!ptep)
        goto out;
    pte = *ptep;
    if (pte_young(pte))
	accessed = true;
    pte = pte_mkold(pte);
    *ptep = pte;
out:
    return accessed;
}
static void mc_probe(void *__data, struct task_struct *task, int success)
{
	struct mm_struct *mm;
	if (strcmp(task->comm,"qemu-system-x86") == 0){
			mm = task->mm;
			unsigned int page_accessed = 0;
			const struct vm_area_struct *vma = mm->mmap;
			while (vma != NULL) 
			{
				unsigned long address;
				for (address = vma->vm_start; address < vma->vm_end; address += PAGE_SIZE) 
				{	
			  	 	if (page_by_address(task, address))
						page_accessed++;
		  	}
				vma = vma->vm_next;
				}
	  	trace_sched_mc(page_accessed);
		}
}
static int __init lttng_addons_mc_init(void)
{
	int ret;

	(void) wrapper_lttng_fixup_sig(THIS_MODULE);
	ret = lttng_wrapper_tracepoint_probe_register(SCHED_SWITCH_TP, mc_probe, NULL);
	if (ret) {
		printk("Failed to register probe, returned %d\n", ret);
		goto err;
	}

	printk("lttng-mc loaded (tracepoint)\n");
	return 0;
err:
	lttng_wrapper_tracepoint_probe_unregister(SCHED_SWITCH_TP, mc_probe, NULL);
	return -1;
}
module_init(lttng_addons_mc_init);

static void __exit lttng_addons_mc_exit(void)
{
	lttng_wrapper_tracepoint_probe_unregister(SCHED_SWITCH_TP, mc_probe, NULL);
	printk("lttng-mc removed\n");
}
module_exit(lttng_addons_mc_exit);

MODULE_LICENSE("GPL and additional rights");
MODULE_AUTHOR("Hani Nemati <hani.nemati@gmail.com>");
MODULE_DESCRIPTION("LTTng sched_mc event");

