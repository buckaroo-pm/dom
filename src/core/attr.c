/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <stddef.h>

#include <dom/core/attr.h>

#include "core/attr.h"
#include "core/document.h"
#include "core/node.h"
#include "utils/utils.h"

struct dom_element;
struct dom_type_info;

/**
 * DOM node attribute
 */
struct dom_attr {
	struct dom_node base;		/**< Base node */

	bool specified;			/**< Whether attribute was specified
					 * or defaulted */

	struct dom_element *owner;	/**< Owning element */

	struct dom_type_info *schema_type_info;	/**< Type information */

	bool is_id;			/**< Attribute is of type ID */
};

/**
 * Create an attribute node
 *
 * \param doc     The owning document
 * \param name    The name of the node to create
 * \param result  Pointer to location to receive created attribute
 * \return DOM_NO_ERR                on success,
 *         DOM_INVALID_CHARACTER_ERR if ::name is invalid,
 *         DOM_NO_MEM_ERR            on memory exhaustion.
 *
 * ::doc and ::name will have their reference counts increased.
 *
 * The returned attribute will already be referenced.
 */
dom_exception dom_attr_create(struct dom_document *doc,
		struct dom_string *name, struct dom_attr **result)
{
	struct dom_attr *a;
	dom_exception err;

	/** \todo Sanity check the attribute name */

	/* Allocate the element */
	a = dom_document_alloc(doc, NULL, sizeof(struct dom_attr));
	if (a == NULL)
		return DOM_NO_MEM_ERR;

	/* Initialise the base class */
	err = dom_node_initialise(&a->base, doc, DOM_ATTRIBUTE_NODE,
			name, NULL);
	if (err != DOM_NO_ERR) {
		dom_document_alloc(doc, a, 0);
		return err;
	}

	/* Perform our type-specific initialisation */
	a->specified = false;
	a->owner = NULL;
	a->schema_type_info = NULL;
	a->is_id = false;

	*result = a;

	return DOM_NO_ERR;
}

/**
 * Retrieve an attribute's name
 *
 * \param attr    Attribute to retrieve name from
 * \param result  Pointer to location to receive result
 * \return DOM_NO_ERR.
 *
 * The returned string will have its reference count increased. It is
 * the responsibility of the caller to unref the string once it has
 * finished with it.
 */
dom_exception dom_attr_get_name(struct dom_attr *attr,
		struct dom_string **result)
{
	UNUSED(attr);
	UNUSED(result);

	return DOM_NOT_SUPPORTED_ERR;
}

/**
 * Determine if attribute was specified or defaulted
 *
 * \param attr    Attribute to inspect
 * \param result  Pointer to location to receive result
 * \return DOM_NO_ERR.
 */
dom_exception dom_attr_get_specified(struct dom_attr *attr, bool *result)
{

	*result = attr->specified;

	return DOM_NO_ERR;
}

/**
 * Retrieve an attribute's value
 *
 * \param attr    Attribute to retrieve value from
 * \param result  Pointer to location to receive result
 * \return DOM_NO_ERR.
 *
 * The returned string will have its reference count increased. It is
 * the responsibility of the caller to unref the string once it has
 * finished with it.
 */
dom_exception dom_attr_get_value(struct dom_attr *attr,
		struct dom_string **result)
{
	UNUSED(attr);
	UNUSED(result);

	return DOM_NOT_SUPPORTED_ERR;
}

/**
 * Set an attribute's value
 *
 * \param attr   Attribute to retrieve value from
 * \param value  New value for attribute
 * \return DOM_NO_ERR                      on success,
 *         DOM_NO_MODIFICATION_ALLOWED_ERR if attribute is readonly.
 */
dom_exception dom_attr_set_value(struct dom_attr *attr,
		struct dom_string *value)
{
	UNUSED(attr);
	UNUSED(value);

	return DOM_NOT_SUPPORTED_ERR;
}

/**
 * Retrieve the owning element of an attribute
 *
 * \param attr    The attribute to extract owning element from
 * \param result  Pointer to location to receive result
 * \return DOM_NO_ERR.
 *
 * The returned node will have its reference count increased. The caller
 * should unref it once it has finished with it.
 */
dom_exception dom_attr_get_owner(struct dom_attr *attr,
		struct dom_element **result)
{
	/* If there is an owning element, increase its reference count */
	if (attr->owner != NULL)
		dom_node_ref((struct dom_node *) attr->owner);

	*result = attr->owner;

	return DOM_NO_ERR;
}

/**
 * Retrieve an attribute's type information
 *
 * \param attr    The attribute to extract type information from
 * \param result  Pointer to location to receive result
 * \return DOM_NO_ERR.
 *
 * The returned type info will have its reference count increased. The caller
 * should unref it once it has finished with it.
 */
dom_exception dom_attr_get_schema_type_info(struct dom_attr *attr,
		struct dom_type_info **result)
{
	UNUSED(attr);
	UNUSED(result);

	return DOM_NOT_SUPPORTED_ERR;
}

/**
 * Determine if an attribute if of type ID
 *
 * \param attr    The attribute to inspect
 * \param result  Pointer to location to receive result
 * \return DOM_NO_ERR.
 */
dom_exception dom_attr_is_id(struct dom_attr *attr, bool *result)
{
	*result = attr->is_id;

	return DOM_NO_ERR;
}
