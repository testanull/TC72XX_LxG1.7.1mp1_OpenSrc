/******************************************************************************
 *
 * $Id: namespacedef.cpp,v 1.2 2014/11/19 09:12:51 wtchen Exp $
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

#include "qtbc.h"
#include "namespacedef.h"
#include "outputlist.h"
#include "util.h"
#include "language.h"
#include "classdef.h"
#include "classlist.h"
#include "memberlist.h"
#include "doxygen.h"
#include "message.h"
#include "docparser.h"
#include "searchindex.h"

NamespaceDef::NamespaceDef(const char *df,int dl,
                           const char *name,const char *lref,
                           const char *fName) : 
   Definition(df,dl,name)
{
  if (fName)
  {
    fileName = stripExtension(fName);
  }
  else
  {
    fileName="namespace";
    fileName+=name;
  }
  classSDict = new ClassSDict(17);
  namespaceSDict = new NamespaceSDict(17);
  m_innerCompounds = new SDict<Definition>(17);
  usingDirList = 0;
  usingDeclList = 0;
  setReference(lref);
  memberGroupSDict = new MemberGroupSDict;
  memberGroupSDict->setAutoDelete(TRUE);
  visited=FALSE;
}

NamespaceDef::~NamespaceDef()
{
  delete classSDict;
  delete namespaceSDict;
  delete m_innerCompounds;
  delete usingDirList;
  delete usingDeclList;
  delete memberGroupSDict;
}

void NamespaceDef::distributeMemberGroupDocumentation()
{
  MemberGroupSDict::Iterator mgli(*memberGroupSDict);
  MemberGroup *mg;
  for (;(mg=mgli.current());++mgli)
  {
    mg->distributeMemberGroupDocumentation();
  }
}

void NamespaceDef::findSectionsInDocumentation()
{
  docFindSections(documentation(),this,0,docFile());
  MemberGroupSDict::Iterator mgli(*memberGroupSDict);
  MemberGroup *mg;
  for (;(mg=mgli.current());++mgli)
  {
    mg->findSectionsInDocumentation();
  }
  decDefineMembers.findSectionsInDocumentation();
  decProtoMembers.findSectionsInDocumentation();
  decTypedefMembers.findSectionsInDocumentation();
  decEnumMembers.findSectionsInDocumentation();
  decFuncMembers.findSectionsInDocumentation();
  decVarMembers.findSectionsInDocumentation();
}

void NamespaceDef::insertUsedFile(const char *f)
{
  if (files.find(f)==-1) 
  {
    if (Config_getBool("SORT_MEMBER_DOCS"))
      files.inSort(f);
    else
      files.append(f);
  }
}

void NamespaceDef::addInnerCompound(Definition *d)
{
  m_innerCompounds->append(d->localName(),d);
  if (d->definitionType()==Definition::TypeNamespace)
  {
    insertNamespace((NamespaceDef *)d);
  }
  else if (d->definitionType()==Definition::TypeClass)
  {
    insertClass((ClassDef *)d);
  }
}

void NamespaceDef::insertClass(ClassDef *cd)
{
  if (classSDict->find(cd->name())==0)
  {
    if (Config_getBool("SORT_BRIEF_DOCS"))
      classSDict->inSort(cd->name(),cd);
    else
      classSDict->append(cd->name(),cd);
  }
}

void NamespaceDef::insertNamespace(NamespaceDef *nd)
{
  if (namespaceSDict->find(nd->name())==0)
  {
    if (Config_getBool("SORT_MEMBER_DOCS"))
      namespaceSDict->inSort(nd->name(),nd);
    else
      namespaceSDict->append(nd->name(),nd);
  }
}


void NamespaceDef::addMembersToMemberGroup()
{
  ::addMembersToMemberGroup(&decDefineMembers,memberGroupSDict,this);
  ::addMembersToMemberGroup(&decProtoMembers,memberGroupSDict,this);
  ::addMembersToMemberGroup(&decTypedefMembers,memberGroupSDict,this);
  ::addMembersToMemberGroup(&decEnumMembers,memberGroupSDict,this);
  ::addMembersToMemberGroup(&decFuncMembers,memberGroupSDict,this);
  ::addMembersToMemberGroup(&decVarMembers,memberGroupSDict,this);
}

void NamespaceDef::insertMember(MemberDef *md)
{
  //memList->append(md);
  allMemberList.append(md); 
  static bool sortBriefDocs=Config_getBool("SORT_BRIEF_DOCS");
  switch(md->memberType())
  {
    case MemberDef::Variable:     
      if (sortBriefDocs)
        decVarMembers.inSort(md);
      else
        decVarMembers.append(md);
      if (sortBriefDocs)
        docVarMembers.inSort(md); 
      else
        docVarMembers.append(md);
      break;
    case MemberDef::Function: 
      if (sortBriefDocs)
        decFuncMembers.inSort(md);
      else
        decFuncMembers.append(md);
      if (sortBriefDocs)
        docFuncMembers.inSort(md); 
      else
        docFuncMembers.append(md);
      break;
    case MemberDef::Typedef:      
      if (sortBriefDocs)
        decTypedefMembers.inSort(md);
      else
        decTypedefMembers.append(md);
      if (sortBriefDocs)
        docTypedefMembers.inSort(md); 
      else
        docTypedefMembers.append(md);
      break;
    case MemberDef::Enumeration:  
      if (sortBriefDocs)
        decEnumMembers.inSort(md);
      else
        decEnumMembers.append(md);
      if (sortBriefDocs)
        docEnumMembers.inSort(md); 
      else
        docEnumMembers.append(md);
      break;
    case MemberDef::EnumValue:    
      break;
    case MemberDef::Prototype:    
      if (sortBriefDocs)
        decProtoMembers.inSort(md);
      else
        decProtoMembers.append(md);
      if (sortBriefDocs)
        docProtoMembers.inSort(md); 
      else
        docProtoMembers.append(md);
      break;
    case MemberDef::Define:       
      if (sortBriefDocs)
        decDefineMembers.inSort(md);
      else
        decDefineMembers.append(md);
      if (sortBriefDocs)
        docDefineMembers.inSort(md); 
      else
        docDefineMembers.append(md);
      break;
    default:
      err("NamespaceDef::insertMembers(): "
           "member `%s' with class scope `%s' inserted in namespace scope `%s'!\n",
           md->name().data(),
           md->getClassDef() ? md->getClassDef()->name().data() : "",
           name().data());
  }
  //addMemberToGroup(md,groupId);
}

void NamespaceDef::computeAnchors()
{
  setAnchors(0,'a',&allMemberList);
}

void NamespaceDef::writeDetailedDocumentation(OutputList &ol)
{
  if ((!briefDescription().isEmpty() && Config_getBool("REPEAT_BRIEF")) || 
      !documentation().isEmpty())
  {
    ol.writeRuler();
    ol.pushGeneratorState();
    ol.disableAllBut(OutputGenerator::Html);
    //bool latexOn = ol.isEnabled(OutputGenerator::Latex);
    //if (latexOn) ol.disable(OutputGenerator::Latex);
    ol.writeAnchor(0,"_details"); 
    //if (latexOn) ol.enable(OutputGenerator::Latex);
    ol.popGeneratorState();
    ol.startGroupHeader();
    ol.parseText(theTranslator->trDetailedDescription());
    ol.endGroupHeader();
    ol.startTextBlock();
    if (!briefDescription().isEmpty() && Config_getBool("REPEAT_BRIEF"))
    {
      ol.parseDoc(briefFile(),briefLine(),this,0,briefDescription(),FALSE,FALSE);
    }
    if (!briefDescription().isEmpty() && Config_getBool("REPEAT_BRIEF") &&
        !documentation().isEmpty())
    {
      ol.pushGeneratorState();
        ol.disable(OutputGenerator::Man);
        ol.newParagraph();
        ol.enableAll();
        ol.disableAllBut(OutputGenerator::Man);
        ol.writeString("\n\n");
      ol.popGeneratorState();
    }
    if (!documentation().isEmpty())
    {
      ol.parseDoc(docFile(),docLine(),this,0,documentation()+"\n",TRUE,FALSE);
      ol.newParagraph();
    }
    ol.endTextBlock();
  }
}
  
void NamespaceDef::writeDocumentation(OutputList &ol)
{
  QCString pageTitle;
  if (Config_getBool("OPTIMIZE_OUTPUT_JAVA"))
  {
    pageTitle = theTranslator->trPackage(displayName());
  }
  else
  {
    pageTitle = theTranslator->trNamespaceReference(displayName());
  }
  startFile(ol,getOutputFileBase(),name(),pageTitle,HLI_NamespaceVisible);
  if (getOuterScope()!=Doxygen::globalScope)
  {
    writeNavigationPath(ol);
  }
  startTitle(ol,getOutputFileBase());
  ol.parseText(pageTitle);
  addGroupListToTitle(ol,this);
  endTitle(ol,getOutputFileBase(),displayName());
  
  if (Config_getBool("SEARCHENGINE"))
  {
    Doxygen::searchIndex->setCurrentDoc(pageTitle,getOutputFileBase());
    Doxygen::searchIndex->addWord(localName(),TRUE);
  }

  if (!Config_getString("GENERATE_TAGFILE").isEmpty())
  {
    Doxygen::tagFile << "  <compound kind=\"namespace\">" << endl;
    Doxygen::tagFile << "    <name>" << convertToXML(name()) << "</name>" << endl;
    Doxygen::tagFile << "    <filename>" << convertToXML(getOutputFileBase()) << Doxygen::htmlFileExtension << "</filename>" << endl;
  }
  
  ol.startTextBlock();
    
  if (Config_getBool("DETAILS_AT_TOP"))
  {
    writeDetailedDocumentation(ol);
    ol.newParagraph();
  }
  else if (!briefDescription().isEmpty()) 
  {
    ol.parseDoc(briefFile(),briefLine(),this,0,briefDescription(),TRUE,FALSE);
    ol.writeString(" \n");
    ol.pushGeneratorState();
    ol.disableAllBut(OutputGenerator::Html);
    ol.startTextLink(0,"_details");
    ol.parseText(theTranslator->trMore());
    ol.endTextLink();
    ol.enableAll();
    ol.disableAllBut(OutputGenerator::Man);
    ol.newParagraph();
    ol.popGeneratorState();
  }
  ol.disable(OutputGenerator::Man);
  ol.newParagraph();
  ol.enable(OutputGenerator::Man);
  ol.writeSynopsis();

  ol.endTextBlock();
  
  ol.startMemberSections();
  classSDict->writeDeclaration(ol,0,0,TRUE);

  namespaceSDict->writeDeclaration(ol,TRUE);

  /* write user defined member groups */
  MemberGroupSDict::Iterator mgli(*memberGroupSDict);
  MemberGroup *mg;
  for (;(mg=mgli.current());++mgli)
  {
    mg->writeDeclarations(ol,0,this,0,0);
  }

  //allMemberList.writeDeclarations(ol,0,this,0,0,0,0);
  decDefineMembers.writeDeclarations(ol,0,this,0,0,theTranslator->trDefines(),0);
  decProtoMembers.writeDeclarations(ol,0,this,0,0,theTranslator->trFuncProtos(),0);
  decTypedefMembers.writeDeclarations(ol,0,this,0,0,theTranslator->trTypedefs(),0);
  decEnumMembers.writeDeclarations(ol,0,this,0,0,theTranslator->trEnumerations(),0);
  decFuncMembers.writeDeclarations(ol,0,this,0,0,theTranslator->trFunctions(),0);
  decVarMembers.writeDeclarations(ol,0,this,0,0,theTranslator->trVariables(),0);
  ol.endMemberSections();
  
  if (!Config_getBool("DETAILS_AT_TOP"))
  {
    writeDetailedDocumentation(ol);
  }

  writeMemberDocumentation(ol);

  // write Author section (Man only)
  ol.pushGeneratorState();
  ol.disableAllBut(OutputGenerator::Man);
  ol.startGroupHeader();
  ol.parseText(theTranslator->trAuthor(TRUE,TRUE));
  ol.endGroupHeader();
  ol.parseText(theTranslator->trGeneratedAutomatically(Config_getString("PROJECT_NAME")));

  if (!Config_getString("GENERATE_TAGFILE").isEmpty()) 
  {
    writeDocAnchorsToTagFile();
    Doxygen::tagFile << "  </compound>" << endl;
  }

  ol.popGeneratorState();
  endFile(ol);

  if (Config_getBool("SEPARATE_MEMBER_PAGES"))
  {
    allMemberList.sort();
    writeMemberPages(ol);
  }
}

