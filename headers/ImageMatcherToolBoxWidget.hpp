/*
ImageMatcherToolBoxWidget.hpp

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
#ifndef IMAGEMATCHERTOOLBOXWIDGET_HPP
#define IMAGEMATCHERTOOLBOXWIDGET_HPP

#include <QDockWidget>
#include <QObject>
#include <QString>
#include <QtGui>
#include <QtWidgets>

#include "ui_ImageMatcherToolBoxWidget.h"
#include "toolboxmgr.h"
#include <libmilan/milan.h>
#include <libmilan/cv.h>
#include <workspace.h>

namespace Ui{
	class ImageMatcherToolBoxWidget;
}

class ImageMatcherToolBoxWidget : public MToolBoxWidget, public Ui::ImageMatcherToolBoxWidget{
	// Macro
	Q_OBJECT
public:
	ImageMatcherToolBoxWidget(MWorkspaceIF* pWs);
	MWorkspaceIF* m_pWs;
	virtual ~ImageMatcherToolBoxWidget(void);

public slots:
	virtual QString title() { return tr("Image Matcher Toolbox"); }
	virtual QIcon icon() { return QIcon(":/resources/humanTransmutationCircle.ico"); }

	// Reaction to buttons:
	void onBtnRestoreImage();
	void onBtnDummy();
	void onBtnInitViewers();

	void onSettingsChanged();

private:
	void iniFileDataExchange(bool bDirection);
};

#endif