#ifndef _ASM_X8664_NUMA_H 
#define _ASM_X8664_NUMA_H 1

#include <xen/nodemask.h>
#include <xen/topology.h>
#include <asm/numnodes.h>
#include <asm/smp.h>

struct node { 
	u64 start,end; 
};

extern int compute_hash_shift(struct node *nodes, int numnodes);
extern int pxm_to_node(int nid);

#define ZONE_ALIGN (1UL << (MAX_ORDER+PAGE_SHIFT))
#define VIRTUAL_BUG_ON(x) 
#define NODEMAPSIZE 0xfff

extern void numa_add_cpu(int cpu);
extern void numa_init_array(void);
extern int numa_off;

extern void numa_set_node(int cpu, int node);

extern void setup_node_bootmem(int nodeid, u64 start, u64 end);
extern unsigned char apicid_to_node[256];
#ifdef CONFIG_NUMA
extern void __init init_cpu_to_node(void);

static inline void clear_node_cpumask(int cpu)
{
	clear_bit(cpu, &node_to_cpumask[cpu_to_node(cpu)]);
}

/* Simple perfect hash to map physical addresses to node numbers */
extern int memnode_shift; 
extern u8  memnodemap[NODEMAPSIZE]; 

extern struct node_data node_data[];

static inline __attribute__((pure)) int phys_to_nid(unsigned long addr) 
{ 
	unsigned nid; 
	VIRTUAL_BUG_ON((addr >> memnode_shift) >= NODEMAPSIZE);
	nid = memnodemap[addr >> memnode_shift]; 
	VIRTUAL_BUG_ON(nid >= MAX_NUMNODES || !node_data[nid]); 
	return nid; 
} 

#define NODE_DATA(nid)		(&(node_data[nid]))

#define node_start_pfn(nid)	(NODE_DATA(nid)->node_start_pfn)
#define node_end_pfn(nid)       (NODE_DATA(nid)->node_start_pfn + \
				 NODE_DATA(nid)->node_spanned_pages)


#else
#define init_cpu_to_node() do {} while (0)
#define clear_node_cpumask(cpu) do {} while (0)
#endif

#define NUMA_NO_NODE 0xff

#endif
