<?xml version="1.0" encoding="ISO-8859-1"?>
<!--   
This transform generates C source code from a language independent
test representation.
-->

<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:str="http://xsltsl.org/string">
    
    	<!--
    	Import string functions from XSLT Standard Library
    	http://xsltsl.sourceforge.net/
    	-->
    	<xsl:import href="string.xsl"/>
    
    	<!--
    	The interfaces document is generated from the W3C test suite.
    	It contains the signatures of DOM interfaces, their methods
    	and attributes
    	-->
	<xsl:param name="interfaces-docname">dom1-interfaces.xml</xsl:param>
	
	<!--
	The ctypes document maps interfaces, methods and attributes in the
	interfaces document to their C counterparts, proving the ability to
	override names and types.
	-->
	<xsl:param name="ctypes-docname">ctypes.xml</xsl:param>
	<xsl:param name="target-uri-base">http://www.w3.org/2001/DOM-Test-Suite/tests/Level-1/</xsl:param>
	<xsl:output method="text" encoding="UTF-8"/>
	<xsl:variable name="domspec" select="document($interfaces-docname)"/>
	<xsl:variable name="ctypes" select="document($ctypes-docname)"/>

<!-- swallow any text which we don't understand -->
<xsl:template match="text()" mode="body"/>

<!--
for anything that doesn't match another template,
we expect this the element to be found in the library located
at $interfaces-docname.

This should either be a <method> or <attribute>.  If it is neither,
we generate an <xsl:message> reporting that the element is not known.
-->
<xsl:template match="*"  mode="body">
	<!-- the element name matches by this template -->
	<xsl:variable name="feature" select="local-name(.)"/>
	<xsl:variable name="interface" select="@interface"/>
	
	<!--
	Try to find a method having the @name of $feature.
	If $interface is defined, make sure search for
	a match on that interface in the $domspec document.
	-->
	<xsl:variable name="method" select="$domspec/library/interface[not($interface) or @name = $interface]/method[@name = $feature]"/>
	<xsl:choose>
		<xsl:when test="$method">
			<xsl:call-template name="produce-method">
				<xsl:with-param name="method" select="$method"/>
            		</xsl:call-template>
		</xsl:when>
		<xsl:otherwise>
			<!--
			Try to find an attribute having the name of $feature.
			Again, if $interface is defined, restrict the search to
			that interface
			-->
			<xsl:variable name="attribute" select="$domspec/library/interface[not($interface) or @name = $interface]/attribute[@name = $feature]"/>
			<xsl:choose>
				<xsl:when test="$attribute">
					<xsl:call-template name="produce-attribute"/>
				</xsl:when>
				
				<xsl:otherwise>
					<xsl:message>Unrecognized element <xsl:value-of select="local-name(.)"/></xsl:message>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template match="/">
    <xsl:apply-templates/>
</xsl:template>

<xsl:template match="*[local-name() = 'test']">
<xsl:text>#include &lt;string.h&gt;

#include &lt;dom/dom.h&gt;
#include "testutils.h"

</xsl:text>

	<xsl:apply-templates select="*[local-name() = 'metadata']"/>
<xsl:text>
int main(int argc, char **argv)
{
	dom_exception err;
</xsl:text>
<xsl:apply-templates mode="body"/>
<xsl:text>
	return 0;
}
</xsl:text>
</xsl:template>

<xsl:template match="comment()[contains(., 'Copyright')]">
/*
This source file was generated by test-to-c.xsl
and is a derived work from the source document.
The source document contained the following notice:

<xsl:value-of select="."/>
*/
</xsl:template>

<xsl:template match="*[local-name() = 'metadata']">
<xsl:text>/**
</xsl:text>
	<xsl:call-template name="emit-description">
		<xsl:with-param name="description" select="translate(*[local-name() = 'description'], '&#9;', ' ')"/>
	</xsl:call-template>
	<xsl:text> */</xsl:text>
