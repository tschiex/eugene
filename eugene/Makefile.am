# ------------------------------------------------------------------
# Copyright (C) 2004 INRA <eugene@ossau.toulouse.inra.fr>
#
# This program is open source; you can redistribute it and/or modify
# it under the terms of the Artistic License (see LICENSE file).
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
#
# You should have received a copy of Artistic License along with
# this program; if not, please see http://www.opensource.org
#
# $Id$
# ------------------------------------------------------------------
# File:     Makefile.am
# Contents: compilation of the root directory
# ------------------------------------------------------------------


export eugene_CXXFLAGS = -O6 -funroll-loops -fomit-frame-pointer \
                           -fPIC -frtti -Wall -pipe -rdynamic


 

EXTRA_DIST = reconf configure  config/* CREATE_LINKS \
	     doc/EuGeneDoc.pdf doc/Sequences/SYNO_ARATH.fasta* \
	     doc/GenerateDoc.tcl doc/Doc.tex doc/Doc_*.tex \
	     doc/ParaOptimization.pdf doc/ParaOptimization.fig \
	     src/getopt.h src/eugene.par \
	     src/PLUGINS/Ara2UTR.mat src/PLUGINS/BLOSUM* src/PLUGINS/WAM/* \
	     src/PLUGINS/swissprot.maxorder2.bin src/PLUGINS/SwP41.noFragm.mininfo1.order2.bin \
	     src/PLUGINS/exon.dist src/PLUGINS/intergenic.dist src/PLUGINS/intron.dist \
	     src/PLUGINS/term.dist src/PLUGINS/init.dist       src/PLUGINS/intr.dist \
             src/PLUGINS/sngl.dist src/PLUGINS/utr.dist \
	     src/WEB/Images/back.jpg src/WEB/Images/eg.jpg src/WEB/Images/euGpred_on.jpg \
             src/WEB/Images/first.jpg src/WEB/Images/last.jpg src/WEB/Images/left.jpg \
	     src/WEB/Images/m_doc.jpg src/WEB/Images/m_download.jpg src/WEB/Images/m_home.jpg \
	     src/WEB/Images/m_links_contacts.jpg src/WEB/Images/m_orga.jpg \
	     src/WEB/Images/m_plugins.jpg src/WEB/Images/next.jpg src/WEB/Images/top.jpg \
	     src/WEB/Javascripts/cm_addins.js src/WEB/Javascripts/coolmenus4.js \
	     src/WEB/Javascripts/diap.js src/WEB/Javascripts/euG_menu.js \
	     src/WEB/Style/eugene.css \
	     lib/gd-2.0.22/libgd.a  lib/libpng-1.2.5/libpng.a  lib/zlib-1.2.1/libz.a


SUBDIRS = src doc

CLEANFILES = *~