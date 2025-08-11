/* internal_umalloc.c - memory allocator implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/internal.h>
#include <libf/ds/list.h>


/* @def: umalloc
* head | node <--> node <--> node --> end
*        node  | { list, size, chunk }
*        chunk | chunk <--> chunk --> end
*                { prev_size, size|flags }<alloc>
*/
#undef CHUNK_NODE_SIZE
#define CHUNK_NODE_SIZE sizeof(struct umalloc_chunk_node)
#undef CHUNK_SIZE
#define CHUNK_SIZE sizeof(struct umalloc_chunk)

/* 32/64bit are 8/16byte aligend */
#undef CHUNK_ALIGNED_SIZE
#define CHUNK_ALIGNED_SIZE(x) (16 * (((x) + CHUNK_SIZE + 15) / 16))
#undef CHUNK_NODE_ALIGNED_SIZE
#define CHUNK_NODE_ALIGNED_SIZE(x) (16 * (((x) + 15) / 16))

#undef NEXT_CHUNK
#define NEXT_CHUNK(x) \
	((struct umalloc_chunk *)((char *)(x) + GET_SIZE(x)))
#undef PREV_CHUNK
#define PREV_CHUNK(x) \
	((struct umalloc_chunk *)((char *)(x) - (x)->prev_size))

#undef GET_INUSE
#define GET_INUSE(x) ((x)->size_flags & 0x01)
#undef GET_END
#define GET_END(x) ((x)->size_flags & 0x02)
#undef GET_BIG
#define GET_BIG(x) ((x)->size_flags & 0x04)
#undef GET_SIZE
#define GET_SIZE(x) ((x)->size_flags & ~0x07)
#undef SET_SIZE
#define SET_SIZE(x) ((x) & ~0x07)
#undef FG_INUSE
#define FG_INUSE 0x01
#undef FG_END
#define FG_END 0x02
#undef FG_BIG
#define FG_BIG 0x04

#undef ALLOC_CHUNK_SIZE
#define ALLOC_CHUNK_SIZE (2 << 17)
#undef ALLOC_SIZE_MAX
#define ALLOC_SIZE_MAX (1 << 17)
/* end */

/* @func: _umalloc_new_node (static) - allocate a memory chunk
* @param1: struct umalloc_ctx * # umalloc context struct
* @param2: uint32               # chunk size
* @return: void *               # chunk pointer
*/
static void *_umalloc_new_node(struct umalloc_ctx *ctx, uint32 size) {
	struct umalloc_chunk_node *node = ctx->call_alloc(size, ctx->arg);
	if (!node)
		return NULL;

	node->size = size;
	node->chunk.prev_size = 0;
	node->chunk.size_flags = SET_SIZE(size - CHUNK_NODE_SIZE) | FG_END;

	FSYMBOL(list_add)(&ctx->chunk, &node->list);
	ctx->idle = &node->chunk;

	return node;
} /* end */

/* @func: _umalloc_new_big_node (static) - allocate large chunk of memory
* @param1: struct umalloc_ctx * # umalloc context struct
* @param2: uint64               # chunk size
* @return: void *               # chunk pointer
*/
static void *_umalloc_new_big_node(struct umalloc_ctx *ctx, uint64 size) {
	size = CHUNK_NODE_ALIGNED_SIZE(size);
	struct umalloc_chunk_node *node = ctx->call_alloc(
		size + CHUNK_NODE_SIZE, ctx->arg);
	if (!node)
		return NULL;

	node->size = size + CHUNK_NODE_SIZE;
	node->chunk.prev_size = 0;
	node->chunk.size_flags = SET_SIZE(size) | FG_INUSE | FG_END | FG_BIG;

	FSYMBOL(list_add)(&ctx->chunk, &node->list);

	return node;
} /* end */

