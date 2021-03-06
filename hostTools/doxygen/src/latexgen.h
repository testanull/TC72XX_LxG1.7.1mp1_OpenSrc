/******************************************************************************
 *
 * $Id: latexgen.h,v 1.2 2014/11/19 09:12:50 wtchen Exp $
 *
 * Copyright (C) 1997-2006 by Dimitri van Heesch.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License is hereby 
 * granted. No representations are made about the suitability of this software 
 * for any purpose. It is provided "as is" without express or implied warranty.
 * See the GNU General Public License for more details.
 *
 * Documents produced by Doxygen are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
 */

#ifndef LATEXGEN_H
#define LATEXGEN_H

#include "outputgen.h"

class QFile;

class LatexGenerator : public OutputGenerator
{
  public:
    LatexGenerator();
   ~LatexGenerator();
    static void init();
    static void writeStyleSheetFile(QFile &f);
    static void writeHeaderFile(QFile &f);

    //OutputGenerator *copy();
    //OutputGenerator *clone() { return new LatexGenerator(*this); }
    //void append(const OutputGenerator *o);
    void enable() 
    { if (genStack->top()) active=*genStack->top(); else active=TRUE; }
    void disable() { active=FALSE; }
    void enableIf(OutputType o)  { if (o==Latex) active=TRUE;  }
    void disableIf(OutputType o) { if (o==Latex) active=FALSE; }
    void disableIfNot(OutputType o) { if (o!=Latex) active=FALSE; }
    bool isEnabled(OutputType o) { return (o==Latex && active); } 
    OutputGenerator *get(OutputType o) { return (o==Latex) ? this : 0; }

    void printDoc(DocNode *,const char *);

    void startFile(const char *name,const char *manName,const char *title);
    void writeFooter() {}
    void endFile();
    void clearBuffer();
    
    void startIndexSection(IndexSections);
    void endIndexSection(IndexSections);
    void startProjectNumber();
    void endProjectNumber() {}
    void writeStyleInfo(int part);
    void startTitleHead(const char *);
    void startTitle();
    void endTitleHead(const char *,const char *name);
    void endTitle()   { t << "}"; }

    void newParagraph();
    void writeString(const char *text);
    void startIndexList() { t << "\\begin{CompactList}"    << endl; }
    void endIndexList()   { t << "\\end{CompactList}"      << endl; }
    void startIndexKey();
    void endIndexKey();
    void startIndexValue(bool);
    void endIndexValue(const char *,bool);
    void startItemList()  { t << "\\begin{CompactItemize}" << endl; }
    void endItemList()    { t << "\\end{CompactItemize}"   << endl; }
    void writeIndexItem(const char *ref,const char *file,const char *name);
    void docify(const char *text);
    void codify(const char *text);
    void writeObjectLink(const char *ref,const char *file,
                         const char *anchor,const char *name);
    void writeCodeLink(const char *ref, const char *file,
                       const char *anchor,const char *name);
    void startTextLink(const char *,const char *);
    void endTextLink();
    void startHtmlLink(const char *url);
    void endHtmlLink();
    void startTypewriter() { t << "{\\tt "; }
    void endTypewriter()   { t << "}";      }
    void startGroupHeader();
    void endGroupHeader();
    void writeListItem() { t << "\\item " << endl; }

    void startMemberSections() {}
    void endMemberSections() {} 
    void startMemberHeader();
    void endMemberHeader();
    void startMemberSubtitle() {}
    void endMemberSubtitle() {}
    void startMemberDocList() {}
    void endMemberDocList() {}
    void startMemberList();
    void endMemberList();
    void startAnonTypeScope(int);
    void endAnonTypeScope(int);
    void startMemberItem(int);
    void endMemberItem();
    void startMemberTemplateParams() {}
    void endMemberTemplateParams() {}

    void startMemberGroupHeader(bool);
    void endMemberGroupHeader();
    void startMemberGroupDocs();
    void endMemberGroupDocs();
    void startMemberGroup();
    void endMemberGroup(bool);
    
    void insertMemberAlign(bool) {}

