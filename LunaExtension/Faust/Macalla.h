/* ------------------------------------------------------------
name: "Macalla "
Code generated with Faust 2.38.8 (https://faust.grame.fr)
Compilation options: -a faustMinimal.h -lang cpp -mapp -es 1 -vec -lv 0 -vs 32 -single -ftz 0 -mcd 16
------------------------------------------------------------ */

#ifndef  __Macalla_H__
#define  __Macalla_H__

#include <cmath>
#include <cstring>

/************************** BEGIN MapUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>
#include <stdio.h>

/************************** BEGIN UI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* zone, const char* key, const char* val) {}
    
    // To be used by LLVM client
    virtual int sizeOfFAUSTFLOAT() { return sizeof(FAUSTFLOAT); }
};

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN PathBuilder.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_PATHBUILDER_H
#define FAUST_PATHBUILDER_H

#include <vector>
#include <string>
#include <algorithm>

/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class PathBuilder
{

    protected:
    
        std::vector<std::string> fControlsLevel;
       
    public:
    
        PathBuilder() {}
        virtual ~PathBuilder() {}
    
        std::string replaceCharList(std::string str, const std::vector<char>& ch1, char ch2)
        {
            std::vector<char>::const_iterator beg = ch1.begin();
            std::vector<char>::const_iterator end = ch1.end();
            for (size_t i = 0; i < str.length(); ++i) {
                if (std::find(beg, end, str[i]) != end) {
                    str[i] = ch2;
                }
            }
            return str;
        }
    
        std::string buildPath(const std::string& label) 
        {
            std::string res = "/";
            for (size_t i = 0; i < fControlsLevel.size(); i++) {
                res += fControlsLevel[i];
                res += "/";
            }
            res += label;
            std::vector<char> rep = {' ', '#', '*', ',', '/', '?', '[', ']', '{', '}', '(', ')'};
            replaceCharList(res, rep, '_');
            return res;
        }
    
        void pushLabel(const std::string& label) { fControlsLevel.push_back(label); }
        void popLabel() { fControlsLevel.pop_back(); }
    
};

#endif  // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface.
 *
 * This class creates:
 * - a map of 'labels' and zones for each UI item.
 * - a map of complete hierarchical 'paths' and zones for each UI item.
 *
 * Simple 'labels' and complete 'paths' (to fully discriminate between possible same
 * 'labels' at different location in the UI hierachy) can be used to access a given parameter.
 ******************************************************************************/

class MapUI : public UI, public PathBuilder
{
    
    protected:
    
        // Complete path map
        std::map<std::string, FAUSTFLOAT*> fPathZoneMap;
    
        // Label zone map
        std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;
    
    public:
        
        MapUI() {}
        virtual ~MapUI() {}
        
        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            popLabel();
        }
        
        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        
        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
        
        // -- metadata declarations
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {}
        
        // setParamValue/getParamValue
        void setParamValue(const std::string& path, FAUSTFLOAT value)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                *fPathZoneMap[path] = value;
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                *fLabelZoneMap[path] = value;
            } else {
                fprintf(stderr, "ERROR : setParamValue '%s' not found\n", path.c_str());
            }
        }
        
        FAUSTFLOAT getParamValue(const std::string& path)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                return *fPathZoneMap[path];
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                return *fLabelZoneMap[path];
            } else {
                fprintf(stderr, "ERROR : getParamValue '%s' not found\n", path.c_str());
                return 0;
            }
        }
    
        // map access 
        std::map<std::string, FAUSTFLOAT*>& getMap() { return fPathZoneMap; }
        
        int getParamsCount() { return int(fPathZoneMap.size()); }
        
        std::string getParamAddress(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return "";
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first;
            }
        }
        
        const char* getParamAddress1(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first.c_str();
            }
        }
    
        std::string getParamAddress(FAUSTFLOAT* zone)
        {
            for (const auto& it : fPathZoneMap) {
                if (it.second == zone) return it.first;
            }
            return "";
        }
    
        FAUSTFLOAT* getParamZone(const std::string& str)
        {
            if (fPathZoneMap.find(str) != fPathZoneMap.end()) {
                return fPathZoneMap[str];
            } else if (fLabelZoneMap.find(str) != fLabelZoneMap.end()) {
                return fLabelZoneMap[str];
            }
            return nullptr;
        }
    
        FAUSTFLOAT* getParamZone(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->second;
            }
        }
    
        static bool endsWith(const std::string& str, const std::string& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }
};

#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN meta.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

/**
 The base class of Meta handler to be used in dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
 */
struct Meta
{
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Return the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceInit(int sample_rate) = 0;
    
        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (like delay lines...) but keep the control parameter values */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = nullptr):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
        virtual std::string getCompileOptions() = 0;
        virtual std::vector<std::string> getLibraryList() = 0;
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        virtual dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

// Denormal handling

#if defined (__SSE__)
#include <xmmintrin.h>
#endif

class ScopedNoDenormals
{
    private:
    
        intptr_t fpsr;
        
        void setFpStatusRegister(intptr_t fpsr_aux) noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
           asm volatile("msr fpcr, %0" : : "ri" (fpsr_aux));
        #elif defined (__SSE__)
            _mm_setcsr(static_cast<uint32_t>(fpsr_aux));
        #endif
        }
        
        void getFpStatusRegister() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("mrs %0, fpcr" : "=r" (fpsr));
        #elif defined ( __SSE__)
            fpsr = static_cast<intptr_t>(_mm_getcsr());
        #endif
        }
    
    public:
    
        ScopedNoDenormals() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            intptr_t mask = (1 << 24 /* FZ */);
        #else
            #if defined(__SSE__)
            #if defined(__SSE2__)
                intptr_t mask = 0x8040;
            #else
                intptr_t mask = 0x8000;
            #endif
            #else
                intptr_t mask = 0x0000;
            #endif
        #endif
            getFpStatusRegister();
            setFpStatusRegister(fpsr | mask);
        }
        
        ~ScopedNoDenormals() noexcept
        {
            setFpStatusRegister(fpsr);
        }

};

#define AVOIDDENORMALS ScopedNoDenormals();

#endif

/************************** END dsp.h **************************/

// BEGIN-FAUSTDSP


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

class MacallaSIG0 {
	
  private:
	
	int iVec1[2];
	int iRec21[2];
	
  public:
	
	int getNumInputsMacallaSIG0() {
		return 0;
	}
	int getNumOutputsMacallaSIG0() {
		return 1;
	}
	
	void instanceInitMacallaSIG0(int sample_rate) {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			iVec1[l12] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			iRec21[l13] = 0;
		}
	}
	
	void fillMacallaSIG0(int count, float* table) {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i1 = 0; (i1 < count); i1 = (i1 + 1)) {
			iVec1[0] = 1;
			iRec21[0] = ((iVec1[1] + iRec21[1]) % 65536);
			table[i1] = std::sin((9.58738019e-05f * float(iRec21[0])));
			iVec1[1] = iVec1[0];
			iRec21[1] = iRec21[0];
		}
	}

};

static MacallaSIG0* newMacallaSIG0() { return (MacallaSIG0*)new MacallaSIG0(); }
static void deleteMacallaSIG0(MacallaSIG0* dsp) { delete dsp; }

static float ftbl0MacallaSIG0[65536];
static float Macalla_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS Macalla
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

