#if 1
//Slightly modified version of VLC's SoundWidget.

/*****************************************************************************
 * controller_widget.hpp : Controller Widget for the controllers
 ****************************************************************************
 * Copyright (C) 2006-2008 the VideoLAN team
 * $Id: 832017516e0c5feaf24c3a32f02b6fcc75c8001d $
 *
 * Authors: Jean-Baptiste Kempf <jb@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef _CONTROLLER_WIDGET_H_
#define _CONTROLLER_WIDGET_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

//#include <vlc_vout.h>                       /* vout_thread_t for aspect ratio combobox */

#include "SharedExportExtern.h"

#include <QWidget>

class QLabel;
class QAbstractSlider;
class QFrame;
class QMenu;
class VlcMediaPlayer;
class VlcAudio;

#define VOLUME_MAX 200
class VLCQT_EXTERN_EXPORT SoundWidget : public QWidget
{
    Q_OBJECT

public:
    SoundWidget( QWidget *parent, VlcMediaPlayer * player = 0, bool = true,
                 bool b_special = false );
    void init(VlcMediaPlayer * player, bool b_shiny = true, bool b_special = false);
    void setMuted( bool );

protected:
    virtual bool eventFilter( QObject *obj, QEvent *e );

private:
    QLabel              *volMuteLabel;
    QAbstractSlider     *volumeSlider;
    QFrame              *volumeControlWidget;
    QMenu               *volumeMenu;

    bool                b_is_muted;
    bool                b_ignore_valuechanged;

    VlcAudio            *vlcAudio;

protected slots:
    void userUpdateVolume( int );
    void libUpdateVolume( float );
    void libUpdateVolume( int );
    void updateMuteStatus( bool );
    void refreshLabels( void );
    void showVolumeMenu( QPoint pos );
    void valueChangedFilter( int );

signals:
    void valueReallyChanged( int );
};

#endif
#endif