void NamespaceDef::writeMemberDocumentation(OutputList &ol)
{
  if (Config_getBool("SEPARATE_MEMBER_PAGES"))
  {
    ol.disable(OutputGenerator::Html);
  }
  
  docDefineMembers.writeDocumentation(ol,name(),this,
                          theTranslator->trDefineDocumentation());
  
  docProtoMembers.writeDocumentation(ol,name(),this,
                          theTranslator->trFunctionPrototypeDocumentation());

  docTypedefMembers.writeDocumentation(ol,name(),this,
                          theTranslator->trTypedefDocumentation());
  
  docEnumMembers.writeDocumentation(ol,name(),this,
                          theTranslator->trEnumerationTypeDocumentation());

  docFuncMembers.writeDocumentation(ol,name(),this,
                          theTranslator->trFunctionDocumentation());
  
  docVarMembers.writeDocumentation(ol,name(),this,
                          theTranslator->trVariableDocumentation());

  if (Config_getBool("SEPARATE_MEMBER_PAGES"))
  {
    ol.enable(OutputGenerator::Html);
  }
}

void NamespaceDef::writeMemberPages(OutputList &ol)
{
  ol.pushGeneratorState();
  ol.disableAllBut(OutputGenerator::Html);

  docDefineMembers.writeDocumentationPage(ol,name(),this);
  docProtoMembers.writeDocumentationPage(ol,name(),this);
  docTypedefMembers.writeDocumentationPage(ol,name(),this);
  docEnumMembers.writeDocumentationPage(ol,name(),this);
  docFuncMembers.writeDocumentationPage(ol,name(),this);
  docVarMembers.writeDocumentationPage(ol,name(),this);

  ol.popGeneratorState();
}

