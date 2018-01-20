/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2018 Daniel Silverstone <dsilvers@netsurf-browser.org>
 */

#ifndef dom_html_radio_nodelist_h_
#define dom_html_radio_nodelist_h_

#include <dom/core/exceptions.h>
#include <dom/core/string.h>

struct dom_node;

typedef struct dom_html_radio_nodelist dom_html_radio_nodelist;

dom_exception dom_html_radio_nodelist_get_length(dom_html_radio_nodelist *nodelist,
		uint32_t *len);
dom_exception dom_html_radio_nodelist_item(dom_html_radio_nodelist *nodelist,
		uint32_t index, struct dom_node **node);

dom_exception dom_html_radio_nodelist_get_value(dom_html_radio_nodelist *nodelist,
					       dom_string **value);

dom_exception dom_html_radio_nodelist_set_value(dom_html_radio_nodelist *nodelist,
					       dom_string *value);

void dom_html_radio_nodelist_ref(dom_html_radio_nodelist *nodelist);
void dom_html_radio_nodelist_unref(dom_html_radio_nodelist *nodelist);

#endif

