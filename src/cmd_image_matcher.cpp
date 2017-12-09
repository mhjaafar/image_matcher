/*
cmd_opencv_tutorial.cpp

Milan Multi-Platform Imaging Language, an open source image processing library

Copyright (C) 2017 Mohamad Hafiz Jaafar

h_da University of Applied Sciences
Faculty of Mathematics and Science
Dept. Computer Vision and Photonics

Schoefferstrasse 3
D-64295 Darmstadt
Germany

This program is free software; you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation version 2 of the License

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write
to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA

Dieses Programm ist freie Software. Sie koennen es unter den Bedingungen der GNU General Public License, wie
von der Free Software Foundation veroeffentlicht, weitergeben und/oder modifizieren, gemaess Version 2 der Lizenz.

Die Veroeffentlichung dieses Programms erfolgt in der Hoffnung, dass es Ihnen von Nutzen sein wird, aber OHNE
IRGENDEINE GARANTIE, sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT FUER EINEN BESTIMMTEN
ZWECK. Details finden Sie in der GNU General Public License.

Sie sollten ein Exemplar der GNU General Public License zusammen mit diesem Programm erhalten haben. Falls nicht,
schreiben Sie an die Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA.
*/
#include <cmath>
#include <math.h>
#include <vector>
#include <list>
#include <algorithm>
#include <iomanip>
#include <stdio.h>

#ifdef __GNU__
#include <fcntl.h>
#include <unistd.h>	// STDIN_FILENO
#include <sys/times.h>
#endif

#ifndef _MSC_VER
#pragma warning(disable : 4003 4996) // Avoiding warnings from e.g. PCL and security warnings for fopen
#endif

#include <QApplication>
#include <QAction>
#include <QString>
#include <QObject>
#include <QDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QCoreApplication>
#include <QMessageBox>
#include <QtGui>
#include <QTime>
#include <QFileDialog>
#include <QtCore>
#include <QDesktopWidget>
#include <QRect>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <opencv2/opencv.hpp>
#include "headers/cmd_image_matcher.hpp"
#include "headers/robustMatcher.hpp"
#include "headers/matcher.hpp"
#include "headers/image_matcher_constants.hpp"
#include "headers/ImageMatcherToolBoxWidget.hpp"
#include <milan_core/utils/inifile.h> // Testing the usage of MIniFile in the Dummy command.

ImageMatcherToolBoxWidget* gbl_pImageMatcherToolBoxWidget = 0;

MCmdImageMatcherShowToolBox::MCmdImageMatcherShowToolBox(MCommandMgrIF* pCmdMgr) : MImgProcCmdCv(pCmdMgr, "", "", false){
    m_menu.push_back(tr("&Plugins"));
    m_menu.push_back(tr(ImgMatcher::PluginNameInCommand.c_str()));
    m_menu.push_back(tr("Image Matcher Toolbox"));

    m_shortCut = ImgMatcher::CmdToolBox;
    m_hasIcon = true;
    m_icon = QIcon(":/resources/humanTransmutationCircle.ico");
    m_toolBar = ImgMatcher::CmdToolBox;
    m_needsActiveImage = false;
    m_autoCreateNewDoc = false;
}

bool MCmdImageMatcherShowToolBox::execute(MWorkspaceIF* pWs, MParameterMap* pParamMap){
    if (gbl_pImageMatcherToolBoxWidget == 0){
	    gbl_pImageMatcherToolBoxWidget = new ImageMatcherToolBoxWidget(pWs);
    }

    pWs->toolBoxIF()->addToolBoxWidget(gbl_pImageMatcherToolBoxWidget);

    return true;
}

MCmdImageMatcherRestorePreviousImage::MCmdImageMatcherRestorePreviousImage(MCommandMgrIF* pCmdMgr) : MImgProcCmdCv(pCmdMgr, "", "", false){
    m_menu.push_back(tr("&Plugins"));
    m_menu.push_back(tr(ImgMatcher::PluginNameInCommand.c_str()));
    m_menu.push_back(tr("Restore previous image"));

    m_shortCut = ImgMatcher::CmdDummy;
    m_needsActiveImage = false;
    m_autoCreateNewDoc = false;
}

