/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2018 Daniel Silverstone <dsilvers@netsurf-browser.org>
 */

#include <assert.h>
#include <stdlib.h>

#include <libwapcaplet/libwapcaplet.h>

#include <dom/html/html_radio_nodelist.h>
#include <dom/html/html_input_element.h>

#include "html/html_radio_nodelist.h"
#include "html/html_form_controls_collection.h"
#include "html/html_document.h"
#include "html/html_form_element.h"

#include "core/node.h"
#include "core/element.h"
#include "core/string.h"

/*-----------------------------------------------------------------------*/
/* Internal functions */

/* Callback function to test whether certain node is a form control, see
 * src/html/html_collection.h for detail. */
static bool _dom_is_specific_form_control(struct dom_node_internal *node, void *ctx)
{
	struct dom_html_document *doc =
		(struct dom_html_document *)(node->owner);
	struct dom_html_radio_nodelist *nodelist = ctx;
	dom_string *str;
	dom_exception exc;

	if (_dom_is_form_control(node, nodelist->form)) {
		/* It's the control we care about if id or name are equal
		 * to nodelist->name.  We wouldn't be here if node wasn't
		 * an element node.
		 */
		exc = _dom_element_get_id((struct dom_element *)node, &str);

		if (exc != DOM_NO_ERR) {
			return false;
		}

		if (str != NULL && dom_string_isequal(str, nodelist->name)) {
			dom_string_unref(str);
			return true;
		}

		if (str != NULL)
			dom_string_unref(str);

		exc = _dom_element_get_attribute((struct dom_element *)node,
				doc->memoised[hds_name], &str);

		if (str != NULL && dom_string_isequal(str, nodelist->name)) {
			dom_string_unref(str);
			return true;
		}

		if (str != NULL)
			dom_string_unref(str);
	}

	return false;
}

/**
 * Create a dom_html_radio_nodelist
 *
 * \param form  The HTML form element for the radio nodelist
 * \param name  The element id/name for the radio nodelist
 * \param nodelist   The result radio nodelist object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_radio_nodelist_create(struct dom_html_form_element *form,
		dom_string *name,
		struct dom_html_radio_nodelist **nodelist)
{
	dom_exception exc;
	*nodelist = malloc(sizeof(struct dom_html_radio_nodelist));

	if (*nodelist == NULL)
		return DOM_NO_MEM_ERR;

	exc = _dom_html_radio_nodelist_initialise(form, *nodelist, name);

	if (exc != DOM_NO_ERR) {
		/* This is safe because we only ever get an error before we
		 * ref anything which would need finalising
		 */
		free(*nodelist);
		*nodelist = NULL;
	}

	return exc;
}

/**
 * Initialise a dom_html_radio_nodelist
 *
 * \param form  The HTML form element for the radio nodelist
 * \param nodelist The radio nodelist object to initialise
 * \param name  The element id/name for the radio nodelist
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_radio_nodelist_initialise(struct dom_html_form_element *form,
		struct dom_html_radio_nodelist *nodelist,
		dom_string *name)
{
	dom_exception exc;
	dom_html_document *doc = (dom_html_document *) dom_node_get_owner(form);

	assert(form != NULL);
	assert(name != NULL);

	assert(doc != NULL);

	exc =  _dom_html_collection_create(doc,
			(dom_node_internal *) doc,
			_dom_is_specific_form_control,
			nodelist, &nodelist->col);

	if (exc != DOM_NO_ERR)
		return exc;

	nodelist->form = form;
	dom_node_ref(form);

	nodelist->name = name;
	dom_string_ref(name);

	nodelist->refcnt = 1;

	return DOM_NO_ERR;
}

/**
 * Finalise a dom_html_radio_nodelist object
 *
 * \param nodelist The dom_html_radio_nodelist object
 */
