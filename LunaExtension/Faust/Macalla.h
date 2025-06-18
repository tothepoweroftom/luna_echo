/* ------------------------------------------------------------
name: "Macalla "
Code generated with Faust 2.38.8 (https://faust.grame.fr)
Compilation options: -a faustMinimal.h -lang cpp -es 1 -vec -lv 0 -vs 32 -single
-ftz 0 -mcd 16
------------------------------------------------------------ */

#ifndef __Macalla_H__
#define __Macalla_H__

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

template<typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}

    // -- widget's layouts

    virtual void openTabBox(const char* label)        = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label)   = 0;
    virtual void closeBox()                           = 0;

    // -- active widgets

    virtual void addButton(const char* label, REAL* zone)      = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label,
                                   REAL* zone,
                                   REAL init,
                                   REAL min,
                                   REAL max,
                                   REAL step)                  = 0;
    virtual void addHorizontalSlider(const char* label,
                                     REAL* zone,
                                     REAL init,
                                     REAL min,
                                     REAL max,
                                     REAL step)                = 0;
    virtual void addNumEntry(const char* label,
                             REAL* zone,
                             REAL init,
                             REAL min,
                             REAL max,
                             REAL step)                        = 0;

    // -- passive widgets

    virtual void addHorizontalBargraph(const char* label,
                                       REAL* zone,
                                       REAL min,
                                       REAL max) = 0;
    virtual void addVerticalBargraph(const char* label,
                                     REAL* zone,
                                     REAL min,
                                     REAL max)   = 0;

    // -- soundfiles

    virtual void addSoundfile(const char* label,
                              const char* filename,
                              Soundfile** sf_zone) = 0;

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

    std::string replaceCharList(std::string str,
                                const std::vector<char>& ch1,
                                char ch2)
    {
        std::vector<char>::const_iterator beg = ch1.begin();
        std::vector<char>::const_iterator end = ch1.end();
        for (size_t i = 0; i < str.length(); ++i)
        {
            if (std::find(beg, end, str[i]) != end)
            {
                str[i] = ch2;
            }
        }
        return str;
    }

    std::string buildPath(const std::string& label)
    {
        std::string res = "/";
        for (size_t i = 0; i < fControlsLevel.size(); i++)
        {
            res += fControlsLevel[i];
            res += "/";
        }
        res += label;
        std::vector<char> rep = { ' ', '#', '*', ',', '/', '?',
                                  '[', ']', '{', '}', '(', ')' };
        replaceCharList(res, rep, '_');
        return res;
    }

    void pushLabel(const std::string& label)
    {
        fControlsLevel.push_back(label);
    }
    void popLabel() { fControlsLevel.pop_back(); }
};

#endif // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface.
 *
 * This class creates:
 * - a map of 'labels' and zones for each UI item.
 * - a map of complete hierarchical 'paths' and zones for each UI item.
 *
 * Simple 'labels' and complete 'paths' (to fully discriminate between possible
 *same 'labels' at different location in the UI hierachy) can be used to access
 *a given parameter.
 ******************************************************************************/