bool MCmdImageMatcherRestorePreviousImage::execute(MWorkspaceIF* pWs, MParameterMap* pParamMap){
	
    MIniFileIF* settings = pWs->settingsIF();
    QString filePath;

    settings->getItem("Paths/lastImgPathName", filePath, QDir::homePath());

    // Test if file does exist:
    QFileInfo file(filePath);

    if (file.exists() && file.isFile()){

        QString fileSuffix = file.suffix();

        if (fileSuffix == "ply" || fileSuffix == "pcb" || fileSuffix == "pct"
            || fileSuffix == "PLY"
            || fileSuffix == "PCB"
            || fileSuffix == "stl"
            || fileSuffix == "STL"){
            pWs->logWndIF()->addMsg("The last opened image file is a point cloud!");
            return false;
        }
        else{
            // Read-in the last image file
            cv::Mat newImage = cv::imread(filePath.toStdString());

            if (newImage.rows == 0 || newImage.cols == 0){
                pWs->logWndIF()->addMsg("Empty file!");
                return false;
            }

            MDocument* pDoc = pWs->docMgrIF()->activeDoc();
            MImageDocCvIF* pNewDoc = NULL;

            if (pDoc == NULL){
                pNewDoc = (MImageDocCvIF*)pWs->docMgrIF()->newDocument(MImageDocCvIF::getClassName());
            }
            else if (pDoc->className() == MImageDocCvIF::getClassName()){
                pNewDoc = (MImageDocCvIF*)pDoc;
            }
            else{
                pNewDoc = (MImageDocCvIF*)pWs->docMgrIF()->newDocument(MImageDocCvIF::getClassName());
            }

            pNewDoc->docName(filePath);
            pNewDoc->filePath(filePath);

            pNewDoc->replaceActiveImage(newImage, true);
        } // END: else
    } // END: if (file.exists() && file.isFile())

    return true;
}// END: bool MCmdImageMatcherRestorePreviousImage::execute()

MCmdImageMatcherDummy::MCmdImageMatcherDummy(MCommandMgrIF* pCmdMgr) : MImgProcCmdCv(pCmdMgr, "", "", false){
    m_menu.push_back(tr("&Plugins"));
    m_menu.push_back(tr(ImgMatcher::PluginNameInCommand.c_str()));
    m_menu.push_back(tr("Image Matcher Dummy"));

    m_shortCut = ImgMatcher::CmdDummy;
    m_hasIcon = true;
    m_icon = QIcon(":/resources/humanTransmutationCircle.ico");
    m_needsActiveImage = false;
    m_autoCreateNewDoc = false;
}

bool MCmdImageMatcherDummy::execute(MWorkspaceIF* pWs, MParameterMap* pParamMap){

	//MIniFile* pPluginSettings = new MIniFile(pWs, QCoreApplication::applicationDirPath() + "/" + ImgMatcher::IniFileName);
	//pPluginSettings->setItem("Plugin", QString("Image Matcher"), true);
	//pPluginSettings->removeItem("Dummy");

	QString pathToXmlFile = QCoreApplication::applicationDirPath() + "/" + ImgMatcher::XmlFileName;
	QString pathToXmlFileOutput = QCoreApplication::applicationDirPath() + "/" + ImgMatcher::XmlFileOutputName;
	//pWs->logWndIF()->addMsg("Path to Xml file: " + pathToXmlFile);
	QFile inputFile(pathToXmlFile);
	QFile outputFile(pathToXmlFileOutput);

	if (!inputFile.exists()){
		pWs->logWndIF()->addMsg(ImgMatcher::XmlFileName + " does NOT exist.");
		return false;
	}

	if (inputFile.open(QIODevice::ReadOnly)){
		pWs->logWndIF()->addMsg("Input file CAN be opened.");
	}
	else{
		pWs->logWndIF()->addMsg("Input file CANNOT be opened.");
		return false;
	}

	if (!outputFile.open(QIODevice::WriteOnly))
	{
		pWs->logWndIF()->addMsg("Output file CANNOT be opened.");
		return false;
	}

	QXmlStreamReader reader(&inputFile);
	QXmlStreamWriter writer(&outputFile);

	while (!reader.atEnd()){
		reader.readNext();

		if (reader.hasError()){
			pWs->logWndIF()->addMsg("Reader error at: " + QString::number(reader.lineNumber()) + ", " + QString::number(reader.columnNumber()));
		}
		else{
			writer.writeCurrentToken(reader);
		}
	}

	if (reader.hasError() == false){
		pWs->logWndIF()->addMsg("reader has 0 error.");
	}
	else
		pWs->logWndIF()->addMsg("reader HAS error.");

	return true;
} // END: bool MCmdImageMatcherDummy::execute()

MCmdImageMatcherInitDocViews::MCmdImageMatcherInitDocViews(MCommandMgrIF* pCmdMgr) : MImgProcCmdCv(pCmdMgr, "", "", false){
    m_menu.push_back(tr("&Plugins"));
    m_menu.push_back(tr(ImgMatcher::PluginNameInCommand.c_str()));
    m_menu.push_back(tr("Initialize views"));

    m_shortCut = ImgMatcher::CmdInitDocViews;
    m_hasIcon = false;
    m_needsActiveImage = false;
    m_autoCreateNewDoc = false;
}