void _dom_html_radio_nodelist_finalise(struct dom_html_radio_nodelist *nodelist)
{
	assert(nodelist->refcnt == 0);
	dom_node_unref(nodelist->form);
	nodelist->form = NULL;
	dom_string_unref(nodelist->name);
	nodelist->name = NULL;
	dom_html_collection_unref(nodelist->col);
	nodelist->col = NULL;
}

/**
 * Destroy a dom_html_radio_nodelist object
 *
 * \param nodelist The dom_html_radio_nodelist object
 */
void _dom_html_radio_nodelist_destroy(struct dom_html_radio_nodelist *nodelist)
{
	_dom_html_radio_nodelist_finalise(nodelist);
	free(nodelist);
}

/*-----------------------------------------------------------------------*/
/* Public API */

/**
 * Claim a reference on this radio nodelist
 *
 * \param nodelist The radio nodelist object
 */
void dom_html_radio_nodelist_ref(dom_html_radio_nodelist *nodelist)
{
	if (nodelist == NULL)
		return;

	nodelist->refcnt ++;
}

/**
 * Relese a reference on this radio nodelist
 *
 * \param nodelist The radio nodelist object
 */
void dom_html_radio_nodelist_unref(dom_html_radio_nodelist *nodelist)
{
	if (nodelist == NULL)
		return;

	if (nodelist->refcnt > 0)
		nodelist->refcnt --;

	if (nodelist->refcnt == 0)
		_dom_html_radio_nodelist_destroy(nodelist);
}


/**
 * Retrieve the length of a node list
 *
 * \param nodelist List to retrieve length of
 * \param len  Pointer to location to receive length
 * \return DOM_NO_ERR.
 */
dom_exception dom_html_radio_nodelist_get_length(dom_html_radio_nodelist *nodelist,
		uint32_t *len)
{
	return dom_html_collection_get_length(nodelist->col, len);
}

/**
 * Retrieve an item from a node list
 *
 * \param nodelist The list to retrieve the item from
 * \param index  The list index to retrieve
 * \param node   Pointer to location to receive item
 * \return DOM_NO_ERR.
 *
 * ::index is a zero-based index into ::list.
 * ::index lies in the range [0, length-1]
 *
 * The returned node will have had its reference count increased. The client
 * should unref the node once it has finished with it.
 */
dom_exception dom_html_radio_nodelist_item(dom_html_radio_nodelist *nodelist,
		uint32_t index, struct dom_node **node)
{
	return dom_html_collection_item(nodelist->col, index, node);
}

/**
 * Retrieve the value of the radio nodelist per 2.7.2.2.
 *
 * \param nodelist The list to retrieve the value from
 * \param value The value string to fill out
 * \return DOM_NO_ERR
 */
dom_exception dom_html_radio_nodelist_get_value(dom_html_radio_nodelist *nodelist,
					       dom_string **value)
{
	dom_exception exc;
	dom_node *node = NULL;
	uint32_t count;
	struct dom_html_document *doc =
		(struct dom_html_document *)(((struct dom_node_internal *)nodelist->form)->owner);

	assert(doc != NULL);

	exc = dom_html_collection_get_length(nodelist->col, &count);

	if (exc != DOM_NO_ERR)
		return exc;

	/* 1. Let element be the first element in tree order represented by the
	 * RadioNodeList object that is an input element whose type attribute
	 * is in the Radio Button state and whose checkedness is
	 * true. Otherwise, let it be NULL.
	 */

	for (uint32_t idx = 0; idx < count; idx++) {
		exc = dom_html_collection_item(nodelist->col, idx, &node);
		if (exc != DOM_NO_ERR)
			return exc;

		assert(node->type == DOM_ELEMENT_NODE);

		if (dom_string_caseless_isequal(((struct dom_node_internal *)node)->name,
				doc->elements[DOM_HTML_ELEMENT_TYPE_INPUT])) {
			bool checked = false;
			dom_string *type;

			exc = dom_element_get_attribute(node, doc->memoised[hds_type], &type);
			if (exc != DOM_NO_ERR) {
				dom_node_unref(node);
				return exc;
			}

			if (dom_string_caseless_isequal(type, doc->memoised[hds_radio]) == false) {
				dom_node_unref(node);
				dom_string_unref(type);
				continue;
			}

			dom_string_unref(type);

			exc = dom_html_input_element_get_checked((dom_html_input_element *)node, &checked);

			if (exc != DOM_NO_ERR) {
				dom_node_unref(node);
				return exc;
			}
			if (checked == true)
				break;
		}

		dom_node_unref(node);
		node = NULL;
	}

	/* 2. If element is null, return the empty string. */
	if (node == NULL) {
		*value = doc->base._memo_empty;
		dom_string_ref(*value);
		return DOM_NO_ERR;
	}

	exc = dom_html_input_element_get_value((dom_html_input_element *)node, value);

	/* 3. If element is an element with no value attribute, return the
	 * string "on".
	 */
	if (*value == NULL) {
		*value = doc->memoised[hds_on];
		dom_string_ref(*value);
		return DOM_NO_ERR;
	}

	/* 4. Otherwise, return the value of element’s value attribute. */

	dom_node_unref(node);

	return DOM_NO_ERR;
}

