<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- $Id: decoderSuppressRedundantLabel.xsl 25756 2014-05-03 21:31:17Z jacobsen $ -->

<!-- Process a JMRI decoder file, removing the label attribute from a       -->
<!-- variable element if an item attribute or default-language label        -->
<!-- element exists. (The content of the element is not checked)            -->

<!-- xsltproc decoderSuppressRedundantLabel.xsl 0NMRA.xml | diff - 0NMRA.xml  -->

<!-- The sequence of operations to normalize a decoder file is -->
<!--  decoderLabelToItem.xsl                                   -->
<!--  decoderAddLabelElement.xsl                               -->
<!--  decoderSuppressRedundantLabel.xsl                        -->
<!--  xmllint -format                                          -->


<!-- This file is part of JMRI.  Copyright 2009-2014.                       -->
<!--                                                                        -->
<!-- JMRI is free software; you can redistribute it and/or modify it under  -->
<!-- the terms of version 2 of the GNU General Public License as published  -->
<!-- by the Free Software Foundation. See the "COPYING" file for a copy     -->
<!-- of this license.                                                       -->
<!--                                                                        -->
<!-- JMRI is distributed in the hope that it will be useful, but WITHOUT    -->
<!-- ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or  -->
<!-- FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License  -->
<!-- for more details.                                                      -->
 
<xsl:stylesheet	version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:db="http://docbook.org/ns/docbook"
    >

<xsl:output method="xml" encoding="utf-8"/>

<!--specific template match for variable element with label/item match -->
    <xsl:template match="variable[@item=@label]">
      <xsl:copy>
        <xsl:apply-templates select="@*[local-name() != 'label']|node()" />
      </xsl:copy>
    </xsl:template>

<!--specific template match for variable element with default-language label element -->
    <xsl:template match="variable[label[not(@xml:lang)]]">
      <xsl:copy>
        <xsl:apply-templates select="@*[local-name() != 'label']|node()" />
      </xsl:copy>
    </xsl:template>

<!--specific template match for variable element without match-->
    <xsl:template match="variable">
      <xsl:copy>
        <xsl:apply-templates select="@*|node()" />
      </xsl:copy>
    </xsl:template>

<!--Identity template copies content forward -->
    <xsl:template match="@*|node()">
      <xsl:copy>
        <xsl:apply-templates select="@*|node()"/>
      </xsl:copy>
    </xsl:template>


</xsl:stylesheet>