void NamespaceDef::writeQuickMemberLinks(OutputList &ol,MemberDef *currentMd) const
{
  static bool createSubDirs=Config_getBool("CREATE_SUBDIRS");

  ol.writeString("      <div class=\"navtab\">\n");
  ol.writeString("        <table>\n");

  MemberListIterator mli(allMemberList);
  MemberDef *md;
  for (mli.toFirst();(md=mli.current());++mli)
  {
    if (md->getNamespaceDef()==this && md->isLinkable())
    {
      ol.writeString("          <tr><td class=\"navtab\">");
      if (md->isLinkableInProject())
      {
        if (md==currentMd) // selected item => highlight
        {
          ol.writeString("<a class=\"qindexHL\" ");
        }
        else
        {
          ol.writeString("<a class=\"qindex\" ");
        }
        ol.writeString("href=\"");
        if (createSubDirs) ol.writeString("../../");
        ol.writeString(md->getOutputFileBase()+Doxygen::htmlFileExtension+"#"+md->anchor());
        ol.writeString("\">");
        ol.writeString(md->localName());
        ol.writeString("</a>");
      }
      ol.writeString("</td></tr>\n");
    }
  }

  ol.writeString("        </table>\n");
  ol.writeString("      </div>\n");
}

int NamespaceDef::countMembers()
{
  allMemberList.countDocMembers();
  return allMemberList.numDocMembers()+classSDict->count();
}

