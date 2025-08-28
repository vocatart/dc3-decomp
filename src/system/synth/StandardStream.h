#pragma once
#include "synth/Pollable.h"
#include "synth/Stream.h"

class StreamReceiver;

class StandardStream : public Stream, public SynthPollable {
public:
    StandardStream(File *, float, float, Symbol, bool, bool, bool);
    // Stream
    virtual ~StandardStream() {}
    virtual bool Fail();
    virtual bool IsReady() const;
    virtual bool IsFinished() const;
    virtual int GetNumChannels() const;
    virtual int GetNumChanParams() const;
    virtual void Play();
    virtual void Stop();
    virtual bool IsPlaying() const;
    virtual bool IsPaused() const;
    virtual void Resync(float);
    virtual void Fill() {}
    virtual bool FillDone() const { return true; }
    virtual void EnableReads(bool);
    virtual float GetTime();
    virtual float GetJumpBackTotalTime();
    virtual float GetInSongTime();
    virtual int GetJumpInstances();
    virtual float GetFilePos() const { return 0; }
    virtual float GetFileLength() const { return 0; }
    virtual void SetVolume(int, float);
    virtual float GetVolume(int) const;
    virtual void SetPan(int, float);
    virtual float GetPan(int) const;
    virtual void SetFX(int, bool) {}
    virtual bool GetFX(int) const { return false; }
    virtual void SetFXCore(int, FXCore) {}
    virtual FXCore GetFXCore(int) const { return kFXCoreNone; }
    virtual void SetFXSend(int, FxSend *);
    virtual void SetADSR(int, const ADSR &) {}
    virtual void SetSpeed(float);
    virtual float GetSpeed() const;
    virtual void LoadMarkerList(const char *);
    virtual void ClearMarkerList() {}
    virtual void AddMarker(const Marker &);
    virtual int MarkerListSize() const;
    virtual bool MarkerAt(int, Marker &) const;
    virtual void SetJump(float, float, const char *);
    virtual void SetJump(String &, String &);
    virtual bool CurrentJumpPoints(Marker &, Marker &);
    virtual void ClearJump();
    virtual void EnableSlipStreaming(int);
    virtual void SetSlipOffset(int, float);
    virtual void SlipStop(int);
    virtual float GetSlipOffset(int);
    virtual void SetSlipSpeed(int, float);
    virtual void SetStereoPair(int, int) {}
    virtual FaderGroup *ChannelFaders(int);
    virtual void AddVirtualChannels(int);
    virtual void RemapChannel(int, int);
    virtual StreamReceiver *GetChannel(int) const;
    virtual void UpdateTime();
    virtual void UpdateTimeByFiltering();
    virtual float GetRawTime();
    virtual void SetADSR(int, const ADSRImpl &);
    virtual void SetJumpSamples(int, int, const char *);
    virtual int GetSampleRate();
    // SynthPollable
    virtual const char *GetSoundDisplayName();
    virtual void SynthPoll();
};