/* @func: _umalloc_chunk_merge (static) - merge unused chunk backward
* @param1: struct umalloc_chunk * # chunk pointer
* @return: void
*/
static void _umalloc_chunk_merge(struct umalloc_chunk *chunk) {
	struct umalloc_chunk *next = NULL;
	uint32 size = 0;

	for (struct umalloc_chunk *pos = chunk;
			!GET_INUSE(pos);
			pos = NEXT_CHUNK(pos)) {
		size += GET_SIZE(pos);
		if (GET_END(pos)) {
			next = NULL;
			break;
		}
		next = NEXT_CHUNK(pos);
	}
	if (!size)
		return;

	chunk->size_flags = SET_SIZE(size) | (next ? 0 : FG_END);
	if (next)
		next->prev_size = size;
} /* end */

/* @func: _umalloc_prev (static) - forward offset unused chunk
* @param1: struct umalloc_chunk * # chunk pointer
* @return: void *                 # chunk pointer
*/
static void *_umalloc_prev(struct umalloc_chunk *chunk) {
	if (!chunk->prev_size)
		return chunk;

	for (struct umalloc_chunk *pos = PREV_CHUNK(chunk);
			!GET_INUSE(pos);
			pos = PREV_CHUNK(pos)) {
		chunk = pos;
		if (!chunk->prev_size)
			break;
	}

	return chunk;
} /* end */

/* @func: _umalloc_chunk (static) - allocate memory chunk
* @param1: struct umalloc_ctx *   # umalloc context struct
* @param2: struct umalloc_chunk * # chunk pointer
* @param3: uint32                 # allocate size
* @return: void *                 # allocated chunk pointer
*/
static void *_umalloc_chunk(struct umalloc_ctx *ctx,
		struct umalloc_chunk *chunk, uint32 size) {
	size = CHUNK_ALIGNED_SIZE(size);
	struct umalloc_chunk *next = NULL;

	for (struct umalloc_chunk *pos = chunk;
			pos;
			pos = NEXT_CHUNK(pos)) {
		if (GET_INUSE(pos)) {
			if (GET_END(pos))
				return NULL;
			continue;
		}

		_umalloc_chunk_merge(pos);
		uint32 psize = GET_SIZE(pos), pend = GET_END(pos);
		if (psize < size) {
			if (pend)
				return NULL;
			continue;
		}

		ctx->idle = NULL;
		if (psize == size) {
			pos->size_flags |= FG_INUSE;
			return pos;
		}
		if (psize >= (size + CHUNK_ALIGNED_SIZE(8))) {
			pos->size_flags = SET_SIZE(size) | FG_INUSE;
			next = NEXT_CHUNK(pos);
			next->prev_size = size;
			next->size_flags = SET_SIZE(psize - size);
			ctx->idle = next;
			if (pend) { /* pos | cur -> next */
				next->size_flags |= FG_END;
			} else { /* pos | cur -> next -> next */
				next = NEXT_CHUNK(next);
				next->prev_size = SET_SIZE(psize - size);
			}

			return pos;
		}

		pos->size_flags |= FG_INUSE;
		return pos;
	}

	return NULL;
} /* end */

/* @func: internal_umalloc - memory allocator
* @param1: struct umalloc_ctx * # umalloc context struct
* @param2: uint64               # allocate size
* @return: void *               # umalloc allocated pointer
*/
void *XSYMBOL(internal_umalloc)(struct umalloc_ctx *ctx, uint64 size) {
	struct umalloc_chunk_node *node;
	struct umalloc_chunk *chunk;

	if (size >= ALLOC_SIZE_MAX) { /* big-block */
		node = _umalloc_new_big_node(ctx, size);
		if (!node)
			return NULL;
		chunk = &node->chunk;
		return ++chunk;
	} else if (!size) {
		size = 1;
	}

	LIST_FOR_EACH(ctx->chunk.node, pos) {
		node = container_of(pos,
			struct umalloc_chunk_node, list);
		chunk = &node->chunk;
		if (GET_BIG(chunk))
			continue;
		if (ctx->idle)
			chunk = ctx->idle;

		chunk = _umalloc_chunk(ctx, chunk, size);
		if (chunk)
			return ++chunk;
		if (ctx->idle)
			break;
	}

	node = _umalloc_new_node(ctx, ALLOC_CHUNK_SIZE);
	if (!node)
		return NULL;
	chunk = _umalloc_chunk(ctx, &node->chunk, size);

	return chunk ? ++chunk : NULL;
} /* end */

