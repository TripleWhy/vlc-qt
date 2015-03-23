#if 1
/*****************************************************************************
 * controller_widget.cpp : Controller Widget for the controllers
 ****************************************************************************
 * Copyright (C) 2006-2008 the VideoLAN team
 * $Id: db27f3b4c4fb9ac20c4c7d886e55d139bc4f739b $
 *
 * Authors: Jean-Baptiste Kempf <jb@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include "qt4.hpp"

#include "SoundWidget.h"
#include "Audio.h"

#include "WidgetVolumeSlider2.h"     /* SoundSlider */

#include <math.h>

#include <QLabel>
#include <QHBoxLayout>
#include <QMenu>
#include <QWidgetAction>
#include <QMouseEvent>

#include "core/MediaPlayer.h"

SoundWidget::SoundWidget( QWidget *_parent, VlcMediaPlayer * player,
                          bool b_shiny, bool b_special )
                         : QWidget( _parent ),
                           b_is_muted( false ),
                           b_ignore_valuechanged( false ),
                           volMuteLabel(0)
{
    if (player != 0)
        init(player, b_shiny, b_special);
}

void SoundWidget::init(VlcMediaPlayer * player, bool b_shiny, bool b_special)
{
    if (volMuteLabel != 0)  //already initialized
        return;

    /* We need a layout for this widget */
    QHBoxLayout *layout = new QHBoxLayout( this );
    layout->setSpacing( 0 ); layout->setMargin( 0 );

    /* We need a Label for the pix */
    volMuteLabel = new QLabel;
    volMuteLabel->setPixmap( QPixmap( ":/toolbar/volume-medium" ) );

    /* We might need a subLayout too */
    QVBoxLayout *subLayout;

    volMuteLabel->installEventFilter( this );

    /* Normal View, click on icon mutes */
    if( !b_special )
    {
        volumeMenu = NULL; subLayout = NULL;
        volumeControlWidget = NULL;

        /* And add the label */
        layout->addWidget( volMuteLabel, 0, b_shiny? Qt::AlignBottom : Qt::AlignCenter );
    }
    else
    {
        /* Special view, click on button shows the slider */
        b_shiny = false;

        volumeControlWidget = new QFrame( this );
        subLayout = new QVBoxLayout( volumeControlWidget );
        subLayout->setContentsMargins( 4, 4, 4, 4 );
        volumeMenu = new QMenu( this );

        QWidgetAction *widgetAction = new QWidgetAction( volumeControlWidget );
        widgetAction->setDefaultWidget( volumeControlWidget );
        volumeMenu->addAction( widgetAction );

        /* And add the label */
        layout->addWidget( volMuteLabel );
    }

    /* Slider creation: shiny or clean */
    if( b_shiny )
    {
        volumeSlider = new VlcWidgetVolumeSlider2( player, this );
    }
    else
    {
        volumeSlider = new QSlider( NULL );
        volumeSlider->setAttribute( Qt::WA_MacSmallSize);
        volumeSlider->setOrientation( b_special ? Qt::Vertical
                                                : Qt::Horizontal );
        volumeSlider->setMaximum( 200 );
    }

    volumeSlider->setFocusPolicy( Qt::NoFocus );
    if( b_special )
        subLayout->addWidget( volumeSlider );
    else
        layout->addWidget( volumeSlider, 0, b_shiny? Qt::AlignBottom : Qt::AlignCenter );

    vlcAudio = player->audio();
    /* Set the volume from the config */
    int volume = vlcAudio->volume();
    libUpdateVolume( (volume > 0) ? volume : 100 );
    /* Sync mute status */
    if( vlcAudio->getMute() )
        updateMuteStatus( true );

    /* Volume control connection */
    volumeSlider->setTracking( true );
    connect( volumeSlider, SIGNAL(valueChanged( int )), this, SLOT(valueChangedFilter( int )) );
    connect( this, SIGNAL(valueReallyChanged( int )), this, SLOT(userUpdateVolume( int )) );
    connect( vlcAudio, SIGNAL(volumeChanged( int )), this, SLOT(libUpdateVolume( int )) );
    connect( vlcAudio, SIGNAL(muteChanged( bool )), this, SLOT(updateMuteStatus( bool )) );
}

void SoundWidget::refreshLabels()
{
    int i_sliderVolume = volumeSlider->value();

    if( b_is_muted )
    {
        volMuteLabel->setPixmap( QPixmap(":/toolbar/volume-muted" ) );
        volMuteLabel->setToolTip(qfu(vlc_pgettext("Tooltip|Unmute", "Unmute")));
        return;
    }

    if( i_sliderVolume < VOLUME_MAX / 3 )
        volMuteLabel->setPixmap( QPixmap( ":/toolbar/volume-low" ) );
    else if( i_sliderVolume > (VOLUME_MAX * 2 / 3 ) )
        volMuteLabel->setPixmap( QPixmap( ":/toolbar/volume-high" ) );
    else volMuteLabel->setPixmap( QPixmap( ":/toolbar/volume-medium" ) );
    volMuteLabel->setToolTip( qfu(vlc_pgettext("Tooltip|Mute", "Mute")) );
}

/* volumeSlider changed value event slot */
void SoundWidget::userUpdateVolume( int i_sliderVolume )
{
    /* Only if volume is set by user action on slider */
    setMuted( false );
    vlcAudio->setVolume(i_sliderVolume);
    refreshLabels();
}

/* libvlc changed value event slot */
void SoundWidget::libUpdateVolume(float volume )
{
    int i_volume = qRound(volume * 100.f);
    libUpdateVolume(i_volume);
}

void SoundWidget::libUpdateVolume(int i_volume)
{
    if( i_volume != volumeSlider->value()  )
    {
        b_ignore_valuechanged = true;
        volumeSlider->setValue( i_volume );
        b_ignore_valuechanged = false;
    }
    refreshLabels();
}

void SoundWidget::valueChangedFilter( int i_val )
{
    /* valueChanged is also emitted when the lib setValue() */
    if ( !b_ignore_valuechanged ) emit valueReallyChanged( i_val );
}

/* libvlc mute/unmute event slot */
void SoundWidget::updateMuteStatus( bool mute )
{
    b_is_muted = mute;

    SoundSlider *soundSlider = qobject_cast<SoundSlider *>(volumeSlider);
    if( soundSlider )
        soundSlider->setMuted( mute );
    refreshLabels();
}

void SoundWidget::showVolumeMenu( QPoint pos )
{
    volumeMenu->setFixedHeight( volumeMenu->sizeHint().height() );
    volumeMenu->exec( QCursor::pos() - pos - QPoint( 0, volumeMenu->height()/2 )
                          + QPoint( width(), height() /2) );
}

void SoundWidget::setMuted( bool mute )
{
    b_is_muted = mute;
    vlcAudio->setMute(mute);
}

bool SoundWidget::eventFilter( QObject *obj, QEvent *e )
{
    VLC_UNUSED( obj );
    if( e->type() == QEvent::MouseButtonPress )
    {
        QMouseEvent *event = static_cast<QMouseEvent*>(e);
        if( event->button() == Qt::LeftButton )
        {
            if( volumeSlider->orientation() ==  Qt::Vertical )
            {
                showVolumeMenu( event->pos() );
            }
            else
            {
                setMuted( !b_is_muted );
            }
            e->accept();
            return true;
        }
    }
    e->ignore();
    return false;
}
#endif
