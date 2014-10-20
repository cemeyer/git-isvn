#ifndef _ISVN_ISVN_HASHMAP_H_
#define _ISVN_ISVN_HASHMAP_H_

#ifdef __GLIBC__
#include <bsd/sys/queue.h>
#else
#include <sys/queue.h>
#endif

/* TODO: Clean up API now that we are divorced of git(1). */

typedef int (*hashmap_cmp_fn)(const void *, const void *, const void *);

struct hashmap_entry {
	SLIST_ENTRY(hashmap_entry)		he_list;
	unsigned				he_hash;
};

struct hashmap {
	SLIST_HEAD(_hmehd, hashmap_entry)	*hm_table;
	hashmap_cmp_fn				 hm_cmpfn;
	size_t					 hm_size, hm_buckets;
};

static inline bool
hashmap_isempty(struct hashmap *map)
{
	return (map->hm_size == 0);
}

struct hashmap_iter {
	const struct hashmap			*hi_hash;
	const struct hashmap_entry		*hi_next;
	size_t					 hi_bucket;
};

static inline void
hashmap_entry_init(struct hashmap_entry *en, unsigned hash)
{

	SLIST_NEXT(en, he_list) = NULL;
	en->he_hash = hash;
}

static inline void
hashmap_iter_init(const struct hashmap *hm, struct hashmap_iter *iter)
{
	iter->hi_hash = hm;
	iter->hi_next = NULL;
	iter->hi_bucket = 0;
}

void hashmap_init(struct hashmap *, hashmap_cmp_fn, size_t initial_size);
void *hashmap_get(const struct hashmap *, /* const struct hashmap_entry * */const void *,
    const void *dummy);
void *hashmap_remove(struct hashmap *, /* const struct hashmap_entry * */const void *,
    const void *dummy);
void hashmap_add(struct hashmap *, /*struct hashmap_entry * */ void*);
void *hashmap_iter_next(struct hashmap_iter *);
void hashmap_free(struct hashmap *, bool);

void MurmurHash3_128(const void *, int, uint32_t, void *);

#endif
