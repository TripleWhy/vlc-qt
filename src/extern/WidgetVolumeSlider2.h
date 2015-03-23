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

#ifndef VLCQT_WIDGETVOLUMESLIDER_H_
#define VLCQT_WIDGETVOLUMESLIDER_H_

#include "util/input_slider.hpp"
#include "SharedExportExtern.h"

class VlcAudio;
class VlcMediaPlayer;

/*!
    \class VlcWidgetVolumeSlider2 WidgetVolumeSlider.h vlc-qt/WidgetVolumeSlider.h
    \brief Volume slider widget

    This is one of VLC-Qt GUI classes.
    It provides graphical volume control and also visual display of current volume.
*/
class VLCQT_EXTERN_EXPORT VlcWidgetVolumeSlider2 : public SoundSlider
{
Q_OBJECT
public:
    /*!
        \brief VlcWidgetVolumeSlider2 constructor

        \param player media player (VlcMediaPlayer *)
        \param parent volume slider's parent GUI widget (QWidget)
    */
    explicit VlcWidgetVolumeSlider2(VlcMediaPlayer *player,
                                   QWidget *parent = 0);

    /*!
        \brief VlcWidgetVolumeSlider2 constructor
        \param parent volume slider's parent GUI widget (QWidget)
    */
    explicit VlcWidgetVolumeSlider2(QWidget *parent = 0);

    /*!
        \brief VlcWidgetVolumeSlider2 destructor
    */
    ~VlcWidgetVolumeSlider2();


    /*!
        \brief Set media player if initialised without it
        \param player media player (VlcMediaPlayer *)
    */
    void setMediaPlayer(VlcMediaPlayer *player);

    /*!
        \brief Does not really belong here. Left it for compatibility with VlcWidgetVolumeSlider.
    */
    bool mute() const;

    /*!
        \brief Get volume for current media
        \return volume number from 0 to 200 (const int)
    */
    int volume() const;

public slots:
    /*!
        \brief Toggle mute
        \param enabled new mutte status (bool)
    */
    void setMute(bool enabled);

    /*!
        \brief Set volume for current media
        \param volume number from 0 to 200 (int)
    */
    void setVolume(int volume);

    /*!
        \brief Decreases or increases volume for 1, depending on the parameter.

        This function is limited to the slider's range.

        \param up if true increases the volume (bool)
    */
    void volumeControl(bool up);

    /*!
        \brief Decreases volume for 1.

        This function is provided for convenience.

        \sa volumeControl()
    */
    void volumeDown() { volumeControl(false); }

    /*!
        \brief Increases volume for 1.

        This function is provided for convenience.

        \sa volumeControl()
    */
    void volumeUp() { volumeControl(true); }

signals:
    /*!
        \brief Signal for new volume
        \param int new volume (int)
    */
    void newVolume(int);

private:
    void initWidgetVolumeSlider();

    VlcAudio *_vlcAudio;
    VlcMediaPlayer *_vlcMediaPlayer;
};

#endif // VLCQT_WIDGETVOLUMESLIDER_H_
