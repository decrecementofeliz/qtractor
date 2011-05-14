// qtractorInsertPlugin.h
//
/****************************************************************************
   Copyright (C) 2005-2011, rncbc aka Rui Nuno Capela. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/

#ifndef __qtractorInsertPlugin_h
#define __qtractorInsertPlugin_h

#include "qtractorPlugin.h"


// Forward declarations.
class qtractorAudioBus;
class qtractorInsertPluginParam;


//----------------------------------------------------------------------------
// qtractorInsertPluginType -- Insert pseudo-plugin type instance.
//

class qtractorInsertPluginType : public qtractorPluginType
{
public:

	// Constructor.
	qtractorInsertPluginType(unsigned short iChannels)
		: qtractorPluginType(NULL, iChannels, qtractorPluginType::Insert) {}

	// Destructor.
	~qtractorInsertPluginType()
		{ close(); }

	// Derived methods.
	bool open();
	void close();

	// Compute the number of instances needed
	// for the given input/output audio channels.
	unsigned short instances(
		unsigned short iChannels, bool /*bMidi*/) const
		{ return (iChannels == m_iAudioOuts ? 1 : 0); }

	// Factory method (static)
	static qtractorInsertPluginType *createType(unsigned short iChannels);

	// Specific named accessors.
	unsigned short channels() const
		{ return index(); }
};


//----------------------------------------------------------------------------
// qtractorInsertPlugin -- Insert pseudo-plugin instance.
//

class qtractorInsertPlugin : public qtractorPlugin
{
public:

	// Constructors.
	qtractorInsertPlugin(qtractorPluginList *pList,
		qtractorInsertPluginType *pInsertType);

	// Destructor.
	~qtractorInsertPlugin();

	// Channel/intsance number accessors.
	void setChannels(unsigned short iChannels);

	// Do the actual (de)activation.
	void activate();
	void deactivate();

	// The main plugin processing procedure.
	void process(float **ppIBuffer, float **ppOBuffer, unsigned int nframes);

	// Plugin configuration handlers.
	void configure(const QString& sKey, const QString& sValue);

	// Plugin configuration/state snapshot.
	void freezeConfigs();
	void releaseConfigs();

	// Audio specific accessor.
	qtractorAudioBus *audioBus() const;

protected:

	// Plugin configuration (connections).
	void freezeConfigs(int iBusMode);

private:

	// Instance variables.
	qtractorAudioBus *m_pAudioBus;

	qtractorInsertPluginParam *m_pSendGainParam;
	qtractorInsertPluginParam *m_pDryWetParam;

	// Custom optimized processors.
	void (*m_pfnProcessSendGain)(float **, unsigned int,
		unsigned short, float);
	void (*m_pfnProcessDryWet)(float **, float **, unsigned int,
		unsigned short, float);
};


//----------------------------------------------------------------------------
// qtractorInsertPluginParam -- Insert plugin control input port instance.
//

class qtractorInsertPluginParam : public qtractorPluginParam
{
public:

	// Constructors.
	qtractorInsertPluginParam(qtractorInsertPlugin *pInsertPlugin,
		unsigned long iIndex) : qtractorPluginParam(pInsertPlugin, iIndex) {}

	// Port range hints predicate methods.
	bool isBoundedBelow() const { return true; }
	bool isBoundedAbove() const { return true; }
	bool isDefaultValue() const { return true; }
	bool isLogarithmic() const { return true; }
	bool isSampleRate() const { return false; }
	bool isInteger() const { return false; }
	bool isToggled() const { return false; }
	bool isDisplay() const { return false; }
};


#endif  // __qtractorInsertPlugin_h

// end of qtractorInsertPlugin.h
