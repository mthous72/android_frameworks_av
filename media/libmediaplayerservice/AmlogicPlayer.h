/*
**
** Copyright 2008, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef ANDROID_AMLOGICPLAYER_H
#define ANDROID_AMLOGICPLAYER_H


#include <utils/threads.h>

#include <drm/DrmManagerClient.h>
#include <media/MediaPlayerInterface.h>
#include <media/AudioTrack.h>
//#include <ui/Overlay.h>
#include "AmlogicPlayerRender.h"

extern "C" {
#include "libavutil/avstring.h"
#include "libavformat/avformat.h"
}
#include "AmlogicPlayerStreamSource.h"
#include "AmlogicPlayerDataSouceProtocol.h"
#include <player.h>
#include <player_ctrl.h>

typedef struct AmlogicPlayer_File {
  char            *datasource; /* Pointer to a FILE *, NULL. */
  int              seekable;
  int 				oldfd;
  int 		    	fd;
  int				fd_valid;
  int64_t          mOffset;
  int64_t          mCurPos;
  int64_t          mLength;
} AmlogicPlayer_File;
#define OverlayRef ANativeWindow
namespace android {

struct ISurfaceTexture;

class AmlogicPlayer : public MediaPlayerInterface {
	
public:
	static	status_t	BasicInit();
	static	status_t	exitAllThreads();
	static  	bool    PropIsEnable(const char* str);
	static 	float 		PropGetFloat(const char* str);
	static void 		SetCpuScalingOnAudio(float mul_audio);
                        AmlogicPlayer();
                        ~AmlogicPlayer();

    virtual void        onFirstRef();
    virtual status_t    initCheck();
	
    virtual status_t    setDataSource(
            const char *uri, const KeyedVector<String8, String8> *headers);

    virtual status_t    setDataSource(int fd, int64_t offset, int64_t length);
    virtual status_t    setDataSource(const sp<IStreamSource> &source) ;
	virtual status_t    setVideoSurfaceTexture(
                                const sp<ISurfaceTexture>& surfaceTexture);
	virtual status_t    setParameter(int key, const Parcel &request);
    virtual status_t    getParameter(int key, Parcel *reply);
    virtual status_t    setVolume(float leftVolume, float rightVolume);
	
    virtual status_t    prepare();
    virtual status_t    prepareAsync();
    virtual status_t    start();
    virtual status_t    stop();
    virtual status_t    seekTo(int msec);
    virtual status_t    pause();
    virtual bool        isPlaying();
    virtual status_t    getCurrentPosition(int* msec);
    virtual status_t    getDuration(int* msec);
    virtual status_t    release();
    virtual status_t    reset();
    virtual status_t    setLooping(int loop);
    virtual player_type playerType() { return AMLOGIC_PLAYER; }
	virtual bool        hardwareOutput() {return true;};
    virtual status_t    invoke(const Parcel& request, Parcel *reply) {return INVALID_OPERATION;}
	virtual status_t    getMetadata(const media::Metadata::Filter& ids,Parcel *records);

	static int 			notifyhandle(int pid,int msg,unsigned long ext1,unsigned long ext2);
	int 				NotifyHandle(int pid,int msg,unsigned long ext1,unsigned long ext2);
	int 				UpdateProcess(int pid,player_info_t *info);
	int 				GetFileType(char **typestr,int *videos,int *audios);

private:	
            status_t    setdatasource(const char *path, int fd, int64_t offset, int64_t length, const KeyedVector<String8, String8> *headers);
            status_t    reset_nosync();
            status_t    createOutputTrack();
    static  int         renderThread(void*);
            int         render();
	
    static void         initOverlay_l();
	static void         VideoViewOn(void);
	static void         VideoViewClose(void);

	int 				getintfromString8(String8 &s,const char*pre);
	
	status_t			updateMediaInfo(void);
	status_t 			initVideoSurface(void);
	status_t 			UpdateBufLevel(hwbufstats_t *pbufinfo);

	
	//helper functions
	const char* getStrAudioCodec(int atype);
  	const char* getStrVideoCodec(int vtype);

	static int 			vp_open(URLContext *h, const char *filename, int flags);
	static int 			vp_read(URLContext *h, unsigned char *buf, int size);
	static int 			vp_write(URLContext *h, unsigned char *buf, int size);
	static int64_t 		vp_seek(URLContext *h, int64_t pos, int whence);
	static int 			vp_close(URLContext *h);
	static int 			vp_get_file_handle(URLContext *h);

	sp<Surface> 			mSurface;
	sp<ISurfaceTexture>		mSurfaceTexture;
    sp<ANativeWindow> 		mNativeWindow;
	sp<AmlogicPlayerRender>	mPlayerRender;
	
    Mutex               mMutex;
    Condition           mCondition;
    AmlogicPlayer_File  mAmlogicFile;
    int                 mPlayTime;
    int64_t		    mLastPlayTimeUpdateUS;
    int64_t		    mLastStreamTimeUpdateUS;
    int 		    LatestPlayerState;		
    int		    mStreamTime;
    int		    mStreamTimeExtAddS;/**/
    int                 mDuration;
    status_t            mState;
    int                 mStreamType;
    bool                mLoop;
    bool                mAndroidLoop;
    volatile bool       mExit;
    bool                mPaused;
    volatile bool       mRunning;
	bool				mChangedCpuFreq;
	play_control_t 		mPlay_ctl;
	int 				mPlayer_id;
	int 				mWidth;
	int 				mHeight;
	int 				video_rotation_degree;
	
	char 				mTypeStr[64];
	int					mhasVideo;
	int					mhasAudio;
	bool				mIgnoreMsg;
	bool 				mTypeReady;
	media_info_t		mStreamInfo; 
	bool				streaminfo_valied;
	int64_t 				PlayerStartTimeUS;
	
	//added extend info
	char* mStrCurrentVideoCodec;
	char* mStrCurrentAudioCodec;
	
	int 				mAudioTrackNum;
	int 				mVideoTrackNum;
	int 				mInnerSubNum;
	char* 			mAudioExtInfo; //just json string,such as "{ "id": id, "format":format,"bitrate":bitrate,...;"id":id,...}",etc
	char*			mSubExtInfo;
	char*                    mVideoExtInfo;
	bool 				mInbuffering;
	Rect 			curLayout;
	int 				mDelayUpdateTime;
	bool 				mEnded;
	bool                            mHttpWV;
	bool				isTryDRM;
	
	sp<AmlogicPlayerStreamSource> mStreamSource;
    sp <AmlogicPlayerDataSouceProtocol> mSouceProtocol;
	sp<IStreamSource> mSource;
	int fastNotifyMode;// IStreamSource& netflix need it;
	float mStopFeedingBufLevel;
	int 	mStopFeedingBuf_ms;
	bool mLowLevelBufMode;/*save less data on player.*/
	sp <DecryptHandle> mDecryptHandle;
	DrmManagerClient *mDrmManagerClient;
	int mHWaudiobufsize;
	int mHWvideobufsize;
};

}; // namespace android

#endif // ANDROID_AMLOGICPLAYER_H

