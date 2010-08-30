/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 Bo Yang <struggleyb.nku.com>
 */

#include "html/html_isindex_element.h"

#include "core/node.h"
#include "core/document.h"
#include "utils/utils.h"

static struct dom_element_protected_vtable _protect_vtable = {
	{
		DOM_NODE_PROTECT_VTABLE_HTML_ISINDEX_ELEMENT
	},
	DOM_HTML_ISINDEX_ELEMENT_PROTECT_VTABLE
};

/**
 * Create a dom_html_isindex_element object
 *
 * \param doc   The document object
 * \param form  The form element which contains this element
 * \param ele   The returned element object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_isindex_element_create(struct dom_document *doc,
		struct dom_html_form_element *form, 
		struct dom_html_isindex_element **ele)
{
	*ele = _dom_document_alloc(doc, NULL, sizeof(dom_html_isindex_element));
	if (*ele == NULL)
		return DOM_NO_MEM_ERR;
	
	/* Set up vtables */
	struct dom_node_internal *node = (struct dom_node_internal *) *ele;
	node->base.vtable = &_dom_element_vtable;
	node->vtable = &_protect_vtable;

	return _dom_html_isindex_element_initialise(doc, form, *ele);
}

/**
 * Initialise a dom_html_isindex_element object
 *
 * \param doc   The document object
 * \param form  The form element which contains this element
 * \param ele   The dom_html_isindex_element object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_isindex_element_initialise(struct dom_document *doc,
		struct dom_html_form_element *form, 
		struct dom_html_isindex_element *ele)
{
	const char *str = "ISINDEX";
	lwc_string *name = NULL;
	dom_exception err;

	err = _dom_document_create_lwcstring(doc, (const uint8_t *) str, SLEN(str),
			&name);
	if (err != DOM_NO_ERR)
		return err;
	
	err = _dom_html_element_initialise(doc, &ele->base, name, NULL, NULL);
	_dom_document_unref_lwcstring(doc, name);

	ele->base.form = form;
	dom_node_ref(form);

	return err;
}

/**
 * Finalise a dom_html_isindex_element object
 *
 * \param doc  The document object
 * \param ele  The dom_html_isindex_element object
 */
void _dom_html_isindex_element_finalise(struct dom_document *doc,
		struct dom_html_isindex_element *ele)
{
	_dom_html_element_finalise(doc, &ele->base);
}

/**
 * Destroy a dom_html_isindex_element object
 *
 * \param doc  The document object
 * \param ele  The dom_html_isindex_element object
 */
void _dom_html_isindex_element_destroy(struct dom_document *doc,
		struct dom_html_isindex_element *ele)
{
	_dom_html_isindex_element_finalise(doc, ele);
	_dom_document_alloc(doc, ele, 0);
}

/*------------------------------------------------------------------------*/
/* The protected virtual functions */

/* The virtual function used to parse attribute value, see src/core/element.c
 * for detail */
dom_exception _dom_html_isindex_element_parse_attribute(dom_element *ele,
		struct dom_string *name, struct dom_string *value,
		struct dom_string **parsed)
{
	UNUSED(ele);
	UNUSED(name);

	dom_string_ref(value);
	*parsed = value;

	return DOM_NO_ERR;
}

/* The virtual destroy function, see src/core/node.c for detail */
void _dom_virtual_html_isindex_element_destroy(dom_node_internal *node)
{
	struct dom_document *doc = dom_node_get_owner(node);
	_dom_html_isindex_element_destroy(doc, (struct dom_html_isindex_element *) node);
}

/* The virtual allocation function, see src/core/node.c for detail */
dom_exception _dom_html_isindex_element_alloc(struct dom_document *doc,
		struct dom_node_internal *n, struct dom_node_internal **ret)
{
	UNUSED(n);

	*ret = _dom_document_alloc(doc, NULL, sizeof(dom_html_isindex_element));
	if (*ret == NULL)
		return DOM_NO_MEM_ERR;
	
	return DOM_NO_ERR;
}

/* The virtual copy function, see src/core/node.c for detail */
dom_exception _dom_html_isindex_element_copy(struct dom_node_internal *new,
		struct dom_node_internal *old)
{
	return _dom_html_element_copy(new, old);
}


/*-----------------------------------------------------------------------*/
/* Public APIs */

/**
 * Get the form element which contains this element
 *
 * \param ele   The dom_html_isindex_element
 * \param form  The form element
 * \return DOM_NO_ERR on success.
 */
dom_exception dom_html_isindex_element_get_form(dom_html_isindex_element *ele,
		struct dom_html_form_element **form)
{
	_dom_html_element_get_form(&ele->base, form);

	return DOM_NO_ERR;
}