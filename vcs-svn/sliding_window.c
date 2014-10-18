/*
 * Licensed under a two-clause BSD-style license.
 * See LICENSE for details.
 */

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vcs-svn/sliding_window.h"
#include "vcs-svn/line_buffer.h"
#include "vcs-svn/git-strbuf.h"

static int input_error(struct line_buffer *file)
{
	if (!buffer_ferror(file))
		printf("delta preimage ends early\n");
	else
		printf("cannot read delta preimage: %s\n", strerror(errno));
	return -1;
}

static int skip_or_whine(struct line_buffer *file, off_t gap)
{
	if (buffer_skip_bytes(file, gap) != gap)
		return input_error(file);
	return 0;
}

static int read_to_fill_or_whine(struct line_buffer *file,
				struct strbuf *buf, size_t width)
{
	buffer_read_binary(file, buf, width - buf->len);
	if (buf->len != width)
		return input_error(file);
	return 0;
}

static int check_offset_overflow(off_t offset, uintmax_t len)
{
#if 0
	if (len > maximum_signed_value_of_type(off_t))
		return error("unrepresentable length in delta: "
				"%"PRIuMAX" > OFF_MAX", len);
	if (signed_add_overflows(offset, (off_t) len))
		return error("unrepresentable offset in delta: "
				"%"PRIuMAX" + %"PRIuMAX" > OFF_MAX",
				(uintmax_t) offset, len);
#endif
	if (offset > (INT64_MAX/2) || len > (INT64_MAX/2)) {
		printf("Overflow or close enough.\n");
		return -1;
	}
	return 0;
}

int move_window(struct sliding_view *view, off_t off, size_t width)
{
	off_t file_offset;

	assert(view);
	assert(view->width <= view->buf.len);
	assert(!check_offset_overflow(view->off, view->buf.len));

	if (check_offset_overflow(off, width))
		return -1;
	if (off < view->off || off + width < view->off + view->width) {
		printf("invalid delta: window slides left\n");
		return -1;
	}
	if (view->max_off >= 0 && view->max_off < off + (off_t) width) {
		printf("delta preimage ends early\n");
		return -1;
	}

	file_offset = view->off + view->buf.len;
	if (off < file_offset) {
		/* Move the overlapping region into place. */
		strbuf_remove(&view->buf, 0, off - view->off);
	} else {
		/* Seek ahead to skip the gap. */
		if (skip_or_whine(view->file, off - file_offset))
			return -1;
		strbuf_setlen(&view->buf, 0);
	}

	if (view->buf.len > width)
		; /* Already read. */
	else if (read_to_fill_or_whine(view->file, &view->buf, width))
		return -1;

	view->off = off;
	view->width = width;
	return 0;
}
