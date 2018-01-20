/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2018 Daniel Silverstone <dsilvers@netsurf-browser.org>
 */

#include <assert.h>
#include <stdlib.h>

#include <libwapcaplet/libwapcaplet.h>

#include "html/html_form_controls_collection.h"
#include "html/html_document.h"
#include "html/html_form_element.h"

#include "html/html_input_element.h"
#include "html/html_select_element.h"
#include "html/html_text_area_element.h"
#include "html/html_button_element.h"

#include "core/node.h"
#include "core/element.h"
#include "core/string.h"

/*-----------------------------------------------------------------------*/
/* Internal functions */

/* Callback function to test whether certain node is a form control, see
 * src/html/html_collection.h for detail. */
bool _dom_is_form_control(struct dom_node_internal *node, void *ctx)
{
	struct dom_html_document *doc =
		(struct dom_html_document *)(node->owner);
	struct dom_html_form_element *form = ctx;


	assert(node->type == DOM_ELEMENT_NODE);

	/* Form controls are INPUT TEXTAREA SELECT and BUTTON*/
	if (dom_string_caseless_isequal(node->name,
					doc->elements[DOM_HTML_ELEMENT_TYPE_INPUT]))
		return ((dom_html_input_element *)node)->form == form;
	if (dom_string_caseless_isequal(node->name,
					doc->elements[DOM_HTML_ELEMENT_TYPE_TEXTAREA]))
		return ((dom_html_text_area_element *)node)->form == form;
	if (dom_string_caseless_isequal(node->name,
					doc->elements[DOM_HTML_ELEMENT_TYPE_SELECT]))
		return ((dom_html_select_element *)node)->form == form;
	if (dom_string_caseless_isequal(node->name,
					doc->elements[DOM_HTML_ELEMENT_TYPE_BUTTON])) {
		return ((dom_html_button_element *)node)->form == form;
	}

	return false;
}


dom_exception _dom_html_form_controls_collection_create(struct dom_html_form_element *form,
		dom_html_form_controls_collection **col)
{
	dom_html_document *doc = (dom_html_document *) dom_node_get_owner(form);

	assert(doc != NULL);

	return _dom_html_collection_create(doc,
					  (dom_node_internal *) doc,
					  _dom_is_form_control, form, col);
}
