/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2018 Daniel Silverstone <dsilvers@netsurf-browser.org>
 */

#ifndef dom_html_form_controls_collection_h_
#define dom_html_form_controls_collection_h_

#include <dom/core/exceptions.h>
#include <dom/core/string.h>

struct dom_node;
struct dom_html_radionodelist;

typedef struct dom_html_collection dom_html_form_controls_collection;

#define dom_html_form_controls_collection_get_length(col, len)		\
	dom_html_collection_get_length((dom_html_collection *)(col), (len))

#define dom_html_form_controls_collection_item(col, index, node)		\
	dom_html_collection((dom_html_collection *)(col), (index), (node))

#define dom_html_form_controls_collection_named_item(col, name, node)	\
	dom_html_collection((dom_html_collection *)(col), (name), (node))

dom_exception dom_html_form_controls_collection_named_items(dom_html_form_controls_collection *col,
		dom_string *name, struct dom_html_radionodelist **nodes);

#define dom_html_form_controls_collection_ref(col) \
	dom_html_collection_ref((dom_html_collection *)(col))

#define dom_html_form_controls_collection_unref(col) \
	dom_html_collection_unref((dom_html_collection *)(col))

#endif