bool MCmdImageMatcherInitDocViews::execute(MWorkspaceIF* pWs, MParameterMap* pParamMap){

	MImageDocCvIF* pImgDocSampleCurrent = NULL;
	MImageDocCvIF* pImgDocSampleNext = NULL;
	MImageDocCvIF* pImgDocTargetCurrent = NULL;
	MImageDocCvIF* pImgDocTargetNext = NULL;

	// Current sample image
	pImgDocSampleCurrent = (MImageDocCvIF*)pWs->docMgrIF()->docByObjName(ImgMatcher::DocNameImgSampleCurrent);
	if (pImgDocSampleCurrent == NULL){
		pImgDocSampleCurrent = (MImageDocCvIF*)pWs->docMgrIF()->newDocument(MImageDocCvIF::getClassName());
		pImgDocSampleCurrent->setObjectName(ImgMatcher::DocNameImgSampleCurrent);
		pImgDocSampleCurrent->docName(ImgMatcher::DocNameImgSampleCurrent);
	}
	else if (pImgDocSampleCurrent->className() != MImageDocCvIF::getClassName()){
		pImgDocSampleCurrent = (MImageDocCvIF*)pWs->docMgrIF()->newDocument(MImageDocCvIF::getClassName());
		pImgDocSampleCurrent->setObjectName(ImgMatcher::DocNameImgSampleCurrent);
		pImgDocSampleCurrent->docName(ImgMatcher::DocNameImgSampleCurrent);
	}

	// Next sample image
	pImgDocSampleNext = (MImageDocCvIF*)pWs->docMgrIF()->docByObjName(ImgMatcher::DocNameImgSampleNext);
	if (pImgDocSampleNext == NULL){
		pImgDocSampleNext = (MImageDocCvIF*)pWs->docMgrIF()->newDocument(MImageDocCvIF::getClassName());
		pImgDocSampleNext->setObjectName(ImgMatcher::DocNameImgSampleNext);
		pImgDocSampleNext->docName(ImgMatcher::DocNameImgSampleNext);
	}
	else if (pImgDocSampleNext->className() != MImageDocCvIF::getClassName()){
		pImgDocSampleNext = (MImageDocCvIF*)pWs->docMgrIF()->newDocument(MImageDocCvIF::getClassName());
		pImgDocSampleNext->setObjectName(ImgMatcher::DocNameImgSampleNext);
		pImgDocSampleNext->docName(ImgMatcher::DocNameImgSampleNext);
	}

	// Current target image
	pImgDocTargetCurrent = (MImageDocCvIF*)pWs->docMgrIF()->docByObjName(ImgMatcher::DocNameImgTargetCurrent);
	if (pImgDocTargetCurrent == NULL){
		pImgDocTargetCurrent = (MImageDocCvIF*)pWs->docMgrIF()->newDocument(MImageDocCvIF::getClassName());
		pImgDocTargetCurrent->setObjectName(ImgMatcher::DocNameImgTargetCurrent);
		pImgDocTargetCurrent->docName(ImgMatcher::DocNameImgTargetCurrent);
	}
	else if (pImgDocTargetCurrent->className() != MImageDocCvIF::getClassName()){
		pImgDocTargetCurrent = (MImageDocCvIF*)pWs->docMgrIF()->newDocument(MImageDocCvIF::getClassName());
		pImgDocTargetCurrent->setObjectName(ImgMatcher::DocNameImgTargetCurrent);
		pImgDocTargetCurrent->docName(ImgMatcher::DocNameImgTargetCurrent);
	}

	// Next target image
	pImgDocTargetNext = (MImageDocCvIF*)pWs->docMgrIF()->docByObjName(ImgMatcher::DocNameImgTargetNext);
	if (pImgDocTargetNext == NULL){
		pImgDocTargetNext = (MImageDocCvIF*)pWs->docMgrIF()->newDocument(MImageDocCvIF::getClassName());
		pImgDocTargetNext->setObjectName(ImgMatcher::DocNameImgTargetNext);
		pImgDocTargetNext->docName(ImgMatcher::DocNameImgTargetNext);
	}
	else if (pImgDocTargetNext->className() != MImageDocCvIF::getClassName()){
		pImgDocTargetNext = (MImageDocCvIF*)pWs->docMgrIF()->newDocument(MImageDocCvIF::getClassName());
		pImgDocTargetNext->setObjectName(ImgMatcher::DocNameImgTargetNext);
		pImgDocTargetNext->docName(ImgMatcher::DocNameImgTargetNext);
	}

	pWs->mainWndIF()->windowTile();

	return true;
}// END: bool MCmdImageMatcherInitDocViews::execute()

MCommand* MImageMatcherCommands::getCommand(MCommandMgrIF* pCmdMgr, QString& commandName){
    if (commandName == ImgMatcher::CmdToolBox)
        return new MCmdImageMatcherShowToolBox(pCmdMgr);
    if (commandName == ImgMatcher::CmdDummy)
        return new MCmdImageMatcherDummy(pCmdMgr);
    if (commandName == ImgMatcher::CmdRestorePreviousImage)
        return new MCmdImageMatcherRestorePreviousImage(pCmdMgr);
    if (commandName == ImgMatcher::CmdInitDocViews)
        return new MCmdImageMatcherInitDocViews(pCmdMgr);
    return 0;
}

/*
	@brief: This returns the commands to be made available for the plugin.
	The order of these commands determines their order from the pull-down menu bar.
*/
QStringList MImageMatcherCommands::commands() const{
    return QStringList()
        << ImgMatcher::CmdToolBox
        << ImgMatcher::CmdDummy
        << ImgMatcher::CmdRestorePreviousImage
        << ImgMatcher::CmdInitDocViews;
}

bool MImageMatcherCommands::onModuleStartup(MWorkspaceIF& ws){
    return true;
}

void MImageMatcherCommands::onModuleShutdown(MWorkspaceIF& ws){
}