void NamespaceDef::addUsingDirective(NamespaceDef *nd)
{
  if (usingDirList==0)
  {
    usingDirList = new NamespaceSDict;
  }
  if (usingDirList->find(nd->qualifiedName())==0)
  {
    usingDirList->append(nd->qualifiedName(),nd);
  }
}

void NamespaceDef::addUsingDeclaration(Definition *d)
{
  if (usingDeclList==0)
  {
    usingDeclList = new SDict<Definition>(17);
  }
  if (usingDeclList->find(d->qualifiedName())==0)
  {
    usingDeclList->append(d->qualifiedName(),d);
  }
}

QCString NamespaceDef::getOutputFileBase() const 
{ 
  if (isReference())
  {
    return fileName;
  }
  else
  {
    return convertNameToFile(fileName); 
  }
}

Definition *NamespaceDef::findInnerCompound(const char *n)
{
  if (n==0) return 0;
  Definition *d = m_innerCompounds->find(n);
  if (d==0)
  {
    if (usingDirList)
    {
      d = usingDirList->find(n);
    }
    if (d==0 && usingDeclList)
    {
      d = usingDeclList->find(n);
    }
  }
  return d;
}

void NamespaceDef::addListReferences()
{
  addRefItem(xrefListItems(),
             theTranslator->trNamespace(TRUE,TRUE),
             getOutputFileBase(),displayName()
            );
  MemberGroupSDict::Iterator mgli(*memberGroupSDict);
  MemberGroup *mg;
  for (;(mg=mgli.current());++mgli)
  {
    mg->addListReferences(this);
  }
  docDefineMembers.addListReferences(this);
  docProtoMembers.addListReferences(this);
  docTypedefMembers.addListReferences(this);
  docEnumMembers.addListReferences(this);
  docFuncMembers.addListReferences(this);
  docVarMembers.addListReferences(this);
}