class Macalla : public dsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fConst2;
	float fRec1_perm[4];
	FAUSTFLOAT fHslider1;
	float fRec2_perm[4];
	FAUSTFLOAT fHslider2;
	float fRec3_perm[4];
	int iVec0_perm[4];
	float fConst3;
	float fRec6_perm[4];
	float fConst4;
	FAUSTFLOAT fHslider3;
	float fRec14_perm[4];
	int iRec16_perm[4];
	float fConst5;
	float fRec17_perm[4];
	float fYec0_perm[4];
	float fRec15_perm[4];
	float fRec19_perm[4];
	float fConst6;
	float fRec20_perm[4];
	float fRec18_perm[4];
	float fConst9;
	float fConst10;
	float fConst11;
	float fRec13_perm[4];
	int iRec23_perm[4];
	int iConst12;
	float fRec22_perm[4];
	FAUSTFLOAT fCheckbox0;
	float fConst13;
	float fConst14;
	float fRec25_perm[4];
	float fConst15;
	float fConst16;
	float fRec24_perm[4];
	float fConst17;
	float fConst18;
	float fConst19;
	float fRec26_perm[4];
	FAUSTFLOAT fHslider4;
	float fRec27_perm[4];
	float fRec29_perm[4];
	float fRec30_perm[4];
	float fYec1_perm[4];
	float fRec31_perm[4];
	float fConst20;
	float fRec33_perm[4];
	float fConst21;
	float fYec2_perm[4];
	float fYec3[2048];
	int fYec3_idx;
	int fYec3_idx_save;
	int iConst23;
	float fConst26;
	float fConst27;
	int iConst28;
	float fRec32_perm[4];
	float fConst29;
	float fRec34_perm[4];
	float fConst30;
	float fRec35_perm[4];
	float fConst31;
	float fConst39;
	float fConst40;
	float fConst41;
	float fRec28_perm[4];
	FAUSTFLOAT fHslider5;
	float fRec36_perm[4];
	float fConst42;
	float fConst43;
	float fConst44;
	float fRec12_perm[4];
	FAUSTFLOAT fHslider6;
	float fRec37_perm[4];
	float fRec11_perm[4];
	float fConst45;
	FAUSTFLOAT fHslider7;
	float fRec38_perm[4];
	float fConst46;
	float fRec39_perm[4];
	float fYec4[2048];
	int fYec4_idx;
	int fYec4_idx_save;
	int iConst48;
	float fConst49;
	float fConst50;
	float fRec9_perm[4];
	float fYec5[4096];
	int fYec5_idx;
	int fYec5_idx_save;
	float fRec7_perm[4];
	float fYec6[4096];
	int fYec6_idx;
	int fYec6_idx_save;
	float fRec4_perm[4];
	FAUSTFLOAT fHslider8;
	float fRec40_perm[4];
	FAUSTFLOAT fHslider9;
	float fRec45_perm[4];
	float fRec46_perm[4];
	FAUSTFLOAT fHslider10;
	float fRec41_perm[4];
	float fRec42_perm[4];
	float fRec43_perm[4];
	float fRec44_perm[4];
	float fYec7[131072];
	int fYec7_idx;
	int fYec7_idx_save;
	float fYec8[1048576];
	int fYec8_idx;
	int fYec8_idx_save;
	float fRec0[262144];
	int fRec0_idx;
	int fRec0_idx_save;
	FAUSTFLOAT fHslider11;
	float fRec47_perm[4];
	float fConst51;
	float fConst52;
	float fRec51_perm[4];
	float fRec52_perm[4];
	float fConst54;
	float fConst55;
	float fConst56;
	float fRec50_perm[4];
	float fConst57;
	float fRec49_perm[4];
	float fRec48_perm[4];
	float fRec54_perm[4];
	float fRec53_perm[4];
	FAUSTFLOAT fHslider12;
	FAUSTFLOAT fHslider13;
	float fRec55_perm[4];
	FAUSTFLOAT fHslider14;
	float fRec56_perm[4];
	FAUSTFLOAT fHslider15;
	FAUSTFLOAT fHslider16;
	float fRec60_perm[4];
	float fRec61_perm[4];
	float fRec70_perm[4];
	float fRec69_perm[4];
	float fRec72_perm[4];
	float fRec71_perm[4];
	float fRec68_perm[4];
	float fRec73_perm[4];
	float fRec75_perm[4];
	float fRec74_perm[4];
	float fRec76_perm[4];
	float fRec67_perm[4];
	float fRec66_perm[4];
	float fRec77_perm[4];
	float fYec9[16384];
	int fYec9_idx;
	int fYec9_idx_save;
	float fRec64_perm[4];
	float fYec10[16384];
	int fYec10_idx;
	int fYec10_idx_save;
	float fRec62_perm[4];
	float fYec11[16384];
	int fYec11_idx;
	int fYec11_idx_save;
	float fRec58_perm[4];
	float fRec82_perm[4];
	float fRec78_perm[4];
	float fRec79_perm[4];
	float fRec80_perm[4];
	float fRec81_perm[4];
	float fYec12[131072];
	int fYec12_idx;
	int fYec12_idx_save;
	float fYec13[1048576];
	int fYec13_idx;
	int fYec13_idx_save;
	float fRec57[262144];
	int fRec57_idx;
	int fRec57_idx_save;
	float fRec85_perm[4];
	float fRec84_perm[4];
	float fRec83_perm[4];
	float fRec87_perm[4];
	float fRec86_perm[4];
	float fRec88_perm[4];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/amp_follower_ar:author", "Jonatan Liljedahl, revised by Romain Michon");
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.2.0");
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/downSample:author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.18.0");
		m->declare("category", "Echo / Delay");
		m->declare("compile_options", "-a faustMinimal.h -lang cpp -mapp -es 1 -vec -lv 0 -vs 32 -single -ftz 0 -mcd 16");
		m->declare("compressors.lib/compression_gain_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compression_gain_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compression_gain_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/compressor_lad_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_lad_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_lad_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/compressor_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/limiter_1176_R4_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/limiter_1176_R4_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/limiter_1176_R4_mono:license", "MIT-style STK-4.3 license");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/peak_compression_gain_mono_db:author", "Bart Brouns");
		m->declare("compressors.lib/peak_compression_gain_mono_db:license", "GPLv3");
		m->declare("compressors.lib/ratio2strength:author", "Bart Brouns");
		m->declare("compressors.lib/ratio2strength:license", "GPLv3");
		m->declare("compressors.lib/version", "1.6.0");
		m->declare("delays.lib/fdelay1:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.1.0");
		m->declare("filename", "Macalla.dsp");
		m->declare("filters.lib/allpass_fcomb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_fcomb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_fcomb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/bandpass0_bandstop1:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass0_bandstop1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass0_bandstop1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/bandpass:author", "Julius O. Smith III");
		m->declare("filters.lib/bandpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/bandpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/nlf2:author", "Julius O. Smith III");
		m->declare("filters.lib/nlf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/nlf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1sb:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1sb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1sb:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.3.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.0");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.5.0");
		m->declare("name", "Macalla ");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.4.1");
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/lf_triangle:author", "Bart Brouns");
		m->declare("oscillators.lib/lf_triangle:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/saw1:author", "Bart Brouns");
		m->declare("oscillators.lib/saw1:licence", "STK-4.3");
		m->declare("oscillators.lib/sawN:author", "Julius O. Smith III");
		m->declare("oscillators.lib/sawN:license", "STK-4.3");
		m->declare("oscillators.lib/version", "1.5.1");
		m->declare("phaflangers.lib/name", "Faust Phaser and Flanger Library");
		m->declare("phaflangers.lib/version", "1.1.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/onePoleSwitching:author", "Jonatan Liljedahl, revised by Dario Sanfilippo");
		m->declare("signals.lib/onePoleSwitching:licence", "STK-4.3");
		m->declare("signals.lib/version", "1.5.0");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
		MacallaSIG0* sig0 = newMacallaSIG0();
		sig0->instanceInitMacallaSIG0(sample_rate);
		sig0->fillMacallaSIG0(65536, ftbl0MacallaSIG0);
		deleteMacallaSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = (44.0999985f / fConst0);
		fConst2 = (1.0f - fConst1);
		fConst3 = (0.230000004f / fConst0);
		fConst4 = (30.8700008f / fConst0);
		fConst5 = (1.0f / fConst0);
		fConst6 = (0.100000001f / fConst0);
		float fConst7 = std::tan((25132.7422f / fConst0));
		float fConst8 = (1.0f / fConst7);
		fConst9 = (1.0f / (((fConst8 + 1.41421354f) / fConst7) + 1.0f));
		fConst10 = (((fConst8 + -1.41421354f) / fConst7) + 1.0f);
		fConst11 = (2.0f * (1.0f - (1.0f / Macalla_faustpower2_f(fConst7))));
		iConst12 = int((8.33333324e-05f * fConst0));
		fConst13 = std::exp((0.0f - (2.0f / fConst0)));
		fConst14 = std::exp((0.0f - (1250.0f / fConst0)));
		fConst15 = std::exp((0.0f - (2500.0f / fConst0)));
		fConst16 = (1.0f - fConst15);
		fConst17 = (20.0f / fConst0);
		fConst18 = (0.0f - fConst17);
		fConst19 = std::exp(fConst18);
		fConst20 = (60.0f / fConst0);
		fConst21 = (0.00416666688f * fConst0);
		float fConst22 = std::max<float>(0.0f, std::min<float>(2047.0f, (0.00833333377f * fConst0)));
		iConst23 = int(fConst22);
		float fConst24 = float(iConst23);
		float fConst25 = ((fConst22 == fConst24) ? fConst22 : ((fConst22 >= 0.0f) ? fConst24 : (fConst24 + -1.0f)));
		fConst26 = ((fConst25 + 1.0f) - fConst22);
		fConst27 = (fConst22 - fConst25);
		iConst28 = (iConst23 + 1);
		fConst29 = (528.0f / fConst0);
		fConst30 = (110.0f / fConst0);
		fConst31 = (48.0f / fConst0);
		float fConst32 = std::tan((31415.9258f / fConst0));
		float fConst33 = Macalla_faustpower2_f(std::sqrt((4.0f * ((Macalla_faustpower2_f(fConst0) * fConst32) * std::tan((1570.79639f / fConst0))))));
		float fConst34 = (Macalla_faustpower2_f(fConst5) * fConst33);
		float fConst35 = (fConst0 * fConst32);
		float fConst36 = ((2.0f * fConst35) - (0.5f * (fConst33 / fConst35)));
		float fConst37 = (2.0f * (fConst36 / fConst0));
		float fConst38 = ((fConst34 + fConst37) + 4.0f);
		fConst39 = (1.0f / fConst38);
		fConst40 = ((2.0f * fConst34) + -8.0f);
		fConst41 = (fConst34 + (4.0f - fConst37));
		fConst42 = (2.0f * (fConst36 / (fConst0 * fConst38)));
		fConst43 = (0.0f - fConst42);
		fConst44 = (3.14159274f / fConst0);
		fConst45 = (41.8950005f / fConst0);
		fConst46 = (0.150000006f / fConst0);
		int iConst47 = int((0.0500000007f * fConst0));
		iConst48 = (iConst47 + 2);
		fConst49 = float(iConst47);
		fConst50 = (0.00100000005f * fConst0);
		fConst51 = std::sin(fConst44);
		fConst52 = std::cos(fConst44);
		float fConst53 = std::exp((0.0f - (3141.59277f / fConst0)));
		fConst54 = (0.0f - (2.0f * fConst53));
		fConst55 = (1.25f / fConst0);
		fConst56 = Macalla_faustpower2_f(fConst53);
		fConst57 = (1.5625f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.0f);
		fHslider1 = FAUSTFLOAT(0.0f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(0.10000000000000001f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
		fHslider4 = FAUSTFLOAT(0.10000000000000001f);
		fHslider5 = FAUSTFLOAT(250.0f);
		fHslider6 = FAUSTFLOAT(10000.0f);
		fHslider7 = FAUSTFLOAT(0.0f);
		fHslider8 = FAUSTFLOAT(0.0f);
		fHslider9 = FAUSTFLOAT(0.0f);
		fHslider10 = FAUSTFLOAT(500.0f);
		fHslider11 = FAUSTFLOAT(1.0f);
		fHslider12 = FAUSTFLOAT(200.0f);
		fHslider13 = FAUSTFLOAT(10.0f);
		fHslider14 = FAUSTFLOAT(0.0f);
		fHslider15 = FAUSTFLOAT(50.0f);
		fHslider16 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 4); l0 = (l0 + 1)) {
			fRec1_perm[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 4); l1 = (l1 + 1)) {
			fRec2_perm[l1] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 4); l2 = (l2 + 1)) {
			fRec3_perm[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 4); l3 = (l3 + 1)) {
			iVec0_perm[l3] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 4); l4 = (l4 + 1)) {
			fRec6_perm[l4] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 4); l5 = (l5 + 1)) {
			fRec14_perm[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 4); l6 = (l6 + 1)) {
			iRec16_perm[l6] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 4); l7 = (l7 + 1)) {
			fRec17_perm[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 4); l8 = (l8 + 1)) {
			fYec0_perm[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 4); l9 = (l9 + 1)) {
			fRec15_perm[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 4); l10 = (l10 + 1)) {
			fRec19_perm[l10] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 4); l11 = (l11 + 1)) {
			fRec20_perm[l11] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 4); l14 = (l14 + 1)) {
			fRec18_perm[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 4); l15 = (l15 + 1)) {
			fRec13_perm[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 4); l16 = (l16 + 1)) {
			iRec23_perm[l16] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 4); l17 = (l17 + 1)) {
			fRec22_perm[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 4); l18 = (l18 + 1)) {
			fRec25_perm[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 4); l19 = (l19 + 1)) {
			fRec24_perm[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 4); l20 = (l20 + 1)) {
			fRec26_perm[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 4); l21 = (l21 + 1)) {
			fRec27_perm[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 4); l22 = (l22 + 1)) {
			fRec29_perm[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 4); l23 = (l23 + 1)) {
			fRec30_perm[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 4); l24 = (l24 + 1)) {
			fYec1_perm[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 4); l25 = (l25 + 1)) {
			fRec31_perm[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 4); l26 = (l26 + 1)) {
			fRec33_perm[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 4); l27 = (l27 + 1)) {
			fYec2_perm[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 2048); l28 = (l28 + 1)) {
			fYec3[l28] = 0.0f;
		}
		fYec3_idx = 0;
		fYec3_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 4); l29 = (l29 + 1)) {
			fRec32_perm[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 4); l30 = (l30 + 1)) {
			fRec34_perm[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 4); l31 = (l31 + 1)) {
			fRec35_perm[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 4); l32 = (l32 + 1)) {
			fRec28_perm[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 4); l33 = (l33 + 1)) {
			fRec36_perm[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 4); l34 = (l34 + 1)) {
			fRec12_perm[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 4); l35 = (l35 + 1)) {
			fRec37_perm[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 4); l36 = (l36 + 1)) {
			fRec11_perm[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 4); l37 = (l37 + 1)) {
			fRec38_perm[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 4); l38 = (l38 + 1)) {
			fRec39_perm[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 2048); l39 = (l39 + 1)) {
			fYec4[l39] = 0.0f;
		}
		fYec4_idx = 0;
		fYec4_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 4); l40 = (l40 + 1)) {
			fRec9_perm[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 4096); l41 = (l41 + 1)) {
			fYec5[l41] = 0.0f;
		}
		fYec5_idx = 0;
		fYec5_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 4); l42 = (l42 + 1)) {
			fRec7_perm[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 4096); l43 = (l43 + 1)) {
			fYec6[l43] = 0.0f;
		}
		fYec6_idx = 0;
		fYec6_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 4); l44 = (l44 + 1)) {
			fRec4_perm[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 4); l45 = (l45 + 1)) {
			fRec40_perm[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 4); l46 = (l46 + 1)) {
			fRec45_perm[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 4); l47 = (l47 + 1)) {
			fRec46_perm[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 4); l48 = (l48 + 1)) {
			fRec41_perm[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 4); l49 = (l49 + 1)) {
			fRec42_perm[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 4); l50 = (l50 + 1)) {
			fRec43_perm[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 4); l51 = (l51 + 1)) {
			fRec44_perm[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 131072); l52 = (l52 + 1)) {
			fYec7[l52] = 0.0f;
		}
		fYec7_idx = 0;
		fYec7_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 1048576); l53 = (l53 + 1)) {
			fYec8[l53] = 0.0f;
		}
		fYec8_idx = 0;
		fYec8_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 262144); l54 = (l54 + 1)) {
			fRec0[l54] = 0.0f;
		}
		fRec0_idx = 0;
		fRec0_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 4); l55 = (l55 + 1)) {
			fRec47_perm[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 4); l56 = (l56 + 1)) {
			fRec51_perm[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 4); l57 = (l57 + 1)) {
			fRec52_perm[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 4); l58 = (l58 + 1)) {
			fRec50_perm[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 4); l59 = (l59 + 1)) {
			fRec49_perm[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 4); l60 = (l60 + 1)) {
			fRec48_perm[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 4); l61 = (l61 + 1)) {
			fRec54_perm[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 4); l62 = (l62 + 1)) {
			fRec53_perm[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 4); l63 = (l63 + 1)) {
			fRec55_perm[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 4); l64 = (l64 + 1)) {
			fRec56_perm[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 4); l65 = (l65 + 1)) {
			fRec60_perm[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 4); l66 = (l66 + 1)) {
			fRec61_perm[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 4); l67 = (l67 + 1)) {
			fRec70_perm[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 4); l68 = (l68 + 1)) {
			fRec69_perm[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 4); l69 = (l69 + 1)) {
			fRec72_perm[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 4); l70 = (l70 + 1)) {
			fRec71_perm[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 4); l71 = (l71 + 1)) {
			fRec68_perm[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 4); l72 = (l72 + 1)) {
			fRec73_perm[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 4); l73 = (l73 + 1)) {
			fRec75_perm[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 4); l74 = (l74 + 1)) {
			fRec74_perm[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 4); l75 = (l75 + 1)) {
			fRec76_perm[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 4); l76 = (l76 + 1)) {
			fRec67_perm[l76] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l77 = 0; (l77 < 4); l77 = (l77 + 1)) {
			fRec66_perm[l77] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l78 = 0; (l78 < 4); l78 = (l78 + 1)) {
			fRec77_perm[l78] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l79 = 0; (l79 < 16384); l79 = (l79 + 1)) {
			fYec9[l79] = 0.0f;
		}
		fYec9_idx = 0;
		fYec9_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l80 = 0; (l80 < 4); l80 = (l80 + 1)) {
			fRec64_perm[l80] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l81 = 0; (l81 < 16384); l81 = (l81 + 1)) {
			fYec10[l81] = 0.0f;
		}
		fYec10_idx = 0;
		fYec10_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l82 = 0; (l82 < 4); l82 = (l82 + 1)) {
			fRec62_perm[l82] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l83 = 0; (l83 < 16384); l83 = (l83 + 1)) {
			fYec11[l83] = 0.0f;
		}
		fYec11_idx = 0;
		fYec11_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l84 = 0; (l84 < 4); l84 = (l84 + 1)) {
			fRec58_perm[l84] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l85 = 0; (l85 < 4); l85 = (l85 + 1)) {
			fRec82_perm[l85] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l86 = 0; (l86 < 4); l86 = (l86 + 1)) {
			fRec78_perm[l86] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l87 = 0; (l87 < 4); l87 = (l87 + 1)) {
			fRec79_perm[l87] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l88 = 0; (l88 < 4); l88 = (l88 + 1)) {
			fRec80_perm[l88] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l89 = 0; (l89 < 4); l89 = (l89 + 1)) {
			fRec81_perm[l89] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l90 = 0; (l90 < 131072); l90 = (l90 + 1)) {
			fYec12[l90] = 0.0f;
		}
		fYec12_idx = 0;
		fYec12_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l91 = 0; (l91 < 1048576); l91 = (l91 + 1)) {
			fYec13[l91] = 0.0f;
		}
		fYec13_idx = 0;
		fYec13_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l92 = 0; (l92 < 262144); l92 = (l92 + 1)) {
			fRec57[l92] = 0.0f;
		}
		fRec57_idx = 0;
		fRec57_idx_save = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l93 = 0; (l93 < 4); l93 = (l93 + 1)) {
			fRec85_perm[l93] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l94 = 0; (l94 < 4); l94 = (l94 + 1)) {
			fRec84_perm[l94] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l95 = 0; (l95 < 4); l95 = (l95 + 1)) {
			fRec83_perm[l95] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l96 = 0; (l96 < 4); l96 = (l96 + 1)) {
			fRec87_perm[l96] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l97 = 0; (l97 < 4); l97 = (l97 + 1)) {
			fRec86_perm[l97] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l98 = 0; (l98 < 4); l98 = (l98 + 1)) {
			fRec88_perm[l98] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual Macalla* clone() {
		return new Macalla();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("Macalla ");
		ui_interface->addCheckButton("BitCrush Enable", &fCheckbox0);
		ui_interface->declare(&fHslider14, "unit", "percent");
		ui_interface->addHorizontalSlider("Ducking Amount", &fHslider14, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->declare(&fHslider9, "unit", "percent");
		ui_interface->addHorizontalSlider("Glitch", &fHslider9, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->declare(&fHslider4, "unit", "percent");
		ui_interface->addHorizontalSlider("Tape Noise", &fHslider4, FAUSTFLOAT(0.100000001f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->declare(&fHslider3, "unit", "percent");
		ui_interface->addHorizontalSlider("Wow & Flutter", &fHslider3, FAUSTFLOAT(0.100000001f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("delaytime", &fHslider10, FAUSTFLOAT(500.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(5000.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->addHorizontalSlider("diffusion", &fHslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("duck_attack", &fHslider13, FAUSTFLOAT(10.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(100.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("duck_release", &fHslider12, FAUSTFLOAT(200.0f), FAUSTFLOAT(50.0f), FAUSTFLOAT(1000.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("feedback", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("highpass", &fHslider5, FAUSTFLOAT(250.0f), FAUSTFLOAT(20.0f), FAUSTFLOAT(20000.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("lowpass", &fHslider6, FAUSTFLOAT(10000.0f), FAUSTFLOAT(20.0f), FAUSTFLOAT(20000.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("mix", &fHslider15, FAUSTFLOAT(50.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(100.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("output_gain", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(-96.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("phaserDepth", &fHslider11, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("pitch_shift", &fHslider8, FAUSTFLOAT(0.0f), FAUSTFLOAT(-12.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("pitch_shift2", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(-12.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("spread_amount", &fHslider16, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		float fSlow0 = (fConst1 * float(fHslider0));
		float fRec1_tmp[36];
		float* fRec1 = &fRec1_tmp[4];
		float fSlow1 = float(fHslider1);
		float fSlow2 = std::pow(2.0f, (0.0833333358f * fSlow1));
		float fRec2_tmp[36];
		float* fRec2 = &fRec2_tmp[4];
		float fSlow3 = (fConst1 * float(fHslider2));
		float fRec3_tmp[36];
		float* fRec3 = &fRec3_tmp[4];
		int iVec0_tmp[36];
		int* iVec0 = &iVec0_tmp[4];
		int iZec0[32];
		int iZec1[32];
		float fZec2[32];
		float fZec3[32];
		float fRec6_tmp[36];
		float* fRec6 = &fRec6_tmp[4];
		float fSlow4 = (fConst4 * float(fHslider3));
		float fRec14_tmp[36];
		float* fRec14 = &fRec14_tmp[4];
		int iRec16_tmp[36];
		int* iRec16 = &iRec16_tmp[4];
		float fZec4[32];
		float fZec5[32];
		float fZec6[32];
		float fRec17_tmp[36];
		float* fRec17 = &fRec17_tmp[4];
		float fYec0_tmp[36];
		float* fYec0 = &fYec0_tmp[4];
		float fZec7[32];
		float fRec15_tmp[36];
		float* fRec15 = &fRec15_tmp[4];
		float fZec8[32];
		float fZec9[32];
		float fZec10[32];
		float fRec19_tmp[36];
		float* fRec19 = &fRec19_tmp[4];
		float fZec11[32];
		float fZec12[32];
		float fRec20_tmp[36];
		float* fRec20 = &fRec20_tmp[4];
		float fZec13[32];
		float fRec18_tmp[36];
		float* fRec18 = &fRec18_tmp[4];
		float fZec14[32];
		float fZec15[32];
		int iZec16[32];
		float fZec17[32];
		float fZec18[32];
		float fRec13_tmp[36];
		float* fRec13 = &fRec13_tmp[4];
		int iRec23_tmp[36];
		int* iRec23 = &iRec23_tmp[4];
		int iZec19[32];
		float fZec20[32];
		float fZec21[32];
		float fZec22[32];
		float fRec22_tmp[36];
		float* fRec22 = &fRec22_tmp[4];
		int iSlow5 = int(float(fCheckbox0));
		float fZec23[32];
		float fZec24[32];
		float fZec25[32];
		float fRec25_tmp[36];
		float* fRec25 = &fRec25_tmp[4];
		float fRec24_tmp[36];
		float* fRec24 = &fRec24_tmp[4];
		float fZec26[32];
		float fZec27[32];
		float fZec28[32];
		float fRec26_tmp[36];
		float* fRec26 = &fRec26_tmp[4];
		float fSlow6 = (fConst1 * float(fHslider4));
		float fRec27_tmp[36];
		float* fRec27 = &fRec27_tmp[4];
		float fRec29_tmp[36];
		float* fRec29 = &fRec29_tmp[4];
		float fZec29[32];
		float fZec30[32];
		float fRec30_tmp[36];
		float* fRec30 = &fRec30_tmp[4];
		float fYec1_tmp[36];
		float* fYec1 = &fYec1_tmp[4];
		int iZec31[32];
		float fRec31_tmp[36];
		float* fRec31 = &fRec31_tmp[4];
		float fZec32[32];
		float fZec33[32];
		float fRec33_tmp[36];
		float* fRec33 = &fRec33_tmp[4];
		float fYec2_tmp[36];
		float* fYec2 = &fYec2_tmp[4];
		float fRec32_tmp[36];
		float* fRec32 = &fRec32_tmp[4];
		float fZec34[32];
		float fZec35[32];
		float fRec34_tmp[36];
		float* fRec34 = &fRec34_tmp[4];
		float fZec36[32];
		float fZec37[32];
		float fRec35_tmp[36];
		float* fRec35 = &fRec35_tmp[4];
		float fZec38[32];
		float fRec28_tmp[36];
		float* fRec28 = &fRec28_tmp[4];
		float fSlow7 = (fConst1 * float(fHslider5));
		float fRec36_tmp[36];
		float* fRec36 = &fRec36_tmp[4];
		float fZec39[32];
		float fZec40[32];
		float fZec41[32];
		float fZec42[32];
		float fZec43[32];
		float fZec44[32];
		float fZec45[32];
		float fZec46[32];
		float fRec12_tmp[36];
		float* fRec12 = &fRec12_tmp[4];
		float fSlow8 = (fConst1 * float(fHslider6));
		float fRec37_tmp[36];
		float* fRec37 = &fRec37_tmp[4];
		float fZec47[32];
		float fZec48[32];
		float fZec49[32];
		float fZec50[32];
		float fZec51[32];
		float fZec52[32];
		float fRec11_tmp[36];
		float* fRec11 = &fRec11_tmp[4];
		float fSlow9 = (fConst45 * float(fHslider7));
		float fRec38_tmp[36];
		float* fRec38 = &fRec38_tmp[4];
		float fZec53[32];
		float fZec54[32];
		float fRec39_tmp[36];
		float* fRec39 = &fRec39_tmp[4];
		float fZec55[32];
		float fZec56[32];
		int iZec57[32];
		float fZec58[32];
		float fZec59[32];
		float fRec9_tmp[36];
		float* fRec9 = &fRec9_tmp[4];
		float fRec10[32];
		float fZec60[32];
		float fZec61[32];
		int iZec62[32];
		int iZec63[32];
		float fZec64[32];
		float fZec65[32];
		float fZec66[32];
		float fZec67[32];
		int iZec68[32];
		float fRec7_tmp[36];
		float* fRec7 = &fRec7_tmp[4];
		float fRec8[32];
		float fRec4_tmp[36];
		float* fRec4 = &fRec4_tmp[4];
		float fRec5[32];
		float fSlow10 = float(fHslider8);
		float fSlow11 = std::pow(2.0f, (0.0833333358f * fSlow10));
		float fRec40_tmp[36];
		float* fRec40 = &fRec40_tmp[4];
		float fSlow12 = (fConst1 * float(fHslider9));
		float fRec45_tmp[36];
		float* fRec45 = &fRec45_tmp[4];
		float fZec69[32];
		float fZec70[32];
		float fZec71[32];
		float fRec46_tmp[36];
		float* fRec46 = &fRec46_tmp[4];
		float fSlow13 = float(fHslider10);
		float fSlow14 = (0.00100000005f * fSlow13);
		float fZec72[32];
		float fZec73[32];
		float fRec41_tmp[36];
		float* fRec41 = &fRec41_tmp[4];
		float fRec42_tmp[36];
		float* fRec42 = &fRec42_tmp[4];
		float fRec43_tmp[36];
		float* fRec43 = &fRec43_tmp[4];
		float fRec44_tmp[36];
		float* fRec44 = &fRec44_tmp[4];
		int iSlow15 = (fSlow1 != 0.0f);
		float fZec74[32];
		int iZec75[32];
		int iZec76[32];
		float fZec77[32];
		float fZec78[32];
		float fZec79[32];
		float fZec80[32];
		int iZec81[32];
		float fZec82[32];
		float fZec83[32];
		int iZec84[32];
		int iZec85[32];
		float fZec86[32];
		float fZec87[32];
		float fZec88[32];
		float fZec89[32];
		int iZec90[32];
		float fZec91[32];
		int iSlow16 = (fSlow10 != 0.0f);
		int iZec92[32];
		int iZec93[32];
		float fZec94[32];
		float fZec95[32];
		float fZec96[32];
		float fZec97[32];
		int iZec98[32];
		float fZec99[32];
		float fZec100[32];
		int iZec101[32];
		int iZec102[32];
		float fZec103[32];
		float fZec104[32];
		float fZec105[32];
		float fZec106[32];
		int iZec107[32];
		float fZec108[32];
		float fZec109[32];
		float fSlow17 = (fConst1 * float(fHslider11));
		float fRec47_tmp[36];
		float* fRec47 = &fRec47_tmp[4];
		float fRec51_tmp[36];
		float* fRec51 = &fRec51_tmp[4];
		float fRec52_tmp[36];
		float* fRec52 = &fRec52_tmp[4];
		float fZec110[32];
		float fZec111[32];
		float fZec112[32];
		float fRec50_tmp[36];
		float* fRec50 = &fRec50_tmp[4];
		float fZec113[32];
		float fZec114[32];
		float fRec49_tmp[36];
		float* fRec49 = &fRec49_tmp[4];
		float fRec48_tmp[36];
		float* fRec48 = &fRec48_tmp[4];
		float fZec115[32];
		float fZec116[32];
		float fZec117[32];
		float fZec118[32];
		float fRec54_tmp[36];
		float* fRec54 = &fRec54_tmp[4];
		float fRec53_tmp[36];
		float* fRec53 = &fRec53_tmp[4];
		float fZec119[32];
		float fSlow18 = (0.00100000005f * float(fHslider12));
		int iSlow19 = (std::fabs(fSlow18) < 1.1920929e-07f);
		float fThen62 = std::exp((0.0f - (fConst5 / (iSlow19 ? 1.0f : fSlow18))));
		float fSlow20 = (iSlow19 ? 0.0f : fThen62);
		float fSlow21 = (0.00100000005f * float(fHslider13));
		int iSlow22 = (std::fabs(fSlow21) < 1.1920929e-07f);
		float fThen64 = std::exp((0.0f - (fConst5 / (iSlow22 ? 1.0f : fSlow21))));
		float fSlow23 = (iSlow22 ? 0.0f : fThen64);
		float fZec120[32];
		float fRec55_tmp[36];
		float* fRec55 = &fRec55_tmp[4];
		float fSlow24 = (fConst1 * float(fHslider14));
		float fRec56_tmp[36];
		float* fRec56 = &fRec56_tmp[4];
		float fSlow25 = (0.00999999978f * float(fHslider15));
		float fSlow26 = (1.0f - fSlow25);
		float fZec121[32];
		float fZec122[32];
		float fZec123[32];
		float fZec124[32];
		int iZec125[32];
		float fZec126[32];
		float fZec127[32];
		float fSlow27 = (fConst1 * float(fHslider16));
		float fRec60_tmp[36];
		float* fRec60 = &fRec60_tmp[4];
		float fZec128[32];
		float fZec129[32];
		float fRec61_tmp[36];
		float* fRec61 = &fRec61_tmp[4];
		float fZec130[32];
		float fZec131[32];
		float fZec132[32];
		float fRec70_tmp[36];
		float* fRec70 = &fRec70_tmp[4];
		float fRec69_tmp[36];
		float* fRec69 = &fRec69_tmp[4];
		float fZec133[32];
		float fZec134[32];
		float fRec72_tmp[36];
		float* fRec72 = &fRec72_tmp[4];
		float fRec71_tmp[36];
		float* fRec71 = &fRec71_tmp[4];
		float fZec135[32];
		float fZec136[32];
		int iZec137[32];
		float fZec138[32];
		float fZec139[32];
		float fRec68_tmp[36];
		float* fRec68 = &fRec68_tmp[4];
		float fZec140[32];
		float fZec141[32];
		float fZec142[32];
		float fRec73_tmp[36];
		float* fRec73 = &fRec73_tmp[4];
		float fZec143[32];
		float fZec144[32];
		float fZec145[32];
		float fRec75_tmp[36];
		float* fRec75 = &fRec75_tmp[4];
		float fRec74_tmp[36];
		float* fRec74 = &fRec74_tmp[4];
		float fZec146[32];
		float fZec147[32];
		float fZec148[32];
		float fRec76_tmp[36];
		float* fRec76 = &fRec76_tmp[4];
		float fRec67_tmp[36];
		float* fRec67 = &fRec67_tmp[4];
		float fRec66_tmp[36];
		float* fRec66 = &fRec66_tmp[4];
		float fZec149[32];
		float fZec150[32];
		float fRec77_tmp[36];
		float* fRec77 = &fRec77_tmp[4];
		float fZec151[32];
		float fZec152[32];
		int iZec153[32];
		float fZec154[32];
		float fZec155[32];
		float fRec64_tmp[36];
		float* fRec64 = &fRec64_tmp[4];
		float fRec65[32];
		float fZec156[32];
		float fZec157[32];
		int iZec158[32];
		int iZec159[32];
		float fZec160[32];
		float fZec161[32];
		float fZec162[32];
		float fZec163[32];
		int iZec164[32];
		float fRec62_tmp[36];
		float* fRec62 = &fRec62_tmp[4];
		float fRec63[32];
		float fRec58_tmp[36];
		float* fRec58 = &fRec58_tmp[4];
		float fRec59[32];
		float fZec165[32];
		float fZec166[32];
		float fRec82_tmp[36];
		float* fRec82 = &fRec82_tmp[4];
		float fSlow28 = (fConst50 * fSlow13);
		float fZec167[32];
		float fZec168[32];
		float fRec78_tmp[36];
		float* fRec78 = &fRec78_tmp[4];
		float fRec79_tmp[36];
		float* fRec79 = &fRec79_tmp[4];
		float fRec80_tmp[36];
		float* fRec80 = &fRec80_tmp[4];
		float fRec81_tmp[36];
		float* fRec81 = &fRec81_tmp[4];
		float fZec169[32];
		float fZec170[32];
		float fRec85_tmp[36];
		float* fRec85 = &fRec85_tmp[4];
		float fZec171[32];
		float fRec84_tmp[36];
		float* fRec84 = &fRec84_tmp[4];
		float fRec83_tmp[36];
		float* fRec83 = &fRec83_tmp[4];
		float fZec172[32];
		float fZec173[32];
		float fZec174[32];
		float fRec87_tmp[36];
		float* fRec87 = &fRec87_tmp[4];
		float fRec86_tmp[36];
		float* fRec86 = &fRec86_tmp[4];
		float fZec175[32];
		float fZec176[32];
		float fRec88_tmp[36];
		float* fRec88 = &fRec88_tmp[4];
		float fZec177[32];
		int iZec178[32];
		float fZec179[32];
		int vindex = 0;
		/* Main loop */
		for (vindex = 0; (vindex <= (count - 32)); vindex = (vindex + 32)) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = 32;
			/* Vectorizable loop 0 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j6 = 0; (j6 < 4); j6 = (j6 + 1)) {
				iVec0_tmp[j6] = iVec0_perm[j6];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iVec0[i] = 1;
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j7 = 0; (j7 < 4); j7 = (j7 + 1)) {
				iVec0_perm[j7] = iVec0_tmp[(vsize + j7)];
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec0[i] = (1 - iVec0[(i - 1)]);
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec1[i] = iZec0[i];
			}
			/* Recursive loop 3 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j10 = 0; (j10 < 4); j10 = (j10 + 1)) {
				fRec14_tmp[j10] = fRec14_perm[j10];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec14[i] = (fSlow4 + (fConst2 * fRec14[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j11 = 0; (j11 < 4); j11 = (j11 + 1)) {
				fRec14_perm[j11] = fRec14_tmp[(vsize + j11)];
			}
			/* Recursive loop 4 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j12 = 0; (j12 < 4); j12 = (j12 + 1)) {
				iRec16_tmp[j12] = iRec16_perm[j12];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iRec16[i] = ((1103515245 * iRec16[(i - 1)]) + 12345);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j13 = 0; (j13 < 4); j13 = (j13 + 1)) {
				iRec16_perm[j13] = iRec16_tmp[(vsize + j13)];
			}
			/* Recursive loop 5 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j112 = 0; (j112 < 4); j112 = (j112 + 1)) {
				fRec60_tmp[j112] = fRec60_perm[j112];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec60[i] = (fSlow27 + (fConst2 * fRec60[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j113 = 0; (j113 < 4); j113 = (j113 + 1)) {
				fRec60_perm[j113] = fRec60_tmp[(vsize + j113)];
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec4[i] = (1.99000001f * fRec14[i]);
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec130[i] = (0.200000003f * fRec60[i]);
			}
			/* Vectorizable loop 8 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j16 = 0; (j16 < 4); j16 = (j16 + 1)) {
				fYec0_tmp[j16] = fYec0_perm[j16];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec0[i] = float(iRec16[i]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j17 = 0; (j17 < 4); j17 = (j17 + 1)) {
				fYec0_perm[j17] = fYec0_tmp[(vsize + j17)];
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec8[i] = (18.0f * fRec14[i]);
			}
			/* Recursive loop 10 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j22 = 0; (j22 < 4); j22 = (j22 + 1)) {
				fRec20_tmp[j22] = fRec20_perm[j22];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen9 = (fConst6 + fRec20[(i - 1)]);
				fZec11[i] = (iZec1[i] ? 0.0f : fThen9);
				fZec12[i] = float(int(fZec11[i]));
				float fThen10 = (fZec12[i] + -1.0f);
				float fElse10 = fZec12[i];
				float fThen11 = ((fZec11[i] >= 0.0f) ? fElse10 : fThen10);
				float fElse11 = fZec11[i];
				fRec20[i] = (fZec11[i] - ((fZec11[i] == fZec12[i]) ? fElse11 : fThen11));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j23 = 0; (j23 < 4); j23 = (j23 + 1)) {
				fRec20_perm[j23] = fRec20_tmp[(vsize + j23)];
			}
			/* Recursive loop 11 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j8 = 0; (j8 < 4); j8 = (j8 + 1)) {
				fRec6_tmp[j8] = fRec6_perm[j8];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen0 = (fConst3 + fRec6[(i - 1)]);
				fZec2[i] = (iZec1[i] ? 0.0f : fThen0);
				fZec3[i] = float(int(fZec2[i]));
				float fThen1 = (fZec3[i] + -1.0f);
				float fElse1 = fZec3[i];
				float fThen2 = ((fZec2[i] >= 0.0f) ? fElse1 : fThen1);
				float fElse2 = fZec2[i];
				fRec6[i] = (fZec2[i] - ((fZec2[i] == fZec3[i]) ? fElse2 : fThen2));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j9 = 0; (j9 < 4); j9 = (j9 + 1)) {
				fRec6_perm[j9] = fRec6_tmp[(vsize + j9)];
			}
			/* Recursive loop 12 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j14 = 0; (j14 < 4); j14 = (j14 + 1)) {
				fRec17_tmp[j14] = fRec17_perm[j14];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen3 = (fRec17[(i - 1)] + (fConst5 * (fZec4[i] + 0.00999999978f)));
				fZec5[i] = (iZec1[i] ? 0.0f : fThen3);
				fZec6[i] = float(int(fZec5[i]));
				float fThen4 = (fZec6[i] + -1.0f);
				float fElse4 = fZec6[i];
				float fThen5 = ((fZec5[i] >= 0.0f) ? fElse4 : fThen4);
				float fElse5 = fZec5[i];
				fRec17[i] = (fZec5[i] - ((fZec5[i] == fZec6[i]) ? fElse5 : fThen5));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j15 = 0; (j15 < 4); j15 = (j15 + 1)) {
				fRec17_perm[j15] = fRec17_tmp[(vsize + j15)];
			}
			/* Recursive loop 13 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j114 = 0; (j114 < 4); j114 = (j114 + 1)) {
				fRec61_tmp[j114] = fRec61_perm[j114];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen68 = (fRec61[(i - 1)] + (fConst5 * ((0.0299999993f * fRec60[i]) + 0.230000004f)));
				fZec128[i] = (iZec1[i] ? 0.0f : fThen68);
				fZec129[i] = float(int(fZec128[i]));
				float fThen69 = (fZec129[i] + -1.0f);
				float fElse69 = fZec129[i];
				float fThen70 = ((fZec128[i] >= 0.0f) ? fElse69 : fThen69);
				float fElse70 = fZec128[i];
				fRec61[i] = (fZec128[i] - ((fZec128[i] == fZec129[i]) ? fElse70 : fThen70));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j115 = 0; (j115 < 4); j115 = (j115 + 1)) {
				fRec61_perm[j115] = fRec61_tmp[(vsize + j115)];
			}
			/* Recursive loop 14 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j116 = 0; (j116 < 4); j116 = (j116 + 1)) {
				fRec70_tmp[j116] = fRec70_perm[j116];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen71 = (fRec70[(i - 1)] + (fConst5 * ((fZec4[i] + fZec130[i]) + 0.00999999978f)));
				fZec131[i] = (iZec1[i] ? 0.0f : fThen71);
				fZec132[i] = float(int(fZec131[i]));
				float fThen72 = (fZec132[i] + -1.0f);
				float fElse72 = fZec132[i];
				float fThen73 = ((fZec131[i] >= 0.0f) ? fElse72 : fThen72);
				float fElse73 = fZec131[i];
				fRec70[i] = (fZec131[i] - ((fZec131[i] == fZec132[i]) ? fElse73 : fThen73));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j117 = 0; (j117 < 4); j117 = (j117 + 1)) {
				fRec70_perm[j117] = fRec70_tmp[(vsize + j117)];
			}
			/* Recursive loop 15 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j120 = 0; (j120 < 4); j120 = (j120 + 1)) {
				fRec72_tmp[j120] = fRec72_perm[j120];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen74 = (fRec72[(i - 1)] + (fConst5 * ((fZec8[i] + fZec130[i]) + 2.0f)));
				fZec133[i] = (iZec1[i] ? 0.0f : fThen74);
				fZec134[i] = float(int(fZec133[i]));
				float fThen75 = (fZec134[i] + -1.0f);
				float fElse75 = fZec134[i];
				float fThen76 = ((fZec133[i] >= 0.0f) ? fElse75 : fThen75);
				float fElse76 = fZec133[i];
				fRec72[i] = (fZec133[i] - ((fZec133[i] == fZec134[i]) ? fElse76 : fThen76));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j121 = 0; (j121 < 4); j121 = (j121 + 1)) {
				fRec72_perm[j121] = fRec72_tmp[(vsize + j121)];
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec7[i] = (4.65661277e-12f * (fRec14[i] * fYec0[i]));
			}
			/* Recursive loop 17 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j20 = 0; (j20 < 4); j20 = (j20 + 1)) {
				fRec19_tmp[j20] = fRec19_perm[j20];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen6 = (fRec19[(i - 1)] + (fConst5 * (fZec8[i] + 2.0f)));
				fZec9[i] = (iZec1[i] ? 0.0f : fThen6);
				fZec10[i] = float(int(fZec9[i]));
				float fThen7 = (fZec10[i] + -1.0f);
				float fElse7 = fZec10[i];
				float fThen8 = ((fZec9[i] >= 0.0f) ? fElse7 : fThen7);
				float fElse8 = fZec9[i];
				fRec19[i] = (fZec9[i] - ((fZec9[i] == fZec10[i]) ? fElse8 : fThen8));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j21 = 0; (j21 < 4); j21 = (j21 + 1)) {
				fRec19_perm[j21] = fRec19_tmp[(vsize + j21)];
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec13[i] = ((0.200000003f * ftbl0MacallaSIG0[int((65536.0f * fRec20[i]))]) + 0.800000012f);
			}
			/* Recursive loop 19 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j42 = 0; (j42 < 4); j42 = (j42 + 1)) {
				fRec30_tmp[j42] = fRec30_perm[j42];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen20 = (fConst17 + fRec30[(i - 1)]);
				fZec29[i] = (iZec1[i] ? 0.0f : fThen20);
				fZec30[i] = float(int(fZec29[i]));
				float fThen21 = (fZec30[i] + -1.0f);
				float fElse21 = fZec30[i];
				float fThen22 = ((fZec29[i] >= 0.0f) ? fElse21 : fThen21);
				float fElse22 = fZec29[i];
				fRec30[i] = (fZec29[i] - ((fZec29[i] == fZec30[i]) ? fElse22 : fThen22));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j43 = 0; (j43 < 4); j43 = (j43 + 1)) {
				fRec30_perm[j43] = fRec30_tmp[(vsize + j43)];
			}
			/* Recursive loop 20 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j138 = 0; (j138 < 4); j138 = (j138 + 1)) {
				fRec77_tmp[j138] = fRec77_perm[j138];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen85 = (fRec77[(i - 1)] + (fConst5 * ((0.0199999996f * fRec60[i]) + 0.150000006f)));
				fZec149[i] = (iZec1[i] ? 0.0f : fThen85);
				fZec150[i] = float(int(fZec149[i]));
				float fThen86 = (fZec150[i] + -1.0f);
				float fElse86 = fZec150[i];
				float fThen87 = ((fZec149[i] >= 0.0f) ? fElse86 : fThen86);
				float fElse87 = fZec149[i];
				fRec77[i] = (fZec149[i] - ((fZec149[i] == fZec150[i]) ? fElse87 : fThen87));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j139 = 0; (j139 < 4); j139 = (j139 + 1)) {
				fRec77_perm[j139] = fRec77_tmp[(vsize + j139)];
			}
			/* Recursive loop 21 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; (j2 < 4); j2 = (j2 + 1)) {
				fRec2_tmp[j2] = fRec2_perm[j2];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec2[i] = ((fRec2[(i - 1)] + (4097.0f - float((4096 * int((0.000244140625f * ((fRec2[(i - 1)] + 4097.0f) - fSlow2))))))) - fSlow2);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; (j3 < 4); j3 = (j3 + 1)) {
				fRec2_perm[j3] = fRec2_tmp[(vsize + j3)];
			}
			/* Recursive loop 22 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j18 = 0; (j18 < 4); j18 = (j18 + 1)) {
				fRec15_tmp[j18] = fRec15_perm[j18];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec15[i] = ((9.99999975e-05f * (fZec7[i] + (1.0f - std::fabs(((2.0f * fRec17[i]) + -1.0f))))) + (0.999899983f * fRec15[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j19 = 0; (j19 < 4); j19 = (j19 + 1)) {
				fRec15_perm[j19] = fRec15_tmp[(vsize + j19)];
			}
			/* Recursive loop 23 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j70 = 0; (j70 < 4); j70 = (j70 + 1)) {
				fRec39_tmp[j70] = fRec39_perm[j70];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen34 = (fConst46 + fRec39[(i - 1)]);
				fZec53[i] = (iZec1[i] ? 0.0f : fThen34);
				fZec54[i] = float(int(fZec53[i]));
				float fThen35 = (fZec54[i] + -1.0f);
				float fElse35 = fZec54[i];
				float fThen36 = ((fZec53[i] >= 0.0f) ? fElse35 : fThen35);
				float fElse36 = fZec53[i];
				fRec39[i] = (fZec53[i] - ((fZec53[i] == fZec54[i]) ? fElse36 : fThen36));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j71 = 0; (j71 < 4); j71 = (j71 + 1)) {
				fRec39_perm[j71] = fRec39_tmp[(vsize + j71)];
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec60[i] = std::max<float>(0.0f, std::min<float>(fConst49, (fConst50 * ((1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec6[i]) + -1.0f)))) + -1.0f)) + 11.3000002f))));
			}
			/* Recursive loop 25 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j118 = 0; (j118 < 4); j118 = (j118 + 1)) {
				fRec69_tmp[j118] = fRec69_perm[j118];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec69[i] = ((9.99999975e-05f * (fZec7[i] + (1.0f - std::fabs(((2.0f * fRec70[i]) + -1.0f))))) + (0.999899983f * fRec69[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j119 = 0; (j119 < 4); j119 = (j119 + 1)) {
				fRec69_perm[j119] = fRec69_tmp[(vsize + j119)];
			}
			/* Recursive loop 26 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j122 = 0; (j122 < 4); j122 = (j122 + 1)) {
				fRec71_tmp[j122] = fRec71_perm[j122];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec71[i] = ((9.99999975e-05f * (fZec13[i] * (fZec7[i] + (1.0f - std::fabs(((2.0f * fRec72[i]) + -1.0f)))))) + (0.999899983f * fRec71[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j123 = 0; (j123 < 4); j123 = (j123 + 1)) {
				fRec71_perm[j123] = fRec71_tmp[(vsize + j123)];
			}
			/* Recursive loop 27 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j24 = 0; (j24 < 4); j24 = (j24 + 1)) {
				fRec18_tmp[j24] = fRec18_perm[j24];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec18[i] = ((9.99999975e-05f * ((fZec7[i] + (1.0f - std::fabs(((2.0f * fRec19[i]) + -1.0f)))) * fZec13[i])) + (0.999899983f * fRec18[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j25 = 0; (j25 < 4); j25 = (j25 + 1)) {
				fRec18_perm[j25] = fRec18_tmp[(vsize + j25)];
			}
			/* Vectorizable loop 28 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j44 = 0; (j44 < 4); j44 = (j44 + 1)) {
				fYec1_tmp[j44] = fYec1_perm[j44];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec1[i] = (fRec30[i] - fRec30[(i - 1)]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j45 = 0; (j45 < 4); j45 = (j45 + 1)) {
				fYec1_perm[j45] = fYec1_tmp[(vsize + j45)];
			}
			/* Recursive loop 29 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j48 = 0; (j48 < 4); j48 = (j48 + 1)) {
				fRec33_tmp[j48] = fRec33_perm[j48];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen23 = (fConst20 + fRec33[(i - 1)]);
				fZec32[i] = (iZec1[i] ? 0.0f : fThen23);
				fZec33[i] = float(int(fZec32[i]));
				float fThen24 = (fZec33[i] + -1.0f);
				float fElse24 = fZec33[i];
				float fThen25 = ((fZec32[i] >= 0.0f) ? fElse24 : fThen24);
				float fElse25 = fZec32[i];
				fRec33[i] = (fZec32[i] - ((fZec32[i] == fZec33[i]) ? fElse25 : fThen25));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j49 = 0; (j49 < 4); j49 = (j49 + 1)) {
				fRec33_perm[j49] = fRec33_tmp[(vsize + j49)];
			}
			/* Recursive loop 30 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j78 = 0; (j78 < 4); j78 = (j78 + 1)) {
				fRec40_tmp[j78] = fRec40_perm[j78];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec40[i] = ((fRec40[(i - 1)] + (4097.0f - float((4096 * int((0.000244140625f * ((fRec40[(i - 1)] + 4097.0f) - fSlow11))))))) - fSlow11);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j79 = 0; (j79 < 4); j79 = (j79 + 1)) {
				fRec40_perm[j79] = fRec40_tmp[(vsize + j79)];
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec156[i] = std::max<float>(0.0f, std::min<float>(fConst49, (fConst50 * (((0.699999988f * fRec60[i]) + (1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec61[i]) + -1.0f)))) + -1.0f))) + 11.3000002f))));
			}
			/* Vectorizable loop 32 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec151[i] = std::max<float>(0.0f, std::min<float>(fConst49, (fConst50 * (((0.5f * fRec60[i]) + (1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec77[i]) + -1.0f)))) + -1.0f))) + 7.0f))));
			}
			/* Vectorizable loop 33 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec157[i] = (fZec156[i] + -1.0f);
			}
			/* Vectorizable loop 34 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec55[i] = std::max<float>(0.0f, std::min<float>(fConst49, (fConst50 * ((1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec39[i]) + -1.0f)))) + -1.0f)) + 7.0f))));
			}
			/* Vectorizable loop 35 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec61[i] = (fZec60[i] + -1.0f);
			}
			/* Vectorizable loop 36 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec14[i] = (fRec14[i] * ((600.0f * fRec15[i]) + (100.0f * fRec18[i])));
			}
			/* Vectorizable loop 37 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec31[i] = ((fYec1[(i - 1)] <= 0.0f) & (fYec1[i] > 0.0f));
			}
			/* Vectorizable loop 38 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j50 = 0; (j50 < 4); j50 = (j50 + 1)) {
				fYec2_tmp[j50] = fYec2_perm[j50];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec2[i] = Macalla_faustpower2_f(((2.0f * fRec33[i]) + -1.0f));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j51 = 0; (j51 < 4); j51 = (j51 + 1)) {
				fYec2_perm[j51] = fYec2_tmp[(vsize + j51)];
			}
			/* Recursive loop 39 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j80 = 0; (j80 < 4); j80 = (j80 + 1)) {
				fRec45_tmp[j80] = fRec45_perm[j80];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec45[i] = (fSlow12 + (fConst2 * fRec45[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j81 = 0; (j81 < 4); j81 = (j81 + 1)) {
				fRec45_perm[j81] = fRec45_tmp[(vsize + j81)];
			}
			/* Vectorizable loop 40 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec83[i] = (fRec2[i] + 4096.0f);
			}
			/* Vectorizable loop 41 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec100[i] = (fRec40[i] + 4096.0f);
			}
			/* Vectorizable loop 42 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec135[i] = (fRec14[i] * ((600.0f * fRec69[i]) + (100.0f * fRec71[i])));
			}
			/* Vectorizable loop 43 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec152[i] = (fZec151[i] + -1.0f);
			}
			/* Vectorizable loop 44 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec158[i] = int(fZec157[i]);
			}
			/* Recursive loop 45 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j60 = 0; (j60 < 4); j60 = (j60 + 1)) {
				fRec36_tmp[j60] = fRec36_perm[j60];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec36[i] = (fSlow7 + (fConst2 * fRec36[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j61 = 0; (j61 < 4); j61 = (j61 + 1)) {
				fRec36_perm[j61] = fRec36_tmp[(vsize + j61)];
			}
			/* Recursive loop 46 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j64 = 0; (j64 < 4); j64 = (j64 + 1)) {
				fRec37_tmp[j64] = fRec37_perm[j64];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec37[i] = (fSlow8 + (fConst2 * fRec37[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j65 = 0; (j65 < 4); j65 = (j65 + 1)) {
				fRec37_perm[j65] = fRec37_tmp[(vsize + j65)];
			}
			/* Vectorizable loop 47 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec56[i] = (fZec55[i] + -1.0f);
			}
			/* Vectorizable loop 48 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec62[i] = int(fZec61[i]);
			}
			/* Vectorizable loop 49 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec15[i] = (fZec14[i] + 100.000008f);
			}
			/* Recursive loop 50 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j46 = 0; (j46 < 4); j46 = (j46 + 1)) {
				fRec31_tmp[j46] = fRec31_perm[j46];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec31[i] = ((fRec31[(i - 1)] * float((1 - iZec31[i]))) + (4.65661287e-10f * (fYec0[i] * float(iZec31[i]))));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j47 = 0; (j47 < 4); j47 = (j47 + 1)) {
				fRec31_perm[j47] = fRec31_tmp[(vsize + j47)];
			}
			/* Vectorizable loop 51 */
			/* Pre code */
			fYec3_idx = ((fYec3_idx + fYec3_idx_save) & 2047);
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec3[((i + fYec3_idx) & 2047)] = (float(iVec0[(i - 1)]) * (fYec2[i] - fYec2[(i - 1)]));
			}
			/* Post code */
			fYec3_idx_save = vsize;
			/* Vectorizable loop 52 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec69[i] = (4.98999977f * fRec45[i]);
			}
			/* Vectorizable loop 53 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec75[i] = int(fRec2[i]);
			}
			/* Vectorizable loop 54 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec84[i] = int(fZec83[i]);
			}
			/* Vectorizable loop 55 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec92[i] = int(fRec40[i]);
			}
			/* Vectorizable loop 56 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec101[i] = int(fZec100[i]);
			}
			/* Vectorizable loop 57 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec136[i] = (fZec135[i] + 100.000008f);
			}
			/* Vectorizable loop 58 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec153[i] = int(fZec152[i]);
			}
			/* Vectorizable loop 59 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec160[i] = float(iZec158[i]);
			}
			/* Recursive loop 60 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j0 = 0; (j0 < 4); j0 = (j0 + 1)) {
				fRec1_tmp[j0] = fRec1_perm[j0];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec1[i] = (fSlow0 + (fConst2 * fRec1[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; (j1 < 4); j1 = (j1 + 1)) {
				fRec1_perm[j1] = fRec1_tmp[(vsize + j1)];
			}
			/* Recursive loop 61 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j54 = 0; (j54 < 4); j54 = (j54 + 1)) {
				fRec34_tmp[j54] = fRec34_perm[j54];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen28 = (fConst29 + fRec34[(i - 1)]);
				fZec34[i] = (iZec1[i] ? 0.0f : fThen28);
				fZec35[i] = float(int(fZec34[i]));
				float fThen29 = (fZec35[i] + -1.0f);
				float fElse29 = fZec35[i];
				float fThen30 = ((fZec34[i] >= 0.0f) ? fElse29 : fThen29);
				float fElse30 = fZec34[i];
				fRec34[i] = (fZec34[i] - ((fZec34[i] == fZec35[i]) ? fElse30 : fThen30));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j55 = 0; (j55 < 4); j55 = (j55 + 1)) {
				fRec34_perm[j55] = fRec34_tmp[(vsize + j55)];
			}
			/* Recursive loop 62 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j56 = 0; (j56 < 4); j56 = (j56 + 1)) {
				fRec35_tmp[j56] = fRec35_perm[j56];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen31 = (fConst30 + fRec35[(i - 1)]);
				fZec36[i] = (iZec1[i] ? 0.0f : fThen31);
				fZec37[i] = float(int(fZec36[i]));
				float fThen32 = (fZec37[i] + -1.0f);
				float fElse32 = fZec37[i];
				float fThen33 = ((fZec36[i] >= 0.0f) ? fElse32 : fThen32);
				float fElse33 = fZec36[i];
				fRec35[i] = (fZec36[i] - ((fZec36[i] == fZec37[i]) ? fElse33 : fThen33));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j57 = 0; (j57 < 4); j57 = (j57 + 1)) {
				fRec35_perm[j57] = fRec35_tmp[(vsize + j57)];
			}
			/* Vectorizable loop 63 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec38[i] = (0.5f * (fRec31[i] + 1.0f));
			}
			/* Vectorizable loop 64 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec41[i] = std::tan((fConst44 * fRec36[i]));
			}
			/* Vectorizable loop 65 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec48[i] = std::tan((fConst44 * fRec37[i]));
			}
			/* Vectorizable loop 66 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec57[i] = int(fZec56[i]);
			}
			/* Vectorizable loop 67 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec64[i] = float(iZec62[i]);
			}
			/* Vectorizable loop 68 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec16[i] = int(fZec15[i]);
			}
			/* Recursive loop 69 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j40 = 0; (j40 < 4); j40 = (j40 + 1)) {
				fRec29_tmp[j40] = fRec29_perm[j40];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec29[i] = (((4.65661287e-10f * fYec0[i]) + ((2.49495602f * fRec29[(i - 1)]) + (0.522189379f * fRec29[(i - 3)]))) - (2.0172658f * fRec29[(i - 2)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j41 = 0; (j41 < 4); j41 = (j41 + 1)) {
				fRec29_perm[j41] = fRec29_tmp[(vsize + j41)];
			}
			/* Recursive loop 70 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j52 = 0; (j52 < 4); j52 = (j52 + 1)) {
				fRec32_tmp[j52] = fRec32_perm[j52];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec32[i] = ((0.999000013f * fRec32[(i - 1)]) + (fConst21 * ((fYec3[((i + fYec3_idx) & 2047)] - (fYec3[(((i + fYec3_idx) - iConst23) & 2047)] * fConst26)) - (fConst27 * fYec3[(((i + fYec3_idx) - iConst28) & 2047)]))));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j53 = 0; (j53 < 4); j53 = (j53 + 1)) {
				fRec32_perm[j53] = fRec32_tmp[(vsize + j53)];
			}
			/* Recursive loop 71 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j82 = 0; (j82 < 4); j82 = (j82 + 1)) {
				fRec46_tmp[j82] = fRec46_perm[j82];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen41 = (fRec46[(i - 1)] + (fConst5 * (fZec69[i] + 0.00999999978f)));
				fZec70[i] = (iZec1[i] ? 0.0f : fThen41);
				fZec71[i] = float(int(fZec70[i]));
				float fThen42 = (fZec71[i] + -1.0f);
				float fElse42 = fZec71[i];
				float fThen43 = ((fZec70[i] >= 0.0f) ? fElse42 : fThen42);
				float fElse43 = fZec70[i];
				fRec46[i] = (fZec70[i] - ((fZec70[i] == fZec71[i]) ? fElse43 : fThen43));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j83 = 0; (j83 < 4); j83 = (j83 + 1)) {
				fRec46_perm[j83] = fRec46_tmp[(vsize + j83)];
			}
			/* Vectorizable loop 72 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec77[i] = float(iZec75[i]);
			}
			/* Vectorizable loop 73 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec86[i] = float(iZec84[i]);
			}
			/* Vectorizable loop 74 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec94[i] = float(iZec92[i]);
			}
			/* Vectorizable loop 75 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec103[i] = float(iZec101[i]);
			}
			/* Vectorizable loop 76 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec137[i] = int(fZec136[i]);
			}
			/* Recursive loop 77 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j146 = 0; (j146 < 4); j146 = (j146 + 1)) {
				fRec82_tmp[j146] = fRec82_perm[j146];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen92 = (fRec82[(i - 1)] + (fConst5 * ((fZec69[i] + (2.0f * fRec60[i])) + 0.00999999978f)));
				fZec165[i] = (iZec1[i] ? 0.0f : fThen92);
				fZec166[i] = float(int(fZec165[i]));
				float fThen93 = (fZec166[i] + -1.0f);
				float fElse93 = fZec166[i];
				float fThen94 = ((fZec165[i] >= 0.0f) ? fElse93 : fThen93);
				float fElse94 = fZec165[i];
				fRec82[i] = (fZec165[i] - ((fZec165[i] == fZec166[i]) ? fElse94 : fThen94));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j147 = 0; (j147 < 4); j147 = (j147 + 1)) {
				fRec82_perm[j147] = fRec82_tmp[(vsize + j147)];
			}
			/* Vectorizable loop 78 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec154[i] = float(iZec153[i]);
			}
			/* Vectorizable loop 79 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen90 = (fZec160[i] + -1.0f);
				float fElse90 = fZec160[i];
				float fThen91 = ((fZec157[i] >= 0.0f) ? fElse90 : fThen90);
				float fElse91 = fZec157[i];
				fZec161[i] = ((fZec157[i] == fZec160[i]) ? fElse91 : fThen91);
			}
			/* Vectorizable loop 80 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec167[i] = std::min<float>(524288.0f, (fSlow28 + ((40.0f * fRec60[i]) + ((fRec45[i] * (fConst0 + (1000.0f * fRec60[i]))) * std::fabs(((2.0f * (1.0f - std::fabs(((2.0f * fRec82[i]) + -1.0f)))) + -1.0f))))));
			}
			/* Recursive loop 81 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j28 = 0; (j28 < 4); j28 = (j28 + 1)) {
				iRec23_tmp[j28] = iRec23_perm[j28];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iRec23[i] = (iRec23[(i - 1)] + 1);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j29 = 0; (j29 < 4); j29 = (j29 + 1)) {
				iRec23_perm[j29] = iRec23_tmp[(vsize + j29)];
			}
			/* Vectorizable loop 82 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec42[i] = (1.0f / fZec41[i]);
			}
			/* Vectorizable loop 83 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec44[i] = Macalla_faustpower2_f(fZec41[i]);
			}
			/* Vectorizable loop 84 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec49[i] = (1.0f / fZec48[i]);
			}
			/* Vectorizable loop 85 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec58[i] = float(iZec57[i]);
			}
			/* Vectorizable loop 86 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen39 = (fZec64[i] + -1.0f);
				float fElse39 = fZec64[i];
				float fThen40 = ((fZec61[i] >= 0.0f) ? fElse39 : fThen39);
				float fElse40 = fZec61[i];
				fZec65[i] = ((fZec61[i] == fZec64[i]) ? fElse40 : fThen40);
			}
			/* Vectorizable loop 87 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec17[i] = float(iZec16[i]);
			}
			/* Recursive loop 88 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j38 = 0; (j38 < 4); j38 = (j38 + 1)) {
				fRec27_tmp[j38] = fRec27_perm[j38];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec27[i] = (fSlow6 + (fConst2 * fRec27[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j39 = 0; (j39 < 4); j39 = (j39 + 1)) {
				fRec27_perm[j39] = fRec27_tmp[(vsize + j39)];
			}
			/* Recursive loop 89 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j58 = 0; (j58 < 4); j58 = (j58 + 1)) {
				fRec28_tmp[j58] = fRec28_perm[j58];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec28[i] = (((((1.20000005f * (((0.0499220341f * fRec29[i]) + (0.0506126992f * fRec29[(i - 2)])) - ((0.0959935337f * fRec29[(i - 1)]) + (0.00440878607f * fRec29[(i - 3)])))) + (2.32830644e-10f * (fYec0[(i - 1)] * float(((fRec30[i] >= fZec38[i]) * (fRec30[(i - 1)] < fZec38[i])))))) + (fConst31 * fRec32[i])) + (0.00999999978f * (ftbl0MacallaSIG0[int((65536.0f * fRec34[i]))] + ftbl0MacallaSIG0[int((65536.0f * fRec35[i]))]))) - (fConst39 * ((fConst40 * fRec28[(i - 1)]) + (fConst41 * fRec28[(i - 2)]))));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j59 = 0; (j59 < 4); j59 = (j59 + 1)) {
				fRec28_perm[j59] = fRec28_tmp[(vsize + j59)];
			}
			/* Vectorizable loop 90 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec72[i] = std::min<float>(524288.0f, (fConst0 * (fSlow14 + (fRec45[i] * std::fabs(((2.0f * (1.0f - std::fabs(((2.0f * fRec46[i]) + -1.0f)))) + -1.0f))))));
			}
			/* Vectorizable loop 91 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec74[i] = std::pow(10.0f, (0.0500000007f * fRec1[i]));
			}
			/* Vectorizable loop 92 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen50 = (fZec77[i] + -1.0f);
				float fElse50 = fZec77[i];
				float fThen51 = ((fRec2[i] >= 0.0f) ? fElse50 : fThen50);
				float fElse51 = fRec2[i];
				fZec78[i] = ((fRec2[i] == fZec77[i]) ? fElse51 : fThen51);
			}
			/* Vectorizable loop 93 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec82[i] = std::min<float>((0.001953125f * fRec2[i]), 1.0f);
			}
			/* Vectorizable loop 94 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen52 = (fZec86[i] + -1.0f);
				float fElse52 = fZec86[i];
				float fThen53 = ((fZec83[i] >= 0.0f) ? fElse52 : fThen52);
				float fElse53 = fZec83[i];
				fZec87[i] = ((fZec83[i] == fZec86[i]) ? fElse53 : fThen53);
			}
			/* Vectorizable loop 95 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen55 = (fZec94[i] + -1.0f);
				float fElse55 = fZec94[i];
				float fThen56 = ((fRec40[i] >= 0.0f) ? fElse55 : fThen55);
				float fElse56 = fRec40[i];
				fZec95[i] = ((fRec40[i] == fZec94[i]) ? fElse56 : fThen56);
			}
			/* Vectorizable loop 96 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec99[i] = std::min<float>((0.001953125f * fRec40[i]), 1.0f);
			}
			/* Vectorizable loop 97 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen57 = (fZec103[i] + -1.0f);
				float fElse57 = fZec103[i];
				float fThen58 = ((fZec100[i] >= 0.0f) ? fElse57 : fThen57);
				float fElse58 = fZec100[i];
				fZec104[i] = ((fZec100[i] == fZec103[i]) ? fElse58 : fThen58);
			}
			/* Recursive loop 98 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j94 = 0; (j94 < 4); j94 = (j94 + 1)) {
				fRec51_tmp[j94] = fRec51_perm[j94];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j96 = 0; (j96 < 4); j96 = (j96 + 1)) {
				fRec52_tmp[j96] = fRec52_perm[j96];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec51[i] = ((fConst51 * fRec52[(i - 1)]) + (fConst52 * fRec51[(i - 1)]));
				fRec52[i] = ((float(iZec0[i]) + (fConst52 * fRec52[(i - 1)])) - (fConst51 * fRec51[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j95 = 0; (j95 < 4); j95 = (j95 + 1)) {
				fRec51_perm[j95] = fRec51_tmp[(vsize + j95)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j97 = 0; (j97 < 4); j97 = (j97 + 1)) {
				fRec52_perm[j97] = fRec52_tmp[(vsize + j97)];
			}
			/* Vectorizable loop 99 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec138[i] = float(iZec137[i]);
			}
			/* Vectorizable loop 100 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen88 = (fZec154[i] + -1.0f);
				float fElse88 = fZec154[i];
				float fThen89 = ((fZec152[i] >= 0.0f) ? fElse88 : fThen88);
				float fElse89 = fZec152[i];
				fZec155[i] = ((fZec152[i] == fZec154[i]) ? fElse89 : fThen89);
			}
			/* Vectorizable loop 101 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec159[i] = std::min<int>(iConst48, std::max<int>(0, iZec158[i]));
			}
			/* Vectorizable loop 102 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec162[i] = (fZec161[i] + (2.0f - fZec156[i]));
			}
			/* Recursive loop 103 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; (j4 < 4); j4 = (j4 + 1)) {
				fRec3_tmp[j4] = fRec3_perm[j4];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec3[i] = (fSlow3 + (fConst2 * fRec3[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; (j5 < 4); j5 = (j5 + 1)) {
				fRec3_perm[j5] = fRec3_tmp[(vsize + j5)];
			}
			/* Recursive loop 104 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j148 = 0; (j148 < 4); j148 = (j148 + 1)) {
				fRec78_tmp[j148] = fRec78_perm[j148];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j150 = 0; (j150 < 4); j150 = (j150 + 1)) {
				fRec79_tmp[j150] = fRec79_perm[j150];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j152 = 0; (j152 < 4); j152 = (j152 + 1)) {
				fRec80_tmp[j152] = fRec80_perm[j152];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j154 = 0; (j154 < 4); j154 = (j154 + 1)) {
				fRec81_tmp[j154] = fRec81_perm[j154];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen96 = (((fRec79[(i - 1)] == 1.0f) & (fZec167[i] != fRec81[(i - 1)])) ? fConst18 : 0.0f);
				float fElse97 = fRec78[(i - 1)];
				float fThen98 = (((fRec79[(i - 1)] == 0.0f) & (fZec167[i] != fRec80[(i - 1)])) ? fConst17 : fThen96);
				float fElse98 = (((fRec79[(i - 1)] > 0.0f) & (fRec79[(i - 1)] < 1.0f)) ? fElse97 : 0.0f);
				fZec168[i] = ((fRec78[(i - 1)] != 0.0f) ? fElse98 : fThen98);
				fRec78[i] = fZec168[i];
				fRec79[i] = std::max<float>(0.0f, std::min<float>(1.0f, (fRec79[(i - 1)] + fZec168[i])));
				float fThen99 = fRec80[(i - 1)];
				float fElse99 = fZec167[i];
				fRec80[i] = (((fRec79[(i - 1)] >= 1.0f) & (fRec81[(i - 1)] != fZec167[i])) ? fElse99 : fThen99);
				float fThen100 = fRec81[(i - 1)];
				float fElse100 = fZec167[i];
				fRec81[i] = (((fRec79[(i - 1)] <= 0.0f) & (fRec80[(i - 1)] != fZec167[i])) ? fElse100 : fThen100);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j149 = 0; (j149 < 4); j149 = (j149 + 1)) {
				fRec78_perm[j149] = fRec78_tmp[(vsize + j149)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j151 = 0; (j151 < 4); j151 = (j151 + 1)) {
				fRec79_perm[j151] = fRec79_tmp[(vsize + j151)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j153 = 0; (j153 < 4); j153 = (j153 + 1)) {
				fRec80_perm[j153] = fRec80_tmp[(vsize + j153)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j155 = 0; (j155 < 4); j155 = (j155 + 1)) {
				fRec81_perm[j155] = fRec81_tmp[(vsize + j155)];
			}
			/* Vectorizable loop 105 */
			/* Pre code */
			fYec12_idx = ((fYec12_idx + fYec12_idx_save) & 131071);
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec12[((i + fYec12_idx) & 131071)] = (float(input1[i]) * fZec74[i]);
			}
			/* Post code */
			fYec12_idx_save = vsize;
			/* Vectorizable loop 106 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec19[i] = (((iRec23[i] + -1) % iConst12) == 0);
			}
			/* Vectorizable loop 107 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec39[i] = Macalla_faustpower2_f(fRec27[i]);
			}
			/* Vectorizable loop 108 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec40[i] = ((fConst42 * fRec28[i]) + (fConst43 * fRec28[(i - 2)]));
			}
			/* Vectorizable loop 109 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec43[i] = (((fZec42[i] + -1.41421354f) / fZec41[i]) + 1.0f);
			}
			/* Vectorizable loop 110 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec45[i] = (1.0f - (1.0f / fZec44[i]));
			}
			/* Vectorizable loop 111 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec46[i] = (((fZec42[i] + 1.41421354f) / fZec41[i]) + 1.0f);
			}
			/* Vectorizable loop 112 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec50[i] = (((fZec49[i] + -1.41421354f) / fZec48[i]) + 1.0f);
			}
			/* Vectorizable loop 113 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec47[i] = (0.0f - (2.0f / fZec44[i]));
			}
			/* Vectorizable loop 114 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec51[i] = (1.0f - (1.0f / Macalla_faustpower2_f(fZec48[i])));
			}
			/* Vectorizable loop 115 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec52[i] = (((fZec49[i] + 1.41421354f) / fZec48[i]) + 1.0f);
			}
			/* Recursive loop 116 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j68 = 0; (j68 < 4); j68 = (j68 + 1)) {
				fRec38_tmp[j68] = fRec38_perm[j68];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec38[i] = (fSlow9 + (fConst2 * fRec38[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j69 = 0; (j69 < 4); j69 = (j69 + 1)) {
				fRec38_perm[j69] = fRec38_tmp[(vsize + j69)];
			}
			/* Vectorizable loop 117 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen37 = (fZec58[i] + -1.0f);
				float fElse37 = fZec58[i];
				float fThen38 = ((fZec56[i] >= 0.0f) ? fElse37 : fThen37);
				float fElse38 = fZec56[i];
				fZec59[i] = ((fZec56[i] == fZec58[i]) ? fElse38 : fThen38);
			}
			/* Vectorizable loop 118 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec66[i] = (fZec65[i] + (2.0f - fZec60[i]));
			}
			/* Vectorizable loop 119 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec63[i] = std::min<int>(iConst48, std::max<int>(0, iZec62[i]));
			}
			/* Vectorizable loop 120 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec67[i] = (fZec60[i] + (-1.0f - fZec65[i]));
			}
			/* Vectorizable loop 121 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec68[i] = std::min<int>(iConst48, std::max<int>(0, (iZec62[i] + 1)));
			}
			/* Vectorizable loop 122 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen12 = (fZec17[i] + -1.0f);
				float fElse12 = fZec17[i];
				float fThen13 = ((fZec15[i] >= 0.0f) ? fElse12 : fThen12);
				float fElse13 = fZec15[i];
				fZec18[i] = ((fZec15[i] == fZec17[i]) ? fElse13 : fThen13);
			}
			/* Recursive loop 123 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j84 = 0; (j84 < 4); j84 = (j84 + 1)) {
				fRec41_tmp[j84] = fRec41_perm[j84];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j86 = 0; (j86 < 4); j86 = (j86 + 1)) {
				fRec42_tmp[j86] = fRec42_perm[j86];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j88 = 0; (j88 < 4); j88 = (j88 + 1)) {
				fRec43_tmp[j88] = fRec43_perm[j88];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j90 = 0; (j90 < 4); j90 = (j90 + 1)) {
				fRec44_tmp[j90] = fRec44_perm[j90];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen45 = (((fRec42[(i - 1)] == 1.0f) & (fZec72[i] != fRec44[(i - 1)])) ? fConst18 : 0.0f);
				float fElse46 = fRec41[(i - 1)];
				float fThen47 = (((fRec42[(i - 1)] == 0.0f) & (fZec72[i] != fRec43[(i - 1)])) ? fConst17 : fThen45);
				float fElse47 = (((fRec42[(i - 1)] > 0.0f) & (fRec42[(i - 1)] < 1.0f)) ? fElse46 : 0.0f);
				fZec73[i] = ((fRec41[(i - 1)] != 0.0f) ? fElse47 : fThen47);
				fRec41[i] = fZec73[i];
				fRec42[i] = std::max<float>(0.0f, std::min<float>(1.0f, (fRec42[(i - 1)] + fZec73[i])));
				float fThen48 = fRec43[(i - 1)];
				float fElse48 = fZec72[i];
				fRec43[i] = (((fRec42[(i - 1)] >= 1.0f) & (fRec44[(i - 1)] != fZec72[i])) ? fElse48 : fThen48);
				float fThen49 = fRec44[(i - 1)];
				float fElse49 = fZec72[i];
				fRec44[i] = (((fRec42[(i - 1)] <= 0.0f) & (fRec43[(i - 1)] != fZec72[i])) ? fElse49 : fThen49);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j85 = 0; (j85 < 4); j85 = (j85 + 1)) {
				fRec41_perm[j85] = fRec41_tmp[(vsize + j85)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j87 = 0; (j87 < 4); j87 = (j87 + 1)) {
				fRec42_perm[j87] = fRec42_tmp[(vsize + j87)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j89 = 0; (j89 < 4); j89 = (j89 + 1)) {
				fRec43_perm[j89] = fRec43_tmp[(vsize + j89)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j91 = 0; (j91 < 4); j91 = (j91 + 1)) {
				fRec44_perm[j91] = fRec44_tmp[(vsize + j91)];
			}
			/* Vectorizable loop 124 */
			/* Pre code */
			fYec7_idx = ((fYec7_idx + fYec7_idx_save) & 131071);
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec7[((i + fYec7_idx) & 131071)] = (float(input0[i]) * fZec74[i]);
			}
			/* Post code */
			fYec7_idx_save = vsize;
			/* Vectorizable loop 125 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec76[i] = std::min<int>(65537, std::max<int>(0, iZec75[i]));
			}
			/* Vectorizable loop 126 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec79[i] = (fZec78[i] + (1.0f - fRec2[i]));
			}
			/* Vectorizable loop 127 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec80[i] = (fRec2[i] - fZec78[i]);
			}
			/* Vectorizable loop 128 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec81[i] = std::min<int>(65537, std::max<int>(0, (iZec75[i] + 1)));
			}
			/* Vectorizable loop 129 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec85[i] = std::min<int>(65537, std::max<int>(0, iZec84[i]));
			}
			/* Vectorizable loop 130 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec88[i] = (fZec87[i] + (-4095.0f - fRec2[i]));
			}
			/* Vectorizable loop 131 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec89[i] = (fRec2[i] + (4096.0f - fZec87[i]));
			}
			/* Vectorizable loop 132 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec90[i] = std::min<int>(65537, std::max<int>(0, (iZec84[i] + 1)));
			}
			/* Vectorizable loop 133 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec91[i] = (1.0f - fZec82[i]);
			}
			/* Vectorizable loop 134 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec93[i] = std::min<int>(65537, std::max<int>(0, iZec92[i]));
			}
			/* Vectorizable loop 135 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec96[i] = (fZec95[i] + (1.0f - fRec40[i]));
			}
			/* Vectorizable loop 136 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec97[i] = (fRec40[i] - fZec95[i]);
			}
			/* Vectorizable loop 137 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec98[i] = std::min<int>(65537, std::max<int>(0, (iZec92[i] + 1)));
			}
			/* Vectorizable loop 138 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec102[i] = std::min<int>(65537, std::max<int>(0, iZec101[i]));
			}
			/* Vectorizable loop 139 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec105[i] = (fZec104[i] + (-4095.0f - fRec40[i]));
			}
			/* Vectorizable loop 140 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec106[i] = (fRec40[i] + (4096.0f - fZec104[i]));
			}
			/* Vectorizable loop 141 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec107[i] = std::min<int>(65537, std::max<int>(0, (iZec101[i] + 1)));
			}
			/* Vectorizable loop 142 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec108[i] = (1.0f - fZec99[i]);
			}
			/* Vectorizable loop 143 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec110[i] = ((2984.51294f * (1.0f - fRec51[i])) + 314.159271f);
			}
			/* Vectorizable loop 144 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen77 = (fZec138[i] + -1.0f);
				float fElse77 = fZec138[i];
				float fThen78 = ((fZec136[i] >= 0.0f) ? fElse77 : fThen77);
				float fElse78 = fZec136[i];
				fZec139[i] = ((fZec136[i] == fZec138[i]) ? fElse78 : fThen78);
			}
			/* Vectorizable loop 145 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec163[i] = (fZec156[i] + (-1.0f - fZec161[i]));
			}
			/* Vectorizable loop 146 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec164[i] = std::min<int>(iConst48, std::max<int>(0, (iZec158[i] + 1)));
			}
			/* Recursive loop 147 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j26 = 0; (j26 < 4); j26 = (j26 + 1)) {
				fRec13_tmp[j26] = fRec13_perm[j26];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j30 = 0; (j30 < 4); j30 = (j30 + 1)) {
				fRec22_tmp[j30] = fRec22_perm[j30];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j32 = 0; (j32 < 4); j32 = (j32 + 1)) {
				fRec25_tmp[j32] = fRec25_perm[j32];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j34 = 0; (j34 < 4); j34 = (j34 + 1)) {
				fRec24_tmp[j34] = fRec24_perm[j34];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j36 = 0; (j36 < 4); j36 = (j36 + 1)) {
				fRec26_tmp[j36] = fRec26_perm[j36];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j62 = 0; (j62 < 4); j62 = (j62 + 1)) {
				fRec12_tmp[j62] = fRec12_perm[j62];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j66 = 0; (j66 < 4); j66 = (j66 + 1)) {
				fRec11_tmp[j66] = fRec11_perm[j66];
			}
			fYec4_idx = ((fYec4_idx + fYec4_idx_save) & 2047);
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j72 = 0; (j72 < 4); j72 = (j72 + 1)) {
				fRec9_tmp[j72] = fRec9_perm[j72];
			}
			fYec5_idx = ((fYec5_idx + fYec5_idx_save) & 4095);
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j74 = 0; (j74 < 4); j74 = (j74 + 1)) {
				fRec7_tmp[j74] = fRec7_perm[j74];
			}
			fYec6_idx = ((fYec6_idx + fYec6_idx_save) & 4095);
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j76 = 0; (j76 < 4); j76 = (j76 + 1)) {
				fRec4_tmp[j76] = fRec4_perm[j76];
			}
			fYec8_idx = ((fYec8_idx + fYec8_idx_save) & 1048575);
			fRec0_idx = ((fRec0_idx + fRec0_idx_save) & 262143);
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec13[i] = (((fRec0[(((i + fRec0_idx) - (int(std::min<float>(fConst0, float(std::max<int>(0, iZec16[i])))) + 1)) & 262143)] * (0.0f - (fZec14[i] + (99.0f - fZec18[i])))) + ((fZec14[i] + (100.0f - fZec18[i])) * fRec0[(((i + fRec0_idx) - (int(std::min<float>(fConst0, float(std::max<int>(0, (iZec16[i] + 1))))) + 1)) & 262143)])) - (fConst9 * ((fConst10 * fRec13[(i - 2)]) + (fConst11 * fRec13[(i - 1)]))));
				fZec20[i] = (fConst9 * (fRec13[(i - 2)] + (fRec13[i] + (2.0f * fRec13[(i - 1)]))));
				fZec21[i] = ((255.0f * std::fabs(fZec20[i])) + 0.5f);
				fZec22[i] = float(int(fZec21[i]));
				float fThen14 = (fZec22[i] + -1.0f);
				float fElse14 = fZec22[i];
				float fThen15 = ((fZec21[i] >= 0.0f) ? fElse14 : fThen14);
				float fElse15 = fZec21[i];
				float fThen16 = fRec22[(i - 1)];
				float fElse16 = (0.00392156886f * (((fZec21[i] == fZec22[i]) ? fElse15 : fThen15) * (float((2 * (fZec20[i] > 0.0f))) + -1.0f)));
				fRec22[i] = (iZec19[i] ? fElse16 : fThen16);
				float fThen17 = fZec20[i];
				float fElse17 = fRec22[i];
				fZec23[i] = (iSlow5 ? fElse17 : fThen17);
				fZec24[i] = std::fabs(fZec23[i]);
				fZec25[i] = ((fZec24[i] > fRec25[(i - 1)]) ? fConst14 : fConst13);
				fRec25[i] = ((fZec24[i] * (1.0f - fZec25[i])) + (fRec25[(i - 1)] * fZec25[i]));
				fRec24[i] = ((fConst16 * (0.0f - (0.75f * std::max<float>(((20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec25[i]))) + 6.0f), 0.0f)))) + (fConst15 * fRec24[(i - 1)]));
				fZec26[i] = (fZec23[i] * std::pow(10.0f, (0.0500000007f * fRec24[i])));
				fZec27[i] = std::fabs(fZec26[i]);
				fZec28[i] = ((fZec27[i] > fRec26[(i - 1)]) ? fConst19 : fConst13);
				fRec26[i] = ((fZec27[i] * (1.0f - fZec28[i])) + (fRec26[(i - 1)] * fZec28[i]));
				fRec12[i] = ((fZec26[i] + ((fRec26[i] * fZec39[i]) * fZec40[i])) - (((fRec12[(i - 2)] * fZec43[i]) + (2.0f * (fRec12[(i - 1)] * fZec45[i]))) / fZec46[i]));
				fRec11[i] = (((((fRec12[(i - 1)] * fZec47[i]) + (fRec12[i] / fZec44[i])) + (fRec12[(i - 2)] / fZec44[i])) / fZec46[i]) - (((fRec11[(i - 2)] * fZec50[i]) + (2.0f * (fRec11[(i - 1)] * fZec51[i]))) / fZec52[i]));
				fYec4[((i + fYec4_idx) & 2047)] = ((0.980000019f * ((fRec11[(i - 2)] + (fRec11[i] + (2.0f * fRec11[(i - 1)]))) / fZec52[i])) - (fRec38[i] * fRec9[(i - 1)]));
				fRec9[i] = ((fYec4[(((i + fYec4_idx) - std::min<int>(iConst48, std::max<int>(0, iZec57[i]))) & 2047)] * (fZec59[i] + (2.0f - fZec55[i]))) + ((fZec55[i] + (-1.0f - fZec59[i])) * fYec4[(((i + fYec4_idx) - std::min<int>(iConst48, std::max<int>(0, (iZec57[i] + 1)))) & 2047)]));
				fRec10[i] = (fRec38[i] * fYec4[((i + fYec4_idx) & 2047)]);
				fYec5[((i + fYec5_idx) & 4095)] = ((fRec10[i] + fRec9[(i - 1)]) - (0.899999976f * (fRec38[i] * fRec7[(i - 1)])));
				fRec7[i] = ((fYec5[(((i + fYec5_idx) - iZec63[i]) & 4095)] * fZec66[i]) + (fZec67[i] * fYec5[(((i + fYec5_idx) - iZec68[i]) & 4095)]));
				fRec8[i] = (0.899999976f * (fRec38[i] * fYec5[((i + fYec5_idx) & 4095)]));
				fYec6[((i + fYec6_idx) & 4095)] = ((fRec8[i] + fRec7[(i - 1)]) - (0.800000012f * (fRec38[i] * fRec4[(i - 1)])));
				fRec4[i] = ((fZec66[i] * fYec6[(((i + fYec6_idx) - iZec63[i]) & 4095)]) + (fZec67[i] * fYec6[(((i + fYec6_idx) - iZec68[i]) & 4095)]));
				fRec5[i] = (0.800000012f * (fRec38[i] * fYec6[((i + fYec6_idx) & 4095)]));
				float fThen54 = fYec7[((i + fYec7_idx) & 131071)];
				float fElse54 = ((((fYec7[(((i + fYec7_idx) - iZec76[i]) & 131071)] * fZec79[i]) + (fZec80[i] * fYec7[(((i + fYec7_idx) - iZec81[i]) & 131071)])) * fZec82[i]) + (((fYec7[(((i + fYec7_idx) - iZec85[i]) & 131071)] * fZec88[i]) + (fZec89[i] * fYec7[(((i + fYec7_idx) - iZec90[i]) & 131071)])) * fZec91[i]));
				float fThen59 = fYec7[((i + fYec7_idx) & 131071)];
				float fElse59 = ((((fYec7[(((i + fYec7_idx) - iZec93[i]) & 131071)] * fZec96[i]) + (fZec97[i] * fYec7[(((i + fYec7_idx) - iZec98[i]) & 131071)])) * fZec99[i]) + (((fYec7[(((i + fYec7_idx) - iZec102[i]) & 131071)] * fZec105[i]) + (fZec106[i] * fYec7[(((i + fYec7_idx) - iZec107[i]) & 131071)])) * fZec108[i]));
				fYec8[((i + fYec8_idx) & 1048575)] = ((iSlow15 ? fElse54 : fThen54) + ((fRec3[i] * (fRec5[i] + fRec4[(i - 1)])) + (iSlow16 ? fElse59 : fThen59)));
				fZec109[i] = fYec8[(((i + fYec8_idx) - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec43[i])))) & 1048575)];
				fRec0[((i + fRec0_idx) & 262143)] = (fZec109[i] + (fRec42[i] * (fYec8[(((i + fYec8_idx) - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec44[i])))) & 1048575)] - fZec109[i])));
			}
			/* Post code */
			fYec8_idx_save = vsize;
			fYec6_idx_save = vsize;
			fYec5_idx_save = vsize;
			fYec4_idx_save = vsize;
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j37 = 0; (j37 < 4); j37 = (j37 + 1)) {
				fRec26_perm[j37] = fRec26_tmp[(vsize + j37)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j33 = 0; (j33 < 4); j33 = (j33 + 1)) {
				fRec25_perm[j33] = fRec25_tmp[(vsize + j33)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j35 = 0; (j35 < 4); j35 = (j35 + 1)) {
				fRec24_perm[j35] = fRec24_tmp[(vsize + j35)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j31 = 0; (j31 < 4); j31 = (j31 + 1)) {
				fRec22_perm[j31] = fRec22_tmp[(vsize + j31)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j27 = 0; (j27 < 4); j27 = (j27 + 1)) {
				fRec13_perm[j27] = fRec13_tmp[(vsize + j27)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j63 = 0; (j63 < 4); j63 = (j63 + 1)) {
				fRec12_perm[j63] = fRec12_tmp[(vsize + j63)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j67 = 0; (j67 < 4); j67 = (j67 + 1)) {
				fRec11_perm[j67] = fRec11_tmp[(vsize + j67)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j73 = 0; (j73 < 4); j73 = (j73 + 1)) {
				fRec9_perm[j73] = fRec9_tmp[(vsize + j73)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j75 = 0; (j75 < 4); j75 = (j75 + 1)) {
				fRec7_perm[j75] = fRec7_tmp[(vsize + j75)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j77 = 0; (j77 < 4); j77 = (j77 + 1)) {
				fRec4_perm[j77] = fRec4_tmp[(vsize + j77)];
			}
			fRec0_idx_save = vsize;
			/* Vectorizable loop 148 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec113[i] = std::cos((fConst57 * fZec110[i]));
			}
			/* Recursive loop 149 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j124 = 0; (j124 < 4); j124 = (j124 + 1)) {
				fRec68_tmp[j124] = fRec68_perm[j124];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j126 = 0; (j126 < 4); j126 = (j126 + 1)) {
				fRec73_tmp[j126] = fRec73_perm[j126];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j128 = 0; (j128 < 4); j128 = (j128 + 1)) {
				fRec75_tmp[j128] = fRec75_perm[j128];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j130 = 0; (j130 < 4); j130 = (j130 + 1)) {
				fRec74_tmp[j130] = fRec74_perm[j130];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j132 = 0; (j132 < 4); j132 = (j132 + 1)) {
				fRec76_tmp[j132] = fRec76_perm[j132];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j134 = 0; (j134 < 4); j134 = (j134 + 1)) {
				fRec67_tmp[j134] = fRec67_perm[j134];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j136 = 0; (j136 < 4); j136 = (j136 + 1)) {
				fRec66_tmp[j136] = fRec66_perm[j136];
			}
			fYec9_idx = ((fYec9_idx + fYec9_idx_save) & 16383);
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j140 = 0; (j140 < 4); j140 = (j140 + 1)) {
				fRec64_tmp[j140] = fRec64_perm[j140];
			}
			fYec10_idx = ((fYec10_idx + fYec10_idx_save) & 16383);
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j142 = 0; (j142 < 4); j142 = (j142 + 1)) {
				fRec62_tmp[j142] = fRec62_perm[j142];
			}
			fYec11_idx = ((fYec11_idx + fYec11_idx_save) & 16383);
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j144 = 0; (j144 < 4); j144 = (j144 + 1)) {
				fRec58_tmp[j144] = fRec58_perm[j144];
			}
			fYec13_idx = ((fYec13_idx + fYec13_idx_save) & 1048575);
			fRec57_idx = ((fRec57_idx + fRec57_idx_save) & 262143);
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec68[i] = (((fRec57[(((i + fRec57_idx) - (int(std::min<float>(fConst0, float(std::max<int>(0, iZec137[i])))) + 1)) & 262143)] * (0.0f - (fZec135[i] + (99.0f - fZec139[i])))) + ((fZec135[i] + (100.0f - fZec139[i])) * fRec57[(((i + fRec57_idx) - (int(std::min<float>(fConst0, float(std::max<int>(0, (iZec137[i] + 1))))) + 1)) & 262143)])) - (fConst9 * ((fConst10 * fRec68[(i - 2)]) + (fConst11 * fRec68[(i - 1)]))));
				fZec140[i] = (fConst9 * (fRec68[(i - 2)] + (fRec68[i] + (2.0f * fRec68[(i - 1)]))));
				fZec141[i] = ((255.0f * std::fabs(fZec140[i])) + 0.5f);
				fZec142[i] = float(int(fZec141[i]));
				float fThen79 = (fZec142[i] + -1.0f);
				float fElse79 = fZec142[i];
				float fThen80 = ((fZec141[i] >= 0.0f) ? fElse79 : fThen79);
				float fElse80 = fZec141[i];
				float fThen81 = fRec73[(i - 1)];
				float fElse81 = (0.00392156886f * (((fZec141[i] == fZec142[i]) ? fElse80 : fThen80) * (float((2 * (fZec140[i] > 0.0f))) + -1.0f)));
				fRec73[i] = (iZec19[i] ? fElse81 : fThen81);
				float fThen82 = fZec140[i];
				float fElse82 = fRec73[i];
				fZec143[i] = (iSlow5 ? fElse82 : fThen82);
				fZec144[i] = std::fabs(fZec143[i]);
				fZec145[i] = ((fZec144[i] > fRec75[(i - 1)]) ? fConst14 : fConst13);
				fRec75[i] = ((fZec144[i] * (1.0f - fZec145[i])) + (fRec75[(i - 1)] * fZec145[i]));
				fRec74[i] = ((fConst16 * (0.0f - (0.75f * std::max<float>(((20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec75[i]))) + 6.0f), 0.0f)))) + (fConst15 * fRec74[(i - 1)]));
				fZec146[i] = (fZec143[i] * std::pow(10.0f, (0.0500000007f * fRec74[i])));
				fZec147[i] = std::fabs(fZec146[i]);
				fZec148[i] = ((fZec147[i] > fRec76[(i - 1)]) ? fConst19 : fConst13);
				fRec76[i] = ((fZec147[i] * (1.0f - fZec148[i])) + (fRec76[(i - 1)] * fZec148[i]));
				fRec67[i] = ((fZec146[i] + ((fZec39[i] * fRec76[i]) * fZec40[i])) - (((fZec43[i] * fRec67[(i - 2)]) + (2.0f * (fZec45[i] * fRec67[(i - 1)]))) / fZec46[i]));
				fRec66[i] = (((((fZec47[i] * fRec67[(i - 1)]) + (fRec67[i] / fZec44[i])) + (fRec67[(i - 2)] / fZec44[i])) / fZec46[i]) - (((fZec50[i] * fRec66[(i - 2)]) + (2.0f * (fZec51[i] * fRec66[(i - 1)]))) / fZec52[i]));
				fYec9[((i + fYec9_idx) & 16383)] = ((0.980000019f * ((fRec66[(i - 2)] + (fRec66[i] + (2.0f * fRec66[(i - 1)]))) / fZec52[i])) - (fRec38[i] * fRec64[(i - 1)]));
				fRec64[i] = ((fYec9[(((i + fYec9_idx) - std::min<int>(iConst48, std::max<int>(0, iZec153[i]))) & 16383)] * (fZec155[i] + (2.0f - fZec151[i]))) + ((fZec151[i] + (-1.0f - fZec155[i])) * fYec9[(((i + fYec9_idx) - std::min<int>(iConst48, std::max<int>(0, (iZec153[i] + 1)))) & 16383)]));
				fRec65[i] = (fRec38[i] * fYec9[((i + fYec9_idx) & 16383)]);
				fYec10[((i + fYec10_idx) & 16383)] = ((fRec65[i] + fRec64[(i - 1)]) - (0.899999976f * (fRec38[i] * fRec62[(i - 1)])));
				fRec62[i] = ((fYec10[(((i + fYec10_idx) - iZec159[i]) & 16383)] * fZec162[i]) + (fZec163[i] * fYec10[(((i + fYec10_idx) - iZec164[i]) & 16383)]));
				fRec63[i] = (0.899999976f * (fRec38[i] * fYec10[((i + fYec10_idx) & 16383)]));
				fYec11[((i + fYec11_idx) & 16383)] = ((fRec63[i] + fRec62[(i - 1)]) - (0.800000012f * (fRec38[i] * fRec58[(i - 1)])));
				fRec58[i] = ((fZec162[i] * fYec11[(((i + fYec11_idx) - iZec159[i]) & 16383)]) + (fZec163[i] * fYec11[(((i + fYec11_idx) - iZec164[i]) & 16383)]));
				fRec59[i] = (0.800000012f * (fRec38[i] * fYec11[((i + fYec11_idx) & 16383)]));
				float fThen101 = fYec12[((i + fYec12_idx) & 131071)];
				float fElse101 = ((fZec82[i] * ((fZec79[i] * fYec12[(((i + fYec12_idx) - iZec76[i]) & 131071)]) + (fZec80[i] * fYec12[(((i + fYec12_idx) - iZec81[i]) & 131071)]))) + (fZec91[i] * ((fZec88[i] * fYec12[(((i + fYec12_idx) - iZec85[i]) & 131071)]) + (fZec89[i] * fYec12[(((i + fYec12_idx) - iZec90[i]) & 131071)]))));
				float fThen102 = fYec12[((i + fYec12_idx) & 131071)];
				float fElse102 = ((fZec99[i] * ((fZec96[i] * fYec12[(((i + fYec12_idx) - iZec93[i]) & 131071)]) + (fZec97[i] * fYec12[(((i + fYec12_idx) - iZec98[i]) & 131071)]))) + (fZec108[i] * ((fZec105[i] * fYec12[(((i + fYec12_idx) - iZec102[i]) & 131071)]) + (fZec106[i] * fYec12[(((i + fYec12_idx) - iZec107[i]) & 131071)]))));
				fYec13[((i + fYec13_idx) & 1048575)] = ((iSlow15 ? fElse101 : fThen101) + ((fRec3[i] * (fRec59[i] + fRec58[(i - 1)])) + (iSlow16 ? fElse102 : fThen102)));
				fZec169[i] = fYec13[(((i + fYec13_idx) - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec80[i])))) & 1048575)];
				fRec57[((i + fRec57_idx) & 262143)] = (fZec169[i] + (fRec79[i] * (fYec13[(((i + fYec13_idx) - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec81[i])))) & 1048575)] - fZec169[i])));
			}
			/* Post code */
			fYec13_idx_save = vsize;
			fYec11_idx_save = vsize;
			fYec10_idx_save = vsize;
			fYec9_idx_save = vsize;
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j133 = 0; (j133 < 4); j133 = (j133 + 1)) {
				fRec76_perm[j133] = fRec76_tmp[(vsize + j133)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j129 = 0; (j129 < 4); j129 = (j129 + 1)) {
				fRec75_perm[j129] = fRec75_tmp[(vsize + j129)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j131 = 0; (j131 < 4); j131 = (j131 + 1)) {
				fRec74_perm[j131] = fRec74_tmp[(vsize + j131)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j127 = 0; (j127 < 4); j127 = (j127 + 1)) {
				fRec73_perm[j127] = fRec73_tmp[(vsize + j127)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j125 = 0; (j125 < 4); j125 = (j125 + 1)) {
				fRec68_perm[j125] = fRec68_tmp[(vsize + j125)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j135 = 0; (j135 < 4); j135 = (j135 + 1)) {
				fRec67_perm[j135] = fRec67_tmp[(vsize + j135)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j137 = 0; (j137 < 4); j137 = (j137 + 1)) {
				fRec66_perm[j137] = fRec66_tmp[(vsize + j137)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j141 = 0; (j141 < 4); j141 = (j141 + 1)) {
				fRec64_perm[j141] = fRec64_tmp[(vsize + j141)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j143 = 0; (j143 < 4); j143 = (j143 + 1)) {
				fRec62_perm[j143] = fRec62_tmp[(vsize + j143)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j145 = 0; (j145 < 4); j145 = (j145 + 1)) {
				fRec58_perm[j145] = fRec58_tmp[(vsize + j145)];
			}
			fRec57_idx_save = vsize;
			/* Recursive loop 150 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j92 = 0; (j92 < 4); j92 = (j92 + 1)) {
				fRec47_tmp[j92] = fRec47_perm[j92];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec47[i] = (fSlow17 + (fConst2 * fRec47[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j93 = 0; (j93 < 4); j93 = (j93 + 1)) {
				fRec47_perm[j93] = fRec47_tmp[(vsize + j93)];
			}
			/* Vectorizable loop 151 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec111[i] = std::cos((fConst55 * fZec110[i]));
			}
			/* Vectorizable loop 152 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec115[i] = (1.0f - (0.5f * fRec47[i]));
			}
			/* Recursive loop 153 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j98 = 0; (j98 < 4); j98 = (j98 + 1)) {
				fRec50_tmp[j98] = fRec50_perm[j98];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j100 = 0; (j100 < 4); j100 = (j100 + 1)) {
				fRec49_tmp[j100] = fRec49_perm[j100];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j102 = 0; (j102 < 4); j102 = (j102 + 1)) {
				fRec48_tmp[j102] = fRec48_perm[j102];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec112[i] = (fRec50[(i - 1)] * fZec111[i]);
				fRec50[i] = ((fRec0[((i + fRec0_idx) & 262143)] + (0.230000004f * fRec48[(i - 1)])) - ((fConst54 * fZec112[i]) + (fConst56 * fRec50[(i - 2)])));
				fZec114[i] = (fRec49[(i - 1)] * fZec113[i]);
				fRec49[i] = ((fConst56 * (fRec50[i] - fRec49[(i - 2)])) + (fRec50[(i - 2)] + (fConst54 * (fZec112[i] - fZec114[i]))));
				fRec48[i] = ((fConst56 * fRec49[i]) + ((fConst54 * fZec114[i]) + fRec49[(i - 2)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j99 = 0; (j99 < 4); j99 = (j99 + 1)) {
				fRec50_perm[j99] = fRec50_tmp[(vsize + j99)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j101 = 0; (j101 < 4); j101 = (j101 + 1)) {
				fRec49_perm[j101] = fRec49_tmp[(vsize + j101)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j103 = 0; (j103 < 4); j103 = (j103 + 1)) {
				fRec48_perm[j103] = fRec48_tmp[(vsize + j103)];
			}
			/* Recursive loop 154 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j156 = 0; (j156 < 4); j156 = (j156 + 1)) {
				fRec85_tmp[j156] = fRec85_perm[j156];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j158 = 0; (j158 < 4); j158 = (j158 + 1)) {
				fRec84_tmp[j158] = fRec84_perm[j158];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j160 = 0; (j160 < 4); j160 = (j160 + 1)) {
				fRec83_tmp[j160] = fRec83_perm[j160];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec170[i] = (fZec111[i] * fRec85[(i - 1)]);
				fRec85[i] = ((fRec57[((i + fRec57_idx) & 262143)] + (0.230000004f * fRec83[(i - 1)])) - ((fConst54 * fZec170[i]) + (fConst56 * fRec85[(i - 2)])));
				fZec171[i] = (fZec113[i] * fRec84[(i - 1)]);
				fRec84[i] = ((fConst56 * (fRec85[i] - fRec84[(i - 2)])) + (fRec85[(i - 2)] + (fConst54 * (fZec170[i] - fZec171[i]))));
				fRec83[i] = ((fConst56 * fRec84[i]) + ((fConst54 * fZec171[i]) + fRec84[(i - 2)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j157 = 0; (j157 < 4); j157 = (j157 + 1)) {
				fRec85_perm[j157] = fRec85_tmp[(vsize + j157)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j159 = 0; (j159 < 4); j159 = (j159 + 1)) {
				fRec84_perm[j159] = fRec84_tmp[(vsize + j159)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j161 = 0; (j161 < 4); j161 = (j161 + 1)) {
				fRec83_perm[j161] = fRec83_tmp[(vsize + j161)];
			}
			/* Vectorizable loop 155 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec116[i] = ((fRec0[((i + fRec0_idx) & 262143)] * fZec115[i]) + (0.5f * (fRec47[i] * fRec48[i])));
			}
			/* Vectorizable loop 156 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec119[i] = std::fabs(float(input0[i]));
			}
			/* Recursive loop 157 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j110 = 0; (j110 < 4); j110 = (j110 + 1)) {
				fRec56_tmp[j110] = fRec56_perm[j110];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec56[i] = (fSlow24 + (fConst2 * fRec56[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j111 = 0; (j111 < 4); j111 = (j111 + 1)) {
				fRec56_perm[j111] = fRec56_tmp[(vsize + j111)];
			}
			/* Vectorizable loop 158 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec172[i] = ((fRec57[((i + fRec57_idx) & 262143)] * fZec115[i]) + (0.5f * (fRec47[i] * fRec83[i])));
			}
			/* Vectorizable loop 159 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec175[i] = std::fabs(float(input1[i]));
			}
			/* Vectorizable loop 160 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec117[i] = std::fabs(fZec116[i]);
			}
			/* Recursive loop 161 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j108 = 0; (j108 < 4); j108 = (j108 + 1)) {
				fRec55_tmp[j108] = fRec55_perm[j108];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec120[i] = ((fZec119[i] > fRec55[(i - 1)]) ? fSlow23 : fSlow20);
				fRec55[i] = ((fZec119[i] * (1.0f - fZec120[i])) + (fRec55[(i - 1)] * fZec120[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j109 = 0; (j109 < 4); j109 = (j109 + 1)) {
				fRec55_perm[j109] = fRec55_tmp[(vsize + j109)];
			}
			/* Vectorizable loop 162 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec122[i] = (50.0f * fRec56[i]);
			}
			/* Vectorizable loop 163 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec173[i] = std::fabs(fZec172[i]);
			}
			/* Recursive loop 164 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j166 = 0; (j166 < 4); j166 = (j166 + 1)) {
				fRec88_tmp[j166] = fRec88_perm[j166];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec176[i] = ((fZec175[i] > fRec88[(i - 1)]) ? fSlow23 : fSlow20);
				fRec88[i] = ((fZec175[i] * (1.0f - fZec176[i])) + (fRec88[(i - 1)] * fZec176[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j167 = 0; (j167 < 4); j167 = (j167 + 1)) {
				fRec88_perm[j167] = fRec88_tmp[(vsize + j167)];
			}
			/* Recursive loop 165 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j104 = 0; (j104 < 4); j104 = (j104 + 1)) {
				fRec54_tmp[j104] = fRec54_perm[j104];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec118[i] = ((fZec117[i] > fRec54[(i - 1)]) ? fConst14 : fConst13);
				fRec54[i] = ((fZec117[i] * (1.0f - fZec118[i])) + (fRec54[(i - 1)] * fZec118[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j105 = 0; (j105 < 4); j105 = (j105 + 1)) {
				fRec54_perm[j105] = fRec54_tmp[(vsize + j105)];
			}
			/* Vectorizable loop 166 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec121[i] = (20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec55[i])));
			}
			/* Vectorizable loop 167 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec123[i] = (-1.5f - fZec122[i]);
			}
			/* Vectorizable loop 168 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec124[i] = (1.5f - fZec122[i]);
			}
			/* Recursive loop 169 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j162 = 0; (j162 < 4); j162 = (j162 + 1)) {
				fRec87_tmp[j162] = fRec87_perm[j162];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec174[i] = ((fZec173[i] > fRec87[(i - 1)]) ? fConst14 : fConst13);
				fRec87[i] = ((fZec173[i] * (1.0f - fZec174[i])) + (fRec87[(i - 1)] * fZec174[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j163 = 0; (j163 < 4); j163 = (j163 + 1)) {
				fRec87_perm[j163] = fRec87_tmp[(vsize + j163)];
			}
			/* Vectorizable loop 170 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec177[i] = (20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec88[i])));
			}
			/* Recursive loop 171 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j106 = 0; (j106 < 4); j106 = (j106 + 1)) {
				fRec53_tmp[j106] = fRec53_perm[j106];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec53[i] = ((fConst16 * (0.0f - (0.75f * std::max<float>(((20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec54[i]))) + 6.0f), 0.0f)))) + (fConst15 * fRec53[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j107 = 0; (j107 < 4); j107 = (j107 + 1)) {
				fRec53_perm[j107] = fRec53_tmp[(vsize + j107)];
			}
			/* Vectorizable loop 172 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec125[i] = ((fZec121[i] > fZec123[i]) + (fZec121[i] > fZec124[i]));
			}
			/* Vectorizable loop 173 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec126[i] = (fZec121[i] + fZec122[i]);
			}
			/* Vectorizable loop 174 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec127[i] = (1.0f - (1.0f / ((19.0f * fRec56[i]) + 1.0f)));
			}
			/* Recursive loop 175 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j164 = 0; (j164 < 4); j164 = (j164 + 1)) {
				fRec86_tmp[j164] = fRec86_perm[j164];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec86[i] = ((fConst16 * (0.0f - (0.75f * std::max<float>(((20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec87[i]))) + 6.0f), 0.0f)))) + (fConst15 * fRec86[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j165 = 0; (j165 < 4); j165 = (j165 + 1)) {
				fRec86_perm[j165] = fRec86_tmp[(vsize + j165)];
			}
			/* Vectorizable loop 176 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec178[i] = ((fZec177[i] > fZec123[i]) + (fZec177[i] > fZec124[i]));
			}
			/* Vectorizable loop 177 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec179[i] = (fZec122[i] + fZec177[i]);
			}
			/* Vectorizable loop 178 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen66 = fZec126[i];
				float fElse66 = (0.166666672f * Macalla_faustpower2_f((fZec126[i] + 1.5f)));
				float fThen67 = ((iZec125[i] == 1) ? fElse66 : fThen66);
				output0[i] = FAUSTFLOAT(((fSlow26 * float(input0[i])) + (fSlow25 * ((fZec116[i] * std::pow(10.0f, (0.0500000007f * fRec53[i]))) * std::pow(10.0f, (0.0500000007f * (0.0f - (std::max<float>(0.0f, ((iZec125[i] == 0) ? 0.0f : fThen67)) * fZec127[i]))))))));
			}
			/* Vectorizable loop 179 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen105 = fZec179[i];
				float fElse105 = (0.166666672f * Macalla_faustpower2_f((fZec179[i] + 1.5f)));
				float fThen106 = ((iZec178[i] == 1) ? fElse105 : fThen105);
				output1[i] = FAUSTFLOAT(((fSlow26 * float(input1[i])) + (fSlow25 * ((fZec172[i] * std::pow(10.0f, (0.0500000007f * fRec86[i]))) * std::pow(10.0f, (0.0500000007f * (0.0f - (fZec127[i] * std::max<float>(0.0f, ((iZec178[i] == 0) ? 0.0f : fThen106))))))))));
			}
		}
		/* Remaining frames */
		if ((vindex < count)) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = (count - vindex);
			/* Vectorizable loop 0 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j6 = 0; (j6 < 4); j6 = (j6 + 1)) {
				iVec0_tmp[j6] = iVec0_perm[j6];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iVec0[i] = 1;
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j7 = 0; (j7 < 4); j7 = (j7 + 1)) {
				iVec0_perm[j7] = iVec0_tmp[(vsize + j7)];
			}
			/* Vectorizable loop 1 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec0[i] = (1 - iVec0[(i - 1)]);
			}
			/* Vectorizable loop 2 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec1[i] = iZec0[i];
			}
			/* Recursive loop 3 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j10 = 0; (j10 < 4); j10 = (j10 + 1)) {
				fRec14_tmp[j10] = fRec14_perm[j10];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec14[i] = (fSlow4 + (fConst2 * fRec14[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j11 = 0; (j11 < 4); j11 = (j11 + 1)) {
				fRec14_perm[j11] = fRec14_tmp[(vsize + j11)];
			}
			/* Recursive loop 4 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j12 = 0; (j12 < 4); j12 = (j12 + 1)) {
				iRec16_tmp[j12] = iRec16_perm[j12];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iRec16[i] = ((1103515245 * iRec16[(i - 1)]) + 12345);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j13 = 0; (j13 < 4); j13 = (j13 + 1)) {
				iRec16_perm[j13] = iRec16_tmp[(vsize + j13)];
			}
			/* Recursive loop 5 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j112 = 0; (j112 < 4); j112 = (j112 + 1)) {
				fRec60_tmp[j112] = fRec60_perm[j112];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec60[i] = (fSlow27 + (fConst2 * fRec60[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j113 = 0; (j113 < 4); j113 = (j113 + 1)) {
				fRec60_perm[j113] = fRec60_tmp[(vsize + j113)];
			}
			/* Vectorizable loop 6 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec4[i] = (1.99000001f * fRec14[i]);
			}
			/* Vectorizable loop 7 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec130[i] = (0.200000003f * fRec60[i]);
			}
			/* Vectorizable loop 8 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j16 = 0; (j16 < 4); j16 = (j16 + 1)) {
				fYec0_tmp[j16] = fYec0_perm[j16];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec0[i] = float(iRec16[i]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j17 = 0; (j17 < 4); j17 = (j17 + 1)) {
				fYec0_perm[j17] = fYec0_tmp[(vsize + j17)];
			}
			/* Vectorizable loop 9 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec8[i] = (18.0f * fRec14[i]);
			}
			/* Recursive loop 10 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j22 = 0; (j22 < 4); j22 = (j22 + 1)) {
				fRec20_tmp[j22] = fRec20_perm[j22];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen9 = (fConst6 + fRec20[(i - 1)]);
				fZec11[i] = (iZec1[i] ? 0.0f : fThen9);
				fZec12[i] = float(int(fZec11[i]));
				float fThen10 = (fZec12[i] + -1.0f);
				float fElse10 = fZec12[i];
				float fThen11 = ((fZec11[i] >= 0.0f) ? fElse10 : fThen10);
				float fElse11 = fZec11[i];
				fRec20[i] = (fZec11[i] - ((fZec11[i] == fZec12[i]) ? fElse11 : fThen11));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j23 = 0; (j23 < 4); j23 = (j23 + 1)) {
				fRec20_perm[j23] = fRec20_tmp[(vsize + j23)];
			}
			/* Recursive loop 11 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j8 = 0; (j8 < 4); j8 = (j8 + 1)) {
				fRec6_tmp[j8] = fRec6_perm[j8];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen0 = (fConst3 + fRec6[(i - 1)]);
				fZec2[i] = (iZec1[i] ? 0.0f : fThen0);
				fZec3[i] = float(int(fZec2[i]));
				float fThen1 = (fZec3[i] + -1.0f);
				float fElse1 = fZec3[i];
				float fThen2 = ((fZec2[i] >= 0.0f) ? fElse1 : fThen1);
				float fElse2 = fZec2[i];
				fRec6[i] = (fZec2[i] - ((fZec2[i] == fZec3[i]) ? fElse2 : fThen2));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j9 = 0; (j9 < 4); j9 = (j9 + 1)) {
				fRec6_perm[j9] = fRec6_tmp[(vsize + j9)];
			}
			/* Recursive loop 12 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j14 = 0; (j14 < 4); j14 = (j14 + 1)) {
				fRec17_tmp[j14] = fRec17_perm[j14];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen3 = (fRec17[(i - 1)] + (fConst5 * (fZec4[i] + 0.00999999978f)));
				fZec5[i] = (iZec1[i] ? 0.0f : fThen3);
				fZec6[i] = float(int(fZec5[i]));
				float fThen4 = (fZec6[i] + -1.0f);
				float fElse4 = fZec6[i];
				float fThen5 = ((fZec5[i] >= 0.0f) ? fElse4 : fThen4);
				float fElse5 = fZec5[i];
				fRec17[i] = (fZec5[i] - ((fZec5[i] == fZec6[i]) ? fElse5 : fThen5));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j15 = 0; (j15 < 4); j15 = (j15 + 1)) {
				fRec17_perm[j15] = fRec17_tmp[(vsize + j15)];
			}
			/* Recursive loop 13 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j114 = 0; (j114 < 4); j114 = (j114 + 1)) {
				fRec61_tmp[j114] = fRec61_perm[j114];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen68 = (fRec61[(i - 1)] + (fConst5 * ((0.0299999993f * fRec60[i]) + 0.230000004f)));
				fZec128[i] = (iZec1[i] ? 0.0f : fThen68);
				fZec129[i] = float(int(fZec128[i]));
				float fThen69 = (fZec129[i] + -1.0f);
				float fElse69 = fZec129[i];
				float fThen70 = ((fZec128[i] >= 0.0f) ? fElse69 : fThen69);
				float fElse70 = fZec128[i];
				fRec61[i] = (fZec128[i] - ((fZec128[i] == fZec129[i]) ? fElse70 : fThen70));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j115 = 0; (j115 < 4); j115 = (j115 + 1)) {
				fRec61_perm[j115] = fRec61_tmp[(vsize + j115)];
			}
			/* Recursive loop 14 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j116 = 0; (j116 < 4); j116 = (j116 + 1)) {
				fRec70_tmp[j116] = fRec70_perm[j116];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen71 = (fRec70[(i - 1)] + (fConst5 * ((fZec4[i] + fZec130[i]) + 0.00999999978f)));
				fZec131[i] = (iZec1[i] ? 0.0f : fThen71);
				fZec132[i] = float(int(fZec131[i]));
				float fThen72 = (fZec132[i] + -1.0f);
				float fElse72 = fZec132[i];
				float fThen73 = ((fZec131[i] >= 0.0f) ? fElse72 : fThen72);
				float fElse73 = fZec131[i];
				fRec70[i] = (fZec131[i] - ((fZec131[i] == fZec132[i]) ? fElse73 : fThen73));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j117 = 0; (j117 < 4); j117 = (j117 + 1)) {
				fRec70_perm[j117] = fRec70_tmp[(vsize + j117)];
			}
			/* Recursive loop 15 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j120 = 0; (j120 < 4); j120 = (j120 + 1)) {
				fRec72_tmp[j120] = fRec72_perm[j120];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen74 = (fRec72[(i - 1)] + (fConst5 * ((fZec8[i] + fZec130[i]) + 2.0f)));
				fZec133[i] = (iZec1[i] ? 0.0f : fThen74);
				fZec134[i] = float(int(fZec133[i]));
				float fThen75 = (fZec134[i] + -1.0f);
				float fElse75 = fZec134[i];
				float fThen76 = ((fZec133[i] >= 0.0f) ? fElse75 : fThen75);
				float fElse76 = fZec133[i];
				fRec72[i] = (fZec133[i] - ((fZec133[i] == fZec134[i]) ? fElse76 : fThen76));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j121 = 0; (j121 < 4); j121 = (j121 + 1)) {
				fRec72_perm[j121] = fRec72_tmp[(vsize + j121)];
			}
			/* Vectorizable loop 16 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec7[i] = (4.65661277e-12f * (fRec14[i] * fYec0[i]));
			}
			/* Recursive loop 17 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j20 = 0; (j20 < 4); j20 = (j20 + 1)) {
				fRec19_tmp[j20] = fRec19_perm[j20];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen6 = (fRec19[(i - 1)] + (fConst5 * (fZec8[i] + 2.0f)));
				fZec9[i] = (iZec1[i] ? 0.0f : fThen6);
				fZec10[i] = float(int(fZec9[i]));
				float fThen7 = (fZec10[i] + -1.0f);
				float fElse7 = fZec10[i];
				float fThen8 = ((fZec9[i] >= 0.0f) ? fElse7 : fThen7);
				float fElse8 = fZec9[i];
				fRec19[i] = (fZec9[i] - ((fZec9[i] == fZec10[i]) ? fElse8 : fThen8));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j21 = 0; (j21 < 4); j21 = (j21 + 1)) {
				fRec19_perm[j21] = fRec19_tmp[(vsize + j21)];
			}
			/* Vectorizable loop 18 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec13[i] = ((0.200000003f * ftbl0MacallaSIG0[int((65536.0f * fRec20[i]))]) + 0.800000012f);
			}
			/* Recursive loop 19 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j42 = 0; (j42 < 4); j42 = (j42 + 1)) {
				fRec30_tmp[j42] = fRec30_perm[j42];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen20 = (fConst17 + fRec30[(i - 1)]);
				fZec29[i] = (iZec1[i] ? 0.0f : fThen20);
				fZec30[i] = float(int(fZec29[i]));
				float fThen21 = (fZec30[i] + -1.0f);
				float fElse21 = fZec30[i];
				float fThen22 = ((fZec29[i] >= 0.0f) ? fElse21 : fThen21);
				float fElse22 = fZec29[i];
				fRec30[i] = (fZec29[i] - ((fZec29[i] == fZec30[i]) ? fElse22 : fThen22));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j43 = 0; (j43 < 4); j43 = (j43 + 1)) {
				fRec30_perm[j43] = fRec30_tmp[(vsize + j43)];
			}
			/* Recursive loop 20 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j138 = 0; (j138 < 4); j138 = (j138 + 1)) {
				fRec77_tmp[j138] = fRec77_perm[j138];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen85 = (fRec77[(i - 1)] + (fConst5 * ((0.0199999996f * fRec60[i]) + 0.150000006f)));
				fZec149[i] = (iZec1[i] ? 0.0f : fThen85);
				fZec150[i] = float(int(fZec149[i]));
				float fThen86 = (fZec150[i] + -1.0f);
				float fElse86 = fZec150[i];
				float fThen87 = ((fZec149[i] >= 0.0f) ? fElse86 : fThen86);
				float fElse87 = fZec149[i];
				fRec77[i] = (fZec149[i] - ((fZec149[i] == fZec150[i]) ? fElse87 : fThen87));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j139 = 0; (j139 < 4); j139 = (j139 + 1)) {
				fRec77_perm[j139] = fRec77_tmp[(vsize + j139)];
			}
			/* Recursive loop 21 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j2 = 0; (j2 < 4); j2 = (j2 + 1)) {
				fRec2_tmp[j2] = fRec2_perm[j2];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec2[i] = ((fRec2[(i - 1)] + (4097.0f - float((4096 * int((0.000244140625f * ((fRec2[(i - 1)] + 4097.0f) - fSlow2))))))) - fSlow2);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j3 = 0; (j3 < 4); j3 = (j3 + 1)) {
				fRec2_perm[j3] = fRec2_tmp[(vsize + j3)];
			}
			/* Recursive loop 22 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j18 = 0; (j18 < 4); j18 = (j18 + 1)) {
				fRec15_tmp[j18] = fRec15_perm[j18];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec15[i] = ((9.99999975e-05f * (fZec7[i] + (1.0f - std::fabs(((2.0f * fRec17[i]) + -1.0f))))) + (0.999899983f * fRec15[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j19 = 0; (j19 < 4); j19 = (j19 + 1)) {
				fRec15_perm[j19] = fRec15_tmp[(vsize + j19)];
			}
			/* Recursive loop 23 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j70 = 0; (j70 < 4); j70 = (j70 + 1)) {
				fRec39_tmp[j70] = fRec39_perm[j70];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen34 = (fConst46 + fRec39[(i - 1)]);
				fZec53[i] = (iZec1[i] ? 0.0f : fThen34);
				fZec54[i] = float(int(fZec53[i]));
				float fThen35 = (fZec54[i] + -1.0f);
				float fElse35 = fZec54[i];
				float fThen36 = ((fZec53[i] >= 0.0f) ? fElse35 : fThen35);
				float fElse36 = fZec53[i];
				fRec39[i] = (fZec53[i] - ((fZec53[i] == fZec54[i]) ? fElse36 : fThen36));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j71 = 0; (j71 < 4); j71 = (j71 + 1)) {
				fRec39_perm[j71] = fRec39_tmp[(vsize + j71)];
			}
			/* Vectorizable loop 24 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec60[i] = std::max<float>(0.0f, std::min<float>(fConst49, (fConst50 * ((1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec6[i]) + -1.0f)))) + -1.0f)) + 11.3000002f))));
			}
			/* Recursive loop 25 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j118 = 0; (j118 < 4); j118 = (j118 + 1)) {
				fRec69_tmp[j118] = fRec69_perm[j118];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec69[i] = ((9.99999975e-05f * (fZec7[i] + (1.0f - std::fabs(((2.0f * fRec70[i]) + -1.0f))))) + (0.999899983f * fRec69[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j119 = 0; (j119 < 4); j119 = (j119 + 1)) {
				fRec69_perm[j119] = fRec69_tmp[(vsize + j119)];
			}
			/* Recursive loop 26 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j122 = 0; (j122 < 4); j122 = (j122 + 1)) {
				fRec71_tmp[j122] = fRec71_perm[j122];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec71[i] = ((9.99999975e-05f * (fZec13[i] * (fZec7[i] + (1.0f - std::fabs(((2.0f * fRec72[i]) + -1.0f)))))) + (0.999899983f * fRec71[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j123 = 0; (j123 < 4); j123 = (j123 + 1)) {
				fRec71_perm[j123] = fRec71_tmp[(vsize + j123)];
			}
			/* Recursive loop 27 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j24 = 0; (j24 < 4); j24 = (j24 + 1)) {
				fRec18_tmp[j24] = fRec18_perm[j24];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec18[i] = ((9.99999975e-05f * ((fZec7[i] + (1.0f - std::fabs(((2.0f * fRec19[i]) + -1.0f)))) * fZec13[i])) + (0.999899983f * fRec18[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j25 = 0; (j25 < 4); j25 = (j25 + 1)) {
				fRec18_perm[j25] = fRec18_tmp[(vsize + j25)];
			}
			/* Vectorizable loop 28 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j44 = 0; (j44 < 4); j44 = (j44 + 1)) {
				fYec1_tmp[j44] = fYec1_perm[j44];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec1[i] = (fRec30[i] - fRec30[(i - 1)]);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j45 = 0; (j45 < 4); j45 = (j45 + 1)) {
				fYec1_perm[j45] = fYec1_tmp[(vsize + j45)];
			}
			/* Recursive loop 29 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j48 = 0; (j48 < 4); j48 = (j48 + 1)) {
				fRec33_tmp[j48] = fRec33_perm[j48];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen23 = (fConst20 + fRec33[(i - 1)]);
				fZec32[i] = (iZec1[i] ? 0.0f : fThen23);
				fZec33[i] = float(int(fZec32[i]));
				float fThen24 = (fZec33[i] + -1.0f);
				float fElse24 = fZec33[i];
				float fThen25 = ((fZec32[i] >= 0.0f) ? fElse24 : fThen24);
				float fElse25 = fZec32[i];
				fRec33[i] = (fZec32[i] - ((fZec32[i] == fZec33[i]) ? fElse25 : fThen25));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j49 = 0; (j49 < 4); j49 = (j49 + 1)) {
				fRec33_perm[j49] = fRec33_tmp[(vsize + j49)];
			}
			/* Recursive loop 30 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j78 = 0; (j78 < 4); j78 = (j78 + 1)) {
				fRec40_tmp[j78] = fRec40_perm[j78];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec40[i] = ((fRec40[(i - 1)] + (4097.0f - float((4096 * int((0.000244140625f * ((fRec40[(i - 1)] + 4097.0f) - fSlow11))))))) - fSlow11);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j79 = 0; (j79 < 4); j79 = (j79 + 1)) {
				fRec40_perm[j79] = fRec40_tmp[(vsize + j79)];
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec156[i] = std::max<float>(0.0f, std::min<float>(fConst49, (fConst50 * (((0.699999988f * fRec60[i]) + (1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec61[i]) + -1.0f)))) + -1.0f))) + 11.3000002f))));
			}
			/* Vectorizable loop 32 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec151[i] = std::max<float>(0.0f, std::min<float>(fConst49, (fConst50 * (((0.5f * fRec60[i]) + (1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec77[i]) + -1.0f)))) + -1.0f))) + 7.0f))));
			}
			/* Vectorizable loop 33 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec157[i] = (fZec156[i] + -1.0f);
			}
			/* Vectorizable loop 34 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec55[i] = std::max<float>(0.0f, std::min<float>(fConst49, (fConst50 * ((1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec39[i]) + -1.0f)))) + -1.0f)) + 7.0f))));
			}
			/* Vectorizable loop 35 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec61[i] = (fZec60[i] + -1.0f);
			}
			/* Vectorizable loop 36 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec14[i] = (fRec14[i] * ((600.0f * fRec15[i]) + (100.0f * fRec18[i])));
			}
			/* Vectorizable loop 37 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec31[i] = ((fYec1[(i - 1)] <= 0.0f) & (fYec1[i] > 0.0f));
			}
			/* Vectorizable loop 38 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j50 = 0; (j50 < 4); j50 = (j50 + 1)) {
				fYec2_tmp[j50] = fYec2_perm[j50];
			}
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec2[i] = Macalla_faustpower2_f(((2.0f * fRec33[i]) + -1.0f));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j51 = 0; (j51 < 4); j51 = (j51 + 1)) {
				fYec2_perm[j51] = fYec2_tmp[(vsize + j51)];
			}
			/* Recursive loop 39 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j80 = 0; (j80 < 4); j80 = (j80 + 1)) {
				fRec45_tmp[j80] = fRec45_perm[j80];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec45[i] = (fSlow12 + (fConst2 * fRec45[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j81 = 0; (j81 < 4); j81 = (j81 + 1)) {
				fRec45_perm[j81] = fRec45_tmp[(vsize + j81)];
			}
			/* Vectorizable loop 40 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec83[i] = (fRec2[i] + 4096.0f);
			}
			/* Vectorizable loop 41 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec100[i] = (fRec40[i] + 4096.0f);
			}
			/* Vectorizable loop 42 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec135[i] = (fRec14[i] * ((600.0f * fRec69[i]) + (100.0f * fRec71[i])));
			}
			/* Vectorizable loop 43 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec152[i] = (fZec151[i] + -1.0f);
			}
			/* Vectorizable loop 44 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec158[i] = int(fZec157[i]);
			}
			/* Recursive loop 45 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j60 = 0; (j60 < 4); j60 = (j60 + 1)) {
				fRec36_tmp[j60] = fRec36_perm[j60];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec36[i] = (fSlow7 + (fConst2 * fRec36[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j61 = 0; (j61 < 4); j61 = (j61 + 1)) {
				fRec36_perm[j61] = fRec36_tmp[(vsize + j61)];
			}
			/* Recursive loop 46 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j64 = 0; (j64 < 4); j64 = (j64 + 1)) {
				fRec37_tmp[j64] = fRec37_perm[j64];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec37[i] = (fSlow8 + (fConst2 * fRec37[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j65 = 0; (j65 < 4); j65 = (j65 + 1)) {
				fRec37_perm[j65] = fRec37_tmp[(vsize + j65)];
			}
			/* Vectorizable loop 47 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec56[i] = (fZec55[i] + -1.0f);
			}
			/* Vectorizable loop 48 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec62[i] = int(fZec61[i]);
			}
			/* Vectorizable loop 49 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec15[i] = (fZec14[i] + 100.000008f);
			}
			/* Recursive loop 50 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j46 = 0; (j46 < 4); j46 = (j46 + 1)) {
				fRec31_tmp[j46] = fRec31_perm[j46];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec31[i] = ((fRec31[(i - 1)] * float((1 - iZec31[i]))) + (4.65661287e-10f * (fYec0[i] * float(iZec31[i]))));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j47 = 0; (j47 < 4); j47 = (j47 + 1)) {
				fRec31_perm[j47] = fRec31_tmp[(vsize + j47)];
			}
			/* Vectorizable loop 51 */
			/* Pre code */
			fYec3_idx = ((fYec3_idx + fYec3_idx_save) & 2047);
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec3[((i + fYec3_idx) & 2047)] = (float(iVec0[(i - 1)]) * (fYec2[i] - fYec2[(i - 1)]));
			}
			/* Post code */
			fYec3_idx_save = vsize;
			/* Vectorizable loop 52 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec69[i] = (4.98999977f * fRec45[i]);
			}
			/* Vectorizable loop 53 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec75[i] = int(fRec2[i]);
			}
			/* Vectorizable loop 54 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec84[i] = int(fZec83[i]);
			}
			/* Vectorizable loop 55 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec92[i] = int(fRec40[i]);
			}
			/* Vectorizable loop 56 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec101[i] = int(fZec100[i]);
			}
			/* Vectorizable loop 57 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec136[i] = (fZec135[i] + 100.000008f);
			}
			/* Vectorizable loop 58 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec153[i] = int(fZec152[i]);
			}
			/* Vectorizable loop 59 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec160[i] = float(iZec158[i]);
			}
			/* Recursive loop 60 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j0 = 0; (j0 < 4); j0 = (j0 + 1)) {
				fRec1_tmp[j0] = fRec1_perm[j0];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec1[i] = (fSlow0 + (fConst2 * fRec1[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j1 = 0; (j1 < 4); j1 = (j1 + 1)) {
				fRec1_perm[j1] = fRec1_tmp[(vsize + j1)];
			}
			/* Recursive loop 61 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j54 = 0; (j54 < 4); j54 = (j54 + 1)) {
				fRec34_tmp[j54] = fRec34_perm[j54];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen28 = (fConst29 + fRec34[(i - 1)]);
				fZec34[i] = (iZec1[i] ? 0.0f : fThen28);
				fZec35[i] = float(int(fZec34[i]));
				float fThen29 = (fZec35[i] + -1.0f);
				float fElse29 = fZec35[i];
				float fThen30 = ((fZec34[i] >= 0.0f) ? fElse29 : fThen29);
				float fElse30 = fZec34[i];
				fRec34[i] = (fZec34[i] - ((fZec34[i] == fZec35[i]) ? fElse30 : fThen30));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j55 = 0; (j55 < 4); j55 = (j55 + 1)) {
				fRec34_perm[j55] = fRec34_tmp[(vsize + j55)];
			}
			/* Recursive loop 62 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j56 = 0; (j56 < 4); j56 = (j56 + 1)) {
				fRec35_tmp[j56] = fRec35_perm[j56];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen31 = (fConst30 + fRec35[(i - 1)]);
				fZec36[i] = (iZec1[i] ? 0.0f : fThen31);
				fZec37[i] = float(int(fZec36[i]));
				float fThen32 = (fZec37[i] + -1.0f);
				float fElse32 = fZec37[i];
				float fThen33 = ((fZec36[i] >= 0.0f) ? fElse32 : fThen32);
				float fElse33 = fZec36[i];
				fRec35[i] = (fZec36[i] - ((fZec36[i] == fZec37[i]) ? fElse33 : fThen33));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j57 = 0; (j57 < 4); j57 = (j57 + 1)) {
				fRec35_perm[j57] = fRec35_tmp[(vsize + j57)];
			}
			/* Vectorizable loop 63 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec38[i] = (0.5f * (fRec31[i] + 1.0f));
			}
			/* Vectorizable loop 64 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec41[i] = std::tan((fConst44 * fRec36[i]));
			}
			/* Vectorizable loop 65 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec48[i] = std::tan((fConst44 * fRec37[i]));
			}
			/* Vectorizable loop 66 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec57[i] = int(fZec56[i]);
			}
			/* Vectorizable loop 67 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec64[i] = float(iZec62[i]);
			}
			/* Vectorizable loop 68 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec16[i] = int(fZec15[i]);
			}
			/* Recursive loop 69 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j40 = 0; (j40 < 4); j40 = (j40 + 1)) {
				fRec29_tmp[j40] = fRec29_perm[j40];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec29[i] = (((4.65661287e-10f * fYec0[i]) + ((2.49495602f * fRec29[(i - 1)]) + (0.522189379f * fRec29[(i - 3)]))) - (2.0172658f * fRec29[(i - 2)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j41 = 0; (j41 < 4); j41 = (j41 + 1)) {
				fRec29_perm[j41] = fRec29_tmp[(vsize + j41)];
			}
			/* Recursive loop 70 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j52 = 0; (j52 < 4); j52 = (j52 + 1)) {
				fRec32_tmp[j52] = fRec32_perm[j52];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec32[i] = ((0.999000013f * fRec32[(i - 1)]) + (fConst21 * ((fYec3[((i + fYec3_idx) & 2047)] - (fYec3[(((i + fYec3_idx) - iConst23) & 2047)] * fConst26)) - (fConst27 * fYec3[(((i + fYec3_idx) - iConst28) & 2047)]))));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j53 = 0; (j53 < 4); j53 = (j53 + 1)) {
				fRec32_perm[j53] = fRec32_tmp[(vsize + j53)];
			}
			/* Recursive loop 71 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j82 = 0; (j82 < 4); j82 = (j82 + 1)) {
				fRec46_tmp[j82] = fRec46_perm[j82];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen41 = (fRec46[(i - 1)] + (fConst5 * (fZec69[i] + 0.00999999978f)));
				fZec70[i] = (iZec1[i] ? 0.0f : fThen41);
				fZec71[i] = float(int(fZec70[i]));
				float fThen42 = (fZec71[i] + -1.0f);
				float fElse42 = fZec71[i];
				float fThen43 = ((fZec70[i] >= 0.0f) ? fElse42 : fThen42);
				float fElse43 = fZec70[i];
				fRec46[i] = (fZec70[i] - ((fZec70[i] == fZec71[i]) ? fElse43 : fThen43));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j83 = 0; (j83 < 4); j83 = (j83 + 1)) {
				fRec46_perm[j83] = fRec46_tmp[(vsize + j83)];
			}
			/* Vectorizable loop 72 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec77[i] = float(iZec75[i]);
			}
			/* Vectorizable loop 73 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec86[i] = float(iZec84[i]);
			}
			/* Vectorizable loop 74 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec94[i] = float(iZec92[i]);
			}
			/* Vectorizable loop 75 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec103[i] = float(iZec101[i]);
			}
			/* Vectorizable loop 76 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec137[i] = int(fZec136[i]);
			}
			/* Recursive loop 77 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j146 = 0; (j146 < 4); j146 = (j146 + 1)) {
				fRec82_tmp[j146] = fRec82_perm[j146];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen92 = (fRec82[(i - 1)] + (fConst5 * ((fZec69[i] + (2.0f * fRec60[i])) + 0.00999999978f)));
				fZec165[i] = (iZec1[i] ? 0.0f : fThen92);
				fZec166[i] = float(int(fZec165[i]));
				float fThen93 = (fZec166[i] + -1.0f);
				float fElse93 = fZec166[i];
				float fThen94 = ((fZec165[i] >= 0.0f) ? fElse93 : fThen93);
				float fElse94 = fZec165[i];
				fRec82[i] = (fZec165[i] - ((fZec165[i] == fZec166[i]) ? fElse94 : fThen94));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j147 = 0; (j147 < 4); j147 = (j147 + 1)) {
				fRec82_perm[j147] = fRec82_tmp[(vsize + j147)];
			}
			/* Vectorizable loop 78 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec154[i] = float(iZec153[i]);
			}
			/* Vectorizable loop 79 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen90 = (fZec160[i] + -1.0f);
				float fElse90 = fZec160[i];
				float fThen91 = ((fZec157[i] >= 0.0f) ? fElse90 : fThen90);
				float fElse91 = fZec157[i];
				fZec161[i] = ((fZec157[i] == fZec160[i]) ? fElse91 : fThen91);
			}
			/* Vectorizable loop 80 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec167[i] = std::min<float>(524288.0f, (fSlow28 + ((40.0f * fRec60[i]) + ((fRec45[i] * (fConst0 + (1000.0f * fRec60[i]))) * std::fabs(((2.0f * (1.0f - std::fabs(((2.0f * fRec82[i]) + -1.0f)))) + -1.0f))))));
			}
			/* Recursive loop 81 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j28 = 0; (j28 < 4); j28 = (j28 + 1)) {
				iRec23_tmp[j28] = iRec23_perm[j28];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iRec23[i] = (iRec23[(i - 1)] + 1);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j29 = 0; (j29 < 4); j29 = (j29 + 1)) {
				iRec23_perm[j29] = iRec23_tmp[(vsize + j29)];
			}
			/* Vectorizable loop 82 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec42[i] = (1.0f / fZec41[i]);
			}
			/* Vectorizable loop 83 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec44[i] = Macalla_faustpower2_f(fZec41[i]);
			}
			/* Vectorizable loop 84 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec49[i] = (1.0f / fZec48[i]);
			}
			/* Vectorizable loop 85 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec58[i] = float(iZec57[i]);
			}
			/* Vectorizable loop 86 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen39 = (fZec64[i] + -1.0f);
				float fElse39 = fZec64[i];
				float fThen40 = ((fZec61[i] >= 0.0f) ? fElse39 : fThen39);
				float fElse40 = fZec61[i];
				fZec65[i] = ((fZec61[i] == fZec64[i]) ? fElse40 : fThen40);
			}
			/* Vectorizable loop 87 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec17[i] = float(iZec16[i]);
			}
			/* Recursive loop 88 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j38 = 0; (j38 < 4); j38 = (j38 + 1)) {
				fRec27_tmp[j38] = fRec27_perm[j38];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec27[i] = (fSlow6 + (fConst2 * fRec27[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j39 = 0; (j39 < 4); j39 = (j39 + 1)) {
				fRec27_perm[j39] = fRec27_tmp[(vsize + j39)];
			}
			/* Recursive loop 89 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j58 = 0; (j58 < 4); j58 = (j58 + 1)) {
				fRec28_tmp[j58] = fRec28_perm[j58];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec28[i] = (((((1.20000005f * (((0.0499220341f * fRec29[i]) + (0.0506126992f * fRec29[(i - 2)])) - ((0.0959935337f * fRec29[(i - 1)]) + (0.00440878607f * fRec29[(i - 3)])))) + (2.32830644e-10f * (fYec0[(i - 1)] * float(((fRec30[i] >= fZec38[i]) * (fRec30[(i - 1)] < fZec38[i])))))) + (fConst31 * fRec32[i])) + (0.00999999978f * (ftbl0MacallaSIG0[int((65536.0f * fRec34[i]))] + ftbl0MacallaSIG0[int((65536.0f * fRec35[i]))]))) - (fConst39 * ((fConst40 * fRec28[(i - 1)]) + (fConst41 * fRec28[(i - 2)]))));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j59 = 0; (j59 < 4); j59 = (j59 + 1)) {
				fRec28_perm[j59] = fRec28_tmp[(vsize + j59)];
			}
			/* Vectorizable loop 90 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec72[i] = std::min<float>(524288.0f, (fConst0 * (fSlow14 + (fRec45[i] * std::fabs(((2.0f * (1.0f - std::fabs(((2.0f * fRec46[i]) + -1.0f)))) + -1.0f))))));
			}
			/* Vectorizable loop 91 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec74[i] = std::pow(10.0f, (0.0500000007f * fRec1[i]));
			}
			/* Vectorizable loop 92 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen50 = (fZec77[i] + -1.0f);
				float fElse50 = fZec77[i];
				float fThen51 = ((fRec2[i] >= 0.0f) ? fElse50 : fThen50);
				float fElse51 = fRec2[i];
				fZec78[i] = ((fRec2[i] == fZec77[i]) ? fElse51 : fThen51);
			}
			/* Vectorizable loop 93 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec82[i] = std::min<float>((0.001953125f * fRec2[i]), 1.0f);
			}
			/* Vectorizable loop 94 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen52 = (fZec86[i] + -1.0f);
				float fElse52 = fZec86[i];
				float fThen53 = ((fZec83[i] >= 0.0f) ? fElse52 : fThen52);
				float fElse53 = fZec83[i];
				fZec87[i] = ((fZec83[i] == fZec86[i]) ? fElse53 : fThen53);
			}
			/* Vectorizable loop 95 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen55 = (fZec94[i] + -1.0f);
				float fElse55 = fZec94[i];
				float fThen56 = ((fRec40[i] >= 0.0f) ? fElse55 : fThen55);
				float fElse56 = fRec40[i];
				fZec95[i] = ((fRec40[i] == fZec94[i]) ? fElse56 : fThen56);
			}
			/* Vectorizable loop 96 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec99[i] = std::min<float>((0.001953125f * fRec40[i]), 1.0f);
			}
			/* Vectorizable loop 97 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen57 = (fZec103[i] + -1.0f);
				float fElse57 = fZec103[i];
				float fThen58 = ((fZec100[i] >= 0.0f) ? fElse57 : fThen57);
				float fElse58 = fZec100[i];
				fZec104[i] = ((fZec100[i] == fZec103[i]) ? fElse58 : fThen58);
			}
			/* Recursive loop 98 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j94 = 0; (j94 < 4); j94 = (j94 + 1)) {
				fRec51_tmp[j94] = fRec51_perm[j94];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j96 = 0; (j96 < 4); j96 = (j96 + 1)) {
				fRec52_tmp[j96] = fRec52_perm[j96];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec51[i] = ((fConst51 * fRec52[(i - 1)]) + (fConst52 * fRec51[(i - 1)]));
				fRec52[i] = ((float(iZec0[i]) + (fConst52 * fRec52[(i - 1)])) - (fConst51 * fRec51[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j95 = 0; (j95 < 4); j95 = (j95 + 1)) {
				fRec51_perm[j95] = fRec51_tmp[(vsize + j95)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j97 = 0; (j97 < 4); j97 = (j97 + 1)) {
				fRec52_perm[j97] = fRec52_tmp[(vsize + j97)];
			}
			/* Vectorizable loop 99 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec138[i] = float(iZec137[i]);
			}
			/* Vectorizable loop 100 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen88 = (fZec154[i] + -1.0f);
				float fElse88 = fZec154[i];
				float fThen89 = ((fZec152[i] >= 0.0f) ? fElse88 : fThen88);
				float fElse89 = fZec152[i];
				fZec155[i] = ((fZec152[i] == fZec154[i]) ? fElse89 : fThen89);
			}
			/* Vectorizable loop 101 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec159[i] = std::min<int>(iConst48, std::max<int>(0, iZec158[i]));
			}
			/* Vectorizable loop 102 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec162[i] = (fZec161[i] + (2.0f - fZec156[i]));
			}
			/* Recursive loop 103 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j4 = 0; (j4 < 4); j4 = (j4 + 1)) {
				fRec3_tmp[j4] = fRec3_perm[j4];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec3[i] = (fSlow3 + (fConst2 * fRec3[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j5 = 0; (j5 < 4); j5 = (j5 + 1)) {
				fRec3_perm[j5] = fRec3_tmp[(vsize + j5)];
			}
			/* Recursive loop 104 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j148 = 0; (j148 < 4); j148 = (j148 + 1)) {
				fRec78_tmp[j148] = fRec78_perm[j148];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j150 = 0; (j150 < 4); j150 = (j150 + 1)) {
				fRec79_tmp[j150] = fRec79_perm[j150];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j152 = 0; (j152 < 4); j152 = (j152 + 1)) {
				fRec80_tmp[j152] = fRec80_perm[j152];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j154 = 0; (j154 < 4); j154 = (j154 + 1)) {
				fRec81_tmp[j154] = fRec81_perm[j154];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen96 = (((fRec79[(i - 1)] == 1.0f) & (fZec167[i] != fRec81[(i - 1)])) ? fConst18 : 0.0f);
				float fElse97 = fRec78[(i - 1)];
				float fThen98 = (((fRec79[(i - 1)] == 0.0f) & (fZec167[i] != fRec80[(i - 1)])) ? fConst17 : fThen96);
				float fElse98 = (((fRec79[(i - 1)] > 0.0f) & (fRec79[(i - 1)] < 1.0f)) ? fElse97 : 0.0f);
				fZec168[i] = ((fRec78[(i - 1)] != 0.0f) ? fElse98 : fThen98);
				fRec78[i] = fZec168[i];
				fRec79[i] = std::max<float>(0.0f, std::min<float>(1.0f, (fRec79[(i - 1)] + fZec168[i])));
				float fThen99 = fRec80[(i - 1)];
				float fElse99 = fZec167[i];
				fRec80[i] = (((fRec79[(i - 1)] >= 1.0f) & (fRec81[(i - 1)] != fZec167[i])) ? fElse99 : fThen99);
				float fThen100 = fRec81[(i - 1)];
				float fElse100 = fZec167[i];
				fRec81[i] = (((fRec79[(i - 1)] <= 0.0f) & (fRec80[(i - 1)] != fZec167[i])) ? fElse100 : fThen100);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j149 = 0; (j149 < 4); j149 = (j149 + 1)) {
				fRec78_perm[j149] = fRec78_tmp[(vsize + j149)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j151 = 0; (j151 < 4); j151 = (j151 + 1)) {
				fRec79_perm[j151] = fRec79_tmp[(vsize + j151)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j153 = 0; (j153 < 4); j153 = (j153 + 1)) {
				fRec80_perm[j153] = fRec80_tmp[(vsize + j153)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j155 = 0; (j155 < 4); j155 = (j155 + 1)) {
				fRec81_perm[j155] = fRec81_tmp[(vsize + j155)];
			}
			/* Vectorizable loop 105 */
			/* Pre code */
			fYec12_idx = ((fYec12_idx + fYec12_idx_save) & 131071);
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec12[((i + fYec12_idx) & 131071)] = (float(input1[i]) * fZec74[i]);
			}
			/* Post code */
			fYec12_idx_save = vsize;
			/* Vectorizable loop 106 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec19[i] = (((iRec23[i] + -1) % iConst12) == 0);
			}
			/* Vectorizable loop 107 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec39[i] = Macalla_faustpower2_f(fRec27[i]);
			}
			/* Vectorizable loop 108 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec40[i] = ((fConst42 * fRec28[i]) + (fConst43 * fRec28[(i - 2)]));
			}
			/* Vectorizable loop 109 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec43[i] = (((fZec42[i] + -1.41421354f) / fZec41[i]) + 1.0f);
			}
			/* Vectorizable loop 110 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec45[i] = (1.0f - (1.0f / fZec44[i]));
			}
			/* Vectorizable loop 111 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec46[i] = (((fZec42[i] + 1.41421354f) / fZec41[i]) + 1.0f);
			}
			/* Vectorizable loop 112 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec50[i] = (((fZec49[i] + -1.41421354f) / fZec48[i]) + 1.0f);
			}
			/* Vectorizable loop 113 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec47[i] = (0.0f - (2.0f / fZec44[i]));
			}
			/* Vectorizable loop 114 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec51[i] = (1.0f - (1.0f / Macalla_faustpower2_f(fZec48[i])));
			}
			/* Vectorizable loop 115 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec52[i] = (((fZec49[i] + 1.41421354f) / fZec48[i]) + 1.0f);
			}
			/* Recursive loop 116 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j68 = 0; (j68 < 4); j68 = (j68 + 1)) {
				fRec38_tmp[j68] = fRec38_perm[j68];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec38[i] = (fSlow9 + (fConst2 * fRec38[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j69 = 0; (j69 < 4); j69 = (j69 + 1)) {
				fRec38_perm[j69] = fRec38_tmp[(vsize + j69)];
			}
			/* Vectorizable loop 117 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen37 = (fZec58[i] + -1.0f);
				float fElse37 = fZec58[i];
				float fThen38 = ((fZec56[i] >= 0.0f) ? fElse37 : fThen37);
				float fElse38 = fZec56[i];
				fZec59[i] = ((fZec56[i] == fZec58[i]) ? fElse38 : fThen38);
			}
			/* Vectorizable loop 118 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec66[i] = (fZec65[i] + (2.0f - fZec60[i]));
			}
			/* Vectorizable loop 119 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec63[i] = std::min<int>(iConst48, std::max<int>(0, iZec62[i]));
			}
			/* Vectorizable loop 120 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec67[i] = (fZec60[i] + (-1.0f - fZec65[i]));
			}
			/* Vectorizable loop 121 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec68[i] = std::min<int>(iConst48, std::max<int>(0, (iZec62[i] + 1)));
			}
			/* Vectorizable loop 122 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen12 = (fZec17[i] + -1.0f);
				float fElse12 = fZec17[i];
				float fThen13 = ((fZec15[i] >= 0.0f) ? fElse12 : fThen12);
				float fElse13 = fZec15[i];
				fZec18[i] = ((fZec15[i] == fZec17[i]) ? fElse13 : fThen13);
			}
			/* Recursive loop 123 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j84 = 0; (j84 < 4); j84 = (j84 + 1)) {
				fRec41_tmp[j84] = fRec41_perm[j84];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j86 = 0; (j86 < 4); j86 = (j86 + 1)) {
				fRec42_tmp[j86] = fRec42_perm[j86];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j88 = 0; (j88 < 4); j88 = (j88 + 1)) {
				fRec43_tmp[j88] = fRec43_perm[j88];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j90 = 0; (j90 < 4); j90 = (j90 + 1)) {
				fRec44_tmp[j90] = fRec44_perm[j90];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen45 = (((fRec42[(i - 1)] == 1.0f) & (fZec72[i] != fRec44[(i - 1)])) ? fConst18 : 0.0f);
				float fElse46 = fRec41[(i - 1)];
				float fThen47 = (((fRec42[(i - 1)] == 0.0f) & (fZec72[i] != fRec43[(i - 1)])) ? fConst17 : fThen45);
				float fElse47 = (((fRec42[(i - 1)] > 0.0f) & (fRec42[(i - 1)] < 1.0f)) ? fElse46 : 0.0f);
				fZec73[i] = ((fRec41[(i - 1)] != 0.0f) ? fElse47 : fThen47);
				fRec41[i] = fZec73[i];
				fRec42[i] = std::max<float>(0.0f, std::min<float>(1.0f, (fRec42[(i - 1)] + fZec73[i])));
				float fThen48 = fRec43[(i - 1)];
				float fElse48 = fZec72[i];
				fRec43[i] = (((fRec42[(i - 1)] >= 1.0f) & (fRec44[(i - 1)] != fZec72[i])) ? fElse48 : fThen48);
				float fThen49 = fRec44[(i - 1)];
				float fElse49 = fZec72[i];
				fRec44[i] = (((fRec42[(i - 1)] <= 0.0f) & (fRec43[(i - 1)] != fZec72[i])) ? fElse49 : fThen49);
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j85 = 0; (j85 < 4); j85 = (j85 + 1)) {
				fRec41_perm[j85] = fRec41_tmp[(vsize + j85)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j87 = 0; (j87 < 4); j87 = (j87 + 1)) {
				fRec42_perm[j87] = fRec42_tmp[(vsize + j87)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j89 = 0; (j89 < 4); j89 = (j89 + 1)) {
				fRec43_perm[j89] = fRec43_tmp[(vsize + j89)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j91 = 0; (j91 < 4); j91 = (j91 + 1)) {
				fRec44_perm[j91] = fRec44_tmp[(vsize + j91)];
			}
			/* Vectorizable loop 124 */
			/* Pre code */
			fYec7_idx = ((fYec7_idx + fYec7_idx_save) & 131071);
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fYec7[((i + fYec7_idx) & 131071)] = (float(input0[i]) * fZec74[i]);
			}
			/* Post code */
			fYec7_idx_save = vsize;
			/* Vectorizable loop 125 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec76[i] = std::min<int>(65537, std::max<int>(0, iZec75[i]));
			}
			/* Vectorizable loop 126 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec79[i] = (fZec78[i] + (1.0f - fRec2[i]));
			}
			/* Vectorizable loop 127 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec80[i] = (fRec2[i] - fZec78[i]);
			}
			/* Vectorizable loop 128 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec81[i] = std::min<int>(65537, std::max<int>(0, (iZec75[i] + 1)));
			}
			/* Vectorizable loop 129 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec85[i] = std::min<int>(65537, std::max<int>(0, iZec84[i]));
			}
			/* Vectorizable loop 130 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec88[i] = (fZec87[i] + (-4095.0f - fRec2[i]));
			}
			/* Vectorizable loop 131 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec89[i] = (fRec2[i] + (4096.0f - fZec87[i]));
			}
			/* Vectorizable loop 132 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec90[i] = std::min<int>(65537, std::max<int>(0, (iZec84[i] + 1)));
			}
			/* Vectorizable loop 133 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec91[i] = (1.0f - fZec82[i]);
			}
			/* Vectorizable loop 134 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec93[i] = std::min<int>(65537, std::max<int>(0, iZec92[i]));
			}
			/* Vectorizable loop 135 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec96[i] = (fZec95[i] + (1.0f - fRec40[i]));
			}
			/* Vectorizable loop 136 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec97[i] = (fRec40[i] - fZec95[i]);
			}
			/* Vectorizable loop 137 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec98[i] = std::min<int>(65537, std::max<int>(0, (iZec92[i] + 1)));
			}
			/* Vectorizable loop 138 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec102[i] = std::min<int>(65537, std::max<int>(0, iZec101[i]));
			}
			/* Vectorizable loop 139 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec105[i] = (fZec104[i] + (-4095.0f - fRec40[i]));
			}
			/* Vectorizable loop 140 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec106[i] = (fRec40[i] + (4096.0f - fZec104[i]));
			}
			/* Vectorizable loop 141 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec107[i] = std::min<int>(65537, std::max<int>(0, (iZec101[i] + 1)));
			}
			/* Vectorizable loop 142 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec108[i] = (1.0f - fZec99[i]);
			}
			/* Vectorizable loop 143 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec110[i] = ((2984.51294f * (1.0f - fRec51[i])) + 314.159271f);
			}
			/* Vectorizable loop 144 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen77 = (fZec138[i] + -1.0f);
				float fElse77 = fZec138[i];
				float fThen78 = ((fZec136[i] >= 0.0f) ? fElse77 : fThen77);
				float fElse78 = fZec136[i];
				fZec139[i] = ((fZec136[i] == fZec138[i]) ? fElse78 : fThen78);
			}
			/* Vectorizable loop 145 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec163[i] = (fZec156[i] + (-1.0f - fZec161[i]));
			}
			/* Vectorizable loop 146 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec164[i] = std::min<int>(iConst48, std::max<int>(0, (iZec158[i] + 1)));
			}
			/* Recursive loop 147 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j26 = 0; (j26 < 4); j26 = (j26 + 1)) {
				fRec13_tmp[j26] = fRec13_perm[j26];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j30 = 0; (j30 < 4); j30 = (j30 + 1)) {
				fRec22_tmp[j30] = fRec22_perm[j30];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j32 = 0; (j32 < 4); j32 = (j32 + 1)) {
				fRec25_tmp[j32] = fRec25_perm[j32];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j34 = 0; (j34 < 4); j34 = (j34 + 1)) {
				fRec24_tmp[j34] = fRec24_perm[j34];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j36 = 0; (j36 < 4); j36 = (j36 + 1)) {
				fRec26_tmp[j36] = fRec26_perm[j36];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j62 = 0; (j62 < 4); j62 = (j62 + 1)) {
				fRec12_tmp[j62] = fRec12_perm[j62];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j66 = 0; (j66 < 4); j66 = (j66 + 1)) {
				fRec11_tmp[j66] = fRec11_perm[j66];
			}
			fYec4_idx = ((fYec4_idx + fYec4_idx_save) & 2047);
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j72 = 0; (j72 < 4); j72 = (j72 + 1)) {
				fRec9_tmp[j72] = fRec9_perm[j72];
			}
			fYec5_idx = ((fYec5_idx + fYec5_idx_save) & 4095);
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j74 = 0; (j74 < 4); j74 = (j74 + 1)) {
				fRec7_tmp[j74] = fRec7_perm[j74];
			}
			fYec6_idx = ((fYec6_idx + fYec6_idx_save) & 4095);
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j76 = 0; (j76 < 4); j76 = (j76 + 1)) {
				fRec4_tmp[j76] = fRec4_perm[j76];
			}
			fYec8_idx = ((fYec8_idx + fYec8_idx_save) & 1048575);
			fRec0_idx = ((fRec0_idx + fRec0_idx_save) & 262143);
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec13[i] = (((fRec0[(((i + fRec0_idx) - (int(std::min<float>(fConst0, float(std::max<int>(0, iZec16[i])))) + 1)) & 262143)] * (0.0f - (fZec14[i] + (99.0f - fZec18[i])))) + ((fZec14[i] + (100.0f - fZec18[i])) * fRec0[(((i + fRec0_idx) - (int(std::min<float>(fConst0, float(std::max<int>(0, (iZec16[i] + 1))))) + 1)) & 262143)])) - (fConst9 * ((fConst10 * fRec13[(i - 2)]) + (fConst11 * fRec13[(i - 1)]))));
				fZec20[i] = (fConst9 * (fRec13[(i - 2)] + (fRec13[i] + (2.0f * fRec13[(i - 1)]))));
				fZec21[i] = ((255.0f * std::fabs(fZec20[i])) + 0.5f);
				fZec22[i] = float(int(fZec21[i]));
				float fThen14 = (fZec22[i] + -1.0f);
				float fElse14 = fZec22[i];
				float fThen15 = ((fZec21[i] >= 0.0f) ? fElse14 : fThen14);
				float fElse15 = fZec21[i];
				float fThen16 = fRec22[(i - 1)];
				float fElse16 = (0.00392156886f * (((fZec21[i] == fZec22[i]) ? fElse15 : fThen15) * (float((2 * (fZec20[i] > 0.0f))) + -1.0f)));
				fRec22[i] = (iZec19[i] ? fElse16 : fThen16);
				float fThen17 = fZec20[i];
				float fElse17 = fRec22[i];
				fZec23[i] = (iSlow5 ? fElse17 : fThen17);
				fZec24[i] = std::fabs(fZec23[i]);
				fZec25[i] = ((fZec24[i] > fRec25[(i - 1)]) ? fConst14 : fConst13);
				fRec25[i] = ((fZec24[i] * (1.0f - fZec25[i])) + (fRec25[(i - 1)] * fZec25[i]));
				fRec24[i] = ((fConst16 * (0.0f - (0.75f * std::max<float>(((20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec25[i]))) + 6.0f), 0.0f)))) + (fConst15 * fRec24[(i - 1)]));
				fZec26[i] = (fZec23[i] * std::pow(10.0f, (0.0500000007f * fRec24[i])));
				fZec27[i] = std::fabs(fZec26[i]);
				fZec28[i] = ((fZec27[i] > fRec26[(i - 1)]) ? fConst19 : fConst13);
				fRec26[i] = ((fZec27[i] * (1.0f - fZec28[i])) + (fRec26[(i - 1)] * fZec28[i]));
				fRec12[i] = ((fZec26[i] + ((fRec26[i] * fZec39[i]) * fZec40[i])) - (((fRec12[(i - 2)] * fZec43[i]) + (2.0f * (fRec12[(i - 1)] * fZec45[i]))) / fZec46[i]));
				fRec11[i] = (((((fRec12[(i - 1)] * fZec47[i]) + (fRec12[i] / fZec44[i])) + (fRec12[(i - 2)] / fZec44[i])) / fZec46[i]) - (((fRec11[(i - 2)] * fZec50[i]) + (2.0f * (fRec11[(i - 1)] * fZec51[i]))) / fZec52[i]));
				fYec4[((i + fYec4_idx) & 2047)] = ((0.980000019f * ((fRec11[(i - 2)] + (fRec11[i] + (2.0f * fRec11[(i - 1)]))) / fZec52[i])) - (fRec38[i] * fRec9[(i - 1)]));
				fRec9[i] = ((fYec4[(((i + fYec4_idx) - std::min<int>(iConst48, std::max<int>(0, iZec57[i]))) & 2047)] * (fZec59[i] + (2.0f - fZec55[i]))) + ((fZec55[i] + (-1.0f - fZec59[i])) * fYec4[(((i + fYec4_idx) - std::min<int>(iConst48, std::max<int>(0, (iZec57[i] + 1)))) & 2047)]));
				fRec10[i] = (fRec38[i] * fYec4[((i + fYec4_idx) & 2047)]);
				fYec5[((i + fYec5_idx) & 4095)] = ((fRec10[i] + fRec9[(i - 1)]) - (0.899999976f * (fRec38[i] * fRec7[(i - 1)])));
				fRec7[i] = ((fYec5[(((i + fYec5_idx) - iZec63[i]) & 4095)] * fZec66[i]) + (fZec67[i] * fYec5[(((i + fYec5_idx) - iZec68[i]) & 4095)]));
				fRec8[i] = (0.899999976f * (fRec38[i] * fYec5[((i + fYec5_idx) & 4095)]));
				fYec6[((i + fYec6_idx) & 4095)] = ((fRec8[i] + fRec7[(i - 1)]) - (0.800000012f * (fRec38[i] * fRec4[(i - 1)])));
				fRec4[i] = ((fZec66[i] * fYec6[(((i + fYec6_idx) - iZec63[i]) & 4095)]) + (fZec67[i] * fYec6[(((i + fYec6_idx) - iZec68[i]) & 4095)]));
				fRec5[i] = (0.800000012f * (fRec38[i] * fYec6[((i + fYec6_idx) & 4095)]));
				float fThen54 = fYec7[((i + fYec7_idx) & 131071)];
				float fElse54 = ((((fYec7[(((i + fYec7_idx) - iZec76[i]) & 131071)] * fZec79[i]) + (fZec80[i] * fYec7[(((i + fYec7_idx) - iZec81[i]) & 131071)])) * fZec82[i]) + (((fYec7[(((i + fYec7_idx) - iZec85[i]) & 131071)] * fZec88[i]) + (fZec89[i] * fYec7[(((i + fYec7_idx) - iZec90[i]) & 131071)])) * fZec91[i]));
				float fThen59 = fYec7[((i + fYec7_idx) & 131071)];
				float fElse59 = ((((fYec7[(((i + fYec7_idx) - iZec93[i]) & 131071)] * fZec96[i]) + (fZec97[i] * fYec7[(((i + fYec7_idx) - iZec98[i]) & 131071)])) * fZec99[i]) + (((fYec7[(((i + fYec7_idx) - iZec102[i]) & 131071)] * fZec105[i]) + (fZec106[i] * fYec7[(((i + fYec7_idx) - iZec107[i]) & 131071)])) * fZec108[i]));
				fYec8[((i + fYec8_idx) & 1048575)] = ((iSlow15 ? fElse54 : fThen54) + ((fRec3[i] * (fRec5[i] + fRec4[(i - 1)])) + (iSlow16 ? fElse59 : fThen59)));
				fZec109[i] = fYec8[(((i + fYec8_idx) - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec43[i])))) & 1048575)];
				fRec0[((i + fRec0_idx) & 262143)] = (fZec109[i] + (fRec42[i] * (fYec8[(((i + fYec8_idx) - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec44[i])))) & 1048575)] - fZec109[i])));
			}
			/* Post code */
			fYec8_idx_save = vsize;
			fYec6_idx_save = vsize;
			fYec5_idx_save = vsize;
			fYec4_idx_save = vsize;
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j37 = 0; (j37 < 4); j37 = (j37 + 1)) {
				fRec26_perm[j37] = fRec26_tmp[(vsize + j37)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j33 = 0; (j33 < 4); j33 = (j33 + 1)) {
				fRec25_perm[j33] = fRec25_tmp[(vsize + j33)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j35 = 0; (j35 < 4); j35 = (j35 + 1)) {
				fRec24_perm[j35] = fRec24_tmp[(vsize + j35)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j31 = 0; (j31 < 4); j31 = (j31 + 1)) {
				fRec22_perm[j31] = fRec22_tmp[(vsize + j31)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j27 = 0; (j27 < 4); j27 = (j27 + 1)) {
				fRec13_perm[j27] = fRec13_tmp[(vsize + j27)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j63 = 0; (j63 < 4); j63 = (j63 + 1)) {
				fRec12_perm[j63] = fRec12_tmp[(vsize + j63)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j67 = 0; (j67 < 4); j67 = (j67 + 1)) {
				fRec11_perm[j67] = fRec11_tmp[(vsize + j67)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j73 = 0; (j73 < 4); j73 = (j73 + 1)) {
				fRec9_perm[j73] = fRec9_tmp[(vsize + j73)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j75 = 0; (j75 < 4); j75 = (j75 + 1)) {
				fRec7_perm[j75] = fRec7_tmp[(vsize + j75)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j77 = 0; (j77 < 4); j77 = (j77 + 1)) {
				fRec4_perm[j77] = fRec4_tmp[(vsize + j77)];
			}
			fRec0_idx_save = vsize;
			/* Vectorizable loop 148 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec113[i] = std::cos((fConst57 * fZec110[i]));
			}
			/* Recursive loop 149 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j124 = 0; (j124 < 4); j124 = (j124 + 1)) {
				fRec68_tmp[j124] = fRec68_perm[j124];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j126 = 0; (j126 < 4); j126 = (j126 + 1)) {
				fRec73_tmp[j126] = fRec73_perm[j126];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j128 = 0; (j128 < 4); j128 = (j128 + 1)) {
				fRec75_tmp[j128] = fRec75_perm[j128];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j130 = 0; (j130 < 4); j130 = (j130 + 1)) {
				fRec74_tmp[j130] = fRec74_perm[j130];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j132 = 0; (j132 < 4); j132 = (j132 + 1)) {
				fRec76_tmp[j132] = fRec76_perm[j132];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j134 = 0; (j134 < 4); j134 = (j134 + 1)) {
				fRec67_tmp[j134] = fRec67_perm[j134];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j136 = 0; (j136 < 4); j136 = (j136 + 1)) {
				fRec66_tmp[j136] = fRec66_perm[j136];
			}
			fYec9_idx = ((fYec9_idx + fYec9_idx_save) & 16383);
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j140 = 0; (j140 < 4); j140 = (j140 + 1)) {
				fRec64_tmp[j140] = fRec64_perm[j140];
			}
			fYec10_idx = ((fYec10_idx + fYec10_idx_save) & 16383);
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j142 = 0; (j142 < 4); j142 = (j142 + 1)) {
				fRec62_tmp[j142] = fRec62_perm[j142];
			}
			fYec11_idx = ((fYec11_idx + fYec11_idx_save) & 16383);
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j144 = 0; (j144 < 4); j144 = (j144 + 1)) {
				fRec58_tmp[j144] = fRec58_perm[j144];
			}
			fYec13_idx = ((fYec13_idx + fYec13_idx_save) & 1048575);
			fRec57_idx = ((fRec57_idx + fRec57_idx_save) & 262143);
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec68[i] = (((fRec57[(((i + fRec57_idx) - (int(std::min<float>(fConst0, float(std::max<int>(0, iZec137[i])))) + 1)) & 262143)] * (0.0f - (fZec135[i] + (99.0f - fZec139[i])))) + ((fZec135[i] + (100.0f - fZec139[i])) * fRec57[(((i + fRec57_idx) - (int(std::min<float>(fConst0, float(std::max<int>(0, (iZec137[i] + 1))))) + 1)) & 262143)])) - (fConst9 * ((fConst10 * fRec68[(i - 2)]) + (fConst11 * fRec68[(i - 1)]))));
				fZec140[i] = (fConst9 * (fRec68[(i - 2)] + (fRec68[i] + (2.0f * fRec68[(i - 1)]))));
				fZec141[i] = ((255.0f * std::fabs(fZec140[i])) + 0.5f);
				fZec142[i] = float(int(fZec141[i]));
				float fThen79 = (fZec142[i] + -1.0f);
				float fElse79 = fZec142[i];
				float fThen80 = ((fZec141[i] >= 0.0f) ? fElse79 : fThen79);
				float fElse80 = fZec141[i];
				float fThen81 = fRec73[(i - 1)];
				float fElse81 = (0.00392156886f * (((fZec141[i] == fZec142[i]) ? fElse80 : fThen80) * (float((2 * (fZec140[i] > 0.0f))) + -1.0f)));
				fRec73[i] = (iZec19[i] ? fElse81 : fThen81);
				float fThen82 = fZec140[i];
				float fElse82 = fRec73[i];
				fZec143[i] = (iSlow5 ? fElse82 : fThen82);
				fZec144[i] = std::fabs(fZec143[i]);
				fZec145[i] = ((fZec144[i] > fRec75[(i - 1)]) ? fConst14 : fConst13);
				fRec75[i] = ((fZec144[i] * (1.0f - fZec145[i])) + (fRec75[(i - 1)] * fZec145[i]));
				fRec74[i] = ((fConst16 * (0.0f - (0.75f * std::max<float>(((20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec75[i]))) + 6.0f), 0.0f)))) + (fConst15 * fRec74[(i - 1)]));
				fZec146[i] = (fZec143[i] * std::pow(10.0f, (0.0500000007f * fRec74[i])));
				fZec147[i] = std::fabs(fZec146[i]);
				fZec148[i] = ((fZec147[i] > fRec76[(i - 1)]) ? fConst19 : fConst13);
				fRec76[i] = ((fZec147[i] * (1.0f - fZec148[i])) + (fRec76[(i - 1)] * fZec148[i]));
				fRec67[i] = ((fZec146[i] + ((fZec39[i] * fRec76[i]) * fZec40[i])) - (((fZec43[i] * fRec67[(i - 2)]) + (2.0f * (fZec45[i] * fRec67[(i - 1)]))) / fZec46[i]));
				fRec66[i] = (((((fZec47[i] * fRec67[(i - 1)]) + (fRec67[i] / fZec44[i])) + (fRec67[(i - 2)] / fZec44[i])) / fZec46[i]) - (((fZec50[i] * fRec66[(i - 2)]) + (2.0f * (fZec51[i] * fRec66[(i - 1)]))) / fZec52[i]));
				fYec9[((i + fYec9_idx) & 16383)] = ((0.980000019f * ((fRec66[(i - 2)] + (fRec66[i] + (2.0f * fRec66[(i - 1)]))) / fZec52[i])) - (fRec38[i] * fRec64[(i - 1)]));
				fRec64[i] = ((fYec9[(((i + fYec9_idx) - std::min<int>(iConst48, std::max<int>(0, iZec153[i]))) & 16383)] * (fZec155[i] + (2.0f - fZec151[i]))) + ((fZec151[i] + (-1.0f - fZec155[i])) * fYec9[(((i + fYec9_idx) - std::min<int>(iConst48, std::max<int>(0, (iZec153[i] + 1)))) & 16383)]));
				fRec65[i] = (fRec38[i] * fYec9[((i + fYec9_idx) & 16383)]);
				fYec10[((i + fYec10_idx) & 16383)] = ((fRec65[i] + fRec64[(i - 1)]) - (0.899999976f * (fRec38[i] * fRec62[(i - 1)])));
				fRec62[i] = ((fYec10[(((i + fYec10_idx) - iZec159[i]) & 16383)] * fZec162[i]) + (fZec163[i] * fYec10[(((i + fYec10_idx) - iZec164[i]) & 16383)]));
				fRec63[i] = (0.899999976f * (fRec38[i] * fYec10[((i + fYec10_idx) & 16383)]));
				fYec11[((i + fYec11_idx) & 16383)] = ((fRec63[i] + fRec62[(i - 1)]) - (0.800000012f * (fRec38[i] * fRec58[(i - 1)])));
				fRec58[i] = ((fZec162[i] * fYec11[(((i + fYec11_idx) - iZec159[i]) & 16383)]) + (fZec163[i] * fYec11[(((i + fYec11_idx) - iZec164[i]) & 16383)]));
				fRec59[i] = (0.800000012f * (fRec38[i] * fYec11[((i + fYec11_idx) & 16383)]));
				float fThen101 = fYec12[((i + fYec12_idx) & 131071)];
				float fElse101 = ((fZec82[i] * ((fZec79[i] * fYec12[(((i + fYec12_idx) - iZec76[i]) & 131071)]) + (fZec80[i] * fYec12[(((i + fYec12_idx) - iZec81[i]) & 131071)]))) + (fZec91[i] * ((fZec88[i] * fYec12[(((i + fYec12_idx) - iZec85[i]) & 131071)]) + (fZec89[i] * fYec12[(((i + fYec12_idx) - iZec90[i]) & 131071)]))));
				float fThen102 = fYec12[((i + fYec12_idx) & 131071)];
				float fElse102 = ((fZec99[i] * ((fZec96[i] * fYec12[(((i + fYec12_idx) - iZec93[i]) & 131071)]) + (fZec97[i] * fYec12[(((i + fYec12_idx) - iZec98[i]) & 131071)]))) + (fZec108[i] * ((fZec105[i] * fYec12[(((i + fYec12_idx) - iZec102[i]) & 131071)]) + (fZec106[i] * fYec12[(((i + fYec12_idx) - iZec107[i]) & 131071)]))));
				fYec13[((i + fYec13_idx) & 1048575)] = ((iSlow15 ? fElse101 : fThen101) + ((fRec3[i] * (fRec59[i] + fRec58[(i - 1)])) + (iSlow16 ? fElse102 : fThen102)));
				fZec169[i] = fYec13[(((i + fYec13_idx) - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec80[i])))) & 1048575)];
				fRec57[((i + fRec57_idx) & 262143)] = (fZec169[i] + (fRec79[i] * (fYec13[(((i + fYec13_idx) - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec81[i])))) & 1048575)] - fZec169[i])));
			}
			/* Post code */
			fYec13_idx_save = vsize;
			fYec11_idx_save = vsize;
			fYec10_idx_save = vsize;
			fYec9_idx_save = vsize;
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j133 = 0; (j133 < 4); j133 = (j133 + 1)) {
				fRec76_perm[j133] = fRec76_tmp[(vsize + j133)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j129 = 0; (j129 < 4); j129 = (j129 + 1)) {
				fRec75_perm[j129] = fRec75_tmp[(vsize + j129)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j131 = 0; (j131 < 4); j131 = (j131 + 1)) {
				fRec74_perm[j131] = fRec74_tmp[(vsize + j131)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j127 = 0; (j127 < 4); j127 = (j127 + 1)) {
				fRec73_perm[j127] = fRec73_tmp[(vsize + j127)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j125 = 0; (j125 < 4); j125 = (j125 + 1)) {
				fRec68_perm[j125] = fRec68_tmp[(vsize + j125)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j135 = 0; (j135 < 4); j135 = (j135 + 1)) {
				fRec67_perm[j135] = fRec67_tmp[(vsize + j135)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j137 = 0; (j137 < 4); j137 = (j137 + 1)) {
				fRec66_perm[j137] = fRec66_tmp[(vsize + j137)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j141 = 0; (j141 < 4); j141 = (j141 + 1)) {
				fRec64_perm[j141] = fRec64_tmp[(vsize + j141)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j143 = 0; (j143 < 4); j143 = (j143 + 1)) {
				fRec62_perm[j143] = fRec62_tmp[(vsize + j143)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j145 = 0; (j145 < 4); j145 = (j145 + 1)) {
				fRec58_perm[j145] = fRec58_tmp[(vsize + j145)];
			}
			fRec57_idx_save = vsize;
			/* Recursive loop 150 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j92 = 0; (j92 < 4); j92 = (j92 + 1)) {
				fRec47_tmp[j92] = fRec47_perm[j92];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec47[i] = (fSlow17 + (fConst2 * fRec47[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j93 = 0; (j93 < 4); j93 = (j93 + 1)) {
				fRec47_perm[j93] = fRec47_tmp[(vsize + j93)];
			}
			/* Vectorizable loop 151 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec111[i] = std::cos((fConst55 * fZec110[i]));
			}
			/* Vectorizable loop 152 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec115[i] = (1.0f - (0.5f * fRec47[i]));
			}
			/* Recursive loop 153 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j98 = 0; (j98 < 4); j98 = (j98 + 1)) {
				fRec50_tmp[j98] = fRec50_perm[j98];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j100 = 0; (j100 < 4); j100 = (j100 + 1)) {
				fRec49_tmp[j100] = fRec49_perm[j100];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j102 = 0; (j102 < 4); j102 = (j102 + 1)) {
				fRec48_tmp[j102] = fRec48_perm[j102];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec112[i] = (fRec50[(i - 1)] * fZec111[i]);
				fRec50[i] = ((fRec0[((i + fRec0_idx) & 262143)] + (0.230000004f * fRec48[(i - 1)])) - ((fConst54 * fZec112[i]) + (fConst56 * fRec50[(i - 2)])));
				fZec114[i] = (fRec49[(i - 1)] * fZec113[i]);
				fRec49[i] = ((fConst56 * (fRec50[i] - fRec49[(i - 2)])) + (fRec50[(i - 2)] + (fConst54 * (fZec112[i] - fZec114[i]))));
				fRec48[i] = ((fConst56 * fRec49[i]) + ((fConst54 * fZec114[i]) + fRec49[(i - 2)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j99 = 0; (j99 < 4); j99 = (j99 + 1)) {
				fRec50_perm[j99] = fRec50_tmp[(vsize + j99)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j101 = 0; (j101 < 4); j101 = (j101 + 1)) {
				fRec49_perm[j101] = fRec49_tmp[(vsize + j101)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j103 = 0; (j103 < 4); j103 = (j103 + 1)) {
				fRec48_perm[j103] = fRec48_tmp[(vsize + j103)];
			}
			/* Recursive loop 154 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j156 = 0; (j156 < 4); j156 = (j156 + 1)) {
				fRec85_tmp[j156] = fRec85_perm[j156];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j158 = 0; (j158 < 4); j158 = (j158 + 1)) {
				fRec84_tmp[j158] = fRec84_perm[j158];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j160 = 0; (j160 < 4); j160 = (j160 + 1)) {
				fRec83_tmp[j160] = fRec83_perm[j160];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec170[i] = (fZec111[i] * fRec85[(i - 1)]);
				fRec85[i] = ((fRec57[((i + fRec57_idx) & 262143)] + (0.230000004f * fRec83[(i - 1)])) - ((fConst54 * fZec170[i]) + (fConst56 * fRec85[(i - 2)])));
				fZec171[i] = (fZec113[i] * fRec84[(i - 1)]);
				fRec84[i] = ((fConst56 * (fRec85[i] - fRec84[(i - 2)])) + (fRec85[(i - 2)] + (fConst54 * (fZec170[i] - fZec171[i]))));
				fRec83[i] = ((fConst56 * fRec84[i]) + ((fConst54 * fZec171[i]) + fRec84[(i - 2)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j157 = 0; (j157 < 4); j157 = (j157 + 1)) {
				fRec85_perm[j157] = fRec85_tmp[(vsize + j157)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j159 = 0; (j159 < 4); j159 = (j159 + 1)) {
				fRec84_perm[j159] = fRec84_tmp[(vsize + j159)];
			}
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j161 = 0; (j161 < 4); j161 = (j161 + 1)) {
				fRec83_perm[j161] = fRec83_tmp[(vsize + j161)];
			}
			/* Vectorizable loop 155 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec116[i] = ((fRec0[((i + fRec0_idx) & 262143)] * fZec115[i]) + (0.5f * (fRec47[i] * fRec48[i])));
			}
			/* Vectorizable loop 156 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec119[i] = std::fabs(float(input0[i]));
			}
			/* Recursive loop 157 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j110 = 0; (j110 < 4); j110 = (j110 + 1)) {
				fRec56_tmp[j110] = fRec56_perm[j110];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec56[i] = (fSlow24 + (fConst2 * fRec56[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j111 = 0; (j111 < 4); j111 = (j111 + 1)) {
				fRec56_perm[j111] = fRec56_tmp[(vsize + j111)];
			}
			/* Vectorizable loop 158 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec172[i] = ((fRec57[((i + fRec57_idx) & 262143)] * fZec115[i]) + (0.5f * (fRec47[i] * fRec83[i])));
			}
			/* Vectorizable loop 159 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec175[i] = std::fabs(float(input1[i]));
			}
			/* Vectorizable loop 160 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec117[i] = std::fabs(fZec116[i]);
			}
			/* Recursive loop 161 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j108 = 0; (j108 < 4); j108 = (j108 + 1)) {
				fRec55_tmp[j108] = fRec55_perm[j108];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec120[i] = ((fZec119[i] > fRec55[(i - 1)]) ? fSlow23 : fSlow20);
				fRec55[i] = ((fZec119[i] * (1.0f - fZec120[i])) + (fRec55[(i - 1)] * fZec120[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j109 = 0; (j109 < 4); j109 = (j109 + 1)) {
				fRec55_perm[j109] = fRec55_tmp[(vsize + j109)];
			}
			/* Vectorizable loop 162 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec122[i] = (50.0f * fRec56[i]);
			}
			/* Vectorizable loop 163 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec173[i] = std::fabs(fZec172[i]);
			}
			/* Recursive loop 164 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j166 = 0; (j166 < 4); j166 = (j166 + 1)) {
				fRec88_tmp[j166] = fRec88_perm[j166];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec176[i] = ((fZec175[i] > fRec88[(i - 1)]) ? fSlow23 : fSlow20);
				fRec88[i] = ((fZec175[i] * (1.0f - fZec176[i])) + (fRec88[(i - 1)] * fZec176[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j167 = 0; (j167 < 4); j167 = (j167 + 1)) {
				fRec88_perm[j167] = fRec88_tmp[(vsize + j167)];
			}
			/* Recursive loop 165 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j104 = 0; (j104 < 4); j104 = (j104 + 1)) {
				fRec54_tmp[j104] = fRec54_perm[j104];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec118[i] = ((fZec117[i] > fRec54[(i - 1)]) ? fConst14 : fConst13);
				fRec54[i] = ((fZec117[i] * (1.0f - fZec118[i])) + (fRec54[(i - 1)] * fZec118[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j105 = 0; (j105 < 4); j105 = (j105 + 1)) {
				fRec54_perm[j105] = fRec54_tmp[(vsize + j105)];
			}
			/* Vectorizable loop 166 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec121[i] = (20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec55[i])));
			}
			/* Vectorizable loop 167 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec123[i] = (-1.5f - fZec122[i]);
			}
			/* Vectorizable loop 168 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec124[i] = (1.5f - fZec122[i]);
			}
			/* Recursive loop 169 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j162 = 0; (j162 < 4); j162 = (j162 + 1)) {
				fRec87_tmp[j162] = fRec87_perm[j162];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec174[i] = ((fZec173[i] > fRec87[(i - 1)]) ? fConst14 : fConst13);
				fRec87[i] = ((fZec173[i] * (1.0f - fZec174[i])) + (fRec87[(i - 1)] * fZec174[i]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j163 = 0; (j163 < 4); j163 = (j163 + 1)) {
				fRec87_perm[j163] = fRec87_tmp[(vsize + j163)];
			}
			/* Vectorizable loop 170 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec177[i] = (20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec88[i])));
			}
			/* Recursive loop 171 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j106 = 0; (j106 < 4); j106 = (j106 + 1)) {
				fRec53_tmp[j106] = fRec53_perm[j106];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec53[i] = ((fConst16 * (0.0f - (0.75f * std::max<float>(((20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec54[i]))) + 6.0f), 0.0f)))) + (fConst15 * fRec53[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j107 = 0; (j107 < 4); j107 = (j107 + 1)) {
				fRec53_perm[j107] = fRec53_tmp[(vsize + j107)];
			}
			/* Vectorizable loop 172 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec125[i] = ((fZec121[i] > fZec123[i]) + (fZec121[i] > fZec124[i]));
			}
			/* Vectorizable loop 173 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec126[i] = (fZec121[i] + fZec122[i]);
			}
			/* Vectorizable loop 174 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec127[i] = (1.0f - (1.0f / ((19.0f * fRec56[i]) + 1.0f)));
			}
			/* Recursive loop 175 */
			/* Pre code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j164 = 0; (j164 < 4); j164 = (j164 + 1)) {
				fRec86_tmp[j164] = fRec86_perm[j164];
			}
			/* Compute code */
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fRec86[i] = ((fConst16 * (0.0f - (0.75f * std::max<float>(((20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec87[i]))) + 6.0f), 0.0f)))) + (fConst15 * fRec86[(i - 1)]));
			}
			/* Post code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j165 = 0; (j165 < 4); j165 = (j165 + 1)) {
				fRec86_perm[j165] = fRec86_tmp[(vsize + j165)];
			}
			/* Vectorizable loop 176 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				iZec178[i] = ((fZec177[i] > fZec123[i]) + (fZec177[i] > fZec124[i]));
			}
			/* Vectorizable loop 177 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				fZec179[i] = (fZec122[i] + fZec177[i]);
			}
			/* Vectorizable loop 178 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen66 = fZec126[i];
				float fElse66 = (0.166666672f * Macalla_faustpower2_f((fZec126[i] + 1.5f)));
				float fThen67 = ((iZec125[i] == 1) ? fElse66 : fThen66);
				output0[i] = FAUSTFLOAT(((fSlow26 * float(input0[i])) + (fSlow25 * ((fZec116[i] * std::pow(10.0f, (0.0500000007f * fRec53[i]))) * std::pow(10.0f, (0.0500000007f * (0.0f - (std::max<float>(0.0f, ((iZec125[i] == 0) ? 0.0f : fThen67)) * fZec127[i]))))))));
			}
			/* Vectorizable loop 179 */
			/* Compute code */
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int i = 0; (i < vsize); i = (i + 1)) {
				float fThen105 = fZec179[i];
				float fElse105 = (0.166666672f * Macalla_faustpower2_f((fZec179[i] + 1.5f)));
				float fThen106 = ((iZec178[i] == 1) ? fElse105 : fThen105);
				output1[i] = FAUSTFLOAT(((fSlow26 * float(input1[i])) + (fSlow25 * ((fZec172[i] * std::pow(10.0f, (0.0500000007f * fRec86[i]))) * std::pow(10.0f, (0.0500000007f * (0.0f - (fZec127[i] * std::max<float>(0.0f, ((iZec178[i] == 0) ? 0.0f : fThen106))))))))));
			}
		}
	}

};

  // END-FAUSTDSP

#endif