</xsl:template>

<!-- swallowing templates in body mode -->
<xsl:template match="*[local-name() = 'metadata']" mode="body"/>

<xsl:template match="*[local-name() = 'load']" mode="body">
	<!--
	TODO: need to handle the case where we load more than one testObject.
	append a counter to the variable name?
	-->
	<xsl:text>
	TestObject *testObject = test_object_create(argc, argv, "</xsl:text><xsl:value-of select="@href"/><xsl:text>.xml", false);
	assert(testObject != NULL);
	
	</xsl:text><xsl:value-of select="@var"/><xsl:text> = test_object_get_doc(testObject);
	assert(</xsl:text><xsl:value-of select="@var"/><xsl:text> != NULL);
</xsl:text>
</xsl:template>

<!--
not sure what <contentType> is used for,
but it's implemented in subclasses of DOMTestDocumentBuilderFactory.getContentType()
-->
<xsl:template match="*[local-name() = 'contentType']" mode="body">
	<xsl:text>strcmp(test_object_get_mimetype(testObject), "</xsl:text><xsl:value-of select="@type"/><xsl:text>") == 0</xsl:text>
</xsl:template>

<!--
================================
Language construct templates
================================
-->

<xsl:template match="*[local-name() = 'var']" mode="body">
	<xsl:text>	</xsl:text>
	<xsl:call-template name="produce-var-type-declaration">
		<xsl:with-param name="var-type" select="./@type"/>
	</xsl:call-template>
	<xsl:value-of select="@name"/>;
</xsl:template>

<xsl:template match="*[local-name() = 'if']" mode="body">
	<xsl:text>
	if (</xsl:text><xsl:apply-templates select="*[1]" mode="body"/><xsl:text>) {
	</xsl:text>
	<xsl:apply-templates select="*[position() &gt; 1 and local-name() != 'else']" mode="body"/>
	<xsl:text>	}</xsl:text>
	<xsl:for-each select="*[local-name() = 'else']">
		<xsl:text> else {
</xsl:text>
		<xsl:apply-templates mode="body"/>
		<xsl:text>	}</xsl:text>
	</xsl:for-each>
	<xsl:text>
</xsl:text>
</xsl:template>

<xsl:template match="*[local-name() = 'equals']" mode="body">
	<xsl:message terminate="yes"><!-- FIXME: implement -->equals not implemented</xsl:message>
</xsl:template>

<!--
================================
DOM templates
================================
-->

<xsl:template name="produce-method">
	<xsl:variable name="methodName" select="local-name(.)"/>
	<!--  if interface is specified -->
	<xsl:choose>
		<xsl:when test="@interface">
			<xsl:variable name="interface" select="@interface"/>
			<xsl:call-template name="produce-specific-method">
				<!-- TODO: move vardefs up -->
				<xsl:with-param name="vardefs" select="//*[local-name() = 'var']"/>
				<xsl:with-param name="method" select="$domspec/library/interface[@name = $interface]/method[@name = $methodName]"/>
			</xsl:call-template>
		</xsl:when>
		<xsl:otherwise>
			<xsl:variable name="methods" select="$domspec/library/interface/method[@name = $methodName]"/>
			<xsl:call-template name="produce-specific-method">
				<xsl:with-param name="vardefs" select="//*[local-name() = 'var']"/>
				<xsl:with-param name="method" select="$methods[1]"/>
			</xsl:call-template>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="produce-attribute">
	<xsl:variable name="attribName" select="local-name(.)"/>
	<xsl:choose>
		<!--  if interface is specified -->
		<xsl:when test="@interface">
			<xsl:variable name="interface" select="@interface"/>
			<xsl:call-template name="produce-specific-attribute">
				<xsl:with-param name="attribute" select="$domspec/library/interface[@name = $interface]/attribute[@name = $attribName]"/>
			</xsl:call-template>
		</xsl:when>
		<xsl:otherwise>
			<xsl:call-template name="produce-specific-attribute">
				<xsl:with-param name="attribute" select="$domspec/library/interface/attribute[@name = $attribName]"/>	
			</xsl:call-template>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<!-- 