dom_exception dom_html_radio_nodelist_set_value(dom_html_radio_nodelist *nodelist,
					       dom_string *value)
{
	dom_exception exc;
	dom_node *node = NULL;
	uint32_t count;
	bool goal_is_on = false;
	struct dom_html_document *doc =
		(struct dom_html_document *)(((struct dom_node_internal *)nodelist->form)->owner);

	assert(doc != NULL);

	exc = dom_html_collection_get_length(nodelist->col, &count);

	if (exc != DOM_NO_ERR)
		return exc;

	goal_is_on = dom_string_caseless_isequal(value, doc->memoised[hds_on]);

	/* 1. If the new value is the string "on": let element be the first
	 * element in tree order represented by the RadioNodeList object that
	 * is an input element whose type attribute is in the Radio Button
	 * state and whose value content attribute is either absent, or present
	 * and equal to the new value, if any. If no such element exists, then
	 * instead let element be null.
	 *
	 * Otherwise: let element be the first element in tree order
	 * represented by the RadioNodeList object that is an input element
	 * whose type attribute is in the Radio Button state and whose value
	 * content attribute is present and equal to the new value, if any. If
	 * no such element exists, then instead let element be null.
	 */

	for (uint32_t idx = 0; idx < count; idx++) {
		exc = dom_html_collection_item(nodelist->col, idx, &node);
		if (exc != DOM_NO_ERR)
			return exc;

		assert(node->type == DOM_ELEMENT_NODE);
		if (dom_string_caseless_isequal(((struct dom_node_internal *)node)->name,
				doc->elements[DOM_HTML_ELEMENT_TYPE_INPUT])) {
			dom_string *type;
			dom_string *test_value;

			exc = dom_element_get_attribute(node, doc->memoised[hds_type], &type);
			if (exc != DOM_NO_ERR) {
				dom_node_unref(node);
				return exc;
			}

			if (dom_string_caseless_isequal(type, doc->memoised[hds_radio]) == false) {
				dom_node_unref(node);
				dom_string_unref(type);
				continue;
			}

			dom_string_unref(type);

			exc = dom_html_input_element_get_value((dom_html_input_element *)node, &test_value);

			if (test_value == NULL) {
				if (goal_is_on) {
					dom_string_unref(test_value);
					break;
				}
			} else {
				if (dom_string_caseless_isequal(test_value, value)) {
					dom_string_unref(test_value);
					break;
				}
			}

			dom_string_unref(test_value);
		}

		dom_node_unref(node);
		node = NULL;
	}

	exc = DOM_NO_ERR;

	/* 2. If element is not null, then set its checkedness to true. */
	if (node != NULL) {
		exc = dom_html_input_element_set_checked((dom_html_input_element *)node, true);
	}

	dom_node_unref(node);

	return exc;
}
