/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2012 Daniel Silverstone <dsilvers@netsurf-browser.org>
 */

/* Note, this file deliberately lacks guards since it's included many times
 * in many places in order to correctly handle the loading of the strings.
 */

#ifndef HTML_DOCUMENT_STRINGS_ACTION
#define HTML_DOCUMENT_STRINGS_INTERNAL_ACTION 1
#define HTML_DOCUMENT_STRINGS_PREFIX      \
	typedef enum {
#define HTML_DOCUMENT_STRINGS_SUFFIX		\
	hds_COUNT				\
	} html_document_memo_string_e;
#define HTML_DOCUMENT_STRINGS_ACTION(tag,str)	\
	hds_##tag,
#endif

#define HTML_DOCUMENT_STRINGS_ACTION1(x) HTML_DOCUMENT_STRINGS_ACTION(x,x)

#ifdef HTML_DOCUMENT_STRINGS_PREFIX
HTML_DOCUMENT_STRINGS_PREFIX
#endif

/* Useful attributes for HTMLElement */
HTML_DOCUMENT_STRINGS_ACTION1(id)
HTML_DOCUMENT_STRINGS_ACTION1(title)
HTML_DOCUMENT_STRINGS_ACTION1(lang)
HTML_DOCUMENT_STRINGS_ACTION1(dir)
HTML_DOCUMENT_STRINGS_ACTION1(class)
/* Useful attributes used by HTMLHtmlElement */
HTML_DOCUMENT_STRINGS_ACTION1(version)
/* Useful attributes used by HTMLHeadElement */
HTML_DOCUMENT_STRINGS_ACTION1(profile)
/* Useful attributes used by HTMLLinkElement */
HTML_DOCUMENT_STRINGS_ACTION1(charset)
HTML_DOCUMENT_STRINGS_ACTION1(href)
HTML_DOCUMENT_STRINGS_ACTION1(hreflang)
HTML_DOCUMENT_STRINGS_ACTION1(media)
HTML_DOCUMENT_STRINGS_ACTION1(rel)
HTML_DOCUMENT_STRINGS_ACTION1(rev)
HTML_DOCUMENT_STRINGS_ACTION1(target)
HTML_DOCUMENT_STRINGS_ACTION1(type)
/* Useful attributes used by HTMLMetaElement */
HTML_DOCUMENT_STRINGS_ACTION1(content)
HTML_DOCUMENT_STRINGS_ACTION(http_equiv,http-equiv)
HTML_DOCUMENT_STRINGS_ACTION1(name)
HTML_DOCUMENT_STRINGS_ACTION1(scheme)
/* HTMLBodyElement attributes */
HTML_DOCUMENT_STRINGS_ACTION(a_link,alink)
HTML_DOCUMENT_STRINGS_ACTION(v_link,vlink)
HTML_DOCUMENT_STRINGS_ACTION(bg_color,bgcolor)
HTML_DOCUMENT_STRINGS_ACTION1(background)
HTML_DOCUMENT_STRINGS_ACTION1(link)
HTML_DOCUMENT_STRINGS_ACTION1(text)
/* Useful attributes used by HTMLFormElement */
HTML_DOCUMENT_STRINGS_ACTION(accept_charset,accept-charset)
HTML_DOCUMENT_STRINGS_ACTION1(action)
HTML_DOCUMENT_STRINGS_ACTION1(enctype)
HTML_DOCUMENT_STRINGS_ACTION1(method)
/* HTML_DOCUMENT_STRINGS_ACTION1(target) */
/* Useful attributes used by HTMLButtonElement */
HTML_DOCUMENT_STRINGS_ACTION(access_key,accesskey)
/* Useful attributes used by HTMLBrElement */
HTML_DOCUMENT_STRINGS_ACTION1(clear)
/* HTML_DOCUMENT_STRINGS_ACTION1(name) */
/* HTML_DOCUMENT_STRINGS_ACTION1(type) */
HTML_DOCUMENT_STRINGS_ACTION1(value)
/* Useful attributes used by HTMLInputElement */
HTML_DOCUMENT_STRINGS_ACTION1(accept)
/* HTML_DOCUMENT_STRINGS_ACTION(access_key,accesskey) */
HTML_DOCUMENT_STRINGS_ACTION1(align)
HTML_DOCUMENT_STRINGS_ACTION1(alt)
HTML_DOCUMENT_STRINGS_ACTION1(checked)
HTML_DOCUMENT_STRINGS_ACTION1(disabled)
HTML_DOCUMENT_STRINGS_ACTION(max_length,maxlength)
/* HTML_DOCUMENT_STRINGS_ACTION1(name) */
HTML_DOCUMENT_STRINGS_ACTION(read_only,readonly)
HTML_DOCUMENT_STRINGS_ACTION1(size)
HTML_DOCUMENT_STRINGS_ACTION1(src)
HTML_DOCUMENT_STRINGS_ACTION1(width)
HTML_DOCUMENT_STRINGS_ACTION1(compact)
HTML_DOCUMENT_STRINGS_ACTION1(cite)
HTML_DOCUMENT_STRINGS_ACTION1(color)
HTML_DOCUMENT_STRINGS_ACTION1(face)
HTML_DOCUMENT_STRINGS_ACTION1(coords)
HTML_DOCUMENT_STRINGS_ACTION1(shape)
HTML_DOCUMENT_STRINGS_ACTION1(border)
HTML_DOCUMENT_STRINGS_ACTION1(code)
HTML_DOCUMENT_STRINGS_ACTION1(archive)
HTML_DOCUMENT_STRINGS_ACTION1(data)
HTML_DOCUMENT_STRINGS_ACTION1(height)
HTML_DOCUMENT_STRINGS_ACTION1(standby)
HTML_DOCUMENT_STRINGS_ACTION1(object)
HTML_DOCUMENT_STRINGS_ACTION1(event)
HTML_DOCUMENT_STRINGS_ACTION1(defer)
HTML_DOCUMENT_STRINGS_ACTION1(abbr)
HTML_DOCUMENT_STRINGS_ACTION1(axis)
HTML_DOCUMENT_STRINGS_ACTION1(headers)
HTML_DOCUMENT_STRINGS_ACTION1(scope)
HTML_DOCUMENT_STRINGS_ACTION1(frame)
HTML_DOCUMENT_STRINGS_ACTION1(rules)
HTML_DOCUMENT_STRINGS_ACTION1(summary)
HTML_DOCUMENT_STRINGS_ACTION(tab_index,tabindex)
HTML_DOCUMENT_STRINGS_ACTION(html_for,for)
HTML_DOCUMENT_STRINGS_ACTION(date_time,datetime)
HTML_DOCUMENT_STRINGS_ACTION(long_desc,longdesc)
HTML_DOCUMENT_STRINGS_ACTION(code_base,codebase)
HTML_DOCUMENT_STRINGS_ACTION(code_type,codetype)
HTML_DOCUMENT_STRINGS_ACTION(value_type,valuetype)
HTML_DOCUMENT_STRINGS_ACTION(v_align,valign)
HTML_DOCUMENT_STRINGS_ACTION(ch,char)
HTML_DOCUMENT_STRINGS_ACTION(ch_off,charoff)
HTML_DOCUMENT_STRINGS_ACTION(cell_padding,cellpadding)
HTML_DOCUMENT_STRINGS_ACTION(cell_spacing,cellspacing)
/* HTML_DOCUMENT_STRINGS_ACTION1(type) */
HTML_DOCUMENT_STRINGS_ACTION(use_map,usemap)
/* HTML_DOCUMENT_STRINGS_ACTION1(value) */
/* HTMLTextAreaElement type */
HTML_DOCUMENT_STRINGS_ACTION1(textarea)
/* HTMLOptGroupElement attributes */
HTML_DOCUMENT_STRINGS_ACTION1(label)
/* HTMLOptionElement attributes */
/* HTML_DOCUMENT_STRINGS_ACTION1(label) */
HTML_DOCUMENT_STRINGS_ACTION1(selected)
/* HTML_DOCUMENT_STRINGS_ACTION1(value) */
/* HTMLSelectElement strings */
HTML_DOCUMENT_STRINGS_ACTION(select_multiple,select-multiple)
HTML_DOCUMENT_STRINGS_ACTION(select_one,select-one)
/* Some event strings for later */
HTML_DOCUMENT_STRINGS_ACTION1(blur)
HTML_DOCUMENT_STRINGS_ACTION1(focus)
HTML_DOCUMENT_STRINGS_ACTION1(select)
HTML_DOCUMENT_STRINGS_ACTION1(click)
HTML_DOCUMENT_STRINGS_ACTION1(submit)
HTML_DOCUMENT_STRINGS_ACTION1(reset)
/* Names for elements which get specialised. */
HTML_DOCUMENT_STRINGS_ACTION1(HTML)
HTML_DOCUMENT_STRINGS_ACTION1(HEAD)
HTML_DOCUMENT_STRINGS_ACTION1(LINK)
HTML_DOCUMENT_STRINGS_ACTION1(TITLE)
HTML_DOCUMENT_STRINGS_ACTION1(META)
HTML_DOCUMENT_STRINGS_ACTION1(BASE)
HTML_DOCUMENT_STRINGS_ACTION1(ISINDEX)
HTML_DOCUMENT_STRINGS_ACTION1(STYLE)
HTML_DOCUMENT_STRINGS_ACTION1(BODY)
HTML_DOCUMENT_STRINGS_ACTION1(FORM)
HTML_DOCUMENT_STRINGS_ACTION1(SELECT)
HTML_DOCUMENT_STRINGS_ACTION1(OPTGROUP)
HTML_DOCUMENT_STRINGS_ACTION1(OPTION)
HTML_DOCUMENT_STRINGS_ACTION1(INPUT)
HTML_DOCUMENT_STRINGS_ACTION1(TEXTAREA)
HTML_DOCUMENT_STRINGS_ACTION1(BUTTON)
HTML_DOCUMENT_STRINGS_ACTION1(LABEL)
HTML_DOCUMENT_STRINGS_ACTION1(FIELDSET)
HTML_DOCUMENT_STRINGS_ACTION1(LEGEND)
HTML_DOCUMENT_STRINGS_ACTION1(UL)
HTML_DOCUMENT_STRINGS_ACTION1(OL)
HTML_DOCUMENT_STRINGS_ACTION1(DL)
HTML_DOCUMENT_STRINGS_ACTION1(DIRECTORY)
HTML_DOCUMENT_STRINGS_ACTION1(MENU)
HTML_DOCUMENT_STRINGS_ACTION1(LI)
HTML_DOCUMENT_STRINGS_ACTION1(BLOCKQUOTE)
HTML_DOCUMENT_STRINGS_ACTION1(DIV)
HTML_DOCUMENT_STRINGS_ACTION1(P)
HTML_DOCUMENT_STRINGS_ACTION1(H1)
HTML_DOCUMENT_STRINGS_ACTION1(H2)
HTML_DOCUMENT_STRINGS_ACTION1(H3)
HTML_DOCUMENT_STRINGS_ACTION1(H4)
HTML_DOCUMENT_STRINGS_ACTION1(H5)
HTML_DOCUMENT_STRINGS_ACTION1(H6)
HTML_DOCUMENT_STRINGS_ACTION1(Q)
HTML_DOCUMENT_STRINGS_ACTION1(PRE)
HTML_DOCUMENT_STRINGS_ACTION1(BR)
HTML_DOCUMENT_STRINGS_ACTION1(BASEFONT)
HTML_DOCUMENT_STRINGS_ACTION1(FONT)
HTML_DOCUMENT_STRINGS_ACTION1(HR)
HTML_DOCUMENT_STRINGS_ACTION1(INS)
HTML_DOCUMENT_STRINGS_ACTION1(DEL)
HTML_DOCUMENT_STRINGS_ACTION1(A)
HTML_DOCUMENT_STRINGS_ACTION1(IMG)
HTML_DOCUMENT_STRINGS_ACTION1(OBJECT)
HTML_DOCUMENT_STRINGS_ACTION1(PARAM)
HTML_DOCUMENT_STRINGS_ACTION1(APPLET)
HTML_DOCUMENT_STRINGS_ACTION1(MAP)
HTML_DOCUMENT_STRINGS_ACTION1(AREA)
HTML_DOCUMENT_STRINGS_ACTION1(SCRIPT)
HTML_DOCUMENT_STRINGS_ACTION1(TABLE)
HTML_DOCUMENT_STRINGS_ACTION1(CAPTION)
HTML_DOCUMENT_STRINGS_ACTION1(COL)
HTML_DOCUMENT_STRINGS_ACTION1(COLGROUP)
HTML_DOCUMENT_STRINGS_ACTION1(THEAD)
HTML_DOCUMENT_STRINGS_ACTION1(TFOOT)
HTML_DOCUMENT_STRINGS_ACTION1(TBODY)
HTML_DOCUMENT_STRINGS_ACTION1(TR)
HTML_DOCUMENT_STRINGS_ACTION1(TH)
HTML_DOCUMENT_STRINGS_ACTION1(TD)
HTML_DOCUMENT_STRINGS_ACTION1(FRAMESET)
HTML_DOCUMENT_STRINGS_ACTION1(FRAME)
HTML_DOCUMENT_STRINGS_ACTION1(IFRAME)

#ifdef HTML_DOCUMENT_STRINGS_SUFFIX
HTML_DOCUMENT_STRINGS_SUFFIX
#endif
  
#undef HTML_DOCUMENT_STRINGS_ACTION1

#ifdef HTML_DOCUMENT_STRINGS_INTERNAL_ACTION
#undef HTML_DOCUMENT_STRINGS_INTERNAL_ACTION
#undef HTML_DOCUMENT_STRINGS_PREFIX
#undef HTML_DOCUMENT_STRINGS_SUFFIX
#undef HTML_DOCUMENT_STRINGS_ACTION
#endif