class MapUI
  : public UI
  , public PathBuilder
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
    void openTabBox(const char* label) { pushLabel(label); }
    void openHorizontalBox(const char* label) { pushLabel(label); }
    void openVerticalBox(const char* label) { pushLabel(label); }
    void closeBox() { popLabel(); }

    // -- active widgets
    void addButton(const char* label, FAUSTFLOAT* zone)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label]           = zone;
    }
    void addCheckButton(const char* label, FAUSTFLOAT* zone)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label]           = zone;
    }
    void addVerticalSlider(const char* label,
                           FAUSTFLOAT* zone,
                           FAUSTFLOAT init,
                           FAUSTFLOAT fmin,
                           FAUSTFLOAT fmax,
                           FAUSTFLOAT step)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label]           = zone;
    }
    void addHorizontalSlider(const char* label,
                             FAUSTFLOAT* zone,
                             FAUSTFLOAT init,
                             FAUSTFLOAT fmin,
                             FAUSTFLOAT fmax,
                             FAUSTFLOAT step)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label]           = zone;
    }
    void addNumEntry(const char* label,
                     FAUSTFLOAT* zone,
                     FAUSTFLOAT init,
                     FAUSTFLOAT fmin,
                     FAUSTFLOAT fmax,
                     FAUSTFLOAT step)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label]           = zone;
    }

    // -- passive widgets
    void addHorizontalBargraph(const char* label,
                               FAUSTFLOAT* zone,
                               FAUSTFLOAT fmin,
                               FAUSTFLOAT fmax)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label]           = zone;
    }
    void addVerticalBargraph(const char* label,
                             FAUSTFLOAT* zone,
                             FAUSTFLOAT fmin,
                             FAUSTFLOAT fmax)
    {
        fPathZoneMap[buildPath(label)] = zone;
        fLabelZoneMap[label]           = zone;
    }

    // -- soundfiles
    virtual void addSoundfile(const char* label,
                              const char* filename,
                              Soundfile** sf_zone)
    {
    }

    // -- metadata declarations
    virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) {}

    // setParamValue/getParamValue
    void setParamValue(const std::string& path, FAUSTFLOAT value)
    {
        if (fPathZoneMap.find(path) != fPathZoneMap.end())
        {
            *fPathZoneMap[path] = value;
        }
        else if (fLabelZoneMap.find(path) != fLabelZoneMap.end())
        {
            *fLabelZoneMap[path] = value;
        }
        else
        {
            fprintf(
              stderr, "ERROR : setParamValue '%s' not found\n", path.c_str());
        }
    }

    FAUSTFLOAT getParamValue(const std::string& path)
    {
        if (fPathZoneMap.find(path) != fPathZoneMap.end())
        {
            return *fPathZoneMap[path];
        }
        else if (fLabelZoneMap.find(path) != fLabelZoneMap.end())
        {
            return *fLabelZoneMap[path];
        }
        else
        {
            fprintf(
              stderr, "ERROR : getParamValue '%s' not found\n", path.c_str());
            return 0;
        }
    }

    // map access
    std::map<std::string, FAUSTFLOAT*>& getMap() { return fPathZoneMap; }

    int getParamsCount() { return int(fPathZoneMap.size()); }

    std::string getParamAddress(int index)
    {
        if (index < 0 || index > int(fPathZoneMap.size()))
        {
            return "";
        }
        else
        {
            auto it = fPathZoneMap.begin();
            while (index-- > 0 && it++ != fPathZoneMap.end())
            {
            }
            return it->first;
        }
    }

    const char* getParamAddress1(int index)
    {
        if (index < 0 || index > int(fPathZoneMap.size()))
        {
            return nullptr;
        }
        else
        {
            auto it = fPathZoneMap.begin();
            while (index-- > 0 && it++ != fPathZoneMap.end())
            {
            }
            return it->first.c_str();
        }
    }

    std::string getParamAddress(FAUSTFLOAT* zone)
    {
        for (const auto& it : fPathZoneMap)
        {
            if (it.second == zone)
                return it.first;
        }
        return "";
    }

    FAUSTFLOAT* getParamZone(const std::string& str)
    {
        if (fPathZoneMap.find(str) != fPathZoneMap.end())
        {
            return fPathZoneMap[str];
        }
        else if (fLabelZoneMap.find(str) != fLabelZoneMap.end())
        {
            return fLabelZoneMap[str];
        }
        return nullptr;
    }

    FAUSTFLOAT* getParamZone(int index)
    {
        if (index < 0 || index > int(fPathZoneMap.size()))
        {
            return nullptr;
        }
        else
        {
            auto it = fPathZoneMap.begin();
            while (index-- > 0 && it++ != fPathZoneMap.end())
            {
            }
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
 The base class of Meta handler to be used in dsp::metadata(Meta* m) method to
 retrieve (key, value) metadata.
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

struct dsp_memory_manager
{

    virtual ~dsp_memory_manager() {}

    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr)     = 0;
};

/**
 * Signal processor definition.
 */

class dsp
{

  public:
    dsp() {}
    virtual ~dsp() {}

    /* Return instance number of audio inputs */
    virtual int getNumInputs() = 0;

    /* Return instance number of audio outputs */
    virtual int getNumOutputs() = 0;

    /**
     * Trigger the ui_interface parameter with instance specific calls
     * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build
     * the UI.
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

    /* Init instance state (like delay lines...) but keep the control parameter
     * values */
    virtual void instanceClear() = 0;

    /**
     * Return a clone of the instance.
     *
     * @return a copy of the instance on success, otherwise a null pointer.
     */
    virtual dsp* clone() = 0;

    /**
     * Trigger the Meta* parameter with instance specific calls to 'declare'
     * (key, value) metadata.
     *
     * @param m - the Meta* meta user
     */
    virtual void metadata(Meta* m) = 0;

    /**
     * DSP instance computation, to be called with successive in/out audio
     * buffers.
     *
     * @param count - the number of frames to compute
     * @param inputs - the input audio buffers as an array of non-interleaved
     * FAUSTFLOAT samples (eiher float, double or quad)
     * @param outputs - the output audio buffers as an array of non-interleaved
     * FAUSTFLOAT samples (eiher float, double or quad)
     *
     */
    virtual void compute(int count,
                         FAUSTFLOAT** inputs,
                         FAUSTFLOAT** outputs) = 0;

    /**
     * DSP instance computation: alternative method to be used by subclasses.
     *
     * @param date_usec - the timestamp in microsec given by audio driver.
     * @param count - the number of frames to compute
     * @param inputs - the input audio buffers as an array of non-interleaved
     * FAUSTFLOAT samples (either float, double or quad)
     * @param outputs - the output audio buffers as an array of non-interleaved
     * FAUSTFLOAT samples (either float, double or quad)
     *
     */
    virtual void compute(double /*date_usec*/,
                         int count,
                         FAUSTFLOAT** inputs,
                         FAUSTFLOAT** outputs)
    {
        compute(count, inputs, outputs);
    }
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp
{

  protected:
    dsp* fDSP;

  public:
    decorator_dsp(dsp* dsp = nullptr)
      : fDSP(dsp)
    {
    }
    virtual ~decorator_dsp() { delete fDSP; }

    virtual int getNumInputs() { return fDSP->getNumInputs(); }
    virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
    virtual void buildUserInterface(UI* ui_interface)
    {
        fDSP->buildUserInterface(ui_interface);
    }
    virtual int getSampleRate() { return fDSP->getSampleRate(); }
    virtual void init(int sample_rate) { fDSP->init(sample_rate); }
    virtual void instanceInit(int sample_rate)
    {
        fDSP->instanceInit(sample_rate);
    }
    virtual void instanceConstants(int sample_rate)
    {
        fDSP->instanceConstants(sample_rate);
    }
    virtual void instanceResetUserInterface()
    {
        fDSP->instanceResetUserInterface();
    }
    virtual void instanceClear() { fDSP->instanceClear(); }
    virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
    virtual void metadata(Meta* m) { fDSP->metadata(m); }
    // Beware: subclasses usually have to overload the two 'compute' methods
    virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
    {
        fDSP->compute(count, inputs, outputs);
    }
    virtual void compute(double date_usec,
                         int count,
                         FAUSTFLOAT** inputs,
                         FAUSTFLOAT** outputs)
    {
        fDSP->compute(date_usec, count, inputs, outputs);
    }
};

/**
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
 */

class dsp_factory
{

  protected:
    // So that to force sub-classes to use deleteDSPFactory(dsp_factory*
    // factory);
    virtual ~dsp_factory() {}

  public:
    virtual std::string getName()                          = 0;
    virtual std::string getSHAKey()                        = 0;
    virtual std::string getDSPCode()                       = 0;
    virtual std::string getCompileOptions()                = 0;
    virtual std::vector<std::string> getLibraryList()      = 0;
    virtual std::vector<std::string> getIncludePathnames() = 0;

    virtual dsp* createDSPInstance() = 0;

    virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
    virtual dsp_memory_manager* getMemoryManager()             = 0;
};

// Denormal handling

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

class ScopedNoDenormals
{
  private:
    intptr_t fpsr;

    void setFpStatusRegister(intptr_t fpsr_aux) noexcept
    {
#if defined(__arm64__) || defined(__aarch64__)
        asm volatile("msr fpcr, %0" : : "ri"(fpsr_aux));
#elif defined(__SSE__)
        _mm_setcsr(static_cast<uint32_t>(fpsr_aux));
#endif
    }

    void getFpStatusRegister() noexcept
    {
#if defined(__arm64__) || defined(__aarch64__)
        asm volatile("mrs %0, fpcr" : "=r"(fpsr));
#elif defined(__SSE__)
        fpsr = static_cast<intptr_t>(_mm_getcsr());
#endif
    }

  public:
    ScopedNoDenormals() noexcept
    {
#if defined(__arm64__) || defined(__aarch64__)
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

    ~ScopedNoDenormals() noexcept { setFpStatusRegister(fpsr); }
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

class MacallaSIG0
{

  private:
    int iVec1[2];
    int iRec18[2];

  public:
    int getNumInputsMacallaSIG0() { return 0; }
    int getNumOutputsMacallaSIG0() { return 1; }

    void instanceInitMacallaSIG0(int sample_rate)
    {
        for (int l11 = 0; (l11 < 2); l11 = (l11 + 1))
        {
            iVec1[l11] = 0;
        }
        for (int l12 = 0; (l12 < 2); l12 = (l12 + 1))
        {
            iRec18[l12] = 0;
        }
    }

    void fillMacallaSIG0(int count, float* table)
    {
        for (int i1 = 0; (i1 < count); i1 = (i1 + 1))
        {
            iVec1[0]  = 1;
            iRec18[0] = ((iVec1[1] + iRec18[1]) % 65536);
            table[i1] = std::sin((9.58738019e-05f * float(iRec18[0])));
            iVec1[1]  = iVec1[0];
            iRec18[1] = iRec18[0];
        }
    }
};

static MacallaSIG0*
newMacallaSIG0()
{
    return (MacallaSIG0*)new MacallaSIG0();
}
static void
deleteMacallaSIG0(MacallaSIG0* dsp)
{
    delete dsp;
}

static float ftbl0MacallaSIG0[65536];
static float
Macalla_faustpower2_f(float value)
{
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

class Macalla : public dsp
{

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
    FAUSTFLOAT fHslider3;
    float fRec11_perm[4];
    int iRec13_perm[4];
    int iVec0_perm[4];
    float fConst3;
    float fRec14_perm[4];
    float fYec0_perm[4];
    float fRec12_perm[4];
    float fRec16_perm[4];
    float fConst4;
    float fRec17_perm[4];
    float fRec15_perm[4];
    float fConst7;
    float fConst8;
    float fConst9;
    float fRec10_perm[4];
    int iRec20_perm[4];
    int iConst10;
    float fRec19_perm[4];
    FAUSTFLOAT fCheckbox0;
    float fConst11;
    float fConst12;
    float fRec22_perm[4];
    float fConst13;
    float fConst14;
    float fRec21_perm[4];
    float fConst15;
    float fConst16;
    float fConst17;
    float fRec23_perm[4];
    FAUSTFLOAT fHslider4;
    float fRec24_perm[4];
    float fRec26_perm[4];
    float fRec27_perm[4];
    float fYec1_perm[4];
    float fRec28_perm[4];
    float fConst18;
    float fRec30_perm[4];
    float fConst19;
    float fYec2_perm[4];
    float fYec3[2048];
    int fYec3_idx;
    int fYec3_idx_save;
    float fConst22;
    int iConst23;
    float fConst24;
    int iConst25;
    float fRec29_perm[4];
    float fConst26;
    float fRec31_perm[4];
    float fConst27;
    float fRec32_perm[4];
    float fConst28;
    float fConst36;
    float fConst37;
    float fConst38;
    float fRec25_perm[4];
    FAUSTFLOAT fHslider5;
    float fRec33_perm[4];
    float fConst39;
    float fConst40;
    float fConst41;
    float fRec9_perm[4];
    FAUSTFLOAT fHslider6;
    float fRec34_perm[4];
    float fRec8_perm[4];
    float fConst42;
    FAUSTFLOAT fHslider7;
    float fRec35_perm[4];
    float fConst43;
    float fRec36_perm[4];
    float fYec4[2048];
    int fYec4_idx;
    int fYec4_idx_save;
    int iConst45;
    float fConst46;
    float fConst47;
    float fRec6_perm[4];
    float fConst48;
    float fRec37_perm[4];
    float fYec5[4096];
    int fYec5_idx;
    int fYec5_idx_save;
    float fRec4_perm[4];
    FAUSTFLOAT fHslider8;
    float fRec42_perm[4];
    float fRec43_perm[4];
    FAUSTFLOAT fHslider9;
    float fRec38_perm[4];
    float fRec39_perm[4];
    float fRec40_perm[4];
    float fRec41_perm[4];
    float fYec6[131072];
    int fYec6_idx;
    int fYec6_idx_save;
    float fYec7[1048576];
    int fYec7_idx;
    int fYec7_idx_save;
    float fRec0[262144];
    int fRec0_idx;
    int fRec0_idx_save;
    float fRec44_perm[4];
    float fRec46_perm[4];
    float fRec45_perm[4];
    FAUSTFLOAT fHslider10;
    FAUSTFLOAT fHslider11;
    float fRec47_perm[4];
    FAUSTFLOAT fHslider12;
    float fRec48_perm[4];
    FAUSTFLOAT fHslider13;
    FAUSTFLOAT fHslider14;
    float fRec59_perm[4];
    float fRec58_perm[4];
    float fRec57_perm[4];
    float fRec61_perm[4];
    float fRec60_perm[4];
    float fRec56_perm[4];
    float fRec62_perm[4];
    float fRec64_perm[4];
    float fRec63_perm[4];
    float fRec65_perm[4];
    float fRec55_perm[4];
    float fRec54_perm[4];
    float fRec66_perm[4];
    float fYec8[16384];
    int fYec8_idx;
    int fYec8_idx_save;
    float fRec52_perm[4];
    float fRec67_perm[4];
    float fYec9[16384];
    int fYec9_idx;
    int fYec9_idx_save;
    float fRec50_perm[4];
    float fRec72_perm[4];
    float fRec68_perm[4];
    float fRec69_perm[4];
    float fRec70_perm[4];
    float fRec71_perm[4];
    float fYec10[131072];
    int fYec10_idx;
    int fYec10_idx_save;
    float fYec11[1048576];
    int fYec11_idx;
    int fYec11_idx_save;
    float fRec49[262144];
    int fRec49_idx;
    int fRec49_idx_save;
    float fRec73_perm[4];
    float fRec75_perm[4];
    float fRec74_perm[4];
    float fRec76_perm[4];

  public:
    void metadata(Meta* m)
    {
        m->declare("analyzers.lib/amp_follower_ar:author",
                   "Jonatan Liljedahl, revised by Romain Michon");
        m->declare("analyzers.lib/name", "Faust Analyzer Library");
        m->declare("analyzers.lib/version", "1.2.0");
        m->declare("basics.lib/bypass1:author", "Julius Smith");
        m->declare("basics.lib/downSample:author", "Romain Michon");
        m->declare("basics.lib/name", "Faust Basic Element Library");
        m->declare("basics.lib/sAndH:author", "Romain Michon");
        m->declare("basics.lib/tabulateNd",
                   "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
        m->declare("basics.lib/version", "1.18.0");
        m->declare("category", "Echo / Delay");
        m->declare("compile_options",
                   "-a faustMinimal.h -lang cpp -es 1 -vec -lv 0 -vs 32 "
                   "-single -ftz 0 -mcd 16");
        m->declare("compressors.lib/compression_gain_mono:author",
                   "Julius O. Smith III");
        m->declare("compressors.lib/compression_gain_mono:copyright",
                   "Copyright (C) 2014-2020 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("compressors.lib/compression_gain_mono:license",
                   "MIT-style STK-4.3 license");
        m->declare("compressors.lib/compressor_lad_mono:author",
                   "Julius O. Smith III");
        m->declare("compressors.lib/compressor_lad_mono:copyright",
                   "Copyright (C) 2014-2020 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("compressors.lib/compressor_lad_mono:license",
                   "MIT-style STK-4.3 license");
        m->declare("compressors.lib/compressor_mono:author",
                   "Julius O. Smith III");
        m->declare("compressors.lib/compressor_mono:copyright",
                   "Copyright (C) 2014-2020 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("compressors.lib/compressor_mono:license",
                   "MIT-style STK-4.3 license");
        m->declare("compressors.lib/limiter_1176_R4_mono:author",
                   "Julius O. Smith III");
        m->declare("compressors.lib/limiter_1176_R4_mono:copyright",
                   "Copyright (C) 2014-2020 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("compressors.lib/limiter_1176_R4_mono:license",
                   "MIT-style STK-4.3 license");
        m->declare("compressors.lib/name", "Faust Compressor Effect Library");
        m->declare("compressors.lib/peak_compression_gain_mono_db:author",
                   "Bart Brouns");
        m->declare("compressors.lib/peak_compression_gain_mono_db:license",
                   "GPLv3");
        m->declare("compressors.lib/ratio2strength:author", "Bart Brouns");
        m->declare("compressors.lib/ratio2strength:license", "GPLv3");
        m->declare("compressors.lib/version", "1.6.0");
        m->declare("delays.lib/fdelay1:author", "Julius O. Smith III");
        m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
        m->declare("delays.lib/name", "Faust Delay Library");
        m->declare("delays.lib/version", "1.1.0");
        m->declare("filename", "Macalla.dsp");
        m->declare("filters.lib/allpass_fcomb:author", "Julius O. Smith III");
        m->declare("filters.lib/allpass_fcomb:copyright",
                   "Copyright (C) 2003-2019 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("filters.lib/allpass_fcomb:license",
                   "MIT-style STK-4.3 license");
        m->declare("filters.lib/bandpass0_bandstop1:author",
                   "Julius O. Smith III");
        m->declare("filters.lib/bandpass0_bandstop1:copyright",
                   "Copyright (C) 2003-2019 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("filters.lib/bandpass0_bandstop1:license",
                   "MIT-style STK-4.3 license");
        m->declare("filters.lib/bandpass:author", "Julius O. Smith III");
        m->declare("filters.lib/bandpass:copyright",
                   "Copyright (C) 2003-2019 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("filters.lib/bandpass:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/fir:author", "Julius O. Smith III");
        m->declare("filters.lib/fir:copyright",
                   "Copyright (C) 2003-2019 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/highpass:author", "Julius O. Smith III");
        m->declare("filters.lib/highpass:copyright",
                   "Copyright (C) 2003-2019 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("filters.lib/iir:author", "Julius O. Smith III");
        m->declare("filters.lib/iir:copyright",
                   "Copyright (C) 2003-2019 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/lowpass0_highpass1",
                   "MIT-style STK-4.3 license");
        m->declare("filters.lib/lowpass0_highpass1:author",
                   "Julius O. Smith III");
        m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
        m->declare("filters.lib/lowpass:copyright",
                   "Copyright (C) 2003-2019 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/name", "Faust Filters Library");
        m->declare("filters.lib/pole:author", "Julius O. Smith III");
        m->declare("filters.lib/pole:copyright",
                   "Copyright (C) 2003-2019 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/tf1sb:author", "Julius O. Smith III");
        m->declare("filters.lib/tf1sb:copyright",
                   "Copyright (C) 2003-2019 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("filters.lib/tf1sb:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/tf2:author", "Julius O. Smith III");
        m->declare("filters.lib/tf2:copyright",
                   "Copyright (C) 2003-2019 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
        m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
        m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
        m->declare("filters.lib/tf2s:copyright",
                   "Copyright (C) 2003-2019 by Julius O. Smith III "
                   "<jos@ccrma.stanford.edu>");
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
        m->declare("oscillators.lib/lf_sawpos:author",
                   "Bart Brouns, revised by Stéphane Letz");
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
        m->declare("signals.lib/onePoleSwitching:author",
                   "Jonatan Liljedahl, revised by Dario Sanfilippo");
        m->declare("signals.lib/onePoleSwitching:licence", "STK-4.3");
        m->declare("signals.lib/version", "1.5.0");
    }

    virtual int getNumInputs() { return 2; }
    virtual int getNumOutputs() { return 2; }

    static void classInit(int sample_rate)
    {
        MacallaSIG0* sig0 = newMacallaSIG0();
        sig0->instanceInitMacallaSIG0(sample_rate);
        sig0->fillMacallaSIG0(65536, ftbl0MacallaSIG0);
        deleteMacallaSIG0(sig0);
    }

    virtual void instanceConstants(int sample_rate)
    {
        fSampleRate = sample_rate;
        fConst0 =
          std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
        fConst1       = (44.0999985f / fConst0);
        fConst2       = (1.0f - fConst1);
        fConst3       = (1.0f / fConst0);
        fConst4       = (0.100000001f / fConst0);
        float fConst5 = std::tan((25132.7422f / fConst0));
        float fConst6 = (1.0f / fConst5);
        fConst7       = (1.0f / (((fConst6 + 1.41421354f) / fConst5) + 1.0f));
        fConst8       = (((fConst6 + -1.41421354f) / fConst5) + 1.0f);
        fConst9  = (2.0f * (1.0f - (1.0f / Macalla_faustpower2_f(fConst5))));
        iConst10 = int((9.09090886e-05f * fConst0));
        fConst11 = std::exp((0.0f - (2.0f / fConst0)));
        fConst12 = std::exp((0.0f - (1250.0f / fConst0)));
        fConst13 = std::exp((0.0f - (2500.0f / fConst0)));
        fConst14 = (1.0f - fConst13);
        fConst15 = (20.0f / fConst0);
        fConst16 = (0.0f - fConst15);
        fConst17 = std::exp(fConst16);
        fConst18 = (60.0f / fConst0);
        fConst19 = (0.00416666688f * fConst0);
        float fConst20 = std::max<float>(
          0.0f, std::min<float>(2047.0f, (0.00833333377f * fConst0)));
        float fConst21 = std::floor(fConst20);
        fConst22       = (fConst21 + (1.0f - fConst20));
        iConst23       = int(fConst20);
        fConst24       = (fConst20 - fConst21);
        iConst25       = (iConst23 + 1);
        fConst26       = (528.0f / fConst0);
        fConst27       = (110.0f / fConst0);
        fConst28       = (48.0f / fConst0);
        float fConst29 = std::tan((31415.9258f / fConst0));
        float fConst30 = Macalla_faustpower2_f(
          std::sqrt((4.0f * ((Macalla_faustpower2_f(fConst0) * fConst29) *
                             std::tan((1570.79639f / fConst0))))));
        float fConst31 = (Macalla_faustpower2_f(fConst3) * fConst30);
        float fConst32 = (fConst0 * fConst29);
        float fConst33 = ((2.0f * fConst32) - (0.5f * (fConst30 / fConst32)));
        float fConst34 = (2.0f * (fConst33 / fConst0));
        float fConst35 = ((fConst31 + fConst34) + 4.0f);
        fConst36       = (1.0f / fConst35);
        fConst37       = ((2.0f * fConst31) + -8.0f);
        fConst38       = (fConst31 + (4.0f - fConst34));
        fConst39       = (2.0f * (fConst33 / (fConst0 * fConst35)));
        fConst40       = (0.0f - fConst39);
        fConst41       = (3.14159274f / fConst0);
        fConst42       = (41.8950005f / fConst0);
        fConst43       = (0.150000006f / fConst0);
        int iConst44   = int((0.0500000007f * fConst0));
        iConst45       = (iConst44 + 2);
        fConst46       = float(iConst44);
        fConst47       = (0.00100000005f * fConst0);
        fConst48       = (0.230000004f / fConst0);
    }

    virtual void instanceResetUserInterface()
    {
        fHslider0  = FAUSTFLOAT(0.0f);
        fHslider1  = FAUSTFLOAT(0.0f);
        fHslider2  = FAUSTFLOAT(0.5f);
        fHslider3  = FAUSTFLOAT(0.10000000000000001f);
        fCheckbox0 = FAUSTFLOAT(0.0f);
        fHslider4  = FAUSTFLOAT(0.10000000000000001f);
        fHslider5  = FAUSTFLOAT(250.0f);
        fHslider6  = FAUSTFLOAT(10000.0f);
        fHslider7  = FAUSTFLOAT(0.0f);
        fHslider8  = FAUSTFLOAT(0.0f);
        fHslider9  = FAUSTFLOAT(500.0f);
        fHslider10 = FAUSTFLOAT(200.0f);
        fHslider11 = FAUSTFLOAT(10.0f);
        fHslider12 = FAUSTFLOAT(0.0f);
        fHslider13 = FAUSTFLOAT(50.0f);
        fHslider14 = FAUSTFLOAT(0.0f);
    }

    virtual void instanceClear()
    {
        for (int l0 = 0; (l0 < 4); l0 = (l0 + 1))
        {
            fRec1_perm[l0] = 0.0f;
        }
        for (int l1 = 0; (l1 < 4); l1 = (l1 + 1))
        {
            fRec2_perm[l1] = 0.0f;
        }
        for (int l2 = 0; (l2 < 4); l2 = (l2 + 1))
        {
            fRec3_perm[l2] = 0.0f;
        }
        for (int l3 = 0; (l3 < 4); l3 = (l3 + 1))
        {
            fRec11_perm[l3] = 0.0f;
        }
        for (int l4 = 0; (l4 < 4); l4 = (l4 + 1))
        {
            iRec13_perm[l4] = 0;
        }
        for (int l5 = 0; (l5 < 4); l5 = (l5 + 1))
        {
            iVec0_perm[l5] = 0;
        }
        for (int l6 = 0; (l6 < 4); l6 = (l6 + 1))
        {
            fRec14_perm[l6] = 0.0f;
        }
        for (int l7 = 0; (l7 < 4); l7 = (l7 + 1))
        {
            fYec0_perm[l7] = 0.0f;
        }
        for (int l8 = 0; (l8 < 4); l8 = (l8 + 1))
        {
            fRec12_perm[l8] = 0.0f;
        }
        for (int l9 = 0; (l9 < 4); l9 = (l9 + 1))
        {
            fRec16_perm[l9] = 0.0f;
        }
        for (int l10 = 0; (l10 < 4); l10 = (l10 + 1))
        {
            fRec17_perm[l10] = 0.0f;
        }
        for (int l13 = 0; (l13 < 4); l13 = (l13 + 1))
        {
            fRec15_perm[l13] = 0.0f;
        }
        for (int l14 = 0; (l14 < 4); l14 = (l14 + 1))
        {
            fRec10_perm[l14] = 0.0f;
        }
        for (int l15 = 0; (l15 < 4); l15 = (l15 + 1))
        {
            iRec20_perm[l15] = 0;
        }
        for (int l16 = 0; (l16 < 4); l16 = (l16 + 1))
        {
            fRec19_perm[l16] = 0.0f;
        }
        for (int l17 = 0; (l17 < 4); l17 = (l17 + 1))
        {
            fRec22_perm[l17] = 0.0f;
        }
        for (int l18 = 0; (l18 < 4); l18 = (l18 + 1))
        {
            fRec21_perm[l18] = 0.0f;
        }
        for (int l19 = 0; (l19 < 4); l19 = (l19 + 1))
        {
            fRec23_perm[l19] = 0.0f;
        }
        for (int l20 = 0; (l20 < 4); l20 = (l20 + 1))
        {
            fRec24_perm[l20] = 0.0f;
        }
        for (int l21 = 0; (l21 < 4); l21 = (l21 + 1))
        {
            fRec26_perm[l21] = 0.0f;
        }
        for (int l22 = 0; (l22 < 4); l22 = (l22 + 1))
        {
            fRec27_perm[l22] = 0.0f;
        }
        for (int l23 = 0; (l23 < 4); l23 = (l23 + 1))
        {
            fYec1_perm[l23] = 0.0f;
        }
        for (int l24 = 0; (l24 < 4); l24 = (l24 + 1))
        {
            fRec28_perm[l24] = 0.0f;
        }
        for (int l25 = 0; (l25 < 4); l25 = (l25 + 1))
        {
            fRec30_perm[l25] = 0.0f;
        }
        for (int l26 = 0; (l26 < 4); l26 = (l26 + 1))
        {
            fYec2_perm[l26] = 0.0f;
        }
        for (int l27 = 0; (l27 < 2048); l27 = (l27 + 1))
        {
            fYec3[l27] = 0.0f;
        }
        fYec3_idx      = 0;
        fYec3_idx_save = 0;
        for (int l28 = 0; (l28 < 4); l28 = (l28 + 1))
        {
            fRec29_perm[l28] = 0.0f;
        }
        for (int l29 = 0; (l29 < 4); l29 = (l29 + 1))
        {
            fRec31_perm[l29] = 0.0f;
        }
        for (int l30 = 0; (l30 < 4); l30 = (l30 + 1))
        {
            fRec32_perm[l30] = 0.0f;
        }
        for (int l31 = 0; (l31 < 4); l31 = (l31 + 1))
        {
            fRec25_perm[l31] = 0.0f;
        }
        for (int l32 = 0; (l32 < 4); l32 = (l32 + 1))
        {
            fRec33_perm[l32] = 0.0f;
        }
        for (int l33 = 0; (l33 < 4); l33 = (l33 + 1))
        {
            fRec9_perm[l33] = 0.0f;
        }
        for (int l34 = 0; (l34 < 4); l34 = (l34 + 1))
        {
            fRec34_perm[l34] = 0.0f;
        }
        for (int l35 = 0; (l35 < 4); l35 = (l35 + 1))
        {
            fRec8_perm[l35] = 0.0f;
        }
        for (int l36 = 0; (l36 < 4); l36 = (l36 + 1))
        {
            fRec35_perm[l36] = 0.0f;
        }
        for (int l37 = 0; (l37 < 4); l37 = (l37 + 1))
        {
            fRec36_perm[l37] = 0.0f;
        }
        for (int l38 = 0; (l38 < 2048); l38 = (l38 + 1))
        {
            fYec4[l38] = 0.0f;
        }
        fYec4_idx      = 0;
        fYec4_idx_save = 0;
        for (int l39 = 0; (l39 < 4); l39 = (l39 + 1))
        {
            fRec6_perm[l39] = 0.0f;
        }
        for (int l40 = 0; (l40 < 4); l40 = (l40 + 1))
        {
            fRec37_perm[l40] = 0.0f;
        }
        for (int l41 = 0; (l41 < 4096); l41 = (l41 + 1))
        {
            fYec5[l41] = 0.0f;
        }
        fYec5_idx      = 0;
        fYec5_idx_save = 0;
        for (int l42 = 0; (l42 < 4); l42 = (l42 + 1))
        {
            fRec4_perm[l42] = 0.0f;
        }
        for (int l43 = 0; (l43 < 4); l43 = (l43 + 1))
        {
            fRec42_perm[l43] = 0.0f;
        }
        for (int l44 = 0; (l44 < 4); l44 = (l44 + 1))
        {
            fRec43_perm[l44] = 0.0f;
        }
        for (int l45 = 0; (l45 < 4); l45 = (l45 + 1))
        {
            fRec38_perm[l45] = 0.0f;
        }
        for (int l46 = 0; (l46 < 4); l46 = (l46 + 1))
        {
            fRec39_perm[l46] = 0.0f;
        }
        for (int l47 = 0; (l47 < 4); l47 = (l47 + 1))
        {
            fRec40_perm[l47] = 0.0f;
        }
        for (int l48 = 0; (l48 < 4); l48 = (l48 + 1))
        {
            fRec41_perm[l48] = 0.0f;
        }
        for (int l49 = 0; (l49 < 131072); l49 = (l49 + 1))
        {
            fYec6[l49] = 0.0f;
        }
        fYec6_idx      = 0;
        fYec6_idx_save = 0;
        for (int l50 = 0; (l50 < 1048576); l50 = (l50 + 1))
        {
            fYec7[l50] = 0.0f;
        }
        fYec7_idx      = 0;
        fYec7_idx_save = 0;
        for (int l51 = 0; (l51 < 262144); l51 = (l51 + 1))
        {
            fRec0[l51] = 0.0f;
        }
        fRec0_idx      = 0;
        fRec0_idx_save = 0;
        for (int l52 = 0; (l52 < 4); l52 = (l52 + 1))
        {
            fRec44_perm[l52] = 0.0f;
        }
        for (int l53 = 0; (l53 < 4); l53 = (l53 + 1))
        {
            fRec46_perm[l53] = 0.0f;
        }
        for (int l54 = 0; (l54 < 4); l54 = (l54 + 1))
        {
            fRec45_perm[l54] = 0.0f;
        }
        for (int l55 = 0; (l55 < 4); l55 = (l55 + 1))
        {
            fRec47_perm[l55] = 0.0f;
        }
        for (int l56 = 0; (l56 < 4); l56 = (l56 + 1))
        {
            fRec48_perm[l56] = 0.0f;
        }
        for (int l57 = 0; (l57 < 4); l57 = (l57 + 1))
        {
            fRec59_perm[l57] = 0.0f;
        }
        for (int l58 = 0; (l58 < 4); l58 = (l58 + 1))
        {
            fRec58_perm[l58] = 0.0f;
        }
        for (int l59 = 0; (l59 < 4); l59 = (l59 + 1))
        {
            fRec57_perm[l59] = 0.0f;
        }
        for (int l60 = 0; (l60 < 4); l60 = (l60 + 1))
        {
            fRec61_perm[l60] = 0.0f;
        }
        for (int l61 = 0; (l61 < 4); l61 = (l61 + 1))
        {
            fRec60_perm[l61] = 0.0f;
        }
        for (int l62 = 0; (l62 < 4); l62 = (l62 + 1))
        {
            fRec56_perm[l62] = 0.0f;
        }
        for (int l63 = 0; (l63 < 4); l63 = (l63 + 1))
        {
            fRec62_perm[l63] = 0.0f;
        }
        for (int l64 = 0; (l64 < 4); l64 = (l64 + 1))
        {
            fRec64_perm[l64] = 0.0f;
        }
        for (int l65 = 0; (l65 < 4); l65 = (l65 + 1))
        {
            fRec63_perm[l65] = 0.0f;
        }
        for (int l66 = 0; (l66 < 4); l66 = (l66 + 1))
        {
            fRec65_perm[l66] = 0.0f;
        }
        for (int l67 = 0; (l67 < 4); l67 = (l67 + 1))
        {
            fRec55_perm[l67] = 0.0f;
        }
        for (int l68 = 0; (l68 < 4); l68 = (l68 + 1))
        {
            fRec54_perm[l68] = 0.0f;
        }
        for (int l69 = 0; (l69 < 4); l69 = (l69 + 1))
        {
            fRec66_perm[l69] = 0.0f;
        }
        for (int l70 = 0; (l70 < 16384); l70 = (l70 + 1))
        {
            fYec8[l70] = 0.0f;
        }
        fYec8_idx      = 0;
        fYec8_idx_save = 0;
        for (int l71 = 0; (l71 < 4); l71 = (l71 + 1))
        {
            fRec52_perm[l71] = 0.0f;
        }
        for (int l72 = 0; (l72 < 4); l72 = (l72 + 1))
        {
            fRec67_perm[l72] = 0.0f;
        }
        for (int l73 = 0; (l73 < 16384); l73 = (l73 + 1))
        {
            fYec9[l73] = 0.0f;
        }
        fYec9_idx      = 0;
        fYec9_idx_save = 0;
        for (int l74 = 0; (l74 < 4); l74 = (l74 + 1))
        {
            fRec50_perm[l74] = 0.0f;
        }
        for (int l75 = 0; (l75 < 4); l75 = (l75 + 1))
        {
            fRec72_perm[l75] = 0.0f;
        }
        for (int l76 = 0; (l76 < 4); l76 = (l76 + 1))
        {
            fRec68_perm[l76] = 0.0f;
        }
        for (int l77 = 0; (l77 < 4); l77 = (l77 + 1))
        {
            fRec69_perm[l77] = 0.0f;
        }
        for (int l78 = 0; (l78 < 4); l78 = (l78 + 1))
        {
            fRec70_perm[l78] = 0.0f;
        }
        for (int l79 = 0; (l79 < 4); l79 = (l79 + 1))
        {
            fRec71_perm[l79] = 0.0f;
        }
        for (int l80 = 0; (l80 < 131072); l80 = (l80 + 1))
        {
            fYec10[l80] = 0.0f;
        }
        fYec10_idx      = 0;
        fYec10_idx_save = 0;
        for (int l81 = 0; (l81 < 1048576); l81 = (l81 + 1))
        {
            fYec11[l81] = 0.0f;
        }
        fYec11_idx      = 0;
        fYec11_idx_save = 0;
        for (int l82 = 0; (l82 < 262144); l82 = (l82 + 1))
        {
            fRec49[l82] = 0.0f;
        }
        fRec49_idx      = 0;
        fRec49_idx_save = 0;
        for (int l83 = 0; (l83 < 4); l83 = (l83 + 1))
        {
            fRec73_perm[l83] = 0.0f;
        }
        for (int l84 = 0; (l84 < 4); l84 = (l84 + 1))
        {
            fRec75_perm[l84] = 0.0f;
        }
        for (int l85 = 0; (l85 < 4); l85 = (l85 + 1))
        {
            fRec74_perm[l85] = 0.0f;
        }
        for (int l86 = 0; (l86 < 4); l86 = (l86 + 1))
        {
            fRec76_perm[l86] = 0.0f;
        }
    }

    virtual void init(int sample_rate)
    {
        classInit(sample_rate);
        instanceInit(sample_rate);
    }
    virtual void instanceInit(int sample_rate)
    {
        instanceConstants(sample_rate);
        instanceResetUserInterface();
        instanceClear();
    }

    virtual Macalla* clone() { return new Macalla(); }

    virtual int getSampleRate() { return fSampleRate; }

    virtual void buildUserInterface(UI* ui_interface)
    {
        ui_interface->openVerticalBox("Macalla ");
        ui_interface->addCheckButton("bitcrush", &fCheckbox0);
        ui_interface->declare(&fHslider12, "unit", "percent");
        ui_interface->addHorizontalSlider("ducking_amount",
                                          &fHslider12,
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(1.0f),
                                          FAUSTFLOAT(0.00999999978f));
        ui_interface->declare(&fHslider8, "unit", "percent");
        ui_interface->addHorizontalSlider("glitch",
                                          &fHslider8,
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(1.0f),
                                          FAUSTFLOAT(0.00999999978f));
        ui_interface->declare(&fHslider4, "unit", "percent");
        ui_interface->addHorizontalSlider("tape_noise",
                                          &fHslider4,
                                          FAUSTFLOAT(0.100000001f),
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(1.0f),
                                          FAUSTFLOAT(0.00999999978f));
        ui_interface->declare(&fHslider3, "unit", "percent");
        ui_interface->addHorizontalSlider("wow_flutter",
                                          &fHslider3,
                                          FAUSTFLOAT(0.100000001f),
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(1.0f),
                                          FAUSTFLOAT(0.00999999978f));
        ui_interface->addHorizontalSlider("delaytime",
                                          &fHslider9,
                                          FAUSTFLOAT(500.0f),
                                          FAUSTFLOAT(1.0f),
                                          FAUSTFLOAT(5000.0f),
                                          FAUSTFLOAT(0.100000001f));
        ui_interface->addHorizontalSlider("diffusion",
                                          &fHslider7,
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(1.0f),
                                          FAUSTFLOAT(0.00999999978f));
        ui_interface->addHorizontalSlider("duck_attack",
                                          &fHslider11,
                                          FAUSTFLOAT(10.0f),
                                          FAUSTFLOAT(1.0f),
                                          FAUSTFLOAT(100.0f),
                                          FAUSTFLOAT(1.0f));
        ui_interface->addHorizontalSlider("duck_release",
                                          &fHslider10,
                                          FAUSTFLOAT(200.0f),
                                          FAUSTFLOAT(50.0f),
                                          FAUSTFLOAT(1000.0f),
                                          FAUSTFLOAT(1.0f));
        ui_interface->addHorizontalSlider("feedback",
                                          &fHslider2,
                                          FAUSTFLOAT(0.5f),
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(1.0f),
                                          FAUSTFLOAT(0.00999999978f));
        ui_interface->addHorizontalSlider("highpass",
                                          &fHslider5,
                                          FAUSTFLOAT(250.0f),
                                          FAUSTFLOAT(20.0f),
                                          FAUSTFLOAT(20000.0f),
                                          FAUSTFLOAT(1.0f));
        ui_interface->addHorizontalSlider("lowpass",
                                          &fHslider6,
                                          FAUSTFLOAT(10000.0f),
                                          FAUSTFLOAT(20.0f),
                                          FAUSTFLOAT(20000.0f),
                                          FAUSTFLOAT(1.0f));
        ui_interface->addHorizontalSlider("mix",
                                          &fHslider13,
                                          FAUSTFLOAT(50.0f),
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(100.0f),
                                          FAUSTFLOAT(1.0f));
        ui_interface->addHorizontalSlider("output_gain",
                                          &fHslider0,
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(-96.0f),
                                          FAUSTFLOAT(12.0f),
                                          FAUSTFLOAT(0.00999999978f));
        ui_interface->addHorizontalSlider("pitch_shift",
                                          &fHslider1,
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(-12.0f),
                                          FAUSTFLOAT(12.0f),
                                          FAUSTFLOAT(1.0f));
        ui_interface->addHorizontalSlider("spread_amount",
                                          &fHslider14,
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(0.0f),
                                          FAUSTFLOAT(1.0f),
                                          FAUSTFLOAT(0.00999999978f));
        ui_interface->closeBox();
    }

    virtual void compute(int count,
                         FAUSTFLOAT** RESTRICT inputs,
                         FAUSTFLOAT** RESTRICT outputs)
    {
        FAUSTFLOAT* input0_ptr  = inputs[0];
        FAUSTFLOAT* input1_ptr  = inputs[1];
        FAUSTFLOAT* output0_ptr = outputs[0];
        FAUSTFLOAT* output1_ptr = outputs[1];
        float fSlow0            = (fConst1 * float(fHslider0));
        float fRec1_tmp[36];
        float* fRec1 = &fRec1_tmp[4];
        float fSlow1 = std::pow(2.0f, (0.0833333358f * float(fHslider1)));
        float fRec2_tmp[36];
        float* fRec2 = &fRec2_tmp[4];
        float fSlow2 = (fConst1 * float(fHslider2));
        float fRec3_tmp[36];
        float* fRec3 = &fRec3_tmp[4];
        float fSlow3 = (fConst1 * float(fHslider3));
        float fRec11_tmp[36];
        float* fRec11 = &fRec11_tmp[4];
        int iRec13_tmp[36];
        int* iRec13 = &iRec13_tmp[4];
        int iVec0_tmp[36];
        int* iVec0 = &iVec0_tmp[4];
        int iZec0[32];
        float fZec1[32];
        float fZec2[32];
        float fRec14_tmp[36];
        float* fRec14 = &fRec14_tmp[4];
        float fYec0_tmp[36];
        float* fYec0 = &fYec0_tmp[4];
        float fZec3[32];
        float fRec12_tmp[36];
        float* fRec12 = &fRec12_tmp[4];
        float fZec4[32];
        float fZec5[32];
        float fRec16_tmp[36];
        float* fRec16 = &fRec16_tmp[4];
        float fZec6[32];
        float fRec17_tmp[36];
        float* fRec17 = &fRec17_tmp[4];
        float fZec7[32];
        float fRec15_tmp[36];
        float* fRec15 = &fRec15_tmp[4];
        float fZec8[32];
        float fZec9[32];
        int iZec10[32];
        float fZec11[32];
        float fRec10_tmp[36];
        float* fRec10 = &fRec10_tmp[4];
        int iRec20_tmp[36];
        int* iRec20 = &iRec20_tmp[4];
        int iZec12[32];
        float fZec13[32];
        float fRec19_tmp[36];
        float* fRec19 = &fRec19_tmp[4];
        int iSlow4    = int(float(fCheckbox0));
        float fZec14[32];
        float fZec15[32];
        float fZec16[32];
        float fRec22_tmp[36];
        float* fRec22 = &fRec22_tmp[4];
        float fRec21_tmp[36];
        float* fRec21 = &fRec21_tmp[4];
        float fZec17[32];
        float fZec18[32];
        float fZec19[32];
        float fRec23_tmp[36];
        float* fRec23 = &fRec23_tmp[4];
        float fSlow5  = (fConst1 * float(fHslider4));
        float fRec24_tmp[36];
        float* fRec24 = &fRec24_tmp[4];
        float fRec26_tmp[36];
        float* fRec26 = &fRec26_tmp[4];
        float fZec20[32];
        float fRec27_tmp[36];
        float* fRec27 = &fRec27_tmp[4];
        float fYec1_tmp[36];
        float* fYec1 = &fYec1_tmp[4];
        int iZec21[32];
        float fRec28_tmp[36];
        float* fRec28 = &fRec28_tmp[4];
        float fZec22[32];
        float fRec30_tmp[36];
        float* fRec30 = &fRec30_tmp[4];
        float fYec2_tmp[36];
        float* fYec2 = &fYec2_tmp[4];
        float fRec29_tmp[36];
        float* fRec29 = &fRec29_tmp[4];
        float fZec23[32];
        float fRec31_tmp[36];
        float* fRec31 = &fRec31_tmp[4];
        float fZec24[32];
        float fRec32_tmp[36];
        float* fRec32 = &fRec32_tmp[4];
        float fZec25[32];
        float fRec25_tmp[36];
        float* fRec25 = &fRec25_tmp[4];
        float fSlow6  = (fConst1 * float(fHslider5));
        float fRec33_tmp[36];
        float* fRec33 = &fRec33_tmp[4];
        float fZec26[32];
        float fZec27[32];
        float fZec28[32];
        float fZec29[32];
        float fZec30[32];
        float fZec31[32];
        float fZec32[32];
        float fZec33[32];
        float fRec9_tmp[36];
        float* fRec9 = &fRec9_tmp[4];
        float fSlow7 = (fConst1 * float(fHslider6));
        float fRec34_tmp[36];
        float* fRec34 = &fRec34_tmp[4];
        float fZec34[32];
        float fZec35[32];
        float fZec36[32];
        float fZec37[32];
        float fZec38[32];
        float fZec39[32];
        float fRec8_tmp[36];
        float* fRec8 = &fRec8_tmp[4];
        float fSlow8 = (fConst42 * float(fHslider7));
        float fRec35_tmp[36];
        float* fRec35 = &fRec35_tmp[4];
        float fZec40[32];
        float fRec36_tmp[36];
        float* fRec36 = &fRec36_tmp[4];
        float fZec41[32];
        float fZec42[32];
        int iZec43[32];
        float fZec44[32];
        float fRec6_tmp[36];
        float* fRec6 = &fRec6_tmp[4];
        float fRec7[32];
        float fZec45[32];
        float fRec37_tmp[36];
        float* fRec37 = &fRec37_tmp[4];
        float fZec46[32];
        float fZec47[32];
        int iZec48[32];
        float fZec49[32];
        float fRec4_tmp[36];
        float* fRec4 = &fRec4_tmp[4];
        float fRec5[32];
        float fSlow9 = (fConst1 * float(fHslider8));
        float fRec42_tmp[36];
        float* fRec42 = &fRec42_tmp[4];
        float fZec50[32];
        float fZec51[32];
        float fRec43_tmp[36];
        float* fRec43 = &fRec43_tmp[4];
        float fSlow10 = float(fHslider9);
        float fSlow11 = (0.00100000005f * fSlow10);
        float fZec52[32];
        float fZec53[32];
        float fRec38_tmp[36];
        float* fRec38 = &fRec38_tmp[4];
        float fRec39_tmp[36];
        float* fRec39 = &fRec39_tmp[4];
        float fRec40_tmp[36];
        float* fRec40 = &fRec40_tmp[4];
        float fRec41_tmp[36];
        float* fRec41 = &fRec41_tmp[4];
        float fZec54[32];
        float fZec55[32];
        int iZec56[32];
        int iZec57[32];
        float fZec58[32];
        float fZec59[32];
        float fZec60[32];
        int iZec61[32];
        float fZec62[32];
        float fZec63[32];
        int iZec64[32];
        int iZec65[32];
        float fZec66[32];
        float fZec67[32];
        float fZec68[32];
        int iZec69[32];
        float fZec70[32];
        float fZec71[32];
        float fZec72[32];
        float fRec44_tmp[36];
        float* fRec44 = &fRec44_tmp[4];
        float fZec73[32];
        float fZec74[32];
        float fZec75[32];
        float fRec46_tmp[36];
        float* fRec46 = &fRec46_tmp[4];
        float fRec45_tmp[36];
        float* fRec45 = &fRec45_tmp[4];
        float fZec76[32];
        float fSlow12 = (0.00100000005f * float(fHslider10));
        int iSlow13   = (std::fabs(fSlow12) < 1.1920929e-07f);
        float fThen23 =
          std::exp((0.0f - (fConst3 / (iSlow13 ? 1.0f : fSlow12))));
        float fSlow14 = (iSlow13 ? 0.0f : fThen23);
        float fSlow15 = (0.00100000005f * float(fHslider11));
        int iSlow16   = (std::fabs(fSlow15) < 1.1920929e-07f);
        float fThen25 =
          std::exp((0.0f - (fConst3 / (iSlow16 ? 1.0f : fSlow15))));
        float fSlow17 = (iSlow16 ? 0.0f : fThen25);
        float fZec77[32];
        float fRec47_tmp[36];
        float* fRec47 = &fRec47_tmp[4];
        float fSlow18 = (fConst1 * float(fHslider12));
        float fRec48_tmp[36];
        float* fRec48 = &fRec48_tmp[4];
        float fSlow19 = (0.00999999978f * float(fHslider13));
        float fSlow20 = (1.0f - fSlow19);
        float fZec78[32];
        float fZec79[32];
        float fZec80[32];
        float fZec81[32];
        int iZec82[32];
        float fZec83[32];
        float fZec84[32];
        float fSlow21 = (fConst1 * float(fHslider14));
        float fRec59_tmp[36];
        float* fRec59 = &fRec59_tmp[4];
        float fZec85[32];
        float fZec86[32];
        float fRec58_tmp[36];
        float* fRec58 = &fRec58_tmp[4];
        float fRec57_tmp[36];
        float* fRec57 = &fRec57_tmp[4];
        float fZec87[32];
        float fRec61_tmp[36];
        float* fRec61 = &fRec61_tmp[4];
        float fRec60_tmp[36];
        float* fRec60 = &fRec60_tmp[4];
        float fZec88[32];
        float fZec89[32];
        int iZec90[32];
        float fZec91[32];
        float fRec56_tmp[36];
        float* fRec56 = &fRec56_tmp[4];
        float fZec92[32];
        float fRec62_tmp[36];
        float* fRec62 = &fRec62_tmp[4];
        float fZec93[32];
        float fZec94[32];
        float fZec95[32];
        float fRec64_tmp[36];
        float* fRec64 = &fRec64_tmp[4];
        float fRec63_tmp[36];
        float* fRec63 = &fRec63_tmp[4];
        float fZec96[32];
        float fZec97[32];
        float fZec98[32];
        float fRec65_tmp[36];
        float* fRec65 = &fRec65_tmp[4];
        float fRec55_tmp[36];
        float* fRec55 = &fRec55_tmp[4];
        float fRec54_tmp[36];
        float* fRec54 = &fRec54_tmp[4];
        float fZec99[32];
        float fRec66_tmp[36];
        float* fRec66 = &fRec66_tmp[4];
        float fZec100[32];
        float fZec101[32];
        int iZec102[32];
        float fZec103[32];
        float fRec52_tmp[36];
        float* fRec52 = &fRec52_tmp[4];
        float fRec53[32];
        float fZec104[32];
        float fRec67_tmp[36];
        float* fRec67 = &fRec67_tmp[4];
        float fZec105[32];
        float fZec106[32];
        int iZec107[32];
        float fZec108[32];
        float fRec50_tmp[36];
        float* fRec50 = &fRec50_tmp[4];
        float fRec51[32];
        float fZec109[32];
        float fRec72_tmp[36];
        float* fRec72 = &fRec72_tmp[4];
        float fSlow22 = (fConst47 * fSlow10);
        float fZec110[32];
        float fZec111[32];
        float fRec68_tmp[36];
        float* fRec68 = &fRec68_tmp[4];
        float fRec69_tmp[36];
        float* fRec69 = &fRec69_tmp[4];
        float fRec70_tmp[36];
        float* fRec70 = &fRec70_tmp[4];
        float fRec71_tmp[36];
        float* fRec71 = &fRec71_tmp[4];
        float fZec112[32];
        float fZec113[32];
        float fZec114[32];
        float fRec73_tmp[36];
        float* fRec73 = &fRec73_tmp[4];
        float fZec115[32];
        float fZec116[32];
        float fZec117[32];
        float fRec75_tmp[36];
        float* fRec75 = &fRec75_tmp[4];
        float fRec74_tmp[36];
        float* fRec74 = &fRec74_tmp[4];
        float fZec118[32];
        float fZec119[32];
        float fRec76_tmp[36];
        float* fRec76 = &fRec76_tmp[4];
        float fZec120[32];
        int iZec121[32];
        float fZec122[32];
        int vindex = 0;
        /* Main loop */
        for (vindex = 0; (vindex <= (count - 32)); vindex = (vindex + 32))
        {
            FAUSTFLOAT* input0  = &input0_ptr[vindex];
            FAUSTFLOAT* input1  = &input1_ptr[vindex];
            FAUSTFLOAT* output0 = &output0_ptr[vindex];
            FAUSTFLOAT* output1 = &output1_ptr[vindex];
            int vsize           = 32;
            /* Vectorizable loop 0 */
            /* Pre code */
            for (int j10 = 0; (j10 < 4); j10 = (j10 + 1))
            {
                iVec0_tmp[j10] = iVec0_perm[j10];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iVec0[i] = 1;
            }
            /* Post code */
            for (int j11 = 0; (j11 < 4); j11 = (j11 + 1))
            {
                iVec0_perm[j11] = iVec0_tmp[(vsize + j11)];
            }
            /* Vectorizable loop 1 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec0[i] = (1 - iVec0[(i - 1)]);
            }
            /* Recursive loop 2 */
            /* Pre code */
            for (int j40 = 0; (j40 < 4); j40 = (j40 + 1))
            {
                fRec27_tmp[j40] = fRec27_perm[j40];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen7 = (fConst15 + fRec27[(i - 1)]);
                fZec20[i]    = (iZec0[i] ? 0.0f : fThen7);
                fRec27[i]    = (fZec20[i] - std::floor(fZec20[i]));
            }
            /* Post code */
            for (int j41 = 0; (j41 < 4); j41 = (j41 + 1))
            {
                fRec27_perm[j41] = fRec27_tmp[(vsize + j41)];
            }
            /* Recursive loop 3 */
            /* Pre code */
            for (int j6 = 0; (j6 < 4); j6 = (j6 + 1))
            {
                fRec11_tmp[j6] = fRec11_perm[j6];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec11[i] = (fSlow3 + (fConst2 * fRec11[(i - 1)]));
            }
            /* Post code */
            for (int j7 = 0; (j7 < 4); j7 = (j7 + 1))
            {
                fRec11_perm[j7] = fRec11_tmp[(vsize + j7)];
            }
            /* Recursive loop 4 */
            /* Pre code */
            for (int j8 = 0; (j8 < 4); j8 = (j8 + 1))
            {
                iRec13_tmp[j8] = iRec13_perm[j8];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iRec13[i] = ((1103515245 * iRec13[(i - 1)]) + 12345);
            }
            /* Post code */
            for (int j9 = 0; (j9 < 4); j9 = (j9 + 1))
            {
                iRec13_perm[j9] = iRec13_tmp[(vsize + j9)];
            }
            /* Recursive loop 5 */
            /* Pre code */
            for (int j98 = 0; (j98 < 4); j98 = (j98 + 1))
            {
                fRec59_tmp[j98] = fRec59_perm[j98];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec59[i] = (fSlow21 + (fConst2 * fRec59[(i - 1)]));
            }
            /* Post code */
            for (int j99 = 0; (j99 < 4); j99 = (j99 + 1))
            {
                fRec59_perm[j99] = fRec59_tmp[(vsize + j99)];
            }
            /* Vectorizable loop 6 */
            /* Pre code */
            for (int j42 = 0; (j42 < 4); j42 = (j42 + 1))
            {
                fYec1_tmp[j42] = fYec1_perm[j42];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fYec1[i] = (fRec27[i] - fRec27[(i - 1)]);
            }
            /* Post code */
            for (int j43 = 0; (j43 < 4); j43 = (j43 + 1))
            {
                fYec1_perm[j43] = fYec1_tmp[(vsize + j43)];
            }
            /* Recursive loop 7 */
            /* Pre code */
            for (int j46 = 0; (j46 < 4); j46 = (j46 + 1))
            {
                fRec30_tmp[j46] = fRec30_perm[j46];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen8 = (fConst18 + fRec30[(i - 1)]);
                fZec22[i]    = (iZec0[i] ? 0.0f : fThen8);
                fRec30[i]    = (fZec22[i] - std::floor(fZec22[i]));
            }
            /* Post code */
            for (int j47 = 0; (j47 < 4); j47 = (j47 + 1))
            {
                fRec30_perm[j47] = fRec30_tmp[(vsize + j47)];
            }
            /* Vectorizable loop 8 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec1[i] = (1.99000001f * fRec11[i]);
            }
            /* Vectorizable loop 9 */
            /* Pre code */
            for (int j14 = 0; (j14 < 4); j14 = (j14 + 1))
            {
                fYec0_tmp[j14] = fYec0_perm[j14];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fYec0[i] = float(iRec13[i]);
            }
            /* Post code */
            for (int j15 = 0; (j15 < 4); j15 = (j15 + 1))
            {
                fYec0_perm[j15] = fYec0_tmp[(vsize + j15)];
            }
            /* Vectorizable loop 10 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec4[i] = (18.0f * fRec11[i]);
            }
            /* Recursive loop 11 */
            /* Pre code */
            for (int j20 = 0; (j20 < 4); j20 = (j20 + 1))
            {
                fRec17_tmp[j20] = fRec17_perm[j20];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen2 = (fConst4 + fRec17[(i - 1)]);
                fZec6[i]     = (iZec0[i] ? 0.0f : fThen2);
                fRec17[i]    = (fZec6[i] - std::floor(fZec6[i]));
            }
            /* Post code */
            for (int j21 = 0; (j21 < 4); j21 = (j21 + 1))
            {
                fRec17_perm[j21] = fRec17_tmp[(vsize + j21)];
            }
            /* Vectorizable loop 12 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec85[i] = (0.200000003f * fRec59[i]);
            }
            /* Vectorizable loop 13 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec21[i] = ((fYec1[(i - 1)] <= 0.0f) & (fYec1[i] > 0.0f));
            }
            /* Vectorizable loop 14 */
            /* Pre code */
            for (int j48 = 0; (j48 < 4); j48 = (j48 + 1))
            {
                fYec2_tmp[j48] = fYec2_perm[j48];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fYec2[i] = Macalla_faustpower2_f(((2.0f * fRec30[i]) + -1.0f));
            }
            /* Post code */
            for (int j49 = 0; (j49 < 4); j49 = (j49 + 1))
            {
                fYec2_perm[j49] = fYec2_tmp[(vsize + j49)];
            }
            /* Recursive loop 15 */
            /* Pre code */
            for (int j76 = 0; (j76 < 4); j76 = (j76 + 1))
            {
                fRec42_tmp[j76] = fRec42_perm[j76];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec42[i] = (fSlow9 + (fConst2 * fRec42[(i - 1)]));
            }
            /* Post code */
            for (int j77 = 0; (j77 < 4); j77 = (j77 + 1))
            {
                fRec42_perm[j77] = fRec42_tmp[(vsize + j77)];
            }
            /* Recursive loop 16 */
            /* Pre code */
            for (int j12 = 0; (j12 < 4); j12 = (j12 + 1))
            {
                fRec14_tmp[j12] = fRec14_perm[j12];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen0 =
                  (fRec14[(i - 1)] + (fConst3 * (fZec1[i] + 0.00999999978f)));
                fZec2[i]  = (iZec0[i] ? 0.0f : fThen0);
                fRec14[i] = (fZec2[i] - std::floor(fZec2[i]));
            }
            /* Post code */
            for (int j13 = 0; (j13 < 4); j13 = (j13 + 1))
            {
                fRec14_perm[j13] = fRec14_tmp[(vsize + j13)];
            }
            /* Vectorizable loop 17 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec3[i] = (4.65661277e-12f * (fRec11[i] * fYec0[i]));
            }
            /* Recursive loop 18 */
            /* Pre code */
            for (int j18 = 0; (j18 < 4); j18 = (j18 + 1))
            {
                fRec16_tmp[j18] = fRec16_perm[j18];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen1 =
                  (fRec16[(i - 1)] + (fConst3 * (fZec4[i] + 2.0f)));
                fZec5[i]  = (iZec0[i] ? 0.0f : fThen1);
                fRec16[i] = (fZec5[i] - std::floor(fZec5[i]));
            }
            /* Post code */
            for (int j19 = 0; (j19 < 4); j19 = (j19 + 1))
            {
                fRec16_perm[j19] = fRec16_tmp[(vsize + j19)];
            }
            /* Vectorizable loop 19 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec7[i] = ((0.200000003f *
                             ftbl0MacallaSIG0[int((65536.0f * fRec17[i]))]) +
                            0.800000012f);
            }
            /* Recursive loop 20 */
            /* Pre code */
            for (int j100 = 0; (j100 < 4); j100 = (j100 + 1))
            {
                fRec58_tmp[j100] = fRec58_perm[j100];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen29 =
                  (fRec58[(i - 1)] +
                   (fConst3 * ((fZec1[i] + fZec85[i]) + 0.00999999978f)));
                fZec86[i] = (iZec0[i] ? 0.0f : fThen29);
                fRec58[i] = (fZec86[i] - std::floor(fZec86[i]));
            }
            /* Post code */
            for (int j101 = 0; (j101 < 4); j101 = (j101 + 1))
            {
                fRec58_perm[j101] = fRec58_tmp[(vsize + j101)];
            }
            /* Recursive loop 21 */
            /* Pre code */
            for (int j104 = 0; (j104 < 4); j104 = (j104 + 1))
            {
                fRec61_tmp[j104] = fRec61_perm[j104];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen30 = (fRec61[(i - 1)] +
                                 (fConst3 * ((fZec4[i] + fZec85[i]) + 2.0f)));
                fZec87[i]     = (iZec0[i] ? 0.0f : fThen30);
                fRec61[i]     = (fZec87[i] - std::floor(fZec87[i]));
            }
            /* Post code */
            for (int j105 = 0; (j105 < 4); j105 = (j105 + 1))
            {
                fRec61_perm[j105] = fRec61_tmp[(vsize + j105)];
            }
            /* Recursive loop 22 */
            /* Pre code */
            for (int j44 = 0; (j44 < 4); j44 = (j44 + 1))
            {
                fRec28_tmp[j44] = fRec28_perm[j44];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec28[i] = ((fRec28[(i - 1)] * float((1 - iZec21[i]))) +
                             (4.65661287e-10f * (fYec0[i] * float(iZec21[i]))));
            }
            /* Post code */
            for (int j45 = 0; (j45 < 4); j45 = (j45 + 1))
            {
                fRec28_perm[j45] = fRec28_tmp[(vsize + j45)];
            }
            /* Vectorizable loop 23 */
            /* Pre code */
            fYec3_idx = ((fYec3_idx + fYec3_idx_save) & 2047);
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fYec3[((i + fYec3_idx) & 2047)] =
                  (float(iVec0[(i - 1)]) * (fYec2[i] - fYec2[(i - 1)]));
            }
            /* Post code */
            fYec3_idx_save = vsize;
            /* Recursive loop 24 */
            /* Pre code */
            for (int j58 = 0; (j58 < 4); j58 = (j58 + 1))
            {
                fRec33_tmp[j58] = fRec33_perm[j58];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec33[i] = (fSlow6 + (fConst2 * fRec33[(i - 1)]));
            }
            /* Post code */
            for (int j59 = 0; (j59 < 4); j59 = (j59 + 1))
            {
                fRec33_perm[j59] = fRec33_tmp[(vsize + j59)];
            }
            /* Recursive loop 25 */
            /* Pre code */
            for (int j62 = 0; (j62 < 4); j62 = (j62 + 1))
            {
                fRec34_tmp[j62] = fRec34_perm[j62];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec34[i] = (fSlow7 + (fConst2 * fRec34[(i - 1)]));
            }
            /* Post code */
            for (int j63 = 0; (j63 < 4); j63 = (j63 + 1))
            {
                fRec34_perm[j63] = fRec34_tmp[(vsize + j63)];
            }
            /* Recursive loop 26 */
            /* Pre code */
            for (int j68 = 0; (j68 < 4); j68 = (j68 + 1))
            {
                fRec36_tmp[j68] = fRec36_perm[j68];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen11 = (fConst43 + fRec36[(i - 1)]);
                fZec40[i]     = (iZec0[i] ? 0.0f : fThen11);
                fRec36[i]     = (fZec40[i] - std::floor(fZec40[i]));
            }
            /* Post code */
            for (int j69 = 0; (j69 < 4); j69 = (j69 + 1))
            {
                fRec36_perm[j69] = fRec36_tmp[(vsize + j69)];
            }
            /* Recursive loop 27 */
            /* Pre code */
            for (int j72 = 0; (j72 < 4); j72 = (j72 + 1))
            {
                fRec37_tmp[j72] = fRec37_perm[j72];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen12 = (fConst48 + fRec37[(i - 1)]);
                fZec45[i]     = (iZec0[i] ? 0.0f : fThen12);
                fRec37[i]     = (fZec45[i] - std::floor(fZec45[i]));
            }
            /* Post code */
            for (int j73 = 0; (j73 < 4); j73 = (j73 + 1))
            {
                fRec37_perm[j73] = fRec37_tmp[(vsize + j73)];
            }
            /* Vectorizable loop 28 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec50[i] = (4.98999977f * fRec42[i]);
            }
            /* Recursive loop 29 */
            /* Pre code */
            for (int j2 = 0; (j2 < 4); j2 = (j2 + 1))
            {
                fRec2_tmp[j2] = fRec2_perm[j2];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec2[i] =
                  std::fmod(((fRec2[(i - 1)] + 4097.0f) - fSlow1), 4096.0f);
            }
            /* Post code */
            for (int j3 = 0; (j3 < 4); j3 = (j3 + 1))
            {
                fRec2_perm[j3] = fRec2_tmp[(vsize + j3)];
            }
            /* Recursive loop 30 */
            /* Pre code */
            for (int j16 = 0; (j16 < 4); j16 = (j16 + 1))
            {
                fRec12_tmp[j16] = fRec12_perm[j16];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec12[i] =
                  ((9.99999975e-05f *
                    (fZec3[i] +
                     (1.0f - std::fabs(((2.0f * fRec14[i]) + -1.0f))))) +
                   (0.999899983f * fRec12[(i - 1)]));
            }
            /* Post code */
            for (int j17 = 0; (j17 < 4); j17 = (j17 + 1))
            {
                fRec12_perm[j17] = fRec12_tmp[(vsize + j17)];
            }
            /* Recursive loop 31 */
            /* Pre code */
            for (int j22 = 0; (j22 < 4); j22 = (j22 + 1))
            {
                fRec15_tmp[j22] = fRec15_perm[j22];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec15[i] =
                  ((9.99999975e-05f *
                    ((fZec3[i] +
                      (1.0f - std::fabs(((2.0f * fRec16[i]) + -1.0f)))) *
                     fZec7[i])) +
                   (0.999899983f * fRec15[(i - 1)]));
            }
            /* Post code */
            for (int j23 = 0; (j23 < 4); j23 = (j23 + 1))
            {
                fRec15_perm[j23] = fRec15_tmp[(vsize + j23)];
            }
            /* Recursive loop 32 */
            /* Pre code */
            for (int j102 = 0; (j102 < 4); j102 = (j102 + 1))
            {
                fRec57_tmp[j102] = fRec57_perm[j102];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec57[i] =
                  ((9.99999975e-05f *
                    (fZec3[i] +
                     (1.0f - std::fabs(((2.0f * fRec58[i]) + -1.0f))))) +
                   (0.999899983f * fRec57[(i - 1)]));
            }
            /* Post code */
            for (int j103 = 0; (j103 < 4); j103 = (j103 + 1))
            {
                fRec57_perm[j103] = fRec57_tmp[(vsize + j103)];
            }
            /* Recursive loop 33 */
            /* Pre code */
            for (int j106 = 0; (j106 < 4); j106 = (j106 + 1))
            {
                fRec60_tmp[j106] = fRec60_perm[j106];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec60[i] =
                  ((9.99999975e-05f *
                    (fZec7[i] *
                     (fZec3[i] +
                      (1.0f - std::fabs(((2.0f * fRec61[i]) + -1.0f)))))) +
                   (0.999899983f * fRec60[(i - 1)]));
            }
            /* Post code */
            for (int j107 = 0; (j107 < 4); j107 = (j107 + 1))
            {
                fRec60_perm[j107] = fRec60_tmp[(vsize + j107)];
            }
            /* Recursive loop 34 */
            /* Pre code */
            for (int j122 = 0; (j122 < 4); j122 = (j122 + 1))
            {
                fRec66_tmp[j122] = fRec66_perm[j122];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen35 =
                  (fRec66[(i - 1)] +
                   (fConst3 * ((0.0199999996f * fRec59[i]) + 0.150000006f)));
                fZec99[i] = (iZec0[i] ? 0.0f : fThen35);
                fRec66[i] = (fZec99[i] - std::floor(fZec99[i]));
            }
            /* Post code */
            for (int j123 = 0; (j123 < 4); j123 = (j123 + 1))
            {
                fRec66_perm[j123] = fRec66_tmp[(vsize + j123)];
            }
            /* Recursive loop 35 */
            /* Pre code */
            for (int j126 = 0; (j126 < 4); j126 = (j126 + 1))
            {
                fRec67_tmp[j126] = fRec67_perm[j126];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen36 =
                  (fRec67[(i - 1)] +
                   (fConst3 * ((0.0299999993f * fRec59[i]) + 0.230000004f)));
                fZec104[i] = (iZec0[i] ? 0.0f : fThen36);
                fRec67[i]  = (fZec104[i] - std::floor(fZec104[i]));
            }
            /* Post code */
            for (int j127 = 0; (j127 < 4); j127 = (j127 + 1))
            {
                fRec67_perm[j127] = fRec67_tmp[(vsize + j127)];
            }
            /* Recursive loop 36 */
            /* Pre code */
            for (int j38 = 0; (j38 < 4); j38 = (j38 + 1))
            {
                fRec26_tmp[j38] = fRec26_perm[j38];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec26[i] = (((0.522189379f * fRec26[(i - 3)]) +
                              ((4.65661287e-10f * fYec0[i]) +
                               (2.49495602f * fRec26[(i - 1)]))) -
                             (2.0172658f * fRec26[(i - 2)]));
            }
            /* Post code */
            for (int j39 = 0; (j39 < 4); j39 = (j39 + 1))
            {
                fRec26_perm[j39] = fRec26_tmp[(vsize + j39)];
            }
            /* Recursive loop 37 */
            /* Pre code */
            for (int j50 = 0; (j50 < 4); j50 = (j50 + 1))
            {
                fRec29_tmp[j50] = fRec29_perm[j50];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec29[i] =
                  ((0.999000013f * fRec29[(i - 1)]) +
                   (fConst19 *
                    ((fYec3[((i + fYec3_idx) & 2047)] -
                      (fConst22 *
                       fYec3[(((i + fYec3_idx) - iConst23) & 2047)])) -
                     (fConst24 *
                      fYec3[(((i + fYec3_idx) - iConst25) & 2047)]))));
            }
            /* Post code */
            for (int j51 = 0; (j51 < 4); j51 = (j51 + 1))
            {
                fRec29_perm[j51] = fRec29_tmp[(vsize + j51)];
            }
            /* Recursive loop 38 */
            /* Pre code */
            for (int j52 = 0; (j52 < 4); j52 = (j52 + 1))
            {
                fRec31_tmp[j52] = fRec31_perm[j52];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen9 = (fConst26 + fRec31[(i - 1)]);
                fZec23[i]    = (iZec0[i] ? 0.0f : fThen9);
                fRec31[i]    = (fZec23[i] - std::floor(fZec23[i]));
            }
            /* Post code */
            for (int j53 = 0; (j53 < 4); j53 = (j53 + 1))
            {
                fRec31_perm[j53] = fRec31_tmp[(vsize + j53)];
            }
            /* Recursive loop 39 */
            /* Pre code */
            for (int j54 = 0; (j54 < 4); j54 = (j54 + 1))
            {
                fRec32_tmp[j54] = fRec32_perm[j54];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen10 = (fConst27 + fRec32[(i - 1)]);
                fZec24[i]     = (iZec0[i] ? 0.0f : fThen10);
                fRec32[i]     = (fZec24[i] - std::floor(fZec24[i]));
            }
            /* Post code */
            for (int j55 = 0; (j55 < 4); j55 = (j55 + 1))
            {
                fRec32_perm[j55] = fRec32_tmp[(vsize + j55)];
            }
            /* Vectorizable loop 40 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec25[i] = (0.5f * (fRec28[i] + 1.0f));
            }
            /* Vectorizable loop 41 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec28[i] = std::tan((fConst41 * fRec33[i]));
            }
            /* Vectorizable loop 42 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec35[i] = std::tan((fConst41 * fRec34[i]));
            }
            /* Vectorizable loop 43 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec41[i] = std::max<float>(
                  0.0f,
                  std::min<float>(
                    fConst46,
                    (fConst47 *
                     ((1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec36[i]) +
                                                          -1.0f)))) +
                               -1.0f)) +
                      7.0f))));
            }
            /* Vectorizable loop 44 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec46[i] = std::max<float>(
                  0.0f,
                  std::min<float>(
                    fConst46,
                    (fConst47 *
                     ((1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec37[i]) +
                                                          -1.0f)))) +
                               -1.0f)) +
                      11.3000002f))));
            }
            /* Recursive loop 45 */
            /* Pre code */
            for (int j78 = 0; (j78 < 4); j78 = (j78 + 1))
            {
                fRec43_tmp[j78] = fRec43_perm[j78];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen13 =
                  (fRec43[(i - 1)] + (fConst3 * (fZec50[i] + 0.00999999978f)));
                fZec51[i] = (iZec0[i] ? 0.0f : fThen13);
                fRec43[i] = (fZec51[i] - std::floor(fZec51[i]));
            }
            /* Post code */
            for (int j79 = 0; (j79 < 4); j79 = (j79 + 1))
            {
                fRec43_perm[j79] = fRec43_tmp[(vsize + j79)];
            }
            /* Vectorizable loop 46 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec55[i] = (fRec2[i] + 4096.0f);
            }
            /* Recursive loop 47 */
            /* Pre code */
            for (int j0 = 0; (j0 < 4); j0 = (j0 + 1))
            {
                fRec1_tmp[j0] = fRec1_perm[j0];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec1[i] = (fSlow0 + (fConst2 * fRec1[(i - 1)]));
            }
            /* Post code */
            for (int j1 = 0; (j1 < 4); j1 = (j1 + 1))
            {
                fRec1_perm[j1] = fRec1_tmp[(vsize + j1)];
            }
            /* Vectorizable loop 48 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec8[i] =
                  (fRec11[i] * ((600.0f * fRec12[i]) + (100.0f * fRec15[i])));
            }
            /* Vectorizable loop 49 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec88[i] =
                  (fRec11[i] * ((600.0f * fRec57[i]) + (100.0f * fRec60[i])));
            }
            /* Vectorizable loop 50 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec100[i] = std::max<float>(
                  0.0f,
                  std::min<float>(
                    fConst46,
                    (fConst47 *
                     (((0.5f * fRec59[i]) +
                       (1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec66[i]) +
                                                           -1.0f)))) +
                                -1.0f))) +
                      7.0f))));
            }
            /* Vectorizable loop 51 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec105[i] = std::max<float>(
                  0.0f,
                  std::min<float>(
                    fConst46,
                    (fConst47 *
                     (((0.699999988f * fRec59[i]) +
                       (1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec67[i]) +
                                                           -1.0f)))) +
                                -1.0f))) +
                      11.3000002f))));
            }
            /* Recursive loop 52 */
            /* Pre code */
            for (int j130 = 0; (j130 < 4); j130 = (j130 + 1))
            {
                fRec72_tmp[j130] = fRec72_perm[j130];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen37 = (fRec72[(i - 1)] +
                                 (fConst3 * ((fZec50[i] + (2.0f * fRec59[i])) +
                                             0.00999999978f)));
                fZec109[i]    = (iZec0[i] ? 0.0f : fThen37);
                fRec72[i]     = (fZec109[i] - std::floor(fZec109[i]));
            }
            /* Post code */
            for (int j131 = 0; (j131 < 4); j131 = (j131 + 1))
            {
                fRec72_perm[j131] = fRec72_tmp[(vsize + j131)];
            }
            /* Recursive loop 53 */
            /* Pre code */
            for (int j26 = 0; (j26 < 4); j26 = (j26 + 1))
            {
                iRec20_tmp[j26] = iRec20_perm[j26];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iRec20[i] = (iRec20[(i - 1)] + 1);
            }
            /* Post code */
            for (int j27 = 0; (j27 < 4); j27 = (j27 + 1))
            {
                iRec20_perm[j27] = iRec20_tmp[(vsize + j27)];
            }
            /* Recursive loop 54 */
            /* Pre code */
            for (int j36 = 0; (j36 < 4); j36 = (j36 + 1))
            {
                fRec24_tmp[j36] = fRec24_perm[j36];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec24[i] = (fSlow5 + (fConst2 * fRec24[(i - 1)]));
            }
            /* Post code */
            for (int j37 = 0; (j37 < 4); j37 = (j37 + 1))
            {
                fRec24_perm[j37] = fRec24_tmp[(vsize + j37)];
            }
            /* Recursive loop 55 */
            /* Pre code */
            for (int j56 = 0; (j56 < 4); j56 = (j56 + 1))
            {
                fRec25_tmp[j56] = fRec25_perm[j56];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec25[i] =
                  (((((1.20000005f * (((0.0499220341f * fRec26[i]) +
                                       (0.0506126992f * fRec26[(i - 2)])) -
                                      ((0.0959935337f * fRec26[(i - 1)]) +
                                       (0.00440878607f * fRec26[(i - 3)])))) +
                      (2.32830644e-10f *
                       (fYec0[(i - 1)] *
                        float(((fRec27[i] >= fZec25[i]) *
                               (fRec27[(i - 1)] < fZec25[i])))))) +
                     (fConst28 * fRec29[i])) +
                    (0.00999999978f *
                     (ftbl0MacallaSIG0[int((65536.0f * fRec31[i]))] +
                      ftbl0MacallaSIG0[int((65536.0f * fRec32[i]))]))) -
                   (fConst36 * ((fConst37 * fRec25[(i - 1)]) +
                                (fConst38 * fRec25[(i - 2)]))));
            }
            /* Post code */
            for (int j57 = 0; (j57 < 4); j57 = (j57 + 1))
            {
                fRec25_perm[j57] = fRec25_tmp[(vsize + j57)];
            }
            /* Vectorizable loop 56 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec29[i] = (1.0f / fZec28[i]);
            }
            /* Vectorizable loop 57 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec31[i] = Macalla_faustpower2_f(fZec28[i]);
            }
            /* Vectorizable loop 58 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec36[i] = (1.0f / fZec35[i]);
            }
            /* Vectorizable loop 59 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec42[i] = (fZec41[i] + -1.0f);
            }
            /* Vectorizable loop 60 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec47[i] = (fZec46[i] + -1.0f);
            }
            /* Vectorizable loop 61 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec52[i] = std::min<float>(
                  524288.0f,
                  (fConst0 *
                   (fSlow11 +
                    (fRec42[i] *
                     std::fabs(((2.0f * (1.0f - std::fabs(((2.0f * fRec43[i]) +
                                                           -1.0f)))) +
                                -1.0f))))));
            }
            /* Vectorizable loop 62 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec54[i] = std::pow(10.0f, (0.0500000007f * fRec1[i]));
            }
            /* Vectorizable loop 63 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec56[i] = int(fZec55[i]);
            }
            /* Vectorizable loop 64 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec58[i] = std::floor(fZec55[i]);
            }
            /* Vectorizable loop 65 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec62[i] = std::min<float>((0.001953125f * fRec2[i]), 1.0f);
            }
            /* Vectorizable loop 66 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec64[i] = int(fRec2[i]);
            }
            /* Vectorizable loop 67 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec66[i] = std::floor(fRec2[i]);
            }
            /* Vectorizable loop 68 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec9[i] = (fZec8[i] + 100.000008f);
            }
            /* Vectorizable loop 69 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec89[i] = (fZec88[i] + 100.000008f);
            }
            /* Vectorizable loop 70 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec101[i] = (fZec100[i] + -1.0f);
            }
            /* Vectorizable loop 71 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec106[i] = (fZec105[i] + -1.0f);
            }
            /* Vectorizable loop 72 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec110[i] = std::min<float>(
                  524288.0f,
                  (fSlow22 +
                   ((40.0f * fRec59[i]) +
                    ((fRec42[i] * (fConst0 + (1000.0f * fRec59[i]))) *
                     std::fabs(((2.0f * (1.0f - std::fabs(((2.0f * fRec72[i]) +
                                                           -1.0f)))) +
                                -1.0f))))));
            }
            /* Vectorizable loop 73 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec12[i] = (((iRec20[i] + -1) % iConst10) == 0);
            }
            /* Vectorizable loop 74 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec26[i] = Macalla_faustpower2_f(fRec24[i]);
            }
            /* Vectorizable loop 75 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec27[i] =
                  ((fConst39 * fRec25[i]) + (fConst40 * fRec25[(i - 2)]));
            }
            /* Vectorizable loop 76 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec30[i] = (((fZec29[i] + -1.41421354f) / fZec28[i]) + 1.0f);
            }
            /* Vectorizable loop 77 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec32[i] = (1.0f - (1.0f / fZec31[i]));
            }
            /* Vectorizable loop 78 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec33[i] = (((fZec29[i] + 1.41421354f) / fZec28[i]) + 1.0f);
            }
            /* Vectorizable loop 79 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec37[i] = (((fZec36[i] + -1.41421354f) / fZec35[i]) + 1.0f);
            }
            /* Vectorizable loop 80 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec34[i] = (0.0f - (2.0f / fZec31[i]));
            }
            /* Vectorizable loop 81 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec38[i] = (1.0f - (1.0f / Macalla_faustpower2_f(fZec35[i])));
            }
            /* Vectorizable loop 82 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec39[i] = (((fZec36[i] + 1.41421354f) / fZec35[i]) + 1.0f);
            }
            /* Recursive loop 83 */
            /* Pre code */
            for (int j66 = 0; (j66 < 4); j66 = (j66 + 1))
            {
                fRec35_tmp[j66] = fRec35_perm[j66];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec35[i] = (fSlow8 + (fConst2 * fRec35[(i - 1)]));
            }
            /* Post code */
            for (int j67 = 0; (j67 < 4); j67 = (j67 + 1))
            {
                fRec35_perm[j67] = fRec35_tmp[(vsize + j67)];
            }
            /* Vectorizable loop 84 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec43[i] = int(fZec42[i]);
            }
            /* Vectorizable loop 85 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec44[i] = std::floor(fZec42[i]);
            }
            /* Vectorizable loop 86 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec48[i] = int(fZec47[i]);
            }
            /* Vectorizable loop 87 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec49[i] = std::floor(fZec47[i]);
            }
            /* Recursive loop 88 */
            /* Pre code */
            for (int j80 = 0; (j80 < 4); j80 = (j80 + 1))
            {
                fRec38_tmp[j80] = fRec38_perm[j80];
            }
            for (int j82 = 0; (j82 < 4); j82 = (j82 + 1))
            {
                fRec39_tmp[j82] = fRec39_perm[j82];
            }
            for (int j84 = 0; (j84 < 4); j84 = (j84 + 1))
            {
                fRec40_tmp[j84] = fRec40_perm[j84];
            }
            for (int j86 = 0; (j86 < 4); j86 = (j86 + 1))
            {
                fRec41_tmp[j86] = fRec41_perm[j86];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen15 =
                  (((fRec39[(i - 1)] == 1.0f) & (fZec52[i] != fRec41[(i - 1)]))
                     ? fConst16
                     : 0.0f);
                float fElse16 = fRec38[(i - 1)];
                float fThen17 =
                  (((fRec39[(i - 1)] == 0.0f) & (fZec52[i] != fRec40[(i - 1)]))
                     ? fConst15
                     : fThen15);
                float fElse17 =
                  (((fRec39[(i - 1)] > 0.0f) & (fRec39[(i - 1)] < 1.0f))
                     ? fElse16
                     : 0.0f);
                fZec53[i] = ((fRec38[(i - 1)] != 0.0f) ? fElse17 : fThen17);
                fRec38[i] = fZec53[i];
                fRec39[i] = std::max<float>(
                  0.0f, std::min<float>(1.0f, (fRec39[(i - 1)] + fZec53[i])));
                float fThen18 = fRec40[(i - 1)];
                float fElse18 = fZec52[i];
                fRec40[i] =
                  (((fRec39[(i - 1)] >= 1.0f) & (fRec41[(i - 1)] != fZec52[i]))
                     ? fElse18
                     : fThen18);
                float fThen19 = fRec41[(i - 1)];
                float fElse19 = fZec52[i];
                fRec41[i] =
                  (((fRec39[(i - 1)] <= 0.0f) & (fRec40[(i - 1)] != fZec52[i]))
                     ? fElse19
                     : fThen19);
            }
            /* Post code */
            for (int j81 = 0; (j81 < 4); j81 = (j81 + 1))
            {
                fRec38_perm[j81] = fRec38_tmp[(vsize + j81)];
            }
            for (int j83 = 0; (j83 < 4); j83 = (j83 + 1))
            {
                fRec39_perm[j83] = fRec39_tmp[(vsize + j83)];
            }
            for (int j85 = 0; (j85 < 4); j85 = (j85 + 1))
            {
                fRec40_perm[j85] = fRec40_tmp[(vsize + j85)];
            }
            for (int j87 = 0; (j87 < 4); j87 = (j87 + 1))
            {
                fRec41_perm[j87] = fRec41_tmp[(vsize + j87)];
            }
            /* Vectorizable loop 89 */
            /* Pre code */
            fYec6_idx = ((fYec6_idx + fYec6_idx_save) & 131071);
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fYec6[((i + fYec6_idx) & 131071)] =
                  (float(input0[i]) * fZec54[i]);
            }
            /* Post code */
            fYec6_idx_save = vsize;
            /* Vectorizable loop 90 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec57[i] = std::min<int>(65537, std::max<int>(0, iZec56[i]));
            }
            /* Vectorizable loop 91 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec59[i] = (fZec58[i] + (-4095.0f - fRec2[i]));
            }
            /* Vectorizable loop 92 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec60[i] = (fRec2[i] + (4096.0f - fZec58[i]));
            }
            /* Vectorizable loop 93 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec61[i] =
                  std::min<int>(65537, std::max<int>(0, (iZec56[i] + 1)));
            }
            /* Vectorizable loop 94 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec63[i] = (1.0f - fZec62[i]);
            }
            /* Vectorizable loop 95 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec65[i] = std::min<int>(65537, std::max<int>(0, iZec64[i]));
            }
            /* Vectorizable loop 96 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec67[i] = (fZec66[i] + (1.0f - fRec2[i]));
            }
            /* Vectorizable loop 97 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec68[i] = (fRec2[i] - fZec66[i]);
            }
            /* Vectorizable loop 98 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec69[i] =
                  std::min<int>(65537, std::max<int>(0, (iZec64[i] + 1)));
            }
            /* Recursive loop 99 */
            /* Pre code */
            for (int j4 = 0; (j4 < 4); j4 = (j4 + 1))
            {
                fRec3_tmp[j4] = fRec3_perm[j4];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec3[i] = (fSlow2 + (fConst2 * fRec3[(i - 1)]));
            }
            /* Post code */
            for (int j5 = 0; (j5 < 4); j5 = (j5 + 1))
            {
                fRec3_perm[j5] = fRec3_tmp[(vsize + j5)];
            }
            /* Vectorizable loop 100 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec10[i] = int(fZec9[i]);
            }
            /* Vectorizable loop 101 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec11[i] = std::floor(fZec9[i]);
            }
            /* Vectorizable loop 102 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec90[i] = int(fZec89[i]);
            }
            /* Vectorizable loop 103 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec91[i] = std::floor(fZec89[i]);
            }
            /* Vectorizable loop 104 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec102[i] = int(fZec101[i]);
            }
            /* Vectorizable loop 105 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec103[i] = std::floor(fZec101[i]);
            }
            /* Vectorizable loop 106 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec107[i] = int(fZec106[i]);
            }
            /* Vectorizable loop 107 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec108[i] = std::floor(fZec106[i]);
            }
            /* Recursive loop 108 */
            /* Pre code */
            for (int j132 = 0; (j132 < 4); j132 = (j132 + 1))
            {
                fRec68_tmp[j132] = fRec68_perm[j132];
            }
            for (int j134 = 0; (j134 < 4); j134 = (j134 + 1))
            {
                fRec69_tmp[j134] = fRec69_perm[j134];
            }
            for (int j136 = 0; (j136 < 4); j136 = (j136 + 1))
            {
                fRec70_tmp[j136] = fRec70_perm[j136];
            }
            for (int j138 = 0; (j138 < 4); j138 = (j138 + 1))
            {
                fRec71_tmp[j138] = fRec71_perm[j138];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen39 =
                  (((fRec69[(i - 1)] == 1.0f) & (fZec110[i] != fRec71[(i - 1)]))
                     ? fConst16
                     : 0.0f);
                float fElse40 = fRec68[(i - 1)];
                float fThen41 =
                  (((fRec69[(i - 1)] == 0.0f) & (fZec110[i] != fRec70[(i - 1)]))
                     ? fConst15
                     : fThen39);
                float fElse41 =
                  (((fRec69[(i - 1)] > 0.0f) & (fRec69[(i - 1)] < 1.0f))
                     ? fElse40
                     : 0.0f);
                fZec111[i] = ((fRec68[(i - 1)] != 0.0f) ? fElse41 : fThen41);
                fRec68[i]  = fZec111[i];
                fRec69[i]  = std::max<float>(
                  0.0f, std::min<float>(1.0f, (fRec69[(i - 1)] + fZec111[i])));
                float fThen42 = fRec70[(i - 1)];
                float fElse42 = fZec110[i];
                fRec70[i] =
                  (((fRec69[(i - 1)] >= 1.0f) & (fRec71[(i - 1)] != fZec110[i]))
                     ? fElse42
                     : fThen42);
                float fThen43 = fRec71[(i - 1)];
                float fElse43 = fZec110[i];
                fRec71[i] =
                  (((fRec69[(i - 1)] <= 0.0f) & (fRec70[(i - 1)] != fZec110[i]))
                     ? fElse43
                     : fThen43);
            }
            /* Post code */
            for (int j133 = 0; (j133 < 4); j133 = (j133 + 1))
            {
                fRec68_perm[j133] = fRec68_tmp[(vsize + j133)];
            }
            for (int j135 = 0; (j135 < 4); j135 = (j135 + 1))
            {
                fRec69_perm[j135] = fRec69_tmp[(vsize + j135)];
            }
            for (int j137 = 0; (j137 < 4); j137 = (j137 + 1))
            {
                fRec70_perm[j137] = fRec70_tmp[(vsize + j137)];
            }
            for (int j139 = 0; (j139 < 4); j139 = (j139 + 1))
            {
                fRec71_perm[j139] = fRec71_tmp[(vsize + j139)];
            }
            /* Vectorizable loop 109 */
            /* Pre code */
            fYec10_idx = ((fYec10_idx + fYec10_idx_save) & 131071);
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fYec10[((i + fYec10_idx) & 131071)] =
                  (float(input1[i]) * fZec54[i]);
            }
            /* Post code */
            fYec10_idx_save = vsize;
            /* Recursive loop 110 */
            /* Pre code */
            for (int j24 = 0; (j24 < 4); j24 = (j24 + 1))
            {
                fRec10_tmp[j24] = fRec10_perm[j24];
            }
            for (int j28 = 0; (j28 < 4); j28 = (j28 + 1))
            {
                fRec19_tmp[j28] = fRec19_perm[j28];
            }
            for (int j30 = 0; (j30 < 4); j30 = (j30 + 1))
            {
                fRec22_tmp[j30] = fRec22_perm[j30];
            }
            for (int j32 = 0; (j32 < 4); j32 = (j32 + 1))
            {
                fRec21_tmp[j32] = fRec21_perm[j32];
            }
            for (int j34 = 0; (j34 < 4); j34 = (j34 + 1))
            {
                fRec23_tmp[j34] = fRec23_perm[j34];
            }
            for (int j60 = 0; (j60 < 4); j60 = (j60 + 1))
            {
                fRec9_tmp[j60] = fRec9_perm[j60];
            }
            for (int j64 = 0; (j64 < 4); j64 = (j64 + 1))
            {
                fRec8_tmp[j64] = fRec8_perm[j64];
            }
            fYec4_idx = ((fYec4_idx + fYec4_idx_save) & 2047);
            for (int j70 = 0; (j70 < 4); j70 = (j70 + 1))
            {
                fRec6_tmp[j70] = fRec6_perm[j70];
            }
            fYec5_idx = ((fYec5_idx + fYec5_idx_save) & 4095);
            for (int j74 = 0; (j74 < 4); j74 = (j74 + 1))
            {
                fRec4_tmp[j74] = fRec4_perm[j74];
            }
            fYec7_idx = ((fYec7_idx + fYec7_idx_save) & 1048575);
            fRec0_idx = ((fRec0_idx + fRec0_idx_save) & 262143);
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec10[i] =
                  (((fRec0[(((i + fRec0_idx) -
                             (int(std::min<float>(
                                fConst0, float(std::max<int>(0, iZec10[i])))) +
                              1)) &
                            262143)] *
                     (0.0f - (fZec8[i] + (99.0f - fZec11[i])))) +
                    ((fZec8[i] + (100.0f - fZec11[i])) *
                     fRec0[(
                       ((i + fRec0_idx) -
                        (int(std::min<float>(
                           fConst0, float(std::max<int>(0, (iZec10[i] + 1))))) +
                         1)) &
                       262143)])) -
                   (fConst7 * ((fConst8 * fRec10[(i - 2)]) +
                               (fConst9 * fRec10[(i - 1)]))));
                fZec13[i] =
                  (fConst7 *
                   (fRec10[(i - 2)] + (fRec10[i] + (2.0f * fRec10[(i - 1)]))));
                float fThen3 = fRec19[(i - 1)];
                float fElse3 =
                  (0.00392156886f *
                   (std::floor(((255.0f * std::fabs(fZec13[i])) + 0.5f)) *
                    (float((2 * (fZec13[i] > 0.0f))) + -1.0f)));
                fRec19[i]    = (iZec12[i] ? fElse3 : fThen3);
                float fThen4 = fZec13[i];
                float fElse4 = fRec19[i];
                fZec14[i]    = (iSlow4 ? fElse4 : fThen4);
                fZec15[i]    = std::fabs(fZec14[i]);
                fZec16[i] =
                  ((fZec15[i] > fRec22[(i - 1)]) ? fConst12 : fConst11);
                fRec22[i] = ((fZec15[i] * (1.0f - fZec16[i])) +
                             (fRec22[(i - 1)] * fZec16[i]));
                fRec21[i] =
                  ((fConst14 *
                    (0.0f -
                     (0.75f * std::max<float>(
                                ((20.0f * std::log10(std::max<float>(
                                            1.17549435e-38f, fRec22[i]))) +
                                 6.0f),
                                0.0f)))) +
                   (fConst13 * fRec21[(i - 1)]));
                fZec17[i] =
                  (fZec14[i] * std::pow(10.0f, (0.0500000007f * fRec21[i])));
                fZec18[i] = std::fabs((fZec17[i] + 9.99999975e-05f));
                fZec19[i] =
                  ((fZec18[i] > fRec23[(i - 1)]) ? fConst17 : fConst11);
                fRec23[i] = ((fZec18[i] * (1.0f - fZec19[i])) +
                             (fRec23[(i - 1)] * fZec19[i]));
                fRec9[i] =
                  ((fZec17[i] + ((fRec23[i] * fZec26[i]) * fZec27[i])) -
                   (((fRec9[(i - 2)] * fZec30[i]) +
                     (2.0f * (fRec9[(i - 1)] * fZec32[i]))) /
                    fZec33[i]));
                fRec8[i] =
                  (((((fRec9[(i - 1)] * fZec34[i]) + (fRec9[i] / fZec31[i])) +
                     (fRec9[(i - 2)] / fZec31[i])) /
                    fZec33[i]) -
                   (((fRec8[(i - 2)] * fZec37[i]) +
                     (2.0f * (fRec8[(i - 1)] * fZec38[i]))) /
                    fZec39[i]));
                fYec4[((i + fYec4_idx) & 2047)] =
                  ((0.980000019f *
                    ((fRec8[(i - 2)] + (fRec8[i] + (2.0f * fRec8[(i - 1)]))) /
                     fZec39[i])) -
                   (fRec35[i] * fRec6[(i - 1)]));
                fRec6[i] =
                  ((fYec4[(
                      ((i + fYec4_idx) -
                       std::min<int>(iConst45, std::max<int>(0, iZec43[i]))) &
                      2047)] *
                    (fZec44[i] + (2.0f - fZec41[i]))) +
                   ((fZec41[i] + (-1.0f - fZec44[i])) *
                    fYec4[(((i + fYec4_idx) -
                            std::min<int>(iConst45,
                                          std::max<int>(0, (iZec43[i] + 1)))) &
                           2047)]));
                fRec7[i] = (fRec35[i] * fYec4[((i + fYec4_idx) & 2047)]);
                fYec5[((i + fYec5_idx) & 4095)] =
                  ((fRec7[i] + fRec6[(i - 1)]) -
                   (0.899999976f * (fRec35[i] * fRec4[(i - 1)])));
                fRec4[i] =
                  ((fYec5[(
                      ((i + fYec5_idx) -
                       std::min<int>(iConst45, std::max<int>(0, iZec48[i]))) &
                      4095)] *
                    (fZec49[i] + (2.0f - fZec46[i]))) +
                   ((fZec46[i] + (-1.0f - fZec49[i])) *
                    fYec5[(((i + fYec5_idx) -
                            std::min<int>(iConst45,
                                          std::max<int>(0, (iZec48[i] + 1)))) &
                           4095)]));
                fRec5[i] = (0.899999976f *
                            (fRec35[i] * fYec5[((i + fYec5_idx) & 4095)]));
                fYec7[((i + fYec7_idx) & 1048575)] =
                  ((((fYec6[(((i + fYec6_idx) - iZec57[i]) & 131071)] *
                      fZec59[i]) +
                     (fZec60[i] *
                      fYec6[(((i + fYec6_idx) - iZec61[i]) & 131071)])) *
                    fZec63[i]) +
                   ((fRec3[i] * (fRec5[i] + fRec4[(i - 1)])) +
                    (((fYec6[(((i + fYec6_idx) - iZec65[i]) & 131071)] *
                       fZec67[i]) +
                      (fZec68[i] *
                       fYec6[(((i + fYec6_idx) - iZec69[i]) & 131071)])) *
                     fZec62[i])));
                fZec70[i] =
                  fYec7[(((i + fYec7_idx) -
                          int(std::min<float>(
                            524288.0f, std::max<float>(0.0f, fRec40[i])))) &
                         1048575)];
                fRec0[((i + fRec0_idx) & 262143)] =
                  (fZec70[i] +
                   (fRec39[i] *
                    (fYec7[(((i + fYec7_idx) -
                             int(std::min<float>(
                               524288.0f, std::max<float>(0.0f, fRec41[i])))) &
                            1048575)] -
                     fZec70[i])));
            }
            /* Post code */
            fYec7_idx_save = vsize;
            fYec5_idx_save = vsize;
            fYec4_idx_save = vsize;
            for (int j35 = 0; (j35 < 4); j35 = (j35 + 1))
            {
                fRec23_perm[j35] = fRec23_tmp[(vsize + j35)];
            }
            for (int j31 = 0; (j31 < 4); j31 = (j31 + 1))
            {
                fRec22_perm[j31] = fRec22_tmp[(vsize + j31)];
            }
            for (int j33 = 0; (j33 < 4); j33 = (j33 + 1))
            {
                fRec21_perm[j33] = fRec21_tmp[(vsize + j33)];
            }
            for (int j29 = 0; (j29 < 4); j29 = (j29 + 1))
            {
                fRec19_perm[j29] = fRec19_tmp[(vsize + j29)];
            }
            for (int j25 = 0; (j25 < 4); j25 = (j25 + 1))
            {
                fRec10_perm[j25] = fRec10_tmp[(vsize + j25)];
            }
            for (int j61 = 0; (j61 < 4); j61 = (j61 + 1))
            {
                fRec9_perm[j61] = fRec9_tmp[(vsize + j61)];
            }
            for (int j65 = 0; (j65 < 4); j65 = (j65 + 1))
            {
                fRec8_perm[j65] = fRec8_tmp[(vsize + j65)];
            }
            for (int j71 = 0; (j71 < 4); j71 = (j71 + 1))
            {
                fRec6_perm[j71] = fRec6_tmp[(vsize + j71)];
            }
            for (int j75 = 0; (j75 < 4); j75 = (j75 + 1))
            {
                fRec4_perm[j75] = fRec4_tmp[(vsize + j75)];
            }
            fRec0_idx_save = vsize;
            /* Recursive loop 111 */
            /* Pre code */
            for (int j108 = 0; (j108 < 4); j108 = (j108 + 1))
            {
                fRec56_tmp[j108] = fRec56_perm[j108];
            }
            for (int j110 = 0; (j110 < 4); j110 = (j110 + 1))
            {
                fRec62_tmp[j110] = fRec62_perm[j110];
            }
            for (int j112 = 0; (j112 < 4); j112 = (j112 + 1))
            {
                fRec64_tmp[j112] = fRec64_perm[j112];
            }
            for (int j114 = 0; (j114 < 4); j114 = (j114 + 1))
            {
                fRec63_tmp[j114] = fRec63_perm[j114];
            }
            for (int j116 = 0; (j116 < 4); j116 = (j116 + 1))
            {
                fRec65_tmp[j116] = fRec65_perm[j116];
            }
            for (int j118 = 0; (j118 < 4); j118 = (j118 + 1))
            {
                fRec55_tmp[j118] = fRec55_perm[j118];
            }
            for (int j120 = 0; (j120 < 4); j120 = (j120 + 1))
            {
                fRec54_tmp[j120] = fRec54_perm[j120];
            }
            fYec8_idx = ((fYec8_idx + fYec8_idx_save) & 16383);
            for (int j124 = 0; (j124 < 4); j124 = (j124 + 1))
            {
                fRec52_tmp[j124] = fRec52_perm[j124];
            }
            fYec9_idx = ((fYec9_idx + fYec9_idx_save) & 16383);
            for (int j128 = 0; (j128 < 4); j128 = (j128 + 1))
            {
                fRec50_tmp[j128] = fRec50_perm[j128];
            }
            fYec11_idx = ((fYec11_idx + fYec11_idx_save) & 1048575);
            fRec49_idx = ((fRec49_idx + fRec49_idx_save) & 262143);
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec56[i] =
                  (((fRec49[(((i + fRec49_idx) -
                              (int(std::min<float>(
                                 fConst0, float(std::max<int>(0, iZec90[i])))) +
                               1)) &
                             262143)] *
                     (0.0f - (fZec88[i] + (99.0f - fZec91[i])))) +
                    ((fZec88[i] + (100.0f - fZec91[i])) *
                     fRec49[(
                       ((i + fRec49_idx) -
                        (int(std::min<float>(
                           fConst0, float(std::max<int>(0, (iZec90[i] + 1))))) +
                         1)) &
                       262143)])) -
                   (fConst7 * ((fConst8 * fRec56[(i - 2)]) +
                               (fConst9 * fRec56[(i - 1)]))));
                fZec92[i] =
                  (fConst7 *
                   (fRec56[(i - 2)] + (fRec56[i] + (2.0f * fRec56[(i - 1)]))));
                float fThen31 = fRec62[(i - 1)];
                float fElse31 =
                  (0.00392156886f *
                   (std::floor(((255.0f * std::fabs(fZec92[i])) + 0.5f)) *
                    (float((2 * (fZec92[i] > 0.0f))) + -1.0f)));
                fRec62[i]     = (iZec12[i] ? fElse31 : fThen31);
                float fThen32 = fZec92[i];
                float fElse32 = fRec62[i];
                fZec93[i]     = (iSlow4 ? fElse32 : fThen32);
                fZec94[i]     = std::fabs(fZec93[i]);
                fZec95[i] =
                  ((fZec94[i] > fRec64[(i - 1)]) ? fConst12 : fConst11);
                fRec64[i] = ((fZec94[i] * (1.0f - fZec95[i])) +
                             (fRec64[(i - 1)] * fZec95[i]));
                fRec63[i] =
                  ((fConst14 *
                    (0.0f -
                     (0.75f * std::max<float>(
                                ((20.0f * std::log10(std::max<float>(
                                            1.17549435e-38f, fRec64[i]))) +
                                 6.0f),
                                0.0f)))) +
                   (fConst13 * fRec63[(i - 1)]));
                fZec96[i] =
                  (fZec93[i] * std::pow(10.0f, (0.0500000007f * fRec63[i])));
                fZec97[i] = std::fabs((fZec96[i] + 9.99999975e-05f));
                fZec98[i] =
                  ((fZec97[i] > fRec65[(i - 1)]) ? fConst17 : fConst11);
                fRec65[i] = ((fZec97[i] * (1.0f - fZec98[i])) +
                             (fRec65[(i - 1)] * fZec98[i]));
                fRec55[i] =
                  ((fZec96[i] + ((fZec26[i] * fRec65[i]) * fZec27[i])) -
                   (((fZec30[i] * fRec55[(i - 2)]) +
                     (2.0f * (fZec32[i] * fRec55[(i - 1)]))) /
                    fZec33[i]));
                fRec54[i] =
                  (((((fZec34[i] * fRec55[(i - 1)]) + (fRec55[i] / fZec31[i])) +
                     (fRec55[(i - 2)] / fZec31[i])) /
                    fZec33[i]) -
                   (((fZec37[i] * fRec54[(i - 2)]) +
                     (2.0f * (fZec38[i] * fRec54[(i - 1)]))) /
                    fZec39[i]));
                fYec8[((i + fYec8_idx) & 16383)] =
                  ((0.980000019f * ((fRec54[(i - 2)] +
                                     (fRec54[i] + (2.0f * fRec54[(i - 1)]))) /
                                    fZec39[i])) -
                   (fRec35[i] * fRec52[(i - 1)]));
                fRec52[i] =
                  ((fYec8[(
                      ((i + fYec8_idx) -
                       std::min<int>(iConst45, std::max<int>(0, iZec102[i]))) &
                      16383)] *
                    (fZec103[i] + (2.0f - fZec100[i]))) +
                   ((fZec100[i] + (-1.0f - fZec103[i])) *
                    fYec8[(((i + fYec8_idx) -
                            std::min<int>(iConst45,
                                          std::max<int>(0, (iZec102[i] + 1)))) &
                           16383)]));
                fRec53[i] = (fRec35[i] * fYec8[((i + fYec8_idx) & 16383)]);
                fYec9[((i + fYec9_idx) & 16383)] =
                  ((fRec53[i] + fRec52[(i - 1)]) -
                   (0.899999976f * (fRec35[i] * fRec50[(i - 1)])));
                fRec50[i] =
                  ((fYec9[(
                      ((i + fYec9_idx) -
                       std::min<int>(iConst45, std::max<int>(0, iZec107[i]))) &
                      16383)] *
                    (fZec108[i] + (2.0f - fZec105[i]))) +
                   ((fZec105[i] + (-1.0f - fZec108[i])) *
                    fYec9[(((i + fYec9_idx) -
                            std::min<int>(iConst45,
                                          std::max<int>(0, (iZec107[i] + 1)))) &
                           16383)]));
                fRec51[i] = (0.899999976f *
                             (fRec35[i] * fYec9[((i + fYec9_idx) & 16383)]));
                fYec11[((i + fYec11_idx) & 1048575)] =
                  ((fZec63[i] *
                    ((fZec59[i] *
                      fYec10[(((i + fYec10_idx) - iZec57[i]) & 131071)]) +
                     (fZec60[i] *
                      fYec10[(((i + fYec10_idx) - iZec61[i]) & 131071)]))) +
                   ((fRec3[i] * (fRec51[i] + fRec50[(i - 1)])) +
                    (fZec62[i] *
                     ((fZec67[i] *
                       fYec10[(((i + fYec10_idx) - iZec65[i]) & 131071)]) +
                      (fZec68[i] *
                       fYec10[(((i + fYec10_idx) - iZec69[i]) & 131071)])))));
                fZec112[i] =
                  fYec11[(((i + fYec11_idx) -
                           int(std::min<float>(
                             524288.0f, std::max<float>(0.0f, fRec70[i])))) &
                          1048575)];
                fRec49[((i + fRec49_idx) & 262143)] =
                  (fZec112[i] +
                   (fRec69[i] *
                    (fYec11[(((i + fYec11_idx) -
                              int(std::min<float>(
                                524288.0f, std::max<float>(0.0f, fRec71[i])))) &
                             1048575)] -
                     fZec112[i])));
            }
            /* Post code */
            fYec11_idx_save = vsize;
            fYec9_idx_save  = vsize;
            fYec8_idx_save  = vsize;
            for (int j117 = 0; (j117 < 4); j117 = (j117 + 1))
            {
                fRec65_perm[j117] = fRec65_tmp[(vsize + j117)];
            }
            for (int j113 = 0; (j113 < 4); j113 = (j113 + 1))
            {
                fRec64_perm[j113] = fRec64_tmp[(vsize + j113)];
            }
            for (int j115 = 0; (j115 < 4); j115 = (j115 + 1))
            {
                fRec63_perm[j115] = fRec63_tmp[(vsize + j115)];
            }
            for (int j111 = 0; (j111 < 4); j111 = (j111 + 1))
            {
                fRec62_perm[j111] = fRec62_tmp[(vsize + j111)];
            }
            for (int j109 = 0; (j109 < 4); j109 = (j109 + 1))
            {
                fRec56_perm[j109] = fRec56_tmp[(vsize + j109)];
            }
            for (int j119 = 0; (j119 < 4); j119 = (j119 + 1))
            {
                fRec55_perm[j119] = fRec55_tmp[(vsize + j119)];
            }
            for (int j121 = 0; (j121 < 4); j121 = (j121 + 1))
            {
                fRec54_perm[j121] = fRec54_tmp[(vsize + j121)];
            }
            for (int j125 = 0; (j125 < 4); j125 = (j125 + 1))
            {
                fRec52_perm[j125] = fRec52_tmp[(vsize + j125)];
            }
            for (int j129 = 0; (j129 < 4); j129 = (j129 + 1))
            {
                fRec50_perm[j129] = fRec50_tmp[(vsize + j129)];
            }
            fRec49_idx_save = vsize;
            /* Vectorizable loop 112 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec71[i] = std::fabs(
                  (fRec0[((i + fRec0_idx) & 262143)] + 9.99999975e-05f));
            }
            /* Vectorizable loop 113 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec113[i] = std::fabs(
                  (fRec49[((i + fRec49_idx) & 262143)] + 9.99999975e-05f));
            }
            /* Recursive loop 114 */
            /* Pre code */
            for (int j88 = 0; (j88 < 4); j88 = (j88 + 1))
            {
                fRec44_tmp[j88] = fRec44_perm[j88];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec72[i] =
                  ((fZec71[i] > fRec44[(i - 1)]) ? fConst17 : fConst11);
                fRec44[i] = ((fZec71[i] * (1.0f - fZec72[i])) +
                             (fRec44[(i - 1)] * fZec72[i]));
            }
            /* Post code */
            for (int j89 = 0; (j89 < 4); j89 = (j89 + 1))
            {
                fRec44_perm[j89] = fRec44_tmp[(vsize + j89)];
            }
            /* Recursive loop 115 */
            /* Pre code */
            for (int j140 = 0; (j140 < 4); j140 = (j140 + 1))
            {
                fRec73_tmp[j140] = fRec73_perm[j140];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec114[i] =
                  ((fZec113[i] > fRec73[(i - 1)]) ? fConst17 : fConst11);
                fRec73[i] = ((fZec113[i] * (1.0f - fZec114[i])) +
                             (fRec73[(i - 1)] * fZec114[i]));
            }
            /* Post code */
            for (int j141 = 0; (j141 < 4); j141 = (j141 + 1))
            {
                fRec73_perm[j141] = fRec73_tmp[(vsize + j141)];
            }
            /* Vectorizable loop 116 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec73[i] = (fRec0[((i + fRec0_idx) & 262143)] +
                             ((fZec26[i] * fRec44[i]) * fZec27[i]));
            }
            /* Vectorizable loop 117 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec76[i] = std::fabs(float(input0[i]));
            }
            /* Recursive loop 118 */
            /* Pre code */
            for (int j96 = 0; (j96 < 4); j96 = (j96 + 1))
            {
                fRec48_tmp[j96] = fRec48_perm[j96];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec48[i] = (fSlow18 + (fConst2 * fRec48[(i - 1)]));
            }
            /* Post code */
            for (int j97 = 0; (j97 < 4); j97 = (j97 + 1))
            {
                fRec48_perm[j97] = fRec48_tmp[(vsize + j97)];
            }
            /* Vectorizable loop 119 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec115[i] = (fRec49[((i + fRec49_idx) & 262143)] +
                              ((fZec26[i] * fRec73[i]) * fZec27[i]));
            }
            /* Vectorizable loop 120 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec118[i] = std::fabs(float(input1[i]));
            }
            /* Vectorizable loop 121 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec74[i] = std::fabs(fZec73[i]);
            }
            /* Recursive loop 122 */
            /* Pre code */
            for (int j94 = 0; (j94 < 4); j94 = (j94 + 1))
            {
                fRec47_tmp[j94] = fRec47_perm[j94];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec77[i] = ((fZec76[i] > fRec47[(i - 1)]) ? fSlow17 : fSlow14);
                fRec47[i] = ((fZec76[i] * (1.0f - fZec77[i])) +
                             (fRec47[(i - 1)] * fZec77[i]));
            }
            /* Post code */
            for (int j95 = 0; (j95 < 4); j95 = (j95 + 1))
            {
                fRec47_perm[j95] = fRec47_tmp[(vsize + j95)];
            }
            /* Vectorizable loop 123 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec79[i] = (60.0f * fRec48[i]);
            }
            /* Vectorizable loop 124 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec116[i] = std::fabs(fZec115[i]);
            }
            /* Recursive loop 125 */
            /* Pre code */
            for (int j146 = 0; (j146 < 4); j146 = (j146 + 1))
            {
                fRec76_tmp[j146] = fRec76_perm[j146];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec119[i] =
                  ((fZec118[i] > fRec76[(i - 1)]) ? fSlow17 : fSlow14);
                fRec76[i] = ((fZec118[i] * (1.0f - fZec119[i])) +
                             (fRec76[(i - 1)] * fZec119[i]));
            }
            /* Post code */
            for (int j147 = 0; (j147 < 4); j147 = (j147 + 1))
            {
                fRec76_perm[j147] = fRec76_tmp[(vsize + j147)];
            }
            /* Recursive loop 126 */
            /* Pre code */
            for (int j90 = 0; (j90 < 4); j90 = (j90 + 1))
            {
                fRec46_tmp[j90] = fRec46_perm[j90];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec75[i] =
                  ((fZec74[i] > fRec46[(i - 1)]) ? fConst12 : fConst11);
                fRec46[i] = ((fZec74[i] * (1.0f - fZec75[i])) +
                             (fRec46[(i - 1)] * fZec75[i]));
            }
            /* Post code */
            for (int j91 = 0; (j91 < 4); j91 = (j91 + 1))
            {
                fRec46_perm[j91] = fRec46_tmp[(vsize + j91)];
            }
            /* Vectorizable loop 127 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec78[i] = (20.0f * std::log10(std::max<float>(1.17549435e-38f,
                                                                fRec47[i])));
            }
            /* Vectorizable loop 128 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec80[i] = (-1.5f - fZec79[i]);
            }
            /* Vectorizable loop 129 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec81[i] = (1.5f - fZec79[i]);
            }
            /* Recursive loop 130 */
            /* Pre code */
            for (int j142 = 0; (j142 < 4); j142 = (j142 + 1))
            {
                fRec75_tmp[j142] = fRec75_perm[j142];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec117[i] =
                  ((fZec116[i] > fRec75[(i - 1)]) ? fConst12 : fConst11);
                fRec75[i] = ((fZec116[i] * (1.0f - fZec117[i])) +
                             (fRec75[(i - 1)] * fZec117[i]));
            }
            /* Post code */
            for (int j143 = 0; (j143 < 4); j143 = (j143 + 1))
            {
                fRec75_perm[j143] = fRec75_tmp[(vsize + j143)];
            }
            /* Vectorizable loop 131 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec120[i] =
                  (20.0f *
                   std::log10(std::max<float>(1.17549435e-38f, fRec76[i])));
            }
            /* Recursive loop 132 */
            /* Pre code */
            for (int j92 = 0; (j92 < 4); j92 = (j92 + 1))
            {
                fRec45_tmp[j92] = fRec45_perm[j92];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec45[i] =
                  ((fConst14 *
                    (0.0f -
                     (0.75f * std::max<float>(
                                ((20.0f * std::log10(std::max<float>(
                                            1.17549435e-38f, fRec46[i]))) +
                                 6.0f),
                                0.0f)))) +
                   (fConst13 * fRec45[(i - 1)]));
            }
            /* Post code */
            for (int j93 = 0; (j93 < 4); j93 = (j93 + 1))
            {
                fRec45_perm[j93] = fRec45_tmp[(vsize + j93)];
            }
            /* Vectorizable loop 133 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec82[i] = ((fZec78[i] > fZec80[i]) + (fZec78[i] > fZec81[i]));
            }
            /* Vectorizable loop 134 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec83[i] = (fZec78[i] + fZec79[i]);
            }
            /* Vectorizable loop 135 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec84[i] = (1.0f - (1.0f / ((19.0f * fRec48[i]) + 1.0f)));
            }
            /* Recursive loop 136 */
            /* Pre code */
            for (int j144 = 0; (j144 < 4); j144 = (j144 + 1))
            {
                fRec74_tmp[j144] = fRec74_perm[j144];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec74[i] =
                  ((fConst14 *
                    (0.0f -
                     (0.75f * std::max<float>(
                                ((20.0f * std::log10(std::max<float>(
                                            1.17549435e-38f, fRec75[i]))) +
                                 6.0f),
                                0.0f)))) +
                   (fConst13 * fRec74[(i - 1)]));
            }
            /* Post code */
            for (int j145 = 0; (j145 < 4); j145 = (j145 + 1))
            {
                fRec74_perm[j145] = fRec74_tmp[(vsize + j145)];
            }
            /* Vectorizable loop 137 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec121[i] =
                  ((fZec120[i] > fZec80[i]) + (fZec120[i] > fZec81[i]));
            }
            /* Vectorizable loop 138 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec122[i] = (fZec79[i] + fZec120[i]);
            }
            /* Vectorizable loop 139 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen27 = fZec83[i];
                float fElse27 =
                  (0.166666672f * Macalla_faustpower2_f((fZec83[i] + 1.5f)));
                float fThen28 = ((iZec82[i] == 1) ? fElse27 : fThen27);
                output0[i]    = FAUSTFLOAT((
                  (fSlow20 * float(input0[i])) +
                  (fSlow19 *
                   ((fZec73[i] * std::pow(10.0f, (0.0500000007f * fRec45[i]))) *
                    std::pow(
                      10.0f,
                      (0.0500000007f *
                       (0.0f - (std::max<float>(
                                  0.0f, ((iZec82[i] == 0) ? 0.0f : fThen28)) *
                                fZec84[i]))))))));
            }
            /* Vectorizable loop 140 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen47 = fZec122[i];
                float fElse47 =
                  (0.166666672f * Macalla_faustpower2_f((fZec122[i] + 1.5f)));
                float fThen48 = ((iZec121[i] == 1) ? fElse47 : fThen47);
                output1[i]    = FAUSTFLOAT(
                  ((fSlow20 * float(input1[i])) +
                   (fSlow19 *
                    ((fZec115[i] *
                      std::pow(10.0f, (0.0500000007f * fRec74[i]))) *
                     std::pow(
                       10.0f,
                       (0.0500000007f *
                        (0.0f -
                         (fZec84[i] *
                          std::max<float>(
                            0.0f, ((iZec121[i] == 0) ? 0.0f : fThen48))))))))));
            }
        }
        /* Remaining frames */
        if ((vindex < count))
        {
            FAUSTFLOAT* input0  = &input0_ptr[vindex];
            FAUSTFLOAT* input1  = &input1_ptr[vindex];
            FAUSTFLOAT* output0 = &output0_ptr[vindex];
            FAUSTFLOAT* output1 = &output1_ptr[vindex];
            int vsize           = (count - vindex);
            /* Vectorizable loop 0 */
            /* Pre code */
            for (int j10 = 0; (j10 < 4); j10 = (j10 + 1))
            {
                iVec0_tmp[j10] = iVec0_perm[j10];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iVec0[i] = 1;
            }
            /* Post code */
            for (int j11 = 0; (j11 < 4); j11 = (j11 + 1))
            {
                iVec0_perm[j11] = iVec0_tmp[(vsize + j11)];
            }
            /* Vectorizable loop 1 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec0[i] = (1 - iVec0[(i - 1)]);
            }
            /* Recursive loop 2 */
            /* Pre code */
            for (int j40 = 0; (j40 < 4); j40 = (j40 + 1))
            {
                fRec27_tmp[j40] = fRec27_perm[j40];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen7 = (fConst15 + fRec27[(i - 1)]);
                fZec20[i]    = (iZec0[i] ? 0.0f : fThen7);
                fRec27[i]    = (fZec20[i] - std::floor(fZec20[i]));
            }
            /* Post code */
            for (int j41 = 0; (j41 < 4); j41 = (j41 + 1))
            {
                fRec27_perm[j41] = fRec27_tmp[(vsize + j41)];
            }
            /* Recursive loop 3 */
            /* Pre code */
            for (int j6 = 0; (j6 < 4); j6 = (j6 + 1))
            {
                fRec11_tmp[j6] = fRec11_perm[j6];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec11[i] = (fSlow3 + (fConst2 * fRec11[(i - 1)]));
            }
            /* Post code */
            for (int j7 = 0; (j7 < 4); j7 = (j7 + 1))
            {
                fRec11_perm[j7] = fRec11_tmp[(vsize + j7)];
            }
            /* Recursive loop 4 */
            /* Pre code */
            for (int j8 = 0; (j8 < 4); j8 = (j8 + 1))
            {
                iRec13_tmp[j8] = iRec13_perm[j8];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iRec13[i] = ((1103515245 * iRec13[(i - 1)]) + 12345);
            }
            /* Post code */
            for (int j9 = 0; (j9 < 4); j9 = (j9 + 1))
            {
                iRec13_perm[j9] = iRec13_tmp[(vsize + j9)];
            }
            /* Recursive loop 5 */
            /* Pre code */
            for (int j98 = 0; (j98 < 4); j98 = (j98 + 1))
            {
                fRec59_tmp[j98] = fRec59_perm[j98];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec59[i] = (fSlow21 + (fConst2 * fRec59[(i - 1)]));
            }
            /* Post code */
            for (int j99 = 0; (j99 < 4); j99 = (j99 + 1))
            {
                fRec59_perm[j99] = fRec59_tmp[(vsize + j99)];
            }
            /* Vectorizable loop 6 */
            /* Pre code */
            for (int j42 = 0; (j42 < 4); j42 = (j42 + 1))
            {
                fYec1_tmp[j42] = fYec1_perm[j42];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fYec1[i] = (fRec27[i] - fRec27[(i - 1)]);
            }
            /* Post code */
            for (int j43 = 0; (j43 < 4); j43 = (j43 + 1))
            {
                fYec1_perm[j43] = fYec1_tmp[(vsize + j43)];
            }
            /* Recursive loop 7 */
            /* Pre code */
            for (int j46 = 0; (j46 < 4); j46 = (j46 + 1))
            {
                fRec30_tmp[j46] = fRec30_perm[j46];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen8 = (fConst18 + fRec30[(i - 1)]);
                fZec22[i]    = (iZec0[i] ? 0.0f : fThen8);
                fRec30[i]    = (fZec22[i] - std::floor(fZec22[i]));
            }
            /* Post code */
            for (int j47 = 0; (j47 < 4); j47 = (j47 + 1))
            {
                fRec30_perm[j47] = fRec30_tmp[(vsize + j47)];
            }
            /* Vectorizable loop 8 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec1[i] = (1.99000001f * fRec11[i]);
            }
            /* Vectorizable loop 9 */
            /* Pre code */
            for (int j14 = 0; (j14 < 4); j14 = (j14 + 1))
            {
                fYec0_tmp[j14] = fYec0_perm[j14];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fYec0[i] = float(iRec13[i]);
            }
            /* Post code */
            for (int j15 = 0; (j15 < 4); j15 = (j15 + 1))
            {
                fYec0_perm[j15] = fYec0_tmp[(vsize + j15)];
            }
            /* Vectorizable loop 10 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec4[i] = (18.0f * fRec11[i]);
            }
            /* Recursive loop 11 */
            /* Pre code */
            for (int j20 = 0; (j20 < 4); j20 = (j20 + 1))
            {
                fRec17_tmp[j20] = fRec17_perm[j20];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen2 = (fConst4 + fRec17[(i - 1)]);
                fZec6[i]     = (iZec0[i] ? 0.0f : fThen2);
                fRec17[i]    = (fZec6[i] - std::floor(fZec6[i]));
            }
            /* Post code */
            for (int j21 = 0; (j21 < 4); j21 = (j21 + 1))
            {
                fRec17_perm[j21] = fRec17_tmp[(vsize + j21)];
            }
            /* Vectorizable loop 12 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec85[i] = (0.200000003f * fRec59[i]);
            }
            /* Vectorizable loop 13 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec21[i] = ((fYec1[(i - 1)] <= 0.0f) & (fYec1[i] > 0.0f));
            }
            /* Vectorizable loop 14 */
            /* Pre code */
            for (int j48 = 0; (j48 < 4); j48 = (j48 + 1))
            {
                fYec2_tmp[j48] = fYec2_perm[j48];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fYec2[i] = Macalla_faustpower2_f(((2.0f * fRec30[i]) + -1.0f));
            }
            /* Post code */
            for (int j49 = 0; (j49 < 4); j49 = (j49 + 1))
            {
                fYec2_perm[j49] = fYec2_tmp[(vsize + j49)];
            }
            /* Recursive loop 15 */
            /* Pre code */
            for (int j76 = 0; (j76 < 4); j76 = (j76 + 1))
            {
                fRec42_tmp[j76] = fRec42_perm[j76];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec42[i] = (fSlow9 + (fConst2 * fRec42[(i - 1)]));
            }
            /* Post code */
            for (int j77 = 0; (j77 < 4); j77 = (j77 + 1))
            {
                fRec42_perm[j77] = fRec42_tmp[(vsize + j77)];
            }
            /* Recursive loop 16 */
            /* Pre code */
            for (int j12 = 0; (j12 < 4); j12 = (j12 + 1))
            {
                fRec14_tmp[j12] = fRec14_perm[j12];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen0 =
                  (fRec14[(i - 1)] + (fConst3 * (fZec1[i] + 0.00999999978f)));
                fZec2[i]  = (iZec0[i] ? 0.0f : fThen0);
                fRec14[i] = (fZec2[i] - std::floor(fZec2[i]));
            }
            /* Post code */
            for (int j13 = 0; (j13 < 4); j13 = (j13 + 1))
            {
                fRec14_perm[j13] = fRec14_tmp[(vsize + j13)];
            }
            /* Vectorizable loop 17 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec3[i] = (4.65661277e-12f * (fRec11[i] * fYec0[i]));
            }
            /* Recursive loop 18 */
            /* Pre code */
            for (int j18 = 0; (j18 < 4); j18 = (j18 + 1))
            {
                fRec16_tmp[j18] = fRec16_perm[j18];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen1 =
                  (fRec16[(i - 1)] + (fConst3 * (fZec4[i] + 2.0f)));
                fZec5[i]  = (iZec0[i] ? 0.0f : fThen1);
                fRec16[i] = (fZec5[i] - std::floor(fZec5[i]));
            }
            /* Post code */
            for (int j19 = 0; (j19 < 4); j19 = (j19 + 1))
            {
                fRec16_perm[j19] = fRec16_tmp[(vsize + j19)];
            }
            /* Vectorizable loop 19 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec7[i] = ((0.200000003f *
                             ftbl0MacallaSIG0[int((65536.0f * fRec17[i]))]) +
                            0.800000012f);
            }
            /* Recursive loop 20 */
            /* Pre code */
            for (int j100 = 0; (j100 < 4); j100 = (j100 + 1))
            {
                fRec58_tmp[j100] = fRec58_perm[j100];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen29 =
                  (fRec58[(i - 1)] +
                   (fConst3 * ((fZec1[i] + fZec85[i]) + 0.00999999978f)));
                fZec86[i] = (iZec0[i] ? 0.0f : fThen29);
                fRec58[i] = (fZec86[i] - std::floor(fZec86[i]));
            }
            /* Post code */
            for (int j101 = 0; (j101 < 4); j101 = (j101 + 1))
            {
                fRec58_perm[j101] = fRec58_tmp[(vsize + j101)];
            }
            /* Recursive loop 21 */
            /* Pre code */
            for (int j104 = 0; (j104 < 4); j104 = (j104 + 1))
            {
                fRec61_tmp[j104] = fRec61_perm[j104];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen30 = (fRec61[(i - 1)] +
                                 (fConst3 * ((fZec4[i] + fZec85[i]) + 2.0f)));
                fZec87[i]     = (iZec0[i] ? 0.0f : fThen30);
                fRec61[i]     = (fZec87[i] - std::floor(fZec87[i]));
            }
            /* Post code */
            for (int j105 = 0; (j105 < 4); j105 = (j105 + 1))
            {
                fRec61_perm[j105] = fRec61_tmp[(vsize + j105)];
            }
            /* Recursive loop 22 */
            /* Pre code */
            for (int j44 = 0; (j44 < 4); j44 = (j44 + 1))
            {
                fRec28_tmp[j44] = fRec28_perm[j44];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec28[i] = ((fRec28[(i - 1)] * float((1 - iZec21[i]))) +
                             (4.65661287e-10f * (fYec0[i] * float(iZec21[i]))));
            }
            /* Post code */
            for (int j45 = 0; (j45 < 4); j45 = (j45 + 1))
            {
                fRec28_perm[j45] = fRec28_tmp[(vsize + j45)];
            }
            /* Vectorizable loop 23 */
            /* Pre code */
            fYec3_idx = ((fYec3_idx + fYec3_idx_save) & 2047);
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fYec3[((i + fYec3_idx) & 2047)] =
                  (float(iVec0[(i - 1)]) * (fYec2[i] - fYec2[(i - 1)]));
            }
            /* Post code */
            fYec3_idx_save = vsize;
            /* Recursive loop 24 */
            /* Pre code */
            for (int j58 = 0; (j58 < 4); j58 = (j58 + 1))
            {
                fRec33_tmp[j58] = fRec33_perm[j58];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec33[i] = (fSlow6 + (fConst2 * fRec33[(i - 1)]));
            }
            /* Post code */
            for (int j59 = 0; (j59 < 4); j59 = (j59 + 1))
            {
                fRec33_perm[j59] = fRec33_tmp[(vsize + j59)];
            }
            /* Recursive loop 25 */
            /* Pre code */
            for (int j62 = 0; (j62 < 4); j62 = (j62 + 1))
            {
                fRec34_tmp[j62] = fRec34_perm[j62];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec34[i] = (fSlow7 + (fConst2 * fRec34[(i - 1)]));
            }
            /* Post code */
            for (int j63 = 0; (j63 < 4); j63 = (j63 + 1))
            {
                fRec34_perm[j63] = fRec34_tmp[(vsize + j63)];
            }
            /* Recursive loop 26 */
            /* Pre code */
            for (int j68 = 0; (j68 < 4); j68 = (j68 + 1))
            {
                fRec36_tmp[j68] = fRec36_perm[j68];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen11 = (fConst43 + fRec36[(i - 1)]);
                fZec40[i]     = (iZec0[i] ? 0.0f : fThen11);
                fRec36[i]     = (fZec40[i] - std::floor(fZec40[i]));
            }
            /* Post code */
            for (int j69 = 0; (j69 < 4); j69 = (j69 + 1))
            {
                fRec36_perm[j69] = fRec36_tmp[(vsize + j69)];
            }
            /* Recursive loop 27 */
            /* Pre code */
            for (int j72 = 0; (j72 < 4); j72 = (j72 + 1))
            {
                fRec37_tmp[j72] = fRec37_perm[j72];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen12 = (fConst48 + fRec37[(i - 1)]);
                fZec45[i]     = (iZec0[i] ? 0.0f : fThen12);
                fRec37[i]     = (fZec45[i] - std::floor(fZec45[i]));
            }
            /* Post code */
            for (int j73 = 0; (j73 < 4); j73 = (j73 + 1))
            {
                fRec37_perm[j73] = fRec37_tmp[(vsize + j73)];
            }
            /* Vectorizable loop 28 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec50[i] = (4.98999977f * fRec42[i]);
            }
            /* Recursive loop 29 */
            /* Pre code */
            for (int j2 = 0; (j2 < 4); j2 = (j2 + 1))
            {
                fRec2_tmp[j2] = fRec2_perm[j2];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec2[i] =
                  std::fmod(((fRec2[(i - 1)] + 4097.0f) - fSlow1), 4096.0f);
            }
            /* Post code */
            for (int j3 = 0; (j3 < 4); j3 = (j3 + 1))
            {
                fRec2_perm[j3] = fRec2_tmp[(vsize + j3)];
            }
            /* Recursive loop 30 */
            /* Pre code */
            for (int j16 = 0; (j16 < 4); j16 = (j16 + 1))
            {
                fRec12_tmp[j16] = fRec12_perm[j16];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec12[i] =
                  ((9.99999975e-05f *
                    (fZec3[i] +
                     (1.0f - std::fabs(((2.0f * fRec14[i]) + -1.0f))))) +
                   (0.999899983f * fRec12[(i - 1)]));
            }
            /* Post code */
            for (int j17 = 0; (j17 < 4); j17 = (j17 + 1))
            {
                fRec12_perm[j17] = fRec12_tmp[(vsize + j17)];
            }
            /* Recursive loop 31 */
            /* Pre code */
            for (int j22 = 0; (j22 < 4); j22 = (j22 + 1))
            {
                fRec15_tmp[j22] = fRec15_perm[j22];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec15[i] =
                  ((9.99999975e-05f *
                    ((fZec3[i] +
                      (1.0f - std::fabs(((2.0f * fRec16[i]) + -1.0f)))) *
                     fZec7[i])) +
                   (0.999899983f * fRec15[(i - 1)]));
            }
            /* Post code */
            for (int j23 = 0; (j23 < 4); j23 = (j23 + 1))
            {
                fRec15_perm[j23] = fRec15_tmp[(vsize + j23)];
            }
            /* Recursive loop 32 */
            /* Pre code */
            for (int j102 = 0; (j102 < 4); j102 = (j102 + 1))
            {
                fRec57_tmp[j102] = fRec57_perm[j102];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec57[i] =
                  ((9.99999975e-05f *
                    (fZec3[i] +
                     (1.0f - std::fabs(((2.0f * fRec58[i]) + -1.0f))))) +
                   (0.999899983f * fRec57[(i - 1)]));
            }
            /* Post code */
            for (int j103 = 0; (j103 < 4); j103 = (j103 + 1))
            {
                fRec57_perm[j103] = fRec57_tmp[(vsize + j103)];
            }
            /* Recursive loop 33 */
            /* Pre code */
            for (int j106 = 0; (j106 < 4); j106 = (j106 + 1))
            {
                fRec60_tmp[j106] = fRec60_perm[j106];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec60[i] =
                  ((9.99999975e-05f *
                    (fZec7[i] *
                     (fZec3[i] +
                      (1.0f - std::fabs(((2.0f * fRec61[i]) + -1.0f)))))) +
                   (0.999899983f * fRec60[(i - 1)]));
            }
            /* Post code */
            for (int j107 = 0; (j107 < 4); j107 = (j107 + 1))
            {
                fRec60_perm[j107] = fRec60_tmp[(vsize + j107)];
            }
            /* Recursive loop 34 */
            /* Pre code */
            for (int j122 = 0; (j122 < 4); j122 = (j122 + 1))
            {
                fRec66_tmp[j122] = fRec66_perm[j122];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen35 =
                  (fRec66[(i - 1)] +
                   (fConst3 * ((0.0199999996f * fRec59[i]) + 0.150000006f)));
                fZec99[i] = (iZec0[i] ? 0.0f : fThen35);
                fRec66[i] = (fZec99[i] - std::floor(fZec99[i]));
            }
            /* Post code */
            for (int j123 = 0; (j123 < 4); j123 = (j123 + 1))
            {
                fRec66_perm[j123] = fRec66_tmp[(vsize + j123)];
            }
            /* Recursive loop 35 */
            /* Pre code */
            for (int j126 = 0; (j126 < 4); j126 = (j126 + 1))
            {
                fRec67_tmp[j126] = fRec67_perm[j126];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen36 =
                  (fRec67[(i - 1)] +
                   (fConst3 * ((0.0299999993f * fRec59[i]) + 0.230000004f)));
                fZec104[i] = (iZec0[i] ? 0.0f : fThen36);
                fRec67[i]  = (fZec104[i] - std::floor(fZec104[i]));
            }
            /* Post code */
            for (int j127 = 0; (j127 < 4); j127 = (j127 + 1))
            {
                fRec67_perm[j127] = fRec67_tmp[(vsize + j127)];
            }
            /* Recursive loop 36 */
            /* Pre code */
            for (int j38 = 0; (j38 < 4); j38 = (j38 + 1))
            {
                fRec26_tmp[j38] = fRec26_perm[j38];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec26[i] = (((0.522189379f * fRec26[(i - 3)]) +
                              ((4.65661287e-10f * fYec0[i]) +
                               (2.49495602f * fRec26[(i - 1)]))) -
                             (2.0172658f * fRec26[(i - 2)]));
            }
            /* Post code */
            for (int j39 = 0; (j39 < 4); j39 = (j39 + 1))
            {
                fRec26_perm[j39] = fRec26_tmp[(vsize + j39)];
            }
            /* Recursive loop 37 */
            /* Pre code */
            for (int j50 = 0; (j50 < 4); j50 = (j50 + 1))
            {
                fRec29_tmp[j50] = fRec29_perm[j50];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec29[i] =
                  ((0.999000013f * fRec29[(i - 1)]) +
                   (fConst19 *
                    ((fYec3[((i + fYec3_idx) & 2047)] -
                      (fConst22 *
                       fYec3[(((i + fYec3_idx) - iConst23) & 2047)])) -
                     (fConst24 *
                      fYec3[(((i + fYec3_idx) - iConst25) & 2047)]))));
            }
            /* Post code */
            for (int j51 = 0; (j51 < 4); j51 = (j51 + 1))
            {
                fRec29_perm[j51] = fRec29_tmp[(vsize + j51)];
            }
            /* Recursive loop 38 */
            /* Pre code */
            for (int j52 = 0; (j52 < 4); j52 = (j52 + 1))
            {
                fRec31_tmp[j52] = fRec31_perm[j52];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen9 = (fConst26 + fRec31[(i - 1)]);
                fZec23[i]    = (iZec0[i] ? 0.0f : fThen9);
                fRec31[i]    = (fZec23[i] - std::floor(fZec23[i]));
            }
            /* Post code */
            for (int j53 = 0; (j53 < 4); j53 = (j53 + 1))
            {
                fRec31_perm[j53] = fRec31_tmp[(vsize + j53)];
            }
            /* Recursive loop 39 */
            /* Pre code */
            for (int j54 = 0; (j54 < 4); j54 = (j54 + 1))
            {
                fRec32_tmp[j54] = fRec32_perm[j54];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen10 = (fConst27 + fRec32[(i - 1)]);
                fZec24[i]     = (iZec0[i] ? 0.0f : fThen10);
                fRec32[i]     = (fZec24[i] - std::floor(fZec24[i]));
            }
            /* Post code */
            for (int j55 = 0; (j55 < 4); j55 = (j55 + 1))
            {
                fRec32_perm[j55] = fRec32_tmp[(vsize + j55)];
            }
            /* Vectorizable loop 40 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec25[i] = (0.5f * (fRec28[i] + 1.0f));
            }
            /* Vectorizable loop 41 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec28[i] = std::tan((fConst41 * fRec33[i]));
            }
            /* Vectorizable loop 42 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec35[i] = std::tan((fConst41 * fRec34[i]));
            }
            /* Vectorizable loop 43 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec41[i] = std::max<float>(
                  0.0f,
                  std::min<float>(
                    fConst46,
                    (fConst47 *
                     ((1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec36[i]) +
                                                          -1.0f)))) +
                               -1.0f)) +
                      7.0f))));
            }
            /* Vectorizable loop 44 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec46[i] = std::max<float>(
                  0.0f,
                  std::min<float>(
                    fConst46,
                    (fConst47 *
                     ((1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec37[i]) +
                                                          -1.0f)))) +
                               -1.0f)) +
                      11.3000002f))));
            }
            /* Recursive loop 45 */
            /* Pre code */
            for (int j78 = 0; (j78 < 4); j78 = (j78 + 1))
            {
                fRec43_tmp[j78] = fRec43_perm[j78];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen13 =
                  (fRec43[(i - 1)] + (fConst3 * (fZec50[i] + 0.00999999978f)));
                fZec51[i] = (iZec0[i] ? 0.0f : fThen13);
                fRec43[i] = (fZec51[i] - std::floor(fZec51[i]));
            }
            /* Post code */
            for (int j79 = 0; (j79 < 4); j79 = (j79 + 1))
            {
                fRec43_perm[j79] = fRec43_tmp[(vsize + j79)];
            }
            /* Vectorizable loop 46 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec55[i] = (fRec2[i] + 4096.0f);
            }
            /* Recursive loop 47 */
            /* Pre code */
            for (int j0 = 0; (j0 < 4); j0 = (j0 + 1))
            {
                fRec1_tmp[j0] = fRec1_perm[j0];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec1[i] = (fSlow0 + (fConst2 * fRec1[(i - 1)]));
            }
            /* Post code */
            for (int j1 = 0; (j1 < 4); j1 = (j1 + 1))
            {
                fRec1_perm[j1] = fRec1_tmp[(vsize + j1)];
            }
            /* Vectorizable loop 48 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec8[i] =
                  (fRec11[i] * ((600.0f * fRec12[i]) + (100.0f * fRec15[i])));
            }
            /* Vectorizable loop 49 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec88[i] =
                  (fRec11[i] * ((600.0f * fRec57[i]) + (100.0f * fRec60[i])));
            }
            /* Vectorizable loop 50 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec100[i] = std::max<float>(
                  0.0f,
                  std::min<float>(
                    fConst46,
                    (fConst47 *
                     (((0.5f * fRec59[i]) +
                       (1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec66[i]) +
                                                           -1.0f)))) +
                                -1.0f))) +
                      7.0f))));
            }
            /* Vectorizable loop 51 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec105[i] = std::max<float>(
                  0.0f,
                  std::min<float>(
                    fConst46,
                    (fConst47 *
                     (((0.699999988f * fRec59[i]) +
                       (1.5f * ((2.0f * (1.0f - std::fabs(((2.0f * fRec67[i]) +
                                                           -1.0f)))) +
                                -1.0f))) +
                      11.3000002f))));
            }
            /* Recursive loop 52 */
            /* Pre code */
            for (int j130 = 0; (j130 < 4); j130 = (j130 + 1))
            {
                fRec72_tmp[j130] = fRec72_perm[j130];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen37 = (fRec72[(i - 1)] +
                                 (fConst3 * ((fZec50[i] + (2.0f * fRec59[i])) +
                                             0.00999999978f)));
                fZec109[i]    = (iZec0[i] ? 0.0f : fThen37);
                fRec72[i]     = (fZec109[i] - std::floor(fZec109[i]));
            }
            /* Post code */
            for (int j131 = 0; (j131 < 4); j131 = (j131 + 1))
            {
                fRec72_perm[j131] = fRec72_tmp[(vsize + j131)];
            }
            /* Recursive loop 53 */
            /* Pre code */
            for (int j26 = 0; (j26 < 4); j26 = (j26 + 1))
            {
                iRec20_tmp[j26] = iRec20_perm[j26];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iRec20[i] = (iRec20[(i - 1)] + 1);
            }
            /* Post code */
            for (int j27 = 0; (j27 < 4); j27 = (j27 + 1))
            {
                iRec20_perm[j27] = iRec20_tmp[(vsize + j27)];
            }
            /* Recursive loop 54 */
            /* Pre code */
            for (int j36 = 0; (j36 < 4); j36 = (j36 + 1))
            {
                fRec24_tmp[j36] = fRec24_perm[j36];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec24[i] = (fSlow5 + (fConst2 * fRec24[(i - 1)]));
            }
            /* Post code */
            for (int j37 = 0; (j37 < 4); j37 = (j37 + 1))
            {
                fRec24_perm[j37] = fRec24_tmp[(vsize + j37)];
            }
            /* Recursive loop 55 */
            /* Pre code */
            for (int j56 = 0; (j56 < 4); j56 = (j56 + 1))
            {
                fRec25_tmp[j56] = fRec25_perm[j56];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec25[i] =
                  (((((1.20000005f * (((0.0499220341f * fRec26[i]) +
                                       (0.0506126992f * fRec26[(i - 2)])) -
                                      ((0.0959935337f * fRec26[(i - 1)]) +
                                       (0.00440878607f * fRec26[(i - 3)])))) +
                      (2.32830644e-10f *
                       (fYec0[(i - 1)] *
                        float(((fRec27[i] >= fZec25[i]) *
                               (fRec27[(i - 1)] < fZec25[i])))))) +
                     (fConst28 * fRec29[i])) +
                    (0.00999999978f *
                     (ftbl0MacallaSIG0[int((65536.0f * fRec31[i]))] +
                      ftbl0MacallaSIG0[int((65536.0f * fRec32[i]))]))) -
                   (fConst36 * ((fConst37 * fRec25[(i - 1)]) +
                                (fConst38 * fRec25[(i - 2)]))));
            }
            /* Post code */
            for (int j57 = 0; (j57 < 4); j57 = (j57 + 1))
            {
                fRec25_perm[j57] = fRec25_tmp[(vsize + j57)];
            }
            /* Vectorizable loop 56 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec29[i] = (1.0f / fZec28[i]);
            }
            /* Vectorizable loop 57 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec31[i] = Macalla_faustpower2_f(fZec28[i]);
            }
            /* Vectorizable loop 58 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec36[i] = (1.0f / fZec35[i]);
            }
            /* Vectorizable loop 59 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec42[i] = (fZec41[i] + -1.0f);
            }
            /* Vectorizable loop 60 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec47[i] = (fZec46[i] + -1.0f);
            }
            /* Vectorizable loop 61 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec52[i] = std::min<float>(
                  524288.0f,
                  (fConst0 *
                   (fSlow11 +
                    (fRec42[i] *
                     std::fabs(((2.0f * (1.0f - std::fabs(((2.0f * fRec43[i]) +
                                                           -1.0f)))) +
                                -1.0f))))));
            }
            /* Vectorizable loop 62 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec54[i] = std::pow(10.0f, (0.0500000007f * fRec1[i]));
            }
            /* Vectorizable loop 63 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec56[i] = int(fZec55[i]);
            }
            /* Vectorizable loop 64 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec58[i] = std::floor(fZec55[i]);
            }
            /* Vectorizable loop 65 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec62[i] = std::min<float>((0.001953125f * fRec2[i]), 1.0f);
            }
            /* Vectorizable loop 66 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec64[i] = int(fRec2[i]);
            }
            /* Vectorizable loop 67 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec66[i] = std::floor(fRec2[i]);
            }
            /* Vectorizable loop 68 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec9[i] = (fZec8[i] + 100.000008f);
            }
            /* Vectorizable loop 69 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec89[i] = (fZec88[i] + 100.000008f);
            }
            /* Vectorizable loop 70 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec101[i] = (fZec100[i] + -1.0f);
            }
            /* Vectorizable loop 71 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec106[i] = (fZec105[i] + -1.0f);
            }
            /* Vectorizable loop 72 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec110[i] = std::min<float>(
                  524288.0f,
                  (fSlow22 +
                   ((40.0f * fRec59[i]) +
                    ((fRec42[i] * (fConst0 + (1000.0f * fRec59[i]))) *
                     std::fabs(((2.0f * (1.0f - std::fabs(((2.0f * fRec72[i]) +
                                                           -1.0f)))) +
                                -1.0f))))));
            }
            /* Vectorizable loop 73 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec12[i] = (((iRec20[i] + -1) % iConst10) == 0);
            }
            /* Vectorizable loop 74 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec26[i] = Macalla_faustpower2_f(fRec24[i]);
            }
            /* Vectorizable loop 75 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec27[i] =
                  ((fConst39 * fRec25[i]) + (fConst40 * fRec25[(i - 2)]));
            }
            /* Vectorizable loop 76 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec30[i] = (((fZec29[i] + -1.41421354f) / fZec28[i]) + 1.0f);
            }
            /* Vectorizable loop 77 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec32[i] = (1.0f - (1.0f / fZec31[i]));
            }
            /* Vectorizable loop 78 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec33[i] = (((fZec29[i] + 1.41421354f) / fZec28[i]) + 1.0f);
            }
            /* Vectorizable loop 79 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec37[i] = (((fZec36[i] + -1.41421354f) / fZec35[i]) + 1.0f);
            }
            /* Vectorizable loop 80 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec34[i] = (0.0f - (2.0f / fZec31[i]));
            }
            /* Vectorizable loop 81 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec38[i] = (1.0f - (1.0f / Macalla_faustpower2_f(fZec35[i])));
            }
            /* Vectorizable loop 82 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec39[i] = (((fZec36[i] + 1.41421354f) / fZec35[i]) + 1.0f);
            }
            /* Recursive loop 83 */
            /* Pre code */
            for (int j66 = 0; (j66 < 4); j66 = (j66 + 1))
            {
                fRec35_tmp[j66] = fRec35_perm[j66];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec35[i] = (fSlow8 + (fConst2 * fRec35[(i - 1)]));
            }
            /* Post code */
            for (int j67 = 0; (j67 < 4); j67 = (j67 + 1))
            {
                fRec35_perm[j67] = fRec35_tmp[(vsize + j67)];
            }
            /* Vectorizable loop 84 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec43[i] = int(fZec42[i]);
            }
            /* Vectorizable loop 85 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec44[i] = std::floor(fZec42[i]);
            }
            /* Vectorizable loop 86 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec48[i] = int(fZec47[i]);
            }
            /* Vectorizable loop 87 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec49[i] = std::floor(fZec47[i]);
            }
            /* Recursive loop 88 */
            /* Pre code */
            for (int j80 = 0; (j80 < 4); j80 = (j80 + 1))
            {
                fRec38_tmp[j80] = fRec38_perm[j80];
            }
            for (int j82 = 0; (j82 < 4); j82 = (j82 + 1))
            {
                fRec39_tmp[j82] = fRec39_perm[j82];
            }
            for (int j84 = 0; (j84 < 4); j84 = (j84 + 1))
            {
                fRec40_tmp[j84] = fRec40_perm[j84];
            }
            for (int j86 = 0; (j86 < 4); j86 = (j86 + 1))
            {
                fRec41_tmp[j86] = fRec41_perm[j86];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen15 =
                  (((fRec39[(i - 1)] == 1.0f) & (fZec52[i] != fRec41[(i - 1)]))
                     ? fConst16
                     : 0.0f);
                float fElse16 = fRec38[(i - 1)];
                float fThen17 =
                  (((fRec39[(i - 1)] == 0.0f) & (fZec52[i] != fRec40[(i - 1)]))
                     ? fConst15
                     : fThen15);
                float fElse17 =
                  (((fRec39[(i - 1)] > 0.0f) & (fRec39[(i - 1)] < 1.0f))
                     ? fElse16
                     : 0.0f);
                fZec53[i] = ((fRec38[(i - 1)] != 0.0f) ? fElse17 : fThen17);
                fRec38[i] = fZec53[i];
                fRec39[i] = std::max<float>(
                  0.0f, std::min<float>(1.0f, (fRec39[(i - 1)] + fZec53[i])));
                float fThen18 = fRec40[(i - 1)];
                float fElse18 = fZec52[i];
                fRec40[i] =
                  (((fRec39[(i - 1)] >= 1.0f) & (fRec41[(i - 1)] != fZec52[i]))
                     ? fElse18
                     : fThen18);
                float fThen19 = fRec41[(i - 1)];
                float fElse19 = fZec52[i];
                fRec41[i] =
                  (((fRec39[(i - 1)] <= 0.0f) & (fRec40[(i - 1)] != fZec52[i]))
                     ? fElse19
                     : fThen19);
            }
            /* Post code */
            for (int j81 = 0; (j81 < 4); j81 = (j81 + 1))
            {
                fRec38_perm[j81] = fRec38_tmp[(vsize + j81)];
            }
            for (int j83 = 0; (j83 < 4); j83 = (j83 + 1))
            {
                fRec39_perm[j83] = fRec39_tmp[(vsize + j83)];
            }
            for (int j85 = 0; (j85 < 4); j85 = (j85 + 1))
            {
                fRec40_perm[j85] = fRec40_tmp[(vsize + j85)];
            }
            for (int j87 = 0; (j87 < 4); j87 = (j87 + 1))
            {
                fRec41_perm[j87] = fRec41_tmp[(vsize + j87)];
            }
            /* Vectorizable loop 89 */
            /* Pre code */
            fYec6_idx = ((fYec6_idx + fYec6_idx_save) & 131071);
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fYec6[((i + fYec6_idx) & 131071)] =
                  (float(input0[i]) * fZec54[i]);
            }
            /* Post code */
            fYec6_idx_save = vsize;
            /* Vectorizable loop 90 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec57[i] = std::min<int>(65537, std::max<int>(0, iZec56[i]));
            }
            /* Vectorizable loop 91 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec59[i] = (fZec58[i] + (-4095.0f - fRec2[i]));
            }
            /* Vectorizable loop 92 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec60[i] = (fRec2[i] + (4096.0f - fZec58[i]));
            }
            /* Vectorizable loop 93 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec61[i] =
                  std::min<int>(65537, std::max<int>(0, (iZec56[i] + 1)));
            }
            /* Vectorizable loop 94 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec63[i] = (1.0f - fZec62[i]);
            }
            /* Vectorizable loop 95 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec65[i] = std::min<int>(65537, std::max<int>(0, iZec64[i]));
            }
            /* Vectorizable loop 96 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec67[i] = (fZec66[i] + (1.0f - fRec2[i]));
            }
            /* Vectorizable loop 97 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec68[i] = (fRec2[i] - fZec66[i]);
            }
            /* Vectorizable loop 98 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec69[i] =
                  std::min<int>(65537, std::max<int>(0, (iZec64[i] + 1)));
            }
            /* Recursive loop 99 */
            /* Pre code */
            for (int j4 = 0; (j4 < 4); j4 = (j4 + 1))
            {
                fRec3_tmp[j4] = fRec3_perm[j4];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec3[i] = (fSlow2 + (fConst2 * fRec3[(i - 1)]));
            }
            /* Post code */
            for (int j5 = 0; (j5 < 4); j5 = (j5 + 1))
            {
                fRec3_perm[j5] = fRec3_tmp[(vsize + j5)];
            }
            /* Vectorizable loop 100 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec10[i] = int(fZec9[i]);
            }
            /* Vectorizable loop 101 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec11[i] = std::floor(fZec9[i]);
            }
            /* Vectorizable loop 102 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec90[i] = int(fZec89[i]);
            }
            /* Vectorizable loop 103 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec91[i] = std::floor(fZec89[i]);
            }
            /* Vectorizable loop 104 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec102[i] = int(fZec101[i]);
            }
            /* Vectorizable loop 105 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec103[i] = std::floor(fZec101[i]);
            }
            /* Vectorizable loop 106 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec107[i] = int(fZec106[i]);
            }
            /* Vectorizable loop 107 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec108[i] = std::floor(fZec106[i]);
            }
            /* Recursive loop 108 */
            /* Pre code */
            for (int j132 = 0; (j132 < 4); j132 = (j132 + 1))
            {
                fRec68_tmp[j132] = fRec68_perm[j132];
            }
            for (int j134 = 0; (j134 < 4); j134 = (j134 + 1))
            {
                fRec69_tmp[j134] = fRec69_perm[j134];
            }
            for (int j136 = 0; (j136 < 4); j136 = (j136 + 1))
            {
                fRec70_tmp[j136] = fRec70_perm[j136];
            }
            for (int j138 = 0; (j138 < 4); j138 = (j138 + 1))
            {
                fRec71_tmp[j138] = fRec71_perm[j138];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen39 =
                  (((fRec69[(i - 1)] == 1.0f) & (fZec110[i] != fRec71[(i - 1)]))
                     ? fConst16
                     : 0.0f);
                float fElse40 = fRec68[(i - 1)];
                float fThen41 =
                  (((fRec69[(i - 1)] == 0.0f) & (fZec110[i] != fRec70[(i - 1)]))
                     ? fConst15
                     : fThen39);
                float fElse41 =
                  (((fRec69[(i - 1)] > 0.0f) & (fRec69[(i - 1)] < 1.0f))
                     ? fElse40
                     : 0.0f);
                fZec111[i] = ((fRec68[(i - 1)] != 0.0f) ? fElse41 : fThen41);
                fRec68[i]  = fZec111[i];
                fRec69[i]  = std::max<float>(
                  0.0f, std::min<float>(1.0f, (fRec69[(i - 1)] + fZec111[i])));
                float fThen42 = fRec70[(i - 1)];
                float fElse42 = fZec110[i];
                fRec70[i] =
                  (((fRec69[(i - 1)] >= 1.0f) & (fRec71[(i - 1)] != fZec110[i]))
                     ? fElse42
                     : fThen42);
                float fThen43 = fRec71[(i - 1)];
                float fElse43 = fZec110[i];
                fRec71[i] =
                  (((fRec69[(i - 1)] <= 0.0f) & (fRec70[(i - 1)] != fZec110[i]))
                     ? fElse43
                     : fThen43);
            }
            /* Post code */
            for (int j133 = 0; (j133 < 4); j133 = (j133 + 1))
            {
                fRec68_perm[j133] = fRec68_tmp[(vsize + j133)];
            }
            for (int j135 = 0; (j135 < 4); j135 = (j135 + 1))
            {
                fRec69_perm[j135] = fRec69_tmp[(vsize + j135)];
            }
            for (int j137 = 0; (j137 < 4); j137 = (j137 + 1))
            {
                fRec70_perm[j137] = fRec70_tmp[(vsize + j137)];
            }
            for (int j139 = 0; (j139 < 4); j139 = (j139 + 1))
            {
                fRec71_perm[j139] = fRec71_tmp[(vsize + j139)];
            }
            /* Vectorizable loop 109 */
            /* Pre code */
            fYec10_idx = ((fYec10_idx + fYec10_idx_save) & 131071);
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fYec10[((i + fYec10_idx) & 131071)] =
                  (float(input1[i]) * fZec54[i]);
            }
            /* Post code */
            fYec10_idx_save = vsize;
            /* Recursive loop 110 */
            /* Pre code */
            for (int j24 = 0; (j24 < 4); j24 = (j24 + 1))
            {
                fRec10_tmp[j24] = fRec10_perm[j24];
            }
            for (int j28 = 0; (j28 < 4); j28 = (j28 + 1))
            {
                fRec19_tmp[j28] = fRec19_perm[j28];
            }
            for (int j30 = 0; (j30 < 4); j30 = (j30 + 1))
            {
                fRec22_tmp[j30] = fRec22_perm[j30];
            }
            for (int j32 = 0; (j32 < 4); j32 = (j32 + 1))
            {
                fRec21_tmp[j32] = fRec21_perm[j32];
            }
            for (int j34 = 0; (j34 < 4); j34 = (j34 + 1))
            {
                fRec23_tmp[j34] = fRec23_perm[j34];
            }
            for (int j60 = 0; (j60 < 4); j60 = (j60 + 1))
            {
                fRec9_tmp[j60] = fRec9_perm[j60];
            }
            for (int j64 = 0; (j64 < 4); j64 = (j64 + 1))
            {
                fRec8_tmp[j64] = fRec8_perm[j64];
            }
            fYec4_idx = ((fYec4_idx + fYec4_idx_save) & 2047);
            for (int j70 = 0; (j70 < 4); j70 = (j70 + 1))
            {
                fRec6_tmp[j70] = fRec6_perm[j70];
            }
            fYec5_idx = ((fYec5_idx + fYec5_idx_save) & 4095);
            for (int j74 = 0; (j74 < 4); j74 = (j74 + 1))
            {
                fRec4_tmp[j74] = fRec4_perm[j74];
            }
            fYec7_idx = ((fYec7_idx + fYec7_idx_save) & 1048575);
            fRec0_idx = ((fRec0_idx + fRec0_idx_save) & 262143);
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec10[i] =
                  (((fRec0[(((i + fRec0_idx) -
                             (int(std::min<float>(
                                fConst0, float(std::max<int>(0, iZec10[i])))) +
                              1)) &
                            262143)] *
                     (0.0f - (fZec8[i] + (99.0f - fZec11[i])))) +
                    ((fZec8[i] + (100.0f - fZec11[i])) *
                     fRec0[(
                       ((i + fRec0_idx) -
                        (int(std::min<float>(
                           fConst0, float(std::max<int>(0, (iZec10[i] + 1))))) +
                         1)) &
                       262143)])) -
                   (fConst7 * ((fConst8 * fRec10[(i - 2)]) +
                               (fConst9 * fRec10[(i - 1)]))));
                fZec13[i] =
                  (fConst7 *
                   (fRec10[(i - 2)] + (fRec10[i] + (2.0f * fRec10[(i - 1)]))));
                float fThen3 = fRec19[(i - 1)];
                float fElse3 =
                  (0.00392156886f *
                   (std::floor(((255.0f * std::fabs(fZec13[i])) + 0.5f)) *
                    (float((2 * (fZec13[i] > 0.0f))) + -1.0f)));
                fRec19[i]    = (iZec12[i] ? fElse3 : fThen3);
                float fThen4 = fZec13[i];
                float fElse4 = fRec19[i];
                fZec14[i]    = (iSlow4 ? fElse4 : fThen4);
                fZec15[i]    = std::fabs(fZec14[i]);
                fZec16[i] =
                  ((fZec15[i] > fRec22[(i - 1)]) ? fConst12 : fConst11);
                fRec22[i] = ((fZec15[i] * (1.0f - fZec16[i])) +
                             (fRec22[(i - 1)] * fZec16[i]));
                fRec21[i] =
                  ((fConst14 *
                    (0.0f -
                     (0.75f * std::max<float>(
                                ((20.0f * std::log10(std::max<float>(
                                            1.17549435e-38f, fRec22[i]))) +
                                 6.0f),
                                0.0f)))) +
                   (fConst13 * fRec21[(i - 1)]));
                fZec17[i] =
                  (fZec14[i] * std::pow(10.0f, (0.0500000007f * fRec21[i])));
                fZec18[i] = std::fabs((fZec17[i] + 9.99999975e-05f));
                fZec19[i] =
                  ((fZec18[i] > fRec23[(i - 1)]) ? fConst17 : fConst11);
                fRec23[i] = ((fZec18[i] * (1.0f - fZec19[i])) +
                             (fRec23[(i - 1)] * fZec19[i]));
                fRec9[i] =
                  ((fZec17[i] + ((fRec23[i] * fZec26[i]) * fZec27[i])) -
                   (((fRec9[(i - 2)] * fZec30[i]) +
                     (2.0f * (fRec9[(i - 1)] * fZec32[i]))) /
                    fZec33[i]));
                fRec8[i] =
                  (((((fRec9[(i - 1)] * fZec34[i]) + (fRec9[i] / fZec31[i])) +
                     (fRec9[(i - 2)] / fZec31[i])) /
                    fZec33[i]) -
                   (((fRec8[(i - 2)] * fZec37[i]) +
                     (2.0f * (fRec8[(i - 1)] * fZec38[i]))) /
                    fZec39[i]));
                fYec4[((i + fYec4_idx) & 2047)] =
                  ((0.980000019f *
                    ((fRec8[(i - 2)] + (fRec8[i] + (2.0f * fRec8[(i - 1)]))) /
                     fZec39[i])) -
                   (fRec35[i] * fRec6[(i - 1)]));
                fRec6[i] =
                  ((fYec4[(
                      ((i + fYec4_idx) -
                       std::min<int>(iConst45, std::max<int>(0, iZec43[i]))) &
                      2047)] *
                    (fZec44[i] + (2.0f - fZec41[i]))) +
                   ((fZec41[i] + (-1.0f - fZec44[i])) *
                    fYec4[(((i + fYec4_idx) -
                            std::min<int>(iConst45,
                                          std::max<int>(0, (iZec43[i] + 1)))) &
                           2047)]));
                fRec7[i] = (fRec35[i] * fYec4[((i + fYec4_idx) & 2047)]);
                fYec5[((i + fYec5_idx) & 4095)] =
                  ((fRec7[i] + fRec6[(i - 1)]) -
                   (0.899999976f * (fRec35[i] * fRec4[(i - 1)])));
                fRec4[i] =
                  ((fYec5[(
                      ((i + fYec5_idx) -
                       std::min<int>(iConst45, std::max<int>(0, iZec48[i]))) &
                      4095)] *
                    (fZec49[i] + (2.0f - fZec46[i]))) +
                   ((fZec46[i] + (-1.0f - fZec49[i])) *
                    fYec5[(((i + fYec5_idx) -
                            std::min<int>(iConst45,
                                          std::max<int>(0, (iZec48[i] + 1)))) &
                           4095)]));
                fRec5[i] = (0.899999976f *
                            (fRec35[i] * fYec5[((i + fYec5_idx) & 4095)]));
                fYec7[((i + fYec7_idx) & 1048575)] =
                  ((((fYec6[(((i + fYec6_idx) - iZec57[i]) & 131071)] *
                      fZec59[i]) +
                     (fZec60[i] *
                      fYec6[(((i + fYec6_idx) - iZec61[i]) & 131071)])) *
                    fZec63[i]) +
                   ((fRec3[i] * (fRec5[i] + fRec4[(i - 1)])) +
                    (((fYec6[(((i + fYec6_idx) - iZec65[i]) & 131071)] *
                       fZec67[i]) +
                      (fZec68[i] *
                       fYec6[(((i + fYec6_idx) - iZec69[i]) & 131071)])) *
                     fZec62[i])));
                fZec70[i] =
                  fYec7[(((i + fYec7_idx) -
                          int(std::min<float>(
                            524288.0f, std::max<float>(0.0f, fRec40[i])))) &
                         1048575)];
                fRec0[((i + fRec0_idx) & 262143)] =
                  (fZec70[i] +
                   (fRec39[i] *
                    (fYec7[(((i + fYec7_idx) -
                             int(std::min<float>(
                               524288.0f, std::max<float>(0.0f, fRec41[i])))) &
                            1048575)] -
                     fZec70[i])));
            }
            /* Post code */
            fYec7_idx_save = vsize;
            fYec5_idx_save = vsize;
            fYec4_idx_save = vsize;
            for (int j35 = 0; (j35 < 4); j35 = (j35 + 1))
            {
                fRec23_perm[j35] = fRec23_tmp[(vsize + j35)];
            }
            for (int j31 = 0; (j31 < 4); j31 = (j31 + 1))
            {
                fRec22_perm[j31] = fRec22_tmp[(vsize + j31)];
            }
            for (int j33 = 0; (j33 < 4); j33 = (j33 + 1))
            {
                fRec21_perm[j33] = fRec21_tmp[(vsize + j33)];
            }
            for (int j29 = 0; (j29 < 4); j29 = (j29 + 1))
            {
                fRec19_perm[j29] = fRec19_tmp[(vsize + j29)];
            }
            for (int j25 = 0; (j25 < 4); j25 = (j25 + 1))
            {
                fRec10_perm[j25] = fRec10_tmp[(vsize + j25)];
            }
            for (int j61 = 0; (j61 < 4); j61 = (j61 + 1))
            {
                fRec9_perm[j61] = fRec9_tmp[(vsize + j61)];
            }
            for (int j65 = 0; (j65 < 4); j65 = (j65 + 1))
            {
                fRec8_perm[j65] = fRec8_tmp[(vsize + j65)];
            }
            for (int j71 = 0; (j71 < 4); j71 = (j71 + 1))
            {
                fRec6_perm[j71] = fRec6_tmp[(vsize + j71)];
            }
            for (int j75 = 0; (j75 < 4); j75 = (j75 + 1))
            {
                fRec4_perm[j75] = fRec4_tmp[(vsize + j75)];
            }
            fRec0_idx_save = vsize;
            /* Recursive loop 111 */
            /* Pre code */
            for (int j108 = 0; (j108 < 4); j108 = (j108 + 1))
            {
                fRec56_tmp[j108] = fRec56_perm[j108];
            }
            for (int j110 = 0; (j110 < 4); j110 = (j110 + 1))
            {
                fRec62_tmp[j110] = fRec62_perm[j110];
            }
            for (int j112 = 0; (j112 < 4); j112 = (j112 + 1))
            {
                fRec64_tmp[j112] = fRec64_perm[j112];
            }
            for (int j114 = 0; (j114 < 4); j114 = (j114 + 1))
            {
                fRec63_tmp[j114] = fRec63_perm[j114];
            }
            for (int j116 = 0; (j116 < 4); j116 = (j116 + 1))
            {
                fRec65_tmp[j116] = fRec65_perm[j116];
            }
            for (int j118 = 0; (j118 < 4); j118 = (j118 + 1))
            {
                fRec55_tmp[j118] = fRec55_perm[j118];
            }
            for (int j120 = 0; (j120 < 4); j120 = (j120 + 1))
            {
                fRec54_tmp[j120] = fRec54_perm[j120];
            }
            fYec8_idx = ((fYec8_idx + fYec8_idx_save) & 16383);
            for (int j124 = 0; (j124 < 4); j124 = (j124 + 1))
            {
                fRec52_tmp[j124] = fRec52_perm[j124];
            }
            fYec9_idx = ((fYec9_idx + fYec9_idx_save) & 16383);
            for (int j128 = 0; (j128 < 4); j128 = (j128 + 1))
            {
                fRec50_tmp[j128] = fRec50_perm[j128];
            }
            fYec11_idx = ((fYec11_idx + fYec11_idx_save) & 1048575);
            fRec49_idx = ((fRec49_idx + fRec49_idx_save) & 262143);
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec56[i] =
                  (((fRec49[(((i + fRec49_idx) -
                              (int(std::min<float>(
                                 fConst0, float(std::max<int>(0, iZec90[i])))) +
                               1)) &
                             262143)] *
                     (0.0f - (fZec88[i] + (99.0f - fZec91[i])))) +
                    ((fZec88[i] + (100.0f - fZec91[i])) *
                     fRec49[(
                       ((i + fRec49_idx) -
                        (int(std::min<float>(
                           fConst0, float(std::max<int>(0, (iZec90[i] + 1))))) +
                         1)) &
                       262143)])) -
                   (fConst7 * ((fConst8 * fRec56[(i - 2)]) +
                               (fConst9 * fRec56[(i - 1)]))));
                fZec92[i] =
                  (fConst7 *
                   (fRec56[(i - 2)] + (fRec56[i] + (2.0f * fRec56[(i - 1)]))));
                float fThen31 = fRec62[(i - 1)];
                float fElse31 =
                  (0.00392156886f *
                   (std::floor(((255.0f * std::fabs(fZec92[i])) + 0.5f)) *
                    (float((2 * (fZec92[i] > 0.0f))) + -1.0f)));
                fRec62[i]     = (iZec12[i] ? fElse31 : fThen31);
                float fThen32 = fZec92[i];
                float fElse32 = fRec62[i];
                fZec93[i]     = (iSlow4 ? fElse32 : fThen32);
                fZec94[i]     = std::fabs(fZec93[i]);
                fZec95[i] =
                  ((fZec94[i] > fRec64[(i - 1)]) ? fConst12 : fConst11);
                fRec64[i] = ((fZec94[i] * (1.0f - fZec95[i])) +
                             (fRec64[(i - 1)] * fZec95[i]));
                fRec63[i] =
                  ((fConst14 *
                    (0.0f -
                     (0.75f * std::max<float>(
                                ((20.0f * std::log10(std::max<float>(
                                            1.17549435e-38f, fRec64[i]))) +
                                 6.0f),
                                0.0f)))) +
                   (fConst13 * fRec63[(i - 1)]));
                fZec96[i] =
                  (fZec93[i] * std::pow(10.0f, (0.0500000007f * fRec63[i])));
                fZec97[i] = std::fabs((fZec96[i] + 9.99999975e-05f));
                fZec98[i] =
                  ((fZec97[i] > fRec65[(i - 1)]) ? fConst17 : fConst11);
                fRec65[i] = ((fZec97[i] * (1.0f - fZec98[i])) +
                             (fRec65[(i - 1)] * fZec98[i]));
                fRec55[i] =
                  ((fZec96[i] + ((fZec26[i] * fRec65[i]) * fZec27[i])) -
                   (((fZec30[i] * fRec55[(i - 2)]) +
                     (2.0f * (fZec32[i] * fRec55[(i - 1)]))) /
                    fZec33[i]));
                fRec54[i] =
                  (((((fZec34[i] * fRec55[(i - 1)]) + (fRec55[i] / fZec31[i])) +
                     (fRec55[(i - 2)] / fZec31[i])) /
                    fZec33[i]) -
                   (((fZec37[i] * fRec54[(i - 2)]) +
                     (2.0f * (fZec38[i] * fRec54[(i - 1)]))) /
                    fZec39[i]));
                fYec8[((i + fYec8_idx) & 16383)] =
                  ((0.980000019f * ((fRec54[(i - 2)] +
                                     (fRec54[i] + (2.0f * fRec54[(i - 1)]))) /
                                    fZec39[i])) -
                   (fRec35[i] * fRec52[(i - 1)]));
                fRec52[i] =
                  ((fYec8[(
                      ((i + fYec8_idx) -
                       std::min<int>(iConst45, std::max<int>(0, iZec102[i]))) &
                      16383)] *
                    (fZec103[i] + (2.0f - fZec100[i]))) +
                   ((fZec100[i] + (-1.0f - fZec103[i])) *
                    fYec8[(((i + fYec8_idx) -
                            std::min<int>(iConst45,
                                          std::max<int>(0, (iZec102[i] + 1)))) &
                           16383)]));
                fRec53[i] = (fRec35[i] * fYec8[((i + fYec8_idx) & 16383)]);
                fYec9[((i + fYec9_idx) & 16383)] =
                  ((fRec53[i] + fRec52[(i - 1)]) -
                   (0.899999976f * (fRec35[i] * fRec50[(i - 1)])));
                fRec50[i] =
                  ((fYec9[(
                      ((i + fYec9_idx) -
                       std::min<int>(iConst45, std::max<int>(0, iZec107[i]))) &
                      16383)] *
                    (fZec108[i] + (2.0f - fZec105[i]))) +
                   ((fZec105[i] + (-1.0f - fZec108[i])) *
                    fYec9[(((i + fYec9_idx) -
                            std::min<int>(iConst45,
                                          std::max<int>(0, (iZec107[i] + 1)))) &
                           16383)]));
                fRec51[i] = (0.899999976f *
                             (fRec35[i] * fYec9[((i + fYec9_idx) & 16383)]));
                fYec11[((i + fYec11_idx) & 1048575)] =
                  ((fZec63[i] *
                    ((fZec59[i] *
                      fYec10[(((i + fYec10_idx) - iZec57[i]) & 131071)]) +
                     (fZec60[i] *
                      fYec10[(((i + fYec10_idx) - iZec61[i]) & 131071)]))) +
                   ((fRec3[i] * (fRec51[i] + fRec50[(i - 1)])) +
                    (fZec62[i] *
                     ((fZec67[i] *
                       fYec10[(((i + fYec10_idx) - iZec65[i]) & 131071)]) +
                      (fZec68[i] *
                       fYec10[(((i + fYec10_idx) - iZec69[i]) & 131071)])))));
                fZec112[i] =
                  fYec11[(((i + fYec11_idx) -
                           int(std::min<float>(
                             524288.0f, std::max<float>(0.0f, fRec70[i])))) &
                          1048575)];
                fRec49[((i + fRec49_idx) & 262143)] =
                  (fZec112[i] +
                   (fRec69[i] *
                    (fYec11[(((i + fYec11_idx) -
                              int(std::min<float>(
                                524288.0f, std::max<float>(0.0f, fRec71[i])))) &
                             1048575)] -
                     fZec112[i])));
            }
            /* Post code */
            fYec11_idx_save = vsize;
            fYec9_idx_save  = vsize;
            fYec8_idx_save  = vsize;
            for (int j117 = 0; (j117 < 4); j117 = (j117 + 1))
            {
                fRec65_perm[j117] = fRec65_tmp[(vsize + j117)];
            }
            for (int j113 = 0; (j113 < 4); j113 = (j113 + 1))
            {
                fRec64_perm[j113] = fRec64_tmp[(vsize + j113)];
            }
            for (int j115 = 0; (j115 < 4); j115 = (j115 + 1))
            {
                fRec63_perm[j115] = fRec63_tmp[(vsize + j115)];
            }
            for (int j111 = 0; (j111 < 4); j111 = (j111 + 1))
            {
                fRec62_perm[j111] = fRec62_tmp[(vsize + j111)];
            }
            for (int j109 = 0; (j109 < 4); j109 = (j109 + 1))
            {
                fRec56_perm[j109] = fRec56_tmp[(vsize + j109)];
            }
            for (int j119 = 0; (j119 < 4); j119 = (j119 + 1))
            {
                fRec55_perm[j119] = fRec55_tmp[(vsize + j119)];
            }
            for (int j121 = 0; (j121 < 4); j121 = (j121 + 1))
            {
                fRec54_perm[j121] = fRec54_tmp[(vsize + j121)];
            }
            for (int j125 = 0; (j125 < 4); j125 = (j125 + 1))
            {
                fRec52_perm[j125] = fRec52_tmp[(vsize + j125)];
            }
            for (int j129 = 0; (j129 < 4); j129 = (j129 + 1))
            {
                fRec50_perm[j129] = fRec50_tmp[(vsize + j129)];
            }
            fRec49_idx_save = vsize;
            /* Vectorizable loop 112 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec71[i] = std::fabs(
                  (fRec0[((i + fRec0_idx) & 262143)] + 9.99999975e-05f));
            }
            /* Vectorizable loop 113 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec113[i] = std::fabs(
                  (fRec49[((i + fRec49_idx) & 262143)] + 9.99999975e-05f));
            }
            /* Recursive loop 114 */
            /* Pre code */
            for (int j88 = 0; (j88 < 4); j88 = (j88 + 1))
            {
                fRec44_tmp[j88] = fRec44_perm[j88];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec72[i] =
                  ((fZec71[i] > fRec44[(i - 1)]) ? fConst17 : fConst11);
                fRec44[i] = ((fZec71[i] * (1.0f - fZec72[i])) +
                             (fRec44[(i - 1)] * fZec72[i]));
            }
            /* Post code */
            for (int j89 = 0; (j89 < 4); j89 = (j89 + 1))
            {
                fRec44_perm[j89] = fRec44_tmp[(vsize + j89)];
            }
            /* Recursive loop 115 */
            /* Pre code */
            for (int j140 = 0; (j140 < 4); j140 = (j140 + 1))
            {
                fRec73_tmp[j140] = fRec73_perm[j140];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec114[i] =
                  ((fZec113[i] > fRec73[(i - 1)]) ? fConst17 : fConst11);
                fRec73[i] = ((fZec113[i] * (1.0f - fZec114[i])) +
                             (fRec73[(i - 1)] * fZec114[i]));
            }
            /* Post code */
            for (int j141 = 0; (j141 < 4); j141 = (j141 + 1))
            {
                fRec73_perm[j141] = fRec73_tmp[(vsize + j141)];
            }
            /* Vectorizable loop 116 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec73[i] = (fRec0[((i + fRec0_idx) & 262143)] +
                             ((fZec26[i] * fRec44[i]) * fZec27[i]));
            }
            /* Vectorizable loop 117 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec76[i] = std::fabs(float(input0[i]));
            }
            /* Recursive loop 118 */
            /* Pre code */
            for (int j96 = 0; (j96 < 4); j96 = (j96 + 1))
            {
                fRec48_tmp[j96] = fRec48_perm[j96];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec48[i] = (fSlow18 + (fConst2 * fRec48[(i - 1)]));
            }
            /* Post code */
            for (int j97 = 0; (j97 < 4); j97 = (j97 + 1))
            {
                fRec48_perm[j97] = fRec48_tmp[(vsize + j97)];
            }
            /* Vectorizable loop 119 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec115[i] = (fRec49[((i + fRec49_idx) & 262143)] +
                              ((fZec26[i] * fRec73[i]) * fZec27[i]));
            }
            /* Vectorizable loop 120 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec118[i] = std::fabs(float(input1[i]));
            }
            /* Vectorizable loop 121 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec74[i] = std::fabs(fZec73[i]);
            }
            /* Recursive loop 122 */
            /* Pre code */
            for (int j94 = 0; (j94 < 4); j94 = (j94 + 1))
            {
                fRec47_tmp[j94] = fRec47_perm[j94];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec77[i] = ((fZec76[i] > fRec47[(i - 1)]) ? fSlow17 : fSlow14);
                fRec47[i] = ((fZec76[i] * (1.0f - fZec77[i])) +
                             (fRec47[(i - 1)] * fZec77[i]));
            }
            /* Post code */
            for (int j95 = 0; (j95 < 4); j95 = (j95 + 1))
            {
                fRec47_perm[j95] = fRec47_tmp[(vsize + j95)];
            }
            /* Vectorizable loop 123 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec79[i] = (60.0f * fRec48[i]);
            }
            /* Vectorizable loop 124 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec116[i] = std::fabs(fZec115[i]);
            }
            /* Recursive loop 125 */
            /* Pre code */
            for (int j146 = 0; (j146 < 4); j146 = (j146 + 1))
            {
                fRec76_tmp[j146] = fRec76_perm[j146];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec119[i] =
                  ((fZec118[i] > fRec76[(i - 1)]) ? fSlow17 : fSlow14);
                fRec76[i] = ((fZec118[i] * (1.0f - fZec119[i])) +
                             (fRec76[(i - 1)] * fZec119[i]));
            }
            /* Post code */
            for (int j147 = 0; (j147 < 4); j147 = (j147 + 1))
            {
                fRec76_perm[j147] = fRec76_tmp[(vsize + j147)];
            }
            /* Recursive loop 126 */
            /* Pre code */
            for (int j90 = 0; (j90 < 4); j90 = (j90 + 1))
            {
                fRec46_tmp[j90] = fRec46_perm[j90];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec75[i] =
                  ((fZec74[i] > fRec46[(i - 1)]) ? fConst12 : fConst11);
                fRec46[i] = ((fZec74[i] * (1.0f - fZec75[i])) +
                             (fRec46[(i - 1)] * fZec75[i]));
            }
            /* Post code */
            for (int j91 = 0; (j91 < 4); j91 = (j91 + 1))
            {
                fRec46_perm[j91] = fRec46_tmp[(vsize + j91)];
            }
            /* Vectorizable loop 127 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec78[i] = (20.0f * std::log10(std::max<float>(1.17549435e-38f,
                                                                fRec47[i])));
            }
            /* Vectorizable loop 128 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec80[i] = (-1.5f - fZec79[i]);
            }
            /* Vectorizable loop 129 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec81[i] = (1.5f - fZec79[i]);
            }
            /* Recursive loop 130 */
            /* Pre code */
            for (int j142 = 0; (j142 < 4); j142 = (j142 + 1))
            {
                fRec75_tmp[j142] = fRec75_perm[j142];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec117[i] =
                  ((fZec116[i] > fRec75[(i - 1)]) ? fConst12 : fConst11);
                fRec75[i] = ((fZec116[i] * (1.0f - fZec117[i])) +
                             (fRec75[(i - 1)] * fZec117[i]));
            }
            /* Post code */
            for (int j143 = 0; (j143 < 4); j143 = (j143 + 1))
            {
                fRec75_perm[j143] = fRec75_tmp[(vsize + j143)];
            }
            /* Vectorizable loop 131 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec120[i] =
                  (20.0f *
                   std::log10(std::max<float>(1.17549435e-38f, fRec76[i])));
            }
            /* Recursive loop 132 */
            /* Pre code */
            for (int j92 = 0; (j92 < 4); j92 = (j92 + 1))
            {
                fRec45_tmp[j92] = fRec45_perm[j92];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec45[i] =
                  ((fConst14 *
                    (0.0f -
                     (0.75f * std::max<float>(
                                ((20.0f * std::log10(std::max<float>(
                                            1.17549435e-38f, fRec46[i]))) +
                                 6.0f),
                                0.0f)))) +
                   (fConst13 * fRec45[(i - 1)]));
            }
            /* Post code */
            for (int j93 = 0; (j93 < 4); j93 = (j93 + 1))
            {
                fRec45_perm[j93] = fRec45_tmp[(vsize + j93)];
            }
            /* Vectorizable loop 133 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec82[i] = ((fZec78[i] > fZec80[i]) + (fZec78[i] > fZec81[i]));
            }
            /* Vectorizable loop 134 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec83[i] = (fZec78[i] + fZec79[i]);
            }
            /* Vectorizable loop 135 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec84[i] = (1.0f - (1.0f / ((19.0f * fRec48[i]) + 1.0f)));
            }
            /* Recursive loop 136 */
            /* Pre code */
            for (int j144 = 0; (j144 < 4); j144 = (j144 + 1))
            {
                fRec74_tmp[j144] = fRec74_perm[j144];
            }
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fRec74[i] =
                  ((fConst14 *
                    (0.0f -
                     (0.75f * std::max<float>(
                                ((20.0f * std::log10(std::max<float>(
                                            1.17549435e-38f, fRec75[i]))) +
                                 6.0f),
                                0.0f)))) +
                   (fConst13 * fRec74[(i - 1)]));
            }
            /* Post code */
            for (int j145 = 0; (j145 < 4); j145 = (j145 + 1))
            {
                fRec74_perm[j145] = fRec74_tmp[(vsize + j145)];
            }
            /* Vectorizable loop 137 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                iZec121[i] =
                  ((fZec120[i] > fZec80[i]) + (fZec120[i] > fZec81[i]));
            }
            /* Vectorizable loop 138 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                fZec122[i] = (fZec79[i] + fZec120[i]);
            }
            /* Vectorizable loop 139 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen27 = fZec83[i];
                float fElse27 =
                  (0.166666672f * Macalla_faustpower2_f((fZec83[i] + 1.5f)));
                float fThen28 = ((iZec82[i] == 1) ? fElse27 : fThen27);
                output0[i]    = FAUSTFLOAT((
                  (fSlow20 * float(input0[i])) +
                  (fSlow19 *
                   ((fZec73[i] * std::pow(10.0f, (0.0500000007f * fRec45[i]))) *
                    std::pow(
                      10.0f,
                      (0.0500000007f *
                       (0.0f - (std::max<float>(
                                  0.0f, ((iZec82[i] == 0) ? 0.0f : fThen28)) *
                                fZec84[i]))))))));
            }
            /* Vectorizable loop 140 */
            /* Compute code */
            for (int i = 0; (i < vsize); i = (i + 1))
            {
                float fThen47 = fZec122[i];
                float fElse47 =
                  (0.166666672f * Macalla_faustpower2_f((fZec122[i] + 1.5f)));
                float fThen48 = ((iZec121[i] == 1) ? fElse47 : fThen47);
                output1[i]    = FAUSTFLOAT(
                  ((fSlow20 * float(input1[i])) +
                   (fSlow19 *
                    ((fZec115[i] *
                      std::pow(10.0f, (0.0500000007f * fRec74[i]))) *
                     std::pow(
                       10.0f,
                       (0.0500000007f *
                        (0.0f -
                         (fZec84[i] *
                          std::max<float>(
                            0.0f, ((iZec121[i] == 0) ? 0.0f : fThen48))))))))));
            }
        }
    }
};

// END-FAUSTDSP

#endif