Produce a statement to get or set an attribute.
If @var is specified, the accessor is called and @var is given the result value.
err = dom_document_get_doctype(doc, &docType);
assert(err == DOM_NO_ERR);

If @value is specified, the mutator is called and @value is used as the parameter argument.

-->
<xsl:template name="produce-specific-attribute">
	<!--
	An <attribute> node in the $domspec document.
	-->
	<xsl:param name="attribute"/>
	
	<!-- the object which contains the attribute -->
	<xsl:variable name="obj" select="@obj"/>
	
	<!-- the <var> for the $obj -->
	<xsl:variable name="obj-var" select="//*[local-name() = 'var' and @name = $obj]"/>
	
	<!--
	The C type for the object's type containing $attribute.
	This may be the object's own type, or a supertype.  For example, $obj
	may be an Element, but the nodeValue attribute is a member of the Node
	supertype.
	Note that this is the <type> element itself, not the @c attribute.
	-->
	<xsl:variable name="obj-ctype" select="$ctypes/types/type[@idl = $attribute/parent::interface/@name]"/>
	
	<!--
	The C name of the attribute.  This is either $attribute/@name, or
	the value in $ctypes if it overrides it.
	-->
	<xsl:variable name="attribute-cname">
		<xsl:choose>
			<xsl:when test="$obj-ctype/attribute[@idl = $attribute/@name]/@c">
				<xsl:value-of select="$obj-ctype/attribute[@idl = $attribute/@name]/@c"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$attribute/@name"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>
	
	<!--
	The attribute type.  This is either $attribute/@type, or
	the <override-type> in $ctypes if it is specified.
	The resulting attribute type may be an IDL-style name (e.g. DocumentType)
	or a C-style name (e.g. dom_node_type).
	-->
	<xsl:variable name="attribute-type">
		<xsl:choose>
			<xsl:when test="$obj-ctype/attribute[@idl = $attribute/@name]/override-type">
				<xsl:value-of select="$obj-ctype/attribute[@idl = $attribute/@name]/override-type/text()"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$attribute/@type"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:text>	err = </xsl:text>
	<xsl:if test="@value">
		<xsl:value-of select="$obj-ctype/@c"/>
		<xsl:text>_set_</xsl:text>
		<xsl:value-of select="$attribute-cname"/>
		<xsl:text>(</xsl:text>
		<!-- TODO: function parameters -->
		<xsl:text>);</xsl:text>
	</xsl:if>
	<xsl:if test="@var">
		<xsl:variable name="var" select="@var"/>
		<xsl:variable name="var-type" select="//*[local-name() = 'var' and @name = $var]/@type"/>
	
		<xsl:value-of select="$obj-ctype/@c"/>
		<xsl:text>_get_</xsl:text>
		<xsl:value-of select="$attribute-cname"/>
		<xsl:text>(</xsl:text>
		<xsl:call-template name="cast">
			<xsl:with-param name="var-type" select="//*[local-name() = 'var' and @name = $obj]/@type"/>
			<xsl:with-param name="interface-type" select="$obj-ctype/@c"/>
		</xsl:call-template>
		<xsl:value-of select="$obj"/>
		<xsl:text>, </xsl:text>
		<xsl:call-template name="cast">
			<xsl:with-param name="var-type" select="$var-type"/>
			<xsl:with-param name="interface-type" select="$attribute-type"/>
		</xsl:call-template>
		<xsl:text>&amp;</xsl:text>
		<xsl:value-of select="@var"/>
		<xsl:text>);</xsl:text>
	</xsl:if>
	<xsl:text>
	assert(err == DOM_NO_ERR);
</xsl:text>
</xsl:template>

