/*
 * Copyright (C) 2008, 2009, 2010 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MediaControlElements_h
#define MediaControlElements_h

#if ENABLE(VIDEO)

#include "HTMLDivElement.h"
#include "HTMLInputElement.h"
#include "HTMLMediaElement.h"
#include "RenderBlock.h"

// These are the shadow elements used in RenderMedia

namespace WebCore {

class Event;
class Frame;

// Must match WebKitSystemInterface.h
enum MediaControlElementType {
    MediaFullscreenButton = 0,
    MediaMuteButton,
    MediaPlayButton,
    MediaSeekBackButton,
    MediaSeekForwardButton,
    MediaSlider,
    MediaSliderThumb,
    MediaRewindButton,
    MediaReturnToRealtimeButton,
    MediaShowClosedCaptionsButton,
    MediaHideClosedCaptionsButton,
    MediaUnMuteButton,
    MediaPauseButton,
    MediaTimelineContainer,
    MediaCurrentTimeDisplay,
    MediaTimeRemainingDisplay,
    MediaStatusDisplay,
    MediaControlsPanel,
    MediaVolumeSliderContainer,
    MediaVolumeSlider,
    MediaVolumeSliderThumb,
    MediaVolumeSliderMuteButton,
};

HTMLMediaElement* toParentMediaElement(RenderObject*);

class MediaControlShadowRootElement : public HTMLDivElement {
public:
    static PassRefPtr<MediaControlShadowRootElement> create(HTMLMediaElement*);

    virtual void detach();
    
private:
    MediaControlShadowRootElement(HTMLMediaElement*);
};

// ----------------------------

class MediaControlElement : public HTMLDivElement {
public:
    virtual void attach();
    void attachToParent(Element*);
    void update();
    void updateStyle();

    virtual MediaControlElementType displayType() const = 0;

    HTMLMediaElement* mediaElement() const { return m_mediaElement; }

protected:
    MediaControlElement(HTMLMediaElement*);

    virtual bool rendererIsNeeded(RenderStyle*);

    virtual PassRefPtr<RenderStyle> styleForElement();

private:
    virtual bool isMediaControlElement() const { return true; }

    HTMLMediaElement* m_mediaElement;   
};

// ----------------------------

class MediaControlPanelElement : public MediaControlElement {
public:
    static PassRefPtr<MediaControlPanelElement> create(HTMLMediaElement*);

private:
    MediaControlPanelElement(HTMLMediaElement*);
    virtual MediaControlElementType displayType() const;
    virtual const AtomicString& shadowPseudoId() const;
};


// ----------------------------

class MediaControlTimelineContainerElement : public MediaControlElement {
public:
    static PassRefPtr<MediaControlTimelineContainerElement> create(HTMLMediaElement*);

private:
    MediaControlTimelineContainerElement(HTMLMediaElement*);
    virtual MediaControlElementType displayType() const;
    virtual bool rendererIsNeeded(RenderStyle*);
    virtual const AtomicString& shadowPseudoId() const;
};

// ----------------------------

class MediaControlVolumeSliderContainerElement : public MediaControlElement {
public:
    static PassRefPtr<MediaControlVolumeSliderContainerElement> create(HTMLMediaElement*);

    virtual PassRefPtr<RenderStyle> styleForElement();
    void setVisible(bool);
    bool isVisible() { return m_isVisible; }
    void setPosition(int x, int y);
    bool hitTest(const IntPoint& absPoint);

private:
    MediaControlVolumeSliderContainerElement(HTMLMediaElement*);
    virtual MediaControlElementType displayType() const;
    virtual const AtomicString& shadowPseudoId() const;

    bool m_isVisible;
    int m_x, m_y;
};

// ----------------------------

class MediaControlStatusDisplayElement : public MediaControlElement {
public:
    static PassRefPtr<MediaControlStatusDisplayElement> create(HTMLMediaElement*);

    void update();

private:
    MediaControlStatusDisplayElement(HTMLMediaElement*);

    virtual MediaControlElementType displayType() const;
    virtual bool rendererIsNeeded(RenderStyle*);
    virtual const AtomicString& shadowPseudoId() const;

    enum StateBeingDisplayed { Nothing, Loading, LiveBroadcast };
    StateBeingDisplayed m_stateBeingDisplayed;
};

// ----------------------------

class MediaControlInputElement : public HTMLInputElement {
public:
    void attachToParent(Element*);
    void update();
    void updateStyle();

    bool hitTest(const IntPoint& absPoint);

    MediaControlElementType displayType() const { return m_displayType; }

    HTMLMediaElement* mediaElement() const { return m_mediaElement; }

protected:
    MediaControlInputElement(HTMLMediaElement*, MediaControlElementType);

    void setDisplayType(MediaControlElementType);

private:
    virtual void attach();
    virtual bool rendererIsNeeded(RenderStyle*);

    virtual PassRefPtr<RenderStyle> styleForElement();

    virtual bool isMediaControlElement() const { return true; }

    virtual void updateDisplayType() { }

    HTMLMediaElement* m_mediaElement;
    MediaControlElementType m_displayType;
};

// ----------------------------

class MediaControlMuteButtonElement : public MediaControlInputElement {
public:
    static PassRefPtr<MediaControlMuteButtonElement> create(HTMLMediaElement*);

    virtual void defaultEventHandler(Event*);

protected:
    MediaControlMuteButtonElement(HTMLMediaElement*, MediaControlElementType);

private:
    virtual void updateDisplayType();
    virtual const AtomicString& shadowPseudoId() const;
};

// ----------------------------

class MediaControlVolumeSliderMuteButtonElement : public MediaControlMuteButtonElement {
public:
    static PassRefPtr<MediaControlVolumeSliderMuteButtonElement> create(HTMLMediaElement*);

private:
    MediaControlVolumeSliderMuteButtonElement(HTMLMediaElement*);

    virtual const AtomicString& shadowPseudoId() const;
};


// ----------------------------

class MediaControlPlayButtonElement : public MediaControlInputElement {
public:
    static PassRefPtr<MediaControlPlayButtonElement> create(HTMLMediaElement*);

    virtual void defaultEventHandler(Event*);

private:
    MediaControlPlayButtonElement(HTMLMediaElement*);

    virtual void updateDisplayType();
    virtual const AtomicString& shadowPseudoId() const;
};

// ----------------------------

class MediaControlSeekButtonElement : public MediaControlInputElement {
public:
    virtual void defaultEventHandler(Event*);

protected:
    MediaControlSeekButtonElement(HTMLMediaElement*, MediaControlElementType);

private:
    virtual bool isForwardButton() const = 0;

    virtual void detach();
    void seekTimerFired(Timer<MediaControlSeekButtonElement>*);

    bool m_seeking;
    bool m_capturing;
    Timer<MediaControlSeekButtonElement> m_seekTimer;
};

// ----------------------------

class MediaControlSeekForwardButtonElement : public MediaControlSeekButtonElement {
public:
    static PassRefPtr<MediaControlSeekForwardButtonElement> create(HTMLMediaElement*);

private:
    MediaControlSeekForwardButtonElement(HTMLMediaElement*);

    virtual bool isForwardButton() const { return true; }
    virtual const AtomicString& shadowPseudoId() const;
};

// ----------------------------

class MediaControlSeekBackButtonElement : public MediaControlSeekButtonElement {
public:
    static PassRefPtr<MediaControlSeekBackButtonElement> create(HTMLMediaElement*);

private:
    MediaControlSeekBackButtonElement(HTMLMediaElement*);

    virtual bool isForwardButton() const { return false; }
    virtual const AtomicString& shadowPseudoId() const;
};

// ----------------------------

class MediaControlRewindButtonElement : public MediaControlInputElement {
public:
    static PassRefPtr<MediaControlRewindButtonElement> create(HTMLMediaElement*);

    virtual void defaultEventHandler(Event*);

private:
    MediaControlRewindButtonElement(HTMLMediaElement*);

    virtual const AtomicString& shadowPseudoId() const;
};

// ----------------------------

class MediaControlReturnToRealtimeButtonElement : public MediaControlInputElement {
public:
    static PassRefPtr<MediaControlReturnToRealtimeButtonElement> create(HTMLMediaElement*);

    virtual void defaultEventHandler(Event*);

private:
    MediaControlReturnToRealtimeButtonElement(HTMLMediaElement*);

    virtual const AtomicString& shadowPseudoId() const;
};    

// ----------------------------

class MediaControlToggleClosedCaptionsButtonElement : public MediaControlInputElement {
public:
    static PassRefPtr<MediaControlToggleClosedCaptionsButtonElement> create(HTMLMediaElement*);

    virtual void defaultEventHandler(Event*);

private:
    MediaControlToggleClosedCaptionsButtonElement(HTMLMediaElement*);

    virtual void updateDisplayType();
    virtual const AtomicString& shadowPseudoId() const;
};    

// ----------------------------

class MediaControlTimelineElement : public MediaControlInputElement {
public:
    static PassRefPtr<MediaControlTimelineElement> create(HTMLMediaElement*);

    virtual void defaultEventHandler(Event*);
    void update(bool updateDuration = true);

private:
    MediaControlTimelineElement(HTMLMediaElement*);

    virtual const AtomicString& shadowPseudoId() const;
};

// ----------------------------

class MediaControlVolumeSliderElement : public MediaControlInputElement {
public:
    static PassRefPtr<MediaControlVolumeSliderElement> create(HTMLMediaElement*);

    virtual void defaultEventHandler(Event*);
    void update();

private:
    MediaControlVolumeSliderElement(HTMLMediaElement*);

    virtual const AtomicString& shadowPseudoId() const;
};

// ----------------------------

class MediaControlFullscreenButtonElement : public MediaControlInputElement {
public:
    static PassRefPtr<MediaControlFullscreenButtonElement> create(HTMLMediaElement*);

    virtual void defaultEventHandler(Event*);

private:
    MediaControlFullscreenButtonElement(HTMLMediaElement*);

    virtual const AtomicString& shadowPseudoId() const;
};

// ----------------------------

class MediaControlTimeDisplayElement : public MediaControlElement {
public:
    void setVisible(bool);

    void setCurrentValue(float);
    float currentValue() const { return m_currentValue; }

protected:
    MediaControlTimeDisplayElement(HTMLMediaElement*);

private:
    virtual PassRefPtr<RenderStyle> styleForElement();
    float m_currentValue;
    bool m_isVisible;
};

// ----------------------------

class MediaControlTimeRemainingDisplayElement : public MediaControlTimeDisplayElement {
public:
    static PassRefPtr<MediaControlTimeRemainingDisplayElement> create(HTMLMediaElement*);

private:
    MediaControlTimeRemainingDisplayElement(HTMLMediaElement*);

    virtual MediaControlElementType displayType() const;
    virtual const AtomicString& shadowPseudoId() const;
};

// ----------------------------

class MediaControlCurrentTimeDisplayElement : public MediaControlTimeDisplayElement {
public:
    static PassRefPtr<MediaControlCurrentTimeDisplayElement> create(HTMLMediaElement*);

private:
    MediaControlCurrentTimeDisplayElement(HTMLMediaElement*);

    virtual MediaControlElementType displayType() const;
    virtual const AtomicString& shadowPseudoId() const;
};

// ----------------------------

class RenderMediaControlShadowRoot : public RenderBlock {
public:
    RenderMediaControlShadowRoot(Element* e) : RenderBlock(e) { }
    void setParent(RenderObject* p) { RenderObject::setParent(p); }
};
    
// ----------------------------

} //namespace WebCore

#endif // ENABLE(VIDEO)

#endif // MediaControlElements_h