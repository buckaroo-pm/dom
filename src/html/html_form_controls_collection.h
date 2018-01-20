/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2018 Daniel Silverstone <dsilvers@netsurf-browser.org>
 */

#ifndef dom_internal_html_form_controls_collection_h_
#define dom_internal_html_form_controls_collection_h_

#include <dom/html/html_form_controls_collection.h>

#include "html/html_collection.h"

struct dom_html_form_element;

/* Exposed so that the radio nodelist can use it as a sub-criterion */
bool _dom_is_form_control(struct dom_node_internal *node, void *ctx);

dom_exception _dom_html_form_controls_collection_create(struct dom_html_form_element *form,
		dom_html_form_controls_collection **col);

#endif