<xsl:template name="produce-specific-method">
	<xsl:param name="vardefs"/>

	<!-- a <method> from $domspec -->
	<xsl:param name="method"/>
	
	<!-- the current context node -->
	<xsl:variable name="current-node" select="."/>
	
	<!-- the object which contains the attribute -->
	<xsl:variable name="obj" select="@obj"/>
	
	<!-- the <var> for the $obj -->
	<xsl:variable name="obj-var" select="//*[local-name() = 'var' and @name = $obj]"/>
	
	<!-- the C-type on which the method is called -->
	<xsl:variable name="interface-ctype">
		<xsl:call-template name="get-ctype">
			<xsl:with-param name="type" select="$method/parent::interface/@name"/>
		</xsl:call-template>
	</xsl:variable>
	
	<xsl:variable name="method-cname">
		<xsl:choose>
			<xsl:when test="$ctypes/types/type[@c = $interface-ctype]/method[@idl = $method/@name]/@c">
				<xsl:value-of select="$ctypes/types/type[@c = $interface-ctype]/method[@idl = $method/@name]/@c"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$method/@name"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>
	
	<xsl:variable name="current-position"><xsl:number level="any" count="*"/></xsl:variable>

	<!--
	setup variables to hold parameter literals (for DOMStrings)
	TODO: needs testing with method that takes more than one parameter
	TODO: needs testing with more than one method call per test
	-->
	<xsl:for-each select="$method/parameters/param">
		<xsl:variable name="paramDef" select="."/>
        	<xsl:variable name="value" select="$current-node/@*[name() = $paramDef/@name]"/>
        	
        	<xsl:if test="starts-with($value, '&quot;')">
        		<xsl:call-template name="produce-dom-string">
        			<xsl:with-param name="vardefs" select="$vardefs"/>
        			<xsl:with-param name="var-name"><xsl:text>domString</xsl:text><xsl:value-of select="$current-position"/>_<xsl:number/></xsl:with-param>
				<xsl:with-param name="string" select="$value"/>
        		</xsl:call-template>
        	</xsl:if>
	</xsl:for-each>
	
	<!-- function call -->
	<xsl:text>	err = </xsl:text>
	<xsl:value-of select="$interface-ctype"/>
	<xsl:text>_</xsl:text>
	<xsl:value-of select="$method-cname"/>
	<xsl:text>(</xsl:text>
	
	<!-- the object to invoke the method upon -->
	<xsl:call-template name="cast">
		<xsl:with-param name="var-type" select="$obj-var/@type"/>
		<xsl:with-param name="interface-type" select="$method/parent::interface/@name"/>
	</xsl:call-template>
	<xsl:value-of select="$obj"/>
	
	<!-- method parameters -->
	<xsl:for-each select="$method/parameters/param">
		<xsl:variable name="paramDef" select="."/>
        	<xsl:variable name="value" select="$current-node/@*[name() = $paramDef/@name]"/>
        	
        	<xsl:text>, </xsl:text>
        	
        	<xsl:call-template name="produce-param">
        		<xsl:with-param name="vardefs" select="$vardefs"/>
        		<xsl:with-param name="var-or-literal" select="$value"/>
        		<xsl:with-param name="interface-type" select="./@type"/>
        		<xsl:with-param name="current-position" select="$current-position"/>
        	</xsl:call-template>
	</xsl:for-each>
	
	<!-- return variable -->
	<xsl:if test="@var">
		<xsl:variable name="var" select="@var"/>
		
		<!-- the <var> for the variable $var -->
		<xsl:variable name="var-var" select="//*[local-name() = 'var' and @name = $var]"/>
        	
		<xsl:text>, </xsl:text>
		<!--
		FIXME: any explicit cast won't work because _address of_ return params are passed (using &amp;)
		need to use extra * on the cast
		 -->
		<xsl:call-template name="cast">
			<xsl:with-param name="var-type" select="$var-var/@type"/>
			<xsl:with-param name="interface-type" select="$method/returns/@type"/>
			<xsl:with-param name="indirection" select="2"/>
		</xsl:call-template>
		<xsl:text>&amp;</xsl:text>
		<xsl:value-of select="$var"/>
	</xsl:if>
	<xsl:text>);</xsl:text>
	<xsl:text>
	assert(err == DOM_NO_ERR);
