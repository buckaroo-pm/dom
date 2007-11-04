/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef xml_xmlerror_h_
#define xml_xmlerror_h_

typedef enum {
	XML_OK          = 0,

	XML_NOMEM       = 1,

	XML_LIBXML_ERR  = (1<<16),
} dom_xml_error;

#endif