/* @func: internal_ufree - free allocated memory
* @param1: struct umalloc_ctx * # umalloc context struct
* @param2: void *               # umalloc allocated pointer
* @return: void
*/
void XSYMBOL(internal_ufree)(struct umalloc_ctx *ctx, void *p) {
	struct umalloc_chunk_node *node;
	struct umalloc_chunk *chunk = p;
	chunk--;

	if (!GET_INUSE(chunk))
		return;

	if (GET_BIG(chunk)) {
		node = container_of(chunk,
			struct umalloc_chunk_node, chunk);
		FSYMBOL(list_del)(&ctx->chunk, &node->list);
		ctx->call_free(node, node->size, ctx->arg);
		return;
	}

	chunk->size_flags &= ~FG_INUSE;
	chunk = _umalloc_prev(chunk);
	_umalloc_chunk_merge(chunk);

	ctx->idle = NULL;
	if (!chunk->prev_size && GET_END(chunk)) {
		node = container_of(chunk,
			struct umalloc_chunk_node, chunk);
		FSYMBOL(list_del)(&ctx->chunk, &node->list);
		ctx->call_free(node, node->size, ctx->arg);
	}
} /* end */

/* @func: internal_ufree_all - free all allocated memory
* @param1: struct umalloc_ctx * # umalloc context struct
* @return: void
*/
void XSYMBOL(internal_ufree_all)(struct umalloc_ctx *ctx) {
	struct umalloc_chunk_node *node;
	while (ctx->chunk.node) {
		LIST_FOR_EACH(ctx->chunk.node, pos) {
			node = container_of(pos,
				struct umalloc_chunk_node, list);
			FSYMBOL(list_del)(&ctx->chunk, &node->list);
			ctx->call_free(node, node->size, ctx->arg);
			break;
		}
	}
} /* end */

/* @func: internal_umalloc_idle - count idle chunk
* @param1: struct umalloc_ctx * # umalloc context struct
* @param2: int32                # 0: chunk, 1: size, 2: inuse, 3: inuse size
* @return: uint64               # count size
*/
uint64 XSYMBOL(internal_umalloc_idle)(struct umalloc_ctx *ctx, int32 type) {
	uint64 n = 0;
	struct umalloc_chunk_node *node;
	struct umalloc_chunk *chunk;

	LIST_FOR_EACH(ctx->chunk.node, pos) {
		node = container_of(pos,
			struct umalloc_chunk_node, list);
		chunk = &node->chunk;
		if (GET_BIG(chunk))
			continue;

		switch (type) {
			case 2: case 3:
				goto e;
			default:
				break;
		}

		for (struct umalloc_chunk *pos = chunk;
				pos;
				pos = NEXT_CHUNK(pos)) {
			if (GET_END(pos)) {
				if (GET_INUSE(pos))
					break;
				n += type ? GET_SIZE(pos) : 1;
				break;
			}
			if (GET_INUSE(pos))
				continue;
			n += type ? GET_SIZE(pos) : 1;
		}
		continue;

e:
		for (struct umalloc_chunk *pos = chunk;
				pos;
				pos = NEXT_CHUNK(pos)) {
			if (GET_END(pos)) {
				if (!GET_INUSE(pos))
					break;
				n += (type == 3) ? GET_SIZE(pos) : 1;
				break;
			}
			if (!GET_INUSE(pos))
				continue;
			n += (type == 3) ? GET_SIZE(pos) : 1;
		}
	}

	return n;
} /* end */