</xsl:text>
</xsl:template>

<!--
This template expects to be called with
a current context of a $domspec//method/parameters/param.

TODO: If this template needs to be more flexible, will need to pass $current-position
through as a parameter
-->
<xsl:template name="produce-param">
	<xsl:param name="vardefs"/>
	
	<!-- a string that may be a literal or a variable name -->
	<xsl:param name="var-or-literal"/>
	
	<!-- the parameter's expected type -->
	<xsl:param name="interface-type"/>
	
	<!-- the IDL type of $var-or-literal -->
	<xsl:variable name="var-type">
		<xsl:call-template name="guess-var-or-literal-type">
			<xsl:with-param name="vardefs" select="$vardefs"/>
			<xsl:with-param name="var-or-literal" select="$var-or-literal"/>
			<xsl:with-param name="interface-type" select="$interface-type"/>
		</xsl:call-template>
	</xsl:variable>
	
	<!-- used for referencing DOMStrings  -->
	<xsl:param name="current-position"/>
	
	<xsl:choose>
		<xsl:when test="$var-type = 'DOMString'">
			<!--
			TODO: put this string creation in its own template so it can be reused.
			Be careful with the behaviour of xsl:number, though.
			-->
			<xsl:text>domString</xsl:text><xsl:value-of select="$current-position"/>_<xsl:number/>
		</xsl:when>
		<xsl:otherwise>
			<xsl:call-template name="cast">
				<xsl:with-param name="var-type" select="$var-type"/>
				<xsl:with-param name="interface-type" select="$interface-type"/>
			</xsl:call-template>
			<xsl:value-of select="$var-or-literal"/>
		</xsl:otherwise>
	</xsl:choose>

</xsl:template>

<!-- guesses at the IDL type of the supplied var or literal -->
<xsl:template name="guess-var-or-literal-type">
	<xsl:param name="vardefs"/>
	
	<!-- the variable name or literal -->
	<xsl:param name="var-or-literal"/>
	
	<!-- the expected type of $var-or-literal -->
	<xsl:param name="interface-type"/>
	
	<xsl:choose>
		<!-- string literal -->
		<xsl:when test="starts-with($var-or-literal, '&quot;')">
			<xsl:text>DOMString</xsl:text>
		</xsl:when>
		<!-- variable -->
		<xsl:when test="$vardefs[@name = $var-or-literal]">
			<xsl:value-of select="$vardefs[@name = $var-or-literal]/@type"/>
		</xsl:when>
		<!-- unknown; return the interface type -->
		<xsl:otherwise>
			<xsl:value-of select="$interface-type"/>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="cast">
	<!-- the variable's type (e.g. Document, dom_node_type or int) -->
	<xsl:param name="var-type"/>
	
	<!-- the required type (e.g. Document or dom_node_type) -->
	<xsl:param name="interface-type"/>
	
	<xsl:param name="indirection" select="1"/>
	
	<!-- the variable's C-style type -->
	<xsl:variable name="var-ctype">
		<xsl:call-template name="get-ctype">
			<xsl:with-param name="type" select="$var-type"/>
		</xsl:call-template>
	</xsl:variable>
	
	<!-- the interface's C-style type -->
	<xsl:variable name="interface-ctype">
		<xsl:call-template name="get-ctype">
			<xsl:with-param name="type" select="$interface-type"/>
		</xsl:call-template>
	</xsl:variable>
	
	<xsl:if test="$var-ctype != $interface-ctype">
		<xsl:text>(</xsl:text>
		<xsl:call-template name="produce-var-type-declaration">
			<xsl:with-param name="var-type" select="$interface-ctype"/>
			<xsl:with-param name="indirection" select="$indirection"/>
		</xsl:call-template>
		<xsl:text>) </xsl:text>
	</xsl:if>
