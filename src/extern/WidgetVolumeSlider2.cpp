/****************************************************************************
* VLC-Qt - Qt and libvlc connector library
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "WidgetVolumeSlider2.h"
#include <QtCore/QTimer>
#include <QtGui/QMouseEvent>

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QHBoxLayout>
    #include <QtWidgets/QLabel>
    #include <QtWidgets/QSlider>
#else
    #include <QtGui/QHBoxLayout>
    #include <QtGui/QLabel>
    #include <QtGui/QSlider>
#endif

#include "core/Audio.h"
#include "core/Error.h"
#include "core/MediaPlayer.h"

VlcWidgetVolumeSlider2::VlcWidgetVolumeSlider2(VlcMediaPlayer *player,
                                             QWidget *parent)
    : SoundSlider(parent, 12.8, "153;210;153;20;210;20;255;199;15;245;39;29", 125)
{
    setMediaPlayer(player);

    initWidgetVolumeSlider();
}

VlcWidgetVolumeSlider2::VlcWidgetVolumeSlider2(QWidget *parent)
    : SoundSlider(parent, 12.8, "153;210;153;20;210;20;255;199;15;245;39;29", 125),
      _vlcAudio(0),
      _vlcMediaPlayer(0)
{
    initWidgetVolumeSlider();
}

VlcWidgetVolumeSlider2::~VlcWidgetVolumeSlider2()
{
}

void VlcWidgetVolumeSlider2::initWidgetVolumeSlider()
{
    connect(this, SIGNAL(valueChanged(int)), this, SIGNAL(newVolume(int)));

    setFocusPolicy( Qt::NoFocus );
    setTracking( true );
}

void VlcWidgetVolumeSlider2::setMediaPlayer(VlcMediaPlayer *player)
{
    if (player == 0)
        return;

    _vlcAudio = player->audio();
    _vlcMediaPlayer = player;

//    _timer->start(100);
}

bool VlcWidgetVolumeSlider2::mute() const
{
    if (!(_vlcMediaPlayer->state() == Vlc::Buffering ||
        _vlcMediaPlayer->state() == Vlc::Playing ||
        _vlcMediaPlayer->state() == Vlc::Paused))
        return false;
    else
        return _vlcAudio->getMute();
}

void VlcWidgetVolumeSlider2::setMute(bool enabled)
{
    SoundSlider::setMuted(enabled);
}

void VlcWidgetVolumeSlider2::setVolume(int volume)
{
    SoundSlider::setValue(volume);
}

int VlcWidgetVolumeSlider2::volume() const
{
    return value();
}

void VlcWidgetVolumeSlider2::volumeControl(bool up)
{
    if (up) {
        setValue(value() + 1);
    } else {
        setValue(value() - 1);
    }
}
