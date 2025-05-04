/* ------------------------------------------------------------
name: "Macalla "
Code generated with Faust 2.38.8 (https://faust.grame.fr)
Compilation options: -a faustMinimal.h -lang cpp -es 1 -single -ftz 0
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
	
	int iVec3[2];
	int iRec13[2];
	
  public:
	
	int getNumInputsMacallaSIG0() {
		return 0;
	}
	int getNumOutputsMacallaSIG0() {
		return 1;
	}
	
	void instanceInitMacallaSIG0(int sample_rate) {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			iVec3[l10] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
			iRec13[l11] = 0;
		}
	}
	
	void fillMacallaSIG0(int count, float* table) {
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i1 = 0; (i1 < count); i1 = (i1 + 1)) {
			iVec3[0] = 1;
			iRec13[0] = ((iVec3[1] + iRec13[1]) % 65536);
			table[i1] = std::sin((9.58738019e-05f * float(iRec13[0])));
			iVec3[1] = iVec3[0];
			iRec13[1] = iRec13[0];
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
	
	FAUSTFLOAT fVslider0;
	int IOTA;
	float fVec0[131072];
	int iVec1[2];
	FAUSTFLOAT fHslider0;
	float fRec1[2];
	int fSampleRate;
	float fConst0;
	float fConst4;
	float fConst5;
	FAUSTFLOAT fVslider1;
	float fConst6;
	float fRec2[2];
	float fConst9;
	FAUSTFLOAT fHslider1;
	int iRec8[2];
	float fVec2[2];
	float fConst10;
	FAUSTFLOAT fHslider2;
	float fRec9[2];
	float fRec7[2];
	FAUSTFLOAT fHslider3;
	float fRec10[2];
	FAUSTFLOAT fHslider4;
	float fRec12[2];
	float fConst11;
	float fRec14[2];
	float fRec11[2];
	FAUSTFLOAT fHslider5;
	float fRec15[2];
	float fConst12;
	float fConst13;
	float fRec6[3];
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fRec17[2];
	float fRec16[2];
	float fConst18;
	float fConst19;
	float fConst20;
	float fRec18[2];
	FAUSTFLOAT fHslider6;
	float fRec19[2];
	float fConst28;
	float fRec21[4];
	float fRec22[2];
	float fVec4[2];
	float fRec23[2];
	float fConst29;
	float fConst30;
	float fConst31;
	float fRec25[2];
	float fVec5[2];
	float fVec6[2048];
	float fConst34;
	int iConst35;
	float fConst36;
	int iConst37;
	float fRec24[2];
	float fConst38;
	float fRec26[2];
	float fConst39;
	float fRec27[2];
	float fConst40;
	float fConst41;
	float fConst42;
	float fRec20[3];
	float fConst43;
	float fConst44;
	FAUSTFLOAT fHslider7;
	float fRec28[2];
	float fRec5[3];
	FAUSTFLOAT fHslider8;
	float fRec29[2];
	float fRec4[3];
	float fConst45;
	float fConst46;
	float fConst47;
	float fRec3[3];
	float fVec7[1048576];
	float fConst49;
	float fConst50;
	float fConst51;
	FAUSTFLOAT fHslider9;
	float fVec8[2];
	float fRec34[2];
	float fConst52;
	float fRec35[2];
	FAUSTFLOAT fVslider2;
	float fRec36[2];
	FAUSTFLOAT fVslider3;
	float fRec38[2];
	float fRec37[2];
	float fRec30[2];
	float fRec31[2];
	float fRec32[2];
	float fRec33[2];
	float fRec0[262144];
	FAUSTFLOAT fHslider10;
	float fRec39[2];
	float fRec40[2];
	float fRec42[2];
	float fRec41[2];
	FAUSTFLOAT fHslider11;
	FAUSTFLOAT fHslider12;
	FAUSTFLOAT fHslider13;
	float fRec43[2];
	FAUSTFLOAT fHslider14;
	float fVec9[131072];
	FAUSTFLOAT fHslider15;
	float fRec51[2];
	float fRec50[2];
	float fRec49[2];
	float fRec53[2];
	float fRec52[2];
	float fRec48[3];
	float fRec55[2];
	float fRec54[2];
	float fRec56[2];
	float fRec47[3];
	float fRec46[3];
	float fRec45[3];
	float fVec10[1048576];
	float fRec61[2];
	float fRec57[2];
	float fRec58[2];
	float fRec59[2];
	float fRec60[2];
	float fRec44[262144];
	float fRec62[2];
	float fRec64[2];
	float fRec63[2];
	float fRec65[2];
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/amp_follower_ar:author", "Jonatan Liljedahl, revised by Romain Michon");
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.2.0");
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/version", "1.18.0");
		m->declare("category", "Echo / Delay");
		m->declare("compile_options", "-a faustMinimal.h -lang cpp -es 1 -single -ftz 0");
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
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
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
		float fConst1 = std::tan((21991.1484f / fConst0));
		float fConst2 = (1.0f / fConst1);
		float fConst3 = (((fConst2 + 1.41421354f) / fConst1) + 1.0f);
		fConst4 = (0.959999979f / fConst3);
		fConst5 = (44.0999985f / fConst0);
		fConst6 = (1.0f - fConst5);
		float fConst7 = std::tan((25132.7422f / fConst0));
		float fConst8 = (1.0f / fConst7);
		fConst9 = (1.0f / (((fConst8 + 1.41421354f) / fConst7) + 1.0f));
		fConst10 = (1.0f / fConst0);
		fConst11 = (0.100000001f / fConst0);
		fConst12 = (((fConst8 + -1.41421354f) / fConst7) + 1.0f);
		fConst13 = (2.0f * (1.0f - (1.0f / Macalla_faustpower2_f(fConst7))));
		fConst14 = std::exp((0.0f - (2500.0f / fConst0)));
		fConst15 = (1.0f - fConst14);
		fConst16 = std::exp((0.0f - (2.0f / fConst0)));
		fConst17 = std::exp((0.0f - (1250.0f / fConst0)));
		fConst18 = (20.0f / fConst0);
		fConst19 = (0.0f - fConst18);
		fConst20 = std::exp(fConst19);
		float fConst21 = std::tan((31415.9258f / fConst0));
		float fConst22 = (fConst0 * fConst21);
		float fConst23 = Macalla_faustpower2_f(std::sqrt((4.0f * ((Macalla_faustpower2_f(fConst0) * fConst21) * std::tan((1570.79639f / fConst0))))));
		float fConst24 = ((2.0f * fConst22) - (0.5f * (fConst23 / fConst22)));
		float fConst25 = (Macalla_faustpower2_f(fConst10) * fConst23);
		float fConst26 = (2.0f * (fConst24 / fConst0));
		float fConst27 = ((fConst25 + fConst26) + 4.0f);
		fConst28 = (2.0f * (fConst24 / (fConst0 * fConst27)));
		fConst29 = (48.0f / fConst0);
		fConst30 = (0.00416666688f * fConst0);
		fConst31 = (60.0f / fConst0);
		float fConst32 = std::max<float>(0.0f, std::min<float>(2047.0f, (0.00833333377f * fConst0)));
		float fConst33 = std::floor(fConst32);
		fConst34 = (fConst33 + (1.0f - fConst32));
		iConst35 = int(fConst32);
		fConst36 = (fConst32 - fConst33);
		iConst37 = (iConst35 + 1);
		fConst38 = (528.0f / fConst0);
		fConst39 = (110.0f / fConst0);
		fConst40 = (1.0f / fConst27);
		fConst41 = ((2.0f * fConst25) + -8.0f);
		fConst42 = (fConst25 + (4.0f - fConst26));
		fConst43 = (0.0f - fConst28);
		fConst44 = (3.14159274f / fConst0);
		fConst45 = (1.0f / fConst3);
		fConst46 = (((fConst2 + -1.41421354f) / fConst1) + 1.0f);
		fConst47 = (2.0f * (1.0f - (1.0f / Macalla_faustpower2_f(fConst1))));
		float fConst48 = (1.0f / std::tan((219.911484f / fConst0)));
		fConst49 = (1.0f / (fConst48 + 1.0f));
		fConst50 = (1.0f - fConst48);
		fConst51 = (3.49245978e-16f * fConst0);
		fConst52 = (0.00100000005f * fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(50.0f);
		fHslider0 = FAUSTFLOAT(0.0f);
		fVslider1 = FAUSTFLOAT(0.5f);
		fHslider1 = FAUSTFLOAT(0.002f);
		fHslider2 = FAUSTFLOAT(0.5f);
		fHslider3 = FAUSTFLOAT(0.29999999999999999f);
		fHslider4 = FAUSTFLOAT(8.0f);
		fHslider5 = FAUSTFLOAT(0.10000000000000001f);
		fHslider6 = FAUSTFLOAT(0.0f);
		fHslider7 = FAUSTFLOAT(250.0f);
		fHslider8 = FAUSTFLOAT(10000.0f);
		fHslider9 = FAUSTFLOAT(300.0f);
		fVslider2 = FAUSTFLOAT(0.0f);
		fVslider3 = FAUSTFLOAT(0.5f);
		fHslider10 = FAUSTFLOAT(0.0f);
		fHslider11 = FAUSTFLOAT(4.0f);
		fHslider12 = FAUSTFLOAT(200.0f);
		fHslider13 = FAUSTFLOAT(10.0f);
		fHslider14 = FAUSTFLOAT(-30.0f);
		fHslider15 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		IOTA = 0;
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l0 = 0; (l0 < 131072); l0 = (l0 + 1)) {
			fVec0[l0] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			iVec1[l1] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec1[l2] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
			fRec2[l3] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			iRec8[l4] = 0;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
			fVec2[l5] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec9[l6] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l7 = 0; (l7 < 2); l7 = (l7 + 1)) {
			fRec7[l7] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec10[l8] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l9 = 0; (l9 < 2); l9 = (l9 + 1)) {
			fRec12[l9] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec14[l12] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l13 = 0; (l13 < 2); l13 = (l13 + 1)) {
			fRec11[l13] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec15[l14] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l15 = 0; (l15 < 3); l15 = (l15 + 1)) {
			fRec6[l15] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec17[l16] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l17 = 0; (l17 < 2); l17 = (l17 + 1)) {
			fRec16[l17] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec18[l18] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l19 = 0; (l19 < 2); l19 = (l19 + 1)) {
			fRec19[l19] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l20 = 0; (l20 < 4); l20 = (l20 + 1)) {
			fRec21[l20] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec22[l21] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l22 = 0; (l22 < 2); l22 = (l22 + 1)) {
			fVec4[l22] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l23 = 0; (l23 < 2); l23 = (l23 + 1)) {
			fRec23[l23] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l24 = 0; (l24 < 2); l24 = (l24 + 1)) {
			fRec25[l24] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l25 = 0; (l25 < 2); l25 = (l25 + 1)) {
			fVec5[l25] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l26 = 0; (l26 < 2048); l26 = (l26 + 1)) {
			fVec6[l26] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l27 = 0; (l27 < 2); l27 = (l27 + 1)) {
			fRec24[l27] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l28 = 0; (l28 < 2); l28 = (l28 + 1)) {
			fRec26[l28] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l29 = 0; (l29 < 2); l29 = (l29 + 1)) {
			fRec27[l29] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l30 = 0; (l30 < 3); l30 = (l30 + 1)) {
			fRec20[l30] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l31 = 0; (l31 < 2); l31 = (l31 + 1)) {
			fRec28[l31] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l32 = 0; (l32 < 3); l32 = (l32 + 1)) {
			fRec5[l32] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l33 = 0; (l33 < 2); l33 = (l33 + 1)) {
			fRec29[l33] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l34 = 0; (l34 < 3); l34 = (l34 + 1)) {
			fRec4[l34] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l35 = 0; (l35 < 3); l35 = (l35 + 1)) {
			fRec3[l35] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l36 = 0; (l36 < 1048576); l36 = (l36 + 1)) {
			fVec7[l36] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l37 = 0; (l37 < 2); l37 = (l37 + 1)) {
			fVec8[l37] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l38 = 0; (l38 < 2); l38 = (l38 + 1)) {
			fRec34[l38] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l39 = 0; (l39 < 2); l39 = (l39 + 1)) {
			fRec35[l39] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l40 = 0; (l40 < 2); l40 = (l40 + 1)) {
			fRec36[l40] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l41 = 0; (l41 < 2); l41 = (l41 + 1)) {
			fRec38[l41] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l42 = 0; (l42 < 2); l42 = (l42 + 1)) {
			fRec37[l42] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l43 = 0; (l43 < 2); l43 = (l43 + 1)) {
			fRec30[l43] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l44 = 0; (l44 < 2); l44 = (l44 + 1)) {
			fRec31[l44] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l45 = 0; (l45 < 2); l45 = (l45 + 1)) {
			fRec32[l45] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l46 = 0; (l46 < 2); l46 = (l46 + 1)) {
			fRec33[l46] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l47 = 0; (l47 < 262144); l47 = (l47 + 1)) {
			fRec0[l47] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l48 = 0; (l48 < 2); l48 = (l48 + 1)) {
			fRec39[l48] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l49 = 0; (l49 < 2); l49 = (l49 + 1)) {
			fRec40[l49] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l50 = 0; (l50 < 2); l50 = (l50 + 1)) {
			fRec42[l50] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l51 = 0; (l51 < 2); l51 = (l51 + 1)) {
			fRec41[l51] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l52 = 0; (l52 < 2); l52 = (l52 + 1)) {
			fRec43[l52] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l53 = 0; (l53 < 131072); l53 = (l53 + 1)) {
			fVec9[l53] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l54 = 0; (l54 < 2); l54 = (l54 + 1)) {
			fRec51[l54] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l55 = 0; (l55 < 2); l55 = (l55 + 1)) {
			fRec50[l55] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l56 = 0; (l56 < 2); l56 = (l56 + 1)) {
			fRec49[l56] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l57 = 0; (l57 < 2); l57 = (l57 + 1)) {
			fRec53[l57] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l58 = 0; (l58 < 2); l58 = (l58 + 1)) {
			fRec52[l58] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l59 = 0; (l59 < 3); l59 = (l59 + 1)) {
			fRec48[l59] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l60 = 0; (l60 < 2); l60 = (l60 + 1)) {
			fRec55[l60] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l61 = 0; (l61 < 2); l61 = (l61 + 1)) {
			fRec54[l61] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l62 = 0; (l62 < 2); l62 = (l62 + 1)) {
			fRec56[l62] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l63 = 0; (l63 < 3); l63 = (l63 + 1)) {
			fRec47[l63] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l64 = 0; (l64 < 3); l64 = (l64 + 1)) {
			fRec46[l64] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l65 = 0; (l65 < 3); l65 = (l65 + 1)) {
			fRec45[l65] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l66 = 0; (l66 < 1048576); l66 = (l66 + 1)) {
			fVec10[l66] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l67 = 0; (l67 < 2); l67 = (l67 + 1)) {
			fRec61[l67] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l68 = 0; (l68 < 2); l68 = (l68 + 1)) {
			fRec57[l68] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l69 = 0; (l69 < 2); l69 = (l69 + 1)) {
			fRec58[l69] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l70 = 0; (l70 < 2); l70 = (l70 + 1)) {
			fRec59[l70] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l71 = 0; (l71 < 2); l71 = (l71 + 1)) {
			fRec60[l71] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l72 = 0; (l72 < 262144); l72 = (l72 + 1)) {
			fRec44[l72] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l73 = 0; (l73 < 2); l73 = (l73 + 1)) {
			fRec62[l73] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l74 = 0; (l74 < 2); l74 = (l74 + 1)) {
			fRec64[l74] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l75 = 0; (l75 < 2); l75 = (l75 + 1)) {
			fRec63[l75] = 0.0f;
		}
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int l76 = 0; (l76 < 2); l76 = (l76 + 1)) {
			fRec65[l76] = 0.0f;
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
		ui_interface->addHorizontalSlider("attack", &fHslider13, FAUSTFLOAT(10.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(100.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("delay(ms)", &fHslider9, FAUSTFLOAT(300.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1000.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("ducking", &fHslider14, FAUSTFLOAT(-30.0f), FAUSTFLOAT(-60.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->addVerticalSlider("feedback", &fVslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("flutter", &fHslider4, FAUSTFLOAT(8.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(20.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("flutter_intensity", &fHslider5, FAUSTFLOAT(0.100000001f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addVerticalSlider("glitch_amount", &fVslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addVerticalSlider("glitch_rate", &fVslider3, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.00999999978f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("highpass", &fHslider7, FAUSTFLOAT(250.0f), FAUSTFLOAT(20.0f), FAUSTFLOAT(20000.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("lowpass", &fHslider8, FAUSTFLOAT(10000.0f), FAUSTFLOAT(20.0f), FAUSTFLOAT(20000.0f), FAUSTFLOAT(1.0f));
		ui_interface->addVerticalSlider("mix", &fVslider0, FAUSTFLOAT(50.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(100.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("noise_amount", &fHslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("output_gain", &fHslider10, FAUSTFLOAT(0.0f), FAUSTFLOAT(-96.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("pitch_shift", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(-12.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("random_mod", &fHslider1, FAUSTFLOAT(0.00200000009f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.00999999978f), FAUSTFLOAT(9.99999975e-05f));
		ui_interface->addHorizontalSlider("ratio", &fHslider11, FAUSTFLOAT(4.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(20.0f), FAUSTFLOAT(0.100000001f));
		ui_interface->addHorizontalSlider("release", &fHslider12, FAUSTFLOAT(200.0f), FAUSTFLOAT(50.0f), FAUSTFLOAT(1000.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("spread_amount", &fHslider15, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("wow", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->addHorizontalSlider("wow_intensity", &fHslider3, FAUSTFLOAT(0.300000012f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.00999999978f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = (0.00999999978f * float(fVslider0));
		float fSlow1 = (1.0f - fSlow0);
		float fSlow2 = std::pow(2.0f, (0.0833333358f * float(fHslider0)));
		float fSlow3 = (fConst5 * float(fVslider1));
		float fSlow4 = (4.65661287e-10f * float(fHslider1));
		float fSlow5 = float(fHslider2);
		float fSlow6 = (fConst10 * fSlow5);
		float fSlow7 = (9.99999975e-05f * float(fHslider3));
		float fSlow8 = float(fHslider4);
		float fSlow9 = (fConst10 * fSlow8);
		float fSlow10 = (9.99999975e-05f * float(fHslider5));
		float fSlow11 = (fConst5 * float(fHslider6));
		float fSlow12 = (fConst5 * float(fHslider7));
		float fSlow13 = (fConst5 * float(fHslider8));
		float fSlow14 = float(fHslider9);
		float fSlow15 = (fConst51 * fSlow14);
		float fSlow16 = (fConst52 * fSlow14);
		float fSlow17 = (0.00100000005f * float(fVslider2));
		float fSlow18 = (0.00100000005f * float(fVslider3));
		float fSlow19 = (fConst5 * float(fHslider10));
		float fSlow20 = (1.0f - (1.0f / float(fHslider11)));
		float fSlow21 = (0.00100000005f * float(fHslider12));
		int iSlow22 = (std::fabs(fSlow21) < 1.1920929e-07f);
		float fThen19 = std::exp((0.0f - (fConst10 / (iSlow22 ? 1.0f : fSlow21))));
		float fSlow23 = (iSlow22 ? 0.0f : fThen19);
		float fSlow24 = (0.00100000005f * float(fHslider13));
		int iSlow25 = (std::fabs(fSlow24) < 1.1920929e-07f);
		float fThen21 = std::exp((0.0f - (fConst10 / (iSlow25 ? 1.0f : fSlow24))));
		float fSlow26 = (iSlow25 ? 0.0f : fThen21);
		float fSlow27 = float(fHslider14);
		float fSlow28 = (fSlow27 + -1.5f);
		float fSlow29 = (fSlow27 + 1.5f);
		float fSlow30 = (fConst5 * float(fHslider15));
		#pragma clang loop vectorize(enable) interleave(enable)
		for (int i0 = 0; (i0 < count); i0 = (i0 + 1)) {
			float fTemp0 = float(input0[i0]);
			fVec0[(IOTA & 131071)] = fTemp0;
			iVec1[0] = 1;
			fRec1[0] = std::fmod(((fRec1[1] + 4097.0f) - fSlow2), 4096.0f);
			float fTemp1 = (fRec1[0] + 4096.0f);
			int iTemp2 = int(fTemp1);
			int iTemp3 = std::min<int>(65537, std::max<int>(0, iTemp2));
			float fTemp4 = std::floor(fTemp1);
			float fTemp5 = (fTemp4 + (-4095.0f - fRec1[0]));
			int iTemp6 = std::min<int>(65537, std::max<int>(0, (iTemp2 + 1)));
			float fTemp7 = (fRec1[0] + (4096.0f - fTemp4));
			float fTemp8 = std::min<float>((0.001953125f * fRec1[0]), 1.0f);
			float fTemp9 = (1.0f - fTemp8);
			fRec2[0] = (fSlow3 + (fConst6 * fRec2[1]));
			iRec8[0] = ((1103515245 * iRec8[1]) + 12345);
			float fTemp10 = float(iRec8[0]);
			fVec2[0] = fTemp10;
			float fTemp11 = (fSlow4 * fTemp10);
			int iTemp12 = (1 - iVec1[1]);
			float fThen0 = (fSlow6 + fRec9[1]);
			float fTemp13 = (iTemp12 ? 0.0f : fThen0);
			fRec9[0] = (fTemp13 - std::floor(fTemp13));
			fRec7[0] = ((9.99999975e-05f * (fTemp11 + (1.0f - std::fabs(((2.0f * fRec9[0]) + -1.0f))))) + (0.999899983f * fRec7[1]));
			fRec10[0] = (fSlow7 + (0.999899983f * fRec10[1]));
			float fThen1 = (fSlow9 + fRec12[1]);
			float fTemp14 = (iTemp12 ? 0.0f : fThen1);
			fRec12[0] = (fTemp14 - std::floor(fTemp14));
			float fThen2 = (fConst11 + fRec14[1]);
			float fTemp15 = (iTemp12 ? 0.0f : fThen2);
			fRec14[0] = (fTemp15 - std::floor(fTemp15));
			float fTemp16 = ((0.200000003f * ftbl0MacallaSIG0[int((65536.0f * fRec14[0]))]) + 0.800000012f);
			fRec11[0] = ((9.99999975e-05f * ((fTemp11 + (1.0f - std::fabs(((2.0f * fRec12[0]) + -1.0f)))) * fTemp16)) + (0.999899983f * fRec11[1]));
			fRec15[0] = (fSlow10 + (0.999899983f * fRec15[1]));
			float fTemp17 = ((1000.0f * (fRec7[0] * fRec10[0])) + (200.0f * (fRec11[0] * fRec15[0])));
			float fTemp18 = (fTemp17 + 100.000008f);
			int iTemp19 = int(fTemp18);
			float fTemp20 = std::floor(fTemp18);
			fRec6[0] = (((fRec0[((IOTA - (int(std::min<float>(fConst0, float(std::max<int>(0, iTemp19)))) + 1)) & 262143)] * (0.0f - (fTemp17 + (99.0f - fTemp20)))) + ((fTemp17 + (100.0f - fTemp20)) * fRec0[((IOTA - (int(std::min<float>(fConst0, float(std::max<int>(0, (iTemp19 + 1))))) + 1)) & 262143)])) - (fConst9 * ((fConst12 * fRec6[2]) + (fConst13 * fRec6[1]))));
			float fTemp21 = (fRec6[2] + (fRec6[0] + (2.0f * fRec6[1])));
			float fTemp22 = std::fabs((fConst9 * fTemp21));
			float fTemp23 = ((fTemp22 > fRec17[1]) ? fConst17 : fConst16);
			fRec17[0] = ((fTemp22 * (1.0f - fTemp23)) + (fRec17[1] * fTemp23));
			fRec16[0] = ((fConst15 * (0.0f - (0.75f * std::max<float>(((20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec17[0]))) + 6.0f), 0.0f)))) + (fConst14 * fRec16[1]));
			float fTemp24 = (fConst9 * (fTemp21 * std::pow(10.0f, (0.0500000007f * fRec16[0]))));
			float fTemp25 = std::fabs((fTemp24 + 9.99999975e-05f));
			float fTemp26 = ((fTemp25 > fRec18[1]) ? fConst20 : fConst16);
			fRec18[0] = ((fTemp25 * (1.0f - fTemp26)) + (fRec18[1] * fTemp26));
			fRec19[0] = (fSlow11 + (fConst6 * fRec19[1]));
			fRec21[0] = (((0.522189379f * fRec21[3]) + ((4.65661287e-10f * fTemp10) + (2.49495602f * fRec21[1]))) - (2.0172658f * fRec21[2]));
			float fThen5 = (fConst18 + fRec22[1]);
			float fTemp27 = (iTemp12 ? 0.0f : fThen5);
			fRec22[0] = (fTemp27 - std::floor(fTemp27));
			float fTemp28 = (fRec22[0] - fRec22[1]);
			fVec4[0] = fTemp28;
			int iTemp29 = ((fVec4[1] <= 0.0f) & (fTemp28 > 0.0f));
			fRec23[0] = ((fRec23[1] * float((1 - iTemp29))) + (4.65661287e-10f * (fTemp10 * float(iTemp29))));
			float fTemp30 = (0.5f * (fRec23[0] + 1.0f));
			float fThen6 = (fConst31 + fRec25[1]);
			float fTemp31 = (iTemp12 ? 0.0f : fThen6);
			fRec25[0] = (fTemp31 - std::floor(fTemp31));
			float fTemp32 = Macalla_faustpower2_f(((2.0f * fRec25[0]) + -1.0f));
			fVec5[0] = fTemp32;
			float fTemp33 = (float(iVec1[1]) * (fTemp32 - fVec5[1]));
			fVec6[(IOTA & 2047)] = fTemp33;
			fRec24[0] = ((0.999000013f * fRec24[1]) + (fConst30 * ((fTemp33 - (fConst34 * fVec6[((IOTA - iConst35) & 2047)])) - (fConst36 * fVec6[((IOTA - iConst37) & 2047)]))));
			float fThen7 = (fConst38 + fRec26[1]);
			float fTemp34 = (iTemp12 ? 0.0f : fThen7);
			fRec26[0] = (fTemp34 - std::floor(fTemp34));
			float fThen8 = (fConst39 + fRec27[1]);
			float fTemp35 = (iTemp12 ? 0.0f : fThen8);
			fRec27[0] = (fTemp35 - std::floor(fTemp35));
			fRec20[0] = (((((1.5f * (((0.0499220341f * fRec21[0]) + (0.0506126992f * fRec21[2])) - ((0.0959935337f * fRec21[1]) + (0.00440878607f * fRec21[3])))) + (2.32830644e-10f * (fVec2[1] * float(((fRec22[0] >= fTemp30) * (fRec22[1] < fTemp30)))))) + (fConst29 * fRec24[0])) + (0.00999999978f * (ftbl0MacallaSIG0[int((65536.0f * fRec26[0]))] + ftbl0MacallaSIG0[int((65536.0f * fRec27[0]))]))) - (fConst40 * ((fConst41 * fRec20[1]) + (fConst42 * fRec20[2]))));
			float fTemp36 = ((fConst28 * fRec20[0]) + (fConst43 * fRec20[2]));
			fRec28[0] = (fSlow12 + (fConst6 * fRec28[1]));
			float fTemp37 = std::tan((fConst44 * fRec28[0]));
			float fTemp38 = (1.0f / fTemp37);
			float fTemp39 = (((fTemp38 + -1.41421354f) / fTemp37) + 1.0f);
			float fTemp40 = Macalla_faustpower2_f(fTemp37);
			float fTemp41 = (1.0f - (1.0f / fTemp40));
			float fTemp42 = (((fTemp38 + 1.41421354f) / fTemp37) + 1.0f);
			fRec5[0] = ((fTemp24 + (1.5f * ((fRec18[0] * fRec19[0]) * fTemp36))) - (((fRec5[2] * fTemp39) + (2.0f * (fRec5[1] * fTemp41))) / fTemp42));
			float fTemp43 = (0.0f - (2.0f / fTemp40));
			fRec29[0] = (fSlow13 + (fConst6 * fRec29[1]));
			float fTemp44 = std::tan((fConst44 * fRec29[0]));
			float fTemp45 = (1.0f / fTemp44);
			float fTemp46 = (((fTemp45 + -1.41421354f) / fTemp44) + 1.0f);
			float fTemp47 = (1.0f - (1.0f / Macalla_faustpower2_f(fTemp44)));
			float fTemp48 = (((fTemp45 + 1.41421354f) / fTemp44) + 1.0f);
			fRec4[0] = (((((fRec5[1] * fTemp43) + (fRec5[0] / fTemp40)) + (fRec5[2] / fTemp40)) / fTemp42) - (((fRec4[2] * fTemp46) + (2.0f * (fRec4[1] * fTemp47))) / fTemp48));
			fRec3[0] = (((fRec4[2] + (fRec4[0] + (2.0f * fRec4[1]))) / fTemp48) - (fConst45 * ((fConst46 * fRec3[2]) + (fConst47 * fRec3[1]))));
			int iTemp49 = int(fRec1[0]);
			int iTemp50 = std::min<int>(65537, std::max<int>(0, iTemp49));
			float fTemp51 = std::floor(fRec1[0]);
			float fTemp52 = (fTemp51 + (1.0f - fRec1[0]));
			float fTemp53 = (fRec1[0] - fTemp51);
			int iTemp54 = std::min<int>(65537, std::max<int>(0, (iTemp49 + 1)));
			float fTemp55 = ((((fVec0[((IOTA - iTemp3) & 131071)] * fTemp5) + (fVec0[((IOTA - iTemp6) & 131071)] * fTemp7)) * fTemp9) + ((fConst4 * (fRec2[0] * (fRec3[2] + (fRec3[0] + (2.0f * fRec3[1]))))) + (((fVec0[((IOTA - iTemp50) & 131071)] * fTemp52) + (fTemp53 * fVec0[((IOTA - iTemp54) & 131071)])) * fTemp8)));
			fVec7[(IOTA & 1048575)] = fTemp55;
			float fTemp56 = (fSlow15 * (fRec19[0] * fTemp10));
			fVec8[0] = fTemp56;
			fRec34[0] = (0.0f - (fConst49 * ((fConst50 * fRec34[1]) - (fTemp56 + fVec8[1]))));
			fRec35[0] = (fSlow16 + (0.998000026f * fRec35[1]));
			float fTemp57 = (fRec34[0] + (0.00200000009f * fRec35[0]));
			fRec36[0] = (fSlow17 + (0.999000013f * fRec36[1]));
			fRec38[0] = (fSlow18 + (0.999000013f * fRec38[1]));
			float fThen9 = (fRec37[1] + (fConst10 * fRec38[0]));
			float fTemp58 = (iTemp12 ? 0.0f : fThen9);
			fRec37[0] = (fTemp58 - std::floor(fTemp58));
			float fTemp59 = std::min<float>(524288.0f, std::max<float>(0.0f, (fTemp57 + (fConst0 * (fRec36[0] * std::fabs(((2.0f * (1.0f - std::fabs(((2.0f * fRec37[0]) + -1.0f)))) + -1.0f)))))));
			float fThen11 = (((fRec31[1] == 1.0f) & (fTemp59 != fRec33[1])) ? fConst19 : 0.0f);
			float fThen13 = (((fRec31[1] == 0.0f) & (fTemp59 != fRec32[1])) ? fConst18 : fThen11);
			float fElse13 = (((fRec31[1] > 0.0f) & (fRec31[1] < 1.0f)) ? fRec30[1] : 0.0f);
			float fTemp60 = ((fRec30[1] != 0.0f) ? fElse13 : fThen13);
			fRec30[0] = fTemp60;
			fRec31[0] = std::max<float>(0.0f, std::min<float>(1.0f, (fRec31[1] + fTemp60)));
			fRec32[0] = (((fRec31[1] >= 1.0f) & (fRec33[1] != fTemp59)) ? fTemp59 : fRec32[1]);
			fRec33[0] = (((fRec31[1] <= 0.0f) & (fRec32[1] != fTemp59)) ? fTemp59 : fRec33[1]);
			float fTemp61 = fVec7[((IOTA - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec32[0])))) & 1048575)];
			fRec0[(IOTA & 262143)] = (fTemp61 + (fRec31[0] * (fVec7[((IOTA - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec33[0])))) & 1048575)] - fTemp61)));
			fRec39[0] = (fSlow19 + (fConst6 * fRec39[1]));
			float fTemp62 = std::pow(10.0f, (0.0500000007f * fRec39[0]));
			float fTemp63 = (fRec0[((IOTA - 0) & 262143)] * fTemp62);
			float fTemp64 = std::fabs((fTemp63 + 9.99999975e-05f));
			float fTemp65 = ((fTemp64 > fRec40[1]) ? fConst20 : fConst16);
			fRec40[0] = ((fTemp64 * (1.0f - fTemp65)) + (fRec40[1] * fTemp65));
			float fTemp66 = (fTemp63 + (1.5f * ((fRec19[0] * fRec40[0]) * fTemp36)));
			float fTemp67 = std::fabs(fTemp66);
			float fTemp68 = ((fTemp67 > fRec42[1]) ? fConst17 : fConst16);
			fRec42[0] = ((fTemp67 * (1.0f - fTemp68)) + (fRec42[1] * fTemp68));
			fRec41[0] = ((fConst15 * (0.0f - (0.75f * std::max<float>(((20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec42[0]))) + 6.0f), 0.0f)))) + (fConst14 * fRec41[1]));
			float fTemp69 = std::fabs(fTemp0);
			float fTemp70 = ((fTemp69 > fRec43[1]) ? fSlow26 : fSlow23);
			fRec43[0] = ((fTemp69 * (1.0f - fTemp70)) + (fRec43[1] * fTemp70));
			float fTemp71 = (20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec43[0])));
			int iTemp72 = ((fTemp71 > fSlow28) + (fTemp71 > fSlow29));
			float fThen23 = (fTemp71 - fSlow27);
			float fElse23 = (0.166666672f * Macalla_faustpower2_f(((fTemp71 + 1.5f) - fSlow27)));
			float fThen24 = ((iTemp72 == 1) ? fElse23 : fThen23);
			output0[i0] = FAUSTFLOAT(((fSlow1 * fTemp0) + (fSlow0 * ((fTemp66 * std::pow(10.0f, (0.0500000007f * fRec41[0]))) * std::pow(10.0f, (0.0500000007f * (0.0f - (fSlow20 * std::max<float>(0.0f, ((iTemp72 == 0) ? 0.0f : fThen24))))))))));
			float fTemp73 = float(input1[i0]);
			fVec9[(IOTA & 131071)] = fTemp73;
			fRec51[0] = (fSlow30 + (fConst6 * fRec51[1]));
			float fThen25 = (fRec50[1] + (fConst10 * (fSlow5 + (0.100000001f * fRec51[0]))));
			float fTemp74 = (iTemp12 ? 0.0f : fThen25);
			fRec50[0] = (fTemp74 - std::floor(fTemp74));
			fRec49[0] = ((9.99999975e-05f * (fTemp11 + (1.0f - std::fabs(((2.0f * fRec50[0]) + -1.0f))))) + (0.999899983f * fRec49[1]));
			float fThen26 = (fRec53[1] + (fConst10 * (fSlow8 + (0.200000003f * fRec51[0]))));
			float fTemp75 = (iTemp12 ? 0.0f : fThen26);
			fRec53[0] = (fTemp75 - std::floor(fTemp75));
			fRec52[0] = ((9.99999975e-05f * (fTemp16 * (fTemp11 + (1.0f - std::fabs(((2.0f * fRec53[0]) + -1.0f)))))) + (0.999899983f * fRec52[1]));
			float fTemp76 = ((1000.0f * (fRec10[0] * fRec49[0])) + (200.0f * (fRec15[0] * fRec52[0])));
			float fTemp77 = (fTemp76 + 100.000008f);
			int iTemp78 = int(fTemp77);
			float fTemp79 = std::floor(fTemp77);
			fRec48[0] = (((fRec44[((IOTA - (int(std::min<float>(fConst0, float(std::max<int>(0, iTemp78)))) + 1)) & 262143)] * (0.0f - (fTemp76 + (99.0f - fTemp79)))) + ((fTemp76 + (100.0f - fTemp79)) * fRec44[((IOTA - (int(std::min<float>(fConst0, float(std::max<int>(0, (iTemp78 + 1))))) + 1)) & 262143)])) - (fConst9 * ((fConst12 * fRec48[2]) + (fConst13 * fRec48[1]))));
			float fTemp80 = (fRec48[2] + (fRec48[0] + (2.0f * fRec48[1])));
			float fTemp81 = std::fabs((fConst9 * fTemp80));
			float fTemp82 = ((fTemp81 > fRec55[1]) ? fConst17 : fConst16);
			fRec55[0] = ((fTemp81 * (1.0f - fTemp82)) + (fRec55[1] * fTemp82));
			fRec54[0] = ((fConst15 * (0.0f - (0.75f * std::max<float>(((20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec55[0]))) + 6.0f), 0.0f)))) + (fConst14 * fRec54[1]));
			float fTemp83 = (fConst9 * (fTemp80 * std::pow(10.0f, (0.0500000007f * fRec54[0]))));
			float fTemp84 = std::fabs((fTemp83 + 9.99999975e-05f));
			float fTemp85 = ((fTemp84 > fRec56[1]) ? fConst20 : fConst16);
			fRec56[0] = ((fTemp84 * (1.0f - fTemp85)) + (fRec56[1] * fTemp85));
			fRec47[0] = ((fTemp83 + (1.5f * ((fRec19[0] * fRec56[0]) * fTemp36))) - (((fTemp39 * fRec47[2]) + (2.0f * (fTemp41 * fRec47[1]))) / fTemp42));
			fRec46[0] = (((((fTemp43 * fRec47[1]) + (fRec47[0] / fTemp40)) + (fRec47[2] / fTemp40)) / fTemp42) - (((fTemp46 * fRec46[2]) + (2.0f * (fTemp47 * fRec46[1]))) / fTemp48));
			fRec45[0] = (((fRec46[2] + (fRec46[0] + (2.0f * fRec46[1]))) / fTemp48) - (fConst45 * ((fConst46 * fRec45[2]) + (fConst47 * fRec45[1]))));
			float fTemp86 = ((fTemp9 * ((fVec9[((IOTA - iTemp3) & 131071)] * fTemp5) + (fVec9[((IOTA - iTemp6) & 131071)] * fTemp7))) + ((fConst4 * (fRec2[0] * (fRec45[2] + (fRec45[0] + (2.0f * fRec45[1]))))) + (fTemp8 * ((fVec9[((IOTA - iTemp50) & 131071)] * fTemp52) + (fTemp53 * fVec9[((IOTA - iTemp54) & 131071)])))));
			fVec10[(IOTA & 1048575)] = fTemp86;
			float fThen29 = (fRec61[1] + (fConst10 * (fRec38[0] + (2.0f * fRec51[0]))));
			float fTemp87 = (iTemp12 ? 0.0f : fThen29);
			fRec61[0] = (fTemp87 - std::floor(fTemp87));
			float fTemp88 = std::min<float>(524288.0f, std::max<float>(0.0f, (fTemp57 + ((fRec36[0] * (fConst0 + (1000.0f * fRec51[0]))) * std::fabs(((2.0f * (1.0f - std::fabs(((2.0f * fRec61[0]) + -1.0f)))) + -1.0f))))));
			float fThen31 = (((fRec58[1] == 1.0f) & (fTemp88 != fRec60[1])) ? fConst19 : 0.0f);
			float fThen33 = (((fRec58[1] == 0.0f) & (fTemp88 != fRec59[1])) ? fConst18 : fThen31);
			float fElse33 = (((fRec58[1] > 0.0f) & (fRec58[1] < 1.0f)) ? fRec57[1] : 0.0f);
			float fTemp89 = ((fRec57[1] != 0.0f) ? fElse33 : fThen33);
			fRec57[0] = fTemp89;
			fRec58[0] = std::max<float>(0.0f, std::min<float>(1.0f, (fRec58[1] + fTemp89)));
			fRec59[0] = (((fRec58[1] >= 1.0f) & (fRec60[1] != fTemp88)) ? fTemp88 : fRec59[1]);
			fRec60[0] = (((fRec58[1] <= 0.0f) & (fRec59[1] != fTemp88)) ? fTemp88 : fRec60[1]);
			float fTemp90 = fVec10[((IOTA - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec59[0])))) & 1048575)];
			fRec44[(IOTA & 262143)] = (fTemp90 + (fRec58[0] * (fVec10[((IOTA - int(std::min<float>(524288.0f, std::max<float>(0.0f, fRec60[0])))) & 1048575)] - fTemp90)));
			float fTemp91 = (fRec44[((IOTA - 0) & 262143)] * fTemp62);
			float fTemp92 = std::fabs((fTemp91 + 9.99999975e-05f));
			float fTemp93 = ((fTemp92 > fRec62[1]) ? fConst20 : fConst16);
			fRec62[0] = ((fTemp92 * (1.0f - fTemp93)) + (fRec62[1] * fTemp93));
			float fTemp94 = (fTemp91 + (1.5f * ((fRec19[0] * fRec62[0]) * fTemp36)));
			float fTemp95 = std::fabs(fTemp94);
			float fTemp96 = ((fTemp95 > fRec64[1]) ? fConst17 : fConst16);
			fRec64[0] = ((fTemp95 * (1.0f - fTemp96)) + (fRec64[1] * fTemp96));
			fRec63[0] = ((fConst15 * (0.0f - (0.75f * std::max<float>(((20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec64[0]))) + 6.0f), 0.0f)))) + (fConst14 * fRec63[1]));
			float fTemp97 = std::fabs(fTemp73);
			float fTemp98 = ((fTemp97 > fRec65[1]) ? fSlow26 : fSlow23);
			fRec65[0] = ((fTemp97 * (1.0f - fTemp98)) + (fRec65[1] * fTemp98));
			float fTemp99 = (20.0f * std::log10(std::max<float>(1.17549435e-38f, fRec65[0])));
			int iTemp100 = ((fTemp99 > fSlow28) + (fTemp99 > fSlow29));
			float fThen39 = (fTemp99 - fSlow27);
			float fElse39 = (0.166666672f * Macalla_faustpower2_f(((fTemp99 + 1.5f) - fSlow27)));
			float fThen40 = ((iTemp100 == 1) ? fElse39 : fThen39);
			output1[i0] = FAUSTFLOAT(((fSlow1 * fTemp73) + (fSlow0 * ((fTemp94 * std::pow(10.0f, (0.0500000007f * fRec63[0]))) * std::pow(10.0f, (0.0500000007f * (0.0f - (fSlow20 * std::max<float>(0.0f, ((iTemp100 == 0) ? 0.0f : fThen40))))))))));
			IOTA = (IOTA + 1);
			iVec1[1] = iVec1[0];
			fRec1[1] = fRec1[0];
			fRec2[1] = fRec2[0];
			iRec8[1] = iRec8[0];
			fVec2[1] = fVec2[0];
			fRec9[1] = fRec9[0];
			fRec7[1] = fRec7[0];
			fRec10[1] = fRec10[0];
			fRec12[1] = fRec12[0];
			fRec14[1] = fRec14[0];
			fRec11[1] = fRec11[0];
			fRec15[1] = fRec15[0];
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fRec17[1] = fRec17[0];
			fRec16[1] = fRec16[0];
			fRec18[1] = fRec18[0];
			fRec19[1] = fRec19[0];
			#pragma clang loop vectorize(enable) interleave(enable)
			for (int j0 = 3; (j0 > 0); j0 = (j0 - 1)) {
				fRec21[j0] = fRec21[(j0 - 1)];
			}
			fRec22[1] = fRec22[0];
			fVec4[1] = fVec4[0];
			fRec23[1] = fRec23[0];
			fRec25[1] = fRec25[0];
			fVec5[1] = fVec5[0];
			fRec24[1] = fRec24[0];
			fRec26[1] = fRec26[0];
			fRec27[1] = fRec27[0];
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fRec28[1] = fRec28[0];
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fRec29[1] = fRec29[0];
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fVec8[1] = fVec8[0];
			fRec34[1] = fRec34[0];
			fRec35[1] = fRec35[0];
			fRec36[1] = fRec36[0];
			fRec38[1] = fRec38[0];
			fRec37[1] = fRec37[0];
			fRec30[1] = fRec30[0];
			fRec31[1] = fRec31[0];
			fRec32[1] = fRec32[0];
			fRec33[1] = fRec33[0];
			fRec39[1] = fRec39[0];
			fRec40[1] = fRec40[0];
			fRec42[1] = fRec42[0];
			fRec41[1] = fRec41[0];
			fRec43[1] = fRec43[0];
			fRec51[1] = fRec51[0];
			fRec50[1] = fRec50[0];
			fRec49[1] = fRec49[0];
			fRec53[1] = fRec53[0];
			fRec52[1] = fRec52[0];
			fRec48[2] = fRec48[1];
			fRec48[1] = fRec48[0];
			fRec55[1] = fRec55[0];
			fRec54[1] = fRec54[0];
			fRec56[1] = fRec56[0];
			fRec47[2] = fRec47[1];
			fRec47[1] = fRec47[0];
			fRec46[2] = fRec46[1];
			fRec46[1] = fRec46[0];
			fRec45[2] = fRec45[1];
			fRec45[1] = fRec45[0];
			fRec61[1] = fRec61[0];
			fRec57[1] = fRec57[0];
			fRec58[1] = fRec58[0];
			fRec59[1] = fRec59[0];
			fRec60[1] = fRec60[0];
			fRec62[1] = fRec62[0];
			fRec64[1] = fRec64[0];
			fRec63[1] = fRec63[0];
			fRec65[1] = fRec65[0];
		}
	}

};

    // END-FAUSTDSP

#endif