</xsl:template>

<!--
================================
Assert templates
================================
-->

<xsl:template match="*[local-name() = 'assertNotNull']" mode="body">
	<!-- TODO: what does the @id string do, and do we need it here? -->
	<xsl:text>
	assert(</xsl:text><xsl:value-of select="@actual"/><xsl:text> != NULL);
</xsl:text>
</xsl:template>

<xsl:template match="*[local-name() = 'assertNull']" mode="body">
	<!-- TODO: what does the @id string do, and do we need it here? -->
	<xsl:text>
	assert(</xsl:text><xsl:value-of select="@actual"/><xsl:text> == NULL);
	</xsl:text>
</xsl:template>

<xsl:template match="*[local-name() = 'assertEquals']" mode="body">
	<xsl:variable name="vardefs" select="//*[local-name() = 'var']"/>

	<xsl:variable name="actual" select="@actual"/>
	
	<xsl:variable name="var-type" select="$vardefs[@name = $actual]/@type"/>
	
	<xsl:choose>
		<xsl:when test="$var-type = 'DOMString'">
			<!-- a globally unique identifier for the temporary string -->
			<xsl:variable name="dom-string-id"><xsl:number level="any" count="*"/></xsl:variable>
		
			<xsl:call-template name="produce-dom-string">
				<xsl:with-param name="vardefs" select="$vardefs"/>
				<xsl:with-param name="var-name">matchString_<xsl:value-of select="$dom-string-id"/></xsl:with-param>
				<xsl:with-param name="string" select="@expected"/>
			</xsl:call-template>
			<xsl:text>	assert(</xsl:text>
			<xsl:choose>
				<xsl:when test="@ignoreCase = 'true'">
					<xsl:text>dom_string_icmp</xsl:text>
				</xsl:when>
				<xsl:when test="@ignoreCase = 'auto'">
					<!--
					TODO: implement auto case comparison (see java's DOMTestCase.assertEqualsAutoCase()
					-->
					<xsl:message>&lt;assertEquals ignoreCase='auto'&gt; not supported</xsl:message>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>dom_string_cmp</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text>(</xsl:text>
			<xsl:value-of select="@actual"/>
			<xsl:text>, matchString_</xsl:text>
			<xsl:value-of select="$dom-string-id"/>
			<xsl:text>) == 0);
</xsl:text>
		</xsl:when>
		<xsl:when test="$var-type = 'int'">
			<xsl:text>	assert(</xsl:text>
			<xsl:value-of select="@actual"/>
			<xsl:text> == </xsl:text>
			<xsl:value-of select="@expected"/>
			<xsl:text>);
</xsl:text>
		</xsl:when>
		<xsl:otherwise>
			<xsl:message>assertEquals on unknown type <xsl:value-of select="$var-type"/></xsl:message>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match="*[local-name() = 'assertSize']" mode="body">
	<xsl:message terminate="yes"><!-- FIXME: implement -->assertSize not implemented</xsl:message>
</xsl:template>

<xsl:template match="*[local-name() = 'assertDOMException']" mode="body">
	<xsl:message terminate="yes"><!-- FIXME: implement -->assertDOMException not implemented</xsl:message>
</xsl:template>

<!--
================================
Helper templates
================================
-->

<xsl:template name="produce-var-type-declaration">
	<!-- a type (e.g. Document, dom_node_type or int) -->
	<xsl:param name="var-type"/>
	
	<!--
	Number of *s to generate.
	Applies to structs and enums only (not primitives)
	Default is 1
	-->
	<xsl:param name="indirection" select="1"/>
	
	<xsl:variable name="var-ctype">
		<xsl:call-template name="get-ctype">
			<xsl:with-param name="type" select="$var-type"/>
		</xsl:call-template>
	</xsl:variable>
	
	<xsl:choose>
		<xsl:when test="$ctypes/types/primitive[@c = $var-ctype]">
			<!-- TODO: support the overriding of primitive name in ctypes document -->
			<xsl:value-of select="$var-ctype"/><xsl:text> </xsl:text>
		</xsl:when>
		<xsl:when test="$ctypes/types/type[@c = $var-ctype]">
			<xsl:text>struct </xsl:text>
			<xsl:value-of select="$ctypes/types/type[@c = $var-ctype]/@c"/>
			<xsl:text> </xsl:text>
			<xsl:call-template name="str:generate-string">
				<xsl:with-param name="text" select="'*'"/>
				<xsl:with-param name="count" select="$indirection"/>
			</xsl:call-template>
		</xsl:when>
		
		<!-- assume this is not a struct, and not a primitive (e.g. an enum) -->
		<xsl:otherwise>
			<xsl:value-of select="$var-ctype"/>
			<!-- TODO: should this * be here?  Probably, for normal parameters, no.
			Probably need one fewer *s that $indirection -->
			<xsl:text> *</xsl:text>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="produce-dom-string">
	<xsl:param name="vardefs"/>

	<!-- the variable name for the dom_string -->
	<xsl:param name="var-name"/>
	
	<!-- The string literal.  Should already be enclosed in double quotes. -->
	<xsl:param name="string"/>
	
	<!-- a list of all Document variables -->
	<xsl:variable name="docs" select="$vardefs[@type = 'Document']"/>
	
	<xsl:text>
	struct dom_string *</xsl:text><xsl:value-of select="$var-name"/>
	<xsl:text>;
	err = dom_string_create_from_const_ptr(</xsl:text>
	<xsl:value-of select="$docs[1]/@name"/>
	<xsl:text>, (uint8_t *) </xsl:text>
	<xsl:value-of select="$string"/>
	<xsl:text>, SLEN(</xsl:text>
	<xsl:value-of select="$string"/>
	<xsl:text>), &amp;</xsl:text>
	<xsl:value-of select="$var-name"/>
	<xsl:text>);
	assert(err == DOM_NO_ERR);
</xsl:text>
</xsl:template>

<xsl:template name="get-ctype">
	<!--
	a type (e.g. Document, dom_node_type or int)
	if $type is already a C-style type, or was a primitive, this is
	used instead
	 -->
	<xsl:param name="type"/>
	
	<xsl:choose>
		<xsl:when test="$ctypes/types/type[@idl = $type]/@c">
			<xsl:value-of select="$ctypes/types/type[@idl = $type]/@c"/>
		</xsl:when>
		<xsl:otherwise>
			<xsl:value-of select="$type"/>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<!--
Taken from test-to-java.xsl
Prepends every line with asterisks, suitable for use in a block comment
-->
<xsl:template name="emit-description">
	<xsl:param name="description"/>
	<xsl:choose>
		<xsl:when test="contains($description, '&#xA;')">
			<xsl:variable name="preceding" select="substring-before($description, '&#xA;')"/>
			<xsl:if test="string-length($preceding) &gt; 0">		
				<xsl:text> * </xsl:text>
				<xsl:value-of select="substring-before($description, '&#xA;')"/>
<xsl:text>
</xsl:text>
			</xsl:if>
			<xsl:variable name="following" select="substring-after($description, '&#xA;')"/>
			<xsl:if test="string-length($following) &gt; 0">
				<xsl:call-template name="emit-description">
					<xsl:with-param name="description" select="substring-after($description, '&#xA;')"/>
				</xsl:call-template>
			</xsl:if>
		</xsl:when>
		<xsl:otherwise>
			<xsl:text> * </xsl:text>
			<xsl:value-of select="$description"/>
			<xsl:text>
			</xsl:text>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

</xsl:stylesheet>
