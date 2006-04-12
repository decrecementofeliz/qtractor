// qtractorAudioBuffer.h
//
/****************************************************************************
   Copyright (C) 2005-2006, rncbc aka Rui Nuno Capela. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*****************************************************************************/

#ifndef __qtractorAudioBuffer_h
#define __qtractorAudioBuffer_h

#include "qtractorList.h"
#include "qtractorAudioFile.h"
#include "qtractorRingBuffer.h"

#ifdef CONFIG_LIBSAMPLERATE
// libsamplerate API
#include <samplerate.h>
#endif

#include <qthread.h>


//----------------------------------------------------------------------
// class qtractorAudioBuffer -- Ring buffer/cache template declaration.
//

class qtractorAudioBuffer : public qtractorList<qtractorAudioBuffer>::Link
{
public:

	// Constructors.
	qtractorAudioBuffer(unsigned short iChannels, unsigned int iSampleRate);
	// Default destructor.
	~qtractorAudioBuffer();

	// Internal file descriptor accessors.
	qtractorAudioFile* file() const;

	// File implementation properties.
	unsigned short channels() const;
	unsigned long frames() const;
	unsigned int sampleRate() const;

 	// Resample ratio accessor.
	float resampleRatio() const;

	// Operational initializer/terminator.
	bool open(const QString& sFilename, int iMode = qtractorAudioFile::Read);
	void close();

	// Buffer data read/write.
	int read(float **ppFrames, unsigned int iFrames, unsigned int iOffset = 0);
	int write(float **ppFrames, unsigned int iFrames);

	// Special kind of super-read/channel-mix.
	int readMix(float **ppFrames, unsigned int iFrames,
		unsigned short iChannels, unsigned int iOffset = 0, float fGain = 1.0f);

	// Buffer data seek.
	bool seek(unsigned long iOffset);

	// Reset this buffer's state.
	void reset(bool bLooping);

	// Physical (next read-ahead/write-behind) offset accessors.
	void setOffset(unsigned long iOffset);
	unsigned long offset() const;

	// Current known length (in frames).
	unsigned long length() const;

	// Whether concrete file fits completely in buffer.
	bool integral() const;

	// Whether file read has exausted.
	bool eof() const;

	// Loop points accessors.
	void setLoop(unsigned long iLoopStart, unsigned long iLoopEnd);
	unsigned long loopStart() const;
	unsigned long loopEnd() const;

	// Base sync method.
	void sync();
	
#ifdef DEBUG
	void dump_state(const char *pszPrefix) const;
#endif

protected:

	// Sync mode methods.
	void readSync();
	void writeSync();

	// Buffer process methods.
	int readBuffer  (unsigned int iFrames);
	int writeBuffer (unsigned int iFrames);

	// Special kind of super-read/channel-mix buffer helper.
	int readMixBuffer(float **ppFrames, unsigned int iFrames,
		unsigned short iChannels, unsigned int iOffset, float fGain);

	// I/O buffer release.
	void deleteIOBuffers();

	// Frame position converters.
	unsigned long framesIn(unsigned long iFrames) const;
	unsigned long framesOut(unsigned long iFrames) const;

private:

	// Audio file instance variables.
	unsigned short m_iChannels;
	unsigned int   m_iSampleRate;

	qtractorAudioFile *m_pFile;

	qtractorRingBuffer<float> *m_pRingBuffer;

	unsigned int   m_iThreshold;
	unsigned long  m_iOffset;
	unsigned long  m_iLength;
	bool           m_bIntegral;
	bool           m_bEndOfFile;

	unsigned long  m_iLoopStart;
	unsigned long  m_iLoopEnd;

	unsigned int   m_iSeekPending;
	unsigned long  m_iSeekOffset;

	bool           m_bResample;
	float          m_fResampleRatio;
	unsigned int   m_iInputPending;
	float        **m_ppFrames;
	float        **m_ppInBuffer;
#ifdef CONFIG_LIBSAMPLERATE
	float        **m_ppOutBuffer;
	SRC_STATE    **m_ppSrcState;
#endif  // CONFIG_LIBSAMPLERATE
};


#endif  // __qtractorAudioBuffer_h


// end of qtractorAudioBuffer.h
