/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2018 Daniel Silverstone <dsilvers@netsurf-browser.org>
 */

#ifndef dom_internal_html_radio_nodelist_h_
#define dom_internal_html_radio_nodelist_h_

#include <dom/html/html_collection.h>

struct dom_html_form_element;

struct dom_html_radio_nodelist {
	uint32_t refcnt; /**< The reference count of the nodelist */
	dom_string *name; /**< The name used for the nodelist */
	struct dom_html_form_element *form; /**< The form on which we constructed */
	dom_html_collection *col; /**< the underlying collection */
};

dom_exception _dom_html_radio_nodelist_create(struct dom_html_form_element *form,
		dom_string *name,
		struct dom_html_radio_nodelist **nodelist);

dom_exception _dom_html_radio_nodelist_initialise(struct dom_html_form_element *form,
		struct dom_html_radio_nodelist *nodelist,
		dom_string *name);

void _dom_html_radio_nodelist_finalise(struct dom_html_radio_nodelist *nodelist);

void _dom_html_radio_nodelist_destroy(struct dom_html_radio_nodelist *nodelist);


#endif