QCString NamespaceDef::displayName() const
{
  QCString result=name();
  if (Config_getBool("OPTIMIZE_OUTPUT_JAVA"))
  {
    result = substitute(result,"::",".");
  }
  return result; 
}

void NamespaceDef::combineUsingRelations()
{
  if (visited) return; // already done
  visited=TRUE;
  if (usingDirList)
  {
    NamespaceSDict::Iterator nli(*usingDirList);
    NamespaceDef *nd;
    for (nli.toFirst();(nd=nli.current());++nli)
    {
      nd->combineUsingRelations();
    }
    for (nli.toFirst();(nd=nli.current());++nli)
    {
      // add used namespaces of namespace nd to this namespace
      if (nd->getUsedNamespaces())
      {
        NamespaceSDict::Iterator unli(*nd->getUsedNamespaces());
        NamespaceDef *und;
        for (unli.toFirst();(und=unli.current());++unli)
        {
          //printf("Adding namespace %s to the using list of %s\n",und->qualifiedName().data(),qualifiedName().data());
          addUsingDirective(und);
        }
      }
      // add used classes of namespace nd to this namespace
      if (nd->getUsedClasses())
      {
        SDict<Definition>::Iterator cli(*nd->getUsedClasses());
        Definition *ucd;
        for (cli.toFirst();(ucd=cli.current());++cli)
        {
          //printf("Adding class %s to the using list of %s\n",cd->qualifiedName().data(),qualifiedName().data());
          addUsingDeclaration(ucd);
        }
      }
    }
  }
}

void NamespaceSDict::writeDeclaration(OutputList &ol,bool localName)
{
  if (count()==0) return; // no namespaces in the list

  SDict<NamespaceDef>::Iterator ni(*this);
  NamespaceDef *nd;
  bool found=FALSE;
  for (ni.toFirst();(nd=ni.current()) && !found;++ni)
  {
    if (nd->isLinkable()) found=TRUE;
  }
  if (!found) return; // no linkable namespaces in the list

  // write list of namespaces
  ol.startMemberHeader();
  bool javaOpt = Config_getBool("OPTIMIZE_OUTPUT_JAVA");
  if (javaOpt)
  {
    ol.parseText(theTranslator->trPackages());
  }
  else
  {
    ol.parseText(theTranslator->trNamespaces());
  }
  ol.endMemberHeader();
  ol.startMemberList();
  for (ni.toFirst();(nd=ni.current());++ni)
  {
    if (nd->isLinkable())
    {
      ol.startMemberItem(0);
      if (javaOpt)
      {
        ol.docify("package ");
      }
      else
      {
        ol.docify("namespace ");
      }
      ol.insertMemberAlign();
      QCString name;
      if (localName)
      {
        name = nd->localName();
      }
      else
      {
        name = nd->displayName();
      }
      ol.writeObjectLink(nd->getReference(),nd->getOutputFileBase(),0,name);
      if (!Config_getString("GENERATE_TAGFILE").isEmpty()) 
      {
        Doxygen::tagFile << "    <namespace>" << convertToXML(nd->name()) << "</namespace>" << endl;
      }
      ol.endMemberItem();
      if (!nd->briefDescription().isEmpty() && Config_getBool("BRIEF_MEMBER_DESC"))
      {
        ol.startMemberDescription();
        ol.parseDoc(nd->briefFile(),nd->briefLine(),nd,0,nd->briefDescription(),FALSE,FALSE);
        ol.endMemberDescription();
        ol.newParagraph();
      }
    }
  }
  ol.endMemberList();
}

