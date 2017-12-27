/*
ImageMatcherWidget.cpp

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

#include "headers/cmd_image_matcher.hpp"
#include "headers/ImageMatcherToolBoxWidget.hpp"
#include "headers/image_matcher_constants.hpp"
#include "ui_ImageMatcherToolBoxWidget.h"
#include <toolboxwidget.h>

#include <libmilan/milan.h>
#include <libmilan/objects.h>
#include <workspace_if.h>
#include <workspace.h>

using namespace ImgMatcher;

ImageMatcherToolBoxWidget::ImageMatcherToolBoxWidget(MWorkspaceIF* pWs) : MToolBoxWidget(*pWs){
    setupUi(this);

    iniFileDataExchange(false);

    connect(btnDummy, SIGNAL(clicked()), this, SLOT(onBtnDummy()));
    connect(btnRestoreImg, SIGNAL(clicked()), this, SLOT(onBtnRestoreImage()));
    connect(btnInitViewers, SIGNAL(clicked()), this, SLOT(onBtnInitViewers()));

    connect(comboBox_savedTasks, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxSavedTasksCurrentIndexChanged()));
}// END: ImageMatcherWidget::ImageMatcherWidget(MWorkspaceIF* pWs)

ImageMatcherToolBoxWidget::~ImageMatcherToolBoxWidget(void){
    // Empty destructors
}

void ImageMatcherToolBoxWidget::onSettingsChanged(){
    iniFileDataExchange(true);
}

void ImageMatcherToolBoxWidget::onBtnDummy(){
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_ws.cmdMgrIF()->executeCommand(ImgMatcher::CmdDummy);
    QApplication::restoreOverrideCursor();
}

void ImageMatcherToolBoxWidget::onBtnRestoreImage(){
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_ws.cmdMgrIF()->executeCommand(ImgMatcher::CmdRestorePreviousImage);
    QApplication::restoreOverrideCursor();
}

void ImageMatcherToolBoxWidget::onBtnInitViewers(){
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_ws.cmdMgrIF()->executeCommand(ImgMatcher::CmdInitDocViews);
    QApplication::restoreOverrideCursor();
}

void ImageMatcherToolBoxWidget::onComboBoxSavedTasksCurrentIndexChanged(){
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_ws.cmdMgrIF()->executeCommand(ImgMatcher::CmdPrintCurrentPickedTask);
    QApplication::restoreOverrideCursor();
}

void ImageMatcherToolBoxWidget::iniFileDataExchange(bool bDirection){
    MIniFileIF* pif = m_ws.settingsIF();

    if (bDirection){
        //pif->setItem(PluginName + "/" + QToolBoxes + "/" + PixelManipulation, tbManipulation->currentIndex());
        pif->flush();
    } // END: if (bDirection)
    else{
        //int m;
        //double d;
        //bool b;

        //pif->getItem(PluginName + "/" + QToolBoxes + "/" + PixelManipulation, m, 0);
        //tbManipulation->setCurrentIndex(m);
    } // END: else
} // END: void ImageMatcherToolBoxWidget::iniFileDataExchange(bool bDirection)