    void writeRuler() { t << endl << endl; /*t << "\\vspace{0.4cm}\\hrule\\vspace{0.2cm}" << endl; */ }
    void writeAnchor(const char *fileName,const char *name);
    void startCodeFragment() { t << endl << endl << "\\begin{Code}\\begin{verbatim}"; }
    void endCodeFragment()   { t << "\\end{verbatim}\\end{Code}" << endl; }
    void writeLineNumber(const char *,const char *,const char *,int l) { t << l << " "; }
    void startCodeLine() { col=0; }
    void endCodeLine() { codify("\n"); }
    void startEmphasis() { t << "{\\em ";  }
    void endEmphasis()   { t << "}"; }
    void startBold()     { t << "\\textbf{"; }
    void endBold()       { t << "}"; }
    void startDescription();
    void endDescription();
    void startDescItem();
    void endDescItem();
    void lineBreak() { t << "\\par\n"; }
    void startMemberDoc(const char *,const char *,const char *,const char *);
    void endMemberDoc(bool);
    void startDoxyAnchor(const char *,const char *,const char *,const char *,const char *);
    void endDoxyAnchor(const char *,const char *);
    void startCodeAnchor(const char *) {}
    void endCodeAnchor() {}
    void writeChar(char c);
    void writeLatexSpacing() { t << "\\hspace{0.3cm}"; }
    void writeStartAnnoItem(const char *type,const char *file, 
                            const char *path,const char *name);
    void writeEndAnnoItem(const char *name);
    void startSubsection() { t << "\\subsection*{"; }
    void endSubsection() { t << "}" << endl; }
    void startSubsubsection() { t << "\\subsubsection*{"; }
    void endSubsubsection() { t << "}" << endl; }
    void startCenter()      { t << "\\begin{center}" << endl; }
    void endCenter()        { t << "\\end{center}" << endl; }
    void startSmall()       { t << "\\footnotesize "; }
    void endSmall()         { t << "\\normalsize "; }
    void startMemberDescription();
    void endMemberDescription();
    void startDescList(SectionTypes)     { t << "\\begin{Desc}\n\\item["; }
    void endDescList()       { t << "\\end{Desc}" << endl; }
    void startSimpleSect(SectionTypes,const char *,const char *,const char *);
    void endSimpleSect();
    void startParamList(ParamListTypes,const char *title);
    void endParamList();
    void writeDescItem()     { t << "\\par" << endl; }
    void startSection(const char *,const char *,SectionInfo::SectionType);
    void endSection(const char *,SectionInfo::SectionType);
    void addIndexItem(const char *,const char *);
    void startIndent()       {}
    void endIndent()         {}
    void writeSynopsis()     {}
    void startClassDiagram();
    void endClassDiagram(ClassDiagram &,const char *,const char *);
    void startPageRef();
    void endPageRef(const char *,const char *);
    void writeQuickLinks(bool,HighlightedItem) {}
    void writeNonBreakableSpace(int);
    
    void startDescTable()
    { t << "\\begin{description}" << endl; }
    void endDescTable()
    { t << "\\end{description}" << endl; }
    void startDescTableTitle()
    { t << "\\item[{\\em " << endl; }
    void endDescTableTitle()
    { t << "}]"; }
    void startDescTableData() {}
    void endDescTableData() {}
    void lastIndexPage() {}

    void startDotGraph();
    void endDotGraph(DotClassGraph &);
    void startInclDepGraph();
    void endInclDepGraph(DotInclDepGraph &);
    void startCallGraph();
    void startGroupCollaboration();
    void endGroupCollaboration(DotGroupCollaboration &g);
    void endCallGraph(DotCallGraph &);
    void startDirDepGraph();
    void endDirDepGraph(DotDirDeps &g);
    void writeGraphicalHierarchy(DotGfxHierarchyTable &) {}

    void startTextBlock(bool) {}
    void endTextBlock(bool) {}

    void startMemberDocPrefixItem() {}
    void endMemberDocPrefixItem() {}
    void startMemberDocName(bool) {}
    void endMemberDocName() {}
    void startParameterType(bool,const char *) {}
    void endParameterType() {}
    void startParameterName(bool) {}
    void endParameterName(bool,bool,bool) {}
    void startParameterList(bool) {}
    void endParameterList() {}

    void startFontClass(const char *) {}
    void endFontClass() {}

    void writeCodeAnchor(const char *) {}

  private:
    LatexGenerator(const LatexGenerator &);
    LatexGenerator &operator=(const LatexGenerator &);
    int col;
    bool insideTabbing;
    bool firstDescItem;
    QCString relPath;
    int m_indent;
};

#endif
